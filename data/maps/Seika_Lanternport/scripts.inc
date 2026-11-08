Seika_Lanternport_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, QoL_Skyrift_Pier_OnTransition
	.byte 0

Seika_Lanternport_EventScript_TownSign::
	msgbox Seika_Lanternport_Text_TownSign, MSGBOX_SIGN
	end

Seika_Lanternport_Text_TownSign::
	.string "LANTERNPORT\n"
	.string "Where lanterns light the way\l"
	.string "for falling stars.$"
	.string "{JPN}{JPN}トモシビタウン　おちる　ほしの　みちを\n"
	.string "ランタンが　てらす　みなと$"

Seika_Lanternport_EventScript_GymSign::
	msgbox Seika_Lanternport_Text_GymSign, MSGBOX_SIGN
	end

Seika_Lanternport_Text_GymSign::
	.string "LANTERNPORT POKéMON GYM\n"
	.string "LEADER: WICK\p"
	.string "The lantern maker whose\n"
	.string "flame never goes out!$"
	.string "{JPN}{JPN}トモシビ　ポケモンジム　リーダー　ウィック\p"
	.string "けっして　きえない　ほのおの\n"
	.string "ランタンしょくにん！$"

Seika_Lanternport_EventScript_HallSign::
	msgbox Seika_Lanternport_Text_HallSign, MSGBOX_SIGN
	end

Seika_Lanternport_Text_HallSign::
	.string "LANTERN HALL\n"
	.string "Starwatch Festival office$"
	.string "{JPN}{JPN}ランタンホール　ほしみまつり　じむきょく$"

Seika_Lanternport_EventScript_Elder::
	msgbox Seika_Lanternport_Text_Elder, MSGBOX_NPC
	end

Seika_Lanternport_Text_Elder::
	.string "Welcome to SEIKA, traveler.\p"
	.string "A thousand years ago, the first\n"
	.string "STARFALL came down over this land.\p"
	.string "Our ancestors wrote that one small\n"
	.string "star fell on MT. SKYREACH and\l"
	.string "went to sleep there.\p"
	.string "The WISHING STAR, they called it.\n"
	.string "It wakes for seven days every\l"
	.string "thousand years.\p"
	.string "We light lanterns so it can find\n"
	.string "its way. This year... the sky\l"
	.string "fell again. Perhaps it is awake.$"
	.string "{JPN}{JPN}ようこそ　セイカへ　たびの　かた。\p"
	.string "せんねんまえ　さいしょの　スターフォールが　この\n"
	.string "ちに　ふりそそいだ。\p"
	.string "ちいさな　ほしが　ひとつ　ソラトドキやまに\n"
	.string "おちて　ねむりに　ついたと　ごせんぞさまは\l"
	.string "かきのこして　おる。\p"
	.string "それを　ねがいぼしと　よんだ。　せんねんに\n"
	.string "いちど　7かかん　だけ　めを　さますのじゃ。\p"
	.string "ほしが　まよわぬよう　わしらは　ランタンを\n"
	.string "ともす。　ことし‥　また　そらが　ふった。　もう\l"
	.string "めざめて　おるのかも　しれんのう。$"

Seika_Lanternport_EventScript_LanternGirl::
	msgbox Seika_Lanternport_Text_LanternGirl, MSGBOX_NPC
	end

Seika_Lanternport_Text_LanternGirl::
	.string "Every night of the festival we\n"
	.string "send lanterns up into the sky!\p"
	.string "WICK makes all of them. His\n"
	.string "lanterns never, ever go out!$"
	.string "{JPN}{JPN}おまつりの　あいだ　まいばん　ランタンを　そらに\n"
	.string "とばすの！\p"
	.string "ぜんぶ　ウィックさんが　つくるの。　その\n"
	.string "ランタンは　ぜったいに　きえないんだよ！$"

