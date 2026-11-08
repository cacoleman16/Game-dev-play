// Every hand-built place in the world: houses, ruins, shrine, vault, camps,
// the floating Heartshard and its sky islands. Pieces are merged per location
// into a few meshes (solid / glowing / windows) and register colliders and
// interactables (waystones, chests, doors) as they go.

import * as THREE from 'three';
import { RNG } from '../core/noise.js';
import { LOC } from './locations.js';
import { makeGlowMaterial } from './vegetation.js';
import { paint, place, merge, box, cyl, cone, ico, gable, crystal, lumpy, torus } from './geo.js';

const P = {
  plaster: [0xf1e4c8, 0xeadbb8, 0xf4ead6, 0xe2d0b0],
  timber: 0x5b3a26,
  roofs: [0xc8513c, 0xd2693c, 0x4c7fbf, 0x3f9f9a, 0x7a5aa8, 0xb8453a],
  stone: 0x9d968a, stoneDark: 0x7a746b, stoneLight: 0xc4bcae,
  wood: 0x8a6240, woodDark: 0x5e412b,
  canvas: 0xe9dfc6, redCanvas: 0xb84a3a,
  iron: 0x4a4a52, gold: 0xe6b84a,
};

// Collects pieces for one location, then merges them.
class Batch {
  constructor() { this.solid = []; this.glow = []; this.window = []; }
  // Add a geometry built in local space of a frame (x,y,z,rot).
  add(kind, geo, f) {
    if (f) place(geo, 0, 0, 0, [0, f.rot, 0]).translate(f.x, f.y, f.z);
    this[kind].push(geo);
  }
}

export class Structures {
  constructor(game) {
    this.game = game;
    this.group = new THREE.Group();
    this.group.name = 'structures';
    this.solidMat = new THREE.MeshLambertMaterial({ vertexColors: true });
    this.glowMat = makeGlowMaterial(1.1);
    this.windowMat = new THREE.MeshBasicMaterial({ color: 0x4a5a6e });
    this.animated = []; // {obj, fn(dt, t)}
    this.waystones = [];
    this.chests = [];
    this.points = {}; // named spots for NPCs, spawns, etc.
    this.lanterns = [];
  }

  get colliders() { return this.game.colliders; }

  // Remove grass blades under a footprint (circle).
  clearGrass(x, z, r) {
    const w = this.game.world, tex = this.game.worldTex?.grass;
    w.forGridInRadius(x, z, r + 1.5, (i, d) => {
      const k = d < r ? 0 : (d - r) / 1.5;
      const v = Math.round(w.grass[i] * k);
      w.grass[i] = v;
      if (tex) tex.image.data[i * 4 + 3] = v;
    });
    this.grassDirty = true;
  }
  ground(x, z) { return this.game.world.heightAt(x, z); }
  // Lowest ground under a rectangle, so foundations never float.
  groundRect(x, z, w, d, rot) {
    let m = Infinity;
    const c = Math.cos(rot), s = Math.sin(rot);
    for (const [lx, lz] of [[-w / 2, -d / 2], [w / 2, -d / 2], [-w / 2, d / 2], [w / 2, d / 2], [0, 0]]) {
      m = Math.min(m, this.ground(x + lx * c + lz * s, z - lx * s + lz * c));
    }
    return m;
  }
  frame(x, z, rot = 0, y = null) { return { x, z, rot, y: y ?? this.ground(x, z) }; }
  facing(x, z, tx, tz) { return Math.atan2(tx - x, tz - z); }
  // Local offset (lx, lz) of a frame -> world XZ.
  local(f, lx, lz) {
    const c = Math.cos(f.rot), s = Math.sin(f.rot);
    return [f.x + lx * c + lz * s, f.z - lx * s + lz * c];
  }

  flush(b, name) {
    const g = new THREE.Group();
    g.name = name;
    if (b.solid.length) {
      const m = new THREE.Mesh(merge(b.solid), this.solidMat);
      m.castShadow = true; m.receiveShadow = true; g.add(m);
    }
    if (b.glow.length) g.add(new THREE.Mesh(merge(b.glow), this.glowMat));
    if (b.window.length) g.add(new THREE.Mesh(merge(b.window), this.windowMat));
    g.traverse((o) => { o.matrixAutoUpdate = false; o.updateMatrix(); });
    this.group.add(g);
    return g;
  }

  build() {
    // Every collider placed while building also clears the grass beneath it.
    const col = this.game.colliders;
    const addCircle = col.addCircle.bind(col), addBox = col.addBox.bind(col);
    col.addCircle = (x, z, r, y0, y1, tag) => { if (tag !== 'sky-island') this.clearGrass(x, z, r + 0.3); return addCircle(x, z, r, y0, y1, tag); };
    col.addBox = (x, z, hw, hd, rot, y0, y1, tag) => { if (tag !== 'pier') this.clearGrass(x, z, Math.hypot(hw, hd) * 0.85 + 0.3); return addBox(x, z, hw, hd, rot, y0, y1, tag); };
    this.buildBrightwater();
    this.buildFarm();
    this.buildSaltmere();
    this.buildLighthouse();
    this.buildScholars();
    this.buildHeartshard();
    this.buildRuins();
    this.buildShrine();
    this.buildVault();
    this.buildBandits();
    this.buildWatchtower();
    this.buildStoneCircle();
    this.buildSkyIslands();
    this.buildSignposts();
    // Wide stone platforms that have no single collider.
    this.clearGrass(LOC.heartshard.x, LOC.heartshard.z, 17.5);
    this.clearGrass(LOC.whisperwood.x, LOC.whisperwood.z, 8);
    col.addCircle = addCircle; col.addBox = addBox;
    if (this.grassDirty && this.game.worldTex) this.game.worldTex.grass.needsUpdate = true;
  }

  // ---------------------------------------------------------------- pieces
  house(b, x, z, rot, o = {}) {
    const w = o.w ?? 6, d = o.d ?? 5, h = o.h ?? 3.4;
    const roofH = o.roofH ?? w * 0.62;
    const y = this.groundRect(x, z, w + 1, d + 1, rot);
    const f = { x, z, rot, y };
    const wall = o.wall ?? P.plaster[0];
    const parts = [];
    parts.push(place(box(w + 0.4, 1.6, d + 0.4, P.stone, 0.15), 0, -0.4, 0));
    parts.push(place(box(w, h, d, wall, 0.05), 0, h / 2 + 0.3, 0));
    if (o.stories === 2) {
      parts.push(place(box(w + 0.5, h * 0.9, d + 0.5, o.wall2 ?? wall, 0.05), 0, h + 0.3 + h * 0.45, 0));
      parts.push(place(box(w + 0.7, 0.25, d + 0.7, P.timber), 0, h + 0.3, 0));
    }
    const top = (o.stories === 2 ? h * 1.9 : h) + 0.3;
    // Timber frame.
    const tw = o.stories === 2 ? w + 0.5 : w, td = o.stories === 2 ? d + 0.5 : d;
    for (const sx of [-1, 1]) for (const sz of [-1, 1]) parts.push(place(box(0.28, top - 0.3, 0.28, P.timber), sx * tw / 2, (top + 0.3) / 2, sz * td / 2));
    parts.push(place(box(w + 0.1, 0.22, d + 0.1, P.timber), 0, top - 0.1, 0));
    if (o.timberBand !== false) parts.push(place(box(w + 0.06, 0.18, d + 0.06, P.timber), 0, 0.3 + h * 0.55, 0));
    // Roof.
    parts.push(place(gable(w, d, roofH, o.roof ?? P.roofs[0], 0.45), 0, top, 0));
    parts.push(place(box(0.3, 0.3, d + 1.0, P.timber), 0, top + roofH, 0));
    if (o.chimney !== false) parts.push(place(box(0.8, roofH * 0.9 + 0.8, 0.8, P.stoneDark, 0.1), w * 0.22, top + roofH * 0.5, -d * 0.2));
    // Door + step.
    parts.push(place(box(1.2, 2.1, 0.14, P.woodDark), 0, 1.35, d / 2 + 0.05));
    parts.push(place(box(1.6, 0.25, 0.9, P.stone), 0, 0.18, d / 2 + 0.5));
    parts.push(place(box(1.5, 0.18, 0.2, P.timber), 0, 2.45, d / 2 + 0.08));
    const solid = merge(parts);
    b.add('solid', solid, f);
    // Windows (glow at night).
    const wins = [];
    const wy = 0.3 + h * 0.55;
    for (const sx of [-1, 1]) wins.push(place(box(0.9, 0.9, 0.1, 0xffffff), sx * w * 0.3, wy, d / 2 + 0.05));
    for (const sx of [-1, 1]) wins.push(place(box(0.1, 0.9, 0.9, 0xffffff), sx * (w / 2 + 0.05), wy, 0));
    if (o.stories === 2) {
      for (const sx of [-1, 0, 1]) wins.push(place(box(0.8, 0.8, 0.1, 0xffffff), sx * w * 0.3, h * 1.4 + 0.3, (d + 0.5) / 2 + 0.05));
    }
    b.add('window', merge(wins), f);
    this.colliders.addBox(x, z, w / 2 + 0.2, d / 2 + 0.2, rot, y - 3, y + top + roofH * 0.45, 'building');
    const door = this.local(f, 0, d / 2 + 1.4);
    return { f, door: { x: door[0], z: door[1] }, top };
  }

