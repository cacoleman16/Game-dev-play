// The Sunscar Vault: a hand-laid interior on a 4 m tile grid, rendered in its
// own scene with torchlight. It implements the same environment interface as
// the overworld (ground, resolve, water...) so actors move through it alike.

import * as THREE from 'three';
import { paint, place, merge, box, cyl, cone, ico, torus, lumpy } from './geo.js';
import { makeGlowMaterial } from './vegetation.js';
import { makeDetailedMaterial } from './terrain.js';

const T = 4; // tile size
const OX = 5000, OZ = 5000; // far from the overworld so colliders never overlap

// # wall  . floor  L lava  E entry  X exit portal  C chest  c big chest
// S revenant  M magma gloop  G golem  B boss  P pillar
const MAP = [
  '#####################',
  '########.....########',
  '########..E..########',
  '########.....########',
  '##########.##########',
  '######.........######',
  '######.S.....S.######',
  '######P.......P######',
  '######.........######',
  '######P...S...P######',
  '##########.##########',
  '###.....##.##.....###',
  '###.C.........S.M.###',
  '###.....##.##.....###',
  '##########.####.#####',
  '##########.####.#####',
  '######.....LL.....###',
  '######.M..LLLL..S.###',
  '######..G..LL...c.###',
  '##########.##########',
  '#######...........###',
  '######.............##',
  '######......B......##',
  '######.............##',
  '#######.....X.....###',
  '#####################',
];

export class Dungeon {
  constructor(game) {
    this.game = game;
    this.scene = new THREE.Scene();
    this.scene.background = new THREE.Color(0x070405);
    this.scene.fog = new THREE.FogExp2(0x1a0c08, 0.03);
    this.H = MAP.length; this.W = MAP[0].length;
    this.isDungeon = true;
    this.built = false;
    this.torches = [];
    this.lavaT = 0;
    this.spawned = [];
    this.flows = new Map();
    this.wp = { x: 0, z: 0 };
  }

  tile(ix, iz) {
    if (iz < 0 || iz >= this.H || ix < 0 || ix >= this.W) return '#';
    return MAP[iz][ix];
  }
  toTile(x, z) { return [Math.floor((x - OX) / T), Math.floor((z - OZ) / T)]; }
  center(ix, iz) { return [OX + ix * T + T / 2, OZ + iz * T + T / 2]; }
  find(ch) {
    const out = [];
    for (let z = 0; z < this.H; z++) for (let x = 0; x < this.W; x++) if (MAP[z][x] === ch) out.push([x, z]);
    return out;
  }

