// Quest definitions and the journal. Stages describe what to do next and
// where (for compass/map markers). Kill and collect objectives update
// themselves from game events.

import { LOC } from '../world/locations.js';
import { ITEMS } from './content.js';

const npcPos = (game, id) => game.npcs?.byId[id]?.position;
const locPos = (id) => ({ x: LOC[id].x, z: LOC[id].z });

export const QUESTS = {
  main: {
    name: 'The Shattered Heart', main: true,
    stages: [
      { desc: 'Speak with Elder Maren in Brightwater.', target: (g) => npcPos(g, 'maren') },
      { desc: "Travel north to the Scholars' Camp and find Ysolde.", target: (g) => npcPos(g, 'ysolde') },
      { desc: 'Cross the causeway and examine the pedestal beneath the Heartshard.', target: () => ({ x: LOC.heartshard.x, z: LOC.heartshard.z + 3 }) },
      { desc: 'Recover the three Resonance Shards.', multi: true, objectives: [
        { id: 'shard_verdance', label: 'Shard of Verdance — Whisperwood Ruins (west)', target: () => locPos('whisperwood') },
        { id: 'shard_rime', label: 'Shard of Rime — Frostspire Shrine (north)', target: () => locPos('frostspire') },
        { id: 'shard_ember', label: 'Shard of Embers — Sunscar Vault (east)', target: () => locPos('sunscar') },
      ] },
      { desc: 'Return the shards to the pedestal beneath the Heartshard.', target: () => ({ x: LOC.heartshard.x, z: LOC.heartshard.z + 3 }) },
      { desc: 'Defeat the Hollow Herald.', target: () => ({ x: LOC.heartshard.x, z: LOC.heartshard.z }) },
      { desc: 'The Heartshard is whole again. Lumeria is yours to explore.', final: true },
    ],
    reward: { xp: 500, gold: 500 },
  },
  wolves: {
    name: 'Wolves at the Door', giver: 'tobin',
    stages: [
      { desc: 'Hunt Dusk Wolves near Tobin\'s farm.', kill: 'wolf', count: 5, target: () => ({ x: 300, z: 300 }) },
      { desc: 'Return to Tobin at his farm.', target: (g) => npcPos(g, 'tobin') },
    ],
    reward: { xp: 120, gold: 120, items: [['potion_health', 3]] },
  },
  herbs: {
    name: 'A Glow in the Dark', giver: 'sera',
    stages: [
      { desc: 'Gather Glowcaps. They grow in the forests and the crystal meadows.', collect: 'glowcap', count: 5, target: () => ({ x: -300, z: 150 }) },
      { desc: 'Bring the Glowcaps to Sera in Brightwater.', target: (g) => npcPos(g, 'sera') },
    ],
    reward: { xp: 100, gold: 60, items: [['elixir', 1]] },
  },
  locket: {
    name: 'The Lost Locket', giver: 'mira',
    stages: [
      { desc: 'Search the Redcloak Hideout for Mira\'s locket.', collect: 'locket', count: 1, target: () => locPos('banditcamp') },
      { desc: 'Return the locket to Mira.', target: (g) => npcPos(g, 'mira') },
    ],
    reward: { xp: 150, items: [['ring_vigor', 1]] },
  },
  bounty: {
    name: 'Redcloak Bounty', giver: 'dain',
    stages: [
      { desc: 'Defeat Redcloak bandits.', kill: ['bandit', 'bandit_mage'], count: 6, target: () => locPos('banditcamp') },
      { desc: 'Report to Captain Dain in Brightwater.', target: (g) => npcPos(g, 'dain') },
    ],
    reward: { xp: 180, gold: 200 },
  },
  letter: {
    name: 'A Letter for Old Wick', giver: 'lyra',
    stages: [
      { desc: 'Deliver Lyra\'s letter to Old Wick at Gull Point Light.', target: (g) => npcPos(g, 'wick') },
    ],
    reward: { xp: 90, gold: 50, items: [['windstep_charm', 1]] },
  },
  cores: {
    name: 'Heart of Stone', giver: 'garrick',
    stages: [
      { desc: 'Collect Golem Cores from the ruin golems.', collect: 'golem_core', count: 2, target: () => locPos('whisperwood') },
      { desc: 'Bring the cores to Garrick the smith.', target: (g) => npcPos(g, 'garrick') },
    ],
    reward: { xp: 140, items: [['steel_longsword', 1]] },
  },
};