  lamp(b, x, z) {
    const y = this.ground(x, z);
    const f = { x, z, rot: 0, y };
    b.add('solid', merge([
      place(cyl(0.08, 0.12, 3.2, 6, P.iron), 0, 1.6, 0),
      place(box(0.5, 0.08, 0.5, P.iron), 0, 3.25, 0),
      place(cone(0.4, 0.4, 4, P.iron), 0, 3.85, 0, [0, Math.PI / 4, 0]),
    ]), f);
    b.add('window', place(box(0.32, 0.45, 0.32, 0xffffff), 0, 3.5, 0), f);
    this.colliders.addCircle(x, z, 0.2, y - 1, y + 3.5, 'lamp');
    this.lanterns.push(new THREE.Vector3(x, y + 3.5, z));
  }

  well(b, x, z) {
    const y = this.ground(x, z);
    const f = { x, z, rot: 0.3, y };
    b.add('solid', merge([
      place(cyl(1.2, 1.3, 1.0, 10, P.stone, 0.15), 0, 0.5, 0),
      place(cyl(1.0, 1.0, 0.05, 10, 0x2a5a7a), 0, 0.9, 0),
      place(box(0.15, 2.2, 0.15, P.wood), -1.0, 1.6, 0),
      place(box(0.15, 2.2, 0.15, P.wood), 1.0, 1.6, 0),
      place(gable(2.4, 1.6, 0.8, P.roofs[0], 0.15), 0, 2.6, 0, [0, Math.PI / 2, 0]),
      place(cyl(0.12, 0.12, 2.0, 6, P.woodDark), 0, 2.0, 0, [0, 0, Math.PI / 2]),
    ]), f);
    this.colliders.addCircle(x, z, 1.35, y - 1, y + 1.0, 'well');
  }

  stall(b, x, z, rot, awning) {
    const y = this.ground(x, z);
    const f = { x, z, rot, y };
    const stripes = (px, py, pz) => (Math.floor((px + 2) * 1.5) % 2 === 0 ? awning : 0xf6efe0);
    b.add('solid', merge([
      place(box(3, 1.0, 1.4, P.wood, 0.1), 0, 0.5, 0),
      ...[-1.4, 1.4].map((sx) => place(box(0.12, 2.6, 0.12, P.woodDark), sx, 1.3, -0.6)),
      ...[-1.4, 1.4].map((sx) => place(box(0.12, 2.2, 0.12, P.woodDark), sx, 1.1, 0.6)),
      place(paint(new THREE.BoxGeometry(3.3, 0.1, 1.9, 8, 1, 1), stripes), 0, 2.4, 0, [0.25, 0, 0]),
      place(box(0.5, 0.35, 0.4, 0xd2a040), -0.8, 1.15, 0),
      place(box(0.5, 0.3, 0.4, 0x9ac24a), 0, 1.15, 0.1),
      place(box(0.5, 0.35, 0.4, 0xd8503a), 0.8, 1.15, 0),
    ]), f);
    this.colliders.addBox(x, z, 1.6, 0.8, rot, y - 1, y + 1.0, 'stall');
  }

  crate(b, x, z, s = 1, rot = 0) {
    const y = this.ground(x, z);
    b.add('solid', merge([place(box(s, s, s, P.wood, 0.12), 0, s / 2, 0), place(box(s * 1.02, s * 0.12, s * 1.02, P.woodDark), 0, s * 0.5, 0)]), { x, z, rot, y });
    this.colliders.addBox(x, z, s / 2, s / 2, rot, y - 1, y + s, 'crate');
  }

  barrel(b, x, z) {
    const y = this.ground(x, z);
    b.add('solid', merge([place(cyl(0.42, 0.42, 1.1, 10, P.wood, 0.1), 0, 0.55, 0), place(torus(0.43, 0.04, 4, 10, P.iron), 0, 0.25, 0, [Math.PI / 2, 0, 0]), place(torus(0.43, 0.04, 4, 10, P.iron), 0, 0.85, 0, [Math.PI / 2, 0, 0])]), { x, z, rot: 0, y });
    this.colliders.addCircle(x, z, 0.45, y - 1, y + 1.1, 'barrel');
  }

  fence(b, x0, z0, x1, z1) {
    const len = Math.hypot(x1 - x0, z1 - z0);
    const n = Math.max(1, Math.round(len / 2.5));
    const rot = Math.atan2(x1 - x0, z1 - z0);
    for (let i = 0; i <= n; i++) {
      const x = x0 + (x1 - x0) * (i / n), z = z0 + (z1 - z0) * (i / n);
      const y = this.ground(x, z);
      b.add('solid', place(box(0.16, 1.2, 0.16, P.wood), 0, 0.5, 0), { x, z, rot, y });
      if (i < n) {
        const mx = x0 + (x1 - x0) * ((i + 0.5) / n), mz = z0 + (z1 - z0) * ((i + 0.5) / n);
        const my = this.ground(mx, mz);
        const seg = len / n;
        b.add('solid', merge([place(box(0.08, 0.12, seg, P.wood), 0, 0.85, 0), place(box(0.08, 0.12, seg, P.wood), 0, 0.45, 0)]), { x: mx, z: mz, rot, y: my });
        this.colliders.addBox(mx, mz, 0.12, seg / 2, rot, my - 1, my + 1.0, 'fence');
      }
    }
  }

  campfire(b, x, z) {
    const y = this.ground(x, z);
    const parts = [];
    for (let i = 0; i < 8; i++) {
      const a = (i / 8) * Math.PI * 2;
      parts.push(place(ico(0.28, 0, P.stoneDark, 0.2), Math.cos(a) * 0.8, 0.12, Math.sin(a) * 0.8));
    }
    for (let i = 0; i < 3; i++) parts.push(place(cyl(0.1, 0.1, 1.2, 5, P.woodDark), 0, 0.2, 0, [0, (i / 3) * Math.PI, Math.PI / 2 - 0.25]));
    b.add('solid', merge(parts), { x, z, rot: 0, y });
    b.add('glow', place(ico(0.25, 0, 0xff7a2a), 0, 0.2, 0), { x, z, rot: 0, y });
    this.game.events.emit('fx:fire', new THREE.Vector3(x, y + 0.3, z), 1);
    (this.fires || (this.fires = [])).push(new THREE.Vector3(x, y + 0.3, z));
  }

  tent(b, x, z, rot, color = P.canvas, s = 1) {
    const y = this.ground(x, z);
    b.add('solid', merge([
      place(gable(2.6 * s, 3.2 * s, 2.2 * s, color, 0.05), 0, 0, 0),
      place(box(0.1, 2.5 * s, 0.1, P.woodDark), 0, 1.15 * s, 1.65 * s),
      place(box(0.9 * s, 1.6 * s, 0.05, 0x3a2a20), 0, 0.75 * s, 1.62 * s),
    ]), { x, z, rot, y });
    this.colliders.addBox(x, z, 1.4 * s, 1.65 * s, rot, y - 1, y + 2.0 * s, 'tent');
  }

