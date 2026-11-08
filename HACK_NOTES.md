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
> and flags are set up in `NewGameInitData`.

## Build

```sh
make firered -j$(nproc)      # → pokefirered.gba (non-matching, by design)
```
Toolchain: see `INSTALL.md` / `docs/install/` (arm-none-eabi gcc + libpng).

The repo is **public**, so CI only checks that the ROM builds and never uploads
it. Build it on your own machine to get the `.gba`. If you make the repo
private, CI could attach the ROM to each run for you to download. Ask and I'll
add that.

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
| Randomizer | ✅ optional | Each species becomes another with a similar base stat total (legendaries swap among themselves). Fixed per save file (seeded by your Trainer ID). Changes wild Pokémon, DexNav, trainers' teams, scripted encounters and both starters (Oak names what's in each ball). Gift Pokémon and trades stay vanilla. Turn it on before picking a starter. |
| Difficulty tiers | ✅ optional | **HARD MODE**: competitive 4–6 Pokémon teams with items, natures, EVs and Megas for the 8 gym leaders, the Elite Four and the Champion. Aces keep their vanilla levels. Other trainers are unchanged. |
| Challenge settings | ✅ | The **NES in your bedroom** is a menu: NUZLOCKE, RANDOMIZER, HARD MODE, LEVEL CAPS, FOLLOWERS (each shows ON/OFF). The bedroom notice lists the L/R controls |
| Side quests | ✅ | **Field Research** (a researcher in Oak's Lab): 8 tasks about Pokémon from faraway regions, each with a reward, ending in a post-game hunt for **Galarian Articuno, Zapdos and Moltres**. See *Side quests* below |
| Battle Frontier | ⏳ later | Content work |
| Every region in one map | ⏳ later | The expansion contains both Hoenn and Kanto maps, so merging is a map-data project (see docs/tutorials/how_to_frlg.md) |
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
| `FLAG_QOL_NUZLOCKE_AREAS_START` | `0x35D`–`0x3C5` | one "encounter used" flag per Kanto/Sevii map section |
| `VAR_QOL_LAST_REPEL` | `0x40A1` | last Repel used |
| `VAR_QOL_DEXNAV_SPECIES` / `_STEPS` | `0x40F7` / `0x40F8` | DexNav |
| `VAR_QOL_DIFFICULTY` | `0x40F9` | trainer difficulty (1 Normal, 2 Hard) |
| `FLAG_QOL_HIDE_GALAR_*` | `0x8EC`–`0x8EE` | Galarian birds hidden until the research reveals them |
| `FLAG_QOL_CAUGHT_GALAR_*` | `0x8EF`–`0x8F1` | Galarian birds caught |
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
4. **More side quests / Battle Frontier**: the expansion ships Emerald's
   Frontier code; FRLG has the Trainer Tower and the Sevii Islands to build on.
5. **Bigger project**: merged regions.
