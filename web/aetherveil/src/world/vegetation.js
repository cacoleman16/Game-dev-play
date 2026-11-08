// Trees, bushes, rocks, cacti and crystal clusters, scattered by biome and
// drawn as instanced meshes batched per 128 m cell with two levels of detail.
// Foliage sways in the wind; crystals glow.

import * as THREE from 'three';
import { hash2 } from '../core/noise.js';
import { smoothstep, lerp } from '../core/util.js';
import { BIOME, HALF, WATER_LEVEL } from './worldgen.js';
import { paint, place, lumpy, merge, cyl, cone, ico, crystal } from './geo.js';
import { makeDetailedMaterial } from './terrain.js';

const CELL = 128;
const NC = (HALF * 2) / CELL;
const GRID = 5; // candidate spacing in metres

const gradient = (lo, hi, y0, y1) => {
  const a = new THREE.Color(lo), b = new THREE.Color(hi), c = new THREE.Color();
  return (x, y) => c.copy(a).lerp(b, smoothstep(y0, y1, y));
};

function foliage(r, detail, x, y, z, col, seed, sy = 1) {
  const g = new THREE.IcosahedronGeometry(r, detail);
  g.scale(1, sy, 1);
  g.translate(x, y, z);
  const p = lumpy(paint(g, col), r * 0.22, 1, seed);
  // Soft "cotton ball" shading: normals point away from the blob centre.
  const pos = p.attributes.position, nor = p.attributes.normal;
  for (let i = 0; i < pos.count; i++) {
    let nx = pos.getX(i) - x, ny = (pos.getY(i) - y) / sy + 0.35, nz = pos.getZ(i) - z;
    const l = Math.hypot(nx, ny, nz) || 1;
    nor.setXYZ(i, nx / l, ny / l, nz / l);
  }
  return p;
}

