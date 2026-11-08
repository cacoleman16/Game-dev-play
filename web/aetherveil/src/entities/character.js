// Procedural humanoid characters. Body parts are primitives rigidly bound to
// a bone skeleton and merged into ONE skinned mesh (plus an ink outline that
// shares the skeleton), cel-shaded with a toon ramp. All animation is
// procedural: locomotion cycles plus layered actions.

import * as THREE from 'three';
import { mergeGeometries } from 'three/addons';
import { damp, clamp, lerp, smoothstep } from '../core/util.js';

let toonRamp = null;
export function getToonRamp() {
  if (toonRamp) return toonRamp;
  const data = new Uint8Array([105, 105, 105, 255, 185, 185, 185, 255, 255, 255, 255, 255]);
  toonRamp = new THREE.DataTexture(data, 3, 1, THREE.RGBAFormat);
  toonRamp.minFilter = toonRamp.magFilter = THREE.NearestFilter;
  toonRamp.generateMipmaps = false;
  toonRamp.needsUpdate = true;
  return toonRamp;
}

export function makeToonMaterial(opts = {}) {
  return new THREE.MeshToonMaterial({ vertexColors: true, gradientMap: getToonRamp(), ...opts });
}

export function makeOutlineMaterial(width = 0.012, color = 0x1a1424) {
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

// Skeleton layout in bind pose (metres, relative to parent). Facing +Z.
// Character's right side is -X.
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
];

const C = (hex) => new THREE.Color(hex);

function colorGeo(geo, color) {
  const g = geo.index ? geo.toNonIndexed() : geo;
  for (const k of Object.keys(g.attributes)) if (k !== 'position' && k !== 'normal') g.deleteAttribute(k);
  const n = g.attributes.position.count;
  const col = new Float32Array(n * 3);
  const fn = typeof color === 'function' ? color : null;
  const c = fn ? new THREE.Color() : C(color);
  for (let i = 0; i < n; i++) {
    if (fn) c.copy(fn(g.attributes.position.getX(i), g.attributes.position.getY(i), g.attributes.position.getZ(i)));
    col[i * 3] = c.r; col[i * 3 + 1] = c.g; col[i * 3 + 2] = c.b;
  }
  g.setAttribute('color', new THREE.BufferAttribute(col, 3));
  return g;
}

const tf = (geo, x = 0, y = 0, z = 0, rx = 0, ry = 0, rz = 0, sx = 1, sy = 1, sz = 1) => {
  const m = new THREE.Matrix4().compose(new THREE.Vector3(x, y, z), new THREE.Quaternion().setFromEuler(new THREE.Euler(rx, ry, rz)), new THREE.Vector3(sx, sy, sz));
  geo.applyMatrix4(m);
  return geo;
};
const capsule = (r, len, col, seg = 8) => colorGeo(new THREE.CapsuleGeometry(r, len, 3, seg), col);
const sph = (r, col, ws = 12, hs = 8) => colorGeo(new THREE.SphereGeometry(r, ws, hs), col);
const cylg = (rt, rb, h, col, seg = 10, open = false) => colorGeo(new THREE.CylinderGeometry(rt, rb, h, seg, 1, open), col);
const coneg = (r, h, col, seg = 6) => colorGeo(new THREE.ConeGeometry(r, h, seg), col);
const boxg = (w, h, d, col) => colorGeo(new THREE.BoxGeometry(w, h, d), col);

export { BONES, colorGeo, tf, capsule, sph, cylg, coneg, boxg };

export const DEFAULT_LOOK = {
  skin: 0xf3d2b4, hair: 0x2e2630, hairStyle: 'spiky', eyes: 0x3a6ad8,
  top: 0x2f5fa8, trim: 0xe8d8b0, pants: 0x3a3440, boots: 0x5a3a28, belt: 0x6a4a2a,
  scarf: 0xc8453a, cape: null, pauldrons: true, hat: null, beard: false, female: false,
  skirt: false, apron: null, build: 1, height: 1, gloves: 0x4a3a30,
};

