Seika_FrostRoad_MapScripts::
	.byte 0

Seika_FrostRoad_EventScript_Sign::
	msgbox Seika_FrostRoad_Text_Sign, MSGBOX_SIGN
	end

Seika_FrostRoad_Text_Sign::
	.string "FROST ROAD\n"
	.string "FROSTMERE - DUSKPEAK VILLAGE$"
	.string "{JPN}{JPN}シモかいどう　シモミズむらから　タソガレむらへ$"

Seika_FrostRoad_EventScript_Fuyumi::
	trainerbattle_single TRAINER_SEIKA_RANGER_FUYUMI, Seika_FrostRoad_Text_FuyumiIntro, Seika_FrostRoad_Text_FuyumiDefeat
	msgbox Seika_FrostRoad_Text_FuyumiPost, MSGBOX_AUTOCLOSE
	end

Seika_FrostRoad_Text_FuyumiIntro::
	.string "The snow came early this year.\n"
	.string "My POKéMON love it!$"
	.string "{JPN}{JPN}ことしは　ゆきが　はやかったの。　わたしの\n"
	.string "ポケモンは　おおよろこびよ！$"

Seika_FrostRoad_Text_FuyumiDefeat::
	.string "Brr... That was a cold loss.$"
	.string "{JPN}{JPN}ブルル‥　こごえる　まけっぷりだわ。$"

Seika_FrostRoad_Text_FuyumiPost::
	.string "SWINUB dig under the snow for\n"
	.string "food. Follow one and you might\l"
	.string "find something!$"
	.string "{JPN}{JPN}ウリムーは　ゆきの　したを　ほって　えさを\n"
	.string "さがすの。　ついて　いけば　なにか\l"
	.string "みつかるかもよ！$"

Seika_FrostRoad_EventScript_Shingo::
	trainerbattle_single TRAINER_SEIKA_BLACK_BELT_SHINGO, Seika_FrostRoad_Text_ShingoIntro, Seika_FrostRoad_Text_ShingoDefeat
	msgbox Seika_FrostRoad_Text_ShingoPost, MSGBOX_AUTOCLOSE
	end

Seika_FrostRoad_Text_ShingoIntro::
	.string "I train barefoot in the snow!\n"
	.string "Hyah!$"
	.string "{JPN}{JPN}おれは　ゆきの　なかで　はだしで　しゅぎょう\n"
	.string "しているのだ！　ハーッ！$"

Seika_FrostRoad_Text_ShingoDefeat::
	.string "My feet are frozen...$"
	.string "{JPN}{JPN}あしが　こおりついた‥$"

Seika_FrostRoad_Text_ShingoPost::
	.string "The cold makes the body strong.\n"
	.string "And the nose red.$"
	.string "{JPN}{JPN}さむさは　からだを　きたえる。　そして　はなを\n"
	.string "あかくする。$"

Seika_FrostRoad_EventScript_Genzo::
	trainerbattle_single TRAINER_SEIKA_HIKER_GENZO, Seika_FrostRoad_Text_GenzoIntro, Seika_FrostRoad_Text_GenzoDefeat
	msgbox Seika_FrostRoad_Text_GenzoPost, MSGBOX_AUTOCLOSE
	end

Seika_FrostRoad_Text_GenzoIntro::
	.string "I climb MT. SKYREACH every year.\n"
	.string "Well, the bottom of it!$"
	.string "{JPN}{JPN}おれは　まいとし　ソラトドキやまに　のぼるんだ。\n"
	.string "まあ　ふもと　だけだがな！$"

Seika_FrostRoad_Text_GenzoDefeat::
	.string "Avalanche!$"
	.string "{JPN}{JPN}なだれだー！$"

Seika_FrostRoad_Text_GenzoPost::
	.string "They say the summit is above the\n"
	.string "clouds. Only the CHAMPION has\l"
	.string "seen it.$"
	.string "{JPN}{JPN}てっぺんは　くもの　うえに　あるらしい。　みた\n"
	.string "ことが　あるのは　チャンピオン　だけさ。$"
