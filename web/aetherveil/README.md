# Aetherveil: Shards of the Hollow Sky

An open-world 3D action RPG that runs in the browser, on desktop and on phones.
The world, characters, creatures, music and sound effects are all generated in code.
There are no model, texture or audio files. The only dependency is three.js, vendored in `vendor/`.

The art direction borrows the bright, cel-shaded, crystal-and-sky look of classic JRPGs.
The structure is closer to an Elder Scrolls game: a free-roaming island, quests in any order,
skills that improve with use, loot, merchants, fast travel and a dungeon.

## Play it

- **Hosted:** this folder sits under `web/`, so the existing Vercel deploy serves it at
  `https://<your-deployment>/aetherveil/`.
- **Locally:** serve the `web/` folder with any static server and open `/aetherveil/`:

  ```sh
  cd web && python3 -m http.server 8000
  # then open http://localhost:8000/aetherveil/
  ```

  Opening `index.html` straight from disk (`file://`) won't work, because browsers block ES modules there.

- **On a phone:** open the URL in Chrome or Safari, rotate to landscape, and use
  "Add to Home Screen" for full-screen play. After the first visit the game works offline.

URL options: `?quality=low|medium|high|ultra` overrides the graphics preset, and `?play` skips the title screen.

## What's in the world

- **A 2 km island** with five regions: the Amber Meadows, Whisperwood forest, the snowy Frostspire Range,
  the red-mesa Sunscar Badlands, and Heartshard Lake with a giant floating crystal.
- **12 named places:** two villages, a scholars' camp, ruins, a mountain shrine, a vault dungeon,
  a bandit hideout, a lighthouse, a windmill farm, a watchtower with a spiral stair, standing stones,
  and floating sky islands that hide treasure.
- **Day and night cycle** with stars, moon, drifting clouds, a glowing rift in the sky,
  and weather (rain and storms in the lowlands, snow in the peaks).
- **13 NPCs** with branching dialogue, shops, painted portraits and their own speaking voices.
- **10 creature types and 4 bosses**, placed by region: gloops, dusk wolves, thornback boars,
  Redcloak bandits and archers, ruin golems, bramble sentinels, gale skyrays, ash revenants,
  and night-only aether wisps.

## Story and quests

- **Main quest, "The Shattered Heart":** speak with Elder Maren, meet the scholar Ysolde,
  have a vision at the Heartshard pedestal, then recover three shards in any order:
  - Shard of Verdance: the Warden of Whisperwood (golem boss) in the ruins.
  - Shard of Rime: Rimefang (giant frost wolf) at Frostspire. Teaches Frost Lance.
  - Shard of Embers: the Cinder Colossus at the bottom of the Sunscar Vault dungeon. Teaches Skybreaker.

  Return all three to face the Hollow Herald.
- **Six side quests:** Wolves at the Door, A Glow in the Dark, The Lost Locket, Redcloak Bounty,
  A Letter for Old Wick, and Heart of Stone.

## Systems

- **Combat:** 3-hit sword combos with hit-stop and a sword trail. Hold attack to charge a heavy
  strike, attack while sprinting for a spin slash, and attack in mid-air to plunge.
  Blocking just before a hit lands is a **parry**, which staggers the attacker; a dodge timed
  the same way is a **perfect dodge** that slows time for a moment.
  Enemies have a **break gauge**: fill it with heavy hits and parries and they collapse, open to
  bonus damage. Landing hits fills the **Aether Surge** meter, an area ultimate.
  At most two enemies attack at once while the rest circle, and boss attacks are telegraphed by
  red ground markers. Four spells, elemental weaknesses (shown as "WEAK" numbers) and lock-on.
- **Movement:** sprint, jump, glide on aether wings, swim, and crouch to sneak.
  Stamina limits sprinting and gliding, and long falls hurt.
- **Progression:** XP and levels (each level grants a +Health, +Mana or +Stamina boon).
  Blade, Sorcery, Guard and Athletics level up as you use them.
- **Items:** weapons and armour that change how you look, rings, potions, herbs to gather,
  chests, and four merchants.
- **Saves and travel:** waystones heal you, save the game, and fast travel between each other.
  You can also fast travel from the map. There are three save slots plus an autosave.
