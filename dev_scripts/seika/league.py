"""Seika League (Starwatch Plateau lobby, Elite Four, Champion) and the
Sky Shrine on top of Mt. Skyreach, where the Wishing Star comes down."""
import common as c
from common import g, T, add, door, npc, mapsec, trainer, flag

SEC_PLATEAU = 'MAPSEC_SEIKA_STARWATCH_PLATEAU'   # defined by duskpeak.py
SEC_SHRINE = mapsec('MAPSEC_SEIKA_SKY_SHRINE', 'SKY SHRINE')

E4_FLAGS = [flag('FLAG_QOL_SEIKA_E4_%d' % i, 'beat Elite Four member %d this run' % i) for i in range(1, 5)]
SHRINE_NOVA = flag('FLAG_QOL_SEIKA_SHRINE_NOVA', 'beat Altair and Deneb at the Sky Shrine')
HIDE_SHRINE_NOVA = flag('FLAG_QOL_SEIKA_HIDE_SHRINE_NOVA', 'Altair and Deneb gone from the Sky Shrine')
HIDE_JIRACHI = flag('FLAG_QOL_SEIKA_HIDE_JIRACHI', 'the Wishing Star is not on the Sky Shrine')
SEIKA_DONE = flag('FLAG_QOL_SEIKA_DONE', 'the Wishing Star met; Seika story finished')

LOBBY = 'MAP_SEIKA_LEAGUE_LOBBY'
PLATEAU = 'MAP_SEIKA_STARWATCH_PLATEAU'
ROOMS = [  # (map id, name, layout, lighting index, leader...)
    dict(id='MAP_SEIKA_LEAGUE_STEEL_ROOM', name='Seika_League_SteelRoom', layout='LAYOUT_POKEMON_LEAGUE_BRUNOS_ROOM',
         light=1, music='MUS_RG_ROCKET_HIDEOUT', who='TETSU', gfx='OBJ_EVENT_GFX_WORKER_M',
         const='TRAINER_SEIKA_E4_TETSU', pic='Hiker Frlg', gender='Male', party='''
Skarmory @ Leftovers
Level: 70
- Brave Bird
- Spikes
- Roost
- Iron Defense

Forretress
Level: 70
- Gyro Ball
- Volt Switch
- Stealth Rock
- Rapid Spin

Steelix @ Steelixite
Level: 71
- Iron Tail
- Earthquake
- Stone Edge
- Dragon Dance

Magnezone
Level: 70
- Flash Cannon
- Thunderbolt
- Volt Switch
- Body Press

Scizor @ Sitrus Berry
Level: 71
- Bullet Punch
- U-turn
- Swords Dance
- Close Combat
''', intro='Welcome to the SEIKA LEAGUE.\fI am TETSU of the ELITE FOUR.\nI forged my POKéMON from the\nmeteor iron of the first STARFALL.\fLet us see if your team bends.',
         defeat='You bent iron. Well done.',
         post='Go on. The next room is darker\nthan this one.'),
    dict(id='MAP_SEIKA_LEAGUE_DARK_ROOM', name='Seika_League_DarkRoom', layout='LAYOUT_POKEMON_LEAGUE_AGATHAS_ROOM',
         light=2, music='MUS_RG_POKE_TOWER', who='KUROE', gfx='OBJ_EVENT_GFX_COOLTRAINER_F',
         const='TRAINER_SEIKA_E4_KUROE', pic='Cooltrainer F Frlg', gender='Female', party='''
Umbreon @ Leftovers
Level: 71
- Foul Play
- Wish
- Protect
- Toxic

Honchkrow
Level: 71
- Brave Bird
- Sucker Punch
- Night Slash
- Heat Wave

Weavile
Level: 71
- Knock Off
- Ice Shard
- Triple Axel
- Swords Dance

Absol @ Absolite
Level: 72
- Knock Off
- Sucker Punch
- Play Rough
- Swords Dance

Houndoom
Level: 72
- Dark Pulse
- Flamethrower
- Sludge Bomb
- Nasty Plot
''', intro='I am KUROE. Stars are only bright\nbecause the night is so dark.\fYour light against my dark.\nShall we?',
         defeat='So bright... I can hardly see.',
         post='Keep that light. The sky is\nwaiting for you.'),
    dict(id='MAP_SEIKA_LEAGUE_SKY_ROOM', name='Seika_League_SkyRoom', layout='LAYOUT_POKEMON_LEAGUE_LORELEIS_ROOM',
         light=0, music='MUS_RG_GYM', who='SORA', gfx='OBJ_EVENT_GFX_BEAUTY_FRLG',
         const='TRAINER_SEIKA_E4_SORA', pic='Beauty Frlg', gender='Female', party='''
Crobat
Level: 72
- Brave Bird
- Cross Poison
- U-turn
- Roost

Togekiss @ Leftovers
Level: 72
- Air Slash
- Dazzling Gleam
- Aura Sphere
- Roost

Xatu
Level: 72
- Psychic
- Air Slash
- Heat Wave
- Roost

Yanmega
Level: 72
- Bug Buzz
- Air Slash
- Giga Drain
- U-turn

Pidgeot @ Pidgeotite
Level: 73
- Hurricane
- Heat Wave
- U-turn
- Roost
''', intro='I am SORA. I have flown higher\nthan MT. SKYREACH.\fI saw the STARFALL from above\nthe clouds. Can you keep up?',
         defeat='You flew right past me!',
         post='The last of us waits beyond\nthat door. Breathe, then go.'),
    dict(id='MAP_SEIKA_LEAGUE_DRAGON_ROOM', name='Seika_League_DragonRoom', layout='LAYOUT_POKEMON_LEAGUE_LANCES_ROOM',
         light=3, music='MUS_RG_VICTORY_ROAD', who='RYOKO', gfx='OBJ_EVENT_GFX_EXPERT_F',
         const='TRAINER_SEIKA_E4_RYOKO', pic='Expert F', gender='Female', party='''
Kingdra @ Leftovers
Level: 73
- Draco Meteor
- Surf
- Ice Beam
- Rain Dance

Altaria @ Altarianite
Level: 73
- Dragon Dance
- Return
- Earthquake
- Roost

Flygon
Level: 73
- Earthquake
- Dragon Claw
- Stone Edge
- U-turn

Dragonite @ Lum Berry
Level: 74
- Dragon Dance
- Extreme Speed
- Earthquake
- Fire Punch

Garchomp @ Sitrus Berry
Level: 74
- Earthquake
- Dragon Claw
- Stone Edge
- Swords Dance
''', intro='I am RYOKO, last of the ELITE\nFOUR. My dragons were here before\nthe first STARFALL.\fThey will not fall to a\ntraveler. Show me otherwise!',
         defeat='The dragons bow to you.',
         post='The CHAMPION is waiting.\nHe has waited a long time.'),
]
CHAMP = dict(id='MAP_SEIKA_LEAGUE_CHAMPION_ROOM', name='Seika_League_ChampionRoom', layout='LAYOUT_POKEMON_LEAGUE_CHAMPIONS_ROOM')

