// The Game object owns the renderer, scene, world and every system, and runs
// the frame loop. Systems reach each other through `game.*` and `game.events`.

import * as THREE from 'three';
import { Events, isTouchDevice } from './core/util.js';
import { QUALITY, loadSettings } from './core/config.js';
import { Input } from './core/input.js';
import { WorldData } from './world/worldgen.js';
import { Terrain } from './world/terrain.js';
import { Sky } from './world/sky.js';
import { Water } from './world/water.js';
import { Grass } from './world/grass.js';
import { makeWorldTextures } from './world/textures.js';
import { Colliders } from './world/colliders.js';
import { Vegetation } from './world/vegetation.js';
import { Structures } from './world/structures.js';
import { OverworldEnv } from './world/env.js';
import { Dungeon } from './world/dungeon.js';
import { LOC } from './world/locations.js';
import { Interact } from './systems/interact.js';
import { Combat } from './systems/combat.js';
import { Spawner } from './systems/spawner.js';
import { Inventory } from './systems/inventory.js';
import { Quests } from './systems/quests.js';
import { Dialogue } from './systems/dialogue.js';
import { Story } from './systems/story.js';
import { SaveSystem } from './systems/save.js';
import { FX } from './fx/particles.js';
import { Post } from './fx/post.js';
import { Player } from './entities/player.js';
import { CameraRig } from './entities/camera.js';
import { NPCs } from './entities/npcs.js';
import { Audio } from './audio/audio.js';
import { UI } from './ui/ui.js';
import { DEFAULT_LOOK } from './entities/character.js';

export class Game {
  constructor(canvas) {
    this.canvas = canvas;
    this.events = new Events();
    this.settings = loadSettings();
    this.quality = QUALITY[this.settings.quality];
    this.isTouch = isTouchDevice();
    this.uniforms = { uTime: { value: 0 } };
    this.time = 0;
    this.dt = 0;
    this.hitStop = 0;
    this.paused = false;
    this.mode = 'loading';
    this.inDungeon = false;
    this.inDialogue = false;
    this.flags = {};
    this.totalHours = 0;
    this.slowT = 0;
    this.slowScale = 1;
  }

  // Brief slow motion for parries, perfect dodges and the surge.
  slowMo(scale, dur) { this.slowScale = scale; this.slowT = Math.max(this.slowT, dur); }

  async init(progress) {
    const q = this.quality;
    const renderer = new THREE.WebGLRenderer({ canvas: this.canvas, antialias: q.antialias && q.post === false, powerPreference: 'high-performance', stencil: false });
    renderer.setPixelRatio(q.pixelRatio);
    renderer.setSize(window.innerWidth, window.innerHeight, false);
    renderer.outputColorSpace = THREE.SRGBColorSpace;
    renderer.toneMapping = THREE.NeutralToneMapping;
    renderer.toneMappingExposure = 1.05;
    renderer.shadowMap.enabled = q.shadows;
    renderer.shadowMap.type = THREE.PCFShadowMap;
    renderer.info.autoReset = false;
    this.renderer = renderer;

    this.scene = new THREE.Scene();
    this.overworld = this.scene;
    this.camera = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 0.15, 3200);
    this.camera.position.set(0, 60, 200);

    this.world = new WorldData();
    await this.world.generate((p, msg) => progress(p * 0.55, msg));
    progress(0.56, 'Uploading the land');
    await tick();
    this.worldTex = makeWorldTextures(this.world);

    this.sky = new Sky(this);
    this.sky.dayLength = this.settings.dayLengthMinutes * 60;
    this.sky.addTo(this.scene);
    this.terrain = new Terrain(this);
    this.scene.add(this.terrain.group);
    this.water = new Water(this);
    this.scene.add(this.water.mesh);
    this.grass = new Grass(this);
    this.scene.add(this.grass.mesh);
    this.colliders = new Colliders();
    this.interact = new Interact(this);
    this.fx = new FX(this);
    this.fx.addTo(this.scene);
    progress(0.6, 'Raising villages and ruins');
    await tick();
    this.structures = new Structures(this);
    this.structures.build();
    this.scene.add(this.structures.group);
    this.vegetation = new Vegetation(this);
    await this.vegetation.build((p) => progress(0.62 + p * 0.2, 'Growing forests'));
    this.scene.add(this.vegetation.group);
    progress(0.84, 'Waking the people of Lumeria');
    await tick();

