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
| IV maxing (Hyper Training) | ✅ | The gentleman on Celadon **5F** Hyper Trains: Bottle Cap = one stat, Gold Bottle Cap = all six. Caps are sold on 5F. |
| DexNav | ✅ on | START menu from a new game; **R** searches the registered species |
| Mega / Z-Moves / Dynamax / Tera | ✅ | Mega Ring, Z-Power Ring, Dynamax Band and Tera Orb are given at new game; Dynamax is enabled. **Celadon 4F** sells all 92 Mega Stones (¥10,000) and 35 Z-Crystals (¥8,000), plus every evolution item. |
| Smart trainer AI | ✅ | All 80 bosses (gym leaders, Elite Four, champion and rival battles, Cooltrainers, Rangers) use **Smart Trainer** AI; the other 543 trainers use Basic Trainer AI. Teams and held items are still vanilla. |
| Gen 1–9 roster | ✅ | Every Kanto/Sevii table gained up to 5 Gen 4–9 Pokémon in rare duplicate slots (forest bugs, Tower ghosts, Power Plant electrics, Seafoam ice, baby pseudo-legendaries in Cerulean Cave...). Every vanilla species is still there. Regenerate with `dev_scripts/qol_add_modern_encounters.py`. |
| Level caps | ✅ optional | Kanto caps (14/21/24/29/43/43/47/50, then 63 before the Champion). Off by default; toggle at the **NES in your bedroom**. Rare Candies and the Day Care respect them. |
| Challenge settings | ✅ | The NES in your bedroom toggles level caps and following Pokémon; the bedroom notice lists the L/R controls |
| Nuzlocke, randomizer, difficulty tiers | ⏳ next | Not built yet. Difficulty tiers exist in the engine (`B_VAR_DIFFICULTY`) but FRLG trainers have only one team each |
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
| `data/maps/CeladonCity_DepartmentStore_5F_Frlg/scripts.inc` | Bottle Caps, mints, Ability Capsule/Patch, EV Berries; Hyper Training gentleman |
| `data/maps/CeladonCity_DepartmentStore_4F_Frlg/scripts.inc` | menu shop: gifts, evolution items, Mega Stones, Z-Crystals |
| `src/data/items.h` | Mega Stone / Z-Crystal prices (`QOL_MEGA_STONE_PRICE`, `QOL_Z_CRYSTAL_PRICE` in `config/item.h`) |
| `src/caps.c`, `include/config/caps.h` | optional Kanto level caps |
| `data/maps/PalletTown_PlayersHouse_2F_Frlg/scripts.inc` | NES challenge settings, updated notice |
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

1. **Play-test a new game** in the web player: followers through doors, Surf
   and Cut via HM compatibility, auto-run, DexNav, Exp. Share messages.
2. **Nuzlocke mode** (first encounter per area, fainted = boxed, nicknames) and a **randomizer**, as NES options.
3. **Trainer teams**: competitive sets and held items for bosses in `src/data/trainers_frlg.party`, maybe as a Hard difficulty tier.
4. **EV/IV judge** NPC.
5. **Bigger projects**: merged regions, side quests, bilingual text.
