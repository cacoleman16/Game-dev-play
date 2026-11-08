// Graphics presets and persistent settings. Phones default to "medium",
// desktops to "high"; the player can change it from the Settings menu.

import { isTouchDevice } from './util.js';

const dpr = typeof window !== 'undefined' ? window.devicePixelRatio || 1 : 1;

export const QUALITY = {
  low: {
    label: 'Low', pixelRatio: Math.min(dpr, 1), antialias: false, shadows: false, shadowSize: 512, shadowRange: 30,
    viewDist: 420, lodScale: 0.55, chunkBuildsPerFrame: 1, treeDensity: 0.4, propDensity: 0.4,
    grassTile: 10, grassTiles: 5, grassDensity: 2.5, grassHeight: 0.85, bloom: false, post: false, outlines: false, maxParticles: 400, npcOutlines: false,
  },
  medium: {
    label: 'Medium', pixelRatio: Math.min(dpr, 1.5), antialias: false, shadows: true, shadowSize: 1024, shadowRange: 38,
    viewDist: 560, lodScale: 0.75, chunkBuildsPerFrame: 2, treeDensity: 0.65, propDensity: 0.7,
    grassTile: 12, grassTiles: 6, grassDensity: 4.5, grassHeight: 0.95, bloom: false, outlines: true, maxParticles: 900, npcOutlines: false,
  },
  high: {
    label: 'High', pixelRatio: Math.min(dpr, 2), antialias: true, shadows: true, shadowSize: 2048, shadowRange: 50,
    viewDist: 760, lodScale: 1, chunkBuildsPerFrame: 3, treeDensity: 1, propDensity: 1,
    grassTile: 14, grassTiles: 8, grassDensity: 7, grassHeight: 1.0, bloom: true, outlines: true, maxParticles: 2000, npcOutlines: true,
  },
  ultra: {
    label: 'Ultra', pixelRatio: dpr, antialias: true, shadows: true, shadowSize: 4096, shadowRange: 64,
    viewDist: 980, lodScale: 1.35, chunkBuildsPerFrame: 4, treeDensity: 1.25, propDensity: 1.2,
    grassTile: 16, grassTiles: 9, grassDensity: 9, grassHeight: 1.05, bloom: true, outlines: true, maxParticles: 3000, npcOutlines: true,
  },
};

const SETTINGS_KEY = 'aetherveil.settings.v1';

export const DEFAULT_SETTINGS = {
  quality: null, // chosen automatically on first run
  masterVolume: 0.8,
  musicVolume: 0.55,
  sfxVolume: 0.9,
  voiceVolume: 1,
  voiceMode: 'speech', // speech | babble | off
  mouseSensitivity: 1,
  invertY: false,
  showFps: false,
  cameraShake: true,
  dayLengthMinutes: 24,
};

export function loadSettings() {
  let s = {};
  try { s = JSON.parse(localStorage.getItem(SETTINGS_KEY) || '{}'); } catch (e) { s = {}; }
  const out = { ...DEFAULT_SETTINGS, ...s };
  const forced = new URLSearchParams(location.search).get('quality');
  if (forced && QUALITY[forced]) out.quality = forced;
  if (!out.quality || !QUALITY[out.quality]) out.quality = detectQuality();
  return out;
}

export function saveSettings(s) {
  try { localStorage.setItem(SETTINGS_KEY, JSON.stringify(s)); } catch (e) { /* storage blocked */ }
}

export function detectQuality() {
  const mobile = isTouchDevice() || /Android|iPhone|iPad|iPod/i.test(navigator.userAgent);
  if (mobile) return 'medium';
  const cores = navigator.hardwareConcurrency || 4;
  return cores >= 6 ? 'high' : 'medium';
}
