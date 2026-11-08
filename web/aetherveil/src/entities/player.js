// The player: movement physics (run, sprint, jump, glide, swim, slopes),
// combat actions (combo attacks, block, dodge roll, spells), and the RPG
// numbers (health/mana/stamina, level, skills that improve with use).

import * as THREE from 'three';
import { Character, makeSword } from './character.js';
import { clamp, damp, dampAngle, lerp } from '../core/util.js';
import { ITEMS, SPELLS } from '../systems/content.js';
import { makeGlowMaterial } from '../world/vegetation.js';
import { SwordTrail } from '../fx/trail.js';

const GRAV = 22;
const tmpV = new THREE.Vector3(), fwd = new THREE.Vector3(), nrm = new THREE.Vector3();

export const SPELL_SLOTS = ['firebolt', 'heal', 'frostlance', 'skybreaker'];

export const SKILLS = {
  blade: { name: 'Blade', desc: '+3% melee damage per level' },
  sorcery: { name: 'Sorcery', desc: '+4% spell power, -1% spell cost per level' },
  guard: { name: 'Guard', desc: '+2% block strength per level' },
  athletics: { name: 'Athletics', desc: '+1% speed, +3% stamina regen per level' },
};

export class Player {
  constructor(game) {
    this.game = game;
    this.position = new THREE.Vector3(0, 0, 0);
    this.velocity = new THREE.Vector3();
    this.yaw = Math.PI;
    this.grounded = false;
    this.swimming = false;
    this.gliding = false;
    this.sprinting = false;
    this.crouching = false;
    this.speed = 0;
    this.radius = 0.38;
    this.height = 1.8;
    this.action = null;
    this.comboQueued = false;
    this.lockTarget = null;
    this.softTarget = null;
    this.spellCd = {};
    this.selectedSpell = 'firebolt';
    this.invuln = 0;
    this.dead = false;
    this.deadT = 0;
    this.staminaDelay = 0;
    this.combatTimer = 0;
    this.lastStep = 0;
    this.fallStart = 0;
    this.slow = 0;
    this.render = { bob: 0 };
    this.name = 'Wayfarer';
    this.look = {};
    this.stats = {
      level: 1, xp: 0, perks: 0,
      baseHp: 100, baseMp: 60, baseSp: 100,
      hp: 100, mp: 60, sp: 100,
      gold: 0,
      skills: { blade: { lvl: 1, xp: 0 }, sorcery: { lvl: 1, xp: 0 }, guard: { lvl: 1, xp: 0 }, athletics: { lvl: 1, xp: 0 } },
      spells: ['firebolt', 'heal'],
      kills: 0,
    };
    this.character = null;
    this.wings = null;
    this.surge = 0;
    this.blockT = -10;
    this.counterT = 0;
    this.comboCount = 0;
    this.comboT = 0;
    this.trail = null;
  }

  addSurge(n) {
    const was = this.surge >= 100;
    this.surge = Math.min(100, this.surge + n);
    if (!was && this.surge >= 100) this.game.events.emit('surge:ready');
  }

  registerHits(n, dmg) {
    if (n <= 0) return;
    this.comboCount = this.comboT > 0 ? this.comboCount + n : n;
    this.comboT = 2.2;
    this.addSurge(dmg * 0.22 * n);
    this.game.events.emit('combo', this.comboCount);
  }

  // ---------------- derived stats
  get maxHp() { return this.stats.baseHp + this.game.inventory.bonus('hp'); }
  get maxMp() { return this.stats.baseMp + this.game.inventory.bonus('mp'); }
  get maxSp() { return this.stats.baseSp + this.game.inventory.bonus('sp'); }
  get armor() { return this.game.inventory.bonus('armor'); }
  xpForLevel(l) { return Math.round(80 * Math.pow(l, 1.45)); }
  skill(k) { return this.stats.skills[k].lvl; }