Seika_Lanternport_EventScript_Fisher::
	msgbox Seika_Lanternport_Text_Fisher, MSGBOX_NPC
	end

Seika_Lanternport_Text_Fisher::
	.string "The CHINCHOU out here light up\n"
	.string "the sea at night.\p"
	.string "That's how this town got its\n"
	.string "name, if you ask me.$"
	.string "{JPN}{JPN}この　あたりの　チョンチーは　よるに　なると\n"
	.string "うみを　てらすんだ。\p"
	.string "この　まちの　なまえも　そこから　きたんじゃ\n"
	.string "ないかな。$"

Seika_Lanternport_EventScript_Woman::
	msgbox Seika_Lanternport_Text_Woman, MSGBOX_NPC
	end

Seika_Lanternport_Text_Woman::
	.string "LANTERN ROAD goes north to\n"
	.string "INKBRUSH TOWN.\p"
	.string "The scribes there keep the oldest\n"
	.string "record of the STARFALL.$"
	.string "{JPN}{JPN}トモシビかいどうを　きたへ　いくと\n"
	.string "スミフデタウンよ。\p"
	.string "あそこの　まきものかんには　スターフォールの\n"
	.string "いちばん　ふるい　きろくが　あるの。$"

Seika_Lanternport_EventScript_Sailor::
	goto_if_set FLAG_QOL_SKYRIFT_CHOSEN, QoL_EventScript_LanternportSailor @ skyrift_story.py
	lock
	faceplayer
	msgbox QoL_Text_OfferKanto, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, QoL_EventScript_SailToKanto
	msgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT
	release
	end

Seika_Lanternport_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_LANTERNPORT
	end

QoL_Skyrift_Pier_OnTransition::
	call QoL_Skyrift_Pier_EventScript_Setup
	goto Seika_Lanternport_QoLFly_OnTransition

QoL_Skyrift_Pier_EventScript_Setup::
	setflag FLAG_QOL_SKYRIFT_HIDE_TRIO
	setvar VAR_TEMP_4, 1
	call_if_set FLAG_QOL_SEIKA_DONE, QoL_Skyrift_Pier_EventScript_Open
	goto_if_unset FLAG_QOL_SKYRIFT_OPEN, Common_EventScript_NopReturn
	goto_if_set FLAG_QOL_SKYRIFT_CHOSEN, Common_EventScript_NopReturn
	clearflag FLAG_QOL_SKYRIFT_HIDE_TRIO
	setvar VAR_TEMP_4, 0
	return

QoL_Skyrift_Pier_EventScript_Open::
	setflag FLAG_QOL_SKYRIFT_OPEN
	return

QoL_Skyrift_Pier_EventScript_Talk::
	lockall
	setvar VAR_TEMP_4, 1
	applymovement LOCALID_SKYRIFT_PIER_MIRA, Common_Movement_FaceUp
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_SKYRIFT_PIER_MIRA, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox QoL_Skyrift_Pier_Text_Mira1, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Pier_Text_Altair1, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Pier_Text_Deneb1, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Pier_Text_Mira2, MSGBOX_DEFAULT
QoL_Skyrift_Pier_Choose::
	message QoL_Skyrift_Pier_Text_Choose
	waitmessage
	dynmultipush QoL_Skyrift_Pier_Text_OptAltair, 0
	dynmultipush QoL_Skyrift_Pier_Text_OptDeneb, 1
	dynmultipush QoL_Skyrift_Pier_Text_OptMira, 2
	dynmultipush QoL_Skyrift_Pier_Text_OptLater, 3
	dynmultistack 0, 0, FALSE, 4, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, QoL_Skyrift_Pier_PickALTAIR
	case 1, QoL_Skyrift_Pier_PickDENEB
	case 2, QoL_Skyrift_Pier_PickMIRA
	msgbox QoL_Skyrift_Pier_Text_Later, MSGBOX_DEFAULT
	releaseall
	end

