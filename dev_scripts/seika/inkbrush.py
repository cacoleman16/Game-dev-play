"""Inkbrush Town (Gym 2, Psychic), Mossgrove Woods and Mossgrove Village (Gym 3, Bug)."""
from common import (g, T, add, door, npc, signpost, pokecenter, mart, house, wild, mapsec,
                    trainer, battle, flag, link, PC, MART, GYM)

FUCH = 'LAYOUT_FUCHSIA_CITY'
VIR = 'LAYOUT_VIRIDIAN_CITY'
VERM = 'LAYOUT_VERMILION_CITY'

SEC_INK = mapsec('MAPSEC_SEIKA_INKBRUSH', 'INKBRUSH TOWN')
SEC_WOODS = mapsec('MAPSEC_SEIKA_MOSSGROVE_WOODS', 'MOSSGROVE WOODS')
SEC_MOSS = mapsec('MAPSEC_SEIKA_MOSSGROVE', 'MOSSGROVE')

BADGE2 = flag('FLAG_QOL_SEIKA_BADGE02', 'INK BADGE (Sumi)')
SCROLL = flag('FLAG_QOL_SEIKA_SCROLL', 'heard about the STARFALL SCROLL in Inkbrush')
ALTAIR = flag('FLAG_QOL_SEIKA_ALTAIR_WOODS', 'beat Altair in Mossgrove Woods (hides him)')
BADGE3 = flag('FLAG_QOL_SEIKA_BADGE03', 'GROVE BADGE (Kaya)')
ITEM_W1 = flag('FLAG_QOL_SEIKA_WOODS_STAR_PIECE', 'Mossgrove Woods item ball')
ITEM_W2 = flag('FLAG_QOL_SEIKA_WOODS_BIG_ROOT', 'Mossgrove Woods item ball')

INK = 'Seika_Inkbrush'
WOODS = 'Seika_MossgroveWoods'
MOSS = 'Seika_Mossgrove'

# Buildings cut from Kanto (layout, x, y, w, h, door x, door y relative to the stamp)
GRAY_HOUSE = (FUCH, 32, 28, 5, 4, 1, 3)          # Fuchsia's tiled-roof house
OLD_HALL = (FUCH, 26, 13, 6, 4, 2, 3)            # Fuchsia's thatched hall, minus its baked top row
GREEN_HOUSE = (VIR, 24, 15, 5, 4, 1, 3)          # Viridian's green-roof house (school front)
GYM_SIGN = (VERM, 10, 23, 1, 2)                  # primary-only gym sign (2 tall)

INK_MART = ['ITEM_ULTRA_BALL', 'ITEM_GREAT_BALL', 'ITEM_QUICK_BALL', 'ITEM_TIMER_BALL', 'ITEM_HYPER_POTION',
            'ITEM_MAX_POTION', 'ITEM_FULL_RESTORE', 'ITEM_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL']
MOSS_MART = ['ITEM_ULTRA_BALL', 'ITEM_NET_BALL', 'ITEM_NEST_BALL', 'ITEM_HYPER_POTION', 'ITEM_MAX_POTION',
             'ITEM_FULL_RESTORE', 'ITEM_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL', 'ITEM_HONEY']

IV31 = 'IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe'


def stamp_at(c, st, x, y):
    """Stamp a building; returns its door (x, y)."""
    lid, sx, sy, w, h, dx, dy = st
    c.stamp(lid, sx, sy, w, h, x, y)
    return x + dx, y + dy


def green_house_b(c, x, y):
    """Viridian's green-roof house with the other facade (house front)."""
    c.stamp(VIR, 24, 15, 5, 3, x, y)
    c.stamp(VIR, 24, 11, 5, 1, x, y + 3)
    return x + 1, y + 3


def coarse(c, rows, ox=0, oy=0):
    """Paint a map from a coarse ASCII plan: one character = one 2x2 block.
    # tree  = path  t tall grass  . grass  f flowers  p plain grass"""
    cls = {'=': 'path', 't': 'tall', '.': 'grass', 'f': 'flowers', 'p': 'plain', 'b': 'beach'}
    for cy, row in enumerate(rows):
        for cx, ch in enumerate(row):
            x, y = ox + cx * 2, oy + cy * 2
            if ch == '#':
                c.tree(x, y, merge=True)
            else:
                c.rect(x, y, 2, 2, cls[ch])


def leader_script(name, label, const, badge, tm, intro, defeat_badge, gift, post):
    s = ('%s_EventScript_%s::\n'
         '\ttrainerbattle_single %s, %s_Text_%sIntro, %s_Text_%sDefeat, %s_EventScript_%sDefeated, NO_MUSIC\n'
         '\tmsgbox %s_Text_%sPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         '%s_EventScript_%sDefeated::\n'
         '\tsetflag %s\n'
         '\tmsgbox %s_Text_%sGift, MSGBOX_DEFAULT\n'
         '\tgiveitem %s\n'
         '\tmsgbox %s_Text_%sPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         % (name, label, const, name, label, name, label, name, label, name, label,
            name, label, badge, name, label, tm, name, label))
    s += T('%s_Text_%sIntro' % (name, label), intro)
    s += T('%s_Text_%sDefeat' % (name, label), defeat_badge)
    s += T('%s_Text_%sGift' % (name, label), gift)
    s += T('%s_Text_%sPost' % (name, label), post)
    return s


def gym_guy(name, badge, advice, won):
    return (name + '_EventScript_GymGuy::\n\tlock\n\tfaceplayer\n'
            '\tgoto_if_set %s, %s_EventScript_GymGuyWon\n'
            '\tmsgbox %s_Text_GymGuy, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            '%s_EventScript_GymGuyWon::\n\tmsgbox %s_Text_GymGuyWon, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
            % (badge, name, name, name, name)
            + T(name + '_Text_GymGuy', advice) + T(name + '_Text_GymGuyWon', won))


def gym_statues(m, name, cells, badge, text, won):
    for x, y in cells:
        m.bgs.append(g.sign(x, y, name + '_EventScript_Statue', 'BG_EVENT_PLAYER_FACING_NORTH'))
    m.scripts += (name + '_EventScript_Statue::\n\tlockall\n'
                  '\tgoto_if_set %s, %s_EventScript_StatueWon\n'
                  '\tmsgbox %s_Text_Statue, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
                  '%s_EventScript_StatueWon::\n\tmsgbox %s_Text_StatueWon, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
                  % (badge, name, name, name, name)
                  + T(name + '_Text_Statue', text) + T(name + '_Text_StatueWon', won))


def build():
    t = inkbrush()
    w = woods(t)
    mossgrove(w)


# ================================================================ Inkbrush Town

