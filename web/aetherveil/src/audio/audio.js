// Aetherveil's sound, all synthesised with WebAudio:
//  - instruments: plucked harp (Karplus-Strong, rendered to buffers), FM
//    bells, breathy flute, bowed strings, choir and string pads, taiko drums;
//  - composed themes for each region and mood, crossfaded by a sequencer;
//  - 3D-positioned effects (PannerNodes follow the camera as the listener);
//  - voices: spoken dialogue through the browser's speech engine with a
//    distinct voice per character (or a formant "babble" fallback), plus
//    formant-synthesised barks for the hero and creatures.

const NOTE = (n) => 440 * Math.pow(2, (n - 69) / 12);
const clamp = (v, a, b) => Math.max(a, Math.min(b, v));

// Vowel formants (F1, F2, F3) for the voice synth.
const VOWELS = { a: [800, 1200, 2500], e: [500, 1900, 2600], i: [320, 2300, 3000], o: [500, 900, 2400], u: [350, 750, 2400], ah: [700, 1100, 2450] };

// ------------------------------------------------------------------ themes
// Melodies: [semitones from key | null for rest, length in eighths].
// Chords per bar: [root offset, 'maj'|'min'|'dim'|'sus'].
const THEMES = {
  explore: {
    name: "Wayfarer's Road", bpm: 92, key: 62, bars: 8,
    chords: [[0, 'maj'], [7, 'maj'], [9, 'min'], [5, 'maj'], [0, 'maj'], [2, 'min'], [5, 'maj'], [7, 'maj']],
    melody: [[7, 3], [4, 1], [7, 2], [12, 2], [11, 4], [9, 2], [7, 2], [9, 3], [7, 1], [4, 2], [7, 2], [5, 6], [null, 2],
      [4, 3], [5, 1], [7, 2], [12, 2], [14, 4], [12, 2], [9, 2], [12, 3], [11, 1], [9, 2], [5, 2], [7, 6], [null, 2]],
    lead: 'flute', arp: [0, 2, 1, 2, 3, 2, 1, 2], arpInst: 'harp', bass: [0, null, null, null, 7, null, null, null], pad: 'strings', drums: null, leadVol: 0.9,
  },
  town: {
    name: 'Brightwater', bpm: 104, key: 65, bars: 8,
    chords: [[0, 'maj'], [7, 'maj'], [9, 'min'], [5, 'maj'], [0, 'maj'], [7, 'maj'], [5, 'maj'], [0, 'maj']],
    melody: [[7, 1], [4, 1], [7, 2], [12, 2], [11, 2], [9, 2], [7, 2], [2, 4], [4, 1], [5, 1], [7, 2], [9, 2], [12, 2], [11, 2], [9, 2], [7, 4],
      [4, 2], [7, 2], [12, 2], [16, 2], [14, 2], [11, 2], [7, 4], [9, 2], [12, 2], [11, 2], [14, 2], [12, 6], [null, 2]],
    lead: 'pluckLead', arp: [0, 1, 2, 1, 0, 1, 2, 1], arpInst: 'harp', bass: [0, null, 7, null, 0, null, 7, null], pad: 'warm', drums: { k: 'x...x...', s: '....x...', h: '..x...x.' }, drumVol: 0.35, leadVol: 0.8,
  },
  night: {
    name: 'Starlit Meadow', bpm: 66, key: 57, bars: 8,
    chords: [[0, 'min'], [0, 'min'], [8, 'maj'], [8, 'maj'], [3, 'maj'], [3, 'maj'], [10, 'maj'], [7, 'maj']],
    melody: [[19, 4], [17, 2], [15, 2], [12, 6], [null, 2], [10, 4], [15, 2], [14, 2], [7, 6], [null, 2],
      [8, 2], [12, 2], [15, 4], [14, 2], [12, 2], [10, 4], [12, 3], [14, 1], [15, 2], [19, 2], [12, 8]],
    lead: 'bell', arp: [0, null, 2, null, 1, null, 2, null], arpInst: 'harp', bass: [0, null, null, null, null, null, null, null], pad: 'choir', drums: null, leadVol: 0.7,
  },
  forest: {
    name: 'Whisperwood', bpm: 78, key: 52, bars: 8,
    chords: [[0, 'min'], [0, 'min'], [10, 'maj'], [10, 'maj'], [8, 'maj'], [8, 'maj'], [10, 'maj'], [10, 'sus']],
    melody: [[19, 4], [17, 2], [15, 2], [14, 6], [null, 2], [12, 2], [14, 2], [15, 2], [17, 2], [19, 6], [null, 2],
      [22, 4], [21, 2], [19, 2], [17, 4], [14, 4], [15, 2], [14, 2], [12, 2], [10, 2], [12, 8]],
    lead: 'flute', arp: [0, 2, 3, 2, 1, 2, 3, 2], arpInst: 'harp', bass: [0, null, null, null, null, null, 7, null], pad: 'strings', drums: null, leadVol: 0.75,
  },
  mountain: {
    name: 'Frostspire', bpm: 72, key: 59, bars: 8,
    chords: [[0, 'min'], [8, 'maj'], [3, 'maj'], [10, 'maj'], [0, 'min'], [8, 'maj'], [5, 'min'], [7, 'maj']],
    melody: [[19, 4], [17, 2], [15, 2], [12, 8], [15, 3], [14, 1], [12, 2], [10, 2], [7, 8],
      [8, 2], [12, 2], [15, 2], [20, 2], [19, 6], [null, 2], [17, 2], [15, 2], [14, 2], [10, 2], [12, 8]],
    lead: 'bell', arp: [0, null, 1, null, 2, null, 3, null], arpInst: 'bell', bass: [0, null, null, null, null, null, null, null], pad: 'choir', drums: null, leadVol: 0.8,
  },
  desert: {
    name: 'Sunscar', bpm: 96, key: 62, bars: 4,
    chords: [[0, 'maj'], [1, 'maj'], [0, 'maj'], [10, 'min']],
    melody: [[7, 2], [8, 1], [7, 1], [5, 2], [4, 2], [5, 2], [4, 2], [1, 2], [0, 2], [12, 3], [10, 1], [8, 2], [7, 2], [8, 4], [7, 4]],
    lead: 'pluckLead', arp: [0, null, 0, 1, null, 0, 2, null], arpInst: 'harp', bass: [0, null, 0, null, 0, null, 0, null], pad: 'warm', drums: { k: 'x..x..x.', s: '....x..x', h: 'x.x.x.x.' }, drumVol: 0.4, leadVol: 0.85,
  },
  dungeon: {
    name: 'The Vault Below', bpm: 64, key: 50, bars: 4,
    chords: [[0, 'min'], [1, 'maj'], [0, 'min'], [11, 'dim']],
    melody: [[12, 6], [null, 2], [13, 4], [12, 4], [null, 4], [15, 2], [13, 2], [11, 8]],
    lead: 'bell', arp: [0, null, null, 1, null, null, 2, null], arpInst: 'harp', bass: [0, null, null, null, null, null, null, null], pad: 'choir', drums: { k: 'x.......', s: '', h: '' }, drumVol: 0.3, leadVol: 0.55,
  },
  combat: {
    name: 'Steel and Aether', bpm: 140, key: 57, bars: 4,
    chords: [[0, 'min'], [8, 'maj'], [10, 'maj'], [7, 'maj']],
    melody: [[19, 2], [19, 1], [17, 1], [15, 2], [14, 2], [15, 2], [12, 2], [12, 4], [14, 2], [15, 2], [17, 2], [22, 2], [11, 6], [null, 2]],
    lead: 'strings', arp: [0, 0, 1, 0, 2, 0, 1, 0], arpInst: 'stab', bass: [0, 0, 0, 0, 0, 0, 0, 0], pad: 'strings', drums: { k: 'x..x.x..', s: '....x...', h: 'xxxxxxxx' }, drumVol: 0.6, leadVol: 0.8,
  },
  boss: {
    name: 'Guardian', bpm: 152, key: 52, bars: 4,
    chords: [[0, 'min'], [8, 'maj'], [5, 'min'], [7, 'maj']],
    melody: [[19, 1], [19, 1], [20, 2], [19, 2], [15, 2], [14, 2], [15, 2], [12, 4], [24, 2], [22, 2], [20, 2], [19, 2], [23, 6], [null, 2]],
    lead: 'strings', arp: [0, 0, 2, 0, 1, 0, 2, 1], arpInst: 'stab', bass: [0, 0, 0, 0, 0, 0, 7, 7], pad: 'choir', drums: { k: 'x.xx..x.', s: '....x..x', h: 'x.x.x.x.' }, drumVol: 0.75, leadVol: 0.85,
  },
  final: {
    name: 'The Hollow Herald', bpm: 160, key: 50, bars: 4,
    chords: [[0, 'min'], [10, 'maj'], [3, 'maj'], [9, 'maj']],
    melody: [[19, 2], [24, 2], [22, 2], [19, 2], [20, 4], [19, 4], [15, 2], [17, 2], [19, 2], [24, 2], [23, 6], [null, 2]],
    lead: 'strings', arp: [0, 1, 2, 1, 0, 1, 2, 3], arpInst: 'stab', bass: [0, 0, 0, 0, 0, 0, 0, 0], pad: 'choir', drums: { k: 'x.x.x.xx', s: '....x...', h: 'xxxxxxxx' }, drumVol: 0.8, leadVol: 0.9,
  },
  victory: {
    name: 'Dawn Over Lumeria', bpm: 96, key: 62, bars: 8,
    chords: [[0, 'maj'], [5, 'maj'], [9, 'min'], [7, 'maj'], [0, 'maj'], [5, 'maj'], [7, 'maj'], [0, 'maj']],
    melody: [[12, 4], [14, 2], [16, 2], [19, 6], [16, 2], [17, 4], [16, 2], [14, 2], [12, 8], [12, 2], [14, 2], [16, 2], [19, 2], [21, 4], [19, 4], [17, 2], [16, 2], [14, 4], [12, 8]],
    lead: 'flute', arp: [0, 1, 2, 3, 2, 1, 0, 1], arpInst: 'harp', bass: [0, null, null, null, 7, null, null, null], pad: 'strings', drums: null, leadVol: 0.9,
  },
};
const CHORD = { maj: [0, 4, 7, 12], min: [0, 3, 7, 12], dim: [0, 3, 6, 9], sus: [0, 5, 7, 12] };

