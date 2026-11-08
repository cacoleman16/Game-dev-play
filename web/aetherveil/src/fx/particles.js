// Particle effects: two pooled point clouds (additive glow and soft alpha),
// emitters for fires, bursts for hits and spells, weather around the camera,
// fireflies at night, ground telegraphs for enemy attacks and lightning bolts.

import * as THREE from 'three';
import { clamp } from '../core/util.js';

const VERT = /* glsl */`
  attribute float aSize;
  attribute vec4 aColor;
  varying vec4 vColor;
  #include <fog_pars_vertex>
  void main() {
    vColor = aColor;
    vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
    gl_PointSize = aSize * (300.0 / max(-mvPosition.z, 0.1));
    gl_Position = projectionMatrix * mvPosition;
    #include <fog_vertex>
  }`;
const FRAG = /* glsl */`
  varying vec4 vColor;
  #include <fog_pars_fragment>
  void main() {
    vec2 c = gl_PointCoord - 0.5;
    float d = length(c);
    float a = smoothstep(0.5, 0.0, d);
    a *= a;
    if (a * vColor.a < 0.004) discard;
    gl_FragColor = vec4(vColor.rgb, a * vColor.a);
    #include <fog_fragment>
  }`;

class Pool {
  constructor(max, additive) {
    this.max = max;
    this.pos = new Float32Array(max * 3);
    this.col = new Float32Array(max * 4);
    this.size = new Float32Array(max);
    this.vel = new Float32Array(max * 3);
    this.life = new Float32Array(max);
    this.maxLife = new Float32Array(max);
    this.base = new Float32Array(max * 4); // r g b size
    this.drag = new Float32Array(max);
    this.grav = new Float32Array(max);
    this.grow = new Float32Array(max);
    this.alive = 0;
    this.cursor = 0;
    const g = new THREE.BufferGeometry();
    this.posAttr = new THREE.BufferAttribute(this.pos, 3).setUsage(THREE.DynamicDrawUsage);
    this.colAttr = new THREE.BufferAttribute(this.col, 4).setUsage(THREE.DynamicDrawUsage);
    this.sizeAttr = new THREE.BufferAttribute(this.size, 1).setUsage(THREE.DynamicDrawUsage);
    g.setAttribute('position', this.posAttr);
    g.setAttribute('aColor', this.colAttr);
    g.setAttribute('aSize', this.sizeAttr);
    const mat = new THREE.ShaderMaterial({
      uniforms: THREE.UniformsUtils.merge([THREE.UniformsLib.fog]),
      vertexShader: VERT, fragmentShader: FRAG, transparent: true, depthWrite: false, fog: true,
      blending: additive ? THREE.AdditiveBlending : THREE.NormalBlending,
    });
    this.points = new THREE.Points(g, mat);
    this.points.frustumCulled = false;
    this.points.renderOrder = additive ? 20 : 19;
  }

  spawn(x, y, z, vx, vy, vz, r, g, b, size, life, drag = 1, grav = 0, grow = 0) {
    const i = this.cursor;
    this.cursor = (this.cursor + 1) % this.max;
    this.pos[i * 3] = x; this.pos[i * 3 + 1] = y; this.pos[i * 3 + 2] = z;
    this.vel[i * 3] = vx; this.vel[i * 3 + 1] = vy; this.vel[i * 3 + 2] = vz;
    this.base[i * 4] = r; this.base[i * 4 + 1] = g; this.base[i * 4 + 2] = b; this.base[i * 4 + 3] = size;
    this.life[i] = life; this.maxLife[i] = life;
    this.drag[i] = drag; this.grav[i] = grav; this.grow[i] = grow;
  }

