// Creature models. Quadrupeds (wolves, boars) are smooth skinned sweeps with
// manes and tails; gloops are glossy jellies; skyrays glide on flapping wings;
// wisps and the Hollow Herald float. Humanoid foes (bandits, archers, golems,
// treants, revenants) reuse Character. Each model exposes `group` and
// `animate(dt, state)`.

import * as THREE from 'three';
import { mergeGeometries } from 'three/addons';
import { buildSkinned, makeToonMaterial, makeOutlineMaterial, Character, makeSword, makeStaff, colorGeo, tf, sph, cylg, coneg, boxg, torusg } from './character.js';
import { tube, strand } from './body.js';
import { makeGlowMaterial } from '../world/vegetation.js';
import { damp, clamp, lerp, smoothstep } from '../core/util.js';
import { crystal } from '../world/geo.js';

const glowMat = makeGlowMaterial(1.0);
const V = (x, y, z) => new THREE.Vector3(x, y, z);
const withGlow = (g, k = 0) => { g.setAttribute('aGlow', new THREE.BufferAttribute(new Float32Array(g.attributes.position.count).fill(k), 1)); return g; };

// ---------------------------------------------------------------- gloop
export class GloopModel {
  constructor(color = 0x6fd86a, scale = 1, outline = true) {
    this.group = new THREE.Group();
    const c = new THREE.Color(color);
    const light = c.clone().lerp(new THREE.Color(0xffffff), 0.35);
    const deep = c.clone().multiplyScalar(0.62);
    const body = colorGeo(new THREE.SphereGeometry(0.55, 24, 16), (x, y) => deep.clone().lerp(light, clamp((y + 0.4) / 0.9, 0, 1)));
    tf(body, 0, 0.45, 0, 0, 0, 0, 1, 0.82, 1);
    // Sag: widen the base so it sits like jelly.
    const p = body.attributes.position;
    for (let i = 0; i < p.count; i++) { const y = p.getY(i); if (y < 0.3) { const k = 1 + (0.3 - y) * 0.35; p.setX(i, p.getX(i) * k); p.setZ(i, p.getZ(i) * k); } }
    body.computeVertexNormals();
    const parts = [withGlow(body)];
    // Glossy highlight and a darker core suspended inside.
    parts.push(withGlow(tf(sph(0.12, 0xffffff, 12, 8), -0.18, 0.72, 0.28, 0, 0, 0.5, 1.3, 0.55, 0.6), 0.6));
    parts.push(withGlow(tf(sph(0.05, 0xffffff, 8, 6), -0.03, 0.8, 0.33, 0, 0, 0, 1, 0.6, 0.6), 0.6));
    for (const s of [1, -1]) {
      parts.push(withGlow(tf(sph(0.085, 0x16101c, 12, 8), s * 0.17, 0.55, 0.47, 0, 0, 0, 0.8, 1.15, 0.45)));
      parts.push(withGlow(tf(sph(0.03, 0xffffff, 8, 6), s * 0.15, 0.6, 0.51), 0.5));
    }
    parts.push(withGlow(tf(colorGeo(new THREE.TorusGeometry(0.05, 0.012, 6, 12, Math.PI), 0x16101c), 0, 0.44, 0.52, 0, 0, Math.PI)));
    const geo = mergeGeometries(parts);
    this.body = new THREE.Mesh(geo, makeToonMaterial({ rim: 0.5 }));
    this.body.castShadow = true;
    this.inner = new THREE.Group();
    this.inner.add(this.body);
    if (outline) this.inner.add(new THREE.Mesh(geo, makeOutlineMaterial(0.02)));
    this.group.add(this.inner);
    this.group.scale.setScalar(scale);
    this.t = Math.random() * 10;
    this.wob = 0; this.wobV = 0;
  }
  animate(dt, s) {
    this.t += dt * (s.speed > 0.5 ? 8 : 2.5);
    const b = Math.abs(Math.sin(this.t));
    let sy = 1 - 0.1 * (1 - b), sxz = 1 + 0.07 * (1 - b), y = s.speed > 0.5 ? b * 0.32 : 0;
    if (s.action === 'windup') { sy = 0.68; sxz = 1.25; y = 0; }
    if (s.action === 'strike') { sy = 1.3; sxz = 0.85; }
    if (s.action === 'stagger') { sy = 0.8; sxz = 1.2; }
    if (s.dead) { sy = Math.max(0.05, 1 - s.deadT * 2); sxz = 1 + s.deadT * 1.5; y = 0; }
    // Jelly wobble spring.
    const a = (sy - this.wob) * 180 - this.wobV * 9;
    this.wobV += a * dt; this.wob += this.wobV * dt;
    const k = this.wob;
    this.inner.scale.set(sxz + (1 - k) * 0.4, k, sxz + (1 - k) * 0.4);
    this.inner.position.y = y;
  }
}