// NPC voice profiles for spoken dialogue.
export const VOICES = {
  maren: { g: 'f', pitch: 0.85, rate: 0.86, f0: 190 },
  lyra: { g: 'f', pitch: 1.12, rate: 1.06, f0: 230 },
  garrick: { g: 'm', pitch: 0.62, rate: 0.9, f0: 95 },
  sera: { g: 'f', pitch: 1.05, rate: 0.9, f0: 220 },
  mira: { g: 'f', pitch: 1.25, rate: 1.0, f0: 250 },
  dain: { g: 'm', pitch: 0.82, rate: 1.0, f0: 115 },
  pip: { g: 'm', pitch: 1.7, rate: 1.15, f0: 300 },
  tobin: { g: 'm', pitch: 0.92, rate: 0.95, f0: 125 },
  ysolde: { g: 'f', pitch: 1.08, rate: 1.1, f0: 225 },
  bram: { g: 'm', pitch: 0.72, rate: 0.88, f0: 105 },
  wick: { g: 'm', pitch: 0.78, rate: 0.82, f0: 110 },
  marin: { g: 'f', pitch: 0.98, rate: 1.04, f0: 205 },
  aveline: { g: 'f', pitch: 0.95, rate: 0.84, f0: 210 },
  narrator: { g: 'f', pitch: 0.9, rate: 0.9, f0: 200 },
  herald: { g: 'm', pitch: 0.4, rate: 0.78, f0: 70 },
};

export class Audio {
  constructor(game) {
    this.game = game;
    this.ctx = null;
    this.ready = false;
    this.mood = null;
    this.forced = null;
    this.rain = 0;
    this.pluckCache = new Map();
    this.voices = [];
    if (typeof speechSynthesis !== 'undefined') {
      const load = () => { this.voices = speechSynthesis.getVoices().filter((v) => /^en/i.test(v.lang)); };
      load();
      speechSynthesis.addEventListener?.('voiceschanged', load);
    }
  }

  // Must be called from a user gesture.
  init() {
    if (this.ctx) { if (this.ctx.state === 'suspended') this.ctx.resume(); return; }
    const AC = window.AudioContext || window.webkitAudioContext;
    if (!AC) return;
    const ctx = new AC();
    this.ctx = ctx;
    this.comp = ctx.createDynamicsCompressor();
    this.comp.threshold.value = -14; this.comp.knee.value = 12; this.comp.ratio.value = 3.5; this.comp.attack.value = 0.005; this.comp.release.value = 0.25;
    this.master = ctx.createGain();
    this.master.connect(this.comp); this.comp.connect(ctx.destination);
    this.sfx = ctx.createGain(); this.sfx.connect(this.master);
    this.musicBus = ctx.createGain(); this.musicBus.connect(this.master);
    this.amb = ctx.createGain(); this.amb.connect(this.master);
    this.vox = ctx.createGain(); this.vox.connect(this.master);
    this.reverb = ctx.createConvolver();
    this.reverb.buffer = this.makeImpulse(3.2, 2.4);
    this.revGain = ctx.createGain(); this.revGain.gain.value = 0.32;
    this.reverb.connect(this.revGain); this.revGain.connect(this.master);
    this.noiseBuf = this.makeNoise(2);
    this.applyVolumes();
    this.startAmbience();
    this.ready = true;
    this.seq = { theme: null, step: 0, next: ctx.currentTime + 0.3, gain: null };
  }

