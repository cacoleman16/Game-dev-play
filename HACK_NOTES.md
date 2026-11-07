# FireRed QoL Hack

A personal, single-player FireRed hack built on
[pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion)
**1.17.2**, using its FireRed/LeafGreen build (`make firered`). The expansion
already provides the modern battle engine. This hack switches on the parts
you asked for, adds the missing QoL pieces, and ships a browser player.

Every hack change is tagged `[QoL]` in the source: `git grep -n "\[QoL\]"`.
The earlier pokefirered-based version (7 features) is kept under
[`reference/pokefirered-qol/`](reference/pokefirered-qol/) as git patches.

> **Status:** `make firered` builds cleanly in GitHub Actions ("Build ROM"
> workflow) with these changes. Nothing has been play-tested in an emulator
> yet. Start a **new save**: the starting items and flags are set up in
> `NewGameInitData`.

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
| Ability Capsule / Patch | ✅ | Celadon 5F |
| EV wipe | ✅ | EV-lowering Berries at Celadon 5F; vitamins have no Gen 3 cap |
| IV maxing (Hyper Training) | ⏳ next | Bottle Caps exist but need a Hyper Training NPC |
| DexNav | ✅ on | START menu from a new game; **R** searches the registered species |
| Mega / Z-Moves / Dynamax / Tera | ✅ engine, ⏳ items | Mega Ring, Z-Power Ring, Dynamax Band and Tera Orb are given at new game; Dynamax is enabled. **Mega Stones and Z-Crystals still need to be placed** in the world or a shop. |
| Smart trainer AI | ✅ | All 80 bosses (gym leaders, Elite Four, champion and rival battles, Cooltrainers, Rangers) use **Smart Trainer** AI; the other 543 trainers use Basic Trainer AI. Teams and held items are still vanilla. |
| Gen 1–9 roster | ✅ engine, ⏳ encounters | All species/moves exist in the engine; Kanto encounter tables still only have Gen 1–3 Pokémon |
| Challenge modes (Nuzlocke, level caps, randomizer, difficulty) | ⏳ next | Level caps and difficulty tiers exist in the engine (`include/config/caps.h`, `B_VAR_DIFFICULTY`); a main-menu toggle screen and Nuzlocke/randomizer rules are still to build |
| Side quests, battle frontier | ⏳ later | Content work |
| Every region in one map | ⏳ later | The expansion contains both Hoenn and Kanto maps, so merging is a map-data project (see docs/tutorials/how_to_frlg.md) |
| Japanese + English | ⏳ later | The player UI already has EN/日本語; in-game text needs a language flag and dual text banks |

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
| `data/maps/CeladonCity_DepartmentStore_5F_Frlg/scripts.inc` | mints, Ability Capsule/Patch, EV Berries |

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
| `VAR_QOL_LAST_REPEL` | `0x40A1` | last Repel used |
| `VAR_QOL_DEXNAV_SPECIES` / `_STEPS` | `0x40F7` / `0x40F8` | DexNav |

The vars are unused in **both** the Emerald and FRLG var maps, because the
expansion's shared engine code still uses Emerald var numbers in FRLG builds.

---

## Web player (`web/`)

A browser player built on EmulatorJS 4.2.3 with the mGBA core, vendored into
`web/emulatorjs/` (GPL-3.0). It needs no special server headers.

- **Your ROM never leaves your device.** You pick the `.gba` file; it's kept
  in this browser's storage (IndexedDB), so next time it's one tap.
- **Saves** are stored in the browser under the fixed name "FireRed QoL", so
  they carry over to newer builds. Use **Export save** now and then. Clearing
  site data erases them. A save from an older build can break if the save
  layout changed.
- **Turbo** button with ×2/×3/×4/×8, on-screen gamepad on phones, EN/日本語
  toggle, save states and control remapping in the emulator's own menu.

**Put it online (one time):** GitHub repo → **Settings → Pages → Build and
deployment → Source: GitHub Actions**, then re-run the **Web player** workflow
(Actions tab). It will be at `https://cacoleman16.github.io/Game-dev-play/`.
The page holds no game data, so it being public is fine.

**Or run it locally:** `cd web && python3 -m http.server 8000`, then open
http://localhost:8000.

---

## Next steps, in a sensible order

1. **Play-test a new game** in the web player: followers through doors, Surf
   and Cut via HM compatibility, auto-run, DexNav, Exp. Share messages.
2. **Hyper Training NPC** for Bottle Caps, plus an EV/IV judge.
3. **Mega Stones / Z-Crystals**: a shop or gift NPC, or hidden items.
4. **Challenge menu** after the intro: level caps (Kanto badge caps), difficulty, Nuzlocke rules, randomizer.
5. **Encounters**: add Gen 4–9 Pokémon to Kanto/Sevii wild tables (`src/data/wild_encounters.json`).
6. **Trainer teams**: competitive sets and held items for bosses in `src/data/trainers_frlg.party`.
7. **Bigger projects**: merged regions, side quests, bilingual text.
