Skyrift_MeteorBasin_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, QoL_Skyrift_Necrozma_OnTransition
	.byte 0

Skyrift_MeteorBasin_EventScript_Sign::
	msgbox Skyrift_MeteorBasin_Text_Sign, MSGBOX_SIGN
	end

Skyrift_MeteorBasin_Text_Sign::
	.string "METEOR BASIN\p"
	.string "Where the brightest star of the\n"
	.string "second STARFALL came down.$"

Skyrift_MeteorBasin_EventScript_Meteorite::
	msgbox Skyrift_MeteorBasin_Text_Meteorite, MSGBOX_SIGN
	end

Skyrift_MeteorBasin_Text_Meteorite::
	.string "A huge meteorite, half sunk in\n"
	.string "the crater floor.\p"
	.string "It is still warm. Something deep\n"
	.string "inside it hums, very softly.$"

Skyrift_MeteorBasin_EventScript_ItemCometShard::
	finditem ITEM_COMET_SHARD
	end

Skyrift_MeteorBasin_EventScript_Makoa::
	trainerbattle_single TRAINER_SKYRIFT_HIKER_MAKOA, Skyrift_MeteorBasin_Text_MakoaIntro, Skyrift_MeteorBasin_Text_MakoaDefeat
	msgbox Skyrift_MeteorBasin_Text_MakoaPost, MSGBOX_AUTOCLOSE
	end

Skyrift_MeteorBasin_Text_MakoaIntro::
	.string "I climbed down to see the big\n"
	.string "rock. Now I can't stop staring!$"

Skyrift_MeteorBasin_Text_MakoaDefeat::
	.string "Rockslide! On me!$"

Skyrift_MeteorBasin_Text_MakoaPost::
	.string "The steps cut through the south\n"
	.string "rim. The ledges are a one-way\l"
	.string "trip down!$"

Skyrift_MeteorBasin_EventScript_Hoku::
	trainerbattle_single TRAINER_SKYRIFT_RUIN_MANIAC_HOKU, Skyrift_MeteorBasin_Text_HokuIntro, Skyrift_MeteorBasin_Text_HokuDefeat
	msgbox Skyrift_MeteorBasin_Text_HokuPost, MSGBOX_AUTOCLOSE
	end

Skyrift_MeteorBasin_Text_HokuIntro::
	.string "This meteorite is not from our\n"
	.string "sky. I've measured it!$"

Skyrift_MeteorBasin_Text_HokuDefeat::
	.string "You hit harder than a meteor!$"

Skyrift_MeteorBasin_Text_HokuPost::
	.string "Sometimes the light inside it\n"
	.string "flickers, like it's hungry.\p"
	.string "For light, maybe?$"

Skyrift_MeteorBasin_EventScript_NovaA::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_BASIN_1, Skyrift_MeteorBasin_Text_NovaAIntro, Skyrift_MeteorBasin_Text_NovaADefeat
	msgbox Skyrift_MeteorBasin_Text_NovaAPost, MSGBOX_AUTOCLOSE
	end

Skyrift_MeteorBasin_Text_NovaAIntro::
	.string "The crater is sacred ground.\n"
	.string "The second STARFALL hit here!$"

Skyrift_MeteorBasin_Text_NovaADefeat::
	.string "The crater... rejects me?$"

Skyrift_MeteorBasin_Text_NovaAPost::
	.string "VEGA studied this crater for\n"
	.string "months. Then she built the\l"
	.string "ENGINE.$"

Skyrift_MeteorBasin_EventScript_NovaB::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_BASIN_2, Skyrift_MeteorBasin_Text_NovaBIntro, Skyrift_MeteorBasin_Text_NovaBDefeat
	msgbox Skyrift_MeteorBasin_Text_NovaBPost, MSGBOX_AUTOCLOSE
	end

Skyrift_MeteorBasin_Text_NovaBIntro::
	.string "You and your friend won't pass\n"
	.string "the crystal gate!$"

Skyrift_MeteorBasin_Text_NovaBDefeat::
	.string "The gate is open... Go, then.$"

