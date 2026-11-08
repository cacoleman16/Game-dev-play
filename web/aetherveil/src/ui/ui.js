// Everything on screen that isn't the 3D world: HUD, compass, notifications,
// dialogue box, menus (character, inventory, journal, map, settings, system),
// shops, waystones, title screen, character creator and touch controls.

import * as THREE from 'three';
import { ITEMS, SPELLS, SHOPS } from '../systems/content.js';
import { QUESTS } from '../systems/quests.js';
import { SKILLS, SPELL_SLOTS } from '../entities/player.js';
import { QUALITY, saveSettings } from '../core/config.js';
import { LOCATIONS } from '../world/locations.js';
import { REGION_NAMES, N, HALF } from '../world/worldgen.js';
import { formatTime, clamp, wrapAngle, el } from '../core/util.js';
import { injectIcons, icon, SPELL_ICON, ITEM_ICON } from './icons.js';

const $ = (id) => document.getElementById(id);
const v3 = new THREE.Vector3();
const TIPS = [
  'Jump, then press jump again in mid-air to glide on aether wings.',
  'Golems crumble under lightning. Frost beasts fear fire. Things of flame hate the cold.',
  'Touch a waystone to rest, save, and fast travel between the ones you have found.',
  'Hold block to soften blows. Dodge at the last moment to avoid them entirely.',
  'Skills improve as you use them: swing a sword, cast a spell, block, sprint.',
  'A red circle on the ground means something heavy is about to land there.',
  'Crouch (C) to slip past monsters unnoticed.',
  'Herbalists pay well for glowing mushrooms found in shady forests.',
];
const LOOK_OPTIONS = {
  hairStyle: ['spiky', 'short', 'bob', 'ponytail', 'long', 'twin'],
  hair: [0x2e2630, 0x6a3a1a, 0xb8542a, 0xe8c86a, 0xf0f0f4, 0x4a6ad8, 0xd86aa8, 0x3a8a5a],
  skin: [0xf6dcc6, 0xf3d2b4, 0xe0b48a, 0xc89070, 0xa86a4a, 0x7a4a32],
  eyes: [0x3a6ad8, 0x3a8a3a, 0x8a5a2a, 0xd8a83a, 0x8a3ad8, 0xd83a4a],
  top: [0x2f5fa8, 0x8a2a2a, 0x3f8f6a, 0x5a3a7a, 0x2a2a34, 0xd8d0c0],
  scarf: [0xc8453a, 0xe8c84a, 0x3a8ad8, 0xf4f4f8, 0x2a2a34, 0xd86aa8],
};

export class UI {
  constructor(game) {
    this.game = game;
    this.menuOpen = false;
    this.tab = 'character';
    this.dmg = [];
    this.feedItems = [];
    this.toastT = 0;
    this.bannerT = 0;
    this.fpsAcc = 0; this.fpsN = 0;
    this.dlgSel = 0;
    this.typing = null;
    this.invFilter = 'all';
    this.invSel = null;
    this.journalSel = null;
    this.compassEls = [];
    if (game.isTouch) document.body.classList.add('touch');
    injectIcons();
    this.bind();
  }

  bind() {
    const g = this.game, ev = g.events;
    ev.on('damage', (d) => this.damageNumber(d.pos, d.amount, d.weak ? 'weak' : d.crit ? 'crit' : d.resist ? 'resist' : '', d.weak ? 'WEAK' : d.crit ? 'CRITICAL' : d.resist ? 'RESIST' : ''));
    ev.on('player:hurt', (n) => { this.damageNumber(g.player.position.clone().setY(g.player.position.y + 2), n, 'player'); this.hurtT = 0.4; g.audio?.play('hurt'); });
    ev.on('dodged', () => this.damageNumber(g.player.position.clone().setY(g.player.position.y + 2), 'Dodge!', 'resist'));
    ev.on('toast', (t) => this.toast(t));
    ev.on('loot', (items, xp, src) => { for (const it of items) this.feed(`+ ${it}`); if (xp) this.feed(`+${xp} XP`, 'xp'); });
    ev.on('inventory:add', (id, n, silent) => { if (!silent) this.feed(`+ ${ITEMS[id].name}${n > 1 ? ' ×' + n : ''}`); });
    ev.on('levelup', (l) => { this.banner(`Level ${l}`, 'You feel stronger. Open the Character menu to choose a boon.', 0xffe27a); g.audio?.play('levelup'); g.fx.levelUp(g.player.position); });
    ev.on('skillup', (k, l) => this.feed(`${SKILLS[k].name} increased to ${l}`, 'xp'));
    ev.on('discover', (l) => this.banner(l.name, l.desc, 0x8ff0ff));
    ev.on('quest:started', (id) => this.feed(`New quest: ${QUESTS[id].name}`, 'q'));
    ev.on('quest:updated', (id) => { const i = g.quests.stageInfo(id); if (i) this.feed(i.desc, 'q'); });
    ev.on('quest:progress', (id, n, c) => this.feed(`${QUESTS[id].name}: ${n}/${c}`, 'q'));
    ev.on('quest:completed', (id, r) => { this.banner('Quest Complete', QUESTS[id].name + (r.gold ? ` · +${r.gold} gold` : ''), 0xf2d27a); });
    ev.on('interact:target', (it) => this.setPrompt(it));
    ev.on('flash', (k) => { if (!g.post?.enabled) this.flash(k); });
    ev.on('break', (e) => this.damageNumber(e.position.clone().setY(e.position.y + (e.boss ? 4 : 2.4)), 'BREAK!', 'big'));
    ev.on('parry', (e) => this.damageNumber(g.player.position.clone().setY(g.player.position.y + 2.3), 'PARRY', 'big parry'));
    ev.on('perfectDodge', () => this.damageNumber(g.player.position.clone().setY(g.player.position.y + 2.3), 'AETHER STEP', 'big parry'));
    ev.on('surge:ready', () => { this.toast(g.isTouch ? 'Aether Surge ready! Tap the glowing button' : 'Aether Surge ready! Press G'); g.audio?.play('surgeReady'); });
    ev.on('combo', (n) => { if (n >= 3) { $('combo-n').textContent = n; $('combo').classList.add('on'); this.comboT = 1.8; } });
    ev.on('saved', (slot) => slot !== 'auto' && this.toast(`Saved to slot ${slot}`));
    ev.on('input:press', (a) => this.onPress(a));
    $('death-btn').addEventListener('click', () => { $('death').classList.add('hidden'); g.story.respawn(); });
    window.addEventListener('keydown', (e) => {
      if (e.code === 'Enter' && !$('dialogue').classList.contains('hidden')) this.confirmDialogue();
    });
  }

  // ---------------------------------------------------------------- HUD
  showHUD(on) {
    $('hud').classList.toggle('hidden', !on);
    $('touch').classList.toggle('hidden', !on || !this.game.isTouch);
  }