// ---------------------------------------------------------------- quadrupeds
const QUAD_BONES = [
  ['body', null, [0, 0.72, -0.1]],
  ['chest', 'body', [0, 0.04, 0.42]],
  ['neck', 'chest', [0, 0.1, 0.18]],
  ['head', 'neck', [0, 0.14, 0.12]],
  ['jaw', 'head', [0, -0.07, 0.08]],
  ['tail1', 'body', [0, 0.08, -0.3]],
  ['tail2', 'tail1', [0, 0, -0.26]],
  ['flL', 'chest', [0.13, -0.08, 0.06]], ['flL2', 'flL', [0, -0.3, -0.04]], ['flL3', 'flL2', [0, -0.27, 0.03]],
  ['flR', 'chest', [-0.13, -0.08, 0.06]], ['flR2', 'flR', [0, -0.3, -0.04]], ['flR3', 'flR2', [0, -0.27, 0.03]],
  ['blL', 'body', [0.13, -0.04, -0.22]], ['blL2', 'blL', [0, -0.27, 0.08]], ['blL3', 'blL2', [0, -0.25, -0.16]],
  ['blR', 'body', [-0.13, -0.04, -0.22]], ['blR2', 'blR', [0, -0.27, 0.08]], ['blR3', 'blR2', [0, -0.25, -0.16]],
];
const QI = Object.fromEntries(QUAD_BONES.map((b, i) => [b[0], i]));
const QB = {};
for (const [n, p, o] of QUAD_BONES) { QB[n] = V(...o); if (p) QB[n].add(QB[p]); }

const keyed = (table, s) => {
  if (s <= table[0][0]) return table[0].slice(1);
  for (let i = 0; i < table.length - 1; i++) {
    const a = table[i], b = table[i + 1];
    if (s <= b[0]) { const t = smoothstep(a[0], b[0], s); return a.slice(1).map((v, k) => lerp(v, b[k + 1], t)); }
  }
  return table[table.length - 1].slice(1);
};

