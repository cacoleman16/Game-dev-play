"""Skyrift: the wish, your companion, the rifts and Vega's last stand
(docs/SKYRIFT.md). The island maps come from skyrift.py; this module adds
the people and the story to them, and to Seika's Sky Shrine and Lanternport.
"""
import re

import common as c
import flymap
from common import g, T, flag, trainer

F = {k: flag('FLAG_QOL_SKYRIFT_' + k, why) for k, why in [
    ('OPEN', 'the sky over the eastern sea tore open'),
    ('WISH_DONE', 'made a wish to the Wishing Star'),
    ('WISH_PEACE', 'wished for peace for Team Nova'),
    ('CHOSEN', 'chose a companion at Lanternport'),
    ('WITH_ALTAIR', 'companion: Altair'),
    ('WITH_DENEB', 'companion: Deneb'),
    ('WITH_MIRA', 'companion: Mira'),
    ('HIDE_TRIO', 'Altair, Deneb and Mira not on Lanternport pier'),
    ('HALLEY', 'met Dr. Halley'),
    ('SIRIUS_GATE', 'beat Sirius on the cliff road'),
    ('CLIMAX', 'the battle on the observatory roof is over'),
    ('VEGA_SAVED', 'took Vega\'s hand'),
    ('VEGA_LOST', 'let Vega go'),
    ('HIDE_REMATCH_VEGA', 'Vega not on the roof for rematches'),
    ('HIDE_REMATCH_SIRIUS', 'Sirius not on the roof for rematches'),
    ('UB_NIHILEGO', 'caught the rift\'s Nihilego'),
    ('UB_BUZZWOLE', 'caught the rift\'s Buzzwole'),
    ('UB_PHEROMOSA', 'caught the rift\'s Pheromosa'),
    ('UB_XURKITREE', 'caught the rift\'s Xurkitree'),
    ('UB_KARTANA', 'caught the rift\'s Kartana'),
    ('FLY_RIFTWAKE', 'Fly unlocked: Riftwake Harbor'),
    ('NECROZMA', 'caught Necrozma in the sixth rift'),
    ('HIDE_NECROZMA', 'the sixth rift is not open'),
]}

COMPANION_FLAGS = '0x3E'   # land + water, kept after a white-out (no FNPC_WHITE_OUT)
COMPANIONS = [  # key, name, overworld sprite, battle partner
    ('ALTAIR', 'ALTAIR', 'OBJ_EVENT_GFX_HIKER_FRLG', 'PARTNER_QOL_ALTAIR'),
    ('DENEB', 'DENEB', 'OBJ_EVENT_GFX_HEX_MANIAC', 'PARTNER_QOL_DENEB'),
    ('MIRA', 'MIRA', 'OBJ_EVENT_GFX_LASS_FRLG', 'PARTNER_QOL_MIRA'),
]
RIFTS = [  # spot key, species, sprite
    ('rift_beach', 'NIHILEGO', 'OBJ_EVENT_GFX_QOL_NIHILEGO'),
    ('rift_basin_1', 'BUZZWOLE', 'OBJ_EVENT_GFX_QOL_BUZZWOLE'),
    ('rift_basin_2', 'PHEROMOSA', 'OBJ_EVENT_GFX_QOL_PHEROMOSA'),
    ('rift_cliffs_1', 'XURKITREE', 'OBJ_EVENT_GFX_QOL_XURKITREE'),
    ('rift_cliffs_2', 'KARTANA', 'OBJ_EVENT_GFX_QOL_KARTANA'),
]
HARBOR = 'MAP_SKYRIFT_RIFTWAKE_HARBOR'
FLY_GEO = [  # map, Fly map cell, kind, Japanese name
    (HARBOR, 24, 12, 'town', 'ハザマみなと'),
    ('MAP_SKYRIFT_SHARDFALL_BEACH', 25, 12, 'route', 'カケラはま'),
    ('MAP_SKYRIFT_METEOR_BASIN', 26, 11, 'route', 'インセキのくぼち'),
    ('MAP_SKYRIFT_OBSERVATORY_CLIFFS', 26, 10, 'route', 'テンモンがけ'),
    ('MAP_SKYRIFT_OBSERVATORY_1F', 26, 9, 'route', 'ノバてんもんだい'),
]
P = 'QoL_Skyrift'

trainer('TRAINER_SKYRIFT_SIRIUS_GATE', 'SIRIUS', 'Nova Admin', 'Cooltrainer M Frlg', 'Male', '''
Lycanroc-Midnight @ Life Orb
Level: 74
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Stone Edge
- Crunch
- Close Combat
- Swords Dance

Houndoom
Level: 74
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Dark Pulse
- Flamethrower
- Sludge Bomb
- Nasty Plot

Arcanine @ Sitrus Berry
Level: 74
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Flare Blitz
- Extreme Speed
- Wild Charge
- Close Combat

Mightyena
Level: 74
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Sucker Punch
- Crunch
- Play Rough
- Swords Dance
''', music='Suspicious')
trainer('TRAINER_SKYRIFT_SIRIUS', 'SIRIUS', 'Nova Admin', 'Cooltrainer M Frlg', 'Male', '''
Lycanroc-Midnight @ Life Orb
Level: 76
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Stone Edge
- Crunch
- Close Combat
- Swords Dance

Houndoom @ Houndoominite
Level: 76
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Dark Pulse
- Flamethrower
- Sludge Bomb
- Nasty Plot

Arcanine @ Sitrus Berry
Level: 76
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Flare Blitz
- Extreme Speed
- Wild Charge
- Close Combat

Luxray
Level: 76
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Wild Charge
- Crunch
- Ice Fang
- Volt Switch
''', music='Suspicious')
trainer('TRAINER_SKYRIFT_VEGA', 'VEGA', 'Nova Leader', 'Lady', 'Female', '''
Clefable @ Leftovers
Level: 78
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Moonblast
- Cosmic Power
- Moonlight
- Flamethrower

Starmie
Level: 78
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Surf
- Psychic
- Ice Beam
- Thunderbolt

Reuniclus @ Sitrus Berry
Level: 79
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Psychic
- Focus Blast
- Shadow Ball
- Recover

Celesteela @ Leftovers
Level: 80
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
- Heavy Slam
- Air Slash
- Flamethrower
- Leech Seed
''', music='Rich')


def _width(s):
    return len(re.sub(r'\{[A-Z_]+ ?[0-9A-Fx]*\}', '', s.replace('{PLAYER}', 'PLAYERNA')))


def say(who, text):
    """A speaker's line: 'NAME: ...', with the first page re-wrapped to fit."""
    if not who:
        return text
    pages = text.split('\f')
    lines, cur = [], ''
    for w in (who + ': ' + pages[0].replace('\n', ' ')).split():
        if cur and _width(cur + ' ' + w) > c.LINE_MAX:
            lines.append(cur)
            cur = w
        else:
            cur = (cur + ' ' + w) if cur else w
    pages[0] = '\n'.join(lines + [cur])
    return '\f'.join(pages)


def find(map_id):
    return next((m for m in c.MAPS if m.id == map_id), None)


def wrap_transition(m, asm, label):
    """Run `asm` on entering map m, before its existing ON_TRANSITION script."""
    orig = [l for k, l in m.map_scripts if k == 'MAP_SCRIPT_ON_TRANSITION']
    m.map_scripts = [(k, l) for k, l in m.map_scripts if k != 'MAP_SCRIPT_ON_TRANSITION']
    m.map_scripts.append(('MAP_SCRIPT_ON_TRANSITION', label))
    m.scripts += '%s::\n%s\t%s\n\n' % (label, asm, ('goto ' + orig[0]) if orig else 'end')