  buildCharacter(look) {
    if (look) this.look = { ...look };
    const armor = this.game.inventory?.equipped.armor;
    const armorLook = armor ? ITEMS[armor].look : {};
    const full = { ...this.look, ...armorLook, scarf: this.look.scarf ?? 0xc8453a, cape: armor === 'aether_plate' ? 0x9f7fff : (this.look.cape ?? null) };
    const old = this.character;
    this.character = new Character(full, { outline: this.game.quality.outlines });
    if (old) {
      this.character.weaponDrawn = old.weaponDrawn;
      this.game.scene.remove(old.group);
      old.dispose();
    }
    this.game.scene.add(this.character.group);
    this.updateWeaponModel();
    // Aether wings for gliding.
    const wingGeo = new THREE.BufferGeometry();
    const v = [];
    for (const s of [1, -1]) {
      v.push(0, 0, 0, s * 1.6, 0.35, -0.2, s * 1.2, -0.25, -0.15);
      v.push(0, 0, 0, s * 1.2, -0.25, -0.15, s * 0.6, -0.6, -0.1);
    }
    wingGeo.setAttribute('position', new THREE.Float32BufferAttribute(v, 3));
    const col = new Float32Array(v.length).fill(1);
    for (let i = 0; i < col.length; i += 3) { col[i] = 0.55; col[i + 1] = 0.95; col[i + 2] = 1.0; }
    wingGeo.setAttribute('color', new THREE.BufferAttribute(col, 3));
    wingGeo.computeVertexNormals();
    const wm = new THREE.MeshBasicMaterial({ color: 0x9ff0ff, transparent: true, opacity: 0.55, side: THREE.DoubleSide, blending: THREE.AdditiveBlending, depthWrite: false });
    this.wings = new THREE.Mesh(wingGeo, wm);
    this.wings.position.set(0, 0.15, -0.18);
    this.wings.scale.setScalar(0.01);
    this.character.bones.chest.add(this.wings);
  }

  updateWeaponModel() {
    const w = this.game.inventory?.equipped.weapon;
    if (!this.character) return;
    this.character.setWeapon(w ? makeSword(ITEMS[w].model) : null);
  }

  setFirstPerson(fp) {
    if (this.character) this.character.group.visible = !fp || this.dead;
  }

  teleport(x, y, z) {
    this.position.set(x, y, z);
    this.velocity.set(0, 0, 0);
    this.gliding = false;
    this.fallStart = y;
  }

  // ---------------- experience & skills
  gainXp(n) {
    const st = this.stats;
    st.xp += n;
    this.game.events.emit('xp', n);
    while (st.xp >= this.xpForLevel(st.level)) {
      st.xp -= this.xpForLevel(st.level);
      st.level++;
      st.perks++;
      st.hp = this.maxHp; st.mp = this.maxMp; st.sp = this.maxSp;
      this.game.events.emit('levelup', st.level);
    }
  }

  trainSkill(k, amount) {
    const s = this.stats.skills[k];
    if (!s || s.lvl >= 50) return;
    s.xp += amount;
    const need = 12 + s.lvl * 8;
    if (s.xp >= need) {
      s.xp -= need;
      s.lvl++;
      this.game.events.emit('skillup', k, s.lvl);
      this.gainXp(10 + s.lvl * 2);
    }
  }

  spendPerk(attr) {
    const st = this.stats;
    if (st.perks <= 0) return false;
    st.perks--;
    if (attr === 'hp') { st.baseHp += 15; st.hp += 15; }
    if (attr === 'mp') { st.baseMp += 12; st.mp += 12; }
    if (attr === 'sp') { st.baseSp += 12; st.sp += 12; }
    return true;
  }

  // ---------------- combat
  meleeDamage() {
    const w = this.game.inventory.equipped.weapon;
    const base = w ? ITEMS[w].dmg : 4;
    return base * (1 + (this.skill('blade') - 1) * 0.03);
  }

  startAction(name, dur, extra = {}) {
    this.action = { name, t: 0, dur, hit: false, ...extra };
  }

  tryAttack() {
    const st = this.stats;
    if (this.swimming) return;
    // Plunge from the air.
    if (!this.grounded && !this.action) {
      const g = this.game.env.ground(this.position.x, this.position.z, this.position.y);
      if (this.position.y - g.y > 2.2) {
        this.gliding = false;
        this.character.drawWeapon(true);
        this.startAction('plunge', 3, {});
        this.velocity.y = Math.min(this.velocity.y, -6);
        this.game.audio?.play('swing', { pitch: 0.7 });
        this.game.audio?.voice?.('attack');
      }
      return;
    }
    if (this.gliding) return;
    if (this.action && this.action.name.startsWith('attack')) {
      if (this.action.t / this.action.dur > 0.3) this.comboQueued = true;
      return;
    }
    if (this.action && this.action.name !== 'block') return;
    if (st.sp < 4) { this.game.events.emit('toast', 'Too tired to attack'); return; }
    this.character.drawWeapon(true);
    if (this.sprinting && this.speed > 7) {
      st.sp -= 12; this.staminaDelay = 0.8;
      this.startAction('spin', 0.62, { strikeAt: 0.4 });
      this.game.audio?.play('swing', { pitch: 0.8 });
      this.game.audio?.voice?.('attack');
      return;
    }
    this.beginAttack(1);
  }

