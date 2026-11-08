// Procedural characters. Bodies are smooth skinned sweeps (blended weights at
// elbows, knees and spine), heads carry a painted anime face, hair is built
// from tapered strands, and clothing layers on top: tunics and skirts that
// swing with the legs, capes, scarves, straps and armour. One skinned mesh per
// character (plus an outline and the face decal that share its skeleton).

import * as THREE from 'three';
import { mergeGeometries } from 'three/addons';
import { damp, clamp, lerp, smoothstep } from '../core/util.js';
import { tube, strand, bindRigid, paintFace } from './body.js';

// ------------------------------------------------------------------ materials
let toonRamp = null;
export function getToonRamp() {
  if (toonRamp) return toonRamp;
  const v = [102, 150, 196, 232];
  const data = new Uint8Array(v.length * 4);
  v.forEach((x, i) => { data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = x; data[i * 4 + 3] = 255; });
  toonRamp = new THREE.DataTexture(data, v.length, 1, THREE.RGBAFormat);
  toonRamp.minFilter = toonRamp.magFilter = THREE.NearestFilter;
  toonRamp.generateMipmaps = false;
  toonRamp.needsUpdate = true;
  return toonRamp;
}

// Toon shading with a soft rim light and optional per-vertex glow (aGlow).
export function makeToonMaterial(opts = {}) {
  const { rim: _r, ...rest } = opts;
  const m = new THREE.MeshToonMaterial({ vertexColors: true, gradientMap: getToonRamp(), ...rest });
  const rim = opts.rim ?? 0.22;
  m.onBeforeCompile = (sh) => {
    sh.vertexShader = sh.vertexShader
      .replace('#include <common>', '#include <common>\nattribute float aGlow;\nvarying float vGlow;')
      .replace('#include <color_vertex>', '#include <color_vertex>\nvGlow = aGlow;');
    sh.fragmentShader = sh.fragmentShader
      .replace('#include <common>', '#include <common>\nvarying float vGlow;')
      .replace('#include <emissivemap_fragment>', `#include <emissivemap_fragment>
        float rimK = pow(1.0 - clamp(dot(normal, normalize(vViewPosition)), 0.0, 1.0), 3.0);
        totalEmissiveRadiance += diffuseColor.rgb * (rimK * ${rim.toFixed(2)} + vGlow * 1.7);`);
  };
  m.customProgramCacheKey = () => 'toon-rim-' + rim;
  return m;
}

export function makeOutlineMaterial(width = 0.009, color = 0x221a2c) {
  const m = new THREE.MeshBasicMaterial({ color, side: THREE.BackSide });
  m.onBeforeCompile = (sh) => {
    sh.uniforms.uWidth = { value: width };
    sh.vertexShader = sh.vertexShader
      .replace('#include <common>', '#include <common>\nuniform float uWidth;')
      .replace('#include <skinning_vertex>', `#include <skinning_vertex>
        #ifdef USE_SKINNING
          transformed += normalize(objectNormal) * uWidth;
        #else
          transformed += normalize(normal) * uWidth;
        #endif`);
  };
  m.customProgramCacheKey = () => 'outline' + width;
  return m;
}

// ------------------------------------------------------------------ skeleton
// Bind pose in metres, facing +Z; the character's right side is -X.
const BONES = [
  ['hips', null, [0, 0.95, 0]],
  ['spine', 'hips', [0, 0.12, 0]],
  ['chest', 'spine', [0, 0.2, 0]],
  ['neck', 'chest', [0, 0.22, 0]],
  ['head', 'neck', [0, 0.08, 0]],
  ['eyes', 'head', [0, 0.13, 0.12]],
  ['armL', 'chest', [0.2, 0.15, 0]],
  ['forearmL', 'armL', [0, -0.27, 0]],
  ['handL', 'forearmL', [0, -0.25, 0]],
  ['armR', 'chest', [-0.2, 0.15, 0]],
  ['forearmR', 'armR', [0, -0.27, 0]],
  ['handR', 'forearmR', [0, -0.25, 0]],
  ['thighL', 'hips', [0.1, -0.04, 0]],
  ['shinL', 'thighL', [0, -0.42, 0]],
  ['footL', 'shinL', [0, -0.42, 0]],
  ['thighR', 'hips', [-0.1, -0.04, 0]],
  ['shinR', 'thighR', [0, -0.42, 0]],
  ['footR', 'shinR', [0, -0.42, 0]],
  ['cape', 'chest', [0, 0.2, -0.16]],
  ['hair', 'head', [0, 0.2, -0.13]],
  ['skirtF', 'hips', [0, 0.04, 0.12]],
  ['skirtB', 'hips', [0, 0.04, -0.12]],
];
const IDX = Object.fromEntries(BONES.map((b, i) => [b[0], i]));
const BIND = {};
for (const [name, parent, p] of BONES) {
  const v = new THREE.Vector3(...p);
  if (parent) v.add(BIND[parent]);
  BIND[name] = v;
}

// ------------------------------------------------------------------ geometry helpers
const C = (hex) => new THREE.Color(hex);

function colorGeo(geo, color, glow = 0) {
  const g = geo.index ? geo.toNonIndexed() : geo;
  for (const k of Object.keys(g.attributes)) if (k !== 'position' && k !== 'normal') g.deleteAttribute(k);
  if (!g.attributes.normal) g.computeVertexNormals();
  const n = g.attributes.position.count;
  const col = new Float32Array(n * 3);
  const fn = typeof color === 'function' ? color : null;
  const c = fn ? new THREE.Color() : C(color);
  for (let i = 0; i < n; i++) {
    if (fn) c.set(fn(g.attributes.position.getX(i), g.attributes.position.getY(i), g.attributes.position.getZ(i), g.attributes.normal.getX(i), g.attributes.normal.getY(i), g.attributes.normal.getZ(i)));
    col[i * 3] = c.r; col[i * 3 + 1] = c.g; col[i * 3 + 2] = c.b;
  }
  g.setAttribute('color', new THREE.BufferAttribute(col, 3));
  if (glow) g.setAttribute('aGlow', new THREE.BufferAttribute(new Float32Array(n).fill(glow), 1));
  return g;
}

const tf = (geo, x = 0, y = 0, z = 0, rx = 0, ry = 0, rz = 0, sx = 1, sy = 1, sz = 1) => {
  const m = new THREE.Matrix4().compose(new THREE.Vector3(x, y, z), new THREE.Quaternion().setFromEuler(new THREE.Euler(rx, ry, rz)), new THREE.Vector3(sx, sy, sz));
  geo.applyMatrix4(m);
  return geo;
};
const capsule = (r, len, col, seg = 10) => colorGeo(new THREE.CapsuleGeometry(r, len, 4, seg), col);
const sph = (r, col, ws = 14, hs = 10, glow = 0) => colorGeo(new THREE.SphereGeometry(r, ws, hs), col, glow);
const hemi = (r, col, ws = 14, hs = 6) => colorGeo(new THREE.SphereGeometry(r, ws, hs, 0, Math.PI * 2, 0, Math.PI / 2), col);
const cylg = (rt, rb, h, col, seg = 12, open = false) => colorGeo(new THREE.CylinderGeometry(rt, rb, h, seg, 1, open), col);
const coneg = (r, h, col, seg = 8) => colorGeo(new THREE.ConeGeometry(r, h, seg), col);
const boxg = (w, h, d, col, glow = 0) => colorGeo(new THREE.BoxGeometry(w, h, d), col, glow);
const torusg = (r, t, col, rs = 8, ts = 20) => colorGeo(new THREE.TorusGeometry(r, t, rs, ts), col);
// A chunky, chiselled rock (for golems): subdivided box with consistent jitter.
function rockg(w, h, d, col, seed = 1, glow = 0) {
  const g = new THREE.BoxGeometry(w, h, d, 2, 2, 2);
  const p = g.attributes.position;
  for (let i = 0; i < p.count; i++) {
    const x = p.getX(i), y = p.getY(i), z = p.getZ(i);
    const h1 = Math.sin(x * 91.7 + y * 37.1 + z * 13.3 + seed * 7.1) * 0.5;
    const h2 = Math.sin(x * 17.3 - y * 71.9 + z * 43.7 + seed * 3.3) * 0.5;
    const k = Math.min(w, h, d) * 0.12;
    p.setXYZ(i, x * (1 + h1 * 0.08) + h2 * k * 0.3, y + h1 * k * 0.4, z * (1 + h2 * 0.08));
  }
  g.computeVertexNormals();
  return colorGeo(g, (x, y, z, nx, ny) => (ny > 0.6 && col.moss ? col.moss : col.base ?? col), glow);
}
const smooth = (geo) => geo; // normals already smooth for parametric shapes

export { BONES, IDX as BONE_INDEX, colorGeo, tf, capsule, sph, cylg, coneg, boxg, torusg, rockg };

// Piecewise-smooth interpolation through [s, a, b, ...] keys.
function keys(table, s) {
  if (s <= table[0][0]) return table[0].slice(1);
  for (let i = 0; i < table.length - 1; i++) {
    const a = table[i], b = table[i + 1];
    if (s <= b[0]) {
      const t = smoothstep(a[0], b[0], s);
      return a.slice(1).map((v, k) => lerp(v, b[k + 1] ?? v, t));
    }
  }
  return table[table.length - 1].slice(1);
}

export const DEFAULT_LOOK = {
  skin: 0xeec6a6, hair: 0x2e2630, hairStyle: 'spiky', eyes: 0x3a6ad8,
  top: 0x2f5fa8, trim: 0xe8d8b0, pants: 0x3a3440, boots: 0x5a3a28, belt: 0x6a4a2a,
  scarf: 0xc8453a, cape: null, pauldrons: true, hat: null, beard: false, female: false,
  skirt: false, apron: null, build: 1, height: 1, gloves: 0x4a3a30, strap: true,
};

