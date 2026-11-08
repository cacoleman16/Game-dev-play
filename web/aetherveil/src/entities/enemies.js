// Enemy types and their AI: wander, notice, chase, telegraphed attacks
// (melee, lunges, ground slams, projectiles, volleys, breath, summons,
// blinks), stagger, leash back home, die and drop loot.

import * as THREE from 'three';
import { GloopModel, WolfModel, QuadrupedModel, SkyrayModel, WispModel, WraithModel, HumanoidModel } from './creatures.js';
import { clamp, damp, dampAngle, lerp } from '../core/util.js';

const v1 = new THREE.Vector3(), v2 = new THREE.Vector3();

const bandLook = (o = {}) => ({ hairStyle: 'short', hair: 0x2a1a1a, hat: 'hood', hatColor: 0x8a2a2a, top: 0x5a2a2a, trim: 0x2a1a1a, pants: 0x3a2a2a, scarf: null, pauldrons: false, ...o });

export const ENEMY_TYPES = {
  gloop: { name: 'Gloop', hp: 32, xp: 10, speed: 3.0, aggro: 12, radius: 0.55, loot: 'gloop', weak: 'fire',
    model: (q) => new GloopModel(0x6fd86a, 1, q.outlines),
    attacks: [{ kind: 'lunge', range: 2.4, windup: 0.55, strike: 0.35, recover: 0.6, dmg: 7, cd: 1.3, lunge: 7.5 }] },
  frost_gloop: { name: 'Frost Gloop', hp: 52, xp: 16, speed: 3.0, aggro: 12, radius: 0.6, loot: 'gloop', weak: 'fire', resist: 'frost',
    model: (q) => new GloopModel(0x8fd0ff, 1.15, q.outlines),
    attacks: [{ kind: 'lunge', range: 2.6, windup: 0.55, strike: 0.35, recover: 0.6, dmg: 12, cd: 1.3, lunge: 7.5, slow: 0.4 }] },
  magma_gloop: { name: 'Magma Gloop', hp: 60, xp: 20, speed: 3.2, aggro: 13, radius: 0.6, loot: 'gloop', weak: 'frost', resist: 'fire',
    model: (q) => new GloopModel(0xff7a3a, 1.15, q.outlines),
    attacks: [{ kind: 'lunge', range: 2.6, windup: 0.5, strike: 0.35, recover: 0.6, dmg: 14, cd: 1.2, lunge: 8, element: 'fire' }] },
  wolf: { name: 'Dusk Wolf', hp: 48, xp: 20, speed: 7.0, aggro: 22, radius: 0.5, loot: 'wolf',
    model: (q) => new WolfModel({ outline: q.outlines }),
    attacks: [
      { kind: 'melee', range: 2.0, windup: 0.35, strike: 0.15, recover: 0.45, dmg: 10, cd: 0.9, arc: 0.3 },
      { kind: 'lunge', range: 7, minRange: 3.5, windup: 0.45, strike: 0.45, recover: 0.7, dmg: 14, cd: 4, lunge: 13, leap: 4 },
    ] },
  frost_wolf: { name: 'Rime Wolf', hp: 64, xp: 28, speed: 7.2, aggro: 24, radius: 0.5, loot: 'wolf', weak: 'fire', resist: 'frost',
    model: (q) => new WolfModel({ fur: 0xd8e4f0, belly: 0xf4f8ff, dark: 0x8aa0c0, eye: 0x7fd8ff, outline: q.outlines }),
    attacks: [
      { kind: 'melee', range: 2.0, windup: 0.35, strike: 0.15, recover: 0.45, dmg: 13, cd: 0.9, arc: 0.3, slow: 0.3 },
      { kind: 'lunge', range: 7, minRange: 3.5, windup: 0.45, strike: 0.45, recover: 0.7, dmg: 16, cd: 4, lunge: 13, leap: 4 },
    ] },
  bandit: { name: 'Redcloak Bandit', hp: 70, xp: 32, speed: 5.2, aggro: 20, radius: 0.4, loot: 'bandit_drop', humanoid: true, blockChance: 0.25,
    model: (q) => new HumanoidModel(bandLook(), 'iron', q.npcOutlines),
    attacks: [
      { kind: 'melee', range: 2.4, windup: 0.45, strike: 0.2, recover: 0.5, dmg: 12, cd: 1.1, arc: 0.2, anim: 'attack1' },
      { kind: 'melee', range: 2.6, windup: 0.8, strike: 0.25, recover: 0.7, dmg: 20, cd: 3.5, arc: 0.0, anim: 'attack3', heavy: true },
    ] },
  bandit_mage: { name: 'Redcloak Pyromancer', hp: 48, xp: 36, speed: 4.6, aggro: 24, radius: 0.4, loot: 'bandit_drop', humanoid: true, keep: 9,
    model: (q) => new HumanoidModel(bandLook({ hatColor: 0x5a2a6a, top: 0x3a2a4a, skirt: true }), 'staff', q.npcOutlines),
    attacks: [{ kind: 'projectile', range: 20, windup: 0.65, strike: 0.2, recover: 0.6, dmg: 14, cd: 2.0, speed: 18, color: 0xff7a2a, element: 'fire', anim: 'cast' }] },
  golem: { name: 'Ruin Golem', hp: 160, xp: 60, speed: 2.5, aggro: 15, radius: 0.9, loot: 'golem', weak: 'storm', humanoid: true, heavyBody: true,
    model: (q) => new HumanoidModel({ kind: 'golem', height: 1.5, build: 1.15 }, null, q.npcOutlines),
    attacks: [
      { kind: 'melee', range: 2.8, windup: 0.6, strike: 0.2, recover: 0.6, dmg: 16, cd: 1.6, arc: 0.1, anim: 'attack1' },
      { kind: 'aoe', range: 3.5, windup: 1.0, strike: 0.25, recover: 0.9, dmg: 24, cd: 4, radius: 3.4, at: 'front', anim: 'attack3' },
    ] },
  revenant: { name: 'Ash Revenant', hp: 62, xp: 30, speed: 4.6, aggro: 18, radius: 0.4, loot: 'revenant', weak: 'storm', resist: 'fire', humanoid: true,
    model: (q) => new HumanoidModel({ kind: 'revenant' }, 'rusty', q.npcOutlines),
    attacks: [
      { kind: 'melee', range: 2.3, windup: 0.4, strike: 0.2, recover: 0.4, dmg: 12, cd: 1.0, arc: 0.2, anim: 'attack2' },
      { kind: 'melee', range: 2.5, windup: 0.75, strike: 0.25, recover: 0.6, dmg: 19, cd: 3, arc: 0.0, anim: 'attack3', heavy: true },
    ] },
  boar: { name: 'Thornback Boar', hp: 70, xp: 26, speed: 6.2, aggro: 18, radius: 0.7, loot: 'boar',
    model: (q) => new QuadrupedModel({ kind: 'boar', fur: 0x6a4a3a, belly: 0x9a7a62, dark: 0x3a2a22, eye: 0xff6a3a, scale: 1.05, outline: q.outlines }),
    attacks: [
      { kind: 'melee', range: 2.2, windup: 0.45, strike: 0.2, recover: 0.5, dmg: 12, cd: 1.2, arc: 0.3 },
      { kind: 'charge', range: 16, minRange: 5, windup: 0.9, strike: 0.9, recover: 0.8, dmg: 20, cd: 5, speed: 15, width: 1.6 },
    ] },
  archer: { name: 'Redcloak Archer', hp: 52, xp: 30, speed: 4.8, aggro: 26, radius: 0.4, loot: 'bandit_drop', humanoid: true, keep: 12,
    model: (q) => new HumanoidModel(bandLook({ hatColor: 0x6a3a2a, top: 0x4a3a2a, trim: 0x8a2a2a }), 'bow', q.npcOutlines),
    attacks: [{ kind: 'projectile', range: 26, minRange: 4, windup: 0.8, strike: 0.2, recover: 0.5, dmg: 13, cd: 1.8, speed: 30, color: 0xf0e0c0, size: 0.12, arrow: true, anim: 'aim' }] },
  skyray: { name: 'Gale Skyray', hp: 44, xp: 28, speed: 6.5, aggro: 24, radius: 0.9, loot: 'wisp', fly: true, keep: 7, weak: 'storm',
    model: (q) => new SkyrayModel({ outline: q.outlines }),
    attacks: [{ kind: 'lunge', range: 11, minRange: 3, windup: 0.8, strike: 0.6, recover: 0.8, dmg: 14, cd: 3.2, lunge: 16 }] },
  treant: { name: 'Bramble Sentinel', hp: 190, xp: 70, speed: 2.4, aggro: 16, radius: 0.9, loot: 'golem', weak: 'fire', humanoid: true, heavyBody: true,
    model: (q) => new HumanoidModel({ kind: 'treant', height: 1.65, build: 1.1 }, null, q.npcOutlines),
    attacks: [
      { kind: 'melee', range: 3.0, windup: 0.65, strike: 0.25, recover: 0.6, dmg: 17, cd: 1.6, arc: -0.1, anim: 'attack2' },
      { kind: 'aoe', range: 14, minRange: 4, windup: 1.1, strike: 0.3, recover: 0.7, dmg: 18, cd: 5, radius: 2.4, at: 'target', anim: 'castUp', slow: 0.7, color: 0x6a9a3a, thorns: true },
    ] },
  wisp: { name: 'Aether Wisp', hp: 26, xp: 18, speed: 4.2, aggro: 20, radius: 0.4, loot: 'wisp', fly: true, keep: 8, night: true,
    model: () => new WispModel(0x9ff0ff),
    attacks: [{ kind: 'projectile', range: 18, windup: 0.75, strike: 0.2, recover: 0.6, dmg: 10, cd: 2.2, speed: 14, color: 0x9ff0ff, element: 'aether' }] },

  // ---------------- bosses
  warden: { name: 'The Warden of Whisperwood', boss: true, hp: 650, xp: 400, speed: 2.9, aggro: 30, radius: 1.5, loot: 'boss_warden', weak: 'fire', humanoid: true, heavyBody: true,
    model: (q) => new HumanoidModel({ kind: 'golem', height: 2.6, build: 1.2, glow: 0x7dff9a, moss: 0x4f8a3a, stone: 0x9a9484 }, null, true),
    attacks: [
      { kind: 'melee', range: 4.0, windup: 0.7, strike: 0.25, recover: 0.6, dmg: 22, cd: 2, arc: -0.3, anim: 'attack1' },
      { kind: 'aoe', range: 6, windup: 1.1, strike: 0.3, recover: 1.0, dmg: 32, cd: 5, radius: 4.6, at: 'front', anim: 'attack3' },
      { kind: 'projectile', range: 26, minRange: 8, windup: 0.9, strike: 0.25, recover: 0.7, dmg: 22, cd: 4, speed: 20, color: 0x8a8478, size: 0.7, anim: 'castUp' },
    ] },
  rimefang: { name: 'Rimefang, Alpha of the Peaks', boss: true, hp: 760, xp: 450, speed: 8.2, aggro: 32, radius: 1.6, loot: 'boss_rimefang', weak: 'fire', resist: 'frost',
    model: (q) => new WolfModel({ fur: 0xc8d6e8, belly: 0xe4ecf6, dark: 0x6a84b0, eye: 0x5ad8ff, spikes: 0x9fdcff, scale: 3.1, outline: true }),
    attacks: [
      { kind: 'melee', range: 3.6, windup: 0.4, strike: 0.15, recover: 0.5, dmg: 18, cd: 1.2, arc: 0.2 },
      { kind: 'lunge', range: 12, minRange: 5, windup: 0.55, strike: 0.5, recover: 0.8, dmg: 24, cd: 4, lunge: 18, leap: 5 },
      { kind: 'breath', range: 8, windup: 0.6, strike: 1.6, recover: 0.6, dmg: 5, cd: 7, slow: 0.6, color: 0xbfe8ff, element: 'frost' },
      { kind: 'summon', range: 40, windup: 1.0, strike: 0.3, recover: 0.8, cd: 18, summon: 'frost_wolf', count: 2, anim: 'howl', hpBelow: 0.6 },
    ] },
  colossus: { name: 'The Cinder Colossus', boss: true, hp: 950, xp: 550, speed: 2.6, aggro: 30, radius: 1.6, loot: 'boss_colossus', weak: 'frost', resist: 'fire', humanoid: true, heavyBody: true,
    model: (q) => new HumanoidModel({ kind: 'golem', height: 2.8, build: 1.25, glow: 0xff6a1a, moss: 0x3a2a2a, stone: 0x5a4a44, stoneDark: 0x3a3030 }, null, true),
    attacks: [
      { kind: 'aoe', range: 6, windup: 1.0, strike: 0.3, recover: 0.9, dmg: 34, cd: 4, radius: 4.8, at: 'front', anim: 'attack3', element: 'fire' },
      { kind: 'aoe', range: 7, windup: 1.4, strike: 0.3, recover: 1.0, dmg: 28, cd: 8, radius: 7.5, at: 'self', anim: 'castUp', element: 'fire' },
      { kind: 'volley', range: 24, minRange: 6, windup: 0.8, strike: 0.3, recover: 0.8, dmg: 14, cd: 5, speed: 16, count: 5, color: 0xff7a2a, element: 'fire', anim: 'cast' },
      { kind: 'melee', range: 4.2, windup: 0.6, strike: 0.25, recover: 0.6, dmg: 22, cd: 2, arc: -0.3, anim: 'attack1' },
    ] },
  herald: { name: 'The Hollow Herald', boss: true, hp: 1500, xp: 1000, speed: 4, aggro: 60, radius: 1.2, fly: true, keep: 10, loot: null,
    model: () => new WraithModel(),
    attacks: [
      { kind: 'volley', range: 30, windup: 0.8, strike: 0.3, recover: 0.6, dmg: 14, cd: 3, speed: 17, count: 7, color: 0xff7af0, element: 'aether' },
      { kind: 'aoe', range: 9, windup: 1.2, strike: 0.3, recover: 0.8, dmg: 30, cd: 6, radius: 7, at: 'self' },
      { kind: 'aoe', range: 30, windup: 1.0, strike: 0.3, recover: 0.6, dmg: 26, cd: 5, radius: 3.5, at: 'target', color: 0xff7af0 },
      { kind: 'blink', range: 40, windup: 0.4, strike: 0.1, recover: 0.4, cd: 7 },
      { kind: 'summon', range: 40, windup: 1.0, strike: 0.3, recover: 0.6, cd: 20, summon: 'wisp', count: 3, hpBelow: 0.7 },
    ] },
};