  applyVolumes() {
    if (!this.ctx) return;
    const s = this.game.settings;
    this.master.gain.value = s.masterVolume;
    this.sfx.gain.value = s.sfxVolume;
    this.musicBus.gain.value = s.musicVolume * 0.85;
    this.amb.gain.value = 0.55;
    this.vox.gain.value = s.voiceVolume ?? 1;
  }

  makeNoise(sec) {
    const ctx = this.ctx;
    const b = ctx.createBuffer(1, ctx.sampleRate * sec, ctx.sampleRate);
    const d = b.getChannelData(0);
    for (let i = 0; i < d.length; i++) d[i] = Math.random() * 2 - 1;
    return b;
  }

  makeImpulse(sec, decay) {
    const ctx = this.ctx;
    const len = Math.floor(ctx.sampleRate * sec);
    const b = ctx.createBuffer(2, len, ctx.sampleRate);
    for (let c = 0; c < 2; c++) {
      const d = b.getChannelData(c);
      let lp = 0;
      for (let i = 0; i < len; i++) {
        lp = lp * 0.6 + (Math.random() * 2 - 1) * 0.4; // darker tail
        d[i] = lp * Math.pow(1 - i / len, decay) * (i < ctx.sampleRate * 0.01 ? i / (ctx.sampleRate * 0.01) : 1);
      }
    }
    return b;
  }

  // ------------------------------------------------------------------ 3D
  spatial(pos, bus) {
    const p = this.ctx.createPanner();
    p.panningModel = 'equalpower';
    p.distanceModel = 'inverse';
    p.refDistance = 4; p.maxDistance = 80; p.rolloffFactor = 1.3;
    if (p.positionX) { p.positionX.value = pos.x; p.positionY.value = pos.y; p.positionZ.value = pos.z; }
    else p.setPosition(pos.x, pos.y, pos.z);
    p.connect(bus || this.sfx);
    setTimeout(() => p.disconnect(), 4000);
    return p;
  }

  updateListener() {
    const cam = this.game.camera, L = this.ctx.listener;
    const f = this._f || (this._f = cam.position.clone());
    cam.getWorldDirection(f);
    if (L.positionX) {
      const t = this.ctx.currentTime;
      L.positionX.setTargetAtTime(cam.position.x, t, 0.02); L.positionY.setTargetAtTime(cam.position.y, t, 0.02); L.positionZ.setTargetAtTime(cam.position.z, t, 0.02);
      L.forwardX.setTargetAtTime(f.x, t, 0.02); L.forwardY.setTargetAtTime(f.y, t, 0.02); L.forwardZ.setTargetAtTime(f.z, t, 0.02);
      L.upX.value = 0; L.upY.value = 1; L.upZ.value = 0;
    } else {
      L.setPosition(cam.position.x, cam.position.y, cam.position.z);
      L.setOrientation(f.x, f.y, f.z, 0, 1, 0);
    }
  }

  // ------------------------------------------------------------------ primitives
  tone({ f = 440, f2 = null, type = 'sine', dur = 0.2, vol = 0.3, attack = 0.005, out = null, when = 0, rev = 0, detune = 0 }) {
    const ctx = this.ctx;
    const t = ctx.currentTime + Math.max(0, when);
    const o = ctx.createOscillator();
    o.type = type;
    o.frequency.setValueAtTime(clamp(f, 10, 20000), t);
    o.detune.value = detune;
    if (f2) o.frequency.exponentialRampToValueAtTime(clamp(f2, 10, 20000), t + dur);
    const g = ctx.createGain();
    g.gain.setValueAtTime(0.0001, t);
    g.gain.exponentialRampToValueAtTime(Math.max(vol, 0.0002), t + attack);
    g.gain.exponentialRampToValueAtTime(0.0001, t + dur);
    o.connect(g);
    g.connect(out || this.sfx);
    if (rev) { const r = ctx.createGain(); r.gain.value = rev; g.connect(r); r.connect(this.reverb); }
    o.start(t);
    o.stop(t + dur + 0.05);
    return g;
  }

  noise({ dur = 0.2, vol = 0.3, type = 'bandpass', f = 1000, f2 = null, q = 1, attack = 0.005, out = null, when = 0, rev = 0 }) {
    const ctx = this.ctx;
    const t = ctx.currentTime + Math.max(0, when);
    const s = ctx.createBufferSource();
    s.buffer = this.noiseBuf;
    s.loop = true;
    const fl = ctx.createBiquadFilter();
    fl.type = type; fl.Q.value = q;
    fl.frequency.setValueAtTime(clamp(f, 20, 20000), t);
    if (f2) fl.frequency.exponentialRampToValueAtTime(clamp(f2, 20, 20000), t + dur);
    const g = ctx.createGain();
    g.gain.setValueAtTime(0.0001, t);
    g.gain.exponentialRampToValueAtTime(Math.max(vol, 0.0002), t + attack);
    g.gain.exponentialRampToValueAtTime(0.0001, t + dur);
    s.connect(fl); fl.connect(g); g.connect(out || this.sfx);
    if (rev) { const r = ctx.createGain(); r.gain.value = rev; g.connect(r); r.connect(this.reverb); }
    s.start(t, Math.random());
    s.stop(t + dur + 0.05);
  }

  // Karplus-Strong plucked string rendered once per note and cached.
  pluckBuffer(midi, bright = 0.5) {
    const key = midi + ':' + bright;
    if (this.pluckCache.has(key)) return this.pluckCache.get(key);
    const sr = this.ctx.sampleRate, f = NOTE(midi);
    const len = Math.floor(sr * 2.4);
    const buf = this.ctx.createBuffer(1, len, sr);
    const d = buf.getChannelData(0);
    const N = Math.max(2, Math.round(sr / f));
    const ring = new Float32Array(N);
    for (let i = 0; i < N; i++) ring[i] = (Math.random() * 2 - 1) * (0.6 + bright * 0.4);
    const decay = 0.996 - (midi > 72 ? 0.002 : 0);
    let idx = 0, prev = 0;
    for (let i = 0; i < len; i++) {
      const cur = ring[idx];
      const nxt = ring[(idx + 1) % N];
      const v = (cur + nxt) * 0.5 * decay;
      ring[idx] = v * (0.5 + bright * 0.5) + prev * (0.5 - bright * 0.5);
      prev = v;
      d[i] = cur;
      idx = (idx + 1) % N;
    }
    this.pluckCache.set(key, buf);
    return buf;
  }

  pluck(midi, when, vol, out, bright = 0.5, rev = 0.35) {
    const ctx = this.ctx, t = ctx.currentTime + Math.max(0, when);
    const s = ctx.createBufferSource();
    s.buffer = this.pluckBuffer(midi, bright);
    const g = ctx.createGain(); g.gain.value = vol;
    s.connect(g); g.connect(out);
    if (rev) { const r = ctx.createGain(); r.gain.value = rev; g.connect(r); r.connect(this.reverb); }
    s.start(t);
    s.stop(t + 2.4);
  }