  beginAttack(n) {
    const durs = { 1: 0.48, 2: 0.48, 3: 0.68 };
    this.startAction('attack' + n, durs[n], { combo: n, strikeAt: n === 3 ? 0.42 : 0.38 });
    this.stats.sp -= n === 3 ? 10 : 6;
    this.staminaDelay = 0.8;
    this.faceTarget(true);
    this.game.audio?.play('swing', { pitch: 0.9 + n * 0.08 });
    if (n === 3 || Math.random() < 0.35) this.game.audio?.voice?.('attack');
  }

  // Area strike used by heavy finishers, plunges and the Aether Surge.
  shockwave(radius, dmg, element, opts = {}) {
    const g = this.game;
    let hits = 0;
    for (const e of g.combat.enemies) {
      if (e.dead) continue;
      const d = Math.hypot(e.position.x - this.position.x, e.position.z - this.position.z);
      if (d < radius + e.radius && Math.abs(e.position.y - this.position.y) < 3.5) {
        e.takeDamage(dmg * (1 - (d / (radius + e.radius)) * 0.3), this, { element, knockback: opts.knockback ?? 9, heavy: true, melee: true, breakMult: opts.breakMult ?? 1.5 });
        hits++;
      }
    }
    g.fx.shockwave(this.position, radius, opts.color ?? 0xdfe8ff);
    g.fx.dust(this.position, 16);
    g.cameraRig.shake(opts.shake ?? 0.5);
    g.audio?.play('slam');
    this.registerHits(hits, dmg);
    if (hits) g.hitStop = 0.1;
    return hits;
  }

  faceTarget(snap) {
    const t = this.lockTarget || this.softTarget;
    if (t && !t.dead) {
      const yaw = Math.atan2(t.position.x - this.position.x, t.position.z - this.position.z);
      this.yaw = snap ? yaw : dampAngle(this.yaw, yaw, 12, this.game.dt);
    }
  }

  tryCast(id) {
    const sp = SPELLS[id];
    if (!sp || !this.stats.spells.includes(id)) return;
    if (this.action && this.action.name !== 'block') return;
    if (this.swimming) return;
    if ((this.spellCd[id] || 0) > 0) return;
    const cost = sp.cost * (1 - (this.skill('sorcery') - 1) * 0.01);
    if (this.stats.mp < cost) { this.game.events.emit('toast', 'Not enough mana'); this.game.audio?.play('error'); return; }
    this.stats.mp -= cost;
    this.spellCd[id] = sp.cd;
    this.selectedSpell = id;
    this.faceTarget(true);
    this.startAction(sp.kind === 'strike' ? 'castUp' : 'cast', sp.kind === 'heal' ? 0.6 : 0.5, { spell: id, castAt: 0.3 });
    this.game.audio?.play('cast', { spell: id });
    this.combatTimer = Math.max(this.combatTimer, sp.kind === 'heal' ? 0 : 4);
  }

