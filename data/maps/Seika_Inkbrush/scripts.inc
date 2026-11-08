Seika_Inkbrush_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Inkbrush_QoLFly_OnTransition
	.byte 0

Seika_Inkbrush_EventScript_TownSign::
	msgbox Seika_Inkbrush_Text_TownSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_TownSign::
	.string "INKBRUSH TOWN\n"
	.string "Every stroke tells a story.$"

Seika_Inkbrush_EventScript_GymSign::
	msgbox Seika_Inkbrush_Text_GymSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_GymSign::
	.string "INKBRUSH POKéMON GYM\n"
	.string "LEADER: SUMI\p"
	.string "The calligrapher who reads\n"
	.string "the mind behind every stroke!$"

Seika_Inkbrush_EventScript_HallSign::
	msgbox Seika_Inkbrush_Text_HallSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_HallSign::
	.string "INKBRUSH SCRIPTORIUM\n"
	.string "Keepers of the STARFALL SCROLL$"

Seika_Inkbrush_EventScript_WestSign::
	msgbox Seika_Inkbrush_Text_WestSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_WestSign::
	.string "WEST: MOSSGROVE WOODS\n"
	.string "MOSSGROVE VILLAGE lies beyond.$"

Seika_Inkbrush_EventScript_OldScribe::
	msgbox Seika_Inkbrush_Text_OldScribe, MSGBOX_NPC
	end

Seika_Inkbrush_Text_OldScribe::
	.string "I have copied books in that\n"
	.string "SCRIPTORIUM for sixty years.\p"
	.string "In all that time, nobody asked to\n"
	.string "see the STARFALL SCROLL. Then\l"
	.string "last week, a stranger did.$"

Seika_Inkbrush_EventScript_Boy::
	msgbox Seika_Inkbrush_Text_Boy, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Boy::
	.string "I'm practicing my brush strokes!\p"
	.string "SUMI says a good stroke comes\n"
	.string "from a calm mind. Mine wobble...$"

Seika_Inkbrush_EventScript_Woman::
	msgbox Seika_Inkbrush_Text_Woman, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Woman::
	.string "MOSSGROVE WOODS is just west of\n"
	.string "town. It's easy to get lost in\l"
	.string "there.\p"
	.string "A man with dirty gloves went in\n"
	.string "with a shovel. Digging for what,\l"
	.string "I wonder?$"

Seika_Inkbrush_EventScript_Fisher::
	msgbox Seika_Inkbrush_Text_Fisher, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Fisher::
	.string "The scribes rinse their brushes\n"
	.string "in this pond.\p"
	.string "The POLIWAG here have ink-black\n"
	.string "swirls. Coincidence? Ha!$"

Seika_Inkbrush_EventScript_Gentleman::
	msgbox Seika_Inkbrush_Text_Gentleman, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Gentleman::
	.string "SUMI can read a person's heart\n"
	.string "from one line of their writing.\p"
	.string "I wrote her a letter once. She\n"
	.string "still laughs when she sees me.$"

Seika_Inkbrush_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_INKBRUSH
	end
