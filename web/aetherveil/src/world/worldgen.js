// Generates the land of Lumeria as a heightfield plus per-point colour, biome
// and grass density. Pure data (no three.js) so it can also run in Node for
// map previews. Physics, terrain meshes, grass, water and the world map all
// sample the arrays built here.

import { Simplex2 } from '../core/noise.js';
import { clamp, lerp, smoothstep } from '../core/util.js';
import { LOCATIONS, ROADS, CAUSEWAY } from './locations.js';

export const WORLD_SIZE = 2048;
export const HALF = WORLD_SIZE / 2;
export const CELL = 2;
export const N = WORLD_SIZE / CELL + 1; // 1025 grid points per side
export const WATER_LEVEL = 0;
export const SEED = 7331;

export const BIOME = {
  SEA: 0, BEACH: 1, MEADOW: 2, FOREST: 3, HIGHLAND: 4, ROCK: 5, SNOW: 6,
  DESERT: 7, ROAD: 8, PLAZA: 9, CRYSTAL: 10, MESA: 11,
};
export const REGION = { MEADOW: 0, FOREST: 1, MOUNTAIN: 2, DESERT: 3, LAKE: 4, COAST: 5 };
export const REGION_NAMES = ['The Amber Meadows', 'Whisperwood', 'The Frostspire Range', 'The Sunscar Badlands', 'Heartshard Lake', 'The Coast'];

const LAKE = { x: 0, z: -60, r0: 110, r1: 178 };

const hex = (h) => [(h >> 16) & 255, (h >> 8) & 255, h & 255];
const C = {
  meadowA: hex(0x72b252), meadowB: hex(0x98c46a), meadowC: hex(0x5a9c4a),
  forestA: hex(0x467f3c), forestB: hex(0x376a34),
  highland: hex(0x7c9c58), highland2: hex(0x93a565),
  rock: hex(0x8d877b), rockDark: hex(0x6c6760),
  snow: hex(0xf3f7fc), snowShade: hex(0xd8e4f2),
  sand: hex(0xecdcaa), wetSand: hex(0xc8b585), seabed: hex(0x9c946e),
  desert: hex(0xe0a66c), desertB: hex(0xd28e58),
  road: hex(0xb8956a), plaza: hex(0xaea38f),
  crystal: hex(0x6cc2a6), crystalB: hex(0x8ad2bc),
  dryGrass: hex(0xc9b765),
};

const mix3 = (a, b, t) => [lerp(a[0], b[0], t), lerp(a[1], b[1], t), lerp(a[2], b[2], t)];
const terrace = (h, step) => {
  const k = h / step;
  const f = k - Math.floor(k);
  return (Math.floor(k) + smoothstep(0.7, 1.0, f)) * step;
};

export class WorldData {
  constructor() {
    this.noise = new Simplex2(SEED);
    this.noise2 = new Simplex2(SEED + 99);
    this.H = new Float32Array(N * N);
    this.color = new Uint8Array(N * N * 3); // sRGB
    this.grass = new Uint8Array(N * N); // 0..255 density
    this.biome = new Uint8Array(N * N);
    this.region = new Uint8Array(N * N);
    this.road = new Uint8Array(N * N); // 0..255 road coverage
    this.strata = new Uint8Array(N * N); // desert cliff banding weight
    this.wMount = new Uint8Array(N * N);
    this.wDesert = new Uint8Array(N * N);
    this.wForest = new Uint8Array(N * N);
    this.locations = LOCATIONS;
  }