  build() {
    if (this.built) return;
    this.built = true;
    const g = this.game;
    const solid = [], glow = [], lava = [];
    const stone = (x, y, z) => new THREE.Color(0x6a4a3a).offsetHSL(0, 0, ((Math.sin(x * 3.1 + z * 1.7) + Math.sin(y * 4.3)) * 0.03));
    for (let iz = 0; iz < this.H; iz++) {
      for (let ix = 0; ix < this.W; ix++) {
        const c = this.tile(ix, iz);
        const [x, z] = this.center(ix, iz);
        if (c === '#') {
          // Only build walls that border a floor tile.
          let border = false;
          for (const [dx, dz] of [[1, 0], [-1, 0], [0, 1], [0, -1], [1, 1], [-1, -1], [1, -1], [-1, 1]]) if (this.tile(ix + dx, iz + dz) !== '#') border = true;
          if (!border) continue;
          solid.push(place(paint(new THREE.BoxGeometry(T, 7, T), stone, 0.12), x, 3.5, z));
          continue;
        }
        if (c === 'L') {
          lava.push(place(paint(new THREE.BoxGeometry(T, 0.2, T), (px, py, pz) => new THREE.Color(0xff5a1a).lerp(new THREE.Color(0xffc23a), (Math.sin(px * 1.3) * Math.sin(pz * 1.1) + 1) / 2)), x, -0.45, z));
          continue;
        }
        const checker = (ix + iz) % 2 ? 0x8a6a52 : 0x7a5a46;
        solid.push(place(box(T, 0.4, T, checker, 0.08), x, -0.2, z));
        if (c === 'P') {
          solid.push(place(cyl(0.7, 0.85, 7, 10, 0xb08a68, 0.1), x, 3.5, z));
          solid.push(place(box(2, 0.6, 2, 0x9a7a5a), x, 0.3, z));
        }
      }
    }
    // Torches on walls facing floor, every few tiles.
    let k = 0;
    for (let iz = 1; iz < this.H - 1; iz++) {
      for (let ix = 1; ix < this.W - 1; ix++) {
        if (this.tile(ix, iz) === '#' ) continue;
        for (const [dx, dz] of [[1, 0], [-1, 0], [0, 1], [0, -1]]) {
          if (this.tile(ix + dx, iz + dz) !== '#') continue;
          if ((ix * 7 + iz * 13 + dx * 3 + dz * 5) % 5 !== 0) continue;
          const [cx, cz] = this.center(ix, iz);
          const wx = cx + dx * (T / 2 - 0.25), wz = cz + dz * (T / 2 - 0.25);
          solid.push(place(box(0.2, 0.7, 0.2, 0x3a2a20), wx, 2.4, wz));
          glow.push(place(ico(0.18, 0, 0xffa040), wx, 2.85, wz));
          const pos = new THREE.Vector3(wx - dx * 0.1, 2.9, wz - dz * 0.1);
          this.torches.push(pos);
          k++;
        }
      }
    }
    // Banners and bones for flavour.
    for (const [ix, iz] of this.find('.').filter((_, i) => i % 9 === 0)) {
      const [x, z] = this.center(ix, iz);
      solid.push(place(lumpy(ico(0.22, 0, 0xe8dcc8), 0.05, 1, ix), x + 1.2, 0.1, z - 0.8));
      solid.push(place(box(0.7, 0.08, 0.12, 0xe8dcc8), x + 0.6, 0.05, z - 1.1, [0, ix, 0]));
    }
    const mat = makeDetailedMaterial({ detail: 0.3 });
    const m = new THREE.Mesh(merge(solid), mat);
    m.receiveShadow = true;
    this.scene.add(m);
    this.scene.add(new THREE.Mesh(merge(glow), makeGlowMaterial(1.6)));
    if (lava.length) {
      this.lavaMesh = new THREE.Mesh(merge(lava), makeGlowMaterial(1.4));
      this.scene.add(this.lavaMesh);
    }
    // Lighting: dim warm ambient, plus a few point lights that follow the nearest torches.
    this.scene.add(new THREE.HemisphereLight(0x8a5a40, 0x200a05, 0.9));
    this.scene.add(new THREE.AmbientLight(0x402018, 0.6));
    this.lights = [];
    for (let i = 0; i < 4; i++) {
      const l = new THREE.PointLight(0xff9a50, 30, 16, 1.6);
      this.scene.add(l);
      this.lights.push(l);
    }
    this.playerLight = new THREE.PointLight(0xffd0a0, 5, 14, 1.2);
    this.scene.add(this.playerLight);
    this.lavaLight = new THREE.PointLight(0xff6a20, 40, 22, 1.4);
    const [lx, lz] = this.center(12, 17);
    this.lavaLight.position.set(lx, 1.5, lz);
    this.scene.add(this.lavaLight);
    // Exit portal and the entry stairs.
    const [ex, ez] = this.center(...this.find('X')[0]);
    const portal = new THREE.Mesh(new THREE.TorusGeometry(1.4, 0.15, 8, 32), new THREE.MeshBasicMaterial({ color: new THREE.Color(0x8ff0ff).multiplyScalar(1.8) }));
    portal.position.set(ex, 1.8, ez);
    this.scene.add(portal);
    this.portal = portal;
    const disc = new THREE.Mesh(new THREE.CircleGeometry(1.3, 32), new THREE.MeshBasicMaterial({ color: 0x8ff0ff, transparent: true, opacity: 0.35, side: THREE.DoubleSide, blending: THREE.AdditiveBlending, depthWrite: false }));
    portal.add(disc);
    g.interact.add({ id: 'dungeon:exit', x: ex, y: 1, z: ez, r: 3, label: 'Leave the vault', enabled: () => g.inDungeon && !!g.flags.colossus, action: () => this.exit() });
    const [nx, nz] = this.center(...this.find('E')[0]);
    this.entry = { x: nx, z: nz };
    const stairs = new THREE.Mesh(merge([0, 1, 2, 3].map((i) => place(box(4, 0.4, 1, 0x9a7a5a), nx, 0.2 + i * 0.4, nz - 4 - i * 1))), mat);
    this.scene.add(stairs);
    g.interact.add({ id: 'dungeon:entry', x: nx, y: 1, z: nz - 3, r: 3, label: 'Climb back to the surface', enabled: () => g.inDungeon, action: () => this.exit() });
    // Chests.
    let ci = 0;
    for (const [ix, iz] of this.find('C').concat(this.find('c'))) {
      const [x, z] = this.center(ix, iz);
      const big = this.tile(ix, iz) === 'c';
      const c = g.structures.chest('vault_' + ci++, x, z, Math.PI, big ? 'vault' : 'vault_small', { y: 0 });
      this.scene.add(c.group);
      if (g.story.openedChests.has(c.id)) { c.opened = true; c.openT = 1; c.lid.rotation.x = -1.9; }
    }
    // Fire particles at torches handled by FX when inside.
    this.fireDefs = this.torches.map((p) => ({ pos: p, scale: 0.45, color: new THREE.Color(0xff8a3a) }));
  }

