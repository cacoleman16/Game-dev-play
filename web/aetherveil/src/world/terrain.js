// Chunked terrain with distance LOD. Each 128 m chunk swaps between four
// resolutions (2, 4, 8, 16 m) and hides seams with skirts. Geometry is built
// lazily and cached; the shared material adds painterly detail noise and
// banded strata on the red desert cliffs.

import * as THREE from 'three';
import { N, HALF, CELL } from './worldgen.js';

const CHUNK_CELLS = 128;
const NCH = (N - 1) / CHUNK_CELLS; // 8
const STEPS = [1, 2, 4, 8];
const SKIRT = 6;

export class Terrain {
  constructor(game) {
    this.game = game;
    this.world = game.world;
    this.group = new THREE.Group();
    this.group.name = 'terrain';
    this.material = makeTerrainMaterial(game);
    this.indexCache = new Map();
    this.geomCache = new Map();
    this.chunks = [];
    this.buildQueue = [];
    const lodScale = game.quality.lodScale;
    this.lodDist = [110, 300, 600].map((d) => d * lodScale);
    for (let cz = 0; cz < NCH; cz++) {
      for (let cx = 0; cx < NCH; cx++) {
        const mesh = new THREE.Mesh(new THREE.BufferGeometry(), this.material);
        mesh.receiveShadow = true;
        mesh.castShadow = false;
        mesh.matrixAutoUpdate = false;
        mesh.visible = false;
        const minX = cx * CHUNK_CELLS * CELL - HALF, minZ = cz * CHUNK_CELLS * CELL - HALF;
        const size = CHUNK_CELLS * CELL;
        const chunk = { cx, cz, mesh, step: -1, wanted: -1, cxw: minX + size / 2, czw: minZ + size / 2, size };
        this.chunks.push(chunk);
        this.group.add(mesh);
      }
    }
  }

  lodFor(chunk, px, pz) {
    const half = chunk.size / 2;
    const dx = Math.max(Math.abs(px - chunk.cxw) - half, 0);
    const dz = Math.max(Math.abs(pz - chunk.czw) - half, 0);
    const d = Math.hypot(dx, dz);
    if (d < this.lodDist[0]) return 0;
    if (d < this.lodDist[1]) return 1;
    if (d < this.lodDist[2]) return 2;
    return 3;
  }

  // Called each frame with the camera position. `immediate` builds everything now.
  update(px, pz, immediate = false) {
    const viewDist = this.game.quality.viewDist * 1.35 + 100;
    let budget = immediate ? Infinity : this.game.quality.chunkBuildsPerFrame;
    const pending = [];
    for (const c of this.chunks) {
      const dist = Math.hypot(px - c.cxw, pz - c.czw) - c.size * 0.7;
      if (dist > viewDist) { c.mesh.visible = false; continue; }
      const lod = this.lodFor(c, px, pz);
      if (lod !== c.step) pending.push({ c, lod, dist });
      else c.mesh.visible = true;
    }
    pending.sort((a, b) => a.dist - b.dist);
    for (const p of pending) {
      const key = p.c.cx + ',' + p.c.cz + ',' + p.lod;
      let geom = this.geomCache.get(key);
      if (!geom) {
        if (budget <= 0) { if (p.c.step >= 0) p.c.mesh.visible = true; continue; }
        geom = this.buildGeometry(p.c, STEPS[p.lod]);
        this.geomCache.set(key, geom);
        budget--;
      }
      p.c.mesh.geometry = geom;
      p.c.step = p.lod;
      p.c.mesh.visible = true;
    }
    if (this.geomCache.size > 160) this.evict(px, pz);
  }

  evict(px, pz) {
    for (const [key, geom] of this.geomCache) {
      const [cx, cz, lod] = key.split(',').map(Number);
      const c = this.chunks[cz * NCH + cx];
      if (c.mesh.geometry === geom) continue;
      if (lod <= 1 && this.lodFor(c, px, pz) > 1) {
        geom.dispose();
        this.geomCache.delete(key);
      }
    }
  }

