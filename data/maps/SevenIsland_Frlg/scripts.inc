SevenIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_OnTransition
	.byte 0

SevenIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SEVEN_ISLAND
	end

SevenIsland_EventScript_IslandSign::
	msgbox SevenIsland_Text_IslandSign, MSGBOX_SIGN
	end

SevenIsland_EventScript_OldWoman::
	msgbox SevenIsland_Text_IslandsMadeInSevenDays, MSGBOX_NPC
	end

SevenIsland_EventScript_Scientist::
	msgbox SevenIsland_Text_IslandVirtuallyUntouched, MSGBOX_NPC
	end

Text_SwordsDanceTeach::
	.string "Not many people come out here.\p"
	.string "If I train here, I'm convinced that\n"
	.string "I'll get stronger and stronger.\p"
	.string "Yep, stronger and stronger…\p"
	.string "How would you like to learn a move\n"
	.string "like that? It's SWORDS DANCE.$"
	.string "{JPN}{JPN}ここには　あまり　ひとが　こないんだ。\p"
	.string "ここで　きたえれば　おれは　どんどん　つよく\n"
	.string "なれる　きが　するんだ。\p"
	.string "そう　どんどん　つよく‥\p"
	.string "そんな　わざを　おぼえて　みないか？\n"
	.string "つるぎのまい　って　いうんだ。$"

Text_SwordsDanceDeclined::
	.string "Well, that's fine, too.$"
	.string "{JPN}{JPN}まあ　それも　いいさ。$"

Text_SwordsDanceWhichMon::
	.string "All right, pick which POKéMON\n"
	.string "I should teach it to.$"
	.string "{JPN}{JPN}よし　どの　ポケモンに　おしえるか　えらんで\n"
	.string "くれ。$"

Text_SwordsDanceTaught::
	.string "If only we could keep getting\n"
	.string "stronger and stronger, eh?$"
	.string "{JPN}{JPN}どこまでも　どんどん　つよく　なれたら\n"
	.string "いいよな？$"

SevenIsland_Text_IslandSign::
	.string "SEVEN ISLAND\n"
	.string "Quest Island of Infinity$"
	.string "{JPN}{JPN}7のしま\n"
	.string "はてしない　ぼうけんの　しま$"

SevenIsland_Text_IslandsMadeInSevenDays::
	.string "These islands are called the SEVII\n"
	.string "ISLANDS because there are seven.\p"
	.string "…Or at least that's what the young\n"
	.string "people believe.\p"
	.string "The truth is, these islands are so\n"
	.string "named because they are said to\l"
	.string "have been made in seven days.$"
	.string "{JPN}{JPN}この　しまじまが　ナナシマと　よばれるのは\n"
	.string "しまが　7つ　あるから‥\p"
	.string "‥と　わかい　ものは　おもって　おる。\p"
	.string "じつは　この　しまじまは　7にちで　つくられたと\n"
	.string "いわれて　おるから　ナナシマと　いうんじゃよ。$"

SevenIsland_Text_IslandVirtuallyUntouched::
	.string "This island has been left virtually\n"
	.string "untouched in its natural state.\p"
	.string "It could be a little difficult to\n"
	.string "hike around because of that.$"
	.string "{JPN}{JPN}この　しまは　しぜんの　ままの　すがたが\n"
	.string "ほとんど　のこって　いるの。\p"
	.string "だから　あるきまわるのは　ちょっと　たいへん\n"
	.string "かもね。$"

