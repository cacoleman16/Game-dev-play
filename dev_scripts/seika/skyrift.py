"""Skyrift Isles (docs/SKYRIFT.md): the island chain where the sky tore open.

Riftwake Harbor (town, ferry pier), Shardfall Beach, Meteor Basin, the
Observatory Cliffs and Vega's Nova Observatory (1F, 2F and the roof).
The story people (Dr. Halley, the innkeeper, the rifts, Sirius, Vega and
the climax) come from skyrift_story.py, at the tiles in STORY_SPOTS.
"""
import common as c
from common import (g, T, add, door, npc, signpost, wild, mapsec, trainer, battle, flag, link, PC, MART)

SEC_HARBOR = mapsec('MAPSEC_SKYRIFT_RIFTWAKE_HARBOR', 'RIFTWAKE HARBOR')
SEC_BEACH = mapsec('MAPSEC_SKYRIFT_SHARDFALL_BEACH', 'SHARDFALL BEACH')
SEC_BASIN = mapsec('MAPSEC_SKYRIFT_METEOR_BASIN', 'METEOR BASIN')
SEC_CLIFFS = mapsec('MAPSEC_SKYRIFT_OBSERVATORY_CLIFFS', 'OBSERV. CLIFFS')   # names are 16 chars at most
SEC_OBS = mapsec('MAPSEC_SKYRIFT_OBSERVATORY', 'OBSERVATORY')   # + ' ROOFTOP' must fit the 21-char popup

HARBOR = 'MAP_SKYRIFT_RIFTWAKE_HARBOR'
BEACH = 'MAP_SKYRIFT_SHARDFALL_BEACH'
BASIN = 'MAP_SKYRIFT_METEOR_BASIN'
CLIFFS = 'MAP_SKYRIFT_OBSERVATORY_CLIFFS'
OBS1 = 'MAP_SKYRIFT_OBSERVATORY_1F'
OBS2 = 'MAP_SKYRIFT_OBSERVATORY_2F'
ROOF = 'MAP_SKYRIFT_OBSERVATORY_ROOF'

P_HARBOR = 'Skyrift_RiftwakeHarbor'
P_BEACH = 'Skyrift_ShardfallBeach'
P_BASIN = 'Skyrift_MeteorBasin'
P_CLIFFS = 'Skyrift_ObservatoryCliffs'
P_OBS1 = 'Skyrift_Observatory_1F'
P_OBS2 = 'Skyrift_Observatory_2F'
P_ROOF = 'Skyrift_Observatory_Roof'

ITEM_BEACH = flag('FLAG_QOL_SKYRIFT_ITEM_BEACH', 'Shardfall Beach item ball')
ITEM_BASIN = flag('FLAG_QOL_SKYRIFT_ITEM_BASIN', 'Meteor Basin item ball')
ITEM_CLIFFS = flag('FLAG_QOL_SKYRIFT_ITEM_CLIFFS', 'Observatory Cliffs item ball')
ITEM_OBS = flag('FLAG_QOL_SKYRIFT_ITEM_OBSERVATORY', 'Nova Observatory 2F item ball')

MART_ITEMS = ['ITEM_BEAST_BALL', 'ITEM_ULTRA_BALL', 'ITEM_DUSK_BALL', 'ITEM_QUICK_BALL', 'ITEM_TIMER_BALL',
              'ITEM_FULL_RESTORE', 'ITEM_MAX_POTION', 'ITEM_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL']

IV31 = 'IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe'

# Places where skyrift_story.py puts people and objects: name -> (map id, x, y).
# Every one is a free floor tile the player can walk to (or talk across a counter to).
STORY_SPOTS = {}


# ---------------------------------------------------------------- painting helpers

# FireRed's brown mountain blocks (primary tileset, so they work on any map)
ROCK_TOP = (0x068, 0x069, 0x06A)
ROCK_IN_TOP = (0x06B, 0x06C, 0x06D)
ROCK_MID = (0x070, 0x073, 0x071, 0x075, 0x072)
ROCK_IN_BOT = (0x07B, 0x07C, 0x07D)
ROCK_BOT = (0x078, 0x079, 0x07A)
STAIRS = (0x091, 0x089)
BOULDER = 0x09F


def _fix(cv, x, y, mid):
    if 0 <= x < cv.w and 0 <= y < cv.h:
        cv.put(x, y, mid)
        cv.cls[y][x] = 'fixed'
        cv._untree(x, y)


def rock(cv, x, y, w, h):
    """A free-standing mountain block (Route 23 style); w, h >= 2."""
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
            _fix(cv, x + xx, y + yy, m)


def stairs(cv, x, y, h):
    """Two-wide stone steps up a cliff band, h rows tall."""
    for yy in range(h):
        _fix(cv, x, y + yy, STAIRS[0])
        _fix(cv, x + 1, y + yy, STAIRS[1])


def boulders(cv, pts):
    for x, y in pts:
        _fix(cv, x, y, BOULDER)


def item_ball(m, x, y, label, item, flg):
    m.objects.append(g.obj('OBJ_EVENT_GFX_ITEM_BALL', x, y, label, flag=flg))
    m.scripts += '%s::\n\tfinditem %s\n\tend\n\n' % (label, item)


def indoor(name, map_id, mapsec_id, layout, music):
    return add(g.MapDef(name, map_id, mapsec_id, layout, music, 'MAP_TYPE_INDOOR', 'WEATHER_NONE',
                        False, False, False))


# ---------------------------------------------------------------- Pokemon Center and Mart

def pokecenter(outside, out_warp, npcs=()):
    """common.pokecenter, with Skyrift names: Pokemon Center 1F + 2F."""
    name1, name2 = 'Skyrift_Riftwake_PokemonCenter_1F', 'Skyrift_Riftwake_PokemonCenter_2F'
    id1, id2 = 'MAP_SKYRIFT_RIFTWAKE_POKEMON_CENTER_1F', 'MAP_SKYRIFT_RIFTWAKE_POKEMON_CENTER_2F'
    heal = 'HEAL_LOCATION_SKYRIFT_RIFTWAKE'
    f1 = indoor(name1, id1, SEC_HARBOR, 'LAYOUT_POKEMON_CENTER_1F_FRLG', 'MUS_RG_POKE_CENTER')
    nurse = 'LOCALID_SKYRIFT_RIFTWAKE_NURSE'
    f1.objects.append(g.obj('OBJ_EVENT_GFX_NURSE_FRLG', 7, 2, '%s_EventScript_Nurse' % name1, local_id=nurse))
    for k in (6, 7, 8):
        door(f1, k, 8, outside.id, out_warp)
    door(f1, 1, 6, id2, 0, elevation=4)
    f1.map_scripts = [('MAP_SCRIPT_ON_TRANSITION', '%s_OnTransition' % name1),
                      ('MAP_SCRIPT_ON_RESUME', 'CableClub_OnResume')]
    f1.scripts = ('%s_OnTransition::\n\tsetrespawn %s\n\tend\n\n%s_EventScript_Nurse::\n\tlock\n\tfaceplayer\n'
                  '\tcall EventScript_PkmnCenterNurse_Frlg\n\trelease\n\tend\n\n' % (name1, heal, name1))
    for i, (gfx, x, y, text) in enumerate(npcs):
        npc(f1, gfx, x, y, '%s_EventScript_Npc%d' % (name1, i), text, 'MOVEMENT_TYPE_FACE_DOWN')
    f2 = indoor(name2, id2, SEC_HARBOR, 'LAYOUT_POKEMON_CENTER_2F_FRLG', 'MUS_RG_POKE_CENTER')
    door(f2, 1, 6, id1, 3, elevation=4)
    door(f2, 5, 1, 'MAP_UNION_ROOM_FRLG', 0, elevation=0)
    door(f2, 9, 1, 'MAP_TRADE_CENTER_FRLG', 0, elevation=0)
    for x, scr in ((5, 'Common_EventScript_UnionRoomAttendant'), (9, 'Common_EventScript_WirelessClubAttendant'),
                   (13, 'Common_EventScript_DirectCornerAttendant')):
        f2.objects.append(g.obj('OBJ_EVENT_GFX_CABLE_CLUB_RECEPTIONIST', x, 2, scr))
    f2.map_scripts = [('MAP_SCRIPT_ON_FRAME_TABLE', 'CableClub_OnFrame'),
                      ('MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE', 'CableClub_OnWarp'),
                      ('MAP_SCRIPT_ON_LOAD', 'CableClub_OnLoad'),
                      ('MAP_SCRIPT_ON_TRANSITION', 'CableClub_OnTransition')]
    c.HEALS.append({'id': heal, 'map': outside.id, 'x': outside.warps[out_warp]['x'],
                    'y': outside.warps[out_warp]['y'] + 1, 'respawn_map': id1, 'respawn_npc': nurse})
    return f1


