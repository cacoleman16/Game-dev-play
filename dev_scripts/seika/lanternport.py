"""Lanternport: Seika's harbor town (Gym 1, Fire) and Lantern Road."""
from common import (g, T, add, door, npc, signpost, pokecenter, mart, house, wild, mapsec,
                    trainer, battle, flag, link, PC, MART, GYM)

VERM = 'LAYOUT_VERMILION_CITY'
SEC = mapsec('MAPSEC_SEIKA_LANTERNPORT', 'LANTERNPORT')
SEC_ROAD = mapsec('MAPSEC_SEIKA_LANTERN_ROAD', 'LANTERN ROAD')

BADGE = flag('FLAG_QOL_SEIKA_BADGE01', 'LANTERN BADGE (Wick)')
MART_ITEMS = ['ITEM_ULTRA_BALL', 'ITEM_GREAT_BALL', 'ITEM_DUSK_BALL', 'ITEM_HYPER_POTION', 'ITEM_MAX_POTION',
              'ITEM_FULL_RESTORE', 'ITEM_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL', 'ITEM_FIRE_STONE']

P = 'Seika_Lanternport'


def town():
    c = g.Canvas(40, 34, 'gTileset_VermilionCity', seed=11)
    # forests (thick, so the border never shows past them)
    c.forest(0, 0, 18, 6)
    c.forest(22, 0, 18, 6)
    c.forest(0, 6, 8, 18)
    c.forest(32, 6, 8, 18)
    c.rect(0, 24, 40, 4, 'beach')
    c.rect(0, 28, 40, 6, 'sea')
    # streets: beach sand, like Vermilion's, so they blend into the shore
    c.rect(19, 0, 2, 11, 'beach')
    c.rect(10, 11, 20, 5, 'beach')      # festival square
    c.rect(19, 16, 2, 4, 'beach')
    c.rect(9, 20, 22, 2, 'beach')
    c.rect(19, 22, 2, 2, 'beach')
    c.rect(15, 7, 3, 3, 'flowers')
    c.rect(22, 7, 2, 3, 'flowers')
    c.rect(26, 22, 4, 1, 'flowers')
    # buildings (Vermilion City's)
    for st, x, y in ((PC, 9, 6), (GYM, 25, 6), (MART, 27, 16)):
        c.stamp(*st[:5], x, y)            # doors: PC (11,10), Gym (28,10), Mart (29,19)
    c.stamp(VERM, 10, 23, 1, 2, 24, 9)    # gym sign
    c.stamp(VERM, 27, 21, 4, 4, 9, 16)    # house 1, door (10,19)
    c.stamp(VERM, 27, 21, 4, 4, 13, 16)   # Lantern Hall, door (14,19)
    c.stamp(VERM, 27, 21, 4, 4, 22, 16)   # house 2, door (23,19)
    c.put(21, 9, g.SIGN)
    c.put(13, 20, g.SIGN)
    g.pier(c, 19, 25, 3, 5)
    return c