  bell(midi, when, vol, out, dur = 2.2) {
    const ctx = this.ctx, t = ctx.currentTime + Math.max(0, when);
    const f = NOTE(midi);
    const car = ctx.createOscillator(), mod = ctx.createOscillator(), mg = ctx.createGain(), g = ctx.createGain();
    car.frequency.value = f; mod.frequency.value = f * 3.5;
    mg.gain.setValueAtTime(f * 2.2, t); mg.gain.exponentialRampToValueAtTime(f * 0.05, t + dur);
    mod.connect(mg); mg.connect(car.frequency);
    g.gain.setValueAtTime(0.0001, t); g.gain.exponentialRampToValueAtTime(vol, t + 0.004); g.gain.exponentialRampToValueAtTime(0.0001, t + dur);
    car.connect(g); g.connect(out);
    const r = ctx.createGain(); r.gain.value = 0.5; g.connect(r); r.connect(this.reverb);
    car.start(t); mod.start(t); car.stop(t + dur + 0.05); mod.stop(t + dur + 0.05);
  }

  flute(midi, when, dur, vol, out) {
    const ctx = this.ctx, t = ctx.currentTime + Math.max(0, when);
    const f = NOTE(midi);
    const o = ctx.createOscillator(); o.type = 'triangle'; o.frequency.value = f;
    const o2 = ctx.createOscillator(); o2.type = 'sine'; o2.frequency.value = f * 2; const g2 = ctx.createGain(); g2.gain.value = 0.15;
    const vib = ctx.createOscillator(), vg = ctx.createGain(); vib.frequency.value = 5.2; vg.gain.setValueAtTime(0, t); vg.gain.linearRampToValueAtTime(f * 0.012, t + Math.min(0.4, dur * 0.6));
    vib.connect(vg); vg.connect(o.frequency); vg.connect(o2.frequency);
    const g = ctx.createGain();
    g.gain.setValueAtTime(0.0001, t); g.gain.exponentialRampToValueAtTime(vol, t + 0.07); g.gain.setValueAtTime(vol, t + Math.max(0.08, dur - 0.08)); g.gain.exponentialRampToValueAtTime(0.0001, t + dur + 0.25);
    const lp = ctx.createBiquadFilter(); lp.type = 'lowpass'; lp.frequency.value = 3200;
    o.connect(g); o2.connect(g2); g2.connect(g); g.connect(lp); lp.connect(out);
    const r = ctx.createGain(); r.gain.value = 0.45; lp.connect(r); r.connect(this.reverb);
    this.noise({ dur: 0.12, vol: vol * 0.25, f: f * 2, q: 2, out, when });
    for (const x of [o, o2, vib]) { x.start(t); x.stop(t + dur + 0.3); }
  }

  strings(midi, when, dur, vol, out, attack = 0.12) {
    const ctx = this.ctx, t = ctx.currentTime + Math.max(0, when);
    const f = NOTE(midi);
    const lp = ctx.createBiquadFilter(); lp.type = 'lowpass'; lp.frequency.value = 2400; lp.Q.value = 0.7;
    const g = ctx.createGain();
    g.gain.setValueAtTime(0.0001, t); g.gain.exponentialRampToValueAtTime(vol, t + attack); g.gain.setValueAtTime(vol, t + Math.max(attack, dur - 0.1)); g.gain.exponentialRampToValueAtTime(0.0001, t + dur + 0.3);
    const vib = ctx.createOscillator(), vg = ctx.createGain(); vib.frequency.value = 5.5; vg.gain.value = f * 0.006;
    vib.connect(vg);
    const oscs = [-9, 0, 8].map((det) => { const o = ctx.createOscillator(); o.type = 'sawtooth'; o.frequency.value = f; o.detune.value = det; vg.connect(o.frequency); o.connect(lp); return o; });
    lp.connect(g); g.connect(out);
    const r = ctx.createGain(); r.gain.value = 0.35; g.connect(r); r.connect(this.reverb);
    for (const o of [...oscs, vib]) { o.start(t); o.stop(t + dur + 0.4); }
  }

  pad(midis, when, dur, vol, out, kind) {
    const ctx = this.ctx, t = ctx.currentTime + Math.max(0, when);
    const lp = ctx.createBiquadFilter(); lp.type = 'lowpass';
    lp.frequency.value = kind === 'choir' ? 1400 : kind === 'warm' ? 1100 : 1800;
    const g = ctx.createGain();
    const atk = Math.min(0.6, dur * 0.3);
    g.gain.setValueAtTime(0.0001, t); g.gain.exponentialRampToValueAtTime(vol, t + atk); g.gain.setValueAtTime(vol, t + dur * 0.8); g.gain.exponentialRampToValueAtTime(0.0001, t + dur + 0.6);
    lp.connect(g); g.connect(out);
    const r = ctx.createGain(); r.gain.value = 0.6; g.connect(r); r.connect(this.reverb);
    const nodes = [];
    for (const m of midis) {
      const f = NOTE(m);
      if (kind === 'choir') {
        const src = ctx.createOscillator(); src.type = 'sawtooth'; src.frequency.value = f;
        const vib = ctx.createOscillator(), vg = ctx.createGain(); vib.frequency.value = 4.6 + Math.random(); vg.gain.value = f * 0.008; vib.connect(vg); vg.connect(src.frequency);
        for (const [ff, gg] of [[800, 1], [1150, 0.5], [2900, 0.12]]) {
          const bp = ctx.createBiquadFilter(); bp.type = 'bandpass'; bp.frequency.value = ff; bp.Q.value = 6;
          const fg = ctx.createGain(); fg.gain.value = gg * 2.2;
          src.connect(bp); bp.connect(fg); fg.connect(lp);
        }
        nodes.push(src, vib);
      } else {
        for (const det of [-6, 6]) {
          const o = ctx.createOscillator(); o.type = kind === 'warm' ? 'triangle' : 'sawtooth'; o.frequency.value = f; o.detune.value = det;
          o.connect(lp); nodes.push(o);
        }
      }
    }
    for (const n of nodes) { n.start(t); n.stop(t + dur + 0.7); }
  }

  drum(kind, when, vol, out) {
    if (kind === 'k') { this.tone({ f: 95, f2: 38, dur: 0.32, vol, out, when }); this.noise({ dur: 0.06, vol: vol * 0.3, type: 'lowpass', f: 900, out, when }); }
    else if (kind === 's') { this.noise({ dur: 0.16, vol: vol * 0.45, f: 1700, q: 0.9, out, when, rev: 0.2 }); this.tone({ f: 190, f2: 140, dur: 0.1, vol: vol * 0.3, out, when }); }
    else if (kind === 'h') this.noise({ dur: 0.04, vol: vol * 0.18, type: 'highpass', f: 7000, out, when });
  }

