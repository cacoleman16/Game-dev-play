Seika_Tidewatch_TideShrine_MapScripts::
	.byte 0

Seika_Tidewatch_TideShrine_EventScript_Keeper::
	msgbox Seika_Tidewatch_TideShrine_Text_Keeper, MSGBOX_NPC
	end

Seika_Tidewatch_TideShrine_Text_Keeper::
	.string "Welcome to the TIDE SHRINE.\p"
	.string "A thousand years ago, the first\n"
	.string "star fell into the sky over the\l"
	.string "sea, and the tide rose to meet it.\p"
	.string "We have watched the tide ever\n"
	.string "since. When it rises too high,\l"
	.string "the star is stirring.\p"
	.string "This year the sea has been very\n"
	.string "high indeed...$"

Seika_Tidewatch_TideShrine_EventScript_Attendant::
	msgbox Seika_Tidewatch_TideShrine_Text_Attendant, MSGBOX_NPC
	end

Seika_Tidewatch_TideShrine_Text_Attendant::
	.string "NAGI is our shrine keeper. She\n"
	.string "dives to the bottom of the bay to\l"
	.string "read the old stones.\p"
	.string "She says the stones point north.\n"
	.string "Toward the mountain.$"

Seika_Tidewatch_TideShrine_EventScript_TideChart::
	msgbox Seika_Tidewatch_TideShrine_Text_TideChart, MSGBOX_SIGN
	end

Seika_Tidewatch_TideShrine_Text_TideChart::
	.string "Old tide charts, a thousand years\n"
	.string "of them.\p"
	.string "One page is marked with a star. On\n"
	.string "it, the tide line runs off the top\l"
	.string "of the page.$"