let nextId = 1;

export class Enemy {
  constructor(game, type, x, z, opts = {}) {
    this.game = game;
    this.id = nextId++;
    this.type = type;
    this.def = ENEMY_TYPES[type];
    const lvlScale = opts.level ? 1 + (opts.level - 1) * 0.15 : 1;
    this.maxHp = Math.round(this.def.hp * lvlScale);
    this.hp = this.maxHp;
    this.level = opts.level ?? 1;
    this.position = new THREE.Vector3(x, 0, z);
    const g = game.env.ground(x, z, 999);
    this.position.y = g.y;
    this.home = this.position.clone();
    this.velocity = new THREE.Vector3();
    this.yaw = Math.random() * Math.PI * 2;
    this.state = 'idle';
    this.stateT = 0;
    this.wanderT = Math.random() * 3;
    this.wanderTarget = null;
    this.cooldowns = this.def.attacks.map(() => 1 + Math.random());
    this.attack = null;
    this.dead = false;
    this.deadT = 0;
    this.slow = 0;
    this.radius = this.def.radius;
    this.boss = !!this.def.boss;
    this.zone = opts.zone || null;
    this.lastHit = -10;
    this.model = this.def.model(game.quality);
    game.scene.add(this.model.group);
    this.poise = 0;
    this.poiseMax = Math.round(this.maxHp * (this.boss ? 0.42 : 0.65));
    this.poiseT = 0;
    this.flashT = 0;
    this.flashMats = [];
    this.model.group.traverse((o) => { if (o.material && o.material.isMeshToonMaterial && !o.material.map && !this.flashMats.includes(o.material)) this.flashMats.push(o.material); });
    this.sync();
  }