// The head is a slightly tall sphere with a narrowing jaw and a soft chin.
const HEAD_R = 0.128;
const HEAD_C = new THREE.Vector3(0, 0.122, 0.006); // relative to the head bone
function shapeHead(geo, r) {
  const p = geo.attributes.position;
  for (let i = 0; i < p.count; i++) {
    let x = p.getX(i), y = p.getY(i), z = p.getZ(i);
    const ny = y / r;
    const k = smoothstep(0.05, -0.95, ny);
    x *= 1 - 0.3 * k;
    z *= 1 - 0.1 * k;
    if (z > 0) { z += 0.06 * r * k * (z / r); y -= 0.1 * r * k * Math.max(0, z / r) * (1 - Math.min(1, Math.abs(x) / r)); }
    y *= 1.08;
    if (z < 0) z *= 1.04;
    p.setXYZ(i, x, y, z);
  }
  geo.computeVertexNormals();
  return geo;
}

// ------------------------------------------------------------------ humanoid body
function buildHumanoid(L) {
  const P = [];
  const add = (bone, geo) => P.push([bone, geo]);
  const pre = (geo) => P.push(['@', geo]);
  const v = (x, y, z) => new THREE.Vector3(x, y, z);
  const fem = L.female;
  const top = L.top, trim = L.trim, skin = L.skin;
  const gloveCol = L.gloves ?? skin;
  const sh = (c, k) => new THREE.Color(c).multiplyScalar(k);

  // ---- torso
  const torsoK = fem
    ? [[0, 0.125, 0.1], [0.18, 0.16, 0.112], [0.42, 0.105, 0.084], [0.62, 0.138, 0.098], [0.8, 0.152, 0.1], [1, 0.11, 0.08]]
    : [[0, 0.13, 0.1], [0.18, 0.15, 0.108], [0.42, 0.125, 0.09], [0.65, 0.165, 0.112], [0.84, 0.19, 0.108], [1, 0.13, 0.08]];
  pre(tube([
    { p: v(0, 0.8, 0), bone: IDX.hips }, { p: v(0, 0.98, 0), bone: IDX.spine }, { p: v(0, 1.13, 0), bone: IDX.chest },
    { p: v(0, 1.32, 0), bone: IDX.chest }, { p: v(0, 1.47, 0), bone: IDX.chest },
  ], (s, a) => {
    const [rx, rz] = keys(torsoK, s);
    const front = Math.max(0, Math.sin(a));
    const bust = fem ? 0.03 * front * front * Math.exp(-Math.pow((s - 0.66) / 0.1, 2)) : 0.008 * front * Math.exp(-Math.pow((s - 0.72) / 0.12, 2));
    const back = Math.max(0, -Math.sin(a));
    return { rx, rz: rz + bust + 0.006 * back * Math.exp(-Math.pow((s - 0.8) / 0.1, 2)) };
  }, (s, a) => {
    if (s < 0.14) return L.pants;
    if (s > 0.3 && s < 0.37) return L.belt;
    if (s > 0.94) return trim;
    if (L.trimStripe !== false && Math.abs(a - Math.PI / 2) < 0.15 && s > 0.37) return trim;
    return s > 0.6 ? top : sh(top, 0.94);
  }, { radial: 18, rings: 5, capStart: true }));

  // ---- tunic or dress skirt (front/back panels swing with the legs)
  const hem = L.skirt ? 0.12 : fem ? 0.56 : 0.6;
  const flare = L.skirt ? 0.16 : 0.07;
  const skirtGeo = tube([{ p: v(0, 1.0, 0), bone: IDX.hips }, { p: v(0, hem, 0), bone: IDX.hips }],
    (s, a) => ({ rx: 0.155 + s * flare + (fem ? 0.008 : 0), rz: 0.118 + s * flare * 0.8 }),
    (s, a) => {
      if (L.apron && Math.sin(a) > 0.55) return s > 0.94 ? sh(L.apron, 0.85) : L.apron;
      if (s > 0.9) return trim;
      if (L.trimStripe !== false && Math.abs(a - Math.PI / 2) < 0.12) return trim;
      return sh(top, 0.92 - s * 0.08);
    }, { radial: 18, rings: L.skirt ? 7 : 4 });
  // Re-weight: lower front to skirtF, lower back to skirtB.
  {
    const p = skirtGeo.attributes.position, si = skirtGeo.attributes.skinIndex, sw = skirtGeo.attributes.skinWeight;
    for (let i = 0; i < p.count; i++) {
      const y = p.getY(i), z = p.getZ(i);
      const t = clamp((1.0 - y) / (1.0 - hem), 0, 1);
      const w = smoothstep(0.05, 0.9, t);
      const front = clamp(z / 0.08, -1, 1);
      si.setXYZW(i, IDX.hips, front >= 0 ? IDX.skirtF : IDX.skirtB, 0, 0);
      sw.setXYZW(i, 1 - w * Math.abs(front), w * Math.abs(front), 0, 0);
    }
  }
  pre(skirtGeo);

  // ---- neck & head
  pre(tube([{ p: v(0, 1.43, 0), bone: IDX.neck }, { p: v(0, 1.6, 0), bone: IDX.head }], () => ({ rx: fem ? 0.04 : 0.047, rz: fem ? 0.038 : 0.044 }), () => skin, { radial: 10, rings: 3 }));
  const headGeo = shapeHead(new THREE.SphereGeometry(HEAD_R, 24, 18), HEAD_R);
  add('head', tf(colorGeo(headGeo, skin), HEAD_C.x, HEAD_C.y, HEAD_C.z));
  for (const s of [1, -1]) add('head', tf(sph(0.026, skin, 8, 6), s * HEAD_R * 0.97, HEAD_C.y - 0.01, HEAD_C.z - 0.01, 0, s * 0.3, 0, 0.45, 1, 0.8));
  add('head', tf(sph(0.011, sh(skin, 0.97), 6, 4), 0, HEAD_C.y - 0.035, HEAD_C.z + HEAD_R * 0.99));

  // ---- arms & hands
  for (const sx of [1, -1]) {
    const S = sx > 0 ? 'L' : 'R';
    const k = fem ? 0.86 : 1;
    const armK = [[0, 0.064 * k], [0.12, 0.058 * k], [0.45, 0.044 * k], [0.55, 0.045 * k], [0.72, 0.043 * k], [1, 0.034 * k]];
    const sleeve = L.longSleeves ? 0.9 : L.skirt ? 0.8 : 0.42;
    pre(tube([
      { p: BIND['arm' + S].clone(), bone: IDX['arm' + S] }, { p: BIND['forearm' + S].clone(), bone: IDX['forearm' + S] }, { p: BIND['hand' + S].clone(), bone: IDX['forearm' + S] },
    ], (s) => { const [r] = keys(armK, s); return { rx: r, rz: r * 0.95 }; }, (s) => {
      if (s < sleeve) return s > sleeve - 0.05 ? trim : top;
      if (L.gloves && s > 0.76) return s < 0.8 ? sh(gloveCol, 1.15) : gloveCol;
      return skin;
    }, { radial: 12, rings: 5, capEnd: true }));
    add('arm' + S, tf(sph(0.07 * k, top, 14, 10), sx * 0.005, -0.005, 0));
    // Hand: palm, fingers block and thumb.
    add('hand' + S, tf(sph(0.04 * k, gloveCol, 10, 8), 0, -0.045, 0.004, 0, 0, 0, 0.85, 1.25, 0.62));
    add('hand' + S, tf(capsule(0.022 * k, 0.03, gloveCol, 8), 0, -0.085, 0.006, 0, 0, 0, 1.35, 1, 0.7));
    add('hand' + S, tf(capsule(0.012 * k, 0.03, gloveCol, 6), -sx * 0.014, -0.04, 0.028, 0.5, 0, sx * 0.4));
    if (L.pauldrons) {
      add('arm' + S, tf(hemi(0.095, trim), sx * 0.015, 0.012, 0, 0, 0, -sx * 0.35, 1.05, 0.62, 1.05));
      add('arm' + S, tf(hemi(0.088, sh(trim, 0.8)), sx * 0.03, -0.035, 0, 0, 0, -sx * 0.55, 1.0, 0.55, 1.0));
    }
  }

  // ---- legs & boots
  for (const sx of [1, -1]) {
    const S = sx > 0 ? 'L' : 'R';
    const legK = [[0, fem ? 0.09 : 0.088], [0.15, 0.084], [0.46, 0.055], [0.62, 0.059, -0.012], [0.86, 0.046, -0.004], [1, 0.043]];
    const bootTop = L.skirt ? 0.7 : 0.6;
    pre(tube([
      { p: BIND['thigh' + S].clone().add(v(0, 0.03, 0)), bone: IDX['thigh' + S] }, { p: BIND['shin' + S].clone(), bone: IDX['shin' + S] },
      { p: BIND['foot' + S].clone().add(v(0, 0.04, 0)), bone: IDX['shin' + S] },
    ], (s) => { const [r, oz] = keys(legK, s); return { rx: r, rz: r, oz: oz ?? 0 }; }, (s) => (s > bootTop ? L.boots : L.pants), { radial: 12, rings: 5 }));
    // Boot cuff and foot.
    add('shin' + S, tf(cylg(0.066, 0.06, 0.055, sh(L.boots, 1.18), 14), 0, -0.05, -0.004));
    add('foot' + S, tf(sph(0.05, L.boots, 12, 8), 0, -0.012, 0.045, 0, 0, 0, 1.05, 0.85, 2.1));
    add('foot' + S, tf(boxg(0.098, 0.022, 0.235, 0x2a2018), 0, -0.052, 0.045));
  }

  // ---- belt, straps, collar, scarf, cape
  add('hips', tf(torusg(0.135, 0.016, L.belt, 6, 24), 0, 0.075, 0, Math.PI / 2, 0, 0, 1, 0.8, 1));
  add('hips', tf(boxg(0.05, 0.045, 0.02, 0xe6b84a), 0, 0.075, 0.108));
  add('hips', tf(boxg(0.075, 0.07, 0.045, sh(L.belt, 0.85)), 0.12, 0.04, -0.06, 0, 0.5, 0));
  if (L.strap) {
    pre(strand(v(0.13, 1.43, 0.085), v(0.0, 1.24, 0.15), v(-0.14, 1.03, 0.13), 0.02, { root: L.belt, tip: L.belt, thick: 0.35, taper: 0, bone: IDX.chest, segs: 6, radial: 4, center: v(0, 1.2, 0) }));
    pre(strand(v(0.13, 1.43, -0.085), v(0.0, 1.24, -0.135), v(-0.14, 1.03, -0.115), 0.02, { root: L.belt, tip: L.belt, thick: 0.35, taper: 0, bone: IDX.chest, segs: 6, radial: 4, center: v(0, 1.2, 0) }));
  }
  add('chest', tf(torusg(0.066, 0.016, trim, 6, 18), 0, 0.2, 0.004, Math.PI / 2, 0, 0));
  if (L.scarf) {
    add('neck', tf(torusg(0.07, 0.03, L.scarf, 8, 20), 0, 0.0, 0.005, Math.PI / 2 + 0.15, 0, 0, 1, 1, 0.9));
    for (const sx of [1, -1]) {
      pre(strand(v(sx * 0.03, 1.47, -0.07), v(sx * 0.06, 1.3, -0.2), v(sx * 0.08, 1.05, -0.24), 0.035, { root: L.scarf, tip: sh(L.scarf, 0.8), thick: 0.25, taper: 0.4, bone: IDX.chest, bone2: IDX.cape, center: v(0, 1.3, 0) }));
    }
  }
  if (L.cape) {
    pre(strand(v(0, 1.45, -0.1), v(0, 1.05, -0.2), v(0, 0.48, -0.26), 0.2, { root: L.cape, tip: sh(L.cape, 0.75), thick: 0.12, taper: -0.4, bone: IDX.chest, bone2: IDX.cape, center: v(0, 1.0, 0.05), segs: 8, radial: 6 }));
  }

  // ---- hair
  buildHair(L, pre, add);

  // ---- hats & beards
  const hc = HEAD_C, R = HEAD_R;
  if (L.hat === 'wizard') {
    const hcol = L.hatColor ?? 0x3a3a7a;
    add('head', tf(cylg(0.25, 0.25, 0.018, hcol, 24), hc.x, hc.y + R * 0.92, hc.z - 0.01, -0.08, 0, 0));
    const base = BIND.head.clone().add(new THREE.Vector3(0, hc.y + R * 0.92, hc.z - 0.02));
    pre(strand(base, base.clone().add(v(0, 0.22, -0.04)), base.clone().add(v(0, 0.36, -0.18)), 0.15, { root: hcol, tip: sh(hcol, 0.8), thick: 1, taper: 0.97, bone: IDX.head, segs: 8, radial: 12, center: base.clone().add(v(0, 0.1, 0.1)) }));
    add('head', tf(torusg(0.148, 0.014, L.trim ?? 0xe6c87a, 6, 24), hc.x, hc.y + R * 0.95, hc.z - 0.015, Math.PI / 2 - 0.08, 0, 0));
  } else if (L.hat === 'hood') {
    const hcol = L.hatColor ?? 0x7a2a2a;
    add('head', tf(colorGeo(new THREE.SphereGeometry(R * 1.25, 18, 12, 0, Math.PI * 2, 0, Math.PI * 0.62), hcol), hc.x, hc.y, hc.z - 0.015, -0.32, 0, 0, 1, 1.12, 1.12));
    add('head', tf(boxg(0.2, 0.07, 0.04, 0x2a1a1a), hc.x, hc.y - 0.07, hc.z + R * 0.88));
    add('chest', tf(colorGeo(new THREE.CylinderGeometry(0.11, 0.21, 0.14, 16, 1, true), hcol), 0, 0.18, -0.01));
  } else if (L.hat === 'cap') {
    const hcol = L.hatColor ?? 0x6a4a2a;
    add('head', tf(hemi(R * 1.12, hcol), hc.x, hc.y + 0.02, hc.z, -0.15, 0, 0, 1, 0.75, 1.02));
    add('head', tf(cylg(0.2, 0.2, 0.014, hcol, 20), hc.x, hc.y + 0.035, hc.z + 0.01, -0.1, 0, 0));
  } else if (L.hat === 'helm') {
    add('head', tf(hemi(R * 1.18, 0x9aa0aa), hc.x, hc.y + 0.0, hc.z, -0.1, 0, 0, 1, 1.05, 1.05));
    add('head', tf(boxg(0.028, 0.11, 0.02, 0x8a909a), hc.x, hc.y - 0.0, hc.z + R * 1.05));
    add('head', tf(boxg(0.03, 0.06, 0.2, L.trim ?? 0x2f5fa8), hc.x, hc.y + R * 1.12, hc.z - 0.02));
  }
  if (L.beard) {
    const bc = L.beardColor ?? L.hair;
    const hb = BIND.head.clone().add(hc);
    for (let i = 0; i < 7; i++) {
      const a = (i - 3) * 0.32;
      const root = hb.clone().add(v(Math.sin(a) * R * 0.85, -R * 0.35, Math.cos(a) * R * 0.75));
      pre(strand(root, root.clone().add(v(Math.sin(a) * 0.02, -0.05, 0.03)), root.clone().add(v(Math.sin(a) * 0.01, -0.11 + Math.abs(a) * 0.04, 0.02)), 0.035, { root: bc, tip: new THREE.Color(bc).multiplyScalar(1.15), thick: 0.6, bone: IDX.head, center: hb, segs: 4, radial: 5 }));
    }
  }
  return P;
}

