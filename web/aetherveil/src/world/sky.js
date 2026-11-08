// Sky dome, sun/moon lighting and the day-night cycle. The dome shader draws
// a gradient sky, sun and moon discs, twinkling stars, drifting clouds and the
// glowing "rift" in the sky left by the shattered Heartshard.

import * as THREE from 'three';
import { clamp, lerp, smoothstep } from '../core/util.js';

const KEYS = [
  // elevation, zenith, horizon, sun colour, sun intensity, hemi sky, hemi ground, hemi intensity, fog
  { e: -0.35, top: 0x070b22, hor: 0x18234a, sun: 0x8aa6ff, si: 0.55, hs: 0x34488a, hg: 0x141a26, hi: 0.75, fog: 0x141d3c },
  { e: -0.08, top: 0x141c45, hor: 0x3c3f6e, sun: 0x8aa6ff, si: 0.35, hs: 0x44509a, hg: 0x1e1e2a, hi: 0.75, fog: 0x2c3158 },
  { e: 0.02, top: 0x3b4f9c, hor: 0xff9a62, sun: 0xffa060, si: 1.1, hs: 0x9a9fd0, hg: 0x4a3a30, hi: 0.8, fog: 0xd99a7a },
  { e: 0.14, top: 0x4a7fd6, hor: 0xffcf9a, sun: 0xffd2a0, si: 1.9, hs: 0xa8c8f0, hg: 0x5a5a40, hi: 0.75, fog: 0xc8d8e8 },
  { e: 0.4, top: 0x3a86e0, hor: 0xb4dcf7, sun: 0xfff2dc, si: 2.7, hs: 0xc2e2ff, hg: 0x6a7a50, hi: 0.95, fog: 0xbcdcf2 },
  { e: 1.0, top: 0x2f7ee0, hor: 0xa6d4f6, sun: 0xffffff, si: 3.0, hs: 0xc8e6ff, hg: 0x6f8050, hi: 1.0, fog: 0xb2d6f2 },
];
const tmpA = new THREE.Color(), tmpB = new THREE.Color();

function sampleKeys(e) {
  let a = KEYS[0], b = KEYS[KEYS.length - 1];
  for (let i = 0; i < KEYS.length - 1; i++) {
    if (e >= KEYS[i].e && e <= KEYS[i + 1].e) { a = KEYS[i]; b = KEYS[i + 1]; break; }
  }
  if (e < KEYS[0].e) b = a;
  const t = a === b ? 0 : clamp((e - a.e) / (b.e - a.e), 0, 1);
  const c = (k) => tmpA.setHex(a[k]).lerp(tmpB.setHex(b[k]), t).clone();
  return {
    top: c('top'), hor: c('hor'), sun: c('sun'), hs: c('hs'), hg: c('hg'), fog: c('fog'),
    si: lerp(a.si, b.si, t), hi: lerp(a.hi, b.hi, t),
  };
}

