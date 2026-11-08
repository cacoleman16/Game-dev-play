// All sound is synthesised with WebAudio: footsteps, swings, hits, spells,
// UI chimes, ambience (wind, rain, birds, crickets) and generative music that
// shifts between exploration, night, town, combat and boss moods.

const NOTE = (n) => 440 * Math.pow(2, (n - 69) / 12);

export class Audio {
  constructor(game) {
    this.game = game;
    this.ctx = null;
    this.ready = false;
    this.mood = 'explore';
    this.forced = null;
    this.musicT = 0;
    this.bar = 0;
    this.rain = 0;
  }

  // Must be called from a user gesture.
  init() {
    if (this.ctx) { if (this.ctx.state === 'suspended') this.ctx.resume(); return; }
    const AC = window.AudioContext || window.webkitAudioContext;
    if (!AC) return;
    const ctx = new AC();
    this.ctx = ctx;
    this.master = ctx.createGain();
    this.master.connect(ctx.destination);
    this.sfx = ctx.createGain(); this.sfx.connect(this.master);
    this.music = ctx.createGain(); this.music.connect(this.master);
    this.amb = ctx.createGain(); this.amb.connect(this.master);
    // Shared reverb for music and spells.
    this.reverb = ctx.createConvolver();
    this.reverb.buffer = this.makeImpulse(2.8, 2.2);
    this.revGain = ctx.createGain(); this.revGain.gain.value = 0.35;
    this.reverb.connect(this.revGain); this.revGain.connect(this.master);
    this.noiseBuf = this.makeNoise(2);
    this.applyVolumes();
    this.startAmbience();
    this.ready = true;
    this.nextNoteTime = ctx.currentTime + 0.5;
  }

  applyVolumes() {
    if (!this.ctx) return;
    const s = this.game.settings;
    this.master.gain.value = s.masterVolume;
    this.sfx.gain.value = s.sfxVolume;
    this.music.gain.value = s.musicVolume * 0.5;
    this.amb.gain.value = 0.6;
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
    const len = ctx.sampleRate * sec;
    const b = ctx.createBuffer(2, len, ctx.sampleRate);
    for (let c = 0; c < 2; c++) {
      const d = b.getChannelData(c);
      for (let i = 0; i < len; i++) d[i] = (Math.random() * 2 - 1) * Math.pow(1 - i / len, decay);
    }
    return b;
  }

  // ---------------- primitives
  tone({ f = 440, f2 = null, type = 'sine', dur = 0.2, vol = 0.3, attack = 0.005, out = null, when = 0, rev = 0, detune = 0 }) {
    const ctx = this.ctx;
    const t = ctx.currentTime + when;
    const o = ctx.createOscillator();
    o.type = type;
    o.frequency.setValueAtTime(f, t);
    o.detune.value = detune;
    if (f2) o.frequency.exponentialRampToValueAtTime(Math.max(f2, 1), t + dur);
    const g = ctx.createGain();
    g.gain.setValueAtTime(0.0001, t);
    g.gain.exponentialRampToValueAtTime(vol, t + attack);
    g.gain.exponentialRampToValueAtTime(0.0001, t + dur);
    o.connect(g);
    g.connect(out || this.sfx);
    if (rev) { const r = ctx.createGain(); r.gain.value = rev; g.connect(r); r.connect(this.reverb); }
    o.start(t);
    o.stop(t + dur + 0.05);
  }

  noise({ dur = 0.2, vol = 0.3, type = 'bandpass', f = 1000, f2 = null, q = 1, attack = 0.005, out = null, when = 0, rev = 0 }) {
    const ctx = this.ctx;
    const t = ctx.currentTime + when;
    const s = ctx.createBufferSource();
    s.buffer = this.noiseBuf;
    s.loop = true;
    const fl = ctx.createBiquadFilter();
    fl.type = type; fl.Q.value = q;
    fl.frequency.setValueAtTime(f, t);
    if (f2) fl.frequency.exponentialRampToValueAtTime(f2, t + dur);
    const g = ctx.createGain();
    g.gain.setValueAtTime(0.0001, t);
    g.gain.exponentialRampToValueAtTime(vol, t + attack);
    g.gain.exponentialRampToValueAtTime(0.0001, t + dur);
    s.connect(fl); fl.connect(g); g.connect(out || this.sfx);
    if (rev) { const r = ctx.createGain(); r.gain.value = rev; g.connect(r); r.connect(this.reverb); }
    s.start(t, Math.random());
    s.stop(t + dur + 0.05);
  }