// Hairstyles are built from tapered strands rooted on the scalp.
function buildHair(L, pre, add) {
  const style = L.hairStyle;
  if (style === 'bald') return;
  const covered = L.hat === 'hood' || L.hat === 'helm';
  const hcol = new THREE.Color(L.hair);
  const root = hcol.clone().multiplyScalar(0.72), tip = hcol.clone().lerp(new THREE.Color(0xffffff), 0.12), shine = hcol.clone().lerp(new THREE.Color(0xffffff), 0.5);
  const R = HEAD_R, hb = BIND.head.clone().add(HEAD_C);
  const scalp = (yaw, el, k = 1.0) => hb.clone().add(new THREE.Vector3(Math.sin(yaw) * Math.cos(el), Math.sin(el) * 1.08, Math.cos(yaw) * Math.cos(el)).multiplyScalar(R * k));
  const out = (yaw, el) => new THREE.Vector3(Math.sin(yaw) * Math.cos(el), Math.sin(el), Math.cos(yaw) * Math.cos(el)).normalize();
  const S = (yaw, el, dir, len, w, bend, o = {}) => {
    const r0 = scalp(yaw, el, 0.98);
    const d = dir.clone().normalize();
    const tipP = r0.clone().addScaledVector(d, len).add(bend);
    const ctrl = r0.clone().addScaledVector(d, len * 0.55).addScaledVector(out(yaw, el), len * 0.18);
    pre(strand(r0, ctrl, tipP, w, { root, tip, shine: o.shine ? shine : null, thick: o.thick ?? 0.42, bone: o.bone ?? IDX.head, bone2: o.bone2, center: hb, segs: o.segs ?? 6, radial: 6 }));
  };
  const up = new THREE.Vector3(0, 1, 0), down = new THREE.Vector3(0, -1, 0);
  if (!covered) {
    // Hair base: covers the whole skull down to the nape, with the face
    // and a hairline cut out at the front (those vertices tuck inside the head).
    const rr = R * 1.055;
    const capG = new THREE.SphereGeometry(rr, 28, 18, 0, Math.PI * 2, 0, Math.PI * 0.82);
    const p = capG.attributes.position;
    for (let i = 0; i < p.count; i++) {
      const x = p.getX(i) / rr, y = p.getY(i) / rr, z = p.getZ(i) / rr;
      const hairline = 0.5 + 0.18 * Math.abs(x) - (style === 'bob' || style === 'long' || style === 'twin' ? 0.12 : 0);
      const face = z > 0.18 && y < hairline && Math.abs(x) < 0.82;
      const ear = Math.abs(x) > 0.82 && y < 0.05 && y > -0.45 && z > -0.2 && style !== 'long' && style !== 'bob';
      if (face || ear) p.setXYZ(i, x * rr * 0.86, y * rr * 0.86, z * rr * 0.86);
    }
    capG.computeVertexNormals();
    shapeHead(capG, rr);
    const capC = colorGeo(capG, (x, y) => hcol.clone().multiplyScalar(0.88 + 0.12 * clamp(y / rr, 0, 1)));
    add('head', tf(capC, HEAD_C.x, HEAD_C.y + 0.004, HEAD_C.z - 0.006));
  }
  const bangs = (n, len, spread = 0.75, w = 0.045) => {
    for (let i = 0; i < n; i++) {
      const t = n === 1 ? 0 : i / (n - 1) - 0.5;
      const yaw = t * spread * 2;
      const dir = new THREE.Vector3(Math.sin(yaw) * 0.6 + t * 0.4, -1, 0.45).normalize();
      S(yaw, 0.75, dir, len * (1 - Math.abs(t) * 0.3), w, new THREE.Vector3(t * 0.04, 0, 0.02), { shine: true });
    }
  };
  if (covered) { bangs(3, 0.07, 0.5, 0.04); return; }
  if (style === 'spiky') {
    for (let i = 0; i < 9; i++) {
      const t = i / 8 - 0.5;
      const yaw = Math.PI + t * 2.4;
      const dir = out(yaw, 0.6).add(new THREE.Vector3(0, 0.45, 0)).add(new THREE.Vector3(Math.sin(yaw), 0, Math.cos(yaw)).multiplyScalar(0.55));
      S(yaw, 0.85 - Math.abs(t) * 0.5, dir, 0.16 + (0.5 - Math.abs(t)) * 0.1, 0.058, new THREE.Vector3(0, 0.06, 0), { shine: true });
    }
    for (let i = 0; i < 5; i++) {
      const yaw = Math.PI + (i / 4 - 0.5) * 1.6;
      S(yaw, 0.05, out(yaw, -0.3).add(new THREE.Vector3(0, -1.2, 0)), 0.1, 0.05, new THREE.Vector3(0, 0, -0.015));
    }
    for (const sx of [-1, 1]) {
      S(sx * 0.5, 1.15, out(sx * 0.5, 1.0).add(up), 0.15, 0.05, new THREE.Vector3(sx * 0.03, 0.02, -0.04), { shine: true });
      S(sx * 1.45, 0.45, out(sx * 1.45, 0).add(down.clone().multiplyScalar(1.2)), 0.11, 0.04, new THREE.Vector3(sx * 0.02, 0, 0.01));
      S(sx * 1.9, 0.35, out(sx * 1.9, 0).add(down), 0.1, 0.04, new THREE.Vector3(sx * 0.02, 0, -0.01));
    }
    bangs(5, 0.15);
  } else if (style === 'short') {
    for (let i = 0; i < 8; i++) {
      const t = i / 7 - 0.5;
      const yaw = Math.PI + t * 2.6;
      S(yaw, 0.7, out(yaw, 0.2).add(new THREE.Vector3(0, -0.4, 0)), 0.07, 0.05, new THREE.Vector3(0, 0, 0));
    }
    bangs(4, 0.08, 0.6, 0.05);
  } else {
    // Bob, long, ponytail and twin tails share a fuller base.
    for (let i = 0; i < 11; i++) {
      const t = i / 10 - 0.5;
      const yaw = Math.PI + t * 4.2;
      const len = style === 'ponytail' ? 0.1 : 0.2;
      S(yaw, 0.55, out(yaw, -0.2).add(down.clone().multiplyScalar(1.4)), len, 0.06, new THREE.Vector3(0, -0.01, 0), { shine: true });
    }
    bangs(5, 0.13, 0.85);
    if (style === 'long') {
      for (let i = 0; i < 9; i++) {
        const t = i / 8 - 0.5;
        const yaw = Math.PI + t * 1.8;
        const r0 = scalp(yaw, 0.2, 0.98);
        pre(strand(r0, r0.clone().add(new THREE.Vector3(Math.sin(yaw) * 0.05, -0.18, -0.05)), r0.clone().add(new THREE.Vector3(Math.sin(yaw) * 0.04, -0.46, -0.07)), 0.055, { root, tip, thick: 0.4, bone: IDX.head, bone2: IDX.hair, center: hb.clone().add(new THREE.Vector3(0, -0.2, 0.05)), segs: 7, radial: 6 }));
      }
    }
    if (style === 'ponytail') {
      const base = scalp(Math.PI, 0.55, 1.05);
      add('head', tf(torusg(0.03, 0.012, L.ribbon ?? 0xd84a6a, 6, 12), base.x - BIND.head.x, base.y - BIND.head.y, base.z - BIND.head.z, 0.3, 0, 0));
      for (let i = 0; i < 6; i++) {
        const a = (i / 6) * Math.PI * 2;
        const off = new THREE.Vector3(Math.cos(a) * 0.015, Math.sin(a) * 0.015, 0);
        const r0 = base.clone().add(off);
        pre(strand(r0, r0.clone().add(new THREE.Vector3(off.x, -0.06, -0.12)), r0.clone().add(new THREE.Vector3(off.x * 2, -0.36, -0.1)), 0.045, { root, tip, shine: shine, thick: 0.6, bone: IDX.head, bone2: IDX.hair, center: base.clone().add(new THREE.Vector3(0, -0.15, -0.02)), segs: 7, radial: 6 }));
      }
    }
    if (style === 'twin') {
      for (const sx of [-1, 1]) {
        const base = scalp(sx * 1.7, 0.6, 1.05);
        add('head', tf(sph(0.03, L.ribbon ?? 0xd84a6a, 8, 6), base.x - BIND.head.x, base.y - BIND.head.y, base.z - BIND.head.z));
        for (let i = 0; i < 4; i++) {
          const off = new THREE.Vector3(0, 0, (i - 1.5) * 0.012);
          const r0 = base.clone().add(off);
          pre(strand(r0, r0.clone().add(new THREE.Vector3(sx * 0.1, -0.05, -0.02)), r0.clone().add(new THREE.Vector3(sx * 0.12, -0.32, -0.04)), 0.04, { root, tip, thick: 0.6, bone: IDX.head, bone2: IDX.hair, center: base.clone().add(new THREE.Vector3(-sx * 0.1, -0.1, 0)), segs: 7, radial: 6 }));
        }
      }
    }
  }
}