  takeDamage(amount, source, opts = {}) {
    if (this.dead || this.invuln > 0 || this.game.godMode) return 0;
    if (this.action && (this.action.name === 'surge' || this.action.name === 'plunge')) return 0;
    if (this.action && this.action.name === 'dodge' && this.action.t > 0.03 && this.action.t < this.action.dur * 0.8) {
      if (this.action.t < 0.22 && !this.action.perfect) {
        this.action.perfect = true;
        this.game.slowMo(0.3, 0.55);
        this.addSurge(15);
        this.game.events.emit('perfectDodge');
        this.game.audio?.play('perfect');
      } else this.game.events.emit('dodged');
      return 0;
    }
    let dmg = amount;
    const blocking = this.action && (this.action.name === 'block' || this.action.name === 'parry');
    // Parry: block just before a melee blow lands.
    if (blocking && source && !opts.ranged && !opts.unblockable && this.game.time - this.blockT < 0.24 && source.parried) {
      const dx = source.position.x - this.position.x, dz = source.position.z - this.position.z;
      const facing = (dx * Math.sin(this.yaw) + dz * Math.cos(this.yaw)) / (Math.hypot(dx, dz) || 1);
      if (facing > 0) {
        source.parried(this);
        this.counterT = 1.6;
        this.addSurge(20);
        this.trainSkill('guard', 5);
        this.startAction('parry', 0.35);
        this.game.slowMo(0.25, 0.4);
        this.game.events.emit('parry', source);
        this.game.events.emit('flash', 0.35);
        this.game.audio?.play('parry');
        this.game.fx?.sparks(tmpV.copy(this.position).add(new THREE.Vector3(Math.sin(this.yaw) * 0.7, 1.3, Math.cos(this.yaw) * 0.7)), 0xffffff, 30);
        return 0;
      }
    }
    if (blocking && source) {
      const dx = source.position.x - this.position.x, dz = source.position.z - this.position.z;
      const facing = (dx * Math.sin(this.yaw) + dz * Math.cos(this.yaw)) / (Math.hypot(dx, dz) || 1);
      if (facing > 0.2 && this.stats.sp > 0 && !opts.unblockable) {
        const strength = 0.75 + (this.skill('guard') - 1) * 0.02;
        dmg *= 1 - Math.min(strength, 0.92);
        this.stats.sp = Math.max(0, this.stats.sp - amount * 0.6);
        this.staminaDelay = 1;
        this.trainSkill('guard', 2);
        this.game.events.emit('block', this);
        this.game.audio?.play('block');
        this.game.fx?.sparks(tmpV.copy(this.position).add(new THREE.Vector3(Math.sin(this.yaw) * 0.6, 1.3, Math.cos(this.yaw) * 0.6)), 0xffe8a0, 14);
      }
    }
    dmg *= 100 / (100 + this.armor * 2.5);
    dmg = Math.max(1, Math.round(dmg));
    this.stats.hp -= dmg;
    this.addSurge(dmg * 0.5);
    this.game.audio?.voice?.('hurt', dmg);
    this.combatTimer = 6;
    this.game.events.emit('player:hurt', dmg, opts);
    this.game.cameraRig?.shake(Math.min(0.6, dmg / 30));
    if (this.stats.hp <= 0) { this.stats.hp = 0; this.die(); return dmg; }
    const armored = this.action && (['heavy', 'charge', 'spin', 'surge', 'plunge'].includes(this.action.name) || (this.action.name.startsWith('attack') && this.action.combo === 3));
    if (!blocking && dmg > 8 && !armored) {
      this.startAction('hit', 0.35);
      if (source) {
        const dx = this.position.x - source.position.x, dz = this.position.z - source.position.z;
        const d = Math.hypot(dx, dz) || 1;
        this.velocity.x += (dx / d) * (opts.knockback ?? 4);
        this.velocity.z += (dz / d) * (opts.knockback ?? 4);
      }
    }
    if (opts.slow) this.slow = Math.max(this.slow, opts.slow);
    return dmg;
  }

  heal(n) {
    this.stats.hp = Math.min(this.maxHp, this.stats.hp + n);
  }

  die() {
    this.dead = true;
    this.deadT = 0;
    this.action = null;
    this.gliding = false;
    this.game.audio?.voice?.('death');
    this.game.events.emit('player:died');
  }

  revive() {
    this.dead = false;
    this.deadT = 0;
    this.stats.hp = this.maxHp;
    this.stats.mp = this.maxMp;
    this.stats.sp = this.maxSp;
    this.invuln = 2;
  }

