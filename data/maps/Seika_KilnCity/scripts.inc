Seika_KilnCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_KilnCity_QoLFly_OnTransition
	.byte 0

Seika_KilnCity_EventScript_CitySign::
	msgbox Seika_KilnCity_Text_CitySign, MSGBOX_SIGN
	end

Seika_KilnCity_Text_CitySign::
	.string "KILN CITY\n"
	.string "A thousand kilns, one fire.$"

Seika_KilnCity_EventScript_GymSign::
	msgbox Seika_KilnCity_Text_GymSign, MSGBOX_SIGN
	end

Seika_KilnCity_Text_GymSign::
	.string "KILN CITY POKéMON GYM\n"
	.string "LEADER: TOBIN\p"
	.string "The potter whose clay never\n"
	.string "cracks!$"

Seika_KilnCity_EventScript_EastSign::
	msgbox Seika_KilnCity_Text_EastSign, MSGBOX_SIGN
	end

Seika_KilnCity_Text_EastSign::
	.string "TIDE ROAD ahead\n"
	.string "TIDEWATCH TOWN to the east$"

Seika_KilnCity_EventScript_KilnWorker::
	msgbox Seika_KilnCity_Text_KilnWorker, MSGBOX_NPC
	end

Seika_KilnCity_Text_KilnWorker::
	.string "These are the oldest kilns in\n"
	.string "SEIKA. Their fire has not gone\l"
	.string "out in six hundred years.\p"
	.string "The old masters fired a bell of\n"
	.string "starglass here, long ago. They say\l"
	.string "it sings when the star is near.$"

Seika_KilnCity_EventScript_Fountain::
	msgbox Seika_KilnCity_Text_Fountain, MSGBOX_NPC
	end

Seika_KilnCity_Text_Fountain::
	.string "The water in this fountain comes\n"
	.string "down from the hills.\p"
	.string "The potters say good clay needs\n"
	.string "good water. So does good tea!$"

Seika_KilnCity_EventScript_Lass::
	msgbox Seika_KilnCity_Text_Lass, MSGBOX_NPC
	end

Seika_KilnCity_Text_Lass::
	.string "TOBIN the potter is our GYM\n"
	.string "LEADER. His pots ring like bells\l"
	.string "when you tap them!$"

Seika_KilnCity_EventScript_Man::
	msgbox Seika_KilnCity_Text_Man, MSGBOX_NPC
	end

Seika_KilnCity_Text_Man::
	.string "KILN ROAD goes south, back to\n"
	.string "MOSSGROVE VILLAGE.\p"
	.string "The hills along it are full of\n"
	.string "red clay. And grumpy DONPHAN.$"

Seika_KilnCity_EventScript_Youngster::
	msgbox Seika_KilnCity_Text_Youngster, MSGBOX_NPC
	end

Seika_KilnCity_Text_Youngster::
	.string "A lady in a fine coat walked into\n"
	.string "the old workshop this morning.\p"
	.string "She had a strange bag with a star\n"
	.string "on it. Is she a collector?$"

Seika_KilnCity_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_KILN_CITY
	end
