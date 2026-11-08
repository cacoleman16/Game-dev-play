// Combat glue: melee hit detection, spells, projectiles, targeting, hit-stop,
// and rewards (XP, loot) when enemies fall.

import * as THREE from 'three';
import { SPELLS, rollLoot, ITEMS } from './content.js';
import { ENEMY_TYPES } from '../entities/enemies.js';

const v1 = new THREE.Vector3(), v2 = new THREE.Vector3();

export class Combat {
  constructor(game) {
    this.game = game;
    this.enemies = [];
    this.projectiles = [];
    this.projGeo = new THREE.IcosahedronGeometry(1, 1);
    game.events.on('enemy:killed', (e, source) => this.onKilled(e, source));
  }

  add(enemy) { this.enemies.push(enemy); return enemy; }

  findTarget(player, range, minDot) {
    const cam = this.game.cameraRig;
    const f = cam ? cam.forward(v1) : v1.set(Math.sin(player.yaw), 0, Math.cos(player.yaw));
    let best = null, bestScore = Infinity;
    for (const e of this.enemies) {
      if (e.dead) continue;
      const dx = e.position.x - player.position.x, dz = e.position.z - player.position.z;
      const d = Math.hypot(dx, dz);
      if (d > range + e.radius) continue;
      const dot = d > 0.01 ? (dx * f.x + dz * f.z) / d : 1;
      if (dot < minDot && d > 3) continue;
      const score = d * (2.2 - dot);
      if (score < bestScore) { bestScore = score; best = e; }
    }
    return best;
  }

  melee(attacker, o) {
    let hits = 0;
    const fx = Math.sin(attacker.yaw), fz = Math.cos(attacker.yaw);
    for (const e of this.enemies) {
      if (e.dead) continue;
      const dx = e.position.x - attacker.position.x, dz = e.position.z - attacker.position.z;
      const d = Math.hypot(dx, dz);
      if (d > o.range + e.radius) continue;
      if (Math.abs(e.position.y - attacker.position.y) > 2.5 + (e.boss ? 2 : 0)) continue;
      const dot = d > 0.01 ? (dx * fx + dz * fz) / d : 1;
      if (dot < o.arc && d > e.radius + 0.5) continue;
      e.takeDamage(o.dmg, attacker, { element: o.element, knockback: o.knockback, heavy: o.heavy, melee: true });
      hits++;
      const hp = v1.copy(e.position).setY(e.position.y + (e.boss ? 2 : 1.1));
      this.game.fx.hit(hp, o.element === 'fire' ? 0xff8a3a : o.element === 'frost' ? 0x9fdcff : o.element === 'aether' ? 0xb48cff : 0xfff2c0);
    }
    if (hits) {
      this.game.hitStop = o.heavy ? 0.09 : 0.055;
      this.game.cameraRig.shake(o.heavy ? 0.35 : 0.15);
      this.game.audio?.play('hit', { heavy: o.heavy });
    }
    return hits;
  }

  castSpell(caster, id, target) {
    const sp = SPELLS[id];
    const game = this.game;
    const power = 1 + (caster.skill ? (caster.skill('sorcery') - 1) * 0.04 : 0);
    const hand = v1.set(caster.position.x + Math.sin(caster.yaw) * 0.5 + Math.cos(caster.yaw) * 0.25, caster.position.y + 1.35, caster.position.z + Math.cos(caster.yaw) * 0.5 - Math.sin(caster.yaw) * 0.25).clone();
    if (sp.kind === 'heal') {
      caster.heal(sp.amount * power);
      game.fx.healAura(caster.position);
      game.events.emit('toast', `+${Math.round(sp.amount * power)} Health`);
      return;
    }
    if (sp.kind === 'bolt') {
      let aim;
      if (target && !target.dead) aim = target.position.clone().setY(target.position.y + (target.boss ? 2 : 1));
      else {
        const f = game.cameraRig.forward(v2);
        const pitch = game.cameraRig.pitch;
        aim = hand.clone().add(new THREE.Vector3(f.x, -Math.sin(pitch) * 0.6 + 0.02, f.z).normalize().multiplyScalar(40));
      }
      this.spawnProjectile({ from: hand, target: aim, speed: sp.speed, dmg: sp.dmg * power, team: 'player', owner: caster, color: sp.color, element: sp.element, slow: sp.slow, homing: target && !target.dead ? target : null, size: 0.3 });
      return;
    }
    if (sp.kind === 'strike') {
      let at;
      if (target && !target.dead) at = target.position.clone();
      else {
        const f = game.cameraRig.forward(v2);
        at = caster.position.clone().add(new THREE.Vector3(f.x, 0, f.z).multiplyScalar(10));
        at.y = game.env.ground(at.x, at.z, at.y + 5).y;
      }
      const from = at.clone().add(new THREE.Vector3(Math.random() * 4 - 2, 40, Math.random() * 4 - 2));
      game.fx.lightning(from, at.clone().setY(at.y + 0.2));
      game.audio?.play('thunder');
      game.cameraRig.shake(0.5);
      game.events.emit('flash', 0.6);
      for (const e of this.enemies) {
        if (e.dead) continue;
        const d = Math.hypot(e.position.x - at.x, e.position.z - at.z);
        if (d < sp.radius + e.radius) e.takeDamage(sp.dmg * power * (d < 1.5 ? 1.2 : 1), caster, { element: sp.element, knockback: 6, heavy: true });
      }
    }
  }