QoL_Skyrift_Pier_PickALTAIR::
	msgbox QoL_Skyrift_Pier_Text_ConfirmALTAIR, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Pier_Choose
	setflag FLAG_QOL_SKYRIFT_CHOSEN
	setflag FLAG_QOL_SKYRIFT_WITH_ALTAIR
	msgbox QoL_Skyrift_Pier_Text_PickedALTAIR, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SKYRIFT_PIER_ALTAIR
	removeobject LOCALID_SKYRIFT_PIER_DENEB
	removeobject LOCALID_SKYRIFT_PIER_MIRA
	setflag FLAG_QOL_SKYRIFT_HIDE_TRIO
	fadescreen FADE_FROM_BLACK
	createfollowernpc OBJ_EVENT_GFX_HIKER_FRLG, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_ALTAIR
	msgbox QoL_Skyrift_Pier_Text_Ready, MSGBOX_DEFAULT
	releaseall
	end

QoL_Skyrift_Pier_PickDENEB::
	msgbox QoL_Skyrift_Pier_Text_ConfirmDENEB, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Pier_Choose
	setflag FLAG_QOL_SKYRIFT_CHOSEN
	setflag FLAG_QOL_SKYRIFT_WITH_DENEB
	msgbox QoL_Skyrift_Pier_Text_PickedDENEB, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SKYRIFT_PIER_ALTAIR
	removeobject LOCALID_SKYRIFT_PIER_DENEB
	removeobject LOCALID_SKYRIFT_PIER_MIRA
	setflag FLAG_QOL_SKYRIFT_HIDE_TRIO
	fadescreen FADE_FROM_BLACK
	createfollowernpc OBJ_EVENT_GFX_HEX_MANIAC, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_DENEB
	msgbox QoL_Skyrift_Pier_Text_Ready, MSGBOX_DEFAULT
	releaseall
	end

QoL_Skyrift_Pier_PickMIRA::
	msgbox QoL_Skyrift_Pier_Text_ConfirmMIRA, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Pier_Choose
	setflag FLAG_QOL_SKYRIFT_CHOSEN
	setflag FLAG_QOL_SKYRIFT_WITH_MIRA
	msgbox QoL_Skyrift_Pier_Text_PickedMIRA, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SKYRIFT_PIER_ALTAIR
	removeobject LOCALID_SKYRIFT_PIER_DENEB
	removeobject LOCALID_SKYRIFT_PIER_MIRA
	setflag FLAG_QOL_SKYRIFT_HIDE_TRIO
	fadescreen FADE_FROM_BLACK
	createfollowernpc OBJ_EVENT_GFX_LASS_FRLG, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_MIRA
	msgbox QoL_Skyrift_Pier_Text_Ready, MSGBOX_DEFAULT
	releaseall
	end

QoL_Skyrift_Pier_Text_Mira1::
	.string "MIRA: {PLAYER}! You came!\p"
	.string "I'm MIRA. Grandpa SEIJI says\n"
	.string "you're the new CHAMPION!\p"
	.string "I saw it from the SKY SHRINE.\n"
	.string "The sky tore open over the\l"
	.string "eastern sea!$"

QoL_Skyrift_Pier_Text_Altair1::
	.string "ALTAIR: ...And VEGA went through\n"
	.string "it.\p"
	.string "She's out on the SKYRIFT ISLES\n"
	.string "with what's left of TEAM NOVA.$"

QoL_Skyrift_Pier_Text_Deneb1::
	.string "DENEB: We can't reach her alone,\n"
	.string "darling.\p"
	.string "Her bodyguard SIRIUS won't even\n"
	.string "let us near the ISLES.$"

QoL_Skyrift_Pier_Text_Mira2::
	.string "MIRA: Only two can make the\n"
	.string "crossing on a sky this wild.\p"
	.string "So... take one of us with you!$"

