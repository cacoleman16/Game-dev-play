"""Duskpeak Village (Gym 8, Ghost), Frost Road, Skyreach Pass and
Starwatch Plateau: the last stretch below Mt. Skyreach."""
from common import (g, T, add, door, npc, signpost, pokecenter, mart, house, wild, mapsec,
                    trainer, battle, flag, link, PC, MART, GYM)

LAV = 'LAYOUT_LAVENDER_TOWN'
INDIGO = 'LAYOUT_INDIGO_PLATEAU_EXTERIOR'
R23 = 'LAYOUT_ROUTE23'
SEC = mapsec('MAPSEC_SEIKA_DUSKPEAK', 'DUSKPEAK VILLAGE')
SEC_FROST = mapsec('MAPSEC_SEIKA_FROST_ROAD', 'FROST ROAD')
SEC_PASS = mapsec('MAPSEC_SEIKA_SKYREACH_PASS', 'SKYREACH PASS')
SEC_PLATEAU = mapsec('MAPSEC_SEIKA_STARWATCH_PLATEAU', 'STARWATCH')  # names are 16 chars at most

BADGE = flag('FLAG_QOL_SEIKA_BADGE08', 'DUSK BADGE (Tomoe)')
CHAMPION = flag('FLAG_QOL_SEIKA_CHAMPION', 'beat Seiji: Champion of the Seika League')
BADGES = ['FLAG_QOL_SEIKA_BADGE%02d' % i for i in range(1, 9)]

MART_ITEMS = ['ITEM_ULTRA_BALL', 'ITEM_DUSK_BALL', 'ITEM_QUICK_BALL', 'ITEM_HYPER_POTION', 'ITEM_MAX_POTION',
              'ITEM_FULL_RESTORE', 'ITEM_REVIVE', 'ITEM_MAX_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL',
              'ITEM_ESCAPE_ROPE', 'ITEM_DUSK_STONE']

P = 'Seika_Duskpeak'
FROST = 'Seika_FrostRoad'
PASS = 'Seika_SkyreachPass'
PLATEAU = 'Seika_StarwatchPlateau'

# FireRed's mountain blocks (primary tileset, so they work on any map)
ROCK_TOP = (0x068, 0x069, 0x06A)       # rim: top-left, top, top-right
ROCK_IN_TOP = (0x06B, 0x06C, 0x06D)    # inner rim under it
ROCK_MID = (0x070, 0x073, 0x071, 0x075, 0x072)   # left rim, inner left, rock, inner right, right rim
ROCK_IN_BOT = (0x07B, 0x07C, 0x07D)
ROCK_BOT = (0x078, 0x079, 0x07A)       # the cliff face at the bottom
STAIRS = (0x091, 0x089)                # stone steps cut into a cliff (left, right)
BOULDER = 0x09F
CAVE = 0x0A9                           # cave mouth (a door)


# ---------------------------------------------------------------- painting helpers

def _fix(c, x, y, mid):
    if 0 <= x < c.w and 0 <= y < c.h:
        c.put(x, y, mid)
        c.cls[y][x] = 'fixed'
        c._untree(x, y)


def rock(c, x, y, w, h):
    """A free-standing mountain block (Route 23 style): a rim, an inner rim
    and rock in the middle. h == 2 gives a low cliff band; w, h >= 2."""
    for yy in range(h):
        for xx in range(w):
            side = 0 if xx == 0 else 2 if xx == w - 1 else 1
            if yy == 0:
                m = ROCK_TOP[side]
            elif yy == h - 1:
                m = ROCK_BOT[side]
            elif xx == 0:
                m = ROCK_MID[0]
            elif xx == w - 1:
                m = ROCK_MID[4]
            else:
                inner = 0 if xx == 1 else 2 if xx == w - 2 else 1
                if yy == 1:
                    m = ROCK_IN_TOP[inner]
                elif yy == h - 2:
                    m = ROCK_IN_BOT[inner]
                else:
                    m = ROCK_MID[1 + inner]
            _fix(c, x + xx, y + yy, m)


def stairs(c, x, y, h):
    """Two-wide stone steps up a cliff band, h rows tall."""
    for yy in range(h):
        _fix(c, x, y + yy, STAIRS[0])
        _fix(c, x + 1, y + yy, STAIRS[1])


def boulders(c, pts):
    for x, y in pts:
        _fix(c, x, y, BOULDER)


# ---------------------------------------------------------------- Frost Road

def frost_road():
    c = g.Canvas(24, 50, 'gTileset_PalletTown', seed=81)
    c.forest(0, 0, 8, 4)                  # north end: opening to Duskpeak
    c.forest(14, 0, 10, 4)
    c.forest(0, 4, 4, 42)
    c.forest(20, 4, 4, 42)
    c.forest(0, 46, 8, 4)                 # south end: opening to Frostmere
    c.forest(14, 46, 10, 4)
    # the road: up from Frostmere, west round the lake, east, north
    c.rect(10, 40, 2, 10, 'path')
    c.rect(5, 38, 7, 2, 'path')
    c.rect(5, 22, 2, 16, 'path')
    c.rect(5, 20, 11, 2, 'path')
    c.rect(14, 8, 2, 12, 'path')
    c.rect(10, 6, 6, 2, 'path')
    c.rect(10, 0, 2, 6, 'path')
    # tall grass
    c.rect(13, 30, 7, 7, 'tall')
    c.rect(8, 24, 5, 7, 'tall')
    c.rect(4, 9, 8, 8, 'tall')
    c.rect(16, 14, 4, 5, 'tall')
    # a cold lake by the south bend
    c.pond(13, 39, 6, 4)
    # rocks and trees
    rock(c, 16, 22, 4, 5)
    boulders(c, [(13, 23), (7, 41), (17, 9), (4, 18), (12, 44)])
    c.tree(8, 34)
    c.tree(16, 4)
    c.tree(18, 4)
    c.tree(4, 4)
    c.tree(6, 4)
    c.ledge(7, 32, 6)
    c.rect(4, 40, 2, 4, 'flowers')
    return c


