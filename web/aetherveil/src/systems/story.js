// World events that tie systems together: discovering places, waystones and
// fast travel, chests, gathering herbs, boss encounters and their shards, the
// Heartshard pedestal and finale, death and respawn, and the weather.

import * as THREE from 'three';
import { LOC, LOCATIONS } from '../world/locations.js';
import { ITEMS, rollLoot, SPELLS } from './content.js';
import { REGION } from '../world/worldgen.js';
import { hash2 } from '../core/noise.js';
import { makeGlowMaterial } from '../world/vegetation.js';
import { paint, place, merge } from '../world/geo.js';
import { VOICES } from '../audio/audio.js';
import { clamp, damp, smoothstep } from '../core/util.js';

const HERB_KINDS = {
  glowcap: { name: 'Glowcap', color: 0x6ad8ff, stem: 0xe8e0d0 },
  frostpetal: { name: 'Frostpetal', color: 0xe8f4ff, stem: 0x7ab8d8 },
  emberroot: { name: 'Emberroot', color: 0xff8a3a, stem: 0x8a4a2a },
};

export class Story {
  constructor(game) {
    this.game = game;
    this.discovered = new Set();
    this.waystones = new Set();
    this.lastWaystone = 'brightwater';
    this.openedChests = new Set();
    this.herbs = [];
    this.activeBoss = null;
    this.bossKey = null;
    this.checkT = 0;
    this.weatherT = 0;
    this.weatherTarget = { cloud: 0.4, rain: 0, storm: 0 };
    this.endingT = -1;
    const ev = game.events;
    ev.on('waystone:use', (ws) => this.useWaystone(ws));
    ev.on('chest:open', (c) => this.openChest(c));
    ev.on('heartshard:pedestal', () => this.pedestal());
    ev.on('enemy:killed', (e) => this.onKill(e));
    ev.on('player:died', () => this.onDeath());
    this.matGlow = makeGlowMaterial(1.2);
  }

  // ---------------- herbs
  buildHerbs() {
    const w = this.game.world;
    const geos = {};
    // One merged, vertex-coloured mesh per herb so each costs a single draw.
    const mk = (kind) => {
      if (geos[kind]) return geos[kind];
      const k = HERB_KINDS[kind];
      const parts = [];
      const glowCol = new THREE.Color(k.color).multiplyScalar(1.5);
      if (kind === 'glowcap') {
        for (let i = 0; i < 3; i++) {
          const a = i * 2.1, r = i ? 0.18 : 0;
          parts.push(place(paint(new THREE.CylinderGeometry(0.03, 0.04, 0.22 - i * 0.04, 6), k.stem), Math.cos(a) * r, 0.11, Math.sin(a) * r));
          parts.push(place(paint(new THREE.SphereGeometry(0.12 - i * 0.02, 10, 6, 0, Math.PI * 2, 0, Math.PI / 2), glowCol), Math.cos(a) * r, 0.22 - i * 0.04, Math.sin(a) * r));
        }
      } else {
        parts.push(place(paint(new THREE.CylinderGeometry(0.02, 0.03, 0.4, 5), k.stem), 0, 0.2, 0));
        for (let i = 0; i < 5; i++) {
          const a = (i / 5) * Math.PI * 2;
          parts.push(place(paint(new THREE.SphereGeometry(0.07, 6, 4), glowCol), Math.cos(a) * 0.09, 0.42, Math.sin(a) * 0.09, [0, -a, 0], [1, 0.5, 1.6]));
        }
      }
      geos[kind] = merge(parts);
      return geos[kind];
    };
    let n = 0;
    for (let i = 0; i < 4000 && n < 90; i++) {
      const x = (hash2(i, 1, 77) - 0.5) * 1800, z = (hash2(i, 2, 77) - 0.5) * 1800;
      const h = w.heightAt(x, z);
      if (h < 1.5 || w.slopeAt(x, z) > 0.35 || w.roadAt(x, z) > 0) continue;
      const reg = w.regionAt(x, z), biome = w.biomeAt(x, z);
      let kind = null;
      if (reg === REGION.FOREST || biome === 10) kind = 'glowcap';
      else if (reg === REGION.MOUNTAIN && h > 60) kind = 'frostpetal';
      else if (reg === REGION.DESERT) kind = 'emberroot';
      if (!kind) continue;
      if (this.game.colliders.occupied(x, z, 1)) continue;
      const mesh = new THREE.Mesh(mk(kind), this.matGlow);
      mesh.position.set(x, h, z);
      mesh.rotation.y = hash2(i, 3, 77) * 6.28;
      mesh.updateMatrix();
      mesh.matrixAutoUpdate = false;
      mesh.visible = false;
      this.game.scene.add(mesh);
      const herb = { id: n, kind, x, z, y: h, mesh, takenAt: -1e9 };
      this.herbs.push(herb);
      this.game.interact.add({
        id: 'herb:' + n, x, y: h + 0.5, z, r: 2.2, label: `Gather ${HERB_KINDS[kind].name}`,
        enabled: () => this.herbReady(herb) && mesh.visible,
        action: () => this.gather(herb),
      });
      n++;
    }
  }