  update(dt) {
    const g = this.game, p = g.player, st = p.stats;
    // FPS
    this.fpsAcc += dt; this.fpsN++;
    if (this.fpsAcc > 0.5) {
      if (g.settings.showFps) {
        const info = g.renderer.info.render;
        $('fps').textContent = `${Math.round(this.fpsN / this.fpsAcc)} fps · ${info.calls} draws · ${(info.triangles / 1000).toFixed(0)}k tris`;
      }
      $('fps').classList.toggle('hidden', !g.settings.showFps);
      this.fpsAcc = 0; this.fpsN = 0;
    }
    if (g.mode !== 'play') return;
    // Vitals
    const bar = (id, v, m) => { $(id + '-fill').style.width = `${clamp(v / m, 0, 1) * 100}%`; $(id + '-text').textContent = `${Math.ceil(v)} / ${m}`; };
    bar('hp', st.hp, p.maxHp); bar('mp', st.mp, p.maxMp); bar('sp', st.sp, p.maxSp);
    $('lvl-num').textContent = st.level;
    $('xp-fill').style.width = `${(st.xp / p.xpForLevel(st.level)) * 100}%`;
    const low = st.hp / p.maxHp < 0.3;
    this.hurtT = Math.max(0, (this.hurtT || 0) - dt);
    const vig = Math.max(this.hurtT * 2, low ? 0.35 + Math.sin(g.time * 5) * 0.15 : 0);
    if (!g.post?.enabled) $('vignette').style.boxShadow = `inset 0 0 ${120 + vig * 80}px rgba(200, 0, 0, ${vig * 0.8})`;
    // Clock & region
    const t = g.sky.timeOfDay;
    $('clock-time').textContent = formatTime(t);
    $('clock-region').textContent = g.inDungeon ? 'Sunscar Vault' : REGION_NAMES[g.world.regionAt(p.position.x, p.position.z)];
    this.updateSpells();
    const sg = g.player.surge;
    $('surge-fill').style.width = `${sg}%`;
    $('surge').classList.toggle('ready', sg >= 100);
    $('surge-label').textContent = sg >= 100 ? (g.isTouch ? 'AETHER SURGE · READY' : 'AETHER SURGE · PRESS G') : 'AETHER SURGE';
    $('t-surge').classList.toggle('hidden', !(sg >= 100 && g.isTouch));
    if (this.comboT > 0) { this.comboT -= dt; if (this.comboT <= 0) $('combo').classList.remove('on'); }
    this.updateCompass();
    this.updateTracker();
    this.updateTarget();
    this.updateDamageNumbers(dt);
    // Toast & banner timers.
    if (this.toastT > 0) { this.toastT -= dt; if (this.toastT <= 0) $('toast').classList.remove('on'); }
    if (this.bannerT > 0) { this.bannerT -= dt; if (this.bannerT <= 0) $('banner').style.opacity = 0; if (this.bannerT < -1) $('banner').classList.add('hidden'); }
    else if (this.bannerT > -2) { this.bannerT -= dt; if (this.bannerT < -1) $('banner').classList.add('hidden'); }
    for (const f of this.feedItems) { f.t -= dt; if (f.t < 0.6) f.el.style.opacity = Math.max(0, f.t / 0.6); }
    this.feedItems = this.feedItems.filter((f) => { if (f.t <= 0) { f.el.remove(); return false; } return true; });
    if (this.typing) this.updateTyping(dt);
  }

  updateSpells() {
    const p = this.game.player, wrap = $('spells');
    const ids = SPELL_SLOTS;
    if (!this.spellEls) {
      this.spellEls = ids.map((id, i) => {
        const s = el('div', 'slot s-' + SPELL_ICON[id], `<span class="key">${i + 1}</span>${icon(SPELL_ICON[id])}<span class="cost">${SPELLS[id].cost}</span><div class="cd"></div>`);
        s.title = SPELLS[id].name;
        s.style.pointerEvents = 'auto';
        s.addEventListener('pointerdown', (e) => { e.stopPropagation(); p.tryCast(id); });
        wrap.appendChild(s);
        return s;
      });
    }
    ids.forEach((id, i) => {
      const s = this.spellEls[i];
      const known = p.stats.spells.includes(id);
      s.classList.toggle('locked', !known);
      s.classList.toggle('sel', p.selectedSpell === id);
      const cd = p.spellCd[id] || 0;
      s.querySelector('.cd').style.height = `${(cd / SPELLS[id].cd) * 100}%`;
      s.style.opacity = known && p.stats.mp < SPELLS[id].cost ? 0.6 : '';
    });
  }

  updateCompass() {
    const g = this.game, p = g.player.position;
    const strip = $('compass-strip');
    const W = strip.clientWidth || 560;
    const heading = -g.cameraRig.yaw;
    const items = [];
    const cards = [['N', 0], ['NE', 0.25], ['E', 0.5], ['SE', 0.75], ['S', 1], ['SW', 1.25], ['W', 1.5], ['NW', 1.75]];
    for (const [n, a] of cards) items.push({ cls: n.length > 1 ? 'tick minor' : 'tick', html: n, ang: a * Math.PI });
    if (!g.inDungeon) {
      for (const m of g.quests.markers()) items.push({ cls: 'mark quest' + (m.tracked ? '' : ' dim'), html: icon('diamond'), ang: Math.atan2(m.x - p.x, -(m.z - p.z)) });
      for (const l of LOCATIONS) {
        const d = Math.hypot(l.x - p.x, l.z - p.z);
        if (d < 400 && d > l.r) items.push({ cls: 'mark loc' + (g.story.discovered.has(l.id) ? '' : ' dim'), html: icon(l.waystone ? 'crystal' : 'house'), ang: Math.atan2(l.x - p.x, -(l.z - p.z)) });
      }
    }
    for (const e of g.combat.enemies) {
      if (e.dead) continue;
      const d = e.dist2Player();
      if (d < 40 && (e.state === 'chase' || e.state === 'attack')) items.push({ cls: 'mark enemy', html: '●', ang: Math.atan2(e.position.x - p.x, -(e.position.z - p.z)) });
    }
    while (this.compassEls.length < items.length) { const e = el('div'); strip.appendChild(e); this.compassEls.push(e); }
    this.compassEls.forEach((e, i) => {
      const it = items[i];
      if (!it) { e.style.display = 'none'; return; }
      const rel = wrapAngle(it.ang - heading);
      if (Math.abs(rel) > Math.PI / 2) { e.style.display = 'none'; return; }
      e.style.display = '';
      e.className = it.cls;
      if (e._h !== it.html) { e.innerHTML = it.html; e._h = it.html; }
      e.style.left = `${W / 2 + (rel / (Math.PI / 2)) * (W / 2)}px`;
    });
  }

  updateTracker() {
    const q = this.game.quests;
    const id = q.tracked;
    const key = id ? JSON.stringify([id, q.state[id]?.stage, q.state[id]?.progress, this.game.inventory.count('shard_verdance'), this.game.inventory.count('shard_rime'), this.game.inventory.count('shard_ember')]) : '';
    if (key === this.trackerKey) return;
    this.trackerKey = key;
    const info = id ? q.stageInfo(id) : null;
    if (!info || info.done) { $('tracker').innerHTML = ''; return; }
    let html = `<div class="q-name">${info.name}</div><div class="q-desc">${info.desc}</div>`;
    if (info.objectives) for (const o of info.objectives) html += `<div class="q-obj ${o.done ? 'done' : ''}">◆ ${o.label}</div>`;
    $('tracker').innerHTML = html;
  }

  project(pos) {
    v3.copy(pos).project(this.game.camera);
    if (v3.z > 1 || v3.z < -1) return null;
    return { x: (v3.x * 0.5 + 0.5) * window.innerWidth, y: (-v3.y * 0.5 + 0.5) * window.innerHeight };
  }

  updateTarget() {
    const g = this.game, p = g.player;
    const t = p.lockTarget || p.softTarget;
    const lockEl = $('lock'), tgtEl = $('target');
    if (t && !t.dead && !t.boss && (p.lockTarget || g.time - t.lastHit < 6 || t.state === 'chase')) {
      const s = this.project(v3.copy(t.position).setY(t.position.y + (t.def.humanoid ? 2.2 * (t.model.group.scale.y || 1) : 1.8)));
      if (s) {
        tgtEl.classList.remove('hidden');
        tgtEl.style.left = s.x + 'px'; tgtEl.style.top = s.y + 'px';
        $('target-name').textContent = `${t.name}${t.level > 1 ? ' · Lv ' + t.level : ''}`;
        $('target-fill').style.width = `${(t.hp / t.maxHp) * 100}%`;
        $('target-break').style.width = `${Math.min(1, t.poise / t.poiseMax) * 100}%`;
      } else tgtEl.classList.add('hidden');
    } else tgtEl.classList.add('hidden');
    if (p.lockTarget && !p.lockTarget.dead) {
      const lt = p.lockTarget;
      const s = this.project(v3.copy(lt.position).setY(lt.position.y + (lt.boss ? 2.5 : 1.1)));
      if (s) { lockEl.classList.remove('hidden'); lockEl.style.left = s.x + 'px'; lockEl.style.top = s.y + 'px'; }
      else lockEl.classList.add('hidden');
    } else lockEl.classList.add('hidden');
  }