QoL_Skyrift_Pier_Text_Choose::
	.string "Who will go with you to the\n"
	.string "SKYRIFT ISLES?$"

QoL_Skyrift_Pier_Text_OptAltair::
	.string "ALTAIR$"

QoL_Skyrift_Pier_Text_OptDeneb::
	.string "DENEB$"

QoL_Skyrift_Pier_Text_OptMira::
	.string "MIRA$"

QoL_Skyrift_Pier_Text_OptLater::
	.string "NOT YET$"

QoL_Skyrift_Pier_Text_Later::
	.string "MIRA: We'll wait right here. Don't\n"
	.string "take too long!$"

QoL_Skyrift_Pier_Text_ConfirmALTAIR::
	.string "ALTAIR battles with GROUND, ROCK\n"
	.string "and STEEL POKéMON.\p"
	.string "Take ALTAIR with you?$"

QoL_Skyrift_Pier_Text_ConfirmDENEB::
	.string "DENEB battles with PSYCHIC and\n"
	.string "FAIRY POKéMON.\p"
	.string "Take DENEB with you?$"

QoL_Skyrift_Pier_Text_ConfirmMIRA::
	.string "MIRA battles with PSYCHIC, FAIRY\n"
	.string "and WATER POKéMON.\p"
	.string "Take MIRA with you?$"

QoL_Skyrift_Pier_Text_PickedALTAIR::
	.string "ALTAIR: Ha! Didn't think you'd pick\n"
	.string "the guy who tried to stop you.\p"
	.string "I dug up that star stone. I owe\n"
	.string "VEGA the truth. Let's go.\p"
	.string "DENEB: Bring her home, you lump.$"

QoL_Skyrift_Pier_Text_PickedDENEB::
	.string "DENEB: Me? How flattering, darling.\p"
	.string "I paid for every bolt of VEGA's\n"
	.string "machines. Let me help switch\l"
	.string "them off.\p"
	.string "ALTAIR: Don't let her charm you,\n"
	.string "kid.$"

QoL_Skyrift_Pier_Text_PickedMIRA::
	.string "MIRA: Really?! I won't let you\n"
	.string "down!\p"
	.string "Grandpa taught me to read the\n"
	.string "stars. Now I get to see one up\l"
	.string "close!\p"
	.string "ALTAIR: ...Keep her safe.$"

QoL_Skyrift_Pier_Text_Ready::
	.string "Your companion will travel and\n"
	.string "battle beside you.\p"
	.string "The S.S. TIDAL can sail you both\n"
	.string "to the SKYRIFT ISLES.$"

QoL_Skyrift_Chat_Text_ALTAIRAfterA::
	.string "ALTAIR: Whatever happens next, I'm\n"
	.string "done digging for other people.$"

QoL_Skyrift_Chat_Text_ALTAIRAfterB::
	.string "ALTAIR: Thanks, kid. Really. Don't\n"
	.string "make me say it twice.$"

QoL_Skyrift_Chat_Text_ALTAIRRoofA::
	.string "ALTAIR: SIRIUS was always her\n"
	.string "shadow. Never liked him. Never had\l"
	.string "to.$"

QoL_Skyrift_Chat_Text_ALTAIRRoofB::
	.string "ALTAIR: VEGA's up there. I can feel\n"
	.string "the ground humming. Stay close.$"

QoL_Skyrift_Chat_Text_ALTAIRIslesA::
	.string "ALTAIR: These shards... they're the\n"
	.string "same stuff as the star stone. The\l"
	.string "whole beach is made of sky.$"

QoL_Skyrift_Chat_Text_ALTAIRIslesB::
	.string "ALTAIR: Ultra Beasts in the rifts.\n"
	.string "VEGA always said the sky had more\l"
	.string "in it than DEOXYS.$"

QoL_Skyrift_Chat_Text_ALTAIRStartA::
	.string "ALTAIR: I haven't been on a boat\n"
	.string "since KANTO. My stomach remembers.$"