function quadParts(o) {
  const boar = o.kind === 'boar';
  const fur = new THREE.Color(o.fur ?? 0x5a5a66), belly = new THREE.Color(o.belly ?? 0x9a98a0), dark = new THREE.Color(o.dark ?? 0x3a3a44);
  const P = [];
  const pre = (g) => P.push(['@', g]);
  const add = (b, g) => P.push([b, g]);
  // Body sweep (rear -> chest). Ring "x" is vertical, "z" lateral for a horizontal chain.
  const bodyK = boar
    ? [[0, 0.17, 0.15, 0], [0.3, 0.22, 0.19, 0.0], [0.6, 0.23, 0.2, -0.01], [0.85, 0.24, 0.19, -0.02], [1, 0.18, 0.15, 0]]
    : [[0, 0.16, 0.14, 0], [0.22, 0.19, 0.16, -0.01], [0.5, 0.155, 0.14, 0.04], [0.78, 0.28, 0.19, -0.08], [1, 0.22, 0.16, -0.03]];
  pre(tube([{ p: V(0, 0.79, -0.44), bone: QI.body }, { p: V(0, 0.74, -0.08), bone: QI.chest }, { p: V(0, 0.77, 0.25), bone: QI.chest }, { p: V(0, 0.8, 0.42), bone: QI.chest }],
    (s) => { const [rv, rl, ov] = keyed(bodyK, s); return { rx: rv, rz: rl, ox: ov }; },
    (s, a) => { const up = Math.cos(a); return up > 0.2 ? fur.clone().lerp(dark, up * 0.4) : up < -0.4 ? belly : fur.clone().lerp(belly, (0.2 - up) / 0.6); },
    { radial: 16, rings: 4, capStart: true, ref: V(1, 0, 0) }));
  // Neck.
  pre(tube([{ p: V(0, 0.8, 0.36), bone: QI.chest }, { p: V(0, 0.88, 0.52), bone: QI.neck }, { p: V(0, 0.98, 0.62), bone: QI.head }],
    (s) => ({ rx: lerp(boar ? 0.19 : 0.19, boar ? 0.14 : 0.11, s), rz: lerp(boar ? 0.17 : 0.16, boar ? 0.13 : 0.1, s) }),
    (s, a) => (Math.cos(a) < -0.5 ? belly : fur), { radial: 12, rings: 3, ref: V(1, 0, 0) }));
  // Head: skull, snout, nose, ears, eyes.
  const hb = QB.head;
  add('head', tf(colorGeo(new THREE.SphereGeometry(boar ? 0.16 : 0.15, 16, 12), (x, y) => (y < -0.04 ? belly : fur)), 0, 0.02, 0.0, 0, 0, 0, 1.0, 0.86, 1.1));
  const sn0 = hb.clone().add(V(0, -0.01, 0.07)), sn2 = hb.clone().add(V(0, boar ? -0.06 : -0.045, boar ? 0.26 : 0.31));
  pre(strand(sn0, sn0.clone().lerp(sn2, 0.5).add(V(0, 0.012, 0)), sn2, boar ? 0.085 : 0.07, { root: fur, tip: boar ? new THREE.Color(0xc89a8a) : dark, thick: 0.85, taper: 0.45, bone: QI.head, center: hb.clone().add(V(0, -0.1, 0.1)), segs: 5, radial: 8 }));
  add('head', withGlow(tf(sph(boar ? 0.045 : 0.032, boar ? 0xc8848a : 0x16101a, 8, 6), 0, boar ? -0.06 : -0.04, boar ? 0.3 : 0.335, 0, 0, 0, 1.15, 0.85, 1)));
  for (const s of [1, -1]) {
    if (boar) {
      add('head', tf(colorGeo(new THREE.ConeGeometry(0.035, 0.12, 6), 0x5a3a2a), s * 0.08, 0.13, -0.02, -0.3, 0, s * -0.5));
      pre(strand(hb.clone().add(V(s * 0.05, -0.08, 0.22)), hb.clone().add(V(s * 0.09, -0.06, 0.28)), hb.clone().add(V(s * 0.08, 0.04, 0.3)), 0.022, { root: 0xf0e8d8, tip: 0xffffff, thick: 1, taper: 0.9, bone: QI.head, center: hb.clone(), segs: 4, radial: 6 }));
    } else {
      add('head', tf(colorGeo(new THREE.ConeGeometry(0.055, 0.17, 6), (x, y) => (y > 0.02 ? dark : fur)), s * 0.08, 0.15, -0.04, -0.2, 0, s * -0.3, 1, 1, 0.55));
      // Cheek fur sweeping back from the jaw.
      pre(strand(hb.clone().add(V(s * 0.1, -0.04, 0.02)), hb.clone().add(V(s * 0.15, -0.06, -0.06)), hb.clone().add(V(s * 0.17, -0.1, -0.14)), 0.05, { root: fur, tip: belly, thick: 0.7, taper: 0.2, bone: QI.head, center: hb.clone(), segs: 4, radial: 6 }));
    }
    add('head', withGlow(tf(sph(0.022, o.eye ?? 0xffd23a, 8, 6), s * (boar ? 0.08 : 0.068), 0.045, 0.1), 1));
  }
  add('jaw', tf(colorGeo(new THREE.CapsuleGeometry(boar ? 0.05 : 0.038, boar ? 0.1 : 0.14, 3, 8), belly), 0, -0.015, 0.11, Math.PI / 2, 0, 0, 1, 1, 0.55));
  // A ruff of longer fur around the neck and chest.
  if (!boar) for (let i = 0; i < 9; i++) {
    const a = -1.25 + (i / 8) * 2.5;
    const r0 = V(Math.sin(a) * 0.15, 0.84 - Math.cos(a) * 0.12 + 0.02, 0.46);
    pre(strand(r0, r0.clone().add(V(Math.sin(a) * 0.05, -0.03, -0.09)), r0.clone().add(V(Math.sin(a) * 0.07, -0.08 - (1 - Math.abs(a) / 1.25) * 0.05, -0.2)), 0.065, { root: fur, tip: i === 4 ? belly : fur.clone().lerp(belly, 0.4), thick: 0.55, taper: 0.25, bone: QI.chest, center: V(0, 0.78, 0.4), segs: 4, radial: 6 }));
  }
  // Mane / bristles along the neck and back.
  for (let i = 0; i < (boar ? 10 : 8); i++) {
    const z = 0.55 - i * (boar ? 0.1 : 0.09);
    const y = (z > 0.2 ? 0.92 - (0.55 - z) * 0.25 : 0.86) + (boar ? 0.08 : 0);
    const bone = z > 0.4 ? QI.neck : z > 0.0 ? QI.chest : QI.body;
    const r0 = V(0, y, z);
    pre(strand(r0, r0.clone().add(V(0, 0.06, -0.06)), r0.clone().add(V(0, boar ? 0.12 : 0.08, -0.14)), boar ? 0.05 : 0.06, { root: dark, tip: o.spikes ? new THREE.Color(o.spikes) : dark.clone().multiplyScalar(0.8), thick: 0.4, bone, center: V(0, 0.6, z), segs: 4, radial: 5 }));
  }
  // Legs: muscular shoulder and thigh blending into the body, a forward
  // knee and a hock angled back on the hind legs, elongated paws.
  for (const L of ['flL', 'flR', 'blL', 'blR']) {
    const front = L[0] === 'f';
    const bw = boar ? 1.25 : 1;
    let chain, K;
    if (front) {
      chain = [
        { p: QB[L].clone().add(V(0, 0.1, -0.02)), bone: QI[L] },
        { p: QB[L + '2'].clone().add(V(0, 0, -0.01)), bone: QI[L + '2'] },
        { p: QB[L + '3'].clone(), bone: QI[L + '3'] },
        { p: QB[L + '3'].clone().add(V(0, -0.055, 0.02)), bone: QI[L + '3'] },
      ];
      K = [[0, 0.115, 1.2], [0.3, 0.08, 1.15], [0.6, 0.05, 1.0], [0.85, 0.042, 1.0], [1, 0.046, 1.1]];
    } else {
      chain = [
        { p: QB[L].clone().add(V(0, 0.1, 0.05)), bone: QI[L] },
        { p: QB[L + '2'].clone().add(V(0, 0, 0.02)), bone: QI[L + '2'] },
        { p: QB[L + '3'].clone().add(V(0, 0, -0.02)), bone: QI[L + '3'] },
        { p: QB[L + '3'].clone().add(V(0, -0.1, 0.03)), bone: QI[L + '3'] },
      ];
      K = [[0, 0.17, 1.35], [0.3, 0.105, 1.3], [0.55, 0.056, 1.05], [0.78, 0.044, 1.0], [1, 0.045, 1.1]];
    }
    pre(tube(chain, (s) => { const [r, d] = keyed(K, s); return { rx: r * bw * 0.85, rz: r * bw * d }; },
      (s) => (s > 0.7 ? dark : s > 0.5 ? fur.clone().lerp(dark, (s - 0.5) * 5) : fur), { radial: 12, rings: 4 }));
    // Paw: a flattened pad with two toe lumps.
    const pawY = front ? -0.075 : -0.12, pawZ = front ? 0.05 : 0.07;
    add(L + '3', tf(sph(boar ? 0.055 : 0.05, dark, 12, 8), 0, pawY, pawZ, 0, 0, 0, 1.05, 0.5, 1.45));
    if (!boar) for (const tx of [-0.022, 0.022]) add(L + '3', tf(sph(0.022, dark, 8, 6), tx, pawY - 0.004, pawZ + 0.055, 0, 0, 0, 1, 0.7, 1.1));
  }
  // Tail.
  const t0 = V(0, 0.8, -0.42);
  if (boar) pre(strand(t0, t0.clone().add(V(0, -0.02, -0.08)), t0.clone().add(V(0, -0.14, -0.12)), 0.02, { root: fur, tip: dark, thick: 1, bone: QI.tail1, segs: 4, radial: 5 }));
  else pre(strand(t0, t0.clone().add(V(0, 0.0, -0.3)), t0.clone().add(V(0, -0.2, -0.52)), 0.075, { root: fur, tip: belly, thick: 0.85, taper: 0.7, bone: QI.tail1, bone2: QI.tail2, center: t0.clone().add(V(0, 0.2, -0.2)), segs: 7, radial: 8 }));
  // Ice crystals on Rimefang's back.
  if (o.spikes) {
    for (let i = 0; i < 6; i++) {
      const z = 0.38 - i * 0.14;
      const g = colorGeo(new THREE.ConeGeometry(0.05 + (i % 2) * 0.02, 0.22 + (i % 3) * 0.06, 5), o.spikes, 0.8);
      add(z > 0.0 ? 'chest' : 'body', tf(g, 0, 0.2 - Math.abs(z - 0.1) * 0.1, z - (z > 0 ? 0.32 : -0.1), -0.45, 0, (i % 2 ? 0.15 : -0.15)));
    }
  }
  return P;
}