  enter() {
    const g = this.game;
    this.build();
    g.ui.fade(() => {
      g.spawner.despawnAll();
      g.story.despawnBoss();
      g.inDungeon = true;
      this.prevScene = g.scene;
      this.prevEnv = g.env;
      g.scene = this.scene;
      g.env = this;
      this.scene.add(g.player.character.group);
      this.scene.add(g.fx.group);
      this.overworldFires = g.fx.fires;
      g.fx.fires = this.fireDefs;
      g.player.teleport(this.entry.x, 0, this.entry.z + 1);
      g.player.yaw = 0;
      g.cameraRig.yaw = Math.PI;
      g.cameraRig.pitch = -0.25;
      // Populate.
      for (const [ch, type] of [['S', 'revenant'], ['M', 'magma_gloop'], ['G', 'golem']]) {
        for (const [ix, iz] of this.find(ch)) {
          const [x, z] = this.center(ix, iz);
          const e = g.spawner.spawn(type, x, z, { level: 2 });
          this.spawned.push(e);
        }
      }
      g.events.emit('banner', 'Sunscar Vault');
      g.ui.banner('The Sunscar Vault', 'Heat rolls up from somewhere far below.', 0xff9a50);
      g.audio?.setMusic(null);
    });
  }

  exit(silent = false) {
    const g = this.game;
    const go = () => {
      g.combat.clearAll();
      g.story.despawnBoss();
      g.inDungeon = false;
      g.scene = this.prevScene;
      g.env = this.prevEnv;
      g.scene.add(g.player.character.group);
      g.scene.add(g.fx.group);
      g.fx.fires = this.overworldFires;
      this.spawned = [];
      const d = g.structures.points['vault.door'];
      if (!silent) {
        g.player.teleport(d.x, g.world.heightAt(d.x, d.z + 3) + 0.2, d.z + 3);
        g.player.yaw = 0;
        g.cameraRig.yaw = Math.PI;
      }
      g.terrain.update(d.x, d.z, true);
      g.vegetation.lastX = 1e9;
    };
    if (silent) go(); else g.ui.fade(go);
  }

