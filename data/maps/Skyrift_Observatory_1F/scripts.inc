Skyrift_Observatory_1F_MapScripts::
	.byte 0

Skyrift_Observatory_1F_EventScript_Cases::
	msgbox Skyrift_Observatory_1F_Text_Cases, MSGBOX_SIGN
	end

Skyrift_Observatory_1F_Text_Cases::
	.string "Glass cases full of meteorite\n"
	.string "chips, each with a label:\p"
	.string "“METEOR BASIN, SECOND STARFALL.”$"
	.string "{JPN}{JPN}ガラスケースに　いんせきの　かけらが　ぎっしり。\n"
	.string "どれにも　ラベルが　はってある。\p"
	.string "「インセキのくぼち　2どめの　スターフォール」$"

Skyrift_Observatory_1F_EventScript_NovaA::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_1F_1, Skyrift_Observatory_1F_Text_NovaAIntro, Skyrift_Observatory_1F_Text_NovaADefeat
	msgbox Skyrift_Observatory_1F_Text_NovaAPost, MSGBOX_AUTOCLOSE
	end

Skyrift_Observatory_1F_Text_NovaAIntro::
	.string "Intruders in the OBSERVATORY!\n"
	.string "Sound the alarm!$"
	.string "{JPN}{JPN}ノバてんもんだいに　しんにゅうしゃだ！\n"
	.string "けいほうを　ならせ！$"

Skyrift_Observatory_1F_Text_NovaADefeat::
	.string "Alarm... off...$"
	.string "{JPN}{JPN}けいほう‥　ていし‥$"

Skyrift_Observatory_1F_Text_NovaAPost::
	.string "The lab is upstairs. VEGA is\n"
	.string "above that, on the roof.$"
	.string "{JPN}{JPN}けんきゅうしつは　2かいだ。　ベガさまは　その\n"
	.string "うえの　おくじょうに　いらっしゃる。$"

Skyrift_Observatory_1F_EventScript_NovaB::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_1F_2, Skyrift_Observatory_1F_Text_NovaBIntro, Skyrift_Observatory_1F_Text_NovaBDefeat
	msgbox Skyrift_Observatory_1F_Text_NovaBPost, MSGBOX_AUTOCLOSE
	end

Skyrift_Observatory_1F_Text_NovaBIntro::
	.string "I built half the machines in\n"
	.string "this building. Let me show you!$"
	.string "{JPN}{JPN}この　たてものの　きかいの　はんぶんは　わたしが\n"
	.string "つくった。　おみせ　しよう！$"

Skyrift_Observatory_1F_Text_NovaBDefeat::
	.string "My calculations were off...$"
	.string "{JPN}{JPN}けいさんが　くるって　いた‥$"

Skyrift_Observatory_1F_Text_NovaBPost::
	.string "The ENGINE draws its power from\n"
	.string "the rift itself. Brilliant, no?$"
	.string "{JPN}{JPN}エンジンは　さけめ　そのものから　ちからを　えて\n"
	.string "いる。　みごとだろう？$"