# ---------------------------------------------------------------- Duskpeak Village

def village():
    c = g.Canvas(32, 28, 'gTileset_LavenderTown', seed=88)
    c.forest(0, 0, 14, 4)
    c.forest(20, 0, 12, 4)
    c.forest(0, 4, 8, 20)
    c.forest(24, 4, 8, 20)
    c.forest(0, 24, 14, 4)
    c.forest(20, 24, 12, 4)
    # streets
    c.rect(16, 0, 2, 28, 'path')
    c.rect(9, 9, 14, 2, 'path')
    c.rect(9, 15, 14, 2, 'path')
    c.rect(16, 21, 8, 2, 'path')
    # buildings: Gym (8,4) door (11,8); PC (19,4) door (21,8); Mart (20,17) door (22,20)
    for st, x, y in ((GYM, 8, 4), (PC, 19, 4), (MART, 20, 17)):
        c.stamp(*st[:5], x, y)
    c.stamp(LAV, 3, 13, 5, 4, 8, 11)      # house 1, door (10,14)
    c.stamp(LAV, 3, 13, 5, 4, 19, 11)     # house 2, door (21,14)
    c.put(15, 8, g.SIGN)                  # gym sign
    c.put(15, 22, g.SIGN)                 # town sign
    # the keepers' pond: still water ringed by standing stones
    c.pond(9, 18, 5, 3)
    boulders(c, [(8, 18), (8, 20), (14, 18), (14, 20)])
    c.rect(9, 17, 5, 1, 'flowers')
    c.put(14, 17, g.SIGN)
    c.tree(13, 12)
    c.rect(18, 12, 1, 2, 'flowers')
    return c


# ---------------------------------------------------------------- Skyreach Pass

def skyreach_pass():
    c = g.Canvas(24, 60, 'gTileset_PalletTown', seed=97)
    c.forest(0, 0, 8, 4)                  # north end: opening to Starwatch Plateau
    c.forest(14, 0, 10, 4)
    c.forest(0, 4, 4, 52)
    c.forest(20, 4, 4, 52)
    c.forest(0, 56, 8, 4)                 # south end: opening to Duskpeak
    c.forest(14, 56, 10, 4)
    # the guard's gate: a 2-wide cut between two rocks
    rock(c, 4, 48, 6, 5)
    rock(c, 12, 48, 8, 5)
    c.rect(10, 44, 2, 16, 'path')
    c.rect(10, 44, 7, 2, 'path')
    # first cliff, steps on the right
    rock(c, 4, 41, 16, 3)
    stairs(c, 15, 41, 3)
    # the climb: ledges you walk round going up and hop going down
    c.rect(4, 39, 7, 2, 'tall')
    c.ledge(8, 38, 12)
    c.rect(4, 36, 4, 2, 'tall')
    c.ledge(4, 35, 12)
    c.rect(12, 36, 6, 2, 'tall')
    c.rect(16, 30, 4, 5, 'tall')
    c.ledge(4, 32, 4)
    rock(c, 9, 31, 5, 4)
    # second cliff, steps on the left
    rock(c, 4, 28, 16, 2)
    stairs(c, 5, 28, 2)
    c.rect(5, 23, 2, 5, 'path')
    c.rect(5, 21, 9, 2, 'path')
    c.rect(8, 23, 6, 5, 'tall')
    rock(c, 14, 19, 6, 7)
    rock(c, 4, 17, 3, 3)
    # third cliff, steps in the middle
    rock(c, 4, 14, 16, 3)
    stairs(c, 12, 14, 3)
    c.rect(12, 8, 2, 6, 'path')
    c.rect(10, 6, 4, 2, 'path')
    c.rect(10, 0, 2, 6, 'path')
    c.rect(14, 6, 6, 7, 'tall')
    rock(c, 4, 5, 5, 5)
    boulders(c, [(7, 45), (18, 46), (14, 39), (7, 33), (8, 19), (10, 11), (5, 12), (18, 27), (14, 26)])
    return c


# ---------------------------------------------------------------- Starwatch Plateau

def plateau():
    c = g.Canvas(24, 24, 'gTileset_IndigoPlateau', seed=99)
    c.forest(0, 0, 6, 18)
    c.forest(0, 18, 8, 6)                 # south end: opening to Skyreach Pass
    c.forest(14, 18, 10, 6)
    # the SEIKA LEAGUE: Indigo Plateau's building with its statues and square
    c.stamp(INDIGO, 6, 0, 11, 12, 6, 0)   # door (11,6)
    c.stamp(INDIGO, 10, 12, 3, 1, 10, 12)  # end of the stone walk
    # Mt. Skyreach: a cliff with Victory Road's cave mouth and its guardian statues
    rock(c, 17, 0, 7, 8)
    c.stamp(R23, 3, 28, 5, 2, 18, 7)      # cave (20,7)
    for y in range(9, 12):
        c.stamp(R23, 4, 30, 3, 1, 19, y)
    c.stamp(INDIGO, 10, 12, 3, 1, 19, 12)
    # sand paths that run on from the two stone walks
    c.rect(10, 13, 12, 2, 'path')
    c.rect(10, 15, 2, 9, 'path')
    for x in (10, 11, 12, 19, 20, 21):
        c.cls[12][x] = 'path'
    c.tree(22, 9)
    c.tree(22, 11)
    c.tree(22, 13)
    c.tree(22, 15)
    c.rect(17, 16, 4, 2, 'flowers')
    return c


