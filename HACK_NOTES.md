# FireRed QoL Hack

A personal, single-player FireRed hack built on
[pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion)
**1.17.2**, using its FireRed/LeafGreen build (`make firered`). The expansion
already provides the modern battle engine. This hack switches on the parts
you asked for, adds the missing QoL pieces, and ships a browser player.

Every hack change is tagged `[QoL]` in the source: `git grep -n "\[QoL\]"`.
The earlier pokefirered-based version (7 features) is kept under
[`reference/pokefirered-qol/`](reference/pokefirered-qol/) as git patches.

> **Status:** `make firered` builds cleanly, both locally and in GitHub Actions
> ("Build ROM" workflow). Partly play-tested in the web player (headless
> Chromium): the intro, new-game items, the NES settings menu, randomized
> starters and the first rival battle, following Pokémon, the IV/EV summary,
> the move relearner, and saving, reloading and importing saves. Gyms and the
> later game haven't been played yet. Start a **new save**: the starting items
> and flags are set up in `NewGameInitData`. Hoenn and the Battle Frontier
> (merged regions) work with old saves too.

## Build

```sh
make firered -j$(nproc)      # → pokefirered.gba (non-matching, by design)
```
Toolchain: see `INSTALL.md` / `docs/install/` (arm-none-eabi gcc + libpng).

**Getting the ROM:** while the repo is **public**, CI only checks that the ROM
builds and never uploads it; build it yourself with `make firered`. If you make
the repo **private** (Settings → General → Danger Zone → Change visibility),
every CI run attaches the ROM: Actions → "Build ROM" → the run → Artifacts →
`FireRed-QoL-<commit>` (a zip with `pokefirered.gba`, kept 30 days).

---

## Your wishlist, item by item