QoL_Skyrift_Chat_Text_ALTAIRStartB::
	.string "ALTAIR: You and me, eh? Strange\n"
	.string "world. Last month I was digging\l"
	.string "rocks to stop you.$"

QoL_Skyrift_Chat_Text_DENEBAfterA::
	.string "DENEB: You know, darling, I think\n"
	.string "I'll keep this hat.$"

QoL_Skyrift_Chat_Text_DENEBAfterB::
	.string "DENEB: Strange... I don't miss the\n"
	.string "money at all.$"

QoL_Skyrift_Chat_Text_DENEBRoofA::
	.string "DENEB: SIRIUS hates me. I signed\n"
	.string "his paychecks. It's complicated.$"

QoL_Skyrift_Chat_Text_DENEBRoofB::
	.string "DENEB: If we turn that engine off,\n"
	.string "I'm finally out of work. What a\l"
	.string "relief.$"

QoL_Skyrift_Chat_Text_DENEBIslesA::
	.string "DENEB: Rifts, shards, beasts from\n"
	.string "another sky... VEGA spent a fortune\l"
	.string "on this place.$"

QoL_Skyrift_Chat_Text_DENEBIslesB::
	.string "DENEB: DR. HALLEY used to work for\n"
	.string "us. He quit the day VEGA bought the\l"
	.string "engine.$"

QoL_Skyrift_Chat_Text_DENEBStartA::
	.string "DENEB: Do you like my hat? I chose\n"
	.string "it for the crossing. Priorities,\l"
	.string "darling.$"

QoL_Skyrift_Chat_Text_DENEBStartB::
	.string "DENEB: VEGA once said I counted\n"
	.string "money like other people count\l"
	.string "stars. She wasn't wrong.$"

QoL_Skyrift_Chat_Text_MIRAAfterA::
	.string "MIRA: Wait until Grandpa hears!\n"
	.string "He'll fall right off his throne!$"

QoL_Skyrift_Chat_Text_MIRAAfterB::
	.string "MIRA: I'm going to draw a new star\n"
	.string "chart. You're on it!$"

QoL_Skyrift_Chat_Text_MIRARoofA::
	.string "MIRA: VEGA, DENEB, ALTAIR...\n"
	.string "they're all named after stars. Like\l"
	.string "a constellation that went wrong.$"

QoL_Skyrift_Chat_Text_MIRARoofB::
	.string "MIRA: I'm a little scared. But I'm\n"
	.string "more excited! Is that weird?$"

QoL_Skyrift_Chat_Text_MIRAIslesA::
	.string "MIRA: Look at the sky through the\n"
	.string "rift! Those stars aren't any I\l"
	.string "know!$"

QoL_Skyrift_Chat_Text_MIRAIslesB::
	.string "MIRA: Ultra Beasts! I've only seen\n"
	.string "them in Grandpa's oldest books.$"

QoL_Skyrift_Chat_Text_MIRAStartA::
	.string "MIRA: Grandpa gave me his old star\n"
	.string "chart. I can read every\l"
	.string "constellation!$"

QoL_Skyrift_Chat_Text_MIRAStartB::
	.string "MIRA: Do you think the WISHING STAR\n"
	.string "will come back in another thousand\l"
	.string "years?$"

QoL_EventScript_CompanionTalk::
	lock
	facefollowernpc
	goto_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Chat_ALTAIR
	goto_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Chat_DENEB
	goto_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Chat_MIRA
	release
	end

QoL_Skyrift_Chat_ALTAIR::
	goto_if_set FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Chat_ALTAIRAfter
	goto_if_set FLAG_QOL_SKYRIFT_SIRIUS_GATE, QoL_Skyrift_Chat_ALTAIRRoof
	goto_if_set FLAG_QOL_SKYRIFT_HALLEY, QoL_Skyrift_Chat_ALTAIRIsles
	goto QoL_Skyrift_Chat_ALTAIRStart

