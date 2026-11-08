Seika_KilnRoad_MapScripts::
	.byte 0

Seika_KilnRoad_EventScript_SignSouth::
	msgbox Seika_KilnRoad_Text_SignSouth, MSGBOX_SIGN
	end

Seika_KilnRoad_Text_SignSouth::
	.string "KILN ROAD\n"
	.string "MOSSGROVE VILLAGE - KILN CITY$"

Seika_KilnRoad_EventScript_SignNorth::
	msgbox Seika_KilnRoad_Text_SignNorth, MSGBOX_SIGN
	end

Seika_KilnRoad_Text_SignNorth::
	.string "KILN ROAD\n"
	.string "KILN CITY - MOSSGROVE VILLAGE$"

Seika_KilnRoad_EventScript_ItemPPUp::
	finditem ITEM_PP_UP
	end

Seika_KilnRoad_EventScript_OldMan::
	msgbox Seika_KilnRoad_Text_OldMan, MSGBOX_NPC
	end

Seika_KilnRoad_Text_OldMan::
	.string "The clay of these hills is red and\n"
	.string "fine. KILN CITY was built on it.\p"
	.string "Its kilns have burned day and\n"
	.string "night for six hundred years.$"

Seika_KilnRoad_EventScript_Bunta::
	trainerbattle_single TRAINER_SEIKA_HIKER_BUNTA, Seika_KilnRoad_Text_BuntaIntro, Seika_KilnRoad_Text_BuntaDefeat
	msgbox Seika_KilnRoad_Text_BuntaPost, MSGBOX_AUTOCLOSE
	end

Seika_KilnRoad_Text_BuntaIntro::
	.string "I dig clay for the potters of\n"
	.string "KILN CITY. My rocks dig back!$"

Seika_KilnRoad_Text_BuntaDefeat::
	.string "Crumbled like dry clay!$"

Seika_KilnRoad_Text_BuntaPost::
	.string "Good clay needs water and fire.\n"
	.string "Good POKéMON need both, too.$"

Seika_KilnRoad_EventScript_Hana::
	trainerbattle_single TRAINER_SEIKA_PICNICKER_HANA, Seika_KilnRoad_Text_HanaIntro, Seika_KilnRoad_Text_HanaDefeat
	msgbox Seika_KilnRoad_Text_HanaPost, MSGBOX_AUTOCLOSE
	end

Seika_KilnRoad_Text_HanaIntro::
	.string "A picnic in the foothills! And\n"
	.string "a battle for dessert!$"

Seika_KilnRoad_Text_HanaDefeat::
	.string "That was a bitter dessert...$"

Seika_KilnRoad_Text_HanaPost::
	.string "I bought a teacup in KILN CITY.\n"
	.string "It is too pretty to drink from!$"

Seika_KilnRoad_EventScript_Ryuji::
	trainerbattle_single TRAINER_SEIKA_BLACK_BELT_RYUJI, Seika_KilnRoad_Text_RyujiIntro, Seika_KilnRoad_Text_RyujiDefeat
	msgbox Seika_KilnRoad_Text_RyujiPost, MSGBOX_AUTOCLOSE
	end

Seika_KilnRoad_Text_RyujiIntro::
	.string "I train on this steep road every\n"
	.string "morning. Hoo-hah!$"

Seika_KilnRoad_Text_RyujiDefeat::
	.string "My legs gave out!$"

Seika_KilnRoad_Text_RyujiPost::
	.string "TOBIN, the GYM LEADER, carries\n"
	.string "his own clay up from these hills.$"