def build():
    t = add(g.MapDef(P, 'MAP_SEIKA_LANTERNPORT', SEC, town(), 'MUS_RG_SEVII_123', 'MAP_TYPE_TOWN',
                     border=g.SEA_BORDER))
    w_pc = door(t, 11, 10, 'MAP_SEIKA_LANTERNPORT_POKEMON_CENTER_1F', 0)
    w_gym = door(t, 28, 10, 'MAP_SEIKA_LANTERNPORT_GYM', 0)
    w_mart = door(t, 29, 19, 'MAP_SEIKA_LANTERNPORT_MART', 0)
    w_h1 = door(t, 10, 19, 'MAP_SEIKA_LANTERNPORT_HOUSE1', 0)
    w_hall = door(t, 14, 19, 'MAP_SEIKA_LANTERNPORT_LANTERN_HALL', 0)
    w_h2 = door(t, 23, 19, 'MAP_SEIKA_LANTERNPORT_HOUSE2', 0)

    signpost(t, 21, 9, P + '_EventScript_TownSign',
             'LANTERNPORT\n'
             'Where lanterns light the way\n'
             'for falling stars.')
    signpost(t, 24, 10, P + '_EventScript_GymSign',
             'LANTERNPORT POKéMON GYM\n'
             'LEADER: WICK\fThe lantern maker whose\n'
             'flame never goes out!')
    signpost(t, 13, 20, P + '_EventScript_HallSign',
             'LANTERN HALL\n'
             'Starwatch Festival office')
    npc(t, 'OBJ_EVENT_GFX_OLD_MAN_1', 17, 13, P + '_EventScript_Elder',
        'Welcome to SEIKA, traveler.\fA thousand years ago, the first\n'
        'STARFALL came down over this land.\fOur ancestors wrote that one small\n'
        'star fell on MT. SKYREACH and\n'
        'went to sleep there.\fThe WISHING STAR, they called it.\n'
        'It wakes for seven days every\n'
        'thousand years.\fWe light lanterns so it can find\n'
        'its way. This year... the sky\n'
        'fell again. Perhaps it is awake.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_LITTLE_GIRL_FRLG', 23, 13, P + '_EventScript_LanternGirl',
        'Every night of the festival we\n'
        'send lanterns up into the sky!\fWICK makes all of them. His\n'
        'lanterns never, ever go out!', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_FISHER', 12, 25, P + '_EventScript_Fisher',
        'The CHINCHOU out here light up\n'
        'the sea at night.\fThat\'s how this town got its\n'
        'name, if you ask me.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_WOMAN_1_FRLG', 25, 22, P + '_EventScript_Woman',
        'LANTERN ROAD goes north to\n'
        'INKBRUSH TOWN.\fThe scribes there keep the oldest\n'
        'record of the STARFALL.', 'MOVEMENT_TYPE_WANDER_AROUND')
    t.objects.append(g.obj('OBJ_EVENT_GFX_SAILOR_FRLG', 20, 29, P + '_EventScript_Sailor',
                           move='MOVEMENT_TYPE_FACE_UP', local_id='LOCALID_SEIKA_LANTERNPORT_SAILOR'))
    t.objects.append(g.obj('OBJ_EVENT_GFX_SS_TIDAL', 20, 31, '0x0', move='MOVEMENT_TYPE_FACE_RIGHT', elevation=1))
    t.scripts += (P + '_EventScript_Sailor::\n'
                  '\tgoto_if_set FLAG_QOL_SKYRIFT_CHOSEN, QoL_EventScript_LanternportSailor @ skyrift_story.py\n'
                  '\tlock\n\tfaceplayer\n'
                  '\tmsgbox QoL_Text_OfferKanto, MSGBOX_YESNO\n'
                  '\tgoto_if_eq VAR_RESULT, YES, QoL_EventScript_SailToKanto\n'
                  '\tmsgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT\n\trelease\n\tend\n\n')

    pc = pokecenter('Lanternport', SEC, t, w_pc, 'HEAL_LOCATION_SEIKA_LANTERNPORT', npcs=[
        ('OBJ_EVENT_GFX_YOUNGSTER_FRLG', 11, 5,
         'POKéMON in SEIKA are strong.\n'
         'The sea air makes them tough!'),
    ])
    # the Shiny Painter (data/scripts/qol_shiny.inc), also in Viridian City
    pc.objects.append(g.obj('OBJ_EVENT_GFX_ARTIST', 14, 3, 'QoL_EventScript_ShinyPainter'))
    mart('Lanternport', SEC, t, w_mart, MART_ITEMS, npcs=[
        ('OBJ_EVENT_GFX_GENTLEMAN_FRLG', 6, 4,
         'A FIRE STONE for the festival\n'
         'bonfire? Why not!'),
    ])
    house('Lanternport', 1, SEC, t, w_h1, 'MUS_RG_SEVII_123', [
        ('OBJ_EVENT_GFX_WOMAN_2_FRLG', 6, 3,
         'My grandfather painted the old\n'
         'star on our door, for luck.\fThey say it looks like a little\n'
         'person with a long tail.'),
        ('OBJ_EVENT_GFX_LITTLE_BOY_FRLG', 3, 4,
         'When I grow up I want to catch\n'
         'the WISHING STAR!', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ])
    house('Lanternport', 2, SEC, t, w_h2, 'MUS_RG_SEVII_123', [
        ('OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 4, 3,
         'Last week a big ship came by\n'
         'from KANTO with strange people.\fA man with dirt on his gloves.\n'
         'A woman in a fine coat.\fThey asked the way to INKBRUSH,\n'
         'not to the festival. Odd folk.'),
    ])
    hall = add(g.MapDef(P + '_LanternHall', 'MAP_SEIKA_LANTERNPORT_LANTERN_HALL', SEC,
                        'LAYOUT_VERMILION_CITY_POKEMON_FAN_CLUB', 'MUS_RG_SEVII_123', 'MAP_TYPE_INDOOR',
                        'WEATHER_NONE', False, False, False))
    for k in (4, 5, 6):
        door(hall, k, 10, t.id, w_hall)
    npc(hall, 'OBJ_EVENT_GFX_GENTLEMAN_FRLG', 5, 4, P + '_LanternHall_EventScript_Chair',
        'I chair the STARWATCH FESTIVAL.\fEvery town in SEIKA keeps a star\n'
        'shrine. The shrine keepers are\n'
        'our GYM LEADERS.\fWin all eight BADGES and the\n'
        'STARWATCH PLATEAU will open to\n'
        'you, below MT. SKYREACH.')
    npc(hall, 'OBJ_EVENT_GFX_WORKER_F', 6, 6, P + '_LanternHall_EventScript_Worker',
        'We fold a thousand paper\n'
        'lanterns before the festival.\fMy fingers are so tired!', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(hall, 'OBJ_EVENT_GFX_FAT_MAN_FRLG', 3, 6, P + '_LanternHall_EventScript_Man',
        'The old songs say the WISHING\n'
        'STAR grants one wish when it\n'
        'wakes.\fWhat would you wish for?',)

    gym(t, w_gym)
    road(t)


def gym(t, w_gym):
    src = 'LAYOUT_CINNABAR_ISLAND_GYM'
    W, H = 15, 18
    r = g.Room(W, H, 'gTileset_BuildingFrlg', 'gTileset_CinnabarGym', source=src)
    r.fill(0, 0, W, H, r.at(2, 4))                     # orange floor
    r.fill(0, 0, W, 1, r.at(21, 0))                    # wall
    r.fill(0, 1, W, 1, r.at(21, 1))
    r.fill(0, 2, W, 1, r.at(3, 2))                     # shadow under the wall
    r.fill(0, 3, W, 1, r.at(3, 3))
    for x in (0, W - 1):
        r.set(x, 0, r.at(0, 0))
        r.set(x, 1, r.at(0, 1))
        for y in range(2, H - 3):
            r.set(x, y, r.at(0, 5))
        r.set(x, H - 3, r.at(0, 22))
        r.set(x, H - 2, r.at(0, 23))
    for y in range(2, H - 1):
        r.set(1, y, r.at(1, 4))
    r.fill(0, H - 1, W, 1, r.at(20, 24))
    mx = W // 2
    for i, sx in enumerate((24, 25, 26)):
        r.set(mx - 1 + i, H - 2, r.at(sx, 23))
        r.set(mx - 1 + i, H - 1, r.at(sx, 24))
    r.copy(1, 1, 2, 2, 3, 1)              # machines on the back wall
    r.copy(1, 1, 2, 2, 10, 1)
    r.copy(4, 3, 3, 2, mx - 1, 4)         # rug in front of the leader
    r.copy(23, 19, 2, 3, mx - 3, H - 5)   # statues by the entrance
    r.copy(23, 19, 2, 3, mx + 2, H - 5)
    r.copy(23, 19, 2, 3, 3, 10)
    r.copy(23, 19, 2, 3, 10, 10)
    name = P + '_Gym'
    m = add(g.MapDef(name, 'MAP_SEIKA_LANTERNPORT_GYM', SEC, r, 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (mx - 1, mx, mx + 1):
        door(m, k, H - 2, t.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_COOLTRAINER_M', mx, 3, name + '_EventScript_Wick', move='MOVEMENT_TYPE_FACE_DOWN'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', mx - 2, H - 4, name + '_EventScript_GymGuy'))
    trainer('TRAINER_SEIKA_KINDLER_HOMURA', 'HOMURA', 'Kindler', 'Kindler', 'Male', '''
Magmar
Level: 48
- Flamethrower
- Thunder Punch
- Cross Chop
- Confuse Ray

Camerupt
Level: 48
- Earth Power
- Flamethrower
- Yawn
- Rock Slide
''')
    trainer('TRAINER_SEIKA_KINDLER_ENKA', 'ENKA', 'Kindler', 'Kindler', 'Male', '''
Rapidash
Level: 49
- Flare Blitz
- Megahorn
- Bounce
- Wild Charge

Torkoal
Level: 49
- Lava Plume
- Body Press
- Stealth Rock
- Yawn
''')
    trainer('TRAINER_SEIKA_LEADER_WICK', 'WICK', 'Leader Frlg', 'Kindler', 'Male', '''
Houndoom
Level: 50
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Dark Pulse
- Flamethrower
- Nasty Plot
- Sludge Bomb

Ninetales @ Charcoal
Level: 50
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Flamethrower
- Extrasensory
- Will-O-Wisp
- Hypnosis

Magcargo
Level: 50
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Lava Plume
- Power Gem
- Earth Power
- Shell Smash

Typhlosion @ Sitrus Berry
Level: 52
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Eruption
- Focus Blast
- Extrasensory
- Solar Beam
''', music='Male', ai='Smart Trainer')
    m.objects.append(g.obj('OBJ_EVENT_GFX_BLACK_BELT_FRLG', 4, 9, name + '_EventScript_Homura',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=4))
    m.objects.append(g.obj('OBJ_EVENT_GFX_BLACK_BELT_FRLG', 10, 7, name + '_EventScript_Enka',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=4))
    m.scripts += battle(name + '_EventScript_Homura', 'TRAINER_SEIKA_KINDLER_HOMURA',
                        'WICK\'s fire burned for a thousand\nnights. Mine burns for him!',
                        'My flame... flickered out.',
                        'WICK lights every lantern in town\nwith one breath. Really!', name)
    m.scripts += battle(name + '_EventScript_Enka', 'TRAINER_SEIKA_KINDLER_ENKA',
                        'Too close to the fire? Too bad!',
                        'Whoa, I got burned!',
                        'WICK opens with dark fire. Watch\nout for his HOUNDOOM!', name)
    m.scripts += (name + '_EventScript_Wick::\n'
                  '\ttrainerbattle_single TRAINER_SEIKA_LEADER_WICK, %s_Text_WickIntro, %s_Text_WickDefeat, %s_EventScript_WickDefeated, NO_MUSIC\n'
                  '\tmsgbox %s_Text_WickPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%s_EventScript_WickDefeated::\n'
                  '\tsetflag %s\n'
                  '\tmsgbox %s_Text_WickGift, MSGBOX_DEFAULT\n'
                  '\tgiveitem ITEM_TM_FLAMETHROWER\n'
                  '\tmsgbox %s_Text_WickPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % (name, name, name, name, name, BADGE, name, name))
    m.scripts += T(name + '_Text_WickIntro',
                   'So the traveler from KANTO has\n'
                   'come to my shrine.\fI am WICK. I make the lanterns\n'
                   'that guide the WISHING STAR home.\fA lantern that fears the wind is\n'
                   'no lantern at all! Show me your\n'
                   'fire!')
    m.scripts += T(name + '_Text_WickDefeat',
                   'Ha! My flame bows to yours.\fTake the LANTERN BADGE. Let it\n'
                   'light your way through SEIKA.\f'
                   '{FONT_NORMAL}{PLAYER} received the LANTERN BADGE\n'
                   'from WICK!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}')
    m.scripts += T(name + '_Text_WickGift', 'And take this, too. A flame that\nnever goes out.')
    m.scripts += T(name + '_Text_WickPost',
                   'Go north to INKBRUSH TOWN.\fSUMI keeps the STARFALL SCROLL\n'
                   'there. If anyone knows where the\n'
                   'WISHING STAR sleeps, she does.')
    m.scripts += (name + '_EventScript_GymGuy::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_set %s, %s_EventScript_GymGuyWon\n'
                  '\tmsgbox %s_Text_GymGuy, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%s_EventScript_GymGuyWon::\n\tmsgbox %s_Text_GymGuyWon, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % (BADGE, name, name, name, name))
    m.scripts += T(name + '_Text_GymGuy',
                   'Yo! Welcome to SEIKA\'s first GYM!\fWICK burns hot. WATER, GROUND\n'
                   'and ROCK moves cool him down.\fHis TYPHLOSION can erupt at full\n'
                   'health, so hit it hard and fast!')
    m.scripts += T(name + '_Text_GymGuyWon', 'That was blazing! One BADGE down,\nseven to go!')


def road(t):
    c = g.Canvas(24, 50, 'gTileset_PalletTown', seed=12)
    c.forest(0, 0, 14, 4)                 # north end: opening to Inkbrush Town
    c.forest(20, 0, 4, 4)
    c.rect(16, 0, 2, 4, 'path')
    c.forest(0, 4, 4, 40)
    c.forest(20, 4, 4, 40)
    c.forest(0, 44, 10, 6)
    c.forest(14, 44, 10, 6)
    c.rect(11, 38, 2, 12, 'path')
    c.rect(6, 36, 7, 2, 'path')
    c.rect(6, 20, 2, 16, 'path')
    c.rect(6, 18, 12, 2, 'path')
    c.rect(16, 4, 2, 14, 'path')
    c.rect(13, 25, 6, 9, 'tall')
    c.rect(8, 7, 7, 9, 'tall')
    c.rect(4, 39, 6, 5, 'tall')
    c.rect(8, 27, 4, 3, 'flowers')
    c.ledge(8, 31, 5)
    c.pond(14, 38, 5, 3)
    c.tree(9, 21)
    c.tree(11, 21)
    c.tree(18, 8)
    c.tree(4, 14)
    name = 'Seika_LanternRoad'
    m = add(g.MapDef(name, 'MAP_SEIKA_LANTERN_ROAD', SEC_ROAD, c, 'MUS_RG_SEVII_ROUTE', 'MAP_TYPE_ROUTE'))
    t.connect(m, 'up', 8)
    link('MAP_SEIKA_LANTERN_ROAD', 'up', -4, 'MAP_SEIKA_INKBRUSH')  # road path x16-17 = town x20-21
    signpost(m, 13, 46, name + '_EventScript_Sign', 'LANTERN ROAD\nLANTERNPORT - INKBRUSH TOWN')
    c.put(13, 46, g.SIGN)
    trainer('TRAINER_SEIKA_HIKER_GORO', 'GORO', 'Hiker Frlg', 'Hiker Frlg', 'Male', '''
Graveler
Level: 45
- Rock Slide
- Earthquake
- Explosion
- Rock Polish

Sudowoodo
Level: 46
- Wood Hammer
- Head Smash
- Sucker Punch
- Low Kick
''')
    trainer('TRAINER_SEIKA_LASS_MIYU', 'MIYU', 'Lass Frlg', 'Lass Frlg', 'Female', '''
Granbull
Level: 45
- Play Rough
- Crunch
- Thunder Fang
- Bulk Up

Jumpluff
Level: 45
- Acrobatics
- Leech Seed
- Sleep Powder
- U-turn

Furret
Level: 46
- Double-Edge
- Sucker Punch
- Knock Off
- Coil
''', music='Female')
    trainer('TRAINER_SEIKA_BIRD_KEEPER_TOBI', 'TOBI', 'Bird Keeper Frlg', 'Bird Keeper Frlg', 'Male', '''
Noctowl
Level: 46
- Air Slash
- Hypnosis
- Extrasensory
- Roost

Xatu
Level: 46
- Psychic
- Air Slash
- Confuse Ray
- Heat Wave

Skarmory
Level: 47
- Steel Wing
- Drill Peck
- Spikes
- Roost
''')
    m.objects.append(g.obj('OBJ_EVENT_GFX_HIKER_FRLG', 9, 37, name + '_EventScript_Goro',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_LASS_FRLG', 8, 24, name + '_EventScript_Miyu',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_YOUNGSTER_FRLG', 15, 12, name + '_EventScript_Tobi',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.scripts += battle(name + '_EventScript_Goro', 'TRAINER_SEIKA_HIKER_GORO',
                        'These hills are older than any\nbook. Same goes for my rocks!',
                        'You crumbled my rocks!',
                        'MT. SKYREACH is the tallest peak\nin SEIKA. Clouds sit on top.', name)
    m.scripts += battle(name + '_EventScript_Miyu', 'TRAINER_SEIKA_LASS_MIYU',
                        'I came to see the lanterns. And\nto battle someone from KANTO!',
                        'KANTO trainers are amazing!',
                        'I\'ll catch a HOPPIP and float up\nto the stars one day.', name)
    m.scripts += battle(name + '_EventScript_Tobi', 'TRAINER_SEIKA_BIRD_KEEPER_TOBI',
                        'My birds watched the STARFALL\nall night long!',
                        'Grounded!',
                        'XATU stare at the sun all day.\nThey say they can see the future.', name)
    wild('MAP_SEIKA_LANTERN_ROAD', 'gSeika_LanternRoad',
         land=[('FURRET', 42, 44), ('NOCTOWL', 42, 44), ('FLAAFFY', 42, 43), ('SKIPLOOM', 42, 43),
               ('LEDIAN', 43, 44), ('ARIADOS', 43, 44), ('AIPOM', 43, 45), ('STANTLER', 43, 45),
               ('GIRAFARIG', 44, 44), ('SUNFLORA', 44, 44), ('AMPHAROS', 46, 46), ('AMBIPOM', 46, 46)],
         water=[('MARILL', 40, 42), ('WOOPER', 40, 42), ('AZUMARILL', 42, 45), ('QUAGSIRE', 42, 45),
                ('POLITOED', 45, 45)],
         fishing=[('MAGIKARP', 40, 42), ('GOLDEEN', 40, 42), ('POLIWHIRL', 40, 44), ('SEAKING', 42, 44),
                  ('QUAGSIRE', 42, 44), ('GYARADOS', 42, 46), ('SEAKING', 44, 46), ('POLIWRATH', 44, 46),
                  ('AZUMARILL', 44, 46), ('GYARADOS', 46, 48)])
    wild('MAP_SEIKA_LANTERNPORT', 'gSeika_Lanternport',
         water=[('TENTACRUEL', 40, 44), ('LANTURN', 40, 44), ('PELIPPER', 42, 44), ('MANTINE', 42, 45),
                ('LANTURN', 45, 46)],
         fishing=[('MAGIKARP', 40, 42), ('CHINCHOU', 40, 42), ('REMORAID', 40, 44), ('CHINCHOU', 42, 44),
                  ('CORSOLA', 42, 44), ('LANTURN', 42, 46), ('OCTILLERY', 44, 46), ('QWILFISH', 44, 46),
                  ('MANTINE', 44, 46), ('GYARADOS', 46, 48)])
