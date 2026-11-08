Seika_Tidewatch_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Tidewatch_QoLFly_OnTransition
	.byte 0

Seika_Tidewatch_EventScript_TownSign::
	msgbox Seika_Tidewatch_Text_TownSign, MSGBOX_SIGN
	end

Seika_Tidewatch_Text_TownSign::
	.string "TIDEWATCH TOWN\n"
	.string "The sea keeps time here.$"

Seika_Tidewatch_EventScript_GymSign::
	msgbox Seika_Tidewatch_Text_GymSign, MSGBOX_SIGN
	end

Seika_Tidewatch_Text_GymSign::
	.string "TIDEWATCH POKéMON GYM\n"
	.string "LEADER: NAGI\p"
	.string "The diver who swims with\n"
	.string "the tide!$"

Seika_Tidewatch_EventScript_ShrineSign::
	msgbox Seika_Tidewatch_Text_ShrineSign, MSGBOX_SIGN
	end

Seika_Tidewatch_Text_ShrineSign::
	.string "TIDE SHRINE\n"
	.string "The sea rises for the star.$"

Seika_Tidewatch_EventScript_FisherWest::
	msgbox Seika_Tidewatch_Text_FisherWest, MSGBOX_NPC
	end

Seika_Tidewatch_Text_FisherWest::
	.string "Everyone in TIDEWATCH fishes.\n"
	.string "Even the GYM LEADER!\p"
	.string "Well, NAGI dives for her catch.\n"
	.string "Same thing, really.$"

Seika_Tidewatch_EventScript_FisherEast::
	msgbox Seika_Tidewatch_Text_FisherEast, MSGBOX_NPC
	end

Seika_Tidewatch_Text_FisherEast::
	.string "At high tide the REMORAID come\n"
	.string "right up to the pier.\p"
	.string "At low tide, the OCTILLERY come\n"
	.string "after the REMORAID!$"

Seika_Tidewatch_EventScript_OldSailor::
	msgbox Seika_Tidewatch_Text_OldSailor, MSGBOX_NPC
	end

Seika_Tidewatch_Text_OldSailor::
	.string "The tide here never misses a\n"
	.string "beat. Twice a day, for a thousand\l"
	.string "years.\p"
	.string "Except one night, they say. The\n"
	.string "night the first star fell, the\l"
	.string "sea rose all the way to the hills.$"

Seika_Tidewatch_EventScript_Girl::
	msgbox Seika_Tidewatch_Text_Girl, MSGBOX_NPC
	end

Seika_Tidewatch_Text_Girl::
	.string "NAGI can hold her breath for ten\n"
	.string "whole minutes! I can do ten\l"
	.string "seconds.$"

Seika_Tidewatch_EventScript_NorthWoman::
	msgbox Seika_Tidewatch_Text_NorthWoman, MSGBOX_NPC
	end

Seika_Tidewatch_Text_NorthWoman::
	.string "The path north goes through the\n"
	.string "hills to SILKHOLLOW TOWN.\p"
	.string "The weavers there make the finest\n"
	.string "silk in all of SEIKA.$"

Seika_Tidewatch_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_TIDEWATCH
	end