  // ---------------- sound effects
  play(name, o = {}) {
    if (!this.ready) return;
    const r = (a, b) => a + Math.random() * (b - a);
    switch (name) {
      case 'step': {
        const surf = o.surface || 'grass';
        const f = { grass: 900, stone: 1800, sand: 700, snow: 500 }[surf] || 1000;
        this.noise({ dur: 0.09, vol: 0.06 * (o.vol || 1), f: f * r(0.8, 1.2), q: surf === 'stone' ? 3 : 0.8, type: surf === 'snow' ? 'lowpass' : 'bandpass' });
        break;
      }
      case 'swing': this.noise({ dur: 0.22, vol: 0.18, f: 600 * (o.pitch || 1), f2: 2400 * (o.pitch || 1), q: 2.5 }); break;
      case 'hit':
        this.noise({ dur: 0.12, vol: 0.32, f: 2400, q: 1.2 });
        this.tone({ f: o.heavy ? 90 : 140, f2: 50, type: 'sine', dur: 0.18, vol: 0.4 });
        break;
      case 'block': this.tone({ f: 1200, f2: 900, type: 'square', dur: 0.12, vol: 0.08 }); this.noise({ dur: 0.1, vol: 0.2, f: 3500, q: 4 }); break;
      case 'bite': this.noise({ dur: 0.12, vol: 0.2, f: 700 * (o.pitch || 1), q: 1.5 }); this.tone({ f: 180 * (o.pitch || 1), f2: 90, type: 'sawtooth', dur: 0.15, vol: 0.06 }); break;
      case 'slam': this.tone({ f: 70, f2: 30, type: 'sine', dur: 0.6, vol: 0.6 }); this.noise({ dur: 0.5, vol: 0.3, type: 'lowpass', f: 500 }); break;
      case 'jump': this.noise({ dur: 0.1, vol: 0.06, f: 500, q: 1 }); break;
      case 'land': this.noise({ dur: 0.15, vol: 0.15 * (o.volume || 1), type: 'lowpass', f: 400 }); break;
      case 'dodge': this.noise({ dur: 0.3, vol: 0.12, f: 400, f2: 1600, q: 1 }); break;
      case 'glide': this.tone({ f: 600, f2: 1200, type: 'sine', dur: 0.4, vol: 0.08, rev: 0.6 }); this.noise({ dur: 0.5, vol: 0.08, f: 800, f2: 400, q: 0.7 }); break;
      case 'splash': this.noise({ dur: 0.5, vol: 0.25, f: 1200, f2: 400, q: 0.6 }); break;
      case 'cast': {
        const sp = o.spell;
        if (sp === 'heal') [0, 4, 7, 12].forEach((n, i) => this.tone({ f: NOTE(72 + n), dur: 0.6, vol: 0.08, when: i * 0.06, rev: 0.8 }));
        else if (sp === 'frostlance') { this.tone({ f: 1800, f2: 2600, type: 'triangle', dur: 0.3, vol: 0.08, rev: 0.6 }); this.noise({ dur: 0.3, vol: 0.12, f: 5000, q: 3 }); }
        else if (sp === 'skybreaker') this.tone({ f: 200, f2: 800, type: 'sawtooth', dur: 0.4, vol: 0.06, rev: 0.6 });
        else { this.noise({ dur: 0.35, vol: 0.2, f: 400, f2: 1400, q: 0.8 }); this.tone({ f: 300, f2: 600, type: 'triangle', dur: 0.25, vol: 0.06 }); }
        break;
      }
      case 'impact': this.noise({ dur: 0.35, vol: 0.22, type: 'lowpass', f: o.element === 'frost' ? 3000 : 900 }); break;
      case 'enemyCast': this.tone({ f: 500, f2: 250, type: 'triangle', dur: 0.3, vol: 0.05 }); break;
      case 'thunder': this.noise({ dur: 1.4, vol: 0.5, type: 'lowpass', f: 1200, f2: 80, attack: 0.002 }); this.noise({ dur: 0.15, vol: 0.4, f: 3000, q: 0.5 }); break;
      case 'howl': this.tone({ f: 320, f2: 520, type: 'sawtooth', dur: 1.4, vol: 0.05, rev: 0.8, attack: 0.3 }); break;
      case 'bossWindup': this.tone({ f: 90, f2: 60, type: 'sawtooth', dur: 0.5, vol: 0.06 }); break;
      case 'blink': this.tone({ f: 1400, f2: 200, type: 'sine', dur: 0.3, vol: 0.1, rev: 0.7 }); break;
      case 'pickup': [0, 7].forEach((n, i) => this.tone({ f: NOTE(84 + n), dur: 0.25, vol: 0.08, when: i * 0.07, rev: 0.4 })); break;
      case 'coin': [0, 12].forEach((n, i) => this.tone({ f: NOTE(88 + n), type: 'square', dur: 0.12, vol: 0.04, when: i * 0.06 })); break;
      case 'chest': [0, 4, 7, 11, 14].forEach((n, i) => this.tone({ f: NOTE(72 + n), dur: 0.5, vol: 0.07, when: i * 0.07, rev: 0.6, type: 'triangle' })); break;
      case 'click': this.tone({ f: 1400, dur: 0.05, vol: 0.05, type: 'triangle' }); break;
      case 'hover': this.tone({ f: 1800, dur: 0.03, vol: 0.02, type: 'sine' }); break;
      case 'open': this.tone({ f: 880, f2: 1320, dur: 0.12, vol: 0.06, type: 'triangle' }); break;
      case 'close': this.tone({ f: 1320, f2: 880, dur: 0.12, vol: 0.05, type: 'triangle' }); break;
      case 'error': this.tone({ f: 220, type: 'square', dur: 0.15, vol: 0.05 }); this.tone({ f: 196, type: 'square', dur: 0.2, vol: 0.05, when: 0.1 }); break;
      case 'equip': this.noise({ dur: 0.15, vol: 0.12, f: 3000, q: 5 }); break;
      case 'drink': [0, 1, 2].forEach((i) => this.tone({ f: 300 + i * 80, f2: 200, dur: 0.1, vol: 0.06, when: i * 0.12 })); break;
      case 'talk': this.tone({ f: 660, f2: 880, dur: 0.1, vol: 0.04, type: 'triangle' }); break;
      case 'quest': [0, 7, 12].forEach((n, i) => this.tone({ f: NOTE(67 + n), dur: 0.6, vol: 0.08, when: i * 0.12, rev: 0.6, type: 'triangle' })); break;
      case 'questUpdate': [0, 5].forEach((n, i) => this.tone({ f: NOTE(72 + n), dur: 0.4, vol: 0.06, when: i * 0.1, rev: 0.5, type: 'triangle' })); break;
      case 'questDone':
      case 'victory': [0, 4, 7, 12, 16, 19, 24].forEach((n, i) => this.tone({ f: NOTE(60 + n), dur: 0.9, vol: 0.07, when: i * 0.09, rev: 0.7, type: i % 2 ? 'triangle' : 'sine' })); break;
      case 'levelup': [0, 4, 7, 12, 7, 12, 16].forEach((n, i) => this.tone({ f: NOTE(67 + n), dur: 0.5, vol: 0.08, when: i * 0.08, rev: 0.8, type: 'triangle' })); break;
      case 'discover': [0, 7, 14, 19].forEach((n, i) => this.tone({ f: NOTE(62 + n), dur: 1.4, vol: 0.06, when: i * 0.18, rev: 0.9 })); break;
      case 'waystone': [0, 7, 12, 19].forEach((n, i) => this.tone({ f: NOTE(76 + n), dur: 1.2, vol: 0.05, when: i * 0.05, rev: 1 })); break;
      case 'death': [12, 7, 3, 0].forEach((n, i) => this.tone({ f: NOTE(55 + n), dur: 1.2, vol: 0.08, when: i * 0.3, rev: 0.8, type: 'triangle' })); break;
      case 'hurt': this.tone({ f: 200, f2: 120, type: 'sawtooth', dur: 0.15, vol: 0.06 }); break;
    }
  }