  damageNumber(pos, amount, cls = '', label = '') {
    const e = el('div', 'dmg ' + cls, `${label ? `<small>${label}</small>` : ''}${amount}`);
    $('dmg-layer').appendChild(e);
    this.dmg.push({ el: e, pos: pos.clone(), t: cls.includes('big') ? -0.4 : 0, dx: cls.includes('big') ? 0 : (Math.random() - 0.5) * 40 });
    if (this.dmg.length > 30) { this.dmg[0].el.remove(); this.dmg.shift(); }
  }

  updateDamageNumbers(dt) {
    for (const d of this.dmg) {
      d.t += dt;
      const s = this.project(d.pos);
      if (!s) { d.el.style.display = 'none'; continue; }
      d.el.style.display = '';
      const rise = Math.max(0, d.t) * 60;
      const sc = d.t < 0.12 ? 1.6 - d.t * 5 : 1;
      d.el.style.transform = `translate(calc(-50% + ${d.dx * d.t}px), calc(-50% - ${rise}px)) scale(${sc})`;
      d.el.style.left = s.x + 'px'; d.el.style.top = s.y + 'px';
      d.el.style.opacity = d.t > 0.7 ? Math.max(0, 1 - (d.t - 0.7) / 0.4) : 1;
    }
    this.dmg = this.dmg.filter((d) => { if (d.t > 1.1) { d.el.remove(); return false; } return true; });
  }

  feed(text, cls = '') {
    const e = el('div', 'f ' + cls, text);
    $('feed').appendChild(e);
    this.feedItems.push({ el: e, t: 5 });
    if (this.feedItems.length > 6) { const f = this.feedItems.shift(); f.el.remove(); }
  }

  toast(text) {
    const t = $('toast');
    t.textContent = text;
    t.classList.add('on');
    this.toastT = 2.4;
  }

  banner(title, sub, color = 0xffffff) {
    const b = $('banner');
    b.classList.remove('hidden');
    b.style.opacity = 1;
    b.style.animation = 'none'; void b.offsetWidth; b.style.animation = '';
    $('banner-title').textContent = title;
    $('banner-title').style.color = '#' + new THREE.Color(color).getHexString();
    $('banner-sub').textContent = sub || '';
    this.bannerT = 4.5;
  }

  setPrompt(it) {
    const pr = $('prompt');
    const g = this.game;
    if (!it || g.mode !== 'play') { pr.classList.add('hidden'); $('t-interact').classList.add('hidden'); return; }
    const key = g.isTouch ? 'Tap' : g.input.lastDevice === 'gamepad' ? 'Y' : 'E';
    pr.innerHTML = g.isTouch ? `${icon('hand')}${it.label}` : `<kbd>${key}</kbd>${it.label}`;
    pr.classList.remove('hidden');
    $('t-interact').classList.remove('hidden');
  }

  showBossBar(e) { $('boss').classList.remove('hidden'); $('boss-name').textContent = e.name; this.updateBossBar(e); }
  updateBossBar(e) { const w = `${Math.max(0, e.hp / e.maxHp) * 100}%`; $('boss-fill').style.width = w; $('boss-trail').style.width = w; $('boss-break').style.width = `${Math.min(1, e.poise / e.poiseMax) * 100}%`; }
  hideBossBar() { $('boss').classList.add('hidden'); }

  flash(k = 0.6) {
    const f = $('flash');
    f.style.transition = 'none'; f.style.opacity = k; void f.offsetWidth;
    f.style.transition = 'opacity 0.6s'; f.style.opacity = 0;
  }

  fade(mid) {
    const f = $('fade');
    f.classList.add('on');
    setTimeout(() => { mid?.(); setTimeout(() => f.classList.remove('on'), 150); }, 480);
  }

  showDeath() { this.game.input.releasePointer(); $('death').classList.remove('hidden'); }

  showCredits() {
    const g = this.game, st = g.player.stats;
    const c = el('div', 'credits', `
      <div class="logo"><div class="logo-main" style="font-size:clamp(30px,6vw,60px)">The Heartshard Sings</div></div>
      <p style="max-width:560px;color:#dfe6ff">The rift seals with a sound like a held breath finally let go. Across Lumeria, the floating isles steady, the wind turns warm, and in Brightwater someone starts ringing the bell.</p>
      <p class="gold">${g.player.name} · Level ${st.level} · ${st.kills} foes defeated · ${g.story.discovered.size}/${LOCATIONS.length} places found</p>
      <p style="color:var(--muted);font-size:13px">Aetherveil: a world, its creatures, music and story, generated entirely in code.</p>
      <button class="btn primary">Keep exploring</button>`);
    c.querySelector('button').onclick = () => { c.remove(); this.game.input.enabled = true; };
    this.game.input.enabled = false;
    this.game.input.releasePointer();
    document.body.appendChild(c);
  }

  // ---------------------------------------------------------------- dialogue
  showDialogue(speaker, text, options) {
    const d = $('dialogue');
    d.classList.remove('hidden');
    const pimg = $('dlg-portrait');
    if (speaker.portrait) { pimg.src = speaker.portrait; pimg.classList.remove('hidden'); } else pimg.classList.add('hidden');
    $('dlg-name').innerHTML = `${speaker.name}${speaker.title ? `<small>${speaker.title}</small>` : ''}`;
    this.typing = { text, i: 0, t: 0 };
    $('dlg-text').textContent = '';
    const box = $('dlg-options');
    box.innerHTML = '';
    box.style.visibility = 'hidden';
    this.dlgSel = 0;
    options.forEach((o, i) => {
      const b = el('button', 'opt', `<span class="n">${i + 1}</span><span>${o}</span>`);
      b.addEventListener('click', () => { if (this.typing) this.finishTyping(); else this.game.dialogue.choose(i); });
      b.addEventListener('pointerenter', () => { this.dlgSel = i; this.highlightOption(); });
      box.appendChild(b);
    });
    this.highlightOption();
    $('hud').classList.add('hidden');
    $('touch').classList.add('hidden');
  }

  updateTyping(dt) {
    const ty = this.typing;
    ty.t += dt * 70;
    const n = Math.min(ty.text.length, Math.floor(ty.t));
    if (n !== ty.i) { ty.i = n; $('dlg-text').textContent = ty.text.slice(0, n); }
    if (n >= ty.text.length) this.finishTyping();
  }

  finishTyping() {
    if (!this.typing) return;
    $('dlg-text').textContent = this.typing.text;
    this.typing = null;
    $('dlg-options').style.visibility = 'visible';
  }

  highlightOption() {
    [...$('dlg-options').children].forEach((c, i) => c.classList.toggle('sel', i === this.dlgSel));
  }

  confirmDialogue() {
    if (this.typing) { this.finishTyping(); return; }
    this.game.dialogue.choose(this.dlgSel);
  }

  hideDialogue() {
    $('dialogue').classList.add('hidden');
    this.typing = null;
    if (this.game.mode === 'play' && !this.menuOpen) this.showHUD(true);
  }

