Skyrift_Observatory_2F_MapScripts::
	.byte 0

Skyrift_Observatory_2F_EventScript_Machine::
	msgbox Skyrift_Observatory_2F_Text_Machine, MSGBOX_SIGN
	end

Skyrift_Observatory_2F_Text_Machine::
	.string "A big machine, humming. Cables\n"
	.string "from it run up through the\l"
	.string "ceiling.\p"
	.string "A note: “ENGINE POWER LINE.\n"
	.string "DO NOT TOUCH.”$"
	.string "{JPN}{JPN}うなりを　あげる　おおきな　きかい。　ケーブルが\n"
	.string "てんじょうを　ぬけて　うえへ　のびて　いる。\p"
	.string "はりがみが　ある。　「エンジン　でんりょくせん\n"
	.string "さわるな　きけん」$"

Skyrift_Observatory_2F_EventScript_ItemMaxElixir::
	finditem ITEM_MAX_ELIXIR
	end

Skyrift_Observatory_2F_EventScript_Researcher::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_2F_1, Skyrift_Observatory_2F_Text_ResearcherIntro, Skyrift_Observatory_2F_Text_ResearcherDefeat
	msgbox Skyrift_Observatory_2F_Text_ResearcherPost, MSGBOX_AUTOCLOSE
	end

Skyrift_Observatory_2F_Text_ResearcherIntro::
	.string "Don't touch the machines!\n"
	.string "They feed the ENGINE!$"
	.string "{JPN}{JPN}きかいに　さわるな！　エンジンに　ちからを\n"
	.string "おくって　いるんだ！$"

Skyrift_Observatory_2F_Text_ResearcherDefeat::
	.string "My readings... ruined!$"
	.string "{JPN}{JPN}わたしの　データが‥　だいなしだ！$"

Skyrift_Observatory_2F_Text_ResearcherPost::
	.string "The ENGINE is calling DEOXYS\n"
	.string "back through the rift. Or so\l"
	.string "VEGA says.$"
	.string "{JPN}{JPN}エンジンは　さけめの　むこうから　デオキシスを\n"
	.string "よびもどして　いる。　ベガさまは　そう\l"
	.string "おっしゃって　いる。$"

Skyrift_Observatory_2F_EventScript_NovaB::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_2F_2, Skyrift_Observatory_2F_Text_NovaBIntro, Skyrift_Observatory_2F_Text_NovaBDefeat
	msgbox Skyrift_Observatory_2F_Text_NovaBPost, MSGBOX_AUTOCLOSE
	end

Skyrift_Observatory_2F_Text_NovaBIntro::
	.string "The roof is VEGA's alone.\n"
	.string "Turn back, or be turned back!$"
	.string "{JPN}{JPN}おくじょうは　ベガさま　だけの　ばしょよ。\n"
	.string "ひきかえしなさい！　さもないと　おいかえすわよ！$"

Skyrift_Observatory_2F_Text_NovaBDefeat::
	.string "I couldn't hold the line...$"
	.string "{JPN}{JPN}くいとめられ　なかった‥$"

Skyrift_Observatory_2F_Text_NovaBPost::
	.string "Go up, then. But SIRIUS won't\n"
	.string "let her fall without a fight.$"
	.string "{JPN}{JPN}なら　いきなさい。　でも　シリウスが　だまって\n"
	.string "ベガさまを　まけさせる　はず　ないわ。$"