# ---------------------------------------------------------------- the Gym: a dim shrine hall

TOWER = 'LAYOUT_POKEMON_TOWER_1F'
GW, GH = 15, 20


def gym_room():
    """Pokemon Tower's purple panels, green floor and grave markers, laid
    out as a shrine hall: a lit circle before the medium, statues at her
    sides, and rows of stones to weave through."""
    r = g.Room(GW, GH, 'gTileset_BuildingFrlg', 'gTileset_PokemonTower', source=TOWER)
    t5 = g.Room(1, 1, 'gTileset_BuildingFrlg', 'gTileset_PokemonTower', source='LAYOUT_POKEMON_TOWER_5F')
    t7 = g.Room(1, 1, 'gTileset_BuildingFrlg', 'gTileset_PokemonTower', source='LAYOUT_POKEMON_TOWER_7F')
    VOID, FLOOR = r.at(0, 0), r.at(10, 10)
    SH_TOP, SH_LEFT, SH_CORNER, SH_DIAG = r.at(10, 2), r.at(2, 9), r.at(2, 8), r.at(9, 3)
    RIM, PANEL = r.at(10, 0), r.at(10, 1)
    r.fill(0, 0, GW, GH, VOID)
    r.fill(1, 2, GW - 2, GH - 4, FLOOR)
    # back wall, one step down at each end (the tower's own corner)
    r.fill(2, 0, GW - 4, 1, RIM)
    r.fill(2, 1, GW - 4, 1, PANEL)
    r.set(1, 0, r.at(8, 0)); r.set(1, 1, r.at(8, 1)); r.set(1, 2, PANEL)
    r.set(GW - 2, 0, r.at(14, 0)); r.set(GW - 2, 1, r.at(14, 1)); r.set(GW - 2, 2, PANEL)
    # shadows under the wall
    r.fill(3, 2, GW - 6, 1, SH_TOP)
    r.set(2, 2, r.at(9, 2)); r.set(GW - 3, 2, SH_TOP)
    r.set(1, 3, SH_CORNER); r.set(2, 3, SH_DIAG); r.set(GW - 2, 3, SH_TOP)
    for y in range(4, 11):
        r.set(1, y, SH_LEFT)
    # rounded lower corners, and the exit mat in a notch of the front wall
    for x, y in ((1, GH - 4), (GW - 2, GH - 4), (1, GH - 3), (2, GH - 3), (GW - 2, GH - 3), (GW - 3, GH - 3)):
        r.set(x, y, VOID)
    mx = GW // 2
    r.copy(10, 18, 3, 2, mx - 1, GH - 2)
    # the medium's dais: statues on pillars and the lit circle
    for x, head in ((mx - 2, t7.src[2][9]), (mx + 2, t7.src[2][13])):
        r.set(x, 2, head)
        r.set(x, 3, t7.src[3][9])
        r.set(x, 4, t7.src[4][9])
    for yy in range(3):
        for xx in range(3):
            r.set(mx - 1 + xx, 5 + yy, t5.src[8 + yy][10 + xx])
    # rows of stones
    GRAVE, GOLD = t7.src[9][3], t7.src[6][5]
    stones = []
    stones += [(x, 13) for x in range(1, 10)] + [(x, 14) for x in range(1, 10)]
    stones += [(x, 9) for x in range(5, 14)] + [(x, 10) for x in range(5, 14)]
    stones += [(1, 6), (2, 6), (12, 6), (13, 6), (1, 7), (13, 7)]
    for x, y in stones:
        r.set(x, y, GOLD if (x + y) % 5 == 0 else GRAVE)
    return r




# ---------------------------------------------------------------- scripts

def sign_at(m, x, y, label):
    """Another sign event that runs an existing sign script."""
    m.bgs.append(g.sign(x, y, label, 'BG_EVENT_PLAYER_FACING_ANY'))


def build():
    road = add(g.MapDef(FROST, 'MAP_SEIKA_FROST_ROAD', SEC_FROST, frost_road(), 'MUS_ROUTE120', 'MAP_TYPE_ROUTE',
                        weather='WEATHER_SNOW'))
    link('MAP_SEIKA_FROSTMERE', 'up', 6, 'MAP_SEIKA_FROST_ROAD')    # Frostmere's path x16-17 = road x10-11
    link('MAP_SEIKA_FROST_ROAD', 'up', -6, 'MAP_SEIKA_DUSKPEAK')     # road x10-11 = village x16-17
    t = add(g.MapDef(P, 'MAP_SEIKA_DUSKPEAK', SEC, village(), 'MUS_MT_PYRE_EXTERIOR', 'MAP_TYPE_TOWN',
                     weather='WEATHER_SHADE'))
    sky = add(g.MapDef(PASS, 'MAP_SEIKA_SKYREACH_PASS', SEC_PASS, skyreach_pass(), 'MUS_RG_VICTORY_ROAD',
                       'MAP_TYPE_ROUTE'))
    link('MAP_SEIKA_DUSKPEAK', 'up', 6, 'MAP_SEIKA_SKYREACH_PASS')   # village x16-17 = pass x10-11
    top = add(g.MapDef(PLATEAU, 'MAP_SEIKA_STARWATCH_PLATEAU', SEC_PLATEAU, plateau(), 'MUS_EVER_GRANDE',
                       'MAP_TYPE_TOWN'))
    link('MAP_SEIKA_SKYREACH_PASS', 'up', 0, 'MAP_SEIKA_STARWATCH_PLATEAU')
    build_road(road)
    build_village(t)
    build_pass(sky)
    build_plateau(top)