def mart(outside, out_warp, npcs=()):
    name, mid = 'Skyrift_Riftwake_Mart', 'MAP_SKYRIFT_RIFTWAKE_MART'
    m = indoor(name, mid, SEC_HARBOR, 'LAYOUT_MART_FRLG', 'MUS_RG_POKE_CENTER')
    for k in (3, 4, 5):
        door(m, k, 7, outside.id, out_warp)
    m.objects.append(g.obj('OBJ_EVENT_GFX_CLERK', 2, 3, '%s_EventScript_Clerk' % name, move='MOVEMENT_TYPE_FACE_RIGHT'))
    m.scripts = ('%s_EventScript_Clerk::\n\tlock\n\tfaceplayer\n\tmessage gText_HowMayIServeYou\n\twaitmessage\n'
                 '\tpokemart %s_Items\n\tmsgbox gText_PleaseComeAgain\n\trelease\n\tend\n\n\t.align 2\n%s_Items::\n'
                 % (name, name, name) + ''.join('\t.2byte %s\n' % it for it in MART_ITEMS)
                 + '\t.2byte ITEM_NONE\n\trelease\n\tend\n\n')
    for i, (gfx, x, y, text) in enumerate(npcs):
        npc(m, gfx, x, y, '%s_EventScript_Npc%d' % (name, i), text, 'MOVEMENT_TYPE_WANDER_AROUND')
    return m


# ---------------------------------------------------------------- Riftwake Harbor

TWO = 'LAYOUT_TWO_ISLAND'
THREE = 'LAYOUT_THREE_ISLAND'
HUT = (TWO, 32, 6, 5, 4)          # purple-roofed Sevii house, door (1, 3)
INN = (THREE, 2, 28, 5, 4)        # red-roofed Sevii house, door (1, 3)


def harbor_canvas():
    cv = g.Canvas(36, 30, 'gTileset_SeviiIslands123', seed=101)
    cv.forest(0, 0, 36, 6)                # north
    cv.forest(0, 6, 8, 18)                # west, down to the shore
    cv.forest(28, 6, 8, 6)                # east: opening at y 12..17
    cv.forest(28, 18, 8, 6)
    cv.rect(0, 24, 36, 6, 'sea')
    cv.rect(8, 20, 20, 4, 'beach')        # the shore
    # streets: beach sand, like Vermilion's
    cv.rect(9, 11, 19, 5, 'beach')        # the square in front of the buildings
    cv.rect(28, 14, 8, 2, 'beach')        # east to Shardfall Beach
    cv.rect(17, 16, 3, 4, 'beach')        # down to the pier
    # buildings: PC (9,6) door (11,10); Rift Hut (15,7) door (16,10); Mart (22,7) door (24,10);
    # Inn (9,16) door (10,19)
    cv.stamp(*PC[:5], 9, 6)
    cv.stamp(*HUT, 15, 7)
    cv.stamp(*MART[:5], 22, 7)
    cv.stamp(*INN, 9, 16)
    cv.tree(26, 6)
    cv.tree(26, 8)
    cv.rect(20, 8, 2, 2, 'flowers')
    cv.rect(14, 16, 2, 3, 'flowers')
    cv.rect(21, 16, 6, 1, 'flowers')
    cv.tree(22, 17)
    cv.tree(24, 17)
    cv.rect(26, 17, 2, 3, 'flowers')
    cv.put(20, 10, g.SIGN)                # town sign
    cv.put(14, 10, g.SIGN)                # Rift Hut sign
    cv.put(14, 19, g.SIGN)                # inn sign
    g.pier(cv, 17, 21, 3, 6)              # pier x17-19, y21-26
    return cv


def inn_room():
    """Celadon's hotel lobby, with a gap at the end of the front desk so the
    innkeeper can step out from behind it."""
    r = g.Room(17, 11, 'gTileset_BuildingFrlg', 'gTileset_RestaurantHotel', source='LAYOUT_CELADON_CITY_HOTEL')
    r.copy(0, 0, 17, 11, 0, 0)
    r.set(6, 5, r.at(7, 5))               # the desk's right end moves one tile left
    r.set(7, 5, r.at(8, 6))               # plain floor in the gap
    r.set(7, 6, r.at(8, 6))               # no desk shadow under the gap
    return r