// ------------------------------------------------------------------ golem & revenant
function golemParts(L) {
  const st = L.stone ?? 0x8a8478, dk = L.stoneDark ?? 0x6a655c, glow = L.glow ?? 0x7dff9a, moss = L.moss ?? 0x6f9a4a;
  const P = [];
  const add = (b, g) => P.push([b, g]);
  const R = (w, h, d, c, s) => rockg(w, h, d, { base: c, moss }, s);
  add('hips', R(0.44, 0.24, 0.32, dk, 1));
  add('spine', tf(R(0.4, 0.28, 0.3, st, 2), 0, 0.1, 0));
  add('chest', tf(R(0.66, 0.46, 0.44, st, 3), 0, 0.13, -0.01));
  add('chest', tf(R(0.3, 0.2, 0.3, st, 31), 0.22, 0.3, -0.02));
  add('chest', tf(R(0.3, 0.2, 0.3, st, 32), -0.22, 0.3, -0.02));
  add('chest', tf(colorGeo(new THREE.OctahedronGeometry(0.1, 0), glow, 1), 0, 0.14, 0.22, 0, 0, 0, 1, 1.4, 0.5));
  for (let i = 0; i < 3; i++) add('chest', tf(boxg(0.015, 0.16 - i * 0.03, 0.01, glow, 1), -0.14 + i * 0.14, 0.18, 0.222));
  add('neck', tf(R(0.18, 0.1, 0.18, dk, 4), 0, 0.03, 0));
  add('head', tf(R(0.26, 0.24, 0.26, st, 5), 0, 0.1, 0.02));
  add('head', tf(boxg(0.17, 0.035, 0.02, glow, 1), 0, 0.09, 0.15));
  for (const sx of [1, -1]) {
    const S = sx > 0 ? 'L' : 'R';
    add('arm' + S, tf(R(0.26, 0.24, 0.26, st, 6), sx * 0.03, 0.02, 0));
    add('arm' + S, tf(R(0.17, 0.28, 0.17, dk, 7), 0, -0.14, 0));
    add('forearm' + S, tf(R(0.21, 0.27, 0.21, st, 8), 0, -0.12, 0));
    add('forearm' + S, tf(boxg(0.012, 0.18, 0.01, glow, 1), sx * 0.0, -0.12, 0.107));
    add('hand' + S, tf(R(0.25, 0.21, 0.23, dk, 9), 0, -0.07, 0));
    add('thigh' + S, tf(R(0.21, 0.42, 0.21, st, 10), 0, -0.2, 0));
    add('shin' + S, tf(R(0.19, 0.4, 0.19, dk, 11), 0, -0.2, 0));
    add('foot' + S, tf(R(0.23, 0.11, 0.31, st, 12), 0, -0.02, 0.04));
  }
  return P;
}

// Bramble Sentinel: a walking tree. Bark limbs, a leafy crown, ember eyes.
function treantParts(L) {
  const bark = { base: L.bark ?? 0x6a4a32, moss: L.moss ?? 0x5a7a3a };
  const dark = { base: 0x4a3222, moss: L.moss ?? 0x5a7a3a };
  const leaf = new THREE.Color(L.leaf ?? 0x4f8a3a), leafHi = new THREE.Color(L.leafHi ?? 0x8ac25a);
  const glow = L.glow ?? 0xffc84a;
  const P = [];
  const add = (b, g) => P.push([b, g]);
  const v = (x, y, z) => new THREE.Vector3(x, y, z);
  add('hips', rockg(0.4, 0.26, 0.3, dark, 21));
  add('spine', tf(rockg(0.34, 0.3, 0.28, bark, 22), 0, 0.12, 0));
  add('chest', tf(rockg(0.56, 0.5, 0.4, bark, 23), 0, 0.15, -0.01));
  add('chest', tf(colorGeo(new THREE.OctahedronGeometry(0.08, 0), glow, 1), 0, 0.12, 0.2, 0, 0, 0, 1, 1.5, 0.5));
  add('head', tf(rockg(0.26, 0.3, 0.24, bark, 24), 0, 0.12, 0.02));
  for (const s of [1, -1]) add('head', tf(boxg(0.06, 0.025, 0.02, glow, 1), s * 0.06, 0.13, 0.135, 0, 0, s * 0.25));
  // Leafy crown and shoulder foliage.
  const hb = BIND.head.clone().add(v(0, 0.25, 0));
  for (let i = 0; i < 9; i++) {
    const a = (i / 9) * Math.PI * 2;
    const r0 = hb.clone().add(v(Math.cos(a) * 0.1, 0, Math.sin(a) * 0.1));
    P.push(['@', strand(r0, r0.clone().add(v(Math.cos(a) * 0.15, 0.15, Math.sin(a) * 0.15)), r0.clone().add(v(Math.cos(a) * 0.32, 0.1, Math.sin(a) * 0.32)), 0.12, { root: leaf, tip: leafHi, thick: 0.35, taper: 0.6, bone: IDX.head, center: hb.clone().add(v(0, -0.2, 0)), segs: 4, radial: 6 })]);
  }
  for (const sx of [1, -1]) {
    const S = sx > 0 ? 'L' : 'R';
    add('arm' + S, tf(rockg(0.24, 0.22, 0.24, bark, 25), sx * 0.03, 0.02, 0));
    add('arm' + S, tf(rockg(0.15, 0.3, 0.15, dark, 26), 0, -0.15, 0));
    add('forearm' + S, tf(rockg(0.17, 0.3, 0.17, bark, 27), 0, -0.14, 0));
    for (let k = 0; k < 3; k++) add('hand' + S, tf(colorGeo(new THREE.ConeGeometry(0.035, 0.22, 5), 0x4a3222), (k - 1) * 0.06, -0.12, 0.02, 0.2, 0, (k - 1) * 0.3));
    add('thigh' + S, tf(rockg(0.2, 0.42, 0.2, bark, 28), 0, -0.2, 0));
    add('shin' + S, tf(rockg(0.18, 0.4, 0.18, dark, 29), 0, -0.2, 0));
    for (let k = 0; k < 3; k++) add('foot' + S, tf(colorGeo(new THREE.ConeGeometry(0.05, 0.26, 5), 0x4a3222), (k - 1) * 0.07, -0.04, 0.08, Math.PI / 2 - 0.2, 0, (k - 1) * 0.4));
    P.push(['@', strand(BIND['arm' + S].clone().add(v(0, 0.08, 0)), BIND['arm' + S].clone().add(v(sx * 0.1, 0.18, -0.05)), BIND['arm' + S].clone().add(v(sx * 0.18, 0.12, -0.1)), 0.1, { root: leaf, tip: leafHi, thick: 0.35, bone: IDX['arm' + S], segs: 4, radial: 6 })]);
  }
  return P;
}

