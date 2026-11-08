// Item, spell and loot data.

export const ITEMS = {
  // Weapons
  rusty_sword: { name: 'Rusty Sword', type: 'weapon', model: 'rusty', dmg: 10, value: 6, desc: 'Pitted and dull, but better than bare fists.' },
  iron_sword: { name: 'Iron Sword', type: 'weapon', model: 'iron', dmg: 13, value: 70, desc: 'A dependable blade from Garrick\'s forge.' },
  steel_longsword: { name: 'Steel Longsword', type: 'weapon', model: 'steel', dmg: 19, value: 190, desc: 'Gold-hilted and perfectly balanced.' },
  rimeblade: { name: 'Rimeblade', type: 'weapon', model: 'frost', dmg: 21, element: 'frost', value: 380, desc: 'Cold enough to frost the air it cuts. Deals frost damage.' },
  emberbrand: { name: 'Emberbrand', type: 'weapon', model: 'flame', dmg: 23, element: 'fire', value: 420, desc: 'Forged in the Sunscar Vault. Deals fire damage.' },
  aether_edge: { name: 'Aether Edge', type: 'weapon', model: 'crystal', dmg: 32, element: 'aether', value: 999, desc: 'A sword grown from a sliver of the Heartshard.' },
  // Armour
  travelers_garb: { name: "Traveller's Garb", type: 'armor', armor: 4, value: 10, look: { top: 0x2f5fa8, trim: 0xe8d8b0 }, desc: 'Sturdy cloth for a long road.' },
  leather_armor: { name: 'Leather Armour', type: 'armor', armor: 10, value: 80, look: { top: 0x7a5232, trim: 0xc8a070 }, desc: 'Boiled leather, oiled against rain.' },
  chain_hauberk: { name: 'Chain Hauberk', type: 'armor', armor: 18, value: 220, look: { top: 0x8a909a, trim: 0x4a5a7a }, desc: 'Heavy, noisy, and very reassuring.' },
  frostweave_robe: { name: 'Frostweave Robe', type: 'armor', armor: 11, mp: 35, value: 260, look: { top: 0xd8e8f8, trim: 0x4a7ab8 }, desc: 'Woven by the shrine keepers. +35 Mana.' },
  redcloak: { name: 'Redcloak Leathers', type: 'armor', armor: 13, sp: 20, value: 150, look: { top: 0x8a2a2a, trim: 0x2a1a1a }, desc: 'Bandit gear. Light and quick. +20 Stamina.' },
  aether_plate: { name: 'Aether Plate', type: 'armor', armor: 30, hp: 30, value: 900, look: { top: 0xe8e4f0, trim: 0x9f7fff }, desc: 'Crystal-laced armour that hums softly. +30 Health.' },
  // Accessories
  ring_vigor: { name: 'Ring of Vigour', type: 'ring', hp: 25, value: 150, desc: '+25 Health.' },
  ring_focus: { name: 'Ring of Focus', type: 'ring', mp: 30, value: 150, desc: '+30 Mana.' },
  windstep_charm: { name: 'Windstep Charm', type: 'ring', sp: 35, value: 160, desc: '+35 Stamina. Gliding costs less.' },
  // Consumables
  potion_health: { name: 'Healing Draught', type: 'potion', heal: 60, value: 25, desc: 'Restores 60 Health.' },
  potion_mana: { name: 'Aether Tonic', type: 'potion', mana: 50, value: 25, desc: 'Restores 50 Mana.' },
  potion_stamina: { name: 'Windroot Brew', type: 'potion', stamina: 80, value: 20, desc: 'Restores 80 Stamina.' },
  elixir: { name: 'Crystal Elixir', type: 'potion', heal: 999, mana: 999, stamina: 999, value: 120, desc: 'Fully restores Health, Mana and Stamina.' },
  honey_bread: { name: 'Honey Bread', type: 'potion', heal: 25, value: 6, desc: 'Still warm. Restores 25 Health.' },
  roast_fish: { name: 'Roast Saltfish', type: 'potion', heal: 35, stamina: 30, value: 9, desc: 'Restores 35 Health and 30 Stamina.' },
  // Materials
  glowcap: { name: 'Glowcap', type: 'material', value: 5, desc: 'A mushroom that glows softly. Herbalists prize it.' },
  frostpetal: { name: 'Frostpetal', type: 'material', value: 8, desc: 'A pale flower that only blooms in snow.' },
  emberroot: { name: 'Emberroot', type: 'material', value: 8, desc: 'Warm to the touch, even at night.' },
  wolf_pelt: { name: 'Dusk Wolf Pelt', type: 'material', value: 12, desc: 'Thick grey fur.' },
  slime_gel: { name: 'Gloop Gel', type: 'material', value: 3, desc: 'Wobbly. Faintly minty.' },
  aether_dust: { name: 'Aether Dust', type: 'material', value: 20, desc: 'Glittering residue left by wisps.' },
  golem_core: { name: 'Golem Core', type: 'material', value: 60, desc: 'A warm stone heart.' },
  // Quest items
  locket: { name: "Mira's Locket", type: 'quest', value: 0, desc: 'A silver locket engraved with a lighthouse.' },
  letter: { name: 'Sealed Letter', type: 'quest', value: 0, desc: 'Addressed to Old Wick at Gull Point Light.' },
  shard_verdance: { name: 'Shard of Verdance', type: 'quest', value: 0, desc: 'A green crystal shard, warm with forest life.' },
  shard_rime: { name: 'Shard of Rime', type: 'quest', value: 0, desc: 'A blue crystal shard, cold as a mountain night.' },
  shard_ember: { name: 'Shard of Embers', type: 'quest', value: 0, desc: 'An orange crystal shard that pulses like a heartbeat.' },
};

