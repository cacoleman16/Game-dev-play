SSAnne_1F_Room2_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room2_EventScript_Woman::
	msgbox SSAnne_1F_Room2_Text_CruisingAroundWorld, MSGBOX_NPC
	end

SSAnne_1F_Room2_EventScript_Ann::
	trainerbattle_single TRAINER_LASS_ANN, SSAnne_1F_Room2_Text_AnnIntro, SSAnne_1F_Room2_Text_AnnDefeat
	msgbox SSAnne_1F_Room2_Text_AnnPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room2_EventScript_Tyler::
	trainerbattle_single TRAINER_YOUNGSTER_TYLER, SSAnne_1F_Room2_Text_TylerIntro, SSAnne_1F_Room2_Text_TylerDefeat
	msgbox SSAnne_1F_Room2_Text_TylerPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room2_Text_TylerIntro::
	.string "I love POKéMON!\n"
	.string "Do you?$"
	.string "{JPN}{JPN}ぼく　ポケモン　だいすき！　きみは？$"

SSAnne_1F_Room2_Text_TylerDefeat::
	.string "Wow! \n"
	.string "You're great!$"
	.string "{JPN}{JPN}わあ！　きみ　すごいね！$"

SSAnne_1F_Room2_Text_TylerPostBattle::
	.string "Listen, listen!\n"
	.string "Let me be your friend, okay?\p"
	.string "Then we can trade POKéMON and\n"
	.string "do stuff.$"
	.string "{JPN}{JPN}ねえねえ！　ぼくと　ともだちに　なってよ！\p"
	.string "そしたら　ポケモンを　こうかん　したり\n"
	.string "できるでしょ。$"

SSAnne_1F_Room2_Text_AnnIntro::
	.string "I collected these POKéMON\n"
	.string "from all around the world!$"
	.string "{JPN}{JPN}せかいじゅうから　あつめた　ポケモン　たちよ！$"

SSAnne_1F_Room2_Text_AnnDefeat::
	.string "Oh, no!\n"
	.string "I went around the world for these!$"
	.string "{JPN}{JPN}そんな！　せかいじゅうを　まわって\n"
	.string "あつめたのに！$"

SSAnne_1F_Room2_Text_AnnPostBattle::
	.string "You hurt my poor POKéMON!\p"
	.string "I demand that you heal them at a\n"
	.string "POKéMON CENTER!$"
	.string "{JPN}{JPN}わたしの　かわいそうな　ポケモンを\n"
	.string "きずつけたわね！\p"
	.string "ポケモンセンターで　かいふく　させて\n"
	.string "ちょうだい！$"

SSAnne_1F_Room2_Text_CruisingAroundWorld::
	.string "We are cruising around the world,\n"
	.string "my children and I.$"
	.string "{JPN}{JPN}こどもたちと　いっしょに　せかいいっしゅうの\n"
	.string "ふなたびを　しているんです。$"

