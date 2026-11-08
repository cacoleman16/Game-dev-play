"""Silkhollow: Hollow Path, Silkhollow Town (Gym 6, Fairy), Weaver's Road and
Frostmere Village (Gym 7, Ice)."""
from common import (g, T, add, door, npc, signpost, pokecenter, mart, house, wild, mapsec,
                    trainer, battle, flag, link, PC, MART, GYM)

PEWTER = 'LAYOUT_PEWTER_CITY'
FOUR = 'LAYOUT_FOUR_ISLAND'

SEC_PATH = mapsec('MAPSEC_SEIKA_HOLLOW_PATH', 'HOLLOW PATH')
SEC_SILK = mapsec('MAPSEC_SEIKA_SILKHOLLOW', 'SILKHOLLOW TOWN')
SEC_ROAD = mapsec('MAPSEC_SEIKA_WEAVERS_ROAD', 'WEAVER\'S ROAD')
SEC_FROST = mapsec('MAPSEC_SEIKA_FROSTMERE', 'FROSTMERE')

BADGE6 = flag('FLAG_QOL_SEIKA_BADGE06', 'SILK BADGE (Ayame)')
BADGE7 = flag('FLAG_QOL_SEIKA_BADGE07', 'FROST BADGE (Rime)')
HIDE_ALTAIR = flag('FLAG_QOL_SEIKA_HIDE_ALTAIR_HOLLOW', 'Altair left Hollow Path')
HIDE_GRUNTS = flag('FLAG_QOL_SEIKA_HIDE_GRUNTS_FROSTMERE', 'Nova grunts left Frostmere')

MART_SILK = ['ITEM_ULTRA_BALL', 'ITEM_GREAT_BALL', 'ITEM_QUICK_BALL', 'ITEM_HYPER_POTION', 'ITEM_MAX_POTION',
             'ITEM_FULL_RESTORE', 'ITEM_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL', 'ITEM_MOON_STONE',
             'ITEM_SILK_SCARF']
MART_FROST = ['ITEM_ULTRA_BALL', 'ITEM_GREAT_BALL', 'ITEM_DIVE_BALL', 'ITEM_HYPER_POTION', 'ITEM_MAX_POTION',
              'ITEM_FULL_RESTORE', 'ITEM_REVIVE', 'ITEM_MAX_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL',
              'ITEM_ICE_STONE', 'ITEM_NEVER_MELT_ICE']

IV31 = 'IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe'


# ---------------------------------------------------------------- painting helpers

# 9-slice tile sets: edges, corners and inner corners (learned from Kanto's maps)
ROCK = dict(tl=0x068, t=0x069, tr=0x06A, l=0x070, c=0x071, r=0x072, bl=0x078, b=0x079, br=0x07A,
            inw=0x0BB, ine=0x0BA, isw=0x0B3, ise=0x0B2)
PAVE = dict(tl=0x1B8, t=0x1B9, tr=0x1BA, l=0x1C0, c=0x1C1, r=0x1C2, bl=0x1C8, b=0x1C9, br=0x1CA,
            inw=0x1BB, ine=0x1BC, isw=0x1C3, ise=0x1C4)
SMALL_ROCK = 0x09F


class Blobs:
    """Cells painted with a 9-slice tile set once everything else is placed.
    oob(x, y) says whether a cell outside the map counts as part of the blob."""

    def __init__(self, tiles, oob=None):
        self.tiles, self.cells, self.ghost = tiles, set(), set()
        self.oob = oob or (lambda x, y: False)

    def rect(self, x, y, w, h):
        self.cells |= {(xx, yy) for yy in range(y, y + h) for xx in range(x, x + w)}

    def cut(self, x, y, w, h):
        self.cells -= {(xx, yy) for yy in range(y, y + h) for xx in range(x, x + w)}

    def paint(self, c):
        t = self.tiles

        def has(x, y):
            if not (0 <= x < c.w and 0 <= y < c.h):
                return self.oob(x, y)
            return (x, y) in self.cells or (x, y) in self.ghost
        for (x, y) in self.cells:
            if not (0 <= x < c.w and 0 <= y < c.h):
                continue
            n, s, w, e = has(x, y - 1), has(x, y + 1), has(x - 1, y), has(x + 1, y)
            if not n:
                k = 'tl' if not w else 'tr' if not e else 't'
            elif not s:
                k = 'bl' if not w else 'br' if not e else 'b'
            elif not w:
                k = 'l'
            elif not e:
                k = 'r'
            elif not has(x - 1, y - 1):
                k = 'inw'
            elif not has(x + 1, y - 1):
                k = 'ine'
            elif not has(x - 1, y + 1):
                k = 'isw'
            elif not has(x + 1, y + 1):
                k = 'ise'
            else:
                k = 'c'
            c.put(x, y, t[k])
            c.cls[y][x] = 'fixed'


# Kanto's pond rim: drawn inside the water at the top and sides; the grass
# row under the water gets the lip instead
LAKE = dict(tl=0x1A5, t=0x1A6, tr=0x1A7, l=0x1AD, c=0x1AE, r=0x1AF, bl=0x1AD, b=0x1AE, br=0x1AF,
            inw=0x1B4, ine=0x1B3, isw=0x1AE, ise=0x1AE)
LAKE_LIP = 0x133


class Lake(Blobs):
    def __init__(self):
        Blobs.__init__(self, LAKE)

    def paint(self, c):
        Blobs.paint(self, c)
        for (x, y) in self.cells:
            if (x, y + 1) not in self.cells and y + 1 < c.h:
                c.put(x, y + 1, LAKE_LIP)
                c.cls[y + 1][x] = 'fixed'


# ---------------------------------------------------------------- Hollow Path

def hollow_path():
    W, H = 24, 56
    c = g.Canvas(W, H, 'gTileset_PewterCity', seed=61)
    rock = Blobs(ROCK, oob=lambda x, y: 0 <= y < H)
    # north and south ends: trees around the 6-tile openings (x 8..13)
    c.forest(0, 0, 8, 4)
    c.forest(14, 0, 10, 4)
    c.forest(4, 4, 4, 2)
    c.forest(14, 4, 6, 2)
    c.forest(0, 52, 8, 4)
    c.forest(14, 52, 10, 4)
    c.forest(4, 50, 4, 2)
    c.forest(14, 50, 6, 2)
    # canyon walls
    rock.rect(0, 4, 4, 48)
    rock.rect(20, 4, 4, 48)
    # the narrows where Altair waits
    rock.rect(4, 8, 5, 6)
    rock.rect(13, 8, 7, 6)
    # outcrops
    rock.rect(4, 30, 5, 10)
    rock.rect(17, 18, 3, 7)
    rock.rect(4, 14, 2, 3)
    rock.rect(12, 20, 2, 3)
    # path
    c.rect(10, 0, 2, 18, 'path')
    c.rect(7, 16, 5, 2, 'path')
    c.rect(7, 16, 2, 12, 'path')
    c.rect(7, 26, 10, 2, 'path')
    c.rect(15, 26, 2, 18, 'path')
    c.rect(10, 42, 7, 2, 'path')
    c.rect(10, 42, 2, 14, 'path')
    # tall grass
    c.rect(13, 45, 6, 5, 'tall')
    c.rect(9, 30, 5, 8, 'tall')
    c.rect(4, 19, 3, 6, 'tall')
    c.rect(12, 14, 6, 3, 'tall')
    c.rect(17, 30, 3, 8, 'tall')
    # ledges (jump south)
    c.ledge(4, 41, 6)
    c.ledge(9, 24, 6)
    rock.paint(c)
    for x, y in ((5, 46), (18, 41), (6, 26), (18, 28)):
        c.put(x, y, SMALL_ROCK)
    return c