export class Sky {
  constructor(game) {
    this.game = game;
    this.timeOfDay = 9.5; // hours
    this.dayLength = 24 * 60; // real seconds per in-game day
    this.sunDir = new THREE.Vector3();
    this.moonDir = new THREE.Vector3();
    this.weather = { cloud: 0.45, rain: 0, storm: 0 };
    this.riftStrength = 1; // fades once the Heartshard is restored

    const uniforms = {
      uSunDir: { value: this.sunDir },
      uMoonDir: { value: this.moonDir },
      uTop: { value: new THREE.Color() },
      uHor: { value: new THREE.Color() },
      uSunCol: { value: new THREE.Color() },
      uNight: { value: 0 },
      uTime: game.uniforms.uTime,
      uCloud: { value: 0.45 },
      uRift: { value: 1 },
      uStorm: { value: 0 },
    };
    this.uniforms = uniforms;
    const mat = new THREE.ShaderMaterial({
      uniforms,
      side: THREE.BackSide,
      depthWrite: false,
      fog: false,
      vertexShader: /* glsl */`
        varying vec3 vDir;
        void main() {
          vDir = position;
          vec4 p = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
          gl_Position = p.xyww;
        }`,
      fragmentShader: /* glsl */`
        uniform vec3 uSunDir, uMoonDir, uTop, uHor, uSunCol;
        uniform float uNight, uTime, uCloud, uRift, uStorm;
        varying vec3 vDir;
        float hash(vec2 p) { return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453); }
        float hash3(vec3 p) { return fract(sin(dot(p, vec3(127.1, 311.7, 74.7))) * 43758.5453); }
        float vnoise(vec2 p) {
          vec2 i = floor(p), f = fract(p);
          vec2 u = f * f * (3.0 - 2.0 * f);
          return mix(mix(hash(i), hash(i + vec2(1, 0)), u.x), mix(hash(i + vec2(0, 1)), hash(i + vec2(1, 1)), u.x), u.y);
        }
        float fbm(vec2 p) {
          float s = 0.0, a = 0.5;
          for (int i = 0; i < 5; i++) { s += a * vnoise(p); p = p * 2.03 + 17.1; a *= 0.5; }
          return s;
        }
        void main() {
          vec3 d = normalize(vDir);
          float h = d.y;
          vec3 col = mix(uHor, uTop, pow(clamp(h, 0.0, 1.0), 0.55));
          col = mix(col, uHor * 0.75, smoothstep(0.0, -0.3, h));
          // Sun glow, disc and a warm haze along the horizon at dusk.
          float sd = max(dot(d, uSunDir), 0.0);
          float sunUp = smoothstep(-0.15, 0.05, uSunDir.y);
          col += uSunCol * (pow(sd, 6.0) * 0.18 + pow(sd, 48.0) * 0.45) * sunUp;
          col += uSunCol * smoothstep(0.99935, 0.99965, sd) * 6.0 * sunUp;
          float dusk = (1.0 - smoothstep(0.0, 0.35, abs(uSunDir.y))) * (1.0 - smoothstep(0.0, 0.35, abs(h)));
          col += vec3(1.0, 0.45, 0.2) * dusk * pow(max(dot(normalize(d.xz + 1e-4), normalize(uSunDir.xz + 1e-4)), 0.0), 3.0) * 0.35;
          // Moon.
          float md = max(dot(d, uMoonDir), 0.0);
          float moonDisc = smoothstep(0.99955, 0.99975, md);
          col += vec3(0.85, 0.9, 1.0) * (moonDisc * 2.5 + pow(md, 120.0) * 0.25) * uNight;
          // Stars.
          if (uNight > 0.01 && h > -0.05) {
            vec3 sp = d * 220.0;
            vec3 cell = floor(sp);
            float r = hash3(cell);
            float star = step(0.985, r) * smoothstep(0.5, 0.0, length(fract(sp) - 0.5));
            float tw = 0.6 + 0.4 * sin(uTime * (2.0 + r * 5.0) + r * 40.0);
            col += vec3(0.9, 0.95, 1.0) * star * tw * uNight * smoothstep(-0.05, 0.25, h) * 1.6;
            // A faint band of galaxy across the night sky.
            float band = exp(-pow(dot(d, normalize(vec3(0.6, 0.2, -0.75))) * 3.5, 2.0));
            col += vec3(0.35, 0.3, 0.6) * band * fbm(d.xz * 6.0 + 3.0) * 0.35 * uNight;
          }
          // The rift: a jagged tear of violet light high in the northern sky.
          if (uRift > 0.01) {
            vec2 rp = vec2(atan(d.x, -d.z) * 1.5, h * 3.0);
            float wig = fbm(vec2(rp.x * 3.0, uTime * 0.03)) * 0.35;
            float line = abs(rp.y - 1.65 - sin(rp.x * 2.3) * 0.18 - wig);
            float mask = smoothstep(0.9, 0.2, abs(rp.x - 0.1));
            float core = smoothstep(0.025, 0.0, line) * mask;
            float glow = smoothstep(0.35, 0.0, line) * mask;
            vec3 rc = mix(vec3(0.55, 0.3, 1.0), vec3(0.4, 0.95, 1.0), fbm(rp * 4.0 + uTime * 0.05));
            col += rc * (core * 2.5 + glow * 0.35) * uRift * (0.6 + 0.4 * uNight);
          }
          // Clouds: a slab of fbm projected onto a dome.
          if (h > -0.02) {
            vec2 uv = d.xz / (h + 0.12) * 1.3 + vec2(uTime * 0.006, uTime * 0.002);
            float n = fbm(uv);
            float cov = mix(0.62, 0.38, uCloud);
            float c = smoothstep(cov, cov + 0.22, n);
            float lit = 0.75 + 0.25 * smoothstep(0.0, 1.0, dot(d, uSunDir) * 0.5 + 0.5);
            vec3 cloudCol = mix(uHor * 1.1 + 0.08, uSunCol * 0.6 + uHor * 0.6, 0.35) * lit;
            cloudCol = mix(cloudCol, vec3(0.06, 0.07, 0.12), uNight * 0.75);
            cloudCol = mix(cloudCol, vec3(0.35, 0.37, 0.42), uStorm * 0.8);
            col = mix(col, cloudCol, c * smoothstep(-0.02, 0.18, h) * 0.9);
          }
          col = mix(col, vec3(0.3, 0.33, 0.38), uStorm * 0.55);
          gl_FragColor = vec4(col, 1.0);
          #include <colorspace_fragment>
        }`,
    });
    this.dome = new THREE.Mesh(new THREE.SphereGeometry(1, 32, 16), mat);
    this.dome.scale.setScalar(2800);
    this.dome.frustumCulled = false;
    this.dome.renderOrder = -10;
    this.dome.name = 'sky';

    // Lights.
    this.sun = new THREE.DirectionalLight(0xffffff, 2.5);
    this.sun.name = 'sun';
    this.hemi = new THREE.HemisphereLight(0xc8e6ff, 0x6f8050, 1.0);
    this.ambient = new THREE.AmbientLight(0xffffff, 0.12);
    const q = game.quality;
    if (q.shadows) {
      this.sun.castShadow = true;
      this.sun.shadow.mapSize.set(q.shadowSize, q.shadowSize);
      const r = q.shadowRange;
      const cam = this.sun.shadow.camera;
      cam.left = -r; cam.right = r; cam.top = r; cam.bottom = -r;
      cam.near = 1; cam.far = 600;
      this.sun.shadow.bias = -0.0006;
      this.sun.shadow.normalBias = 0.04;
    }
    this.fog = new THREE.FogExp2(0xbcdcf2, 0.0016);
    this.fogBase = 1.25 / q.viewDist;
    this.lightTarget = new THREE.Vector3();
  }