  onPress(a) {
    const g = this.game;
    if (g.mode === 'title' || g.mode === 'creator') return;
    if (g.dialogue.active) {
      const n = $('dlg-options').children.length;
      if (a === 'up' || a === 'spellPrev') { this.dlgSel = (this.dlgSel + n - 1) % n; this.highlightOption(); g.audio?.play('hover'); }
      if (a === 'down' || a === 'spellNext') { this.dlgSel = (this.dlgSel + 1) % n; this.highlightOption(); g.audio?.play('hover'); }
      if (a === 'interact' || a === 'jump' || a === 'attack') this.confirmDialogue();
      const m = /^spell(\d)$/.exec(a);
      if (m && !this.typing) g.dialogue.choose(Number(m[1]) - 1);
      if (a === 'menu') g.dialogue.end();
      return;
    }
    if (this.menuOpen) {
      if (a === 'menu') this.closeMenu();
      if (a === 'inventory') this.tab === 'inventory' ? this.closeMenu() : this.openMenu('inventory');
      if (a === 'journal') this.tab === 'journal' ? this.closeMenu() : this.openMenu('journal');
      if (a === 'map') this.tab === 'map' ? this.closeMenu() : this.openMenu('map');
      return;
    }
    if (g.mode !== 'play' || g.player.dead) return;
    if (a === 'menu') this.openMenu('character');
    if (a === 'inventory') this.openMenu('inventory');
    if (a === 'journal') this.openMenu('journal');
    if (a === 'map') this.openMenu('map');
    if (a === 'help') this.openMenu('controls');
  }

  // ---------------------------------------------------------------- menus
  openMenu(tab = 'character', extra = null) {
    const g = this.game;
    this.menuOpen = true;
    this.tab = tab;
    this.extra = extra;
    g.input.enabled = false;
    g.input.releasePointer();
    g.paused = true;
    g.audio?.play('open');
    this.showHUD(false);
    this.renderMenu();
  }

  closeMenu() {
    const g = this.game;
    this.menuOpen = false;
    $('menu').classList.add('hidden');
    $('menu').innerHTML = '';
    g.paused = false;
    g.audio?.play('close');
    if (g.mode === 'play') { g.input.enabled = !g.dialogue.active; this.showHUD(true); }
    if (g.mode === 'title') this.showTitle();
  }

  renderMenu() {
    const g = this.game;
    const m = $('menu');
    m.classList.remove('hidden');
    m.innerHTML = '';
    const box = el('div', 'window menu-box');
    const tabs = el('div', 'tabs');
    const list = g.mode === 'play'
      ? [['character', 'Character'], ['inventory', 'Inventory'], ['journal', 'Journal'], ['map', 'Map'], ['settings', 'Settings'], ['system', 'System'], ['controls', 'Controls']]
      : [['settings', 'Settings'], ['controls', 'Controls']];
    if (this.tab === 'shop' || this.tab === 'waystone') list.unshift([this.tab, this.tab === 'shop' ? this.extra.def.name : 'Waystone']);
    for (const [id, name] of list) {
      const t = el('button', 'tab' + (this.tab === id ? ' sel' : ''), name);
      t.onclick = () => { this.tab = id; g.audio?.play('click'); this.renderMenu(); };
      tabs.appendChild(t);
    }
    const close = el('button', 'tab tab-close', '✕ Close');
    close.onclick = () => this.closeMenu();
    tabs.appendChild(close);
    const body = el('div', 'menu-body');
    box.append(tabs, body);
    m.appendChild(box);
    m.onclick = (e) => { if (e.target === m) this.closeMenu(); };
    const r = { character: this.renderCharacter, inventory: this.renderInventory, journal: this.renderJournal, map: this.renderMap, settings: this.renderSettings, system: this.renderSystem, controls: this.renderControls, shop: this.renderShop, waystone: this.renderWaystone }[this.tab];
    r.call(this, body);
  }

  renderCharacter(body) {
    const g = this.game, p = g.player, st = p.stats;
    const wpn = g.inventory.equipped.weapon;
    body.innerHTML = `
      <div class="cols">
        <div class="detail">
          <h3>${p.name}</h3>
          <div class="meta">Level ${st.level} Wayfarer · ${st.xp} / ${p.xpForLevel(st.level)} XP · <span class="gold">${st.gold} gold</span></div>
          <div class="statgrid">
            <span>Health</span><span></span><b>${Math.ceil(st.hp)} / ${p.maxHp}</b>
            <span>Mana</span><span></span><b>${Math.ceil(st.mp)} / ${p.maxMp}</b>
            <span>Stamina</span><span></span><b>${Math.ceil(st.sp)} / ${p.maxSp}</b>
            <span>Weapon damage</span><span></span><b>${Math.round(p.meleeDamage())}${wpn && ITEMS[wpn].element ? ' + ' + ITEMS[wpn].element : ''}</b>
            <span>Armour</span><span></span><b>${p.armor}</b>
            <span>Foes defeated</span><span></span><b>${st.kills}</b>
          </div>
          <div id="perkbox"></div>
          <h3 style="margin-top:16px;font-size:16px">Spells</h3>
          <div class="list">${SPELL_SLOTS.filter((id) => st.spells.includes(id)).map((id) => `<div class="li"><span>${icon(SPELL_ICON[id])}${SPELLS[id].name} <span class="cnt">[${SPELL_SLOTS.indexOf(id) + 1}]</span></span><span class="cnt">${SPELLS[id].cost} MP</span></div>`).join('')}</div>
        </div>
        <div class="detail">
          <h3 style="font-size:16px">Skills</h3>
          <div class="meta">Skills improve as you use them.</div>
          <div class="statgrid">${Object.entries(SKILLS).map(([k, s]) => {
            const sk = st.skills[k];
            const need = 12 + sk.lvl * 8;
            return `<span title="${s.desc}">${s.name}</span><div class="sk-bar"><div style="width:${(sk.xp / need) * 100}%"></div></div><b>${sk.lvl}</b>`;
          }).join('')}</div>
          <p class="meta" style="margin-top:12px">${Object.values(SKILLS).map((s) => `<b>${s.name}</b>: ${s.desc}`).join('<br>')}</p>
        </div>
      </div>`;
    if (st.perks > 0) {
      const pb = body.querySelector('#perkbox');
      pb.className = 'perk';
      pb.innerHTML = `<b class="gold">Level up!</b> Choose a boon (${st.perks} left):<div class="row" style="margin-top:8px"></div>`;
      const row = pb.querySelector('.row');
      for (const [k, label] of [['hp', '+15 Health'], ['mp', '+12 Mana'], ['sp', '+12 Stamina']]) {
        const b = el('button', 'btn', label);
        b.onclick = () => { p.spendPerk(k); g.audio?.play('levelup'); this.renderMenu(); };
        row.appendChild(b);
      }
    }
  }