# ---------------------------------------------------------------- Silkhollow Town

HOUSE_PEW = (PEWTER, 32, 8, 5, 4, 1, 3)      # gray-roofed Pewter house, door (1, 3)
GYM_SIGN_PEW = (PEWTER, 11, 15, 1, 2)


def silkhollow_town():
    W, H = 36, 30
    c = g.Canvas(W, H, 'gTileset_PewterCity', seed=62)
    pave = Blobs(PAVE)
    c.forest(0, 0, 36, 6)
    c.forest(0, 6, 8, 4)                  # west opening: y 10..15
    c.forest(0, 16, 8, 8)
    c.forest(28, 6, 8, 18)
    c.forest(0, 24, 16, 6)                # south opening: x 16..21
    c.forest(22, 24, 14, 6)
    # roads out of town: Route-style sand
    c.rect(0, 12, 8, 2, 'path')
    c.rect(18, 24, 2, 6, 'path')
    # stone streets (Pewter's paving) and the dance square
    pave.rect(8, 11, 19, 3)
    pave.rect(15, 14, 8, 6)
    pave.rect(9, 19, 18, 2)
    pave.rect(18, 21, 2, 3)
    pave.ghost |= {(7, 12), (7, 13), (18, 24), (19, 24)}
    # gardens
    c.rect(14, 21, 4, 2, 'flowers')
    c.rect(20, 21, 4, 2, 'flowers')
    c.rect(8, 21, 2, 2, 'flowers')
    c.rect(26, 21, 2, 2, 'flowers')
    c.rect(9, 14, 4, 1, 'flowers')
    c.rect(23, 14, 4, 1, 'flowers')
    pave.paint(c)
    for x, y in ((8, 12), (8, 13), (18, 23), (19, 23)):
        c.cls[y][x] = 'path'              # the sand roads run on into the paving
    # buildings
    for st, x, y in ((PC, 9, 6), (GYM, 15, 6), (MART, 23, 7)):
        c.stamp(*st[:5], x, y)            # doors: PC (11,10), Gym (18,10), Mart (25,10)
    c.stamp(*GYM_SIGN_PEW, 14, 9)
    c.stamp(*HOUSE_PEW[:5], 9, 15)       # weaver's house, door (10,18)
    c.stamp(*HOUSE_PEW[:5], 23, 15)      # dancer's house, door (24,18)
    c.tree(10, 22)
    c.tree(24, 22)
    c.put(17, 22, g.SIGN)
    c.put(6, 11, g.SIGN)
    return c


# ---------------------------------------------------------------- Weaver's Road

def weavers_road():
    W, H = 64, 24
    c = g.Canvas(W, H, 'gTileset_SeviiIslands45', seed=63)
    c.forest(0, 0, 64, 4)
    c.forest(0, 20, 64, 4)
    for x in (0, 60):                     # both ends: openings at y 10..15
        c.forest(x, 4, 4, 6)
        c.forest(x, 16, 4, 4)
    # the road
    c.rect(52, 12, 12, 2, 'path')
    c.rect(52, 6, 2, 8, 'path')
    c.rect(36, 6, 18, 2, 'path')
    c.rect(36, 6, 2, 12, 'path')
    c.rect(22, 16, 16, 2, 'path')
    c.rect(22, 12, 2, 6, 'path')
    c.rect(0, 12, 24, 2, 'path')
    # groves
    c.forest(4, 4, 6, 4)
    c.forest(56, 16, 4, 4)
    c.forest(26, 4, 8, 2)
    c.forest(40, 16, 8, 4)
    for x, y in ((16, 8), (30, 14), (56, 8), (12, 16), (28, 10)):
        c.tree(x, y)
    # tall grass
    c.rect(10, 5, 10, 5, 'tall')
    c.rect(4, 15, 8, 4, 'tall')
    c.rect(25, 7, 3, 7, 'tall')
    c.rect(54, 14, 2, 5, 'tall')
    c.rect(48, 15, 6, 4, 'tall')
    c.rect(38, 9, 3, 6, 'tall')
    # flowers and the pond
    c.rect(14, 16, 6, 3, 'flowers')
    c.rect(30, 9, 4, 4, 'flowers')
    c.rect(56, 5, 4, 2, 'flowers')
    c.pond(42, 9, 8, 4)
    c.ledge(24, 15, 6)
    c.ledge(4, 11, 6)
    c.put(57, 11, g.SIGN)
    c.put(5, 14, g.SIGN)
    return c


# ---------------------------------------------------------------- Frostmere Village

FOUR_HOUSE = (FOUR, 24, 23, 5, 4, 1, 3)      # purple-roofed house, door (1, 3)
FOUR_SMALL = (FOUR, 11, 10, 4, 4, 1, 3)      # orange-roofed cottage


def frostmere_town():
    W, H = 36, 32
    c = g.Canvas(W, H, 'gTileset_SeviiIslands45', seed=64)
    lake = Lake()
    c.forest(0, 0, 14, 6)                 # north opening: x 14..19
    c.forest(20, 0, 16, 6)
    c.forest(0, 6, 8, 22)
    c.forest(28, 6, 8, 4)                 # east opening: y 10..15
    c.forest(28, 16, 8, 12)
    c.forest(0, 28, 36, 4)
    # the lake; its north shore is wooded, so nobody can surf past the north road
    c.forest(8, 6, 8, 2)
    lake.rect(8, 8, 8, 4)
    lake.rect(8, 12, 6, 10)
    # roads
    c.rect(16, 0, 2, 14, 'path')          # north to Frost Road
    c.rect(16, 12, 20, 2, 'path')         # east to Weaver's Road
    c.rect(19, 12, 2, 10, 'path')
    c.rect(14, 20, 12, 2, 'path')
    c.rect(20, 22, 2, 4, 'path')
    c.rect(10, 26, 12, 2, 'path')
    # gardens
    c.rect(27, 6, 1, 4, 'flowers')
    c.rect(22, 26, 6, 2, 'flowers')
    lake.paint(c)
    # buildings
    c.stamp(*FOUR_HOUSE[:5], 18, 6)       # door (19,9)
    c.stamp(*MART[:5], 23, 6)             # door (25,9)
    c.stamp(*PC[:5], 14, 15)              # door (16,19)
    c.stamp(*GYM[:5], 21, 15)             # door (24,19)
    c.stamp(*FOUR_HOUSE[:5], 14, 22)      # door (15,25)
    c.stamp(*FOUR_SMALL[:5], 23, 22)      # door (24,25)
    c.put(26, 20, g.SIGN)
    c.put(27, 11, g.SIGN)
    return c


# ---------------------------------------------------------------- shared script bits

def leader_script(name, who, const, badge, tm):
    """Wick's pattern: battle, badge flag, a TM, then the post-battle text."""
    return ('%s_EventScript_%s::\n'
            '\ttrainerbattle_single %s, %s_Text_%sIntro, %s_Text_%sDefeat, %s_EventScript_%sDefeated, NO_MUSIC\n'
            '\tmsgbox %s_Text_%sPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            '%s_EventScript_%sDefeated::\n'
            '\tsetflag %s\n'
            '\tmsgbox %s_Text_%sGift, MSGBOX_DEFAULT\n'
            '\tgiveitem %s\n'
            '\tmsgbox %s_Text_%sPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            % (name, who, const, name, who, name, who, name, who, name, who, name, who, badge, name, who, tm,
               name, who))


def gym_guy_script(name, badge):
    return (name + '_EventScript_GymGuy::\n\tlock\n\tfaceplayer\n'
            '\tgoto_if_set %s, %s_EventScript_GymGuyWon\n'
            '\tmsgbox %s_Text_GymGuy, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            '%s_EventScript_GymGuyWon::\n\tmsgbox %s_Text_GymGuyWon, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            % (badge, name, name, name, name))


