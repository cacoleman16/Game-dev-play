SSAnne_2F_Room4_Frlg_MapScripts::
	.byte 0

SSAnne_2F_Room4_EventScript_Lamar::
	trainerbattle_single TRAINER_GENTLEMAN_LAMAR, SSAnne_2F_Room4_Text_LamarIntro, SSAnne_2F_Room4_Text_LamarDefeat
	msgbox SSAnne_2F_Room4_Text_LamarPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_2F_Room4_EventScript_Dawn::
	trainerbattle_single TRAINER_LASS_DAWN, SSAnne_2F_Room4_Text_DawnIntro, SSAnne_2F_Room4_Text_DawnDefeat
	msgbox SSAnne_2F_Room4_Text_DawnPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_2F_Room4_Text_LamarIntro::
	.string "Which do you find more worthy,\n"
	.string "a strong or a rare POKéMON?$"
	.string "{JPN}{JPN}つよい　ポケモンと　めずらしい　ポケモン　きみは\n"
	.string "どちらに　かちが　あると　おもう？$"

SSAnne_2F_Room4_Text_LamarDefeat::
	.string "I must salute you!$"
	.string "{JPN}{JPN}おみごと！$"

SSAnne_2F_Room4_Text_LamarPostBattle::
	.string "Personally, I prefer strong and\n"
	.string "rare POKéMON.$"
	.string "{JPN}{JPN}わたしは　つよくて　めずらしい　ポケモンが\n"
	.string "すきだね。$"

SSAnne_2F_Room4_Text_DawnIntro::
	.string "I don't believe I saw you at the\n"
	.string "party?$"
	.string "{JPN}{JPN}パーティーで　おみかけ　しなかった　かしら？$"

SSAnne_2F_Room4_Text_DawnDefeat::
	.string "Take it easy!$"
	.string "{JPN}{JPN}おてやわらかに！$"

SSAnne_2F_Room4_Text_DawnPostBattle::
	.string "Oh, I adore your strong POKéMON!\n"
	.string "Oh, how I envy you for them!$"
	.string "{JPN}{JPN}まあ　つよい　ポケモン　すてき！\n"
	.string "うらやましいわ！$"