export class Quests {
  constructor(game) {
    this.game = game;
    this.state = {};
    this.tracked = 'main';
    game.events.on('enemy:killed', (e) => this.onKill(e.type));
    game.events.on('inventory:changed', () => this.checkCollect());
  }

  get(id) { return this.state[id]; }
  active(id) { const s = this.state[id]; return !!s && !s.done; }
  done(id) { return !!this.state[id]?.done; }
  stage(id) { return this.state[id] ? this.state[id].stage : -1; }

  start(id) {
    if (this.state[id]) return;
    this.state[id] = { stage: 0, progress: 0, done: false };
    this.tracked = id;
    this.game.events.emit('quest:started', id);
    this.game.audio?.play('quest');
    this.checkCollect();
  }

  advance(id, to) {
    const s = this.state[id];
    if (!s || s.done) return;
    s.stage = to ?? s.stage + 1;
    s.progress = 0;
    const def = QUESTS[id];
    if (s.stage >= def.stages.length || def.stages[s.stage].final) {
      if (def.stages[s.stage]?.final) { this.complete(id); return; }
      this.complete(id);
      return;
    }
    this.game.events.emit('quest:updated', id);
    this.game.audio?.play('questUpdate');
    this.checkCollect();
    this.game.save?.autosave();
  }

  complete(id) {
    const s = this.state[id];
    const def = QUESTS[id];
    s.done = true;
    const r = def.reward || {};
    const p = this.game.player;
    if (r.gold) p.stats.gold += r.gold;
    if (r.items) for (const [it, n] of r.items) this.game.inventory.add(it, n);
    if (r.xp) p.gainXp(r.xp);
    this.game.events.emit('quest:completed', id, r);
    this.game.audio?.play('questDone');
    if (this.tracked === id) this.tracked = this.active('main') ? 'main' : Object.keys(this.state).find((k) => this.active(k)) || null;
    this.game.save?.autosave();
  }

  onKill(type) {
    for (const id in this.state) {
      const s = this.state[id];
      if (s.done) continue;
      const st = QUESTS[id].stages[s.stage];
      if (!st.kill) continue;
      const kinds = Array.isArray(st.kill) ? st.kill : [st.kill];
      if (!kinds.includes(type)) continue;
      s.progress++;
      this.game.events.emit('quest:progress', id, s.progress, st.count);
      if (s.progress >= st.count) this.advance(id);
    }
  }

  checkCollect() {
    const inv = this.game.inventory;
    for (const id in this.state) {
      const s = this.state[id];
      if (s.done) continue;
      const st = QUESTS[id].stages[s.stage];
      if (st.collect) {
        const have = inv.count(st.collect);
        if (have !== s.progress) {
          s.progress = Math.min(have, st.count);
          if (have > 0 && have < st.count) this.game.events.emit('quest:progress', id, s.progress, st.count);
        }
        if (have >= st.count) this.advance(id);
      }
      if (id === 'main' && s.stage === 3) {
        const all = QUESTS.main.stages[3].objectives.every((o) => inv.has(o.id));
        if (all) this.advance('main', 4);
      }
    }
  }

  stageInfo(id) {
    const s = this.state[id];
    if (!s) return null;
    const def = QUESTS[id];
    const st = def.stages[Math.min(s.stage, def.stages.length - 1)];
    let desc = st.desc;
    if (st.count && !s.done) desc += ` (${s.progress}/${st.count})`;
    const objectives = st.objectives?.map((o) => ({ ...o, done: this.game.inventory.has(o.id) })) || null;
    return { name: def.name, desc, objectives, done: s.done, main: def.main };
  }

  // Marker positions for the compass and map.
  markers() {
    const out = [];
    for (const id in this.state) {
      const s = this.state[id];
      if (s.done) continue;
      const st = QUESTS[id].stages[s.stage];
      const tracked = id === this.tracked;
      if (st.objectives) {
        for (const o of st.objectives) if (!this.game.inventory.has(o.id)) { const t = o.target(this.game); if (t) out.push({ x: t.x, z: t.z, label: o.label, quest: id, tracked }); }
      } else if (st.target) {
        const t = st.target(this.game);
        if (t) out.push({ x: t.x, z: t.z, label: st.desc, quest: id, tracked });
      }
    }
    return out;
  }

  serialize() { return { state: this.state, tracked: this.tracked }; }
  restore(d) { this.state = d.state || {}; this.tracked = d.tracked ?? 'main'; }
}

export { ITEMS };