Skyrift_MeteorBasin_Text_NovaBPost::
	.string "North of here are the cliffs,\n"
	.string "and the OBSERVATORY on top.$"

QoL_Skyrift_EventScript_RiftBUZZWOLE::
	lock
	faceplayer
	msgbox QoL_Skyrift_Text_RiftHum, MSGBOX_DEFAULT
	playmoncry SPECIES_BUZZWOLE, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_BUZZWOLE, 75
	settotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, QoL_Skyrift_RiftBUZZWOLE_Caught
	msgbox QoL_Skyrift_Text_RiftBack, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_RiftBUZZWOLE_Caught::
	setflag FLAG_QOL_SKYRIFT_UB_BUZZWOLE
	removeobject LOCALID_SKYRIFT_RIFT_BUZZWOLE
	msgbox QoL_Skyrift_Text_RiftClosed, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_EventScript_RiftPHEROMOSA::
	lock
	faceplayer
	msgbox QoL_Skyrift_Text_RiftHum, MSGBOX_DEFAULT
	playmoncry SPECIES_PHEROMOSA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_PHEROMOSA, 75
	settotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, QoL_Skyrift_RiftPHEROMOSA_Caught
	msgbox QoL_Skyrift_Text_RiftBack, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_RiftPHEROMOSA_Caught::
	setflag FLAG_QOL_SKYRIFT_UB_PHEROMOSA
	removeobject LOCALID_SKYRIFT_RIFT_PHEROMOSA
	msgbox QoL_Skyrift_Text_RiftClosed, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Necrozma_OnTransition::
	call QoL_Skyrift_Necrozma_Check
	end

QoL_Skyrift_Necrozma_Check::
	setflag FLAG_QOL_SKYRIFT_HIDE_NECROZMA
	goto_if_unset FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Necrozma_Shut
	goto_if_set FLAG_QOL_SKYRIFT_NECROZMA, QoL_Skyrift_Necrozma_Shut
	goto_if_unset FLAG_QOL_SKYRIFT_UB_NIHILEGO, QoL_Skyrift_Necrozma_Shut
	goto_if_unset FLAG_QOL_SKYRIFT_UB_BUZZWOLE, QoL_Skyrift_Necrozma_Shut
	goto_if_unset FLAG_QOL_SKYRIFT_UB_PHEROMOSA, QoL_Skyrift_Necrozma_Shut
	goto_if_unset FLAG_QOL_SKYRIFT_UB_XURKITREE, QoL_Skyrift_Necrozma_Shut
	goto_if_unset FLAG_QOL_SKYRIFT_UB_KARTANA, QoL_Skyrift_Necrozma_Shut
	clearflag FLAG_QOL_SKYRIFT_HIDE_NECROZMA
QoL_Skyrift_Necrozma_Shut::
	return

QoL_Skyrift_Necrozma_EventScript::
	lock
	faceplayer
	msgbox QoL_Skyrift_Necrozma_Text_Seen, MSGBOX_DEFAULT
	playmoncry SPECIES_NECROZMA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_NECROZMA, 80
	settotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, QoL_Skyrift_Necrozma_Caught
	msgbox QoL_Skyrift_Necrozma_Text_Back, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Necrozma_Caught::
	setflag FLAG_QOL_SKYRIFT_NECROZMA
	setflag FLAG_QOL_SKYRIFT_HIDE_NECROZMA
	removeobject LOCALID_SKYRIFT_NECROZMA
	msgbox QoL_Skyrift_Necrozma_Text_Closed, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Necrozma_Text_Seen::
	.string "Light is pouring into the\n"
	.string "sixth rift... and something\l"
	.string "inside is drinking it!\p"
	.string "A prism-black POKéMON, starved\n"
	.string "for light, crawls out of the\l"
	.string "tear!$"

QoL_Skyrift_Necrozma_Text_Back::
	.string "It pulled back into the dark\n"
	.string "of the rift... It's still there.$"

QoL_Skyrift_Necrozma_Text_Closed::
	.string "The sixth rift flickered\n"
	.string "and went out.$"
