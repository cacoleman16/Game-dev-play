Seika_Silkhollow_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Silkhollow_QoLFly_OnTransition
	.byte 0

Seika_Silkhollow_EventScript_TownSign::
	msgbox Seika_Silkhollow_Text_TownSign, MSGBOX_SIGN
	end

Seika_Silkhollow_Text_TownSign::
	.string "SILKHOLLOW TOWN\n"
	.string "Where silk is spun from\l"
	.string "moonlight.$"

Seika_Silkhollow_EventScript_GymSign::
	msgbox Seika_Silkhollow_Text_GymSign, MSGBOX_SIGN
	end

Seika_Silkhollow_Text_GymSign::
	.string "SILKHOLLOW POKéMON GYM\n"
	.string "LEADER: AYAME\p"
	.string "The weaver who spins every\n"
	.string "battle into silk!$"

Seika_Silkhollow_EventScript_RoadSign::
	msgbox Seika_Silkhollow_Text_RoadSign, MSGBOX_SIGN
	end

Seika_Silkhollow_Text_RoadSign::
	.string "WEAVER'S ROAD\n"
	.string "West to FROSTMERE VILLAGE$"

Seika_Silkhollow_EventScript_Dancer::
	msgbox Seika_Silkhollow_Text_Dancer, MSGBOX_NPC
	end

Seika_Silkhollow_Text_Dancer::
	.string "We dance on this square every\n"
	.string "night of the full moon.\p"
	.string "AYAME dances with us. Her steps\n"
	.string "are as light as a JUMPLUFF!$"

Seika_Silkhollow_EventScript_Weaver::
	msgbox Seika_Silkhollow_Text_Weaver, MSGBOX_NPC
	end

Seika_Silkhollow_Text_Weaver::
	.string "Our silk comes from ARIADOS.\n"
	.string "Their thread never breaks.\p"
	.string "We dye it with flowers and weave\n"
	.string "it by lantern light.$"

Seika_Silkhollow_EventScript_OldMan::
	msgbox Seika_Silkhollow_Text_OldMan, MSGBOX_NPC
	end

Seika_Silkhollow_Text_OldMan::
	.string "WEAVER'S ROAD runs west to\n"
	.string "FROSTMERE VILLAGE.\p"
	.string "The wind off the lake there cuts\n"
	.string "like a knife. Bring a scarf!$"

Seika_Silkhollow_EventScript_Girl::
	msgbox Seika_Silkhollow_Text_Girl, MSGBOX_NPC
	end

Seika_Silkhollow_Text_Girl::
	.string "A man with muddy gloves ran\n"
	.string "through town and out to the west.\p"
	.string "He was holding a shiny stone,\n"
	.string "and laughing to himself!$"

Seika_Silkhollow_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_SILKHOLLOW
	end