  update(dt) {
    const g = this.game, p = g.player.position;
    // Nearest torches get the real lights.
    const sorted = this.torches.map((t) => [t, t.distanceToSquared(p)]).sort((a, b) => a[1] - b[1]);
    this.lights.forEach((l, i) => {
      const t = sorted[i];
      if (t) { l.position.copy(t[0]); l.intensity = 26 + Math.sin(g.time * 13 + i * 2) * 4; }
    });
    this.playerLight.position.set(p.x, p.y + 4.5, p.z);
    this.lavaLight.intensity = 36 + Math.sin(g.time * 2) * 6;
    if (this.portal) {
      this.portal.rotation.y += dt;
      this.portal.visible = !!g.flags.colossus;
    }
    // Lava burns.
    const [ix, iz] = this.toTile(p.x, p.z);
    this.lavaT -= dt;
    if (this.tile(ix, iz) === 'L' && g.player.grounded && this.lavaT <= 0) {
      this.lavaT = 0.5;
      g.player.takeDamage(9, null, { element: 'fire', unblockable: true, knockback: 0 });
      g.fx.burst(p.clone().setY(p.y + 0.3), 0xff7a2a, 12, { speed: 3, up: 2 });
    }
    // Boss trigger.
    if (!g.flags.colossus && !g.story.activeBoss) {
      const [bx, bz] = this.center(...this.find('B')[0]);
      if (Math.hypot(p.x - bx, p.z - bz) < 18) {
        const e = g.story.spawnBoss('colossus', 'colossus', bx, bz - 4);
        e.home.set(bx, 0, bz);
      }
    }
  }