def harbor(t):
    P = P_HARBOR
    w_pc = door(t, 11, 10, 'MAP_SKYRIFT_RIFTWAKE_POKEMON_CENTER_1F', 0)
    w_hut = door(t, 16, 10, 'MAP_SKYRIFT_RIFTWAKE_RIFT_HUT', 0)
    w_mart = door(t, 24, 10, 'MAP_SKYRIFT_RIFTWAKE_MART', 0)
    w_inn = door(t, 10, 19, 'MAP_SKYRIFT_RIFTWAKE_INN', 0)
    signpost(t, 20, 10, P + '_EventScript_TownSign',
             'RIFTWAKE HARBOR\n'
             'The last port under the torn sky')
    signpost(t, 14, 10, P + '_EventScript_HutSign',
             'DR. HALLEY\'S RIFT HUT\fKnock first. If the door hums,\n'
             'knock twice.')
    signpost(t, 14, 19, P + '_EventScript_InnSign',
             'RIFTWAKE INN\n'
             'Rest under a quiet roof.')
    npc(t, 'OBJ_EVENT_GFX_OLD_MAN_1', 21, 12, P + '_EventScript_OldMan',
        'See that crack in the sky, over\n'
        'the hills to the east?\fIt opened the night the WISHING\n'
        'STAR left SEIKA. Ripped open like\n'
        'an old sail.\fSixty years I\'ve fished here.\n'
        'Never seen the stars bleed before.', 'MOVEMENT_TYPE_FACE_UP')
    npc(t, 'OBJ_EVENT_GFX_WOMAN_1_FRLG', 13, 13, P + '_EventScript_Woman',
        'At night the rift glows violet.\n'
        'Nobody in town can sleep.\fThe inn is full of folk who came\n'
        'just to watch it.', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_LITTLE_BOY_FRLG', 24, 13, P + '_EventScript_Boy',
        'Something with tentacles floated\n'
        'down onto SHARDFALL BEACH!\fIt looked like a jellyfish made\n'
        'of glass. Mom says I dreamed it.', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_FISHER', 23, 21, P + '_EventScript_Fisher',
        'Since the sky tore, the sea\n'
        'glitters at night.\fLittle shards keep falling into\n'
        'the water. STARYU love them!', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_WORKER_M', 29, 13, P + '_EventScript_Worker',
        'East of town is SHARDFALL BEACH.\fPast it, the crater where the big\n'
        'meteorite hit, and the cliffs up\n'
        'to that OBSERVATORY.\fThe people in the strange coats\n'
        'came off a private boat. They\n'
        'went up there and never left.', 'MOVEMENT_TYPE_FACE_LEFT')
    # the ferry: the sailor's script comes from skyrift_story.py
    t.objects.append(g.obj('OBJ_EVENT_GFX_SAILOR_FRLG', 18, 26, 'Skyrift_EventScript_HarborSailor',
                           move='MOVEMENT_TYPE_FACE_UP', local_id='LOCALID_SKYRIFT_HARBOR_SAILOR'))
    t.objects.append(g.obj('OBJ_EVENT_GFX_SS_TIDAL', 18, 28, '0x0', move='MOVEMENT_TYPE_FACE_RIGHT', elevation=1))
    STORY_SPOTS['arrival'] = (HARBOR, 18, 24)
    STORY_SPOTS['sailor'] = (HARBOR, 18, 26)
    wild(HARBOR, 'gSkyrift_RiftwakeHarbor',
         water=[('TENTACRUEL', 66, 68), ('PELIPPER', 66, 68), ('MANTINE', 67, 69), ('STARMIE', 67, 69),
                ('LAPRAS', 70, 70)],
         fishing=[('MAGIKARP', 60, 64), ('STARYU', 64, 66), ('WISHIWASHI', 66, 68), ('STARYU', 66, 68),
                  ('BRUXISH', 66, 68), ('STARMIE', 67, 69), ('WISHIWASHI', 68, 70), ('BRUXISH', 68, 70),
                  ('GYARADOS', 68, 70), ('DHELMISE', 70, 72)])

    pokecenter(t, w_pc, npcs=[
        ('OBJ_EVENT_GFX_HIKER_FRLG', 11, 5,
         'I came all this way to see the\n'
         'SKYRIFT up close.\fNow I can\'t stop looking up.\n'
         'My neck hurts!'),
    ])
    mart(t, w_mart, npcs=[
        ('OBJ_EVENT_GFX_GENTLEMAN_FRLG', 6, 4,
         'BEAST BALLS! They only make them\n'
         'for what comes out of the rift.\fOn anything else, they barely\n'
         'work at all. Choose wisely!'),
    ])
    # Dr. Halley's Rift Hut: Bill's sea cottage, with its humming machines
    hut = indoor('Skyrift_Riftwake_RiftHut', 'MAP_SKYRIFT_RIFTWAKE_RIFT_HUT', SEC_HARBOR,
                 'LAYOUT_ROUTE25_SEA_COTTAGE', 'MUS_RG_NET_CENTER')
    for k in (6, 7, 8):
        door(hut, k, 9, t.id, w_hut)
    signpost(hut, 4, 5, 'Skyrift_Riftwake_RiftHut_EventScript_Monitor',
             'Lines of numbers scroll across\n'
             'the screen.\fRIFT ENERGY: RISING\n'
             'SOURCE: NOVA OBSERVATORY')
    STORY_SPOTS['halley'] = ('MAP_SKYRIFT_RIFTWAKE_RIFT_HUT', 7, 5)
    # the Riftwake Inn: Celadon's hotel lobby
    inn = indoor('Skyrift_Riftwake_Inn', 'MAP_SKYRIFT_RIFTWAKE_INN', SEC_HARBOR, inn_room(), 'MUS_LILYCOVE')
    door(inn, 4, 9, t.id, w_inn, elevation=0)
    door(inn, 3, 10, t.id, w_inn, elevation=0)
    door(inn, 5, 10, t.id, w_inn, elevation=0)
    signpost(inn, 8, 5, 'Skyrift_Riftwake_Inn_EventScript_Ledger',
             'The guest book. Every name in\n'
             'it wrote the same thing:\f"Came to see the sky."')
    npc(inn, 'OBJ_EVENT_GFX_BEAUTY_FRLG', 15, 8, 'Skyrift_Riftwake_Inn_EventScript_Guest',
        'I sailed out from VERMILION just\n'
        'to watch the rift at night.\fIt\'s beautiful. And a little bit\n'
        'frightening.', 'MOVEMENT_TYPE_FACE_LEFT')
    STORY_SPOTS['innkeeper'] = ('MAP_SKYRIFT_RIFTWAKE_INN', 4, 4)


# ---------------------------------------------------------------- Shardfall Beach

CRYSTAL_GRASS, CRYSTAL_SAND = 0x356, 0x357   # Sevii's pale boulders: the sky-crystal shards


def crystals(cv, pts):
    """Pale sky-crystal boulders (SeviiIslands123), on grass or sand."""
    for x, y in pts:
        mid = CRYSTAL_SAND if cv.cls[y][x] in ('beach', 'path') else CRYSTAL_GRASS
        cv.put(x, y, mid, 1 << 10)        # the cell keeps its class, so the ground around blends


def sea_rocks(cv, pts):
    """2x2 rocks standing in the sea."""
    for x, y in pts:
        for dx, dy, mid in ((0, 0, 0x110), (1, 0, 0x111), (0, 1, 0x118), (1, 1, 0x119)):
            cv.put(x + dx, y + dy, mid, 1 << 10)   # still 'sea' for the rims around it


def beach_canvas():
    cv = g.Canvas(64, 26, 'gTileset_SeviiIslands123', seed=111)
    cv.forest(0, 0, 64, 4)                # top
    cv.forest(0, 4, 4, 2)                 # west: opening at y 6..11 (Riftwake Harbor)
    cv.forest(0, 12, 4, 6)
    cv.rect(0, 18, 64, 8, 'sea')
    cv.forest(60, 10, 4, 16)              # east: opening at y 4..9 (Meteor Basin)
    cv.forest(56, 16, 4, 10)
    # sand: the beach, the road in from the harbor and the road up to the basin
    cv.rect(4, 13, 52, 5, 'beach')
    cv.rect(0, 8, 13, 2, 'beach')
    cv.rect(11, 10, 2, 3, 'beach')
    cv.rect(50, 6, 14, 2, 'beach')
    cv.rect(50, 8, 2, 5, 'beach')
    cv.rect(20, 17, 8, 1, 'sea')          # a little bay
    cv.rect(40, 18, 6, 1, 'beach')        # and a sand spit
    # the headland of sky crystal: the only dry way east is the sand below it
    rock(cv, 30, 4, 8, 11)
    # tall grass and trees
    cv.rect(15, 4, 9, 6, 'tall')
    cv.rect(4, 10, 6, 3, 'tall')
    cv.rect(40, 4, 8, 8, 'tall')
    cv.rect(53, 9, 6, 4, 'tall')
    cv.rect(24, 10, 6, 3, 'flowers')
    for x, y in ((24, 4), (26, 4), (28, 4), (6, 4), (8, 4), (38, 4), (48, 4), (54, 4), (56, 4), (58, 4)):
        cv.tree(x, y)
    cv.forest(13, 10, 2, 2)
    cv.tree(4, 6)
    cv.rect(6, 6, 3, 2, 'flowers')
    cv.tree(26, 6)
    crystals(cv, [(16, 14), (24, 15), (39, 13), (46, 16), (55, 13), (5, 13), (28, 12), (29, 13),
                  (38, 12), (52, 13), (53, 13)])
    sea_rocks(cv, [(8, 21), (26, 23), (36, 20), (47, 22), (14, 24)])
    cv.put(13, 8, g.SIGN)
    return cv


# ---------------------------------------------------------------- Meteor Basin