  // ---------- generation ----------
  async generate(progress = () => {}) {
    const H = this.H;
    const yieldEvery = 64;
    for (let iz = 0; iz < N; iz++) {
      const z = iz * CELL - HALF;
      for (let ix = 0; ix < N; ix++) {
        const x = ix * CELL - HALF;
        const i = iz * N + ix;
        const r = this.rawHeight(x, z);
        H[i] = r.h;
        this.region[i] = r.region;
        this.wMount[i] = r.mount * 255; this.wDesert[i] = r.desert * 255; this.wForest[i] = r.forest * 255;
      }
      if (iz % yieldEvery === 0) { progress(0.55 * iz / N, 'Raising the land'); await tick(); }
    }
    progress(0.56, 'Settling villages');
    await tick();
    this.flattenLocations();
    progress(0.6, 'Laying roads');
    await tick();
    this.buildRoads();
    progress(0.68, 'Painting the land');
    await tick();
    for (let iz = 0; iz < N; iz++) {
      for (let ix = 0; ix < N; ix++) this.shadePoint(ix, iz);
      if (iz % 128 === 0) { progress(0.68 + 0.3 * iz / N, 'Painting the land'); await tick(); }
    }
    for (const loc of LOCATIONS) loc.y = this.heightAt(loc.x, loc.z);
    progress(1, 'Done');
  }

  regionWeights(x, z) {
    const n = this.noise;
    const wx = x + 130 * n.fbm(x * 0.0016 + 11.3, z * 0.0016 - 7.1, 3);
    const wz = z + 130 * n.fbm(x * 0.0016 - 31.7, z * 0.0016 + 5.9, 3);
    const mount = smoothstep(-250, -560, wz);
    const desert = smoothstep(250, 520, wx) * (1 - mount * 0.85) * (1 - smoothstep(160, 400, wz));
    const forest = smoothstep(-200, -470, wx) * (1 - mount * 0.85);
    return { mount, desert, forest, wx, wz };
  }

  rawHeight(x, z) {
    const n = this.noise, n2 = this.noise2;
    const { mount, desert, forest } = this.regionWeights(x, z);

    // Rolling meadow base everywhere.
    let h = 9.5 + 11 * n.fbm(x * 0.0035, z * 0.0035, 4) + 2.2 * n2.fbm(x * 0.02, z * 0.02, 3);

    if (forest > 0.001) {
      h += forest * (9 + 15 * n2.fbm(x * 0.006 + 50, z * 0.006 - 20, 4));
    }
    if (mount > 0.001) {
      const qx = x + 90 * n2.noise(x * 0.004 + 3, z * 0.004 - 8);
      const qz = z + 90 * n2.noise(x * 0.004 - 13, z * 0.004 + 2);
      const r = n.ridged(qx * 0.0024 + 100, qz * 0.0024 + 100, 5);
      const big = 0.55 + 0.45 * n2.fbm(x * 0.0011, z * 0.0011, 2);
      h += mount * (38 + 300 * r * r * big + 6 * n2.fbm(x * 0.03, z * 0.03, 2));
    }
    if (desert > 0.001) {
      const m = n2.fbm(x * 0.0042 + 200, z * 0.0042 - 90, 4);
      const plateau = smoothstep(0.02, 0.2, m) * 34 + smoothstep(0.32, 0.45, m) * 22;
      h += desert * (3 + terrace(plateau + 3 * n.fbm(x * 0.01, z * 0.01, 2), 8.5) + 2 * n.fbm(x * 0.03, z * 0.03, 2));
    }

    // The island continent: fall away into the sea at the edges.
    const d = Math.sqrt(x * x + z * z) / HALF + 0.11 * n.fbm(x * 0.0021 + 7, z * 0.0021 + 3, 3);
    const land = 1 - smoothstep(0.76, 0.95, d);
    h = lerp(-26, h, land);

    // The Heartshard lake with an island in the middle.
    const ld = Math.hypot(x - LAKE.x, z - LAKE.z) + 14 * n.noise(x * 0.01, z * 0.01);
    const lakeW = 1 - smoothstep(LAKE.r0, LAKE.r1, ld);
    if (lakeW > 0) h = lerp(h, -10 + 2 * n2.noise(x * 0.02, z * 0.02), lakeW);
    const isl = 1 - smoothstep(26, 44, Math.hypot(x - LAKE.x, z - LAKE.z));
    if (isl > 0) h = lerp(h, 5.5, isl);

    // Causeway across the lake.
    const cd = segDist(x, z, CAUSEWAY.x0, CAUSEWAY.z0, CAUSEWAY.x1, CAUSEWAY.z1);
    if (cd.d < CAUSEWAY.width * 1.6) {
      const w = 1 - smoothstep(CAUSEWAY.width * 0.5, CAUSEWAY.width * 1.6, cd.d);
      h = Math.max(h, lerp(-4, CAUSEWAY.height, w));
    }

    let region = 0;
    if (lakeW > 0.3) region = 4;
    else if (mount > 0.5) region = 2;
    else if (desert > 0.5) region = 3;
    else if (forest > 0.5) region = 1;
    else if (land < 0.6) region = 5;
    return { h, region, mount, desert, forest };
  }