  get name() { return this.def.name; }

  dist2Player() {
    const p = this.game.player.position;
    return Math.hypot(p.x - this.position.x, p.z - this.position.z);
  }

  takeDamage(amount, source, opts = {}) {
    if (this.dead) return 0;
    let mult = 1, weak = false, resist = false;
    if (opts.element && opts.element === this.def.weak) { mult = 1.75; weak = true; }
    if (opts.element && opts.element === this.def.resist) { mult = 0.5; resist = true; }
    // Humanoids sometimes raise a guard against frontal melee.
    if (this.def.blockChance && opts.melee && this.state !== 'attack' && Math.random() < this.def.blockChance) {
      const facing = this.facingDot(source.position);
      if (facing > 0.3) {
        mult *= 0.2;
        this.state = 'block'; this.stateT = 0.5;
        this.game.fx.sparks(v1.copy(this.position).setY(this.position.y + 1.3), 0xffe8a0, 10);
        this.game.audio?.play('block');
      }
    }
    const crit = opts.crit || (opts.melee && Math.random() < 0.1);
    if (crit) mult *= opts.crit ? 2 : 1.5;
    if (this.state === 'broken') mult *= 1.5;
    const dmg = Math.max(1, Math.round(amount * mult * (0.9 + Math.random() * 0.2)));
    this.hp -= dmg;
    this.lastHit = this.game.time;
    this.flashT = 0.13;
    // Break gauge: fills faster from heavy hits and weaknesses.
    if (this.state !== 'broken') {
      this.poise += dmg * (opts.heavy ? 1.6 : 1) * (weak ? 1.6 : 1) * (opts.breakMult ?? 1);
      this.poiseT = 2.5;
      if (this.poise >= this.poiseMax && this.hp > 0) this.breakGuard();
    }
    this.game.events.emit('damage', { pos: v2.copy(this.position).setY(this.position.y + (this.boss ? 3.2 : 1.8)).clone(), amount: dmg, weak, resist, crit, enemy: this });
    if (opts.slow) this.slow = Math.max(this.slow, opts.slow);
    if (this.state === 'idle' || this.state === 'return') this.state = 'chase';
    if (this.hp <= 0) { this.die(source); return dmg; }
    if (Math.random() < 0.3) this.game.audio?.creature(this, 'hurt');
    // Knockback and stagger for small foes.
    if (source && !this.boss && !this.def.heavyBody) {
      const dx = this.position.x - source.position.x, dz = this.position.z - source.position.z;
      const d = Math.hypot(dx, dz) || 1;
      const kb = opts.knockback ?? 3;
      this.velocity.x += (dx / d) * kb; this.velocity.z += (dz / d) * kb;
      if (opts.heavy || dmg > this.maxHp * 0.25) { this.cancelAttack(); this.state = 'stagger'; this.stateT = 0.45; }
    }
    return dmg;
  }