  addTo(scene) {
    scene.add(this.dome, this.sun, this.sun.target, this.hemi, this.ambient);
    scene.fog = this.fog;
  }

  get hours() { return this.timeOfDay; }
  get isNight() { return this.sunDir.y < -0.05; }

  update(dt, focus) {
    this.timeOfDay = (this.timeOfDay + (dt * 24) / this.dayLength) % 24;
    const theta = (this.timeOfDay / 24) * Math.PI * 2 - Math.PI / 2;
    const tilt = 0.45;
    this.sunDir.set(Math.cos(theta), Math.sin(theta) * Math.cos(tilt), Math.sin(theta) * Math.sin(tilt)).normalize();
    this.moonDir.copy(this.sunDir).multiplyScalar(-1);
    this.moonDir.y = Math.abs(this.moonDir.y) * 0.9 + 0.1;
    this.moonDir.normalize();

    const e = this.sunDir.y;
    const k = sampleKeys(e);
    const night = 1 - smoothstep(-0.12, 0.05, e);
    const storm = this.weather.storm;
    const u = this.uniforms;
    u.uTop.value.copy(k.top);
    u.uHor.value.copy(k.hor);
    u.uSunCol.value.copy(k.sun);
    u.uNight.value = night;
    u.uCloud.value = this.weather.cloud;
    u.uStorm.value = storm;
    u.uRift.value = this.riftStrength;

    // Directional light: the sun by day, the moon by night, dimming across the swap.
    const useMoon = e < -0.04;
    const dir = useMoon ? this.moonDir : this.sunDir;
    const swap = smoothstep(0.0, 0.12, Math.abs(e + 0.04));
    this.sun.color.copy(useMoon ? tmpA.setHex(0x9db4ff) : k.sun);
    this.sun.intensity = (useMoon ? 0.55 : k.si) * swap * (1 - storm * 0.6);
    this.hemi.color.copy(k.hs);
    this.hemi.groundColor.copy(k.hg);
    this.hemi.intensity = k.hi * (1 - storm * 0.3);
    this.ambient.intensity = 0.1 + night * 0.12;

    const fogCol = k.fog.clone().lerp(tmpA.setHex(0x7d8590), storm * 0.6);
    this.fog.color.copy(fogCol);
    this.fog.density = this.fogBase * (1 + storm * 1.6 + this.weather.rain * 0.8);

    if (focus) {
      // Snap the shadow frustum to texels so shadows don't shimmer while moving.
      const q = this.game.quality;
      const texel = (q.shadowRange * 2) / Math.max(q.shadowSize, 1);
      const fx = Math.round(focus.x / texel) * texel;
      const fz = Math.round(focus.z / texel) * texel;
      this.lightTarget.set(fx, focus.y, fz);
      this.sun.target.position.copy(this.lightTarget);
      this.sun.position.copy(this.lightTarget).addScaledVector(dir, 250);
      this.sun.target.updateMatrixWorld();
      this.dome.position.copy(this.game.camera.position);
    }
    return { night, horizon: k.hor };
  }
}