  update(dt) {
    for (let i = 0; i < this.max; i++) {
      if (this.life[i] <= 0) { this.size[i] = 0; continue; }
      this.life[i] -= dt;
      const t = 1 - this.life[i] / this.maxLife[i];
      const d = Math.exp(-this.drag[i] * dt);
      this.vel[i * 3] *= d; this.vel[i * 3 + 1] = this.vel[i * 3 + 1] * d - this.grav[i] * dt; this.vel[i * 3 + 2] *= d;
      this.pos[i * 3] += this.vel[i * 3] * dt;
      this.pos[i * 3 + 1] += this.vel[i * 3 + 1] * dt;
      this.pos[i * 3 + 2] += this.vel[i * 3 + 2] * dt;
      const fade = t < 0.15 ? t / 0.15 : 1 - (t - 0.15) / 0.85;
      this.col[i * 4] = this.base[i * 4]; this.col[i * 4 + 1] = this.base[i * 4 + 1]; this.col[i * 4 + 2] = this.base[i * 4 + 2];
      this.col[i * 4 + 3] = clamp(fade, 0, 1);
      this.size[i] = this.base[i * 4 + 3] * (1 + this.grow[i] * t);
      if (this.life[i] <= 0) this.size[i] = 0;
    }
    this.posAttr.needsUpdate = true; this.colAttr.needsUpdate = true; this.sizeAttr.needsUpdate = true;
  }
}

const tmpC = new THREE.Color();

export class FX {
  constructor(game) {
    this.game = game;
    const max = game.quality.maxParticles;
    this.glow = new Pool(max, true);
    this.soft = new Pool(Math.floor(max * 0.6), false);
    this.group = new THREE.Group();
    this.group.name = 'fx';
    this.group.add(this.glow.points, this.soft.points);
    this.fires = []; // {pos, scale, color}
    this.telegraphs = [];
    this.bolts = [];
    this.weather = { rain: 0, snow: 0 };
    this.fireAcc = 0;
    game.events.on('fx:fire', (pos, scale = 1, color = 0xff8a2a) => this.fires.push({ pos: pos.clone(), scale, color: new THREE.Color(color) }));

    const telMat = new THREE.MeshBasicMaterial({ color: 0xff3a2a, transparent: true, opacity: 0.35, depthWrite: false, side: THREE.DoubleSide });
    this.telGeo = new THREE.RingGeometry(0.0, 1, 40);
    this.telGeo.rotateX(-Math.PI / 2);
    this.telRingGeo = new THREE.RingGeometry(0.94, 1, 48);
    this.telRingGeo.rotateX(-Math.PI / 2);
    this.telMat = telMat;
    this.telRingMat = new THREE.MeshBasicMaterial({ color: 0xff6a4a, transparent: true, opacity: 0.8, depthWrite: false, side: THREE.DoubleSide });
  }

  addTo(scene) { scene.add(this.group); }

  c(hex) { return tmpC.set(hex); }

  burst(pos, color, n = 20, opts = {}) {
    const c = this.c(color);
    const sp = opts.speed ?? 4, size = opts.size ?? 0.35, life = opts.life ?? 0.6, grav = opts.grav ?? 0;
    const pool = opts.soft ? this.soft : this.glow;
    for (let i = 0; i < n; i++) {
      const u = Math.random() * 2 - 1, th = Math.random() * Math.PI * 2;
      const r = Math.sqrt(1 - u * u);
      const s = sp * (0.4 + Math.random() * 0.6);
      pool.spawn(pos.x, pos.y, pos.z, r * Math.cos(th) * s, u * s + (opts.up ?? 0), r * Math.sin(th) * s, c.r, c.g, c.b, size * (0.6 + Math.random() * 0.8), life * (0.6 + Math.random() * 0.6), opts.drag ?? 2.5, grav, opts.grow ?? 0);
    }
  }

  sparks(pos, color = 0xffe8a0, n = 12) {
    this.burst(pos, color, n, { speed: 7, size: 0.18, life: 0.35, grav: 9, drag: 1.5 });
  }

  hit(pos, color = 0xffffff) {
    this.burst(pos, color, 14, { speed: 5, size: 0.3, life: 0.35 });
    this.burst(pos, 0xffffff, 4, { speed: 1, size: 0.9, life: 0.15 });
  }

  dust(pos, n = 6, color = 0xc8b898) {
    const c = this.c(color);
    for (let i = 0; i < n; i++) {
      const a = Math.random() * Math.PI * 2;
      this.soft.spawn(pos.x + Math.cos(a) * 0.2, pos.y + 0.1, pos.z + Math.sin(a) * 0.2, Math.cos(a) * 1.2, 0.6 + Math.random() * 0.6, Math.sin(a) * 1.2, c.r, c.g, c.b, 0.6, 0.6 + Math.random() * 0.3, 3, 0, 1.5);
    }
  }

