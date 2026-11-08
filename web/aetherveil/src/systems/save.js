// Save games in localStorage: three manual slots plus an autosave.

const KEY = 'aetherveil.save.';
const VERSION = 1;

export class SaveSystem {
  constructor(game) {
    this.game = game;
    this.autoT = 0;
  }

  snapshot() {
    const g = this.game, p = g.player;
    return {
      v: VERSION,
      at: Date.now(),
      player: {
        name: p.name, look: p.look, pos: [p.position.x, p.position.y, p.position.z], yaw: p.yaw,
        stats: p.stats, selectedSpell: p.selectedSpell,
      },
      inventory: g.inventory.serialize(),
      quests: g.quests.serialize(),
      story: g.story.serialize(),
      flags: g.flags,
      time: g.sky.timeOfDay,
      totalHours: g.totalHours,
      zones: g.spawner.serialize(),
      inDungeon: false,
    };
  }

  save(slot = 'auto') {
    if (this.game.inDungeon && slot === 'auto') return false;
    try {
      const data = this.snapshot();
      if (this.game.inDungeon) {
        // Save at the vault entrance rather than inside.
        const d = this.game.structures.points['vault.door'];
        data.player.pos = [d.x, this.game.world.heightAt(d.x, d.z) + 0.2, d.z + 2];
      }
      localStorage.setItem(KEY + slot, JSON.stringify(data));
      this.game.events.emit('saved', slot);
      return true;
    } catch (e) {
      console.warn('save failed', e);
      return false;
    }
  }

  autosave() {
    if (this.game.mode !== 'play' || this.game.player.dead) return;
    this.save('auto');
  }

  load(slot = 'auto') {
    let data;
    try { data = JSON.parse(localStorage.getItem(KEY + slot) || 'null'); } catch (e) { data = null; }
    if (!data || data.v !== VERSION) return null;
    return data;
  }

  list() {
    const out = [];
    for (const slot of ['auto', '1', '2', '3']) {
      const d = this.load(slot);
      out.push({ slot, data: d });
    }
    return out;
  }

  latest() {
    let best = null;
    for (const { slot, data } of this.list()) if (data && (!best || data.at > best.data.at)) best = { slot, data };
    return best;
  }

  apply(data) {
    const g = this.game, p = g.player;
    p.name = data.player.name;
    p.look = data.player.look;
    p.stats = { ...p.stats, ...data.player.stats };
    p.selectedSpell = data.player.selectedSpell || 'firebolt';
    g.inventory.restore(data.inventory);
    g.quests.restore(data.quests);
    g.flags = data.flags || {};
    g.sky.timeOfDay = data.time ?? 9;
    g.totalHours = data.totalHours ?? 0;
    g.spawner.restore(data.zones);
    g.story.restore(data.story || {});
    p.buildCharacter(p.look);
    const [x, y, z] = data.player.pos;
    p.teleport(x, y + 0.1, z);
    p.yaw = data.player.yaw ?? 0;
    g.cameraRig.yaw = p.yaw + Math.PI;
  }

  remove(slot) { localStorage.removeItem(KEY + slot); }

  update(dt) {
    this.autoT += dt;
    if (this.autoT > 90) { this.autoT = 0; this.autosave(); }
  }
}
