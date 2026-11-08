// Stylised water: depth-tinted from turquoise shallows to deep blue, animated
// ripples, sun glints, fresnel sky reflection and foam that laps at the shore.

import * as THREE from 'three';
import { WATER_LEVEL } from './worldgen.js';
import { TERRAIN_GLSL } from './textures.js';
import { getDetailTexture } from './terrain.js';

export class Water {
  constructor(game) {
    this.game = game;
    const sky = game.sky;
    this.uniforms = THREE.UniformsUtils.merge([THREE.UniformsLib.fog, {
      uHeight: { value: game.worldTex.height },
      uDetail: { value: getDetailTexture() },
      uTime: { value: 0 },
      uSunDir: { value: sky.sunDir },
      uSunCol: { value: new THREE.Color() },
      uSky: { value: new THREE.Color() },
      uLight: { value: 1 },
      uLevel: { value: WATER_LEVEL },
    }]);
    this.uniforms.uTime = game.uniforms.uTime;
    this.uniforms.uSunDir.value = sky.sunDir;
    const mat = new THREE.ShaderMaterial({
      uniforms: this.uniforms,
      transparent: true,
      depthWrite: false,
      fog: true,
      vertexShader: /* glsl */`
        #include <fog_pars_vertex>
        varying vec3 vW;
        void main() {
          vec4 wp = modelMatrix * vec4(position, 1.0);
          vW = wp.xyz;
          vec4 mvPosition = viewMatrix * wp;
          gl_Position = projectionMatrix * mvPosition;
          #include <fog_vertex>
        }`,
      fragmentShader: /* glsl */`
        #include <fog_pars_fragment>
        uniform sampler2D uDetail;
        uniform float uTime, uLight, uLevel;
        uniform vec3 uSunDir, uSunCol, uSky;
        varying vec3 vW;
        ${TERRAIN_GLSL}
        void main() {
          vec2 p = vW.xz;
          float depth = uLevel - terrainHeight(p);
          if (depth < -0.05) discard;
          float nA = texture2D(uDetail, p * 0.031 + uTime * vec2(0.011, 0.007)).r;
          float nB = texture2D(uDetail, p * 0.047 - uTime * vec2(0.006, -0.012)).g;
          float nC = texture2D(uDetail, p * 0.11 + uTime * vec2(-0.02, 0.015)).b;
          vec3 nrm = normalize(vec3((nA - 0.5) * 0.9 + (nC - 0.5) * 0.5, 1.0, (nB - 0.5) * 0.9 + (nC - 0.5) * 0.4));
          vec3 V = normalize(cameraPosition - vW);
          float fres = pow(1.0 - max(dot(nrm, V), 0.0), 3.0);
          vec3 R = reflect(-V, nrm);
          float sunUp = smoothstep(-0.05, 0.1, uSunDir.y);
          float spec = pow(max(dot(R, uSunDir), 0.0), 180.0) * 3.0 + pow(max(dot(R, uSunDir), 0.0), 18.0) * 0.12;
          vec3 shallow = vec3(0.16, 0.72, 0.68);
          vec3 mid = vec3(0.03, 0.35, 0.55);
          vec3 deep = vec3(0.01, 0.10, 0.28);
          vec3 base = mix(shallow, mid, smoothstep(0.0, 3.5, depth));
          base = mix(base, deep, smoothstep(3.5, 16.0, depth));
          vec3 col = base * uLight;
          col = mix(col, uSky, fres * 0.55);
          col += uSunCol * spec * sunUp;
          // Foam bands that roll in toward the shore.
          float wave = sin(depth * 4.0 - uTime * 1.6 + nA * 4.0) * 0.5 + 0.5;
          float edge = 1.0 - smoothstep(0.0, 0.55 + nB * 0.4, depth);
          float bands = smoothstep(0.75, 0.95, wave) * (1.0 - smoothstep(0.3, 1.8, depth));
          float speck = smoothstep(0.62, 0.7, nC) * (1.0 - smoothstep(0.0, 2.5, depth));
          float foam = clamp(edge + bands * 0.7 + speck * 0.5, 0.0, 1.0);
          col = mix(col, vec3(0.95, 0.98, 1.0) * max(uLight, 0.25), foam * 0.85);
          float alpha = mix(0.5, 0.94, smoothstep(0.2, 6.0, depth));
          alpha = max(alpha, foam * 0.9);
          alpha *= smoothstep(-0.05, 0.08, depth);
          gl_FragColor = vec4(col, alpha);
          #include <tonemapping_fragment>
          #include <colorspace_fragment>
          #include <fog_fragment>
        }`,
    });
    const geo = new THREE.PlaneGeometry(7000, 7000, 1, 1);
    geo.rotateX(-Math.PI / 2);
    this.mesh = new THREE.Mesh(geo, mat);
    this.mesh.position.y = WATER_LEVEL;
    this.mesh.frustumCulled = false;
    this.mesh.renderOrder = 5;
    this.mesh.name = 'water';
  }

  update(camPos) {
    this.mesh.position.x = Math.round(camPos.x / 50) * 50;
    this.mesh.position.z = Math.round(camPos.z / 50) * 50;
    const sky = this.game.sky;
    this.uniforms.uSunCol.value.copy(sky.sun.color).multiplyScalar(Math.min(sky.sun.intensity, 3) * 0.5);
    this.uniforms.uSky.value.copy(sky.uniforms.uHor.value);
    this.uniforms.uLight.value = 0.3 + 0.7 * (1 - sky.uniforms.uNight.value);
  }
}
