// A small line-icon set (24x24, stroke = currentColor) injected as an SVG
// sprite, so the HUD and menus never rely on emoji.

const P = {
  fire: '<path d="M12 21c-3.9 0-6.5-2.7-6.5-6.2 0-3.8 3.2-5.6 3.6-9.3 2.4 1.5 3.6 3.6 3.6 5.7 1-.8 1.6-2 1.7-3.4 2.3 1.9 4.1 4.4 4.1 7.1 0 3.4-2.6 6.1-6.5 6.1z"/><path d="M12 21c-1.6 0-2.7-1.1-2.7-2.6 0-1.7 1.6-2.4 2.2-4 1.6 1 3.2 2.4 3.2 4 0 1.5-1.1 2.6-2.7 2.6z"/>',
  heal: '<circle cx="12" cy="12" r="8.5"/><path d="M12 7.5v9M7.5 12h9"/>',
  frost: '<path d="M12 3v18M4.2 7.5l15.6 9M4.2 16.5l15.6-9"/><path d="M9.5 4.5 12 7l2.5-2.5M9.5 19.5 12 17l2.5 2.5M4.6 10.6l3.4-.9-.9-3.4M19.4 13.4l-3.4.9.9 3.4M4.6 13.4l3.4.9-.9 3.4M19.4 10.6l-3.4-.9.9-3.4"/>',
  bolt: '<path d="M13.5 2.5 5 13.5h6l-1.5 8 8.5-11h-6z"/>',
  sword: '<path d="M14.5 3.5h6v6L10 20l-2-2z"/><path d="M5 15l4 4M3.5 20.5l3-3M12.5 9.5l2 2"/>',
  jump: '<path d="M12 18V5M6.5 10.5 12 5l5.5 5.5"/><path d="M5 21h14"/>',
  dodge: '<path d="M19 12a7 7 0 1 1-2.1-5"/><path d="M19.5 4.5v4.2h-4.2"/>',
  shield: '<path d="M12 3 4.5 6v5.5c0 4.6 3.2 8.3 7.5 9.5 4.3-1.2 7.5-4.9 7.5-9.5V6z"/><path d="M12 7v10"/>',
  spark: '<path d="M12 3l2.1 6.9L21 12l-6.9 2.1L12 21l-2.1-6.9L3 12l6.9-2.1z"/>',
  swap: '<path d="M4 8h13M13.5 4.5 17 8l-3.5 3.5M20 16H7M10.5 12.5 7 16l3.5 3.5"/>',
  sprint: '<path d="M5 6l6 6-6 6M12 6l6 6-6 6"/>',
  potion: '<path d="M9.5 3h5M10.5 3v5.2L6 15.5A4.2 4.2 0 0 0 9.6 21h4.8a4.2 4.2 0 0 0 3.6-5.5L13.5 8.2V3"/><path d="M7.5 14h9"/>',
  target: '<circle cx="12" cy="12" r="7.5"/><circle cx="12" cy="12" r="2"/><path d="M12 2v4M12 18v4M2 12h4M18 12h4"/>',
  menu: '<path d="M4 7h16M4 12h16M4 17h16"/>',
  map: '<path d="M3.5 6.5 9 4l6 2.5 5.5-2.5v13.5L15 20l-6-2.5-5.5 2.5z"/><path d="M9 4v13.5M15 6.5V20"/>',
  house: '<path d="M4 11 12 4l8 7"/><path d="M6 10v9.5h12V10M10 19.5V14h4v5.5"/>',
  crystal: '<path d="M12 2.5 17 9l-5 12.5L7 9z"/><path d="M7 9h10M12 2.5V21.5"/>',
  diamond: '<path d="M12 3l7 9-7 9-7-9z"/>',
  ring: '<circle cx="12" cy="14" r="6"/><path d="M9.5 6.5 12 3l2.5 3.5L12 8z"/>',
  armor: '<path d="M8 3.5 4 6v5l3 1v8.5h10V12l3-1V6l-4-2.5c-.6 1.7-2.2 2.8-4 2.8S8.6 5.2 8 3.5z"/>',
  leaf: '<path d="M5 19c0-8 5-13 14-14-1 9-6 14-14 14z"/><path d="M5 19 13 11"/>',
  scroll: '<path d="M7 4h11a2 2 0 0 1 0 4H8M7 4a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h10V8"/><path d="M10 12h5M10 15.5h5"/>',
  coin: '<circle cx="12" cy="12" r="8"/><path d="M12 7.5v9M9.5 9.5c.4-1 1.3-1.5 2.5-1.5 1.4 0 2.5.8 2.5 1.9 0 2.6-5 1.6-5 4.2 0 1.1 1.1 1.9 2.5 1.9 1.2 0 2.1-.6 2.5-1.6"/>',
  hand: '<path d="M8 12V6.5a1.5 1.5 0 0 1 3 0V11V4.5a1.5 1.5 0 0 1 3 0V11V6a1.5 1.5 0 0 1 3 0v7.5c0 4.1-2.6 7-6.3 7-2.3 0-3.6-1-4.9-2.8L4.5 14a1.4 1.4 0 0 1 2.2-1.7z"/>',
  surge: '<path d="M12 2v5M12 17v5M2 12h5M17 12h5M5 5l3.5 3.5M15.5 15.5 19 19M19 5l-3.5 3.5M8.5 15.5 5 19"/><circle cx="12" cy="12" r="2.5"/>',
  heavy: '<path d="M6 18 18 6M14 4h6v6"/><path d="M4 14l6 6M3 21l3-3"/><circle cx="18" cy="18" r="2"/>',
};

export function injectIcons() {
  if (document.getElementById('icon-sprite')) return;
  const syms = Object.entries(P).map(([k, v]) => `<symbol id="i-${k}" viewBox="0 0 24 24">${v}</symbol>`).join('');
  const holder = document.createElement('div');
  holder.innerHTML = `<svg id="icon-sprite" xmlns="http://www.w3.org/2000/svg" style="position:absolute;width:0;height:0;overflow:hidden" aria-hidden="true">${syms}</svg>`;
  document.body.prepend(holder.firstChild);
}

export const icon = (name, cls = '') => `<svg class="ico ${cls}" aria-hidden="true"><use href="#i-${name}"/></svg>`;

export const SPELL_ICON = { firebolt: 'fire', heal: 'heal', frostlance: 'frost', skybreaker: 'bolt' };
export const ITEM_ICON = { weapon: 'sword', armor: 'armor', ring: 'ring', potion: 'potion', material: 'leaf', quest: 'scroll' };
