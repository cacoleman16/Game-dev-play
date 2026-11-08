// Hand-placed points of interest. The terrain generator flattens ground for
// them and lays roads between them; the map, compass, quests and fast travel
// all read from this list.

export const LOCATIONS = [
  { id: 'brightwater', name: 'Brightwater', type: 'village', x: 40, z: 430, r: 70, flatten: 62, plaza: 15, waystone: true,
    desc: 'A sleepy meadow village of red roofs and windmills, where your journey begins.' },
  { id: 'heartshard', name: 'The Heartshard', type: 'landmark', x: 0, z: -60, r: 45, waystone: false,
    desc: 'A colossal crystal hangs above the lake, cracked and humming with restless aether.' },
  { id: 'scholars', name: "Scholars' Camp", type: 'camp', x: 30, z: 150, r: 26, flatten: 24, waystone: true,
    desc: 'A ring of canvas tents where researchers study the Heartshard.' },
  { id: 'whisperwood', name: 'Whisperwood Ruins', type: 'ruins', x: -560, z: 60, r: 55, flatten: 46, plaza: 22, waystone: true,
    desc: 'The overgrown halls of an elder civilisation, deep in the western forest.' },
  { id: 'frostspire', name: 'Frostspire Shrine', type: 'shrine', x: -150, z: -700, r: 40, flatten: 34, plaza: 13, waystone: true,
    desc: 'A wind-scoured shrine high among the northern peaks.' },
  { id: 'sunscar', name: 'Sunscar Vault', type: 'dungeon', x: 610, z: -140, r: 40, flatten: 30, waystone: true,
    desc: 'A sealed vault cut into the red cliffs of the Sunscar badlands.' },
  { id: 'saltmere', name: 'Saltmere', type: 'village', x: 520, z: 560, r: 48, flatten: 42, fh: 3.2, plaza: 9, waystone: true,
    desc: 'A salt-crusted fishing hamlet on the eastern shore.' },
  { id: 'lighthouse', name: 'Gull Point Light', type: 'landmark', x: 660, z: 650, r: 18, flatten: 14, fh: 9, waystone: false,
    desc: 'An old lighthouse keeping watch over the Eastern Sea.' },
  { id: 'banditcamp', name: 'Redcloak Hideout', type: 'camp', x: -360, z: 430, r: 32, flatten: 28, waystone: false,
    desc: 'A camp of Redcloak bandits hidden at the edge of the woods.' },
  { id: 'watchtower', name: 'Old Watchtower', type: 'landmark', x: 318, z: 112, r: 16, flatten: 12, waystone: false,
    desc: 'A crumbling tower that once guarded the eastern road.' },
  { id: 'stonecircle', name: 'Circle of Echoes', type: 'landmark', x: -330, z: -280, r: 22, flatten: 18, waystone: false,
    desc: 'Standing stones that sing faintly when the wind passes through them.' },
  { id: 'windmill', name: 'Tobin\'s Farm', type: 'landmark', x: 250, z: 380, r: 30, flatten: 26, waystone: false,
    desc: 'Golden fields and a creaking windmill east of Brightwater.' },
];

export const LOC = Object.fromEntries(LOCATIONS.map((l) => [l.id, l]));

// Roads as polylines of [x, z]. Each is dirt-coloured and smoothed.
export const ROADS = [
  // Brightwater north to the scholars and the lake shore.
  [[40, 430], [30, 330], [10, 240], [30, 150], [10, 95]],
  // Brightwater east to the farm, then Saltmere.
  [[40, 430], [150, 410], [250, 380], [380, 450], [520, 560], [610, 620], [660, 650]],
  // East road past the watchtower to the badlands.
  [[30, 150], [180, 150], [330, 140], [470, -40], [610, -140]],
  // West road through the forest to the ruins.
  [[30, 330], [-160, 320], [-330, 200], [-470, 110], [-560, 60]],
  // Branch to the bandit camp.
  [[-160, 320], [-280, 400], [-360, 430]],
  // North-west trail to the stone circle and up the mountain to the shrine.
  [[-40, 120], [-200, -40], [-330, -280], [-290, -480], [-200, -600], [-150, -700]],
];

// A raised stone causeway from the camp to the island under the Heartshard.
export const CAUSEWAY = { x0: 10, z0: 120, x1: 0, z1: -32, width: 7, height: 1.6 };
