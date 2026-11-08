# SKYRIFT — the last chapter of the Starfall

What happens after the Wishing Star, in a chain of islands where the sky has
torn open. You choose a companion who travels and fights beside you, you
choose a wish, and the ending depends on both, and on one last choice.

## 1. The wish (Sky Shrine, Seika)

When you talk to the Wishing Star (Jirachi) it offers you one wish:

| Wish | What you get |
|---|---|
| **STRENGTH** | an Ability Patch, a Gold Bottle Cap and 5 Rare Candies |
| **A FRIEND** | Jirachi itself joins your team, with no battle |
| **FORTUNE** | the Shiny Charm |
| **PEACE FOR NOVA** | a Comet Shard, and the star's light reaches Vega (see the ending) |

For every wish except A FRIEND, Jirachi then battles you and can be caught.
When it leaves, the sky over the eastern sea tears open. The Skyrift has
appeared.

## 2. Choose a companion (Lanternport pier)

Three people are waiting on Lanternport's pier, and each asks to come with you:

| Companion | Who | Battles with |
|---|---|---|
| **Altair** | Team Nova's meteorite hunter. He dug out the star stone and wants to bring Vega home | Excadrill, Rhyperior, Probopass (Ground, Rock, Steel) |
| **Deneb** | Nova's strategist. She paid for everything and still believes in Vega's dream | Gardevoir, Bronzong, Togekiss (Psychic, Fairy) |
| **Mira** | Champion Seiji's granddaughter, a young star reader who saw the sky split from the Sky Shrine | Xatu, Clefable, Lanturn |

- Your companion **follows you** and **fights beside you**. Wild battles
  become 2-on-2 with them, and some trainers challenge you both at once.
- **Talk to them** to hear what they think of the place, or of each other.
- What they say, and how the story ends, depends on who you chose.

## 3. The Skyrift Isles

After you've chosen, the S.S. TIDAL in Vermilion or at Lanternport sails to
the SKYRIFT ISLES.

| Place | What's there |
|---|---|
| **Riftwake Harbor** | Island town: Pokémon Center, Mart (Beast Balls), Dr. Halley's rift hut, the inn |
| **Shardfall Beach** | A beach full of sky-crystal shards. Nova loyalists (double battles). A rift |
| **Meteor Basin** | The crater where the second Starfall's biggest meteorite hit. Two rifts |
| **Observatory Cliffs** | The road up to Vega's observatory. Two rifts |
| **Nova Observatory** | Vega's base: the lab, the engine room and the roof |

**Rift battles.** Each rift holds an Ultra Beast boosted by rift energy:
Nihilego, Buzzwole, Pheromosa, Xurkitree and Kartana, all at Lv 75. Beast
Balls work best on them. A rift stays open until you catch its Ultra
Beast.

**The sixth rift.** Once Vega's story is over and all five Ultra Beasts are
caught, Dr. Halley picks up a sixth rift next to the meteorite in Meteor
Basin. Something that feeds on light is inside it: Necrozma, Lv 80, boosted
like the others.

## 4. The climax

On the observatory roof, Vega and her bodyguard **Sirius** are powering the
**Skyrift Engine** to call Deoxys back. You and your companion fight them
together. When the engine overloads, the rift starts to pull Vega in. Then
comes your last choice: **take her hand, or let go**.

| You chose | Ending |
|---|---|
| Take her hand | She lives and gives up the engine. She gives you **Poipole** and later waits on the roof for rematches |
| Let go | The rift swallows her. A **Cosmog** drifts out after her. Sirius swears revenge and waits on the roof for rematches |

The PEACE FOR NOVA wish adds a closing scene: the star's light reaches Vega
in the rift. Each companion closes the story in their own way. Afterwards
your companion asks whether to keep travelling with you. If you say yes,
they follow you everywhere and keep fighting beside you. You can change
your mind at the Riftwake inn.

## Technical notes

- Companions use the expansion's NPC followers (`FNPC_ENABLE_NPC_FOLLOWERS`,
  `include/config/follower_npc.h`) with battle partners
  (`src/data/battle_partners.party`, `include/constants/battle_partner.h`).
- Their back sprites are recoloured copies of existing ones
  (`graphics/trainers/back_pics/qol_*.png`).
- Rift Ultra Beasts use `settotemboost`.
- Maps and scripts: `dev_scripts/seika/skyrift.py` (the isles) and
  `dev_scripts/seika/skyrift_story.py` (the wish, the companions, the
  climax).