def inkbrush_canvas():
    c = g.Canvas(40, 34, 'gTileset_FuchsiaCity', seed=21)
    # forests: 8 thick at the sides and top, 6 at the bottom
    c.forest(0, 0, 40, 8)
    c.forest(0, 8, 8, 4)                  # west opening: rows 12..17
    c.forest(0, 18, 8, 16)
    c.forest(32, 8, 8, 26)
    c.forest(8, 28, 10, 6)                # south opening: x 18..23
    c.forest(24, 28, 8, 6)
    # streets
    c.rect(0, 14, 32, 2, 'beach')         # main street, out to the west exit
    c.rect(20, 14, 2, 20, 'beach')        # south road to Lantern Road
    c.rect(9, 21, 23, 2, 'beach')         # lower street
    # gardens
    c.rect(14, 8, 3, 2, 'flowers')
    c.rect(25, 24, 6, 3, 'flowers')
    c.rect(8, 8, 2, 2, 'flowers')
    c.pond(10, 24, 6, 2)                  # the brush-washing pond (lip on row 26)
    c.tree(8, 24)
    c.tree(22, 24)
    # buildings
    doors = {}
    doors['hall'] = stamp_at(c, OLD_HALL, 18, 10)      # scriptorium, door (20,13)
    doors['h1'] = stamp_at(c, GRAY_HOUSE, 10, 10)      # house 1, door (11,13)
    doors['gym'] = stamp_at(c, GYM, 25, 9)             # door (28,13)
    doors['pc'] = stamp_at(c, PC, 9, 16)               # door (11,20)
    doors['h2'] = stamp_at(c, GRAY_HOUSE, 15, 17)      # house 2, door (16,20)
    doors['school'] = stamp_at(c, GRAY_HOUSE, 22, 17)  # brush school, door (23,20)
    doors['mart'] = stamp_at(c, MART, 28, 17)          # door (30,20)
    c.stamp(*GYM_SIGN, 24, 12)                         # gym sign at (24,13)
    c.put(17, 13, g.SIGN)                              # scriptorium sign
    c.put(23, 27, g.SIGN)                              # town sign by the south road
    c.put(6, 13, g.WOOD_SIGN)                          # west exit sign
    return c, doors