  waystone(b, id, x, z, name) {
    const y = this.ground(x, z);
    const f = { x, z, rot: 0, y };
    b.add('solid', merge([
      place(cyl(1.6, 1.9, 0.5, 8, P.stone, 0.12), 0, 0.15, 0),
      place(cyl(0.8, 1.0, 1.1, 8, P.stoneLight, 0.1), 0, 0.9, 0),
      place(cyl(1.0, 0.8, 0.2, 8, P.stone), 0, 1.5, 0),
    ]), f);
    b.add('glow', place(torus(1.4, 0.06, 4, 24, 0x7fe8ff), 0, 0.42, 0, [Math.PI / 2, 0, 0]), f);
    const gem = new THREE.Mesh(crystal(0.45, 1.6, 0x8ff0ff), this.glowMat);
    gem.position.set(x, y + 3.0, z);
    this.group.add(gem);
    this.animated.push({ obj: gem, fn: (dt, t) => { gem.rotation.y += dt * 0.8; gem.position.y = y + 3.0 + Math.sin(t * 1.5 + x) * 0.18; } });
    this.colliders.addCircle(x, z, 1.1, y - 1, y + 1.6, 'waystone');
    const ws = { id, name, x, y, z, gem };
    this.waystones.push(ws);
    this.game.interact?.add({
      id: 'waystone:' + id, x, y: y + 1, z, r: 2.6, label: 'Touch the Waystone',
      action: () => this.game.events.emit('waystone:use', ws),
    });
    return ws;
  }

  chest(id, x, z, rot, loot, opts = {}) {
    const y = opts.y ?? this.ground(x, z);
    const g = new THREE.Group();
    const base = new THREE.Mesh(merge([place(box(1.1, 0.6, 0.7, 0x8a5a32, 0.1), 0, 0.3, 0), place(box(1.14, 0.1, 0.74, P.gold), 0, 0.12, 0), place(box(0.16, 0.2, 0.06, P.gold), 0, 0.5, 0.36)]), this.solidMat);
    const lidGeo = merge([place(box(1.12, 0.32, 0.72, 0x9a6638, 0.1), 0, 0.16, -0.36), place(box(1.16, 0.08, 0.76, P.gold), 0, 0.3, -0.36)]);
    const lid = new THREE.Mesh(lidGeo, this.solidMat);
    lid.position.set(0, 0.6, 0.36);
    g.add(base, lid);
    base.castShadow = lid.castShadow = true;
    g.position.set(x, y, z);
    g.rotation.y = rot;
    this.group.add(g);
    this.colliders.addBox(x, z, 0.55, 0.35, rot, y - 1, y + 0.9, 'chest');
    const chest = { id, x, y, z, lid, group: g, loot, opened: false, openT: 0, locked: opts.locked };
    this.chests.push(chest);
    this.game.interact?.add({
      id: 'chest:' + id, x, y: y + 0.5, z, r: 2.0, label: 'Open chest',
      enabled: () => !chest.opened,
      action: () => this.game.events.emit('chest:open', chest),
    });
    this.animated.push({ obj: lid, fn: (dt) => { if (chest.opened && chest.openT < 1) { chest.openT = Math.min(1, chest.openT + dt * 2.5); lid.rotation.x = -chest.openT * 1.9; } } });
    return chest;
  }

  pillar(b, x, z, h, broken = false, rot = 0, col = P.stoneLight) {
    const y = this.ground(x, z) - 0.2;
    const parts = [
      place(box(1.4, 0.5, 1.4, col, 0.15), 0, 0.25, 0),
      place(cyl(0.48, 0.55, h, 10, col, 0.12), 0, h / 2 + 0.4, 0),
    ];
    if (!broken) parts.push(place(box(1.3, 0.45, 1.3, col, 0.15), 0, h + 0.6, 0));
    b.add('solid', merge(parts), { x, z, rot, y });
    this.colliders.addCircle(x, z, 0.6, y - 1, y + h + (broken ? 0.4 : 0.85), 'pillar');
  }

  // ---------------------------------------------------------------- places
  buildBrightwater() {
    const L = LOC.brightwater;
    const b = new Batch();
    const rng = new RNG(101);
    this.waystone(b, 'brightwater', L.x - 4, L.z - 2, L.name);
    this.well(b, L.x + 7, L.z + 6);
    const ring = [
      { a: -1.57, r: 30, o: { w: 8, d: 6, stories: 2, roof: 0x7a5aa8, wall: 0xf4ead6 }, tag: 'elder' },
      { a: -0.6, r: 31, o: { w: 9, d: 6.5, stories: 2, roof: 0xc8513c }, tag: 'inn' },
      { a: 0.25, r: 30, o: { w: 6, d: 5, roof: 0x4c7fbf }, tag: 'house1' },
      { a: 1.0, r: 32, o: { w: 6.5, d: 5, roof: 0xd2693c }, tag: 'house2' },
      { a: 1.9, r: 30, o: { w: 5.5, d: 5, roof: 0x3f9f9a }, tag: 'house3' },
      { a: 2.6, r: 31, o: { w: 7, d: 5.5, roof: 0xc8513c, wall: 0xe2d0b0 }, tag: 'smithy' },
      { a: 3.4, r: 32, o: { w: 6, d: 5, roof: 0x4c7fbf }, tag: 'herbalist' },
      { a: 4.15, r: 31, o: { w: 6, d: 5, roof: 0xb8453a }, tag: 'house4' },
    ];
    for (const r of ring) {
      const x = L.x + Math.cos(r.a) * r.r, z = L.z + Math.sin(r.a) * r.r;
      const rot = this.facing(x, z, L.x, L.z);
      const o = { ...r.o, wall: r.o.wall ?? rng.pick(P.plaster) };
      const hs = this.house(b, x, z, rot, o);
      this.points['brightwater.' + r.tag] = hs.door;
    }
    // Smithy forge and anvil beside its house.
    {
      const sm = ring.find((r) => r.tag === 'smithy');
      const x = L.x + Math.cos(sm.a + 0.22) * (sm.r - 3), z = L.z + Math.sin(sm.a + 0.22) * (sm.r - 3);
      const y = this.ground(x, z);
      const f = { x, z, rot: this.facing(x, z, L.x, L.z), y };
      b.add('solid', merge([
        place(box(2.2, 1.2, 1.6, P.stoneDark, 0.15), 0, 0.6, 0),
        place(box(0.7, 2.6, 0.7, P.stoneDark), 0.6, 1.8, -0.4),
        place(box(0.9, 0.5, 0.45, P.iron), -1.8, 0.75, 0.6),
        place(box(0.4, 0.5, 0.4, P.woodDark), -1.8, 0.25, 0.6),
      ]), f);
      b.add('glow', place(box(1.6, 0.15, 1.0, 0xff6a1a), 0, 1.22, 0), f);
      this.colliders.addBox(x, z, 1.2, 0.9, f.rot, y - 1, y + 1.3, 'forge');
      this.points['brightwater.forge'] = { x: x + 2.5 * Math.sin(f.rot), z: z + 2.5 * Math.cos(f.rot) };
    }
    // Market stalls on the plaza.
    this.stall(b, L.x - 10, L.z + 10, this.facing(L.x - 10, L.z + 10, L.x, L.z), 0xd8503a);
    this.stall(b, L.x + 2, L.z + 14, this.facing(L.x + 2, L.z + 14, L.x, L.z), 0x4c7fbf);
    this.points['brightwater.market'] = { x: L.x - 7.5, z: L.z + 7.5 };
    this.points['brightwater.market2'] = { x: L.x + 1.5, z: L.z + 11 };
    for (let i = 0; i < 8; i++) {
      const a = (i / 8) * Math.PI * 2 + 0.2;
      this.lamp(b, L.x + Math.cos(a) * 18, L.z + Math.sin(a) * 18);
    }
    for (let i = 0; i < 10; i++) {
      const a = rng.range(0, Math.PI * 2), r = rng.range(20, 26);
      const x = L.x + Math.cos(a) * r, z = L.z + Math.sin(a) * r;
      if (this.colliders.occupied(x, z, 1.2)) continue;
      rng.chance(0.5) ? this.crate(b, x, z, rng.range(0.7, 1.0), rng.range(0, 3)) : this.barrel(b, x, z);
    }
    // Flower beds.
    for (let i = 0; i < 6; i++) {
      const a = (i / 6) * Math.PI * 2;
      const x = L.x + Math.cos(a) * 12, z = L.z + Math.sin(a) * 12;
      const y = this.ground(x, z);
      const cols = [0xff7aa8, 0xffd84a, 0xffffff, 0xb08aff];
      const parts = [place(box(2.2, 0.35, 1.0, P.woodDark), 0, 0.17, 0)];
      for (let k = 0; k < 7; k++) parts.push(place(ico(0.18, 0, cols[(i + k) % 4]), -0.9 + k * 0.3, 0.45, (k % 2) * 0.3 - 0.15));
      b.add('solid', merge(parts), { x, z, rot: a + Math.PI / 2, y });
    }
    // A fenced garden plot behind the herbalist.
    const he = ring.find((r) => r.tag === 'herbalist');
    const gx = L.x + Math.cos(he.a) * (he.r + 9), gz = L.z + Math.sin(he.a) * (he.r + 9);
    this.fence(b, gx - 5, gz - 4, gx + 5, gz - 4);
    this.fence(b, gx + 5, gz - 4, gx + 5, gz + 4);
    this.fence(b, gx + 5, gz + 4, gx - 5, gz + 4);
    this.points['brightwater.garden'] = { x: gx, z: gz };
    this.chest('bw_inn_cellar', L.x + Math.cos(-0.6) * 40, L.z + Math.sin(-0.6) * 40, 0.3, 'starter');
    this.flush(b, 'brightwater');
  }

