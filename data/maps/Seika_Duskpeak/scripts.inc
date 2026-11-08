Seika_Duskpeak_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Duskpeak_QoLFly_OnTransition
	.byte 0

Seika_Duskpeak_EventScript_TownSign::
	msgbox Seika_Duskpeak_Text_TownSign, MSGBOX_SIGN
	end

Seika_Duskpeak_Text_TownSign::
	.string "DUSKPEAK VILLAGE\n"
	.string "The last lantern below the peak$"

Seika_Duskpeak_EventScript_GymSign::
	msgbox Seika_Duskpeak_Text_GymSign, MSGBOX_SIGN
	end

Seika_Duskpeak_Text_GymSign::
	.string "DUSKPEAK POKéMON GYM\n"
	.string "LEADER: TOMOE\p"
	.string "The shrine medium who hears\n"
	.string "the voices of the past!$"

Seika_Duskpeak_EventScript_PondSign::
	msgbox Seika_Duskpeak_Text_PondSign, MSGBOX_SIGN
	end

Seika_Duskpeak_Text_PondSign::
	.string "KEEPERS' POND\p"
	.string "For the shrine keepers, who\n"
	.string "watched the sky for us.$"

Seika_Duskpeak_EventScript_Channeler::
	msgbox Seika_Duskpeak_Text_Channeler, MSGBOX_NPC
	end

Seika_Duskpeak_Text_Channeler::
	.string "Shh... Can you hear them?\p"
	.string "The MISDREAVUS sing at dusk,\n"
	.string "when the sun slips behind\l"
	.string "MT. SKYREACH.\p"
	.string "Here, it is always nearly dusk.$"

Seika_Duskpeak_EventScript_OldWoman::
	msgbox Seika_Duskpeak_Text_OldWoman, MSGBOX_NPC
	end

Seika_Duskpeak_Text_OldWoman::
	.string "Eight towns, eight shrines, eight\n"
	.string "keepers. For a thousand years\l"
	.string "they watched the sky.\p"
	.string "The pond keeps their faces.\n"
	.string "On still nights you can see them.\p"
	.string "TOMOE is the last keeper. Her\n"
	.string "shrine is the nearest to the star.$"

Seika_Duskpeak_EventScript_Boy::
	msgbox Seika_Duskpeak_Text_Boy, MSGBOX_NPC
	end

Seika_Duskpeak_Text_Boy::
	.string "TOMOE can talk to ghosts!\p"
	.string "She says the old keepers whisper\n"
	.string "to her about the WISHING STAR.$"

Seika_Duskpeak_EventScript_Hiker::
	msgbox Seika_Duskpeak_Text_Hiker, MSGBOX_NPC
	end

Seika_Duskpeak_Text_Hiker::
	.string "North is SKYREACH PASS, the climb\n"
	.string "to STARWATCH PLATEAU.\p"
	.string "The guard lets nobody by without\n"
	.string "all eight SEIKA BADGES.$"

Seika_Duskpeak_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_DUSKPEAK
	end
