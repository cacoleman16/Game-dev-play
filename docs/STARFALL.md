# STARFALL — the story that ties Kanto and Hoenn together

This hack's own story. Kanto's and Hoenn's original adventures still happen,
but they become two halves of one plot: a meteor shower, a Pokémon that fell
with it, and a group that has been quietly bankrolling Team Rocket, Team Magma
and Team Aqua to get its hands on what the meteors left behind.

## The premise

On the night your adventure begins, a meteor shower — **the Starfall** — lights
up the sky over Kanto and Hoenn at the same time. Nobody has seen anything like
it in a thousand years.

A week earlier, **Steven Stone**, a stone collector in Hoenn (and its Champion),
sent **Prof. Oak** a meteorite to study. During the Starfall it started to hum.
When you walk into Oak's Lab, it cracks open and a **Beldum** floats out — and
chooses you. That Beldum is your partner for the whole game.

The flash scares the other Pokémon in Oak's Lab: Bulbasaur and Squirtle burst
out of their Poké Balls and run off into the wild (you can find and catch them
later). Blue takes the last one, Charmander — "fire melts steel!"

## Your family

Your father, **Norman**, has just become a Gym Leader in Petalburg City, Hoenn.
Your mother is packing to join him. When you first sail to Hoenn on the
**S.S. TIDAL**, the moving truck takes you both to **Littleroot Town**, and
Emerald's Hoenn story begins: Prof. Birch, a Hoenn partner, May or Brendan.

## Team Nova

A group of astronomers, collectors and believers who think the Starfall is a
message. Meteor-born Pokémon like your Beldum, and the fragments the meteors
left — **Star Shards** — answer to something in space: **Deoxys**.

- **Vega** — the leader. An heiress whose fortune quietly paid for Team Rocket's
  Silph Co. takeover and for Team Magma's and Team Aqua's machines. Each team
  thinks it's chasing its own dream; all of them were hunting shards for her.
  She wants to call Deoxys down and make it hers.
- **Altair** — a meteorite hunter. Shows up wherever a meteorite does.
- **Deneb** — Vega's strategist. Handles the money and the deals.

Their Pokémon come from the sky: Clefairy, Lunatone, Solrock, Bronzor, Elgyem,
Minior, Staryu, Baltoy, Beldum's cousins, and at the very end an Ultra Beast.

## Chapter by chapter

| # | Where | When | What happens |
|---|-------|------|--------------|
| 0 | Oak's Lab, Pallet Town | New game | The meteorite cracks; Beldum chooses you. Bulbasaur and Squirtle run off; Blue takes Charmander. Mom tells you about Dad in Hoenn and the move |
| 1 | Mt. Moon B2F | After the fossil | **Altair** was waiting for Team Rocket to dig out the Moon's meteorite core. First battle with Team Nova |
| 2 | Silph Co. 11F | After Giovanni | **Deneb** collects the Master Ball data Team Rocket stole "for its sponsor" — Giovanni was working for Nova money all along |
| 3 | Cinnabar Lab | First visit to the fossil room | The lab has been studying Star Shards: Pokémon DNA from space. Nova raids it — beat a grunt and **Deneb** |
| 4 | Littleroot Town | First trip to Hoenn | You move in. Prof. Birch knows about your Beldum from Oak |
| 5 | Meteor Falls | After Magma steals Prof. Cozmo's meteorite | **Altair** watched it happen — Nova paid for Magma's trip |
| 6 | Mt. Chimney | After Maxie | **Deneb** came to take the meteorite back from Magma's machine, but you got there first |
| 7 | Mossdeep Space Center | After Maxie and Tabitha | **Altair** was after the launch's flight data — the next Starfall's path. Steven recognises your Beldum: it came from his meteorite |
| 8 | Steven's house, Mossdeep | After Hoenn's Hall of Fame | Steven's letter: Vega has gathered every shard on **Birth Island** for the second Starfall. Go when you're Champion of **both** regions |
| 9 | Birth Island | Both Champion titles + Steven's letter | Sail there on the S.S. TIDAL. Fight through Team Nova, then **Vega**. The shards fly into the sky, and **Deoxys** comes down — and you face it yourself |
| 10 | Ending | After Deoxys | The Starfall ends. Steven, Oak and Birch's messages; THE END |

## Every starter is still catchable

| Starter(s) | Where |
|---|---|
| **Beldum** | your partner (Oak's Lab) |
| Bulbasaur | Viridian Forest (it ran there) |
| Squirtle | Vermilion City, fishing (the Old Rod and Good Rod) |
| Charmander | Route 4, east of Mt. Moon |
| Treecko / Torchic / Mudkip | one from Prof. Birch; the others: Petalburg Woods (Treecko), Route 117 (Torchic), Route 102 fishing (Mudkip) |
| Chikorita, Cyndaquil, Totodile, Turtwig, Chimchar, Piplup | Kanto's Safari Zone (Prof. Birch also gives a Johto starter after Hoenn's Hall of Fame) |
| Snivy … Quaxly (Gen 5–9) | Hoenn's Safari Zone |

## Starfall Isle

Where the Starfall's meteors came down: Emerald's Faraway Island, renamed.
The S.S. TIDAL in Vermilion sails there once you've been to Hoenn. Its wild
Pokémon roam in the open (walk into one to battle it): meteor and star
Pokémon (Clefairy, Elgyem, Minior, Bronzor), rare babies and dragons
(Togepi, Riolu, Larvitar, Gible, Dreepy) and, rarely, Chimecho. (The Wishing Star itself waits in Seika.) Mew still
hides in the grass.

## Technical notes

- Story flags: `FLAG_QOL_STARFALL_*` and the `FLAG_QOL_HIDE_NOVA_*` object
  flags (FRLG free flags `0x3C6`–`0x3D7`), see `include/constants/flags.h`.
- Team Nova trainers: `TRAINER_NOVA_*` in `include/constants/opponents_frlg.h`,
  teams in `src/data/trainers_frlg.party`; classes `TRAINER_CLASS_TEAM_NOVA`,
  `TRAINER_CLASS_NOVA_ADMIN`, `TRAINER_CLASS_NOVA_LEADER`.
- Scripts and text: `data/scripts/qol_starfall.inc`, hooked from each map's
  own script. Japanese text: `text/ja/data/scripts/qol_starfall.json`.
- The cutscene characters are map objects that only appear during their scene
  (each map hides them on load), so old saves never see them out of place.