  // ---------------- per-frame
  update(dt, input, rig) {
    const game = this.game, env = game.env, st = this.stats;
    if (!this.character) return;
    this.invuln = Math.max(0, this.invuln - dt);
    this.counterT = Math.max(0, this.counterT - dt);
    this.comboT = Math.max(0, this.comboT - dt);
    if (this.comboT <= 0) this.comboCount = 0;
    this.combatTimer = Math.max(0, this.combatTimer - dt);
    this.slow = Math.max(0, this.slow - dt * 0.5);
    for (const k in this.spellCd) this.spellCd[k] = Math.max(0, this.spellCd[k] - dt);

    if (this.dead) {
      this.deadT += dt;
      this.velocity.x = damp(this.velocity.x, 0, 5, dt);
      this.velocity.z = damp(this.velocity.z, 0, 5, dt);
      this.applyPhysics(dt, env, false);
      this.syncCharacter(dt);
      return;
    }

    // ---- intents
    if (input.pressed('lockon')) {
      if (this.lockTarget) this.lockTarget = null;
      else if (this.softTarget) this.lockTarget = this.softTarget;
    }
    if (this.lockTarget && (this.lockTarget.dead || this.lockTarget.position.distanceTo(this.position) > 30)) this.lockTarget = null;
    if (input.pressed('draw')) this.character.drawWeapon(!this.character.weaponDrawn);
    if (input.pressed('crouch')) this.crouching = !this.crouching;
    if (input.pressed('attack')) this.tryAttack();
    for (let i = 1; i <= 4; i++) if (input.pressed('spell' + i)) this.tryCast(SPELL_SLOTS[i - 1]);
    if (input.pressed('cast')) this.tryCast(this.selectedSpell);
    if (input.pressed('spellNext') || input.pressed('spellPrev')) {
      const list = SPELL_SLOTS.filter((id) => st.spells.includes(id));
      const i = list.indexOf(this.selectedSpell);
      const n = (i + (input.pressed('spellNext') ? 1 : list.length - 1)) % list.length;
      this.selectedSpell = list[n];
      game.events.emit('spell:selected', this.selectedSpell);
    }
    if (input.pressed('surge') && this.surge >= 100 && this.grounded && !this.swimming && !(this.action && ['surge', 'plunge', 'dodge'].includes(this.action.name))) {
      this.surge = 0;
      this.character.drawWeapon(true);
      this.startAction('surge', 1.25, { strikeAt: 0.42 });
      this.game.slowMo(0.5, 0.5);
      this.game.audio?.voice?.('surge');
      this.game.audio?.play('surgeCharge');
      this.game.events.emit('surge:used');
    }
    const blockHeld = input.held('block') && !this.swimming && this.grounded;
    if (blockHeld && !this.action) { this.startAction('block', Infinity); this.character.drawWeapon(true); this.blockT = game.time; }
    if (!blockHeld && this.action && this.action.name === 'block') this.action = null;
    if (input.pressed('dodge') && this.grounded && !this.swimming && (!this.action || this.action.name === 'block' || (this.action.name.startsWith('attack') && this.action.t > this.action.dur * 0.5))) {
      if (st.sp >= 15) {
        st.sp -= 15; this.staminaDelay = 0.8;
        this.startAction('dodge', 0.55);
        const mv = this.moveVector(input, rig, tmpV);
        const dir = mv.lengthSq() > 0.01 ? mv.normalize() : fwd.set(-Math.sin(this.yaw), 0, -Math.cos(this.yaw));
        this.action.dir = dir.clone();
        this.yaw = Math.atan2(dir.x, dir.z);
        game.audio?.play('dodge');
        this.trainSkill('athletics', 1);
      }
    }

    // ---- action progress
    const a = this.action;
    if (a) {
      const prev = a.t;
      a.t += dt;
      const w = this.game.inventory.equipped.weapon;
      const element = w ? ITEMS[w].element : null;
      const counter = this.counterT > 0 ? 2 : 1;
      if (a.name.startsWith('attack') && !a.hit && a.t >= a.dur * a.strikeAt) {
        a.hit = true;
        const n = a.combo;
        const dmg = this.meleeDamage() * (n === 3 ? 1.6 : 1) * counter;
        const hits = game.combat.melee(this, { range: 2.6 + (w ? 0.3 : 0), arc: n === 3 ? 0.2 : -0.1, dmg, element, knockback: n === 3 ? 7 : 3, heavy: n === 3, crit: counter > 1 });
        if (hits > 0) { this.trainSkill('blade', hits * 1.5); this.registerHits(hits, dmg); if (counter > 1) this.counterT = 0; }
      }
      if (a.name === 'spin' && !a.hit && a.t >= a.dur * a.strikeAt) {
        a.hit = true;
        const dmg = this.meleeDamage() * 1.35 * counter;
        const hits = game.combat.melee(this, { range: 3.0, arc: -1.1, dmg, element, knockback: 8, heavy: true });
        this.registerHits(hits, dmg);
      }
      if (a.name === 'heavy' && !a.hit && a.t >= a.dur * a.strikeAt) {
        a.hit = true;
        const pw = a.power;
        const dmg = this.meleeDamage() * (1.9 + pw * 1.6) * counter;
        const hits = game.combat.melee(this, { range: 3.0, arc: 0.1, dmg, element, knockback: 10, heavy: true, breakMult: 2.2, crit: counter > 1 });
        this.registerHits(hits, dmg);
        if (pw > 0.95) this.shockwave(3.6, dmg * 0.6, element, { breakMult: 1.5 });
        else game.cameraRig.shake(0.3);
        this.trainSkill('blade', 3);
      }
      if (a.name === 'surge' && !a.hit && a.t >= a.dur * a.strikeAt) {
        a.hit = true;
        const dmg = 70 + this.stats.level * 9 + this.meleeDamage() * 2.5;
        this.shockwave(7.5, dmg, 'aether', { knockback: 16, breakMult: 3, color: 0x9ff0ff, shake: 1.0 });
        game.events.emit('flash', 0.9);
        game.fx.levelUp(this.position);
        game.audio?.play('surge');
      }
      if (a.name === 'plunge') {
        if (this.grounded || this.swimming) {
          if (!this.swimming) this.shockwave(3.2, this.meleeDamage() * 2.2 * counter, element, { breakMult: 1.8 });
          this.action = null;
          this.startAction('attack3', 0.35, { combo: 3, strikeAt: 2, hit: true });
          this.action.t = 0.2;
        } else this.velocity.y = Math.min(this.velocity.y, -22);
      }
      if (a.name === 'charge') {
        a.power = Math.min(1, a.t / 1.1);
        if (a.power >= 1 && !a.full) { a.full = true; game.fx.burst(tmpV.copy(this.position).setY(this.position.y + 1.4), 0xffe8a0, 24, { speed: 3 }); game.audio?.play('chargeFull'); }
        if (!input.held('attack') || st.sp <= 0) {
          this.startAction('heavy', 0.7, { strikeAt: 0.6, power: a.power });
          game.audio?.play('swing', { pitch: 0.65 });
          game.audio?.voice?.('heavy');
        } else { st.sp -= dt * 6; this.staminaDelay = 0.5; }
      }
      // Keep the blade's trail going during swings.
      if (this.trail && this.character.weapon && /attack|heavy|spin|surge|plunge/.test(a.name)) {
        const wpn = this.character.weapon;
        const L = wpn.userData.length ?? 1;
        this.trail.push(wpn.localToWorld(new THREE.Vector3(0, 0.25, 0)), wpn.localToWorld(new THREE.Vector3(0, L, 0)));
      }
      if (a.name.startsWith('attack') && a.t < a.dur * 0.3) this.faceTarget(false);
      if ((a.name === 'cast' || a.name === 'castUp') && !a.hit && a.t >= a.castAt) {
        a.hit = true;
        game.combat.castSpell(this, a.spell, this.lockTarget || this.softTarget);
        this.trainSkill('sorcery', SPELLS[a.spell].cost / 8);
      }
      if (a.t >= a.dur && this.action === a) {
        this.action = null;
        if (a.name.startsWith('attack') && this.comboQueued && a.combo < 3 && st.sp >= 4) this.beginAttack(a.combo + 1);
        else if (a.name.startsWith('attack') && input.held('attack') && st.sp > 8) this.startAction('charge', Infinity, { power: 0 });
        this.comboQueued = false;
      }
      void prev;
    }

    // ---- movement
    const mv = this.moveVector(input, rig, tmpV);
    const hasMove = mv.lengthSq() > 0.0025;
    const athletics = 1 + (this.skill('athletics') - 1) * 0.01;
    this.sprinting = input.held('sprint') && hasMove && st.sp > 1 && !this.crouching && !(this.action && this.action.name === 'block');
    let target = (this.crouching ? 2.2 : 5.4) * athletics;
    if (this.sprinting) target = 8.6 * athletics;
    if (this.swimming) target = this.sprinting ? 4.6 : 3.0;
    if (this.action) {
      if (this.action.name === 'block') target = 1.6;
      else if (this.action.name === 'charge') target = 1.0;
      else if (this.action.name.startsWith('attack') || this.action.name.startsWith('cast') || this.action.name === 'heavy' || this.action.name === 'surge' || this.action.name === 'parry') target = 0.6;
      else if (this.action.name === 'hit') target = 0;
    }
    target *= 1 - this.slow * 0.5;
    const desired = mv.multiplyScalar(target * Math.min(1, mv.length() > 0 ? 1 : 0));

    if (this.action && this.action.name === 'dodge') {
      const k = 1 - this.action.t / this.action.dur;
      desired.copy(this.action.dir).multiplyScalar(4 + 7 * k);
    }
    if (this.action && this.action.name === 'spin') {
      desired.x = Math.sin(this.yaw) * 6; desired.z = Math.cos(this.yaw) * 6;
    }
    if (this.action && (this.action.name.startsWith('attack') || this.action.name === 'heavy') && this.action.t < this.action.dur * 0.45) {
      desired.x += Math.sin(this.yaw) * 2.2;
      desired.z += Math.cos(this.yaw) * 2.2;
    }

    let accel = this.grounded ? 40 : this.gliding ? 2.5 : 7;
    if (this.swimming) accel = 10;
    if (this.action && this.action.name === 'dodge') accel = 60;
    this.velocity.x = damp(this.velocity.x, desired.x, accel * 0.25, dt * 4);
    this.velocity.z = damp(this.velocity.z, desired.z, accel * 0.25, dt * 4);

    // Face movement direction (or the lock target).
    if (this.lockTarget && !this.gliding && !this.swimming) this.faceTarget(false);
    else if (hasMove && (!this.action || this.action.name === 'block')) this.yaw = dampAngle(this.yaw, Math.atan2(desired.x, desired.z), this.gliding ? 3 : 12, dt);

    // Jump / glide.
    if (input.pressed('jump')) {
      if (this.swimming) { /* treading water */ }
      else if (this.grounded && (!this.action || this.action.name === 'block')) {
        this.velocity.y = 8.3;
        this.grounded = false;
        this.action = null;
        game.audio?.play('jump');
        this.trainSkill('athletics', 0.3);
      } else if (!this.grounded && !this.gliding && st.sp > 5 && this.velocity.y < 2) {
        this.gliding = true;
        game.audio?.play('glide');
      } else if (this.gliding) this.gliding = false;
    }
    if (this.gliding) {
      const g = fwd.set(Math.sin(this.yaw), 0, Math.cos(this.yaw));
      const sp = 10.5;
      this.velocity.x = damp(this.velocity.x, g.x * sp + desired.x * 0.25, 1.5, dt);
      this.velocity.z = damp(this.velocity.z, g.z * sp + desired.z * 0.25, 1.5, dt);
      const cost = game.inventory.equipped.ring === 'windstep_charm' ? 3.5 : 6;
      st.sp -= cost * dt;
      this.staminaDelay = 0.6;
      if (st.sp <= 0) { st.sp = 0; this.gliding = false; }
    }

    // Stamina / mana / health regeneration.
    if (this.sprinting) { st.sp -= 13 * dt; this.staminaDelay = 0.6; this.trainSkill('athletics', dt * 0.4); }
    this.staminaDelay -= dt;
    if (this.staminaDelay <= 0 && !this.gliding) st.sp = Math.min(this.maxSp, st.sp + (20 + this.skill('athletics') * 0.6) * dt);
    st.mp = Math.min(this.maxMp, st.mp + (2.5 + this.skill('sorcery') * 0.15) * dt);
    if (this.combatTimer <= 0) st.hp = Math.min(this.maxHp, st.hp + 1.2 * dt);
    st.sp = Math.max(0, st.sp);

    this.applyPhysics(dt, env, true);

    // Soft target: nearest living enemy roughly in front.
    this.softTarget = game.combat.findTarget(this, 16, 0.35);

    this.syncCharacter(dt);
  }