  breakGuard() {
    this.cancelAttack();
    this.state = 'broken';
    this.stateT = this.boss ? 5 : 3.5;
    this.poise = this.poiseMax;
    this.game.events.emit('break', this);
    this.game.audio?.play('break');
    this.game.audio?.creature(this, 'hurt');
    this.game.fx.burst(this.position.clone().setY(this.position.y + (this.boss ? 2.5 : 1.2)), 0xffe27a, 30, { speed: 6 });
  }

  parried() {
    this.cancelAttack();
    this.state = 'stagger';
    this.stateT = this.boss ? 1.2 : 1.6;
    this.poise = Math.min(this.poiseMax * 0.99, this.poise + this.poiseMax * 0.35);
    this.poiseT = 3;
    this.game.audio?.creature(this, 'hurt');
  }

  facingDot(p) {
    const dx = p.x - this.position.x, dz = p.z - this.position.z;
    const d = Math.hypot(dx, dz) || 1;
    return (dx * Math.sin(this.yaw) + dz * Math.cos(this.yaw)) / d;
  }

  cancelAttack() {
    if (this.attack && this.attack.tele) this.attack.tele.cancel = true;
    this.attack = null;
  }

  die(source) {
    this.dead = true;
    this.deadT = 0;
    this.cancelAttack();
    this.game.events.emit('enemy:killed', this, source);
  }