| Wish | Status | How |
| --- | --- | --- |
| Physical/Special split | ✅ built in | expansion battle engine |
| Fairy type | ✅ built in | expansion |
| Following Pokémon | ✅ on | `OW_FOLLOWERS_ENABLED`, sprites for every species; `setflag FLAG_QOL_FOLLOWERS_OFF` hides them |
| Modern Exp. Share | ✅ on | Gen 6+ key item given at new game, **ON** by default; participants get full EXP (`B_SPLIT_EXP`) |
| Reusable TMs | ✅ on | `I_REUSABLE_TMS` |
| Smart Repel prompt | ✅ on | YES/NO "use another?" with the last Repel remembered (`VAR_LAST_REPEL_LURE_USED`) |
| Run indoors, from the start | ✅ on | `OW_RUNNING_INDOORS` (Gen 4+) + Running Shoes at new game |
| Auto-run | ✅ new | **L** toggles auto-run; hold **B** to walk while it's on |
| Turbo speed | ✅ new | Turbo ×2–×8 button in the web player; faster battle text; fast HP/EXP bars |
| Skip through text | ✅ new | **Hold B** in any dialogue (after pressing it once in that text box) and the pages turn on their own, in the field and in battle; it stops at YES/NO and other choices. **OPTION → TEXT SPEED** is **MID / FAST / MAX**, and every setting is quicker than vanilla: MID prints like vanilla FAST, FAST prints three letters a frame, and MAX prints each page at once and scrolls a line in one step. At MAX, battle text pauses are half as long, and holding B skips them. New games start on MAX |
| Faster battles | ✅ new | No opening slide-in at the start of battles (`B_FAST_INTRO_NO_SLIDE`) |
| Fast surf | ✅ new | Hold **B** (or turn on auto-run) while surfing to go at bike speed |
| Less waiting | ✅ new | The location pop-up only shows when you enter a new area (`OW_HIDE_REPEAT_MAP_POPUP`); Pokémon Centers skip the Union Room check |
| Fly between regions | ✅ new | On the **Fly** map, **L/R** switch between Kanto, the Sevii Islands and Hoenn (each once you've been there), so you can fly straight from Kanto to a Hoenn town and back |
| HMs without HM slaves | ✅ new | Own the HM + have the badge, and any party member that *can learn* it can use it (party menu, Surf prompt, scripts). HM moves can also be forgotten. |
| Solo evolutions | ✅ | Kadabra/Machoke/Graveler/Haunter (and later-gen plain trades) also evolve at **level 40**; Linking Cord works; King's Rock, Metal Coat, Up-Grade etc. are **used from the bag** |
| Nature Mints | ✅ | sold at Celadon Dept. Store **5F** (vitamin clerk) |
| EV/IV judge | ✅ | Summary screen, skills page: **A** cycles Stats → IVs (exact numbers) → EVs |
| Move relearner | ✅ | Summary screen, moves page: **START** relearns level-up, egg, TM and tutor moves and pre-evolution moves |
| Ability Capsule / Patch | ✅ | Celadon 5F |
| EV wipe | ✅ | EV-lowering Berries at Celadon 5F; vitamins have no Gen 3 cap |
| IV maxing (Hyper Training) | ✅ | The gentleman on Celadon **5F** Hyper Trains: Bottle Cap = one stat, Gold Bottle Cap = all six. Caps are sold on 5F. |
| DexNav | ✅ on | START menu from a new game; **R** searches the registered species |
| Mega / Z-Moves / Dynamax / Tera | ✅ | Mega Ring, Z-Power Ring, Dynamax Band and Tera Orb are given at new game; Dynamax is enabled. **Celadon 4F** sells all 92 Mega Stones (¥10,000) and 35 Z-Crystals (¥8,000), plus every evolution item. |
| Smart trainer AI | ✅ | All 80 bosses (gym leaders, Elite Four, champion and rival battles, Cooltrainers, Rangers) use **Smart Trainer** AI; the other 543 trainers use Basic Trainer AI. Teams and held items are still vanilla. |
| Gen 1–9 roster | ✅ | Every Kanto/Sevii table gained up to 5 Gen 4–9 Pokémon in rare duplicate slots (forest bugs, Tower ghosts, Power Plant electrics, Seafoam ice, baby pseudo-legendaries in Cerulean Cave...). Every vanilla species is still there. Regenerate with `dev_scripts/qol_add_modern_encounters.py`. |
| Level caps | ✅ optional | Kanto caps (14/21/24/29/43/43/47/50, then 63 before the Champion). Off by default. Rare Candies and the Day Care respect them. |
| Nuzlocke | ✅ optional | Starts when Oak gives you Poké Balls. Only the first random wild Pokémon in each area can be caught (species already caught don't count; shinies always can be). DexNav finds count; scripted encounters and the Safari Zone are exempt. Fainted = dead: no Pokémon Center healing, revives or Revival Blessing. With the whole party dead, wild Pokémon and trainers leave you alone so you can reach a PC. |
| Randomizer | ✅ optional | Each species becomes another with a similar base stat total (legendaries swap among themselves). Fixed per save file (seeded by your Trainer ID). Changes wild Pokémon, DexNav, trainers' teams, scripted encounters, Blue's starter and Prof. Birch's starters (Beldum, your partner, stays a Beldum). Gift Pokémon and trades stay vanilla. Turn it on before picking a starter. |
| Difficulty tiers | ✅ optional | **HARD MODE**: competitive 4–6 Pokémon teams with items, natures, EVs and Megas for the 8 gym leaders, the Elite Four and the Champion. Aces keep their vanilla levels. Other trainers are unchanged. |
| Challenge settings | ✅ | The **NES in your bedroom** is a menu: NUZLOCKE, RANDOMIZER, HARD MODE, LEVEL CAPS, FOLLOWERS, VISIBLE WILD (each shows ON/OFF). The bedroom notice lists the L/R controls |
| Side quests | ✅ | **Field Research** (a researcher in Oak's Lab): 8 tasks about Pokémon from faraway regions, each with a reward, ending in a post-game hunt for **Galarian Articuno, Zapdos and Moltres**. See *Side quests* below |
| Battle Frontier | ✅ | Emerald's full **Battle Frontier** (all 7 facilities, Frontier Pass, BP). Sail there from **Vermilion City's pier** once you're the Kanto (or Hoenn) Champion. See *Kanto + Hoenn* below |
| Every region in one map | ✅ Kanto + Hoenn | All of **Hoenn** is in the game with its own story, trainers, wild Pokémon and badges. The **S.S. TIDAL** sails from Vermilion City's pier once you have the **S.S. TICKET**; your first trip starts Emerald's arrival in Littleroot Town (Prof. Birch, a Hoenn starter). See *Kanto + Hoenn* below |
| Own story: STARFALL | ✅ new | A meteor shower links Kanto and Hoenn. **Beldum** is your partner (it falls out of Steven's meteorite in Oak's Lab), Team **Nova** pulls the strings behind Rocket, Magma and Aqua, and the finale is on **Birth Island** against their leader Vega, then Deoxys. Every starter is still catchable. **Needs a new save.** See *STARFALL* below and [docs/STARFALL.md](docs/STARFALL.md) |
| Visible wild Pokémon | ✅ new | **STARFALL ISLE** (S.S. TIDAL from Vermilion): wild Pokémon roam in the open and you battle the ones you walk into, no random battles. **VISIBLE WILD: ON** on the NES does the same everywhere. See *Visible wild Pokémon* below |
| A third region: SEIKA | ✅ new | An original, post-game region with an old-capital feel (shrines, lanterns, kilns, silk), mostly Gen 2 wild Pokémon, 8 Gyms, its own League and the end of the Starfall story with Jirachi. The S.S. TIDAL sails there after the Birth Island finale. Fly works there too. See *SEIKA* below and [docs/SEIKA.md](docs/SEIKA.md) |
| Japanese + English | ✅ | **OPTION → LANGUAGE** (top row): ENGLISH / にほんご, switchable any time. Kana-only Gen 3 style. Story, NPC, trainer, menu and battle text translated; Pokémon, move, item and ability names and Pokédex entries are the official Japanese ones. See *Japanese mode* below |

## Where the settings live

| File | What changed |
| --- | --- |
| `include/config/overworld.h` | followers on, `QOL_AUTO_RUN_FLAG`, `QOL_FIELD_MOVES_WITHOUT_HMS` |
| `include/config/item.h` | reusable TMs, Exp. Share (FRLG: flag + Gen 6 item), Repel prompt, evo items from bag |
| `include/config/battle.h` | Dynamax / Tera Orb flags, battle text speed, B moves cursor to RUN |
| `include/config/dexnav.h` | DexNav on for FRLG + its flags/vars |
| `include/config/pokemon.h` | `QOL_TRADE_EVO_LEVEL` (40), HM moves forgettable |
| `src/new_game.c` | `SetupQoLNewGame()`: Running Shoes, Exp. Share ON + item, DexNav, gimmick key items |
| `src/field_control_avatar.c`, `src/field_player_avatar.c` | auto-run toggle |
| `src/party_menu.c`, `src/scrcmd.c`, `src/field_player_avatar.c` | `MonCanUseFieldMove()` (HMs without HM slaves) |
| `src/data/pokemon/species_info/gen_{1,5,6}_families.h` | level-40 entries next to plain trade evolutions |
| `src/data/trainers_frlg.party` | trainer AI upgrade |
| `data/maps/CeladonCity_DepartmentStore_5F_Frlg/scripts.inc` | Bottle Caps, mints, Ability Capsule/Patch, EV Berries; Hyper Training gentleman |
| `data/maps/CeladonCity_DepartmentStore_4F_Frlg/scripts.inc` | menu shop: gifts, evolution items, Mega Stones, Z-Crystals |
| `src/data/items.h` | Mega Stone / Z-Crystal prices (`QOL_MEGA_STONE_PRICE`, `QOL_Z_CRYSTAL_PRICE` in `config/item.h`) |
| `src/caps.c`, `include/config/caps.h` | optional Kanto level caps |
| `src/qol_nuzlocke.c`, `include/qol_nuzlocke.h` | Nuzlocke rules; hooks in `battle_setup.c`, `wild_encounter.c`, `dexnav.c`, `script_pokemon_util.c` (healing), `pokemon.c`/`item_use.c` (revives), `battle_script_commands.c` (Revival Blessing), `trainer_see.c` |
| `src/qol_randomizer.c`, `include/qol_randomizer.h` | randomizer; hooks in `wild_encounter.c`, `dexnav.c`, `trainer_util.c`, `script_pokemon_util.c` (scripted battles) and Oak's lab script |
| `src/data/trainers_frlg.party` (end of file), `src/difficulty.c` | HARD MODE teams (`Difficulty: Hard`) |
| `include/config/summary_screen.h` | IV/EV display, move relearners |
| `data/maps/PalletTown_PlayersHouse_2F_Frlg/scripts.inc` | NES challenge-settings menu, updated notice |
| `data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc` | randomized starters |
| `src/qol_quests.c`, `data/scripts/qol_quests.inc` | side quests (map objects added in Oak's Lab, Seafoam B4F, Power Plant, Mt. Ember summit) |
| `src/qol_language.c`, `include/qol_language.h`, `src/option_menu.c` | Japanese mode; hooks in `text.c`, `string_util.c`, `battle_message.c`, `pokemon.c`, `item.c`, `move.h` |
| `text/ja/` | Japanese translations (JSON) and the translation guide `GLOSSARY.md` |
| `src/data/text/qol_ja_names.h`, `qol_ja_descriptions.h` | generated Japanese names and descriptions |
| `src/data/wild_encounters.json` | Gen 4–9 additions (generated by `dev_scripts/qol_add_modern_encounters.py`) |
| `tools/mapjson/mapjson.cpp`, `src/data/tilesets/*.h`, `tools/wild_encounters/` | Hoenn maps, layouts, tilesets and wild Pokémon in the FireRed build |
| `include/constants/flags_frlg.h`, `vars.h`, `opponents.h` | Hoenn's own flags, vars and trainer IDs (rewritten by `dev_scripts/qol_hoenn_constants.py`) |
| `src/qol_hoenn.c`, `include/qol_hoenn.h`, `src/event_data.c` | Hoenn flag/var storage (SaveBlock3), first-trip setup, badges from either region |
| `data/scripts/qol_regions.inc` | S.S. TIDAL sailor (Vermilion), Littleroot shuttle driver, "sail to Kanto?" at Hoenn's ferry desks |
| `src/region_map.c` | Fly map: L/R switch regions |
| `data/scripts/qol_starfall.inc`, `docs/STARFALL.md` | the STARFALL story; hooks in Oak's Lab, Pallet house, Mt. Moon B2F, Silph Co. 11F, Cinnabar Lab, Meteor Falls, Mt. Chimney, the Space Center, Steven's house, Birth Island |
| `src/data/trainers_frlg.party` (end), `include/constants/opponents_frlg.h`, `trainers.h` | Team Nova's 11 trainers and 3 trainer classes |
| `dev_scripts/qol_add_starter_encounters.py` | every starter in the wild / Safari Zones |
| `dev_scripts/seika/`, `docs/SEIKA.md` | the SEIKA region: `build.py` generates its 80 maps, trainers, flags, wild Pokémon and Fly data |
| `dev_scripts/qol_mapgen.py`, `dev_scripts/qol_maptools.py` | map painter (shapes and stamps into FireRed maps) and renderer |
| `src/text.c`, `src/scrcmd.c`, `src/option_menu.c`, `include/config/text.h` | hold B to fast-forward text; MID/FAST/MAX text speed and how fast each prints |
| `src/battle_script_commands.c` (`QoL_BattlePause`) | battle text pauses follow the text speed; hold B to skip them |
| `src/field_player_avatar.c` | fast surf |
| `src/qol_hoenn.c` (`QoL_VisibleWildActive`), `src/wild_encounter_ow.c`, `src/field_control_avatar.c` | visible wild Pokémon: where they're on, and no random battles there |

## New flags and vars (FireRed/LeafGreen)

Defined at the end of `include/constants/flags.h` / `vars.h`. They are `0`
(feature off) in Emerald builds.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_QOL_EXP_SHARE` | FRLG `0x8E0` | set = party Exp. Share ON |
| `FLAG_QOL_FOLLOWERS_OFF` | `0x8E1` | set = hide following Pokémon |
| `FLAG_QOL_TERA_ORB_CHARGED` | `0x8E2` | Tera Orb charge |
| `FLAG_QOL_DYNAMAX_BATTLES` | `0x8E3` | set = Dynamax allowed |
| `FLAG_QOL_DEXNAV_GET` / `_SEARCHING` / `_DETECTOR` | `0x8E4`–`0x8E6` | DexNav |
| `FLAG_QOL_AUTO_RUN` | `0x8E7` | auto-run state |
| `FLAG_QOL_LEVEL_CAPS` | `0x8E8` | set = Kanto level caps ON |
| `FLAG_QOL_NUZLOCKE` | `0x8E9` | set = Nuzlocke ON |
| `FLAG_QOL_NO_CATCHING` | `0x8EA` | set during a wild battle Nuzlocke won't let you catch |
| `FLAG_QOL_RANDOMIZER` | `0x8EB` | set = randomizer ON |
| `QOL_EXTRA_FLAG(n)` | `0x12C0`–`0x13BF` | more hack flags, stored after the Hoenn block in SaveBlock3 |
| `FLAG_QOL_NUZLOCKE_AREAS_START` | `QOL_EXTRA_FLAG(0x00)`+ | one "encounter used" flag per Kanto/Sevii map section (moved from `0x35D`, which overlapped FRLG's daily-reset flags) |
| `FLAG_QOL_STARFALL_*` | `QOL_EXTRA_FLAG(0x80)`+ | Starfall story progress (prologue, each chapter, Steven's letter, Vega, ending) |
| `FLAG_QOL_HIDE_NOVA_*`, `FLAG_QOL_HIDE_STARFALL_LAB`, `FLAG_QOL_HIDE_PALLET_MOM` | `QOL_EXTRA_FLAG(0x90)`+ | Starfall cutscene objects; Mom after the move |
| `QOL_HOENN_FLAG(n)` | `0x900`–`0x12BF` | Hoenn's flags: Emerald flag *n*, stored in SaveBlock3 |
| `QOL_HOENN_VAR(n)` | `0x5000`–`0x50FF` | Hoenn's vars: Emerald var *n*, stored in SaveBlock3 |
| `HOENN_TRAINER(n)` | `1024`+ | Emerald trainer *n* (after the Kanto trainers in `gTrainers`) |
| `FLAG_HOENN_BADGE01_GET`–`08`, `FLAG_HOENN_GAME_CLEAR` | Hoenn block | Hoenn's badges and Hall of Fame |
| `FLAG_QOL_HOENN_SETUP` | Hoenn block | Hoenn's map flags have been set up (first trip) |
| `FLAG_QOL_NUZLOCKE_HOENN_AREAS_START` | Hoenn block | Nuzlocke "encounter used" flags for Hoenn map sections |
| `VAR_KANTO_STARTER` | `0x4023` | your Kanto starter (the slot Kanto's scripts always used) |
| `VAR_QOL_LAST_REPEL` | `0x40A1` | last Repel used |
| `VAR_QOL_DEXNAV_SPECIES` / `_STEPS` | `0x40F7` / `0x40F8` | DexNav |
| `VAR_QOL_DIFFICULTY` | `0x40F9` | trainer difficulty (1 Normal, 2 Hard) |
| `FLAG_QOL_HIDE_GALAR_*` | `0x8EC`–`0x8EE` | Galarian birds hidden until the research reveals them |
| `FLAG_QOL_CAUGHT_GALAR_*` | `0x8EF`–`0x8F1` | Galarian birds caught |
| `FLAG_QOL_VISIBLE_WILD` | `0x8F2` | set = visible wild Pokémon everywhere (NES option) |
| `FLAG_QOL_SEIKA_*` | `QOL_EXTRA_FLAG(0xA0)`+ | Seika badges, story, League run, Fly (generated, see `include/constants/flags.h`) |
| `FLAG_QOL_SKYRIFT_*` | after Seika's | Skyrift wish, companion, rifts, endings, inn (generated) |
| `FLAG_QOL_SHINY_X8` / `_X32` / `_FREE_PAINT` | `QOL_EXTRA_FLAG(0x8B)`–`(0x8D)` | SHINY ODDS setting; the Shiny Painter's free job used |
| `VAR_QOL_FIELD_RESEARCH` | `0x40FA` | current Field Research task |
| `optionsLanguage` (SaveBlock2 option bit) | – | 0 English, 1 Japanese |

The vars are unused in **both** the Emerald and FRLG var maps, because the
expansion's shared engine code still uses Emerald var numbers in FRLG builds.

---

## Side quests

**Field Research.** Once you have the Pokédex, talk to the researcher on the
left side of Oak's Lab. She gives one task at a time; talk to her again when
it's done:

| # | Task | Reward |
| --- | --- | --- |
| 1 | Show her a Pokémon from a faraway region (No. 387 or later) | Ability Capsule |
| 2 | Catch 10 kinds of faraway Pokémon | Lucky Egg |
| 3 | Win 50 trainer battles | 3 Rare Candies |
| 4 | Hatch an Egg | Oval Charm |
| 5 | Catch 25 kinds of faraway Pokémon | Gold Bottle Cap |
| 6 | Catch 100 kinds of Kanto Pokémon (No. 1–151) | Shiny Charm |
| 7 | Become Champion; she reveals the Galarian birds | – |
| 8 | Catch Galarian Articuno, Zapdos and Moltres (Lv 70) | Master Ball + Ability Patch |

The Galarian birds wait near the Kanto birds' homes: **Seafoam Islands B4F**,
the **Power Plant** and the **Mt. Ember summit**. If one faints or you run,
it stays there, so you can try again. A full Bag keeps the task open until
you make room.

## Shiny hunting

- **Better odds:** the base rate is 1 in 4096, as in newer games (vanilla
  FireRed is 1 in 8192).
- **SHINY ODDS** at the NES in your bedroom: NORMAL, **x8** (about 1 in 512)
  or **x32** (about 1 in 128). Pick it again to cycle. The menu now reopens
  on the last setting you picked.
- These stack with the other boosts:
  - the **Shiny Charm**;
  - DexNav search levels;
  - Lures;
  - **fishing chains**: hooking the same Pokémon again and again.
- **The Shiny Painter** is in the Pokémon Centers in **Viridian City** and
  **Lanternport**. She paints any party Pokémon shiny for ¥10,000.
  - **The first one is free**, so your starter can shine right away.
  - She washes the shine off again for free.
  - She sells the **Shiny Charm** for ¥30,000 (you can also get it from the
    Wishing Star).
- **Seeing them in the wild:**
  - With VISIBLE WILD on (and always on Starfall Isle), a shiny wild
    Pokémon walks around in its shiny colours. It sparkles and plays the
    shiny sound when it appears, and it doesn't despawn while you're on
    its map.
  - In normal grass battles a shiny sparkles as it comes out, as usual.
- **Tested in the web player:**
  - painting Mewtwo with the free first job (the summary and the following
    Pokémon both show it shiny);
  - the SHINY ODDS menu entry;
  - a shiny wild Pidgey in its shiny colours, both walking in the grass and
    in battle.

## STARFALL (the hack's own story)

Start a **new game**. The plot, the cast and every chapter are in
[docs/STARFALL.md](docs/STARFALL.md). In short:

- **Oak's Lab:** Steven's meteorite cracks open during the Starfall and a
  **Beldum** (Lv 5: Take Down, Metal Claw, Confusion, Iron Defense) chooses
  you. Bulbasaur and Squirtle run off; Blue takes Charmander.
- **Your family:** Dad is Norman, Petalburg's Gym Leader. Mom moves to
  Littleroot with you on your first S.S. TIDAL trip (in Pallet she leaves a
  note on the TV).
- **Team Nova** (Altair, Deneb, leader Vega) shows up at Mt. Moon, Silph Co.,
  Cinnabar Lab, Meteor Falls, Mt. Chimney and the Mossdeep Space Center.
- **Steven's house** after Hoenn's Hall of Fame: his letter, and a
  **Metagrossite** (instead of Emerald's Beldum).
- **Finale:** Champion of both regions + the letter → the S.S. TIDAL in
  Vermilion sails to **Birth Island**: Team Nova, **Vega** (Lv 72–75, Ultra
  Beast ace), then the triangle puzzle and **Deoxys (Lv 70)**, then the ending.
- **Every starter is catchable:** Bulbasaur (Viridian Forest), Charmander
  (Route 4), Squirtle (Vermilion fishing), Treecko/Torchic/Mudkip (Petalburg
  Woods, Route 117, Route 102 fishing), Gen 2 and 4 starters in Kanto's Safari
  Zone, Gen 5–9 starters in Hoenn's Safari Zone (randomizer off).

**Tested in the web player:** the Oak's Lab scene, Mt. Moon (Altair's battle and
exit), the Cinnabar Lab raid (both battles), the Birth Island route, Team Nova
on the island, Vega's defeat scene, and the ending. Silph Co., Meteor Falls,
Mt. Chimney, the Space Center and Steven's house use the same pattern but
haven't been played. The story's text is in Japanese too.

## Visible wild Pokémon

Wild Pokémon can walk around the map as sprites (the expansion's overworld
wild encounters): walk into one to battle it, or walk around it. While they're
on, random battles in grass, caves and water are off.

- **STARFALL ISLE** is Emerald's Faraway Island, renamed. The Vermilion
  S.S. TIDAL sailor lists it once you've been to Hoenn. Meteors from the
  Starfall fell here, and its Pokémon always roam in the open: Clefairy,
  Eevee, Elgyem, Minior, Bronzor, Ralts, Riolu, Togepi, Larvitar, Gible,
  Dreepy (Lv 25–32) and a rare Chimecho. Emerald's Mew still hides in
  the grass. The island's sailor sails back to Vermilion or Lilycove.
- **VISIBLE WILD: ON** (bedroom NES) does it on every route, cave and lake.
  Fishing, Rock Smash and the DexNav work as before.

In tall grass only the top of a Pokémon shows, as in the main games.

**Tested in the web player:** spawning on Starfall Isle and on Route 1 (with
the option on), walking into a wild Pokémon starts the battle, winning returns
to the field, no random battles while walking through grass, the NES toggle,
and the ferry there and back.

## SEIKA (a third region)

After the Birth Island finale, the Vermilion S.S. TIDAL sailor adds **SEIKA**,
an original region where the first Starfall fell a thousand years ago. It is
post-game: Gyms run from Lv 50 to 68, the League from 70 to 78.

- **Route:** Lanternport → Inkbrush → Mossgrove Woods → Mossgrove → Kiln City
  → Tide Road → Tidewatch → Hollow Path → Silkhollow → Weaver's Road →
  Frostmere → Frost Road → Duskpeak → Skyreach Pass → Starwatch Plateau.
- **Gyms:** Wick (Fire), Sumi (Psychic), Kaya (Bug), Tobin (Ground), Nagi
  (Water), Ayame (Fairy), Rime (Ice), Tomoe (Ghost).
- **League:** Tetsu (Steel), Kuroe (Dark), Sora (Flying), Ryoko (Dragon),
  Champion Seiji.
- **Finale:** the Champion may climb to the Sky Shrine above the clouds,
  where Altair and Deneb ring the Starglass Bell. Beat them and the Wishing
  Star, **Jirachi (Lv 70)**, comes down.
- **Fly:** each Seika town unlocks when you visit it, and L/R on the Fly map
  includes Seika's own map.
- The full plan is in [docs/SEIKA.md](docs/SEIKA.md). The maps are generated by
  `dev_scripts/seika/build.py`, which paints them from shapes and buildings
  taken from Kanto's tilesets.

**Tested in the web player:**
- Lanternport (pier arrival, Pokémon Center, the Fire Gym with all three
  battles) and the walk from Lantern Road into Inkbrush.
- The Fly map's Seika page, and switching regions with L/R.
- Story scenes: Deneb's workshop scene, Altair on Hollow Path, the Frostmere
  grunts, the Skyreach Pass badge gate, and the plateau monk and cave.
- The Seika League: the first two Elite Four rooms, the Dragon room's door,
  and the Champion battle and victory scene. Blacking out sends you back to
  the League lobby.
- The Sky Shrine finale: Deneb, Altair, the bell, and Jirachi's battle.
- Gyms 2–8, most houses and the remaining roads were checked on rendered
  maps but not played through.

## SKYRIFT (the last chapter: companions and choices)

After you meet the Wishing Star, it grants **one wish** (strength, a friend,
fortune, or peace for Nova), and the sky over the eastern sea tears open.

- **Choose a companion** on Lanternport's pier: Altair, Deneb or Mira
  (Seiji's granddaughter). They **follow you and fight beside you**. Wild
  battles become 2-on-2, and every trainer battle becomes a multi battle
  with them. Talk to them for lines that change as the story goes on.
- **The Skyrift Isles**: Riftwake Harbor, Shardfall Beach, Meteor Basin,
  Observatory Cliffs and Vega's observatory. Lv 66–74.
- **Rift raids**: five boosted Ultra Beasts at Lv 75 (Beast Balls are sold
  in Riftwake), then a sixth rift with Necrozma at Lv 80.
- **The climax**: a 2-vs-2 against Vega and Sirius with your companion, then
  one choice. **Take her hand** and you get Poipole and Vega's rematch.
  **Let go** and you get Cosmog, and Sirius waits for revenge.
- Afterwards your companion can keep travelling with you anywhere, and
  talks about where you are (Kanto, Hoenn, Seika or the isles). The others
  wait in the Riftwake Inn, where you can swap.
- Full design: [docs/SKYRIFT.md](docs/SKYRIFT.md).

**Tested in the web player:**
- The wish menu, the Jirachi battle and the sky tearing open.
- The pier scene and companion choice.
- Your companion following you and talking.
- The ferries both ways, and SKYRIFT ISLES on Vermilion's ferry menu.
- Dr. Halley.
- A rift battle with its boost, both running away and catching.
- The Sirius gate as a 2-vs-1 with Altair.
- The rooftop 2-vs-2 with Deneb and both endings.
- Swapping companions in the inn.
- The Necrozma rift appearing.
- The observatory's trainer pair as a 2-vs-2 with Altair.
- Companion lines in Kanto and Seika.

**Not played through:** the other island trainers (the same battle setup as
the observatory pair) and the observatory's 2F. They were checked on the
rendered maps only.

## Kanto + Hoenn (merged regions) and the Battle Frontier

The FireRed build now also contains **every Hoenn map** from the expansion's
Emerald data: towns, routes, caves, the Safari Zone, the Elite Four and the
**Battle Frontier**, with their scripts, trainers and wild Pokémon.

**Getting there.** A sailor on **Vermilion City's pier** (east of the S.S.
Anne's dock) runs the **S.S. TIDAL**. Show the **S.S. TICKET** to board.

- **First trip:** the ship docks in Hoenn and a moving truck takes you to
  **Littleroot Town**, where Emerald's story starts: your new room, the clock,
  the neighbours, and **Prof. Birch** on Route 101, who lets you pick
  **Treecko, Torchic or Mudkip** (randomized if the randomizer is on).
- **Later trips:** Littleroot Town, plus **Slateport** and **Lilycove** once
  you've been there.
- **Battle Frontier:** on the menu once you're the **Kanto Champion** (or
  Hoenn's). The first visit gives you the **Frontier Pass**.
- **Flying:** once you've visited Hoenn towns, press **L/R** on the Fly map to
  switch regions and fly there directly (HMs work with either region's badges).
- **Back to Kanto:** the S.S. TIDAL desks in **Slateport**, **Lilycove** and
  the **Battle Frontier** ask "Would you like to go to VERMILION CITY?" first,
  and a **shuttle driver at the north end of Littleroot Town** drives you to
  the port.

**How the two stories stay apart.** Hoenn's story flags, vars and trainers
have their own storage (in SaveBlock3), so Hoenn never changes Kanto progress
or the other way round:

- Hoenn gyms give **Hoenn badges** and Hoenn's Elite Four gives Hoenn's own
  Hall of Fame flag; Kanto's badges and Champion title are untouched.
- **HMs** work with the Kanto badge (FireRed rules) **or** the Hoenn badge
  (Emerald rules: Cut 1, Flash 2, Rock Smash 3, Strength 4, Surf 5, Fly 6,
  Dive 7, Waterfall 8). **Obedience** counts badges from either region.
- **Level caps** (if on) use whichever region's cap is higher.
- **Nuzlocke** areas include Hoenn's routes; the **randomizer** covers Hoenn's
  wild Pokémon, trainers and Birch's starters; **HARD MODE** teams are
  Kanto-only.
- Old saves keep working: the Hoenn data starts empty the first time.

**Tested in the web player:** the ferry both ways, the Littleroot intro (truck,
house, wall clock), Birch's rescue and the starter choice, a Hoenn trainer
battle (and that he stays beaten), saving and reloading in Hoenn, the Frontier
Pass and a **Battle Tower** challenge up to the first battle. The rest of
Hoenn's story hasn't been played through.

**Known gaps:** the trainer card shows Kanto's badges only; there's no PokéNav
(Match Call) in the START menu; Hoenn's dialogue is
English even in Japanese mode (about 4,300 texts); contests, secret bases and
Dive haven't been tested.

## Japanese mode

Switch on the OPTION screen: the top row (next to the title) is the language,
ENGLISH or にほんご; press up from TEXT SPEED to reach it. It works on the
title menu too, so the whole game, Oak's speech included, can be played in
Japanese. You can switch back and forth at any time.

- **Style:** like the original Japanese FireRed: kana only, with spaces
  between phrases. Lines are wrapped for the Japanese font.
- **Official names:** Pokémon, moves, items, abilities, types, natures,
  Pokédex categories and entries come from PokeAPI's Japanese data
  (`dev_scripts/qol_ja_names.py`, `qol_ja_descriptions.py`). Move, item and
  ability descriptions use the official text where it fits the window and a
  shortened version where it doesn't.
- **Translated text:** the story, NPCs, signs, trainers, menus, the bag, the
  PC, battle messages, and this hack's own features. It was machine-translated
  (by Claude) following `text/ja/GLOSSARY.md`, and hasn't been reviewed by a
  native speaker yet.
- **Names you type** (player, rival, nicknames) use the English keyboard.
  Default player and rival names stay in English letters, so a save looks
  right in both languages. Pokémon caught in Japanese mode get their Japanese
  name and keep it in English mode (shown in kana).
- **How it works:** each translated string carries its Japanese text right
  after the English one (`{JPN}{JPN}` marker). `dev_scripts/qol_japanese.py`
  manages this: `show`, `check`, `apply`, `coverage`. To fix a line, edit its
  JSON in `text/ja/` and run `python3 dev_scripts/qol_japanese.py apply`.

## Web player (`web/`)

A browser player built on EmulatorJS 4.2.3 with the mGBA core, vendored into
`web/emulatorjs/` (GPL-3.0). It needs no special server headers.

- **Your ROM never leaves your device.** You pick the `.gba` file; it's kept
  in this browser's storage (IndexedDB), so next time it's one tap.
- **Saves** are stored in the browser under the fixed name "FireRed QoL", so
  they carry over to newer builds. The page writes the in-game save to browser
  storage every few seconds and when you switch away, so save in-game, wait a
  moment, then close. **Import save** loads a `.sav` and restarts into it. Use
  **Export save** now and then. Clearing site data erases saves. A save from
  an older build can break if the save layout changed.
- The vendored EmulatorJS has one patch (`data/src/GameManager.js`, see
  `web/emulatorjs/README.md`): without it, every page reload deleted the
  stored save.
- **Turbo** button with ×2/×3/×4/×8, on-screen gamepad on phones, EN/日本語
  toggle, save states and control remapping in the emulator's own menu.
- **Installable web app**: a manifest, icons and an offline cache (`web/sw.js`)
  let you add it to your home screen (iPhone: Safari → Share → Add to Home
  Screen; Android/desktop Chrome: menu → Install app). It runs full-screen and
  works offline after the first visit. Bump `CACHE` in `sw.js` whenever `web/`
  changes, so installed copies update.

**Put it online with Vercel (one time, about a minute):**
1. Sign in at [vercel.com](https://vercel.com) with your GitHub account.
2. **Add New → Project → Import** `cacoleman16/Game-dev-play`.
3. Leave every setting as it is (`vercel.json` already says: no build, serve
   the `web/` folder) and press **Deploy**.

You'll get a URL like `https://game-dev-play.vercel.app`. From then on every
push that touches `web/` redeploys automatically. Pushes that only change
the ROM source are skipped (`ignoreCommand` in `vercel.json`). The page holds
no game data and is marked `noindex`, so search engines won't list it. For
extra privacy, turn on Vercel's Deployment Protection in the project settings.

Command-line alternative: `npx vercel --prod` from the repo root
(`.vercelignore` uploads only `web/`).

**Or run it locally:** `cd web && python3 -m http.server 8000`, then open
http://localhost:8000.

---

## Next steps, in a sensible order

1. **Play-test further** in the web player: Surf and Cut via HM compatibility,
   auto-run, DexNav on a route, Exp. Share messages, a Nuzlocke death, a HARD
   MODE gym.
2. **Japanese review**: a native speaker pass over `text/ja/`; Emerald-only
   strings that FireRed never shows are left in English.
3. **Hard mode for more trainers** (rival fights, Giovanni's Rocket battles),
   and a randomizer option for gift Pokémon with matching dialogue.
4. **Hoenn polish**: Hoenn badges on the trainer card, PokéNav, and translating Hoenn's ~4,300 texts into Japanese.
5. **More side quests**, e.g. around the Sevii Islands and the Trainer Tower.