def basin_canvas():
    cv = g.Canvas(44, 44, 'gTileset_SeviiIslands123', seed=121)
    # woods round the crater; openings: west y 26..31 (Shardfall Beach), north x 14..19 (Observatory Cliffs)
    cv.forest(0, 0, 14, 4)
    cv.forest(20, 0, 24, 4)
    cv.forest(0, 4, 4, 22)
    cv.forest(0, 32, 4, 12)
    cv.forest(40, 4, 4, 40)
    cv.forest(4, 40, 36, 4)
    cv.rect(0, 28, 6, 2, 'path')          # in from the beach
    cv.rect(16, 0, 2, 8, 'path')          # out to the cliffs
    # the crater: ledges down into it from the north, steps out on the south
    cv.rect(12, 13, 20, 18, 'path')
    cv.ledge(12, 12, 20)
    rock(cv, 8, 9, 5, 6)                  # the rim: piles of rock round the floor
    rock(cv, 31, 9, 5, 6)
    rock(cv, 9, 15, 3, 7)
    rock(cv, 9, 22, 3, 9)
    rock(cv, 32, 15, 3, 9)
    rock(cv, 32, 24, 3, 7)
    rock(cv, 8, 31, 13, 4)
    rock(cv, 23, 31, 13, 4)
    cv.rect(21, 31, 2, 4, 'path')         # a cut through the south rim
    # the meteorite, and the shards it threw around
    rock(cv, 18, 17, 8, 7)
    crystals(cv, [(17, 18), (26, 19), (17, 22), (26, 22), (19, 16), (24, 16), (13, 15), (30, 29),
                  (13, 29), (30, 14), (20, 27), (25, 26)])
    # the west road only drops down (a ledge you can't climb): go round by the south and east
    cv.ledge(4, 20, 5)
    # tall grass
    cv.rect(4, 5, 4, 8, 'tall')
    cv.rect(22, 5, 9, 5, 'tall')
    cv.rect(13, 24, 4, 5, 'tall')
    cv.rect(27, 23, 4, 5, 'tall')
    cv.rect(6, 36, 9, 4, 'tall')
    cv.rect(26, 36, 9, 4, 'tall')
    cv.rect(36, 26, 4, 9, 'tall')
    cv.rect(36, 6, 4, 9, 'tall')
    rock(cv, 10, 4, 4, 3)
    boulders(cv, [(19, 5), (33, 7), (6, 30), (16, 37), (37, 38), (24, 37), (20, 9)])
    # the gate of crystals in the east road (the Nova pair stands in it)
    crystals(cv, [(35, 21), (39, 21)])
    return cv


# ---------------------------------------------------------------- Observatory Cliffs

TT = 'LAYOUT_SEVEN_ISLAND_TRAINER_TOWER'


def cliffs_canvas():
    cv = g.Canvas(30, 50, 'gTileset_SeviiIslands67', seed=131)
    cv.forest(0, 0, 4, 50)
    cv.forest(26, 0, 4, 50)
    cv.forest(4, 42, 8, 8)                # south end: opening x 12..17 (Meteor Basin)
    cv.forest(18, 42, 8, 8)
    # the observatory: Seven Island's tower on its rock, door (14,7)
    cv.forest(4, 0, 4, 8)
    cv.forest(22, 0, 4, 8)
    cv.stamp(TT, 52, 0, 14, 9, 8, 0, skip={(i, 8) for i in range(14) if i not in (5, 6, 7)})
    # Sirius's gate: a one-tile cleft in the last cliff
    rock(cv, 4, 11, 10, 3)
    rock(cv, 15, 11, 11, 3)
    # the climb: up the west steps, along, up the east steps, back west to the gate
    cv.rect(14, 38, 2, 12, 'path')
    cv.rect(6, 38, 10, 2, 'path')
    rock(cv, 4, 34, 9, 3)
    rock(cv, 13, 33, 13, 4)
    stairs(cv, 6, 34, 3)
    cv.rect(6, 27, 2, 7, 'path')
    cv.rect(6, 27, 18, 2, 'path')
    rock(cv, 4, 21, 10, 4)
    cv.ledge(14, 23, 4)                   # the quick way down
    rock(cv, 18, 22, 8, 3)
    stairs(cv, 22, 22, 3)
    cv.rect(22, 15, 2, 7, 'path')
    cv.rect(14, 15, 10, 2, 'path')
    # grass and rocks
    cv.rect(16, 37, 8, 4, 'tall')
    cv.rect(9, 29, 8, 4, 'tall')
    cv.rect(4, 15, 8, 6, 'tall')
    cv.rect(16, 18, 5, 3, 'tall')
    cv.rect(6, 17, 3, 3, 'plain')         # a bare patch where the grass won't grow (a rift)
    cv.rect(18, 9, 4, 2, 'flowers')
    rock(cv, 18, 29, 5, 4)
    rock(cv, 10, 17, 4, 4)
    rock(cv, 4, 25, 3, 2)
    boulders(cv, [(24, 30), (11, 40), (24, 19), (5, 9), (24, 9), (13, 26), (21, 25), (8, 36), (24, 38)])
    cv.put(16, 41, g.SIGN)
    return cv


# ---------------------------------------------------------------- the Nova Observatory: 1F and 2F (Silph Co.'s tiles)

SILPH1, SILPH5 = 'LAYOUT_SILPH_CO_1F', 'LAYOUT_SILPH_CO_5F'
OW, OH = 22, 15                           # both floors


def silph_room():
    r = g.Room(OW, OH, 'gTileset_BuildingFrlg', 'gTileset_SilphCo', source=SILPH1)
    s5 = g.Room(1, 1, 'gTileset_BuildingFrlg', 'gTileset_SilphCo', source=SILPH5)
    VOID, WALL_TOP, WALL, SHADE, FLOOR = r.at(0, 0), r.at(0, 2), r.at(1, 3), r.at(1, 4), r.at(1, 5)
    r.fill(0, 0, OW, OH, VOID)
    r.fill(0, 2, OW, 1, WALL_TOP)
    r.fill(0, 3, OW, 1, WALL)
    r.fill(0, 4, OW, OH - 5, FLOOR)
    r.fill(0, 4, OW, 1, SHADE)
    for y in range(4, OH - 1):
        r.set(0, y, SHADE)
    mx = OW // 2
    r.copy(7, 20, 3, 2, mx - 1, OH - 2)   # the exit mat, and the dark step below it
    return r, s5


def plant(r, x, y):
    r.set(x, y, r.src[19][29])            # leaves
    r.set(x, y + 1, r.src[20][29])        # pot


def wall_pc(r, s5, x):
    """A computer bank standing against the back wall (three tiles tall)."""
    for yy in range(3):
        r.set(x, 2 + yy, s5.src[2 + yy][2])


def stairs_up(r, x):
    """1F's corner stairs going up (6 wide); the warp is at (x + 2, 2)."""
    r.copy(29, 0, 6, 4, x, 0)


def obs1_room():
    r, s5 = silph_room()
    r.copy(0, 2, 8, 7, 0, 2)              # Silph's front desk, in the corner
    stairs_up(r, 14)                      # warp (16,2)
    for x in (9, 10, 11):
        wall_pc(r, s5, x)
    r.set(12, 2, r.src[2][12]); r.set(12, 3, r.src[3][12])     # a picture of the night sky
    r.set(20, 2, r.src[2][2]); r.set(20, 3, r.src[3][2])
    r.copy(30, 8, 3, 3, 15, 8)            # glass display cases: meteorite samples
    r.copy(30, 12, 3, 2, 18, 8)
    r.copy(1, 12, 3, 2, 2, 11)            # couches by the door
    r.copy(5, 12, 3, 2, 15, 12)
    for x, y in ((8, 4), (21, 4), (1, 9)):
        plant(r, x, y)
    return r


B4F, B3F = 'LAYOUT_ROCKET_HIDEOUT_B4F', 'LAYOUT_ROCKET_HIDEOUT_B3F'