  splash(pos) {
    for (let i = 0; i < 24; i++) {
      const a = Math.random() * Math.PI * 2, s = 1 + Math.random() * 2.5;
      this.soft.spawn(pos.x, 0.1, pos.z, Math.cos(a) * s, 3 + Math.random() * 3, Math.sin(a) * s, 0.85, 0.95, 1, 0.35, 0.8, 0.5, 12, 0);
    }
  }

  trail(pos, color, size = 0.4) {
    const c = this.c(color);
    this.glow.spawn(pos.x + (Math.random() - 0.5) * 0.1, pos.y + (Math.random() - 0.5) * 0.1, pos.z + (Math.random() - 0.5) * 0.1, 0, 0.3, 0, c.r, c.g, c.b, size, 0.35, 1, 0, -0.6);
  }

  healAura(pos) {
    const c = this.c(0x9fffb0);
    for (let i = 0; i < 40; i++) {
      const a = Math.random() * Math.PI * 2, r = 0.4 + Math.random() * 0.6;
      this.glow.spawn(pos.x + Math.cos(a) * r, pos.y + Math.random() * 0.4, pos.z + Math.sin(a) * r, 0, 1.5 + Math.random() * 2, 0, c.r, c.g, c.b, 0.25, 1.1, 0.5, -0.5, 0);
    }
  }

  levelUp(pos) {
    for (let i = 0; i < 80; i++) {
      const a = (i / 80) * Math.PI * 6, r = 1.0;
      const c = this.c(i % 2 ? 0xffe27a : 0x9ff0ff);
      this.glow.spawn(pos.x + Math.cos(a) * r, pos.y + (i / 80) * 2.5, pos.z + Math.sin(a) * r, -Math.sin(a) * 0.8, 1.2, Math.cos(a) * 0.8, c.r, c.g, c.b, 0.3, 1.4, 0.3, 0, 0);
    }
  }

  // Red ground circle warning of an incoming attack.
  telegraph(pos, radius, dur) {
    const fill = new THREE.Mesh(this.telGeo, this.telMat.clone());
    const ring = new THREE.Mesh(this.telRingGeo, this.telRingMat);
    const g = new THREE.Group();
    g.add(fill, ring);
    g.position.copy(pos);
    g.position.y += 0.08;
    ring.scale.setScalar(radius);
    fill.scale.setScalar(0.01);
    this.group.add(g);
    const t = { g, fill, radius, t: 0, dur };
    this.telegraphs.push(t);
    return t;
  }

  lightning(from, to) {
    const pts = [];
    const segs = 14;
    for (let i = 0; i <= segs; i++) {
      const k = i / segs;
      const p = from.clone().lerp(to, k);
      if (i > 0 && i < segs) { p.x += (Math.random() - 0.5) * 2.2; p.z += (Math.random() - 0.5) * 2.2; }
      pts.push(p);
    }
    const geo = new THREE.BufferGeometry().setFromPoints(pts);
    const mat = new THREE.LineBasicMaterial({ color: 0xe8e0ff, transparent: true, opacity: 1, blending: THREE.AdditiveBlending, fog: false });
    const line = new THREE.Line(geo, mat);
    this.group.add(line);
    // Thick glowing core made of particles along the bolt.
    for (let i = 0; i < pts.length - 1; i++) {
      for (let k = 0; k < 4; k++) {
        const p = pts[i].clone().lerp(pts[i + 1], k / 4);
        this.glow.spawn(p.x, p.y, p.z, 0, 0, 0, 0.85, 0.8, 1.0, 1.2, 0.25, 0, 0, -0.5);
      }
    }
    this.bolts.push({ line, t: 0 });
    this.burst(to, 0xd8c8ff, 40, { speed: 9, size: 0.4, life: 0.5 });
  }