function buildSpecies() {
  const S = {};
  const trunk = 0x6e4b33;
  // Oak: round, layered canopy.
  {
    const leaf = gradient(0x3b8034, 0x9ad35e, 2.6, 5.6);
    S.oak = {
      lod0: merge([
        place(cyl(0.22, 0.38, 3.2, 7, trunk, 0.1), 0, 1.6, 0),
        place(cyl(0.07, 0.13, 1.6, 5, trunk), 0.55, 2.7, 0, [0, 0, -0.8]),
        foliage(1.75, 1, 0, 4.0, 0, leaf, 1),
        foliage(1.3, 1, 1.15, 3.5, 0.45, leaf, 2),
        foliage(1.25, 1, -1.0, 3.6, -0.5, leaf, 3),
        foliage(1.15, 1, 0.2, 5.0, -0.25, leaf, 4),
      ]),
      lod1: merge([place(cyl(0.2, 0.36, 3.2, 5, trunk), 0, 1.6, 0), foliage(2.3, 0, 0, 4.2, 0, leaf, 5, 0.85)]),
      sway: true, collide: 0.4, shadow: true,
    };
  }
  // Blossom: pale bark and pink canopy around the Heartshard.
  {
    const leaf = gradient(0xd77fae, 0xffd9ea, 2.6, 5.4);
    S.blossom = {
      lod0: merge([
        place(cyl(0.18, 0.32, 3.0, 7, 0x7a5a52, 0.1), 0, 1.5, 0),
        foliage(1.6, 1, 0, 3.9, 0, leaf, 11),
        foliage(1.2, 1, 1.1, 3.4, 0.3, leaf, 12),
        foliage(1.15, 1, -1.0, 3.5, -0.4, leaf, 13),
        foliage(1.0, 1, 0.1, 4.8, 0.2, leaf, 14),
      ]),
      lod1: merge([place(cyl(0.18, 0.3, 3.0, 5, 0x7a5a52), 0, 1.5, 0), foliage(2.1, 0, 0, 4.0, 0, leaf, 15, 0.85)]),
      sway: true, collide: 0.35, shadow: true,
    };
  }
  // Pines, plain and snow-dusted.
  const pine = (snow) => {
    const base = gradient(0x24583a, 0x4f9152, 1.5, 7);
    const col = snow
      ? (x, y, z, nx, ny) => (ny > 0.25 ? new THREE.Color(0xeef4fb) : base(x, y))
      : base;
    const tiers = [[2.1, 2.6, 2.4], [1.7, 2.4, 3.7], [1.3, 2.1, 4.9], [0.8, 1.8, 6.0]];
    return {
      lod0: merge([place(cyl(0.15, 0.28, 2.2, 6, 0x5a3d2a), 0, 1.1, 0), ...tiers.map(([r, h, y]) => place(cone(r, h, 8, col, 0.08), 0, y, 0))]),
      lod1: merge([place(cyl(0.15, 0.28, 2.2, 4, 0x5a3d2a), 0, 1.1, 0), place(cone(2.1, 4.6, 6, col), 0, 3.4, 0), place(cone(1.1, 2.6, 6, col), 0, 5.6, 0)]),
      sway: true, collide: 0.35, shadow: true,
    };
  };
  S.pine = pine(false);
  S.snowpine = pine(true);
  // Acacia: flat canopy for the badlands.
  {
    const leaf = gradient(0x7d8c36, 0xb2bf55, 3.2, 4.2);
    S.acacia = {
      lod0: merge([
        place(cyl(0.14, 0.24, 3.4, 6, 0x6a4a35), 0.25, 1.7, 0, [0, 0, -0.15]),
        place(cyl(0.07, 0.12, 1.6, 5, 0x6a4a35), -0.2, 3.0, 0, [0, 0, 0.7]),
        foliage(2.3, 1, 0.5, 3.8, 0, leaf, 21, 0.32),
        foliage(1.4, 1, -0.9, 3.5, 0.3, leaf, 22, 0.35),
      ]),
      lod1: merge([place(cyl(0.14, 0.24, 3.4, 4, 0x6a4a35), 0.25, 1.7, 0), foliage(2.6, 0, 0.3, 3.8, 0, leaf, 23, 0.35)]),
      sway: true, collide: 0.3, shadow: true,
    };
  }
  // Dead tree: bare, twisted branches.
  {
    const bark = 0x7b6a5c;
    const parts = [place(cyl(0.16, 0.3, 3.4, 6, bark, 0.1), 0, 1.7, 0)];
    for (let i = 0; i < 4; i++) {
      const a = i * 1.7;
      parts.push(place(cyl(0.05, 0.1, 1.6, 5, bark), Math.cos(a) * 0.5, 2.6 + i * 0.35, Math.sin(a) * 0.5, [Math.sin(a) * 0.8, 0, -Math.cos(a) * 0.8]));
    }
    S.deadtree = { lod0: merge(parts), lod1: merge([place(cyl(0.14, 0.28, 3.4, 4, bark), 0, 1.7, 0)]), sway: false, collide: 0.3, shadow: true };
  }
  // Cactus.
  {
    const g = 0x4f8f4a;
    S.cactus = {
      lod0: merge([
        place(cyl(0.34, 0.38, 3.0, 8, g, 0.15), 0, 1.5, 0),
        place(new THREE.SphereGeometry(0.34, 8, 4, 0, Math.PI * 2, 0, Math.PI / 2), 0, 3.0, 0),
        place(cyl(0.2, 0.2, 0.8, 6, g), 0.55, 1.5, 0, [0, 0, Math.PI / 2]),
        place(cyl(0.2, 0.2, 1.1, 6, g), 0.9, 2.0, 0),
        place(cyl(0.18, 0.18, 0.7, 6, g), -0.5, 2.0, 0, [0, 0, Math.PI / 2]),
        place(cyl(0.18, 0.18, 0.9, 6, g), -0.8, 2.4, 0),
      ].map((x) => (x.attributes.color ? x : paint(x, g)))),
      lod1: merge([place(cyl(0.34, 0.38, 3.0, 5, g), 0, 1.5, 0)]),
      sway: false, collide: 0.4, shadow: true,
    };
  }
  // Bush.
  {
    const leaf = gradient(0x3f7d34, 0x86c25a, 0, 1.2);
    S.bush = {
      lod0: merge([foliage(0.9, 1, 0, 0.55, 0, leaf, 31, 0.75), foliage(0.6, 1, 0.6, 0.4, 0.2, leaf, 32, 0.8)]),
      lod1: merge([foliage(1.0, 0, 0, 0.5, 0, leaf, 33, 0.7)]),
      sway: true, collide: 0, shadow: false,
    };
  }
  // Rocks: mossy, red badland stone, and snow-capped.
  const rock = (base, top, seed) => {
    const col = (x, y, z, nx, ny) => (ny > 0.55 ? new THREE.Color(top) : new THREE.Color(base));
    const g0 = new THREE.IcosahedronGeometry(1, 1);
    g0.scale(1.3, 0.8, 1.1);
    const lod0 = lumpy(paint(g0, col, 0.12, seed), 0.35, 1, seed);
    lod0.translate(0, 0.35, 0);
    const g1 = new THREE.IcosahedronGeometry(1, 0);
    g1.scale(1.3, 0.8, 1.1);
    const lod1 = lumpy(paint(g1, col), 0.25, 1, seed);
    lod1.translate(0, 0.35, 0);
    return { lod0, lod1, sway: false, collide: 1.0, solidTop: 1.0, shadow: true, isRock: true };
  };
  S.rock = rock(0x8a8478, 0x7f9a58, 41);
  S.redrock = rock(0xb0603c, 0xd08a58, 42);
  S.snowrock = rock(0x7d7b80, 0xf0f5fb, 43);
  // Crystal clusters (glowing).
  {
    const parts = [];
    const cols = [0x7ff0ff, 0xb48cff, 0x9ffcf0, 0xd0a8ff];
    for (let i = 0; i < 5; i++) {
      const a = i * 1.3, tilt = i === 0 ? 0 : 0.45;
      const h = i === 0 ? 2.6 : 1.2 + (i % 3) * 0.4;
      const c = crystal(i === 0 ? 0.32 : 0.2, h, cols[i % 4]);
      place(c, Math.cos(a) * (i ? 0.35 : 0), h * 0.4, Math.sin(a) * (i ? 0.35 : 0), [Math.cos(a) * tilt, 0, Math.sin(a) * tilt]);
      parts.push(c);
    }
    const g = merge(parts);
    S.crystal = { lod0: g, lod1: g, sway: false, collide: 0.5, shadow: false, glow: true };
  }
  return S;
}

