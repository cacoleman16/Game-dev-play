// The people of Lumeria: where they stand, what they look like, what they
// do, and what they say. Dialogue is a small graph of nodes whose options
// can check and change quest state.

import * as THREE from 'three';
import { Character, makeSword, makeStaff, makeTool } from './character.js';
import { LOC } from '../world/locations.js';
import { dampAngle } from '../core/util.js';
import { ITEMS } from '../systems/content.js';
import { VOICES } from '../audio/audio.js';

const GREETINGS = ['Morning!', 'Safe travels.', 'Fine day, isn\'t it?', 'Mind the wolves out there.', 'Hello there.', 'Good to see you.'];

const bye = (t = 'Farewell.') => ({ text: t, next: null });

export const NPC_DEFS = [
  {
    id: 'maren', name: 'Elder Maren', title: 'Village Elder',
    at: (s) => s.points['brightwater.elder'], pose: 'talk', prop: 'staff',
    look: { female: true, skin: 0xeac2a0, hair: 0xd8d4d0, hairStyle: 'bob', top: 0x5a3a7a, trim: 0xe6c87a, skirt: true, pants: 0x3a2a4a, scarf: null, pauldrons: false, height: 0.95, eyes: 0x6a4a3a },
    dialogue: (g) => {
      const st = g.quests.stage('main');
      const done = g.quests.done('main');
      const n = {};
      if (st <= 0 && !done) {
        n.start = { text: "Ah, {name}, you're awake. Did last night's tremor shake you from bed too? Look north, child, above the lake. The Heartshard has cracked, and the sky has torn open beside it.", options: [
          { text: 'What is the Heartshard?', next: 'lore' },
          { text: 'What should I do?', next: 'task' },
        ] };
        n.lore = { text: 'For a thousand years that crystal has held the aether that binds earth and sky. Our harvests, our winds, even the floating isles rest upon it. If it fails, Lumeria will come apart like wet paper.', options: [{ text: 'What should I do?', next: 'task' }] };
        n.task = { text: "Go north along the road to the Scholars' Camp on the lake shore. Ysolde has studied the crystal all her life. Take these; the wilds have grown restless since the cracking.", options: [
          { text: "I'll go at once.", next: null, action: (g) => { g.quests.start('main'); g.quests.advance('main', 1); g.inventory.add('potion_health', 2); } },
        ] };
      } else if (done) {
        n.start = { text: 'The sky is whole and the Heartshard sings again. Brightwater will tell this story for a hundred years, {name}. Rest here whenever you like.', options: [bye('Thank you, Elder.')] };
      } else if (st <= 2) {
        n.start = { text: "Ysolde waits at the Scholars' Camp. Follow the road north until you see the lake. Glide down from the hills if you're in a hurry: jump, then jump again in the air.", options: [bye()] };
      } else if (st === 3) {
        n.start = { text: 'Three shards, scattered to the forest, the peaks and the badlands. Rest at the waystones when you are weary; they will carry you back to any you have touched.', options: [
          { text: 'Any advice?', next: 'adv' }, bye(),
        ] };
        n.adv = { text: 'Old stone hates fire, beasts of the frost hate it more, and things born of flame cannot abide the cold. Watch the ground, too: when it glows red, step aside.', options: [bye()] };
      } else {
        n.start = { text: 'You have all three! Then go, child. Return them to the heart of the world.', options: [bye()] };
      }
      return n;
    },
  },
  {
    id: 'lyra', name: 'Lyra', title: 'Merchant', shop: 'lyra',
    at: (s) => s.points['brightwater.market'], pose: 'idle',
    look: { female: true, skin: 0xf3d2b4, hair: 0xb8542a, hairStyle: 'ponytail', top: 0x3f8f6a, trim: 0xf4ead6, apron: 0xf4ead6, skirt: true, scarf: null, pauldrons: false, eyes: 0x2a8a4a },
    dialogue: (g) => {
      const n = {};
      const q = g.quests;
      n.start = { text: 'Welcome, welcome! Potions, bread, a sturdy jerkin. If it fits in a pack, Lyra sells it.', options: [
        { text: 'Show me your wares.', next: null, action: (g, npc) => g.ui.openShop(npc) },
      ] };
      if (!q.get('letter')) n.start.options.push({ text: 'Anything I can help with?', next: 'letter' });
      else if (q.active('letter')) n.start.options.push({ text: 'About that letter...', next: 'letter2' });
      n.start.options.push(bye());
      n.letter = { text: "My father keeps the old light out at Gull Point, past Saltmere. Stubborn as a barnacle; won't come inland. Would you take him this letter? He'll make it worth your while.", options: [
        { text: "I'll deliver it.", next: 'thanks', action: (g) => { g.inventory.add('letter', 1); g.quests.start('letter'); } },
        { text: 'Not right now.', next: null },
      ] };
      n.thanks = { text: 'Follow the east road through the farm to Saltmere, then along the shore. Tell him to eat something green.', options: [bye()] };
      n.letter2 = { text: "Gull Point is past Saltmere on the eastern shore. You can't miss the red and white tower.", options: [bye()] };
      return n;
    },
  },
  {
    id: 'garrick', name: 'Garrick', title: 'Blacksmith', shop: 'garrick',
    at: (s) => s.points['brightwater.forge'], pose: 'work', prop: 'hammer',
    look: { skin: 0xd8a882, hair: 0x2a1a14, hairStyle: 'short', beard: true, top: 0x5a4a3a, apron: 0x3a2a20, trim: 0x8a6a4a, scarf: null, build: 1.2, height: 1.05 },
    dialogue: (g) => {
      const n = {};
      const q = g.quests;
      n.start = { text: '*clang* Need an edge on that blade, or a new blade entirely?', options: [{ text: 'Let me see your stock.', next: null, action: (g, npc) => g.ui.openShop(npc) }] };
      if (!q.get('cores')) n.start.options.push({ text: 'You look like you need something.', next: 'cores' });
      else if (q.active('cores') && q.stage('cores') === 1) n.start.options.push({ text: 'I have the golem cores.', next: 'coresDone' });
      n.start.options.push(bye());
      n.cores = { text: "Golem cores burn hotter than any coal. The old stone guardians in the Whisperwood ruins carry them. Bring me two and I'll forge you a steel longsword, no charge.", options: [
        { text: 'Deal.', next: null, action: (g) => g.quests.start('cores') }, bye('Maybe later.'),
      ] };
      n.coresDone = { text: 'Look at that glow! Give me a moment... *CLANG* ...there. Finest blade I have ever made. Take it.', options: [
        { text: 'Thank you!', next: null, action: (g) => { g.inventory.remove('golem_core', 2); g.quests.advance('cores'); } },
      ] };
      return n;
    },
  },
  {
    id: 'sera', name: 'Sera', title: 'Herbalist', shop: 'sera',
    at: (s) => s.points['brightwater.garden'], pose: 'idle',
    look: { female: true, skin: 0xc89070, hair: 0x2a2a3a, hairStyle: 'long', top: 0x6a8a3a, trim: 0xd8c890, skirt: true, scarf: 0xe8a8c8, pauldrons: false, hat: 'wizard', hatColor: 0x4a6a2a, eyes: 0x6a3a8a },
    dialogue: (g) => {
      const n = {};
      const q = g.quests;
      n.start = { text: 'Mind the seedlings! Oh, a customer. Tonics, draughts, and the occasional elixir, for those with coin.', options: [{ text: 'Show me your remedies.', next: null, action: (g, npc) => g.ui.openShop(npc) }] };
      if (!q.get('herbs')) n.start.options.push({ text: 'Need any help?', next: 'herbs' });
      else if (q.active('herbs') && q.stage('herbs') === 1) n.start.options.push({ text: 'I brought Glowcaps.', next: 'herbsDone' });
      n.start.options.push(bye());
      n.herbs = { text: 'Glowcaps! Little mushrooms that glow blue. They grow in shady forest and around the crystal meadows. Five would see me through the season.', options: [
        { text: "I'll find some.", next: null, action: (g) => g.quests.start('herbs') }, bye('Not now.'),
      ] };
      n.herbsDone = { text: 'Oh, they are perfect, still glowing! Here, a Crystal Elixir. Brewed it myself.', options: [
        { text: 'Thank you.', next: null, action: (g) => { g.inventory.remove('glowcap', 5); g.quests.advance('herbs'); } },
      ] };
      return n;
    },
  },
  {
    id: 'mira', name: 'Mira', title: 'Villager',
    at: () => ({ x: LOC.brightwater.x + 10, z: LOC.brightwater.z + 4 }), pose: 'idle',
    look: { female: true, skin: 0xf6dcc6, hair: 0xe8c86a, hairStyle: 'twin', top: 0xd86a8a, trim: 0xffffff, skirt: true, scarf: null, pauldrons: false, height: 0.94, eyes: 0x4a7ad8 },
    dialogue: (g) => {
      const n = {};
      const q = g.quests;
      if (!q.get('locket')) {
        n.start = { text: "*sniff* Sorry. The Redcloaks came through two nights ago and took my grandmother's locket. It's all I had of her.", options: [
          { text: "I'll get it back.", next: 'go', action: (g) => g.quests.start('locket') },
          { text: 'Who are the Redcloaks?', next: 'who' }, bye("I'm sorry."),
        ] };
        n.who = { text: 'Bandits. They camp at the edge of the western woods, southwest of here. Captain Dain says there are too many to fight.', options: [{ text: "I'll get your locket back.", next: 'go', action: (g) => g.quests.start('locket') }, bye()] };
        n.go = { text: "Really? Oh, thank you! Be careful. They keep their loot in a chest in the middle of the camp.", options: [bye()] };
      } else if (q.active('locket') && q.stage('locket') === 1) {
        n.start = { text: "Is that... my locket! Oh, I could cry! Please, take this ring. Grandmother would want it to go to someone brave.", options: [
          { text: 'Glad I could help.', next: null, action: (g) => { g.inventory.remove('locket'); g.quests.advance('locket'); } },
        ] };
      } else if (q.done('locket')) {
        n.start = { text: 'I wear it every day now. Thank you, {name}.', options: [bye()] };
      } else n.start = { text: 'The Redcloak camp is southwest, at the edge of the woods. Please be careful.', options: [bye()] };
      return n;
    },
  },
  {
    id: 'dain', name: 'Captain Dain', title: 'Guard Captain',
    at: () => ({ x: 34, z: 386 }), pose: 'guard', prop: 'spear',
    look: { skin: 0xc8946a, hair: 0x3a2a1a, hairStyle: 'short', hat: 'helm', top: 0x7a8494, trim: 0x2f5fa8, pants: 0x3a3a48, scarf: 0x2f5fa8, build: 1.1, height: 1.04 },
    dialogue: (g) => {
      const n = {};
      const q = g.quests;
      if (!q.get('bounty')) {
        n.start = { text: "Halt. Oh, it's you. These are bad days, traveller. Wolves at the farms, Redcloaks on the roads, lights in the sky.", options: [
          { text: 'Can I help?', next: 'bounty' }, { text: 'Any news?', next: 'news' }, bye(),
        ] };
        n.bounty = { text: "I've posted a bounty on the Redcloaks: put six of them down and the purse is yours. Two hundred gold. Their camp is southwest, by the treeline.", options: [{ text: "Consider it done.", next: null, action: (g) => g.quests.start('bounty') }, bye('Not yet.')] };
      } else if (q.active('bounty') && q.stage('bounty') === 1) {
        n.start = { text: 'Six Redcloaks? The roads will be quieter tonight. Your purse, as promised.', options: [{ text: 'Pleasure doing business.', next: null, action: (g) => g.quests.advance('bounty') }] };
      } else {
        n.start = { text: 'Stay sharp out there.', options: [{ text: 'Any news?', next: 'news' }, bye()] };
      }
      n.news = { text: 'They say the old watchtower on the east road has treasure at the top, if you can stomach the climb. And keep away from the standing stones at night; the wisps come out.', options: [bye()] };
      return n;
    },
  },
  {
    id: 'pip', name: 'Pip', title: 'Curious Kid', wander: 9,
    at: () => ({ x: LOC.brightwater.x + 2, z: LOC.brightwater.z - 6 }), pose: 'idle',
    look: { skin: 0xf3d2b4, hair: 0x6a3a1a, hairStyle: 'spiky', top: 0xe8a83a, trim: 0x7a4a2a, scarf: 0x3a8ad8, pauldrons: false, height: 0.72, build: 0.95, eyes: 0x3a8a3a },
    dialogue: () => ({
      start: { text: 'Whoa, is that a real sword?! Hey, want to know a secret?', options: [
        { text: 'Tell me a secret.', next: 's1' }, { text: 'Another secret?', next: 's2' }, { text: 'Any more?', next: 's3' }, bye('Run along, Pip.'),
      ] },
      s1: { text: 'If you jump and then jump AGAIN in the air, you glide! I saw a scholar do it with glowy wings. There are treasure chests on the floating islands, I bet!', options: [{ text: 'Another?', next: 's2' }, bye()] },
      s2: { text: "Hold your block and things hit you way softer. And if you dodge right when they swing, they miss completely! Garrick taught me. Kind of.", options: [{ text: 'Any more?', next: 's3' }, bye()] },
      s3: { text: "Crouch and sneak and monsters don't notice you as much. And at night the fireflies come out in the meadows. That's not a fighting secret, it's just nice.", options: [bye('Thanks, Pip.')] },
    }),
  },
  {
    id: 'tobin', name: 'Tobin', title: 'Farmer',
    at: (s) => s.points['farm.field'], pose: 'idle', prop: 'pitchfork',
    look: { skin: 0xd8a070, hair: 0x8a6a3a, hairStyle: 'short', hat: 'cap', hatColor: 0xd8b85a, top: 0x6a7a3a, trim: 0x8a6a4a, scarf: 0xc8453a, beard: true, pauldrons: false, build: 1.1 },
    dialogue: (g) => {
      const n = {};
      const q = g.quests;
      if (!q.get('wolves')) {
        n.start = { text: "Dusk wolves. Bold as you like, right up to the fences. Lost two sheep this week. I'm a farmer, not a fighter.", options: [
          { text: "I'll deal with the wolves.", next: 'yes', action: (g) => g.quests.start('wolves') }, bye('Good luck with that.'),
        ] };
        n.yes = { text: 'Bless you. Five of them ought to teach the pack a lesson. They prowl the hills north and east of the farm.', options: [bye()] };
      } else if (q.active('wolves') && q.stage('wolves') === 1) {
        n.start = { text: 'Quiet nights again! Here, take this for your trouble, and a few draughts my wife brewed.', options: [{ text: 'Happy to help.', next: null, action: (g) => g.quests.advance('wolves') }] };
      } else if (q.active('wolves')) {
        n.start = { text: 'Hills to the north and east, that\'s where the pack runs.', options: [bye()] };
      } else n.start = { text: 'Fine weather for wheat. Fine weather for everything, really.', options: [bye()] };
      return n;
    },
  },
  {
    id: 'ysolde', name: 'Ysolde', title: 'Aether Scholar',
    at: (s) => s.points['scholars.desk'], pose: 'read', prop: 'book',
    look: { female: true, skin: 0xe8c0a0, hair: 0x4a6ad8, hairStyle: 'long', top: 0x2a3a6a, trim: 0xe6c87a, skirt: true, scarf: null, pauldrons: false, hat: 'wizard', hatColor: 0x2a3a6a, eyes: 0xd8a83a },
    dialogue: (g) => {
      const st = g.quests.stage('main');
      const n = {};
      if (st <= 1 && !g.quests.done('main')) {
        n.start = { text: "Maren sent you? Good. Look up: see how the Heartshard flickers? It holds the aether that keeps our sky whole. When it cracked, three Resonance Shards broke loose.", options: [
          { text: 'Where did they go?', next: 'where' }, { text: 'What happens if we do nothing?', next: 'nothing' },
        ] };
        n.nothing = { text: "The rift widens. The isles fall. Something on the other side is pressing in. I've heard it in my instruments: a voice like breaking glass.", options: [{ text: 'Where did the shards go?', next: 'where' }] };
        n.where = { text: "That's what I need you to learn. Cross the causeway to the island and lay your hand on the pedestal beneath the crystal. It remembers. It will show you.", options: [
          { text: "I'll go to the pedestal.", next: null, action: (g) => { g.quests.start('main'); g.quests.advance('main', 2); } },
        ] };
      } else if (st === 2) {
        n.start = { text: 'The causeway is just north of camp. Touch the pedestal and tell me what you see.', options: [bye()] };
      } else if (st === 3) {
        n.start = { text: "Green to the forest, blue to the peaks, orange to the cliffs. Each shard will have drawn a guardian to it like a moth to a lantern. Be ready for a fight.", options: [{ text: 'Tell me about the guardians.', next: 'g' }, bye()] };
        n.g = { text: "In the ruins, the Warden: an ancient golem, slow but crushing. On Frostspire, an alpha wolf of terrible size. Beneath the Sunscar, something molten. Fire for the first two. For the last, cold.", options: [bye()] };
      } else if (st === 4 || st === 5) {
        n.start = { text: 'All three! Place them in the pedestal. I will be right behind you. Well, a safe distance behind you.', options: [bye()] };
      } else {
        n.start = { text: "The rift is closing! Listen to it hum. I'll be writing about this for the rest of my life.", options: [bye()] };
      }
      return n;
    },
  },
  {
    id: 'bram', name: 'Bram', title: 'Ranger',
    at: (s) => s.points['scholars.fire'], pose: 'sit',
    look: { skin: 0xb07a5a, hair: 0x1a1a1a, hairStyle: 'spiky', top: 0x3a5a3a, trim: 0x7a6a4a, scarf: 0x8a5a2a, cape: 0x2a4a2a, beard: true },
    dialogue: () => ({
      start: { text: "Pull up a log. I've been mapping the floating isles. Two of them carry old chests, I'd swear it. One west of Brightwater, one high over the north-eastern slopes.", options: [{ text: 'How would I reach them?', next: 'how' }, bye()] },
      how: { text: 'Climb something tall and glide. The isle by the forest edge sits low enough to reach from the hills. The other needs a mountain to start from. Watch your stamina; when it runs out, so do your wings.', options: [bye('Thanks for the tip.')] },
    }),
  },
  {
    id: 'wick', name: 'Old Wick', title: 'Lighthouse Keeper',
    at: (s) => s.points['lighthouse.door'], pose: 'idle',
    look: { skin: 0xd8a882, hair: 0xe8e8e8, hairStyle: 'short', beard: true, beardColor: 0xf0f0f0, hat: 'cap', hatColor: 0x2a3a5a, top: 0x2a3a5a, trim: 0xf4ead6, scarf: null, height: 0.96 },
    dialogue: (g) => {
      const n = {};
      if (g.quests.active('letter')) {
        n.start = { text: "A letter? From my Lyra? ... Hah! 'Eat something green.' That girl. Here: this charm kept me light on my feet when I was young. Your legs are younger than mine.", options: [{ text: "She misses you.", next: null, action: (g) => { g.inventory.remove('letter'); g.quests.advance('letter'); } }] };
      } else {
        n.start = { text: "Forty years I've kept this light. Seen ships, storms, and now a hole in the sky. Never a dull decade.", options: [{ text: 'Seen anything strange?', next: 'odd' }, bye()] };
        n.odd = { text: 'Lights rising out of the sea at night and drifting toward the lake. Wisps, the scholars call them. Pretty, until they start throwing things.', options: [bye()] };
      }
      return n;
    },
  },
  {
    id: 'marin', name: 'Marin', title: 'Fisher', shop: 'marin',
    at: (s) => s.points['saltmere.pierEnd'], pose: 'idle',
    look: { female: true, skin: 0xa86a4a, hair: 0x1a1a2a, hairStyle: 'ponytail', top: 0x3a6a8a, trim: 0xf4ead6, scarf: 0xe8c84a, pauldrons: false, eyes: 0x3a3a3a },
    dialogue: () => ({
      start: { text: 'Saltfish, fresh off the boat. Well, fresh-ish. Want some?', options: [{ text: "Let's trade.", next: null, action: (g, npc) => g.ui.openShop(npc) }, bye()] },
    }),
  },
  {
    id: 'aveline', name: 'Sister Aveline', title: 'Shrine Keeper',
    at: () => ({ x: LOC.frostspire.x + 4, z: LOC.frostspire.z + 4 }), pose: 'idle',
    look: { female: true, skin: 0xf0d8c8, hair: 0xf4f4f8, hairStyle: 'long', top: 0xd8e8f8, trim: 0x4a7ab8, skirt: true, scarf: 0xa83a3a, pauldrons: false, eyes: 0x4a7ab8 },
    dialogue: (g) => {
      const n = {};
      const beaten = g.flags.rimefang;
      n.start = beaten
        ? { text: 'The mountain breathes easier with Rimefang at rest. The shard chose you, and its cold is yours to command now. Use it well.', options: [bye()] }
        : { text: "You climbed all this way? Then you have felt it too. The shard fell here and woke Rimefang. The great wolf prowls the shrine grounds, and nothing can come near.", options: [{ text: 'How do I defeat it?', next: 'how' }, bye()] };
      n.how = { text: "Fire. Rimefang is the mountain's winter given teeth. When it rears back and draws breath, get out of its path, or behind it.", options: [bye()] };
      return n;
    },
  },
];