def inkbrush():
    c, d = inkbrush_canvas()
    t = add(g.MapDef(INK, 'MAP_SEIKA_INKBRUSH', SEC_INK, c, 'MUS_RG_FUCHSIA', 'MAP_TYPE_TOWN'))
    w_pc = door(t, *d['pc'], 'MAP_SEIKA_INKBRUSH_POKEMON_CENTER_1F', 0)
    w_gym = door(t, *d['gym'], 'MAP_SEIKA_INKBRUSH_GYM', 0)
    w_mart = door(t, *d['mart'], 'MAP_SEIKA_INKBRUSH_MART', 0)
    w_hall = door(t, *d['hall'], 'MAP_SEIKA_INKBRUSH_SCRIPTORIUM', 0)
    w_h1 = door(t, *d['h1'], 'MAP_SEIKA_INKBRUSH_HOUSE1', 0)
    w_h2 = door(t, *d['h2'], 'MAP_SEIKA_INKBRUSH_HOUSE2', 0)
    w_school = door(t, *d['school'], 'MAP_SEIKA_INKBRUSH_HOUSE3', 0)

    signpost(t, 23, 27, INK + '_EventScript_TownSign',
             'INKBRUSH TOWN\n'
             'Every stroke tells a story.')
    signpost(t, 24, 13, INK + '_EventScript_GymSign',
             'INKBRUSH POKéMON GYM\n'
             'LEADER: SUMI\fThe calligrapher who reads\n'
             'the mind behind every stroke!')
    signpost(t, 17, 13, INK + '_EventScript_HallSign',
             'INKBRUSH SCRIPTORIUM\n'
             'Keepers of the STARFALL SCROLL')
    signpost(t, 6, 13, INK + '_EventScript_WestSign',
             'WEST: MOSSGROVE WOODS\n'
             'MOSSGROVE VILLAGE lies beyond.')
    npc(t, 'OBJ_EVENT_GFX_OLD_MAN_1', 17, 16, INK + '_EventScript_OldScribe',
        'I have copied books in that\n'
        'SCRIPTORIUM for sixty years.\fIn all that time, nobody asked to\n'
        'see the STARFALL SCROLL. Then\n'
        'last week, a stranger did.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_LITTLE_BOY_FRLG', 25, 15, INK + '_EventScript_Boy',
        'I\'m practicing my brush strokes!\fSUMI says a good stroke comes\n'
        'from a calm mind. Mine wobble...', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_WOMAN_1_FRLG', 9, 13, INK + '_EventScript_Woman',
        'MOSSGROVE WOODS is just west of\n'
        'town. It\'s easy to get lost in\n'
        'there.\fA man with dirty gloves went in\n'
        'with a shovel. Digging for what,\n'
        'I wonder?', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_FISHER', 16, 25, INK + '_EventScript_Fisher',
        'The scribes rinse their brushes\n'
        'in this pond.\fThe POLIWAG here have ink-black\n'
        'swirls. Coincidence? Ha!', 'MOVEMENT_TYPE_FACE_LEFT')
    npc(t, 'OBJ_EVENT_GFX_GENTLEMAN_FRLG', 27, 22, INK + '_EventScript_Gentleman',
        'SUMI can read a person\'s heart\n'
        'from one line of their writing.\fI wrote her a letter once. She\n'
        'still laughs when she sees me.', 'MOVEMENT_TYPE_WANDER_AROUND')

    pokecenter('Inkbrush', SEC_INK, t, w_pc, 'HEAL_LOCATION_SEIKA_INKBRUSH', npcs=[
        ('OBJ_EVENT_GFX_WOMAN_2_FRLG', 11, 5,
         'The ink here is made from soot\n'
         'and pine resin.\fIt never fades. Some scrolls are\n'
         'a thousand years old!'),
    ])
    mart('Inkbrush', SEC_INK, t, w_mart, INK_MART, npcs=[
        ('OBJ_EVENT_GFX_YOUNGSTER_FRLG', 6, 4,
         'QUICK BALLS are best thrown on\n'
         'the very first turn.\fJust like a brush stroke. No\n'
         'second chances!'),
    ])
    house('Inkbrush', 1, SEC_INK, t, w_h1, 'MUS_RG_FUCHSIA', [
        ('OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 6, 3,
         'A woman in a fine coat stayed in\n'
         'town last week.\fShe paid in gold and spoke like\n'
         'a professor. Polite, but cold.\fShe left for the west early one\n'
         'morning. Never said goodbye.'),
        ('OBJ_EVENT_GFX_LITTLE_GIRL_FRLG', 3, 4,
         'My grandma says shooting stars\n'
         'are wishes on their way down.', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ])
    house('Inkbrush', 2, SEC_INK, t, w_h2, 'MUS_RG_FUCHSIA', [
        ('OBJ_EVENT_GFX_FAT_MAN_FRLG', 4, 3,
         'I make inkstones. You grind the\n'
         'ink stick on them with water.\fThe stone comes from MT. SKYREACH.\n'
         'Some say it still holds starlight.'),
    ], layout='LAYOUT_HOUSE2_FRLG')
    house('Inkbrush', 3, SEC_INK, t, w_school, 'MUS_RG_FUCHSIA', [
        ('OBJ_EVENT_GFX_WOMAN_2_FRLG', 6, 2,
         'Welcome to the brush school!\fWe teach the old way: one breath,\n'
         'one stroke. SUMI studied here\n'
         'as a little girl.'),
        ('OBJ_EVENT_GFX_LASS_FRLG', 4, 5,
         'Today\'s word is STAR. It has\n'
         'nine strokes. I keep using ten!', 'MOVEMENT_TYPE_FACE_UP'),
    ], layout='LAYOUT_VIRIDIAN_CITY_SCHOOL')
    scriptorium(t, w_hall)
    gym_sumi(t, w_gym)
    wild('MAP_SEIKA_INKBRUSH', 'gSeika_Inkbrush',
         water=[('POLIWHIRL', 42, 44), ('WOOPER', 40, 42), ('QUAGSIRE', 42, 45), ('MARILL', 40, 42),
                ('POLITOED', 45, 45)],
         fishing=[('MAGIKARP', 40, 42), ('POLIWAG', 40, 42), ('POLIWAG', 40, 44), ('POLIWHIRL', 42, 44),
                  ('GOLDEEN', 42, 44), ('POLIWHIRL', 42, 46), ('SEAKING', 44, 46), ('POLIWRATH', 44, 46),
                  ('QUAGSIRE', 44, 46), ('GYARADOS', 46, 48)])
    return t


def scriptorium(t, w_hall):
    name = INK + '_Scriptorium'
    m = add(g.MapDef(name, 'MAP_SEIKA_INKBRUSH_SCRIPTORIUM', SEC_INK, 'LAYOUT_FUCHSIA_CITY_WARDENS_HOUSE',
                     'MUS_RG_FUCHSIA', 'MAP_TYPE_INDOOR', 'WEATHER_NONE', False, False, False))
    for k in (6, 5, 7):
        door(m, k, 9, t.id, w_hall)
    m.objects.append(g.obj('OBJ_EVENT_GFX_OLD_MAN_2', 11, 5, name + '_EventScript_Keeper',
                           move='MOVEMENT_TYPE_FACE_DOWN'))
    m.scripts += (name + '_EventScript_Keeper::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_set %s, %s_EventScript_KeeperAgain\n'
                  '\tmsgbox %s_Text_Keeper, MSGBOX_DEFAULT\n'
                  '\tsetflag %s\n\trelease\n\tend\n\n'
                  '%s_EventScript_KeeperAgain::\n'
                  '\tmsgbox %s_Text_KeeperAgain, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % (SCROLL, name, name, SCROLL, name, name))
    m.scripts += T(name + '_Text_Keeper',
                   'Ah, a traveler. I am the keeper\n'
                   'of this SCRIPTORIUM.\fYou have the look of someone\n'
                   'chasing a star. You are the\n'
                   'second this week.\fLast week a woman in a fine coat\n'
                   'came in. She asked to copy the\n'
                   'STARFALL SCROLL.\fShe wrote very fast, and she\n'
                   'never smiled once.\fThe scroll is the oldest record\n'
                   'of the first STARFALL. It says...\f"The WISHING STAR sleeps at the\n'
                   'SKY SHRINE, on the top of\n'
                   'MT. SKYREACH."\f"It wakes fully only when the\n'
                   'STARGLASS BELL rings."\fAnd the STARGLASS BELL is made\n'
                   'in one place only: the kilns of\n'
                   'KILN CITY.\fIf she wants to wake the star,\n'
                   'that is where she will go.')
    m.scripts += T(name + '_Text_KeeperAgain',
                   'The WISHING STAR sleeps at the\n'
                   'SKY SHRINE on MT. SKYREACH.\fIt wakes fully when the\n'
                   'STARGLASS BELL rings. Only\n'
                   'KILN CITY makes that bell.\fThe woman in the fine coat...\n'
                   'I fear she is a step ahead.')
    npc(m, 'OBJ_EVENT_GFX_WOMAN_1_FRLG', 4, 7, name + '_EventScript_Scribe',
        'We copy every old book by hand,\n'
        'so the words never die.\fThat woman\'s copy was perfect.\n'
        'Too perfect. She has done this\n'
        'before.', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(m, 'OBJ_EVENT_GFX_SCIENTIST', 9, 3, name + '_EventScript_Scholar',
        'I came all the way from KANTO to\n'
        'read these scrolls.\fThe old scribes drew the WISHING\n'
        'STAR as a small child with long\n'
        'ribbons. How charming!', 'MOVEMENT_TYPE_FACE_UP')
    for x in (4, 5):
        m.bgs.append(g.sign(x, 4, name + '_EventScript_Case1', 'BG_EVENT_PLAYER_FACING_NORTH'))
    for x in (6, 7):
        m.bgs.append(g.sign(x, 4, name + '_EventScript_Case2', 'BG_EVENT_PLAYER_FACING_NORTH'))
    for i, text in ((1, 'A faded scroll is on display.\fIt shows a star falling on a\n'
                        'mountain, and people kneeling\n'
                        'far below.'),
                    (2, 'Old brushes and an inkstone are\n'
                        'on display.\fThe label says they belonged to\n'
                        'the first keeper of the shrine.')):
        m.scripts += (name + '_EventScript_Case%d::\n\tmsgbox %s_Text_Case%d, MSGBOX_SIGN\n\tend\n\n'
                      % (i, name, i)) + T('%s_Text_Case%d' % (name, i), text)


def gym_sumi(t, w_gym):
    """Inkbrush Gym on Saffron Gym's tiles: three halls stacked, joined by
    doorways at alternate ends of each wall (no teleporters)."""
    src = 'LAYOUT_SAFFRON_CITY_GYM'
    W, H = 19, 24
    r = g.Room(W, H, 'gTileset_BuildingFrlg', 'gTileset_SaffronGym', source=src)
    FLOOR, SH_L, SH_TOP, SH_CORNER = r.at(1, 3), r.at(10, 3), r.at(2, 2), r.at(10, 2)
    WALL_T, WALL_B, PIL_T, PIL_B, PIL_BASE = r.at(2, 0), r.at(2, 1), r.at(1, 0), r.at(1, 1), r.at(1, 2)
    DIV, DIV_TOP, DIV_FOOT1, DIV_FOOT2 = r.at(9, 3), r.at(9, 0), r.at(9, 22), r.at(9, 23)
    BLACK = r.at(0, 24)
    STATUE_T, STATUE_B = r.at(12, 19), r.at(12, 20)
    LAMP_T, LAMP_B = r.at(14, 7), r.at(14, 8)
    mx = W // 2

    r.fill(0, 0, W, H, FLOOR)
    for y in range(3, H - 1):
        r.set(1, y, SH_L)

    def hwall(y, x0, x1, pillars):
        """Wall on rows y, y+1 from x0 to x1, with its shadow on row y+2."""
        for x in range(x0, x1 + 1):
            p = x in pillars
            r.set(x, y, PIL_T if p else WALL_T)
            r.set(x, y + 1, PIL_B if p else WALL_B)
            r.set(x, y + 2, PIL_BASE if p else (SH_CORNER if x == 1 else SH_TOP))

    hwall(0, 1, W - 2, (3, 6, 12, 15))
    hwall(8, 4, W - 2, (4, 8, 13, 16))         # doorway at x 1..3
    hwall(16, 1, 14, (2, 5, 10, 14))           # doorway at x 15..17
    # side walls: Saffron's dividers, with their purple feet at the bottom
    for x in (0, W - 1):
        r.set(x, 0, DIV_TOP)
        for y in range(1, H - 3):
            r.set(x, y, DIV)
        r.set(x, H - 3, DIV_FOOT1)
        r.set(x, H - 2, DIV_FOOT2)
    r.fill(0, H - 1, W, 1, BLACK)
    for i, sx in enumerate((13, 14, 15)):       # exit mat
        r.set(mx - 1 + i, H - 2, r.at(sx, 23))
        r.set(mx - 1 + i, H - 1, r.at(sx, 24))
    r.copy(12, 10, 5, 3, mx - 2, 2)              # pink rug under the leader
    r.copy(12, 10, 5, 3, mx - 2, 10)             # and one in the middle hall
    for x, y in ((mx - 4, 3), (mx + 4, 3), (mx - 3, 19), (mx + 3, 19)):
        r.set(x, y, STATUE_T)
        r.set(x, y + 1, STATUE_B)
    r.set(mx, 7, LAMP_T)                         # lamps on the inner walls
    r.set(mx, 8, LAMP_B)
    r.set(7, 15, LAMP_T)
    r.set(7, 16, LAMP_B)

    name = INK + '_Gym'
    m = add(g.MapDef(name, 'MAP_SEIKA_INKBRUSH_GYM', SEC_INK, r, 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (mx - 1, mx, mx + 1):
        door(m, k, H - 2, t.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_EXPERT_F', mx, 3, name + '_EventScript_Sumi'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', mx - 2, H - 4, name + '_EventScript_GymGuy'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_PSYCHIC_M', 15, 13, name + '_EventScript_Haku',
                           move='MOVEMENT_TYPE_FACE_DOWN', trainer='TRAINER_TYPE_NORMAL', sight=4))
    m.objects.append(g.obj('OBJ_EVENT_GFX_WOMAN_3_FRLG', 4, 12, name + '_EventScript_Shizu',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    gym_statues(m, name, ((mx - 3, 20), (mx + 3, 20)), BADGE2,
                'INKBRUSH POKéMON GYM\nLEADER: SUMI',
                'INKBRUSH POKéMON GYM\nLEADER: SUMI\fWINNING TRAINERS:\n{PLAYER}')

    trainer('TRAINER_SEIKA_PSYCHIC_HAKU', 'HAKU', 'Psychic Frlg', 'Psychic M Frlg', 'Male', '''
Jynx
Level: 51
- Psychic
- Ice Beam
- Lovely Kiss
- Nasty Plot

Slowbro
Level: 52
- Psychic
- Surf
- Slack Off
- Thunder Wave
''')
    trainer('TRAINER_SEIKA_PAINTER_SHIZU', 'SHIZU', 'Painter Frlg', 'Painter Frlg', 'Female', '''
Smeargle
Level: 50
- Psychic
- Shadow Ball
- Thunder Wave
- Focus Blast

Mr. Mime
Level: 51
- Psychic
- Dazzling Gleam
- Reflect
- Light Screen

Xatu
Level: 51
- Psychic
- Air Slash
- Confuse Ray
- Roost
''', music='Female')
    trainer('TRAINER_SEIKA_LEADER_SUMI', 'SUMI', 'Leader Frlg', 'Expert F', 'Female', '''
Xatu
Level: 53
%(iv)s
- Psychic
- Air Slash
- Heat Wave
- Calm Mind

Slowking
Level: 53
%(iv)s
- Psychic
- Scald
- Slack Off
- Fire Blast

Farigiraf
Level: 54
%(iv)s
- Psychic
- Hyper Voice
- Thunderbolt
- Calm Mind

Espeon @ Twisted Spoon
Level: 55
%(iv)s
- Psychic
- Dazzling Gleam
- Shadow Ball
- Calm Mind
''' % {'iv': IV31}, music='Female', ai='Smart Trainer')

    m.scripts += battle(name + '_EventScript_Haku', 'TRAINER_SEIKA_PSYCHIC_HAKU',
                        'I read your next move in the\nshape of your shadow!',
                        'My reading was... smudged.',
                        'SUMI never says a word in battle.\nShe just writes. Then she wins.', name)
    m.scripts += battle(name + '_EventScript_Shizu', 'TRAINER_SEIKA_PAINTER_SHIZU',
                        'My SMEARGLE paints with its tail.\nLet\'s paint you a defeat!',
                        'Oh! You ruined my picture!',
                        'SUMI\'s ESPEON is her last brush\nstroke. Keep something for it!', name)
    m.scripts += leader_script(
        name, 'Sumi', 'TRAINER_SEIKA_LEADER_SUMI', BADGE2, 'ITEM_TM_CALM_MIND',
        'Welcome. I am SUMI. I keep the\n'
        'shrine of INKBRUSH, and its ink.\fA brush cannot lie. One stroke\n'
        'shows the whole heart of the\n'
        'one who holds it.\fLet us see what kind of stroke\n'
        'you are. Begin!',
        'A firm stroke, and an honest\n'
        'one. You have earned this.\fTake the INK BADGE.\f'
        '{FONT_NORMAL}{PLAYER} received the INK BADGE\n'
        'from SUMI!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}',
        'And this. A calm mind makes a\n'
        'steady hand.',
        'You came about the STARFALL\n'
        'SCROLL, didn\'t you?\fOur keeper will tell you what it\n'
        'says. His hall is just north of\n'
        'the main street.\fThen go west, through MOSSGROVE\n'
        'WOODS, to MOSSGROVE VILLAGE.\n'
        'KAYA keeps the shrine there.')
    m.scripts += gym_guy(name, BADGE2,
                         'Hey! SUMI\'s POKéMON are all\n'
                         'PSYCHIC types.\fBUG, GHOST and DARK moves will\n'
                         'smudge her ink. Watch out for her\n'
                         'CALM MIND, too!',
                         'What a finish! That INK BADGE\n'
                         'won\'t ever wash off!')


# ================================================================ Mossgrove Woods

# One character = a 2x2 block (see coarse()). 32 x 17 blocks = 64 x 34 tiles.
WOODS_PLAN = [
    '################################',  # 0
    '################################',  # 1   y 0..3
    '##tttttt#.tttt###.........tttt##',  # 2
    '##tttttt#.tttt###.========tttt##',  # 3
    '##..##..#..##.###.=.tttt.=.ttt##',  # 4
    '##..=======.######=.tttt.=.ttt##',  # 5
    '##tt=.ttt.=##...##=.####.=.ttt..',  # 6   east opening y 12..17
    '##tt=.ttt.=#....f#=.tttt.=======',  # 7   east path y 14..15
    '..tt=.ttt.=#.....#=......ttttt..',  # 8   west opening y 16..21
    '=====.ttt.=#f....#=......ttttt##',  # 9   west path y 18..19
    '..tt..ttt.=#.....#=......ttt####',  # 10
    '##tttt###.=========.tttttt######',  # 11
    '##tttt###...ttttt...tttttt######',  # 12  (no trees right under a path: their caps would cut it)
    '##tt#####ttt######tt###tt#######',  # 13
    '################################',  # 14
    '################################',  # 15
    '################################',  # 16  y 32..33
]


def woods_canvas():
    c = g.Canvas(64, 34, 'gTileset_ViridianForest', seed=31)
    for r in WOODS_PLAN:
        assert len(r) == 32, r
    coarse(c, WOODS_PLAN)
    c.pond(40, 16, 8, 4)                  # forest pond (lip on row 20)
    c.put(60, 12, g.WOOD_SIGN)
    c.put(2, 16, g.WOOD_SIGN)
    return c




def woods(t):
    c = woods_canvas()
    name = WOODS
    m = add(g.MapDef(name, 'MAP_SEIKA_MOSSGROVE_WOODS', SEC_WOODS, c, 'MUS_RG_VIRIDIAN_FOREST', 'MAP_TYPE_ROUTE',
                     weather='WEATHER_SHADE'))
    link('MAP_SEIKA_INKBRUSH', 'left', 0, 'MAP_SEIKA_MOSSGROVE_WOODS')   # same rows: path y 14..15
    signpost(m, 60, 12, name + '_EventScript_EastSign',
             'MOSSGROVE WOODS\n'
             'EAST: INKBRUSH TOWN')
    signpost(m, 2, 16, name + '_EventScript_WestSign',
             'MOSSGROVE WOODS\n'
             'WEST: MOSSGROVE VILLAGE')

    # ---- Altair, digging in the glade north of the path
    trainer('TRAINER_SEIKA_NOVA_ALTAIR_WOODS', 'ALTAIR', 'Nova Admin', 'Ruin Maniac', 'Male', '''
Solrock
Level: 50
%(iv)s
- Rock Slide
- Zen Headbutt
- Earthquake
- Flare Blitz

Steelix
Level: 50
%(iv)s
- Iron Tail
- Earthquake
- Stone Edge
- Rock Polish

Claydol
Level: 51
%(iv)s
- Earth Power
- Psychic
- Ice Beam
- Stealth Rock

Probopass @ Sitrus Berry
Level: 52
%(iv)s
- Flash Cannon
- Earth Power
- Power Gem
- Thunderbolt
''' % {'iv': IV31}, music='Suspicious')
    m.objects.append(g.obj('OBJ_EVENT_GFX_HIKER_FRLG', 28, 21, name + '_EventScript_Altair',
                           local_id='LOCALID_SEIKA_MOSSGROVE_WOODS_ALTAIR', move='MOVEMENT_TYPE_FACE_DOWN',
                           trainer='TRAINER_TYPE_NORMAL', sight=4, flag=ALTAIR))
    m.scripts += (name + '_EventScript_Altair::\n'
                  '\ttrainerbattle_single TRAINER_SEIKA_NOVA_ALTAIR_WOODS, %s_Text_AltairIntro, %s_Text_AltairDefeat, %s_EventScript_AltairDefeated\n'
                  '\tmsgbox %s_Text_AltairAfter, MSGBOX_AUTOCLOSE\n\tend\n\n'
                  '%s_EventScript_AltairDefeated::\n'
                  '\tmsgbox %s_Text_AltairAfter, MSGBOX_DEFAULT\n'
                  '\tclosemessage\n'
                  '\tfadescreen FADE_TO_BLACK\n'
                  '\tremoveobject LOCALID_SEIKA_MOSSGROVE_WOODS_ALTAIR\n'
                  '\tsetflag %s\n'
                  '\tfadescreen FADE_FROM_BLACK\n'
                  '\trelease\n\tend\n\n' % (name, name, name, name, name, name, ALTAIR))
    m.scripts += T(name + '_Text_AltairIntro',
                   'Hah! The kid from KANTO. Of\n'
                   'course it\'s you.\fA thousand years ago the first\n'
                   'STARFALL threw stones all over\n'
                   'these woods.\fOne of them is under my shovel\n'
                   'right now. A star stone!\fDENEB needs it, and I won\'t let\n'
                   'you dig it up first!')
    m.scripts += T(name + '_Text_AltairDefeat',
                   'Bah! Buried again!')
    m.scripts += T(name + '_Text_AltairAfter',
                   'Fine. Keep your hole in the dirt.\fDENEB already has the scroll\n'
                   'copied. She\'s going north to\n'
                   'KILN CITY, for the bell.\fWe\'ll wake that star whether you\n'
                   'like it or not!')

    # ---- route trainers (Lv 46-49)
    trainer('TRAINER_SEIKA_BUG_CATCHER_RIKU', 'RIKU', 'Bug Catcher Frlg', 'Bug Catcher Frlg', 'Male', '''
Ledian
Level: 46
- Air Slash
- Bug Buzz
- Light Screen
- Reflect

Ariados
Level: 46
- Poison Jab
- Sucker Punch
- Leech Life
- Toxic Spikes

Yanma
Level: 47
- Bug Buzz
- Air Slash
- Ancient Power
- Hypnosis
''')
    trainer('TRAINER_SEIKA_PICNICKER_NANAMI', 'NANAMI', 'Picnicker Frlg', 'Picnicker Frlg', 'Female', '''
Furret
Level: 47
- Double-Edge
- Sucker Punch
- Knock Off
- Coil

Stantler
Level: 47
- Double-Edge
- Zen Headbutt
- Megahorn
- Hypnosis

Ursaring
Level: 48
- Crunch
- Play Rough
- Close Combat
- Bulk Up
''', music='Female')
    trainer('TRAINER_SEIKA_AROMA_LADY_KAORI', 'KAORI', 'Aroma Lady Frlg', 'Aroma Lady Frlg', 'Female', '''
Bellossom
Level: 48
- Petal Dance
- Moonblast
- Sleep Powder
- Quiver Dance

Jumpluff
Level: 48
- Acrobatics
- Leech Seed
- Sleep Powder
- U-turn

Sunflora
Level: 49
- Solar Beam
- Earth Power
- Sludge Bomb
- Sunny Day
''', music='Female')
    trainer('TRAINER_SEIKA_BUG_MANIAC_ISAMU', 'ISAMU', 'Bug Maniac', 'Bug Maniac', 'Male', '''
Forretress
Level: 48
- Gyro Ball
- Volt Switch
- Spikes
- Rapid Spin

Heracross
Level: 48
- Megahorn
- Close Combat
- Stone Edge
- Night Slash

Yanmega
Level: 49
- Bug Buzz
- Air Slash
- Ancient Power
- U-turn
''')
    for gfx, x, y, label, move, sight in (
            ('OBJ_EVENT_GFX_BUG_CATCHER_FRLG', 53, 9, 'Riku', 'MOVEMENT_TYPE_FACE_LEFT', 3),
            ('OBJ_EVENT_GFX_PICNICKER_FRLG', 44, 4, 'Nanami', 'MOVEMENT_TYPE_FACE_DOWN', 3),
            ('OBJ_EVENT_GFX_WOMAN_2_FRLG', 12, 8, 'Kaori', 'MOVEMENT_TYPE_FACE_DOWN', 3),
            ('OBJ_EVENT_GFX_BUG_CATCHER', 12, 15, 'Isamu', 'MOVEMENT_TYPE_FACE_LEFT', 4)):
        m.objects.append(g.obj(gfx, x, y, '%s_EventScript_%s' % (name, label), move=move,
                               trainer='TRAINER_TYPE_NORMAL', sight=sight))
    m.scripts += battle(name + '_EventScript_Riku', 'TRAINER_SEIKA_BUG_CATCHER_RIKU',
                        'The woods are full of bugs! And\nI caught the best ones!',
                        'Squashed...',
                        'HERACROSS love the sap in these\ntrees. Look in the tall grass!', name)
    m.scripts += battle(name + '_EventScript_Nanami', 'TRAINER_SEIKA_PICNICKER_NANAMI',
                        'A picnic in the woods! Want to\nbattle for my sandwich?',
                        'Aww, you can have the sandwich.',
                        'URSARING smell honey from miles\naway. MOSSGROVE is full of it!', name)
    m.scripts += battle(name + '_EventScript_Kaori', 'TRAINER_SEIKA_AROMA_LADY_KAORI',
                        'Breathe in... The woods smell of\nmoss and flowers. Now, battle!',
                        'What a sweet battle.',
                        'The bees of MOSSGROVE drink from\nthese flowers. Be gentle here.', name)
    m.scripts += battle(name + '_EventScript_Isamu', 'TRAINER_SEIKA_BUG_MANIAC_ISAMU',
                        'Shh! You\'ll scare the bugs!\nOh, you want to battle? Fine!',
                        'My bugs... buzzed off.',
                        'Somebody\'s been digging holes all\nover the woods. It upsets the\nbugs!', name)

    # ---- items
    for x, y, item, fl, label in ((28, 13, 'ITEM_STAR_PIECE', ITEM_W1, 'StarPiece'),
                                  (19, 27, 'ITEM_BIG_ROOT', ITEM_W2, 'BigRoot')):
        m.objects.append(g.obj('OBJ_EVENT_GFX_ITEM_BALL', x, y, '%s_EventScript_Item%s' % (name, label), flag=fl))
        m.scripts += '%s_EventScript_Item%s::\n\tfinditem %s\n\tend\n\n' % (name, label, item)

    wild('MAP_SEIKA_MOSSGROVE_WOODS', 'gSeika_MossgroveWoods',
         land=[('PINECO', 45, 46), ('ARIADOS', 45, 46), ('LEDIAN', 45, 47), ('NOCTOWL', 45, 47),
               ('YANMA', 46, 47), ('TEDDIURSA', 45, 46), ('SUNFLORA', 46, 47), ('HERACROSS', 46, 48),
               ('FORRETRESS', 47, 48), ('URSARING', 47, 48), ('SHUCKLE', 48, 48), ('YANMEGA', 48, 48)],
         water=[('WOOPER', 44, 46), ('MARILL', 44, 46), ('QUAGSIRE', 45, 47), ('AZUMARILL', 46, 48),
                ('POLITOED', 47, 48)],
         fishing=[('MAGIKARP', 42, 44), ('POLIWAG', 42, 44), ('POLIWAG', 42, 45), ('GOLDEEN', 42, 45),
                  ('POLIWHIRL', 44, 46), ('POLIWHIRL', 44, 46), ('SEAKING', 45, 47), ('QUAGSIRE', 45, 47),
                  ('POLIWRATH', 46, 48), ('GYARADOS', 46, 48)])
    return m


# ================================================================ Mossgrove Village

def mossgrove_canvas():
    c = g.Canvas(36, 32, 'gTileset_ViridianCity', seed=41)
    c.forest(0, 0, 16, 6)                 # north opening: x 16..21
    c.forest(22, 0, 14, 6)
    c.forest(0, 6, 8, 26)
    c.forest(28, 6, 8, 6)                 # east opening: rows 12..17
    c.forest(28, 18, 8, 14)
    c.forest(8, 26, 20, 6)
    # streets
    c.rect(18, 0, 2, 23, 'beach')         # north road down to the lower street
    c.rect(9, 14, 27, 2, 'beach')         # main street, out to the east exit
    c.rect(9, 21, 18, 2, 'beach')         # lower street
    # the bee meadow and gardens
    c.rect(9, 23, 18, 3, 'flowers')
    c.rect(9, 6, 7, 2, 'flowers')
    c.rect(22, 6, 5, 2, 'flowers')
    c.rect(26, 16, 2, 4, 'flowers')
    for x in range(9, 27, 3):             # hedges of bushes through the meadow
        c.put(x, 24, 0x005)
    c.tree(26, 9)
    c.tree(26, 11)
    c.tree(16, 6)
    c.tree(20, 6)
    doors = {}
    doors['gym'] = stamp_at(c, GYM, 9, 9)             # door (12,13)
    doors['pc'] = stamp_at(c, PC, 21, 9)              # door (23,13)
    doors['h1'] = stamp_at(c, GREEN_HOUSE, 9, 17)     # door (10,20)
    doors['mart'] = stamp_at(c, MART, 14, 17)         # door (16,20)
    doors['h2'] = green_house_b(c, 21, 17)            # door (22,20)
    c.stamp(*GYM_SIGN, 16, 12)                        # gym sign at (16,13)
    c.put(20, 13, g.SIGN)                             # town sign
    c.put(27, 13, g.WOOD_SIGN)                        # east exit sign
    return c, doors


def mossgrove(w):
    c, d = mossgrove_canvas()
    t = add(g.MapDef(MOSS, 'MAP_SEIKA_MOSSGROVE', SEC_MOSS, c, 'MUS_FORTREE', 'MAP_TYPE_TOWN'))
    link('MAP_SEIKA_MOSSGROVE_WOODS', 'left', 4, 'MAP_SEIKA_MOSSGROVE')   # woods y 18..19 = village y 14..15
    w_gym = door(t, *d['gym'], 'MAP_SEIKA_MOSSGROVE_GYM', 0)
    w_pc = door(t, *d['pc'], 'MAP_SEIKA_MOSSGROVE_POKEMON_CENTER_1F', 0)
    w_mart = door(t, *d['mart'], 'MAP_SEIKA_MOSSGROVE_MART', 0)
    w_h1 = door(t, *d['h1'], 'MAP_SEIKA_MOSSGROVE_HOUSE1', 0)
    w_h2 = door(t, *d['h2'], 'MAP_SEIKA_MOSSGROVE_HOUSE2', 0)

    signpost(t, 20, 13, MOSS + '_EventScript_TownSign',
             'MOSSGROVE VILLAGE\n'
             'Sweet as honey, soft as moss.')
    signpost(t, 16, 13, MOSS + '_EventScript_GymSign',
             'MOSSGROVE POKéMON GYM\n'
             'LEADER: KAYA\fThe beekeeper who never\n'
             'gets stung!')
    signpost(t, 27, 13, MOSS + '_EventScript_EastSign',
             'EAST: MOSSGROVE WOODS\n'
             'INKBRUSH TOWN lies beyond.')
    npc(t, 'OBJ_EVENT_GFX_MAN', 13, 23, MOSS + '_EventScript_Beekeeper',
        'These flowers feed the village\n'
        'COMBEE.\fWe never take all their honey.\n'
        'Half for us, half for the hive.\n'
        'That\'s the MOSSGROVE way.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_LITTLE_GIRL_FRLG', 23, 16, MOSS + '_EventScript_Girl',
        'KAYA can talk to bugs! She just\n'
        'hums, and they listen.', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_HIKER_FRLG', 20, 4, MOSS + '_EventScript_Hiker',
        'This road runs north to KILN CITY,\n'
        'the biggest city in SEIKA.\fThe kilns there burn day and\n'
        'night. You\'ll smell the smoke\n'
        'before you see the walls.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 25, 22, MOSS + '_EventScript_OldWoman',
        'A woman in a fine coat came\n'
        'through not long ago.\fShe asked me which road goes to\n'
        'KILN CITY. Then she asked about\n'
        'a bell.\fWhat would a lady like that\n'
        'want with a bell?', 'MOVEMENT_TYPE_FACE_DOWN')

    pokecenter('Mossgrove', SEC_MOSS, t, w_pc, 'HEAL_LOCATION_SEIKA_MOSSGROVE', npcs=[
        ('OBJ_EVENT_GFX_BUG_CATCHER_FRLG', 11, 5,
         'I caught a HERACROSS in the woods!\fIt tried to eat the honey in my\n'
         'bag. It has good taste!'),
    ])
    mart('Mossgrove', SEC_MOSS, t, w_mart, MOSS_MART, npcs=[
        ('OBJ_EVENT_GFX_WOMAN_1_FRLG', 6, 4,
         'MOSSGROVE HONEY is famous.\fSpread some in tall grass and\n'
         'wild POKéMON come running!'),
    ])
    house('Mossgrove', 1, SEC_MOSS, t, w_h1, 'MUS_FORTREE', [
        ('OBJ_EVENT_GFX_OLD_MAN_1', 6, 3,
         'KAYA\'s family has kept our star\n'
         'shrine for a hundred years.\fThe story goes that the first\n'
         'COMBEE queen hid the WISHING STAR\n'
         'from the storm, under her wings.'),
        ('OBJ_EVENT_GFX_LITTLE_BOY_FRLG', 3, 4,
         'I got stung once. Only once!\n'
         'Now I wear two hats.', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ])
    house('Mossgrove', 2, SEC_MOSS, t, w_h2, 'MUS_FORTREE', [
        ('OBJ_EVENT_GFX_WOMAN_3_FRLG', 4, 3,
         'Moss only grows where the air is\n'
         'clean and the rain is gentle.\fThat\'s why we never cut the old\n'
         'trees around the village.'),
    ], layout='LAYOUT_HOUSE5_FRLG')
    gym_kaya(t, w_gym)
    return t


def gym_kaya(t, w_gym):
    """Mossgrove Gym on Celadon Gym's tiles: a garden of tree hedges, a sand
    path up the middle and two guarded ways round the top hedge."""
    src = 'LAYOUT_CELADON_CITY_GYM'
    W, H = 15, 22
    r = g.Room(W, H, 'gTileset_BuildingFrlg', 'gTileset_CeladonGym', source=src)
    mx = W // 2
    A = r.at
    GRASS = (A(2, 4), A(2, 3))             # (x+y) even / odd
    TREE_TOP = (A(3, 5), A(9, 6))          # even / odd
    TREE_MID, TREE_BOT = A(3, 6), A(3, 9)
    SH_GRASS = (A(1, 6), A(1, 5))          # shaded column x=1: y even / odd
    SH_TREE_TOP = (A(1, 10), A(1, 3))
    SH_TREE_BOT = (A(1, 4), A(1, 11))
    FLOWER = (A(6, 6), A(4, 5))
    SAND, SAND_SH = A(6, 17), A(1, 17)
    S_TL, S_T, S_TR, S_L, S_R = A(5, 9), A(6, 9), A(7, 9), A(5, 12), A(7, 12)
    S_IN_L, S_IN_R, S_T_SH = A(5, 10), A(7, 10), A(1, 15)
    STATUE_T, STATUE_B = A(4, 15), A(4, 16)

    def grass(x, y):
        if x == 1:
            return SH_GRASS[y & 1]
        return GRASS[(x + y) & 1]

    def tree(x, y0, y1):
        """One-wide tree stack from row y0 to y1 (top, joints, base)."""
        for y in range(y0, y1 + 1):
            if x == 1:
                b = SH_TREE_TOP[y & 1] if y == y0 else SH_TREE_BOT[y & 1]
            else:
                b = TREE_TOP[(x + y) & 1] if y == y0 else TREE_BOT if y == y1 else TREE_MID
            r.set(x, y, b)

    for y in range(2, H - 1):
        for x in range(1, W - 1):
            r.set(x, y, grass(x, y))
    # walls
    r.set(0, 0, A(0, 0))
    r.set(1, 0, A(1, 0))
    r.set(W - 2, 0, A(11, 0))
    r.set(W - 1, 0, A(12, 0))
    r.set(0, 1, A(0, 1))
    r.set(1, 1, A(1, 1))
    r.set(W - 2, 1, A(11, 1))
    r.set(W - 1, 1, A(12, 1))
    for x in range(2, W - 2):
        win = x in (3, 4, 10, 11)
        r.set(x, 0, A(4, 0) if win else A(2, 0))
        r.set(x, 1, A(4, 1) if win else A(2, 1))
    r.set(mx, 0, A(6, 0))                  # the gym's emblem
    for y in range(2, H - 3):
        r.set(0, y, A(0, 2))
        r.set(W - 1, y, A(12, 2))
    for x in (0, W - 1):
        r.set(x, H - 3, A(0, 17))
        r.set(x, H - 2, A(0, 18))
    # shadow under the top wall
    r.set(1, 2, A(1, 2))
    r.set(2, 2, A(2, 2))
    for x in range(3, W - 2):
        r.set(x, 2, A(10, 2))
    r.set(W - 2, 2, A(11, 2))
    # hedges
    for x in range(3, 12):                 # top hedge, open at both ends
        tree(x, 7, 8)
    for x in list(range(1, 6)) + list(range(9, 14)):   # lower hedge, open in the middle
        tree(x, 12, 13)
    for x in (2, 12):
        tree(x, 3, 4)
    # sand: plaza by the door and a path up the middle to row 10
    for y in range(16, H - 1):
        for x in range(1, W - 1):
            r.set(x, y, SAND_SH if x == 1 else SAND)
    for x in range(1, W - 1):
        r.set(x, 16, S_T_SH if x == 1 else S_T)
    r.set(mx - 1, 10, S_TL)
    r.set(mx, 10, S_T)
    r.set(mx + 1, 10, S_TR)
    for y in range(11, 16):
        r.set(mx - 1, y, S_L)
        r.set(mx, y, SAND)
        r.set(mx + 1, y, S_R)
    r.set(mx - 1, 16, S_IN_L)
    r.set(mx, 16, SAND)
    r.set(mx + 1, 16, S_IN_R)
    for x in (mx - 3, mx + 3):             # statues on the plaza's edge
        r.set(x, 16, STATUE_T)
        r.set(x, 17, STATUE_B)
    # flowers for the bees
    for x, y in ((4, 4), (5, 5), (9, 5), (10, 4), (6, 6), (8, 6), (3, 10), (11, 10),
                 (2, 14), (4, 15), (10, 15), (12, 14), (5, 10), (9, 10)):
        r.set(x, y, FLOWER[(x + y) & 1])
    # bottom wall and mat
    r.fill(0, H - 1, W, 1, A(0, 19))
    for i in range(3):
        r.set(mx - 1 + i, H - 2, A(5 + i, 18))
        r.set(mx - 1 + i, H - 1, A(5 + i, 19))

    name = MOSS + '_Gym'
    m = add(g.MapDef(name, 'MAP_SEIKA_MOSSGROVE_GYM', SEC_MOSS, r, 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (mx - 1, mx, mx + 1):
        door(m, k, H - 2, t.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_WOMAN_3_FRLG', mx, 4, name + '_EventScript_Kaya'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', mx - 2, H - 4, name + '_EventScript_GymGuy'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_BUG_CATCHER', 4, 9, name + '_EventScript_Tetsu',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_WOMAN_2_FRLG', 10, 9, name + '_EventScript_Mitsu',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=3))
    gym_statues(m, name, ((mx - 3, 17), (mx + 3, 17)), BADGE3,
                'MOSSGROVE POKéMON GYM\nLEADER: KAYA',
                'MOSSGROVE POKéMON GYM\nLEADER: KAYA\fWINNING TRAINERS:\n{PLAYER}')

    trainer('TRAINER_SEIKA_BUG_MANIAC_TETSU', 'TETSU', 'Bug Maniac', 'Bug Maniac', 'Male', '''
Ariados
Level: 53
- Poison Jab
- Sucker Punch
- X-Scissor
- Sticky Web

Ledian
Level: 53
- Air Slash
- Bug Buzz
- Focus Blast
- Roost

Heracross
Level: 54
- Megahorn
- Close Combat
- Stone Edge
- Knock Off
''')
    trainer('TRAINER_SEIKA_PKMN_BREEDER_MITSU', 'MITSU', 'Pkmn Breeder Frlg', 'Pokemon Breeder Frlg', 'Female', '''
Beedrill
Level: 54
- Poison Jab
- X-Scissor
- Drill Run
- Swords Dance

Butterfree
Level: 55
- Bug Buzz
- Air Slash
- Sleep Powder
- Quiver Dance
''', music='Female')
    trainer('TRAINER_SEIKA_LEADER_KAYA', 'KAYA', 'Leader Frlg', 'Pokemon Breeder Frlg', 'Female', '''
Forretress
Level: 56
%(iv)s
- Gyro Ball
- Volt Switch
- Spikes
- Explosion

Yanmega
Level: 56
%(iv)s
- Bug Buzz
- Air Slash
- Ancient Power
- U-turn

Heracross @ Sitrus Berry
Level: 56
%(iv)s
- Megahorn
- Close Combat
- Stone Edge
- Knock Off

Scizor @ Metal Coat
Level: 56
%(iv)s
- Bullet Punch
- U-turn
- Swords Dance
- Brick Break

Vespiquen @ Leftovers
Level: 57
%(iv)s
- Attack Order
- Defend Order
- Heal Order
- Power Gem
''' % {'iv': IV31}, music='Female', ai='Smart Trainer')

    m.scripts += battle(name + '_EventScript_Tetsu', 'TRAINER_SEIKA_BUG_MANIAC_TETSU',
                        'Bzzz! You walked right into my\nweb!',
                        'My web... it tore!',
                        'KAYA hums to her bugs before a\nbattle. It makes them brave!', name)
    m.scripts += battle(name + '_EventScript_Mitsu', 'TRAINER_SEIKA_PKMN_BREEDER_MITSU',
                        'Careful! The hives are buzzing\ntoday. So are my POKéMON!',
                        'Oh, the poor dears.',
                        'KAYA\'s VESPIQUEN rules the hive.\nIt heals itself, so hit it hard!', name)
    m.scripts += leader_script(
        name, 'Kaya', 'TRAINER_SEIKA_LEADER_KAYA', BADGE3, 'ITEM_TM_GIGA_DRAIN',
        'Hello! I\'m KAYA. I keep the bees,\n'
        'and the bees keep the shrine.\fA hive is a thousand small wings\n'
        'working as one.\fWe\'ll swarm you, sting you, and\n'
        'then heal right up. Ready?',
        'Bzz... Our hive has never been\n'
        'beaten so sweetly!\fThe GROVE BADGE is yours.\f'
        '{FONT_NORMAL}{PLAYER} received the GROVE BADGE\n'
        'from KAYA!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}',
        'Take this TM, too. It drinks your\n'
        'foe\'s strength, the way my bees\n'
        'drink nectar.',
        'The road north of the village\n'
        'goes to KILN CITY.\fTOBIN keeps the shrine there,\n'
        'and its kilns are the only ones\n'
        'that make the STARGLASS BELL.\fIf that woman wants the bell,\n'
        'KILN CITY is where she\'s gone.\n'
        'Hurry, {PLAYER}!')
    m.scripts += gym_guy(name, BADGE3,
                         'Yo! KAYA\'s hive is all BUG\n'
                         'POKéMON.\fFIRE, FLYING and ROCK moves\n'
                         'smoke them out!\fCareful with her SCIZOR. Only\n'
                         'FIRE really scares it.',
                         'You out-buzzed the queen bee!\n'
                         'Sweet!')