function buildParts(look) {
  const L = { ...DEFAULT_LOOK, ...look };
  const parts = []; // [bone, geo]
  const add = (bone, geo) => parts.push([bone, geo]);
  const fem = L.female;
  const sw = fem ? 0.9 : 1.0; // shoulder width factor
  // Pelvis & legs.
  add('hips', tf(sph(0.16, L.pants), 0, 0.02, 0, 0, 0, 0, fem ? 1.12 : 1.05, 0.75, 0.85));
  for (const s of [1, -1]) {
    const side = s > 0 ? 'L' : 'R';
    add('thigh' + side, tf(capsule(fem ? 0.078 : 0.075, 0.3, L.pants), 0, -0.2, 0));
    add('shin' + side, tf(capsule(0.06, 0.3, L.pants), 0, -0.18, 0));
    add('shin' + side, tf(cylg(0.072, 0.064, 0.24, L.boots), 0, -0.3, 0));
    add('foot' + side, tf(boxg(0.11, 0.08, 0.24, L.boots), 0, -0.02, 0.05));
    add('foot' + side, tf(boxg(0.115, 0.03, 0.25, 0x2a2018), 0, -0.065, 0.05));
  }
  // Torso.
  add('spine', tf(cylg(0.15, 0.14, 0.24, L.top), 0, 0.1, 0));
  add('spine', tf(cylg(0.155, 0.155, 0.06, L.belt), 0, 0.0, 0));
  add('spine', tf(boxg(0.05, 0.05, 0.03, 0xe6b84a), 0, 0.0, 0.155));
  add('chest', tf(cylg(0.2 * sw, 0.15, 0.32, L.top, 12), 0, 0.08, 0, 0, 0, 0, 1, 1, 0.78));
  add('chest', tf(sph(0.2 * sw, L.top, 12, 6), 0, 0.2, 0, 0, 0, 0, 1, 0.45, 0.72));
  if (fem) add('chest', tf(sph(0.15, L.top, 10, 6), 0, 0.09, 0.04, 0, 0, 0, 1.1, 0.7, 0.75));
  // Trim down the front of the tunic.
  add('chest', tf(boxg(0.07, 0.3, 0.02, L.trim), 0, 0.08, 0.12));
  // Tunic skirt flaring over the hips (or a long dress).
  const skirtLen = L.skirt ? 0.62 : 0.3;
  add('hips', tf(cylg(0.165, L.skirt ? 0.3 : 0.22, skirtLen, L.skirt ? L.top : L.top, 12, true), 0, 0.03 - skirtLen / 2, 0, 0, 0, 0, 1, 1, 0.85));
  add('hips', tf(cylg(0.165, L.skirt ? 0.3 : 0.22, skirtLen, L.top, 12, true), 0, 0.03 - skirtLen / 2, 0, 0, 0, 0, 0.97, 1, 0.82));
  if (L.apron) add('hips', tf(boxg(0.24, 0.45, 0.02, L.apron), 0, -0.15, 0.2));
  // Neck & head.
  add('neck', tf(cylg(0.05, 0.055, 0.12, L.skin), 0, 0.04, 0));
  add('head', tf(sph(0.15, L.skin, 16, 12), 0, 0.14, 0, 0, 0, 0, 1, 1.06, 1.02));
  add('head', tf(sph(0.035, L.skin, 6, 4), 0.145, 0.13, 0, 0, 0, 0, 0.5, 1, 1));
  add('head', tf(sph(0.035, L.skin, 6, 4), -0.145, 0.13, 0, 0, 0, 0, 0.5, 1, 1));
  add('head', tf(sph(0.015, 0xe8a8a0, 6, 4), 0, 0.07, 0.145, 0, 0, 0, 1.6, 0.5, 0.6)); // mouth blush
  // Eyes (own bone so they can blink).
  for (const s of [1, -1]) {
    add('eyes', tf(sph(0.034, 0x1a1420, 10, 8), s * 0.058, 0, 0.012, 0, 0, 0, 0.75, 1.1, 0.4));
    add('eyes', tf(sph(0.022, L.eyes, 8, 6), s * 0.058, -0.008, 0.024, 0, 0, 0, 0.8, 1.0, 0.3));
    add('eyes', tf(sph(0.009, 0xffffff, 6, 4), s * 0.05 + 0.006, 0.014, 0.028));
    add('head', tf(boxg(0.06, 0.012, 0.02, L.hair), s * 0.06, 0.205, 0.125, 0, 0, s * -0.12));
  }
  // Arms.
  for (const s of [1, -1]) {
    const side = s > 0 ? 'L' : 'R';
    add('arm' + side, tf(sph(0.07, L.top), 0, -0.01, 0));
    add('arm' + side, tf(capsule(0.052, 0.18, L.top), 0, -0.13, 0));
    add('forearm' + side, tf(capsule(0.045, 0.16, L.skin), 0, -0.11, 0));
    add('forearm' + side, tf(cylg(0.055, 0.05, 0.1, L.gloves), 0, -0.19, 0));
    add('hand' + side, tf(sph(0.05, L.gloves, 8, 6), 0, -0.05, 0.005, 0, 0, 0, 0.85, 1.1, 0.9));
    if (L.pauldrons) add('arm' + side, tf(sph(0.1, L.trim, 10, 6, ), s * 0.015, 0.02, 0, 0, 0, 0, 1, 0.7, 1));
  }
  // Scarf.
  if (L.scarf) {
    add('neck', tf(colorGeo(new THREE.TorusGeometry(0.085, 0.035, 6, 12), L.scarf), 0, -0.02, 0, Math.PI / 2, 0, 0));
    add('chest', tf(boxg(0.07, 0.24, 0.025, L.scarf), -0.06, 0.12, -0.17, 0.25, 0, 0.12));
  }
  // Cape.
  if (L.cape) add('cape', tf(colorGeo(new THREE.PlaneGeometry(0.42, 0.85, 1, 4), L.cape), 0, -0.42, -0.01, 0, Math.PI, 0));
  if (L.cape) add('cape', tf(colorGeo(new THREE.PlaneGeometry(0.42, 0.85, 1, 4), L.cape), 0, -0.42, 0.0));
  // Hair.
  const hc = L.hair;
  const style = L.hairStyle;
  if (style !== 'bald') add('head', tf(sph(0.158, hc, 14, 8), 0, 0.16, -0.01, -0.25, 0, 0, 1.02, 0.95, 1.05));
  if (style === 'spiky') {
    const spikes = [[0, 0.32, -0.02, -0.4, 0, 0], [0.1, 0.29, -0.05, -0.6, 0, -0.7], [-0.1, 0.29, -0.05, -0.6, 0, 0.7], [0.05, 0.25, -0.15, -1.3, 0, -0.4], [-0.05, 0.25, -0.15, -1.3, 0, 0.4], [0, 0.18, -0.17, -1.9, 0, 0], [0.13, 0.2, -0.08, -1.0, 0, -1.2], [-0.13, 0.2, -0.08, -1.0, 0, 1.2]];
    for (const [x, y, z, rx, ry, rz] of spikes) add('head', tf(coneg(0.065, 0.2, hc, 5), x, y, z, rx, ry, rz));
    for (const [x, rz] of [[0.07, 0.25], [-0.02, -0.1], [-0.09, -0.35], [0.12, 0.6]]) add('head', tf(coneg(0.04, 0.14, hc, 4), x, 0.23, 0.12, 2.6, 0, rz));
  } else if (style === 'bob' || style === 'long' || style === 'ponytail' || style === 'twin') {
    for (const [x, rz] of [[0.06, 0.2], [-0.02, -0.15], [-0.08, -0.3], [0.1, 0.4]]) add('head', tf(coneg(0.045, 0.14, hc, 4), x, 0.24, 0.12, 2.75, 0, rz));
    add('head', tf(sph(0.165, hc, 12, 8), 0, 0.1, -0.03, 0, 0, 0, 1.0, 0.9, 1.0));
    if (style === 'long') add('head', tf(boxg(0.28, 0.42, 0.08, hc), 0, -0.08, -0.11, 0.1, 0, 0));
    if (style === 'ponytail') {
      add('head', tf(sph(0.05, hc), 0, 0.2, -0.16));
      add('head', tf(coneg(0.07, 0.38, hc, 6), 0, 0.02, -0.22, 2.8, 0, 0));
    }
    if (style === 'twin') for (const s of [1, -1]) add('head', tf(coneg(0.06, 0.34, hc, 6), s * 0.15, 0.02, -0.08, 3.0, 0, s * 0.3));
  } else if (style === 'short') {
    for (const [x, rz] of [[0.05, 0.2], [-0.05, -0.2]]) add('head', tf(coneg(0.05, 0.1, hc, 4), x, 0.26, 0.11, 2.4, 0, rz));
  }
  if (L.beard) add('head', tf(sph(0.1, L.beardColor ?? hc, 10, 6), 0, 0.05, 0.08, 0, 0, 0, 1.15, 0.9, 0.85));
  // Hats.
  if (L.hat === 'wizard') {
    add('head', tf(cylg(0.27, 0.27, 0.025, L.hatColor ?? 0x3a3a7a, 16), 0, 0.27, 0));
    add('head', tf(coneg(0.16, 0.42, L.hatColor ?? 0x3a3a7a, 10), 0, 0.48, -0.03, -0.2, 0, 0));
  } else if (L.hat === 'hood') {
    add('head', tf(sph(0.19, L.hatColor ?? 0x7a2a2a, 12, 8), 0, 0.15, -0.03, 0, 0, 0, 1.05, 1.1, 1.1));
    add('head', tf(coneg(0.12, 0.2, L.hatColor ?? 0x7a2a2a, 6), 0, 0.17, -0.2, -1.9, 0, 0));
    add('head', tf(boxg(0.26, 0.08, 0.04, 0x2a1a1a), 0, 0.06, 0.15)); // mask
  } else if (L.hat === 'cap') {
    add('head', tf(sph(0.165, L.hatColor ?? 0x6a4a2a, 12, 6), 0, 0.2, 0, 0, 0, 0, 1, 0.6, 1));
    add('head', tf(cylg(0.24, 0.24, 0.02, L.hatColor ?? 0x6a4a2a, 14), 0, 0.22, 0.02));
  } else if (L.hat === 'helm') {
    add('head', tf(sph(0.18, 0x9aa0aa, 12, 8), 0, 0.17, -0.01, 0, 0, 0, 1, 0.95, 1.05));
    add('head', tf(boxg(0.04, 0.2, 0.06, 0x9aa0aa), 0, 0.2, 0.16));
  }
  return parts;
}