  moveVector(input, rig, out) {
    const f = rig.forward(fwd);
    const rx = -f.z, rz = f.x; // right = forward rotated -90°
    out.set(f.x * input.move.y + rx * input.move.x, 0, f.z * input.move.y + rz * input.move.x);
    return out;
  }

  applyPhysics(dt, env, alive) {
    const p = this.position, v = this.velocity;
    const wasGrounded = this.grounded;
    // Gravity.
    if (this.swimming) {
      v.y = damp(v.y, 0, 6, dt);
    } else if (this.gliding) {
      v.y = Math.max(v.y - GRAV * 0.5 * dt, -2.6);
      if (v.y < -2.4) v.y = damp(v.y, -2.4, 5, dt);
    } else {
      v.y -= GRAV * dt;
    }
    // Slide down steep terrain.
    if (this.grounded && env.normal && !(this.game.world && !this.game.inDungeon && this.game.world.roadAt(p.x, p.z) > 0.3)) {
      env.normal(p.x, p.z, nrm);
      if (nrm.y < 0.6) {
        const k = (0.6 - nrm.y) * 50;
        v.x += nrm.x * k * dt; v.z += nrm.z * k * dt;
        const into = v.x * nrm.x + v.z * nrm.z;
        if (into < 0) { v.x -= nrm.x * into * 0.85; v.z -= nrm.z * into * 0.85; }
      }
    }
    p.x += v.x * dt; p.z += v.z * dt; p.y += v.y * dt;
    env.resolve(p, this.radius, this.height);
    if (!env.inBounds(p.x, p.z)) {
      if (env.recover) { env.recover(p); v.set(0, 0, 0); }
      else { const d = Math.hypot(p.x, p.z); p.x *= 989 / d; p.z *= 989 / d; }
    }
    // Ground.
    const g = env.ground(p.x, p.z, p.y);
    const snap = wasGrounded && v.y <= 0 && p.y - g.y < 0.6;
    if (p.y <= g.y || snap) {
      if (!wasGrounded && alive) this.land(this.fallStart - g.y, -v.y);
      p.y = g.y;
      if (v.y < 0) v.y = 0;
      this.grounded = true;
      this.gliding = false;
    } else {
      if (wasGrounded) this.fallStart = p.y;
      if (v.y > 0) this.fallStart = Math.max(this.fallStart, p.y);
      this.grounded = false;
    }
    if (this.gliding) this.fallStart = p.y;
    // Water.
    const wl = env.waterLevel(p.x, p.z);
    const depth = wl - g.y;
    if (depth > 1.35 && p.y < wl - 1.1) {
      if (!this.swimming) { this.game.events.emit('splash', p); this.game.audio?.play('splash'); }
      this.swimming = true;
      this.gliding = false;
      this.fallStart = p.y;
      p.y = Math.max(p.y, wl - 1.25);
      if (p.y < wl - 1.25 + 0.05) v.y = Math.max(v.y, 0);
      this.grounded = false;
      if (this.action && this.action.name !== 'hit') this.action = null;
    } else if (this.swimming && (depth <= 1.3 || p.y > wl - 0.9)) {
      this.swimming = false;
    }
    this.speed = Math.hypot(v.x, v.z);
  }