def build():
    try:
        import skyrift
        spots = getattr(skyrift, 'STORY_SPOTS', {})
    except ImportError:
        spots = {}
    shrine = find('MAP_SEIKA_SKY_SHRINE')
    lp = find('MAP_SEIKA_LANTERNPORT')
    if shrine:
        wish(shrine)
    if lp:
        lanternport(lp, spots)
    companion_talk(lp or shrine)
    if not spots:
        return
    isles(spots)
    fly()


# ------------------------------------------------------------------ the wish

def wish(m):
    n = P + '_Wish'
    m.scripts += (
        'QoL_EventScript_SkyriftWish::\n\tlock\n\tfaceplayer\n'
        '\tgoto_if_set %(wd)s, Seika_SkyShrine_EventScript_Jirachi\n'
        '\tplaymoncry SPECIES_JIRACHI, CRY_MODE_ENCOUNTER\n\tmsgbox %(n)s_Text_Intro, MSGBOX_DEFAULT\n\twaitmoncry\n'
        '%(n)s_Ask::\n\tmessage %(n)s_Text_Ask\n\twaitmessage\n'
        '\tdynmultipush %(n)s_Text_Strength, 0\n\tdynmultipush %(n)s_Text_Friend, 1\n'
        '\tdynmultipush %(n)s_Text_Fortune, 2\n\tdynmultipush %(n)s_Text_Peace, 3\n'
        '\tdynmultistack 0, 0, TRUE, 4, FALSE, 0, DYN_MULTICHOICE_CB_NONE\n'
        '\tswitch VAR_RESULT\n\tcase 0, %(n)s_Strength\n\tcase 1, %(n)s_Friend\n'
        '\tcase 2, %(n)s_Fortune\n\tcase 3, %(n)s_Peace\n\tgoto %(n)s_Ask\n\n'
        '%(n)s_Strength::\n\tmsgbox %(n)s_Text_Granted, MSGBOX_DEFAULT\n'
        '\tgiveitem ITEM_ABILITY_PATCH\n\tgiveitem ITEM_GOLD_BOTTLE_CAP\n\tgiveitem ITEM_RARE_CANDY, 5\n'
        '\tgoto %(n)s_Test\n\n'
        '%(n)s_Fortune::\n\tmsgbox %(n)s_Text_Granted, MSGBOX_DEFAULT\n\tgiveitem ITEM_SHINY_CHARM\n\tgoto %(n)s_Test\n\n'
        '%(n)s_Peace::\n\tsetflag %(peace)s\n\tmsgbox %(n)s_Text_PeaceGranted, MSGBOX_DEFAULT\n'
        '\tgiveitem ITEM_COMET_SHARD\n\tgoto %(n)s_Test\n\n'
        '%(n)s_Test::\n\tsetflag %(wd)s\n\tmsgbox %(n)s_Text_Test, MSGBOX_DEFAULT\n'
        '\tgoto Seika_SkyShrine_EventScript_Jirachi\n\n'
        '%(n)s_Friend::\n\tsetflag %(wd)s\n\tmsgbox %(n)s_Text_FriendGranted, MSGBOX_DEFAULT\n'
        '\tgivemon SPECIES_JIRACHI, 70, ITEM_STAR_PIECE, ITEM_CHERISH_BALL\n'
        '\tbufferspeciesname STR_VAR_1, SPECIES_JIRACHI\n\tplayfanfare MUS_RG_OBTAIN_KEY_ITEM\n'
        '\tmessage %(n)s_Text_GotJirachi\n\twaitmessage\n\twaitfanfare\n'
        '\tfadescreen FADE_TO_BLACK\n\tremoveobject LOCALID_SEIKA_SHRINE_JIRACHI\n\tfadescreen FADE_FROM_BLACK\n'
        '\tgoto Seika_SkyShrine_EventScript_Ending\n\n'
        # the sky tears: start of the Skyrift chapter
        'QoL_EventScript_SkyriftOpens::\n\tsetflag %(open)s\n\tdelay 30\n\tplayse SE_THUNDER\n'
        '\tsetvar VAR_0x8004, 2\n\tsetvar VAR_0x8005, 2\n\tsetvar VAR_0x8006, 12\n\tsetvar VAR_0x8007, 3\n'
        '\tspecial ShakeCamera\n\twaitstate\n'
        '\tmsgbox %(n)s_Text_SkyTears, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
        % dict(n=n, wd=F['WISH_DONE'], peace=F['WISH_PEACE'], open=F['OPEN']))
    m.scripts += T(n + '_Text_Intro',
                   'The WISHING STAR opens its eyes\nand looks straight at you.\f'
                   'It is waiting for your wish.')
    m.scripts += T(n + '_Text_Ask', 'What will you wish for?')
    m.scripts += T(n + '_Text_Strength', 'STRENGTH')
    m.scripts += T(n + '_Text_Friend', 'A FRIEND')
    m.scripts += T(n + '_Text_Fortune', 'FORTUNE')
    m.scripts += T(n + '_Text_Peace', 'PEACE FOR NOVA')
    m.scripts += T(n + '_Text_Granted', 'The star glows brightly...\nYour wish is granted!')
    m.scripts += T(n + '_Text_PeaceGranted',
                   'The star is quiet for a long time.\f'
                   'Then its light flies away across\nthe sea, looking for someone.\f'
                   'Something small falls into your\nhands.')
    m.scripts += T(n + '_Text_Test',
                   'The WISHING STAR spins in the air.\nIt wants to see how strong you\nare!')
    m.scripts += T(n + '_Text_FriendGranted',
                   'The WISHING STAR laughs and floats\nright into your arms!')
    m.scripts += T(n + '_Text_GotJirachi', '{PLAYER} made a new friend:\n{STR_VAR_1}!')
    m.scripts += T(n + '_Text_SkyTears',
                   'Far to the east, over the sea,\nthe night sky TEARS open like\npaper.\f'
                   'A crack of strange light hangs\nabove the water.\f'
                   'Someone will be waiting on\nLANTERNPORT\'s pier...')


# ---------------------------------------------------- choosing a companion