  // ---------------- environment interface
  floorAt(x, z) {
    const [ix, iz] = this.toTile(x, z);
    const c = this.tile(ix, iz);
    return c === '#' ? 7 : c === 'L' ? -0.35 : 0;
  }
  solidAt(x, z, y) {
    const [ix, iz] = this.toTile(x, z);
    return this.tile(ix, iz) === '#' && y < 7;
  }
  ground(x, z, y) {
    const t = this.floorAt(x, z);
    const c = this.game.colliders.groundAt(x, z, y + 0.55);
    return c > t ? { y: c, terrain: false } : { y: t === 7 ? 0 : t, terrain: true };
  }
  resolve(pos, r, h) {
    let hit = false;
    let [cx, cz] = this.toTile(pos.x, pos.z);
    // Inside solid rock (or off the map): step back to the nearest open tile.
    if (this.tile(cx, cz) === '#') {
      let best = null, bd = Infinity;
      for (let iz = 0; iz < this.H; iz++) for (let ix = 0; ix < this.W; ix++) {
        if (this.tile(ix, iz) === '#' || this.tile(ix, iz) === 'L') continue;
        const [x, z] = this.center(ix, iz);
        const d = (x - pos.x) ** 2 + (z - pos.z) ** 2;
        if (d < bd) { bd = d; best = [x, z]; }
      }
      if (best && bd > T * T * 4) { pos.x = best[0]; pos.z = best[1]; return true; }
    }
    for (let iz = cz - 1; iz <= cz + 1; iz++) {
      for (let ix = cx - 1; ix <= cx + 1; ix++) {
        if (this.tile(ix, iz) !== '#') continue;
        const minX = OX + ix * T, maxX = minX + T, minZ = OZ + iz * T, maxZ = minZ + T;
        const qx = Math.max(minX, Math.min(pos.x, maxX)), qz = Math.max(minZ, Math.min(pos.z, maxZ));
        const dx = pos.x - qx, dz = pos.z - qz;
        const d = Math.hypot(dx, dz);
        if (d < r) {
          if (d > 1e-5) { pos.x = qx + (dx / d) * r; pos.z = qz + (dz / d) * r; }
          else {
            // Centre inside a wall tile: push out through the nearest face.
            const px = pos.x - minX, pz = pos.z - minZ;
            const m = Math.min(px, T - px, pz, T - pz);
            if (m === px) pos.x = minX - r; else if (m === T - px) pos.x = maxX + r; else if (m === pz) pos.z = minZ - r; else pos.z = maxZ + r;
          }
          hit = true;
        }
      }
    }
    this.game.colliders.resolve(pos, r, h);
    return hit;
  }
  // ---------------- grid navigation
  // BFS distance field toward a tile, cached per target, so actors route
  // around walls instead of pressing into them.
  flowTo(tx, tz) {
    const W = this.W, k = tz * W + tx;
    let f = this.flows.get(k);
    if (f) return f;
    f = new Int16Array(W * this.H).fill(-1);
    f[k] = 0;
    const q = [k];
    for (let h = 0; h < q.length; h++) {
      const c = q[h], cx = c % W, cz = (c / W) | 0;
      for (let d = 0; d < 4; d++) {
        const nx = cx + (d === 0 ? 1 : d === 1 ? -1 : 0), nz = cz + (d === 2 ? 1 : d === 3 ? -1 : 0);
        if (this.tile(nx, nz) === '#') continue;
        const n = nz * W + nx;
        if (f[n] >= 0) continue;
        f[n] = f[c] + 1;
        q.push(n);
      }
    }
    this.flows.set(k, f);
    return f;
  }
  // True when a body of radius pad can walk straight from a to b.
  clearLine(ax, az, bx, bz, pad = 0.6) {
    const n = Math.max(1, Math.ceil(Math.hypot(bx - ax, bz - az) / 0.5));
    for (let k = 0; k <= n; k++) {
      const t = k / n, x = ax + (bx - ax) * t, z = az + (bz - az) * t;
      if (this.solidAt(x + pad, z, 0) || this.solidAt(x - pad, z, 0) || this.solidAt(x, z + pad, 0) || this.solidAt(x, z - pad, 0)) return false;
    }
    return true;
  }
  // Where to walk next to reach (gx, gz): the goal itself when the way is
  // open, otherwise the farthest tile centre along the flow field that can
  // be reached in a straight line. Returns a shared {x, z}.
  steerPoint(x, z, gx, gz, pad = 0.6) {
    const wp = this.wp;
    wp.x = gx; wp.z = gz;
    if (this.clearLine(x, z, gx, gz, pad)) return wp;
    const [tx, tz] = this.toTile(gx, gz);
    if (this.tile(tx, tz) === '#') return wp;
    const f = this.flowTo(tx, tz), W = this.W;
    let [ix, iz] = this.toTile(x, z);
    if (f[iz * W + ix] < 0) {
      // Pressed into a wall seam: start from the nearest open neighbour.
      let bd = 1e9;
      for (let dz = -1; dz <= 1; dz++) for (let dx = -1; dx <= 1; dx++) {
        const v = f[(iz + dz) * W + ix + dx];
        if (v >= 0 && v < bd && this.tile(ix + dx, iz + dz) !== '#') { bd = v; ix += dx; iz += dz; dx = dz = 2; }
      }
      if (bd === 1e9) return wp;
    }
    let cx = ix, cz = iz;
    for (let step = 0; step < 6; step++) {
      const cur = f[cz * W + cx];
      if (cur <= 0) break;
      let nx = -1, nz = -1;
      for (let d = 0; d < 4; d++) {
        const ax = cx + (d === 0 ? 1 : d === 1 ? -1 : 0), az = cz + (d === 2 ? 1 : d === 3 ? -1 : 0);
        if (ax < 0 || az < 0 || ax >= W || az >= this.H) continue;
        if (f[az * W + ax] === cur - 1) { nx = ax; nz = az; break; }
      }
      if (nx < 0) break;
      const [px, pz] = this.center(nx, nz);
      if (step > 0 && !this.clearLine(x, z, px, pz, pad)) break;
      wp.x = px; wp.z = pz;
      cx = nx; cz = nz;
    }
    return wp;
  }
  waterLevel() { return -1e9; }
  waterDepth() { return -1e9; }
  normal(x, z, out) { return out.set(0, 1, 0); }
  inBounds(x, z) { const [ix, iz] = this.toTile(x, z); return ix >= 0 && iz >= 0 && ix < this.W && iz < this.H; }
  recover(p) { p.x = this.entry.x; p.z = this.entry.z + 2; p.y = 0; }
  surface() { return 'stone'; }
}