  buildFarm() {
    const L = LOC.windmill;
    const b = new Batch();
    const hs = this.house(b, L.x - 12, L.z - 8, 0.4, { w: 7, d: 5.5, roof: 0xd2693c });
    this.points['farm.house'] = hs.door;
    // Windmill.
    const wx = L.x + 8, wz = L.z - 10;
    const y = this.ground(wx, wz);
    const f = { x: wx, z: wz, rot: -0.5, y };
    b.add('solid', merge([
      place(cyl(2.2, 3.4, 10, 8, 0xeadbb8, 0.06), 0, 5, 0),
      place(cyl(2.25, 2.25, 0.3, 8, P.timber), 0, 6, 0),
      place(cone(2.9, 3.4, 8, 0xc8513c), 0, 11.6, 0),
      place(box(1.2, 2.0, 0.2, P.woodDark), 0, 1.0, 3.1),
    ]), f);
    b.add('window', merge([place(box(0.7, 0.7, 0.1, 0xffffff), 0, 7.5, 2.55), place(box(0.6, 0.6, 0.1, 0xffffff), 0, 4.0, 2.9)]), f);
    this.colliders.addCircle(wx, wz, 3.2, y - 1, y + 10.5, 'windmill');
    const blades = new THREE.Group();
    const bladeParts = [place(cyl(0.3, 0.3, 0.8, 8, P.woodDark), 0, 0, 0, [Math.PI / 2, 0, 0])];
    for (let i = 0; i < 4; i++) {
      const a = (i / 4) * Math.PI * 2;
      const arm = merge([place(box(0.2, 7, 0.15, P.wood), 0, 3.6, 0), place(box(1.5, 5.6, 0.06, 0xf4ead6, 0.05), 0.85, 4.1, 0.05)]);
      place(arm, 0, 0, 0, [0, 0, a]);
      bladeParts.push(arm);
    }
    const bm = new THREE.Mesh(merge(bladeParts), this.solidMat);
    bm.castShadow = true;
    blades.add(bm);
    const [bx, bz] = this.local(f, 0, 3.3);
    blades.position.set(bx, y + 9.2, bz);
    blades.rotation.y = f.rot;
    this.group.add(blades);
    this.animated.push({ obj: bm, fn: (dt) => { bm.rotation.z += dt * 0.5; } });
    // Crop fields with fences.
    const fx = L.x + 6, fz = L.z + 12;
    const crops = [];
    for (let r = 0; r < 7; r++) {
      for (let c = 0; c < 12; c++) {
        const x = fx - 14 + c * 2.3, z = fz - 7 + r * 2.2;
        const gy = this.ground(x, z);
        const wheat = r % 3 !== 2;
        const g = wheat ? place(cone(0.45, 1.3, 5, 0xe8c45a, 0.15), 0, 0.65, 0) : place(ico(0.45, 0, 0x5aa83e, 0.15), 0, 0.35, 0);
        crops.push(place(g, x, gy, z));
      }
    }
    b.add('solid', merge(crops));
    this.fence(b, fx - 16, fz - 9, fx + 13, fz - 9);
    this.fence(b, fx + 13, fz - 9, fx + 13, fz + 8);
    this.fence(b, fx + 13, fz + 8, fx - 16, fz + 8);
    this.fence(b, fx - 16, fz + 8, fx - 16, fz - 2);
    this.points['farm.field'] = { x: fx - 18, z: fz };
    // Haystacks and a scarecrow.
    for (const [hx, hz] of [[L.x - 4, L.z - 18], [L.x - 1, L.z - 20]]) {
      const hy = this.ground(hx, hz);
      b.add('solid', merge([place(cyl(1.0, 1.4, 1.6, 9, 0xd9b45a, 0.15), 0, 0.8, 0), place(cone(1.0, 0.9, 9, 0xe2c26a), 0, 2.0, 0)]), { x: hx, z: hz, rot: 0, y: hy });
      this.colliders.addCircle(hx, hz, 1.3, hy - 1, hy + 2.2, 'hay');
    }
    {
      const sx = fx, sz = fz, sy = this.ground(sx, sz);
      b.add('solid', merge([place(box(0.12, 2.6, 0.12, P.wood), 0, 1.3, 0), place(box(1.8, 0.1, 0.1, P.wood), 0, 1.9, 0), place(box(0.7, 0.9, 0.4, 0x8a5a9a), 0, 1.6, 0), place(ico(0.3, 1, 0xd9b45a), 0, 2.35, 0), place(cone(0.5, 0.5, 8, 0x7a5a30), 0, 2.7, 0)]), { x: sx, z: sz, rot: 0.3, y: sy });
    }
    this.flush(b, 'farm');
  }

  buildSaltmere() {
    const L = LOC.saltmere;
    const b = new Batch();
    this.waystone(b, 'saltmere', L.x, L.z, L.name);
    const houses = [[-16, -10], [-14, 10], [12, -14], [-2, -20]];
    const roofs = [0x4c7fbf, 0x3f9f9a, 0x4c7fbf, 0x5a6fa8];
    houses.forEach(([dx, dz], i) => {
      const x = L.x + dx, z = L.z + dz;
      const hs = this.house(b, x, z, this.facing(x, z, L.x, L.z), { w: 5.5, d: 4.5, h: 3, roof: roofs[i], wall: 0xf4f0e6 });
      this.points['saltmere.house' + i] = hs.door;
    });
    // Find the shoreline toward the sea and build a pier out over the water.
    const dir = new THREE.Vector2(0.75, 0.66).normalize();
    let sx = L.x, sz = L.z;
    for (let i = 0; i < 80; i++) { if (this.ground(sx, sz) < 0.5) break; sx += dir.x; sz += dir.y; }
    const rot = Math.atan2(dir.x, dir.y);
    const len = 26;
    const planks = [];
    for (let i = -4; i < len; i += 1) {
      planks.push(place(box(3.2, 0.15, 0.9, i % 2 ? P.wood : 0x9a7048, 0.08), 0, 1.2, i));
      if (i % 4 === 0) for (const s of [-1.4, 1.4]) planks.push(place(cyl(0.15, 0.15, 5, 6, P.woodDark), s, -1.2, i));
    }
    const pf = { x: sx, z: sz, rot, y: 0 };
    b.add('solid', merge(planks), pf);
    const [pcx, pcz] = this.local(pf, 0, (len - 4) / 2);
    this.colliders.addBox(pcx, pcz, 1.6, (len + 4) / 2, rot, -6, 1.28, 'pier');
    this.points['saltmere.pierEnd'] = (() => { const [x, z] = this.local(pf, 0, len - 3); return { x, z, y: 1.28 }; })();
    // A moored boat.
    {
      const [bx, bz] = this.local(pf, 3.4, 12);
      const hull = merge([
        place(box(1.6, 0.7, 4.2, 0x7a5030, 0.1), 0, 0.15, 0),
        place(cone(0.8, 1.2, 4, 0x7a5030), 0, 0.15, 2.6, [Math.PI / 2, Math.PI / 4, 0], [1, 1, 0.75]),
        place(box(1.7, 0.12, 4.3, 0xf2e6c8), 0, 0.5, 0),
        place(cyl(0.06, 0.06, 3.5, 5, P.woodDark), 0, 2.0, 0.3),
      ]);
      const boat = new THREE.Mesh(hull, this.solidMat);
      boat.position.set(bx, 0.1, bz);
      boat.rotation.y = rot;
      this.group.add(boat);
      this.animated.push({ obj: boat, fn: (dt, t) => { boat.position.y = 0.05 + Math.sin(t * 1.2) * 0.08; boat.rotation.z = Math.sin(t * 0.9) * 0.05; } });
    }
    // Drying racks and barrels.
    for (let i = 0; i < 3; i++) {
      const x = L.x + 10 + i * 3, z = L.z + 8 - i * 2;
      const y = this.ground(x, z);
      b.add('solid', merge([place(box(0.1, 2, 0.1, P.wood), -1, 1, 0), place(box(0.1, 2, 0.1, P.wood), 1, 1, 0), place(box(2.2, 0.06, 0.06, P.wood), 0, 1.9, 0), place(box(1.8, 1.2, 0.04, 0xb8a888), 0, 1.25, 0)]), { x, z, rot: 0.9, y });
    }
    this.barrel(b, L.x + 6, L.z - 5); this.barrel(b, L.x + 7, L.z - 4); this.crate(b, L.x - 6, L.z + 5, 0.9);
    this.lamp(b, L.x + 5, L.z + 3); this.lamp(b, L.x - 6, L.z - 4);
    this.flush(b, 'saltmere');
  }