  herbReady(h) { return this.game.totalHours - h.takenAt > 24; }

  gather(herb) {
    const g = this.game;
    herb.takenAt = g.totalHours;
    herb.mesh.visible = false;
    const count = 1 + (Math.random() < 0.35 ? 1 : 0);
    g.inventory.add(herb.kind, count);
    g.fx.burst(new THREE.Vector3(herb.x, herb.y + 0.4, herb.z), HERB_KINDS[herb.kind].color, 16, { speed: 2, up: 1.5 });
    g.audio?.play('pickup');
    g.player.startAction('interact', 0.5);
  }

  // ---------------- per-frame
  update(dt) {
    const g = this.game;
    this.checkT -= dt;
    if (this.checkT <= 0) {
      this.checkT = 0.4;
      this.checkDiscovery();
      this.checkBosses();
      const cp = g.camera.position;
      for (const h of this.herbs) {
        const near = Math.abs(h.x - cp.x) + Math.abs(h.z - cp.z) < 160;
        h.mesh.visible = near && !g.inDungeon && this.herbReady(h);
      }
    }
    this.updateWeather(dt);
    if (this.activeBoss) {
      const b = this.activeBoss;
      g.ui.updateBossBar(b);
      if (b.type === 'herald' && !b.dead) {
        const lines = ['You patch the cracks, little wayfarer. The sky remembers how to break.', 'Enough! Let the hollow swallow you whole!'];
        const k = b.hp / b.maxHp;
        if (b.taunted === 0 && k < 0.6) { b.taunted = 1; g.audio?.speak(lines[0], { ...VOICES.herald, id: 'herald' }, true); g.ui.toast('The Herald: "' + lines[0] + '"'); }
        if (b.taunted === 1 && k < 0.25) { b.taunted = 2; g.audio?.speak(lines[1], { ...VOICES.herald, id: 'herald' }, true); g.ui.toast('The Herald: "' + lines[1] + '"'); }
      }
    }
    if (this.endingT >= 0) this.updateEnding(dt);
  }

  checkDiscovery() {
    const g = this.game, p = g.player.position;
    if (g.inDungeon) return;
    for (const l of LOCATIONS) {
      if (this.discovered.has(l.id)) continue;
      if (Math.hypot(p.x - l.x, p.z - l.z) < l.r + 12) {
        this.discovered.add(l.id);
        g.events.emit('discover', l);
        g.player.gainXp(15);
        g.audio?.play('discover');
        g.save?.autosave();
      }
    }
  }

  // ---------------- waystones
  useWaystone(ws) {
    const g = this.game;
    const isNew = !this.waystones.has(ws.id);
    this.waystones.add(ws.id);
    this.lastWaystone = ws.id;
    g.fx.burst(new THREE.Vector3(ws.x, ws.y + 3, ws.z), 0x8ff0ff, 40, { speed: 5 });
    g.audio?.play('waystone');
    if (isNew) g.events.emit('toast', `Waystone attuned: ${ws.name}`);
    g.ui.openWaystone(ws);
  }

  rest() {
    const p = this.game.player;
    p.stats.hp = p.maxHp; p.stats.mp = p.maxMp; p.stats.sp = p.maxSp;
    this.game.fx.healAura(p.position);
    this.game.save.save();
    this.game.events.emit('toast', 'Rested and saved');
  }