  remove() {
    this.game.scene.remove(this.model.group);
    this.model.group.traverse((o) => { if (o.isMesh && o.geometry && !o.geometry.userData.shared) o.geometry.dispose(); });
    this.removed = true;
  }

  // ---------------------------------------------------------------- AI
  update(dt) {
    const game = this.game, player = game.player, def = this.def;
    this.stateT -= dt;
    this.slow = Math.max(0, this.slow - dt * 0.3);
    this.flashT = Math.max(0, this.flashT - dt);
    this.poiseT -= dt;
    if (this.poiseT <= 0 && this.state !== 'broken') this.poise = Math.max(0, this.poise - this.poiseMax * 0.1 * dt);
    for (let i = 0; i < this.cooldowns.length; i++) this.cooldowns[i] -= dt;
    if (this.dead) {
      this.deadT += dt;
      this.velocity.multiplyScalar(Math.exp(-4 * dt));
      this.physics(dt, null);
      if (this.deadT > (this.boss ? 6 : 3.5)) this.remove();
      this.sync(dt);
      return;
    }
    if (def.night && !game.sky.isNight && this.state === 'idle') { this.hp = 0; this.dead = true; this.deadT = 2.5; game.fx.burst(v1.copy(this.position).setY(this.position.y + 1.5), 0x9ff0ff, 20); }

    const d = this.dist2Player();
    const alive = !player.dead && !game.inDialogue;
    let desired = null; // world-space direction to move in
    let speed = def.speed * (1 - this.slow * 0.5);
    const leash = this.boss ? 60 : 38;

    switch (this.state) {
      case 'idle': {
        this.wanderT -= dt;
        if (this.wanderT <= 0) {
          this.wanderT = 3 + Math.random() * 5;
          const a = Math.random() * Math.PI * 2, r = Math.random() * 10;
          this.wanderTarget = v1.set(this.home.x + Math.cos(a) * r, 0, this.home.z + Math.sin(a) * r).clone();
          if (Math.random() < 0.4) this.wanderTarget = null;
        }
        if (this.wanderTarget) {
          const dx = this.wanderTarget.x - this.position.x, dz = this.wanderTarget.z - this.position.z;
          if (Math.hypot(dx, dz) > 0.8) { desired = v2.set(dx, 0, dz).normalize(); speed *= 0.35; }
          else this.wanderTarget = null;
        }
        if (alive && d < def.aggro * (player.crouching ? 0.55 : 1) && Math.abs(player.position.y - this.position.y) < 12) {
          this.state = 'chase';
          game.events.emit('enemy:aggro', this);
        }
        break;
      }
      case 'chase': {
        if (!alive) { this.state = 'return'; break; }
        if (this.position.distanceTo(this.home) > leash && d > 8) { this.state = 'return'; break; }
        // Try an attack (non-bosses take turns: at most two attack at once).
        const dx = player.position.x - this.position.x, dz = player.position.z - this.position.z;
        const busy = !this.boss && game.combat.attackers(this) >= 2;
        const choice = busy ? -1 : this.pickAttack(d);
        if (choice >= 0) { this.beginAttack(choice); break; }
        if (busy && d < 7 && !def.keep) {
          // Circle at a respectful distance while waiting for an opening.
          const side = (this.id % 2 ? 1 : -1);
          const tx = -dz * side, tz = dx * side;
          const outward = d < 4.5 ? -1 : d > 6 ? 0.6 : 0;
          desired = v2.set(tx + dx * outward * 0.5, 0, tz + dz * outward * 0.5).normalize();
          speed *= 0.45;
          this.yaw = dampAngle(this.yaw, Math.atan2(dx, dz), 8, dt);
          break;
        }
        const keep = def.keep || 0;
        if (keep && d < keep * 0.7) desired = v2.set(-dx, 0, -dz).normalize();
        else if (d > Math.max(keep, def.radius + 1.4)) desired = this.toward(player.position.x, player.position.z);
        else desired = null;
        if (keep && d < keep * 1.2 && d > keep * 0.7) {
          // Circle-strafe at range.
          desired = v2.set(-dz, 0, dx).normalize().multiplyScalar(0.5);
        }
        this.yaw = dampAngle(this.yaw, Math.atan2(dx, dz), 8, dt);
        break;
      }
      case 'attack': this.updateAttack(dt, d); break;
      case 'stagger': case 'block':
        if (this.stateT <= 0) this.state = 'chase';
        break;
      case 'broken':
        if (this.stateT <= 0) { this.state = 'chase'; this.poise = 0; }
        break;
      case 'return': {
        const dx = this.home.x - this.position.x, dz = this.home.z - this.position.z;
        const dd = Math.hypot(dx, dz);
        this.hp = Math.min(this.maxHp, this.hp + this.maxHp * 0.3 * dt);
        if (dd < 2) { this.state = 'idle'; break; }
        desired = this.toward(this.home.x, this.home.z);
        if (alive && d < def.aggro * 0.6) this.state = 'chase';
        break;
      }
    }

    if (desired && this.state !== 'attack') {
      this.yaw = dampAngle(this.yaw, Math.atan2(desired.x, desired.z), 7, dt);
    }
    this.physics(dt, desired ? desired.multiplyScalar(speed) : null);
    this.sync(dt);
  }

