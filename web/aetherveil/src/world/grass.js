// GPU grass: one tile of blades drawn many times (instanced) around the
// camera. The vertex shader reads terrain height and ground colour from
// textures, so blades sit on the land, match its tint, sway in the wind and
// part around the player. Some blades become wildflowers.

import * as THREE from 'three';
import { TERRAIN_GLSL } from './textures.js';
import { mulberry32 } from '../core/noise.js';

export class Grass {
  constructor(game) {
    this.game = game;
    const q = game.quality;
    this.tile = q.grassTile;
    this.tilesPerSide = q.grassTiles;
    const density = q.grassDensity; // blades per m^2
    const bladesPerTile = Math.round(this.tile * this.tile * density);
    const rand = mulberry32(42);

    // One blade = 5 vertices (two quads tapering to a tip) = 3 triangles.
    const shape = [-1, 0, 1, 0, -0.7, 0.45, 0.7, 0.45, 0, 1];
    const vcount = bladesPerTile * 5;
    const aShape = new Float32Array(vcount * 2);
    const aBlade = new Float32Array(vcount * 4);
    const idx = [];
    for (let b = 0; b < bladesPerTile; b++) {
      const bx = rand() * this.tile, bz = rand() * this.tile;
      const ang = rand() * Math.PI * 2;
      const r = rand();
      for (let v = 0; v < 5; v++) {
        const k = b * 5 + v;
        aShape[k * 2] = shape[v * 2];
        aShape[k * 2 + 1] = shape[v * 2 + 1];
        aBlade[k * 4] = bx; aBlade[k * 4 + 1] = bz; aBlade[k * 4 + 2] = ang; aBlade[k * 4 + 3] = r;
      }
      const o = b * 5;
      idx.push(o, o + 1, o + 3, o, o + 3, o + 2, o + 2, o + 3, o + 4);
    }
    const geo = new THREE.InstancedBufferGeometry();
    geo.setAttribute('position', new THREE.BufferAttribute(new Float32Array(vcount * 3), 3));
    geo.setAttribute('aShape', new THREE.BufferAttribute(aShape, 2));
    geo.setAttribute('aBlade', new THREE.BufferAttribute(aBlade, 4));
    geo.setIndex(idx);
    const nTiles = this.tilesPerSide * this.tilesPerSide;
    this.tileAttr = new THREE.InstancedBufferAttribute(new Float32Array(nTiles * 2), 2);
    this.tileAttr.setUsage(THREE.DynamicDrawUsage);
    geo.setAttribute('aTile', this.tileAttr);
    geo.instanceCount = nTiles;

    this.uniforms = THREE.UniformsUtils.merge([THREE.UniformsLib.fog, {
      uHeight: { value: game.worldTex.height },
      uGrass: { value: game.worldTex.grass },
      uTime: { value: 0 },
      uPlayer: { value: new THREE.Vector3() },
      uCam: { value: new THREE.Vector3() },
      uRadius: { value: (this.tile * this.tilesPerSide) / 2 - 2 },
      uSunCol: { value: new THREE.Color() },
      uSunDir: { value: game.sky.sunDir },
      uAmb: { value: new THREE.Color() },
      uHeightScale: { value: q.grassHeight },
    }]);
    this.uniforms.uTime = game.uniforms.uTime;
    this.uniforms.uSunDir.value = game.sky.sunDir;

    const mat = new THREE.ShaderMaterial({
      uniforms: this.uniforms,
      side: THREE.DoubleSide,
      fog: true,
      vertexShader: /* glsl */`
        #include <fog_pars_vertex>
        attribute vec2 aShape;
        attribute vec4 aBlade;
        attribute vec2 aTile;
        uniform sampler2D uGrass;
        uniform float uTime, uRadius, uHeightScale;
        uniform vec3 uPlayer, uCam;
        varying vec3 vCol;
        varying float vT;
        ${TERRAIN_GLSL}
        void main() {
          vec2 wxz = aTile + aBlade.xy;
          vec4 g = texture2D(uGrass, worldUV(wxz));
          float rnd = aBlade.w;
          float keep = step(rnd, g.a * 1.05);
          float dist = length(wxz - uCam.xz);
          float fade = 1.0 - smoothstep(uRadius * 0.55, uRadius, dist);
          float r2 = fract(rnd * 13.37);
          float hgt = (0.2 + 0.4 * r2) * keep * fade * uHeightScale * (0.6 + 0.4 * g.a);
          float y = terrainHeight(wxz);
          float t = aShape.y;
          float w = 0.085 * (1.0 - t * 0.8) * (0.75 + r2 * 0.6);
          vec2 dir = vec2(cos(aBlade.z), sin(aBlade.z));
          vec3 p = vec3(wxz.x + dir.y * aShape.x * w, y + t * hgt, wxz.y - dir.x * aShape.x * w);
          float gust = sin(uTime * 0.7 + wxz.x * 0.02 + wxz.y * 0.015) * 0.5 + 0.5;
          float wind = sin(uTime * 2.1 + wxz.x * 0.21 + wxz.y * 0.17) * (0.35 + gust * 0.65) + sin(uTime * 3.7 + wxz.x * 0.7) * 0.15;
          vec2 lean = vec2(fract(rnd * 37.1) - 0.5, fract(rnd * 53.9) - 0.5) * 0.9;
          vec2 bend = (vec2(0.9, 0.45) * wind * 0.32 + lean) * t * t * hgt;
          vec2 away = wxz - uPlayer.xz;
          float pd = length(away);
          float push = (1.0 - smoothstep(0.2, 1.3, pd)) * step(abs(uPlayer.y - y), 2.5);
          bend += normalize(away + 1e-4) * push * t * hgt * 0.9;
          p.xz += bend;
          p.y -= push * t * hgt * 0.35;
          vec3 base = g.rgb;
          // Some blades in lush meadows become wildflowers.
          float flower = step(0.965, r2) * step(0.6, g.a) * step(g.r, g.g);
          vec3 fc = fract(rnd * 91.7) < 0.33 ? vec3(1.0, 0.85, 0.25) : (fract(rnd * 91.7) < 0.66 ? vec3(0.95, 0.55, 0.85) : vec3(0.95, 0.97, 1.0));
          vec3 tip = mix(base * 1.35 + vec3(0.04, 0.05, 0.0), fc, flower * smoothstep(0.7, 1.0, t));
          vCol = mix(base * 0.55, tip, t);
          vT = t;
          vec4 mvPosition = viewMatrix * vec4(p, 1.0);
          gl_Position = projectionMatrix * mvPosition;
          #include <fog_vertex>
        }`,
      fragmentShader: /* glsl */`
        #include <fog_pars_fragment>
        uniform vec3 uSunCol, uAmb, uSunDir;
        varying vec3 vCol;
        varying float vT;
        void main() {
          float ndl = max(uSunDir.y, 0.0) * 0.7 + 0.3;
          vec3 light = uAmb + uSunCol * ndl * (0.55 + 0.45 * vT);
          vec3 col = vCol * light;
          gl_FragColor = vec4(col, 1.0);
          #include <tonemapping_fragment>
          #include <colorspace_fragment>
          #include <fog_fragment>
        }`,
    });
    this.mesh = new THREE.Mesh(geo, mat);
    this.mesh.frustumCulled = false;
    this.mesh.name = 'grass';
    this.lastKey = '';
  }

  update(camPos, playerPos) {
    const T = this.tile, n = this.tilesPerSide;
    const tx = Math.floor(camPos.x / T), tz = Math.floor(camPos.z / T);
    const key = tx + ',' + tz;
    if (key !== this.lastKey) {
      this.lastKey = key;
      const a = this.tileAttr.array;
      const h = Math.floor(n / 2);
      let k = 0;
      for (let j = 0; j < n; j++) {
        for (let i = 0; i < n; i++) {
          a[k++] = (tx + i - h) * T;
          a[k++] = (tz + j - h) * T;
        }
      }
      this.tileAttr.needsUpdate = true;
    }
    this.uniforms.uCam.value.copy(camPos);
    this.uniforms.uPlayer.value.copy(playerPos);
    const sky = this.game.sky;
    this.uniforms.uSunCol.value.copy(sky.sun.color).multiplyScalar(sky.sun.intensity * 0.42);
    this.uniforms.uAmb.value.copy(sky.hemi.color).multiplyScalar(sky.hemi.intensity * 0.55);
  }
}
