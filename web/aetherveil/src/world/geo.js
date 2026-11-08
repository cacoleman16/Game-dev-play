// Geometry kit: build vertex-coloured primitives, place them, and merge many
// into one mesh. Buildings, trees, props and creatures are all made this way,
// so the whole game ships without a single model file.

import * as THREE from 'three';
import { mergeGeometries } from 'three/addons';
import { hash2 } from '../core/noise.js';

const _c = new THREE.Color();
const _m = new THREE.Matrix4();
const _q = new THREE.Quaternion();
const _e = new THREE.Euler();
const _s = new THREE.Vector3();
const _p = new THREE.Vector3();

// Make a geometry non-indexed with position/normal/color only.
export function prep(geo) {
  let g = geo.index ? geo.toNonIndexed() : geo;
  if (g !== geo) geo.dispose();
  for (const k of Object.keys(g.attributes)) if (k !== 'position' && k !== 'normal' && k !== 'color') g.deleteAttribute(k);
  if (!g.attributes.normal) g.computeVertexNormals();
  return g;
}

// colour: hex, or fn(x, y, z, nx, ny, nz) -> hex/Color
export function paint(geo, color, vary = 0, seed = 0) {
  geo = prep(geo);
  const pos = geo.attributes.position, nor = geo.attributes.normal;
  const arr = new Float32Array(pos.count * 3);
  const fixed = typeof color === 'function' ? null : _c.clone().set(color);
  for (let i = 0; i < pos.count; i++) {
    let c;
    if (fixed) c = _c.copy(fixed);
    else {
      const r = color(pos.getX(i), pos.getY(i), pos.getZ(i), nor.getX(i), nor.getY(i), nor.getZ(i));
      c = r instanceof THREE.Color ? _c.copy(r) : _c.set(r);
    }
    if (vary) {
      const h = hash2(Math.round(pos.getX(i) * 7), Math.round(pos.getY(i) * 7 + pos.getZ(i) * 13), seed) - 0.5;
      c.offsetHSL(0, 0, h * vary);
    }
    arr[i * 3] = c.r; arr[i * 3 + 1] = c.g; arr[i * 3 + 2] = c.b;
  }
  geo.setAttribute('color', new THREE.BufferAttribute(arr, 3));
  return geo;
}

// Position/rotate/scale a geometry in place. rot = [x, y, z] radians.
export function place(geo, x = 0, y = 0, z = 0, rot = null, scl = null) {
  _e.set(rot ? rot[0] : 0, rot ? rot[1] : 0, rot ? rot[2] : 0);
  _q.setFromEuler(_e);
  if (scl === null) _s.set(1, 1, 1);
  else if (typeof scl === 'number') _s.set(scl, scl, scl);
  else _s.set(scl[0], scl[1], scl[2]);
  _m.compose(_p.set(x, y, z), _q, _s);
  geo.applyMatrix4(_m);
  return geo;
}

// Push vertices around consistently (same position -> same offset) for organic shapes.
export function lumpy(geo, amount, freq = 1.7, seed = 1) {
  const pos = geo.attributes.position;
  for (let i = 0; i < pos.count; i++) {
    const x = pos.getX(i), y = pos.getY(i), z = pos.getZ(i);
    const kx = Math.round(x * 1000), ky = Math.round(y * 1000), kz = Math.round(z * 1000);
    const h1 = hash2(kx + kz * 7, ky, seed) - 0.5;
    const h2 = hash2(ky + kx * 3, kz, seed + 1) - 0.5;
    const h3 = hash2(kz + ky * 5, kx, seed + 2) - 0.5;
    pos.setXYZ(i, x + h1 * amount, y + h2 * amount, z + h3 * amount);
  }
  geo.computeVertexNormals();
  return geo;
}

export function merge(list) {
  const g = mergeGeometries(list.map(prep), false);
  for (const x of list) x.dispose();
  return g;
}

// ---- primitives ----
export const box = (w, h, d, color, vary) => paint(new THREE.BoxGeometry(w, h, d), color, vary);
export const cyl = (rt, rb, h, seg, color, vary) => paint(new THREE.CylinderGeometry(rt, rb, h, seg), color, vary);
export const cone = (r, h, seg, color, vary) => paint(new THREE.ConeGeometry(r, h, seg), color, vary);
export const ico = (r, detail, color, vary) => paint(new THREE.IcosahedronGeometry(r, detail), color, vary);
export const sphere = (r, ws, hs, color, vary) => paint(new THREE.SphereGeometry(r, ws, hs), color, vary);
export const dodeca = (r, color, vary) => paint(new THREE.DodecahedronGeometry(r, 0), color, vary);
export const torus = (r, t, rs, ts, color, arc) => paint(new THREE.TorusGeometry(r, t, rs, ts, arc), color);

// Gable roof prism: width along x, depth along z, peak height h. Base at y=0.
export function gable(w, d, h, color, overhang = 0.3) {
  const W = w / 2 + overhang, D = d / 2 + overhang;
  const v = [
    // left slope
    -W, 0, -D, 0, h, -D, 0, h, D,
    -W, 0, -D, 0, h, D, -W, 0, D,
    // right slope
    W, 0, -D, W, 0, D, 0, h, D,
    W, 0, -D, 0, h, D, 0, h, -D,
    // front & back gables
    -W, 0, D, 0, h, D, W, 0, D,
    -W, 0, -D, W, 0, -D, 0, h, -D,
    // underside
    -W, 0, -D, -W, 0, D, W, 0, D,
    -W, 0, -D, W, 0, D, W, 0, -D,
  ];
  // Flip winding so faces point outward.
  for (let i = 0; i < v.length; i += 9) {
    for (let k = 0; k < 3; k++) { const t = v[i + 3 + k]; v[i + 3 + k] = v[i + 6 + k]; v[i + 6 + k] = t; }
  }
  const g = new THREE.BufferGeometry();
  g.setAttribute('position', new THREE.Float32BufferAttribute(v, 3));
  g.computeVertexNormals();
  return paint(g, color);
}

// Elongated octahedron, the iconic crystal shape.
export function crystal(r, h, color, sides = 6) {
  const g = new THREE.CylinderGeometry(r, r, h * 0.6, sides, 1, true);
  const top = new THREE.ConeGeometry(r, h * 0.25, sides);
  top.translate(0, h * 0.3 + h * 0.125, 0);
  const bot = new THREE.ConeGeometry(r, h * 0.15, sides);
  bot.rotateX(Math.PI);
  bot.translate(0, -h * 0.3 - h * 0.075, 0);
  const m = mergeGeometries([prep(g), prep(top), prep(bot)]);
  const n = m.toNonIndexed ? m : m;
  n.computeVertexNormals();
  return paint(n, color);
}

export function colorAt(geo) {
  return geo.attributes.color;
}