  getIndex(step) {
    if (this.indexCache.has(step)) return this.indexCache.get(step);
    const S = CHUNK_CELLS / step + 1;
    const idx = [];
    for (let j = 0; j < S - 1; j++) {
      for (let i = 0; i < S - 1; i++) {
        const v00 = j * S + i, v10 = v00 + 1, v01 = v00 + S, v11 = v01 + 1;
        idx.push(v00, v01, v11, v00, v11, v10);
      }
    }
    // Skirts: both windings so they hide cracks from either side.
    const base = S * S;
    const edges = [
      (k) => k, // top row
      (k) => (S - 1) * S + k, // bottom row
      (k) => k * S, // left column
      (k) => k * S + S - 1, // right column
    ];
    for (let e = 0; e < 4; e++) {
      for (let k = 0; k < S - 1; k++) {
        const a = edges[e](k), b = edges[e](k + 1);
        const sa = base + e * S + k, sb = sa + 1;
        idx.push(a, b, sb, a, sb, sa, a, sb, b, a, sa, sb);
      }
    }
    const attr = new THREE.BufferAttribute(new Uint16Array(idx), 1);
    this.indexCache.set(step, attr);
    return attr;
  }

  buildGeometry(chunk, step) {
    const w = this.world;
    const S = CHUNK_CELLS / step + 1;
    const total = S * S + 4 * S;
    const pos = new Float32Array(total * 3);
    const nor = new Float32Array(total * 3);
    const col = new Uint8Array(total * 3);
    const str = new Uint8Array(total);
    const gx0 = chunk.cx * CHUNK_CELLS, gz0 = chunk.cz * CHUNK_CELLS;
    const H = w.H;
    const writeVert = (v, gx, gz, drop) => {
      const i = gz * N + gx;
      pos[v * 3] = gx * CELL - HALF;
      pos[v * 3 + 1] = H[i] - drop;
      pos[v * 3 + 2] = gz * CELL - HALF;
      const xl = gx > 0 ? gx - 1 : gx, xr = gx < N - 1 ? gx + 1 : gx;
      const zl = gz > 0 ? gz - 1 : gz, zr = gz < N - 1 ? gz + 1 : gz;
      const hx = (H[gz * N + xl] - H[gz * N + xr]) / ((xr - xl) * CELL);
      const hz = (H[zl * N + gx] - H[zr * N + gx]) / ((zr - zl) * CELL);
      const inv = 1 / Math.hypot(hx, 1, hz);
      nor[v * 3] = hx * inv; nor[v * 3 + 1] = inv; nor[v * 3 + 2] = hz * inv;
      col[v * 3] = w.color[i * 3]; col[v * 3 + 1] = w.color[i * 3 + 1]; col[v * 3 + 2] = w.color[i * 3 + 2];
      str[v] = w.strata[i];
    };
    for (let j = 0; j < S; j++) for (let i = 0; i < S; i++) writeVert(j * S + i, gx0 + i * step, gz0 + j * step, 0);
    const base = S * S;
    const drop = SKIRT * step;
    for (let k = 0; k < S; k++) {
      writeVert(base + k, gx0 + k * step, gz0, drop);
      writeVert(base + S + k, gx0 + k * step, gz0 + (S - 1) * step, drop);
      writeVert(base + 2 * S + k, gx0, gz0 + k * step, drop);
      writeVert(base + 3 * S + k, gx0 + (S - 1) * step, gz0 + k * step, drop);
    }
    const g = new THREE.BufferGeometry();
    g.setAttribute('position', new THREE.BufferAttribute(pos, 3));
    g.setAttribute('normal', new THREE.BufferAttribute(nor, 3));
    g.setAttribute('color', new THREE.BufferAttribute(col, 3, true));
    g.setAttribute('aStrata', new THREE.BufferAttribute(str, 1, true));
    g.setIndex(this.getIndex(step));
    g.computeBoundingSphere();
    g.computeBoundingBox();
    return g;
  }
}