  travel(id) {
    const g = this.game;
    const ws = g.structures.waystones.find((w) => w.id === id);
    if (!ws) return;
    g.ui.fade(() => {
      if (g.inDungeon) g.dungeon.exit(true);
      g.spawner.despawnAll();
      this.despawnBoss();
      const a = Math.random() * Math.PI * 2;
      const x = ws.x + Math.cos(a) * 3.5, z = ws.z + Math.sin(a) * 3.5;
      g.player.teleport(x, g.env.ground(x, z, ws.y + 5).y + 0.2, z);
      g.player.yaw = Math.atan2(ws.x - x, ws.z - z) + Math.PI;
      g.cameraRig.yaw = g.player.yaw + Math.PI;
      g.sky.timeOfDay = (g.sky.timeOfDay + 1.5) % 24;
      g.totalHours += 1.5;
      g.terrain.update(x, z, true);
      g.vegetation.update(g.player.position);
      this.lastWaystone = id;
    });
  }

  waitHours(h) {
    const g = this.game;
    g.ui.fade(() => {
      g.sky.timeOfDay = (g.sky.timeOfDay + h) % 24;
      g.totalHours += h;
      const p = g.player;
      p.stats.hp = p.maxHp; p.stats.mp = p.maxMp; p.stats.sp = p.maxSp;
    });
  }

  // ---------------- chests
  openChest(c) {
    const g = this.game;
    if (c.opened) return;
    c.opened = true;
    this.openedChests.add(c.id);
    const loot = rollLoot(c.loot);
    const got = [];
    for (const l of loot) {
      if (l.item === 'gold') { g.player.stats.gold += l.n; got.push(`${l.n} gold`); }
      else { g.inventory.add(l.item, l.n, true); got.push(`${ITEMS[l.item].name}${l.n > 1 ? ' ×' + l.n : ''}`); }
    }
    g.events.emit('loot', got, 0, 'Chest');
    g.fx.burst(new THREE.Vector3(c.x, c.y + 0.8, c.z), 0xffe27a, 30, { speed: 3, up: 2 });
    g.audio?.play('chest');
    g.player.startAction('interact', 0.5);
    g.save?.autosave();
  }

  // ---------------- bosses
  bossSites() {
    const s = this.game.structures.points;
    return {
      warden: { type: 'warden', at: s['ruins.altar'], r: 20, flag: 'warden' },
      rimefang: { type: 'rimefang', at: s['shrine.arena'], r: 28, flag: 'rimefang' },
    };
  }

  checkBosses() {
    const g = this.game, p = g.player.position;
    if (g.inDungeon) return;
    if (this.activeBoss) {
      const b = this.activeBoss;
      if (!b.dead && (g.player.dead || Math.hypot(p.x - b.home.x, p.z - b.home.z) > 85)) this.despawnBoss();
      return;
    }
    for (const [key, site] of Object.entries(this.bossSites())) {
      if (g.flags[site.flag] || !site.at) continue;
      if (Math.hypot(p.x - site.at.x, p.z - site.at.z) < site.r) {
        this.spawnBoss(key, site.type, site.at.x, site.at.z);
        break;
      }
    }
  }

  spawnBoss(key, type, x, z) {
    const g = this.game;
    const e = g.spawner.spawn(type, x, z);
    e.state = 'chase';
    this.activeBoss = e;
    this.bossKey = key;
    g.ui.showBossBar(e);
    g.audio?.setMusic('boss');
    g.events.emit('boss:start', e);
    g.cameraRig.shake(0.6);
    g.fx.burst(e.position.clone().setY(e.position.y + 2), 0xffffff, 60, { speed: 8 });
    return e;
  }

  despawnBoss() {
    if (!this.activeBoss) return;
    if (!this.activeBoss.removed) this.activeBoss.remove();
    this.activeBoss = null;
    this.bossKey = null;
    this.game.ui.hideBossBar();
    this.game.audio?.setMusic(null);
  }