  // ---------------- ambience
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
    this.chirpT = 0;
  }

  setRain(v) { this.rain = v; }

  setMusic(mood) { this.forced = mood; }

  update(dt) {
    if (!this.ready) return;
    const g = this.game, ctx = this.ctx, t = ctx.currentTime;
    const p = g.player?.position;
    if (!p) return;
    const alt = g.inDungeon ? 0 : Math.max(0, p.y - 20);
    const night = g.sky.uniforms.uNight.value;
    const windV = g.inDungeon ? 0.02 : 0.05 + Math.min(alt / 120, 1) * 0.25 + (g.player.gliding ? 0.25 : 0) + this.rain * 0.08;
    this.wind.g.gain.setTargetAtTime(windV, t, 0.5);
    this.wind.fl.frequency.setTargetAtTime(300 + Math.sin(t * 0.3) * 120 + (g.player.gliding ? 400 : 0), t, 0.5);
    this.rainN.g.gain.setTargetAtTime(this.rain * 0.12, t, 1);
    const shore = g.inDungeon ? 0 : Math.max(0, 1 - Math.abs(g.world.heightAt(p.x, p.z) - 1) / 6);
    this.waves.g.gain.setTargetAtTime(shore * (0.1 + Math.sin(t * 0.5) * 0.04), t, 0.5);
    // Birds by day, crickets by night.
    this.chirpT -= dt;
    if (this.chirpT <= 0 && !g.inDungeon) {
      this.chirpT = 0.4 + Math.random() * 2.5;
      if (night < 0.5 && this.rain < 0.3 && Math.random() < 0.6) {
        const base = 2200 + Math.random() * 1800;
        const n = 2 + Math.floor(Math.random() * 4);
        for (let i = 0; i < n; i++) this.tone({ f: base, f2: base * (1.2 + Math.random() * 0.3), dur: 0.08, vol: 0.015, when: i * 0.1, out: this.amb });
      } else if (night > 0.5) {
        for (let i = 0; i < 3; i++) this.tone({ f: 4200, type: 'square', dur: 0.03, vol: 0.006, when: i * 0.05, out: this.amb });
      }
    }
    this.updateMusic();
  }

  // ---------------- generative music
  pickMood() {
    const g = this.game;
    if (this.forced) return this.forced;
    if (g.inDungeon) return 'dungeon';
    if (g.combat.enemies.some((e) => !e.dead && e.state === 'chase' && e.dist2Player() < 30)) return 'combat';
    if (g.sky.isNight) return 'night';
    const p = g.player.position;
    for (const l of g.world.locations) if (l.type === 'village' && Math.hypot(p.x - l.x, p.z - l.z) < l.r + 20) return 'town';
    return 'explore';
  }

  updateMusic() {
    const ctx = this.ctx;
    const mood = this.pickMood();
    if (mood !== this.mood) { this.mood = mood; this.bar = 0; }
    const M = MOODS[this.mood];
    while (this.nextNoteTime < ctx.currentTime + 0.25) {
      this.scheduleBeat(M, this.nextNoteTime - ctx.currentTime);
      this.nextNoteTime += 60 / M.bpm / 2; // eighth notes
      this.beat = (this.beat || 0) + 1;
    }
  }

  scheduleBeat(M, when) {
    const b = this.beat || 0;
    const eighth = b % 8;
    const bar = Math.floor(b / 8);
    const chord = M.prog[bar % M.prog.length];
    const root = M.key + chord[0];
    const out = this.music;
    const spb = 60 / M.bpm;
    if (eighth === 0) {
      // Pad: three detuned voices of the chord, through the reverb.
      for (const iv of chord.slice(1)) {
        for (const det of [-7, 7]) this.tone({ f: NOTE(root + iv - 12), type: M.pad, dur: spb * 4.2, vol: 0.028 * M.padVol, attack: spb * 1.2, out, when, rev: 0.7, detune: det });
      }
      this.tone({ f: NOTE(root - 24), type: 'sine', dur: spb * 3.8, vol: 0.06 * M.bassVol, attack: 0.05, out, when });
    }
    // Arpeggio or melody.
    if (M.arp && Math.random() < M.arp) {
      const notes = chord.slice(1).concat(chord.slice(1).map((n) => n + 12));
      const n = notes[(eighth * M.arpStep) % notes.length];
      this.tone({ f: NOTE(root + n + 12), type: M.lead, dur: spb * 1.4, vol: 0.035, out, when, rev: 0.6 });
    }
    if (M.melody && eighth % 2 === 0 && Math.random() < M.melody) {
      const n = M.scale[Math.floor(Math.random() * M.scale.length)];
      this.tone({ f: NOTE(M.key + n + 12), type: 'sine', dur: spb * 1.8, vol: 0.03, out, when, rev: 0.9, attack: 0.02 });
    }
    if (M.drums) {
      if (eighth % 4 === 0) this.tone({ f: 110, f2: 45, type: 'sine', dur: 0.25, vol: 0.18 * M.drums, out, when });
      if (eighth % 4 === 2) this.noise({ dur: 0.12, vol: 0.06 * M.drums, f: 2000, q: 0.8, out, when });
      if (M.drums > 0.8 && eighth % 2 === 1) this.noise({ dur: 0.04, vol: 0.03, type: 'highpass', f: 6000, out, when });
    }
  }
}