// A tileable 3-channel value-noise texture used for surface detail everywhere.
let detailTex = null;
export function getDetailTexture() {
  if (detailTex) return detailTex;
  const S = 256;
  const data = new Uint8Array(S * S * 4);
  const lattice = (period, seed) => {
    const g = new Float32Array(period * period);
    let s = seed;
    for (let i = 0; i < g.length; i++) { s = (s * 16807) % 2147483647; g[i] = s / 2147483647; }
    return (u, v) => {
      const x = u * period, y = v * period;
      const x0 = Math.floor(x), y0 = Math.floor(y);
      const fx = x - x0, fy = y - y0;
      const sx = fx * fx * (3 - 2 * fx), sy = fy * fy * (3 - 2 * fy);
      const at = (a, b) => g[((b % period + period) % period) * period + ((a % period + period) % period)];
      const a = at(x0, y0), b = at(x0 + 1, y0), c = at(x0, y0 + 1), d = at(x0 + 1, y0 + 1);
      return a + (b - a) * sx + (c - a) * sy + (a - b - c + d) * sx * sy;
    };
  };
  const octs = [[lattice(8, 11), lattice(16, 12), lattice(32, 13)], [lattice(4, 21), lattice(8, 22), lattice(16, 23)], [lattice(32, 31), lattice(64, 32), lattice(128, 33)]];
  for (let y = 0; y < S; y++) {
    for (let x = 0; x < S; x++) {
      const u = x / S, v = y / S;
      const o = (y * S + x) * 4;
      for (let c = 0; c < 3; c++) {
        const f = octs[c];
        const val = f[0](u, v) * 0.55 + f[1](u, v) * 0.3 + f[2](u, v) * 0.15;
        data[o + c] = Math.round(val * 255);
      }
      data[o + 3] = 255;
    }
  }
  detailTex = new THREE.DataTexture(data, S, S, THREE.RGBAFormat);
  detailTex.wrapS = detailTex.wrapT = THREE.RepeatWrapping;
  detailTex.magFilter = THREE.LinearFilter;
  detailTex.minFilter = THREE.LinearMipmapLinearFilter;
  detailTex.generateMipmaps = true;
  detailTex.anisotropy = 4;
  detailTex.needsUpdate = true;
  return detailTex;
}

function makeTerrainMaterial(game) {
  const mat = new THREE.MeshLambertMaterial({ vertexColors: true });
  const uniforms = { uDetail: { value: getDetailTexture() }, uTime: game.uniforms.uTime };
  mat.onBeforeCompile = (shader) => {
    Object.assign(shader.uniforms, uniforms);
    shader.vertexShader = shader.vertexShader
      .replace('#include <common>', `#include <common>
        attribute float aStrata;
        varying float vStrata;
        varying vec3 vWPos;`)
      .replace('#include <color_vertex>', `#include <color_vertex>
        vColor.rgb = pow(vColor.rgb, vec3(2.2));
        vStrata = aStrata;`)
      .replace('#include <beginnormal_vertex>', `#include <beginnormal_vertex>
        vWNormal = objectNormal;`)
      .replace('#include <worldpos_vertex>', `#include <worldpos_vertex>
        vWPos = (modelMatrix * vec4(transformed, 1.0)).xyz;`);
    shader.vertexShader = shader.vertexShader.replace('varying vec3 vWPos;', 'varying vec3 vWPos;\nvarying vec3 vWNormal;');
    shader.fragmentShader = shader.fragmentShader
      .replace('#include <common>', `#include <common>
        uniform sampler2D uDetail;
        varying float vStrata;
        varying vec3 vWPos;
        varying vec3 vWNormal;`)
      .replace('#include <color_fragment>', `#include <color_fragment>
        vec3 dA = texture2D(uDetail, vWPos.xz * 0.09).rgb;
        vec3 dB = texture2D(uDetail, vWPos.xz * 0.017).rgb;
        float det = (dA.r - 0.5) * 0.2 + (dB.g - 0.5) * 0.3 + (dA.b - 0.5) * 0.1;
        // Classify the ground from its colour to pick a surface pattern.
        vec3 base = diffuseColor.rgb;
        float mx = max(base.r, max(base.g, base.b)), mn = min(base.r, min(base.g, base.b));
        float sat = (mx - mn) / max(mx, 1e-3);
        float grassW = smoothstep(0.02, 0.1, base.g - max(base.r, base.b));
        float snowW = smoothstep(0.42, 0.62, mn) * (1.0 - smoothstep(0.18, 0.32, sat));
        float sandW = smoothstep(0.08, 0.25, base.r - base.b) * (1.0 - grassW) * (1.0 - snowW) * (1.0 - vStrata);
        float rockW = (1.0 - smoothstep(0.12, 0.3, sat)) * (1.0 - snowW) * (1.0 - grassW);
        // Grass: directional brush strokes that wander in direction, plus darker clumps.
        float ang = dB.r * 6.2831;
        mat2 rot = mat2(cos(ang), -sin(ang), sin(ang), cos(ang));
        float stroke = texture2D(uDetail, (rot * vWPos.xz) * vec2(0.55, 0.11)).r;
        float clump = smoothstep(0.55, 0.75, texture2D(uDetail, vWPos.xz * 0.045).b);
        det += grassW * ((stroke - 0.5) * 0.34 - clump * 0.12);
        // Rock: triplanar cracks and faint layering.
        vec3 an = abs(normalize(vWNormal));
        an /= (an.x + an.y + an.z);
        float rn = texture2D(uDetail, vWPos.zy * 0.21).r * an.x + texture2D(uDetail, vWPos.xz * 0.21).r * an.y + texture2D(uDetail, vWPos.xy * 0.21).r * an.z;
        float crack = 1.0 - smoothstep(0.0, 0.03, abs(rn - 0.5));
        det += rockW * (-crack * 0.32 + sin(vWPos.y * 2.6 + rn * 5.0) * 0.05 + (rn - 0.5) * 0.18);
        // Sand: wind ripples.
        float rip = sin(dot(vWPos.xz, vec2(0.83, 0.55)) * 2.4 + dA.g * 7.0);
        det += sandW * rip * 0.06;
        diffuseColor.rgb *= 1.0 + det;
        // Snow: cold blue in the hollows and scattered glints.
        diffuseColor.rgb = mix(diffuseColor.rgb, diffuseColor.rgb * vec3(0.86, 0.92, 1.05), snowW * (1.0 - an.y) * 0.8);
        float glint = step(0.993, fract(sin(dot(floor(vWPos.xz * 7.0), vec2(12.9898, 78.233))) * 43758.5453));
        diffuseColor.rgb += glint * snowW * 0.5;
        if (vStrata > 0.01) {
          float b = vWPos.y * 0.23 + dB.r * 1.6 + dA.g * 0.3;
          float k = fract(b);
          vec3 s1 = vec3(0.48, 0.11, 0.04);
          vec3 s2 = vec3(0.69, 0.25, 0.08);
          vec3 s3 = vec3(0.82, 0.48, 0.20);
          vec3 sc = k < 0.33 ? mix(s1, s2, smoothstep(0.25, 0.33, k)) : (k < 0.7 ? mix(s2, s3, smoothstep(0.62, 0.7, k)) : mix(s3, s1, smoothstep(0.92, 1.0, k)));
          sc *= 0.85 + dA.r * 0.3;
          diffuseColor.rgb = mix(diffuseColor.rgb, sc, vStrata);
        }
        // Wet, darker ground right at the waterline.
        diffuseColor.rgb *= mix(0.72, 1.0, smoothstep(-0.2, 0.6, vWPos.y));`);
  };
  mat.customProgramCacheKey = () => 'terrain-v2';
  return mat;
}

