Skyrift_Riftwake_RiftHut_MapScripts::
	.byte 0

Skyrift_Riftwake_RiftHut_EventScript_Monitor::
	msgbox Skyrift_Riftwake_RiftHut_Text_Monitor, MSGBOX_SIGN
	end

Skyrift_Riftwake_RiftHut_Text_Monitor::
	.string "Lines of numbers scroll across\n"
	.string "the screen.\p"
	.string "RIFT ENERGY: RISING\n"
	.string "SOURCE: NOVA OBSERVATORY$"
	.string "{JPN}{JPN}がめんに　すうじの　れつが　ながれて　いく。\p"
	.string "さけめエネルギー:　じょうしょうちゅう\n"
	.string "はっせいげん:　ノバてんもんだい$"

QoL_Skyrift_EventScript_Halley::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Halley_After
	goto_if_set FLAG_QOL_SKYRIFT_HALLEY, QoL_Skyrift_Halley_Again
	msgbox QoL_Skyrift_Halley_Text_Intro, MSGBOX_DEFAULT
	giveitem ITEM_BEAST_BALL, 5
	setflag FLAG_QOL_SKYRIFT_HALLEY
	msgbox QoL_Skyrift_Halley_Text_Rifts, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Halley_Again::
	msgbox QoL_Skyrift_Halley_Text_Rifts, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Halley_After::
	call QoL_Skyrift_Necrozma_Check
	goto_if_unset FLAG_QOL_SKYRIFT_HIDE_NECROZMA, QoL_Skyrift_Halley_Sixth
	msgbox QoL_Skyrift_Halley_Text_After, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Halley_Sixth::
	msgbox QoL_Skyrift_Halley_Text_Sixth, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Halley_Text_Intro::
	.string "DR. HALLEY: Visitors! And the\n"
	.string "CHAMPION of SEIKA, no less.\p"
	.string "I study the SKYRIFT. I used to\n"
	.string "work for VEGA, until she\l"
	.string "bought that engine.\p"
	.string "Five tears in the sky have\n"
	.string "opened on these islands. Out of\l"
	.string "each one, an ULTRA BEAST.\p"
	.string "Take these. BEAST BALLS work on\n"
	.string "them far better than anything\l"
	.string "else.$"
	.string "{JPN}{JPN}ハレーはかせ『おきゃくさんです！　しかも\n"
	.string "セイカの　チャンピオンでは　ありませんか！\p"
	.string "わたしは　ソラサケを　けんきゅう　して　います。\n"
	.string "ベガが　あの　エンジンを　かうまでは　ノバだんで\l"
	.string "はたらいて　いました。\p"
	.string "この　しまじまには　そらの　さけめが　5つ\n"
	.string "ひらきました。　その　ひとつ　ひとつから\l"
	.string "ウルトラビーストが　でてきたのです。\p"
	.string "これを　どうぞ。　ウルトラボールは　ほかの\n"
	.string "どんな　ボールより　ずっと　よく　ききますよ。』$"

QoL_Skyrift_Halley_Text_Rifts::
	.string "DR. HALLEY: The rift energy makes\n"
	.string "each ULTRA BEAST much stronger.\p"
	.string "One on SHARDFALL BEACH, two in\n"
	.string "METEOR BASIN, two on the\l"
	.string "OBSERVATORY CLIFFS.\p"
	.string "VEGA is in her OBSERVATORY at\n"
	.string "the top. SIRIUS guards the road.$"
	.string "{JPN}{JPN}ハレーはかせ『さけめの　エネルギーで\n"
	.string "ウルトラビーストは　ずっと　つよく　なるのです。\p"
	.string "さけめは　カケラはまに　1つ　インセキのくぼちに\n"
	.string "2つ　テンモンがけに　2つ　あります。\p"
	.string "ベガは　てっぺんの　ノバてんもんだいに　います。\n"
	.string "みちは　シリウスが　まもって　います。』$"

QoL_Skyrift_Halley_Text_Sixth::
	.string "DR. HALLEY: My readings! A sixth\n"
	.string "rift has opened in METEOR BASIN, by\l"
	.string "the big meteorite.\p"
	.string "Whatever is in it is eating\n"
	.string "the light around it. Be very\l"
	.string "careful!$"
	.string "{JPN}{JPN}ハレーはかせ『けいきが　はんのう　して　います！\n"
	.string "インセキのくぼちの　おおきな　いんせきの　そばに\l"
	.string "6つめの　さけめが　ひらきました！\p"
	.string "なかに　いる　なにかが　まわりの　ひかりを\n"
	.string "たべて　いるのです。　くれぐれも　きを\l"
	.string "つけて！』$"

QoL_Skyrift_Halley_Text_After::
	.string "DR. HALLEY: The engine is quiet.\n"
	.string "The rifts are calming down.\p"
	.string "Some of the ULTRA BEASTS may\n"
	.string "stay. This is their home now,\l"
	.string "too.$"
	.string "{JPN}{JPN}ハレーはかせ『エンジンは　しずかに　なりました。\n"
	.string "さけめも　おちついて　きて　います。\p"
	.string "ウルトラビーストの　なかには　ここに　のこる\n"
	.string "ものも　いるでしょう。　いまでは　ここも\l"
	.string "かれらの　ふるさと　なのです。』$"