export class QuadrupedModel {
  constructor(o = {}) {
    const r = buildSkinned(QUAD_BONES, quadParts(o), { outline: o.outline !== false, centerY: 0.6, radius: 1.3, outlineWidth: 0.012 / Math.sqrt(o.scale ?? 1) });
    this.bones = r.bones;
    this.kind = o.kind ?? 'wolf';
    this.group = new THREE.Group();
    this.group.add(r.mesh);
    if (r.outline) this.group.add(r.outline);
    this.group.scale.setScalar(o.scale ?? 1);
    this.base = r.base;
    this.phase = 0;
    this.t = Math.random() * 10;
  }
  animate(dt, s) {
    const B = this.bones;
    this.t += dt;
    const speed = s.speed || 0;
    const sc = this.group.scale.x;
    const run = clamp(speed / (7 * sc), 0, 1);
    this.phase += dt * (speed > 0.2 ? 5 + (speed / sc) * 1.1 : 0);
    const ph = this.phase;
    const amp = 0.25 + run * 0.55;
    const mv = clamp(speed / 2, 0, 1);
    const k = 1 - Math.exp(-18 * dt);
    const leg = (name, off, back) => {
      const a = Math.sin(ph + off) * amp * mv;
      const lift = Math.max(0, Math.cos(ph + off)) * mv;
      B[name].rotation.x += (a - B[name].rotation.x) * k;
      const knee = back ? -lift * 0.9 : lift * 1.0;
      B[name + '2'].rotation.x += (knee - B[name + '2'].rotation.x) * k;
      B[name + '3'].rotation.x += ((back ? lift * 0.5 : -lift * 0.6) - B[name + '3'].rotation.x) * k;
    };
    if (run > 0.5) { leg('flL', 0, false); leg('flR', 0.55, false); leg('blL', Math.PI, true); leg('blR', Math.PI + 0.55, true); }
    else { leg('flL', 0, false); leg('blR', 0, true); leg('flR', Math.PI, false); leg('blL', Math.PI, true); }
    let bodyX = run > 0.5 ? Math.sin(ph) * 0.1 : 0, headX = -0.1 + (mv < 0.1 ? Math.sin(this.t * 0.9) * 0.05 : 0), jaw = 0, bodyY = -Math.abs(Math.sin(ph)) * 0.04 * mv, headY = mv < 0.1 ? Math.sin(this.t * 0.4) * 0.25 : 0;
    if (s.action === 'windup') { bodyX = 0.18; headX = 0.3; jaw = 0.35; bodyY = -0.12; if (this.kind === 'boar') { headX = 0.45; bodyY = -0.06; } }
    if (s.action === 'strike') { bodyX = -0.22; headX = this.kind === 'boar' ? 0.4 : -0.25; jaw = 0.65; }
    if (s.action === 'howl') { bodyX = -0.4; headX = -0.95; jaw = 0.5; }
    if (s.action === 'breath') { headX = 0.1; jaw = 0.7; }
    if (s.action === 'stagger') { bodyX = -0.15; headX = -0.3; bodyY = -0.06; }
    if (s.dead) {
      const kk = clamp(s.deadT * 2.5, 0, 1);
      B.body.rotation.z += (1.5 * kk - B.body.rotation.z) * k;
      bodyY = -0.45 * kk;
      for (const l of ['flL', 'flR', 'blL', 'blR']) B[l].rotation.x += (0.35 - B[l].rotation.x) * k;
      headX = 0.2;
    }
    const kb = 1 - Math.exp(-12 * dt);
    B.body.rotation.x += (bodyX - B.body.rotation.x) * kb;
    B.body.position.y = this.base.body.y + bodyY;
    B.head.rotation.x += (headX - B.head.rotation.x) * kb;
    B.neck.rotation.y += (headY - B.neck.rotation.y) * kb * 0.5;
    B.jaw.rotation.x += (jaw - B.jaw.rotation.x) * (1 - Math.exp(-16 * dt));
    B.tail1.rotation.x += ((0.35 - run * 0.55) - B.tail1.rotation.x) * kb * 0.5;
    B.tail1.rotation.y = Math.sin(this.t * (speed > 1 ? 8 : 2.5)) * (s.dead ? 0 : 0.32);
    B.tail2.rotation.y = Math.sin(this.t * (speed > 1 ? 8 : 2.5) - 0.8) * (s.dead ? 0 : 0.3);
    B.tail2.rotation.x = 0.2;
  }
}
export class WolfModel extends QuadrupedModel { constructor(o = {}) { super({ ...o, kind: 'wolf' }); } }