  land(height, impact) {
    this.landedImpact = Math.min(1, impact / 14);
    this.game.events.emit('land', this.position, impact);
    if (impact > 4) this.game.audio?.play('land', { volume: Math.min(1, impact / 15) });
    if (height > 9 && impact > 15) {
      const dmg = Math.round((height - 9) * 6);
      this.takeDamage(dmg, null, { fall: true });
      this.game.events.emit('toast', 'Ouch! Glide (Space in mid-air) to avoid fall damage.');
    }
  }

  syncCharacter(dt) {
    const ch = this.character;
    ch.group.position.copy(this.position);
    ch.group.rotation.y = this.yaw;
    const a = this.action;
    const lean = this.grounded && this.speed > 4 ? clamp(-(this.yawVel || 0) * 0.1, -0.4, 0.4) : 0;
    ch.update(dt, {
      speed: this.speed, grounded: this.grounded || this.swimming, vy: this.velocity.y, glide: this.gliding, swim: this.swimming,
      action: a ? a.name : null, actionT: a ? Math.min(1, a.t / (a.dur === Infinity ? 1 : a.dur)) : 0,
      dead: this.dead, deadT: this.deadT, crouch: this.crouching, lean, landed: this.landedImpact || 0,
    });
    this.landedImpact = 0;
    if (!this.trail) { this.trail = new SwordTrail(this.game.scene); }
    this.trail.attach(this.game.scene);
    if (ch.weapon?.userData.color) this.trail.setColor(ch.weapon.userData.color);
    this.trail.update(dt);
    if (this.wings) {
      const s = damp(this.wings.scale.x, this.gliding ? 1 : 0.01, 8, dt);
      this.wings.scale.setScalar(s);
      this.wings.visible = s > 0.05;
      this.wings.material.opacity = 0.45 + Math.sin(this.game.time * 8) * 0.1;
    }
    // Footsteps.
    if (this.grounded && this.speed > 1) {
      const step = Math.floor(ch.anim.phase / Math.PI);
      if (step !== this.lastStep) {
        this.lastStep = step;
        this.game.events.emit('footstep', this.position, this.game.env.surface?.(this.position.x, this.position.z) || 'stone', this.speed);
      }
    }
  }
}

export { makeGlowMaterial, lerp };