  renderInventory(body) {
    const g = this.game, inv = g.inventory;
    const cats = [['all', 'All'], ['weapon', 'Weapons'], ['armor', 'Apparel'], ['potion', 'Consumables'], ['material', 'Materials'], ['quest', 'Quest']];
    const wrap = el('div', 'cols');
    const left = el('div');
    const filters = el('div', 'filters');
    for (const [id, name] of cats) {
      const c = el('button', 'chip' + (this.invFilter === id ? ' sel' : ''), name);
      c.onclick = () => { this.invFilter = id; this.renderMenu(); };
      filters.appendChild(c);
    }
    left.appendChild(filters);
    left.appendChild(el('div', 'meta', `<span class="gold">${g.player.stats.gold} gold</span>`));
    const list = el('div', 'list');
    const ids = Object.keys(inv.items).filter((id) => {
      const t = ITEMS[id].type;
      return this.invFilter === 'all' || t === this.invFilter || (this.invFilter === 'armor' && t === 'ring');
    }).sort((a, b) => ITEMS[a].type.localeCompare(ITEMS[b].type) || ITEMS[a].name.localeCompare(ITEMS[b].name));
    if (!ids.length) list.appendChild(el('div', 'meta', 'Nothing here.'));
    for (const id of ids) {
      const eq = Object.values(inv.equipped).includes(id);
      const li = el('div', 'li' + (this.invSel === id ? ' sel' : ''), `<span>${icon(ITEM_ICON[ITEMS[id].type] || 'leaf')}${ITEMS[id].name} ${eq ? '<span class="eq">EQUIPPED</span>' : ''}</span><span class="cnt">×${inv.count(id)}</span>`);
      li.onclick = () => { this.invSel = id; this.renderMenu(); };
      li.ondblclick = () => { inv.use(id); this.renderMenu(); };
      list.appendChild(li);
    }
    left.appendChild(list);
    const right = el('div', 'detail');
    const id = this.invSel && inv.has(this.invSel) ? this.invSel : null;
    if (id) {
      const it = ITEMS[id];
      const stats = [it.dmg && `Damage ${it.dmg}`, it.element && `Element: ${it.element}`, it.armor && `Armour ${it.armor}`, it.hp && `+${it.hp} Health`, it.mp && `+${it.mp} Mana`, it.sp && `+${it.sp} Stamina`, it.heal && `Heals ${it.heal}`, `Value ${it.value}`].filter(Boolean).join(' · ');
      right.innerHTML = `<h3>${it.name}</h3><div class="meta">${it.type}</div><p>${it.desc}</p><p class="meta">${stats}</p>`;
      const row = el('div', 'row');
      const slot = inv.slotFor(id);
      if (slot) {
        const eq = inv.equipped[slot] === id;
        const b = el('button', 'btn primary', eq ? 'Unequip' : 'Equip');
        b.onclick = () => { inv.use(id); this.renderMenu(); };
        row.appendChild(b);
      } else if (it.type === 'potion') {
        const b = el('button', 'btn primary', 'Use');
        b.onclick = () => { inv.use(id); this.renderMenu(); };
        row.appendChild(b);
      }
      right.appendChild(row);
    } else right.innerHTML = '<p class="meta">Select an item.</p>';
    wrap.append(left, right);
    body.appendChild(wrap);
  }

  renderJournal(body) {
    const g = this.game, q = g.quests;
    const ids = Object.keys(q.state).sort((a, b) => (q.done(a) - q.done(b)) || (QUESTS[b].main ? 1 : 0) - (QUESTS[a].main ? 1 : 0));
    const wrap = el('div', 'cols');
    const list = el('div', 'list');
    if (!ids.length) list.appendChild(el('div', 'meta', 'No quests yet. Speak with the people of Brightwater.'));
    const sel = this.journalSel && q.state[this.journalSel] ? this.journalSel : ids[0];
    for (const id of ids) {
      const li = el('div', 'li' + (id === sel ? ' sel' : '') + (q.done(id) ? ' q-done' : ''), `<span>${QUESTS[id].main ? '★ ' : ''}${QUESTS[id].name}</span><span class="cnt">${q.done(id) ? 'Done' : id === q.tracked ? 'Tracked' : ''}</span>`);
      li.onclick = () => { this.journalSel = id; this.renderMenu(); };
      list.appendChild(li);
    }
    const right = el('div', 'detail');
    if (sel) {
      const info = q.stageInfo(sel);
      right.innerHTML = `<h3>${info.name}</h3><div class="meta">${info.done ? 'Completed' : 'In progress'}</div><p>${info.done ? 'This quest is complete.' : info.desc}</p>${info.objectives && !info.done ? info.objectives.map((o) => `<div class="q-obj ${o.done ? 'done' : ''}" style="${o.done ? 'color:#7fd8a0;text-decoration:line-through' : ''}">◆ ${o.label}</div>`).join('') : ''}`;
      if (!info.done && q.tracked !== sel) {
        const b = el('button', 'btn', 'Track this quest');
        b.style.marginTop = '12px';
        b.onclick = () => { q.tracked = sel; this.trackerKey = null; this.renderMenu(); };
        right.appendChild(b);
      }
    }
    wrap.append(list, right);
    body.appendChild(wrap);
  }

  buildMapImage() {
    if (this.mapBase) return this.mapBase;
    const w = this.game.world;
    const S = 512;
    const c = document.createElement('canvas');
    c.width = c.height = S;
    const ctx = c.getContext('2d');
    const img = ctx.createImageData(S, S);
    for (let y = 0; y < S; y++) {
      for (let x = 0; x < S; x++) {
        const gx = Math.round((x / (S - 1)) * (N - 1)), gz = Math.round((y / (S - 1)) * (N - 1));
        const i = gz * N + gx;
        let r = w.color[i * 3], gg = w.color[i * 3 + 1], b = w.color[i * 3 + 2];
        const h = w.H[i];
        const hx = w.H[Math.min(i + 2, N * N - 1)] - w.H[Math.max(i - 2, 0)];
        const hz = w.H[Math.min(i + 2 * N, N * N - 1)] - w.H[Math.max(i - 2 * N, 0)];
        let sh = 1 + (-hx - hz) * 0.035; sh = clamp(sh, 0.6, 1.35);
        if (h < 0) { const d = Math.min(1, -h / 14); r = 70 - 40 * d; gg = 150 - 60 * d; b = 190 - 40 * d; sh = 1; }
        // Parchment tint.
        const o = (y * S + x) * 4;
        img.data[o] = clamp(r * sh * 0.85 + 30, 0, 255);
        img.data[o + 1] = clamp(gg * sh * 0.85 + 22, 0, 255);
        img.data[o + 2] = clamp(b * sh * 0.8 + 10, 0, 255);
        img.data[o + 3] = 255;
      }
    }
    ctx.putImageData(img, 0, 0);
    this.mapBase = c;
    return c;
  }

  renderMap(body) {
    const g = this.game;
    const wrap = el('div');
    wrap.style.height = '100%';
    wrap.style.display = 'flex'; wrap.style.flexDirection = 'column';
    const mw = el('div'); mw.id = 'map-wrap';
    const cv = document.createElement('canvas');
    cv.id = 'map-canvas';
    cv.width = cv.height = 1024;
    mw.appendChild(cv);
    wrap.appendChild(mw);
    const canTravel = !g.inDungeon && !g.combat.enemies.some((e) => !e.dead && e.state === 'chase' && e.dist2Player() < 40);
    wrap.appendChild(el('div', 'map-legend', `◆ quest &nbsp; ⌂ place &nbsp; ✦ waystone ${canTravel ? '(click a found waystone to fast travel)' : '(cannot fast travel right now)'}`));
    body.appendChild(wrap);
    const ctx = cv.getContext('2d');
    const S = 1024;
    const toMap = (x, z) => [((x + HALF) / (HALF * 2)) * S, ((z + HALF) / (HALF * 2)) * S];
    ctx.imageSmoothingEnabled = true;
    ctx.drawImage(this.buildMapImage(), 0, 0, S, S);
    ctx.textAlign = 'center';
    // Roads hint & labels.
    for (const l of LOCATIONS) {
      const found = g.story.discovered.has(l.id);
      const [x, y] = toMap(l.x, l.z);
      ctx.fillStyle = found ? '#fff6dc' : 'rgba(255,255,255,0.35)';
      ctx.font = `${found ? 700 : 600} ${found ? 24 : 20}px Cinzel, serif`;
      ctx.lineWidth = 4; ctx.strokeStyle = 'rgba(0,0,0,0.6)';
      const label = found ? l.name : '???';
      ctx.strokeText(label, x, y - 16); ctx.fillText(label, x, y - 16);
      ctx.beginPath(); ctx.arc(x, y, found ? 7 : 5, 0, Math.PI * 2); ctx.fill();
    }
    for (const ws of g.structures.waystones) {
      if (!g.story.waystones.has(ws.id)) continue;
      const [x, y] = toMap(ws.x, ws.z);
      ctx.fillStyle = '#8ff0ff'; ctx.font = '30px serif'; ctx.strokeText('✦', x + 16, y + 10); ctx.fillText('✦', x + 16, y + 10);
    }
    for (const m of g.quests.markers()) {
      const [x, y] = toMap(m.x, m.z);
      ctx.fillStyle = m.tracked ? '#f2d27a' : '#d8c890'; ctx.font = '30px serif';
      ctx.strokeText('◆', x, y + 10); ctx.fillText('◆', x, y + 10);
    }
    // Player arrow.
    const pp = g.inDungeon ? g.structures.points['vault.door'] : g.player.position;
    const [px, py] = toMap(pp.x, pp.z);
    ctx.save(); ctx.translate(px, py); ctx.rotate(-g.player.yaw + Math.PI);
    ctx.fillStyle = '#ff4a3a'; ctx.strokeStyle = '#fff'; ctx.lineWidth = 3;
    ctx.beginPath(); ctx.moveTo(0, -18); ctx.lineTo(12, 14); ctx.lineTo(0, 7); ctx.lineTo(-12, 14); ctx.closePath(); ctx.fill(); ctx.stroke();
    ctx.restore();
    cv.onclick = (e) => {
      if (!canTravel) return;
      const r = cv.getBoundingClientRect();
      const mx = ((e.clientX - r.left) / r.width) * S, my = ((e.clientY - r.top) / r.height) * S;
      for (const ws of g.structures.waystones) {
        if (!g.story.waystones.has(ws.id)) continue;
        const [x, y] = toMap(ws.x, ws.z);
        if (Math.hypot(mx - x, my - y) < 36) {
          this.closeMenu();
          g.story.travel(ws.id);
          return;
        }
      }
    };
  }