// ---------------------------------------------------------------- skyray (flying)
export class SkyrayModel {
  constructor(o = {}) {
    const top = new THREE.Color(o.top ?? 0x3a5a9a), under = new THREE.Color(o.under ?? 0xd8e8f8), glow = new THREE.Color(o.glow ?? 0x8ff0ff);
    const mk = (side) => {
      // One wing: a curved triangle fan with thickness, coloured top/bottom.
      const g = new THREE.BufferGeometry();
      const v = [], c = [];
      const pts = [[0, 0, 0.35], [side * 1.1, 0.05, -0.15], [side * 0.95, 0.02, -0.4], [0, 0, -0.35]];
      const push = (p, col) => { v.push(...p); c.push(col.r, col.g, col.b); };
      for (const [ty, col] of [[0.03, top], [-0.03, under]]) {
        const P = pts.map(([x, y, z]) => [x, y + ty, z]);
        const tris = ty > 0 ? [[0, 1, 2], [0, 2, 3]] : [[0, 2, 1], [0, 3, 2]];
        if (side < 0) tris.forEach((t) => t.reverse());
        for (const t of tris) for (const i of t) push(P[i], col);
      }
      g.setAttribute('position', new THREE.Float32BufferAttribute(v, 3));
      g.setAttribute('color', new THREE.Float32BufferAttribute(c, 3));
      g.computeVertexNormals();
      return withGlow(g);
    };
    const body = colorGeo(new THREE.SphereGeometry(0.32, 16, 10), (x, y) => (y > 0 ? top : under));
    tf(body, 0, 0, 0, 0, 0, 0, 1.0, 0.35, 1.5);
    const tail = tf(coneg(0.05, 0.9, top, 6), 0, 0, -0.85, -Math.PI / 2, 0, 0);
    const parts = [withGlow(body), withGlow(tail)];
    for (const s of [1, -1]) parts.push(withGlow(tf(sph(0.04, glow, 8, 6), s * 0.12, 0.06, 0.38), 1));
    for (let i = 0; i < 4; i++) parts.push(withGlow(tf(sph(0.03, glow, 6, 4), 0, 0.1, 0.1 - i * 0.18), 1));
    this.group = new THREE.Group();
    this.inner = new THREE.Group();
    const mat = makeToonMaterial({ side: THREE.DoubleSide, rim: 0.4 });
    const bodyMesh = new THREE.Mesh(mergeGeometries(parts), mat);
    this.wingL = new THREE.Mesh(mk(1), mat); this.wingR = new THREE.Mesh(mk(-1), mat);
    this.wingL.position.x = 0.18; this.wingR.position.x = -0.18;
    this.inner.add(bodyMesh, this.wingL, this.wingR);
    for (const m of [bodyMesh, this.wingL, this.wingR]) m.castShadow = true;
    if (o.outline !== false) this.inner.add(new THREE.Mesh(bodyMesh.geometry, makeOutlineMaterial(0.02)));
    this.inner.position.y = 2.6;
    this.group.add(this.inner);
    this.group.scale.setScalar(o.scale ?? 1.4);
    this.t = Math.random() * 10;
  }
  animate(dt, s) {
    this.t += dt;
    const flap = s.action === 'strike' ? 0.15 : s.action === 'windup' ? 1.2 : 0.55;
    const f = Math.sin(this.t * (s.action === 'windup' ? 9 : 4.5)) * flap;
    this.wingL.rotation.z = f; this.wingR.rotation.z = -f;
    let y = 2.6 + Math.sin(this.t * 1.3) * 0.25, pitch = 0;
    if (s.action === 'windup') { y = 3.4; pitch = -0.35; }
    if (s.action === 'strike') { y = 0.9; pitch = 0.5; }
    if (s.dead) { y = Math.max(0.2, 2.6 - s.deadT * 3); pitch = 0.8; this.inner.rotation.z += dt * 4; }
    this.inner.position.y = damp(this.inner.position.y, y, s.action === 'strike' ? 10 : 3, dt);
    this.inner.rotation.x = damp(this.inner.rotation.x, pitch, 6, dt);
  }
}

