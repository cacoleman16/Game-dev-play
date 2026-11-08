Seika_LanternRoad_MapScripts::
	.byte 0

Seika_LanternRoad_EventScript_Sign::
	msgbox Seika_LanternRoad_Text_Sign, MSGBOX_SIGN
	end

Seika_LanternRoad_Text_Sign::
	.string "LANTERN ROAD\n"
	.string "LANTERNPORT - INKBRUSH TOWN$"

Seika_LanternRoad_EventScript_Goro::
	trainerbattle_single TRAINER_SEIKA_HIKER_GORO, Seika_LanternRoad_Text_GoroIntro, Seika_LanternRoad_Text_GoroDefeat
	msgbox Seika_LanternRoad_Text_GoroPost, MSGBOX_AUTOCLOSE
	end

Seika_LanternRoad_Text_GoroIntro::
	.string "These hills are older than any\n"
	.string "book. Same goes for my rocks!$"

Seika_LanternRoad_Text_GoroDefeat::
	.string "You crumbled my rocks!$"

Seika_LanternRoad_Text_GoroPost::
	.string "MT. SKYREACH is the tallest peak\n"
	.string "in SEIKA. Clouds sit on top.$"

Seika_LanternRoad_EventScript_Miyu::
	trainerbattle_single TRAINER_SEIKA_LASS_MIYU, Seika_LanternRoad_Text_MiyuIntro, Seika_LanternRoad_Text_MiyuDefeat
	msgbox Seika_LanternRoad_Text_MiyuPost, MSGBOX_AUTOCLOSE
	end

Seika_LanternRoad_Text_MiyuIntro::
	.string "I came to see the lanterns. And\n"
	.string "to battle someone from KANTO!$"

Seika_LanternRoad_Text_MiyuDefeat::
	.string "KANTO trainers are amazing!$"

Seika_LanternRoad_Text_MiyuPost::
	.string "I'll catch a HOPPIP and float up\n"
	.string "to the stars one day.$"

Seika_LanternRoad_EventScript_Tobi::
	trainerbattle_single TRAINER_SEIKA_BIRD_KEEPER_TOBI, Seika_LanternRoad_Text_TobiIntro, Seika_LanternRoad_Text_TobiDefeat
	msgbox Seika_LanternRoad_Text_TobiPost, MSGBOX_AUTOCLOSE
	end

Seika_LanternRoad_Text_TobiIntro::
	.string "My birds watched the STARFALL\n"
	.string "all night long!$"

Seika_LanternRoad_Text_TobiDefeat::
	.string "Grounded!$"

Seika_LanternRoad_Text_TobiPost::
	.string "XATU stare at the sun all day.\n"
	.string "They say they can see the future.$"