  update(dt, camPos) {
    const night = this.game.sky ? this.game.sky.uniforms.uNight.value : 0;
    // Fires near the camera.
    this.fireAcc += dt;
    if (this.fireAcc > 0.03) {
      const steps = Math.floor(this.fireAcc / 0.03);
      this.fireAcc -= steps * 0.03;
      for (const f of this.fires) {
        if (f.pos.distanceToSquared(camPos) > 110 * 110) continue;
        for (let s = 0; s < steps; s++) {
          const c = f.color;
          const k = f.scale;
          this.glow.spawn(f.pos.x + (Math.random() - 0.5) * 0.4 * k, f.pos.y, f.pos.z + (Math.random() - 0.5) * 0.4 * k, (Math.random() - 0.5) * 0.3, (1.4 + Math.random()) * k, (Math.random() - 0.5) * 0.3, c.r, c.g * (0.7 + Math.random() * 0.3), c.b, 0.55 * k, 0.6 + Math.random() * 0.3, 1, -0.3, -0.7);
          if (Math.random() < 0.15) this.glow.spawn(f.pos.x, f.pos.y + 0.3, f.pos.z, (Math.random() - 0.5) * 0.8, 2.5 + Math.random() * 1.5, (Math.random() - 0.5) * 0.8, 1, 0.7, 0.3, 0.08, 1.4, 0.5, 0, 0);
          if (Math.random() < 0.08) this.soft.spawn(f.pos.x, f.pos.y + 0.8 * k, f.pos.z, 0.2, 1.2, 0.1, 0.35, 0.33, 0.32, 0.9 * k, 2.2, 0.3, 0, 2);
        }
      }
    }
    // Weather around the camera.
    const w = this.weather;
    if (w.rain > 0.01 && !this.game.inDungeon) {
      const n = Math.floor(w.rain * 90 * dt * 60 * (this.game.quality.maxParticles / 2000));
      for (let i = 0; i < n; i++) {
        const x = camPos.x + (Math.random() - 0.5) * 40, z = camPos.z + (Math.random() - 0.5) * 40;
        this.soft.spawn(x, camPos.y + 12 + Math.random() * 6, z, 1.5, -26, 0.6, 0.7, 0.75, 0.85, 0.12, 0.8, 0, 0, 0);
      }
    }
    if (w.snow > 0.01 && !this.game.inDungeon) {
      const n = Math.floor(w.snow * 50 * dt * 60 * (this.game.quality.maxParticles / 2000));
      for (let i = 0; i < n; i++) {
        const x = camPos.x + (Math.random() - 0.5) * 50, z = camPos.z + (Math.random() - 0.5) * 50;
        this.soft.spawn(x, camPos.y + 10 + Math.random() * 6, z, 1.2 + Math.random(), -2.2 - Math.random(), 0.4, 0.95, 0.97, 1, 0.22, 6, 0, 0, 0);
      }
    }
    // Fireflies at night in meadows and forests.
    if (night > 0.5 && !this.game.inDungeon && Math.random() < dt * 25 * night) {
      const a = Math.random() * Math.PI * 2, r = 5 + Math.random() * 25;
      const x = camPos.x + Math.cos(a) * r, z = camPos.z + Math.sin(a) * r;
      const world = this.game.world;
      if (world.grassAt(x, z) > 0.5) {
        const y = world.heightAt(x, z) + 0.5 + Math.random() * 1.5;
        this.glow.spawn(x, y, z, (Math.random() - 0.5) * 0.5, (Math.random() - 0.5) * 0.3, (Math.random() - 0.5) * 0.5, 0.75, 1.0, 0.35, 0.22, 4 + Math.random() * 3, 0.2, 0, 0);
      }
    }
    this.glow.update(dt);
    this.soft.update(dt);
    // Telegraphs fill up over their duration.
    for (let i = this.telegraphs.length - 1; i >= 0; i--) {
      const t = this.telegraphs[i];
      t.t += dt;
      const k = Math.min(1, t.t / t.dur);
      t.fill.scale.setScalar(t.radius * k);
      t.fill.material.opacity = 0.2 + 0.25 * k;
      if (t.t >= t.dur + 0.1 || t.cancel) {
        this.group.remove(t.g);
        t.fill.material.dispose();
        this.telegraphs.splice(i, 1);
      }
    }
    for (let i = this.bolts.length - 1; i >= 0; i--) {
      const b = this.bolts[i];
      b.t += dt;
      b.line.material.opacity = 1 - b.t / 0.3;
      if (b.t > 0.3) { this.group.remove(b.line); b.line.geometry.dispose(); b.line.material.dispose(); this.bolts.splice(i, 1); }
    }
  }
}