def obs2_room():
    """The lab floor: the Rocket Hideout's olive tiles, its big machines and stairs."""
    r = g.Room(OW, OH, 'gTileset_BuildingFrlg', 'gTileset_SilphCo', source=B4F)
    b3 = g.Room(1, 1, 'gTileset_BuildingFrlg', 'gTileset_SilphCo', source=B3F)
    W, H = OW, OH
    r.fill(0, 0, W, H, r.at(8, 0))        # void
    r.fill(0, 2, W, 1, r.at(1, 0))        # wall top
    r.fill(0, 3, W, 1, r.at(1, 1))        # wall
    r.fill(0, 4, W, H - 6, r.at(2, 3))    # floor
    r.fill(1, 4, W - 2, 1, r.at(3, 2))    # shadow under the wall
    for y in range(4, H - 2):
        r.set(1, y, r.at(1, 6))           # shadow by the left wall
    for y in range(3, H - 2):
        r.set(0, y, r.at(0, 1))
        r.set(W - 1, y, r.at(23, 1))
    r.set(0, 2, r.at(0, 0))
    r.set(W - 1, 2, r.at(23, 0))
    r.fill(1, H - 2, W - 2, 1, r.at(2, 8))                   # the bottom wall
    r.set(0, H - 2, r.at(0, 8))
    r.set(W - 1, H - 2, r.at(7, 8))
    # stairs down (from 1F) on the left, warp (3,4); stairs up (to the roof) on the right, warp (18,4)
    for yy in range(4):
        for xx in range(3):
            r.set(1 + xx, 2 + yy, b3.src[16 + yy][13 + xx])
            r.set(18 + xx, 2 + yy, b3.src[yy][18 + xx])
    r.set(1, 2, r.at(1, 0))
    # the big machines against the back wall
    for x in (6, 9, 13):
        for yy in range(3):
            for xx in range(3):
                r.set(x + xx, 2 + yy, r.at(17 + xx, yy))
    r.set(12, 2, r.at(1, 0))
    # a long lab bench splits the floor: the way round is at the bottom (x 10..12, y 11..13)
    for xx in range(3):
        r.set(10 + xx, 5, r.at(2 + xx, 4))
        for y in range(6, 10):
            r.set(10 + xx, y, r.at(2 + xx, 5))
        r.set(10 + xx, 10, (r.at(2 + xx, 6) & 0xF3FF) | (1 << 10))
    for x in (3, 15):                     # reading desks with their computers
        for yy in range(3):
            for xx in range(3):
                r.set(x + xx, 8 + yy, r.at(18 + xx, 5 + yy))
    for x, y in ((5, 4), (16, 4), (17, 4), (1, 11), (20, 11)):
        r.set(x, y, r.at(1, 2))
        r.set(x, y + 1, r.at(1, 3))
    return r


# ---------------------------------------------------------------- the roof (Trainer Tower's)

TTROOF = 'LAYOUT_TRAINER_TOWER_ROOF'
RW, RH = 20, 21


def roof_room():
    r = g.Room(RW, RH, 'gTileset_BuildingFrlg', 'gTileset_TrainerTower', source=TTROOF)
    walk = lambda b: (b & 0x3FF) | (3 << 12)               # floor: passable, elevation 3
    block = lambda b: (b & 0x3FF) | (1 << 10)               # solid
    FLOOR, PLAT = walk(r.at(5, 8)), block(r.at(3, 0))
    LIP = (block(r.at(2, 4)), block(r.at(3, 4)), block(r.at(15, 4)))     # left end, middle, right end
    FACE = (block(r.at(6, 6)), block(r.at(7, 6)), block(r.at(11, 6)))
    r.fill(0, 0, RW, RH, FLOOR)
    floor_bottom = RH - 6                 # last floor row
    for y in range(floor_bottom + 1):     # the tower's rim on both sides
        r.set(0, y, r.at(0, 0)); r.set(1, y, r.at(1, 0))
        r.set(RW - 2, y, r.at(16, 7)); r.set(RW - 1, y, r.at(17, 0))
    # the roof's front edge and the tower below it
    for x in range(RW):
        sx = 0 if x == 0 else 1 if x == 1 else 17 if x == RW - 1 else 16 if x == RW - 2 else \
            2 if x == 2 else 15 if x == RW - 3 else 3 + (x - 3) % 12
        r.set(x, floor_bottom, r.at(sx, 9) if sx in (0, 1, 2, 15, 16, 17) else FLOOR)
        for k in range(5):
            r.set(x, floor_bottom + 1 + k, r.at(sx, 10 + k))
    # the engine deck: raised, all along the back
    def raised(x0, x1, y_top, y_lip):
        for y in range(y_top, y_lip):
            for x in range(x0, x1 + 1):
                r.set(x, y, PLAT)
        for x in range(x0, x1 + 1):
            k = 0 if x == x0 else 2 if x == x1 else 1
            r.set(x, y_lip, LIP[k])
            r.set(x, y_lip + 1, FACE[k])
    raised(2, RW - 3, 0, 5)
    for y in range(5):
        r.set(2, y, block(r.at(2, 0)))
        r.set(RW - 3, y, block(r.at(15, 0)))
    for y in range(7):
        r.set(RW - 2, y, r.at(16, 0) if y < 6 else r.at(16, 6))
    # the Skyrift Engine: a humming core set into the deck, with emitter masts round it
    for xx in range(3):
        r.set(8 + xx, 5, block(r.at(12 + xx, 4)))
        r.set(8 + xx, 6, block(r.at(12 + xx, 5)))
    for x, low in ((5, r.at(6, 4)), (13, r.at(11, 4)), (7, r.at(6, 4)), (11, r.at(11, 4))):
        r.set(x, 4, block(r.at(6, 3)))
        r.set(x, 5, block(low))
    for x in (4, 6, 8, 10, 12, 14):
        r.set(x, 1, block(r.at(6, 3)))
        r.set(x, 2, block(r.at(6, 4) if x < 9 else r.at(11, 4)))
    # low walls before the engine: the way in is three tiles wide (x 8..10)
    raised(6, 7, 9, 9)
    raised(11, RW - 3, 9, 9)
    # the stairs down to 2F (warp (4,13)), set into a raised corner like the tower's own
    for yy in range(4):
        for xx in range(4):
            r.set(2 + xx, 11 + yy, r.at(2 + xx, 4 + yy))
    return r


# ---------------------------------------------------------------- trainers, signs and items

def foe(m, gfx, x, y, who, move, sight, const, intro, defeat, post):
    """A trainer standing on map m, with its battle script."""
    label = '%s_EventScript_%s' % (m.name, who)
    m.objects.append(g.obj(gfx, x, y, label, move=move, trainer='TRAINER_TYPE_NORMAL', sight=sight))
    m.scripts += battle(label, const, intro, defeat, post, m.name)


def party(*mons):
    """Pokemon blocks (species line, then moves), all at 31 IVs."""
    out = []
    for head, lv, moves in mons:
        out.append('%s\nLevel: %d\n%s\n%s' % (head, lv, IV31, '\n'.join('- ' + mv for mv in moves)))
    return '\n\n'.join(out) + '\n'


NOVA_M = ('LOYALIST', 'Team Nova', 'Psychic M', 'Male')
NOVA_F = ('LOYALIST', 'Team Nova', 'Psychic F', 'Female')
NOVA_SCI = ('RESEARCHER', 'Team Nova', 'Scientist Frlg', 'Male')


def nova(const, who, mons):
    name, cls, pic, gender = who
    trainer(const, name, cls, pic, gender, party(*mons), music='Intense', ai='Smart Trainer')