// Stone golem: blocky limbs, mossy shoulders, glowing crystal core.
function golemParts(L) {
  const st = L.stone ?? 0x8a8478, dk = L.stoneDark ?? 0x6a655c, glow = L.glow ?? 0x7dff9a, moss = L.moss ?? 0x6f9a4a;
  const parts = [];
  const add = (b, g) => parts.push([b, g]);
  const bx = (w, h, d, c) => boxg(w, h, d, c);
  add('hips', tf(bx(0.42, 0.24, 0.3, dk), 0, 0, 0));
  add('spine', tf(bx(0.4, 0.26, 0.3, st), 0, 0.1, 0));
  add('chest', tf(bx(0.62, 0.44, 0.42, st), 0, 0.12, 0));
  add('chest', tf(bx(0.66, 0.1, 0.46, moss), 0, 0.36, 0));
  add('chest', tf(colorGeo(new THREE.OctahedronGeometry(0.11, 0), glow), 0, 0.12, 0.22, 0, 0, 0, 1, 1.4, 0.6));
  add('neck', tf(bx(0.16, 0.1, 0.16, dk), 0, 0.03, 0));
  add('head', tf(bx(0.26, 0.24, 0.26, st), 0, 0.1, 0.02));
  add('eyes', tf(bx(0.16, 0.04, 0.02, glow), 0, -0.03, 0.02));
  for (const s of [1, -1]) {
    const side = s > 0 ? 'L' : 'R';
    add('arm' + side, tf(bx(0.26, 0.24, 0.26, moss), s * 0.03, 0.02, 0));
    add('arm' + side, tf(bx(0.17, 0.28, 0.17, st), 0, -0.14, 0));
    add('forearm' + side, tf(bx(0.2, 0.26, 0.2, dk), 0, -0.12, 0));
    add('hand' + side, tf(bx(0.24, 0.2, 0.22, st), 0, -0.06, 0));
    add('thigh' + side, tf(bx(0.2, 0.42, 0.2, st), 0, -0.2, 0));
    add('shin' + side, tf(bx(0.18, 0.4, 0.18, dk), 0, -0.2, 0));
    add('foot' + side, tf(bx(0.22, 0.1, 0.3, st), 0, -0.02, 0.04));
  }
  return parts;
}

