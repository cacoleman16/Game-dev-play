Seika_Inkbrush_Scriptorium_MapScripts::
	.byte 0

Seika_Inkbrush_Scriptorium_EventScript_Keeper::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_SCROLL, Seika_Inkbrush_Scriptorium_EventScript_KeeperAgain
	msgbox Seika_Inkbrush_Scriptorium_Text_Keeper, MSGBOX_DEFAULT
	setflag FLAG_QOL_SEIKA_SCROLL
	release
	end

Seika_Inkbrush_Scriptorium_EventScript_KeeperAgain::
	msgbox Seika_Inkbrush_Scriptorium_Text_KeeperAgain, MSGBOX_DEFAULT
	release
	end

Seika_Inkbrush_Scriptorium_Text_Keeper::
	.string "Ah, a traveler. I am the keeper\n"
	.string "of this SCRIPTORIUM.\p"
	.string "You have the look of someone\n"
	.string "chasing a star. You are the\l"
	.string "second this week.\p"
	.string "Last week a woman in a fine coat\n"
	.string "came in. She asked to copy the\l"
	.string "STARFALL SCROLL.\p"
	.string "She wrote very fast, and she\n"
	.string "never smiled once.\p"
	.string "The scroll is the oldest record\n"
	.string "of the first STARFALL. It says...\p"
	.string "“The WISHING STAR sleeps at the\n"
	.string "SKY SHRINE, on the top of\l"
	.string "MT. SKYREACH.”\p"
	.string "“It wakes fully only when the\n"
	.string "STARGLASS BELL rings.”\p"
	.string "And the STARGLASS BELL is made\n"
	.string "in one place only: the kilns of\l"
	.string "KILN CITY.\p"
	.string "If she wants to wake the star,\n"
	.string "that is where she will go.$"
	.string "{JPN}{JPN}おお　たびの　かたか。　わしは　この\n"
	.string "まきものかんの　あるじじゃ。\p"
	.string "ほしを　おいかける　ものの　かおを　しておるな。\n"
	.string "こんしゅう　ふたりめじゃよ。\p"
	.string "せんしゅう　りっぱな　コートの　おんなが\n"
	.string "きてな。　スターフォールのまきものを　うつさせて\l"
	.string "くれと　いうた。\p"
	.string "ずいぶん　はやく　かいて‥　いちども\n"
	.string "わらわなんだ。\p"
	.string "この　まきものは　さいしょの　スターフォールの\n"
	.string "いちばん　ふるい　きろくじゃ。　こう　かいて\l"
	.string "ある‥\p"
	.string "「ねがいぼしは　ソラトドキやまの　いただき\n"
	.string "そらのやしろに　ねむる」\p"
	.string "「ほしガラスのかねが　なる　とき　ねがいぼしは\n"
	.string "まことに　めざめる」\p"
	.string "そして　ほしガラスのかねを　つくれるのは　ただ\n"
	.string "ひとつ‥　カマモトシティの　かまだけじゃ。\p"
	.string "あの　おんなが　ほしを　おこしたいなら　きっと\n"
	.string "そこへ　むかうはずじゃ。$"

Seika_Inkbrush_Scriptorium_Text_KeeperAgain::
	.string "The WISHING STAR sleeps at the\n"
	.string "SKY SHRINE on MT. SKYREACH.\p"
	.string "It wakes fully when the\n"
	.string "STARGLASS BELL rings. Only\l"
	.string "KILN CITY makes that bell.\p"
	.string "The woman in the fine coat...\n"
	.string "I fear she is a step ahead.$"
	.string "{JPN}{JPN}ねがいぼしは　ソラトドキやまの　そらのやしろに\n"
	.string "ねむって　おる。\p"
	.string "ほしガラスのかねが　なれば　まことに　めざめる。\n"
	.string "その　かねを　つくれるのは\l"
	.string "カマモトシティだけじゃ。\p"
	.string "あの　コートの　おんな‥　わしらの　いっぽ\n"
	.string "さきを　いっておる　きが　するのう。$"

Seika_Inkbrush_Scriptorium_EventScript_Scribe::
	msgbox Seika_Inkbrush_Scriptorium_Text_Scribe, MSGBOX_NPC
	end

Seika_Inkbrush_Scriptorium_Text_Scribe::
	.string "We copy every old book by hand,\n"
	.string "so the words never die.\p"
	.string "That woman's copy was perfect.\n"
	.string "Too perfect. She has done this\l"
	.string "before.$"
	.string "{JPN}{JPN}わたしたちは　ふるい　ほんを　すべて　てで\n"
	.string "うつします。　ことばが　きえない　ように。\p"
	.string "あの　ひとの　うつしは　かんぺきでした。\n"
	.string "かんぺき　すぎるほど。　きっと　はじめてじゃ\l"
	.string "ないわ。$"

Seika_Inkbrush_Scriptorium_EventScript_Scholar::
	msgbox Seika_Inkbrush_Scriptorium_Text_Scholar, MSGBOX_NPC
	end

Seika_Inkbrush_Scriptorium_Text_Scholar::
	.string "I came all the way from KANTO to\n"
	.string "read these scrolls.\p"
	.string "The old scribes drew the WISHING\n"
	.string "STAR as a small child with long\l"
	.string "ribbons. How charming!$"
	.string "{JPN}{JPN}わたしは　この　まきものを　よむ　ために\n"
	.string "はるばる　カントーから　きたんだ。\p"
	.string "むかしの　かきてたちは　ねがいぼしを　ながい\n"
	.string "リボンの　ちいさな　こどもの　すがたで　えがいて\l"
	.string "いる。　かわいい　じゃないか！$"

Seika_Inkbrush_Scriptorium_EventScript_Case1::
	msgbox Seika_Inkbrush_Scriptorium_Text_Case1, MSGBOX_SIGN
	end

Seika_Inkbrush_Scriptorium_Text_Case1::
	.string "A faded scroll is on display.\p"
	.string "It shows a star falling on a\n"
	.string "mountain, and people kneeling\l"
	.string "far below.$"
	.string "{JPN}{JPN}いろあせた　まきものが　かざって　ある。\p"
	.string "やまに　おちる　ほしと　はるか　したで\n"
	.string "ひざまずく　ひとびとが　えがかれて　いる。$"

Seika_Inkbrush_Scriptorium_EventScript_Case2::
	msgbox Seika_Inkbrush_Scriptorium_Text_Case2, MSGBOX_SIGN
	end

Seika_Inkbrush_Scriptorium_Text_Case2::
	.string "Old brushes and an inkstone are\n"
	.string "on display.\p"
	.string "The label says they belonged to\n"
	.string "the first keeper of the shrine.$"
	.string "{JPN}{JPN}ふるい　ふでと　すずりが　かざって　ある。\p"
	.string "せつめいがきに　よると　やしろの　しょだいの\n"
	.string "もりびとが　つかって　いた　ものらしい。$"