TRAINERS = []
for r in ROOMS:
    trainer(r['const'], r['who'], 'Elite Four Frlg', r['pic'], r['gender'], r['party'],
            music='Male' if r['gender'] == 'Male' else 'Female')
trainer('TRAINER_SEIKA_CHAMPION_SEIJI', 'SEIJI', 'Champion Frlg', 'Expert M', 'Male', '''
Espeon @ Light Clay
Level: 75
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Psychic
- Dazzling Gleam
- Shadow Ball
- Reflect

Lapras @ Leftovers
Level: 75
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Freeze-Dry
- Surf
- Thunderbolt
- Sing

Scizor @ Choice Band
Level: 76
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Bullet Punch
- U-turn
- Knock Off
- Close Combat

Ampharos @ Ampharosite
Level: 76
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Thunderbolt
- Dragon Pulse
- Focus Blast
- Volt Switch

Tyranitar @ Chople Berry
Level: 77
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Stone Edge
- Crunch
- Earthquake
- Dragon Dance

Dragonite @ Lum Berry
Level: 78
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Dragon Dance
- Extreme Speed
- Earthquake
- Fire Punch
''', music='Male', items='Full Restore / Full Restore / Full Restore')
trainer('TRAINER_SEIKA_NOVA_DENEB_SHRINE', 'DENEB', 'Nova Admin', 'Hex Maniac', 'Female', '''
Gardevoir @ Gardevoirite
Level: 72
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Moonblast
- Psychic
- Focus Blast
- Calm Mind

Bronzong @ Leftovers
Level: 72
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Gyro Ball
- Earthquake
- Trick Room
- Stealth Rock

Mismagius
Level: 72
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Shadow Ball
- Dazzling Gleam
- Thunderbolt
- Nasty Plot

Togekiss @ Sitrus Berry
Level: 73
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Air Slash
- Dazzling Gleam
- Aura Sphere
- Roost
''', music='Suspicious')
trainer('TRAINER_SEIKA_NOVA_ALTAIR_SHRINE', 'ALTAIR', 'Nova Admin', 'Ruin Maniac', 'Male', '''
Excadrill @ Life Orb
Level: 72
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Earthquake
- Iron Head
- Rock Slide
- Swords Dance

Rhyperior
Level: 72
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Stone Edge
- Earthquake
- Megahorn
- Ice Punch

Probopass @ Leftovers
Level: 72
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Power Gem
- Earth Power
- Flash Cannon
- Thunder Wave

Metagross @ Metagrossite
Level: 74
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Meteor Mash
- Zen Headbutt
- Earthquake
- Bullet Punch
''', music='Suspicious')