function revenantParts(L) {
  const bone = 0xddd4c2, cloth = L.top ?? 0x3a2a30, glow = L.glow ?? 0xff8a3a;
  const P = [];
  const add = (b, g) => P.push([b, g]);
  const v = (x, y, z) => new THREE.Vector3(x, y, z);
  add('hips', tf(sph(0.12, bone, 10, 8), 0, 0, 0, 0, 0, 0, 1.2, 0.6, 0.8));
  add('hips', tf(cylg(0.17, 0.26, 0.55, cloth, 14, true), 0, -0.22, 0));
  P.push(['@', tube([{ p: v(0, 1.0, -0.02), bone: IDX.spine }, { p: v(0, 1.25, -0.03), bone: IDX.chest }], () => ({ rx: 0.025, rz: 0.025 }), () => bone, { radial: 6, rings: 3 })]);
  for (let i = 0; i < 4; i++) add('chest', tf(torusg(0.12 - i * 0.012, 0.014, bone, 4, 14), 0, 0.0 + i * 0.065, 0, Math.PI / 2, 0, 0, 1, 0.75, 1));
  add('chest', tf(colorGeo(new THREE.CylinderGeometry(0.2, 0.17, 0.24, 14, 1, true), cloth), 0, 0.22, 0));
  add('neck', tf(cylg(0.022, 0.022, 0.1, bone, 6), 0, 0.04, 0));
  const skull = shapeHead(new THREE.SphereGeometry(0.12, 18, 14), 0.12);
  add('head', tf(colorGeo(skull, bone), 0, 0.12, 0, 0, 0, 0, 0.92, 1, 1));
  add('head', tf(boxg(0.13, 0.045, 0.08, bone), 0, 0.03, 0.05));
  add('head', tf(colorGeo(new THREE.SphereGeometry(0.155, 16, 10, 0, Math.PI * 2, 0, Math.PI * 0.6), cloth), 0, 0.14, -0.02, -0.4, 0, 0, 1.05, 1.1, 1.1));
  for (const s of [1, -1]) {
    add('head', tf(sph(0.03, 0x120806, 8, 6), s * 0.045, 0.11, 0.095));
    add('head', tf(sph(0.018, glow, 8, 6, 1), s * 0.045, 0.11, 0.105));
  }
  for (const sx of [1, -1]) {
    const S = sx > 0 ? 'L' : 'R';
    P.push(['@', tube([{ p: BIND['arm' + S].clone(), bone: IDX['arm' + S] }, { p: BIND['forearm' + S].clone(), bone: IDX['forearm' + S] }, { p: BIND['hand' + S].clone(), bone: IDX['forearm' + S] }], (s) => ({ rx: 0.022 + (Math.abs(s - 0.5) < 0.06 ? 0.012 : 0), rz: 0.022 }), () => bone, { radial: 6, rings: 4 })]);
    add('hand' + S, tf(sph(0.035, bone, 8, 6), 0, -0.04, 0, 0, 0, 0, 0.8, 1.3, 0.6));
    P.push(['@', tube([{ p: BIND['thigh' + S].clone(), bone: IDX['thigh' + S] }, { p: BIND['shin' + S].clone(), bone: IDX['shin' + S] }, { p: BIND['foot' + S].clone().add(v(0, 0.03, 0)), bone: IDX['shin' + S] }], (s) => ({ rx: 0.028 + (Math.abs(s - 0.5) < 0.06 ? 0.014 : 0), rz: 0.028 }), () => bone, { radial: 6, rings: 4 })]);
    add('foot' + S, tf(boxg(0.07, 0.04, 0.17, bone), 0, -0.02, 0.04));
    add('arm' + S, tf(hemi(0.07, cloth), 0, 0.01, 0, 0, 0, -sx * 0.4, 1, 0.7, 1));
  }
  return P;
}

// ------------------------------------------------------------------ skinning
// parts: [boneName, geo] are rigid (built in bone space); ['@', geo] are
// already in bind space with skin attributes (indices into boneDefs).
export function buildSkinned(boneDefs, parts, opts = {}) {
  const bones = {};
  const boneList = [];
  for (const [name, parent, pos] of boneDefs) {
    const b = new THREE.Bone();
    b.name = name;
    b.position.set(pos[0], pos[1], pos[2]);
    if (parent) bones[parent].add(b);
    bones[name] = b;
    boneList.push(b);
  }
  const root = boneList[0];
  root.updateMatrixWorld(true);
  const index = Object.fromEntries(boneList.map((b, i) => [b.name, i]));
  const geos = [];
  for (const [bone, geo0] of parts) {
    let geo = geo0;
    if (bone !== '@') {
      geo.applyMatrix4(bones[bone].matrixWorld);
      bindRigid(geo, index[bone]);
    }
    if (!geo.attributes.aGlow) geo.setAttribute('aGlow', new THREE.BufferAttribute(new Float32Array(geo.attributes.position.count), 1));
    for (const k of Object.keys(geo.attributes)) if (!['position', 'normal', 'color', 'skinIndex', 'skinWeight', 'aGlow'].includes(k)) geo.deleteAttribute(k);
    geos.push(geo);
  }
  const geometry = mergeGeometries(geos);
  if (!geometry) throw new Error('character geometry merge failed');
  geos.forEach((g) => g.dispose());
  const skeleton = new THREE.Skeleton(boneList);
  const mesh = new THREE.SkinnedMesh(geometry, opts.material ?? makeToonMaterial());
  mesh.add(root);
  mesh.bind(skeleton);
  mesh.castShadow = true;
  mesh.boundingSphere = new THREE.Sphere(new THREE.Vector3(0, opts.centerY ?? 1, 0), opts.radius ?? 1.6);
  let outline = null;
  if (opts.outline !== false) {
    outline = new THREE.SkinnedMesh(geometry, opts.outlineMaterial ?? makeOutlineMaterial(opts.outlineWidth ?? 0.009, opts.outlineColor));
    outline.bind(skeleton, mesh.bindMatrix);
    outline.boundingSphere = mesh.boundingSphere;
  }
  const base = {};
  for (const b of boneList) base[b.name] = b.position.clone();
  return { bones, boneList, skeleton, mesh, outline, base, geometry };
}

// The painted face: a patch hugging the front of the head, skinned to it.
function faceMesh(look, skeleton, bindMatrix, hi) {
  const R = HEAD_R * 1.012;
  const g = new THREE.SphereGeometry(R, 28, 18, Math.PI / 2 - 0.95, 1.9, Math.PI * 0.3, Math.PI * 0.48);
  shapeHead(g, R);
  g.translate(HEAD_C.x, HEAD_C.y, HEAD_C.z);
  g.translate(BIND.head.x, BIND.head.y, BIND.head.z);
  bindRigid(g, IDX.head);
  const tex = paintFace(look, hi ? 512 : 256, hi ? 256 : 128);
  const mk = (cv) => { const t = new THREE.CanvasTexture(cv); t.colorSpace = THREE.SRGBColorSpace; t.anisotropy = 4; return t; };
  const open = mk(tex.open), closed = mk(tex.closed);
  const mat = new THREE.MeshToonMaterial({ map: open, gradientMap: getToonRamp(), transparent: true, depthWrite: false, polygonOffset: true, polygonOffsetFactor: -2, polygonOffsetUnits: -2 });
  const m = new THREE.SkinnedMesh(g, mat);
  m.bind(skeleton, bindMatrix);
  m.boundingSphere = new THREE.Sphere(new THREE.Vector3(0, 1, 0), 1.6);
  m.renderOrder = 2;
  m.userData = { open, closed };
  return m;
}

export class Character {
  constructor(look = {}, opts = {}) {
    this.look = { ...DEFAULT_LOOK, ...look };
    this.group = new THREE.Group();
    const kind = this.look.kind;
    const parts = kind === 'golem' ? golemParts(this.look) : kind === 'revenant' ? revenantParts(this.look) : kind === 'treant' ? treantParts(this.look) : buildHumanoid(this.look);
    const r = buildSkinned(BONES, parts, { ...opts, outlineWidth: (kind === 'golem' || kind === 'treant' ? 0.012 : 0.0085) / Math.sqrt(this.look.height || 1) });
    this.bones = r.bones; this.skeleton = r.skeleton; this.mesh = r.mesh; this.outline = r.outline; this.base = r.base;
    this.group.add(this.mesh);
    if (this.outline) this.group.add(this.outline);
    if (!kind) {
      this.face = faceMesh(this.look, this.skeleton, this.mesh.bindMatrix, opts.hiFace);
      this.group.add(this.face);
    }
    const scale = this.look.height;
    this.group.scale.set(this.look.build * scale, scale, this.look.build * scale);
    this.anim = new Animator(this);
    this.weapon = null;
    this.weaponDrawn = false;
  }

  blink(closed) {
    if (!this.face) return;
    const t = closed ? this.face.userData.closed : this.face.userData.open;
    if (this.face.material.map !== t) this.face.material.map = t;
  }

  setWeapon(mesh) {
    if (this.weapon) this.weapon.parent?.remove(this.weapon);
    this.weapon = mesh;
    if (mesh) this.placeWeapon();
  }

  placeWeapon() {
    const w = this.weapon;
    if (!w) return;
    if (this.weaponDrawn) {
      this.bones.handR.add(w);
      w.position.set(0, -0.075, 0.012);
      w.rotation.set(1.72, 0, 0);
    } else {
      this.bones.chest.add(w);
      w.position.set(0.06, 0.06, -0.17);
      w.rotation.set(0.12, 0, 2.55);
    }
  }

  drawWeapon(on) {
    if (this.weaponDrawn === on) return;
    this.weaponDrawn = on;
    this.placeWeapon();
  }

  update(dt, state) { this.anim.update(dt, state); }

  dispose() {
    this.mesh.geometry.dispose();
    if (this.face) { this.face.geometry.dispose(); this.face.userData.open.dispose(); this.face.userData.closed.dispose(); this.face.material.dispose(); }
  }
}