def beach(m):
    P = m.name
    signpost(m, 13, 8, P + '_EventScript_Sign', 'SHARDFALL BEACH\nRIFTWAKE HARBOR - METEOR BASIN')
    item_ball(m, 5, 16, P + '_EventScript_ItemStarPiece', 'ITEM_STAR_PIECE', ITEM_BEACH)
    trainer('TRAINER_SKYRIFT_RANGER_KAI', 'KAI', 'Pkmn Ranger Frlg', 'Pokemon Ranger M Frlg', 'Male', party(
        ('Exeggutor-Alola @ Sitrus Berry', 71, ['Dragon Hammer', 'Wood Hammer', 'Flamethrower', 'Earthquake']),
        ('Toucannon', 71, ['Beak Blast', 'Bullet Seed', 'Rock Blast', 'Roost']),
        ('Oricorio-Pom-Pom', 71, ['Revelation Dance', 'Hurricane', 'Quiver Dance', 'Roost']),
        ('Palossand', 72, ['Shadow Ball', 'Earth Power', 'Shore Up', 'Giga Drain'])), music='Serious')
    nova('TRAINER_SKYRIFT_NOVA_BEACH_1', NOVA_M, [
        ('Lunatone', 71, ['Moonblast', 'Psychic', 'Ice Beam', 'Calm Mind']),
        ('Bronzong @ Leftovers', 71, ['Gyro Ball', 'Earthquake', 'Psychic', 'Trick Room']),
        ('Minior', 72, ['Shell Smash', 'Power Gem', 'Acrobatics', 'Earthquake'])])
    nova('TRAINER_SKYRIFT_NOVA_BEACH_2', NOVA_F, [
        ('Solrock', 71, ['Flare Blitz', 'Zen Headbutt', 'Stone Edge', 'Morning Sun']),
        ('Starmie', 71, ['Hydro Pump', 'Psychic', 'Thunderbolt', 'Recover']),
        ('Beheeyem @ Life Orb', 72, ['Psychic', 'Shadow Ball', 'Thunderbolt', 'Nasty Plot'])])
    trainer('TRAINER_SKYRIFT_SWIMMER_LEILANI', 'LEILANI', 'Swimmer F Frlg', 'Swimmer F Frlg', 'Female', party(
        ('Araquanid', 71, ['Liquidation', 'Leech Life', 'Poison Jab', 'Sticky Web']),
        ('Toxapex @ Black Sludge', 71, ['Scald', 'Toxic', 'Recover', 'Baneful Bunker']),
        ('Bruxish', 72, ['Psychic Fangs', 'Aqua Jet', 'Crunch', 'Ice Fang']),
        ('Mantine', 72, ['Scald', 'Air Slash', 'Roost', 'Haze'])), music='Female')
    foe(m, 'OBJ_EVENT_GFX_CAMPER_FRLG', 18, 15, 'Kai', 'MOVEMENT_TYPE_FACE_LEFT', 4,
        'TRAINER_SKYRIFT_RANGER_KAI',
        'Shards fell all over this beach.\nThe POKéMON here went wild!',
        'Okay, okay! You win!',
        'Those pale rocks? They fell out\nof the rift. They\'re warm to\nthe touch.')
    # the Nova pair: the dry way east is one tile wide between them
    foe(m, 'OBJ_EVENT_GFX_PSYCHIC_M', 33, 15, 'NovaA', 'MOVEMENT_TYPE_FACE_DOWN', 2,
        'TRAINER_SKYRIFT_NOVA_BEACH_1',
        'Halt! This beach belongs to\nTEAM NOVA now.\fVEGA\'s orders: no one goes\npast the crystal rock!',
        'The stars... turned on me.',
        'You\'ll never reach the\nOBSERVATORY. SIRIUS is up there.')
    foe(m, 'OBJ_EVENT_GFX_HEX_MANIAC', 33, 17, 'NovaB', 'MOVEMENT_TYPE_FACE_UP', 2,
        'TRAINER_SKYRIFT_NOVA_BEACH_2',
        'Two of you? Then two of us!\nThe rift gives us strength!',
        'Our dream... is it slipping\naway?',
        'VEGA says the ENGINE will bring\nthe sky\'s guardian back to us.')
    foe(m, 'OBJ_EVENT_GFX_SWIMMER_F_WATER', 44, 21, 'Leilani', 'MOVEMENT_TYPE_FACE_LEFT', 5,
        'TRAINER_SKYRIFT_SWIMMER_LEILANI',
        'The water sparkles at night now!\nIt\'s full of tiny stars!',
        'I sank like a stone...',
        'The shards dissolve in the sea.\nThe STARYU eat them, I think!')
    STORY_SPOTS['rift_beach'] = (BEACH, 54, 15)
    wild(BEACH, 'gSkyrift_ShardfallBeach',
         land=[('PALOSSAND', 66, 68), ('PELIPPER', 66, 68), ('EXEGGUTOR_ALOLA', 66, 68), ('ORICORIO_POM_POM', 67, 69),
               ('CRABOMINABLE', 67, 69), ('RIBOMBEE', 67, 69), ('TOUCANNON', 68, 70), ('MINIOR', 68, 70),
               ('CLEFAIRY', 68, 70), ('SANDYGAST', 66, 66), ('MINIOR', 70, 72), ('CLEFABLE', 70, 72)],
         water=[('TENTACRUEL', 66, 68), ('MANTINE', 66, 68), ('STARMIE', 67, 69), ('TOXAPEX', 67, 69),
                ('WISHIWASHI', 68, 70)],
         fishing=[('MAGIKARP', 60, 64), ('STARYU', 64, 66), ('WISHIWASHI', 66, 68), ('STARYU', 66, 68),
                  ('PYUKUMUKU', 66, 68), ('STARMIE', 67, 69), ('BRUXISH', 66, 68), ('BRUXISH', 68, 70),
                  ('GYARADOS', 68, 70), ('DHELMISE', 70, 72)])


def basin(m):
    P = m.name
    m.layout.put(5, 27, g.SIGN)
    signpost(m, 5, 27, P + '_EventScript_Sign', 'METEOR BASIN\fWhere the brightest star of the\nsecond STARFALL came down.')
    for x in (20, 22, 24):
        m.bgs.append(g.sign(x, 23, P + '_EventScript_Meteorite', 'BG_EVENT_PLAYER_FACING_NORTH'))
    m.scripts += (P + '_EventScript_Meteorite::\n\tmsgbox %s_Text_Meteorite, MSGBOX_SIGN\n\tend\n\n' % P
                  + T(P + '_Text_Meteorite',
                      'A huge meteorite, half sunk in\nthe crater floor.\f'
                      'It is still warm. Something deep\ninside it hums, very softly.'))
    item_ball(m, 28, 29, P + '_EventScript_ItemCometShard', 'ITEM_COMET_SHARD', ITEM_BASIN)
    trainer('TRAINER_SKYRIFT_HIKER_MAKOA', 'MAKOA', 'Hiker Frlg', 'Hiker Frlg', 'Male', party(
        ('Golem-Alola', 72, ['Thunder Punch', 'Stone Edge', 'Earthquake', 'Stealth Rock']),
        ('Mudsdale', 72, ['High Horsepower', 'Heavy Slam', 'Rock Slide', 'Close Combat']),
        ('Lycanroc', 72, ['Accelerock', 'Stone Edge', 'Crunch', 'Swords Dance']),
        ('Turtonator @ White Herb', 73, ['Flamethrower', 'Dragon Pulse', 'Body Press', 'Shell Smash'])),
        music='Serious')
    trainer('TRAINER_SKYRIFT_RUIN_MANIAC_HOKU', 'HOKU', 'Ruin Maniac Frlg', 'Ruin Maniac Frlg', 'Male', party(
        ('Carbink', 72, ['Moonblast', 'Power Gem', 'Stealth Rock', 'Reflect']),
        ('Golurk', 72, ['Earthquake', 'Shadow Punch', 'Ice Punch', 'Rock Polish']),
        ('Minior', 73, ['Shell Smash', 'Stone Edge', 'Acrobatics', 'Earthquake'])), music='Serious')
    nova('TRAINER_SKYRIFT_NOVA_BASIN_1', NOVA_M, [
        ('Sigilyph', 72, ['Psychic', 'Air Slash', 'Heat Wave', 'Cosmic Power']),
        ('Claydol', 72, ['Earth Power', 'Psychic', 'Ice Beam', 'Rapid Spin']),
        ('Reuniclus', 73, ['Psychic', 'Focus Blast', 'Shadow Ball', 'Recover'])])
    nova('TRAINER_SKYRIFT_NOVA_BASIN_2', NOVA_F, [
        ('Clefable @ Leftovers', 72, ['Moonblast', 'Cosmic Power', 'Moonlight', 'Flamethrower']),
        ('Musharna', 72, ['Psychic', 'Moonblast', 'Calm Mind', 'Moonlight']),
        ('Lunatone', 72, ['Psychic', 'Power Gem', 'Earth Power', 'Rock Polish']),
        ('Starmie', 73, ['Hydro Pump', 'Psychic', 'Ice Beam', 'Recover'])])
    foe(m, 'OBJ_EVENT_GFX_HIKER_FRLG', 17, 35, 'Makoa', 'MOVEMENT_TYPE_FACE_DOWN', 4,
        'TRAINER_SKYRIFT_HIKER_MAKOA',
        'I climbed down to see the big\nrock. Now I can\'t stop staring!',
        'Rockslide! On me!',
        'The steps cut through the south\nrim. The ledges are a one-way\ntrip down!')
    foe(m, 'OBJ_EVENT_GFX_HIKER_FRLG', 15, 20, 'Hoku',
        'MOVEMENT_TYPE_FACE_RIGHT', 2, 'TRAINER_SKYRIFT_RUIN_MANIAC_HOKU',
        'This meteorite is not from our\nsky. I\'ve measured it!',
        'You hit harder than a meteor!',
        'Sometimes the light inside it\nflickers, like it\'s hungry.\fFor light, maybe?')
    # the Nova pair in the east road's crystal gate
    foe(m, 'OBJ_EVENT_GFX_PSYCHIC_M', 36, 21, 'NovaA', 'MOVEMENT_TYPE_FACE_RIGHT', 2,
        'TRAINER_SKYRIFT_NOVA_BASIN_1',
        'The crater is sacred ground.\nThe second STARFALL hit here!',
        'The crater... rejects me?',
        'VEGA studied this crater for\nmonths. Then she built the\nENGINE.')
    foe(m, 'OBJ_EVENT_GFX_HEX_MANIAC', 38, 21, 'NovaB', 'MOVEMENT_TYPE_FACE_LEFT', 2,
        'TRAINER_SKYRIFT_NOVA_BASIN_2',
        'You and your friend won\'t pass\nthe crystal gate!',
        'The gate is open... Go, then.',
        'North of here are the cliffs,\nand the OBSERVATORY on top.')
    STORY_SPOTS['rift_basin_1'] = (BASIN, 6, 16)
    STORY_SPOTS['rift_basin_2'] = (BASIN, 28, 20)
    STORY_SPOTS['necrozma'] = (BASIN, 21, 24)
    wild(BASIN, 'gSkyrift_MeteorBasin',
         land=[('LUNATONE', 66, 68), ('SOLROCK', 66, 68), ('BRONZOR', 66, 68), ('MINIOR', 67, 69),
               ('CLEFAIRY', 67, 69), ('LYCANROC', 67, 69), ('MUDSDALE', 68, 70), ('GOLEM_ALOLA', 68, 70),
               ('BRONZONG', 69, 71), ('SALAZZLE', 69, 71), ('CLEFABLE', 70, 72), ('BELDUM', 70, 70)])