  buildLighthouse() {
    const L = LOC.lighthouse;
    const b = new Batch();
    const y = this.ground(L.x, L.z);
    const f = { x: L.x, z: L.z, rot: 0, y };
    const stripe = (x, yy) => (Math.floor(yy / 3) % 2 === 0 ? 0xf4f0e6 : 0xc8453a);
    b.add('solid', merge([
      place(cyl(3.6, 4.2, 1.5, 12, P.stone, 0.1), 0, 0.5, 0),
      place(paint(new THREE.CylinderGeometry(2.2, 3.2, 18, 12, 6), stripe), 0, 10, 0),
      place(cyl(3.0, 3.0, 0.4, 12, P.iron), 0, 19.2, 0),
      place(cone(2.4, 2.4, 12, 0xc8453a), 0, 23.4, 0),
      place(box(1.2, 2.2, 0.2, P.woodDark), 0, 1.6, 3.1),
    ]), f);
    b.add('window', place(cyl(1.7, 1.7, 2.6, 10, 0xffffff), 0, 20.8, 0), f);
    this.colliders.addCircle(L.x, L.z, 3.4, y - 2, y + 24, 'lighthouse');
    const beamGeo = new THREE.ConeGeometry(4, 60, 16, 1, true);
    beamGeo.translate(0, -30, 0);
    beamGeo.rotateZ(Math.PI / 2);
    const beamMat = new THREE.MeshBasicMaterial({ color: 0xfff1b0, transparent: true, opacity: 0.0, depthWrite: false, blending: THREE.AdditiveBlending, side: THREE.DoubleSide, fog: false });
    const beam = new THREE.Mesh(beamGeo, beamMat);
    beam.position.set(L.x, y + 20.8, L.z);
    this.group.add(beam);
    this.lighthouseBeam = beam;
    this.animated.push({ obj: beam, fn: (dt) => { beam.rotation.y += dt * 0.6; beamMat.opacity = 0.16 * (this.game.sky?.uniforms.uNight.value ?? 0); beam.visible = beamMat.opacity > 0.01; } });
    const hs = this.house(b, L.x - 9, L.z - 6, 0.9, { w: 5, d: 4.5, h: 2.8, roof: 0x4c7fbf });
    this.points['lighthouse.door'] = hs.door;
    this.chest('lighthouse_top', L.x + 5, L.z - 2, -0.4, 'coast');
    this.flush(b, 'lighthouse');
  }

  buildScholars() {
    const L = LOC.scholars;
    const b = new Batch();
    this.waystone(b, 'scholars', L.x + 10, L.z + 4, L.name);
    const tents = [[-9, -6, 0x5a6fa8], [-10, 6, P.canvas], [3, 11, 0x3f9f9a], [6, -10, P.canvas]];
    tents.forEach(([dx, dz, c], i) => this.tent(b, L.x + dx, L.z + dz, this.facing(L.x + dx, L.z + dz, L.x, L.z), c, 1.25));
    this.campfire(b, L.x, L.z);
    // Desk with books and a telescope aimed at the Heartshard.
    const x = L.x - 2, z = L.z - 9, y = this.ground(x, z);
    const f = { x, z, rot: this.facing(x, z, LOC.heartshard.x, LOC.heartshard.z), y };
    b.add('solid', merge([
      place(box(2.4, 0.12, 1.1, P.wood), 0, 0.9, 0),
      ...[[-1.05, -0.45], [1.05, -0.45], [-1.05, 0.45], [1.05, 0.45]].map(([a, c]) => place(box(0.1, 0.9, 0.1, P.woodDark), a, 0.45, c)),
      place(box(0.5, 0.25, 0.35, 0x7a3a3a), -0.6, 1.08, 0.1),
      place(box(0.45, 0.18, 0.32, 0x3a5a8a), -0.55, 1.29, 0.05),
      place(box(0.6, 0.02, 0.4, 0xf4ead6), 0.5, 0.97, 0),
      place(cyl(0.05, 0.05, 1.6, 5, P.iron), 1.6, 0.8, 0.8),
      place(cyl(0.12, 0.18, 1.6, 8, P.gold), 1.6, 1.8, 0.4, [-0.9, 0, 0]),
    ]), f);
    this.colliders.addBox(x, z, 1.3, 0.6, f.rot, y - 1, y + 1.0, 'desk');
    this.points['scholars.desk'] = { x: x + Math.sin(f.rot) * -1.5, z: z + Math.cos(f.rot) * -1.5 };
    this.points['scholars.fire'] = { x: L.x + 2.2, z: L.z + 1.5 };
    this.crate(b, L.x + 8, L.z - 6, 0.9); this.crate(b, L.x + 8.8, L.z - 5, 0.7, 0.5);
    this.flush(b, 'scholars');
  }

  buildHeartshard() {
    const L = LOC.heartshard;
    const b = new Batch();
    const y = 5.6;
    const f = { x: L.x, z: L.z, rot: 0, y };
    b.add('solid', merge([
      place(cyl(16, 17, 1.2, 24, P.stoneLight, 0.06), 0, -0.4, 0),
      place(cyl(12, 12.5, 0.4, 24, 0xd8d0c0, 0.05), 0, 0.4, 0),
      place(cyl(2.4, 3.0, 1.3, 8, P.stone, 0.1), 0, 1.2, 0),
      place(cyl(2.0, 2.0, 0.25, 8, P.stoneLight), 0, 1.95, 0),
    ]), f);
    // Ring of pillars.
    for (let i = 0; i < 8; i++) {
      const a = (i / 8) * Math.PI * 2;
      this.pillar(b, L.x + Math.cos(a) * 14, L.z + Math.sin(a) * 14, 5.5, i % 3 === 1, a, 0xe0d8c8);
    }
    b.add('glow', place(torus(10.5, 0.08, 4, 48, 0x9ff0ff), 0, 0.62, 0, [Math.PI / 2, 0, 0]), f);
    this.colliders.addCircle(L.x, L.z, 2.6, y - 2, y + 2.05, 'pedestal');
    this.points['heartshard.pedestal'] = { x: L.x, z: L.z + 4 };
    // Three sockets on the pedestal that light up as shards are returned.
    this.sockets = [];
    for (let i = 0; i < 3; i++) {
      const a = (i / 3) * Math.PI * 2 + Math.PI / 2;
      const gem = new THREE.Mesh(crystal(0.28, 1.0, [0x7dff9a, 0x9fdcff, 0xff8a4a][i]), this.glowMat);
      gem.position.set(L.x + Math.cos(a) * 1.2, y + 2.6, L.z + Math.sin(a) * 1.2);
      gem.visible = false;
      this.group.add(gem);
      this.sockets.push(gem);
    }
    this.game.interact?.add({
      id: 'heartshard:pedestal', x: L.x, y: y + 2, z: L.z, r: 4.5, label: 'Examine the pedestal',
      action: () => this.game.events.emit('heartshard:pedestal'),
    });
    // The Heartshard itself, floating high above with orbiting fragments.
    const hs = new THREE.Group();
    hs.position.set(L.x, 68, L.z);
    const coreCol = (x, yy, z, nx, ny, nz) => new THREE.Color().setHSL(0.52 + yy * 0.004 + nx * 0.03, 0.75, 0.62 + ny * 0.12);
    const core = new THREE.Mesh(paint(crystal(7.5, 42, 0xffffff, 6), coreCol), this.glowMat);
    hs.add(core);
    const cracks = new THREE.Mesh(merge([0, 1, 2, 3].map((i) => place(box(0.25, 9 + i * 2, 0.25, 0x2a1840), Math.cos(i * 1.6) * 6.3, -6 + i * 4, Math.sin(i * 1.6) * 6.3, [0.3 * i, i * 1.6, 0.4]))), this.solidMat);
    hs.add(cracks);
    const frags = new THREE.Group();
    for (let i = 0; i < 14; i++) {
      const a = (i / 14) * Math.PI * 2;
      const r = 14 + (i % 3) * 4;
      const m = new THREE.Mesh(crystal(0.6 + (i % 4) * 0.35, 3 + (i % 5), [0x8ff0ff, 0xc0a0ff, 0x9fffe0][i % 3]), this.glowMat);
      m.position.set(Math.cos(a) * r, Math.sin(i * 2.1) * 8, Math.sin(a) * r);
      m.rotation.set(i, i * 0.7, i * 0.3);
      frags.add(m);
    }
    hs.add(frags);
    this.group.add(hs);
    this.heartshard = { group: hs, core, cracks, frags, restored: false };
    this.animated.push({ obj: hs, fn: (dt, t) => {
      core.rotation.y += dt * 0.08;
      hs.position.y = 68 + Math.sin(t * 0.3) * 1.5;
      frags.rotation.y -= dt * (this.heartshard.restored ? 0.25 : 0.06);
      for (const m of frags.children) m.rotation.x += dt * 0.3;
    } });
    this.flush(b, 'heartshard');
  }