  onKill(e) {
    const g = this.game;
    if (e !== this.activeBoss) return;
    const key = this.bossKey;
    this.activeBoss = null;
    this.bossKey = null;
    g.ui.hideBossBar();
    g.audio?.setMusic(null);
    g.audio?.play('victory');
    g.flags[key] = true;
    const p = g.player;
    if (key === 'warden') {
      g.inventory.add('shard_verdance', 1);
      p.stats.baseHp += 25; p.stats.hp = p.maxHp;
      this.shardMoment('Shard of Verdance', 'Warm green light floods through you. (+25 max Health)', 0x7dff9a);
    } else if (key === 'rimefang') {
      g.inventory.add('shard_rime', 1);
      if (!p.stats.spells.includes('frostlance')) p.stats.spells.push('frostlance');
      this.shardMoment('Shard of Rime', 'Cold clarity settles in your mind. You learned Frost Lance! (press 3)', 0x9fdcff);
    } else if (key === 'colossus') {
      g.inventory.add('shard_ember', 1);
      if (!p.stats.spells.includes('skybreaker')) p.stats.spells.push('skybreaker');
      this.shardMoment('Shard of Embers', 'Thunder answers your call. You learned Skybreaker! (press 4)', 0xff8a4a);
    } else if (key === 'herald') {
      this.beginEnding();
    }
    g.save?.autosave();
  }

  shardMoment(name, text, color) {
    const g = this.game;
    g.fx.levelUp(g.player.position);
    g.fx.burst(g.player.position.clone().setY(g.player.position.y + 1.5), color, 60, { speed: 6 });
    g.ui.banner(name, text, color);
  }

  // ---------------- Heartshard pedestal & finale
  pedestal() {
    const g = this.game, q = g.quests;
    const st = q.stage('main');
    const inv = g.inventory;
    const focus = new THREE.Vector3(LOC.heartshard.x, 6, LOC.heartshard.z);
    if (st === 2) {
      g.dialogue.say('The Pedestal', {
        start: { text: 'As your palm meets the cold stone, the world drops away. You see three lights fall from the cracked crystal: green into the western forest, blue onto the northern peaks, orange into the eastern cliffs.', options: [{ text: '...', next: 'v2' }] },
        v2: { text: "A voice like breaking glass whispers from the rift above: 'The vessel is empty. The Herald is coming.' Then you are kneeling on the island again, breathing hard.", options: [{ text: 'I need to find those shards.', next: null, action: (g) => g.quests.advance('main', 3) }] },
      }, focus);
      g.cameraRig.shake(0.5);
      return;
    }
    const have = ['shard_verdance', 'shard_rime', 'shard_ember'].filter((s) => inv.has(s));
    this.updateSockets();
    if (st === 4 || (st === 3 && have.length === 3)) {
      g.dialogue.say('The Pedestal', {
        start: { text: 'The three shards rise from your pack on their own and settle into the sockets. The Heartshard above flares... and the rift answers. Something vast pushes through.', options: [{ text: 'Face it.', next: null, action: (g) => this.summonHerald() }] },
      }, focus);
      return;
    }
    if (st === 5) {
      if (!this.activeBoss) this.summonHerald();
      return;
    }
    const msg = q.done('main') ? 'The pedestal hums, warm and content.' : st >= 3 ? `Three empty sockets. You carry ${have.length} of 3 shards.` : 'An old stone pedestal with three empty sockets. It hums faintly under your hand.';
    g.dialogue.say('The Pedestal', { start: { text: msg, options: [{ text: 'Step back.', next: null }] } }, focus);
  }

  updateSockets() {
    const g = this.game, s = g.structures.sockets;
    if (!s) return;
    const placed = g.quests.stage('main') >= 5 || g.quests.done('main');
    s[0].visible = placed || g.inventory.has('shard_verdance') && g.quests.stage('main') >= 4;
    s[1].visible = placed || g.inventory.has('shard_rime') && g.quests.stage('main') >= 4;
    s[2].visible = placed || g.inventory.has('shard_ember') && g.quests.stage('main') >= 4;
  }

  summonHerald() {
    const g = this.game;
    for (const sid of ['shard_verdance', 'shard_rime', 'shard_ember']) g.inventory.remove(sid);
    g.quests.state.main.stage = 5;
    g.events.emit('quest:updated', 'main');
    this.updateSockets();
    const L = LOC.heartshard;
    const e = this.spawnBoss('herald', 'herald', L.x, L.z - 6);
    e.home.set(L.x, e.position.y, L.z);
    e.taunted = 0;
    setTimeout(() => g.audio?.speak('So, the vessel is full again. Then I will empty it myself.', { ...VOICES.herald, id: 'herald' }, true), 1200);
    g.ui.banner('The Hollow Herald', 'So, the vessel is full again. Then I will empty it myself.', 0xff7af0);
    g.audio?.setMusic('final');
    g.events.emit('flash', 1);
  }

