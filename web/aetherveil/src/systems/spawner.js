// Spawn zones scattered by region: creatures appear when the player comes
// near and despawn when far away. Killed packs respawn after an in-game day.

import { Enemy } from '../entities/enemies.js';
import { RNG } from '../core/noise.js';
import { LOC } from '../world/locations.js';

const NEAR = 120, FAR = 175;

class Zone {
  constructor(spawner, def) {
    this.s = spawner;
    Object.assign(this, def);
    this.alive = [];
    this.active = false;
    this.clearedAt = -1e9; // game hours
    this.killed = 0;
  }
  onKilled(e) {
    this.killed++;
    if (this.alive.every((a) => a.dead || a === e)) this.clearedAt = this.s.game.totalHours;
  }
}

export class Spawner {
  constructor(game) {
    this.game = game;
    this.zones = [];
    this.checkT = 0;
    this.buildZones();
  }

  buildZones() {
    const w = this.game.world;
    const Z = (type, x, z, n, r = 14, extra = {}) => this.zones.push(new Zone(this, { type, x, z, n, r, ...extra }));
    // Hand-placed groups around landmarks.
    Z('gloop', 120, 300, 3, 12); Z('gloop', -60, 360, 3, 14); Z('gloop', 180, 470, 2, 10);
    Z('wolf', 280, 330, 3, 10, { tag: 'farmwolves' }); Z('wolf', 330, 270, 2, 10, { tag: 'farmwolves' });
    Z('bandit', LOC.banditcamp.x, LOC.banditcamp.z, 3, 10); Z('bandit_mage', LOC.banditcamp.x + 4, LOC.banditcamp.z - 4, 1, 4);
    Z('bandit', -280, 380, 2, 8);
    Z('golem', LOC.whisperwood.x + 16, LOC.whisperwood.z, 1, 6); Z('golem', LOC.whisperwood.x - 16, LOC.whisperwood.z + 4, 1, 6);
    Z('wolf', -470, 130, 3, 12); Z('wolf', -420, -60, 3, 12); Z('gloop', -350, 120, 3, 12);
    Z('wisp', LOC.stonecircle.x, LOC.stonecircle.z, 4, 14, { night: true });
    Z('frost_wolf', -200, -620, 3, 12); Z('frost_gloop', -100, -560, 3, 12); Z('frost_wolf', -60, -720, 2, 10);
    Z('magma_gloop', 520, -60, 3, 12); Z('golem', 640, -100, 1, 8); Z('magma_gloop', 600, -260, 3, 12);
    Z('bandit', LOC.watchtower.x + 10, LOC.watchtower.z + 6, 2, 8);
    // Scatter extra groups by region with a seeded RNG.
    const rng = new RNG(2024);
    const byRegion = { 0: ['gloop', 'gloop', 'wolf'], 1: ['wolf', 'gloop', 'bandit'], 2: ['frost_wolf', 'frost_gloop'], 3: ['magma_gloop', 'golem', 'bandit'] };
    let placed = 0;
    for (let i = 0; i < 400 && placed < 34; i++) {
      const x = rng.range(-850, 850), z = rng.range(-850, 850);
      const h = w.heightAt(x, z);
      if (h < 2 || w.slopeAt(x, z) > 0.4) continue;
      const reg = w.regionAt(x, z);
      const list = byRegion[reg];
      if (!list) continue;
      let near = false;
      for (const l of w.locations) if (Math.hypot(l.x - x, l.z - z) < l.r + 40) near = true;
      for (const zn of this.zones) if (Math.hypot(zn.x - x, zn.z - z) < 70) near = true;
      if (near) continue;
      Z(rng.pick(list), x, z, rng.int(2, 3), 12, { level: reg === 2 || reg === 3 ? 2 : 1 });
      placed++;
    }
    // Night wisps roam the crystal meadows.
    Z('wisp', -120, 60, 3, 20, { night: true }); Z('wisp', 140, -40, 3, 20, { night: true });
  }

  spawn(type, x, z, opts = {}) {
    const e = new Enemy(this.game, type, x, z, opts);
    this.game.combat.add(e);
    return e;
  }

  update(dt) {
    this.checkT -= dt;
    if (this.checkT > 0 || this.game.inDungeon) return;
    this.checkT = 0.5;
    const p = this.game.player.position;
    const hours = this.game.totalHours;
    const night = this.game.sky.isNight;
    for (const z of this.zones) {
      const d = Math.hypot(p.x - z.x, p.z - z.z);
      if (!z.active && d < NEAR) {
        if (hours - z.clearedAt < 20) continue;
        if (z.night && !night) continue;
        if (z.disabled) continue;
        z.active = true;
        z.alive = [];
        for (let i = 0; i < z.n; i++) {
          const a = Math.random() * Math.PI * 2, r = Math.random() * z.r;
          const e = this.spawn(z.type, z.x + Math.cos(a) * r, z.z + Math.sin(a) * r, { zone: z, level: z.level });
          z.alive.push(e);
        }
      } else if (z.active && (d > FAR || (z.night && !night))) {
        z.active = false;
        for (const e of z.alive) if (!e.removed && (!e.dead || e.deadT > 0.5)) {
          if (e.state === 'chase' && d < FAR + 40) continue;
          e.remove();
        }
        z.alive = [];
      } else if (z.active && z.alive.length && z.alive.every((e) => e.dead)) {
        // Fully cleared: allow respawn later.
        z.active = false;
        z.alive = [];
      }
    }
  }

  despawnAll() {
    for (const z of this.zones) { z.active = false; z.alive = []; }
    this.game.combat.clearAll();
  }

  serialize() {
    return this.zones.map((z) => z.clearedAt);
  }
  restore(arr) {
    if (!arr) return;
    arr.forEach((c, i) => { if (this.zones[i]) this.zones[i].clearedAt = c; });
  }
}