QoL_Skyrift_Chat_ALTAIRAfter::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_ALTAIRAfterB
	msgbox QoL_Skyrift_Chat_Text_ALTAIRAfterA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_ALTAIRAfterB::
	msgbox QoL_Skyrift_Chat_Text_ALTAIRAfterB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_ALTAIRRoof::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_ALTAIRRoofB
	msgbox QoL_Skyrift_Chat_Text_ALTAIRRoofA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_ALTAIRRoofB::
	msgbox QoL_Skyrift_Chat_Text_ALTAIRRoofB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_ALTAIRIsles::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_ALTAIRIslesB
	msgbox QoL_Skyrift_Chat_Text_ALTAIRIslesA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_ALTAIRIslesB::
	msgbox QoL_Skyrift_Chat_Text_ALTAIRIslesB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_ALTAIRStart::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_ALTAIRStartB
	msgbox QoL_Skyrift_Chat_Text_ALTAIRStartA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_ALTAIRStartB::
	msgbox QoL_Skyrift_Chat_Text_ALTAIRStartB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEB::
	goto_if_set FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Chat_DENEBAfter
	goto_if_set FLAG_QOL_SKYRIFT_SIRIUS_GATE, QoL_Skyrift_Chat_DENEBRoof
	goto_if_set FLAG_QOL_SKYRIFT_HALLEY, QoL_Skyrift_Chat_DENEBIsles
	goto QoL_Skyrift_Chat_DENEBStart

QoL_Skyrift_Chat_DENEBAfter::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_DENEBAfterB
	msgbox QoL_Skyrift_Chat_Text_DENEBAfterA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEBAfterB::
	msgbox QoL_Skyrift_Chat_Text_DENEBAfterB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEBRoof::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_DENEBRoofB
	msgbox QoL_Skyrift_Chat_Text_DENEBRoofA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEBRoofB::
	msgbox QoL_Skyrift_Chat_Text_DENEBRoofB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEBIsles::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_DENEBIslesB
	msgbox QoL_Skyrift_Chat_Text_DENEBIslesA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEBIslesB::
	msgbox QoL_Skyrift_Chat_Text_DENEBIslesB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEBStart::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_DENEBStartB
	msgbox QoL_Skyrift_Chat_Text_DENEBStartA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_DENEBStartB::
	msgbox QoL_Skyrift_Chat_Text_DENEBStartB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRA::
	goto_if_set FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Chat_MIRAAfter
	goto_if_set FLAG_QOL_SKYRIFT_SIRIUS_GATE, QoL_Skyrift_Chat_MIRARoof
	goto_if_set FLAG_QOL_SKYRIFT_HALLEY, QoL_Skyrift_Chat_MIRAIsles
	goto QoL_Skyrift_Chat_MIRAStart

QoL_Skyrift_Chat_MIRAAfter::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_MIRAAfterB
	msgbox QoL_Skyrift_Chat_Text_MIRAAfterA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRAAfterB::
	msgbox QoL_Skyrift_Chat_Text_MIRAAfterB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRARoof::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_MIRARoofB
	msgbox QoL_Skyrift_Chat_Text_MIRARoofA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRARoofB::
	msgbox QoL_Skyrift_Chat_Text_MIRARoofB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRAIsles::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_MIRAIslesB
	msgbox QoL_Skyrift_Chat_Text_MIRAIslesA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRAIslesB::
	msgbox QoL_Skyrift_Chat_Text_MIRAIslesB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRAStart::
	random 2
	goto_if_eq VAR_RESULT, 1, QoL_Skyrift_Chat_MIRAStartB
	msgbox QoL_Skyrift_Chat_Text_MIRAStartA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Chat_MIRAStartB::
	msgbox QoL_Skyrift_Chat_Text_MIRAStartB, MSGBOX_DEFAULT
	release
	end