  renderSettings(body) {
    const g = this.game, s = g.settings;
    const row = (label, ctrl) => { const r = el('div', 'setting'); r.appendChild(el('label', '', label)); r.appendChild(ctrl); body.appendChild(r); return ctrl; };
    const sel = document.createElement('select');
    for (const [k, q] of Object.entries(QUALITY)) { const o = document.createElement('option'); o.value = k; o.textContent = q.label; if (k === s.quality) o.selected = true; sel.appendChild(o); }
    row('Graphics quality', sel);
    const apply = el('button', 'btn', 'Apply quality (reloads)');
    apply.onclick = () => { s.quality = sel.value; saveSettings(s); g.save?.autosave(); location.reload(); };
    body.appendChild(apply);
    const slider = (label, key, min, max, step, fn) => {
      const i = document.createElement('input'); i.type = 'range'; i.min = min; i.max = max; i.step = step; i.value = s[key];
      i.oninput = () => { s[key] = Number(i.value); saveSettings(s); fn?.(); };
      row(label, i);
    };
    slider('Master volume', 'masterVolume', 0, 1, 0.05, () => g.audio?.applyVolumes());
    slider('Music volume', 'musicVolume', 0, 1, 0.05, () => g.audio?.applyVolumes());
    slider('Effects volume', 'sfxVolume', 0, 1, 0.05, () => g.audio?.applyVolumes());
    slider('Voice volume', 'voiceVolume', 0, 1, 0.05, () => g.audio?.applyVolumes());
    {
      const vs = document.createElement('select');
      for (const [k, label] of [['speech', 'Spoken (browser voices)'], ['babble', 'Stylised babble'], ['off', 'Off']]) { const o = document.createElement('option'); o.value = k; o.textContent = label; if ((s.voiceMode ?? 'speech') === k) o.selected = true; vs.appendChild(o); }
      vs.onchange = () => { s.voiceMode = vs.value; saveSettings(s); g.audio?.speak('Hello, Wayfarer.', { g: 'f', pitch: 1, rate: 1, f0: 210, id: 'test' }); };
      row('Character voices', vs);
    }
    slider('Look sensitivity', 'mouseSensitivity', 0.3, 2.5, 0.1);
    slider('Day length (minutes)', 'dayLengthMinutes', 6, 60, 1, () => { g.sky.dayLength = s.dayLengthMinutes * 60; });
    const check = (label, key) => {
      const i = document.createElement('input'); i.type = 'checkbox'; i.checked = !!s[key];
      i.onchange = () => { s[key] = i.checked; saveSettings(s); };
      row(label, i);
    };
    check('Invert look Y', 'invertY');
    check('Camera shake', 'cameraShake');
    check('Show FPS', 'showFps');
  }

  renderSystem(body) {
    const g = this.game;
    body.appendChild(el('p', 'meta', 'The game also autosaves when you discover places, finish quests and rest at waystones.'));
    for (const { slot, data } of g.save.list()) {
      const r = el('div', 'li');
      const when = data ? new Date(data.at).toLocaleString() : 'Empty';
      r.innerHTML = `<span><b>${slot === 'auto' ? 'Autosave' : 'Slot ' + slot}</b> <span class="cnt">${data ? `${data.player.name} · Lv ${data.player.stats.level} · ` : ''}${when}</span></span>`;
      const btns = el('span', 'row');
      if (slot !== 'auto') {
        const sb = el('button', 'btn', 'Save');
        sb.onclick = () => { g.save.save(slot); this.renderMenu(); };
        btns.appendChild(sb);
      }
      if (data) {
        const lb = el('button', 'btn', 'Load');
        lb.onclick = () => { sessionStorage.setItem('aetherveil.loadSlot', slot); location.reload(); };
        btns.appendChild(lb);
      }
      r.appendChild(btns);
      body.appendChild(r);
    }
    const q = el('button', 'btn danger', 'Quit to title');
    q.style.marginTop = '16px';
    q.onclick = () => { g.save.autosave(); location.reload(); };
    body.appendChild(q);
  }

  renderControls(body) {
    body.innerHTML = `
      <div class="cols">
        <div><h3 class="gold" style="font-family:var(--font-title)">Keyboard & Mouse</h3><div class="keys">
          <kbd>WASD</kbd><span>Move</span><kbd>Mouse</kbd><span>Look (click the game to capture the mouse)</span>
          <kbd>Shift</kbd><span>Sprint</span><kbd>Space</kbd><span>Jump · press again in the air to glide</span>
          <kbd>Left click</kbd><span>Attack: click for a 3-hit combo, hold after a swing to charge a heavy blow</span><kbd>Right click / X</kbd><span>Block. Tap just before a hit lands to parry</span>
          <kbd>G</kbd><span>Aether Surge, when the meter is full</span><kbd>Sprint + attack</kbd><span>Spinning slash</span><kbd>Attack in mid-air</kbd><span>Plunging strike</span>
          <kbd>Q</kbd><span>Dodge roll</span><kbd>1 - 4</kbd><span>Cast spells</span><kbd>F</kbd><span>Cast selected spell</span>
          <kbd>E</kbd><span>Interact / talk</span><kbd>Tab / Middle click</kbd><span>Lock on to target</span>
          <kbd>Z</kbd><span>Quick potion</span><kbd>R</kbd><span>Draw / sheathe weapon</span><kbd>C</kbd><span>Crouch (sneak)</span>
          <kbd>V</kbd><span>First / third person</span><kbd>Wheel</kbd><span>Zoom camera</span>
          <kbd>I · J · M</kbd><span>Inventory · Journal · Map</span><kbd>Esc</kbd><span>Menu</span>
        </div></div>
        <div><h3 class="gold" style="font-family:var(--font-title)">Gamepad</h3><div class="keys">
          <kbd>L stick</kbd><span>Move</span><kbd>R stick</kbd><span>Look</span><kbd>A</kbd><span>Jump / glide</span><kbd>X</kbd><span>Attack</span>
          <kbd>B</kbd><span>Dodge</span><kbd>Y</kbd><span>Interact</span><kbd>LB</kbd><span>Block / parry</span><kbd>RB</kbd><span>Cast spell</span><kbd>L3</kbd><span>Aether Surge</span>
          <kbd>D-pad</kbd><span>Choose spell</span><kbd>LT</kbd><span>Potion</span><kbd>RT</kbd><span>Sprint</span><kbd>R3</kbd><span>Lock on</span>
          <kbd>Start</kbd><span>Menu</span><kbd>Back</kbd><span>Map</span>
        </div>
        <h3 class="gold" style="font-family:var(--font-title);margin-top:16px">Touch</h3>
        <p class="meta">Left side: move stick. Right side: drag to look. Buttons: attack, jump/glide, dodge, block (hold), spell, cycle spell, sprint toggle, potion, lock-on. Add the page to your home screen for full-screen play.</p>
        </div>
      </div>`;
  }

