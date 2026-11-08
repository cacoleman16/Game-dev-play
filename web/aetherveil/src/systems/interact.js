// Things the player can use with the Interact key: NPCs, chests, waystones,
// doors, herbs. Picks the nearest usable target in front of the player.

export class Interact {
  constructor(game) {
    this.game = game;
    this.items = [];
    this.current = null;
  }

  add(item) {
    this.items.push(item);
    return item;
  }

  remove(id) {
    this.items = this.items.filter((i) => i.id !== id);
  }

  update(player) {
    let best = null, bestScore = Infinity;
    const p = player.position;
    const fx = Math.sin(player.yaw), fz = Math.cos(player.yaw);
    for (const it of this.items) {
      if (it.enabled && !it.enabled()) continue;
      const ix = typeof it.x === 'function' ? it.x() : it.x;
      const iz = typeof it.z === 'function' ? it.z() : it.z;
      const iy = typeof it.y === 'function' ? it.y() : it.y;
      const dx = ix - p.x, dz = iz - p.z;
      const d = Math.hypot(dx, dz);
      if (d > it.r || Math.abs(iy - (p.y + 1)) > 3.5) continue;
      const facing = d > 0.01 ? (dx * fx + dz * fz) / d : 1;
      const score = d - facing * 1.2;
      if (score < bestScore) { bestScore = score; best = it; }
    }
    if (best !== this.current) {
      this.current = best;
      this.game.events.emit('interact:target', best);
    }
    return best;
  }

  use() {
    if (this.current) {
      const it = this.current;
      it.action();
      this.current = null;
      this.game.events.emit('interact:target', null);
      return true;
    }
    return false;
  }
}
