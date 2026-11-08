// Post-processing: bloom (High+), then a cinematic grade (contrast curve,
// saturation, split toning, vignette, faint grain) and FXAA. Low quality
// renders directly with no composer.

import * as THREE from 'three';
import { EffectComposer, RenderPass, UnrealBloomPass, OutputPass, ShaderPass, FXAAShader } from 'three/addons';

const GradeShader = {
  uniforms: {
    tDiffuse: { value: null },
    uRes: { value: new THREE.Vector2(1, 1) },
    uTime: { value: 0 },
    uContrast: { value: 0.22 },
    uSat: { value: 1.0 },
    uVignette: { value: 0.28 },
    uShadow: { value: new THREE.Color(0x1a2a52) },
    uHighlight: { value: new THREE.Color(0xfff0d8) },
    uTone: { value: 0.07 },
    uFlash: { value: 0 },
    uHurt: { value: 0 },
  },
  vertexShader: /* glsl */`
    varying vec2 vUv;
    void main() { vUv = uv; gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0); }`,
  fragmentShader: /* glsl */`
    uniform sampler2D tDiffuse;
    uniform vec2 uRes;
    uniform float uTime, uContrast, uSat, uVignette, uTone, uFlash, uHurt;
    uniform vec3 uShadow, uHighlight;
    varying vec2 vUv;
    void main() {
      vec3 c = texture2D(tDiffuse, vUv).rgb;
      c = mix(c, c * c * (3.0 - 2.0 * c), uContrast);
      float l = dot(c, vec3(0.2126, 0.7152, 0.0722));
      c = mix(vec3(l), c, uSat);
      // Split toning: cool shadows, warm highlights.
      c = mix(c, c * (uShadow * 2.2), uTone * (1.0 - smoothstep(0.0, 0.45, l)));
      c = mix(c, c * uHighlight, uTone * 2.0 * smoothstep(0.45, 1.0, l));
      vec2 d = vUv - 0.5;
      d.x *= uRes.x / uRes.y;
      float r = length(d);
      c *= mix(1.0 - uVignette, 1.0, smoothstep(0.95, 0.3, r));
      c = mix(c, vec3(0.55, 0.0, 0.02), uHurt * smoothstep(0.35, 0.95, r) * 0.7);
      float g = fract(sin(dot(vUv * uRes + fract(uTime) * 91.7, vec2(12.9898, 78.233))) * 43758.5453);
      c += (g - 0.5) * 0.014;
      c += uFlash;
      gl_FragColor = vec4(c, 1.0);
    }`,
};

export class Post {
  constructor(game) {
    this.game = game;
    const q = game.quality;
    this.enabled = q.post !== false;
    if (!this.enabled) return;
    const r = game.renderer;
    const composer = new EffectComposer(r);
    this.renderPass = new RenderPass(game.scene, game.camera);
    composer.addPass(this.renderPass);
    if (q.bloom) {
      this.bloom = new UnrealBloomPass(new THREE.Vector2(window.innerWidth / 2, window.innerHeight / 2), 0.5, 0.45, 1.05);
      composer.addPass(this.bloom);
    }
    composer.addPass(new OutputPass());
    this.grade = new ShaderPass(GradeShader);
    composer.addPass(this.grade);
    this.fxaa = new ShaderPass(FXAAShader);
    composer.addPass(this.fxaa);
    this.composer = composer;
    this.flash = 0;
    this.hurt = 0;
    game.events.on('flash', (k) => { this.flash = Math.max(this.flash, k * 0.6); });
    game.events.on('player:hurt', (n) => { this.hurt = Math.min(1, this.hurt + 0.25 + n / 60); });
  }

  setSize(w, h) {
    if (!this.enabled) return;
    const pr = this.game.renderer.getPixelRatio();
    this.composer.setSize(w, h);
    if (this.bloom) this.bloom.resolution.set(w / 2, h / 2);
    this.fxaa.material.uniforms.resolution.value.set(1 / (w * pr), 1 / (h * pr));
    this.grade.uniforms.uRes.value.set(w * pr, h * pr);
  }

  render(dt) {
    const g = this.game;
    if (!this.enabled) { g.renderer.render(g.scene, g.camera); return; }
    this.renderPass.scene = g.scene;
    const u = this.grade.uniforms;
    u.uTime.value = g.time;
    this.flash = Math.max(0, this.flash - dt * 2.2);
    this.hurt = Math.max(0, this.hurt - dt * 1.6);
    u.uFlash.value = this.flash;
    const low = g.player && g.mode === 'play' ? 1 - g.player.stats.hp / Math.max(1, g.player.maxHp) : 0;
    u.uHurt.value = Math.max(this.hurt, low > 0.7 ? (low - 0.7) * 1.5 * (0.7 + 0.3 * Math.sin(g.time * 5)) : 0);
    // Night and dungeons: cooler, slightly less saturated, deeper vignette.
    const night = g.inDungeon ? 0.3 : g.sky.uniforms.uNight.value;
    u.uSat.value = 1.02 - night * 0.12;
    u.uVignette.value = 0.26 + night * 0.12 + (g.inDungeon ? 0.12 : 0);
    this.composer.render(dt);
  }
}