// Ash revenant: a gaunt skeleton with smouldering eyes and a tattered cloak.
function revenantParts(L) {
  const bone = 0xd8d0c0, cloth = L.top ?? 0x3a2a30, glow = L.glow ?? 0xff8a3a;
  const parts = [];
  const add = (b, g) => parts.push([b, g]);
  add('hips', tf(sph(0.12, bone, 8, 6), 0, 0, 0, 0, 0, 0, 1.2, 0.6, 0.8));
  add('hips', tf(cylg(0.16, 0.24, 0.5, cloth, 8, true), 0, -0.2, 0));
  add('spine', tf(cylg(0.03, 0.03, 0.2, bone, 6), 0, 0.1, 0));
  for (let i = 0; i < 3; i++) add('chest', tf(colorGeo(new THREE.TorusGeometry(0.13 - i * 0.015, 0.018, 4, 10), bone), 0, 0.02 + i * 0.08, 0, Math.PI / 2, 0, 0, 1, 0.75, 1));
  add('chest', tf(cylg(0.2, 0.17, 0.22, cloth, 8, true), 0, 0.22, 0));
  add('neck', tf(cylg(0.025, 0.025, 0.1, bone, 5), 0, 0.04, 0));
  add('head', tf(sph(0.13, bone, 10, 8), 0, 0.13, 0, 0, 0, 0, 0.9, 1.0, 1.0));
  add('head', tf(boxg(0.14, 0.05, 0.08, bone), 0, 0.04, 0.06));
  add('head', tf(sph(0.16, cloth, 10, 6), 0, 0.17, -0.03, -0.3, 0, 0, 1.05, 1.05, 1.1));
  for (const s of [1, -1]) add('eyes', tf(sph(0.022, glow, 6, 4), s * 0.045, -0.01, 0.0));
  for (const s of [1, -1]) {
    const side = s > 0 ? 'L' : 'R';
    add('arm' + side, tf(capsule(0.025, 0.22, bone, 5), 0, -0.13, 0));
    add('forearm' + side, tf(capsule(0.022, 0.2, bone, 5), 0, -0.12, 0));
    add('hand' + side, tf(sph(0.035, bone, 6, 4), 0, -0.04, 0));
    add('thigh' + side, tf(capsule(0.03, 0.34, bone, 5), 0, -0.2, 0));
    add('shin' + side, tf(capsule(0.026, 0.34, bone, 5), 0, -0.2, 0));
    add('foot' + side, tf(boxg(0.07, 0.04, 0.16, bone), 0, -0.02, 0.04));
  }
  return parts;
}