  // Formant voice: a glottal sawtooth through vowel filters.
  formant(vowel, f0, dur, o = {}) {
    if (!this.ready) return;
    const ctx = this.ctx, t = ctx.currentTime + (o.when || 0);
    const F = VOWELS[vowel] || VOWELS.a;
    const src = ctx.createOscillator(); src.type = 'sawtooth';
    src.frequency.setValueAtTime(f0 * (o.bend0 ?? 1.08), t);
    src.frequency.exponentialRampToValueAtTime(f0 * (o.bend1 ?? 0.82), t + dur);
    const g = ctx.createGain();
    g.gain.setValueAtTime(0.0001, t); g.gain.exponentialRampToValueAtTime(o.vol ?? 0.25, t + 0.02); g.gain.exponentialRampToValueAtTime(0.0001, t + dur);
    const out = o.out || this.vox;
    F.forEach((ff, i) => {
      const bp = ctx.createBiquadFilter(); bp.type = 'bandpass'; bp.frequency.value = ff * (o.formantShift ?? 1); bp.Q.value = 8 - i * 2;
      const fg = ctx.createGain(); fg.gain.value = [1.4, 0.9, 0.35][i];
      src.connect(bp); bp.connect(fg); fg.connect(g);
    });
    g.connect(out);
    if (o.rev) { const r = ctx.createGain(); r.gain.value = o.rev; g.connect(r); r.connect(this.reverb); }
    if (o.breath !== false) this.noise({ dur: 0.07, vol: (o.vol ?? 0.25) * 0.5, type: 'highpass', f: 1500, out, when: o.when || 0 });
    src.start(t); src.stop(t + dur + 0.05);
  }

  // ------------------------------------------------------------------ sound effects
  play(name, o = {}) {
    if (!this.ready) return;
    const out = o.pos ? this.spatial(o.pos) : this.sfx;
    switch (name) {
      case 'step': {
        const surf = o.surface || 'grass';
        const f = { grass: 900, stone: 1900, sand: 650, snow: 480 }[surf] || 1000;
        this.noise({ dur: surf === 'snow' ? 0.14 : 0.09, vol: 0.05 * (o.vol || 1), f: f * (0.85 + Math.random() * 0.3), q: surf === 'stone' ? 3.5 : 0.8, type: surf === 'snow' ? 'lowpass' : 'bandpass', out });
        if (surf === 'stone') this.tone({ f: 130, f2: 90, dur: 0.05, vol: 0.03 * (o.vol || 1), out });
        break;
      }
      case 'swing': this.noise({ dur: 0.24, vol: 0.2, f: 500 * (o.pitch || 1), f2: 2600 * (o.pitch || 1), q: 2.2, out }); break;
      case 'hit':
        this.noise({ dur: 0.1, vol: 0.32, f: 2600, q: 1.1, out });
        this.tone({ f: o.heavy ? 85 : 130, f2: 45, dur: 0.2, vol: 0.45, out });
        this.noise({ dur: 0.18, vol: 0.12, type: 'lowpass', f: 600, out });
        break;
      case 'block': this.tone({ f: 1250, f2: 1180, type: 'triangle', dur: 0.35, vol: 0.09, out, rev: 0.3 }); this.noise({ dur: 0.08, vol: 0.22, f: 3800, q: 4, out }); break;
      case 'parry':
        [0, 7, 12].forEach((n, i) => this.tone({ f: NOTE(88 + n), type: 'triangle', dur: 0.9, vol: 0.09, when: i * 0.03, out, rev: 0.7 }));
        this.noise({ dur: 0.12, vol: 0.3, f: 4500, q: 3, out });
        break;
      case 'perfect': this.tone({ f: 600, f2: 2400, dur: 0.5, vol: 0.08, out, rev: 0.8 }); this.noise({ dur: 0.4, vol: 0.12, f: 900, f2: 4000, q: 1.5, out }); break;
      case 'break': [0, 5, 12, 17].forEach((n, i) => this.tone({ f: NOTE(76 + n), type: 'square', dur: 0.25, vol: 0.05, when: i * 0.05, out })); this.noise({ dur: 0.3, vol: 0.25, type: 'lowpass', f: 1200, out }); break;
      case 'chargeFull': this.tone({ f: 880, f2: 1760, dur: 0.2, vol: 0.08, out, rev: 0.5 }); break;
      case 'surgeCharge': this.tone({ f: 120, f2: 900, type: 'sawtooth', dur: 0.55, vol: 0.06, out, rev: 0.6 }); this.noise({ dur: 0.55, vol: 0.18, f: 300, f2: 3000, q: 1.2, out }); break;
      case 'surge':
        this.tone({ f: 60, f2: 28, dur: 1.4, vol: 0.6, out });
        this.noise({ dur: 1.6, vol: 0.45, type: 'lowpass', f: 2400, f2: 120, out, rev: 0.5 });
        [0, 7, 12, 19, 24].forEach((n, i) => this.bell(74 + n, i * 0.04, 0.06, this.sfx, 2.5));
        break;
      case 'surgeReady': [0, 7, 12].forEach((n, i) => this.bell(79 + n, i * 0.08, 0.05, this.sfx, 1.5)); break;
      case 'bite': this.noise({ dur: 0.12, vol: 0.22, f: 650 * (o.pitch || 1), q: 1.5, out }); this.tone({ f: 170 * (o.pitch || 1), f2: 85, type: 'sawtooth', dur: 0.15, vol: 0.05, out }); break;
      case 'slam': this.tone({ f: 70, f2: 28, dur: 0.7, vol: 0.6, out }); this.noise({ dur: 0.6, vol: 0.32, type: 'lowpass', f: 520, out, rev: 0.3 }); break;
      case 'jump': this.noise({ dur: 0.1, vol: 0.06, f: 520, q: 1, out }); break;
      case 'land': this.noise({ dur: 0.16, vol: 0.16 * (o.volume || 1), type: 'lowpass', f: 420, out }); break;
      case 'dodge': this.noise({ dur: 0.32, vol: 0.13, f: 380, f2: 1700, q: 1, out }); break;
      case 'glide': this.tone({ f: 600, f2: 1200, dur: 0.45, vol: 0.07, out, rev: 0.6 }); this.noise({ dur: 0.55, vol: 0.08, f: 800, f2: 400, q: 0.7, out }); break;
      case 'splash': this.noise({ dur: 0.55, vol: 0.26, f: 1300, f2: 380, q: 0.6, out }); break;
      case 'cast': {
        const sp = o.spell;
        if (sp === 'heal') [0, 4, 7, 12, 16].forEach((n, i) => this.bell(76 + n, i * 0.06, 0.05, this.sfx, 1.4));
        else if (sp === 'frostlance') { this.bell(93, 0, 0.06, this.sfx, 1); this.noise({ dur: 0.35, vol: 0.14, f: 5200, q: 3, out }); }
        else if (sp === 'skybreaker') this.tone({ f: 200, f2: 900, type: 'sawtooth', dur: 0.45, vol: 0.05, out, rev: 0.6 });
        else { this.noise({ dur: 0.38, vol: 0.22, f: 380, f2: 1500, q: 0.8, out }); this.tone({ f: 300, f2: 620, type: 'triangle', dur: 0.26, vol: 0.06, out }); }
        break;
      }
      case 'impact': this.noise({ dur: 0.38, vol: 0.24, type: 'lowpass', f: o.element === 'frost' ? 3200 : 950, out, rev: 0.2 }); break;
      case 'enemyCast': this.tone({ f: 520, f2: 260, type: 'triangle', dur: 0.32, vol: 0.05, out }); break;
      case 'thunder': this.noise({ dur: 1.6, vol: 0.55, type: 'lowpass', f: 1300, f2: 70, attack: 0.002, out, rev: 0.5 }); this.noise({ dur: 0.16, vol: 0.45, f: 3200, q: 0.5, out }); break;
      case 'howl': this.tone({ f: 330, f2: 540, type: 'sawtooth', dur: 1.5, vol: 0.05, out, rev: 0.8, attack: 0.3 }); break;
      case 'bossWindup': this.tone({ f: 90, f2: 60, type: 'sawtooth', dur: 0.55, vol: 0.06, out }); break;
      case 'blink': this.tone({ f: 1400, f2: 200, dur: 0.32, vol: 0.1, out, rev: 0.7 }); break;
      case 'pickup': [0, 7, 12].forEach((n, i) => this.bell(84 + n, i * 0.05, 0.035, this.sfx, 0.9)); break;
      case 'coin': [0, 12].forEach((n, i) => this.tone({ f: NOTE(88 + n), type: 'square', dur: 0.12, vol: 0.035, when: i * 0.06 })); break;
      case 'chest': [0, 4, 7, 11, 14, 19].forEach((n, i) => this.pluck(64 + n, i * 0.07, 0.4, this.sfx, 0.7, 0.5)); break;
      case 'click': this.tone({ f: 1500, dur: 0.045, vol: 0.045, type: 'triangle' }); break;
      case 'hover': this.tone({ f: 1900, dur: 0.03, vol: 0.018, type: 'sine' }); break;
      case 'open': this.tone({ f: 880, f2: 1320, dur: 0.12, vol: 0.05, type: 'triangle' }); break;
      case 'close': this.tone({ f: 1320, f2: 880, dur: 0.12, vol: 0.045, type: 'triangle' }); break;
      case 'error': this.tone({ f: 220, type: 'square', dur: 0.14, vol: 0.045 }); this.tone({ f: 196, type: 'square', dur: 0.2, vol: 0.045, when: 0.1 }); break;
      case 'equip': this.noise({ dur: 0.16, vol: 0.12, f: 3000, q: 5 }); this.tone({ f: 660, f2: 990, dur: 0.1, vol: 0.03, type: 'triangle' }); break;
      case 'drink': [0, 1, 2].forEach((i) => this.tone({ f: 300 + i * 80, f2: 200, dur: 0.1, vol: 0.06, when: i * 0.12 })); break;
      case 'talk': this.pluck(79, 0, 0.25, this.sfx, 0.6, 0.3); break;
      case 'quest': [0, 7, 12, 16].forEach((n, i) => this.pluck(67 + n, i * 0.1, 0.45, this.sfx, 0.7, 0.6)); break;
      case 'questUpdate': [0, 5, 9].forEach((n, i) => this.pluck(72 + n, i * 0.08, 0.35, this.sfx, 0.6, 0.5)); break;
      case 'questDone':
      case 'victory':
        [0, 4, 7, 12].forEach((n, i) => this.pluck(62 + n, i * 0.09, 0.4, this.sfx, 0.7, 0.6));
        [12, 16, 19, 24].forEach((n, i) => this.flute(62 + n, 0.4 + i * 0.16, i === 3 ? 0.9 : 0.16, 0.07, this.sfx));
        break;
      case 'levelup': [0, 4, 7, 12, 16, 19, 24].forEach((n, i) => this.bell(67 + n, i * 0.07, 0.05, this.sfx, 1.6)); break;
      case 'discover': [0, 7, 14, 19].forEach((n, i) => this.bell(62 + n, i * 0.2, 0.05, this.sfx, 2.6)); this.pad([50, 57, 62, 66], 0, 2.4, 0.04, this.sfx, 'choir'); break;
      case 'waystone': [0, 7, 12, 19, 24].forEach((n, i) => this.bell(76 + n, i * 0.06, 0.04, this.sfx, 2.2)); break;
      case 'death': [12, 7, 3, 0].forEach((n, i) => this.strings(55 + n, i * 0.35, 0.5, 0.05, this.sfx, 0.08)); break;
      case 'hurt': this.tone({ f: 200, f2: 120, type: 'sawtooth', dur: 0.15, vol: 0.04 }); break;
    }
  }