const PROPS = {
  staff: () => makeStaff(0xb48cff),
  hammer: () => makeTool('hammer'),
  book: () => makeTool('book'),
  spear: () => makeTool('spear'),
  pitchfork: () => makeTool('spear'),
};

export class NPC {
  constructor(game, def) {
    this.game = game;
    this.def = def;
    this.id = def.id;
    this.name = def.name;
    const p = def.at(game.structures) || { x: 0, z: 0 };
    const y = p.y ?? game.env.ground(p.x, p.z, 200).y;
    this.home = new THREE.Vector3(p.x, y, p.z);
    this.position = this.home.clone();
    this.ch = new Character(def.look, { outline: game.quality.npcOutlines });
    if (def.prop) {
      const prop = PROPS[def.prop]();
      const hand = def.prop === 'book' ? this.ch.bones.handL : this.ch.bones.handR;
      hand.add(prop);
      if (def.prop === 'book') { prop.position.set(0.05, -0.08, 0.08); prop.rotation.set(0.3, 0, 0.4); }
      else { prop.rotation.x = 1.5; prop.position.y = -0.05; }
    }
    this.yaw = Math.atan2(LOC.brightwater.x - p.x, LOC.brightwater.z - p.z);
    if (def.id === 'marin') this.yaw = Math.PI;
    this.baseYaw = this.yaw;
    this.talking = false;
    this.wanderT = 0;
    this.target = null;
    this.speed = 0;
    game.scene.add(this.ch.group);
    game.interact.add({
      id: 'npc:' + def.id,
      x: () => this.position.x, y: () => this.position.y + 1, z: () => this.position.z, r: 3.0,
      label: `Talk to ${def.name}`,
      action: () => game.dialogue.start(this),
    });
  }