export function makeFoliageMaterial(game, sway) {
  const m = new THREE.MeshLambertMaterial({ vertexColors: true });
  if (sway) {
    m.onBeforeCompile = (sh) => {
      sh.uniforms.uTime = game.uniforms.uTime;
      sh.vertexShader = sh.vertexShader
        .replace('#include <common>', '#include <common>\nuniform float uTime;')
        .replace('#include <begin_vertex>', `#include <begin_vertex>
          #ifdef USE_INSTANCING
            vec3 ip = vec3(instanceMatrix[3][0], instanceMatrix[3][1], instanceMatrix[3][2]);
          #else
            vec3 ip = vec3(0.0);
          #endif
          float sw = max(position.y - 1.0, 0.0) * 0.03;
          transformed.x += sin(uTime * 1.3 + ip.x * 0.11 + ip.z * 0.07) * sw;
          transformed.z += cos(uTime * 1.1 + ip.x * 0.07 + ip.z * 0.13) * sw * 0.7;`);
    };
    m.customProgramCacheKey = () => 'foliage-sway';
  }
  return m;
}

export function makeGlowMaterial(strength = 0.9) {
  const m = new THREE.MeshLambertMaterial({ vertexColors: true });
  m.onBeforeCompile = (sh) => {
    sh.fragmentShader = sh.fragmentShader.replace('#include <emissivemap_fragment>',
      `#include <emissivemap_fragment>\n totalEmissiveRadiance = diffuseColor.rgb * ${strength.toFixed(2)};`);
  };
  m.customProgramCacheKey = () => 'glow' + strength;
  return m;
}

export class Vegetation {
  constructor(game) {
    this.game = game;
    this.group = new THREE.Group();
    this.group.name = 'vegetation';
    this.species = buildSpecies();
    this.matSway = makeFoliageMaterial(game, true);
    this.matStill = makeDetailedMaterial({ detail: 0.22 });
    this.matGlow = makeGlowMaterial(0.85);
    this.cells = [];
    this.lastX = 1e9; this.lastZ = 1e9;
    this.counts = {};
  }