  // ------------------------------------------------------------------ voices
  // Hero barks, synthesised so they're instant and never identical.
  voice(kind, arg) {
    if (!this.ready || this.game.settings.voiceMode === 'off') return;
    const fem = !!this.game.player?.look?.female;
    const f0 = (fem ? 245 : 128) * (0.95 + Math.random() * 0.1);
    const fs = fem ? 1.12 : 1;
    const now = this.ctx.currentTime;
    if (this.lastBark && now - this.lastBark < 0.25 && kind !== 'death' && kind !== 'surge') return;
    this.lastBark = now;
    if (kind === 'attack') this.formant(Math.random() < 0.5 ? 'a' : 'ah', f0 * 1.1, 0.13, { vol: 0.16, formantShift: fs, bend1: 0.75 });
    else if (kind === 'heavy') this.formant('a', f0 * 1.05, 0.32, { vol: 0.22, formantShift: fs, bend0: 1.0, bend1: 0.7 });
    else if (kind === 'hurt') this.formant(arg > 20 ? 'a' : 'u', f0 * 1.25, arg > 20 ? 0.28 : 0.16, { vol: 0.18, formantShift: fs, bend0: 1.2, bend1: 0.8 });
    else if (kind === 'death') this.formant('a', f0 * 1.1, 0.9, { vol: 0.2, formantShift: fs, bend0: 1.15, bend1: 0.55, rev: 0.5 });
    else if (kind === 'surge') {
      this.formant('a', f0 * 0.98, 0.55, { vol: 0.26, formantShift: fs, bend0: 0.9, bend1: 1.15, rev: 0.6 });
      this.speak('Aether, break!', { g: fem ? 'f' : 'm', pitch: fem ? 1.0 : 0.75, rate: 1.15, f0, id: 'hero' }, true);
    }
  }