def badge_text(badge, leader):
    return ('{FONT_NORMAL}{PLAYER} received the %s\nfrom %s!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}'
            '{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}' % (badge, leader))


# ---------------------------------------------------------------- Hollow Path content

def hollow(hp):
    P = hp.name
    signpost(hp, 12, 52, P + '_EventScript_Sign', 'HOLLOW PATH\nTIDEWATCH TOWN - SILKHOLLOW TOWN')
    hp.layout.put(12, 52, g.SIGN)
    trainer('TRAINER_SEIKA_HIKER_IWAO', 'IWAO', 'Hiker Frlg', 'Hiker Frlg', 'Male', """
Golem
Level: 54
- Rock Slide
- Earthquake
- Explosion
- Stealth Rock

Sudowoodo
Level: 54
- Wood Hammer
- Head Smash
- Sucker Punch
- Low Kick

Donphan
Level: 55
- Earthquake
- Ice Shard
- Knock Off
- Rapid Spin
""")
    trainer('TRAINER_SEIKA_BLACK_BELT_KENTA', 'KENTA', 'Black Belt Frlg', 'Black Belt Frlg', 'Male', """
Hitmontop
Level: 55
- Close Combat
- Sucker Punch
- Triple Axel
- Rapid Spin

Heracross
Level: 56
- Megahorn
- Close Combat
- Stone Edge
- Knock Off
""")
    trainer('TRAINER_SEIKA_PICNICKER_SAKI', 'SAKI', 'Picnicker Frlg', 'Picnicker Frlg', 'Female', """
Granbull
Level: 53
- Play Rough
- Crunch
- Thunder Fang
- Bulk Up

Jumpluff
Level: 54
- Acrobatics
- Leech Seed
- Sleep Powder
- U-turn

Ampharos
Level: 54
- Thunderbolt
- Dragon Pulse
- Power Gem
- Thunder Wave
""", music='Female')
    trainer('TRAINER_SEIKA_NOVA_ALTAIR_HOLLOW', 'ALTAIR', 'Nova Admin', 'Ruin Maniac', 'Male', """
Solrock
Level: 57
%(iv)s
- Flare Blitz
- Stone Edge
- Zen Headbutt
- Earthquake

Claydol
Level: 57
%(iv)s
- Earth Power
- Psychic
- Ice Beam
- Stealth Rock

Minior Meteor Red
Level: 58
%(iv)s
- Shell Smash
- Power Gem
- Acrobatics
- Earthquake

Clefable @ Sitrus Berry
Level: 59
%(iv)s
- Moonblast
- Cosmic Power
- Moonlight
- Meteor Mash
""" % {'iv': IV31}, music='Suspicious')
    hp.objects.append(g.obj('OBJ_EVENT_GFX_HIKER_FRLG', 12, 47, P + '_EventScript_Iwao',
                            move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    hp.objects.append(g.obj('OBJ_EVENT_GFX_BLACK_BELT_FRLG', 14, 34, P + '_EventScript_Kenta',
                            move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    hp.objects.append(g.obj('OBJ_EVENT_GFX_PICNICKER_FRLG', 9, 21, P + '_EventScript_Saki',
                            move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    hp.scripts += battle(P + '_EventScript_Iwao', 'TRAINER_SEIKA_HIKER_IWAO',
                         'These walls echo like a drum.\nLet\'s make some noise!',
                         'My ears are ringing!',
                         'Up ahead the cliffs close in.\nThat\'s why they call it HOLLOW.', P)
    hp.scripts += battle(P + '_EventScript_Kenta', 'TRAINER_SEIKA_BLACK_BELT_KENTA',
                         'I train by punching boulders!\nYou look softer than a boulder!',
                         'Ouch! Harder than a boulder!',
                         'HERACROSS can lift a hundred\ntimes its own weight. Amazing!', P)
    hp.scripts += battle(P + '_EventScript_Saki', 'TRAINER_SEIKA_PICNICKER_SAKI',
                         'I\'m off to see the silk dancers!\nBut first, a battle!',
                         'You danced circles around me!',
                         'SILKHOLLOW is just north of here.\nIts GYM LEADER is a weaver.', P)
    # Altair waits in the narrows: his sight covers the whole pass (x 9..11)
    hp.objects.append(g.obj('OBJ_EVENT_GFX_HIKER_FRLG', 12, 10, P + '_EventScript_Altair',
                            local_id='LOCALID_SEIKA_HOLLOW_PATH_ALTAIR', move='MOVEMENT_TYPE_FACE_LEFT',
                            trainer='TRAINER_TYPE_NORMAL', sight=3, flag=HIDE_ALTAIR))
    hp.scripts += (P + '_EventScript_Altair::\n'
                   '\ttrainerbattle_single TRAINER_SEIKA_NOVA_ALTAIR_HOLLOW, %s_Text_AltairIntro, %s_Text_AltairDefeat, %s_EventScript_AltairLeaves\n'
                   '\tend\n\n'
                   '%s_EventScript_AltairLeaves::\n'
                   '\tmsgbox %s_Text_AltairAfter, MSGBOX_DEFAULT\n'
                   '\tclosemessage\n'
                   '\tapplymovement LOCALID_SEIKA_HOLLOW_PATH_ALTAIR, %s_Movement_AltairLeave\n'
                   '\twaitmovement 0\n'
                   '\tremoveobject LOCALID_SEIKA_HOLLOW_PATH_ALTAIR\n'
                   '\tsetflag %s\n'
                   '\trelease\n\tend\n\n'
                   '%s_Movement_AltairLeave:\n'
                   '\twalk_fast_up\n\twalk_fast_up\n\twalk_fast_up\n\twalk_fast_up\n\twalk_fast_up\n\tstep_end\n\n'
                   % (P, P, P, P, P, P, HIDE_ALTAIR, P))
    hp.scripts += T(P + '_Text_AltairIntro',
                    'Well, well. The kid from KANTO,\n'
                    'all the way out here.\f'
                    'Look what these old rocks gave\n'
                    'me. A STAR STONE, left by the\n'
                    'first STARFALL!\f'
                    'DENEB has her STARGLASS BELL.\n'
                    'But the bell can\'t ring without\n'
                    'a stone like this one.\f'
                    'Now it can. And you\'re not\n'
                    'taking it from me!')
    hp.scripts += T(P + '_Text_AltairDefeat', 'Bah! Rocks never lie...\nbut they don\'t win battles.')
    hp.scripts += T(P + '_Text_AltairAfter',
                    'Keep your BADGES, kid. I have\n'
                    'what I came for.\f'
                    'When that bell rings, the WISHING\n'
                    'STAR comes down to us. Not you!')
    wild('MAP_SEIKA_HOLLOW_PATH', 'gSeika_HollowPath',
         land=[('GRAVELER', 53, 55), ('GLIGAR', 53, 55), ('DONPHAN', 54, 56), ('URSARING', 54, 56),
               ('SUDOWOODO', 54, 56), ('PUPITAR', 54, 55), ('SKARMORY', 55, 56), ('DUNSPARCE', 54, 56),
               ('MAGCARGO', 55, 55), ('GRANBULL', 55, 55), ('HERACROSS', 56, 56), ('STEELIX', 56, 56)])


# ---------------------------------------------------------------- Silkhollow content

def silkhollow(t):
    P = t.name
    w_pc = door(t, 11, 10, 'MAP_SEIKA_SILKHOLLOW_POKEMON_CENTER_1F', 0)
    w_gym = door(t, 18, 10, 'MAP_SEIKA_SILKHOLLOW_GYM', 0)
    w_mart = door(t, 25, 10, 'MAP_SEIKA_SILKHOLLOW_MART', 0)
    w_h1 = door(t, 10, 18, 'MAP_SEIKA_SILKHOLLOW_HOUSE1', 0)
    w_h2 = door(t, 24, 18, 'MAP_SEIKA_SILKHOLLOW_HOUSE2', 0)
    signpost(t, 17, 22, P + '_EventScript_TownSign',
             'SILKHOLLOW TOWN\n'
             'Where silk is spun from\n'
             'moonlight.')
    signpost(t, 14, 10, P + '_EventScript_GymSign',
             'SILKHOLLOW POKéMON GYM\n'
             'LEADER: AYAME\fThe weaver who spins every\n'
             'battle into silk!')
    signpost(t, 6, 11, P + '_EventScript_RoadSign', 'WEAVER\'S ROAD\nWest to FROSTMERE VILLAGE')
    npc(t, 'OBJ_EVENT_GFX_BEAUTY_FRLG', 17, 16, P + '_EventScript_Dancer',
        'We dance on this square every\n'
        'night of the full moon.\fAYAME dances with us. Her steps\n'
        'are as light as a JUMPLUFF!', 'MOVEMENT_TYPE_WANDER_AROUND', rx=2, ry=1)
    npc(t, 'OBJ_EVENT_GFX_WOMAN_3_FRLG', 21, 18, P + '_EventScript_Weaver',
        'Our silk comes from ARIADOS.\n'
        'Their thread never breaks.\fWe dye it with flowers and weave\n'
        'it by lantern light.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_OLD_MAN_1', 12, 13, P + '_EventScript_OldMan',
        'WEAVER\'S ROAD runs west to\n'
        'FROSTMERE VILLAGE.\fThe wind off the lake there cuts\n'
        'like a knife. Bring a scarf!', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_LITTLE_GIRL_FRLG', 20, 22, P + '_EventScript_Girl',
        'A man with muddy gloves ran\n'
        'through town and out to the west.\fHe was holding a shiny stone,\n'
        'and laughing to himself!', 'MOVEMENT_TYPE_WANDER_AROUND')
    pokecenter('Silkhollow', SEC_SILK, t, w_pc, 'HEAL_LOCATION_SEIKA_SILKHOLLOW', npcs=[
        ('OBJ_EVENT_GFX_GENTLEMAN_FRLG', 10, 5,
         'Up north, the clouds never leave\n'
         'the top of MT. SKYREACH.\fThey say the WISHING STAR sleeps\n'
         'up there, above the clouds.'),
    ])
    mart('Silkhollow', SEC_SILK, t, w_mart, MART_SILK, npcs=[
        ('OBJ_EVENT_GFX_WOMAN_1_FRLG', 6, 4,
         'SILK SCARVES are woven right here\n'
         'in town. They power up NORMAL-\n'
         'type moves!'),
    ])
    house('Silkhollow', 1, SEC_SILK, t, w_h1, 'MUS_FORTREE', [
        ('OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 6, 3,
         'I have woven silk for sixty years.\fThe old songs say the first\n'
         'STARFALL left threads of light\n'
         'across the sky.\fSo we weave a little star into\n'
         'every bolt of silk, for luck.'),
        ('OBJ_EVENT_GFX_WOMAN_2_FRLG', 3, 4,
         'Grandma\'s loom is older than\n'
         'this house!', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ])
    house('Silkhollow', 2, SEC_SILK, t, w_h2, 'MUS_FORTREE', [
        ('OBJ_EVENT_GFX_LASS_FRLG', 4, 3,
         'My CLEFAIRY taught me to dance.\fIt spins around and around under\n'
         'the moon. AYAME says it has the\n'
         'best steps in town!'),
        ('OBJ_EVENT_GFX_CLEFAIRY', 8, 5, 'CLEFAIRY: Pippi! Pi pippi!', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ], layout='LAYOUT_HOUSE2_FRLG')
    silk_gym(t, w_gym)


def silk_gym(t, w_gym):
    """Ayame's weaving hall, from Fuchsia's tiles: rows of dancer statues
    make a zig-zag, like a thread through a loom."""
    src = 'LAYOUT_FUCHSIA_CITY_GYM'
    W, H = 15, 21
    r = g.Room(W, H, 'gTileset_BuildingFrlg', 'gTileset_FuchsiaGym', source=src)
    r.fill(0, 0, W, H, r.at(1, 3))                     # pink floor
    r.copy(0, 0, W, 3, 0, 0)                           # wall, windows, pillars, emblem, shadow
    for y in range(3, H - 1):
        r.set(0, y, r.at(0, 3))                        # shadow along the west wall
    r.fill(0, H - 1, W, 1, r.at(0, 22))
    mx = W // 2
    r.copy(6, 21, 3, 2, mx - 1, H - 2)                 # exit mat
    r.copy(6, 12, 3, 3, mx - 1, 3)                     # the leader's stage
    r.copy(6, 12, 3, 3, mx - 1, 9)                     # dancers' stages
    r.copy(6, 12, 3, 3, 9, 14)
    for x in range(0, 11):                             # statue rows (heads walkable, bases solid)
        r.copy(4, 18, 1, 2, x, 7)
    for x in range(4, W):
        r.copy(4, 18, 1, 2, x, 12)
    name = 'Seika_Silkhollow_Gym'
    m = add(g.MapDef(name, 'MAP_SEIKA_SILKHOLLOW_GYM', SEC_SILK, r, 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (mx - 1, mx, mx + 1):
        door(m, k, H - 2, t.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_BEAUTY', mx, 4, name + '_EventScript_Ayame'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', mx - 2, H - 3, name + '_EventScript_GymGuy'))
    trainer('TRAINER_SEIKA_AROMA_LADY_MIDORI', 'MIDORI', 'Aroma Lady Frlg', 'Aroma Lady Frlg', 'Female', """
Whimsicott
Level: 60
- Moonblast
- Energy Ball
- Encore
- Tailwind

Florges
Level: 60
- Moonblast
- Psychic
- Calm Mind
- Synthesis
""", music='Female')
    trainer('TRAINER_SEIKA_PARASOL_LADY_CHIYO', 'CHIYO', 'Parasol Lady', 'Parasol Lady', 'Female', """
Azumarill
Level: 60
- Play Rough
- Aqua Jet
- Superpower
- Knock Off

Wigglytuff
Level: 60
- Dazzling Gleam
- Hyper Voice
- Fire Blast
- Sing

Togetic
Level: 61
- Dazzling Gleam
- Air Slash
- Aura Sphere
- Roost
""", music='Female')
    trainer('TRAINER_SEIKA_BEAUTY_YUKA', 'YUKA', 'Beauty Frlg', 'Beauty Frlg', 'Female', """
Granbull
Level: 61
- Play Rough
- Close Combat
- Crunch
- Thunder Wave

Mawile
Level: 61
- Play Rough
- Iron Head
- Sucker Punch
- Swords Dance
""", music='Female')
    trainer('TRAINER_SEIKA_LEADER_AYAME', 'AYAME', 'Leader Frlg', 'Beauty', 'Female', """
Azumarill
Level: 62
%(iv)s
- Play Rough
- Aqua Jet
- Knock Off
- Belly Drum

Granbull
Level: 62
%(iv)s
- Play Rough
- Close Combat
- Crunch
- Thunder Wave

Togekiss
Level: 62
%(iv)s
- Air Slash
- Dazzling Gleam
- Aura Sphere
- Roost

Clefable @ Leftovers
Level: 62
%(iv)s
- Moonblast
- Calm Mind
- Moonlight
- Flamethrower

Sylveon @ Sitrus Berry
Level: 63
%(iv)s
- Moonblast
- Hyper Voice
- Calm Mind
- Shadow Ball
""" % {'iv': IV31}, music='Female', ai='Smart Trainer')
    m.objects.append(g.obj('OBJ_EVENT_GFX_WOMAN_2_FRLG', 10, 15, name + '_EventScript_Midori',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=4))
    m.objects.append(g.obj('OBJ_EVENT_GFX_LASS_FRLG', mx, 9, name + '_EventScript_Chiyo',
                           move='MOVEMENT_TYPE_FACE_DOWN', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_BEAUTY_FRLG', 13, 5, name + '_EventScript_Yuka',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.scripts += battle(name + '_EventScript_Midori', 'TRAINER_SEIKA_AROMA_LADY_MIDORI',
                        'Breathe in... the scent of\nflowers in silk. Now battle!',
                        'Oh! My petals scattered!',
                        'AYAME dyes her silk with flowers\nfrom the town gardens.', name)
    m.scripts += battle(name + '_EventScript_Chiyo', 'TRAINER_SEIKA_PARASOL_LADY_CHIYO',
                        'One, two, three, turn!\nKeep in step with me!',
                        'You stepped on my toes!',
                        'Every dancer here learned from\nAYAME. She never misses a step.', name)
    m.scripts += battle(name + '_EventScript_Yuka', 'TRAINER_SEIKA_BEAUTY_YUKA',
                        'Silk looks soft, doesn\'t it?\nFAIRY POKéMON look soft, too!',
                        'You saw right through me!',
                        'The thread that looks weakest\nis often the strongest.', name)
    m.scripts += leader_script(name, 'Ayame', 'TRAINER_SEIKA_LEADER_AYAME', BADGE6, 'ITEM_TM_SAFEGUARD')
    m.scripts += T(name + '_Text_AyameIntro',
                   'Welcome to my weaving hall.\f'
                   'I am AYAME. Every battle is a\n'
                   'thread, and I weave them all\n'
                   'into silk.\f'
                   'Let us see what pattern you and\n'
                   'your POKéMON will make!')
    m.scripts += T(name + '_Text_AyameDefeat',
                   'Oh... what a beautiful pattern.\n'
                   'I could never have woven it.\f'
                   'Please take the SILK BADGE. It is\n'
                   'light as thread, and as strong.\f'
                   + badge_text('SILK BADGE', 'AYAME'))
    m.scripts += T(name + '_Text_AyameGift', 'And this, too. A veil of silk to\nkeep your POKéMON safe.')
    m.scripts += T(name + '_Text_AyamePost',
                   'Go west along WEAVER\'S ROAD to\n'
                   'FROSTMERE VILLAGE.\f'
                   'RIME keeps the shrine by the\n'
                   'cold lake there. He is patient,\n'
                   'like the ice itself.')
    m.scripts += gym_guy_script(name, BADGE6)
    m.scripts += T(name + '_Text_GymGuy',
                   'Yo! AYAME weaves her FAIRY\n'
                   'POKéMON into one strong cloth.\f'
                   'POISON and STEEL moves cut right\n'
                   'through it!\f'
                   'Her SYLVEON is the last thread.\n'
                   'Don\'t let it CALM MIND too long!')
    m.scripts += T(name + '_Text_GymGuyWon', 'What a show! That\'s six BADGES.\nTwo more to go!')


# ---------------------------------------------------------------- Weaver's Road content

def weavers(m):
    P = m.name
    signpost(m, 57, 11, P + '_EventScript_SignEast', 'WEAVER\'S ROAD\nSILKHOLLOW - FROSTMERE')
    signpost(m, 5, 14, P + '_EventScript_SignWest', 'WEAVER\'S ROAD\nFROSTMERE - SILKHOLLOW')
    trainer('TRAINER_SEIKA_BUG_MANIAC_TSUMUGI', 'TSUMUGI', 'Bug Maniac', 'Bug Maniac', 'Male', """
Ariados
Level: 56
- Poison Jab
- Sucker Punch
- Sticky Web
- Megahorn

Yanmega
Level: 57
- Bug Buzz
- Air Slash
- Ancient Power
- U-turn

Scizor
Level: 57
- Bullet Punch
- X-Scissor
- Swords Dance
- Roost
""")
    trainer('TRAINER_SEIKA_AROMA_LADY_KINU', 'KINU', 'Aroma Lady Frlg', 'Aroma Lady Frlg', 'Female', """
Bellossom
Level: 56
- Petal Blizzard
- Moonblast
- Quiver Dance
- Sleep Powder

Sunflora
Level: 56
- Solar Beam
- Earth Power
- Sunny Day
- Sludge Bomb

Jumpluff
Level: 57
- Acrobatics
- Sleep Powder
- Leech Seed
- U-turn
""", music='Female')
    trainer('TRAINER_SEIKA_COOLTRAINER_DAIKI', 'DAIKI', 'Cooltrainer Frlg', 'Cooltrainer M Frlg', 'Male', """
Ursaring
Level: 57
- Facade
- Close Combat
- Crunch
- Swords Dance

Lanturn
Level: 57
- Scald
- Thunderbolt
- Ice Beam
- Thunder Wave

Xatu
Level: 58
- Psychic
- Air Slash
- Heat Wave
- Roost
""")
    trainer('TRAINER_SEIKA_LASS_MAYU', 'MAYU', 'Lass Frlg', 'Lass Frlg', 'Female', """
Ledian
Level: 56
- Bug Buzz
- Air Slash
- Light Screen
- Reflect

Furret
Level: 57
- Double-Edge
- Knock Off
- U-turn
- Coil

Snubbull
Level: 57
- Play Rough
- Crunch
- Fire Fang
- Thunder Wave
""", music='Female')
    m.objects.append(g.obj('OBJ_EVENT_GFX_BUG_CATCHER_FRLG', 50, 8, P + '_EventScript_Tsumugi',
                           move='MOVEMENT_TYPE_FACE_UP', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_WOMAN_2_FRLG', 34, 9, P + '_EventScript_Kinu',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_COOLTRAINER_M', 30, 18, P + '_EventScript_Daiki',
                           move='MOVEMENT_TYPE_FACE_UP', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_LASS_FRLG', 15, 11, P + '_EventScript_Mayu',
                           move='MOVEMENT_TYPE_FACE_DOWN', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.scripts += battle(P + '_EventScript_Tsumugi', 'TRAINER_SEIKA_BUG_MANIAC_TSUMUGI',
                        'You walked right into my web!\nBUG POKéMON, go!',
                        'My web... torn to pieces!',
                        'ARIADOS silk is so strong, the\nweavers use it for their looms.', P)
    m.scripts += battle(P + '_EventScript_Kinu', 'TRAINER_SEIKA_AROMA_LADY_KINU',
                        'The flowers on this road dye the\nsilk of SILKHOLLOW. Lovely, no?',
                        'My bouquet is all wilted!',
                        'Red petals for the festival silk,\nblue for the winter silk.', P)
    m.scripts += battle(P + '_EventScript_Daiki', 'TRAINER_SEIKA_COOLTRAINER_DAIKI',
                        'Six BADGES? Let\'s see if you\'ve\nearned them!',
                        'Yeah, you\'ve earned them.',
                        'It gets cold past here. FROSTMERE\nis where winter lives.', P)
    m.scripts += battle(P + '_EventScript_Mayu', 'TRAINER_SEIKA_LASS_MAYU',
                        'My name means cocoon! Want to see\nmy POKéMON break out?',
                        'Wrapped up and beaten...',
                        'One day my LEDYBA all became\nLEDIAN. Now they never sit still!', P)
    wild('MAP_SEIKA_WEAVERS_ROAD', 'gSeika_WeaversRoad',
         land=[('ARIADOS', 56, 57), ('LEDIAN', 56, 57), ('JUMPLUFF', 56, 58), ('FURRET', 56, 58),
               ('NOCTOWL', 56, 58), ('GRANBULL', 57, 58), ('YANMEGA', 57, 58), ('SUNFLORA', 57, 57),
               ('BELLOSSOM', 57, 57), ('HERACROSS', 58, 58), ('TOGETIC', 58, 58), ('SCYTHER', 58, 58)],
         water=[('AZUMARILL', 55, 57), ('QUAGSIRE', 55, 57), ('POLITOED', 56, 58), ('SLOWKING', 56, 58),
                ('AZUMARILL', 58, 58)],
         fishing=[('MAGIKARP', 50, 52), ('GOLDEEN', 50, 52), ('POLIWHIRL', 52, 54), ('SEAKING', 54, 56),
                  ('QUAGSIRE', 54, 56), ('GYARADOS', 54, 58), ('SEAKING', 56, 58), ('POLITOED', 56, 58),
                  ('AZUMARILL', 56, 58), ('GYARADOS', 58, 58)])


# ---------------------------------------------------------------- Frostmere content

def frostmere(f):
    P = f.name
    w_pc = door(f, 16, 19, 'MAP_SEIKA_FROSTMERE_POKEMON_CENTER_1F', 0)
    w_gym = door(f, 24, 19, 'MAP_SEIKA_FROSTMERE_GYM', 0)
    w_mart = door(f, 25, 9, 'MAP_SEIKA_FROSTMERE_MART', 0)
    w_h1 = door(f, 19, 9, 'MAP_SEIKA_FROSTMERE_HOUSE1', 0)
    w_h2 = door(f, 15, 25, 'MAP_SEIKA_FROSTMERE_HOUSE2', 0)
    w_h3 = door(f, 24, 25, 'MAP_SEIKA_FROSTMERE_HOUSE3', 0)
    signpost(f, 27, 11, P + '_EventScript_TownSign',
             'FROSTMERE VILLAGE\n'
             'Home of the coldest lake\n'
             'in SEIKA.')
    signpost(f, 26, 20, P + '_EventScript_GymSign',
             'FROSTMERE POKéMON GYM\n'
             'LEADER: RIME\fThe ice fisher who waits out\n'
             'the longest winter!')
    npc(f, 'OBJ_EVENT_GFX_FISHER', 14, 14, P + '_EventScript_Fisher',
        'In winter we cut holes in the ice\n'
        'and fish all night long.\fRIME can sit by one hole for a\n'
        'whole day without moving!', 'MOVEMENT_TYPE_FACE_LEFT')
    npc(f, 'OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 22, 14, P + '_EventScript_OldWoman',
        'Strange folk in dark coats came\n'
        'through here last night.\fThey were hauling something big,\n'
        'wrapped in cloth. Up north.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(f, 'OBJ_EVENT_GFX_LITTLE_BOY_FRLG', 11, 25, P + '_EventScript_Boy',
        'I saw a LAPRAS in the lake once!\n'
        'It was singing to the snow!', 'MOVEMENT_TYPE_WANDER_AROUND')
    # two Team Nova grunts block the north road, talking about the bell
    for i, (x, face) in enumerate(((16, 'MOVEMENT_TYPE_FACE_RIGHT'), (17, 'MOVEMENT_TYPE_FACE_LEFT'))):
        f.objects.append(g.obj('OBJ_EVENT_GFX_PSYCHIC_M', x, 8, P + '_EventScript_Grunts',
                               local_id='LOCALID_SEIKA_FROSTMERE_GRUNT%d' % (i + 1), move=face, flag=HIDE_GRUNTS))
    trainer('TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_1', 'GRUNT', 'Team Nova', 'Psychic M', 'Male', """
Bronzong
Level: 60
- Gyro Ball
- Earthquake
- Psychic
- Stealth Rock

Beheeyem
Level: 61
- Psychic
- Shadow Ball
- Thunderbolt
- Recover
""", music='Intense', ai='Basic Trainer')
    trainer('TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_2', 'GRUNT', 'Team Nova', 'Psychic M', 'Male', """
Lunatone
Level: 61
- Moonblast
- Psychic
- Ice Beam
- Earth Power

Starmie
Level: 62
- Hydro Pump
- Psychic
- Ice Beam
- Recover
""", music='Intense', ai='Basic Trainer')
    f.scripts += (P + '_EventScript_Grunts::\n'
                  '\tlock\n'
                  '\tfaceplayer\n'
                  '\tplaybgm MUS_ENCOUNTER_INTENSE, FALSE\n'
                  '\tmsgbox %(p)s_Text_GruntsTalk, MSGBOX_DEFAULT\n'
                  '\tgoto_if_defeated TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_1, %(p)s_EventScript_Grunt2\n'
                  '\ttrainerbattle_no_intro TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_1, %(p)s_Text_Grunt1Defeat\n'
                  '\tgoto %(p)s_EventScript_Grunt2\n\tend\n\n'
                  '%(p)s_EventScript_Grunt2::\n'
                  '\tmsgbox %(p)s_Text_Grunt2Intro, MSGBOX_DEFAULT\n'
                  '\ttrainerbattle_no_intro TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_2, %(p)s_Text_Grunt2Defeat\n'
                  '\tmsgbox %(p)s_Text_GruntsAfter, MSGBOX_DEFAULT\n'
                  '\tclosemessage\n'
                  '\tfadescreen FADE_TO_BLACK\n'
                  '\tremoveobject LOCALID_SEIKA_FROSTMERE_GRUNT1\n'
                  '\tremoveobject LOCALID_SEIKA_FROSTMERE_GRUNT2\n'
                  '\tsetflag %(flag)s\n'
                  '\tfadescreen FADE_FROM_BLACK\n'
                  '\tfadedefaultbgm\n'
                  '\trelease\n\tend\n\n' % {'p': P, 'flag': HIDE_GRUNTS})
    f.scripts += T(P + '_Text_GruntsTalk',
                   '...So we haul the bell all the\n'
                   'way north? To the mountain?\f'
                   'Orders are orders. The bell goes\n'
                   'up MT. SKYREACH, and we-\f'
                   'Huh? How long has this kid been\n'
                   'standing there?\f'
                   'You didn\'t hear a thing! Nothing\n'
                   'about any bell!')
    f.scripts += T(P + '_Text_Grunt1Defeat', 'Ugh! Not in the snow!')
    f.scripts += T(P + '_Text_Grunt2Intro', 'My turn! I\'ll freeze you out!')
    f.scripts += T(P + '_Text_Grunt2Defeat', 'Beaten by a kid... again?')
    f.scripts += T(P + '_Text_GruntsAfter',
                   'Forget it! The bell\'s already on\n'
                   'its way up the mountain.\f'
                   'DENEB and ALTAIR will have it\n'
                   'ringing before you get there!\f'
                   'Come on, let\'s go!')
    pokecenter('Frostmere', SEC_FROST, f, w_pc, 'HEAL_LOCATION_SEIKA_FROSTMERE', npcs=[
        ('OBJ_EVENT_GFX_HIKER_FRLG', 10, 5,
         'Heal up before FROST ROAD.\n'
         'The wind up there bites!'),
    ])
    mart('Frostmere', SEC_FROST, f, w_mart, MART_FROST, npcs=[
        ('OBJ_EVENT_GFX_WOMAN_2_FRLG', 6, 4,
         'NEVER-MELT ICE powers up ICE\n'
         'moves. It never melts, not even\n'
         'in my pocket!'),
    ])
    house('Frostmere', 1, SEC_FROST, f, w_h1, 'MUS_RG_SEVII_45', [
        ('OBJ_EVENT_GFX_OLD_MAN_2', 6, 3,
         'FROST ROAD goes north to\n'
         'DUSKPEAK VILLAGE, at the foot of\n'
         'MT. SKYREACH.\fThe snow up there never melts.\n'
         'Not in a thousand years.'),
    ], layout='LAYOUT_HOUSE5_FRLG')
    house('Frostmere', 2, SEC_FROST, f, w_h2, 'MUS_RG_SEVII_45', [
        ('OBJ_EVENT_GFX_WOMAN_1_FRLG', 6, 3,
         'I knit scarves from SILKHOLLOW\n'
         'silk. Nothing is warmer!'),
        ('OBJ_EVENT_GFX_LITTLE_GIRL_FRLG', 3, 4,
         'RIME gave me a fish once. It was\n'
         'frozen solid!', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ])
    house('Frostmere', 3, SEC_FROST, f, w_h3, 'MUS_RG_SEVII_45', [
        ('OBJ_EVENT_GFX_OLD_MAN_1', 4, 3,
         'The old songs say a bell once\n'
         'rang on top of MT. SKYREACH.\fWhen it rang, the WISHING STAR\n'
         'woke and came down to listen.\fA bell of starglass, they said.\n'
         'Who knows if it was real?'),
    ], layout='LAYOUT_HOUSE2_FRLG')
    frost_gym(f, w_gym)
    wild('MAP_SEIKA_FROSTMERE', 'gSeika_Frostmere',
         water=[('SEEL', 56, 58), ('DEWGONG', 58, 60), ('QUAGSIRE', 58, 60), ('DEWGONG', 60, 62),
                ('LAPRAS', 60, 62)],
         fishing=[('MAGIKARP', 50, 54), ('SHELLDER', 50, 54), ('REMORAID', 54, 57), ('SEEL', 54, 57),
                  ('CLOYSTER', 56, 60), ('OCTILLERY', 56, 60), ('DEWGONG', 56, 60), ('GYARADOS', 56, 60),
                  ('LAPRAS', 58, 60), ('GYARADOS', 60, 62)])


# ice-cave pieces: (layout, x, y) of one block in Four Island's Icefall Cave
ICE_B1F = 'LAYOUT_FOUR_ISLAND_ICEFALL_CAVE_B1F'
ICE_1F = 'LAYOUT_FOUR_ISLAND_ICEFALL_CAVE_1F'
ICE_ENT = 'LAYOUT_FOUR_ISLAND_ICEFALL_CAVE_ENTRANCE'
ICE = {
    'snow': (ICE_B1F, 0, 0), 'wall': (ICE_B1F, 12, 0), 'wall_l': (ICE_B1F, 11, 0), 'wall_r': (ICE_B1F, 23, 0),
    'ridge_l': (ICE_B1F, 6, 3), 'side_l': (ICE_B1F, 7, 3), 'side_r': (ICE_B1F, 22, 3), 'ridge_r': (ICE_B1F, 23, 3),
    'floor': (ICE_B1F, 8, 3), 'ice': (ICE_B1F, 16, 2), 'ice_rock': (ICE_B1F, 19, 2),
    'bottom': (ICE_B1F, 8, 15), 'bottom_l': (ICE_B1F, 7, 15), 'bottom_r': (ICE_B1F, 21, 16),
    'exit_l': (ICE_ENT, 16, 31), 'exit_c': (ICE_ENT, 17, 31), 'exit_r': (ICE_ENT, 18, 31),
    'exit_mat': (ICE_ENT, 17, 30),
    'e_tl': (ICE_1F, 15, 8), 'e_t': (ICE_1F, 8, 8), 'e_tr': (ICE_1F, 17, 8), 'e_l': (ICE_1F, 7, 9),
    'e_r': (ICE_1F, 10, 9), 'e_bl': (ICE_1F, 7, 15), 'e_b': (ICE_1F, 8, 11), 'e_br': (ICE_1F, 9, 15),
    'rock': (ICE_1F, 8, 6),
}


def ice_block(key):
    import qol_maptools as mt
    lid, x, y = ICE[key]
    L = mt.layouts()[lid]
    return mt.read_blocks(L['blockdata_filepath'], L['width'], L['height'])[y][x]


def ice_sheet(r, cells):
    """Slippery ice on the cave floor, ringed by Kanto's ice-edge floor tiles."""
    cells = set(cells)
    for (x, y) in cells:
        r.set(x, y, ice_block('ice'))
    ring = {(x + dx, y + dy) for (x, y) in cells for dx in (-1, 0, 1) for dy in (-1, 0, 1)} - cells
    ring = {(x, y) for (x, y) in ring if 2 <= x < r.w - 2}
    for (x, y) in ring:
        n, s, w, e = (x, y - 1) in cells, (x, y + 1) in cells, (x - 1, y) in cells, (x + 1, y) in cells
        if s:
            k = 'e_t'
        elif n:
            k = 'e_b'
        elif e:
            k = 'e_l'
        elif w:
            k = 'e_r'
        elif (x + 1, y + 1) in cells:
            k = 'e_tl'
        elif (x - 1, y + 1) in cells:
            k = 'e_tr'
        elif (x + 1, y - 1) in cells:
            k = 'e_bl'
        else:
            k = 'e_br'
        r.set(x, y, ice_block(k))


# Rime's hall: the floor plan (x 2..14, y 2..19). '~' ice, '#' rock on the ice;
# everything else is cave floor. Ice runs wall to wall, so the only way up is
# across it: in at the gap in the bottom row, stop on a rock, slide right, then
# up through the gap in the top row (3 slides; checked with a solver: every
# spot you can reach on the ice can slide back to the entrance).
FROST_PLAN = [
    '.............',   # y 2
    '.............',
    '.............',
    '.............',
    '.............',   # y 6: ice edge
    '###########~#',   # y 7
    '~~~~~~~~~~~~~',
    '~~~~~~~#~~~~~',
    '~~#~~~~~~~~~~',
    '~~~~~#~~~~~~~',
    '~~~~~~~~~~~~#',
    '~~~#~~#~~~~~~',
    '~~~~~~~~~#~~~',
    '#####~~~#####',   # y 15
    '.............',   # y 16: ice edge
    '.............',
    '.............',
    '.............',   # y 19: exit mat row
]


def frost_gym(f, w_gym):
    W, H = 17, 21
    r = g.Room(W, H, 'gTileset_General_Frlg', 'gTileset_SeafoamIslands', source=ICE_B1F)
    B = ice_block
    r.fill(0, 0, W, H, B('floor'))
    r.set(0, 0, B('wall_l'))
    r.fill(1, 0, W - 2, 1, B('wall'))
    r.set(W - 1, 0, B('wall_r'))
    r.set(0, 1, B('ridge_l'))
    r.set(1, 1, B('wall_l'))
    r.fill(2, 1, W - 4, 1, B('wall'))
    r.set(W - 2, 1, B('wall_r'))
    r.set(W - 1, 1, B('ridge_r'))
    for y in range(2, H - 1):
        r.set(0, y, B('ridge_l'))
        r.set(1, y, B('side_l'))
        r.set(W - 2, y, B('side_r'))
        r.set(W - 1, y, B('ridge_r'))
    r.set(0, H - 1, B('ridge_l'))
    r.set(1, H - 1, B('bottom_l'))
    r.fill(2, H - 1, W - 4, 1, B('bottom'))
    r.set(W - 2, H - 1, B('bottom_r'))
    r.set(W - 1, H - 1, B('ridge_r'))
    mx = W // 2
    for i, k in enumerate(('exit_l', 'exit_c', 'exit_r')):
        r.set(mx - 1 + i, H - 1, B(k))
        r.set(mx - 1 + i, H - 2, B('exit_mat'))
    ice = [(2 + x, 2 + y) for y, row in enumerate(FROST_PLAN) for x, ch in enumerate(row) if ch in '~#']
    ice_sheet(r, ice)
    for y, row in enumerate(FROST_PLAN):
        for x, ch in enumerate(row):
            if ch == '#':
                r.set(2 + x, 2 + y, B('ice_rock'))
            elif ch == 'o':
                r.set(2 + x, 2 + y, B('rock'))
    for x, y in ((4, 3), (12, 3), (3, 18), (13, 18)):
        r.set(x, y, B('rock'))
    name = 'Seika_Frostmere_Gym'
    m = add(g.MapDef(name, 'MAP_SEIKA_FROSTMERE_GYM', SEC_FROST, r, 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (mx - 1, mx, mx + 1):
        door(m, k, H - 2, f.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_EXPERT_M', mx, 3, name + '_EventScript_Rime'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', mx + 2, H - 3, name + '_EventScript_GymGuy'))
    trainer('TRAINER_SEIKA_FISHERMAN_SHIRO', 'SHIRO', 'Fisherman Frlg', 'Fisherman Frlg', 'Male', """
Dewgong
Level: 62
- Ice Beam
- Surf
- Encore
- Perish Song

Cloyster
Level: 62
- Icicle Spear
- Rock Blast
- Shell Smash
- Ice Shard
""")
    trainer('TRAINER_SEIKA_COOLTRAINER_MIZORE', 'MIZORE', 'Cooltrainer Frlg', 'Cooltrainer F Frlg', 'Female', """
Piloswine
Level: 62
- Icicle Crash
- Earthquake
- Ice Shard
- Stone Edge

Delibird
Level: 62
- Ice Spinner
- Brave Bird
- Drill Run
- Ice Shard

Froslass
Level: 63
- Ice Beam
- Shadow Ball
- Spikes
- Will-O-Wisp
""", music='Female')
    trainer('TRAINER_SEIKA_EXPERT_TOUMA', 'TOUMA', 'Expert', 'Expert M', 'Male', """
Glalie
Level: 63
- Freeze-Dry
- Earthquake
- Crunch
- Spikes

Walrein
Level: 63
- Blizzard
- Surf
- Body Slam
- Rest
""")
    trainer('TRAINER_SEIKA_LEADER_RIME', 'RIME', 'Leader Frlg', 'Expert M', 'Male', """
Cloyster
Level: 64
%(iv)s
- Icicle Spear
- Rock Blast
- Shell Smash
- Ice Shard

Jynx
Level: 64
%(iv)s
- Ice Beam
- Psychic
- Lovely Kiss
- Nasty Plot

Weavile
Level: 64
%(iv)s
- Triple Axel
- Knock Off
- Ice Shard
- Swords Dance

Lapras
Level: 64
%(iv)s
- Freeze-Dry
- Hydro Pump
- Thunderbolt
- Ice Shard

Mamoswine @ Never-Melt Ice
Level: 65
%(iv)s
- Icicle Crash
- Earthquake
- Ice Shard
- Stone Edge
""" % {'iv': IV31}, music='Male', ai='Smart Trainer')
    m.objects.append(g.obj('OBJ_EVENT_GFX_FISHER', 5, 17, name + '_EventScript_Shiro',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_COOLTRAINER_F', 11, 6, name + '_EventScript_Mizore',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_EXPERT_M', 5, 4, name + '_EventScript_Touma',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.scripts += battle(name + '_EventScript_Shiro', 'TRAINER_SEIKA_FISHERMAN_SHIRO',
                        'Shh! You\'ll scare the fish...\nToo late! Battle me instead!',
                        'The big one got away...',
                        'On ice, you slide until something\nstops you. Use the rocks!', name)
    m.scripts += battle(name + '_EventScript_Mizore', 'TRAINER_SEIKA_COOLTRAINER_MIZORE',
                        'You made it across the ice?\nThen let\'s see you handle sleet!',
                        'I melted...',
                        'RIME has fished this lake for\nforty winters. He never gives up.', name)
    m.scripts += battle(name + '_EventScript_Touma', 'TRAINER_SEIKA_EXPERT_TOUMA',
                        'A cold mind is a clear mind.\nShow me yours!',
                        'Hm. Clear as ice.',
                        'FIRE melts ice, but RIME\'s\nPOKéMON can take the heat.', name)
    m.scripts += leader_script(name, 'Rime', 'TRAINER_SEIKA_LEADER_RIME', BADGE7, 'ITEM_TM_ICE_BEAM')
    m.scripts += T(name + '_Text_RimeIntro',
                   '...Shh. I was watching the ice.\n'
                   'You need patience to catch\n'
                   'anything up here.\f'
                   'I am RIME. I have fished this\n'
                   'lake through forty winters.\f'
                   'Let us see if your fire can last\n'
                   'through my winter!')
    m.scripts += T(name + '_Text_RimeDefeat',
                   'The ice cracks... and spring\n'
                   'comes after all.\f'
                   'The FROST BADGE is yours. Wear it\n'
                   'with a warm heart.\f'
                   + badge_text('FROST BADGE', 'RIME'))
    m.scripts += T(name + '_Text_RimeGift', 'Take this too. The coldest beam\nof ice I know.')
    m.scripts += T(name + '_Text_RimePost',
                   'Go north along FROST ROAD to\n'
                   'DUSKPEAK VILLAGE.\f'
                   'TOMOE keeps the last shrine,\n'
                   'below MT. SKYREACH.\f'
                   'Mind the snow, and keep your\n'
                   'POKéMON warm.')
    m.scripts += gym_guy_script(name, BADGE7)
    m.scripts += T(name + '_Text_GymGuy',
                   'Brr! RIME\'s ICE POKéMON are\n'
                   'tough as glaciers.\f'
                   'FIRE, FIGHTING, ROCK and STEEL\n'
                   'moves crack them open!\f'
                   'And watch your step. On the ice\n'
                   'you slide until something stops\n'
                   'you!')
    m.scripts += T(name + '_Text_GymGuyWon', 'Seven BADGES! You\'re hotter than\nthe ice is cold!')


def build():
    hp = add(g.MapDef('Seika_HollowPath', 'MAP_SEIKA_HOLLOW_PATH', SEC_PATH, hollow_path(), 'MUS_RG_ROUTE3',
                      'MAP_TYPE_ROUTE', border=[ROCK['c']] * 4))
    link('MAP_SEIKA_TIDEWATCH', 'up', 10, 'MAP_SEIKA_HOLLOW_PATH')
    t = add(g.MapDef('Seika_Silkhollow', 'MAP_SEIKA_SILKHOLLOW', SEC_SILK, silkhollow_town(), 'MUS_FORTREE'))
    link('MAP_SEIKA_HOLLOW_PATH', 'up', -8, 'MAP_SEIKA_SILKHOLLOW')
    r = add(g.MapDef('Seika_WeaversRoad', 'MAP_SEIKA_WEAVERS_ROAD', SEC_ROAD, weavers_road(), 'MUS_ROUTE120',
                     'MAP_TYPE_ROUTE'))
    link('MAP_SEIKA_SILKHOLLOW', 'left', 0, 'MAP_SEIKA_WEAVERS_ROAD')
    f = add(g.MapDef('Seika_Frostmere', 'MAP_SEIKA_FROSTMERE', SEC_FROST, frostmere_town(), 'MUS_RG_SEVII_45',
                     weather='WEATHER_SNOW'))
    link('MAP_SEIKA_WEAVERS_ROAD', 'left', 0, 'MAP_SEIKA_FROSTMERE')
    hollow(hp)
    silkhollow(t)
    weavers(r)
    frostmere(f)
