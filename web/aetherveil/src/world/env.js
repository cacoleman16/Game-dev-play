// The physical environment actors move through. The overworld uses the
// terrain heightfield plus static colliders; the dungeon provides its own
// implementation of the same small interface.

import { WATER_LEVEL, BIOME } from './worldgen.js';

export class OverworldEnv {
  constructor(game) {
    this.game = game;
    this.world = game.world;
    this.colliders = game.colliders;
    this.isDungeon = false;
  }

  // Highest walkable surface under (x,z) not above y + step.
  ground(x, z, y, step = 0.55) {
    const t = this.world.heightAt(x, z);
    const c = this.colliders.groundAt(x, z, y + step);
    return c > t ? { y: c, terrain: false } : { y: t, terrain: true };
  }

  resolve(pos, r, h) { return this.colliders.resolve(pos, r, h); }
  waterLevel() { return WATER_LEVEL; }
  waterDepth(x, z) { return WATER_LEVEL - this.world.heightAt(x, z); }
  normal(x, z, out) { return this.world.normalAt(x, z, out); }
  inBounds(x, z) { return Math.hypot(x, z) < 990; }

  surface(x, z) {
    const b = this.world.biomeAt(x, z);
    if (b === BIOME.SNOW) return 'snow';
    if (b === BIOME.ROAD || b === BIOME.PLAZA || b === BIOME.ROCK || b === BIOME.MESA) return 'stone';
    if (b === BIOME.BEACH || b === BIOME.DESERT) return 'sand';
    return 'grass';
  }
}