// ---------------------------------------------------------------- wisp
export class WispModel {
  constructor(color = 0x9ff0ff) {
    this.group = new THREE.Group();
    const core = new THREE.Mesh(new THREE.IcosahedronGeometry(0.24, 2), new THREE.MeshBasicMaterial({ color: new THREE.Color(color).multiplyScalar(1.8) }));
    const halo = new THREE.Mesh(new THREE.IcosahedronGeometry(0.48, 2), new THREE.MeshBasicMaterial({ color, transparent: true, opacity: 0.22, blending: THREE.AdditiveBlending, depthWrite: false }));
    this.ring = new THREE.Mesh(new THREE.TorusGeometry(0.42, 0.015, 6, 32), new THREE.MeshBasicMaterial({ color: new THREE.Color(color).multiplyScalar(1.4) }));
    this.inner = new THREE.Group();
    this.inner.add(core, halo, this.ring);
    this.inner.position.y = 1.5;
    this.group.add(this.inner);
    this.t = Math.random() * 10;
  }
  animate(dt, s) {
    this.t += dt;
    this.inner.position.y = 1.5 + Math.sin(this.t * 2) * 0.25;
    this.ring.rotation.x = this.t * 1.7; this.ring.rotation.y = this.t * 1.1;
    const k = s.dead ? Math.max(0.01, 1 - s.deadT * 3) : s.action === 'windup' ? 1.35 : 1;
    this.inner.scale.setScalar(k);
  }
}

