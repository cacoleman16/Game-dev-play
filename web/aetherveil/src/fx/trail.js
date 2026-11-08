// Sword trail: a ribbon swept between the blade's base and tip over the last
// few frames, fading with age. Additive, so it glows under bloom.

import * as THREE from 'three';

const N = 18;

export class SwordTrail {
  constructor(scene) {
    this.base = []; this.tip = []; this.age = [];
    const geo = new THREE.BufferGeometry();
    this.pos = new Float32Array(N * 2 * 3);
    this.alpha = new Float32Array(N * 2);
    geo.setAttribute('position', new THREE.BufferAttribute(this.pos, 3).setUsage(THREE.DynamicDrawUsage));
    geo.setAttribute('aAlpha', new THREE.BufferAttribute(this.alpha, 1).setUsage(THREE.DynamicDrawUsage));
    const idx = [];
    for (let i = 0; i < N - 1; i++) { const a = i * 2; idx.push(a, a + 1, a + 2, a + 1, a + 3, a + 2); }
    geo.setIndex(idx);
    this.mat = new THREE.ShaderMaterial({
      uniforms: { uColor: { value: new THREE.Color(0xdfe8ff) } },
      vertexShader: 'attribute float aAlpha; varying float vA; void main(){ vA = aAlpha; gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0); }',
      fragmentShader: 'uniform vec3 uColor; varying float vA; void main(){ gl_FragColor = vec4(uColor * 1.6, vA * 0.55); }',
      transparent: true, depthWrite: false, blending: THREE.AdditiveBlending, side: THREE.DoubleSide,
    });
    this.mesh = new THREE.Mesh(geo, this.mat);
    this.mesh.frustumCulled = false;
    this.mesh.renderOrder = 25;
    scene.add(this.mesh);
    this.active = false;
  }

  setColor(c) { this.mat.uniforms.uColor.value.set(c); }
  attach(scene) { if (this.mesh.parent !== scene) scene.add(this.mesh); }

  // Push this frame's blade segment (world space) while swinging.
  push(b, t) {
    this.base.unshift(b.clone()); this.tip.unshift(t.clone()); this.age.unshift(0);
    if (this.base.length > N) { this.base.pop(); this.tip.pop(); this.age.pop(); }
  }

  update(dt) {
    for (let i = 0; i < this.age.length; i++) this.age[i] += dt;
    while (this.age.length && this.age[this.age.length - 1] > 0.18) { this.base.pop(); this.tip.pop(); this.age.pop(); }
    const n = this.base.length;
    this.mesh.visible = n > 1;
    if (n < 2) return;
    for (let i = 0; i < N; i++) {
      const k = Math.min(i, n - 1);
      const b = this.base[k], t = this.tip[k];
      this.pos.set([b.x, b.y, b.z], i * 6);
      this.pos.set([t.x, t.y, t.z], i * 6 + 3);
      const a = i < n ? Math.max(0, 1 - this.age[k] / 0.18) * (1 - i / N) : 0;
      this.alpha[i * 2] = a * 0.25;
      this.alpha[i * 2 + 1] = a;
    }
    this.mesh.geometry.attributes.position.needsUpdate = true;
    this.mesh.geometry.attributes.aAlpha.needsUpdate = true;
  }
}
