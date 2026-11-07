# FireRed QoL Hack: Change Notes

A personal, single-player quality-of-life hack built on the
[pret/pokefirered](https://github.com/pret/pokefirered) decompilation
(imported unmodified in commit `0cf1529`, upstream `ce6406d8`). Every feature
is its own commit, so `git show <commit>` gives the exact diff for that feature.
In the source, every change is tagged with a `// [QoL]` (C) or `@ [QoL]` (script) comment:

```sh
git grep -n "\[QoL\]"
```

| # | Feature | Commit |
| --- | --- | --- |
| 1 | Physical/Special split | `Add physical/special split by move category` |
| 2 | Fairy type + 6 Fairy moves | `Add the Fairy type with Gen 6 typings and six Fairy moves` |
| 3 | Modern Exp. Share key item, running indoors from the start | `Add modern Exp. Share key item (default ON)` |
| 4 | Infinite TMs, Smart Repel loop, solo trade evolutions | `Add infinite TMs, Smart Repel loop and solo trade evolutions` |
| 5 | Following Pokémon | `Add Following Pokemon` |

## Building

Follow `INSTALL.md` (install agbcc, then `make`). This is a non-matching
build, so `make compare` will fail by design; use plain `make` (or
`make firered`). `.github/workflows/build.yml` runs the same build on every
push, so compile errors show up in the repo's **Actions** tab. The ROM is not
uploaded anywhere.

> **Verification status:** every changed C file was type-checked with the host
> `gcc` using agbcc's C89 rules (`dev_scripts/syntax_check.sh`). The full ROM
> was not built in the authoring environment, and none of this has been
> play-tested in an emulator yet. Start a **new save**: several features
> (Exp. Share item, running shoes, retyped species) are set up at new-game time.

---

## 1. Physical/Special split

The physical/special decision now comes from a per-move `category` field
instead of the move's type.

**`include/constants/pokemon.h`**: new constants
```c
#define DAMAGE_CATEGORY_PHYSICAL 0
#define DAMAGE_CATEGORY_SPECIAL  1
#define DAMAGE_CATEGORY_STATUS   2
```

**`include/pokemon.h`**: `struct BattleMove` gains a field
```c
    u8 flags;
    u8 category; // [QoL] DAMAGE_CATEGORY_PHYSICAL/SPECIAL/STATUS
};
```

**`include/battle.h`**: replace
```c
#define IS_TYPE_PHYSICAL(moveType)(moveType < TYPE_MYSTERY)
#define IS_TYPE_SPECIAL(moveType)(moveType > TYPE_MYSTERY)
```
with
```c
#define GET_MOVE_CATEGORY(move) (gBattleMoves[move].category)
#define IS_MOVE_PHYSICAL(move)  (GET_MOVE_CATEGORY(move) == DAMAGE_CATEGORY_PHYSICAL)
#define IS_MOVE_SPECIAL(move)   (GET_MOVE_CATEGORY(move) == DAMAGE_CATEGORY_SPECIAL)
#define IS_MOVE_STATUS(move)    (GET_MOVE_CATEGORY(move) == DAMAGE_CATEGORY_STATUS)
```
(The old macros are gone on purpose, so any leftover type-based check fails to compile.)

**`src/data/battle_moves.h`**: every move has a `.category` line after `.type`,
using the Gen 4+ classification (139 physical, 77 special, 139 status). Power 0
means status. Examples: Fire Punch, Waterfall, Crunch, Shadow Punch are now
physical; Hyper Beam, Gust, Shadow Ball, Sludge Bomb are special.
```c
    [MOVE_FIRE_PUNCH] =
    {
        .effect = EFFECT_BURN_HIT,
        .power = 75,
        .type = TYPE_FIRE,
        .category = DAMAGE_CATEGORY_PHYSICAL,
```

**`src/pokemon.c` `CalculateBaseDamage`**
- `if (IS_TYPE_PHYSICAL(type))` becomes `if (IS_MOVE_PHYSICAL(move))`, and the special branch becomes `else if (IS_MOVE_SPECIAL(move))`.
- Type-boosting held items (Charcoal, etc.) raise Atk or Sp. Atk depending on the move's category.
- Weather (rain/sun) and Flash Fire used to sit inside the special-only branch. They now run for every non-status move, so physical Fire/Water moves get them too:
```c
    if (type == TYPE_MYSTERY)
        damage = 0; // is ??? type. does 0 damage.

    // [QoL] ... these modifiers apply to every damaging move.
    if (!IS_MOVE_STATUS(move))
    {
        // Are effects of weather negated with cloud nine or air lock
        if (WEATHER_HAS_EFFECT2)
        ...
```

**`src/battle_script_commands.c`**
- `Cmd_accuracycheck`: Hustle's accuracy drop uses `IS_MOVE_PHYSICAL(move)`.
- `Cmd_datahpupdate`: Counter/Mirror Coat bookkeeping uses `IS_MOVE_PHYSICAL(gCurrentMove)`, so Counter reflects physical Fire Punch, Waterfall, and so on.

---

## 2. Fairy type

**Type constant** (`include/constants/pokemon.h`)
```c
#define TYPE_DARK           17
#define TYPE_FAIRY          18 // [QoL] Fairy type
#define NUMBER_OF_MON_TYPES 19
```

**Type chart** (`src/battle_main.c`, `include/battle_main.h`): `gTypeEffectiveness`
grows from 336 to 366 bytes. It uses the Gen 6 chart: Fairy is strong against
Fighting/Dragon/Dark, resisted by Fire/Poison/Steel, weak to Poison/Steel,
resists Fighting/Bug/Dark, and is immune to Dragon. To match the chart that
introduced Fairy, **Steel no longer resists Ghost or Dark.** The Fairy rows sit
above the `TYPE_FORESIGHT` separator.

**Everything indexed by type**
| File | Change |
| --- | --- |
| `src/battle_main.c` | `gTypeNames[TYPE_FAIRY] = _("FAIRY")` |
| `src/battle_message.c` | `gText_AFairyMove` ("a FAIRY move") in `sATypeMove_Table` |
| `src/list_menu.c` | `[TYPE_FAIRY + 1] = { 32, 12, 0x04 }`, the badge drawn into a free slot of `graphics/interface/menu_info.png` |
| `graphics/interface/pokemon_types.pal` | unused entry 10 became Fairy pink (238,153,172) |
| `src/tm_case.c`, `graphics/tm_case/disc_types_2.pal` | Fairy TM disc palette at offset `0x110` (palette grew to 32 colours) |
| `src/data/union_room.h` | Fairy in the Union Room trade-type list |
| `src/battle_script_commands.c` | Hidden Power keeps its 16 possible types (never Fairy, like Gen 6) |
| `src/data/pokemon/pokedex_orders.h` | primary-Fairy species listed last in Pokédex TYPE order |

**Species retyped** (`src/data/pokemon/species_info.h`, Gen 6 typings):
Clefairy/Clefable/Cleffa and Togepi/Snubbull/Granbull (Fairy), Togetic
(Fairy/Flying), Jigglypuff/Wigglytuff/Igglybuff/Azurill (Normal/Fairy), Mr. Mime
and Ralts/Kirlia/Gardevoir (Psychic/Fairy), Marill/Azumarill (Water/Fairy),
Mawile (Steel/Fairy).

**Moves retyped**: Charm, Sweet Kiss and Moonlight are now Fairy.

**New moves** (IDs 355-360, `MOVES_COUNT` = 361)
| Move | Cat. | Pow | Acc | PP | Effect |
| --- | --- | --- | --- | --- | --- |
| FAIRY WIND | Special | 40 | 100 | 30 | plain hit |
| DISARM VOICE (Disarming Voice) | Special | 40 | never misses | 15 | hits both foes, blocked by Soundproof |
| DRAININGKISS (Draining Kiss) | Special | 50 | 100 | 10 | drains 50% (Gen 3 drain) |
| DAZZLNGGLEAM (Dazzling Gleam) | Special | 80 | 100 | 10 | hits both foes |
| MOONBLAST | Special | 95 | 100 | 15 | 30% Sp. Atk drop |
| PLAY ROUGH | Physical | 90 | 90 | 10 | 10% Attack drop |

Names respect the 12-character limit. Each move was added to
`include/constants/moves.h`, `src/data/battle_moves.h`,
`src/data/text/move_names.h`, `src/move_descriptions.c` and
`data/battle_anim_scripts.s` (reusing the Gust, Hyper Voice, Absorb, Luster
Purge, Mist Ball and Dizzy Punch animations). They're in the level-up
learnsets of the Fairy lines (`src/data/pokemon/level_up_learnsets.h`, lines
tagged `// [QoL] Fairy move`). Example, Gardevoir: Disarming Voice 9, Draining
Kiss 24, Dazzling Gleam 46, Moonblast 56.

---

## 3. Modern Exp. Share (key item, defaults ON)

- **Item** (`src/data/items.json`): EXP. SHARE is now a registrable key item
  (`POCKET_KEY_ITEMS`, importance 1, `FieldUseFunc_ExpShare`). It's added to
  the bag in `NewGameInitData` (`src/new_game.c`).
- **Toggle** (`src/item_use.c`): `FieldUseFunc_ExpShare` flips
  `FLAG_SYS_EXP_SHARE_DISABLED` (the unused `FLAG_0x8E0`). The flag starts
  clear, so the feature is **ON** by default. It works from the bag and when
  registered to SELECT.
- **Distribution** (`src/battle_script_commands.c` `Cmd_getexp`): while ON,
  each Pokémon that battled gets the full EXP. yield (no splitting) and every
  other non-egg party member gets half. Bench members share one message
  ("Your team gained EXP. Points from the EXP. SHARE!",
  `STRINGID_TEAMGAINEDEXP`); level-ups are still announced one by one. While
  OFF, the vanilla Gen 3 formula applies.
```c
            if (IsModernExpShareOn())
            {
                *exp = calculatedExp;                // participants: full amount
                ...
                gExpShareExp = calculatedExp / 2;     // bench: half
```
- The Route 15 aide (`data/maps/Route15_WestEntrance_2F/`) now gives a
  **LUCKY EGG** for 50 caught species, since you already own the Exp. Share.

## 4. Running indoors from the start

- `src/bike.c` `IsRunningDisallowed`: the `if (!gMapHeader.allowRunning) return TRUE;`
  check is removed, so B-dash works in buildings and caves. Individual
  `MB_RUNNING_DISALLOWED` tiles still block running.
- `src/new_game.c`: `FlagSet(FLAG_SYS_B_DASH)` at new game, so you have the
  Running Shoes from the start. The Pewter City aide still gives them in
  dialogue, which is harmless.

## 5. Infinite TMs

- `src/party_menu.c`: the three `if (item < ITEM_HM01) RemoveBagItem(item, 1);`
  calls (`CB2_UseItem`, `CB2_UseTMHMAfterForgettingMove`, `Task_LearnedMove`)
  are removed.
- `src/tm_case.c`: TMs behave like HMs. No quantity is drawn, and they can't
  be given to hold or sold (selling one would lose it for good). The HM badge
  still shows only on real HMs.

## 6. Smart Repel loop

- `data/scripts/repel.inc`: `EventScript_RepelWoreOff` now asks
  "REPEL's effect wore off… Would you like to use another REPEL?" if the bag
  still has one, and uses it on YES.
- `src/item_use.c`: new specials `GetRepelToReuse` (prefers the kind you used
  last, stored in `VAR_LAST_REPEL_USED` = the unused `VAR_0x40A0`, then
  REPEL > SUPER REPEL > MAX REPEL) and `UseRepelToReuse` (removes it and
  restarts `VAR_REPEL_STEP_COUNT`). Both are registered in `data/specials.inc`.
  `Task_UseRepel` records the kind used.

## 7. Solo evolutions (no trading needed)

`src/data/pokemon/evolution.h`
- `EVO_TRADE` becomes `EVO_LEVEL, 40`: **Kadabra → Alakazam, Machoke → Machamp,
  Graveler → Golem, Haunter → Gengar.**
- `EVO_TRADE_ITEM` becomes `EVO_ITEM` (use the item from the bag like an
  evolution stone): Poliwhirl + King's Rock → Politoed, Slowpoke + King's
  Rock → Slowking, Onix/Scyther + Metal Coat → Steelix/Scizor, Seadra +
  Dragon Scale → Kingdra, Porygon + Up-Grade → Porygon2, Clamperl + Deep Sea
  Tooth/Scale → Huntail/Gorebyss.

To support this, those six items use `FieldUseFunc_EvoItem` in `items.json`.
They have no entry in `gItemEffectTable`, so `src/party_menu.c`
(`ItemUseCB_EvolutionStone`, `CB2_UseEvolutionStone`) checks the evolution
table directly for items outside `IS_POKEMON_ITEM`. As in vanilla, evolutions
into non-Kanto species wait for the National Dex.

To change the level, edit the `40` in those four lines.

---

## 8. Following Pokémon

New module: `src/follower_pokemon.c` / `include/follower_pokemon.h`.

**Behaviour**
- The first non-egg, non-fainted party member follows you as an object event
  with local ID `OBJ_EVENT_ID_FOLLOWER` (254, never used by map templates).
- `FollowerPokemon_Update()` runs every overworld frame (hooked in
  `CB2_Overworld`). It spawns, removes or re-skins the follower so it always
  matches the lead: party reorder, the lead fainting (battle or poison), or
  getting your first Pokémon.
- Every time your tile changes, the tile you left is queued. The follower walks
  there at your speed (walk, run, rock stairs, ice, currents) or hops a ledge
  after you. Because it always walks toward you, its facing direction (Up,
  Down, Left, Right) stays in sync.
- Warps, Fly, surfing, biking, an invisible player and cutscene teleports hide
  the follower on your tile, and it pops back out on your next step. Crossing
  into a connected map shifts its queue along with the map coordinates.
- It never blocks you (you can walk through it and swap places), trainers' line
  of sight, or Strength boulders. Wandering NPCs still bump into it.
- Talk to it with A: it turns to face you, plays its cry, and shows one of three
  friendship-based messages (`data/scripts/follower_pokemon.inc`).
- `FLAG_SYS_HIDE_FOLLOWER` (the unused `FLAG_0x8E1`) hides it, e.g. for a
  cutscene: `setflag FLAG_SYS_HIDE_FOLLOWER` / `clearflag FLAG_SYS_HIDE_FOLLOWER`.
- It's disabled in the Union Room, link rooms and Quest Log playback.

**Engine hooks**
| File | Hook |
| --- | --- |
| `src/overworld.c` | `CB2_Overworld` calls `FollowerPokemon_Update()`; `InitObjectEventsLocal` calls `FollowerPokemon_OnMapLoad()`; `ReloadObjectsAndRunReturnToFieldMapScript` calls `FollowerPokemon_OnReturnToField()` |
| `src/event_object_movement.c` | `DoesObjectCollideWithObjectAt` skips the follower via `FollowerPokemon_ShouldIgnoreCollision`; `UpdateObjectEventCoordsForCameraUpdate` calls `FollowerPokemon_OnCameraShift`; new `RemoveObjectEventWithoutTemplate` (the follower has no map template, so the template-based remover must not be used) |
| `src/field_control_avatar.c` | `GetInteractedObjectEventScript` returns `EventScript_FollowerPokemon` for the follower (it has no template script) |
| `data/specials.inc` | `FollowerPokemon_BufferInfo` special |

**Sprites.** Vanilla FireRed only has walking overworld sprites for 38 species
(Pikachu, Clefairy, Jigglypuff, Pidgey, Lapras, Snorlax, the legendaries, ...),
listed in `sFollowerGraphics`. **Every other species, including the three
starters, follows as a Poké Ball** until you add sprites. To add one:
1. Import a 16×16 or 32×32 overworld sprite sheet with the standard 9 frames
   (face/walk for down, up, left) as a new `OBJ_EVENT_GFX_*` entry, following
   how `OBJ_EVENT_GFX_PIKACHU` is defined in
   `src/data/object_events/object_event_graphics_info.h`,
   `object_event_pic_tables.h`, `object_event_graphics.h` and the pointer table.
2. Add `{ SPECIES_X, OBJ_EVENT_GFX_X },` to `sFollowerGraphics` in
   `src/follower_pokemon.c`.

Community follower sprite packs (for example the one bundled with
pokeemerald-expansion) exist if you want to import a full set; credit their
artists.

**Known limits**
- The follower uses one of the 16 object-event slots. On a map that already
  uses all 16, it simply doesn't appear.
- After a ledge jump it trails one extra tile until you stop.

---

## New flags and vars

| Name | Was | Meaning |
| --- | --- | --- |
| `FLAG_SYS_EXP_SHARE_DISABLED` | `FLAG_0x8E0` (unused) | set = Exp. Share OFF |
| `FLAG_SYS_HIDE_FOLLOWER` | `FLAG_0x8E1` (unused) | set = no following Pokémon |
| `VAR_LAST_REPEL_USED` | `VAR_0x40A0` (unused) | last Repel item used |

## Not in this pass

The bigger goals from the original brief still need their own passes:
- **Every region on one giant map.** This is a map-data project: new map
  groups, layouts, tilesets, connections, warps, encounter tables and story
  flags for Johto/Hoenn/... Use **Porymap** on `data/maps` and
  `data/layouts`, one region at a time.
- **Bilingual Japanese/English.** The Gen 3 font and `charmap.txt` already
  include kana. The usual approach is a language flag plus dual string tables
  (or a JP build) for each text bank. It touches every text file, so it works
  best as its own pass.