export const SPELLS = {
  firebolt: { name: 'Firebolt', icon: '🔥', cost: 12, cd: 0.55, kind: 'bolt', dmg: 20, element: 'fire', speed: 30, color: 0xff7a2a, desc: 'Hurl a bolt of fire.' },
  heal: { name: 'Mending Light', icon: '✚', cost: 22, cd: 1.5, kind: 'heal', amount: 45, color: 0x9fffb0, desc: 'Restore your health.' },
  frostlance: { name: 'Frost Lance', icon: '❄', cost: 16, cd: 0.7, kind: 'bolt', dmg: 24, element: 'frost', speed: 38, slow: 0.5, color: 0x9fdcff, desc: 'A spear of ice that slows its target.' },
  skybreaker: { name: 'Skybreaker', icon: '⚡', cost: 30, cd: 2.4, kind: 'strike', dmg: 48, radius: 4, element: 'storm', color: 0xd8c8ff, desc: 'Call lightning down on your target.' },
};

// Loot tables: list of [item, chance, min, max]; 'gold' is money.
export const LOOT = {
  starter: [['potion_health', 1, 2, 2], ['honey_bread', 1, 2, 2], ['gold', 1, 25, 25]],
  coast: [['roast_fish', 1, 2, 3], ['potion_stamina', 1, 1, 1], ['gold', 1, 30, 50], ['windstep_charm', 1, 1, 1]],
  ruins: [['potion_health', 1, 1, 2], ['ring_focus', 1, 1, 1], ['gold', 1, 60, 90]],
  frost: [['potion_mana', 1, 2, 2], ['frostpetal', 1, 3, 3], ['gold', 1, 70, 100]],
  bandit: [['locket', 1, 1, 1], ['gold', 1, 80, 120], ['redcloak', 1, 1, 1], ['potion_health', 1, 1, 2]],
  tower: [['steel_longsword', 1, 1, 1], ['gold', 1, 40, 60]],
  sky: [['elixir', 1, 1, 2], ['ring_vigor', 0.5, 1, 1], ['aether_dust', 1, 2, 4], ['gold', 1, 120, 180]],
  vault: [['emberbrand', 1, 1, 1], ['potion_health', 1, 2, 3], ['gold', 1, 150, 200]],
  vault_small: [['potion_health', 0.7, 1, 1], ['potion_mana', 0.5, 1, 1], ['gold', 1, 20, 50]],
  // Creature drops
  gloop: [['slime_gel', 0.7, 1, 2], ['gold', 0.4, 1, 6]],
  wolf: [['wolf_pelt', 0.6, 1, 1], ['gold', 0.2, 2, 8]],
  bandit_drop: [['gold', 0.9, 6, 22], ['potion_health', 0.2, 1, 1], ['honey_bread', 0.2, 1, 1]],
  wisp: [['aether_dust', 0.8, 1, 2]],
  golem: [['golem_core', 1, 1, 1], ['gold', 1, 20, 40]],
  revenant: [['gold', 0.8, 8, 20], ['potion_mana', 0.15, 1, 1]],
  boss_warden: [['golem_core', 1, 2, 2], ['gold', 1, 150, 150], ['chain_hauberk', 1, 1, 1]],
  boss_rimefang: [['wolf_pelt', 1, 3, 3], ['gold', 1, 150, 150], ['rimeblade', 1, 1, 1]],
  boss_colossus: [['golem_core', 1, 2, 2], ['gold', 1, 200, 200], ['aether_plate', 1, 1, 1]],
};

export function rollLoot(table, rand = Math.random) {
  const out = [];
  for (const [item, chance, min, max] of LOOT[table] || []) {
    if (rand() > chance) continue;
    const n = min + Math.floor(rand() * (max - min + 1));
    if (n > 0) out.push({ item, n });
  }
  return out;
}

// Shops: item ids sold by each merchant.
export const SHOPS = {
  lyra: ['potion_health', 'potion_mana', 'potion_stamina', 'honey_bread', 'ring_focus', 'leather_armor'],
  garrick: ['iron_sword', 'steel_longsword', 'leather_armor', 'chain_hauberk'],
  sera: ['potion_health', 'potion_mana', 'potion_stamina', 'elixir'],
  marin: ['roast_fish', 'potion_stamina', 'windstep_charm'],
};
