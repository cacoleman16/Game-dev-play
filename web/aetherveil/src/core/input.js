// Unified input: keyboard + mouse (pointer lock), gamepad, and the on-screen
// touch controls all feed the same move vector, look deltas and action names.

const KEYMAP = {
  KeyW: 'up', ArrowUp: 'up', KeyS: 'down', ArrowDown: 'down', KeyA: 'left', ArrowLeft: 'left', KeyD: 'right', ArrowRight: 'right',
  ShiftLeft: 'sprint', ShiftRight: 'sprint', Space: 'jump', KeyE: 'interact', KeyF: 'cast', KeyQ: 'dodge', KeyR: 'draw',
  Digit1: 'spell1', Digit2: 'spell2', Digit3: 'spell3', Digit4: 'spell4', Tab: 'lockon', KeyI: 'inventory', KeyJ: 'journal',
  KeyM: 'map', Escape: 'menu', KeyP: 'menu', KeyV: 'view', KeyC: 'crouch', KeyH: 'help', KeyZ: 'potion', KeyX: 'block', KeyT: 'wait', KeyG: 'surge', KeyK: 'attack',
};

export class Input {
  constructor(game, canvas) {
    this.game = game;
    this.canvas = canvas;
    this.keys = new Set();
    this.down = new Set(); // action names currently held
    this.pressedSet = new Set(); // pressed this frame
    this.lookX = 0; this.lookY = 0;
    this.zoom = 0;
    this.move = { x: 0, y: 0 };
    this.touchMove = { x: 0, y: 0, active: false };
    this.mouseButtons = 0;
    this.locked = false;
    this.enabled = true; // gameplay input (false while menus are open)
    this.lastDevice = 'keyboard';
    this.gpPrev = [];

    window.addEventListener('keydown', (e) => {
      if (e.target && (e.target.tagName === 'INPUT' || e.target.tagName === 'TEXTAREA')) return;
      const a = KEYMAP[e.code];
      if (a) {
        if (e.code === 'Tab' || e.code === 'Space' || e.code.startsWith('Arrow')) e.preventDefault();
        if (!e.repeat) this.press(a);
        this.down.add(a);
      }
      this.lastDevice = 'keyboard';
    });
    window.addEventListener('keyup', (e) => {
      const a = KEYMAP[e.code];
      if (a) this.down.delete(a);
    });
    window.addEventListener('blur', () => { this.down.clear(); this.mouseButtons = 0; });

    canvas.addEventListener('mousedown', (e) => {
      if (this.game.isTouch) return;
      if (!this.locked && this.enabled && this.game.mode === 'play') {
        canvas.requestPointerLock?.();
        return;
      }
      if (e.button === 0) { this.press('attack'); this.down.add('attack'); }
      if (e.button === 2) this.down.add('block');
      if (e.button === 1) this.press('lockon');
      this.mouseButtons |= 1 << e.button;
      this.lastDevice = 'keyboard';
    });
    window.addEventListener('mouseup', (e) => {
      if (e.button === 2) this.down.delete('block');
      if (e.button === 0) this.down.delete('attack');
      this.mouseButtons &= ~(1 << e.button);
    });
    canvas.addEventListener('contextmenu', (e) => e.preventDefault());
    document.addEventListener('pointerlockchange', () => {
      this.locked = document.pointerLockElement === canvas;
      this.game.events.emit('input:lock', this.locked);
    });
    window.addEventListener('mousemove', (e) => {
      if (!this.locked) return;
      const s = this.game.settings.mouseSensitivity;
      this.lookX += e.movementX * 0.0022 * s;
      this.lookY += e.movementY * 0.0022 * s * (this.game.settings.invertY ? -1 : 1);
    });
    canvas.addEventListener('wheel', (e) => { this.zoom += Math.sign(e.deltaY); }, { passive: true });
  }

  press(a) {
    this.pressedSet.add(a);
    this.game.events.emit('input:press', a);
  }

  pressed(a) { return this.enabled && this.pressedSet.has(a); }
  held(a) { return this.enabled && this.down.has(a); }
  // Menu keys work even when gameplay input is disabled.
  pressedAny(a) { return this.pressedSet.has(a); }

  releasePointer() { if (this.locked) document.exitPointerLock?.(); }

  // Called once per frame before game logic.
  poll(dt) {
    this.pollGamepad(dt);
    let x = 0, y = 0;
    if (this.down.has('left')) x -= 1;
    if (this.down.has('right')) x += 1;
    if (this.down.has('up')) y += 1;
    if (this.down.has('down')) y -= 1;
    if (this.gpMove) { x += this.gpMove.x; y += this.gpMove.y; }
    if (this.touchMove.active) { x += this.touchMove.x; y += this.touchMove.y; }
    const len = Math.hypot(x, y);
    if (len > 1) { x /= len; y /= len; }
    this.move.x = this.enabled ? x : 0;
    this.move.y = this.enabled ? y : 0;
  }

  // Called at the end of the frame.
  endFrame() {
    this.pressedSet.clear();
    this.lookX = 0; this.lookY = 0; this.zoom = 0;
  }

  pollGamepad(dt) {
    const pads = navigator.getGamepads ? navigator.getGamepads() : [];
    const gp = pads && [...pads].find((p) => p && p.connected);
    this.gpMove = null;
    if (!gp) return;
    const dz = (v) => (Math.abs(v) < 0.18 ? 0 : (v - Math.sign(v) * 0.18) / 0.82);
    const lx = dz(gp.axes[0] || 0), ly = dz(gp.axes[1] || 0), rx = dz(gp.axes[2] || 0), ry = dz(gp.axes[3] || 0);
    if (lx || ly) { this.gpMove = { x: lx, y: -ly }; this.lastDevice = 'gamepad'; }
    if (rx || ry) {
      this.lookX += rx * 2.6 * dt * this.game.settings.mouseSensitivity;
      this.lookY += ry * 1.8 * dt * this.game.settings.mouseSensitivity * (this.game.settings.invertY ? -1 : 1);
    }
    const map = { 0: 'jump', 1: 'dodge', 2: 'attack', 3: 'interact', 4: 'block', 5: 'cast', 6: 'potion', 7: 'sprint', 8: 'map', 9: 'menu', 10: 'surge', 11: 'lockon', 12: 'spellPrev', 13: 'spellNext', 14: 'spellPrev', 15: 'spellNext' };
    gp.buttons.forEach((b, i) => {
      const a = map[i];
      if (!a) return;
      const was = this.gpPrev[i];
      if (b.pressed && !was) { this.press(a); this.lastDevice = 'gamepad'; }
      if (b.pressed) this.down.add(a);
      else if (was) this.down.delete(a);
      this.gpPrev[i] = b.pressed;
    });
  }
}