  beginEnding() {
    const g = this.game;
    g.quests.advance('main', 6);
    this.endingT = 0;
    g.structures.heartshard.restored = true;
    g.audio?.setMusic('victory');
    g.events.emit('flash', 1);
  }

  updateEnding(dt) {
    const g = this.game;
    this.endingT += dt;
    g.sky.riftStrength = Math.max(0, 1 - this.endingT / 6);
    const hs = g.structures.heartshard;
    hs.cracks.visible = this.endingT < 3;
    hs.core.scale.setScalar(1 + Math.sin(Math.min(this.endingT, 3) / 3 * Math.PI) * 0.15);
    if (this.endingT > 4 && !this.creditsShown) { this.creditsShown = true; g.ui.showCredits(); }
    if (this.endingT > 8) this.endingT = -1;
  }

  // ---------------- death
  onDeath() {
    const g = this.game;
    g.audio?.play('death');
    g.audio?.setMusic(null);
    setTimeout(() => g.ui.showDeath(), 1800);
  }

  respawn() {
    const g = this.game;
    g.ui.fade(() => {
      const lost = Math.floor(g.player.stats.gold * 0.1);
      g.player.stats.gold -= lost;
      g.player.revive();
      this.despawnBoss();
      g.spawner.despawnAll();
      if (g.inDungeon) g.dungeon.exit(true);
      const ws = g.structures.waystones.find((w) => w.id === this.lastWaystone) || g.structures.waystones[0];
      const x = ws.x + 3, z = ws.z + 2;
      g.player.teleport(x, g.env.ground(x, z, ws.y + 5).y + 0.2, z);
      g.terrain.update(x, z, true);
      if (lost) g.events.emit('toast', `You lost ${lost} gold`);
    });
  }

  // ---------------- weather
  updateWeather(dt) {
    const g = this.game;
    this.weatherT -= dt;
    const p = g.player.position;
    const reg = g.world.regionAt(p.x, p.z);
    if (this.weatherT <= 0) {
      this.weatherT = 120 + Math.random() * 180;
      const r = Math.random();
      if (r < 0.55) this.weatherTarget = { cloud: 0.25 + Math.random() * 0.3, rain: 0, storm: 0 };
      else if (r < 0.8) this.weatherTarget = { cloud: 0.7, rain: 0, storm: 0.15 };
      else this.weatherTarget = { cloud: 0.95, rain: 1, storm: 0.6 };
      if (g.forceWeather) this.weatherTarget = g.forceWeather;
    }
    const t = this.weatherTarget;
    const desert = reg === REGION.DESERT;
    const mountain = reg === REGION.MOUNTAIN && p.y > 55;
    const w = g.sky.weather;
    w.cloud = damp(w.cloud, desert ? Math.min(t.cloud, 0.3) : t.cloud, 0.3, dt);
    w.storm = damp(w.storm, desert ? 0 : t.storm, 0.3, dt);
    w.rain = damp(w.rain, desert || mountain ? 0 : t.rain, 0.3, dt);
    g.fx.weather.rain = g.inDungeon ? 0 : w.rain;
    g.fx.weather.snow = g.inDungeon ? 0 : damp(g.fx.weather.snow, mountain ? (t.rain > 0.5 ? 1 : 0.35) : 0, 0.5, dt);
    g.audio?.setRain(w.rain);
  }

  serialize() {
    return {
      discovered: [...this.discovered], waystones: [...this.waystones], lastWaystone: this.lastWaystone,
      chests: [...this.openedChests], herbs: this.herbs.map((h) => h.takenAt),
    };
  }

  restore(d) {
    this.discovered = new Set(d.discovered || []);
    this.waystones = new Set(d.waystones || []);
    this.lastWaystone = d.lastWaystone || 'brightwater';
    this.openedChests = new Set(d.chests || []);
    for (const c of this.game.structures.chests) {
      if (this.openedChests.has(c.id)) { c.opened = true; c.openT = 1; c.lid.rotation.x = -1.9; }
    }
    (d.herbs || []).forEach((t, i) => { if (this.herbs[i]) this.herbs[i].takenAt = t; });
    if (this.game.quests.done('main')) {
      this.game.sky.riftStrength = 0;
      this.game.structures.heartshard.restored = true;
      this.game.structures.heartshard.cracks.visible = false;
    }
    this.updateSockets();
  }
}

export { SPELLS, clamp, smoothstep };