- **Character creator:** name, build, hair style and colour, skin, eyes, tunic and scarf.

## Look and sound

- **Characters** are skinned meshes swept from tube profiles, with strand hair, painted faces that
  blink, and spring-driven capes, hair and skirts. They're lit with a toon ramp, rim light and
  ink outlines.
- **Post-processing** (Medium and up): bloom, colour grading with split toning, vignette, film
  grain and FXAA. Fog warms toward the sun.
- **Music** is composed per region and situation (village, wilds, night, dungeon, boss, victory)
  and played on synthesized instruments: plucked strings, FM bells, flute, string and choir pads.
  Tracks crossfade as you move.
- **Voices:** every NPC has a voice profile (pitch, rate, accent) and speaks their dialogue
  aloud through the browser's speech engine. If no speech voices are available, a
  syllable-babble voice is used instead. Fighters grunt and shout, and creatures have their own
  calls. Voice volume and mode are in Settings.
- **3D sound:** combat and creature sounds are positioned in space and run through a compressor.

## Controls

| Action | Keyboard / mouse | Gamepad | Touch |
| --- | --- | --- | --- |
| Move / look | WASD / mouse (click to capture) | Sticks | Left stick / drag right side |
| Sprint | Shift | RT | » toggle, or push the stick to its edge |
| Jump, then glide in mid-air | Space | A | ⤒ |
| Attack (combo) | Left click or K | X | ⚔ |
| Heavy attack | Hold attack | Hold X | Hold ⚔ |
| Aether Surge (when full) | G | L3 | Surge button |
| Block / parry | Right click or X (tap just before a hit to parry) | LB | ⛨ (hold) |
| Dodge | Q | B | ⟲ |
| Spells | 1–4, F casts the selected one | RB, D-pad picks | ✦, ⇆ cycles |
| Interact / talk | E | Y | E |
| Lock on | Tab or middle click | R3 | ◎ |
| Quick potion | Z | LT | ♥ |
| Menu / inventory / journal / map | Esc / I / J / M | Start / Back | ☰ / ⌖ |
| First person, zoom, crouch | V, mouse wheel, C | | |

## Performance

The game detects phones and defaults them to **Medium**; desktops get **High**.
You can change this in Settings.

- Terrain is drawn in 256 m chunks with four levels of detail and skirts to hide seams.
- Nearby trees are batched per 128 m cell; distant trees go into one instanced draw per species.
- Grass is a single instanced draw whose shader reads terrain height and ground colour from textures.
- Each character is one skinned mesh, plus an outline that shares the same skeleton.

In headless testing, a typical Medium frame was 40–150 draw calls and 0.25–0.9 M triangles,
with 1–4 ms of game logic per frame. Low turns off post-processing.

## Testing

The game was play-tested by a bot that drives it only through the input layer (move stick, camera,
buttons and dialogue choices). It plays through the whole main quest and several side quests at a
fixed 60 Hz step. Each run reports deaths, kills, parries, breaks, stuck spots and errors, which is
how the vault pathing bug, a road blocked by a house and the dungeon travel bug were found.

## Code map

```
index.html, style.css        page, HUD and menu styling
src/main.js, src/game.js     boot, the Game object and frame loop
src/core/                    noise + RNG, input (keyboard/mouse/gamepad/touch), settings presets
src/world/worldgen.js        heightfield, biomes, roads, flattened town sites (pure data, also runs in Node)
src/world/terrain.js         chunked LOD terrain + painterly material
src/world/sky.js, water.js, grass.js, vegetation.js, structures.js, dungeon.js (with grid pathing)
src/entities/body.js         tube sweeps, hair strands and painted faces for skinned bodies
src/entities/character.js    skeletons, toon + outline materials, procedural animation with springs
src/entities/player.js, camera.js, enemies.js, creatures.js, npcs.js
src/systems/                 combat, spawning, quests, dialogue, story events, inventory, items, saves
src/fx/                      particles and telegraphs, post-processing, sword trail
src/audio/audio.js           instruments, composed themes, 3D sound, NPC voices
src/ui/ui.js, icons.js       HUD, compass, menus, map, shops, portraits, title, creator, touch controls
```

The world comes from a fixed seed, so every player sees the same island.
`src/world/locations.js` lists the hand-placed sites and roads that the generator builds around.
