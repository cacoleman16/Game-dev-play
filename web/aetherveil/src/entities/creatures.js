// Creature models: gloops (slimes), quadruped wolves, floating wisps and the
// Hollow Herald. Humanoid foes (bandits, golems, revenants) reuse Character.
// Each model exposes `group` and `animate(dt, state)`.

import * as THREE from 'three';
import { mergeGeometries } from 'three/addons';
import { buildSkinned, makeToonMaterial, makeOutlineMaterial, Character, makeSword, makeStaff, colorGeo, tf, capsule, sph, cylg, coneg, boxg } from './character.js';
import { makeGlowMaterial } from '../world/vegetation.js';
import { damp, clamp, lerp } from '../core/util.js';
import { crystal } from '../world/geo.js';

const glowMat = makeGlowMaterial(1.0);

// ---------------------------------------------------------------- gloop
export class GloopModel {
  constructor(color = 0x6fd86a, scale = 1, outline = true) {
    this.group = new THREE.Group();
    const c = new THREE.Color(color);
    const light = c.clone().offsetHSL(0, 0, 0.18);
    const body = colorGeo(new THREE.SphereGeometry(0.55, 18, 12), (x, y) => (y > 0.25 ? light : c));
    tf(body, 0, 0.45, 0, 0, 0, 0, 1, 0.85, 1);
    const parts = [body];
    for (const s of [1, -1]) {
      parts.push(tf(sph(0.09, 0x1a1420, 8, 6), s * 0.17, 0.55, 0.45, 0, 0, 0, 0.8, 1.1, 0.5));
      parts.push(tf(sph(0.03, 0xffffff, 6, 4), s * 0.15, 0.6, 0.5));
    }
    parts.push(tf(sph(0.06, 0x1a1420, 6, 4), 0, 0.38, 0.5, 0, 0, 0, 1.4, 0.5, 0.4));
    const geo = mergeGeometries(parts);
    this.body = new THREE.Mesh(geo, makeToonMaterial({ transparent: false }));
    this.body.castShadow = true;
    this.inner = new THREE.Group();
    this.inner.add(this.body);
    if (outline) {
      const o = new THREE.Mesh(geo, makeOutlineMaterial(0.025));
      this.inner.add(o);
    }
    this.group.add(this.inner);
    this.group.scale.setScalar(scale);
    this.t = Math.random() * 10;
  }
  animate(dt, s) {
    this.t += dt * (s.speed > 0.5 ? 9 : 3);
    const b = Math.abs(Math.sin(this.t));
    let sy = 1 - 0.12 * (1 - b), sxz = 1 + 0.08 * (1 - b), y = s.speed > 0.5 ? b * 0.35 : 0;
    if (s.action === 'windup') { sy = 0.7; sxz = 1.25; y = 0; }
    if (s.action === 'strike') { sy = 1.3; sxz = 0.85; }
    if (s.dead) { sy = Math.max(0.05, 1 - s.deadT * 2); sxz = 1 + s.deadT * 1.5; y = 0; }
    this.inner.scale.set(sxz, sy, sxz);
    this.inner.position.y = y;
  }
}

// ---------------------------------------------------------------- wolf
const WOLF_BONES = [
  ['body', null, [0, 0.72, -0.1]],
  ['chest', 'body', [0, 0.04, 0.42]],
  ['neck', 'chest', [0, 0.1, 0.18]],
  ['head', 'neck', [0, 0.14, 0.12]],
  ['jaw', 'head', [0, -0.07, 0.08]],
  ['tail1', 'body', [0, 0.08, -0.3]],
  ['tail2', 'tail1', [0, 0, -0.26]],
  ['flL', 'chest', [0.13, -0.08, 0.06]], ['flL2', 'flL', [0, -0.3, 0]], ['flL3', 'flL2', [0, -0.3, 0]],
  ['flR', 'chest', [-0.13, -0.08, 0.06]], ['flR2', 'flR', [0, -0.3, 0]], ['flR3', 'flR2', [0, -0.3, 0]],
  ['blL', 'body', [0.13, -0.04, -0.22]], ['blL2', 'blL', [0, -0.32, 0]], ['blL3', 'blL2', [0, -0.3, 0]],
  ['blR', 'body', [-0.13, -0.04, -0.22]], ['blR2', 'blR', [0, -0.32, 0]], ['blR3', 'blR2', [0, -0.3, 0]],
];