  flattenLocations() {
    const H = this.H;
    for (const loc of LOCATIONS) {
      if (!loc.flatten) continue;
      const r = loc.flatten;
      let target = loc.fh;
      if (target === undefined) {
        let sum = 0, cnt = 0;
        for (let a = 0; a < 16; a++) {
          const ang = (a / 16) * Math.PI * 2;
          for (const rr of [0, r * 0.25, r * 0.5]) {
            sum += this.heightAt(loc.x + Math.cos(ang) * rr, loc.z + Math.sin(ang) * rr); cnt++;
          }
        }
        target = Math.max(sum / cnt, 1.5);
      }
      loc.groundY = target;
      const outer = r * 1.7;
      this.forGridInRadius(loc.x, loc.z, outer, (i, d) => {
        const w = 1 - smoothstep(r * 0.75, outer, d);
        H[i] = lerp(H[i], target, w);
      });
    }
  }

  // Give each road a profile no steeper than ROAD_GRADE and fit the land to
  // it. Where a road meets a ridge this carves a pass with sloped walls
  // instead of the road climbing a cliff; small dips are filled. Returns
  // { ceil, pinH, pinW } for buildRoads to re-apply after smoothing.
  gradeRoads() {
    const ROAD_GRADE = 0.4, HW = 3.5, WALL = 1.5;
    const ceil = new Float32Array(N * N).fill(Infinity);
    const pinH = new Float32Array(N * N), pinW = new Float32Array(N * N);
    for (const road of ROADS) {
      const sm = smoothPolyline(road, 6);
      const xs = [], zs = [];
      for (let k = 0; k < sm.length - 1; k++) {
        const [ax, az] = sm[k], [bx, bz] = sm[k + 1];
        const n = Math.max(1, Math.ceil(Math.hypot(bx - ax, bz - az) / 2));
        for (let j = 0; j < n; j++) { xs.push(ax + (bx - ax) * j / n); zs.push(az + (bz - az) * j / n); }
      }
      xs.push(sm[sm.length - 1][0]); zs.push(sm[sm.length - 1][1]);
      const n = xs.length;
      const raw = new Float32Array(n), ds = new Float32Array(n);
      for (let i = 0; i < n; i++) {
        raw[i] = Math.max(this.heightAt(xs[i], zs[i]), WATER_LEVEL + 0.7);
        if (i) ds[i] = Math.hypot(xs[i] - xs[i - 1], zs[i] - zs[i - 1]);
      }
      // Largest grade-limited profile that stays at or under the ground.
      const p = Float32Array.from(raw);
      for (let i = 1; i < n; i++) p[i] = Math.min(p[i], p[i - 1] + ROAD_GRADE * ds[i]);
      for (let i = n - 2; i >= 0; i--) p[i] = Math.min(p[i], p[i + 1] + ROAD_GRADE * ds[i + 1]);
      for (let i = 0; i < n - 1; i++) {
        const depth = Math.max(raw[i] - p[i], raw[i + 1] - p[i + 1]);
        // Deep cuts need wide walls; elsewhere just hold the road bed down.
        const reach = depth > 0.5 ? Math.min(90, HW + (depth + 70) / WALL) : HW + 6;
        const x0 = xs[i], z0 = zs[i], x1 = xs[i + 1], z1 = zs[i + 1];
        const ix0 = clamp(Math.floor((Math.min(x0, x1) - reach + HALF) / CELL), 0, N - 1), ix1 = clamp(Math.ceil((Math.max(x0, x1) + reach + HALF) / CELL), 0, N - 1);
        const iz0 = clamp(Math.floor((Math.min(z0, z1) - reach + HALF) / CELL), 0, N - 1), iz1 = clamp(Math.ceil((Math.max(z0, z1) + reach + HALF) / CELL), 0, N - 1);
        for (let iz = iz0; iz <= iz1; iz++) {
          for (let ix = ix0; ix <= ix1; ix++) {
            const { d, t } = segDist(ix * CELL - HALF, iz * CELL - HALF, x0, z0, x1, z1);
            if (d > reach) continue;
            const ph = p[i] + (p[i + 1] - p[i]) * t;
            const k = iz * N + ix;
            const c = ph + Math.max(0, d - HW) * WALL;
            if (c < ceil[k]) ceil[k] = c;
            const w = 1 - smoothstep(HW - 0.5, HW + 4, d);
            if (w > pinW[k]) { pinW[k] = w; pinH[k] = ph; }
          }
        }
      }
    }
    const g = { ceil, pinH, pinW };
    this.fitRoads(g);
    return g;
  }