// Chords are [rootOffset, ...intervals]. Keys are MIDI notes.
const MOODS = {
  explore: { bpm: 84, key: 62, pad: 'triangle', lead: 'triangle', padVol: 1, bassVol: 1, arp: 0.55, arpStep: 3, melody: 0.35,
    prog: [[0, 0, 4, 7, 11], [5, 0, 4, 7, 11], [9, 0, 3, 7, 10], [7, 0, 4, 7, 9]], scale: [0, 2, 4, 6, 7, 9, 11] },
  town: { bpm: 96, key: 65, pad: 'triangle', lead: 'square', padVol: 0.8, bassVol: 1, arp: 0.8, arpStep: 1, melody: 0.2,
    prog: [[0, 0, 4, 7], [5, 0, 4, 7], [7, 0, 4, 7], [0, 0, 4, 7, 9]], scale: [0, 2, 4, 5, 7, 9] },
  night: { bpm: 66, key: 57, pad: 'sine', lead: 'sine', padVol: 1.2, bassVol: 0.7, arp: 0.3, arpStep: 2, melody: 0.25,
    prog: [[0, 0, 3, 7, 10], [5, 0, 3, 7, 10], [3, 0, 4, 7, 11], [10, 0, 4, 7, 9]], scale: [0, 2, 3, 5, 7, 9, 10] },
  combat: { bpm: 132, key: 57, pad: 'sawtooth', lead: 'square', padVol: 0.5, bassVol: 1.3, arp: 0.9, arpStep: 1, melody: 0, drums: 0.9,
    prog: [[0, 0, 3, 7], [8, 0, 4, 7], [10, 0, 4, 7], [7, 0, 4, 7]], scale: [0, 2, 3, 5, 7, 8, 10] },
  boss: { bpm: 144, key: 52, pad: 'sawtooth', lead: 'sawtooth', padVol: 0.6, bassVol: 1.5, arp: 1, arpStep: 1, melody: 0, drums: 1,
    prog: [[0, 0, 3, 7], [1, 0, 4, 7], [0, 0, 3, 7], [6, 0, 3, 6]], scale: [0, 1, 3, 5, 7, 8, 10] },
  final: { bpm: 150, key: 50, pad: 'sawtooth', lead: 'sawtooth', padVol: 0.7, bassVol: 1.5, arp: 1, arpStep: 2, melody: 0.2, drums: 1,
    prog: [[0, 0, 3, 7], [8, 0, 4, 7], [3, 0, 4, 7], [10, 0, 4, 7]], scale: [0, 2, 3, 5, 7, 8, 11] },
  dungeon: { bpm: 70, key: 50, pad: 'sine', lead: 'triangle', padVol: 1.2, bassVol: 1.2, arp: 0.25, arpStep: 3, melody: 0.15,
    prog: [[0, 0, 3, 7], [1, 0, 4, 7], [0, 0, 3, 6], [11, 0, 4, 7]], scale: [0, 1, 3, 5, 7, 8] },
  victory: { bpm: 100, key: 62, pad: 'triangle', lead: 'triangle', padVol: 1, bassVol: 1, arp: 1, arpStep: 1, melody: 0.4,
    prog: [[0, 0, 4, 7, 11], [5, 0, 4, 7, 14], [7, 0, 4, 7, 10], [0, 0, 4, 7, 12]], scale: [0, 2, 4, 7, 9, 11] },
};
