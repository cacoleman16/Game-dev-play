// GPU copies of the world data, so shaders (grass, water) can read terrain
// height and ground colour directly.

import * as THREE from 'three';
import { N, HALF, CELL, WORLD_SIZE } from './worldgen.js';

export function makeWorldTextures(world) {
  const height = new THREE.DataTexture(world.H, N, N, THREE.RedFormat, THREE.FloatType);
  height.magFilter = THREE.NearestFilter;
  height.minFilter = THREE.NearestFilter;
  height.generateMipmaps = false;
  height.needsUpdate = true;

  const g = new Uint8Array(N * N * 4);
  for (let i = 0; i < N * N; i++) {
    g[i * 4] = world.color[i * 3];
    g[i * 4 + 1] = world.color[i * 3 + 1];
    g[i * 4 + 2] = world.color[i * 3 + 2];
    g[i * 4 + 3] = world.grass[i];
  }
  const grass = new THREE.DataTexture(g, N, N, THREE.RGBAFormat);
  grass.colorSpace = THREE.SRGBColorSpace;
  grass.magFilter = THREE.LinearFilter;
  grass.minFilter = THREE.LinearFilter;
  grass.generateMipmaps = false;
  grass.needsUpdate = true;
  return { height, grass };
}

// GLSL helper shared by shaders that need the terrain height.
export const TERRAIN_GLSL = /* glsl */`
  uniform sampler2D uHeight;
  float terrainHeight(vec2 xz) {
    vec2 g = (xz + ${HALF.toFixed(1)}) / ${CELL.toFixed(1)};
    if (g.x < 0.0 || g.y < 0.0 || g.x > ${(N - 1.001).toFixed(3)} || g.y > ${(N - 1.001).toFixed(3)}) return -30.0;
    ivec2 i = ivec2(floor(g));
    vec2 f = fract(g);
    float h00 = texelFetch(uHeight, i, 0).r;
    float h10 = texelFetch(uHeight, i + ivec2(1, 0), 0).r;
    float h01 = texelFetch(uHeight, i + ivec2(0, 1), 0).r;
    float h11 = texelFetch(uHeight, i + ivec2(1, 1), 0).r;
    if (f.x > f.y) return h00 + f.x * (h10 - h00) + f.y * (h11 - h10);
    return h00 + f.y * (h01 - h00) + f.x * (h11 - h01);
  }
  vec2 worldUV(vec2 xz) { return (xz + ${HALF.toFixed(1)}) / ${WORLD_SIZE.toFixed(1)}; }
`;