  fitRoads({ ceil, pinH, pinW }) {
    const H = this.H;
    for (let i = 0; i < N * N; i++) {
      // Fill dips up to the road bed as an embankment (never out of the sea).
      if (pinW[i] > 0 && H[i] < pinH[i] && H[i] > WATER_LEVEL - 0.5) H[i] = lerp(H[i], pinH[i], pinW[i]);
      if (H[i] > ceil[i]) H[i] = ceil[i];
    }
  }

  buildRoads() {
    const H = this.H;
    const grade = this.gradeRoads();
    const mask = new Float32Array(N * N);
    const paint = (x0, z0, x1, z1, half, edge) => {
      const minx = Math.min(x0, x1) - edge, maxx = Math.max(x0, x1) + edge;
      const minz = Math.min(z0, z1) - edge, maxz = Math.max(z0, z1) + edge;
      const ix0 = clamp(Math.floor((minx + HALF) / CELL), 0, N - 1), ix1 = clamp(Math.ceil((maxx + HALF) / CELL), 0, N - 1);
      const iz0 = clamp(Math.floor((minz + HALF) / CELL), 0, N - 1), iz1 = clamp(Math.ceil((maxz + HALF) / CELL), 0, N - 1);
      for (let iz = iz0; iz <= iz1; iz++) {
        for (let ix = ix0; ix <= ix1; ix++) {
          const x = ix * CELL - HALF, z = iz * CELL - HALF;
          const { d } = segDist(x, z, x0, z0, x1, z1);
          if (d > edge) continue;
          const i = iz * N + ix;
          const m = 1 - smoothstep(half * 0.6, edge, d);
          if (m > mask[i]) mask[i] = m;
          const wob = 0.5 * this.noise2.noise(x * 0.15, z * 0.15);
          const c = 1 - smoothstep(half - 0.4 + wob, half + 0.6 + wob, d);
          const cv = Math.round(c * 255);
          if (cv > this.road[i]) this.road[i] = cv;
        }
      }
    };
    for (const road of ROADS) {
      const pts = smoothPolyline(road, 6);
      for (let k = 0; k < pts.length - 1; k++) paint(pts[k][0], pts[k][1], pts[k + 1][0], pts[k + 1][1], 2.4, 7);
    }
    paint(CAUSEWAY.x0, CAUSEWAY.z0, CAUSEWAY.x1, CAUSEWAY.z1, 2.6, 3.2);
    // Smooth the ground under roads so they read as graded paths.
    const blur = boxBlur(H, N, 5);
    const blur2 = boxBlur(blur, N, 5);
    for (let i = 0; i < N * N; i++) {
      if (mask[i] > 0) {
        H[i] = lerp(H[i], Math.max(blur2[i], 0.7), mask[i] * 0.9);
      }
    }
    // Blurring pulls pass floors up toward their walls: fit the bed again.
    this.fitRoads(grade);
  }

