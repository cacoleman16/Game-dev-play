// Inventory and equipment: stacks of items, three equipment slots, using
// potions, buying and selling.

import { ITEMS } from './content.js';

export class Inventory {
  constructor(game) {
    this.game = game;
    this.items = {}; // id -> count
    this.equipped = { weapon: null, armor: null, ring: null };
  }

  add(id, n = 1, silent = false) {
    if (!ITEMS[id]) { console.warn('unknown item', id); return; }
    this.items[id] = (this.items[id] || 0) + n;
    this.game.events.emit('inventory:add', id, n, silent);
    this.game.events.emit('inventory:changed');
  }

  remove(id, n = 1) {
    if (!this.items[id]) return false;
    this.items[id] -= n;
    if (this.items[id] <= 0) {
      delete this.items[id];
      for (const k in this.equipped) if (this.equipped[k] === id) this.unequip(k);
    }
    this.game.events.emit('inventory:changed');
    return true;
  }

  count(id) { return this.items[id] || 0; }
  has(id, n = 1) { return this.count(id) >= n; }

  slotFor(id) {
    const t = ITEMS[id].type;
    return t === 'weapon' || t === 'armor' || t === 'ring' ? t : null;
  }

  equip(id) {
    const slot = this.slotFor(id);
    if (!slot || !this.has(id)) return;
    this.equipped[slot] = id;
    this.onEquipChange(slot);
  }

  unequip(slot) {
    this.equipped[slot] = null;
    this.onEquipChange(slot);
  }

  onEquipChange(slot) {
    const p = this.game.player;
    if (slot === 'weapon') p.updateWeaponModel();
    if (slot === 'armor') p.buildCharacter();
    p.stats.hp = Math.min(p.stats.hp, p.maxHp);
    p.stats.mp = Math.min(p.stats.mp, p.maxMp);
    p.stats.sp = Math.min(p.stats.sp, p.maxSp);
    this.game.audio?.play('equip');
    this.game.events.emit('inventory:changed');
  }

  bonus(stat) {
    let n = 0;
    for (const k in this.equipped) {
      const id = this.equipped[k];
      if (id && ITEMS[id][stat]) n += ITEMS[id][stat];
    }
    return n;
  }

  use(id) {
    const it = ITEMS[id];
    if (!it || !this.has(id)) return false;
    if (this.slotFor(id)) {
      if (this.equipped[this.slotFor(id)] === id) this.unequip(this.slotFor(id));
      else this.equip(id);
      return true;
    }
    if (it.type === 'potion') {
      const p = this.game.player, st = p.stats;
      if (it.heal) st.hp = Math.min(p.maxHp, st.hp + it.heal);
      if (it.mana) st.mp = Math.min(p.maxMp, st.mp + it.mana);
      if (it.stamina) st.sp = Math.min(p.maxSp, st.sp + it.stamina);
      this.remove(id);
      this.game.audio?.play('drink');
      if (it.heal) this.game.fx.healAura(p.position);
      this.game.events.emit('toast', `Used ${it.name}`);
      return true;
    }
    return false;
  }

  // Best healing item for the quick-potion key.
  quickPotion() {
    const p = this.game.player;
    const order = p.stats.hp / p.maxHp < 0.99 ? ['potion_health', 'roast_fish', 'honey_bread', 'elixir'] : ['potion_mana', 'potion_stamina'];
    for (const id of order) if (this.has(id)) return this.use(id);
    this.game.events.emit('toast', 'No potions left');
    return false;
  }

  sellPrice(id) { return Math.max(1, Math.floor(ITEMS[id].value * 0.4)); }
  buyPrice(id) { return ITEMS[id].value; }

  buy(id) {
    const st = this.game.player.stats;
    const price = this.buyPrice(id);
    if (st.gold < price) { this.game.audio?.play('error'); return false; }
    st.gold -= price;
    this.add(id, 1, true);
    this.game.audio?.play('coin');
    return true;
  }

  sell(id) {
    if (!this.has(id) || ITEMS[id].type === 'quest') return false;
    if (Object.values(this.equipped).includes(id) && this.count(id) === 1) return false;
    this.remove(id);
    this.game.player.stats.gold += this.sellPrice(id);
    this.game.audio?.play('coin');
    return true;
  }

  serialize() { return { items: this.items, equipped: this.equipped }; }
  restore(d) {
    this.items = { ...(d.items || {}) };
    this.equipped = { weapon: null, armor: null, ring: null, ...(d.equipped || {}) };
  }
}