  // Creature vocalisations, positioned in 3D.
  creature(enemy, kind) {
    if (!this.ready || !enemy) return;
    const t = enemy.type;
    const pos = enemy.position.clone(); pos.y += 1;
    const out = this.spatial(pos);
    const big = enemy.boss ? 0.55 : 1;
    if (t.includes('wolf') || t === 'rimefang') {
      if (kind === 'windup') { this.noise({ dur: 0.45, vol: 0.12, type: 'lowpass', f: 300 * big, out }); this.tone({ f: 85 * big, f2: 70 * big, type: 'sawtooth', dur: 0.45, vol: 0.06, out }); }
      else if (kind === 'hurt') this.formant('i', 600 * big, 0.18, { vol: 0.18, out, bend0: 1.3, bend1: 0.8, formantShift: 1.3 });
      else if (kind === 'howl') this.formant('u', 420 * big, 1.4, { vol: 0.16, out, bend0: 0.8, bend1: 1.25, rev: 0.8 });
      else if (kind === 'death') this.formant('i', 520 * big, 0.6, { vol: 0.16, out, bend0: 1.2, bend1: 0.5 });
      else if (kind === 'roar') this.noise({ dur: 0.6, vol: 0.2, type: 'lowpass', f: 500, out });
    } else if (t === 'boar') {
      if (kind === 'hurt' || kind === 'death') this.formant('i', 700, kind === 'death' ? 0.5 : 0.2, { vol: 0.16, out, bend0: 1.4, bend1: 0.7, formantShift: 1.2 });
      else { this.noise({ dur: 0.3, vol: 0.16, f: 400, q: 2, out }); this.formant('o', 140, 0.25, { vol: 0.12, out }); }
    } else if (t.includes('gloop')) {
      this.tone({ f: kind === 'hurt' || kind === 'death' ? 700 : 260, f2: kind === 'death' ? 90 : 520, dur: 0.18, vol: 0.08, out, type: 'sine' });
    } else if (t === 'golem' || t === 'warden' || t === 'colossus' || t === 'treant') {
      this.noise({ dur: 0.6, vol: 0.2, type: 'lowpass', f: 180 * big + 60, out, rev: 0.3 });
      this.tone({ f: 55 * big + 20, f2: 40, type: 'sawtooth', dur: 0.6, vol: 0.06, out });
    } else if (t === 'wisp' || t === 'skyray') {
      this.bell(kind === 'hurt' ? 86 : 81, 0, 0.04, out, 0.8);
    } else if (t === 'herald') {
      this.formant('o', 70, 0.8, { vol: 0.2, out, rev: 0.9, bend0: 1, bend1: 0.7 });
    } else {
      const f0 = (t === 'bandit_mage' ? 150 : 115) * (0.9 + Math.random() * 0.2);
      if (kind === 'windup') this.formant('a', f0, 0.16, { vol: 0.14, out });
      else if (kind === 'hurt') this.formant('u', f0 * 1.2, 0.16, { vol: 0.15, out, bend0: 1.2 });
      else if (kind === 'death') this.formant('a', f0, 0.6, { vol: 0.16, out, bend1: 0.6 });
    }
  }

  // Spoken dialogue. profile: { g, pitch, rate, f0, id }.
  speak(text, profile = VOICES.narrator, oneShot = false) {
    const mode = this.game.settings.voiceMode ?? 'speech';
    if (mode === 'off') return;
    const clean = text.replace(/\*[^*]*\*/g, ' ').replace(/[“”]/g, '"').replace(/\s+/g, ' ').trim();
    if (!clean) return;
    const synth = typeof speechSynthesis !== 'undefined' ? speechSynthesis : null;
    if (mode === 'speech' && synth && this.voices.length) {
      if (!oneShot) synth.cancel();
      const u = new SpeechSynthesisUtterance(clean);
      u.voice = this.pickVoice(profile);
      u.pitch = clamp(profile.pitch ?? 1, 0, 2);
      u.rate = clamp(profile.rate ?? 1, 0.5, 1.6);
      u.volume = clamp((this.game.settings.voiceVolume ?? 1) * this.game.settings.masterVolume, 0, 1);
      synth.speak(u);
      return;
    }
    if (!oneShot) this.babble(clean, profile);
  }

  stopSpeech() {
    if (typeof speechSynthesis !== 'undefined') speechSynthesis.cancel();
    if (this.babbleTimers) { this.babbleTimers.forEach(clearTimeout); this.babbleTimers = []; }
  }

  pickVoice(profile) {
    const fem = /female|zira|aria|jenny|samantha|victoria|karen|moira|tessa|fiona|serena|susan|hazel|libby|sonia|natasha|catherine|allison|ava|joanna|kimberly|salli|emma|olivia|amy|ivy|kendra|nicky|google us english/i;
    const male = /\bmale|david|mark|guy|daniel|alex|fred|george|ryan|thomas|oliver|arthur|rishi|aaron|\btom\b|matthew|joey|justin|brian|eric|google uk english male/i;
    const pool = this.voices.filter((v) => (profile.g === 'f' ? fem.test(v.name) && !/\bmale\b/i.test(v.name) : male.test(v.name) && !/female/i.test(v.name)));
    const list = pool.length ? pool : this.voices;
    let h = 7;
    for (const c of (profile.id || profile.g)) h = (h * 31 + c.charCodeAt(0)) >>> 0;
    return list[h % list.length] || null;
  }

  // Fallback: syllable babble with the character's pitch, Animal-Crossing style.
  babble(text, profile) {
    if (!this.ready) return;
    this.stopSpeech();
    this.babbleTimers = [];
    const words = text.split(/\s+/).slice(0, 40);
    let t = 0;
    const vowels = ['a', 'e', 'i', 'o', 'u'];
    for (const w of words) {
      const syl = Math.max(1, Math.min(3, Math.round(w.length / 3)));
      for (let s = 0; s < syl; s++) {
        const ch = w.toLowerCase().replace(/[^a-z]/g, '')[s * 2] || 'a';
        const v = vowels[ch.charCodeAt(0) % 5];
        const id = setTimeout(() => this.formant(v, (profile.f0 ?? 180) * (0.9 + Math.random() * 0.25), 0.07, { vol: 0.08, breath: false }), t * 1000);
        this.babbleTimers.push(id);
        t += 0.075 / (profile.rate ?? 1);
      }
      t += 0.05;
    }
  }

  // ------------------------------------------------------------------ ambience
  startAmbience() {
    const ctx = this.ctx;
    const mk = (type, f, q) => {
      const s = ctx.createBufferSource(); s.buffer = this.noiseBuf; s.loop = true;
      const fl = ctx.createBiquadFilter(); fl.type = type; fl.frequency.value = f; fl.Q.value = q;
      const g = ctx.createGain(); g.gain.value = 0;
      s.connect(fl); fl.connect(g); g.connect(this.amb); s.start();
      return { g, fl };
    };
    this.wind = mk('bandpass', 400, 0.6);
    this.rainN = mk('highpass', 1200, 0.3);
    this.waves = mk('lowpass', 500, 0.5);
    this.fireN = mk('bandpass', 2500, 0.8);
    this.chirpT = 0;
    this.murmurT = 0;
  }

  setRain(v) { this.rain = v; }
  setMusic(mood) { this.forced = mood; }

