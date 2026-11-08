// Boot: check WebGL2, build the world behind a loading screen, then show the
// title (or resume a save picked from the System menu).

import { Game } from './game.js';

const canvas = document.getElementById('scene');
const msg = document.getElementById('load-msg');

function webgl2() {
  try { return !!document.createElement('canvas').getContext('webgl2'); } catch (e) { return false; }
}

async function boot() {
  if (!webgl2()) {
    msg.textContent = 'Sorry: this browser or device does not support WebGL2, which Aetherveil needs.';
    return;
  }
  const game = new Game(canvas);
  window.__game = game;
  const setLoad = (p, m) => {
    const bar = document.getElementById('load-bar');
    if (bar) bar.style.width = `${Math.round(p * 100)}%`;
    if (m) msg.textContent = m;
  };
  setLoad(0, 'Preparing');
  try {
    await game.init((p, m) => game.ui ? game.ui.setLoading(p, m) : setLoad(p, m));
  } catch (err) {
    console.error(err);
    msg.textContent = 'Failed to start: ' + err.message;
    return;
  }
  game.start();
  const params = new URLSearchParams(location.search);
  const slot = sessionStorage.getItem('aetherveil.loadSlot');
  sessionStorage.removeItem('aetherveil.loadSlot');
  const data = slot ? game.save.load(slot) : null;
  if (data) game.startFromSave(data);
  else if (params.has('play')) { game.player.name = 'Wayfarer'; game.startNewGame(); }
  else game.showTitle();
  game.ui.hideLoading();
  window.__ready = true;
  // Register the offline cache for this game (scoped to its own folder).
  if ('serviceWorker' in navigator && location.protocol === 'https:') {
    navigator.serviceWorker.register('./sw.js', { scope: './' }).catch(() => {});
  }
}

boot();