export class WolfModel {
  constructor(o = {}) {
    const fur = o.fur ?? 0x5a5a66, belly = o.belly ?? 0x9a98a0, eye = o.eye ?? 0xffd23a, dark = o.dark ?? 0x3a3a44;
    const parts = [];
    const add = (b, g) => parts.push([b, g]);
    add('body', tf(capsule(0.2, 0.42, fur, 10), 0, 0, 0.05, Math.PI / 2, 0, 0, 1, 1, 1.05));
    add('body', tf(capsule(0.17, 0.3, belly, 8), 0, -0.06, 0.05, Math.PI / 2, 0, 0));
    add('chest', tf(sph(0.25, fur, 12, 8), 0, 0.02, 0, 0, 0, 0, 1, 1.05, 1.1));
    add('chest', tf(sph(0.2, belly, 10, 6), 0, -0.06, 0.08));
    add('neck', tf(capsule(0.13, 0.12, fur, 8), 0, 0.04, 0.02, 0.6, 0, 0));
    add('head', tf(sph(0.16, fur, 12, 8), 0, 0.02, 0, 0, 0, 0, 1, 0.9, 1.1));
    add('head', tf(boxg(0.13, 0.1, 0.2, fur), 0, -0.03, 0.18));
    add('head', tf(sph(0.035, 0x1a1418, 6, 4), 0, -0.0, 0.29));
    for (const s of [1, -1]) {
      add('head', tf(coneg(0.06, 0.16, dark, 4), s * 0.09, 0.15, -0.03, -0.2, 0, s * -0.3));
      add('head', tf(sph(0.03, eye, 6, 4), s * 0.075, 0.05, 0.12));
    }
    add('jaw', tf(boxg(0.11, 0.04, 0.18, belly), 0, -0.01, 0.1));
    add('tail1', tf(coneg(0.08, 0.32, fur, 6), 0, 0, -0.14, -Math.PI / 2, 0, 0));
    add('tail2', tf(coneg(0.07, 0.3, dark, 6), 0, 0, -0.12, -Math.PI / 2, 0, 0));
    for (const leg of ['flL', 'flR', 'blL', 'blR']) {
      add(leg, tf(capsule(0.065, 0.2, fur, 6), 0, -0.14, 0));
      add(leg + '2', tf(capsule(0.05, 0.22, dark, 6), 0, -0.15, 0));
      add(leg + '3', tf(boxg(0.09, 0.05, 0.13, dark), 0, -0.01, 0.03));
    }
    if (o.spikes) {
      for (let i = 0; i < 5; i++) add(i < 2 ? 'chest' : 'body', tf(colorGeo(new THREE.ConeGeometry(0.06, 0.32, 5), o.spikes), 0, 0.22, i < 2 ? -0.05 + i * 0.12 : 0.25 - (i - 2) * 0.18, -0.4, 0, 0));
    }
    const r = buildSkinned(WOLF_BONES, parts, { outline: o.outline !== false, centerY: 0.6, radius: 1.2, outlineWidth: 0.014 });
    this.bones = r.bones;
    this.group = new THREE.Group();
    this.group.add(r.mesh);
    if (r.outline) this.group.add(r.outline);
    this.group.scale.setScalar(o.scale ?? 1);
    this.base = r.base;
    this.phase = 0;
    this.t = 0;
  }
  animate(dt, s) {
    const B = this.bones;
    this.t += dt;
    const speed = s.speed || 0;
    const run = clamp(speed / 7, 0, 1);
    this.phase += dt * (speed > 0.2 ? 5 + speed * 1.1 : 0);
    const ph = this.phase;
    const amp = 0.25 + run * 0.55;
    const mv = clamp(speed / 2, 0, 1);
    const leg = (name, off, back) => {
      const a = Math.sin(ph + off) * amp * mv;
      const lift = Math.max(0, Math.cos(ph + off)) * mv;
      B[name].rotation.x = damp(B[name].rotation.x, a, 20, dt);
      B[name + '2'].rotation.x = damp(B[name + '2'].rotation.x, back ? -lift * 0.9 : lift * 0.9, 20, dt);
    };
    // Gallop when fast, trot when slow.
    if (run > 0.5) { leg('flL', 0, false); leg('flR', 0.6, false); leg('blL', Math.PI, true); leg('blR', Math.PI + 0.6, true); }
    else { leg('flL', 0, false); leg('blR', 0, true); leg('flR', Math.PI, false); leg('blL', Math.PI, true); }
    let bodyX = run > 0.5 ? Math.sin(ph) * 0.12 : 0, headX = -0.1, jaw = 0, bodyY = -Math.abs(Math.sin(ph)) * 0.04 * mv;
    if (s.action === 'windup') { bodyX = 0.2; headX = 0.3; jaw = 0.35; bodyY = -0.12; }
    if (s.action === 'strike') { bodyX = -0.25; headX = -0.25; jaw = 0.6; }
    if (s.action === 'howl') { bodyX = -0.4; headX = -0.9; jaw = 0.5; }
    if (s.action === 'breath') { headX = 0.1; jaw = 0.7; }
    if (s.dead) {
      const k = clamp(s.deadT * 2.5, 0, 1);
      B.body.rotation.z = damp(B.body.rotation.z, 1.5 * k, 8, dt);
      bodyY = -0.45 * k;
      for (const l of ['flL', 'flR', 'blL', 'blR']) B[l].rotation.x = damp(B[l].rotation.x, 0.3, 8, dt);
    }
    B.body.rotation.x = damp(B.body.rotation.x, bodyX, 12, dt);
    B.body.position.y = this.base.body.y + bodyY;
    B.head.rotation.x = damp(B.head.rotation.x, headX + Math.sin(this.t * 1.3) * 0.05 * (1 - mv), 10, dt);
    B.jaw.rotation.x = damp(B.jaw.rotation.x, jaw, 14, dt);
    B.tail1.rotation.x = damp(B.tail1.rotation.x, 0.4 - run * 0.5, 6, dt);
    B.tail1.rotation.y = Math.sin(this.t * (s.speed > 1 ? 8 : 3)) * 0.3;
    B.tail2.rotation.x = 0.2;
  }
}