def cliffs(m):
    P = m.name
    signpost(m, 16, 41, P + '_EventScript_Sign', 'OBSERVATORY CLIFFS\nNOVA OBSERVATORY ahead')
    m.layout.put(11, 8, g.SIGN)
    signpost(m, 11, 8, P + '_EventScript_ObsSign',
             'NOVA OBSERVATORY\fWe watch the sky so the sky\nwill answer us.  - VEGA')
    door(m, 14, 7, OBS1, 0)
    item_ball(m, 4, 32, P + '_EventScript_ItemPPMax', 'ITEM_PP_MAX', ITEM_CLIFFS)
    trainer('TRAINER_SKYRIFT_BLACK_BELT_KANOA', 'KANOA', 'Black Belt Frlg', 'Black Belt Frlg', 'Male', party(
        ('Hawlucha', 72, ['Acrobatics', 'Close Combat', 'Poison Jab', 'Swords Dance']),
        ('Crabominable', 72, ['Ice Hammer', 'Close Combat', 'Earthquake', 'Thunder Punch']),
        ('Passimian', 72, ['Close Combat', 'U-turn', 'Knock Off', 'Gunk Shot']),
        ('Kommo-o @ Sitrus Berry', 73, ['Dragon Claw', 'Close Combat', 'Poison Jab', 'Dragon Dance'])),
        music='Serious')
    trainer('TRAINER_SKYRIFT_COOLTRAINER_MAILE', 'MAILE', 'Cooltrainer Frlg', 'Cooltrainer F Frlg', 'Female', party(
        ('Ribombee', 72, ['Moonblast', 'Bug Buzz', 'Quiver Dance', 'Psychic']),
        ('Salazzle', 72, ['Flamethrower', 'Sludge Wave', 'Dragon Pulse', 'Nasty Plot']),
        ('Vikavolt', 72, ['Thunderbolt', 'Bug Buzz', 'Energy Ball', 'Flash Cannon']),
        ('Golisopod @ Leftovers', 73, ['First Impression', 'Liquidation', 'Leech Life', 'Aqua Jet'])),
        music='Female')
    nova('TRAINER_SKYRIFT_NOVA_CLIFFS', NOVA_M, [
        ('Beheeyem', 73, ['Psychic', 'Shadow Ball', 'Thunderbolt', 'Trick Room']),
        ('Bronzong', 73, ['Gyro Ball', 'Earthquake', 'Psychic', 'Stealth Rock']),
        ('Metagross @ Life Orb', 74, ['Meteor Mash', 'Zen Headbutt', 'Earthquake', 'Bullet Punch'])])
    foe(m, 'OBJ_EVENT_GFX_BLACK_BELT_FRLG', 10, 37, 'Kanoa', 'MOVEMENT_TYPE_FACE_DOWN', 3,
        'TRAINER_SKYRIFT_BLACK_BELT_KANOA',
        'I climb these cliffs every\nmorning. Two at a time? Fine!',
        'My legs... are jelly...',
        'Use the steps going up. Going\ndown, the ledge is quicker.')
    foe(m, 'OBJ_EVENT_GFX_COOLTRAINER_F', 14, 26, 'Maile', 'MOVEMENT_TYPE_FACE_DOWN', 3,
        'TRAINER_SKYRIFT_COOLTRAINER_MAILE',
        'I came to see the OBSERVATORY.\nThey wouldn\'t let me in!\fSo battle me instead!',
        'That cheered me up!',
        'The big man at the top says the\ndoor is closed. For everyone.')
    foe(m, 'OBJ_EVENT_GFX_PSYCHIC_M', 18, 17, 'Nova', 'MOVEMENT_TYPE_FACE_UP', 2,
        'TRAINER_SKYRIFT_NOVA_CLIFFS',
        'You climbed all this way? Then\nyou\'ll fall all this way!',
        'Fell... right off the cliff...',
        'SIRIUS guards the gate up there.\nHe has never lost. Not once.')
    STORY_SPOTS['rift_cliffs_1'] = (CLIFFS, 24, 32)
    STORY_SPOTS['rift_cliffs_2'] = (CLIFFS, 7, 18)
    STORY_SPOTS['sirius_gate'] = (CLIFFS, 14, 12)
    wild(CLIFFS, 'gSkyrift_ObservatoryCliffs',
         land=[('ELGYEM', 66, 68), ('CLEFAIRY', 66, 68), ('BEHEEYEM', 67, 69), ('HAWLUCHA', 67, 69),
               ('BRONZONG', 67, 69), ('MINIOR', 68, 70), ('TOGEDEMARU', 68, 70), ('ORICORIO_SENSU', 68, 70),
               ('KOMALA', 69, 71), ('DRAMPA', 69, 71), ('BEHEEYEM', 70, 72), ('BELDUM', 70, 70)])