def lanternport(m, spots):
    n = P + '_Pier'
    pos = {'ALTAIR': (17, 25), 'DENEB': (23, 25), 'MIRA': (20, 24)}
    lid = {}
    for key, name, gfx, partner in COMPANIONS:
        lid[key] = 'LOCALID_SKYRIFT_PIER_' + key
        x, y = pos[key]
        m.objects.append(g.obj(gfx, x, y, n + '_EventScript_Talk', local_id=lid[key],
                               move='MOVEMENT_TYPE_FACE_DOWN' if key == 'MIRA' else 'MOVEMENT_TYPE_FACE_UP',
                               flag=F['HIDE_TRIO']))
    for x in (19, 20, 21):
        m.coords.append(g.coord(x, 23, n + '_EventScript_Talk', 'VAR_TEMP_4', 0))
    wrap_transition(m, '\tcall %s_EventScript_Setup\n' % n, n + '_OnTransition')
    s = ('%(n)s_EventScript_Setup::\n\tsetflag %(hide)s\n\tsetvar VAR_TEMP_4, 1\n'
         # a save that met the Wishing Star before the Skyrift existed
         '\tcall_if_set FLAG_QOL_SEIKA_DONE, %(n)s_EventScript_Open\n'
         '\tgoto_if_unset %(open)s, Common_EventScript_NopReturn\n'
         '\tgoto_if_set %(chosen)s, Common_EventScript_NopReturn\n'
         '\tclearflag %(hide)s\n\tsetvar VAR_TEMP_4, 0\n\treturn\n\n'
         '%(n)s_EventScript_Open::\n\tsetflag %(open)s\n\treturn\n\n'
         '%(n)s_EventScript_Talk::\n\tlockall\n\tsetvar VAR_TEMP_4, 1\n'
         '\tmsgbox %(n)s_Text_Mira1, MSGBOX_DEFAULT\n\tmsgbox %(n)s_Text_Altair1, MSGBOX_DEFAULT\n'
         '\tmsgbox %(n)s_Text_Deneb1, MSGBOX_DEFAULT\n\tmsgbox %(n)s_Text_Mira2, MSGBOX_DEFAULT\n'
         '%(n)s_Choose::\n\tmessage %(n)s_Text_Choose\n\twaitmessage\n'
         '\tdynmultipush %(n)s_Text_OptAltair, 0\n\tdynmultipush %(n)s_Text_OptDeneb, 1\n'
         '\tdynmultipush %(n)s_Text_OptMira, 2\n\tdynmultipush %(n)s_Text_OptLater, 3\n'
         '\tdynmultistack 0, 0, FALSE, 4, FALSE, 0, DYN_MULTICHOICE_CB_NONE\n'
         '\tswitch VAR_RESULT\n\tcase 0, %(n)s_PickALTAIR\n\tcase 1, %(n)s_PickDENEB\n'
         '\tcase 2, %(n)s_PickMIRA\n\tmsgbox %(n)s_Text_Later, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
         % dict(n=n, hide=F['HIDE_TRIO'], open=F['OPEN'], chosen=F['CHOSEN']))
    for key, name, gfx, partner in COMPANIONS:
        s += ('%(n)s_Pick%(k)s::\n\tmsgbox %(n)s_Text_Confirm%(k)s, MSGBOX_YESNO\n'
              '\tgoto_if_eq VAR_RESULT, NO, %(n)s_Choose\n'
              '\tsetflag %(chosen)s\n\tsetflag %(with)s\n'
              '\tmsgbox %(n)s_Text_Picked%(k)s, MSGBOX_DEFAULT\n\tclosemessage\n'
              '\tfadescreen FADE_TO_BLACK\n'
              + ''.join('\tremoveobject %s\n' % lid[k2] for k2, _, _, _ in COMPANIONS) +
              '\tsetflag %(hide)s\n'
              '\tcreatefollowernpc %(gfx)s, %(cf)s, QoL_EventScript_CompanionTalk, %(partner)s\n'
              '\tfadescreen FADE_FROM_BLACK\n\tmsgbox %(n)s_Text_Ready, MSGBOX_DEFAULT\n\treleaseall\n\tend\n\n'
              % dict(n=n, k=key, chosen=F['CHOSEN'], hide=F['HIDE_TRIO'], gfx=gfx, cf=COMPANION_FLAGS,
                     partner=partner, **{'with': F['WITH_' + key]}))
    m.scripts += s
    m.scripts += T(n + '_Text_Mira1',
                   say('MIRA', '{PLAYER}! You came!\fI\'m MIRA. Grandpa SEIJI says\nyou\'re the new CHAMPION!\f'
                       'I saw it from the SKY SHRINE.\nThe sky tore open over the\neastern sea!'))
    m.scripts += T(n + '_Text_Altair1',
                   say('ALTAIR', '...And VEGA went through it.\fShe\'s out on the SKYRIFT ISLES\nwith what\'s left of TEAM NOVA.'))
    m.scripts += T(n + '_Text_Deneb1',
                   say('DENEB', 'We can\'t reach her alone,\ndarling.\fHer bodyguard SIRIUS won\'t even\nlet us near the ISLES.'))
    m.scripts += T(n + '_Text_Mira2',
                   say('MIRA', 'Only two can make the crossing\non a sky this wild.\f'
                       'So... take one of us with you!'))
    m.scripts += T(n + '_Text_Choose', 'Who will go with you to the\nSKYRIFT ISLES?')
    m.scripts += T(n + '_Text_OptAltair', 'ALTAIR')
    m.scripts += T(n + '_Text_OptDeneb', 'DENEB')
    m.scripts += T(n + '_Text_OptMira', 'MIRA')
    m.scripts += T(n + '_Text_OptLater', 'NOT YET')
    m.scripts += T(n + '_Text_Later', say('MIRA', 'We\'ll wait right here. Don\'t\ntake too long!'))
    m.scripts += T(n + '_Text_ConfirmALTAIR',
                   'ALTAIR battles with GROUND, ROCK\nand STEEL POKéMON.\fTake ALTAIR with you?')
    m.scripts += T(n + '_Text_ConfirmDENEB',
                   'DENEB battles with PSYCHIC and\nFAIRY POKéMON.\fTake DENEB with you?')
    m.scripts += T(n + '_Text_ConfirmMIRA',
                   'MIRA battles with PSYCHIC, FAIRY\nand WATER POKéMON.\fTake MIRA with you?')
    m.scripts += T(n + '_Text_PickedALTAIR',
                   say('ALTAIR', 'Ha! Didn\'t think you\'d pick the\nguy who tried to stop you.\f'
                       'I dug up that star stone. I owe\nVEGA the truth. Let\'s go.') +
                   '\f' + say('DENEB', 'Bring her home, you lump.'))
    m.scripts += T(n + '_Text_PickedDENEB',
                   say('DENEB', 'Me? How flattering, darling.\f'
                       'I paid for every bolt of VEGA\'s\nmachines. Let me help switch\nthem off.') +
                   '\f' + say('ALTAIR', 'Don\'t let her charm you, kid.'))
    m.scripts += T(n + '_Text_PickedMIRA',
                   say('MIRA', 'Really?! I won\'t let you down!\f'
                       'Grandpa taught me to read the\nstars. Now I get to see one up\nclose!') +
                   '\f' + say('ALTAIR', '...Keep her safe.'))
    m.scripts += T(n + '_Text_Ready',
                   'Your companion will travel and\nbattle beside you.\f'
                   'The S.S. TIDAL can sail you both\nto the SKYRIFT ISLES.')


