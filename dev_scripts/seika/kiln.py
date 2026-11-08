"""Kiln Road, Kiln City (Gym 4, Ground), Tide Road and Tidewatch Town (Gym 5, Water)."""
from common import (g, T, add, door, npc, signpost, pokecenter, mart, house, wild, mapsec,
                    trainer, battle, flag, link, PC, MART, GYM, MAPS)

CEL = 'LAYOUT_CELADON_CITY'
CER = 'LAYOUT_CERULEAN_CITY'

SEC_KROAD = mapsec('MAPSEC_SEIKA_KILN_ROAD', 'KILN ROAD')
SEC_KILN = mapsec('MAPSEC_SEIKA_KILN_CITY', 'KILN CITY')
SEC_TROAD = mapsec('MAPSEC_SEIKA_TIDE_ROAD', 'TIDE ROAD')
SEC_TIDE = mapsec('MAPSEC_SEIKA_TIDEWATCH', 'TIDEWATCH TOWN')

BADGE4 = flag('FLAG_QOL_SEIKA_BADGE04', 'KILN BADGE (Tobin)')
BADGE5 = flag('FLAG_QOL_SEIKA_BADGE05', 'TIDE BADGE (Nagi)')
DENEB_DONE = flag('FLAG_QOL_SEIKA_DENEB_KILN', 'beat Deneb in the Kiln City workshop (Starglass Bell)')
HIDE_DENEB = flag('FLAG_QOL_SEIKA_HIDE_DENEB_KILN', 'Deneb gone from the Kiln City workshop')
ITEM_KROAD = flag('FLAG_QOL_SEIKA_ITEM_KILN_ROAD', 'Kiln Road item ball')
ITEM_TROAD = flag('FLAG_QOL_SEIKA_ITEM_TIDE_ROAD', 'Tide Road item ball')

P_KROAD = 'Seika_KilnRoad'
P_KILN = 'Seika_KilnCity'
P_TROAD = 'Seika_TideRoad'
P_TIDE = 'Seika_Tidewatch'

FULL_IVS = 'IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe'


# ---------------------------------------------------------------- painting helpers

ROCK = [[0x068, 0x069, 0x06A], [0x070, 0x071, 0x072], [0x078, 0x079, 0x07A]]


def rock(c, x, y, w, h):
    """A rocky outcrop (Route 3's mountain rock), w and h >= 2."""
    for yy in range(h):
        for xx in range(w):
            r = 0 if yy == 0 else 2 if yy == h - 1 else 1
            k = 0 if xx == 0 else 2 if xx == w - 1 else 1
            c.put(x + xx, y + yy, ROCK[r][k])
            c.cls[y + yy][x + xx] = 'fixed'


# Celadon's grey pavement (primary tiles): edges by neighbour.
PAVE_IDS = {0x165, 0x166, 0x167, 0x16D, 0x16E, 0x16F, 0x175, 0x176, 0x177, 0x195, 0x196, 0x19D, 0x19E}
NOTCH_IDS = {0x30E, 0x30F, 0x317, 0x31F}   # Celadon's door notches (secondary)


class CityCanvas(g.Canvas):
    """Canvas with an extra terrain class, 'pave': Celadon's grey pavement."""

    def _pavish(self, x, y):
        if not (0 <= x < self.w and 0 <= y < self.h):
            x, y = min(max(x, 0), self.w - 1), min(max(y, 0), self.h - 1)
        if self.cls[y][x] == 'pave':
            return True
        b = self.fixed.get((x, y))
        return b is not None and (b & 0x3FF) in PAVE_IDS | NOTCH_IDS

    def resolve(self):
        out = super().resolve()
        for y in range(self.h):
            for x in range(self.w):
                if self.cls[y][x] != 'pave' or (x, y) in self.fixed or out[y][x]:
                    continue
                p = lambda dx, dy: self._pavish(x + dx, y + dy)
                n, s, e, w = p(0, -1), p(0, 1), p(1, 0), p(-1, 0)
                if not n:
                    mid = 0x165 if not w else 0x167 if not e else 0x166
                elif not s:
                    mid = 0x175 if not w else 0x177 if not e else 0x176
                elif not w:
                    mid = 0x16D
                elif not e:
                    mid = 0x16F
                elif not p(-1, -1):
                    mid = 0x195
                elif not p(1, -1):
                    mid = 0x196
                elif not p(-1, 1):
                    mid = 0x19D
                elif not p(1, 1):
                    mid = 0x19E
                else:
                    mid = 0x16E
                out[y][x] = self.block(mid)
        return out


def notch(c, dx, dy):
    """Celadon's 1-wide pavement notch from a door at (dx, dy) to a street
    whose top row is dy + 2."""
    for x, y, mid in ((dx - 1, dy + 1, 0x30E), (dx, dy + 1, 0x16E), (dx + 1, dy + 1, 0x30F),
                      (dx - 1, dy + 2, 0x317), (dx, dy + 2, 0x16E), (dx + 1, dy + 2, 0x31F)):
        c.put(x, y, mid, 3 << 12)
        c.cls[y][x] = 'fixed'


def item_ball(m, x, y, label, item, flg):
    m.objects.append(g.obj('OBJ_EVENT_GFX_ITEM_BALL', x, y, label, flag=flg))
    m.scripts += '%s::\n\tfinditem %s\n\tend\n\n' % (label, item)