  update(dt) {
    const game = this.game, p = game.player.position;
    const dx = p.x - this.position.x, dz = p.z - this.position.z;
    const d = Math.hypot(dx, dz);
    const visible = d < 140 && !game.inDungeon;
    this.ch.group.visible = visible;
    if (!visible) return;
    let headYaw = 0;
    let moving = false;
    // A passing greeting, now and then.
    this.greetT = (this.greetT ?? 0) - dt;
    if (d < 4 && !this.talking && this.greetT <= 0 && game.mode === 'play' && !game.inDialogue && !(typeof speechSynthesis !== 'undefined' && speechSynthesis.speaking)) {
      this.greetT = 90;
      const line = this.def.greet || GREETINGS[(this.id.length + Math.floor(game.time / 60)) % GREETINGS.length];
      game.audio?.speak(line, { ...(VOICES[this.id] || VOICES.narrator), id: this.id }, true);
    }
    if (this.talking || d < 5) {
      const want = Math.atan2(dx, dz);
      if (this.talking) this.yaw = dampAngle(this.yaw, want, 5, dt);
      else headYaw = Math.max(-1, Math.min(1, ((want - this.yaw + Math.PI * 3) % (Math.PI * 2)) - Math.PI)) * 0.8;
    } else if (this.def.wander) {
      this.wanderT -= dt;
      if (this.wanderT <= 0) {
        this.wanderT = 2 + Math.random() * 4;
        const a = Math.random() * Math.PI * 2, r = Math.random() * this.def.wander;
        this.target = new THREE.Vector3(this.home.x + Math.cos(a) * r, 0, this.home.z + Math.sin(a) * r);
      }
      if (this.target) {
        const tx = this.target.x - this.position.x, tz = this.target.z - this.position.z;
        const td = Math.hypot(tx, tz);
        if (td > 0.5) {
          const sp = 3.6;
          this.position.x += (tx / td) * sp * dt;
          this.position.z += (tz / td) * sp * dt;
          this.yaw = dampAngle(this.yaw, Math.atan2(tx, tz), 8, dt);
          moving = true;
        } else this.target = null;
      }
      this.position.y = game.env.ground(this.position.x, this.position.z, this.position.y + 1).y;
      game.env.resolve(this.position, 0.35, 1.5);
    } else {
      this.yaw = dampAngle(this.yaw, this.baseYaw, 2, dt);
    }
    this.speed = moving ? 3.6 : 0;
    const pose = this.talking ? 'talk' : this.def.pose;
    this.ch.group.position.copy(this.position);
    this.ch.group.rotation.y = this.yaw;
    this.ch.update(dt, { speed: this.speed, grounded: true, vy: 0, pose: d < 40 ? pose : 'idle', headYaw, lookAround: !this.talking && d > 5 });
  }
}

export class NPCs {
  constructor(game) {
    this.game = game;
    this.list = NPC_DEFS.map((d) => new NPC(game, d));
    this.byId = Object.fromEntries(this.list.map((n) => [n.id, n]));
  }
  update(dt) { for (const n of this.list) n.update(dt); }
}

export { ITEMS, makeSword };