// Build a rigidly-skinned mesh (+ optional outline) from bone defs and parts.
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
  for (const [bone, geo] of parts) {
    geo.applyMatrix4(bones[bone].matrixWorld);
    const n = geo.attributes.position.count;
    const si = new Uint16Array(n * 4), swt = new Float32Array(n * 4);
    for (let i = 0; i < n; i++) { si[i * 4] = index[bone]; swt[i * 4] = 1; }
    geo.setAttribute('skinIndex', new THREE.Uint16BufferAttribute(si, 4));
    geo.setAttribute('skinWeight', new THREE.Float32BufferAttribute(swt, 4));
    geos.push(geo);
  }
  const geometry = mergeGeometries(geos);
  geos.forEach((g) => g.dispose());
  const skeleton = new THREE.Skeleton(boneList);
  const mesh = new THREE.SkinnedMesh(geometry, opts.material ?? makeToonMaterial());
  mesh.add(root);
  mesh.bind(skeleton);
  mesh.castShadow = true;
  mesh.boundingSphere = new THREE.Sphere(new THREE.Vector3(0, opts.centerY ?? 1, 0), opts.radius ?? 1.6);
  let outline = null;
  if (opts.outline !== false) {
    outline = new THREE.SkinnedMesh(geometry, opts.outlineMaterial ?? makeOutlineMaterial(opts.outlineWidth ?? 0.011));
    outline.bind(skeleton, mesh.bindMatrix);
    outline.boundingSphere = mesh.boundingSphere;
  }
  const base = {};
  for (const b of boneList) base[b.name] = b.position.clone();
  return { bones, boneList, skeleton, mesh, outline, base, geometry };
}

export class Character {
  constructor(look = {}, opts = {}) {
    this.look = { ...DEFAULT_LOOK, ...look };
    this.group = new THREE.Group();
    const parts = this.look.kind === 'golem' ? golemParts(this.look) : this.look.kind === 'revenant' ? revenantParts(this.look) : buildParts(this.look);
    const r = buildSkinned(BONES, parts, opts);
    this.bones = r.bones; this.skeleton = r.skeleton; this.mesh = r.mesh; this.outline = r.outline; this.base = r.base;
    this.group.add(this.mesh);
    if (this.outline) this.group.add(this.outline);
    const scale = this.look.height;
    this.group.scale.set(this.look.build * scale, scale, this.look.build * scale);
    this.anim = new Animator(this);
    this.weapon = null;
    this.weaponDrawn = false;
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
      w.position.set(0, -0.06, 0.02);
      w.rotation.set(1.75, 0, 0);
    } else {
      this.bones.chest.add(w);
      w.position.set(0.05, 0.05, -0.2);
      w.rotation.set(0.15, 0, 2.5);
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
  }
}

// Weapon models (non-skinned, parented to the hand bone).
export function makeSword(kind = 'iron') {
  const looks = {
    rusty: { blade: 0x9a8a7a, guard: 0x6a5040, len: 0.75 },
    iron: { blade: 0xc8d0dc, guard: 0x6a5a4a, len: 0.85 },
    steel: { blade: 0xe0e8f4, guard: 0xc8a040, len: 0.95 },
    crystal: { blade: 0x8ff0ff, guard: 0xb48cff, len: 1.05, glow: true },
    flame: { blade: 0xff8a3a, guard: 0x5a2a20, len: 1.0, glow: true },
    frost: { blade: 0xbfe8ff, guard: 0x4a6a9a, len: 1.0, glow: true },
  };
  const s = looks[kind] || looks.iron;
  const parts = [
    tf(cylg(0.022, 0.022, 0.2, 0x3a2418, 6), 0, 0.0, 0),
    tf(boxg(0.26, 0.04, 0.06, s.guard), 0, 0.11, 0),
    tf(sph(0.03, s.guard, 6, 4), 0, -0.11, 0),
    tf(boxg(0.07, s.len, 0.016, s.blade), 0, 0.13 + s.len / 2, 0),
    tf(coneg(0.05, 0.12, s.blade, 4), 0, 0.13 + s.len + 0.05, 0, 0, Math.PI / 4, 0, 1, 1, 0.25),
  ];
  const g = mergeGeometries(parts);
  const mat = s.glow
    ? new THREE.MeshToonMaterial({ vertexColors: true, gradientMap: getToonRamp(), emissive: new THREE.Color(s.blade), emissiveIntensity: 0.6 })
    : makeToonMaterial();
  const m = new THREE.Mesh(g, mat);
  m.castShadow = true;
  m.userData.length = s.len + 0.25;
  return m;
}

export function makeStaff(color = 0x9ff0ff) {
  const g = mergeGeometries([
    tf(cylg(0.025, 0.03, 1.5, 0x6a4a2a, 6), 0, 0.3, 0),
    tf(colorGeo(new THREE.OctahedronGeometry(0.09, 0), color), 0, 1.12, 0, 0, 0, 0, 1, 1.6, 1),
  ]);
  const m = new THREE.Mesh(g, new THREE.MeshToonMaterial({ vertexColors: true, gradientMap: getToonRamp(), emissive: new THREE.Color(color), emissiveIntensity: 0.3 }));
  return m;
}