  buildRuins() {
    const L = LOC.whisperwood;
    const b = new Batch();
    const rng = new RNG(303);
    this.waystone(b, 'whisperwood', L.x + 26, L.z + 18, L.name);
    const moss = (x, y, z, nx, ny) => (ny > 0.5 ? new THREE.Color(0x6f9a4a) : new THREE.Color(0xb8b2a2));
    // Great hall outline: colonnade rectangle, some pillars toppled.
    for (let i = 0; i < 6; i++) {
      for (const side of [-1, 1]) {
        const x = L.x - 12 + i * 4.8, z = L.z + side * 8;
        const broken = rng.chance(0.35);
        if (rng.chance(0.15)) {
          const y = this.ground(x, z);
          b.add('solid', place(paint(new THREE.CylinderGeometry(0.5, 0.55, 6, 10), moss), 0, 0.5, 0, [Math.PI / 2, rng.range(0, 3), 0]), { x, z, rot: rng.range(0, 3), y });
          this.colliders.addCircle(x, z, 1.6, y - 1, y + 1.0, 'fallen');
        } else this.pillar(b, x, z, broken ? rng.range(2, 4) : 6.5, broken, 0, 0xc4bcae);
      }
    }
    // Lintels across the standing ends.
    for (const xo of [-12, 12]) {
      const x = L.x + xo, z = L.z;
      const y = this.ground(x, z);
      b.add('solid', place(box(1.4, 1.0, 17.5, 0xc4bcae, 0.12), 0, 7.4, 0), { x, z, rot: 0, y });
      this.pillar(b, x, z - 8, 6.5); this.pillar(b, x, z + 8, 6.5);
    }
    // Raised dais and altar (the Warden's arena).
    {
      const x = L.x, z = L.z, y = this.ground(x, z);
      b.add('solid', merge([
        place(box(10, 1.0, 10, 0xb8b2a2, 0.1), 0, 0.3, 0),
        place(box(12, 0.5, 12, 0xa8a292, 0.1), 0, -0.1, 0),
        place(box(2.0, 1.2, 1.2, 0xc4bcae, 0.1), 0, 1.4, -2),
      ]), { x, z, rot: 0, y });
      b.add('glow', merge([place(torus(3.4, 0.07, 4, 32, 0x7dff9a), 0, 0.82, 0, [Math.PI / 2, 0, 0]), place(torus(2.2, 0.05, 4, 6, 0x7dff9a), 0, 0.82, 0, [Math.PI / 2, 0, 0])]), { x, z, rot: 0, y });
      this.colliders.addBox(x, z, 5, 5, 0, y - 2, y + 0.8, 'dais');
      this.colliders.addBox(x, z - 2, 1.0, 0.6, 0, y, y + 2.0, 'altar');
      this.points['ruins.altar'] = { x, z: z + 1, y: y + 0.8 };
    }
    // Crumbling walls with gaps.
    for (const [x0, z0, x1, z1] of [[-20, -14, -6, -16], [6, -16, 20, -13], [-20, 14, -4, 16], [8, 15, 20, 12], [-22, -12, -21, 10]]) {
      const ax = L.x + x0, az = L.z + z0, bx = L.x + x1, bz = L.z + z1;
      const len = Math.hypot(bx - ax, bz - az), rot = Math.atan2(bx - ax, bz - az);
      const n = Math.floor(len / 2);
      for (let i = 0; i < n; i++) {
        if (rng.chance(0.25)) continue;
        const t = (i + 0.5) / n;
        const x = ax + (bx - ax) * t, z = az + (bz - az) * t;
        const y = this.ground(x, z);
        const h = rng.range(1.0, 3.6);
        b.add('solid', place(paint(new THREE.BoxGeometry(1.0, h, 2.05), moss, 0.12), 0, h / 2 - 0.3, 0), { x, z, rot, y });
        this.colliders.addBox(x, z, 0.5, 1.02, rot, y - 1, y + h - 0.3, 'wall');
      }
    }
    // Vines drooping from lintels (thin green strands).
    for (let i = 0; i < 14; i++) {
      const x = L.x + rng.pick([-12, 12]) + rng.range(-0.5, 0.5), z = L.z + rng.range(-7, 7);
      const y = this.ground(x, z);
      const h = rng.range(1.5, 4);
      b.add('solid', place(box(0.08, h, 0.25, 0x4f8a3a), 0, 6.9 - h / 2, 0), { x, z, rot: 0, y });
    }
    this.chest('ruins_hidden', L.x - 18, L.z + 11, 1.2, 'ruins');
    this.points['ruins.entry'] = { x: L.x + 22, z: L.z + 4 };
    this.flush(b, 'ruins');
  }

