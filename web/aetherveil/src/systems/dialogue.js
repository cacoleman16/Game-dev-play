// Runs conversations: frames the camera, pauses gameplay input, walks the
// dialogue node graph that each NPC (or story event) provides, and voices
// each line in the speaker's own voice.

import { VOICES } from '../audio/audio.js';

export class Dialogue {
  constructor(game) {
    this.game = game;
    this.npc = null;
    this.nodes = null;
    this.node = null;
    this.custom = null;
  }

  get active() { return !!this.nodes; }

  start(npc) {
    const g = this.game;
    this.npc = npc;
    npc.talking = true;
    this.nodes = npc.def.dialogue(g, npc);
    let portrait = null;
    try { portrait = g.ui.portrait(npc.ch, npc.def.look?.height ?? 1); } catch (e) { portrait = null; }
    this.speaker = { name: npc.def.name, title: npc.def.title, portrait };
    this.voice = { ...(VOICES[npc.id] || VOICES.narrator), id: npc.id };
    this.begin(npc.position);
    this.show('start');
    g.events.emit('npc:talk', npc.id);
  }

  // A one-off conversation (signs, visions, story beats). nodes = { start: {...} }
  say(name, nodes, focus = null, title = '') {
    this.npc = null;
    this.nodes = nodes;
    this.speaker = { name, title };
    this.voice = { ...VOICES.narrator, id: 'narrator' };
    this.begin(focus);
    this.show('start');
  }

  begin(focus) {
    const g = this.game;
    g.inDialogue = true;
    g.input.enabled = false;
    g.input.releasePointer();
    if (focus) { g.cameraRig.mode = 'dialogue'; g.cameraRig.focus = focus; }
    g.player.action = null;
    g.player.velocity.set(0, g.player.velocity.y, 0);
    g.audio?.play('talk');
  }

  show(id) {
    const node = this.nodes[id];
    if (!node) { this.end(); return; }
    this.node = node;
    const name = this.game.player.name || 'Wayfarer';
    const opts = (node.options || [{ text: 'Continue', next: null }]).filter((o) => !o.cond || o.cond(this.game));
    this.options = opts;
    const line = node.text.replaceAll('{name}', name);
    this.game.ui.showDialogue(this.speaker, line, opts.map((o) => o.text.replaceAll('{name}', name)));
    this.game.audio?.speak(line, this.voice);
  }

  choose(i) {
    const opt = this.options?.[i];
    if (!opt) return;
    this.game.audio?.play('click');
    const before = this.nodes;
    if (opt.action) opt.action(this.game, this.npc);
    if (!this.nodes || this.nodes !== before) return; // action ended or replaced the conversation
    if (opt.next) {
      // Rebuild nodes: quest state may have changed.
      if (this.npc) this.nodes = this.npc.def.dialogue(this.game, this.npc);
      this.show(opt.next);
    } else this.end();
  }

  end() {
    const g = this.game;
    if (this.npc) this.npc.talking = false;
    this.npc = null;
    this.nodes = null;
    g.inDialogue = false;
    g.input.enabled = !g.ui.menuOpen;
    g.audio?.stopSpeech();
    g.cameraRig.mode = 'follow';
    g.ui.hideDialogue();
  }
}