def lobby(plateau):
    name = 'Seika_League_Lobby'
    m = add(g.MapDef(name, LOBBY, SEC_PLATEAU, 'LAYOUT_INDIGO_PLATEAU_POKEMON_CENTER_1F', 'MUS_RG_GYM',
                     'MAP_TYPE_INDOOR', 'WEATHER_NONE', False, False, False))
    out = 0
    if plateau:
        out = next(i for i, w in enumerate(plateau.warps) if w['dest_map'] == LOBBY)
    door(m, 11, 16, PLATEAU, out)
    door(m, 4, 1, ROOMS[0]['id'], 0)
    door(m, 1, 14, LOBBY + '_2F', 0, elevation=4)
    nurse = 'LOCALID_SEIKA_LEAGUE_NURSE'
    m.objects.append(g.obj('OBJ_EVENT_GFX_NURSE_FRLG', 13, 10, name + '_EventScript_Nurse', local_id=nurse))
    m.objects.append(g.obj('OBJ_EVENT_GFX_CLERK', 0, 7, name + '_EventScript_Clerk', move='MOVEMENT_TYPE_FACE_RIGHT'))
    npc(m, 'OBJ_EVENT_GFX_GYM_GUY', 7, 14, name + '_EventScript_GymGuy',
        'Yo! This is it, the SEIKA LEAGUE!\fFour ELITE trainers, then the\n'
        'CHAMPION. No healing between\nrooms, so stock up here!\fTETSU uses STEEL. FIRE and GROUND\n'
        'melt him down. Good luck, champ!')
    npc(m, 'OBJ_EVENT_GFX_COOLTRAINER_M', 16, 15, name + '_EventScript_Cooltrainer',
        'They say CHAMPION SEIJI guards\nthe path to the SKY SHRINE.\fOnly a CHAMPION may climb to the\n'
        'top of MT. SKYREACH.', 'MOVEMENT_TYPE_WANDER_AROUND')
    m.map_scripts = [('MAP_SCRIPT_ON_TRANSITION', name + '_OnTransition'),
                     ('MAP_SCRIPT_ON_RESUME', 'CableClub_OnResume')]
    m.scripts += (name + '_OnTransition::\n\tsetrespawn HEAL_LOCATION_SEIKA_LEAGUE\n'
                  + ''.join('\tclearflag %s\n' % f for f in E4_FLAGS) + '\tend\n\n'
                  + name + '_EventScript_Nurse::\n\tlock\n\tfaceplayer\n\tcall EventScript_PkmnCenterNurse_Frlg\n\trelease\n\tend\n\n'
                  + name + '_EventScript_Clerk::\n\tlock\n\tfaceplayer\n\tmessage gText_HowMayIServeYou\n\twaitmessage\n'
                  '\tpokemart ' + name + '_Items\n\tmsgbox gText_PleaseComeAgain\n\trelease\n\tend\n\n\t.align 2\n'
                  + name + '_Items::\n' + ''.join('\t.2byte %s\n' % i for i in (
                      'ITEM_ULTRA_BALL', 'ITEM_FULL_RESTORE', 'ITEM_MAX_POTION', 'ITEM_HYPER_POTION', 'ITEM_REVIVE',
                      'ITEM_MAX_REVIVE', 'ITEM_FULL_HEAL', 'ITEM_MAX_ETHER', 'ITEM_MAX_ELIXIR', 'ITEM_MAX_REPEL'))
                  + '\t.2byte ITEM_NONE\n\trelease\n\tend\n\n')
    if plateau:
        w = plateau.warps[out]
        c.HEALS.append({'id': 'HEAL_LOCATION_SEIKA_LEAGUE', 'map': PLATEAU, 'x': w['x'], 'y': w['y'] + 1,
                        'respawn_map': LOBBY, 'respawn_npc': nurse})
    f2 = add(g.MapDef(name + '_2F', LOBBY + '_2F', SEC_PLATEAU, 'LAYOUT_POKEMON_CENTER_2F_FRLG', 'MUS_RG_POKE_CENTER',
                      'MAP_TYPE_INDOOR', 'WEATHER_NONE', False, False, False))
    door(f2, 1, 6, LOBBY, 2, elevation=4)
    door(f2, 5, 1, 'MAP_UNION_ROOM_FRLG', 0, elevation=0)
    door(f2, 9, 1, 'MAP_TRADE_CENTER_FRLG', 0, elevation=0)
    for x, scr in ((5, 'Common_EventScript_UnionRoomAttendant'), (9, 'Common_EventScript_WirelessClubAttendant'),
                   (13, 'Common_EventScript_DirectCornerAttendant')):
        f2.objects.append(g.obj('OBJ_EVENT_GFX_CABLE_CLUB_RECEPTIONIST', x, 2, scr))
    f2.map_scripts = [('MAP_SCRIPT_ON_FRAME_TABLE', 'CableClub_OnFrame'),
                      ('MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE', 'CableClub_OnWarp'),
                      ('MAP_SCRIPT_ON_LOAD', 'CableClub_OnLoad'),
                      ('MAP_SCRIPT_ON_TRANSITION', 'CableClub_OnTransition')]