  shadePoint(ix, iz) {
    const i = iz * N + ix;
    const x = ix * CELL - HALF, z = iz * CELL - HALF;
    const h = this.H[i];
    const n = this.noise, n2 = this.noise2;
    const ny = this.normalYGrid(ix, iz);
    const slope = 1 - ny; // 0 flat .. 1 vertical
    const reg = { mount: this.wMount[i] / 255, desert: this.wDesert[i] / 255, forest: this.wForest[i] / 255 };
    const v = n2.fbm(x * 0.012, z * 0.012, 3); // broad colour variation
    const v2 = n.noise(x * 0.06, z * 0.06);

    let col, biome, grass = 0;
    let strata = 0;

    // Base vegetation colour by region.
    let g = mix3(C.meadowA, v > 0 ? C.meadowB : C.meadowC, Math.abs(v) * 1.4);
    biome = BIOME.MEADOW; grass = 1;
    if (reg.forest > 0) {
      const f = mix3(C.forestA, C.forestB, clamp(v * 0.5 + 0.5, 0, 1));
      g = mix3(g, f, reg.forest);
      if (reg.forest > 0.5) biome = BIOME.FOREST;
    }
    // Crystal-touched meadows near the Heartshard.
    const cd = Math.hypot(x - LAKE.x, z - LAKE.z);
    const cw = 1 - smoothstep(170, 300, cd + 30 * v2);
    if (cw > 0) {
      g = mix3(g, mix3(C.crystal, C.crystalB, clamp(v2 * 0.5 + 0.5, 0, 1)), cw * 0.7);
      if (cw > 0.5) biome = BIOME.CRYSTAL;
    }
    if (reg.mount > 0) {
      g = mix3(g, mix3(C.highland, C.highland2, clamp(v + 0.5, 0, 1)), reg.mount);
      if (reg.mount > 0.5) biome = BIOME.HIGHLAND;
      grass = 1 - smoothstep(60, 110, h) * 0.8;
    }
    if (reg.desert > 0) {
      const dcol = mix3(C.desert, C.desertB, clamp(v * 0.5 + 0.5, 0, 1));
      g = mix3(g, dcol, smoothstep(0.1, 0.6, reg.desert));
      if (reg.desert > 0.5) { biome = BIOME.DESERT; grass = 0.18; }
      else grass = lerp(1, 0.2, reg.desert / 0.5);
    }
    col = g;

    // Rock on steep slopes.
    const rockT = smoothstep(0.22 + 0.06 * v2, 0.38 + 0.06 * v2, slope);
    if (rockT > 0) {
      const rc = mix3(C.rock, C.rockDark, clamp(v * 0.5 + 0.5, 0, 1));
      col = mix3(col, rc, rockT);
      grass *= 1 - rockT;
      if (rockT > 0.5) {
        biome = BIOME.ROCK;
        if (reg.desert > 0.5) { biome = BIOME.MESA; }
      }
      strata = Math.round(rockT * smoothstep(0.3, 0.7, reg.desert) * 255);
    }

    // Snow caps.
    // Snow reaches lower around the Frostspire shrine so it sits in a winter basin.
    const shrineD = Math.hypot(x + 150, z + 700);
    const snowLine = 104 + 16 * v + 8 * v2 - 34 * (1 - smoothstep(70, 260, shrineD));
    const snowT = smoothstep(snowLine, snowLine + 14, h) * (1 - smoothstep(0.55, 0.75, slope) * 0.6);
    if (snowT > 0) {
      col = mix3(col, mix3(C.snow, C.snowShade, slope * 1.5), snowT);
      grass *= 1 - snowT;
      if (snowT > 0.5) biome = BIOME.SNOW;
    }

    // Beaches and sea floor.
    const beach = 1 - smoothstep(0.6, 2.2 + 0.8 * v2, h);
    if (beach > 0 && slope < 0.5) {
      const sandCol = reg.desert > 0.5 ? C.desert : C.sand;
      col = mix3(col, h < 0.2 ? C.wetSand : sandCol, beach);
      grass *= 1 - beach;
      if (beach > 0.5) biome = BIOME.BEACH;
    }
    if (h < -0.5) {
      col = mix3(C.wetSand, C.seabed, smoothstep(-0.5, -8, h));
      grass = 0; biome = BIOME.SEA;
    }

    // Roads and settlement plazas.
    const road = this.road[i] / 255;
    if (road > 0) {
      const rc = this.isPaved(x, z) ? C.plaza : C.road;
      col = mix3(col, rc, road * 0.92);
      grass *= 1 - road;
      if (road > 0.5) biome = BIOME.ROAD;
    }
    for (const loc of LOCATIONS) {
      if (!loc.plaza) continue;
      const d = Math.hypot(x - loc.x, z - loc.z) + 2 * v2;
      if (d < loc.plaza) {
        const t = 1 - smoothstep(loc.plaza - 3, loc.plaza, d);
        col = mix3(col, C.plaza, t); grass *= 1 - t; if (t > 0.5) biome = BIOME.PLAZA;
      }
    }

    // Subtle darkening in hollows, lift on crests (painterly ambient occlusion).
    const cav = this.cavity(ix, iz);
    const shade = clamp(1 + cav * 0.03, 0.85, 1.08);
    const o = i * 3;
    this.color[o] = clamp(col[0] * shade, 0, 255);
    this.color[o + 1] = clamp(col[1] * shade, 0, 255);
    this.color[o + 2] = clamp(col[2] * shade, 0, 255);
    this.biome[i] = biome;
    this.grass[i] = Math.round(clamp(grass, 0, 1) * 255);
    this.strata[i] = strata;
  }

