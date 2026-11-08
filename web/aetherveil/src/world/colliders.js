// Static collision in a spatial hash. Shapes are vertical prisms: a circle or
// an oriented box in XZ with a [yMin, yMax] span. Anything you're above you
// can stand on (rooftops, rocks, platforms); anything else blocks you.

const CS = 8; // hash cell size in metres

export class Colliders {
  constructor() {
    this.cells = new Map();
    this.all = [];
    this._stamp = 0;
  }

  key(ix, iz) { return ix * 73856093 ^ iz * 19349663; }

  insert(c) {
    c._stamp = 0;
    this.all.push(c);
    const r = c.bound;
    const ix0 = Math.floor((c.x - r) / CS), ix1 = Math.floor((c.x + r) / CS);
    const iz0 = Math.floor((c.z - r) / CS), iz1 = Math.floor((c.z + r) / CS);
    for (let iz = iz0; iz <= iz1; iz++) {
      for (let ix = ix0; ix <= ix1; ix++) {
        const k = this.key(ix, iz);
        let arr = this.cells.get(k);
        if (!arr) { arr = []; this.cells.set(k, arr); }
        arr.push(c);
      }
    }
    return c;
  }

  addCircle(x, z, r, yMin, yMax, tag = null) {
    return this.insert({ type: 0, x, z, r, yMin, yMax, bound: r, tag, enabled: true });
  }

  // hw/hd = half width (local x) / half depth (local z); rot = yaw radians.
  addBox(x, z, hw, hd, rot, yMin, yMax, tag = null) {
    return this.insert({ type: 1, x, z, hw, hd, rot, cos: Math.cos(rot), sin: Math.sin(rot), yMin, yMax, bound: Math.hypot(hw, hd), tag, enabled: true });
  }

  query(x, z, r, out = []) {
    out.length = 0;
    const stamp = ++this._stamp;
    const ix0 = Math.floor((x - r) / CS), ix1 = Math.floor((x + r) / CS);
    const iz0 = Math.floor((z - r) / CS), iz1 = Math.floor((z + r) / CS);
    for (let iz = iz0; iz <= iz1; iz++) {
      for (let ix = ix0; ix <= ix1; ix++) {
        const arr = this.cells.get(this.key(ix, iz));
        if (!arr) continue;
        for (const c of arr) {
          if (c._stamp === stamp || !c.enabled) continue;
          c._stamp = stamp;
          out.push(c);
        }
      }
    }
    return out;
  }

  // Is the point (x,z) inside the shape's footprint, grown by `pad`?
  inside(c, x, z, pad = 0) {
    if (c.type === 0) return Math.hypot(x - c.x, z - c.z) < c.r + pad;
    const dx = x - c.x, dz = z - c.z;
    const lx = dx * c.cos - dz * c.sin;
    const lz = dx * c.sin + dz * c.cos;
    return Math.abs(lx) < c.hw + pad && Math.abs(lz) < c.hd + pad;
  }

  // Highest top surface under (x,z) that is at most `maxY`, or -Infinity.
  groundAt(x, z, maxY, pad = 0.15) {
    let best = -Infinity;
    for (const c of this.query(x, z, 1, this._tmp || (this._tmp = []))) {
      if (c.yMax <= maxY && c.yMax > best && this.inside(c, x, z, pad)) best = c.yMax;
    }
    return best;
  }

  // Push a vertical capsule (feet y, height h, radius r) out of blocking shapes.
  // Returns true if anything was hit.
  resolve(pos, r, h, step = 0.45) {
    let hit = false;
    const list = this.query(pos.x, pos.z, r + 1, this._tmp2 || (this._tmp2 = []));
    for (let iter = 0; iter < 2; iter++) {
      for (const c of list) {
        if (pos.y >= c.yMax - step || pos.y + h <= c.yMin) continue; // above it or below it
        if (c.type === 0) {
          const dx = pos.x - c.x, dz = pos.z - c.z;
          const d = Math.hypot(dx, dz), min = c.r + r;
          if (d < min) {
            const nx = d > 1e-5 ? dx / d : 1, nz = d > 1e-5 ? dz / d : 0;
            pos.x = c.x + nx * min; pos.z = c.z + nz * min;
            hit = true;
          }
        } else {
          const dx = pos.x - c.x, dz = pos.z - c.z;
          let lx = dx * c.cos - dz * c.sin;
          let lz = dx * c.sin + dz * c.cos;
          const cx = Math.max(-c.hw, Math.min(c.hw, lx));
          const cz = Math.max(-c.hd, Math.min(c.hd, lz));
          let ox = lx - cx, oz = lz - cz;
          const d = Math.hypot(ox, oz);
          if (d < r) {
            if (d > 1e-5) {
              lx = cx + (ox / d) * r; lz = cz + (oz / d) * r;
            } else {
              // Centre is inside the box: push out along the shallowest axis.
              const px = c.hw - Math.abs(lx), pz = c.hd - Math.abs(lz);
              if (px < pz) lx = Math.sign(lx || 1) * (c.hw + r);
              else lz = Math.sign(lz || 1) * (c.hd + r);
            }
            pos.x = c.x + lx * c.cos + lz * c.sin;
            pos.z = c.z - lx * c.sin + lz * c.cos;
            hit = true;
          }
        }
      }
    }
    return hit;
  }

  // Is there anything solid near (x,z) within radius r (used when placing props)?
  occupied(x, z, r) {
    for (const c of this.query(x, z, r, this._tmp3 || (this._tmp3 = []))) {
      if (c.type === 0 ? Math.hypot(x - c.x, z - c.z) < c.r + r : this.inside(c, x, z, r)) return true;
    }
    return false;
  }

  // March along a segment; return the fraction [0,1] at which it first enters a shape.
  raycast(ax, ay, az, bx, by, bz, steps = 16) {
    for (let i = 1; i <= steps; i++) {
      const t = i / steps;
      const x = ax + (bx - ax) * t, y = ay + (by - ay) * t, z = az + (bz - az) * t;
      for (const c of this.query(x, z, 0.5, this._tmp4 || (this._tmp4 = []))) {
        if (y > c.yMin && y < c.yMax && this.inside(c, x, z, 0.2)) return (i - 1) / steps;
      }
    }
    return 1;
  }
}