  update(dt) {
    if (!this.ready) return;
    const g = this.game, ctx = this.ctx, t = ctx.currentTime;
    const p = g.player?.position;
    if (!p) return;
    this.updateListener();
    const alt = g.inDungeon ? 0 : Math.max(0, p.y - 20);
    const night = g.sky.uniforms.uNight.value;
    const windV = g.inDungeon ? 0.02 : 0.045 + Math.min(alt / 120, 1) * 0.22 + (g.player.gliding ? 0.22 : 0) + this.rain * 0.08;
    this.wind.g.gain.setTargetAtTime(windV, t, 0.5);
    this.wind.fl.frequency.setTargetAtTime(300 + Math.sin(t * 0.3) * 120 + (g.player.gliding ? 400 : 0), t, 0.5);
    this.rainN.g.gain.setTargetAtTime(this.rain * 0.12, t, 1);
    const shore = g.inDungeon ? 0 : Math.max(0, 1 - Math.abs(g.world.heightAt(p.x, p.z) - 1) / 6);
    this.waves.g.gain.setTargetAtTime(shore * (0.1 + Math.sin(t * 0.5) * 0.04), t, 0.5);
    let fireD = 1e9;
    for (const f of g.fx.fires) fireD = Math.min(fireD, f.pos.distanceTo(p));
    this.fireN.g.gain.setTargetAtTime(fireD < 14 ? (1 - fireD / 14) * 0.05 * (0.6 + Math.random() * 0.8) : 0, t, 0.05);
    this.chirpT -= dt;
    if (this.chirpT <= 0 && !g.inDungeon) {
      this.chirpT = 0.4 + Math.random() * 2.5;
      if (night < 0.5 && this.rain < 0.3 && Math.random() < 0.6) {
        const base = 2200 + Math.random() * 1800;
        const n = 2 + Math.floor(Math.random() * 4);
        for (let i = 0; i < n; i++) this.tone({ f: base, f2: base * (1.2 + Math.random() * 0.3), dur: 0.08, vol: 0.014, when: i * 0.1, out: this.amb });
      } else if (night > 0.5) {
        for (let i = 0; i < 3; i++) this.tone({ f: 4200, type: 'square', dur: 0.03, vol: 0.005, when: i * 0.05, out: this.amb });
      }
    }
    if (this.mood === 'town' && !g.inDialogue) {
      this.murmurT -= dt;
      if (this.murmurT <= 0) {
        this.murmurT = 0.15 + Math.random() * 0.4;
        const v = ['a', 'e', 'o', 'i'][Math.floor(Math.random() * 4)];
        this.formant(v, 100 + Math.random() * 140, 0.09, { vol: 0.012, out: this.amb, breath: false });
      }
    }
    this.updateMusic();
  }

  // ------------------------------------------------------------------ music
  pickMood() {
    const g = this.game;
    if (this.forced) return this.forced;
    if (g.mode === 'title' || g.mode === 'creator') return 'explore';
    if (g.inDungeon) return g.combat.enemies.some((e) => !e.dead && e.state === 'chase' && e.dist2Player() < 25) ? 'combat' : 'dungeon';
    if (g.combat.enemies.some((e) => !e.dead && (e.state === 'chase' || e.state === 'attack') && e.dist2Player() < 28)) return 'combat';
    const p = g.player.position;
    for (const l of g.world.locations) if (l.type === 'village' && Math.hypot(p.x - l.x, p.z - l.z) < l.r + 25) return 'town';
    if (g.sky.isNight) return 'night';
    const reg = g.world.regionAt(p.x, p.z);
    return reg === 1 ? 'forest' : reg === 2 ? 'mountain' : reg === 3 ? 'desert' : 'explore';
  }

  compile(th) {
    const ev = [];
    let s = 0;
    for (const [n, len] of th.melody) { if (n !== null) ev.push({ s, len, n }); s += len; }
    th.loop = th.bars * 8;
    th.events = ev;
    return th;
  }

  updateMusic() {
    const ctx = this.ctx, sq = this.seq;
    const mood = this.pickMood();
    if (mood !== this.mood) {
      this.mood = mood;
      const th = THEMES[mood] || THEMES.explore;
      if (!th.events) this.compile(th);
      if (sq.gain) { const old = sq.gain; old.gain.setTargetAtTime(0, ctx.currentTime, 0.6); setTimeout(() => old.disconnect(), 4000); }
      sq.gain = ctx.createGain();
      sq.gain.gain.setValueAtTime(0, ctx.currentTime);
      sq.gain.gain.setTargetAtTime(1, ctx.currentTime + 0.2, 0.8);
      sq.gain.connect(this.musicBus);
      sq.theme = th; sq.step = 0; sq.next = Math.max(sq.next, ctx.currentTime + 0.25);
      this.game.events.emit('music', th.name);
    }
    const th = sq.theme;
    if (!th) return;
    const stepDur = 60 / th.bpm / 2;
    while (sq.next < ctx.currentTime + 0.3) {
      this.scheduleStep(th, sq.step, sq.next - ctx.currentTime, stepDur, sq.gain);
      sq.next += stepDur;
      sq.step = (sq.step + 1) % th.loop;
    }
  }

  scheduleStep(th, step, when, stepDur, out) {
    const bar = Math.floor(step / 8), e8 = step % 8;
    const [root, q] = th.chords[bar % th.chords.length];
    const tones = CHORD[q].map((n) => th.key + root + n);
    if (e8 === 0) this.pad(tones.slice(0, 3).map((n) => n - 12), when, stepDur * 8, th.pad === 'choir' ? 0.022 : 0.03, out, th.pad);
    const b = th.bass?.[e8];
    if (b !== null && b !== undefined) {
      const m = th.key + root + b - 24;
      if (th.drums && th.bpm > 120) this.strings(m, when, stepDur * 0.9, 0.05, out, 0.01);
      else this.tone({ f: NOTE(m), type: 'triangle', dur: stepDur * 3.5, vol: 0.12, out, when, attack: 0.02 });
    }
    const a = th.arp?.[e8];
    if (a !== null && a !== undefined) {
      const m = tones[a % tones.length];
      if (th.arpInst === 'harp') this.pluck(m, when, 0.32, out, 0.55);
      else if (th.arpInst === 'bell') this.bell(m + 12, when, 0.025, out, 1.6);
      else if (th.arpInst === 'stab') this.strings(m, when, stepDur * 0.6, 0.035, out, 0.01);
    }
    for (const ev of th.events) {
      if (ev.s !== step) continue;
      const m = th.key + ev.n;
      const dur = ev.len * stepDur;
      const v = th.leadVol ?? 0.8;
      if (th.lead === 'flute') this.flute(m, when, dur, 0.075 * v, out);
      else if (th.lead === 'bell') this.bell(m, when, 0.06 * v, out, Math.max(1.2, dur * 1.5));
      else if (th.lead === 'pluckLead') this.pluck(m + 12, when, 0.5 * v, out, 0.75, 0.4);
      else if (th.lead === 'strings') this.strings(m, when, dur, 0.06 * v, out, 0.05);
    }
    if (th.drums) {
      const v = th.drumVol ?? 0.5;
      for (const k of ['k', 's', 'h']) if (th.drums[k]?.[e8] === 'x') this.drum(k, when, v * 0.5, out);
    }
  }
}

export { THEMES };