  isPaved(x, z) {
    // The causeway and village squares are stone; everything else is dirt.
    return segDist(x, z, CAUSEWAY.x0, CAUSEWAY.z0, CAUSEWAY.x1, CAUSEWAY.z1).d < 4;
  }

  cavity(ix, iz) {
    const H = this.H;
    const i = iz * N + ix;
    const r = 3;
    const a = H[clampIdx(ix - r, iz)] + H[clampIdx(ix + r, iz)] + H[clampIdx(ix, iz - r)] + H[clampIdx(ix, iz + r)];
    return H[i] - a / 4;
  }

  // ---------- queries ----------
  forGridInRadius(x, z, r, fn) {
    const ix0 = clamp(Math.floor((x - r + HALF) / CELL), 0, N - 1), ix1 = clamp(Math.ceil((x + r + HALF) / CELL), 0, N - 1);
    const iz0 = clamp(Math.floor((z - r + HALF) / CELL), 0, N - 1), iz1 = clamp(Math.ceil((z + r + HALF) / CELL), 0, N - 1);
    for (let iz = iz0; iz <= iz1; iz++) {
      for (let ix = ix0; ix <= ix1; ix++) {
        const d = Math.hypot(ix * CELL - HALF - x, iz * CELL - HALF - z);
        if (d <= r) fn(iz * N + ix, d, ix, iz);
      }
    }
  }

  // Exact height of the rendered (LOD0) terrain surface.
  heightAt(x, z) {
    let gx = (x + HALF) / CELL, gz = (z + HALF) / CELL;
    gx = clamp(gx, 0, N - 1.0001); gz = clamp(gz, 0, N - 1.0001);
    const ix = gx | 0, iz = gz | 0;
    const fx = gx - ix, fz = gz - iz;
    const i = iz * N + ix, H = this.H;
    const h00 = H[i], h10 = H[i + 1], h01 = H[i + N], h11 = H[i + N + 1];
    if (fx > fz) return h00 + fx * (h10 - h00) + fz * (h11 - h10);
    return h00 + fz * (h01 - h00) + fx * (h11 - h01);
  }

  normalAt(x, z, out) {
    const e = 1.0;
    const hx = this.heightAt(x - e, z) - this.heightAt(x + e, z);
    const hz = this.heightAt(x, z - e) - this.heightAt(x, z + e);
    const len = Math.hypot(hx, 2 * e, hz);
    out.x = hx / len; out.y = (2 * e) / len; out.z = hz / len;
    return out;
  }