  // Decide which species (if any) grows at a candidate point.
  choose(x, z, ix, iz) {
    const w = this.game.world;
    const h = w.heightAt(x, z);
    if (h < WATER_LEVEL + 0.8) return null;
    const biome = w.biomeAt(x, z);
    const slope = w.slopeAt(x, z);
    const r = hash2(ix, iz, 9001);
    const r2 = hash2(ix, iz, 4242);
    const grove = w.noise2.fbm(x * 0.007 + 40, z * 0.007 - 12, 3);
    switch (biome) {
      case BIOME.FOREST: {
        if (slope > 0.5) return r < 0.04 ? 'rock' : null;
        const dense = 0.36 + 0.22 * smoothstep(-0.2, 0.4, grove);
        if (r < dense) return r2 < 0.5 ? 'oak' : 'pine';
        if (r < dense + 0.07) return 'bush';
        if (r < dense + 0.09) return 'rock';
        return null;
      }
      case BIOME.MEADOW: {
        const p = grove > 0.3 ? 0.22 : 0.008;
        if (r < p) return 'oak';
        if (r < p + 0.018) return 'bush';
        if (r < p + 0.024) return 'rock';
        return null;
      }
      case BIOME.CRYSTAL: {
        if (r < (grove > 0.25 ? 0.12 : 0.02)) return 'blossom';
        if (r < 0.155 && r > 0.15) return 'crystal';
        if (r < 0.17 && r > 0.155) return 'bush';
        return null;
      }
      case BIOME.HIGHLAND: {
        const alt = 1 - smoothstep(80, 150, h);
        if (slope > 0.55) return r < 0.03 ? 'rock' : null;
        if (r < 0.2 * alt) return h > 95 ? 'snowpine' : 'pine';
        if (r < 0.2 * alt + 0.035) return 'rock';
        if (r < 0.2 * alt + 0.04 && h > 70) return 'crystal';
        return null;
      }
      case BIOME.SNOW: {
        if (r < 0.07 && h < 175 && slope < 0.5) return 'snowpine';
        if (r < 0.05) return 'snowrock';
        return null;
      }
      case BIOME.ROCK: return r < 0.035 ? (w.regionAt(x, z) === 3 ? 'redrock' : 'rock') : null;
      case BIOME.MESA:
      case BIOME.DESERT: {
        if (slope > 0.45) return r < 0.03 ? 'redrock' : null;
        if (r < 0.012) return 'cactus';
        if (r < 0.02) return 'acacia';
        if (r < 0.026) return 'deadtree';
        if (r < 0.05) return 'redrock';
        return null;
      }
      case BIOME.BEACH: return r < 0.01 ? 'rock' : null;
      default: return null;
    }
  }