  pickAttack(d) {
    const atks = this.def.attacks;
    const options = [];
    for (let i = 0; i < atks.length; i++) {
      const a = atks[i];
      if (this.cooldowns[i] > 0) continue;
      if (d > a.range || d < (a.minRange || 0)) continue;
      if (a.hpBelow && this.hp / this.maxHp > a.hpBelow) continue;
      options.push(i);
    }
    if (!options.length) return -1;
    return options[Math.floor(Math.random() * options.length)];
  }

  beginAttack(i) {
    const a = this.def.attacks[i];
    this.state = 'attack';
    this.attack = { def: a, idx: i, phase: 'windup', t: 0, done: false, ticks: 0 };
    this.cooldowns[i] = a.cd;
    const p = this.game.player.position;
    this.attack.targetPos = p.clone();
    if (a.kind === 'aoe') {
      const at = a.at === 'self' ? this.position.clone()
        : a.at === 'target' ? p.clone()
          : this.position.clone().add(v1.set(Math.sin(this.yaw), 0, Math.cos(this.yaw)).multiplyScalar(a.radius * 0.6));
      at.y = this.game.env.ground(at.x, at.z, at.y + 3).y;
      this.attack.at = at;
      this.attack.tele = this.game.fx.telegraph(at, a.radius, a.windup);
    }
    if (a.kind === 'charge') {
      const yaw = Math.atan2(p.x - this.position.x, p.z - this.position.z);
      this.attack.dir = new THREE.Vector3(Math.sin(yaw), 0, Math.cos(yaw));
      this.yaw = yaw;
      this.attack.tele = this.game.fx.telegraphLine(this.position, yaw, a.speed * a.strike, a.width, a.windup);
    }
    this.game.audio?.creature(this, a.kind === 'summon' || a.anim === 'howl' ? 'howl' : 'windup');
    if (a.kind === 'summon' || a.anim === 'howl') this.game.audio?.play('howl');
    else if (this.boss) this.game.audio?.play('bossWindup');
  }

  updateAttack(dt, d) {
    const at = this.attack, a = at.def, game = this.game, player = game.player;
    at.t += dt;
    const p = player.position;
    const dx = p.x - this.position.x, dz = p.z - this.position.z;
    if (at.phase === 'windup') {
      if (a.kind !== 'lunge' && a.kind !== 'charge' || (a.kind === 'lunge' && at.t < a.windup * 0.8)) this.yaw = dampAngle(this.yaw, Math.atan2(dx, dz), 10, dt);
      if (a.kind === 'charge' && Math.random() < dt * 20) game.fx.dust(this.position.clone().add(at.dir.clone().multiplyScalar(-0.6)), 2);
      if (at.t >= a.windup) { at.phase = 'strike'; at.t = 0; this.strike(at, d); }
    } else if (at.phase === 'strike') {
      if (a.kind === 'lunge' && !at.done && d < this.radius + 1.0 && Math.abs(p.y - this.position.y) < 2.5) {
        at.done = true;
        player.takeDamage(a.dmg, this, { slow: a.slow, element: a.element, knockback: 6 });
      }
      if (a.kind === 'charge') {
        this.velocity.x = at.dir.x * a.speed; this.velocity.z = at.dir.z * a.speed;
        if (Math.random() < dt * 30) game.fx.dust(this.position, 2);
        if (!at.done && d < this.radius + 0.9 && Math.abs(p.y - this.position.y) < 2) {
          at.done = true;
          player.takeDamage(a.dmg, this, { knockback: 12, heavy: true });
        }
        if (at.bonked) { at.phase = 'recover'; at.t = 0; this.state = 'stagger'; this.stateT = 1.4; this.attack = null; game.fx.sparks(this.position.clone().setY(this.position.y + 0.8), 0xffe8a0, 16); game.audio?.play('slam'); return; }
      }
      if (a.kind === 'breath') {
        at.ticks += dt;
        const fwdx = Math.sin(this.yaw), fwdz = Math.cos(this.yaw);
        const mouth = v1.copy(this.position).add(v2.set(fwdx * this.radius * 1.4, 1.2 * (this.boss ? 2 : 1), fwdz * this.radius * 1.4));
        for (let i = 0; i < 4; i++) {
          const c = new THREE.Color(a.color);
          game.fx.glow.spawn(mouth.x, mouth.y, mouth.z, fwdx * 12 + (Math.random() - 0.5) * 4, (Math.random() - 0.5) * 2 - 1, fwdz * 12 + (Math.random() - 0.5) * 4, c.r, c.g, c.b, 0.9, 0.55, 1.5, 0, 2);
        }
        this.yaw = dampAngle(this.yaw, Math.atan2(dx, dz), 1.5, dt);
        if (at.ticks > 0.2) {
          at.ticks = 0;
          const dot = (dx * fwdx + dz * fwdz) / (Math.hypot(dx, dz) || 1);
          if (d < a.range && dot > 0.75) player.takeDamage(a.dmg, this, { slow: a.slow, element: a.element, knockback: 0.5, unblockable: false });
        }
      }
      if (at.t >= a.strike) { at.phase = 'recover'; at.t = 0; }
    } else if (at.phase === 'recover') {
      if (at.t >= a.recover) { this.attack = null; this.state = 'chase'; }
    }
  }