// Lambert with world-space triplanar detail, for buildings, rocks and trunks.
export function makeDetailedMaterial(opts = {}) {
  const { detail, ...rest } = opts;
  const m = new THREE.MeshLambertMaterial({ vertexColors: true, ...rest });
  const strength = detail ?? 0.16;
  m.onBeforeCompile = (sh) => {
    sh.uniforms.uDetail = { value: getDetailTexture() };
    sh.vertexShader = sh.vertexShader
      .replace('#include <common>', '#include <common>\nvarying vec3 vDW;\nvarying vec3 vDN;')
      .replace('#include <beginnormal_vertex>', '#include <beginnormal_vertex>\nvDN = normalize(mat3(modelMatrix) * objectNormal);')
      .replace('#include <worldpos_vertex>', `#include <worldpos_vertex>
        #ifdef USE_INSTANCING
          vDW = (modelMatrix * instanceMatrix * vec4(transformed, 1.0)).xyz;
        #else
          vDW = (modelMatrix * vec4(transformed, 1.0)).xyz;
        #endif`);
    sh.fragmentShader = sh.fragmentShader
      .replace('#include <common>', '#include <common>\nuniform sampler2D uDetail;\nvarying vec3 vDW;\nvarying vec3 vDN;')
      .replace('#include <color_fragment>', `#include <color_fragment>
        vec3 an = abs(normalize(vDN)); an /= (an.x + an.y + an.z);
        vec3 tx = texture2D(uDetail, vDW.zy * 0.7).rgb * an.x + texture2D(uDetail, vDW.xz * 0.7).rgb * an.y + texture2D(uDetail, vDW.xy * 0.7).rgb * an.z;
        vec3 tl = texture2D(uDetail, vDW.zy * 0.13).rgb * an.x + texture2D(uDetail, vDW.xz * 0.13).rgb * an.y + texture2D(uDetail, vDW.xy * 0.13).rgb * an.z;
        diffuseColor.rgb *= 1.0 + ((tx.r - 0.5) * 0.8 + (tl.g - 0.5) * 0.7) * ${strength.toFixed(2)};`);
  };
  m.customProgramCacheKey = () => 'detailed-' + strength;
  return m;
}
