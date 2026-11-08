Seika_FrostRoad_MapScripts::
	.byte 0

Seika_FrostRoad_EventScript_Sign::
	msgbox Seika_FrostRoad_Text_Sign, MSGBOX_SIGN
	end

Seika_FrostRoad_Text_Sign::
	.string "FROST ROAD\n"
	.string "FROSTMERE - DUSKPEAK VILLAGE$"

Seika_FrostRoad_EventScript_Fuyumi::
	trainerbattle_single TRAINER_SEIKA_RANGER_FUYUMI, Seika_FrostRoad_Text_FuyumiIntro, Seika_FrostRoad_Text_FuyumiDefeat
	msgbox Seika_FrostRoad_Text_FuyumiPost, MSGBOX_AUTOCLOSE
	end

Seika_FrostRoad_Text_FuyumiIntro::
	.string "The snow came early this year.\n"
	.string "My POKéMON love it!$"

Seika_FrostRoad_Text_FuyumiDefeat::
	.string "Brr... That was a cold loss.$"

Seika_FrostRoad_Text_FuyumiPost::
	.string "SWINUB dig under the snow for\n"
	.string "food. Follow one and you might\l"
	.string "find something!$"

Seika_FrostRoad_EventScript_Shingo::
	trainerbattle_single TRAINER_SEIKA_BLACK_BELT_SHINGO, Seika_FrostRoad_Text_ShingoIntro, Seika_FrostRoad_Text_ShingoDefeat
	msgbox Seika_FrostRoad_Text_ShingoPost, MSGBOX_AUTOCLOSE
	end

Seika_FrostRoad_Text_ShingoIntro::
	.string "I train barefoot in the snow!\n"
	.string "Hyah!$"

Seika_FrostRoad_Text_ShingoDefeat::
	.string "My feet are frozen...$"

Seika_FrostRoad_Text_ShingoPost::
	.string "The cold makes the body strong.\n"
	.string "And the nose red.$"

Seika_FrostRoad_EventScript_Genzo::
	trainerbattle_single TRAINER_SEIKA_HIKER_GENZO, Seika_FrostRoad_Text_GenzoIntro, Seika_FrostRoad_Text_GenzoDefeat
	msgbox Seika_FrostRoad_Text_GenzoPost, MSGBOX_AUTOCLOSE
	end

Seika_FrostRoad_Text_GenzoIntro::
	.string "I climb MT. SKYREACH every year.\n"
	.string "Well, the bottom of it!$"

Seika_FrostRoad_Text_GenzoDefeat::
	.string "Avalanche!$"

Seika_FrostRoad_Text_GenzoPost::
	.string "They say the summit is above the\n"
	.string "clouds. Only the CHAMPION has\l"
	.string "seen it.$"
