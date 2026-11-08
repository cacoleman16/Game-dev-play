SSAnne_1F_Room7_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room7_EventScript_Thomas::
	trainerbattle_single TRAINER_GENTLEMAN_THOMAS, SSAnne_1F_Room7_Text_ThomasIntro, SSAnne_1F_Room7_Text_ThomasDefeat
	msgbox SSAnne_1F_Room7_Text_ThomasPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room7_Text_ThomasIntro::
	.string "I am but a solitary traveler…\p"
	.string "My sole companions and friends are\n"
	.string "POKéMON I caught on my journeys…$"
	.string "{JPN}{JPN}わたしは　ひとり　さすらう　たびびと‥\p"
	.string "なかまは　たびの　とちゅうで　つかまえた\n"
	.string "ポケモン　だけ‥$"

SSAnne_1F_Room7_Text_ThomasDefeat::
	.string "My, my friends…$"
	.string "{JPN}{JPN}わ　わたしの　ともよ‥$"

SSAnne_1F_Room7_Text_ThomasPostBattle::
	.string "You should be nice to friends!$"
	.string "{JPN}{JPN}ともだちは　たいせつに　しなきゃ　だめだよ！$"

