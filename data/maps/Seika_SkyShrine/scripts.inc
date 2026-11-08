Seika_SkyShrine_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_SkyShrine_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, Seika_SkyShrine_OnResume
	.byte 0

Seika_SkyShrine_OnTransition::
	call_if_unset FLAG_QOL_SEIKA_SHRINE_NOVA, Seika_SkyShrine_EventScript_HideStar
	call_if_set FLAG_QOL_SEIKA_DONE, Seika_SkyShrine_EventScript_HideAll
	call_if_set FLAG_QOL_SEIKA_SHRINE_NOVA, Seika_SkyShrine_EventScript_SkipConfront
	end

Seika_SkyShrine_EventScript_HideStar::
	setflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	return

Seika_SkyShrine_EventScript_HideAll::
	setflag FLAG_QOL_SEIKA_HIDE_SHRINE_NOVA
	setflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	setvar VAR_TEMP_2, 1
	return

Seika_SkyShrine_EventScript_SkipConfront::
	setvar VAR_TEMP_2, 1
	return

Seika_SkyShrine_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, Seika_SkyShrine_EventScript_TryRemoveJirachi
	end

Seika_SkyShrine_EventScript_TryRemoveJirachi::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_SEIKA_SHRINE_JIRACHI
	return

Seika_SkyShrine_EventScript_Confront::
	lockall
	setvar VAR_TEMP_2, 1
	msgbox Seika_SkyShrine_Text_BellRings, MSGBOX_DEFAULT
	applymovement LOCALID_SEIKA_SHRINE_DENEB, Common_Movement_FaceDown
	applymovement LOCALID_SEIKA_SHRINE_ALTAIR, Common_Movement_FaceDown
	waitmovement 0
	msgbox Seika_SkyShrine_Text_DenebIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_DENEB_SHRINE, Seika_SkyShrine_Text_DenebDefeat
	msgbox Seika_SkyShrine_Text_AltairIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_ALTAIR_SHRINE, Seika_SkyShrine_Text_AltairDefeat
	setflag FLAG_QOL_SEIKA_SHRINE_NOVA
	msgbox Seika_SkyShrine_Text_NovaLeave, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_SHRINE_DENEB
	removeobject LOCALID_SEIKA_SHRINE_ALTAIR
	setflag FLAG_QOL_SEIKA_HIDE_SHRINE_NOVA
	fadescreen FADE_FROM_BLACK
	msgbox Seika_SkyShrine_Text_StarComes, MSGBOX_DEFAULT
	closemessage
	playse SE_M_DETECT
	clearflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	addobject LOCALID_SEIKA_SHRINE_JIRACHI
	delay 30
	releaseall
	end

Seika_SkyShrine_EventScript_Deneb::
	msgbox Seika_SkyShrine_Text_DenebIntro, MSGBOX_NPC
	end

Seika_SkyShrine_EventScript_Altair::
	msgbox Seika_SkyShrine_Text_AltairIntro, MSGBOX_NPC
	end

Seika_SkyShrine_EventScript_Jirachi::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_JIRACHI, CRY_MODE_ENCOUNTER
	msgbox Seika_SkyShrine_Text_Jirachi, MSGBOX_DEFAULT
	waitmoncry
	setwildbattle SPECIES_JIRACHI, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Seika_SkyShrine_EventScript_Ending
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Seika_SkyShrine_EventScript_StarFlewHome
	msgbox Seika_SkyShrine_Text_StillHere, MSGBOX_DEFAULT
	release
	end

Seika_SkyShrine_EventScript_StarFlewHome::
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_SHRINE_JIRACHI
	fadescreen FADE_FROM_BLACK
	msgbox Seika_SkyShrine_Text_FlewHome, MSGBOX_DEFAULT
Seika_SkyShrine_EventScript_Ending::
	setflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	setflag FLAG_QOL_SEIKA_DONE
	msgbox Seika_SkyShrine_Text_Ending, MSGBOX_DEFAULT
	release
	end

Seika_SkyShrine_Text_BellRings::
	.string "A clear note rings out over the\n"
	.string "clouds. The STARGLASS BELL!$"

Seika_SkyShrine_Text_DenebIntro::
	.string "DENEB: Darling, you are late.\n"
	.string "The bell is ringing.\p"
	.string "When the WISHING STAR comes down,\n"
	.string "our wish brings VEGA back to\l"
	.string "lead us. Stay out of it!{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}$"

Seika_SkyShrine_Text_DenebDefeat::
	.string "Not again... Not on the last\n"
	.string "day!$"

Seika_SkyShrine_Text_AltairIntro::
	.string "ALTAIR: I dug that star stone\n"
	.string "out of the ground with my hands.\p"
	.string "I won't let a kid from KANTO\n"
	.string "waste it. Come on!{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}$"

Seika_SkyShrine_Text_AltairDefeat::
	.string "The bell... it stopped for\n"
	.string "you, not for us.$"

Seika_SkyShrine_Text_NovaLeave::
	.string "DENEB: The star turned away from\n"
	.string "us. It is listening to you.\p"
	.string "ALTAIR: Let's go, DENEB.\n"
	.string "There is nothing left for us\l"
	.string "here.$"

Seika_SkyShrine_Text_StarComes::
	.string "The last note of the bell fades.\p"
	.string "A small light falls from the sky\n"
	.string "and lands on the shrine...$"

Seika_SkyShrine_Text_Jirachi::
	.string "Jiraaa!$"

Seika_SkyShrine_Text_StillHere::
	.string "The WISHING STAR is still\n"
	.string "floating above the shrine...$"

Seika_SkyShrine_Text_FlewHome::
	.string "The WISHING STAR rose into the\n"
	.string "sky and was gone.\p"
	.string "But it looked back at you, just\n"
	.string "once, before it went.$"

Seika_SkyShrine_Text_Ending::
	.string "Far below, the lanterns of\n"
	.string "SEIKA glow in the night.\p"
	.string "The second STARFALL is over.\n"
	.string "Its wish, whatever it was, is\l"
	.string "yours to keep.$"