// ---------------------------------------------------------------- the Hollow Herald
export class WraithModel {
  constructor() {
    this.group = new THREE.Group();
    const robe = colorGeo(new THREE.CylinderGeometry(0.42, 1.05, 2.7, 24, 8, true), (x, y) => new THREE.Color(0x1a1030).lerp(new THREE.Color(0x6a3aa8), clamp((y + 1.35) / 2.7, 0, 1)));
    const p = robe.attributes.position;
    for (let i = 0; i < p.count; i++) {
      const y = p.getY(i), a = Math.atan2(p.getZ(i), p.getX(i));
      if (y < -1.0) { const k = (-1.0 - y) / 0.35; p.setY(i, y + Math.sin(a * 7) * 0.12 * k); }
    }
    robe.computeVertexNormals();
    tf(robe, 0, 1.55, 0);
    const hood = tf(colorGeo(new THREE.SphereGeometry(0.46, 18, 14, 0, Math.PI * 2, 0, Math.PI * 0.75), 0x22143a), 0, 3.0, -0.03, -0.25, 0, 0, 1, 1.15, 1.15);
    const face = tf(sph(0.31, 0x05020a, 14, 10), 0, 2.95, 0.12);
    const mantle = tf(colorGeo(new THREE.SphereGeometry(0.72, 20, 10, 0, Math.PI * 2, 0, Math.PI / 2), 0x3a2058), 0, 2.5, 0, 0, 0, 0, 1.3, 0.55, 0.9);
    const trim = tf(torusg(0.94, 0.03, 0xe6c06a, 6, 40), 0, 2.48, 0, Math.PI / 2, 0, 0, 1, 0.7, 1);
    const geo = mergeGeometries([robe, hood, face, mantle, trim].map((g) => withGlow(g)));
    this.body = new THREE.Mesh(geo, makeToonMaterial({ side: THREE.DoubleSide, rim: 0.45 }));
    this.body.castShadow = true;
    const outline = new THREE.Mesh(geo, makeOutlineMaterial(0.03));
    this.inner = new THREE.Group();
    this.inner.add(this.body, outline);
    for (const s of [1, -1]) {
      const e = new THREE.Mesh(new THREE.SphereGeometry(0.06, 10, 8), new THREE.MeshBasicMaterial({ color: new THREE.Color(0xff5af0).multiplyScalar(2.2) }));
      e.position.set(s * 0.12, 3.0, 0.4);
      e.scale.set(1.4, 0.7, 1);
      this.inner.add(e);
    }
    this.hands = [];
    for (const s of [1, -1]) {
      const h = new THREE.Mesh(new THREE.OctahedronGeometry(0.16, 0), glowMat);
      h.position.set(s * 1.25, 2.1, 0.3);
      this.inner.add(h);
      this.hands.push(h);
    }
    this.ring = new THREE.Group();
    for (let i = 0; i < 8; i++) {
      const m = new THREE.Mesh(crystal(0.18, 1.1, i % 2 ? 0xc0a0ff : 0xff7af0), glowMat);
      const a = (i / 8) * Math.PI * 2;
      m.position.set(Math.cos(a) * 1.9, 0, Math.sin(a) * 1.9);
      this.ring.add(m);
    }
    this.ring.position.y = 2.2;
    this.inner.add(this.ring);
    this.group.add(this.inner);
    this.t = 0;
  }
  animate(dt, s) {
    this.t += dt;
    this.inner.position.y = 0.8 + Math.sin(this.t * 1.2) * 0.3;
    this.ring.rotation.y += dt * (s.action === 'windup' ? 4 : 1);
    this.ring.children.forEach((m, i) => { m.rotation.y += dt * 2; m.position.y = Math.sin(this.t * 2 + i) * 0.3; });
    this.hands.forEach((h, i) => { h.rotation.y += dt * 3; h.position.y = 2.1 + Math.sin(this.t * 1.6 + i * 2) * 0.2 + (s.action === 'windup' ? 0.6 : 0); });
    if (s.dead) { const k = Math.max(0.01, 1 - s.deadT * 0.5); this.inner.scale.setScalar(k); this.inner.rotation.y += dt * 6; }
  }
}