export function makeTool(kind) {
  const parts = kind === 'hammer'
    ? [tf(cylg(0.02, 0.02, 0.45, 0x6a4a2a, 6), 0, 0.15, 0), tf(boxg(0.14, 0.08, 0.08, 0x4a4a52), 0, 0.38, 0)]
    : kind === 'book'
      ? [tf(boxg(0.18, 0.04, 0.24, 0x7a3a3a), 0, 0, 0), tf(boxg(0.17, 0.042, 0.23, 0xf4ead6), 0.005, 0, 0)]
      : [tf(cylg(0.02, 0.02, 1.4, 0x6a4a2a, 6), 0, 0.3, 0), tf(coneg(0.05, 0.14, 0xb0b8c0, 4), 0, 1.07, 0)];
  return new THREE.Mesh(mergeGeometries(parts), makeToonMaterial());
}

// ------------------------------------------------------------------ animator
const ZERO = { x: 0, y: 0, z: 0 };

export class Animator {
  constructor(ch) {
    this.ch = ch;
    this.phase = 0;
    this.t = 0;
    this.target = {};
    for (const k of Object.keys(ch.bones)) this.target[k] = { x: 0, y: 0, z: 0 };
    this.hipY = 0;
    this.blinkT = 2;
    this.lookYaw = 0;
  }