    this.env = new OverworldEnv(this);
    this.input = new Input(this, this.canvas);
    this.cameraRig = new CameraRig(this);
    this.combat = new Combat(this);
    this.spawner = new Spawner(this);
    this.inventory = new Inventory(this);
    this.player = new Player(this);
    this.player.look = { ...DEFAULT_LOOK };
    this.quests = new Quests(this);
    this.dialogue = new Dialogue(this);
    this.story = new Story(this);
    this.story.buildHerbs();
    this.npcs = new NPCs(this);
    this.save = new SaveSystem(this);
    this.audio = new Audio(this);
    this.dungeon = new Dungeon(this);
    this.ui = new UI(this);
    if (this.isTouch) this.ui.setupTouch();
    this.events.on('footstep', (pos, surface, speed) => this.audio.play('step', { surface, vol: speed > 7 ? 1.4 : 1 }));
    this.events.on('land', (pos, impact) => { if (impact > 6) this.fx.dust(pos, 8); });
    this.events.on('splash', (pos) => this.fx.splash(pos));
    this.events.on('dungeon:enter', () => this.dungeon.enter());
    this.player.buildCharacter(this.player.look);
    this.player.character.group.visible = false;

    this.post = new Post(this);

    progress(0.9, 'Placing the last stones');
    await tick();
    // Warm up: build nearby terrain and compile shaders before the first frame.
    this.terrain.update(this.camera.position.x, this.camera.position.z, true);
    renderer.compile(this.scene, this.camera);
    window.addEventListener('resize', () => this.onResize());
    this.onResize();
    progress(1, 'Ready');
  }

  onResize() {
    const w = window.innerWidth, h = window.innerHeight;
    this.camera.aspect = w / h;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(w, h, false);
    this.post?.setSize(w, h);
  }

  // ---------------------------------------------------------------- modes
  showTitle() {
    this.mode = 'title';
    const L = LOC.heartshard;
    this.cameraRig.mode = 'orbit';
    Object.assign(this.cameraRig.orbit, { radius: 150, height: 42, speed: 0.035, angle: 1.2 });
    this.cameraRig.orbit.center.set(L.x, 30, L.z);
    this.sky.timeOfDay = 17.2;
    this.ui.showHUD(false);
    this.ui.showTitle();
  }

  beginCreator() {
    const L = LOC.brightwater;
    const p = this.player;
    const x = L.x + 2, z = L.z + 6;
    p.teleport(x, this.world.heightAt(x, z), z);
    p.yaw = 0;
    p.character.group.visible = true;
    p.syncCharacter(0.016);
    this.cameraRig.mode = 'free';
    const cam = this.camera;
    const off = window.innerWidth > 720 ? 1.1 : 0.6;
    cam.position.set(x + off, p.position.y + 1.3, z + 3.8);
    cam.lookAt(x + off, p.position.y + 1.0, z);
    this.sky.timeOfDay = 10;
    this.creatorSpin = true;
  }

  endCreator() {
    this.creatorSpin = false;
    this.player.character.group.visible = false;
    this.showTitle();
  }

  resetWorldState() {
    this.flags = {};
    this.totalHours = 0;
    this.inventory.items = {};
    this.inventory.equipped = { weapon: null, armor: null, ring: null };
  }

  startNewGame() {
    this.creatorSpin = false;
    this.resetWorldState();
    const inv = this.inventory, p = this.player;
    inv.add('rusty_sword', 1, true); inv.add('travelers_garb', 1, true); inv.add('honey_bread', 2, true); inv.add('potion_health', 1, true);
    inv.equipped.weapon = 'rusty_sword';
    inv.equipped.armor = 'travelers_garb';
    p.stats.gold = 20;
    p.buildCharacter(p.look);
    const d = this.structures.points['brightwater.elder'];
    const L = LOC.brightwater;
    const x = d.x + (L.x - d.x) * 0.25, z = d.z + (L.z - d.z) * 0.25;
    p.teleport(x, this.world.heightAt(x, z) + 0.1, z);
    p.yaw = Math.atan2(d.x - x, d.z - z);
    this.cameraRig.yaw = p.yaw + Math.PI;
    this.cameraRig.pitch = -0.15;
    this.sky.timeOfDay = 8.5;
    this.quests.start('main');
    this.enterPlay();
    this.ui.banner('Brightwater', 'The morning after the sky cracked open.', 0xf2d27a);
    setTimeout(() => this.ui.toast(this.isTouch ? 'Tap E near Elder Maren to talk' : 'Press E near Elder Maren to talk · H for controls'), 2500);
    this.save.autosave();
  }

  startFromSave(data) {
    this.resetWorldState();
    this.save.apply(data);
    this.enterPlay();
    this.ui.toast(`Welcome back, ${this.player.name}`);
  }

  enterPlay() {
    this.mode = 'play';
    this.ui.hideTitle();
    this.ui.showHUD(true);
    this.cameraRig.mode = 'follow';
    this.player.character.group.visible = true;
    this.input.enabled = true;
    this.audio.init();
    this.terrain.update(this.player.position.x, this.player.position.z, true);
    this.vegetation.lastX = 1e9;
    this.story.updateSockets();
  }

  // ---------------------------------------------------------------- loop
  start() {
    this.timer = new THREE.Timer();
    this.timer.connect?.(document);
    const frame = () => {
      requestAnimationFrame(frame);
      this.frame();
    };
    frame();
  }

  frame() {
    this.timer.update();
    let raw = Math.min(this.timer.getDelta(), 0.05);
    this.rawDt = raw;
    if (this.hitStop > 0) { this.hitStop -= raw; raw *= 0.08; }
    else if (this.slowT > 0) { this.slowT -= raw; raw *= this.slowScale; }
    const dt = this.paused ? 0 : raw;
    this.dt = dt;
    this.time += dt;
    this.uniforms.uTime.value = this.time;
    this.input.poll(raw);
    this.update(dt, raw);
    this.render();
    this.input.endFrame();
  }

  update(dt, raw) {
    const cam = this.camera;
    if (this.mode === 'play' && !this.paused) {
      const inp = this.input;
      if (inp.pressed('interact') && !this.player.dead) this.interact.use();
      if (inp.pressed('potion') && !this.player.dead) this.inventory.quickPotion();
      this.player.update(dt, inp, this.cameraRig);
      this.combat.update(dt);
      this.spawner.update(dt);
      this.npcs.update(dt);
      this.story.update(dt);
      if (!this.inDialogue) this.interact.update(this.player);
      this.save.update(dt);
      this.totalHours += (dt * 24) / this.sky.dayLength;
    } else if (this.mode === 'creator') {
      this.player.yaw += raw * (this.creatorSpin ? 0.35 : 0);
      this.player.syncCharacter(raw);
      this.npcs.update(raw);
    } else if (this.mode === 'title') {
      this.npcs.update(raw);
    }
    if (this.mode !== 'creator') this.cameraRig.update(raw, this.input, this.player);
    const skyDt = this.mode === 'play' ? dt : raw * 0.3;
    this.sky.update(skyDt, this.mode === 'play' ? this.player.position : cam.position);
    if (!this.inDungeon) {
      this.terrain.update(cam.position.x, cam.position.z);
      this.water.update(cam.position);
      this.grass.update(cam.position, this.player.position);
      this.vegetation.update(cam.position);
    } else this.dungeon.update(dt);
    this.structures.update(dt || raw * 0.5, this.time);
    this.fx.update(dt || raw * 0.5, cam.position);
    this.audio.update(raw);
    this.ui.update(raw);
  }

  render() {
    this.renderer.info.reset();
    this.post.render(this.rawDt || 0.016);
  }
}

const tick = () => new Promise((r) => setTimeout(r, 0));
