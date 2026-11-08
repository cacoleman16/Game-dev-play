Seika_Mossgrove_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Mossgrove_QoLFly_OnTransition
	.byte 0

Seika_Mossgrove_EventScript_TownSign::
	msgbox Seika_Mossgrove_Text_TownSign, MSGBOX_SIGN
	end

Seika_Mossgrove_Text_TownSign::
	.string "MOSSGROVE VILLAGE\n"
	.string "Sweet as honey, soft as moss.$"
	.string "{JPN}{JPN}コケムスむら　ミツは　あまく　コケは　やわらか$"

Seika_Mossgrove_EventScript_GymSign::
	msgbox Seika_Mossgrove_Text_GymSign, MSGBOX_SIGN
	end

Seika_Mossgrove_Text_GymSign::
	.string "MOSSGROVE POKéMON GYM\n"
	.string "LEADER: KAYA\p"
	.string "The beekeeper who never\n"
	.string "gets stung!$"
	.string "{JPN}{JPN}コケムス　ポケモンジム　リーダー　カヤ\p"
	.string "けっして　ハチに　さされない　ようほうか！$"

Seika_Mossgrove_EventScript_EastSign::
	msgbox Seika_Mossgrove_Text_EastSign, MSGBOX_SIGN
	end

Seika_Mossgrove_Text_EastSign::
	.string "EAST: MOSSGROVE WOODS\n"
	.string "INKBRUSH TOWN lies beyond.$"
	.string "{JPN}{JPN}ひがしへ　コケムスのもり　その　さきに\n"
	.string "スミフデタウン$"

Seika_Mossgrove_EventScript_Beekeeper::
	msgbox Seika_Mossgrove_Text_Beekeeper, MSGBOX_NPC
	end

Seika_Mossgrove_Text_Beekeeper::
	.string "These flowers feed the village\n"
	.string "COMBEE.\p"
	.string "We never take all their honey.\n"
	.string "Half for us, half for the hive.\l"
	.string "That's the MOSSGROVE way.$"
	.string "{JPN}{JPN}この　はなが　むらの　ミツハニーの\n"
	.string "ごはんなんだ。\p"
	.string "ミツは　ぜんぶ　とらない。　はんぶんは　ひとに\n"
	.string "はんぶんは　すに。　それが　コケムスの\l"
	.string "やりかたさ。$"

Seika_Mossgrove_EventScript_Girl::
	msgbox Seika_Mossgrove_Text_Girl, MSGBOX_NPC
	end

Seika_Mossgrove_Text_Girl::
	.string "KAYA can talk to bugs! She just\n"
	.string "hums, and they listen.$"
	.string "{JPN}{JPN}カヤさんは　むしと　おはなし　できるの！\n"
	.string "はなうたを　うたうと　みんな　きいて　くれるの。$"

Seika_Mossgrove_EventScript_Hiker::
	msgbox Seika_Mossgrove_Text_Hiker, MSGBOX_NPC
	end

Seika_Mossgrove_Text_Hiker::
	.string "This road runs north to KILN CITY,\n"
	.string "the biggest city in SEIKA.\p"
	.string "The kilns there burn day and\n"
	.string "night. You'll smell the smoke\l"
	.string "before you see the walls.$"
	.string "{JPN}{JPN}この　みちを　きたへ　いくと　カマモトシティ。\n"
	.string "セイカで　いちばん　おおきな　まちさ。\p"
	.string "あそこの　かまは　ひるも　よるも　もえて　いる。\n"
	.string "まちの　かべが　みえる　まえに　けむりの\l"
	.string "においが　するぞ。$"

Seika_Mossgrove_EventScript_OldWoman::
	msgbox Seika_Mossgrove_Text_OldWoman, MSGBOX_NPC
	end

Seika_Mossgrove_Text_OldWoman::
	.string "A woman in a fine coat came\n"
	.string "through not long ago.\p"
	.string "She asked me which road goes to\n"
	.string "KILN CITY. Then she asked about\l"
	.string "a bell.\p"
	.string "What would a lady like that\n"
	.string "want with a bell?$"
	.string "{JPN}{JPN}ちょっと　まえに　りっぱな　コートの　おんなが\n"
	.string "ここを　とおって　いったよ。\p"
	.string "カマモトシティへは　どの　みちかと　きいて\n"
	.string "それから　かねの　ことを　たずねて　きたんじゃ。\p"
	.string "あんな　おじょうさんが　かねなんぞ　どう\n"
	.string "するんじゃろうのう？$"

Seika_Mossgrove_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_MOSSGROVE
	end