def e4_room(i, r):
    name = r['name']
    m = add(g.MapDef(name, r['id'], SEC_PLATEAU, r['layout'], r['music'], 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    prev = (LOBBY, 1) if i == 0 else (ROOMS[i - 1]['id'], 1)
    nxt = ROOMS[i + 1]['id'] if i + 1 < len(ROOMS) else CHAMP['id']
    lance = r['layout'] == 'LAYOUT_POKEMON_LEAGUE_LANCES_ROOM'
    if lance:
        door(m, 23, 13, *prev)
        door(m, 6, 5, nxt, 0)
        leader_xy = (6, 8)
    else:
        door(m, 6, 12, *prev)
        door(m, 6, 2, nxt, 0)
        leader_xy = (6, 5)
    fl = E4_FLAGS[i]
    m.objects.append(g.obj(r['gfx'], leader_xy[0], leader_xy[1], name + '_EventScript_Leader'))
    open_door = 'PokemonLeague_EventScript_SetDoorOpenLance' if lance else 'PokemonLeague_EventScript_SetDoorOpen'
    close = 'PokemonLeague_LancesRoom_EventScript_SetEntryClosed' if lance else 'PokemonLeague_EventScript_CloseEntry'
    enter = ('\tapplymovement LOCALID_PLAYER, PokemonLeague_LancesRoom_Movement_WalkThroughCorridor\n\twaitmovement 0\n'
             if lance else '\tapplymovement LOCALID_PLAYER, Common_Movement_WalkUp5\n\twaitmovement 0\n')
    m.map_scripts = [('MAP_SCRIPT_ON_RESUME', name + '_OnResume'), ('MAP_SCRIPT_ON_LOAD', name + '_OnLoad'),
                     ('MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE', name + '_OnWarp'),
                     ('MAP_SCRIPT_ON_FRAME_TABLE', name + '_OnFrame')]
    m.scripts += (
        '%(n)s_OnResume::\n\tsetvar VAR_0x8004, %(light)d\n\tcall PokemonLeague_EventScript_DoLightingEffect\n\tend\n\n'
        '%(n)s_OnLoad::\n\tcall_if_set %(fl)s, %(open)s\n\tcall_if_eq VAR_TEMP_1, 1, %(close)s\n\tend\n\n'
        '%(n)s_OnWarp::\n\tmap_script_2 VAR_TEMP_1, 0, %(n)s_EventScript_TurnNorth\n\t.2byte 0\n\n'
        '%(n)s_EventScript_TurnNorth::\n\tturnobject LOCALID_PLAYER, DIR_NORTH\n\tend\n\n'
        '%(n)s_OnFrame::\n\tmap_script_2 VAR_TEMP_1, 0, %(n)s_EventScript_Enter\n\t.2byte 0\n\n'
        '%(n)s_EventScript_Enter::\n\tlockall\n%(enter)s\tplayse SE_UNLOCK\n\tcall %(close)s\n'
        '\tspecial DrawWholeMapView\n\tsetvar VAR_TEMP_1, 1\n\treleaseall\n\tend\n\n'
        '%(n)s_EventScript_Leader::\n\tlock\n\tfaceplayer\n\tgoto_if_set %(fl)s, %(n)s_EventScript_Post\n'
        '\tmsgbox %(n)s_Text_Intro\n\tsetflag FLAG_TEMP_3\n'
        '\ttrainerbattle_no_intro %(const)s, %(n)s_Text_Defeat\n'
        '\tsetflag %(fl)s\n\tapplymovement LOCALID_PLAYER, Common_Movement_Delay32\n\twaitmovement 0\n'
        '\tplayse SE_DOOR\n\tcall %(open)s\n\tspecial DrawWholeMapView\n'
        '\tmsgbox %(n)s_Text_Post\n\trelease\n\tend\n\n'
        '%(n)s_EventScript_Post::\n\tmsgbox %(n)s_Text_Post\n\trelease\n\tend\n\n'
        % dict(n=name, light=r['light'], fl=fl, open=open_door, close=close, enter=enter, const=r['const']))
    m.scripts += T(name + '_Text_Intro', r['intro'] + '{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}')
    m.scripts += T(name + '_Text_Defeat', r['defeat'])
    m.scripts += T(name + '_Text_Post', r['post'])


def champion_room(plateau):
    name = CHAMP['name']
    out_xy = (10, 12)
    if plateau:
        w = next(w for w in plateau.warps if w['dest_map'] == LOBBY)
        out_xy = (w['x'], w['y'] + 1)
    m = add(g.MapDef(name, CHAMP['id'], SEC_PLATEAU, CHAMP['layout'], 'MUS_RG_VICTORY_ROAD', 'MAP_TYPE_INDOOR',
                     'WEATHER_NONE', False, False, False))
    door(m, 6, 19, ROOMS[-1]['id'], 1)
    door(m, 6, 2, PLATEAU, 0)    # the back door is not used; the scene warps out
    m.objects.append(g.obj('OBJ_EVENT_GFX_EXPERT_M', 6, 8, '0x0', local_id='LOCALID_SEIKA_CHAMPION_SEIJI'))
    champ_flag = 'FLAG_QOL_SEIKA_CHAMPION'
    m.map_scripts = [('MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE', name + '_OnWarp'),
                     ('MAP_SCRIPT_ON_FRAME_TABLE', name + '_OnFrame')]
    m.scripts += (
        '%(n)s_OnWarp::\n\tmap_script_2 VAR_TEMP_1, 0, %(n)s_EventScript_TurnNorth\n\t.2byte 0\n\n'
        '%(n)s_EventScript_TurnNorth::\n\tturnobject LOCALID_PLAYER, DIR_NORTH\n\tend\n\n'
        '%(n)s_OnFrame::\n\tmap_script_2 VAR_TEMP_1, 0, %(n)s_EventScript_Enter\n\t.2byte 0\n\n'
        '%(n)s_EventScript_Enter::\n\tlockall\n\tsetvar VAR_TEMP_1, 1\n'
        '\tapplymovement LOCALID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerEnter\n\twaitmovement 0\n'
        '\tdelay 20\n\tgoto_if_set %(cf)s, %(n)s_EventScript_Rematch\n'
        '\tmsgbox %(n)s_Text_Intro\n\tgoto %(n)s_EventScript_Battle\n\n'
        '%(n)s_EventScript_Rematch::\n\tmsgbox %(n)s_Text_Rematch\n'
        '%(n)s_EventScript_Battle::\n\tsetflag FLAG_TEMP_3\n\tsavebgm MUS_VICTORY_GYM_LEADER\n'
        '\ttrainerbattle_no_intro TRAINER_SEIKA_CHAMPION_SEIJI, %(n)s_Text_Defeat\n'
        '\tmsgbox %(n)s_Text_Post\n\tgoto_if_set %(cf)s, %(n)s_EventScript_Leave\n'
        '\tsetflag %(cf)s\n\tplayfanfare MUS_OBTAIN_BADGE\n\tmessage %(n)s_Text_Champion\n\twaitmessage\n\twaitfanfare\n'
        '\tmsgbox %(n)s_Text_Shrine\n'
        '%(n)s_EventScript_Leave::\n\tclosemessage\n\tfadescreen FADE_TO_BLACK\n'
        '\twarp %(plateau)s, %(px)d, %(py)d\n\twaitstate\n\treleaseall\n\tend\n\n'
        % dict(n=name, cf=champ_flag, plateau=PLATEAU, px=out_xy[0], py=out_xy[1]))
    m.scripts += T(name + '_Text_Intro',
                   'So you are the one the lanterns\nhave been guiding.\fI am SEIJI, CHAMPION of SEIKA.\n'
                   'I keep the path to the SKY SHRINE.\fA thousand years ago my family\n'
                   'swore to let only the strongest\nheart walk that path.\fShow me your heart, trainer from\n'
                   'KANTO!{PLAY_BGM}{MUS_RG_VS_CHAMPION}')
    m.scripts += T(name + '_Text_Rematch', 'Back again? Good. The sky never\ntires of a strong heart.{PLAY_BGM}{MUS_RG_VS_CHAMPION}')
    m.scripts += T(name + '_Text_Defeat', 'Remarkable... The lanterns chose\nwell.')
    m.scripts += T(name + '_Text_Post', 'You are stronger than any trainer\nI have faced.')
    m.scripts += T(name + '_Text_Champion', '{PLAYER} became the CHAMPION\nof the SEIKA LEAGUE!')
    m.scripts += T(name + '_Text_Shrine',
                   'Listen. Team NOVA carried the\nSTARGLASS BELL up to the SKY\nSHRINE.\fIf they ring it, the WISHING\n'
                   'STAR will grant THEIR wish.\fThe path above STARWATCH PLATEAU\nis yours now. Hurry!')


def sky_shrine(plateau):
    """The summit of Mt. Skyreach: a stone shrine in the clouds, built from
    Navel Rock's sky tiles with a wider platform."""
    src = 'LAYOUT_NAVEL_ROCK_SUMMIT'
    W, H = 21, 20
    r = g.Room(W, H, 'gTileset_General_Frlg', 'gTileset_NavelRock_Frlg', source=src)
    r.fill(0, 0, W, H, r.at(4, 8))                     # sky
    x0, k = 2, 6                                       # widened platform: left 4..8, centre col 9 x k, right 10..15
    dy = 9 - 16                                        # source row 16 -> row 9
    for sy in range(16, 25):
        y = sy + dy
        x = x0
        for sx in range(4, 9):
            r.set(x, y, r.at(sx, sy)); x += 1
        for _ in range(k):
            r.set(x, y, r.at(9, sy)); x += 1
        for sx in range(10, 16):
            r.set(x, y, r.at(sx, sy)); x += 1
    # the spire, centred on the platform
    cx = x0 + 5 + k // 2
    for sy in range(11, 16):
        for sx in range(6, 13):
            r.set(cx - 3 + (sx - 6), sy + dy, r.at(sx, sy))
    name = 'Seika_SkyShrine'
    m = add(g.MapDef(name, 'MAP_SEIKA_SKY_SHRINE', SEC_SHRINE, r, 'MUS_RG_NANAISEKI', 'MAP_TYPE_UNDERGROUND',
                     'WEATHER_NONE', True, False, False, battle_scene='MAP_BATTLE_SCENE_NORMAL'))
    hole = (x0 + 5 + k, 18 + dy)                       # Navel Rock's way down, kept as the exit
    cave = 0
    if plateau:
        cave = next(i for i, w in enumerate(plateau.warps) if w['dest_map'] == 'MAP_SEIKA_SKY_SHRINE')
    door(m, hole[0], hole[1], PLATEAU, cave)
    m.objects.append(g.obj('OBJ_EVENT_GFX_HEX_MANIAC', cx - 2, 11, name + '_EventScript_Deneb',
                           local_id='LOCALID_SEIKA_SHRINE_DENEB', move='MOVEMENT_TYPE_FACE_UP', flag=HIDE_SHRINE_NOVA))
    m.objects.append(g.obj('OBJ_EVENT_GFX_HIKER_FRLG', cx + 2, 11, name + '_EventScript_Altair',
                           local_id='LOCALID_SEIKA_SHRINE_ALTAIR', move='MOVEMENT_TYPE_FACE_UP', flag=HIDE_SHRINE_NOVA))
    m.objects.append(g.obj('OBJ_EVENT_GFX_QOL_JIRACHI', cx, 7, name + '_EventScript_Jirachi',
                           local_id='LOCALID_SEIKA_SHRINE_JIRACHI', move='MOVEMENT_TYPE_FACE_DOWN', flag=HIDE_JIRACHI,
                           elevation=3))
    m.coords.append(g.coord(cx, 12, name + '_EventScript_Confront', 'VAR_TEMP_2', 0))
    m.coords.append(g.coord(cx - 1, 12, name + '_EventScript_Confront', 'VAR_TEMP_2', 0))
    m.coords.append(g.coord(cx + 1, 12, name + '_EventScript_Confront', 'VAR_TEMP_2', 0))
    m.map_scripts = [('MAP_SCRIPT_ON_TRANSITION', name + '_OnTransition'),
                     ('MAP_SCRIPT_ON_RESUME', name + '_OnResume')]
    m.scripts += (
        '%(n)s_OnTransition::\n'
        '\tcall_if_set %(done)s, %(n)s_EventScript_HideAll\n'
        '\tcall_if_set %(nova)s, %(n)s_EventScript_SkipConfront\n'
        '\tend\n\n'
        '%(n)s_EventScript_HideAll::\n\tsetflag %(hn)s\n\tsetflag %(hj)s\n\tsetvar VAR_TEMP_2, 1\n\treturn\n\n'
        '%(n)s_EventScript_SkipConfront::\n\tsetvar VAR_TEMP_2, 1\n\treturn\n\n'
        '%(n)s_OnResume::\n\tcall_if_set FLAG_SYS_CTRL_OBJ_DELETE, %(n)s_EventScript_TryRemoveJirachi\n\tend\n\n'
        '%(n)s_EventScript_TryRemoveJirachi::\n\tspecialvar VAR_RESULT, GetBattleOutcome\n'
        '\tgoto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn\n'
        '\tremoveobject LOCALID_SEIKA_SHRINE_JIRACHI\n\treturn\n\n'
        # confrontation: Deneb, then Altair, then the bell
        '%(n)s_EventScript_Confront::\n\tlockall\n\tsetvar VAR_TEMP_2, 1\n'
        '\tmsgbox %(n)s_Text_BellRings, MSGBOX_DEFAULT\n'
        '\tapplymovement LOCALID_SEIKA_SHRINE_DENEB, Common_Movement_FaceDown\n'
        '\tapplymovement LOCALID_SEIKA_SHRINE_ALTAIR, Common_Movement_FaceDown\n\twaitmovement 0\n'
        '\tmsgbox %(n)s_Text_DenebIntro, MSGBOX_DEFAULT\n'
        '\ttrainerbattle_no_intro TRAINER_SEIKA_NOVA_DENEB_SHRINE, %(n)s_Text_DenebDefeat\n'
        '\tmsgbox %(n)s_Text_AltairIntro, MSGBOX_DEFAULT\n'
        '\ttrainerbattle_no_intro TRAINER_SEIKA_NOVA_ALTAIR_SHRINE, %(n)s_Text_AltairDefeat\n'
        '\tsetflag %(nova)s\n'
        '\tmsgbox %(n)s_Text_NovaLeave, MSGBOX_DEFAULT\n\tclosemessage\n'
        '\tfadescreen FADE_TO_BLACK\n\tremoveobject LOCALID_SEIKA_SHRINE_DENEB\n\tremoveobject LOCALID_SEIKA_SHRINE_ALTAIR\n'
        '\tsetflag %(hn)s\n\tfadescreen FADE_FROM_BLACK\n'
        '\tmsgbox %(n)s_Text_StarComes, MSGBOX_DEFAULT\n\tclosemessage\n'
        '\tplayse SE_M_DETECT\n\tclearflag %(hj)s\n\taddobject LOCALID_SEIKA_SHRINE_JIRACHI\n\tdelay 30\n'
        '\treleaseall\n\tend\n\n'
        '%(n)s_EventScript_Deneb::\n\tmsgbox %(n)s_Text_DenebIntro, MSGBOX_NPC\n\tend\n\n'
        '%(n)s_EventScript_Altair::\n\tmsgbox %(n)s_Text_AltairIntro, MSGBOX_NPC\n\tend\n\n'
        # the Wishing Star
        '%(n)s_EventScript_Jirachi::\n\tlock\n\tfaceplayer\n\twaitse\n\tplaymoncry SPECIES_JIRACHI, CRY_MODE_ENCOUNTER\n'
        '\tmsgbox %(n)s_Text_Jirachi, MSGBOX_DEFAULT\n\twaitmoncry\n'
        '\tsetwildbattle SPECIES_JIRACHI, 70\n\tsetflag FLAG_SYS_CTRL_OBJ_DELETE\n\tspecial BattleSetup_StartLegendaryBattle\n'
        '\tclearflag FLAG_SYS_CTRL_OBJ_DELETE\n'
        '\tspecialvar VAR_RESULT, GetBattleOutcome\n'
        '\tgoto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, %(n)s_EventScript_Ending\n'
        '\tgoto_if_eq VAR_RESULT, B_OUTCOME_WON, %(n)s_EventScript_StarFlewHome\n'
        '\tgoto_if_eq VAR_RESULT, B_OUTCOME_RAN, %(n)s_EventScript_StarFlewHome\n'
        '\tgoto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, %(n)s_EventScript_StarFlewHome\n'
        '\trelease\n\tend\n\n'
        '%(n)s_EventScript_StarFlewHome::\n\tfadescreen FADE_TO_BLACK\n\tremoveobject LOCALID_SEIKA_SHRINE_JIRACHI\n'
        '\tfadescreen FADE_FROM_BLACK\n\tmsgbox %(n)s_Text_FlewHome, MSGBOX_DEFAULT\n'
        '%(n)s_EventScript_Ending::\n\tsetflag %(hj)s\n\tsetflag %(done)s\n'
        '\tmsgbox %(n)s_Text_Ending, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
        % dict(n=name, done=SEIKA_DONE, nova=SHRINE_NOVA, hn=HIDE_SHRINE_NOVA, hj=HIDE_JIRACHI))
    m.scripts += T(name + '_Text_BellRings',
                   'A clear note rings out over the\nclouds. The STARGLASS BELL!')
    m.scripts += T(name + '_Text_DenebIntro',
                   'DENEB: Darling, you are late.\nThe bell is ringing.\fWhen the WISHING STAR comes down,\n'
                   'our wish brings VEGA back to\nlead us. Stay out of it!{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}')
    m.scripts += T(name + '_Text_DenebDefeat', 'Not again... Not on the last\nday!')
    m.scripts += T(name + '_Text_AltairIntro',
                   'ALTAIR: I dug that star stone\nout of the ground with my hands.\f'
                   'I won\'t let a kid from KANTO\nwaste it. Come on!{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}')
    m.scripts += T(name + '_Text_AltairDefeat', 'The bell... it stopped for\nyou, not for us.')
    m.scripts += T(name + '_Text_NovaLeave',
                   'DENEB: The star turned away from\nus. It is listening to you.\f'
                   'ALTAIR: Let\'s go, DENEB.\nThere is nothing left for us\nhere.')
    m.scripts += T(name + '_Text_StarComes',
                   'The last note of the bell fades.\fA small light falls from the sky\n'
                   'and lands on the shrine...')
    m.scripts += T(name + '_Text_Jirachi', 'Jiraaa!')
    m.scripts += T(name + '_Text_FlewHome',
                   'The WISHING STAR rose into the\nsky and was gone.\fBut it looked back at you, just\n'
                   'once, before it went.')
    m.scripts += T(name + '_Text_Ending',
                   'Far below, the lanterns of\nSEIKA glow in the night.\fThe second STARFALL is over.\n'
                   'Its wish, whatever it was, is\nyours to keep.')


def build():
    plateau = next((m for m in c.MAPS if m.id == PLATEAU), None)
    lobby(plateau)
    for i, r in enumerate(ROOMS):
        e4_room(i, r)
    champion_room(plateau)
    sky_shrine(plateau)