  spawnProjectile(o) {
    const mat = new THREE.MeshBasicMaterial({ color: new THREE.Color(o.color).multiplyScalar(2.2) });
    const mesh = new THREE.Mesh(this.projGeo, mat);
    mesh.scale.setScalar(o.size ?? 0.3);
    mesh.position.copy(o.from);
    this.game.scene.add(mesh);
    const dir = o.target.clone().sub(o.from).normalize();
    this.projectiles.push({ ...o, pos: o.from.clone(), vel: dir.multiplyScalar(o.speed), mesh, life: 3.5 });
    if (o.team === 'enemy') this.game.audio?.play('enemyCast');
  }

  update(dt) {
    const game = this.game;
    for (let i = this.projectiles.length - 1; i >= 0; i--) {
      const p = this.projectiles[i];
      p.life -= dt;
      if (p.homing && !p.homing.dead) {
        const t = v1.copy(p.homing.position).setY(p.homing.position.y + (p.homing.boss ? 2 : 1)).sub(p.pos).normalize().multiplyScalar(p.speed);
        p.vel.lerp(t, 1 - Math.exp(-4 * dt));
      }
      p.pos.addScaledVector(p.vel, dt);
      p.mesh.position.copy(p.pos);
      game.fx.trail(p.pos, p.color, (p.size ?? 0.3) * 1.8);
      let hit = false;
      if (p.team === 'player') {
        for (const e of this.enemies) {
          if (e.dead) continue;
          const cy = e.position.y + (e.boss ? 2 : 1);
          if (Math.hypot(e.position.x - p.pos.x, e.position.z - p.pos.z) < e.radius + 0.5 && Math.abs(cy - p.pos.y) < (e.boss ? 3 : 1.5)) {
            e.takeDamage(p.dmg, p.owner, { element: p.element, slow: p.slow, knockback: 2 });
            hit = true; break;
          }
        }
      } else {
        const pl = game.player;
        if (!pl.dead && Math.hypot(pl.position.x - p.pos.x, pl.position.z - p.pos.z) < 0.7 && Math.abs(pl.position.y + 1 - p.pos.y) < 1.3) {
          pl.takeDamage(p.dmg, p.owner, { element: p.element, knockback: 3 });
          hit = true;
        }
      }
      const g = game.env.ground(p.pos.x, p.pos.z, p.pos.y);
      if (p.pos.y < g.y || (game.inDungeon && game.dungeon.solidAt(p.pos.x, p.pos.z, p.pos.y))) hit = true;
      if (hit || p.life <= 0) {
        game.fx.burst(p.pos, p.color, 22, { speed: 4, size: 0.35, life: 0.45 });
        if (hit) game.audio?.play('impact', { element: p.element });
        game.scene.remove(p.mesh);
        p.mesh.material.dispose();
        this.projectiles.splice(i, 1);
      }
    }
    for (const e of this.enemies) e.update(dt);
    this.enemies = this.enemies.filter((e) => !e.removed);
  }

  clearAll() {
    for (const e of this.enemies) if (!e.removed) e.remove();
    this.enemies = [];
    for (const p of this.projectiles) { this.game.scene.remove(p.mesh); p.mesh.material.dispose(); }
    this.projectiles = [];
  }

  onKilled(e) {
    const game = this.game;
    const def = ENEMY_TYPES[e.type];
    const xp = Math.round(def.xp * (1 + (e.level - 1) * 0.2));
    game.player.gainXp(xp);
    game.player.stats.kills++;
    const loot = def.loot ? rollLoot(def.loot) : [];
    const got = [];
    for (const l of loot) {
      if (l.item === 'gold') { game.player.stats.gold += l.n; got.push(`${l.n} gold`); }
      else { game.inventory.add(l.item, l.n, true); got.push(`${ITEMS[l.item].name}${l.n > 1 ? ' ×' + l.n : ''}`); }
    }
    game.events.emit('loot', got, xp);
    game.fx.burst(v1.copy(e.position).setY(e.position.y + 1), 0xffe27a, 18, { speed: 3, size: 0.25, life: 0.8, up: 2 });
    if (e.zone) e.zone.onKilled(e);
  }
}