// ------------------------------------------------------------------ weapons & props
export function makeSword(kind = 'iron') {
  const looks = {
    rusty: { blade: 0x9a8a7a, edge: 0xb8a898, guard: 0x6a5040, grip: 0x3a2418, len: 0.75 },
    iron: { blade: 0xb8c2d0, edge: 0xe8eef6, guard: 0x6a5a4a, grip: 0x3a2418, len: 0.85 },
    steel: { blade: 0xd0dae8, edge: 0xffffff, guard: 0xc8a040, grip: 0x2a1a30, len: 0.95 },
    crystal: { blade: 0x8ff0ff, edge: 0xe8ffff, guard: 0xb48cff, grip: 0x2a2050, len: 1.05, glow: 0.8 },
    flame: { blade: 0xff8a3a, edge: 0xffd08a, guard: 0x5a2a20, grip: 0x2a1410, len: 1.0, glow: 0.7 },
    frost: { blade: 0xbfe8ff, edge: 0xffffff, guard: 0x4a6a9a, grip: 0x1a2a40, len: 1.0, glow: 0.6 },
  };
  const s = looks[kind] || looks.iron;
  const L = s.len;
  // Blade: a lozenge cross-section (thin diamond) tapering to a point.
  const blade = new THREE.CylinderGeometry(0.0, 0.034, L, 4, 6);
  blade.rotateY(Math.PI / 4);
  blade.scale(1, 1, 0.22);
  const pb = blade.attributes.position;
  for (let i = 0; i < pb.count; i++) {
    const y = pb.getY(i) / L + 0.5; // 0 base .. 1 tip
    const w = y > 0.86 ? (1 - y) / 0.14 : 1;
    pb.setX(i, pb.getX(i) * (y < 0.86 ? 1 / Math.max(0.0001, 1 - y) * (1 - y) : 1) * (y > 0.86 ? w : 1));
  }
  // Rebuild as a straight blade with a pointed tip.
  const bl = new THREE.BufferGeometry();
  const v = [], hw = 0.034, th = 0.007, tipY = L, bodyY = L * 0.86;
  const prof = [[0, hw, th], [bodyY, hw * 0.92, th], [tipY, 0, 0]];
  const ring = (y, w, t) => [[0, y, t], [w, y, 0], [0, y, -t], [-w, y, 0]];
  const rings = prof.map(([y, w, t]) => ring(y, w, t));
  for (let r = 0; r < rings.length - 1; r++) {
    for (let k = 0; k < 4; k++) {
      const a = rings[r][k], b = rings[r][(k + 1) % 4], c = rings[r + 1][k], d = rings[r + 1][(k + 1) % 4];
      v.push(...a, ...b, ...c, ...b, ...d, ...c);
    }
  }
  bl.setAttribute('position', new THREE.Float32BufferAttribute(v, 3));
  bl.computeVertexNormals();
  const bladeG = colorGeo(bl, (x, y, z) => (Math.abs(x) > hw * 0.6 ? s.edge : s.blade), s.glow ? s.glow * 0.6 : 0);
  bladeG.translate(0, 0.13, 0);
  const parts = [
    bladeG,
    tf(colorGeo(new THREE.CylinderGeometry(0.019, 0.022, 0.19, 8), (x, y) => (Math.floor((y + 0.1) * 50) % 2 ? s.grip : new THREE.Color(s.grip).multiplyScalar(1.4))), 0, 0.005, 0),
    tf(boxg(0.24, 0.032, 0.05, s.guard), 0, 0.115, 0),
    tf(sph(0.022, s.guard, 8, 6), 0.125, 0.115, 0), tf(sph(0.022, s.guard, 8, 6), -0.125, 0.115, 0),
    tf(sph(0.028, s.guard, 8, 6), 0, -0.105, 0),
    tf(colorGeo(new THREE.OctahedronGeometry(0.018, 0), s.glow ? s.blade : 0xc83a3a, s.glow ? 1 : 0.4), 0, 0.115, 0.028),
  ];
  for (const p of parts) if (!p.attributes.aGlow) p.setAttribute('aGlow', new THREE.BufferAttribute(new Float32Array(p.attributes.position.count), 1));
  const g = mergeGeometries(parts);
  const m = new THREE.Mesh(g, makeToonMaterial({ rim: 0.5 }));
  m.castShadow = true;
  m.userData.length = L + 0.13;
  m.userData.color = s.glow ? s.blade : 0xdfe8ff;
  return m;
}

export function makeStaff(color = 0x9ff0ff) {
  const parts = [
    tf(colorGeo(new THREE.CylinderGeometry(0.022, 0.03, 1.5, 8), 0x6a4a2a), 0, 0.3, 0),
    tf(torusg(0.06, 0.012, 0xe6c87a, 6, 14), 0, 1.06, 0),
    tf(colorGeo(new THREE.OctahedronGeometry(0.075, 0), color, 1), 0, 1.15, 0, 0, 0, 0, 1, 1.7, 1),
  ];
  for (const p of parts) if (!p.attributes.aGlow) p.setAttribute('aGlow', new THREE.BufferAttribute(new Float32Array(p.attributes.position.count), 1));
  return new THREE.Mesh(mergeGeometries(parts), makeToonMaterial());
}

export function makeTool(kind) {
  const parts = kind === 'hammer'
    ? [tf(cylg(0.018, 0.02, 0.45, 0x6a4a2a, 8), 0, 0.15, 0), tf(boxg(0.15, 0.075, 0.075, 0x4a4a52), 0, 0.38, 0), tf(boxg(0.155, 0.02, 0.08, 0x6a6a72), 0, 0.41, 0)]
    : kind === 'book'
      ? [tf(boxg(0.18, 0.04, 0.24, 0x7a3a3a), 0, 0, 0), tf(boxg(0.17, 0.042, 0.23, 0xf4ead6), 0.005, 0, 0), tf(boxg(0.02, 0.045, 0.24, 0xe6c87a), -0.085, 0, 0)]
      : [tf(cylg(0.017, 0.02, 1.5, 0x6a4a2a, 8), 0, 0.3, 0), tf(coneg(0.04, 0.17, 0xb8c0c8, 4), 0, 1.13, 0), tf(torusg(0.024, 0.008, 0x9a7a4a, 4, 10), 0, 1.03, 0, Math.PI / 2, 0, 0)];
  for (const p of parts) p.setAttribute('aGlow', new THREE.BufferAttribute(new Float32Array(p.attributes.position.count), 1));
  return new THREE.Mesh(mergeGeometries(parts), makeToonMaterial());
}

export function makeShield(color = 0x2f5fa8, trim = 0xe6c87a) {
  const face = colorGeo(new THREE.CylinderGeometry(0.24, 0.24, 0.035, 20), (x, y, z) => (Math.hypot(x, z) > 0.2 ? trim : color));
  face.rotateX(Math.PI / 2);
  const boss = sph(0.05, trim, 10, 6);
  boss.translate(0, 0, 0.025);
  const parts = [face, boss];
  for (const p of parts) p.setAttribute('aGlow', new THREE.BufferAttribute(new Float32Array(p.attributes.position.count), 1));
  return new THREE.Mesh(mergeGeometries(parts), makeToonMaterial());
}

// ------------------------------------------------------------------ animator
// Poses are Euler targets per bone; bones ease toward them. Springs give hair,
// capes and skirt panels follow-through.
class Spring {
  constructor(k = 60, d = 9) { this.x = 0; this.v = 0; this.k = k; this.d = d; }
  step(target, dt) {
    const a = (target - this.x) * this.k - this.v * this.d;
    this.v += a * dt;
    this.x += this.v * dt;
    return this.x;
  }
}

export class Animator {
  constructor(ch) {
    this.ch = ch;
    this.phase = 0;
    this.t = Math.random() * 10;
    this.target = {};
    for (const k of Object.keys(ch.bones)) this.target[k] = { x: 0, y: 0, z: 0 };
    this.hipY = 0;
    this.blinkT = 1 + Math.random() * 3;
    this.springs = { cape: new Spring(40, 7), capeZ: new Spring(40, 7), hair: new Spring(55, 6), hairZ: new Spring(55, 6), skF: new Spring(90, 11), skB: new Spring(90, 11) };
    this.prevSpeed = 0;
    this.prevYaw = null;
    this.land = 0;
  }