  buildShrine() {
    const L = LOC.frostspire;
    const b = new Batch();
    this.waystone(b, 'frostspire', L.x + 15, L.z + 12, L.name);
    const y = this.ground(L.x, L.z);
    const f = { x: L.x, z: L.z - 8, rot: 0, y };
    const snowy = (base) => (x, yy, z, nx, ny) => (ny > 0.6 ? new THREE.Color(0xf4f8ff) : new THREE.Color(base));
    // Stepped platform.
    b.add('solid', merge([
      place(paint(new THREE.BoxGeometry(14, 1, 12), snowy(0x8a8f9a)), 0, 0.2, 0),
      place(paint(new THREE.BoxGeometry(10, 1, 8.5), snowy(0x9aa0aa)), 0, 1.2, 0),
      place(box(4, 0.5, 2, 0x8a8f9a), 0, 0.45, 6.6),
    ]), f);
    this.colliders.addBox(f.x, f.z, 7, 6, 0, y - 2, y + 0.7, 'shrine-base');
    this.colliders.addBox(f.x, f.z, 5, 4.25, 0, y - 2, y + 1.7, 'shrine-top');
    // Pillars and a sweeping roof.
    const parts = [];
    for (const sx of [-1, 1]) for (const sz of [-1, 1]) parts.push(place(cyl(0.35, 0.4, 5, 8, 0xa83a3a), sx * 3.8, 4.2, sz * 3.1));
    parts.push(place(box(9.2, 0.5, 7.6, 0x5a3a2a), 0, 6.8, 0));
    parts.push(place(paint(gable(9.4, 8, 2.8, 0x3a4a6a, 1.2), snowy(0x3a4a6a)), 0, 7.0, 0));
    parts.push(place(box(2, 2.4, 1.2, 0x9aa0aa, 0.1), 0, 2.9, -2.4));
    b.add('solid', merge(parts), f);
    for (const sx of [-1, 1]) for (const sz of [-1, 1]) {
      const [px, pz] = this.local(f, sx * 3.8, sz * 3.1);
      this.colliders.addCircle(px, pz, 0.45, y - 1, y + 7, 'pillar');
    }
    b.add('glow', place(crystal(0.5, 1.4, 0x9fdcff), 0, 4.8, -2.4), f);
    // Braziers with blue flames and prayer flags.
    for (const sx of [-1, 1]) {
      const [px, pz] = this.local(f, sx * 6, 7.5);
      const py = this.ground(px, pz);
      b.add('solid', merge([place(cyl(0.15, 0.25, 1.4, 6, P.iron), 0, 0.7, 0), place(cyl(0.6, 0.3, 0.5, 8, P.iron), 0, 1.6, 0)]), { x: px, z: pz, rot: 0, y: py });
      this.game.events.emit('fx:fire', new THREE.Vector3(px, py + 1.9, pz), 0.7, 0x6ac8ff);
      (this.fires || (this.fires = [])).push(new THREE.Vector3(px, py + 1.9, pz));
      this.colliders.addCircle(px, pz, 0.6, py - 1, py + 1.8, 'brazier');
    }
    const flagCols = [0xd84a3a, 0xf2c94a, 0x4a9ad8, 0x6ac84a, 0xf4f0e6];
    const flags = [];
    for (let i = 0; i < 12; i++) flags.push(place(box(0.5, 0.7, 0.03, flagCols[i % 5]), -7 + i * 1.25, 6.3 - Math.sin((i / 11) * Math.PI) * 1.2, 7.5));
    b.add('solid', merge(flags), f);
    this.points['shrine.arena'] = { x: L.x, z: L.z + 10 };
    this.points['shrine.altar'] = (() => { const [x, z] = this.local(f, 0, -0.5); return { x, z, y: y + 1.7 }; })();
    this.chest('shrine_offering', L.x - 9, L.z + 6, 0.5, 'frost');
    this.flush(b, 'shrine');
  }

  buildVault() {
    const L = LOC.sunscar;
    const b = new Batch();
    this.waystone(b, 'sunscar', L.x - 18, L.z + 14, L.name);
    const y = this.ground(L.x, L.z);
    const f = { x: L.x, z: L.z - 12, rot: 0, y };
    const sand = 0xc0703f, carve = 0xd89a62;
    b.add('solid', merge([
      lumpy(place(box(34, 22, 10, sand, 0.1), 0, 9, -2), 0.2, 1, 5),
      place(box(10, 12, 1.5, carve, 0.08), 0, 6, 3.2),
      place(box(12, 1.5, 2.5, carve), 0, 12.5, 3.4),
      place(box(14, 0.8, 4, carve, 0.1), 0, 0.1, 5),
      place(box(10, 0.8, 3, carve, 0.1), 0, 0.6, 4.4),
      place(box(6, 8.5, 0.3, 0x1a0e0a), 0, 4.6, 4.0),
      ...[-1, 1].map((s) => place(box(1.2, 10, 1.2, carve, 0.1), s * 4.4, 5, 4.2)),
    ]), f);
    // Two seated guardian statues.
    for (const s of [-1, 1]) {
      const g = merge([
        place(box(3, 3, 3, carve, 0.1), 0, 1.5, 0),
        place(box(2.2, 3.4, 1.6, carve, 0.1), 0, 4.6, -0.4),
        place(box(1.6, 1.6, 1.6, carve), 0, 7.1, -0.3),
        place(box(0.6, 2.8, 0.7, carve), -1.4, 4.4, 0.4),
        place(box(0.6, 2.8, 0.7, carve), 1.4, 4.4, 0.4),
      ]);
      b.add('solid', g, { x: L.x + s * 10, z: L.z - 4, rot: 0, y: this.ground(L.x + s * 10, L.z - 4) });
      b.add('glow', merge([place(box(0.3, 0.2, 0.1, 0xff8a3a), -0.35, 7.3, 0.52), place(box(0.3, 0.2, 0.1, 0xff8a3a), 0.35, 7.3, 0.52)]), { x: L.x + s * 10, z: L.z - 4, rot: 0, y: this.ground(L.x + s * 10, L.z - 4) });
      this.colliders.addBox(L.x + s * 10, L.z - 4, 1.6, 1.6, 0, y - 2, y + 7.5, 'statue');
    }
    b.add('glow', place(torus(2.0, 0.12, 4, 6, 0xff8a3a), 0, 6.5, 4.2), f);
    this.colliders.addBox(f.x, f.z - 2, 17, 5, 0, y - 5, y + 20, 'cliff');
    for (const s of [-1, 1]) {
      const [px, pz] = this.local(f, s * 6.5, 8);
      const py = this.ground(px, pz);
      b.add('solid', merge([place(cyl(0.2, 0.3, 1.6, 6, P.iron), 0, 0.8, 0), place(cyl(0.7, 0.35, 0.5, 8, P.iron), 0, 1.8, 0)]), { x: px, z: pz, rot: 0, y: py });
      this.game.events.emit('fx:fire', new THREE.Vector3(px, py + 2.1, pz), 0.8);
      (this.fires || (this.fires = [])).push(new THREE.Vector3(px, py + 2.1, pz));
      this.colliders.addCircle(px, pz, 0.7, py - 1, py + 2, 'brazier');
    }
    const [dx, dz] = this.local(f, 0, 6.5);
    this.points['vault.door'] = { x: dx, z: dz };
    this.game.interact?.add({
      id: 'vault:door', x: dx, y: y + 1.5, z: dz, r: 3.5, label: 'Enter the Sunscar Vault',
      action: () => this.game.events.emit('dungeon:enter', 'sunscar'),
    });
    this.flush(b, 'vault');
  }

  buildBandits() {
    const L = LOC.banditcamp;
    const b = new Batch();
    const rng = new RNG(505);
    this.campfire(b, L.x, L.z);
    for (let i = 0; i < 3; i++) {
      const a = (i / 3) * Math.PI * 2 + 0.4;
      this.tent(b, L.x + Math.cos(a) * 10, L.z + Math.sin(a) * 10, this.facing(L.x + Math.cos(a) * 10, L.z + Math.sin(a) * 10, L.x, L.z), P.redCanvas, 1.2);
    }
    // Palisade arc of sharpened logs.
    for (let i = 0; i < 26; i++) {
      const a = (i / 26) * Math.PI * 1.5 + 2.4;
      const x = L.x + Math.cos(a) * 19, z = L.z + Math.sin(a) * 19;
      const y = this.ground(x, z);
      const h = rng.range(3, 4);
      b.add('solid', merge([place(cyl(0.35, 0.35, h, 6, P.woodDark, 0.1), 0, h / 2 - 0.3, 0), place(cone(0.35, 0.8, 6, P.wood), 0, h - 0.3 + 0.4, 0)]), { x, z, rot: 0, y });
      this.colliders.addCircle(x, z, 0.5, y - 1, y + h, 'palisade');
    }
    // Lookout platform.
    {
      const x = L.x + 12, z = L.z - 10, y = this.ground(x, z);
      const parts = [];
      for (const sx of [-1, 1]) for (const sz of [-1, 1]) parts.push(place(box(0.25, 5, 0.25, P.woodDark), sx * 1.4, 2.5, sz * 1.4));
      parts.push(place(box(3.4, 0.25, 3.4, P.wood, 0.1), 0, 5, 0));
      parts.push(place(box(3.4, 0.9, 0.1, P.wood), 0, 5.5, 1.7));
      parts.push(place(box(3.4, 0.9, 0.1, P.wood), 0, 5.5, -1.7));
      b.add('solid', merge(parts), { x, z, rot: 0.3, y });
      this.colliders.addBox(x, z, 1.7, 1.7, 0.3, y + 4.6, y + 5.13, 'lookout');
      this.points['bandits.lookout'] = { x, z, y: y + 5.13 };
    }
    for (let i = 0; i < 6; i++) {
      const a = rng.range(0, 6.28), r = rng.range(5, 14);
      const x = L.x + Math.cos(a) * r, z = L.z + Math.sin(a) * r;
      if (!this.colliders.occupied(x, z, 1)) rng.chance(0.5) ? this.crate(b, x, z, 0.9, rng.range(0, 3)) : this.barrel(b, x, z);
    }
    this.chest('bandit_stash', L.x - 6, L.z - 9, 0.5, 'bandit');
    this.flush(b, 'bandits');
  }