// ---------------------------------------------------------------- wisp
export class WispModel {
  constructor(color = 0x9ff0ff) {
    this.group = new THREE.Group();
    const core = new THREE.Mesh(new THREE.IcosahedronGeometry(0.28, 1), new THREE.MeshBasicMaterial({ color: new THREE.Color(color).multiplyScalar(1.6) }));
    const halo = new THREE.Mesh(new THREE.IcosahedronGeometry(0.5, 1), new THREE.MeshBasicMaterial({ color, transparent: true, opacity: 0.25, blending: THREE.AdditiveBlending, depthWrite: false }));
    this.inner = new THREE.Group();
    this.inner.add(core, halo);
    this.inner.position.y = 1.5;
    this.group.add(this.inner);
    this.color = color;
    this.t = Math.random() * 10;
  }
  animate(dt, s) {
    this.t += dt;
    this.inner.position.y = 1.5 + Math.sin(this.t * 2) * 0.25;
    const k = s.dead ? Math.max(0.01, 1 - s.deadT * 3) : s.action === 'windup' ? 1.3 : 1;
    this.inner.scale.setScalar(k);
  }
}

// ---------------------------------------------------------------- the Hollow Herald
export class WraithModel {
  constructor() {
    this.group = new THREE.Group();
    const robe = colorGeo(new THREE.ConeGeometry(0.9, 2.6, 14, 4, true), (x, y) => new THREE.Color(0x2a1840).lerp(new THREE.Color(0x6a3aa8), clamp((y + 1.3) / 2.6, 0, 1)));
    tf(robe, 0, 1.6, 0);
    const hood = tf(sph(0.45, 0x1a0e2a, 12, 8), 0, 3.0, 0, 0, 0, 0, 1, 1.15, 1.1);
    const face = tf(sph(0.32, 0x05020a, 10, 6), 0, 2.95, 0.15);
    const shoulders = tf(sph(0.7, 0x3a2058, 12, 6), 0, 2.55, 0, 0, 0, 0, 1.3, 0.5, 0.9);
    const geo = mergeGeometries([robe, hood, face, shoulders]);
    this.body = new THREE.Mesh(geo, makeToonMaterial({ side: THREE.DoubleSide }));
    this.body.castShadow = true;
    const outline = new THREE.Mesh(geo, makeOutlineMaterial(0.03));
    this.inner = new THREE.Group();
    this.inner.add(this.body, outline);
    for (const s of [1, -1]) {
      const e = new THREE.Mesh(new THREE.SphereGeometry(0.06, 8, 6), new THREE.MeshBasicMaterial({ color: new THREE.Color(0xff5af0).multiplyScalar(2) }));
      e.position.set(s * 0.12, 3.0, 0.42);
      this.inner.add(e);
    }
    this.ring = new THREE.Group();
    for (let i = 0; i < 8; i++) {
      const m = new THREE.Mesh(crystal(0.18, 1.1, i % 2 ? 0xc0a0ff : 0xff7af0), glowMat);
      const a = (i / 8) * Math.PI * 2;
      m.position.set(Math.cos(a) * 1.8, 0, Math.sin(a) * 1.8);
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
    if (s.dead) { const k = Math.max(0.01, 1 - s.deadT * 0.5); this.inner.scale.setScalar(k); this.inner.rotation.y += dt * 6; }
  }
}

// ---------------------------------------------------------------- humanoid foes
export class HumanoidModel {
  constructor(look, weapon, outline = true) {
    this.ch = new Character(look, { outline });
    this.group = this.ch.group;
    if (weapon === 'staff') { const st = makeStaff(0xff8a3a); this.ch.bones.handR.add(st); st.rotation.x = 1.6; st.position.y = -0.05; }
    else if (weapon) { this.ch.setWeapon(makeSword(weapon)); this.ch.drawWeapon(true); }
  }
  animate(dt, s) {
    const map = { windup: s.attackAnim ?? 'attack1', strike: s.attackAnim ?? 'attack1', recover: s.attackAnim ?? 'attack1' };
    let action = null, t = 0;
    if (s.action && map[s.action]) {
      action = map[s.action];
      t = s.action === 'windup' ? 0.3 * s.actionK : s.action === 'strike' ? 0.3 + 0.25 * s.actionK : 0.55 + 0.45 * s.actionK;
      if (action === 'cast' || action === 'castUp') t = s.action === 'windup' ? 0.3 * s.actionK : 0.3 + 0.7 * s.actionK;
    }
    if (s.action === 'stagger') { action = 'hit'; t = s.actionK; }
    if (s.action === 'block') { action = 'block'; t = 0.5; }
    this.ch.update(dt, { speed: s.speed, grounded: true, vy: 0, action, actionT: t, dead: s.dead, deadT: s.deadT, pose: s.pose });
  }
}

export { lerp };