  update(dt, s) {
    if (dt <= 0) return;
    this.t += dt;
    const T = this.target;
    for (const k in T) { T[k].x = 0; T[k].y = 0; T[k].z = 0; }
    let hipY = 0;
    let rate = 11;
    const fast = new Set();
    const speed = s.speed || 0;
    const move = clamp(speed / 3, 0, 1);
    const run = clamp((speed - 3.8) / 3.5, 0, 1);
    const sprint = clamp((speed - 7) / 2, 0, 1);
    const freq = speed > 0.15 ? 1.65 + speed * 0.5 : 0;
    this.phase += dt * freq * 2.4;
    const ph = this.phase, sw = Math.sin(ph), cw = Math.cos(ph);
    const breathe = Math.sin(this.t * 1.9);
    const accel = (speed - this.prevSpeed) / dt;
    this.prevSpeed = speed;
    const yaw = this.ch.group.rotation.y;
    let turn = 0;
    if (this.prevYaw !== null) { let d = yaw - this.prevYaw; d = Math.atan2(Math.sin(d), Math.cos(d)); turn = d / dt; }
    this.prevYaw = yaw;
    if (s.landed) this.land = Math.min(1, s.landed);
    this.land = Math.max(0, this.land - dt * 4);

    if (s.dead) {
      const k = clamp(s.deadT * 2.2, 0, 1);
      const e = 1 - Math.pow(1 - k, 3);
      T.hips.x = -1.5 * e; hipY = -0.78 * e;
      T.spine.x = 0.2 * e; T.head.x = -0.3 * e; T.head.y = 0.5 * e;
      T.armL.z = 1.3 * e; T.armR.z = -1.0 * e; T.armR.x = -0.4 * e;
      T.thighL.x = -0.35 * e; T.shinL.x = 0.7 * e; T.thighR.x = 0.1 * e;
      this.apply(dt, 9, hipY, fast);
      this.secondary(dt, 0, 0, 0, s);
      return;
    }

    if (s.swim) {
      T.hips.x = 1.2; hipY = -0.55;
      T.neck.x = -0.75; T.head.x = -0.45;
      const p = this.t * 3.2;
      T.armL.x = -1.6 + Math.sin(p) * 1.5; T.armR.x = -1.6 + Math.sin(p + Math.PI) * 1.5;
      T.forearmL.x = -0.3 - Math.max(0, Math.cos(p)) * 0.6; T.forearmR.x = -0.3 - Math.max(0, -Math.cos(p)) * 0.6;
      T.armL.z = 0.35; T.armR.z = -0.35;
      T.thighL.x = Math.sin(p * 1.6) * 0.4; T.thighR.x = -Math.sin(p * 1.6) * 0.4;
      T.shinL.x = 0.25 + Math.max(0, Math.sin(p * 1.6)) * 0.4; T.shinR.x = 0.25 + Math.max(0, -Math.sin(p * 1.6)) * 0.4;
    } else if (s.glide) {
      T.hips.x = 1.05;
      T.neck.x = -0.6; T.head.x = -0.45;
      T.armL.z = 1.4; T.armR.z = -1.4; T.armL.x = -0.25; T.armR.x = -0.25;
      T.forearmL.x = -0.15; T.forearmR.x = -0.15;
      T.spine.z = -clamp(turn, -1.5, 1.5) * 0.15;
      T.thighL.x = 0.12 + Math.sin(this.t * 3.5) * 0.06; T.thighR.x = 0.22 + Math.sin(this.t * 3.5 + 1) * 0.06; T.shinL.x = 0.35; T.shinR.x = 0.45;
    } else if (!s.grounded) {
      const up = clamp(s.vy / 6, -1, 1);
      const fall = clamp(-s.vy / 10, 0, 1);
      T.thighL.x = -1.0 + (1 - up) * 0.3; T.shinL.x = 1.4 - fall * 0.5;
      T.thighR.x = -0.15 + fall * 0.1; T.shinR.x = 0.55;
      T.armL.z = 0.45 + fall * 0.6; T.armR.z = -0.45 - fall * 0.6;
      T.armL.x = -0.5 + fall * 0.3; T.armR.x = -0.35 + fall * 0.3;
      T.forearmL.x = -0.5; T.forearmR.x = -0.5;
      T.spine.x = 0.12 - fall * 0.1; T.head.x = -0.1 + fall * 0.2;
    } else {
      // ---- locomotion
      const legAmp = lerp(0.45, 0.85, run) * move;
      const armAmp = lerp(0.35, 0.8, run) * move;
      T.thighL.x = -sw * legAmp - run * 0.12; T.thighR.x = sw * legAmp - run * 0.12;
      T.shinL.x = Math.max(0, cw) * legAmp * 1.6 + 0.06 + run * 0.2; T.shinR.x = Math.max(0, -cw) * legAmp * 1.6 + 0.06 + run * 0.2;
      T.footL.x = -Math.max(0, -cw) * 0.35 * move + Math.max(0, sw) * 0.2 * move; T.footR.x = -Math.max(0, cw) * 0.35 * move + Math.max(0, -sw) * 0.2 * move;
      T.armL.x = sw * armAmp - run * 0.15; T.armR.x = -sw * armAmp - run * 0.15;
      T.armL.z = 0.12 + run * 0.1; T.armR.z = -0.12 - run * 0.1;
      T.forearmL.x = -0.2 - run * 1.2 - move * 0.15 - Math.max(0, -sw) * 0.3 * move;
      T.forearmR.x = -0.2 - run * 1.2 - move * 0.15 - Math.max(0, sw) * 0.3 * move;
      T.spine.x = run * 0.2 + sprint * 0.12 + move * 0.03 + clamp(accel * 0.02, -0.1, 0.15);
      T.spine.y = sw * 0.12 * move; T.chest.y = sw * 0.06 * move; T.hips.y = -sw * 0.12 * move;
      T.hips.z = -cw * 0.04 * move;
      T.head.y = -sw * 0.08 * move; T.head.x = -run * 0.15;
      T.spine.z = clamp(-turn * 0.06, -0.3, 0.3) * move;
      hipY = -Math.abs(cw) * 0.045 * move - run * 0.06 + Math.abs(sw) * 0.02 * run;
      if (s.crouch) { hipY -= 0.3; T.thighL.x -= 0.85; T.thighR.x -= 0.85; T.shinL.x += 1.35; T.shinR.x += 1.35; T.footL.x -= 0.4; T.footR.x -= 0.4; T.spine.x += 0.35; T.head.x -= 0.25; }
      if (this.land > 0) { hipY -= this.land * 0.18; T.thighL.x -= this.land * 0.5; T.thighR.x -= this.land * 0.5; T.shinL.x += this.land * 0.9; T.shinR.x += this.land * 0.9; T.spine.x += this.land * 0.2; }
      // ---- idle
      if (move < 0.1 && !s.action) {
        const sway = Math.sin(this.t * 0.55);
        T.hips.z = sway * 0.035; T.spine.z = -sway * 0.025; T.thighL.z = -sway * 0.03; T.thighR.z = -sway * 0.03;
        T.thighL.x = -0.04; T.thighR.x = 0.05; T.shinL.x = 0.08; T.shinR.x = 0.03;
        T.armL.z = 0.11 + breathe * 0.015; T.armR.z = -0.11 - breathe * 0.015;
        T.forearmL.x = -0.18; T.forearmR.x = -0.22;
        T.chest.x = breathe * 0.025; T.head.x = -breathe * 0.015 + (s.headPitch || 0);
        T.head.y = (s.lookAround ? Math.sin(this.t * 0.37) * 0.35 + Math.sin(this.t * 1.13) * 0.08 : 0) + (s.headYaw || 0);
        hipY = breathe * 0.004;
        if (s.pose === 'sit') { hipY = -0.48; T.thighL.x = -1.5; T.thighR.x = -1.45; T.shinL.x = 1.55; T.shinR.x = 1.4; T.armL.x = -0.55; T.armR.x = -0.5; T.forearmL.x = -0.7; T.forearmR.x = -0.6; T.spine.x = 0.2; T.head.x = 0.1; }
        if (s.pose === 'work') { const k = Math.max(0, Math.sin(this.t * 4)); T.armR.x = -1.15 - k * 1.25; T.forearmR.x = -0.9 + k * 0.7; T.spine.x = 0.28; T.armL.x = -0.6; T.forearmL.x = -0.9; fast.add('armR'); fast.add('forearmR'); }
        if (s.pose === 'talk') { T.armL.x = -0.45 + Math.sin(this.t * 2.3) * 0.22; T.forearmL.x = -1.0; T.armL.z = 0.3; T.handL.x = -0.3; T.head.x += Math.sin(this.t * 3.1) * 0.04; T.head.z = Math.sin(this.t * 1.7) * 0.05; }
        if (s.pose === 'read') { T.armL.x = -0.95; T.armR.x = -0.85; T.forearmL.x = -1.15; T.forearmR.x = -1.1; T.armL.z = -0.15; T.armR.z = 0.15; T.head.x = 0.38; }
        if (s.pose === 'guard') { T.armR.x = -0.35; T.forearmR.x = -1.25; T.armL.z = 0.06; T.spine.x = -0.04; }
        if (s.pose === 'cheer') { const k = Math.sin(this.t * 6); T.armL.z = 2.6 + k * 0.2; T.armR.z = -2.6 - k * 0.2; hipY += Math.max(0, k) * 0.06; }
        if (s.pose === 'combat') { T.thighL.x = -0.35; T.shinL.x = 0.3; T.thighR.x = 0.3; T.shinR.x = 0.25; hipY -= 0.06; T.armR.x = -0.55; T.forearmR.x = -0.9; T.armR.y = 0.3; T.armL.x = -0.4; T.forearmL.x = -1.1; T.spine.y = -0.25; T.spine.x = 0.1; }
      }
    }

    // ---------------- actions
    const a = s.action, t = s.actionT || 0;
    if (a) rate = 20;
    const ease = (x) => 1 - Math.pow(1 - clamp(x, 0, 1), 3);
    if (a === 'attack1' || a === 'attack2' || a === 'attack3' || a === 'heavy' || a === 'spin' || a === 'plunge') {
      // Phases: anticipation -> strike -> follow-through -> recover.
      const wind = a === 'heavy' ? 0.5 : a === 'attack3' ? 0.38 : 0.3;
      const hit = a === 'heavy' ? 0.62 : a === 'attack3' ? 0.5 : 0.45;
      const k = ease((t - wind) / (hit - wind));
      const w = smoothstep(0, wind * 0.7, t) * (1 - smoothstep(0.72, 1, t));
      ['armR', 'forearmR', 'spine', 'chest', 'armL', 'forearmL', 'handR'].forEach((b) => fast.add(b));
      const P = {};
      if (a === 'attack1') {
        // Right-to-left horizontal cut.
        P.armR = [lerp(-0.9, -1.45, k), lerp(0.9, -0.5, k), lerp(-1.25, 0.55, k)];
        P.forearmR = [lerp(-1.3, -0.15, k), 0, 0];
        P.handR = [lerp(0.2, -0.2, k), lerp(-1.2, -1.6, k), 0];
        P.spine = [0.08, lerp(-0.6, 0.55, k), 0]; P.chest = [0, lerp(-0.25, 0.3, k), 0];
        P.armL = [-0.6, 0, 0.55]; P.forearmL = [-1.0, 0, 0];
      } else if (a === 'attack2') {
        // Rising backhand, left-to-right.
        P.armR = [lerp(-0.4, -2.0, k), lerp(-0.6, 0.5, k), lerp(0.55, -1.0, k)];
        P.forearmR = [lerp(-1.6, -0.25, k), 0, 0];
        P.handR = [lerp(-0.4, 0.2, k), lerp(1.3, 1.6, k), 0];
        P.spine = [0.05, lerp(0.55, -0.55, k), 0]; P.chest = [0, lerp(0.25, -0.3, k), 0];
        P.armL = [-0.3, 0, 0.7]; P.forearmL = [-0.8, 0, 0];
      } else if (a === 'attack3' || a === 'heavy') {
        // Two-handed overhead smash.
        P.armR = [lerp(-2.9, -0.7, k), 0, lerp(-0.15, 0.25, k)];
        P.forearmR = [lerp(-0.9, -0.05, k), 0, 0];
        P.armL = [lerp(-2.7, -0.75, k), 0, lerp(-0.35, -0.3, k)];
        P.forearmL = [lerp(-0.9, -0.2, k), 0, 0];
        P.spine = [lerp(-0.35, 0.5, k), 0, 0]; P.chest = [lerp(-0.1, 0.15, k), 0, 0];
        hipY += lerp(0.04, -0.2, k) * w;
        T.thighL.x += -0.6 * k * w; T.shinL.x += 0.6 * k * w; T.thighR.x += 0.45 * k * w;
      } else if (a === 'spin') {
        P.armR = [-1.5, 0, -1.4]; P.forearmR = [-0.2, 0, 0];
        P.armL = [-0.4, 0, 1.1]; P.spine = [0.15, 0, 0];
        T.hips.y = -t * Math.PI * 2; fast.add('hips');
      } else if (a === 'plunge') {
        P.armR = [-2.6 + k * 1.2, 0, -0.1]; P.armL = [-2.5 + k * 1.2, 0, -0.3];
        P.forearmR = [-0.3, 0, 0]; P.forearmL = [-0.4, 0, 0];
        P.spine = [0.5 * k, 0, 0];
        T.thighL.x = -1.2; T.shinL.x = 1.3; T.thighR.x = -0.6; T.shinR.x = 1.0;
      }
      for (const b in P) { T[b].x = lerp(T[b].x, P[b][0], w); T[b].y = lerp(T[b].y, P[b][1], w); T[b].z = lerp(T[b].z, P[b][2], w); }
      if (a !== 'plunge') { T.thighL.x += -0.35 * w; T.shinL.x += 0.25 * w; T.thighR.x += 0.3 * w; T.footR.x -= 0.2 * w; hipY -= 0.04 * w; }
    } else if (a === 'charge') {
      // Holding a heavy attack: blade drawn back, low stance, trembling.
      const tr = Math.sin(this.t * 40) * 0.02;
      T.armR.x = -2.6 + tr; T.armR.z = -0.25; T.forearmR.x = -1.0; T.armL.x = -2.4 + tr; T.forearmL.x = -1.0; T.armL.z = -0.3;
      T.spine.x = -0.25; T.thighL.x = -0.5; T.shinL.x = 0.6; T.thighR.x = 0.4; hipY -= 0.12;
      fast.add('armR'); fast.add('armL');
    } else if (a === 'cast') {
      const w = smoothstep(0, 0.25, t) * (1 - smoothstep(0.72, 1, t));
      const push = ease((t - 0.2) / 0.15);
      T.armL.x = -1.55 * w; T.armL.z = 0.1 * w; T.forearmL.x = lerp(-0.9, -0.05, push) * w; T.handL.x = -0.7 * w;
      T.armR.x = -0.35 * w; T.armR.z = -0.45 * w; T.forearmR.x = -0.6 * w;
      T.spine.y = 0.38 * w; T.chest.y = 0.15 * w; T.spine.x = -0.05 * w;
      T.thighL.x -= 0.3 * w; T.shinL.x += 0.2 * w; T.thighR.x += 0.25 * w;
      fast.add('armL'); fast.add('forearmL');
    } else if (a === 'castUp') {
      const w = smoothstep(0, 0.3, t) * (1 - smoothstep(0.75, 1, t));
      T.armL.z = 2.7 * w; T.armR.z = -2.7 * w; T.forearmL.z = 0.2 * w; T.forearmR.z = -0.2 * w; T.head.x = -0.45 * w; T.spine.x = -0.18 * w;
      fast.add('armL'); fast.add('armR');
    } else if (a === 'surge') {
      const w = smoothstep(0, 0.2, t) * (1 - smoothstep(0.85, 1, t));
      const k = ease((t - 0.25) / 0.2);
      T.armR.x = lerp(-3.0, -1.2, k) * w; T.armL.x = lerp(-3.0, -1.3, k) * w; T.forearmR.x = -0.3 * w; T.forearmL.x = -0.4 * w;
      T.spine.x = lerp(-0.4, 0.5, k) * w; hipY += lerp(0.25, -0.25, k) * w;
      T.thighL.x -= 0.7 * k * w; T.shinL.x += 0.8 * k * w; T.thighR.x += 0.5 * k * w;
      fast.add('armR'); fast.add('armL'); fast.add('spine');
    } else if (a === 'block' || a === 'parry') {
      const k = a === 'parry' ? Math.sin(Math.PI * clamp(t * 1.4, 0, 1)) : 1;
      T.armR.x = -1.3 * k; T.armR.z = 0.5 * k; T.forearmR.x = -1.25 * k; T.armR.y = 0.55 * k;
      T.armL.x = -1.1 * k; T.armL.z = -0.3 * k; T.forearmL.x = -1.35 * k;
      T.spine.x += 0.15 * k; T.spine.y = -0.2 * k; T.thighL.x += -0.3; T.shinL.x += 0.35; T.thighR.x += 0.3; hipY -= 0.08;
      ['armR', 'armL', 'forearmR', 'forearmL'].forEach((b) => fast.add(b));
    } else if (a === 'dodge') {
      const k = smoothstep(0, 1, t);
      T.hips.x = k * Math.PI * 2; hipY += -0.5 * Math.sin(Math.PI * t);
      T.thighL.x = -1.7; T.thighR.x = -1.6; T.shinL.x = 2.1; T.shinR.x = 2.0;
      T.armL.x = -1.2; T.armR.x = -1.2; T.forearmL.x = -1.5; T.forearmR.x = -1.5;
      T.spine.x = 0.65; T.neck.x = 0.45;
      rate = 40; fast.add('hips');
    } else if (a === 'hit') {
      const w = 1 - smoothstep(0.15, 1, t);
      T.spine.x = -0.38 * w; T.head.x = -0.35 * w; T.armL.z = 0.6 * w; T.armR.z = -0.6 * w; T.spine.z = 0.15 * w; hipY -= 0.05 * w;
      fast.add('spine'); fast.add('head');
    } else if (a === 'stagger') {
      const w = Math.sin(Math.PI * t);
      T.spine.x = -0.5 * w; T.head.x = -0.4 * w; T.armL.z = 0.9 * w; T.armR.z = -0.9 * w; T.thighR.x = 0.4 * w; hipY -= 0.12 * w;
    } else if (a === 'interact') {
      const w = Math.sin(Math.PI * t);
      T.armR.x = -1.15 * w; T.forearmR.x = -0.45 * w; T.spine.x = 0.4 * w; hipY -= 0.14 * w; T.thighL.x -= 0.3 * w; T.shinL.x += 0.5 * w;
    } else if (a === 'drink') {
      const w = Math.sin(Math.PI * Math.min(t * 1.4, 1));
      T.armR.x = -1.6 * w; T.forearmR.x = -1.95 * w; T.head.x = -0.45 * w;
    }

    if (s.headYaw && !a && move > 0.1) T.head.y += s.headYaw;

    // Blink.
    this.blinkT -= dt;
    if (this.blinkT < 0) { this.ch.blink(true); if (this.blinkT < -0.12) { this.blinkT = 2 + Math.random() * 3.5; this.ch.blink(false); } }

    this.apply(dt, rate, hipY, fast, a === 'dodge' || a === 'spin');
    this.secondary(dt, speed, accel, turn, s);
  }