def companion_talk(m):
    """Talk to your companion: lines by companion and story stage."""
    if m is None:
        return
    n = P + '_Chat'
    s = 'QoL_EventScript_CompanionTalk::\n\tlock\n\tfacefollowernpc\n'
    for key, _, _, _ in COMPANIONS:
        s += '\tgoto_if_set %s, %s_%s\n' % (F['WITH_' + key], n, key)
    s += '\trelease\n\tend\n\n'
    stages = [('CLIMAX', 'After'), ('SIRIUS_GATE', 'Roof'), ('HALLEY', 'Isles'), (None, 'Start')]
    lines = {
        'ALTAIR': {
            'Start': ['I haven\'t been on a boat since\nKANTO. My stomach remembers.',
                      'You and me, eh? Strange world.\nLast month I was digging rocks\nto stop you.'],
            'Isles': ['These shards... they\'re the\nsame stuff as the star stone.\nThe whole beach is made of sky.',
                      'Ultra Beasts in the rifts.\nVEGA always said the sky had\nmore in it than DEOXYS.'],
            'Roof': ['SIRIUS was always her shadow.\nNever liked him. Never had to.',
                     'VEGA\'s up there. I can feel the\nground humming. Stay close.'],
            'After': ['Whatever happens next, I\'m done\ndigging for other people.',
                      'Thanks, kid. Really.\nDon\'t make me say it twice.'],
        },
        'DENEB': {
            'Start': ['Do you like my hat? I chose it\nfor the crossing. Priorities,\ndarling.',
                      'VEGA once said I counted money\nlike other people count stars.\nShe wasn\'t wrong.'],
            'Isles': ['Rifts, shards, beasts from\nanother sky... VEGA spent a\nfortune on this place.',
                      'DR. HALLEY used to work for us.\nShe quit the day VEGA bought\nthe engine.'],
            'Roof': ['SIRIUS hates me. I signed his\npaychecks. It\'s complicated.',
                     'If we turn that engine off,\nI\'m finally out of work.\nWhat a relief.'],
            'After': ['You know, darling, I think I\'ll\nkeep this hat.',
                      'Strange... I don\'t miss the\nmoney at all.'],
        },
        'MIRA': {
            'Start': ['Grandpa gave me his old star\nchart. I can read every\nconstellation!',
                      'Do you think the WISHING STAR\nwill come back in another\nthousand years?'],
            'Isles': ['Look at the sky through the\nrift! Those stars aren\'t any\nI know!',
                      'Ultra Beasts! I\'ve only seen\nthem in Grandpa\'s oldest books.'],
            'Roof': ['VEGA, DENEB, ALTAIR... they\'re\nall named after stars. Like a\nconstellation that went wrong.',
                     'I\'m a little scared. But I\'m\nmore excited! Is that weird?'],
            'After': ['Wait until Grandpa hears!\nHe\'ll fall right off his throne!',
                      'I\'m going to draw a new star\nchart. You\'re on it!'],
        },
    }
    for key, name, _, _ in COMPANIONS:
        s += '%s_%s::\n' % (n, key)
        for fl, st in stages:
            if fl:
                s += '\tgoto_if_set %s, %s_%s%s\n' % (F[fl], n, key, st)
            else:
                s += '\tgoto %s_%s%s\n' % (n, key, st)
        s += '\n'
        for _, st in stages:
            s += ('%(n)s_%(k)s%(st)s::\n\trandom 2\n\tgoto_if_eq VAR_RESULT, 1, %(n)s_%(k)s%(st)sB\n'
                  '\tmsgbox %(n)s_Text_%(k)s%(st)sA, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_%(k)s%(st)sB::\n\tmsgbox %(n)s_Text_%(k)s%(st)sB, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % dict(n=n, k=key, st=st))
            m.scripts += T('%s_Text_%s%sA' % (n, key, st), say(name, lines[key][st][0]))
            m.scripts += T('%s_Text_%s%sB' % (n, key, st), say(name, lines[key][st][1]))
    m.scripts += s


# ------------------------------------------------------------ the islands

def isles(spots):
    by_spot = {}
    for k, (mid, x, y) in spots.items():
        by_spot[k] = (find(mid), x, y)

    def place(key, gfx, script, local_id, flag_name='0', move='MOVEMENT_TYPE_FACE_DOWN'):
        m, x, y = by_spot[key]
        m.objects.append(g.obj(gfx, x, y, script, local_id=local_id, move=move, flag=flag_name))
        return m

    # Dr. Halley
    m = place('halley', 'OBJ_EVENT_GFX_SCIENTIST', P + '_EventScript_Halley', 'LOCALID_SKYRIFT_HALLEY')
    n = P + '_Halley'
    m.scripts += ('%(p)s_EventScript_Halley::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_set %(climax)s, %(n)s_After\n\tgoto_if_set %(met)s, %(n)s_Again\n'
                  '\tmsgbox %(n)s_Text_Intro, MSGBOX_DEFAULT\n\tgiveitem ITEM_BEAST_BALL, 5\n'
                  '\tsetflag %(met)s\n\tmsgbox %(n)s_Text_Rifts, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_Again::\n\tmsgbox %(n)s_Text_Rifts, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_After::\n%(six)s'
                  '\tmsgbox %(n)s_Text_After, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_Sixth::\n\tmsgbox %(n)s_Text_Sixth, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  % dict(p=P, n=n, climax=F['CLIMAX'], met=F['HALLEY'],
                         six=('\tcall %s_Necrozma_Check\n\tgoto_if_unset %s, %s_Sixth\n'
                              % (P, F['HIDE_NECROZMA'], n)) if 'necrozma' in spots else ''))
    m.scripts += T(n + '_Text_Intro',
                   say('DR. HALLEY', 'Visitors! From the CHAMPION\nof SEIKA, no less.\f'
                       'I study the SKYRIFT. I used to\nwork for VEGA, until she\nbought that engine.\f'
                       'Five tears in the sky have\nopened on these islands. Out of\neach one, an ULTRA BEAST.\f'
                       'Take these. BEAST BALLS work on\nthem far better than anything\nelse.'))
    m.scripts += T(n + '_Text_Rifts',
                   say('DR. HALLEY', 'The rift energy makes each\nULTRA BEAST much stronger.\f'
                       'One on SHARDFALL BEACH, two in\nMETEOR BASIN, two on the\nOBSERVATORY CLIFFS.\f'
                       'VEGA is in her OBSERVATORY at\nthe top. SIRIUS guards the road.'))
    m.scripts += T(n + '_Text_Sixth',
                   say('DR. HALLEY', 'My readings! A sixth rift has\nopened in METEOR BASIN, by the\nbig meteorite.\f'
                       'Whatever is in it is eating\nthe light around it. Be very\ncareful!'))
    m.scripts += T(n + '_Text_After',
                   say('DR. HALLEY', 'The engine is quiet. The rifts\nare calming down.\f'
                       'Some of the ULTRA BEASTS may\nstay. This is their home now,\ntoo.'))

    # the inn: rest, and (after the climax) choose who travels with you
    m = place('innkeeper', 'OBJ_EVENT_GFX_WOMAN_2_FRLG', P + '_EventScript_Inn', 'LOCALID_SKYRIFT_INNKEEPER')
    n = P + '_Inn'
    s = ('%(p)s_EventScript_Inn::\n\tlock\n\tfaceplayer\n\tmsgbox %(n)s_Text_Welcome, MSGBOX_YESNO\n'
         '\tgoto_if_eq VAR_RESULT, NO, %(n)s_Companion\n'
         '\tmsgbox %(n)s_Text_Rest, MSGBOX_DEFAULT\n\tclosemessage\n\tfadescreen FADE_TO_BLACK\n'
         '\tplayfanfare MUS_HEAL\n\twaitfanfare\n\tspecial HealPlayerParty\n\tfadescreen FADE_FROM_BLACK\n'
         '\tmsgbox %(n)s_Text_Morning, MSGBOX_DEFAULT\n'
         '%(n)s_Companion::\n\tgoto_if_unset %(climax)s, %(n)s_Bye\n'
         '\tmsgbox %(n)s_Text_Lounge, MSGBOX_DEFAULT\n'
         '\tmessage %(n)s_Text_Who\n\twaitmessage\n'
         '\tdynmultipush %(pier)s_Text_OptAltair, 0\n\tdynmultipush %(pier)s_Text_OptDeneb, 1\n'
         '\tdynmultipush %(pier)s_Text_OptMira, 2\n\tdynmultipush %(n)s_Text_OptAlone, 3\n'
         '\tdynmultipush %(n)s_Text_OptKeep, 4\n'
         '\tdynmultistack 0, 0, FALSE, 5, FALSE, 0, DYN_MULTICHOICE_CB_NONE\n'
         '\tswitch VAR_RESULT\n\tcase 0, %(n)s_SetALTAIR\n\tcase 1, %(n)s_SetDENEB\n\tcase 2, %(n)s_SetMIRA\n'
         '\tcase 3, %(n)s_Alone\n'
         '%(n)s_Bye::\n\tmsgbox %(n)s_Text_Bye, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         '%(n)s_Clear::\n\tcheckfollowernpc\n\tgoto_if_eq VAR_RESULT, FALSE, %(n)s_ClearFlags\n'
         '\tdestroyfollowernpc\n'
         '%(n)s_ClearFlags::\n'
         + ''.join('\tclearflag %s\n' % F['WITH_' + k] for k, _, _, _ in COMPANIONS) +
         '\treturn\n\n'
         '%(n)s_Alone::\n\tcall %(n)s_Clear\n\tmsgbox %(n)s_Text_AloneDone, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
         % dict(p=P, n=n, climax=F['CLIMAX'], pier=P + '_Pier'))
    for key, name, gfx, partner in COMPANIONS:
        s += ('%(n)s_Set%(k)s::\n\tcall %(n)s_Clear\n\tsetflag %(with)s\n'
              '\tcreatefollowernpc %(gfx)s, %(cf)s, QoL_EventScript_CompanionTalk, %(partner)s\n'
              '\tmsgbox %(n)s_Text_Joined%(k)s, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
              % dict(n=n, k=key, gfx=gfx, cf=COMPANION_FLAGS, partner=partner, **{'with': F['WITH_' + key]}))
        m.scripts += T('%s_Text_Joined%s' % (n, key), say(name, {
            'ALTAIR': 'Back on the road? Fine by me.',
            'DENEB': 'Shall we, darling?',
            'MIRA': 'Yay! Where are we going next?'}[key]))
    m.scripts += s
    m.scripts += T(n + '_Text_Welcome',
                   'Welcome to the RIFTWAKE INN!\nWould you like to rest?')
    m.scripts += T(n + '_Text_Rest', 'Sleep well!')
    m.scripts += T(n + '_Text_Morning', 'Good morning! Your POKéMON look\nfull of energy.')
    m.scripts += T(n + '_Text_Lounge',
                   'ALTAIR, DENEB and MIRA are all\nstaying here now.\f'
                   'They\'d each love to travel with\nyou, wherever you go.')
    m.scripts += T(n + '_Text_Who', 'Who should travel with you?')
    m.scripts += T(n + '_Text_OptAlone', 'TRAVEL ALONE')
    m.scripts += T(n + '_Text_OptKeep', 'NO CHANGE')
    m.scripts += T(n + '_Text_AloneDone', 'Your companion waves goodbye.\nThey\'ll be here at the inn.')
    m.scripts += T(n + '_Text_Bye', 'Come back anytime!')

    # the rifts: Ultra Beasts boosted by the rift's energy
    for key, sp, gfx in RIFTS:
        fl = F['UB_' + sp]
        lid = 'LOCALID_SKYRIFT_RIFT_' + sp
        m = place(key, gfx, P + '_EventScript_Rift' + sp, lid, fl)
        n = P + '_Rift' + sp
        m.scripts += ('%(p)s_EventScript_Rift%(sp)s::\n\tlock\n\tfaceplayer\n'
                      '\tmsgbox %(p)s_Text_RiftHum, MSGBOX_DEFAULT\n'
                      '\tplaymoncry SPECIES_%(sp)s, CRY_MODE_ENCOUNTER\n\twaitmoncry\n'
                      '\tsetwildbattle SPECIES_%(sp)s, 75\n'
                      '\tsettotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1\n'
                      '\tsetflag FLAG_SYS_CTRL_OBJ_DELETE\n\tspecial BattleSetup_StartLegendaryBattle\n'
                      '\tclearflag FLAG_SYS_CTRL_OBJ_DELETE\n'
                      '\tspecialvar VAR_RESULT, GetBattleOutcome\n'
                      '\tgoto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, %(n)s_Caught\n'
                      '\tmsgbox %(p)s_Text_RiftBack, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                      '%(n)s_Caught::\n\tsetflag %(fl)s\n\tremoveobject %(lid)s\n'
                      '\tmsgbox %(p)s_Text_RiftClosed, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                      % dict(p=P, sp=sp, n=n, fl=fl, lid=lid))
    m.scripts += T(P + '_Text_RiftHum',
                   'The air around it bends and\nshimmers. Rift energy pours\nout of the tear!\f'
                   'An ULTRA BEAST, wrapped in\nthe rift\'s power!')
    m.scripts += T(P + '_Text_RiftBack', 'It slipped back toward the\nrift... It\'s still here.')
    m.scripts += T(P + '_Text_RiftClosed', 'With the ULTRA BEAST gone,\nthe little rift closed.')

    if 'necrozma' in by_spot:
        sixth_rift(by_spot)
    sailors(by_spot)
    sirius_gate(by_spot)
    roof(by_spot)


def sixth_rift(by_spot):
    """After the climax, with all five Ultra Beasts caught, a sixth rift
    opens in Meteor Basin: Necrozma, drawn by the engine's last light."""
    m, x, y = by_spot['necrozma']
    n = P + '_Necrozma'
    lid = 'LOCALID_SKYRIFT_NECROZMA'
    m.objects.append(g.obj('OBJ_EVENT_GFX_QOL_NECROZMA', x, y, n + '_EventScript', local_id=lid,
                           flag=F['HIDE_NECROZMA']))
    checks = ''.join('	goto_if_unset %s, %s_Shut
' % (F['UB_' + sp], n) for _, sp, _ in RIFTS)
    wrap_transition(m, '	call %s_Check
' % n, n + '_OnTransition')
    m.scripts += ('%(n)s_Check::
	setflag %(hide)s
	goto_if_unset %(climax)s, %(n)s_Shut
'
                  '	goto_if_set %(caught)s, %(n)s_Shut
%(checks)s	clearflag %(hide)s
'
                  '%(n)s_Shut::
	return

'
                  '%(n)s_EventScript::
	lock
	faceplayer
	msgbox %(n)s_Text_Seen, MSGBOX_DEFAULT
'
                  '	playmoncry SPECIES_NECROZMA, CRY_MODE_ENCOUNTER
	waitmoncry
'
                  '	setwildbattle SPECIES_NECROZMA, 80
'
                  '	settotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1
'
                  '	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
'
                  '	clearflag FLAG_SYS_CTRL_OBJ_DELETE
'
                  '	specialvar VAR_RESULT, GetBattleOutcome
'
                  '	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, %(n)s_Caught
'
                  '	msgbox %(n)s_Text_Back, MSGBOX_DEFAULT
	release
	end

'
                  '%(n)s_Caught::
	setflag %(caught)s
	setflag %(hide)s
	removeobject %(lid)s
'
                  '	msgbox %(n)s_Text_Closed, MSGBOX_DEFAULT
	release
	end

'
                  % dict(n=n, lid=lid, hide=F['HIDE_NECROZMA'], climax=F['CLIMAX'],
                         caught=F['NECROZMA'], checks=checks))
    m.scripts += T(n + '_Text_Seen',
                   'Light is pouring into the\nsixth rift... and something\ninside is drinking it!\f'
                   'A prism-black POKéMON, starved\nfor light, crawls out of the\ntear!')
    m.scripts += T(n + '_Text_Back', 'It pulled back into the dark\nof the rift... It\'s still there.')
    m.scripts += T(n + '_Text_Closed', 'The sixth rift flickered\nand went out.')


def sailors(by_spot):
    """Ferries: Lanternport and Vermilion sail to the isles once you have a
    companion; Riftwake's sailor sails back."""
    m, ax, ay = by_spot['arrival']
    n = P + '_Ferry'
    m.scripts += ('Skyrift_EventScript_HarborSailor::\n\tlock\n\tfaceplayer\n'
                  '\tmessage %(n)s_Text_Where\n\twaitmessage\n'
                  '\tdynmultipush %(n)s_Text_Lanternport, 0\n\tdynmultipush QoL_Text_DestCancel, 1\n'
                  '\tdynmultipush %(n)s_Text_Vermilion, 2\n'
                  '\tdynmultistack 0, 0, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE\n'
                  '\tswitch VAR_RESULT\n\tcase 0, %(n)s_ToLanternport\n\tcase 2, QoL_EventScript_SailToKanto\n'
                  '\tmsgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_ToLanternport::\n\tcall QoL_EventScript_SetSail\n\twarp MAP_SEIKA_LANTERNPORT, 20, 28\n'
                  '\twaitstate\n\trelease\n\tend\n\n'
                  'QoL_EventScript_SailToSkyrift::\n\tcall QoL_EventScript_SetSail\n\twarp %(map)s, %(x)d, %(y)d\n'
                  '\twaitstate\n\trelease\n\tend\n\n'
                  'QoL_EventScript_LanternportSailor::\n\tlock\n\tfaceplayer\n'
                  '\tgoto_if_unset %(chosen)s, %(n)s_LanternportKanto\n'
                  '\tmessage %(n)s_Text_Where\n\twaitmessage\n'
                  '\tdynmultipush %(n)s_Text_Skyrift, 0\n\tdynmultipush %(n)s_Text_Vermilion, 1\n'
                  '\tdynmultipush QoL_Text_DestCancel, 2\n'
                  '\tdynmultistack 0, 0, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE\n'
                  '\tswitch VAR_RESULT\n\tcase 0, QoL_EventScript_SailToSkyrift\n\tcase 1, QoL_EventScript_SailToKanto\n'
                  '\tmsgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  '%(n)s_LanternportKanto::\n\tmsgbox QoL_Text_OfferKanto, MSGBOX_YESNO\n'
                  '\tgoto_if_eq VAR_RESULT, YES, QoL_EventScript_SailToKanto\n'
                  '\tmsgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                  'QoL_EventScript_PushSkyrift::\n\tgoto_if_unset %(chosen)s, Common_EventScript_NopReturn\n'
                  '\tdynmultipush %(n)s_Text_Skyrift, 7\n\treturn\n\n'
                  % dict(n=n, map=m.id, x=ax, y=ay, chosen=F['CHOSEN']))
    m.scripts += T(n + '_Text_Where', 'Where would you like to sail?')
    m.scripts += T(n + '_Text_Lanternport', 'LANTERNPORT')
    m.scripts += T(n + '_Text_Vermilion', 'VERMILION CITY')
    m.scripts += T(n + '_Text_Skyrift', 'SKYRIFT ISLES')


def sirius_gate(by_spot):
    m, x, y = by_spot['sirius_gate']
    n = P + '_Gate'
    lid = 'LOCALID_SKYRIFT_SIRIUS_GATE'
    m.objects.append(g.obj('OBJ_EVENT_GFX_COOLTRAINER_M', x, y - 2, n + '_EventScript_Sirius', local_id=lid,
                           flag=F['SIRIUS_GATE']))
    m.coords.append(g.coord(x, y, n + '_EventScript_Sirius', 'VAR_TEMP_4', 0))
    wrap_transition(m, '\tsetvar VAR_TEMP_4, 0\n\tcall_if_set %s, %s_Off\n' % (F['SIRIUS_GATE'], n),
                    n + '_OnTransition')
    m.scripts += ('%(n)s_Off::\n\tsetvar VAR_TEMP_4, 1\n\treturn\n\n'
                  '%(n)s_EventScript_Sirius::\n\tlockall\n\tsetvar VAR_TEMP_4, 1\n'
                  '\tapplymovement %(lid)s, Common_Movement_FaceDown\n\twaitmovement 0\n'
                  '\tplayse SE_PIN\n\tapplymovement %(lid)s, Common_Movement_ExclamationMark\n\twaitmovement 0\n'
                  '\tmsgbox %(n)s_Text_Intro, MSGBOX_DEFAULT\n'
                  '\ttrainerbattle_no_intro TRAINER_SKYRIFT_SIRIUS_GATE, %(n)s_Text_Defeat\n'
                  '\tmsgbox %(n)s_Text_After, MSGBOX_DEFAULT\n\tclosemessage\n'
                  '\tfadescreen FADE_TO_BLACK\n\tremoveobject %(lid)s\n\tsetflag %(fl)s\n\tfadescreen FADE_FROM_BLACK\n'
                  '\treleaseall\n\tend\n\n' % dict(n=n, lid=lid, fl=F['SIRIUS_GATE']))
    m.scripts += T(n + '_Text_Intro',
                   say('SIRIUS', 'Stop right there.\fI am SIRIUS. VEGA\'s orders are\nsimple: no one reaches the\nOBSERVATORY.\f'
                       'Not a CHAMPION from SEIKA,\nand not whoever walks\nbehind you.'))
    m.scripts += T(n + '_Text_Defeat', 'Tch. Fine.')
    m.scripts += T(n + '_Text_After',
                   say('SIRIUS', 'She\'s on the roof, starting the\nSKYRIFT ENGINE.\f'
                       'Come up if you must. I will\nbe waiting at her side.'))


def roof(by_spot):
    m, vx, vy = by_spot['vega']
    _, sx, sy = by_spot['sirius']
    _, tx, ty = by_spot['climax_trigger']
    n = P + '_Roof'
    m.objects.append(g.obj('OBJ_EVENT_GFX_WOMAN_2', vx, vy, '0x0', local_id='LOCALID_SKYRIFT_VEGA', flag=F['CLIMAX']))
    m.objects.append(g.obj('OBJ_EVENT_GFX_COOLTRAINER_M', sx, sy, '0x0', local_id='LOCALID_SKYRIFT_SIRIUS',
                           flag=F['CLIMAX']))
    m.objects.append(g.obj('OBJ_EVENT_GFX_WOMAN_2', vx, vy, n + '_EventScript_RematchVega',
                           local_id='LOCALID_SKYRIFT_REMATCH_VEGA', flag=F['HIDE_REMATCH_VEGA']))
    m.objects.append(g.obj('OBJ_EVENT_GFX_COOLTRAINER_M', sx, sy, n + '_EventScript_RematchSirius',
                           local_id='LOCALID_SKYRIFT_REMATCH_SIRIUS', flag=F['HIDE_REMATCH_SIRIUS']))
    for dx in (-1, 0, 1):
        m.coords.append(g.coord(tx + dx, ty, n + '_EventScript_Climax', 'VAR_TEMP_4', 0))
    wrap_transition(m, ('\tsetvar VAR_TEMP_4, 0\n\tcall_if_set %(c)s, %(n)s_Off\n'
                        '\tsetflag %(hv)s\n\tsetflag %(hs)s\n'
                        '\tcall_if_set %(saved)s, %(n)s_ShowVega\n\tcall_if_set %(lost)s, %(n)s_ShowSirius\n')
                    % dict(c=F['CLIMAX'], n=n, hv=F['HIDE_REMATCH_VEGA'], hs=F['HIDE_REMATCH_SIRIUS'],
                           saved=F['VEGA_SAVED'], lost=F['VEGA_LOST']), n + '_OnTransition')
    s = ('%(n)s_Off::\n\tsetvar VAR_TEMP_4, 1\n\treturn\n\n'
         '%(n)s_ShowVega::\n\tclearflag %(hv)s\n\treturn\n\n'
         '%(n)s_ShowSirius::\n\tclearflag %(hs)s\n\treturn\n\n'
         '%(n)s_EventScript_Climax::\n\tlockall\n\tsetvar VAR_TEMP_4, 1\n'
         '\tmsgbox %(n)s_Text_Vega1, MSGBOX_DEFAULT\n'
         + ''.join('\tcall_if_set %s, %s_Talk%s\n' % (F['WITH_' + k], n, k) for k, _, _, _ in COMPANIONS) +
         '\tmsgbox %(n)s_Text_Vega2, MSGBOX_DEFAULT\n\tmsgbox %(n)s_Text_Sirius1, MSGBOX_DEFAULT\n'
         + ''.join('\tgoto_if_set %s, %s_Battle%s\n' % (F['WITH_' + k], n, k) for k, _, _, _ in COMPANIONS) +
         '\ttrainerbattle_two_trainers TRAINER_SKYRIFT_VEGA, %(n)s_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, %(n)s_Text_SiriusDefeat\n'
         '\tgoto %(n)s_AfterBattle\n\n'
         % dict(n=n, hv=F['HIDE_REMATCH_VEGA'], hs=F['HIDE_REMATCH_SIRIUS']))
    for key, name, gfx, partner in COMPANIONS:
        s += ('%(n)s_Talk%(k)s::\n\tmsgbox %(n)s_Text_Talk%(k)s, MSGBOX_DEFAULT\n\treturn\n\n'
              '%(n)s_Battle%(k)s::\n\tmulti_fixed_2_vs_2 TRAINER_SKYRIFT_VEGA, %(n)s_Text_VegaDefeat, '
              'TRAINER_SKYRIFT_SIRIUS, %(n)s_Text_SiriusDefeat, %(partner)s\n\tgoto %(n)s_AfterBattle\n\n'
              % dict(n=n, k=key, partner=partner))
    s += ('%(n)s_AfterBattle::\n\tspecialvar VAR_RESULT, GetBattleOutcome\n'
          '\tgoto_if_ne VAR_RESULT, B_OUTCOME_WON, %(n)s_Lost\n'
          '\tmsgbox %(n)s_Text_Overload, MSGBOX_DEFAULT\n\tplayse SE_THUNDER\n'
          '\tsetvar VAR_0x8004, 3\n\tsetvar VAR_0x8005, 3\n\tsetvar VAR_0x8006, 20\n\tsetvar VAR_0x8007, 4\n'
          '\tspecial ShakeCamera\n\twaitstate\n'
          '\tmsgbox %(n)s_Text_Pulled, MSGBOX_DEFAULT\n\tmsgbox %(n)s_Text_Choice, MSGBOX_YESNO\n'
          '\tgoto_if_eq VAR_RESULT, NO, %(n)s_LetGo\n'
          # take her hand
          '\tsetflag %(saved)s\n\tcall_if_set %(peace)s, %(n)s_PeaceSave\n'
          '\tmsgbox %(n)s_Text_Saved, MSGBOX_DEFAULT\n'
          + ''.join('\tcall_if_set %s, %s_Saved%s\n' % (F['WITH_' + k], n, k) for k, _, _, _ in COMPANIONS) +
          '\tmsgbox %(n)s_Text_VegaGift, MSGBOX_DEFAULT\n'
          '\tgivemon SPECIES_POIPOLE, 60, ITEM_NONE, ITEM_BEAST_BALL\n'
          '\tbufferspeciesname STR_VAR_1, SPECIES_POIPOLE\n\tplayfanfare MUS_RG_OBTAIN_KEY_ITEM\n'
          '\tmessage %(n)s_Text_Received\n\twaitmessage\n\twaitfanfare\n\tgoto %(n)s_Epilogue\n\n'
          # let go
          '%(n)s_LetGo::\n\tsetflag %(lost)s\n\tmsgbox %(n)s_Text_Falls, MSGBOX_DEFAULT\n'
          '\tcall_if_set %(peace)s, %(n)s_PeaceLost\n'
          + ''.join('\tcall_if_set %s, %s_Lost%s\n' % (F['WITH_' + k], n, k) for k, _, _, _ in COMPANIONS) +
          '\tmsgbox %(n)s_Text_SiriusVow, MSGBOX_DEFAULT\n\tmsgbox %(n)s_Text_Cosmog, MSGBOX_DEFAULT\n'
          '\tgivemon SPECIES_COSMOG, 60, ITEM_NONE, ITEM_BEAST_BALL\n'
          '\tbufferspeciesname STR_VAR_1, SPECIES_COSMOG\n\tplayfanfare MUS_RG_OBTAIN_KEY_ITEM\n'
          '\tmessage %(n)s_Text_Received\n\twaitmessage\n\twaitfanfare\n\tgoto %(n)s_Epilogue\n\n'
          '%(n)s_PeaceSave::\n\tmsgbox %(n)s_Text_PeaceSave, MSGBOX_DEFAULT\n\treturn\n\n'
          '%(n)s_PeaceLost::\n\tmsgbox %(n)s_Text_PeaceLost, MSGBOX_DEFAULT\n\treturn\n\n'
          '%(n)s_Epilogue::\n\tsetflag %(climax)s\n\tfadescreen FADE_TO_BLACK\n'
          '\tremoveobject LOCALID_SKYRIFT_VEGA\n\tremoveobject LOCALID_SKYRIFT_SIRIUS\n'
          '\tfadescreen FADE_FROM_BLACK\n\tmsgbox %(n)s_Text_Keep, MSGBOX_YESNO\n'
          '\tgoto_if_eq VAR_RESULT, NO, %(n)s_Part\n'
          '\tmsgbox %(n)s_Text_KeepYes, MSGBOX_DEFAULT\n\tgoto %(n)s_TheEnd\n\n'
          '%(n)s_Part::\n\tmsgbox %(n)s_Text_KeepNo, MSGBOX_DEFAULT\n\tcall QoL_Skyrift_Inn_Clear\n'
          '%(n)s_TheEnd::\n\tplaybgm MUS_RG_CREDITS, FALSE\n\tmsgbox %(n)s_Text_TheEnd, MSGBOX_DEFAULT\n'
          '\tfadedefaultbgm\n\treleaseall\n\tend\n\n'
          '%(n)s_Lost::\n\treleaseall\n\tend\n\n'
          '%(n)s_EventScript_RematchVega::\n\tlock\n\tfaceplayer\n\tmsgbox %(n)s_Text_RematchVega, MSGBOX_YESNO\n'
          '\tgoto_if_eq VAR_RESULT, NO, %(n)s_RematchNo\n'
          '\ttrainerbattle_no_intro TRAINER_SKYRIFT_VEGA, %(n)s_Text_VegaDefeat\n\trelease\n\tend\n\n'
          '%(n)s_EventScript_RematchSirius::\n\tlock\n\tfaceplayer\n\tmsgbox %(n)s_Text_RematchSirius, MSGBOX_YESNO\n'
          '\tgoto_if_eq VAR_RESULT, NO, %(n)s_RematchNo\n'
          '\ttrainerbattle_no_intro TRAINER_SKYRIFT_SIRIUS, %(n)s_Text_SiriusDefeat\n\trelease\n\tend\n\n'
          '%(n)s_RematchNo::\n\trelease\n\tend\n\n'
          % dict(n=n, saved=F['VEGA_SAVED'], lost=F['VEGA_LOST'], peace=F['WISH_PEACE'], climax=F['CLIMAX']))
    for key, name, _, _ in COMPANIONS:
        s += '%(n)s_Saved%(k)s::\n\tmsgbox %(n)s_Text_Saved%(k)s, MSGBOX_DEFAULT\n\treturn\n\n' % dict(n=n, k=key)
        s += '%(n)s_Lost%(k)s::\n\tmsgbox %(n)s_Text_Lost%(k)s, MSGBOX_DEFAULT\n\treturn\n\n' % dict(n=n, k=key)
    m.scripts += s
    m.scripts += T(n + '_Text_Vega1',
                   say('VEGA', 'So the CHAMPION of two... no,\nthree regions came all this way.\f'
                       'And you brought a friend.'))
    m.scripts += T(n + '_Text_TalkALTAIR',
                   say('ALTAIR', 'VEGA, stop! The star stone\nwasn\'t for this!') + '\f' +
                   say('VEGA', 'ALTAIR. Always loyal.\nToo loyal to ask questions.'))
    m.scripts += T(n + '_Text_TalkDENEB',
                   say('DENEB', 'VEGA... I paid for every bolt of\nthis machine. Let me help you\nturn it off.') + '\f' +
                   say('VEGA', 'My clever DENEB. You never\nunderstood what we were\nbuying.'))
    m.scripts += T(n + '_Text_TalkMIRA',
                   say('MIRA', 'Grandpa says the sky answers\nanyone who listens. Not anyone\nwho shouts!') + '\f' +
                   say('VEGA', 'SEIJI\'s little star reader.\nHow sweet.'))
    m.scripts += T(n + '_Text_Vega2',
                   say('VEGA', 'The SKYRIFT ENGINE will call\nDEOXYS back.\f'
                       'And this time it will answer\nto me. SIRIUS!'))
    m.scripts += T(n + '_Text_Sirius1', say('SIRIUS', 'Ma\'am.'))
    m.scripts += T(n + '_Text_VegaDefeat', 'No... not again...')
    m.scripts += T(n + '_Text_SiriusDefeat', 'Forgive me, VEGA.')
    m.scripts += T(n + '_Text_Overload',
                   say('VEGA', 'The ENGINE! It\'s pulling too\nmuch power... it\'s overloading!'))
    m.scripts += T(n + '_Text_Pulled',
                   'The rift above the roof tears\nwider and pulls at VEGA!\f' +
                   say('VEGA', 'Help... me...!'))
    m.scripts += T(n + '_Text_Choice', 'Take VEGA\'s hand?')
    m.scripts += T(n + '_Text_PeaceSave',
                   'A warm light falls from the sky.\nThe WISHING STAR\'s light!\f'
                   'It wraps around VEGA and pulls\nher back with you.')
    m.scripts += T(n + '_Text_Saved',
                   'You grab VEGA\'s hand and pull!\fThe rift snaps shut above the\nroof. The ENGINE goes quiet.\f'
                   + say('VEGA', '...You saved me. Even after\neverything.'))
    m.scripts += T(n + '_Text_SavedALTAIR',
                   say('ALTAIR', 'Told you, VEGA. The kid\'s the\nreal thing.') + '\f' +
                   say('VEGA', 'Will you... help me take this\nmachine apart, ALTAIR?') + '\f' +
                   say('ALTAIR', 'Rock by rock, boss.'))
    m.scripts += T(n + '_Text_SavedDENEB',
                   say('DENEB', 'Oh, VEGA. You frightened me.') + '\f' +
                   say('VEGA', 'DENEB... sell it all. The\nengine, the island. Give the\nmoney to SEIKA\'s shrines.') + '\f' +
                   say('DENEB', 'Finally, a purchase I approve\nof.'))
    m.scripts += T(n + '_Text_SavedMIRA',
                   say('MIRA', 'You see? The sky DOES answer\nif you listen!') + '\f' +
                   say('VEGA', 'Perhaps I should have asked a\nstar reader before I bought\na telescope.'))
    m.scripts += T(n + '_Text_VegaGift',
                   say('VEGA', 'This little one came through\nthe rift with me. It wants to\ngo with you.'))
    m.scripts += T(n + '_Text_Received', '{PLAYER} received {STR_VAR_1}!')
    m.scripts += T(n + '_Text_Falls',
                   'You hesitate... and VEGA\'s hand\nslips away.\fThe rift swallows her and snaps\nshut.')
    m.scripts += T(n + '_Text_PeaceLost',
                   'As she fell, a warm light went\nwith her. The WISHING STAR\'s\nlight.\f'
                   'Wherever she has gone, she\nis not alone.')
    m.scripts += T(n + '_Text_LostALTAIR',
                   say('ALTAIR', '...Boss.\f...I\'ll keep digging. If there\'s\na way to her, I\'ll find it.'))
    m.scripts += T(n + '_Text_LostDENEB',
                   say('DENEB', 'She chose her sky.\fI suppose... I can stop paying\nfor it now.'))
    m.scripts += T(n + '_Text_LostMIRA',
                   say('MIRA', 'She\'s out there with the\nstars now.\fI\'ll find her on my chart one\nday. I promise.'))
    m.scripts += T(n + '_Text_SiriusVow',
                   say('SIRIUS', 'You let her go.\fI will find a way through that\nrift. I will wait here, on her\nroof, for as long as it takes.'))
    m.scripts += T(n + '_Text_Cosmog',
                   'Something small drifts down from\nwhere the rift closed...\fIt floats over to you.')
    m.scripts += T(n + '_Text_Keep',
                   'Your companion looks out at\nthe quiet sky.\f'
                   '"Will you keep traveling\nwith me?"')
    m.scripts += T(n + '_Text_KeepYes',
                   '"Then let\'s go! Wherever\nyou\'re headed next."')
    m.scripts += T(n + '_Text_KeepNo',
                   '"I understand. I\'ll stay at the\nRIFTWAKE INN. Come find me\nanytime."')
    m.scripts += T(n + '_Text_TheEnd',
                   'The SKYRIFT is closed.\nThe STARFALL is truly over.\f'
                   'STARFALL - THE END\fThank you for playing!')
    m.scripts += T(n + '_Text_RematchVega',
                   say('VEGA', 'I come up here to watch the\nstars now. Just watch.\f'
                       'Would you battle me? For old\ntimes\' sake.'))
    m.scripts += T(n + '_Text_RematchSirius',
                   say('SIRIUS', 'You again. Every night I wait\nfor the rift to open.\f'
                       'Battle me. It passes the time.'))


def fly():
    """Riftwake Harbor joins the Seika Fly map."""
    m = find(HARBOR)
    if not m:
        return
    flymap.FLAGS['RIFTWAKE'] = F['FLY_RIFTWAKE']
    label = m.name + '_QoLFly_OnTransition'
    wrap_transition(m, '\tsetflag %s\n' % F['FLY_RIFTWAKE'], label)
    flymap._ENTRIES.append(('RIFTWAKE', HARBOR, 'HEAL_LOCATION_SKYRIFT_RIFTWAKE', m.mapsec))
    # the isles on the Seika Fly map, east of Lanternport, harbor first
    import region_map as rm
    secs = {s['id']: s for s in c.MAPSECS}
    for map_id, x, y, kind, ja in FLY_GEO:
        mm = find(map_id)
        if not mm or mm.mapsec not in secs or any(s[0] == mm.mapsec for s in rm.SKYRIFT_SECTIONS):
            continue
        rm.SKYRIFT_SECTIONS.append((mm.mapsec, x, y, 1, 1, kind))
        secs[mm.mapsec].update(x=x, y=y, width=1, height=1, name_ja=ja)
