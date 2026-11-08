Seika_Frostmere_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Frostmere_QoLFly_OnTransition
	.byte 0

Seika_Frostmere_EventScript_TownSign::
	msgbox Seika_Frostmere_Text_TownSign, MSGBOX_SIGN
	end

Seika_Frostmere_Text_TownSign::
	.string "FROSTMERE VILLAGE\n"
	.string "Home of the coldest lake\l"
	.string "in SEIKA.$"

Seika_Frostmere_EventScript_GymSign::
	msgbox Seika_Frostmere_Text_GymSign, MSGBOX_SIGN
	end

Seika_Frostmere_Text_GymSign::
	.string "FROSTMERE POKéMON GYM\n"
	.string "LEADER: RIME\p"
	.string "The ice fisher who waits out\n"
	.string "the longest winter!$"

Seika_Frostmere_EventScript_Fisher::
	msgbox Seika_Frostmere_Text_Fisher, MSGBOX_NPC
	end

Seika_Frostmere_Text_Fisher::
	.string "In winter we cut holes in the ice\n"
	.string "and fish all night long.\p"
	.string "RIME can sit by one hole for a\n"
	.string "whole day without moving!$"

Seika_Frostmere_EventScript_OldWoman::
	msgbox Seika_Frostmere_Text_OldWoman, MSGBOX_NPC
	end

Seika_Frostmere_Text_OldWoman::
	.string "Strange folk in dark coats came\n"
	.string "through here last night.\p"
	.string "They were hauling something big,\n"
	.string "wrapped in cloth. Up north.$"

Seika_Frostmere_EventScript_Boy::
	msgbox Seika_Frostmere_Text_Boy, MSGBOX_NPC
	end

Seika_Frostmere_Text_Boy::
	.string "I saw a LAPRAS in the lake once!\n"
	.string "It was singing to the snow!$"

Seika_Frostmere_EventScript_Grunts::
	lock
	faceplayer
	playbgm MUS_ENCOUNTER_INTENSE, FALSE
	msgbox Seika_Frostmere_Text_GruntsTalk, MSGBOX_DEFAULT
	goto_if_defeated TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_1, Seika_Frostmere_EventScript_Grunt2
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_1, Seika_Frostmere_Text_Grunt1Defeat
	goto Seika_Frostmere_EventScript_Grunt2
	end

Seika_Frostmere_EventScript_Grunt2::
	msgbox Seika_Frostmere_Text_Grunt2Intro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_2, Seika_Frostmere_Text_Grunt2Defeat
	msgbox Seika_Frostmere_Text_GruntsAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_FROSTMERE_GRUNT1
	removeobject LOCALID_SEIKA_FROSTMERE_GRUNT2
	setflag FLAG_QOL_SEIKA_HIDE_GRUNTS_FROSTMERE
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	release
	end

Seika_Frostmere_Text_GruntsTalk::
	.string "...So we haul the bell all the\n"
	.string "way north? To the mountain?\p"
	.string "Orders are orders. The bell goes\n"
	.string "up MT. SKYREACH, and we-\p"
	.string "Huh? How long has this kid been\n"
	.string "standing there?\p"
	.string "You didn't hear a thing! Nothing\n"
	.string "about any bell!$"

Seika_Frostmere_Text_Grunt1Defeat::
	.string "Ugh! Not in the snow!$"

Seika_Frostmere_Text_Grunt2Intro::
	.string "My turn! I'll freeze you out!$"

Seika_Frostmere_Text_Grunt2Defeat::
	.string "Beaten by a kid... again?$"

Seika_Frostmere_Text_GruntsAfter::
	.string "Forget it! The bell's already on\n"
	.string "its way up the mountain.\p"
	.string "DENEB and ALTAIR will have it\n"
	.string "ringing before you get there!\p"
	.string "Come on, let's go!$"

Seika_Frostmere_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_FROSTMERE
	end