  // Follow-through for cape, hair and skirt panels.
  secondary(dt, speed, accel, turn, s) {
    const B = this.ch.bones, S = this.springs;
    const airborne = !s.grounded && !s.swim;
    const capeT = 0.08 + speed * 0.075 - clamp(accel, -20, 20) * 0.012 + (airborne ? clamp(-s.vy, -6, 10) * 0.05 : 0) + (s.glide ? 1.0 : 0) + Math.sin(this.t * 7) * 0.03 * clamp(speed / 6, 0, 1);
    B.cape.rotation.x = S.cape.step(clamp(capeT, -0.2, 1.4), dt);
    B.cape.rotation.z = S.capeZ.step(clamp(turn * 0.08, -0.4, 0.4), dt);
    B.hair.rotation.x = S.hair.step(clamp(0.05 + speed * 0.05 - accel * 0.01 + (airborne ? -s.vy * 0.03 : 0), -0.5, 0.8), dt);
    B.hair.rotation.z = S.hairZ.step(clamp(turn * 0.1, -0.5, 0.5), dt);
    // Skirt panels: front follows the forward-most thigh, back the rear-most.
    const tl = B.thighL.rotation.x, tr = B.thighR.rotation.x;
    B.skirtF.rotation.x = S.skF.step(Math.min(0, Math.min(tl, tr)) * 0.8 - speed * 0.01, dt);
    B.skirtB.rotation.x = S.skB.step(Math.max(0, Math.max(tl, tr)) * 0.8 + speed * 0.015 + (B.hips.rotation.x > 0.5 ? -0.3 : 0), dt);
  }

  apply(dt, rate, hipY, fast, snap = false) {
    const T = this.target, B = this.ch.bones;
    const k = 1 - Math.exp(-rate * dt), kf = 1 - Math.exp(-rate * 1.7 * dt);
    for (const name in T) {
      if (name === 'cape' || name === 'hair' || name === 'skirtF' || name === 'skirtB') continue;
      const b = B[name], tg = T[name];
      const kk = fast.has(name) ? kf : k;
      if (snap && name === 'hips') { b.rotation.x = tg.x; b.rotation.y = tg.y; b.rotation.z = lerp(b.rotation.z, tg.z, kk); continue; }
      if (name === 'hips') { b.rotation.x = Math.atan2(Math.sin(b.rotation.x), Math.cos(b.rotation.x)); b.rotation.y = Math.atan2(Math.sin(b.rotation.y), Math.cos(b.rotation.y)); }
      b.rotation.x += (tg.x - b.rotation.x) * kk;
      b.rotation.y += (tg.y - b.rotation.y) * kk;
      b.rotation.z += (tg.z - b.rotation.z) * kk;
    }
    this.hipY = damp(this.hipY, hipY, rate, dt);
    B.hips.position.y = this.ch.base.hips.y + this.hipY;
  }
}

export { lerp, smooth };