  strike(at, d) {
    const a = at.def, game = this.game, player = game.player;
    const p = player.position;
    const dx = p.x - this.position.x, dz = p.z - this.position.z;
    const fwdx = Math.sin(this.yaw), fwdz = Math.cos(this.yaw);
    const hpos = v1.copy(this.position).setY(this.position.y + (this.boss ? 2.4 : 1.2));
    switch (a.kind) {
      case 'melee': {
        const dot = (dx * fwdx + dz * fwdz) / (Math.hypot(dx, dz) || 1);
        if (d < a.range + 0.4 && dot > (a.arc ?? 0.2) && Math.abs(p.y - this.position.y) < 2.5) {
          player.takeDamage(a.dmg, this, { slow: a.slow, element: a.element, knockback: a.heavy ? 8 : 4 });
        }
        game.audio?.play(this.def.humanoid ? 'swing' : 'bite', { pitch: this.boss ? 0.6 : 1 });
        break;
      }
      case 'lunge': {
        const dd = Math.hypot(dx, dz) || 1;
        this.velocity.x = (dx / dd) * a.lunge; this.velocity.z = (dz / dd) * a.lunge;
        if (a.leap) this.velocity.y = a.leap;
        game.audio?.play('bite', { pitch: this.boss ? 0.7 : 1.1 });
        break;
      }
      case 'aoe': {
        const c = at.at;
        const r = Math.hypot(p.x - c.x, p.z - c.z);
        if (r < a.radius && Math.abs(p.y - c.y) < 3) player.takeDamage(a.dmg, this, { element: a.element, knockback: 10, unblockable: a.at === 'self' });
        if (a.thorns) game.fx.thorns(c, a.radius);
        if (a.slow && r < a.radius) player.slow = Math.max(player.slow, a.slow);
        game.fx.burst(c.clone().setY(c.y + 0.3), a.element === 'fire' ? 0xff7a2a : a.color ?? 0xd8c8a8, 50, { speed: a.radius * 2.2, size: 0.6, life: 0.7, up: 2 });
        game.fx.dust(c, 12);
        game.cameraRig?.shake(this.boss ? 0.7 : 0.4);
        game.audio?.play('slam');
        break;
      }
      case 'projectile': {
        // Lead the target a little so strafing in a straight line isn't free.
        const lead = player.velocity.clone().multiplyScalar(Math.min(0.6, d / a.speed) * 0.6);
        game.combat.spawnProjectile({ from: hpos.clone(), target: p.clone().add(lead).setY(p.y + 1.1), speed: a.speed, dmg: a.dmg, team: 'enemy', owner: this, color: a.color, element: a.element, size: a.size ?? 0.35, arrow: a.arrow });
        break;
      }
      case 'charge': {
        game.audio?.creature(this, 'roar');
        break;
      }
      case 'volley': {
        for (let i = 0; i < a.count; i++) {
          const spread = (i - (a.count - 1) / 2) * 0.22;
          const ang = Math.atan2(dx, dz) + spread;
          const tgt = hpos.clone().add(v2.set(Math.sin(ang) * d, p.y + 1 - hpos.y, Math.cos(ang) * d));
          game.combat.spawnProjectile({ from: hpos.clone(), target: tgt, speed: a.speed, dmg: a.dmg, team: 'enemy', owner: this, color: a.color, element: a.element, size: 0.35 });
        }
        break;
      }
      case 'summon': {
        for (let i = 0; i < a.count; i++) {
          const ang = Math.random() * Math.PI * 2;
          const e = game.spawner.spawn(a.summon, this.position.x + Math.cos(ang) * 6, this.position.z + Math.sin(ang) * 6, { level: 2 });
          e.state = 'chase';
          game.fx.burst(e.position.clone().setY(e.position.y + 1), 0xb48cff, 25, { speed: 4 });
        }
        break;
      }
      case 'blink': {
        game.fx.burst(hpos.clone(), 0xff7af0, 40, { speed: 6 });
        const ang = Math.random() * Math.PI * 2;
        const nx = p.x + Math.cos(ang) * 12, nz = p.z + Math.sin(ang) * 12;
        this.position.set(nx, game.env.ground(nx, nz, 999).y, nz);
        game.fx.burst(this.position.clone().setY(this.position.y + 2), 0xff7af0, 40, { speed: 6 });
        game.audio?.play('blink');
        break;
      }
    }
  }