// ---------------------------------------------------------------- bow
export function makeBow() {
  const curve = new THREE.QuadraticBezierCurve3(V(0, -0.55, 0), V(0, 0, 0.22), V(0, 0.55, 0));
  const limb = colorGeo(new THREE.TubeGeometry(curve, 16, 0.018, 6, false), 0x5a3a24);
  const string = colorGeo(new THREE.CylinderGeometry(0.003, 0.003, 1.1, 4), 0xe8e0d0);
  const grip = colorGeo(new THREE.CylinderGeometry(0.024, 0.024, 0.14, 8), 0x2a1a14);
  grip.translate(0, 0, 0.11);
  const g = mergeGeometries([limb, string, grip].map((x) => withGlow(x)));
  return new THREE.Mesh(g, makeToonMaterial());
}

// ---------------------------------------------------------------- humanoid foes
export class HumanoidModel {
  constructor(look, weapon, outline = true) {
    this.ch = new Character(look, { outline });
    this.group = this.ch.group;
    if (weapon === 'staff') { const st = makeStaff(0xff8a3a); this.ch.bones.handR.add(st); st.rotation.x = 1.6; st.position.y = -0.07; }
    else if (weapon === 'bow') { const b = makeBow(); this.ch.bones.handL.add(b); b.rotation.set(0.1, Math.PI / 2, 0); b.position.set(0, -0.07, 0.02); }
    else if (weapon) { this.ch.setWeapon(makeSword(weapon)); this.ch.drawWeapon(true); }
  }
  animate(dt, s) {
    const map = { windup: s.attackAnim ?? 'attack1', strike: s.attackAnim ?? 'attack1', recover: s.attackAnim ?? 'attack1' };
    let action = null, t = 0;
    if (s.action && map[s.action]) {
      action = map[s.action];
      t = s.action === 'windup' ? 0.32 * s.actionK : s.action === 'strike' ? 0.32 + 0.2 * s.actionK : 0.52 + 0.48 * s.actionK;
      if (action === 'cast' || action === 'castUp' || action === 'aim') t = s.action === 'windup' ? 0.3 * s.actionK : 0.3 + 0.7 * s.actionK;
      if (action === 'aim') action = 'cast';
    }
    if (s.action === 'stagger') { action = 'stagger'; t = s.actionK; }
    if (s.action === 'block') { action = 'block'; t = 0.5; }
    this.ch.update(dt, { speed: s.speed, grounded: true, vy: 0, action, actionT: t, dead: s.dead, deadT: s.deadT, pose: s.speed < 0.3 && !action ? (s.alert ? 'combat' : s.pose) : s.pose });
  }
}

export { lerp };