# ---------------------------------------------------------------- Frost Road

def build_road(m):
    name = FROST
    signpost(m, 9, 45, name + '_EventScript_Sign', 'FROST ROAD\nFROSTMERE - DUSKPEAK VILLAGE')
    m.layout.put(9, 45, g.SIGN)
    trainer('TRAINER_SEIKA_RANGER_FUYUMI', 'FUYUMI', 'Pkmn Ranger Frlg', 'Pokemon Ranger F Frlg', 'Female', '''
Piloswine
Level: 58
- Earthquake
- Icicle Crash
- Stone Edge
- Ice Shard

Jynx
Level: 59
- Ice Beam
- Psychic
- Lovely Kiss
- Nasty Plot

Dewgong
Level: 59
- Waterfall
- Ice Beam
- Aqua Jet
- Rest
''', music='Serious')
    trainer('TRAINER_SEIKA_BLACK_BELT_SHINGO', 'SHINGO', 'Black Belt Frlg', 'Black Belt Frlg', 'Male', '''
Hitmontop
Level: 59
- Close Combat
- Triple Axel
- Sucker Punch
- Rapid Spin

Poliwrath
Level: 59
- Waterfall
- Close Combat
- Ice Punch
- Bulk Up

Heracross
Level: 60
- Megahorn
- Close Combat
- Stone Edge
- Night Slash
''', music='Serious')
    trainer('TRAINER_SEIKA_HIKER_IWAO', 'IWAO', 'Hiker Frlg', 'Hiker Frlg', 'Male', '''
Steelix
Level: 60
- Iron Tail
- Earthquake
- Rock Slide
- Curse

Mamoswine
Level: 61
- Earthquake
- Icicle Crash
- Ice Shard
- Stone Edge
''', music='Serious')
    m.objects.append(g.obj('OBJ_EVENT_GFX_COOLTRAINER_F', 12, 39, name + '_EventScript_Fuyumi',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_BLACK_BELT_FRLG', 7, 28, name + '_EventScript_Shingo',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_HIKER_FRLG', 12, 13, name + '_EventScript_Iwao',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.scripts += battle(name + '_EventScript_Fuyumi', 'TRAINER_SEIKA_RANGER_FUYUMI',
                        'The snow came early this year.\nMy POKéMON love it!',
                        'Brr... That was a cold loss.',
                        'SWINUB dig under the snow for\nfood. Follow one and you might\nfind something!', name)
    m.scripts += battle(name + '_EventScript_Shingo', 'TRAINER_SEIKA_BLACK_BELT_SHINGO',
                        'I train barefoot in the snow!\nHyah!',
                        'My feet are frozen...',
                        'The cold makes the body strong.\nAnd the nose red.', name)
    m.scripts += battle(name + '_EventScript_Iwao', 'TRAINER_SEIKA_HIKER_IWAO',
                        'I climb MT. SKYREACH every year.\nWell, the bottom of it!',
                        'Avalanche!',
                        'They say the summit is above the\nclouds. Only the CHAMPION has\nseen it.', name)
    wild('MAP_SEIKA_FROST_ROAD', 'gSeika_FrostRoad',
         land=[('SWINUB', 58, 59), ('SNEASEL', 58, 59), ('DELIBIRD', 58, 60), ('SNORUNT', 58, 60),
               ('PILOSWINE', 59, 60), ('JYNX', 59, 60), ('WEAVILE', 60, 61), ('GLALIE', 60, 61),
               ('MAMOSWINE', 61, 61), ('FROSLASS', 61, 61), ('SMOOCHUM', 58, 58), ('LAPRAS', 61, 61)],
         water=[('SEEL', 58, 60), ('SHELLDER', 58, 60), ('DEWGONG', 59, 61), ('CLOYSTER', 59, 61),
                ('LAPRAS', 61, 61)],
         fishing=[('MAGIKARP', 55, 57), ('SHELLDER', 55, 57), ('SHELLDER', 56, 58), ('REMORAID', 56, 58),
                  ('SEEL', 57, 59), ('OCTILLERY', 58, 60), ('CLOYSTER', 58, 60), ('DEWGONG', 58, 60),
                  ('GYARADOS', 59, 61), ('LAPRAS', 60, 61)])


# ---------------------------------------------------------------- Duskpeak Village

def build_village(t):
    w_gym = door(t, 11, 8, 'MAP_SEIKA_DUSKPEAK_GYM', 0)
    w_pc = door(t, 21, 8, 'MAP_SEIKA_DUSKPEAK_POKEMON_CENTER_1F', 0)
    w_mart = door(t, 22, 20, 'MAP_SEIKA_DUSKPEAK_MART', 0)
    w_h1 = door(t, 10, 14, 'MAP_SEIKA_DUSKPEAK_HOUSE1', 0)
    w_h2 = door(t, 21, 14, 'MAP_SEIKA_DUSKPEAK_HOUSE2', 0)

    signpost(t, 15, 22, P + '_EventScript_TownSign',
             'DUSKPEAK VILLAGE\n'
             'The last lantern below the peak')
    signpost(t, 15, 8, P + '_EventScript_GymSign',
             'DUSKPEAK POKéMON GYM\n'
             'LEADER: TOMOE\fThe shrine medium who hears\n'
             'the voices of the past!')
    signpost(t, 14, 17, P + '_EventScript_PondSign',
             'KEEPERS\' POND\fFor the shrine keepers, who\n'
             'watched the sky for us.')
    npc(t, 'OBJ_EVENT_GFX_CHANNELER', 13, 9, P + '_EventScript_Channeler',
        'Shh... Can you hear them?\fThe MISDREAVUS sing at dusk,\n'
        'when the sun slips behind\nMT. SKYREACH.\fHere, it is always nearly dusk.',
        'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 12, 22, P + '_EventScript_OldWoman',
        'Eight towns, eight shrines, eight\n'
        'keepers. For a thousand years\nthey watched the sky.\f'
        'The pond keeps their faces.\nOn still nights you can see them.\f'
        'TOMOE is the last keeper. Her\nshrine is the nearest to the star.',
        'MOVEMENT_TYPE_FACE_UP')
    npc(t, 'OBJ_EVENT_GFX_LITTLE_BOY_FRLG', 19, 17, P + '_EventScript_Boy',
        'TOMOE can talk to ghosts!\fShe says the old keepers whisper\n'
        'to her about the WISHING STAR.',
        'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_HIKER_FRLG', 18, 6, P + '_EventScript_Hiker',
        'North is SKYREACH PASS, the climb\n'
        'to STARWATCH PLATEAU.\fThe guard lets nobody by without\n'
        'all eight SEIKA BADGES.',
        'MOVEMENT_TYPE_FACE_LEFT')
    wild('MAP_SEIKA_DUSKPEAK', 'gSeika_Duskpeak',
         fishing=[('MAGIKARP', 55, 57), ('POLIWAG', 55, 57), ('POLIWHIRL', 56, 58), ('WOOPER', 56, 58),
                  ('QUAGSIRE', 57, 59), ('SLOWPOKE', 58, 60), ('POLIWHIRL', 58, 60), ('SLOWBRO', 58, 60),
                  ('GYARADOS', 59, 61), ('SLOWKING', 60, 61)])

    pokecenter('Duskpeak', SEC, t, w_pc, 'HEAL_LOCATION_SEIKA_DUSKPEAK', npcs=[
        ('OBJ_EVENT_GFX_GENTLEMAN_FRLG', 11, 5,
         'I came all the way from KANTO\n'
         'to see the top of MT. SKYREACH.\fOnly the CHAMPION may climb it.\n'
         'Can you believe that?'),
    ])
    mart('Duskpeak', SEC, t, w_mart, MART_ITEMS, npcs=[
        ('OBJ_EVENT_GFX_WOMAN_1_FRLG', 6, 4,
         'DUSK BALLS work best in caves\n'
         'and at night.\fHere in DUSKPEAK it always\n'
         'feels like dusk.'),
    ])
    house('Duskpeak', 1, SEC, t, w_h1, 'MUS_MT_PYRE_EXTERIOR', [
        ('OBJ_EVENT_GFX_OLD_MAN_1', 6, 3,
         'When the new STARFALL lit the sky,\n'
         'the WISHING STAR woke up.\fIts seven days began that night.\n'
         'I have counted them since.\fThere are not many left.'),
        ('OBJ_EVENT_GFX_WOMAN_2_FRLG', 3, 5,
         'My grandmother was a shrine\n'
         'keeper. She said the star sleeps\n'
         'curled up like a child.', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ], layout='LAYOUT_HOUSE1_FRLG')
    house('Duskpeak', 2, SEC, t, w_h2, 'MUS_MT_PYRE_EXTERIOR', [
        ('OBJ_EVENT_GFX_WOMAN_3_FRLG', 4, 3,
         'Two nights ago, people in dark\n'
         'coats went up the pass.\fThey carried something wrapped\n'
         'in cloth. It rang like glass\nwith every step.\f'
         'The guard tried to stop them.\n'
         'They flew right over his head!'),
        ('OBJ_EVENT_GFX_LITTLE_GIRL_FRLG', 8, 5,
         'My big sister wants to beat\n'
         'TOMOE one day.\fShe trains with a GASTLY every\n'
         'night. It just laughs at her.', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ], layout='LAYOUT_HOUSE2_FRLG')
    gym(t, w_gym)


def gym(t, w_gym):
    name = P + '_Gym'
    mx = GW // 2
    m = add(g.MapDef(name, 'MAP_SEIKA_DUSKPEAK_GYM', SEC, gym_room(), 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (mx, mx - 1, mx + 1):        # warp 0 is the mat itself
        door(m, k, GH - 2, t.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_CHANNELER', mx, 3, name + '_EventScript_Tomoe', move='MOVEMENT_TYPE_FACE_DOWN'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', mx - 2, GH - 4, name + '_EventScript_GymGuy'))
    trainer('TRAINER_SEIKA_CHANNELER_SAYO', 'SAYO', 'Channeler Frlg', 'Channeler Frlg', 'Female', '''
Dusclops
Level: 64
- Shadow Sneak
- Will-O-Wisp
- Pain Split
- Ice Beam

Banette
Level: 65
- Shadow Claw
- Gunk Shot
- Sucker Punch
- Knock Off
''', music='Dangerous')
    trainer('TRAINER_SEIKA_HEX_MANIAC_MIYAKO', 'MIYAKO', 'Hex Maniac', 'Hex Maniac', 'Female', '''
Drifblim
Level: 64
- Shadow Ball
- Thunderbolt
- Will-O-Wisp
- Calm Mind

Sableye
Level: 65
- Knock Off
- Shadow Sneak
- Will-O-Wisp
- Recover

Golurk
Level: 65
- Earthquake
- Shadow Punch
- Ice Punch
- Rock Slide
''', music='Suspicious')
    trainer('TRAINER_SEIKA_CHANNELER_KIKUE', 'KIKUE', 'Channeler Frlg', 'Channeler Frlg', 'Female', '''
Cofagrigus
Level: 65
- Shadow Ball
- Will-O-Wisp
- Trick Room
- Nasty Plot

Chandelure
Level: 66
- Shadow Ball
- Flamethrower
- Energy Ball
- Calm Mind
''', music='Dangerous')
    trainer('TRAINER_SEIKA_LEADER_TOMOE', 'TOMOE', 'Leader Frlg', 'Channeler Frlg', 'Female', '''
Spiritomb @ Leftovers
Level: 66
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Foul Play
- Shadow Sneak
- Will-O-Wisp
- Pain Split

Froslass
Level: 66
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Ice Beam
- Shadow Ball
- Thunderbolt
- Destiny Bond

Dusknoir
Level: 67
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Poltergeist
- Ice Punch
- Earthquake
- Shadow Sneak

Gengar @ Spell Tag
Level: 67
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Shadow Ball
- Sludge Bomb
- Focus Blast
- Thunderbolt

Mismagius @ Sitrus Berry
Level: 68
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Shadow Ball
- Mystical Fire
- Dazzling Gleam
- Nasty Plot
''', music='Female', ai='Smart Trainer')
    # the way up: right through the lower stones, left through the upper ones
    m.objects.append(g.obj('OBJ_EVENT_GFX_CHANNELER', 13, 14, name + '_EventScript_Sayo',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_HEX_MANIAC', 1, 12, name + '_EventScript_Miyako',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=5))
    m.objects.append(g.obj('OBJ_EVENT_GFX_CHANNELER', 4, 10, name + '_EventScript_Kikue',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.scripts += battle(name + '_EventScript_Sayo', 'TRAINER_SEIKA_CHANNELER_SAYO',
                        'The stones are listening...\nAnd they do not like you!',
                        'The spirits... went quiet.',
                        'Each stone here is for a keeper\nwho watched for the star.', name)
    m.scripts += battle(name + '_EventScript_Miyako', 'TRAINER_SEIKA_HEX_MANIAC_MIYAKO',
                        'Fufufu... A curse on your\nnext turn!',
                        'My curse came back to me...',
                        'TOMOE never curses anyone.\nShe just listens. It is scarier.', name)
    m.scripts += battle(name + '_EventScript_Kikue', 'TRAINER_SEIKA_CHANNELER_KIKUE',
                        'Step into the light of the circle,\nif you can reach it!',
                        'The candle went out...',
                        'TOMOE\'s MISMAGIUS sings before\nit strikes. Do not listen!', name)
    m.scripts += (name + '_EventScript_Tomoe::\n'
                  '\ttrainerbattle_single TRAINER_SEIKA_LEADER_TOMOE, %s_Text_TomoeIntro, %s_Text_TomoeDefeat, %s_EventScript_TomoeDefeated, NO_MUSIC\n'
                  '\tmsgbox %s_Text_TomoePost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%s_EventScript_TomoeDefeated::\n'
                  '\tsetflag %s\n'
                  '\tmsgbox %s_Text_TomoeGift, MSGBOX_DEFAULT\n'
                  '\tgiveitem ITEM_TM_SHADOW_BALL\n'
                  '\tmsgbox %s_Text_TomoePost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % (name, name, name, name, name, BADGE, name, name))
    m.scripts += T(name + '_Text_TomoeIntro',
                   'Welcome. I am TOMOE. I keep the\n'
                   'shrine below MT. SKYREACH.\fThe spirits of the old keepers\n'
                   'speak to me. They have watched\nyou since LANTERNPORT.\f'
                   'They wish to know if your heart\n'
                   'is ready for the peak.\fShow them!')
    m.scripts += T(name + '_Text_TomoeDefeat',
                   'The spirits are smiling...\nThey are proud of you.\f'
                   'Take the DUSK BADGE, the last\nof the eight.\f'
                   '{FONT_NORMAL}{PLAYER} received the DUSK BADGE\n'
                   'from TOMOE!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}')
    m.scripts += T(name + '_Text_TomoeGift', 'Take this, too. Let the dark\ncarry your strength.')
    m.scripts += T(name + '_Text_TomoePost',
                   'Listen well. The WISHING STAR\'s\n'
                   'seven days are nearly over.\f'
                   'Team NOVA carried the STARGLASS\n'
                   'BELL up to the SKY SHRINE.\f'
                   'But the shrine path opens only\n'
                   'for the CHAMPION of the SEIKA\nLEAGUE.\f'
                   'Cross SKYREACH PASS to STARWATCH\n'
                   'PLATEAU, and win. Hurry!')
    m.scripts += (name + '_EventScript_GymGuy::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_set %s, %s_EventScript_GymGuyWon\n'
                  '\tmsgbox %s_Text_GymGuy, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%s_EventScript_GymGuyWon::\n\tmsgbox %s_Text_GymGuyWon, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % (BADGE, name, name, name, name))
    m.scripts += T(name + '_Text_GymGuy',
                   'Yo! This is the last GYM in SEIKA,\n'
                   'and the darkest!\fTOMOE\'s GHOST POKéMON laugh at\n'
                   'NORMAL and FIGHTING moves.\f'
                   'Hit them with DARK and GHOST\nmoves instead.\f'
                   'And her SPIRITOMB? Only FAIRY\nmoves really hurt it!')
    m.scripts += T(name + '_Text_GymGuyWon',
                   'You did it! All eight SEIKA\nBADGES!\fSKYREACH PASS is open to you\nnow. Go, champ!')


# ---------------------------------------------------------------- Skyreach Pass

def build_pass(m):
    name = PASS
    guard = 'LOCALID_SEIKA_SKYREACH_GUARD'
    signpost(m, 9, 54, name + '_EventScript_Sign', 'SKYREACH PASS\nDUSKPEAK - STARWATCH PLATEAU')
    m.layout.put(9, 54, g.SIGN)
    # the guard: steps into the cut stop you until you hold all eight badges
    m.objects.append(g.obj('OBJ_EVENT_GFX_BLACK_BELT_FRLG', 12, 53, name + '_EventScript_Guard',
                           local_id=guard, move='MOVEMENT_TYPE_FACE_LEFT'))
    for x in (10, 11):
        m.coords.append(g.coord(x, 52, name + '_EventScript_GateTrigger', 'VAR_TEMP_1', 0))
    m.map_scripts = [('MAP_SCRIPT_ON_TRANSITION', name + '_OnTransition')]
    m.scripts += (name + '_OnTransition::\n\tsetvar VAR_TEMP_1, 0\n'
                  + ''.join('\tgoto_if_unset %s, %s_EventScript_GateShut\n' % (f, name) for f in BADGES)
                  + '\tsetvar VAR_TEMP_1, 1\n'
                  '%(n)s_EventScript_GateShut::\n\tend\n\n'
                  '%(n)s_EventScript_GateTrigger::\n\tlockall\n'
                  '\tapplymovement %(g)s, Common_Movement_WalkInPlaceFasterLeft\n\twaitmovement 0\n'
                  '\tplayse SE_PIN\n\tapplymovement %(g)s, Common_Movement_ExclamationMark\n\twaitmovement 0\n'
                  '\tmsgbox %(n)s_Text_GuardStop, MSGBOX_DEFAULT\n\tclosemessage\n'
                  '\tapplymovement LOCALID_PLAYER, Common_Movement_WalkDown\n\twaitmovement 0\n'
                  '\treleaseall\n\tend\n\n'
                  '%(n)s_EventScript_Guard::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_eq VAR_TEMP_1, 1, %(n)s_EventScript_GuardPass\n'
                  '\tmsgbox %(n)s_Text_GuardStop, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_EventScript_GuardPass::\n'
                  '\tmsgbox %(n)s_Text_GuardPass, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % dict(n=name, g=guard))
    m.scripts += T(name + '_Text_GuardStop',
                   'Halt! Past here is the climb to\n'
                   'the SEIKA LEAGUE.\fOnly trainers with all eight\n'
                   'SEIKA BADGES may go up.\fCome back when you have them!')
    m.scripts += T(name + '_Text_GuardPass',
                   'Eight SEIKA BADGES! Go on up.\fThe path is steep. Use the\n'
                   'steps in the cliffs, and mind\nthe ledges.')
    trainer('TRAINER_SEIKA_COOLTRAINER_RYUSEI', 'RYUSEI', 'Cooltrainer Frlg', 'Cooltrainer M Frlg', 'Male', '''
Gliscor
Level: 62
- Earthquake
- Knock Off
- Acrobatics
- Swords Dance

Ampharos
Level: 63
- Thunderbolt
- Dragon Pulse
- Focus Blast
- Power Gem
''', music='Serious')
    trainer('TRAINER_SEIKA_COOLTRAINER_HOSHIE', 'HOSHIE', 'Cooltrainer Frlg', 'Cooltrainer F Frlg', 'Female', '''
Togekiss
Level: 63
- Air Slash
- Dazzling Gleam
- Aura Sphere
- Roost

Kingdra
Level: 63
- Surf
- Dragon Pulse
- Ice Beam
- Dragon Dance

Espeon
Level: 63
- Psychic
- Dazzling Gleam
- Shadow Ball
- Calm Mind
''', music='Serious')
    trainer('TRAINER_SEIKA_COOLTRAINER_TAKERU', 'TAKERU', 'Cooltrainer Frlg', 'Cooltrainer M Frlg', 'Male', '''
Donphan
Level: 63
- Earthquake
- Ice Shard
- Knock Off
- Rapid Spin

Ursaring
Level: 63
- Facade
- Close Combat
- Crunch
- Swords Dance

Tyranitar
Level: 64
- Stone Edge
- Crunch
- Earthquake
- Dragon Dance
''', music='Serious')
    trainer('TRAINER_SEIKA_COOLTRAINER_MISORA', 'MISORA', 'Cooltrainer Frlg', 'Cooltrainer F Frlg', 'Female', '''
Lanturn
Level: 64
- Surf
- Thunderbolt
- Ice Beam
- Volt Switch

Skarmory
Level: 64
- Brave Bird
- Iron Head
- Spikes
- Roost

Blissey
Level: 65
- Seismic Toss
- Soft-Boiled
- Ice Beam
- Thunder Wave
''', music='Serious')
    for gfx, x, y, who, move, sight in (
            ('OBJ_EVENT_GFX_COOLTRAINER_M', 13, 46, 'Ryusei', 'MOVEMENT_TYPE_FACE_LEFT', 2),
            ('OBJ_EVENT_GFX_COOLTRAINER_F', 19, 37, 'Hoshie', 'MOVEMENT_TYPE_FACE_LEFT', 4),
            ('OBJ_EVENT_GFX_COOLTRAINER_M', 9, 20, 'Takeru', 'MOVEMENT_TYPE_FACE_DOWN', 2),
            ('OBJ_EVENT_GFX_COOLTRAINER_F', 15, 9, 'Misora', 'MOVEMENT_TYPE_FACE_LEFT', 2)):
        m.objects.append(g.obj(gfx, x, y, name + '_EventScript_' + who, move=move,
                               trainer='TRAINER_TYPE_NORMAL', sight=sight))
    m.scripts += battle(name + '_EventScript_Ryusei', 'TRAINER_SEIKA_COOLTRAINER_RYUSEI',
                        'Eight BADGES, huh? Let\'s see if\nyou earned them!',
                        'Yeah, you earned them.',
                        'The pass has three cliffs. Look\nfor the steps cut into each one.', name)
    m.scripts += battle(name + '_EventScript_Hoshie', 'TRAINER_SEIKA_COOLTRAINER_HOSHIE',
                        'Halfway up and already tired?\nI\'m not!',
                        'Okay... now I\'m tired.',
                        'Going down is easy. Just hop the\nledges! Going up is the hard part.', name)
    m.scripts += battle(name + '_EventScript_Takeru', 'TRAINER_SEIKA_COOLTRAINER_TAKERU',
                        'My TYRANITAR grew up on this\nmountain. It knows every rock!',
                        'Crushed like gravel...',
                        'LARVITAR eat the mountain itself.\nOne day there may be no pass!', name)
    m.scripts += battle(name + '_EventScript_Misora', 'TRAINER_SEIKA_COOLTRAINER_MISORA',
                        'The LEAGUE is just ahead. You\nwill have to beat me first!',
                        'Go on, then. You are ready.',
                        'From STARWATCH PLATEAU you can\nsee every town in SEIKA.', name)
    wild('MAP_SEIKA_SKYREACH_PASS', 'gSeika_SkyreachPass',
         land=[('GRAVELER', 60, 61), ('GLIGAR', 60, 62), ('MURKROW', 60, 62), ('MISDREAVUS', 60, 62),
               ('DONPHAN', 61, 62), ('SNEASEL', 61, 62), ('LARVITAR', 60, 61), ('SKARMORY', 62, 63),
               ('PUPITAR', 62, 63), ('URSARING', 62, 63), ('GLISCOR', 64, 64), ('TYRANITAR', 64, 64)])


# ---------------------------------------------------------------- Starwatch Plateau

def build_plateau(m):
    name = PLATEAU
    monk = 'LOCALID_SEIKA_STARWATCH_MONK'
    door(m, 11, 6, 'MAP_SEIKA_LEAGUE_LOBBY', 0)
    door(m, 20, 7, 'MAP_SEIKA_SKY_SHRINE', 0)
    # the statues' plinths read as signs (Indigo's own text would say INDIGO PLATEAU)
    signpost(m, 8, 7, name + '_EventScript_LeagueSign',
             'STARWATCH PLATEAU\fThe SEIKA LEAGUE: the last\nwatch below the stars.')
    for x, y in ((14, 7), (8, 10), (14, 10)):
        sign_at(m, x, y, name + '_EventScript_LeagueSign')
    signpost(m, 18, 8, name + '_EventScript_CaveSign',
             'MT. SKYREACH: SKY SHRINE\fOnly the CHAMPION of the SEIKA\nLEAGUE may enter.')
    sign_at(m, 22, 8, name + '_EventScript_CaveSign')
    # the monk keeps the cave until you are Champion, then steps aside
    m.objects.append(g.obj('OBJ_EVENT_GFX_OLD_MAN', 20, 8, name + '_EventScript_Monk',
                           local_id=monk, move='MOVEMENT_TYPE_FACE_DOWN'))
    m.map_scripts = [('MAP_SCRIPT_ON_TRANSITION', name + '_OnTransition')]
    m.scripts += ('%(n)s_OnTransition::\n\tcall_if_set %(c)s, %(n)s_EventScript_MonkAside\n\tend\n\n'
                  '%(n)s_EventScript_MonkAside::\n\tsetobjectxyperm %(m)s, 21, 8\n'
                  '\tsetobjectmovementtype %(m)s, MOVEMENT_TYPE_FACE_LEFT\n\treturn\n\n'
                  '%(n)s_EventScript_Monk::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_set %(c)s, %(n)s_EventScript_MonkChampion\n'
                  '\tmsgbox %(n)s_Text_MonkBlock, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_EventScript_MonkChampion::\n'
                  '\tmsgbox %(n)s_Text_MonkChampion, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % dict(n=name, c=CHAMPION, m=monk))
    m.scripts += T(name + '_Text_MonkBlock',
                   'This cave climbs to the summit of\n'
                   'MT. SKYREACH and the SKY SHRINE.\f'
                   'By an oath a thousand years old,\n'
                   'only the CHAMPION of the SEIKA\nLEAGUE may enter.\f'
                   'The LEAGUE is behind you, child.')
    m.scripts += T(name + '_Text_MonkChampion',
                   'CHAMPION {PLAYER}. The mountain\n'
                   'has been waiting for you.\f'
                   'The SKY SHRINE is above. Go\nwith the stars.')
    npc(m, 'OBJ_EVENT_GFX_COOLTRAINER_M', 13, 16, name + '_EventScript_Cooltrainer',
        'The SEIKA LEAGUE! I climbed all\n'
        'the way up here...\fAnd lost to the first ELITE\n'
        'FOUR member. Back down I go.', 'MOVEMENT_TYPE_FACE_LEFT')