  update(dt, s) {
    this.t += dt;
    const T = this.target;
    for (const k in T) { T[k].x = 0; T[k].y = 0; T[k].z = 0; }
    let hipY = 0;
    let rate = 12;
    const fast = new Set();

    const speed = s.speed || 0;
    const move = clamp(speed / 3, 0, 1);
    const run = clamp((speed - 3.5) / 3.5, 0, 1);
    const freq = speed > 0.15 ? 1.8 + speed * 0.62 : 0;
    this.phase += dt * freq * 2.2;
    const ph = this.phase, sw = Math.sin(ph), cw = Math.cos(ph);
    const breathe = Math.sin(this.t * 2.2);

    if (s.dead) {
      const k = clamp(s.deadT * 2.5, 0, 1);
      T.hips.x = -1.45 * k; hipY = -0.75 * k;
      T.armL.z = 1.2 * k; T.armR.z = -1.2 * k; T.thighL.x = -0.3 * k; T.shinL.x = 0.6 * k;
      T.head.y = 0.4 * k;
      this.apply(dt, 10, hipY, fast);
      return;
    }

    if (s.swim) {
      T.hips.x = 1.15; hipY = -0.55;
      T.neck.x = -0.7; T.head.x = -0.4;
      const p = this.t * 3.2;
      T.armL.x = -1.6 + Math.sin(p) * 1.4; T.armR.x = -1.6 + Math.sin(p + Math.PI) * 1.4;
      T.armL.z = 0.4; T.armR.z = -0.4;
      T.thighL.x = Math.sin(p * 1.5) * 0.4; T.thighR.x = -Math.sin(p * 1.5) * 0.4;
      T.shinL.x = 0.3; T.shinR.x = 0.3;
    } else if (s.glide) {
      T.hips.x = 1.0; hipY = 0;
      T.neck.x = -0.6; T.head.x = -0.4;
      T.armL.z = 1.45; T.armR.z = -1.45; T.armL.x = -0.2; T.armR.x = -0.2;
      T.forearmL.z = 0.1; T.forearmR.z = -0.1;
      T.thighL.x = 0.15 + Math.sin(this.t * 4) * 0.05; T.thighR.x = 0.25; T.shinL.x = 0.3; T.shinR.x = 0.4;
      T.cape.x = 1.2 + Math.sin(this.t * 9) * 0.1;
    } else if (!s.grounded) {
      const up = clamp(s.vy / 6, -1, 1);
      T.thighL.x = -0.9 + (1 - up) * 0.2; T.shinL.x = 1.3;
      T.thighR.x = -0.2; T.shinR.x = 0.5;
      T.armL.z = 0.5 + (1 - up) * 0.3; T.armR.z = -0.5 - (1 - up) * 0.3;
      T.armL.x = -0.4; T.armR.x = -0.3;
      T.spine.x = 0.1;
      T.cape.x = 0.4 - up * 0.3;
    } else {
      const legAmp = lerp(0.5, 0.95, run) * move;
      const armAmp = lerp(0.45, 0.9, run) * move;
      T.thighL.x = -sw * legAmp; T.thighR.x = sw * legAmp;
      T.shinL.x = Math.max(0, cw) * legAmp * 1.5 + 0.05; T.shinR.x = Math.max(0, -cw) * legAmp * 1.5 + 0.05;
      T.footL.x = -Math.max(0, -cw) * 0.3 * move; T.footR.x = -Math.max(0, cw) * 0.3 * move;
      T.armL.x = sw * armAmp; T.armR.x = -sw * armAmp;
      T.armL.z = 0.1 + run * 0.15; T.armR.z = -0.1 - run * 0.15;
      T.forearmL.x = -0.15 - run * 1.1 - move * 0.15; T.forearmR.x = -0.15 - run * 1.1 - move * 0.15;
      T.spine.x = run * 0.22 + move * 0.04;
      T.spine.y = sw * 0.14 * move; T.hips.y = -sw * 0.1 * move;
      T.head.y = -sw * 0.06 * move;
      T.spine.z = (s.lean || 0) * 0.25;
      hipY = -Math.abs(cw) * 0.05 * move - run * 0.05 + (move < 0.1 ? breathe * 0.004 : 0);
      T.chest.x = breathe * 0.025 * (1 - move);
      T.cape.x = 0.08 + speed * 0.07 + Math.sin(this.t * 6) * 0.03 * move;
      if (s.crouch) { hipY -= 0.3; T.thighL.x -= 0.8; T.thighR.x -= 0.8; T.shinL.x += 1.3; T.shinR.x += 1.3; T.spine.x += 0.3; }
      // Idle flavour.
      if (move < 0.1 && !s.action) {
        T.armL.z = 0.12 + breathe * 0.02; T.armR.z = -0.12 - breathe * 0.02;
        T.head.y = Math.sin(this.t * 0.4) * 0.25 * (s.lookAround ? 1 : 0.2) + (s.headYaw || 0);
        T.head.x = (s.headPitch || 0);
        if (s.pose === 'sit') { hipY = -0.48; T.thighL.x = -1.5; T.thighR.x = -1.5; T.shinL.x = 1.5; T.shinR.x = 1.5; T.armL.x = -0.5; T.armR.x = -0.5; T.forearmL.x = -0.6; T.forearmR.x = -0.6; }
        if (s.pose === 'work') { const k = Math.max(0, Math.sin(this.t * 4)); T.armR.x = -1.2 - k * 1.2; T.forearmR.x = -0.8 + k * 0.6; T.spine.x = 0.25; fast.add('armR'); fast.add('forearmR'); }
        if (s.pose === 'talk') { T.armL.x = -0.4 + Math.sin(this.t * 2.5) * 0.25; T.forearmL.x = -0.9; T.armL.z = 0.3; T.head.x = Math.sin(this.t * 3.1) * 0.05; }
        if (s.pose === 'read') { T.armL.x = -0.9; T.armR.x = -0.9; T.forearmL.x = -1.1; T.forearmR.x = -1.1; T.armL.z = -0.2; T.armR.z = 0.2; T.head.x = 0.35; }
        if (s.pose === 'guard') { T.armR.x = -0.3; T.forearmR.x = -1.2; T.armL.z = 0.05; }
        if (s.pose === 'cheer') { const k = Math.sin(this.t * 6); T.armL.z = 2.6 + k * 0.2; T.armR.z = -2.6 - k * 0.2; hipY += Math.max(0, k) * 0.06; }
      }
    }

    // ---------------- actions (upper body / full body overrides)
    const a = s.action, t = s.actionT || 0;
    if (a) rate = 22;
    if (a === 'attack1' || a === 'attack2' || a === 'attack3' || a === 'heavy') {
      const wind = a === 'heavy' ? 0.45 : 0.3, strike = a === 'heavy' ? 0.62 : 0.5;
      const w = smoothstep(0, wind, t) * (1 - smoothstep(strike, 1, t));
      const k = smoothstep(wind, strike, t);
      fast.add('armR'); fast.add('forearmR'); fast.add('spine'); fast.add('chest');
      if (a === 'attack1') {
        T.armR.x = lerp(-1.2, -1.45, k); T.armR.z = lerp(-1.1, 0.7, k); T.armR.y = lerp(-0.6, 0.5, k);
        T.forearmR.x = lerp(-0.9, -0.2, k);
        T.spine.y = lerp(-0.55, 0.65, k); T.chest.y = lerp(-0.2, 0.3, k);
        T.armL.x = -0.5; T.armL.z = 0.5;
      } else if (a === 'attack2') {
        T.armR.x = lerp(-0.6, -1.9, k); T.armR.z = lerp(0.9, -1.0, k); T.armR.y = lerp(0.6, -0.4, k);
        T.forearmR.x = lerp(-1.4, -0.3, k);
        T.spine.y = lerp(0.6, -0.6, k); T.chest.y = lerp(0.25, -0.3, k);
        T.armL.x = -0.3; T.armL.z = 0.6;
      } else {
        T.armR.x = lerp(-2.9, -0.7, k); T.armR.z = lerp(-0.2, 0.1, k);
        T.forearmR.x = lerp(-0.7, -0.1, k);
        T.armL.x = lerp(-2.6, -0.8, k); T.armL.z = lerp(-0.4, -0.2, k); T.forearmL.x = -0.6;
        T.spine.x = lerp(-0.3, 0.45, k); hipY += lerp(0.05, -0.18, k);
        T.thighL.x -= 0.5 * k; T.shinL.x += 0.5 * k; T.thighR.x += 0.35 * k;
        fast.add('armL');
      }
      for (const b of ['armR', 'forearmR', 'spine', 'chest', 'armL', 'forearmL']) {
        T[b].x *= w; T[b].y *= w; T[b].z *= w;
      }
      // Lunge stance.
      T.thighL.x += -0.35 * w; T.shinL.x += 0.25 * w; T.thighR.x += 0.3 * w;
    } else if (a === 'cast') {
      const w = smoothstep(0, 0.25, t) * (1 - smoothstep(0.7, 1, t));
      T.armL.x = -1.55 * w; T.armL.z = 0.1 * w; T.forearmL.x = -0.1 * w; T.handL.x = -0.6 * w;
      T.armR.x = -0.4 * w; T.armR.z = -0.4 * w;
      T.spine.y = 0.35 * w; T.chest.y = 0.15 * w;
      fast.add('armL'); fast.add('forearmL');
    } else if (a === 'castUp') {
      const w = smoothstep(0, 0.3, t) * (1 - smoothstep(0.75, 1, t));
      T.armL.z = 2.7 * w; T.armR.z = -2.7 * w; T.head.x = -0.4 * w; T.spine.x = -0.15 * w;
      fast.add('armL'); fast.add('armR');
    } else if (a === 'block') {
      T.armR.x = -1.3; T.armR.z = 0.5; T.forearmR.x = -1.2; T.armR.y = 0.5;
      T.armL.x = -1.1; T.armL.z = -0.3; T.forearmL.x = -1.3;
      T.spine.x += 0.15; T.thighL.x += -0.25; T.shinL.x += 0.3; T.thighR.x += 0.25;
      fast.add('armR'); fast.add('armL'); fast.add('forearmR'); fast.add('forearmL');
    } else if (a === 'dodge') {
      const k = smoothstep(0, 1, t);
      T.hips.x = k * Math.PI * 2; hipY += -0.5 * Math.sin(Math.PI * t);
      T.thighL.x = -1.6; T.thighR.x = -1.6; T.shinL.x = 2.0; T.shinR.x = 2.0;
      T.armL.x = -1.2; T.armR.x = -1.2; T.forearmL.x = -1.4; T.forearmR.x = -1.4;
      T.spine.x = 0.6; T.neck.x = 0.4;
      rate = 40; fast.add('hips');
    } else if (a === 'hit') {
      const w = 1 - smoothstep(0.2, 1, t);
      T.spine.x = -0.35 * w; T.head.x = -0.3 * w; T.armL.z = 0.6 * w; T.armR.z = -0.6 * w;
    } else if (a === 'interact') {
      const w = Math.sin(Math.PI * t);
      T.armR.x = -1.2 * w; T.forearmR.x = -0.4 * w; T.spine.x = 0.35 * w; hipY -= 0.12 * w;
    } else if (a === 'drink') {
      const w = Math.sin(Math.PI * Math.min(t * 1.4, 1));
      T.armR.x = -1.6 * w; T.forearmR.x = -1.9 * w; T.head.x = -0.4 * w;
    }

    if (s.headYaw && !a && move > 0.1) T.head.y += s.headYaw;

    // Blink.
    this.blinkT -= dt;
    const eyes = this.ch.bones.eyes;
    if (this.blinkT < 0) { eyes.scale.y = 0.1; if (this.blinkT < -0.1) this.blinkT = 2 + Math.random() * 3; }
    else eyes.scale.y = 1;

    this.apply(dt, rate, hipY, fast, a === 'dodge');
  }

  apply(dt, rate, hipY, fast, snap = false) {
    const T = this.target, B = this.ch.bones;
    for (const k in T) {
      const b = B[k], tg = T[k];
      const r = fast.has(k) ? rate * 1.6 : rate;
      if (snap && k === 'hips') { b.rotation.x = tg.x; b.rotation.y = damp(b.rotation.y, tg.y, r, dt); b.rotation.z = damp(b.rotation.z, tg.z, r, dt); continue; }
      if (k === 'hips') b.rotation.x = Math.atan2(Math.sin(b.rotation.x), Math.cos(b.rotation.x));
      b.rotation.x = damp(b.rotation.x, tg.x, r, dt);
      b.rotation.y = damp(b.rotation.y, tg.y, r, dt);
      b.rotation.z = damp(b.rotation.z, tg.z, r, dt);
    }
    this.hipY = damp(this.hipY, hipY, rate, dt);
    B.hips.position.y = this.ch.base.hips.y + this.hipY;
  }
}

export { ZERO };