  // Head-and-shoulders portrait. The character is lifted into a small studio
  // scene (painted backdrop, key, fill and rim lights) for one render, so
  // walls and props near where they stand can never block the face.
  portrait(ch, height = 1, opts = {}) {
    const g = this.game, r = g.renderer;
    const S = opts.size || 192;
    if (!this.pScene) {
      const sc = new THREE.Scene();
      const bg = document.createElement('canvas'); bg.width = bg.height = 256;
      const c = bg.getContext('2d');
      const lin = c.createLinearGradient(0, 0, 0, 256);
      lin.addColorStop(0, '#4a5a82'); lin.addColorStop(0.55, '#262d47'); lin.addColorStop(1, '#141726');
      c.fillStyle = lin; c.fillRect(0, 0, 256, 256);
      const rad = c.createRadialGradient(150, 92, 8, 150, 92, 150);
      rad.addColorStop(0, 'rgba(255,226,170,0.42)'); rad.addColorStop(1, 'rgba(255,226,170,0)');
      c.fillStyle = rad; c.fillRect(0, 0, 256, 256);
      const tex = new THREE.CanvasTexture(bg); tex.colorSpace = THREE.SRGBColorSpace;
      sc.background = tex;
      sc.add(new THREE.HemisphereLight(0xd8e2ff, 0x4a3a30, 1.15));
      const key = new THREE.DirectionalLight(0xfff0da, 2.3); key.position.set(2.2, 3.2, 3.0);
      const rim = new THREE.DirectionalLight(0x9cc2ff, 1.9); rim.position.set(-2.6, 2.0, -2.4);
      sc.add(key, rim, new THREE.AmbientLight(0xffffff, 0.1));
      this.pScene = sc;
      this.pCam = new THREE.PerspectiveCamera(24, 1, 0.05, 50);
    }
    if (!this.pRT || this.pRT.width !== S) {
      this.pRT?.dispose();
      this.pRT = new THREE.WebGLRenderTarget(S, S, { depthBuffer: true, samples: 4 });
      this.pBuf = new Uint8Array(S * S * 4);
      this.pCanvas = document.createElement('canvas');
      this.pCanvas.width = this.pCanvas.height = S;
    }
    const grp = ch.group, parent = grp.parent;
    const saved = { p: grp.position.clone(), ry: grp.rotation.y, vis: grp.visible };
    this.pScene.add(grp);
    grp.position.set(0, 0, 0); grp.rotation.y = 0; grp.visible = true;
    const fullBody = !!opts.full;
    const yaw = opts.yaw ?? 0.32;
    const tY = (fullBody ? 0.95 : 1.58) * height, dist = (fullBody ? 6.2 : 1.35) * height;
    this.pCam.position.set(Math.sin(yaw) * dist, tY + 0.05 * height, Math.cos(yaw) * dist);
    this.pCam.lookAt(0, tY - 0.04 * height, 0);
    const prev = r.getRenderTarget();
    r.setRenderTarget(this.pRT);
    r.clear();
    r.render(this.pScene, this.pCam);
    r.readRenderTargetPixels(this.pRT, 0, 0, S, S, this.pBuf);
    r.setRenderTarget(prev);
    if (parent) parent.add(grp); else this.pScene.remove(grp);
    grp.position.copy(saved.p); grp.rotation.y = saved.ry; grp.visible = saved.vis;
    const ctx = this.pCanvas.getContext('2d');
    const img = ctx.createImageData(S, S);
    const lut = this.pLut || (this.pLut = Array.from({ length: 256 }, (_, i) => { const x = i / 255; const t = x * 1.08 / (1 + x * 0.25); return Math.round(Math.pow(Math.min(1, t), 1 / 2.2) * 255); }));
    for (let y = 0; y < S; y++) {
      for (let x = 0; x < S; x++) {
        const si = ((S - 1 - y) * S + x) * 4, di = (y * S + x) * 4;
        img.data[di] = lut[this.pBuf[si]]; img.data[di + 1] = lut[this.pBuf[si + 1]]; img.data[di + 2] = lut[this.pBuf[si + 2]]; img.data[di + 3] = 255;
      }
    }
    ctx.putImageData(img, 0, 0);
    return this.pCanvas.toDataURL('image/jpeg', 0.88);
  }

  // ---------------------------------------------------------------- shops & waystones
  openShop(npc) { this.openMenu('shop', npc); }

  renderShop(body) {
    const g = this.game, inv = g.inventory, npc = this.extra;
    const stock = SHOPS[npc.def.shop] || [];
    const wrap = el('div', 'cols');
    const left = el('div');
    left.appendChild(el('h3', 'gold', `${npc.def.name}'s wares`));
    const ll = el('div', 'list');
    for (const id of stock) {
      const price = inv.buyPrice(id);
      const li = el('div', 'li', `<span>${ITEMS[id].name}<br><span class="cnt" style="font-size:12px">${ITEMS[id].desc}</span></span><span class="gold">${price}g</span>`);
      li.onclick = () => { if (inv.buy(id)) this.toast(`Bought ${ITEMS[id].name}`); else this.toast('Not enough gold'); this.renderMenu(); };
      ll.appendChild(li);
    }
    left.appendChild(ll);
    const right = el('div');
    right.appendChild(el('h3', '', `Sell · <span class="gold">${g.player.stats.gold} gold</span>`));
    const rl = el('div', 'list');
    for (const id of Object.keys(inv.items)) {
      if (ITEMS[id].type === 'quest') continue;
      const li = el('div', 'li', `<span>${ITEMS[id].name} <span class="cnt">×${inv.count(id)}</span></span><span class="gold">${inv.sellPrice(id)}g</span>`);
      li.onclick = () => { if (!inv.sell(id)) this.toast("Can't sell an equipped item"); this.renderMenu(); };
      rl.appendChild(li);
    }
    right.appendChild(rl);
    wrap.append(left, right);
    body.appendChild(wrap);
  }

  openWaystone(ws) { this.openMenu('waystone', ws); }

  renderWaystone(body) {
    const g = this.game, ws = this.extra;
    body.appendChild(el('h3', 'gold', ws.name));
    body.appendChild(el('p', 'meta', 'The crystal hums. Its light washes your weariness away.'));
    const row = el('div', 'row');
    const rest = el('button', 'btn primary', 'Rest & Save');
    rest.onclick = () => { g.story.rest(); this.closeMenu(); };
    const morning = el('button', 'btn', 'Wait until morning');
    morning.onclick = () => { const h = (6.5 - g.sky.timeOfDay + 24) % 24; this.closeMenu(); g.story.waitHours(h); };
    const night = el('button', 'btn', 'Wait until night');
    night.onclick = () => { const h = (20.5 - g.sky.timeOfDay + 24) % 24; this.closeMenu(); g.story.waitHours(h); };
    row.append(rest, morning, night);
    body.appendChild(row);
    body.appendChild(el('h3', '', 'Travel to'));
    const list = el('div', 'list');
    for (const w of g.structures.waystones) {
      if (!g.story.waystones.has(w.id) || w.id === ws.id) continue;
      const li = el('div', 'li', `<span>✦ ${w.name}</span><span class="cnt">${Math.round(Math.hypot(w.x - ws.x, w.z - ws.z))} m</span>`);
      li.onclick = () => { this.closeMenu(); g.story.travel(w.id); };
      list.appendChild(li);
    }
    if (!list.children.length) list.appendChild(el('div', 'meta', 'Touch other waystones to travel between them.'));
    body.appendChild(list);
  }