def leader_script(name, who, const, badge, tm, intro, defeat, gift, post):
    """Wick's Gym Leader script, for another leader."""
    s = (name + '_EventScript_%s::\n'
         '\ttrainerbattle_single %s, %s_Text_%sIntro, %s_Text_%sDefeat, %s_EventScript_%sDefeated, NO_MUSIC\n'
         '\tmsgbox %s_Text_%sPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         '%s_EventScript_%sDefeated::\n'
         '\tsetflag %s\n'
         '\tmsgbox %s_Text_%sGift, MSGBOX_DEFAULT\n'
         '\tgiveitem %s\n'
         '\tmsgbox %s_Text_%sPost, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         % (who, const, name, who, name, who, name, who, name, who, name, who, badge, name, who, tm, name, who))
    s += T('%s_Text_%sIntro' % (name, who), intro)
    s += T('%s_Text_%sDefeat' % (name, who), defeat)
    s += T('%s_Text_%sGift' % (name, who), gift)
    s += T('%s_Text_%sPost' % (name, who), post)
    return s


def gym_guy(name, badge, text, won):
    s = (name + '_EventScript_GymGuy::\n\tlock\n\tfaceplayer\n'
         '\tgoto_if_set %s, %s_EventScript_GymGuyWon\n'
         '\tmsgbox %s_Text_GymGuy, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         '%s_EventScript_GymGuyWon::\n\tmsgbox %s_Text_GymGuyWon, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         % (badge, name, name, name, name))
    return s + T(name + '_Text_GymGuy', text) + T(name + '_Text_GymGuyWon', won)


def gym_statue(m, name, xs, badge, text):
    """Statues by the entrance: 'WINNING TRAINERS' once the badge is won."""
    for x, y in xs:
        m.bgs.append(g.sign(x, y, name + '_EventScript_GymStatue', 'BG_EVENT_PLAYER_FACING_ANY'))
    m.scripts += (name + '_EventScript_GymStatue::\n\tlockall\n'
                  '\tgoto_if_set %s, %s_EventScript_GymStatueWon\n'
                  '\tmsgbox %s_Text_GymStatue, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
                  '%s_EventScript_GymStatueWon::\n\tmsgbox %s_Text_GymStatueWon, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
                  % (badge, name, name, name, name))
    m.scripts += T(name + '_Text_GymStatue', text + '\fWINNING TRAINERS:\n(none yet)')
    m.scripts += T(name + '_Text_GymStatueWon', text + '\fWINNING TRAINERS:\n{PLAYER}')


# ---------------------------------------------------------------- build

def build():
    road = kiln_road()
    if 'kiln_city' not in globals():
        add(g.MapDef(P_KILN, 'MAP_SEIKA_KILN_CITY', SEC_KILN, kiln_city_canvas(), 'MUS_RG_CELADON'))
    if 'kiln_city' in globals():
        city = kiln_city(road)
        if 'tide_road' in globals():
            troad = tide_road(city)
            if 'tidewatch' in globals():
                tidewatch(troad)


# ---------------------------------------------------------------- Kiln Road

def kiln_road():
    c = g.Canvas(24, 60, 'gTileset_PewterCity', seed=41)
    c.forest(0, 0, 8, 6)                  # north end: opening to Kiln City
    c.forest(14, 0, 10, 6)
    c.forest(0, 6, 4, 48)
    c.forest(20, 6, 4, 48)
    c.forest(0, 54, 8, 6)                 # south end: opening to Mossgrove
    c.forest(14, 54, 10, 6)
    # the road climbs north through the foothills
    c.rect(10, 46, 2, 14, 'path')
    c.rect(10, 46, 8, 2, 'path')
    c.rect(16, 30, 2, 18, 'path')
    c.rect(6, 30, 12, 2, 'path')
    c.rect(6, 16, 2, 16, 'path')
    c.rect(6, 16, 6, 2, 'path')
    c.rect(10, 0, 2, 18, 'path')
    # tall grass
    c.rect(13, 7, 6, 6, 'tall')
    c.rect(9, 20, 7, 7, 'tall')
    c.rect(4, 40, 6, 5, 'tall')
    c.rect(12, 49, 8, 4, 'tall')
    c.rect(4, 48, 4, 4, 'flowers')
    # foothills: rock outcrops and ledges
    rock(c, 4, 7, 3, 5)
    rock(c, 17, 19, 3, 4)
    rock(c, 4, 21, 2, 5)
    rock(c, 11, 35, 4, 5)
    rock(c, 18, 34, 2, 4)
    c.ledge(4, 33, 12)                    # hop down from the bend, can't climb back
    c.ledge(4, 46, 6)
    c.tree(8, 10)
    c.tree(14, 43)
    c.put(9, 49, g.SIGN)
    c.put(12, 2, g.SIGN)

    m = add(g.MapDef(P_KROAD, 'MAP_SEIKA_KILN_ROAD', SEC_KROAD, c, 'MUS_RG_ROUTE3', 'MAP_TYPE_ROUTE'))
    link('MAP_SEIKA_MOSSGROVE', 'up', 8, 'MAP_SEIKA_KILN_ROAD')
    signpost(m, 9, 49, P_KROAD + '_EventScript_SignSouth', 'KILN ROAD\nMOSSGROVE VILLAGE - KILN CITY')
    signpost(m, 12, 2, P_KROAD + '_EventScript_SignNorth', 'KILN ROAD\nKILN CITY - MOSSGROVE VILLAGE')
    item_ball(m, 5, 35, P_KROAD + '_EventScript_ItemPPUp', 'ITEM_PP_UP', ITEM_KROAD)
    npc(m, 'OBJ_EVENT_GFX_OLD_MAN_1', 12, 44, P_KROAD + '_EventScript_OldMan',
        'The clay of these hills is red and\n'
        'fine. KILN CITY was built on it.\f'
        'Its kilns have burned day and\n'
        'night for six hundred years.', 'MOVEMENT_TYPE_FACE_LEFT')
    trainer('TRAINER_SEIKA_HIKER_BUNTA', 'BUNTA', 'Hiker Frlg', 'Hiker Frlg', 'Male', '''
Sudowoodo
Level: 48
- Wood Hammer
- Head Smash
- Sucker Punch
- Low Kick

Golem
Level: 49
- Earthquake
- Stone Edge
- Explosion
- Rock Polish

Donphan
Level: 49
- Earthquake
- Ice Shard
- Knock Off
- Rapid Spin
''')
    trainer('TRAINER_SEIKA_PICNICKER_HANA', 'HANA', 'Picnicker Frlg', 'Picnicker Frlg', 'Female', '''
Bellossom
Level: 49
- Petal Blizzard
- Moonblast
- Sleep Powder
- Quiver Dance

Girafarig
Level: 49
- Psychic
- Hyper Voice
- Thunderbolt
- Calm Mind

Ampharos
Level: 50
- Thunderbolt
- Dragon Pulse
- Power Gem
- Cotton Guard
''', music='Female')
    trainer('TRAINER_SEIKA_BLACK_BELT_RYUJI', 'RYUJI', 'Black Belt Frlg', 'Black Belt Frlg', 'Male', '''
Hitmontop
Level: 50
- Close Combat
- Sucker Punch
- Triple Axel
- Fake Out

Heracross
Level: 50
- Megahorn
- Close Combat
- Stone Edge
- Night Slash

Ursaring
Level: 51
- Facade
- Crunch
- Close Combat
- Swords Dance
''')
    m.objects.append(g.obj('OBJ_EVENT_GFX_HIKER_FRLG', 18, 40, P_KROAD + '_EventScript_Bunta',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_PICNICKER_FRLG', 12, 16, P_KROAD + '_EventScript_Hana',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_BLACK_BELT_FRLG', 5, 28, P_KROAD + '_EventScript_Ryuji',
                           move='MOVEMENT_TYPE_FACE_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.scripts += battle(P_KROAD + '_EventScript_Bunta', 'TRAINER_SEIKA_HIKER_BUNTA',
                        'I dig clay for the potters of\nKILN CITY. My rocks dig back!',
                        'Crumbled like dry clay!',
                        'Good clay needs water and fire.\nGood POKéMON need both, too.', P_KROAD)
    m.scripts += battle(P_KROAD + '_EventScript_Hana', 'TRAINER_SEIKA_PICNICKER_HANA',
                        'A picnic in the foothills! And\na battle for dessert!',
                        'That was a bitter dessert...',
                        'I bought a teacup in KILN CITY.\nIt is too pretty to drink from!', P_KROAD)
    m.scripts += battle(P_KROAD + '_EventScript_Ryuji', 'TRAINER_SEIKA_BLACK_BELT_RYUJI',
                        'I train on this steep road every\nmorning. Hoo-hah!',
                        'My legs gave out!',
                        'TOBIN, the GYM LEADER, carries\nhis own clay up from these hills.', P_KROAD)
    wild('MAP_SEIKA_KILN_ROAD', 'gSeika_KilnRoad',
         land=[('PHANPY', 48, 49), ('GLIGAR', 48, 49), ('TEDDIURSA', 48, 50), ('FLAAFFY', 48, 50),
               ('GIRAFARIG', 49, 50), ('DUNSPARCE', 49, 50), ('DONPHAN', 50, 51), ('URSARING', 50, 51),
               ('SKARMORY', 50, 51), ('SUDOWOODO', 50, 51), ('LARVITAR', 48, 49), ('PUPITAR', 51, 51)])
    return m


# ---------------------------------------------------------------- Kiln City

# Celadon City buildings (layout, x, y, w, h)
KILN_A = (CEL, 44, 17, 4, 5)        # house with a chimney: a kiln
KILN_B = (CEL, 48, 17, 4, 5)        # house with a vent
TOWER_S = (CEL, 4, 4, 5, 8)
TOWER_L = (CEL, 18, 4, 7, 8)
FOUNTAIN = (CEL, 12, 18, 3, 4)
GYM_SIGN = (CEL, 16, 30, 1, 2)      # primary tiles


def kiln_house(c, x, y):
    """Celadon's chimney house with a door, on grass (the roof row comes
    from a house that stands on grass, the rest from one with a door).
    Returns the door."""
    c.stamp(CEL, 44, 17, 4, 1, x, y)
    c.stamp(CEL, 36, 26, 4, 4, x, y + 1)
    return x + 1, y + 4


def kiln_city_canvas():
    c = CityCanvas(56, 44, 'gTileset_CeladonCity', seed=43)
    c.forest(0, 0, 56, 6)
    c.forest(0, 6, 8, 32)
    c.forest(48, 6, 8, 12)
    c.forest(48, 24, 8, 14)
    c.forest(0, 38, 24, 6)                # south: opening to Kiln Road
    c.forest(30, 38, 26, 6)
    # streets
    c.rect(9, 12, 38, 2, 'pave')
    c.rect(9, 20, 47, 2, 'pave')          # the avenue, east to Tide Road
    c.rect(9, 30, 38, 2, 'pave')
    c.rect(26, 12, 2, 32, 'pave')         # south to Kiln Road
    c.rect(9, 12, 2, 20, 'pave')
    c.rect(45, 12, 2, 20, 'pave')
    # band A (doors on the north street)
    for st, x, y in ((PC, 11, 7), (MART, 17, 8), (GYM, 30, 7), (GYM_SIGN, 29, 10), (KILN_B, 42, 7)):
        c.stamp(*st[:5], x, y)
    kiln_house(c, 22, 7)                  # pottery shop, door (23,11)
    kiln_house(c, 38, 7)                  # house 1, door (39,11)
    # the plaza
    c.stamp(*FOUNTAIN, 17, 15)
    for x, y in ((15, 15), (15, 18), (21, 15), (21, 18), (16, 15), (20, 18), (16, 18), (20, 15)):
        c.cells([(x, y)], 'flowers')
    c.tree(12, 16)
    c.tree(23, 16)
    # the kiln quarter: the workshop and its kilns
    kiln_house(c, 30, 14)                 # workshop, door (31,18)
    notch(c, 31, 18)
    c.stamp(*KILN_A, 35, 14)
    c.stamp(*KILN_A, 39, 14)
    # towers and a house on the south street
    c.stamp(*TOWER_S, 12, 22)
    kiln_house(c, 20, 25)                 # house 2, door (21,29)
    c.rect(18, 23, 7, 1, 'flowers')
    c.stamp(*TOWER_L, 30, 22)
    c.stamp(*KILN_B, 39, 25)
    # the south park
    c.rect(12, 33, 12, 3, 'flowers')
    c.rect(30, 33, 13, 3, 'flowers')
    c.tree(14, 33)
    c.tree(20, 33)
    c.tree(34, 33)
    c.tree(40, 33)
    c.put(24, 34, g.SIGN)
    return c


def kiln_gym_room():
    """Gym 4 on Viridian Gym's tiles: kiln-brick walls in a zigzag, no
    spinner floor. Returns (room, exit x range, mat row)."""
    src = 'LAYOUT_VIRIDIAN_CITY_GYM'
    W, H = 15, 20
    r = g.Room(W, H, 'gTileset_BuildingFrlg', 'gTileset_ViridianGym', source=src)
    FLOOR, DARK = r.at(1, 16), r.at(0, 16)        # 3281, 3282
    r.fill(0, 0, W, H, FLOOR)
    r.fill(0, 0, W, 1, r.at(5, 0))                # back wall
    r.fill(0, 1, W, 1, r.at(5, 1))
    r.fill(0, 2, W, 1, DARK)                      # its shadow
    r.fill(0, 2, 1, H - 4, DARK)                  # the west wall's shadow
    mx = W // 2
    r.set(mx, 0, r.at(2, 0))                      # emblem
    r.copy(1, 1, 3, 3, mx - 1, 1)                 # the leader's mat under it

    def wall(x0, x1, y):
        """A free-standing kiln wall: top at row y, face at row y + 1."""
        for x in range(x0, x1 + 1):
            end = 0 if x == x0 else 2 if x == x1 else 1
            r.set(x, y - 1, (0x3292, 0x3293, 0x3294)[end])
            r.set(x, y, (0x069D, 0x069B, 0x069C)[end])
            r.set(x, y + 1, (0x06A2, 0x06A3, 0x06A4)[end])
            r.set(x, y + 2, 0x3283 if x == x0 else 0x3284)
        if x1 + 1 < W:
            r.set(x1 + 1, y, 0x3286)
            r.set(x1 + 1, y + 1, 0x3286)
            r.set(x1 + 1, y + 2, 0x3285)
    wall(3, W - 1, 5)
    wall(0, W - 4, 9)
    wall(3, W - 1, 13)
    # statues by the entrance, the exit mat on the bottom wall
    for x in (mx - 3, mx + 3):
        r.set(x, H - 4, r.at(15, 19))
        r.set(x, H - 3, r.at(15, 20))
    r.fill(0, H - 1, W, 1, r.at(0, 23))
    for i in range(3):
        r.set(mx - 1 + i, H - 2, r.at(16 + i, 22))
        r.set(mx - 1 + i, H - 1, r.at(16 + i, 23))
    return r


KILN_MART = ['ITEM_ULTRA_BALL', 'ITEM_GREAT_BALL', 'ITEM_QUICK_BALL', 'ITEM_DUSK_BALL', 'ITEM_TIMER_BALL',
             'ITEM_HEAL_BALL', 'ITEM_HYPER_POTION', 'ITEM_MAX_POTION', 'ITEM_FULL_RESTORE', 'ITEM_REVIVE',
             'ITEM_FULL_HEAL', 'ITEM_MAX_REPEL', 'ITEM_ESCAPE_ROPE', 'ITEM_X_ATTACK', 'ITEM_X_DEFENSE',
             'ITEM_X_SP_ATK', 'ITEM_X_SP_DEF', 'ITEM_X_SPEED']
POTTERY_SHOP = ['ITEM_HEAT_ROCK', 'ITEM_DAMP_ROCK', 'ITEM_SMOOTH_ROCK', 'ITEM_ICY_ROCK', 'ITEM_SOFT_SAND',
                'ITEM_HARD_STONE']


def pc_maps():
    return MAPS[-2:]


def tidy(maps, town):
    """common's helpers name maps after the town string, which also makes
    the ids ('Kiln_City' -> MAP_SEIKA_KILN_CITY_*). Give them the usual
    Seika_<Town>_* folder names."""
    for m in maps:
        assert m.name.startswith('Seika_Kiln_City_'), m.name
        m.name = 'Seika_%s_' % town + m.name[len('Seika_Kiln_City_'):]


def shop_clerk(m, name, x, y, items, text):
    """A clerk with a pokemart list, standing behind a counter."""
    m.objects.append(g.obj('OBJ_EVENT_GFX_CLERK', x, y, name + '_EventScript_Clerk'))
    m.scripts += ('%s_EventScript_Clerk::\n\tlock\n\tfaceplayer\n\tmessage %s_Text_Clerk\n\twaitmessage\n'
                  '\tpokemart %s_Items\n\tmsgbox gText_PleaseComeAgain\n\trelease\n\tend\n\n\t.align 2\n%s_Items::\n'
                  % (name, name, name, name) + ''.join('\t.2byte %s\n' % it for it in items)
                  + '\t.2byte ITEM_NONE\n\trelease\n\tend\n\n' + T(name + '_Text_Clerk', text))


def kiln_city(road):
    c = kiln_city_canvas()
    c.put(47, 19, g.SIGN)
    t = add(g.MapDef(P_KILN, 'MAP_SEIKA_KILN_CITY', SEC_KILN, c, 'MUS_RG_CELADON', 'MAP_TYPE_CITY'))
    link('MAP_SEIKA_KILN_ROAD', 'up', -16, 'MAP_SEIKA_KILN_CITY')   # road path x10-11 = city x26-27
    w_pc = door(t, 13, 11, 'MAP_SEIKA_KILN_CITY_POKEMON_CENTER_1F', 0)
    w_mart = door(t, 19, 11, 'MAP_SEIKA_KILN_CITY_MART', 0)
    w_shop = door(t, 23, 11, 'MAP_SEIKA_KILN_CITY_POTTERY_SHOP', 0)
    w_gym = door(t, 33, 11, 'MAP_SEIKA_KILN_CITY_GYM', 0)
    w_h1 = door(t, 39, 11, 'MAP_SEIKA_KILN_CITY_HOUSE1', 0)
    w_work = door(t, 31, 18, 'MAP_SEIKA_KILN_CITY_WORKSHOP', 0)
    w_h2 = door(t, 21, 29, 'MAP_SEIKA_KILN_CITY_HOUSE2', 0)

    signpost(t, 24, 34, P_KILN + '_EventScript_CitySign',
             'KILN CITY\n'
             'A thousand kilns, one fire.')
    signpost(t, 29, 11, P_KILN + '_EventScript_GymSign',
             'KILN CITY POKéMON GYM\n'
             'LEADER: TOBIN\fThe potter whose clay never\n'
             'cracks!')
    signpost(t, 47, 19, P_KILN + '_EventScript_EastSign',
             'TIDE ROAD ahead\n'
             'TIDEWATCH TOWN to the east')
    npc(t, 'OBJ_EVENT_GFX_WORKER_M', 34, 19, P_KILN + '_EventScript_KilnWorker',
        'These are the oldest kilns in\n'
        'SEIKA. Their fire has not gone\n'
        'out in six hundred years.\fThe old masters fired a bell of\n'
        'starglass here, long ago. They say\n'
        'it sings when the star is near.', 'MOVEMENT_TYPE_FACE_UP')
    npc(t, 'OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 18, 19, P_KILN + '_EventScript_Fountain',
        'The water in this fountain comes\n'
        'down from the hills.\fThe potters say good clay needs\n'
        'good water. So does good tea!', 'MOVEMENT_TYPE_FACE_UP')
    npc(t, 'OBJ_EVENT_GFX_LASS_FRLG', 36, 13, P_KILN + '_EventScript_Lass',
        'TOBIN the potter is our GYM\n'
        'LEADER. His pots ring like bells\n'
        'when you tap them!', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_FAT_MAN_FRLG', 15, 32, P_KILN + '_EventScript_Man',
        'KILN ROAD goes south, back to\n'
        'MOSSGROVE VILLAGE.\fThe hills along it are full of\n'
        'red clay. And grumpy DONPHAN.', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_YOUNGSTER_FRLG', 41, 22, P_KILN + '_EventScript_Youngster',
        'A lady in a fine coat walked into\n'
        'the old workshop this morning.\fShe had a strange bag with a star\n'
        'on it. Is she a collector?', 'MOVEMENT_TYPE_FACE_DOWN', flag=HIDE_DENEB)

    pokecenter('Kiln_City', SEC_KILN, t, w_pc, 'HEAL_LOCATION_SEIKA_KILN_CITY', npcs=[
        ('OBJ_EVENT_GFX_GENTLEMAN_FRLG', 11, 5,
         'I came all the way from KANTO for\n'
         'a KILN CITY teapot.\fNow I need a second bag to carry\n'
         'it home!'),
    ])
    tidy(pc_maps(), 'KilnCity')
    mart('Kiln_City', SEC_KILN, t, w_mart, KILN_MART, npcs=[
        ('OBJ_EVENT_GFX_WOMAN_1_FRLG', 6, 4,
         'This MART has more than any other\n'
         'in SEIKA. Merchants come from\n'
         'every town to KILN CITY.'),
    ])
    tidy(MAPS[-1:], 'KilnCity')
    shop = add(g.MapDef(P_KILN + '_PotteryShop', 'MAP_SEIKA_KILN_CITY_POTTERY_SHOP', SEC_KILN,
                        'LAYOUT_HOUSE2_FRLG', 'MUS_RG_CELADON', 'MAP_TYPE_INDOOR', 'WEATHER_NONE', False, False, False))
    for k in (3, 4, 5):
        door(shop, k, 7, t.id, w_shop)
    shop_clerk(shop, P_KILN + '_PotteryShop', 5, 3, POTTERY_SHOP,
               'Welcome! These stones were fired\n'
               'in our kilns.\fEach one holds a little weather.\n'
               'Take a look!')
    npc(shop, 'OBJ_EVENT_GFX_OLD_MAN_1', 8, 4, P_KILN + '_PotteryShop_EventScript_Potter',
        'A pot remembers the fire that\n'
        'made it. So does a stone.\fA HEAT ROCK from our kilns makes\n'
        'the sun shine longer in battle.', 'MOVEMENT_TYPE_FACE_LEFT')
    house('Kiln_City', 1, SEC_KILN, t, w_h1, 'MUS_RG_CELADON', [
        ('OBJ_EVENT_GFX_WORKER_F', 6, 3,
         'I paint glaze on the pots. Blue\n'
         'for the sea, red for the kiln...\fAnd gold for the WISHING STAR.\n'
         'That one sells best!'),
        ('OBJ_EVENT_GFX_LITTLE_BOY_FRLG', 3, 4,
         'My GLIGAR can glide all the way\n'
         'from the GYM roof to the fountain!', 'MOVEMENT_TYPE_WANDER_AROUND'),
    ], layout='LAYOUT_HOUSE1_FRLG')
    tidy(MAPS[-1:], 'KilnCity')
    house('Kiln_City', 2, SEC_KILN, t, w_h2, 'MUS_RG_CELADON', [
        ('OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 4, 3,
         'Every town in SEIKA keeps a star\n'
         'shrine, and every shrine keeper\n'
         'leads a GYM.\fTOBIN keeps the shrine of the\n'
         'kilns. Fire, earth and patience.'),
    ], layout='LAYOUT_HOUSE5_FRLG')
    tidy(MAPS[-1:], 'KilnCity')
    workshop(t, w_work)
    kiln_gym(t, w_gym)
    return t


def workshop(t, w_work):
    """The old kiln workshop: Deneb buys the Starglass Bell (Warden's House tiles)."""
    name = P_KILN + '_Workshop'
    m = add(g.MapDef(name, 'MAP_SEIKA_KILN_CITY_WORKSHOP', SEC_KILN, 'LAYOUT_FUCHSIA_CITY_WARDENS_HOUSE',
                     'MUS_RG_CELADON', 'MAP_TYPE_INDOOR', 'WEATHER_NONE', False, False, False))
    door(m, 6, 9, t.id, w_work)
    door(m, 5, 10, t.id, w_work, elevation=0)
    door(m, 7, 10, t.id, w_work, elevation=0)
    deneb = 'LOCALID_SEIKA_KILN_CITY_WORKSHOP_DENEB'
    m.objects.append(g.obj('OBJ_EVENT_GFX_OLD_MAN_1', 11, 5, name + '_EventScript_Master',
                           local_id='LOCALID_SEIKA_KILN_CITY_WORKSHOP_MASTER'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_HEX_MANIAC', 11, 6, name + '_EventScript_Deneb', local_id=deneb,
                           move='MOVEMENT_TYPE_FACE_UP', flag=HIDE_DENEB))
    npc(m, 'OBJ_EVENT_GFX_WORKER_M', 2, 6, name + '_EventScript_Apprentice',
        'We fire the glaze three times.\n'
        'Once for the clay, once for the\n'
        'color, once for luck.', 'MOVEMENT_TYPE_FACE_RIGHT')
    for x in (4, 5):
        m.bgs.append(g.sign(x, 4, name + '_EventScript_DisplayBowls', 'BG_EVENT_PLAYER_FACING_NORTH'))
    for x in (6, 7):
        m.bgs.append(g.sign(x, 4, name + '_EventScript_DisplayCase', 'BG_EVENT_PLAYER_FACING_NORTH'))
    m.scripts += (name + '_EventScript_DisplayBowls::\n\tmsgbox %s_Text_DisplayBowls, MSGBOX_SIGN\n\tend\n\n'
                  + name + '_EventScript_DisplayCase::\n\tlockall\n'
                  '\tgoto_if_set %s, %s_EventScript_DisplayEmpty\n'
                  '\tmsgbox %s_Text_DisplayBell, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
                  '%s_EventScript_DisplayEmpty::\n\tmsgbox %s_Text_DisplayEmpty, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
                  ) % (name, DENEB_DONE, name, name, name, name)
    m.scripts += T(name + '_Text_DisplayBowls', 'Tea bowls glazed the color of the\nnight sky.\fA tag on them says: NOT FOR SALE.')
    m.scripts += T(name + '_Text_DisplayBell', 'A small bell of clear, glittering\nglass. It hums when you lean close.')
    m.scripts += T(name + '_Text_DisplayEmpty', 'The case is empty. A ring in the\ndust shows where a bell once sat.')
    # the scene: steps on the row in front of the door
    m.map_scripts = [('MAP_SCRIPT_ON_TRANSITION', name + '_OnTransition')]
    m.scripts += (name + '_OnTransition::\n\tcall_if_set %s, %s_EventScript_SceneDone\n\tend\n\n'
                  '%s_EventScript_SceneDone::\n\tsetvar VAR_TEMP_1, 1\n\treturn\n\n' % (DENEB_DONE, name, name))
    for x in range(1, 12):
        m.coords.append(g.coord(x, 8, name + '_EventScript_DenebScene'))
    m.scripts += (name + '_EventScript_DenebScene::\n'
                  '\tlockall\n'
                  '\ttextcolor NPC_TEXT_COLOR_FEMALE\n'
                  '\tmsgbox %(n)s_Text_DenebBuys, MSGBOX_DEFAULT\n'
                  '\ttextcolor NPC_TEXT_COLOR_MALE\n'
                  '\tmsgbox %(n)s_Text_MasterWorried, MSGBOX_DEFAULT\n'
                  '\tclosemessage\n'
                  '\tplaybgm MUS_ENCOUNTER_SUSPICIOUS, FALSE\n'
                  '\tapplymovement %(d)s, Common_Movement_FaceDown\n'
                  '\twaitmovement 0\n'
                  '\tplayse SE_PIN\n'
                  '\tapplymovement %(d)s, Common_Movement_ExclamationMark\n'
                  '\twaitmovement 0\n'
                  '\ttextcolor NPC_TEXT_COLOR_FEMALE\n'
                  '\tmsgbox %(n)s_Text_DenebIntro, MSGBOX_DEFAULT\n'
                  '\ttrainerbattle_no_intro TRAINER_SEIKA_NOVA_DENEB_KILN, %(n)s_Text_DenebDefeat\n'
                  '\tmsgbox %(n)s_Text_DenebAfter, MSGBOX_DEFAULT\n'
                  '\tclosemessage\n'
                  '\tfadescreen FADE_TO_BLACK\n'
                  '\tremoveobject %(d)s\n'
                  '\tsetflag %(hide)s\n'
                  '\tsetflag %(done)s\n'
                  '\tsetvar VAR_TEMP_1, 1\n'
                  '\tfadescreen FADE_FROM_BLACK\n'
                  '\tfadedefaultbgm\n'
                  '\ttextcolor NPC_TEXT_COLOR_MALE\n'
                  '\tmsgbox %(n)s_Text_MasterAfter, MSGBOX_DEFAULT\n'
                  '\treleaseall\n\tend\n\n'
                  % {'n': name, 'd': deneb, 'hide': HIDE_DENEB, 'done': DENEB_DONE})
    m.scripts += (name + '_EventScript_Deneb::\n\tlock\n\tfaceplayer\n'
                  '\tmsgbox %s_Text_DenebBuys, MSGBOX_DEFAULT\n\trelease\n\tend\n\n' % name)
    m.scripts += (name + '_EventScript_Master::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_set %s, %s_EventScript_MasterLater\n'
                  '\tmsgbox %s_Text_MasterWorried, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%s_EventScript_MasterLater::\n\tmsgbox %s_Text_MasterLater, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % (DENEB_DONE, name, name, name, name))
    m.scripts += T(name + '_Text_DenebBuys',
                   'DENEB: Then it is settled.\fThe STARGLASS BELL belongs to\n'
                   'TEAM NOVA now. The gold is on\n'
                   'your table, master.')
    m.scripts += T(name + '_Text_MasterWorried',
                   'MASTER: But that bell was never\n'
                   'meant to be sold...\fOur first masters fired it to\n'
                   'answer the star when it fell.')
    m.scripts += T(name + '_Text_DenebIntro',
                   'Oh? {PLAYER}. Of course it is you.\fYou followed us from KANTO all\n'
                   'the way to SEIKA. How loyal.\fThe old songs say this bell can\n'
                   'wake the WISHING STAR.\fWe paid a fair price for it.\n'
                   'But if you insist on a quarrel...')
    m.scripts += T(name + '_Text_DenebDefeat',
                   'Hmph. You fight as if the stars\n'
                   'themselves were on your side.')
    m.scripts += T(name + '_Text_DenebAfter',
                   'It changes nothing, {PLAYER}.\fThe bell is already on its way\n'
                   'to the mountain.\fVEGA will have her wish.\n'
                   'Good day.')
    m.scripts += T(name + '_Text_MasterAfter',
                   'MASTER: She left with her people.\n'
                   'Gold on my table, and the bell\n'
                   'gone from its case.\fIf they ring it on MT. SKYREACH,\n'
                   'the star will answer them...\fPlease, young one. Do what you\n'
                   'can. Win the BADGES and follow\n'
                   'her to the mountain.')
    m.scripts += T(name + '_Text_MasterLater',
                   'MASTER: The STARGLASS BELL is\n'
                   'gone. Our oldest treasure...\fGo on, young one. The road to\n'
                   'MT. SKYREACH is long.')
    trainer('TRAINER_SEIKA_NOVA_DENEB_KILN', 'DENEB', 'Nova Admin', 'Hex Maniac', 'Female', '''
Bronzong @ Leftovers
Level: 56
%(iv)s
- Gyro Ball
- Earthquake
- Psychic
- Stealth Rock

Mismagius
Level: 56
%(iv)s
- Shadow Ball
- Dazzling Gleam
- Thunderbolt
- Nasty Plot

Togekiss
Level: 57
%(iv)s
- Air Slash
- Dazzling Gleam
- Aura Sphere
- Roost

Gardevoir @ Sitrus Berry
Level: 58
%(iv)s
- Psychic
- Moonblast
- Focus Blast
- Calm Mind
''' % {'iv': FULL_IVS}, music='Suspicious')
    return m


def kiln_gym(t, w_gym):
    r = kiln_gym_room()
    W, H = r.w, r.h
    mx = W // 2
    name = P_KILN + '_Gym'
    m = add(g.MapDef(name, 'MAP_SEIKA_KILN_CITY_GYM', SEC_KILN, r, 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (mx - 1, mx, mx + 1):
        door(m, k, H - 2, t.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_EXPERT_M', mx, 2, name + '_EventScript_Tobin'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', mx - 2, H - 4, name + '_EventScript_GymGuy'))
    gym_statue(m, name, ((mx - 3, H - 3), (mx + 3, H - 3)), BADGE4,
               'KILN CITY POKéMON GYM\nLEADER: TOBIN')
    trainer('TRAINER_SEIKA_RUIN_MANIAC_TETSU', 'TETSU', 'Ruin Maniac Frlg', 'Ruin Maniac Frlg', 'Male', '''
Sandslash
Level: 55
- Earthquake
- Knock Off
- Rapid Spin
- Swords Dance

Piloswine
Level: 55
- Earthquake
- Icicle Crash
- Ice Shard
- Stone Edge

Rhydon
Level: 56
- Earthquake
- Rock Slide
- Megahorn
- Hammer Arm
''')
    trainer('TRAINER_SEIKA_POKEMANIAC_KAZU', 'KAZU', 'Pokemaniac Frlg', 'Pokemaniac Frlg', 'Male', '''
Marowak @ Thick Club
Level: 56
- Bonemerang
- Rock Slide
- Fire Punch
- Swords Dance

Nidoking
Level: 56
- Earth Power
- Sludge Wave
- Ice Beam
- Thunderbolt
''')
    trainer('TRAINER_SEIKA_CAMPER_DAIKI', 'DAIKI', 'Camper Frlg', 'Camper Frlg', 'Male', '''
Gligar
Level: 56
- Earthquake
- Acrobatics
- Knock Off
- Roost

Quagsire
Level: 56
- Earthquake
- Waterfall
- Recover
- Toxic

Steelix
Level: 57
- Iron Tail
- Earthquake
- Crunch
- Rock Slide
''')
    trainer('TRAINER_SEIKA_LEADER_TOBIN', 'TOBIN', 'Leader Frlg', 'Expert M', 'Male', '''
Claydol
Level: 58
%(iv)s
- Earth Power
- Psychic
- Ice Beam
- Stealth Rock

Gliscor @ Toxic Orb
Level: 58
%(iv)s
- Earthquake
- Knock Off
- Swords Dance
- Roost

Quagsire
Level: 58
%(iv)s
- Earthquake
- Waterfall
- Recover
- Yawn

Donphan
Level: 58
%(iv)s
- Earthquake
- Ice Shard
- Knock Off
- Rapid Spin

Golurk @ Life Orb
Level: 59
%(iv)s
- Earthquake
- Shadow Punch
- Ice Punch
- Rock Polish
''' % {'iv': FULL_IVS}, music='Male', ai='Smart Trainer')
    for gfx, x, y, label, move in (('OBJ_EVENT_GFX_CAMPER_FRLG', 0, 13, 'Daiki', 'MOVEMENT_TYPE_FACE_RIGHT'),
                                   ('OBJ_EVENT_GFX_POKE_MANIAC_FRLG', W - 1, 9, 'Kazu', 'MOVEMENT_TYPE_FACE_LEFT'),
                                   ('OBJ_EVENT_GFX_HIKER_FRLG', 0, 5, 'Tetsu', 'MOVEMENT_TYPE_FACE_RIGHT')):
        m.objects.append(g.obj(gfx, x, y, name + '_EventScript_' + label, move=move,
                               trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.scripts += battle(name + '_EventScript_Daiki', 'TRAINER_SEIKA_CAMPER_DAIKI',
                        'TOBIN taught me to dig my own\nclay. I dig my own wins, too!',
                        'I dug myself into a hole...',
                        'The walls in here are built like\nkilns. TOBIN says it keeps us warm.', name)
    m.scripts += battle(name + '_EventScript_Kazu', 'TRAINER_SEIKA_POKEMANIAC_KAZU',
                        'Bones and clay! Clay and bones!\nMy POKéMON love the ground!',
                        'My bones are rattled!',
                        'TOBIN\'s GOLURK was shaped by his\nown hands. It\'s a real clay giant!', name)
    m.scripts += battle(name + '_EventScript_Tetsu', 'TRAINER_SEIKA_RUIN_MANIAC_TETSU',
                        'I found old pots under the city.\nAnd a few old moves, too!',
                        'Shattered like a pot!',
                        'Go on. The master is waiting at\nthe back of the kiln.', name)
    m.scripts += leader_script(
        name, 'Tobin', 'TRAINER_SEIKA_LEADER_TOBIN', BADGE4, 'ITEM_TM_EARTHQUAKE',
        'So you have walked through my\n'
        'kilns. I am TOBIN, the potter.\fClay remembers every hand that\n'
        'shaped it, and every fire that\n'
        'hardened it.\fLet us see what kind of fire\n'
        'shaped you, trainer!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}',
        'Hah! My clay has cracked, and\n'
        'that is no shame.\fYou have earned the KILN BADGE.\n'
        'Wear it with care.\f'
        '{FONT_NORMAL}{PLAYER} received the KILN BADGE\n'
        'from TOBIN!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}',
        'And take this TM. It is the\n'
        'shaking of the earth itself.',
        'Go east along TIDE ROAD to\n'
        'TIDEWATCH TOWN.\fNAGI keeps the TIDE SHRINE there.\n'
        'She reads the sea the way I\n'
        'read clay.')
    m.scripts += gym_guy(name, BADGE4,
                         'Yo! Welcome to the KILN GYM!\fTOBIN uses GROUND types. WATER,\n'
                         'GRASS and ICE moves soften his\n'
                         'clay.\fHis ace is GOLURK, a clay giant.\n'
                         'Its GHOST type dodges NORMAL\n'
                         'and FIGHTING moves!',
                         'You cracked the KILN GYM!\nThat\'s four BADGES now!')
    return m


# ---------------------------------------------------------------- Tide Road

def tide_road_canvas():
    c = g.Canvas(72, 26, 'gTileset_VermilionCity', seed=47)
    c.forest(0, 0, 72, 6)
    c.forest(0, 6, 6, 4)                  # west end: opening to Kiln City
    c.forest(0, 16, 10, 10)
    c.forest(66, 6, 6, 4)                 # east end: opening to Tidewatch Town
    c.forest(66, 16, 6, 4)
    # the coast
    c.rect(10, 15, 56, 5, 'beach')
    c.rect(62, 20, 10, 2, 'beach')
    c.rect(10, 20, 52, 2, 'sea')
    c.rect(10, 22, 62, 4, 'sea')
    c.rect(20, 18, 7, 2, 'sea')           # bays
    c.rect(43, 17, 9, 3, 'sea')
    g.pier(c, 32, 16, 2, 6)
    # the road
    c.rect(0, 12, 24, 2, 'path')
    c.rect(22, 8, 2, 6, 'path')
    c.rect(22, 8, 20, 2, 'path')
    c.rect(40, 8, 2, 6, 'path')
    c.rect(40, 12, 32, 2, 'path')
    # grass and trees
    c.rect(8, 6, 10, 4, 'tall')
    c.rect(24, 10, 16, 3, 'tall')
    c.rect(44, 6, 12, 5, 'tall')
    c.rect(58, 14, 6, 1, 'flowers')
    c.ledge(6, 10, 14)
    for x, y in ((18, 6), (26, 6), (36, 6), (58, 6), (62, 8)):
        c.tree(x, y)
    c.put(5, 14, g.SIGN)
    c.put(64, 14, g.SIGN)
    return c


def tide_road(city):
    c = tide_road_canvas()
    m = add(g.MapDef(P_TROAD, 'MAP_SEIKA_TIDE_ROAD', SEC_TROAD, c, 'MUS_RG_ROUTE11', 'MAP_TYPE_ROUTE',
                     border=g.SEA_BORDER))
    link('MAP_SEIKA_KILN_CITY', 'right', 8, 'MAP_SEIKA_TIDE_ROAD')    # city y20-21 = road y12-13
    signpost(m, 5, 14, P_TROAD + '_EventScript_SignWest', 'TIDE ROAD\nKILN CITY - TIDEWATCH TOWN')
    signpost(m, 64, 14, P_TROAD + '_EventScript_SignEast', 'TIDE ROAD\nTIDEWATCH TOWN - KILN CITY')
    item_ball(m, 6, 8, P_TROAD + '_EventScript_ItemBigPearl', 'ITEM_BIG_PEARL', ITEM_TROAD)
    npc(m, 'OBJ_EVENT_GFX_WOMAN_3_FRLG', 28, 16, P_TROAD + '_EventScript_Woman',
        'The tide comes in twice a day.\n'
        'The people of TIDEWATCH can tell\n'
        'the hour by it.\fThey say their shrine keeper can\n'
        'feel it in her bones!', 'MOVEMENT_TYPE_WANDER_AROUND')
    trainer('TRAINER_SEIKA_FISHERMAN_SHIGE', 'SHIGE', 'Fisherman Frlg', 'Fisherman Frlg', 'Male', '''
Octillery
Level: 50
- Octazooka
- Ice Beam
- Fire Blast
- Gunk Shot

Qwilfish
Level: 50
- Waterfall
- Poison Jab
- Spikes
- Aqua Jet

Lanturn
Level: 51
- Surf
- Thunderbolt
- Ice Beam
- Confuse Ray
''')
    trainer('TRAINER_SEIKA_SAILOR_DAN', 'DAN', 'Sailor Frlg', 'Sailor Frlg', 'Male', '''
Poliwrath
Level: 51
- Waterfall
- Close Combat
- Ice Punch
- Bulk Up

Machamp
Level: 51
- Cross Chop
- Knock Off
- Stone Edge
- Bullet Punch

Pelipper
Level: 52
- Hurricane
- Surf
- Roost
- U-turn
''')
    trainer('TRAINER_SEIKA_SWIMMER_F_RIN', 'RIN', 'Swimmer F Frlg', 'Swimmer F Frlg', 'Female', '''
Mantine
Level: 52
- Air Slash
- Surf
- Ice Beam
- Roost

Corsola
Level: 52
- Power Gem
- Surf
- Recover
- Stealth Rock

Starmie
Level: 52
- Surf
- Psychic
- Thunderbolt
- Recover
''', music='Female')
    trainer('TRAINER_SEIKA_SWIMMER_M_JUN', 'JUN', 'Swimmer M Frlg', 'Swimmer M Frlg', 'Male', '''
Golduck
Level: 52
- Surf
- Psychic
- Ice Beam
- Calm Mind

Quagsire
Level: 52
- Earthquake
- Waterfall
- Recover
- Yawn

Kingdra
Level: 53
- Surf
- Dragon Pulse
- Ice Beam
- Rain Dance
''')
    m.objects.append(g.obj('OBJ_EVENT_GFX_FISHER', 32, 21, P_TROAD + '_EventScript_Shige',
                           move='MOVEMENT_TYPE_FACE_DOWN', trainer='TRAINER_TYPE_NORMAL', sight=3))
    m.objects.append(g.obj('OBJ_EVENT_GFX_SAILOR_FRLG', 46, 11, P_TROAD + '_EventScript_Dan',
                           move='MOVEMENT_TYPE_FACE_DOWN', trainer='TRAINER_TYPE_NORMAL', sight=2))
    m.objects.append(g.obj('OBJ_EVENT_GFX_SWIMMER_F_WATER', 22, 23, P_TROAD + '_EventScript_Rin',
                           move='MOVEMENT_TYPE_FACE_LEFT_AND_RIGHT', trainer='TRAINER_TYPE_NORMAL', sight=4,
                           elevation=1))
    m.objects.append(g.obj('OBJ_EVENT_GFX_SWIMMER_M_WATER', 57, 22, P_TROAD + '_EventScript_Jun',
                           move='MOVEMENT_TYPE_FACE_LEFT', trainer='TRAINER_TYPE_NORMAL', sight=4, elevation=1))
    m.scripts += battle(P_TROAD + '_EventScript_Shige', 'TRAINER_SEIKA_FISHERMAN_SHIGE',
                        'Shh! You\'ll scare the REMORAID!\nOh, fine. Let\'s battle!',
                        'They all got away...',
                        'OCTILLERY love the rocks under\nthis pier. I fish here every day.', P_TROAD)
    m.scripts += battle(P_TROAD + '_EventScript_Dan', 'TRAINER_SEIKA_SAILOR_DAN',
                        'Ahoy! I just came ashore and my\nlegs want a fight!',
                        'Sunk, by a landlubber!',
                        'The sea round TIDEWATCH is calm.\nNAGI the diver keeps it that way.', P_TROAD)
    m.scripts += battle(P_TROAD + '_EventScript_Rin', 'TRAINER_SEIKA_SWIMMER_F_RIN',
                        'The water is warm today! Let\'s\nmake some waves!',
                        'I swallowed some seawater...',
                        'I swim to TIDEWATCH and back\nevery morning. It\'s good for you!', P_TROAD)
    m.scripts += battle(P_TROAD + '_EventScript_Jun', 'TRAINER_SEIKA_SWIMMER_M_JUN',
                        'Nobody swims faster than me in\nthe rain!',
                        'The tide turned on me!',
                        'My KINGDRA was a HORSEA from\nthis very beach.', P_TROAD)
    wild('MAP_SEIKA_TIDE_ROAD', 'gSeika_TideRoad',
         land=[('WINGULL', 50, 51), ('SLOWPOKE', 50, 51), ('PELIPPER', 51, 52), ('AZUMARILL', 51, 52),
               ('QUAGSIRE', 51, 52), ('FURRET', 51, 52), ('SLOWBRO', 52, 53), ('SLOWKING', 52, 53),
               ('CORSOLA', 51, 53), ('XATU', 52, 53), ('DELIBIRD', 52, 53), ('SKARMORY', 53, 53)],
         water=[('TENTACRUEL', 50, 52), ('MANTINE', 50, 52), ('LANTURN', 51, 53), ('PELIPPER', 51, 53),
                ('LAPRAS', 52, 53)],
         fishing=[('MAGIKARP', 50, 51), ('REMORAID', 50, 51), ('REMORAID', 50, 52), ('CHINCHOU', 50, 52),
                  ('QWILFISH', 50, 52), ('OCTILLERY', 51, 53), ('LANTURN', 51, 53), ('CORSOLA', 51, 53),
                  ('SEADRA', 52, 53), ('KINGDRA', 53, 53)])
    return m


# ---------------------------------------------------------------- Tidewatch Town

TIDE_HOUSE = (CER, 21, 25, 6, 4)           # door (2,3)
TIDE_COTTAGE = (CER, 31, 25, 5, 4)         # no door
SHRINE = ('LAYOUT_ROUTE5', 20, 21, 8, 6)   # Route 5's Day Care and its yard, door (3,4)


def tidewatch_canvas():
    c = g.Canvas(40, 34, 'gTileset_CeruleanCity', seed=53)
    c.forest(0, 0, 18, 6)
    c.forest(24, 0, 16, 6)                # north: opening to Hollow Path
    c.forest(0, 6, 8, 8)
    c.forest(0, 20, 8, 4)                 # west: opening to Tide Road
    c.rect(0, 24, 32, 2, 'beach')
    c.rect(30, 6, 2, 20, 'beach')
    c.rect(0, 26, 40, 8, 'sea')
    c.rect(32, 6, 8, 20, 'sea')
    # streets
    c.rect(0, 16, 32, 2, 'beach')
    c.rect(20, 0, 2, 16, 'beach')
    # buildings
    for st, x, y in ((PC, 9, 11), (GYM, 23, 11), (MART, 9, 19)):
        c.stamp(*st[:5], x, y)            # doors: PC (11,15), Gym (26,15), Mart (11,22)
    c.stamp(*TIDE_HOUSE, 14, 12)          # house 1, door (16,15)
    c.stamp(*SHRINE, 14, 18, ground=True)  # tide shrine, door (17,22)
    c.stamp(*TIDE_HOUSE, 23, 19)          # house 2, door (25,22)
    c.stamp(*GYM_SIGN, 22, 14)
    # the shore
    g.pier(c, 12, 24, 2, 6)
    g.pier(c, 26, 24, 2, 5)
    c.rect(9, 7, 4, 3, 'flowers')
    c.rect(24, 7, 4, 2, 'flowers')
    c.tree(14, 7)
    c.tree(16, 7)
    c.tree(28, 8)
    c.put(19, 10, g.SIGN)
    c.put(13, 21, g.SIGN)
    return c


# Cerulean Gym's pool: walkway and water blocks by their neighbours.
POOL_WALK = ({0x329C, 0x32B9, 0x32BA, 0x32C1, 0x32C4, 0x32C5} | set(range(0x32A1, 0x32AE))
             | set(range(0x32B1, 0x32B6)))
POOL_STATUE = {0x32D0, 0x32D1, 0x06D2}
POOL_NEIGH = [(0, -1), (1, 0), (0, 1), (-1, 0), (-1, -1), (1, -1), (-1, 1), (1, 1)]


def _pool_kind(b):
    if b in POOL_WALK:
        return '='
    if b in POOL_STATUE:
        return 'S'
    if (b >> 12) == 1 or b == 0x0293:
        return '~'
    return '#' if (b >> 10) & 3 else '.'


def tide_gym_room(walks):
    """Gym 5 on Cerulean Gym's tiles: the room, ladders, entrance pier and
    leader's platform as in Kanto, the walkways in the pool redrawn.
    walks: set of (x, y) pool cells that are walkway."""
    import collections
    src = 'LAYOUT_CERULEAN_CITY_GYM'
    W, H = 17, 20
    r = g.Room(W, H, 'gTileset_BuildingFrlg', 'gTileset_CeruleanGym', source=src)
    r.copy(0, 0, W, H, 0, 0)
    S = r.src

    def kind_src(x, y):
        return _pool_kind(S[y][x]) if 0 <= x < W and 0 <= y < H else '#'
    table = collections.defaultdict(collections.Counter)
    for y in range(H):
        for x in range(W):
            k = kind_src(x, y)
            if k in '=~' and S[y][x] not in (0x12A8, 0x12C6):    # not the ladders
                table[(k, ''.join(kind_src(x + dx, y + dy) for dx, dy in POOL_NEIGH))][S[y][x]] += 1
    pool = {(x, y) for y in range(4, 18) for x in range(2, 15) if kind_src(x, y) in '=~'}
    keep = {(3, 5), (13, 5), (3, 17), (13, 17)}           # ladders
    keep |= {(x, y) for x in (7, 8, 9) for y in (16, 17)}  # the entrance pier

    def kind_new(x, y):
        if (x, y) in pool:
            return '=' if (x, y) in walks or ((x, y) in keep and kind_src(x, y) == '=') else '~'
        return kind_src(x, y)
    w8 = [4, 4, 4, 4, 1, 1, 1, 1]
    for (x, y) in sorted(pool - keep):
        k = kind_new(x, y)
        key = ''.join(kind_new(x + dx, y + dy) for dx, dy in POOL_NEIGH)
        if (k, key) in table:
            r.set(x, y, table[(k, key)].most_common(1)[0][0])
            continue
        best = min((kk for kk in table if kk[0] == k),
                   key=lambda kk: sum(w for w, a, b in zip(w8, kk[1], key) if a != b))
        r.set(x, y, table[best].most_common(1)[0][0])
    return r


TIDE_MART = ['ITEM_ULTRA_BALL', 'ITEM_GREAT_BALL', 'ITEM_NET_BALL', 'ITEM_DIVE_BALL', 'ITEM_LURE_BALL',
             'ITEM_HYPER_POTION', 'ITEM_MAX_POTION', 'ITEM_FULL_RESTORE', 'ITEM_REVIVE', 'ITEM_FULL_HEAL',
             'ITEM_MAX_REPEL', 'ITEM_WATER_STONE']


def tidewatch(troad):
    c = tidewatch_canvas()
    t = add(g.MapDef(P_TIDE, 'MAP_SEIKA_TIDEWATCH', SEC_TIDE, c, 'MUS_RG_SEVII_45', 'MAP_TYPE_TOWN',
                     border=g.SEA_BORDER))
    link('MAP_SEIKA_TIDE_ROAD', 'right', -4, 'MAP_SEIKA_TIDEWATCH')   # road y12-13 = town y16-17
    w_pc = door(t, 11, 15, 'MAP_SEIKA_TIDEWATCH_POKEMON_CENTER_1F', 0)
    w_gym = door(t, 26, 15, 'MAP_SEIKA_TIDEWATCH_GYM', 0)
    w_mart = door(t, 11, 22, 'MAP_SEIKA_TIDEWATCH_MART', 0)
    w_h1 = door(t, 16, 15, 'MAP_SEIKA_TIDEWATCH_HOUSE1', 0)
    w_shrine = door(t, 17, 22, 'MAP_SEIKA_TIDEWATCH_TIDE_SHRINE', 0)
    w_h2 = door(t, 25, 22, 'MAP_SEIKA_TIDEWATCH_HOUSE2', 0)
    signpost(t, 19, 10, P_TIDE + '_EventScript_TownSign',
             'TIDEWATCH TOWN\n'
             'The sea keeps time here.')
    signpost(t, 22, 15, P_TIDE + '_EventScript_GymSign',
             'TIDEWATCH POKéMON GYM\n'
             'LEADER: NAGI\fThe diver who swims with\n'
             'the tide!')
    signpost(t, 13, 21, P_TIDE + '_EventScript_ShrineSign',
             'TIDE SHRINE\n'
             'The sea rises for the star.')
    npc(t, 'OBJ_EVENT_GFX_FISHER', 12, 29, P_TIDE + '_EventScript_FisherWest',
        'Everyone in TIDEWATCH fishes.\n'
        'Even the GYM LEADER!\fWell, NAGI dives for her catch.\n'
        'Same thing, really.', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_FISHER', 27, 28, P_TIDE + '_EventScript_FisherEast',
        'At high tide the REMORAID come\n'
        'right up to the pier.\fAt low tide, the OCTILLERY come\n'
        'after the REMORAID!', 'MOVEMENT_TYPE_FACE_DOWN')
    npc(t, 'OBJ_EVENT_GFX_OLD_MAN_1', 31, 12, P_TIDE + '_EventScript_OldSailor',
        'The tide here never misses a\n'
        'beat. Twice a day, for a thousand\n'
        'years.\fExcept one night, they say. The\n'
        'night the first star fell, the\n'
        'sea rose all the way to the hills.', 'MOVEMENT_TYPE_FACE_RIGHT')
    npc(t, 'OBJ_EVENT_GFX_LITTLE_GIRL_FRLG', 24, 18, P_TIDE + '_EventScript_Girl',
        'NAGI can hold her breath for ten\n'
        'whole minutes! I can do ten\n'
        'seconds.', 'MOVEMENT_TYPE_WANDER_AROUND')
    npc(t, 'OBJ_EVENT_GFX_WOMAN_2_FRLG', 21, 6, P_TIDE + '_EventScript_NorthWoman',
        'The path north goes through the\n'
        'hills to SILKHOLLOW TOWN.\fThe weavers there make the finest\n'
        'silk in all of SEIKA.', 'MOVEMENT_TYPE_FACE_DOWN')

    pokecenter('Tidewatch', SEC_TIDE, t, w_pc, 'HEAL_LOCATION_SEIKA_TIDEWATCH', npcs=[
        ('OBJ_EVENT_GFX_SAILOR_FRLG', 11, 5,
         'My ship brings fish from here to\n'
         'KILN CITY, and pots back again.\fThe KILN CITY folk do love their\n'
         'fish on a nice plate!'),
    ])
    mart('Tidewatch', SEC_TIDE, t, w_mart, TIDE_MART, npcs=[
        ('OBJ_EVENT_GFX_FISHER', 6, 4,
         'A LURE BALL works best on POKéMON\n'
         'you hook with a ROD.'),
    ])
    house('Tidewatch', 1, SEC_TIDE, t, w_h1, 'MUS_RG_SEVII_45', [
        ('OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 2, 3,
         'My husband mends nets, I sell the\n'
         'fish. Fifty years now!\fThe sea gives, and the sea takes.\n'
         'Mostly it gives.'),
        ('OBJ_EVENT_GFX_FAT_MAN_FRLG', 7, 4,
         'A fine woman in a coat came\n'
         'through on a fast boat.\fShe asked about the old road to\n'
         'MT. SKYREACH. I told her nothing!', 'MOVEMENT_TYPE_FACE_LEFT'),
    ], layout='LAYOUT_HOUSE2_FRLG')
    house('Tidewatch', 2, SEC_TIDE, t, w_h2, 'MUS_RG_SEVII_45', [
        ('OBJ_EVENT_GFX_SWIMMER_M_LAND', 5, 3,
         'I train in NAGI\'s pool every day.\fShe swims rings around me. Every\n'
         'single time!'),
    ], layout='LAYOUT_HOUSE1_FRLG')
    shrine = add(g.MapDef(P_TIDE + '_TideShrine', 'MAP_SEIKA_TIDEWATCH_TIDE_SHRINE', SEC_TIDE,
                          'LAYOUT_FIVE_ISLAND_RESORT_GORGEOUS_HOUSE', 'MUS_RG_SEVII_45', 'MAP_TYPE_INDOOR',
                          'WEATHER_NONE', False, False, False))
    door(shrine, 4, 7, t.id, w_shrine)
    npc(shrine, 'OBJ_EVENT_GFX_OLD_WOMAN_FRLG', 4, 3, P_TIDE + '_TideShrine_EventScript_Keeper',
        'Welcome to the TIDE SHRINE.\fA thousand years ago, the first\n'
        'star fell into the sky over the\n'
        'sea, and the tide rose to meet it.\fWe have watched the tide ever\n'
        'since. When it rises too high,\n'
        'the star is stirring.\fThis year the sea has been very\n'
        'high indeed...')
    npc(shrine, 'OBJ_EVENT_GFX_WOMAN_1_FRLG', 7, 4, P_TIDE + '_TideShrine_EventScript_Attendant',
        'NAGI is our shrine keeper. She\n'
        'dives to the bottom of the bay to\n'
        'read the old stones.\fShe says the stones point north.\n'
        'Toward the mountain.', 'MOVEMENT_TYPE_FACE_LEFT')
    for x in (8, 9):
        shrine.bgs.append(g.sign(x, 1, P_TIDE + '_TideShrine_EventScript_TideChart', 'BG_EVENT_PLAYER_FACING_NORTH'))
    shrine.scripts += (P_TIDE + '_TideShrine_EventScript_TideChart::\n\tmsgbox %s_TideShrine_Text_TideChart, MSGBOX_SIGN\n\tend\n\n'
                       % P_TIDE + T(P_TIDE + '_TideShrine_Text_TideChart',
                                    'Old tide charts, a thousand years\n'
                                    'of them.\fOne page is marked with a star. On\n'
                                    'it, the tide line runs off the top\n'
                                    'of the page.'))
    tide_gym(t, w_gym)
    wild('MAP_SEIKA_TIDEWATCH', 'gSeika_Tidewatch',
         water=[('TENTACRUEL', 52, 54), ('MANTINE', 52, 54), ('PELIPPER', 52, 54), ('LANTURN', 53, 55),
                ('LAPRAS', 54, 55)],
         fishing=[('MAGIKARP', 50, 52), ('REMORAID', 50, 52), ('REMORAID', 52, 54), ('CHINCHOU', 52, 54),
                  ('QWILFISH', 52, 54), ('OCTILLERY', 53, 55), ('LANTURN', 53, 55), ('CORSOLA', 53, 55),
                  ('SEADRA', 54, 55), ('KINGDRA', 55, 55)])
    return t


def tide_gym(t, w_gym):
    walks = {(x, y) for x in (7, 8, 9) for y in (6, 7, 8)}     # the leader's platform
    walks |= {(x, 7) for x in range(4, 13)}                     # its arms
    walks |= {(x, y) for x in (4, 12) for y in range(7, 13)}    # down both sides
    walks |= {(x, 12) for x in range(4, 13)}                    # joined at the bottom
    walks |= {(9, y) for y in range(12, 16)}                    # and up from the entrance
    r = tide_gym_room(walks)
    W, H = r.w, r.h
    name = P_TIDE + '_Gym'
    m = add(g.MapDef(name, 'MAP_SEIKA_TIDEWATCH_GYM', SEC_TIDE, r, 'MUS_RG_GYM', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    for k in (7, 8, 9):
        door(m, k, 18, t.id, w_gym)
    m.objects.append(g.obj('OBJ_EVENT_GFX_SWIMMER_F_LAND', 8, 6, name + '_EventScript_Nagi'))
    m.objects.append(g.obj('OBJ_EVENT_GFX_GYM_GUY', 7, 16, name + '_EventScript_GymGuy'))
    gym_statue(m, name, ((6, 17), (10, 17)), BADGE5, 'TIDEWATCH POKéMON GYM\nLEADER: NAGI')
    swimmers = (('OBJ_EVENT_GFX_SWIMMER_F_WATER', 3, 10, 'Mina', 'MOVEMENT_TYPE_FACE_RIGHT'),
                ('OBJ_EVENT_GFX_SWIMMER_M_WATER', 13, 10, 'Kaito', 'MOVEMENT_TYPE_FACE_LEFT'),
                ('OBJ_EVENT_GFX_SWIMMER_M_WATER', 8, 14, 'Sora', 'MOVEMENT_TYPE_FACE_RIGHT'))
    for gfx, x, y, label, move in swimmers:
        r.set(x, y, r.grid[y][x] & 0x0FFF)      # elevation 0 under a swimmer, as in Kanto
        m.objects.append(g.obj(gfx, x, y, name + '_EventScript_' + label, move=move,
                               trainer='TRAINER_TYPE_NORMAL', sight=1, elevation=0))
    trainer('TRAINER_SEIKA_SWIMMER_F_MINA', 'MINA', 'Swimmer F Frlg', 'Swimmer F Frlg', 'Female', '''
Azumarill
Level: 57
- Aqua Jet
- Play Rough
- Liquidation
- Superpower

Lanturn
Level: 57
- Scald
- Thunderbolt
- Ice Beam
- Volt Switch
''', music='Female')
    trainer('TRAINER_SEIKA_SWIMMER_M_KAITO', 'KAITO', 'Swimmer M Frlg', 'Swimmer M Frlg', 'Male', '''
Octillery
Level: 57
- Octazooka
- Ice Beam
- Fire Blast
- Energy Ball

Mantine
Level: 57
- Scald
- Air Slash
- Ice Beam
- Roost
''')
    trainer('TRAINER_SEIKA_SWIMMER_M_SORA', 'SORA', 'Swimmer M Frlg', 'Swimmer M Frlg', 'Male', '''
Politoed
Level: 57
- Hydro Pump
- Ice Beam
- Hypnosis
- Rain Dance

Overqwil
Level: 58
- Crunch
- Gunk Shot
- Liquidation
- Swords Dance
''')
    trainer('TRAINER_SEIKA_LEADER_NAGI', 'NAGI', 'Leader Frlg', 'Swimmer F', 'Female', '''
Lanturn
Level: 60
%(iv)s
- Scald
- Thunderbolt
- Ice Beam
- Volt Switch

Mantine
Level: 60
%(iv)s
- Hydro Pump
- Air Slash
- Ice Beam
- Roost

Octillery
Level: 60
%(iv)s
- Octazooka
- Ice Beam
- Fire Blast
- Energy Ball

Azumarill
Level: 60
%(iv)s
- Aqua Jet
- Play Rough
- Liquidation
- Superpower

Kingdra @ Mystic Water
Level: 61
%(iv)s
- Rain Dance
- Hydro Pump
- Draco Meteor
- Ice Beam
''' % {'iv': FULL_IVS}, music='Female', ai='Smart Trainer')
    m.scripts += battle(name + '_EventScript_Mina', 'TRAINER_SEIKA_SWIMMER_F_MINA',
                        'Splash! You walked right past my\nlane. Time to swim for it!',
                        'I sank like a stone...',
                        'NAGI dives deeper than anyone in\nSEIKA. She says it\'s very quiet.', name)
    m.scripts += battle(name + '_EventScript_Kaito', 'TRAINER_SEIKA_SWIMMER_M_KAITO',
                        'The tide is with me today!',
                        'The tide went out...',
                        'NAGI\'s KINGDRA calls the rain.\nThen it gets really fast!', name)
    m.scripts += battle(name + '_EventScript_Sora', 'TRAINER_SEIKA_SWIMMER_M_SORA',
                        'No one gets to NAGI without\ngetting wet first!',
                        'Soaked through!',
                        'Electric and Grass moves work\nwell here. Just saying.', name)
    m.scripts += leader_script(
        name, 'Nagi', 'TRAINER_SEIKA_LEADER_NAGI', BADGE5, 'ITEM_TM_RAIN_DANCE',
        'Hello there. I am NAGI. I keep the\n'
        'TIDE SHRINE, and I dive.\fDown deep, the sea is very calm.\n'
        'You can hear your own heart.\fLet\'s see if yours stays calm\n'
        'when the tide comes in!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}',
        'Whew... You rode the wave all the\n'
        'way in.\fThe TIDE BADGE is yours. The sea\n'
        'will remember you.\f'
        '{FONT_NORMAL}{PLAYER} received the TIDE BADGE\n'
        'from NAGI!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}',
        'Take this TM, too. RAIN DANCE\n'
        'brings the sea down from the sky.',
        'Go north from TIDEWATCH, through\n'
        'HOLLOW PATH, to SILKHOLLOW TOWN.\f'
        'AYAME, the weaver, keeps the\n'
        'shrine there.\fThe stones under the bay all\n'
        'point to MT. SKYREACH. Hurry.')
    m.scripts += gym_guy(name, BADGE5,
                         'Yo! Welcome to the TIDEWATCH GYM!\fNAGI uses WATER types. ELECTRIC\n'
                         'and GRASS moves are your friends.\fCareful: her KINGDRA brings rain\n'
                         'and gets super fast in it!',
                         'You made a splash! Five BADGES\nnow. Way to go!')
    return m
