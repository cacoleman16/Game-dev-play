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
SEC_OBS = mapsec('MAPSEC_SKYRIFT_OBSERVATORY', 'NOVA OBSERVATORY')

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


def build():
    t = add(g.MapDef(P_HARBOR, HARBOR, SEC_HARBOR, harbor_canvas(), 'MUS_LILYCOVE', 'MAP_TYPE_TOWN',
                     border=g.SEA_BORDER))
    harbor(t)