  normalYGrid(ix, iz) {
    const H = this.H;
    const hx = H[clampIdx(ix - 1, iz)] - H[clampIdx(ix + 1, iz)];
    const hz = H[clampIdx(ix, iz - 1)] - H[clampIdx(ix, iz + 1)];
    return (2 * CELL) / Math.hypot(hx, 2 * CELL, hz);
  }

  gridIndex(x, z) {
    const ix = clamp(Math.round((x + HALF) / CELL), 0, N - 1);
    const iz = clamp(Math.round((z + HALF) / CELL), 0, N - 1);
    return iz * N + ix;
  }
  biomeAt(x, z) { return this.biome[this.gridIndex(x, z)]; }
  regionAt(x, z) { return this.region[this.gridIndex(x, z)]; }
  grassAt(x, z) { return this.grass[this.gridIndex(x, z)] / 255; }
  roadAt(x, z) { return this.road[this.gridIndex(x, z)] / 255; }
  slopeAt(x, z) {
    const ix = clamp(Math.round((x + HALF) / CELL), 0, N - 1);
    const iz = clamp(Math.round((z + HALF) / CELL), 0, N - 1);
    return 1 - this.normalYGrid(ix, iz);
  }
  inWorld(x, z) { return Math.abs(x) < HALF - 4 && Math.abs(z) < HALF - 4; }
}

function clampIdx(ix, iz) {
  ix = ix < 0 ? 0 : ix >= N ? N - 1 : ix;
  iz = iz < 0 ? 0 : iz >= N ? N - 1 : iz;
  return iz * N + ix;
}

export function segDist(px, pz, x0, z0, x1, z1) {
  const dx = x1 - x0, dz = z1 - z0;
  const L2 = dx * dx + dz * dz || 1;
  let t = ((px - x0) * dx + (pz - z0) * dz) / L2;
  t = t < 0 ? 0 : t > 1 ? 1 : t;
  const qx = x0 + dx * t, qz = z0 + dz * t;
  return { d: Math.hypot(px - qx, pz - qz), t };
}

// Catmull-Rom through the control points so roads curve naturally.
export function smoothPolyline(pts, sub) {
  if (pts.length < 3) return pts;
  const out = [];
  for (let i = 0; i < pts.length - 1; i++) {
    const p0 = pts[Math.max(0, i - 1)], p1 = pts[i], p2 = pts[i + 1], p3 = pts[Math.min(pts.length - 1, i + 2)];
    for (let s = 0; s < sub; s++) {
      const t = s / sub, t2 = t * t, t3 = t2 * t;
      const f = (a, b, c, d) => 0.5 * (2 * b + (-a + c) * t + (2 * a - 5 * b + 4 * c - d) * t2 + (-a + 3 * b - 3 * c + d) * t3);
      out.push([f(p0[0], p1[0], p2[0], p3[0]), f(p0[1], p1[1], p2[1], p3[1])]);
    }
  }
  out.push(pts[pts.length - 1]);
  return out;
}

function boxBlur(src, n, r) {
  const tmp = new Float32Array(n * n), out = new Float32Array(n * n);
  const w = 2 * r + 1;
  for (let z = 0; z < n; z++) {
    let acc = 0;
    const row = z * n;
    for (let x = -r; x <= r; x++) acc += src[row + clamp(x, 0, n - 1)];
    for (let x = 0; x < n; x++) {
      tmp[row + x] = acc / w;
      acc += src[row + clamp(x + r + 1, 0, n - 1)] - src[row + clamp(x - r, 0, n - 1)];
    }
  }
  for (let x = 0; x < n; x++) {
    let acc = 0;
    for (let z = -r; z <= r; z++) acc += tmp[clamp(z, 0, n - 1) * n + x];
    for (let z = 0; z < n; z++) {
      out[z * n + x] = acc / w;
      acc += tmp[clamp(z + r + 1, 0, n - 1) * n + x] - tmp[clamp(z - r, 0, n - 1) * n + x];
    }
  }
  return out;
}

const tick = () => new Promise((r) => setTimeout(r, 0));