def observatory(o1, o2, rf, cliffs_map):
    # 1F: the lobby
    P = o1.name
    mx = OW // 2
    for k in (mx - 1, mx, mx + 1):
        door(o1, k, OH - 2, cliffs_map.id, 0)
    door(o1, 16, 2, OBS2, 0)              # warp 3: stairs up
    signpost(o1, 16, 9, P + '_EventScript_Cases',
             'Glass cases full of meteorite\nchips, each with a label:\f"METEOR BASIN, SECOND STARFALL."')
    o1.bgs.append(g.sign(15, 9, P + '_EventScript_Cases', 'BG_EVENT_PLAYER_FACING_ANY'))
    o1.bgs.append(g.sign(17, 9, P + '_EventScript_Cases', 'BG_EVENT_PLAYER_FACING_ANY'))
    nova('TRAINER_SKYRIFT_NOVA_1F_1', NOVA_M, [
        ('Gothitelle', 73, ['Psychic', 'Shadow Ball', 'Thunderbolt', 'Trick Room']),
        ('Porygon-Z', 73, ['Tri Attack', 'Thunderbolt', 'Ice Beam', 'Nasty Plot']),
        ('Magnezone @ Leftovers', 73, ['Thunderbolt', 'Flash Cannon', 'Volt Switch', 'Body Press'])])
    nova('TRAINER_SKYRIFT_NOVA_1F_2', NOVA_SCI, [
        ('Porygon2 @ Eviolite', 73, ['Tri Attack', 'Ice Beam', 'Thunderbolt', 'Recover']),
        ('Klinklang', 73, ['Gear Grind', 'Wild Charge', 'Shift Gear', 'Zen Headbutt']),
        ('Starmie', 73, ['Hydro Pump', 'Psychic', 'Thunderbolt', 'Recover']),
        ('Beheeyem', 74, ['Psychic', 'Shadow Ball', 'Dazzling Gleam', 'Calm Mind'])])
    # the pair guards the stairs: the way up is between them
    foe(o1, 'OBJ_EVENT_GFX_PSYCHIC_M', 12, 4, 'NovaA', 'MOVEMENT_TYPE_FACE_RIGHT', 7,
        'TRAINER_SKYRIFT_NOVA_1F_1',
        'Intruders in the OBSERVATORY!\nSound the alarm!',
        'Alarm... off...',
        'The lab is upstairs. VEGA is\nabove that, on the roof.')
    foe(o1, 'OBJ_EVENT_GFX_SCIENTIST', 20, 4, 'NovaB', 'MOVEMENT_TYPE_FACE_LEFT', 7,
        'TRAINER_SKYRIFT_NOVA_1F_2',
        'I built half the machines in\nthis building. Let me show you!',
        'My calculations were off...',
        'The ENGINE draws its power from\nthe rift itself. Brilliant, no?')
    # 2F: the lab
    P = o2.name
    door(o2, 3, 4, OBS1, 3)               # warp 0: stairs down
    door(o2, 18, 4, ROOF, 0)              # warp 1: stairs up to the roof
    for x in (6, 7, 9, 13, 14):
        o2.bgs.append(g.sign(x, 4, P + '_EventScript_Machine', 'BG_EVENT_PLAYER_FACING_NORTH'))
    o2.scripts += (P + '_EventScript_Machine::\n\tmsgbox %s_Text_Machine, MSGBOX_SIGN\n\tend\n\n' % P
                   + T(P + '_Text_Machine',
                       'A big machine, humming. Cables\nfrom it run up through the\nceiling.\f'
                       'A note: "ENGINE POWER LINE.\nDO NOT TOUCH."'))
    item_ball(o2, 2, 13, P + '_EventScript_ItemMaxElixir', 'ITEM_MAX_ELIXIR', ITEM_OBS)
    nova('TRAINER_SKYRIFT_NOVA_2F_1', NOVA_SCI, [
        ('Cryogonal', 74, ['Ice Beam', 'Freeze-Dry', 'Recover', 'Rapid Spin']),
        ('Rotom-Wash', 74, ['Hydro Pump', 'Volt Switch', 'Will-O-Wisp', 'Thunderbolt']),
        ('Claydol', 74, ['Earth Power', 'Psychic', 'Ice Beam', 'Rapid Spin'])])
    nova('TRAINER_SKYRIFT_NOVA_2F_2', NOVA_F, [
        ('Espeon', 74, ['Psychic', 'Dazzling Gleam', 'Shadow Ball', 'Morning Sun']),
        ('Reuniclus', 74, ['Psychic', 'Focus Blast', 'Shadow Ball', 'Recover']),
        ('Hatterene @ Life Orb', 74, ['Psychic', 'Dazzling Gleam', 'Mystical Fire', 'Trick Room'])])
    foe(o2, 'OBJ_EVENT_GFX_SCIENTIST', 11, 11, 'Researcher', 'MOVEMENT_TYPE_FACE_DOWN', 2,
        'TRAINER_SKYRIFT_NOVA_2F_1',
        'Don\'t touch the machines!\nThey feed the ENGINE!',
        'My readings... ruined!',
        'The ENGINE is calling DEOXYS\nback through the rift. Or so\nVEGA says.')
    foe(o2, 'OBJ_EVENT_GFX_HEX_MANIAC', 18, 7, 'NovaB', 'MOVEMENT_TYPE_FACE_UP', 2,
        'TRAINER_SKYRIFT_NOVA_2F_2',
        'The roof is VEGA\'s alone.\nTurn back, or be turned back!',
        'I couldn\'t hold the line...',
        'Go up, then. But SIRIUS won\'t\nlet her fall without a fight.')
    # the roof: no people (the climax comes from skyrift_story.py)
    P = rf.name
    door(rf, 4, 13, OBS2, 1)
    for x in (8, 10):
        rf.bgs.append(g.sign(x, 6, P + '_EventScript_Engine', 'BG_EVENT_PLAYER_FACING_NORTH'))
    rf.scripts += (P + '_EventScript_Engine::\n\tmsgbox %s_Text_Engine, MSGBOX_SIGN\n\tend\n\n' % P
                   + T(P + '_Text_Engine',
                       'The SKYRIFT ENGINE. Its masts\npoint at the tear in the sky.\f'
                       'Deep inside, something throbs\nlike a heartbeat.'))
    STORY_SPOTS['vega'] = (ROOF, 9, 7)
    STORY_SPOTS['sirius'] = (ROOF, 10, 7)
    STORY_SPOTS['climax_trigger'] = (ROOF, 9, 9)

def build():
    t = add(g.MapDef(P_HARBOR, HARBOR, SEC_HARBOR, harbor_canvas(), 'MUS_LILYCOVE', 'MAP_TYPE_TOWN',
                     border=g.SEA_BORDER))
    harbor(t)
    b = add(g.MapDef(P_BEACH, BEACH, SEC_BEACH, beach_canvas(), 'MUS_ROUTE119', 'MAP_TYPE_ROUTE'))
    link(HARBOR, 'right', 6, BEACH)       # harbor road y14-15 = beach road y8-9
    beach(b)
    bs = add(g.MapDef(P_BASIN, BASIN, SEC_BASIN, basin_canvas(), 'MUS_ROUTE120', 'MAP_TYPE_ROUTE',
                      weather='WEATHER_SHADE'))
    link(BEACH, 'right', -22, BASIN)      # beach road y6-7 = basin road y28-29
    basin(bs)
    cl = add(g.MapDef(P_CLIFFS, CLIFFS, SEC_CLIFFS, cliffs_canvas(), 'MUS_ABNORMAL_WEATHER', 'MAP_TYPE_ROUTE',
                      weather='WEATHER_SHADE'))
    link(BASIN, 'up', 2, CLIFFS)          # basin road x16-17 = cliffs road x14-15
    cliffs(cl)
    o1 = add(g.MapDef(P_OBS1, OBS1, SEC_OBS, obs1_room(), 'MUS_RG_SILPH', 'MAP_TYPE_INDOOR', 'WEATHER_NONE',
                      True, False, True, True, border=[0x008] * 4, floor=1))
    o2 = add(g.MapDef(P_OBS2, OBS2, SEC_OBS, obs2_room(), 'MUS_RG_SILPH', 'MAP_TYPE_INDOOR', 'WEATHER_NONE',
                      True, False, True, True, border=[0x008] * 4, floor=2))
    rf = add(g.MapDef(P_ROOF, ROOF, SEC_OBS, roof_room(), 'MUS_ABNORMAL_WEATHER', 'MAP_TYPE_INDOOR',
                      'WEATHER_NONE', True, False, True, False, border=[0x32F] * 4, floor=127))   # sky all round
    observatory(o1, o2, rf, cl)