  buildWatchtower() {
    const L = LOC.watchtower;
    const b = new Batch();
    const y = this.ground(L.x, L.z);
    const f = { x: L.x, z: L.z, rot: 0, y };
    const parts = [place(cyl(3.2, 3.8, 14, 12, P.stone, 0.12), 0, 6.5, 0)];
    for (let i = 0; i < 10; i++) {
      if (i === 3 || i === 7) continue;
      const a = (i / 10) * Math.PI * 2;
      parts.push(place(box(1.1, 1.4, 0.7, P.stone, 0.15), Math.cos(a) * 2.9, 14.1, Math.sin(a) * 2.9, [0, -a, 0]));
    }
    parts.push(place(box(1.3, 2.2, 0.3, P.woodDark), 0, 1.1, 3.6));
    // A spiral of stone steps up the outside.
    for (let i = 0; i < 26; i++) {
      const a = (i / 26) * Math.PI * 2.4 + 0.6;
      parts.push(place(box(1.6, 0.35, 1.0, P.stoneLight, 0.1), Math.cos(a) * 4.3, 0.4 + i * 0.52, Math.sin(a) * 4.3, [0, -a, 0]));
      const sx = L.x + Math.cos(a) * 4.3, sz = L.z + Math.sin(a) * 4.3;
      this.colliders.addBox(sx, sz, 0.8, 0.55, -a + Math.PI / 2, y - 2, y + 0.57 + i * 0.52, 'step');
    }
    b.add('solid', merge(parts), f);
    this.colliders.addCircle(L.x, L.z, 3.4, y - 2, y + 13.5, 'tower');
    b.add('glow', place(box(0.6, 0.6, 0.6, 0xffaa4a), 0, 14.3, 0), f);
    this.chest('watchtower_top', L.x + 0.5, L.z, 0, 'tower', { y: y + 13.5 });
    this.flush(b, 'watchtower');
  }

  buildStoneCircle() {
    const L = LOC.stonecircle;
    const b = new Batch();
    const rng = new RNG(707);
    for (let i = 0; i < 9; i++) {
      const a = (i / 9) * Math.PI * 2;
      const x = L.x + Math.cos(a) * 11, z = L.z + Math.sin(a) * 11;
      const y = this.ground(x, z);
      const h = rng.range(4, 6.5);
      b.add('solid', lumpy(place(box(1.6, h, 0.9, 0x8f8a80, 0.15), 0, h / 2 - 0.4, 0, [rng.range(-0.08, 0.08), 0, rng.range(-0.08, 0.08)]), 0.12, 1, i), { x, z, rot: -a, y });
      b.add('glow', place(box(0.3, 1.2, 0.05, 0x9ff0ff), 0, h * 0.55, 0.47), { x, z, rot: -a + Math.PI, y });
      this.colliders.addBox(x, z, 0.8, 0.45, -a, y - 1, y + h - 0.4, 'standing-stone');
    }
    const y = this.ground(L.x, L.z);
    b.add('solid', place(cyl(2.4, 2.6, 0.6, 10, 0x8f8a80, 0.1), 0, 0.2, 0), { x: L.x, z: L.z, rot: 0, y });
    b.add('glow', place(torus(1.6, 0.06, 4, 5, 0x9ff0ff), 0, 0.52, 0, [Math.PI / 2, 0, 0]), { x: L.x, z: L.z, rot: 0, y });
    this.colliders.addCircle(L.x, L.z, 2.5, y - 1, y + 0.5, 'altar-stone');
    this.points['stonecircle.center'] = { x: L.x, z: L.z };
    this.flush(b, 'stonecircle');
  }

  buildSkyIslands() {
    const islands = [
      { x: -250, z: 250, y: 92, r: 14, chest: 'sky_west' },
      { x: 360, z: -380, y: 135, r: 18, chest: 'sky_east' },
      { x: -110, z: -190, y: 74, r: 10 },
      { x: 70, z: -10, y: 46, r: 6 },
      { x: -60, z: -120, y: 52, r: 5 },
    ];
    const grass = 0x78c255;
    this.skyIslands = [];
    for (const [i, s] of islands.entries()) {
      const parts = [];
      const rock = new THREE.ConeGeometry(s.r, s.r * 1.6, 9, 3);
      rock.rotateX(Math.PI);
      rock.translate(0, -s.r * 0.8, 0);
      parts.push(lumpy(paint(rock, (x, y) => new THREE.Color(y > -1 ? 0x8a7a68 : 0x6a5e52), 0.15, i), s.r * 0.08, 1, i));
      parts.push(lumpy(paint(new THREE.CylinderGeometry(s.r * 1.02, s.r * 0.98, 1.2, 12), (x, y, z, nx, ny) => new THREE.Color(ny > 0.5 ? grass : 0x6a5a48)), 0.25, 1, i + 7));
      const nt = Math.floor(s.r / 4);
      for (let k = 0; k < nt; k++) {
        const a = k * 2.4, rr = s.r * 0.55 * ((k % 2) + 0.4) / 1.4;
        const leaf = (x, y) => new THREE.Color(0x4f9a44).lerp(new THREE.Color(0xa0d870), Math.min(Math.max((y - 2.4) / 3, 0), 1));
        parts.push(place(cyl(0.18, 0.3, 2.6, 6, 0x6e4b33), Math.cos(a) * rr, 1.9, Math.sin(a) * rr));
        parts.push(place(lumpy(paint(new THREE.IcosahedronGeometry(1.6, 1), leaf), 0.3, 1, k), Math.cos(a) * rr, 3.8, Math.sin(a) * rr));
      }
      const g = new THREE.Mesh(merge(parts), this.solidMat);
      g.position.set(s.x, s.y, s.z);
      g.castShadow = true;
      g.receiveShadow = true;
      const gem = new THREE.Mesh(crystal(0.5, 2.4, 0xb48cff), this.glowMat);
      gem.position.set(-s.r * 0.3, 1.6, s.r * 0.25);
      g.add(gem);
      this.group.add(g);
      const base = s.y;
      this.animated.push({ obj: g, fn: (dt, t) => { g.position.y = base + Math.sin(t * 0.4 + i) * 0.6; } });
      this.colliders.addCircle(s.x, s.z, s.r * 0.95, s.y - s.r * 1.5, s.y + 0.6, 'sky-island');
      this.skyIslands.push({ ...s, mesh: g });
      if (s.chest) this.chest(s.chest, s.x + 2, s.z - 2, 0.6, 'sky', { y: s.y + 0.6 });
    }
  }

  buildSignposts() {
    const b = new Batch();
    const posts = [
      { x: 30, z: 330, signs: [['Brightwater', 0.0], ['Whisperwood', -1.6], ['Heartshard', 3.0]] },
      { x: 30, z: 160, signs: [['Sunscar Vault', 1.5], ['Brightwater', 0.0], ['Frostspire', 2.5]] },
      { x: 250, z: 392, signs: [['Saltmere', 0.9], ['Brightwater', -1.6]] },
    ];
    for (const p of posts) {
      const y = this.ground(p.x + 3, p.z);
      const parts = [place(box(0.18, 3, 0.18, P.woodDark), 0, 1.5, 0)];
      p.signs.forEach(([, rot], i) => parts.push(place(box(1.6, 0.35, 0.06, P.wood), 0.7, 2.6 - i * 0.45, 0, [0, rot, 0])));
      b.add('solid', merge(parts), { x: p.x + 3, z: p.z, rot: 0, y });
      this.colliders.addCircle(p.x + 3, p.z, 0.2, y - 1, y + 3, 'sign');
    }
    this.flush(b, 'signs');
  }

  update(dt, t) {
    for (const a of this.animated) a.fn(dt, t);
    // Hide small props that are far from the camera.
    this.cullT = (this.cullT || 0) - dt;
    if (this.cullT <= 0) {
      this.cullT = 0.5;
      const c = this.game.camera.position;
      for (const ch of this.chests) if (ch.x < 4000) ch.group.visible = Math.abs(ch.x - c.x) + Math.abs(ch.z - c.z) < 220;
    }
    const night = this.game.sky.uniforms.uNight.value;
    this.windowMat.color.setRGB(0.29 + night * 1.25, 0.35 + night * 0.85, 0.43 + night * 0.15);
  }
}