  // Unit direction toward a goal; inside the vault this follows the tile
  // grid around walls rather than walking into them.
  toward(gx, gz) {
    const env = this.game.env, p = this.position;
    if (env.steerPoint && !this.def.fly) { const w = env.steerPoint(p.x, p.z, gx, gz, this.radius + 0.15); gx = w.x; gz = w.z; }
    v2.set(gx - p.x, 0, gz - p.z);
    return v2.lengthSq() > 1e-6 ? v2.normalize() : v2;
  }

  physics(dt, desired) {
    const env = this.game.env, p = this.position, v = this.velocity;
    const def = this.def;
    if (desired) {
      v.x = damp(v.x, desired.x, 8, dt);
      v.z = damp(v.z, desired.z, 8, dt);
    } else if (!(this.attack && (this.attack.def.kind === 'lunge' || this.attack.def.kind === 'charge') && this.attack.phase === 'strike')) {
      v.x = damp(v.x, 0, 6, dt);
      v.z = damp(v.z, 0, 6, dt);
    }
    // Keep out of deep water.
    const nx = p.x + v.x * dt * 6, nz = p.z + v.z * dt * 6;
    if (!def.fly && env.waterDepth(nx, nz) > 0.9) { v.x = 0; v.z = 0; }
    p.x += v.x * dt; p.z += v.z * dt;
    const g = env.ground(p.x, p.z, p.y + 0.5);
    if (def.fly) {
      p.y = damp(p.y, Math.max(g.y, env.waterLevel(p.x, p.z)) + (this.type === 'herald' ? 1.5 : 0.3), 3, dt);
    } else {
      v.y -= 22 * dt;
      p.y += v.y * dt;
      if (p.y <= g.y) { p.y = g.y; v.y = 0; }
    }
    if (!this.dead) {
      const bx = p.x, bz = p.z;
      const hit = env.resolve(p, this.radius, this.boss ? 4 : 1.8);
      if (hit && this.attack && this.attack.def.kind === 'charge' && this.attack.phase === 'strike' && Math.hypot(p.x - bx, p.z - bz) > 0.05) this.attack.bonked = true;
    }
    // Separate from other enemies.
    for (const o of this.game.combat.enemies) {
      if (o === this || o.dead) continue;
      const dx = p.x - o.position.x, dz = p.z - o.position.z;
      const min = this.radius + o.radius;
      const dd = dx * dx + dz * dz;
      if (dd < min * min && dd > 1e-6) {
        const d = Math.sqrt(dd), push = (min - d) * 0.5;
        p.x += (dx / d) * push; p.z += (dz / d) * push;
      }
    }
  }

  sync(dt = 0) {
    const m = this.model;
    m.group.position.copy(this.position);
    m.group.rotation.y = this.yaw;
    const at = this.attack;
    let action = null, k = 0;
    if (at) {
      action = at.phase;
      const dur = at.phase === 'windup' ? at.def.windup : at.phase === 'strike' ? at.def.strike : at.def.recover;
      k = clamp(at.t / dur, 0, 1);
      if (at.def.anim === 'howl' && at.phase !== 'recover') action = 'howl';
      if (at.def.kind === 'breath' && at.phase === 'strike') action = 'breath';
    }
    if (this.state === 'stagger') { action = 'stagger'; k = clamp(1 - this.stateT / 0.45, 0, 1); }
    if (this.state === 'broken') { action = 'stagger'; k = 0.5 + Math.sin(this.game.time * 6) * 0.2; }
    if (this.flashMats.length) {
      const f = this.flashT > 0 ? this.flashT / 0.13 : 0;
      for (const m of this.flashMats) m.emissive.setScalar(f * 0.9 + (this.state === 'broken' ? 0.12 + Math.sin(this.game.time * 10) * 0.08 : 0));
    }
    if (this.state === 'block') action = 'block';
    if (dt > 0) m.animate(dt, { speed: Math.hypot(this.velocity.x, this.velocity.z), action, actionK: k, attackAnim: at?.def.anim, dead: this.dead, deadT: this.deadT, alert: this.state === 'chase' || this.state === 'attack' });
  }
}

export { lerp };
