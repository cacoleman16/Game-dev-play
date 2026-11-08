# SEIKA — the region where the first Starfall fell

An original post-game region with an old-capital feel: lantern festivals,
shrines, calligraphy, kilns and mountain temples, and wild Pokémon mostly from
the second generation. Its towns, routes, people and story are this hack's
own. It continues the [Starfall](STARFALL.md) story.

## How you get there

After the Birth Island finale (`FLAG_QOL_STARFALL_DONE`), the S.S. TIDAL in
Vermilion City adds **SEIKA** to its destinations. It docks at Lanternport.

## The legend

A thousand years ago the **first Starfall** came down over Seika. The people
who watched it wrote that a small star fell on **Mt. Skyreach** and went to
sleep there: the **Wishing Star**, who wakes for seven days every thousand
years (Jirachi). They built a shrine in every town to keep watch, and the
shrine keepers became the region's Gym Leaders.

The new Starfall has woken it. What is left of Team Nova wants its wish.

## Towns, roads and Gyms

| # | Town | Gym Leader | Type | Levels | Feel |
|---|------|-----------|------|--------|------|
| 1 | **Lanternport** | Wick, the lantern maker | Fire | 50–52 | Harbor town; the S.S. TIDAL docks here; the Starwatch Festival |
| 2 | **Inkbrush Town** | Sumi, the calligrapher | Psychic | 53–55 | Scriptorium with the Starfall Scroll |
| 3 | **Mossgrove Village** | Kaya, the beekeeper | Bug | 56–57 | Village past Mossgrove Woods |
| 4 | **Kiln City** | Tobin, the potter | Ground | 58–59 | The region's big city: kilns, a department store |
| 5 | **Tidewatch Town** | Nagi, the diver | Water | 60–61 | Fishing town with a tide shrine |
| 6 | **Silkhollow Town** | Ayame, the weaver | Fairy | 62–63 | Silk and dance; reached through Hollow Cave |
| 7 | **Frostmere Village** | Rime, the ice fisher | Ice | 64–65 | A frozen lake village in the north |
| 8 | **Duskpeak Village** | Tomoe, the shrine medium | Ghost | 66–68 | The last village below Mt. Skyreach |
| – | **Starwatch Plateau** | Seika League | – | 70–78 | Elite Four (Steel, Dark, Flying, Dragon) and Champion Seiji |
| – | **Sky Shrine** | the finale | – | 70 | Summit of Mt. Skyreach: the Wishing Star |

Roads between them: Lantern Road, Moss Trail (and Mossgrove Woods), Kiln
Road, Tide Road, Hollow Cave, Weaver's Road, Frost Road, Dusk Road and
Skyreach Pass.

## Story beats

| Where | What happens |
|---|---|
| Lanternport | The festival's lanterns go up for the Wishing Star. The town elder tells the legend |
| Inkbrush Town | The Starfall Scroll was copied by a woman in a fine coat: **Deneb** |
| Mossgrove Woods | **Altair** digs for a star stone from the first Starfall |
| Kiln City | Deneb buys the kilns' Starglass Bell, said to wake the star. Battle her |
| Silkhollow | Altair, again, in Hollow Cave |
| Frostmere | Nova grunts on the ice |
| Duskpeak | Tomoe: the star's seven days are almost over |
| Sky Shrine | After the League: Altair and Deneb ring the bell. Beat them, then the Wishing Star comes down to you |

## Wild Pokémon

Mostly the second generation (Hoothoot, Sentret, Ledyba, Spinarak, Mareep,
Hoppip, Aipom, Yanma, Wooper, Murkrow, Misdreavus, Girafarig, Pineco,
Dunsparce, Gligar, Snubbull, Heracross, Sneasel, Teddiursa, Swinub,
Houndour, Phanpy, Stantler, Natu, Skarmory, Larvitar, Corsola, Remoraid,
Mantine, Chinchou…) with their later evolutions, at Lv 42–66.

## Technical notes

- Maps are built with `dev_scripts/qol_maptools.py` (renderer) and
  `dev_scripts/qol_mapgen.py` (paints outdoor maps from shapes and stamps cut
  from Kanto's maps), driven by `dev_scripts/seika/`.
- Seika maps use `"region": "REGION_SEIKA"` (FireRed build only) and the map
  group `gMapGroup_Seika`, after every existing group, so no existing map
  constant moves.