  // ---------------------------------------------------------------- title & creator
  showTitle() {
    const g = this.game;
    $('title').classList.remove('hidden');
    $('title-q').textContent = `${QUALITY[g.settings.quality].label} quality`;
    const menu = $('title-menu');
    menu.innerHTML = '';
    const latest = g.save.latest();
    const add = (label, fn, disabled = false) => {
      const b = el('button', 'item', label);
      b.disabled = disabled;
      b.onclick = () => { g.audio.init(); g.audio.play('click'); fn(); };
      b.onpointerenter = () => g.audio?.play('hover');
      menu.appendChild(b);
    };
    if (latest) add(`Continue · ${latest.data.player.name}, Lv ${latest.data.player.stats.level}`, () => g.startFromSave(latest.data));
    add('New Journey', () => this.showCreator());
    add('Settings', () => { $('title').classList.add('hidden'); this.openMenu('settings'); });
    add('Controls', () => { $('title').classList.add('hidden'); this.openMenu('controls'); });
    add('Fullscreen', () => { document.documentElement.requestFullscreen?.().catch(() => {}); });
  }

  hideTitle() { $('title').classList.add('hidden'); }

  showCreator() {
    const g = this.game;
    this.hideTitle();
    g.mode = 'creator';
    g.beginCreator();
    $('creator').classList.remove('hidden');
    const look = g.player.look;
    const body = $('creator-body');
    const render = () => {
      body.innerHTML = '';
      const nameF = el('div', 'field', '<label>Name</label>');
      const inp = document.createElement('input');
      inp.type = 'text'; inp.maxLength = 16; inp.value = g.player.name;
      inp.oninput = () => { g.player.name = inp.value.trim() || 'Wayfarer'; };
      nameF.appendChild(inp);
      body.appendChild(nameF);
      const chips = (label, key, opts, names) => {
        const f = el('div', 'field', `<label>${label}</label>`);
        const c = el('div', 'chips');
        opts.forEach((o, i) => {
          const b = el('button', 'chip' + (look[key] === o ? ' sel' : ''), names ? names[i] : o);
          b.onclick = () => { look[key] = o; g.player.buildCharacter(look); render(); };
          c.appendChild(b);
        });
        f.appendChild(c); body.appendChild(f);
      };
      const swatches = (label, key, opts) => {
        const f = el('div', 'field', `<label>${label}</label>`);
        const c = el('div', 'swatches');
        for (const o of opts) {
          const b = el('button', 'swatch' + (look[key] === o ? ' sel' : ''));
          b.style.background = '#' + o.toString(16).padStart(6, '0');
          b.onclick = () => { look[key] = o; g.player.buildCharacter(look); render(); };
          c.appendChild(b);
        }
        f.appendChild(c); body.appendChild(f);
      };
      chips('Frame', 'female', [false, true], ['Broad', 'Slender']);
      chips('Hair', 'hairStyle', LOOK_OPTIONS.hairStyle, ['Spiky', 'Short', 'Bob', 'Ponytail', 'Long', 'Twin tails']);
      swatches('Hair colour', 'hair', LOOK_OPTIONS.hair);
      swatches('Skin', 'skin', LOOK_OPTIONS.skin);
      swatches('Eyes', 'eyes', LOOK_OPTIONS.eyes);
      swatches('Tunic', 'top', LOOK_OPTIONS.top);
      swatches('Scarf', 'scarf', LOOK_OPTIONS.scarf);
    };
    render();
    $('cr-back').onclick = () => { $('creator').classList.add('hidden'); g.mode = 'title'; g.endCreator(); this.showTitle(); };
    $('cr-go').onclick = () => { $('creator').classList.add('hidden'); g.audio.init(); g.startNewGame(); };
  }

  // ---------------------------------------------------------------- touch controls
  setupTouch() {
    const g = this.game, inp = g.input;
    const zone = $('stick-zone'), base = $('stick-base'), knob = $('stick-knob');
    let stickId = null, ox = 0, oy = 0;
    const R = 50;
    zone.addEventListener('pointerdown', (e) => {
      stickId = e.pointerId; ox = e.clientX; oy = e.clientY;
      base.style.left = ox + 'px'; base.style.top = oy + 'px';
      zone.setPointerCapture(e.pointerId);
      inp.touchMove.active = true;
    });
    zone.addEventListener('pointermove', (e) => {
      if (e.pointerId !== stickId) return;
      let dx = e.clientX - ox, dy = e.clientY - oy;
      const d = Math.hypot(dx, dy);
      if (d > R) { dx *= R / d; dy *= R / d; }
      knob.style.transform = `translate(${dx}px, ${dy}px)`;
      inp.touchMove.x = dx / R; inp.touchMove.y = -dy / R;
      if (d > R * 1.15 && !this.sprintToggle) inp.down.add('sprint');
      else if (!this.sprintToggle) inp.down.delete('sprint');
    });
    const endStick = (e) => {
      if (e.pointerId !== stickId) return;
      stickId = null;
      knob.style.transform = '';
      inp.touchMove.active = false; inp.touchMove.x = 0; inp.touchMove.y = 0;
      if (!this.sprintToggle) inp.down.delete('sprint');
    };
    zone.addEventListener('pointerup', endStick);
    zone.addEventListener('pointercancel', endStick);
    const look = $('look-zone');
    const looks = new Map();
    look.addEventListener('pointerdown', (e) => { looks.set(e.pointerId, { x: e.clientX, y: e.clientY }); look.setPointerCapture(e.pointerId); });
    look.addEventListener('pointermove', (e) => {
      const l = looks.get(e.pointerId);
      if (!l) return;
      const s = g.settings.mouseSensitivity;
      inp.lookX += (e.clientX - l.x) * 0.0055 * s;
      inp.lookY += (e.clientY - l.y) * 0.0045 * s * (g.settings.invertY ? -1 : 1);
      l.x = e.clientX; l.y = e.clientY;
    });
    const endLook = (e) => looks.delete(e.pointerId);
    look.addEventListener('pointerup', endLook);
    look.addEventListener('pointercancel', endLook);
    for (const b of document.querySelectorAll('.tbtn')) {
      b.addEventListener('pointerdown', (e) => {
        e.preventDefault(); e.stopPropagation();
        g.audio?.init();
        if (b.dataset.act) inp.press(b.dataset.act);
        if (b.dataset.hold) inp.down.add(b.dataset.hold);
        if (b.dataset.toggle) {
          this.sprintToggle = !this.sprintToggle;
          b.classList.toggle('on', this.sprintToggle);
          if (this.sprintToggle) inp.down.add(b.dataset.toggle); else inp.down.delete(b.dataset.toggle);
        }
      });
      const up = (e) => { if (b.dataset.hold) inp.down.delete(b.dataset.hold); };
      b.addEventListener('pointerup', up);
      b.addEventListener('pointercancel', up);
      b.addEventListener('pointerleave', up);
    }
    $('t-interact').classList.add('hidden');
  }

  setLoading(p, msg) {
    $('load-bar').style.width = `${Math.round(p * 100)}%`;
    if (msg) $('load-msg').textContent = msg;
    if (!this.tipSet) { this.tipSet = true; $('load-tip').textContent = TIPS[Math.floor(Math.random() * TIPS.length)]; }
  }

  hideLoading() {
    $('loading').classList.add('done');
    setTimeout(() => $('loading').classList.add('hidden'), 700);
  }
}