  async build(progress = () => {}) {
    const game = this.game, w = game.world, q = game.quality;
    const colliders = game.colliders;
    const clear = game.world.locations.map((l) => ({ x: l.x, z: l.z, r: l.clear ?? (l.type === 'ruins' ? (l.plaza || 10) + 6 : l.r * 0.9) }));
    const perCell = Array.from({ length: NC * NC }, () => ({}));
    const n = (HALF * 2) / GRID;
    const tmpColor = new THREE.Color();
    for (let iz = 0; iz < n; iz++) {
      for (let ix = 0; ix < n; ix++) {
        const x = ix * GRID - HALF + (hash2(ix, iz, 7) - 0.5) * GRID * 0.9;
        const z = iz * GRID - HALF + (hash2(ix, iz, 8) - 0.5) * GRID * 0.9;
        if (!w.inWorld(x, z)) continue;
        const sp = this.choose(x, z, ix, iz);
        if (!sp) continue;
        const def = this.species[sp];
        const dens = def.isRock ? q.propDensity : q.treeDensity;
        if (hash2(ix, iz, 555) > dens) continue;
        if (w.roadAt(x, z) > 0.02) continue;
        let blocked = false;
        for (const c of clear) if ((x - c.x) ** 2 + (z - c.z) ** 2 < c.r * c.r) { blocked = true; break; }
        if (blocked) continue;
        if (colliders.occupied(x, z, 1.2)) continue;
        const s = def.isRock ? lerp(0.5, 2.2, hash2(ix, iz, 11) ** 2) : lerp(0.8, 1.35, hash2(ix, iz, 12));
        const y = w.heightAt(x, z) - (def.isRock ? 0.25 * s : 0.1);
        const rot = hash2(ix, iz, 13) * Math.PI * 2;
        const cell = Math.floor((x + HALF) / CELL) + Math.floor((z + HALF) / CELL) * NC;
        const list = perCell[cell][sp] || (perCell[cell][sp] = []);
        tmpColor.setHSL(0, 0, 1).offsetHSL((hash2(ix, iz, 14) - 0.5) * 0.04, 0, (hash2(ix, iz, 15) - 0.5) * 0.16);
        list.push({ x, y, z, s, rot, c: tmpColor.clone() });
        this.counts[sp] = (this.counts[sp] || 0) + 1;
        if (def.collide > 0) {
          if (def.isRock) {
            if (s > 0.9) colliders.addCircle(x, z, def.collide * s * 1.05, y - 1, y + 0.35 * s + 0.8 * s * 0.9, 'rock');
          } else colliders.addCircle(x, z, def.collide * s, y - 1, y + 6 * s, 'tree');
        }
      }
      if (iz % 40 === 0) { progress(iz / n); await new Promise((r) => setTimeout(r, 0)); }
    }

    const m4 = new THREE.Matrix4(), qt = new THREE.Quaternion(), up = new THREE.Vector3(0, 1, 0), pos = new THREE.Vector3(), scl = new THREE.Vector3();
    const totals = {};
    for (let c = 0; c < NC * NC; c++) {
      const entries = perCell[c];
      const cx = (c % NC) * CELL - HALF + CELL / 2, cz = Math.floor(c / NC) * CELL - HALF + CELL / 2;
      const cell = { x: cx, z: cz, lod0: [], far: {}, state: -1 };
      for (const sp of Object.keys(entries)) {
        const def = this.species[sp];
        const list = entries[sp];
        const mat = def.glow ? this.matGlow : def.sway ? this.matSway : this.matStill;
        // Raw per-instance data, copied into the shared far batch when needed.
        const mats = new Float32Array(list.length * 16), cols = new Float32Array(list.length * 3);
        list.forEach((e, i) => {
          qt.setFromAxisAngle(up, e.rot);
          m4.compose(pos.set(e.x, e.y, e.z), qt, scl.setScalar(e.s));
          m4.toArray(mats, i * 16);
          cols[i * 3] = e.c.r; cols[i * 3 + 1] = e.c.g; cols[i * 3 + 2] = e.c.b;
        });
        cell.far[sp] = { mats, cols, n: list.length };
        totals[sp] = (totals[sp] || 0) + list.length;
        const im = new THREE.InstancedMesh(def.lod0, mat, list.length);
        im.instanceMatrix.array.set(mats);
        im.instanceColor = new THREE.InstancedBufferAttribute(cols.slice(), 3);
        im.instanceMatrix.needsUpdate = true;
        im.computeBoundingSphere();
        im.castShadow = def.shadow && q.shadows;
        im.receiveShadow = true;
        im.visible = false;
        im.matrixAutoUpdate = false;
        cell.lod0.push(im);
        this.group.add(im);
      }
      this.cells.push(cell);
    }
    // One far-detail batch per species, refilled as the camera moves.
    this.far = {};
    for (const sp of Object.keys(totals)) {
      const def = this.species[sp];
      const mat = def.glow ? this.matGlow : def.sway ? this.matSway : this.matStill;
      const im = new THREE.InstancedMesh(def.lod1, mat, totals[sp]);
      im.instanceMatrix.setUsage(THREE.DynamicDrawUsage);
      im.instanceColor = new THREE.InstancedBufferAttribute(new Float32Array(totals[sp] * 3), 3);
      im.instanceColor.setUsage(THREE.DynamicDrawUsage);
      im.count = 0;
      im.frustumCulled = false;
      im.matrixAutoUpdate = false;
      this.far[sp] = im;
      this.group.add(im);
    }
  }

  update(camPos) {
    if (Math.abs(camPos.x - this.lastX) < 12 && Math.abs(camPos.z - this.lastZ) < 12) return;
    this.lastX = camPos.x; this.lastZ = camPos.z;
    const q = this.game.quality;
    const near = 70 * q.lodScale, far = q.viewDist * 1.05;
    let changed = false;
    for (const cell of this.cells) {
      const dx = Math.max(Math.abs(camPos.x - cell.x) - CELL / 2, 0);
      const dz = Math.max(Math.abs(camPos.z - cell.z) - CELL / 2, 0);
      const d = Math.hypot(dx, dz);
      const state = d < near ? 0 : d < far ? 1 : 2;
      if (state === cell.state) continue;
      changed = true;
      cell.state = state;
      for (const m of cell.lod0) m.visible = state === 0;
    }
    if (!changed) return;
    for (const sp in this.far) {
      const im = this.far[sp];
      let n = 0;
      for (const cell of this.cells) {
        if (cell.state !== 1) continue;
        const f = cell.far[sp];
        if (!f) continue;
        im.instanceMatrix.array.set(f.mats, n * 16);
        im.instanceColor.array.set(f.cols, n * 3);
        n += f.n;
      }
      im.count = n;
      im.instanceMatrix.needsUpdate = true;
      im.instanceColor.needsUpdate = true;
    }
  }
}
