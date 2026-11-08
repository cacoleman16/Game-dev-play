RockTunnel_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RockTunnel_1F_OnTransition
	.byte 0

RockTunnel_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROCK_TUNNEL_1F
	end

RockTunnel_1F_EventScript_RouteSign::
	msgbox RockTunnel_1F_Text_RouteSign, MSGBOX_SIGN
	end

RockTunnel_1F_EventScript_Lenny::
	trainerbattle_single TRAINER_HIKER_LENNY, RockTunnel_1F_Text_LennyIntro, RockTunnel_1F_Text_LennyDefeat
	msgbox RockTunnel_1F_Text_LennyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Oliver::
	trainerbattle_single TRAINER_HIKER_OLIVER, RockTunnel_1F_Text_OliverIntro, RockTunnel_1F_Text_OliverDefeat
	msgbox RockTunnel_1F_Text_OliverPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Lucas::
	trainerbattle_single TRAINER_HIKER_LUCAS, RockTunnel_1F_Text_LucasIntro, RockTunnel_1F_Text_LucasDefeat
	msgbox RockTunnel_1F_Text_LucasPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Ashton::
	trainerbattle_single TRAINER_POKEMANIAC_ASHTON, RockTunnel_1F_Text_AshtonIntro, RockTunnel_1F_Text_AshtonDefeat
	msgbox RockTunnel_1F_Text_AshtonPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Leah::
	trainerbattle_single TRAINER_PICNICKER_LEAH, RockTunnel_1F_Text_LeahIntro, RockTunnel_1F_Text_LeahDefeat
	msgbox RockTunnel_1F_Text_LeahPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Ariana::
	trainerbattle_single TRAINER_PICNICKER_ARIANA, RockTunnel_1F_Text_ArianaIntro, RockTunnel_1F_Text_ArianaDefeat
	msgbox RockTunnel_1F_Text_ArianaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Dana::
	trainerbattle_single TRAINER_PICNICKER_DANA, RockTunnel_1F_Text_DanaIntro, RockTunnel_1F_Text_DanaDefeat
	msgbox RockTunnel_1F_Text_DanaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_Text_LennyIntro::
	.string "This tunnel goes a long way, kid!$"
	.string "{JPN}{JPN}この　トンネルは　ながいぞ　ぼうず！$"

RockTunnel_1F_Text_LennyDefeat::
	.string "Doh!\n"
	.string "You win!$"
	.string "{JPN}{JPN}うわっ！　おまえの　かちだ！$"

RockTunnel_1F_Text_LennyPostBattle::
	.string "Watch for ONIX.\n"
	.string "They appear only occasionally.\p"
	.string "High-level ones can put the\n"
	.string "squeeze on you!$"
	.string "{JPN}{JPN}イワークに　きをつけろ。　たまにしか　でて\n"
	.string "こないがな。\p"
	.string "レベルの　たかい　やつに　しめつけられたら\n"
	.string "たいへんだぞ！$"

RockTunnel_1F_Text_OliverIntro::
	.string "Hmm.\n"
	.string "Maybe I'm lost in here…$"
	.string "{JPN}{JPN}うーん。　もしかして　まよったかな‥$"

RockTunnel_1F_Text_OliverDefeat::
	.string "Ease up!\n"
	.string "What am I doing?\l"
	.string "Which way is out?$"
	.string "{JPN}{JPN}まってくれ！　おれは　なにを　してるんだ？\n"
	.string "でぐちは　どっちだ？$"

RockTunnel_1F_Text_OliverPostBattle::
	.string "That sleeping POKéMON on ROUTE 12\n"
	.string "forced me to take this detour.$"
	.string "{JPN}{JPN}12ばんどうろで　ねむってる　ポケモンの　せいで\n"
	.string "この　まわりみちを　するはめに　なったんだ。$"

RockTunnel_1F_Text_LucasIntro::
	.string "Outsiders like you need to show me\n"
	.string "some respect!$"
	.string "{JPN}{JPN}よそものは　おれに　けいいを　はらえ！$"

RockTunnel_1F_Text_LucasDefeat::
	.string "I give!$"
	.string "{JPN}{JPN}まいった！$"

RockTunnel_1F_Text_LucasPostBattle::
	.string "You're talented enough to hike!$"
	.string "{JPN}{JPN}おまえなら　やまおとこに　なれるぞ！$"

RockTunnel_1F_Text_AshtonIntro::
	.string "POKéMON fight!\n"
	.string "Ready, go!$"
	.string "{JPN}{JPN}ポケモン　しょうぶ！　よーい　どん！$"

RockTunnel_1F_Text_AshtonDefeat::
	.string "Game over!$"
	.string "{JPN}{JPN}ゲームオーバー！$"

RockTunnel_1F_Text_AshtonPostBattle::
	.string "Oh well, I'll get a ZUBAT as I go!$"
	.string "{JPN}{JPN}まあいいや！　とちゅうで　ズバットでも\n"
	.string "つかまえよう！$"

RockTunnel_1F_Text_LeahIntro::
	.string "Eek!\p"
	.string "Don't try anything funny in the\n"
	.string "dark!$"
	.string "{JPN}{JPN}きゃっ！\p"
	.string "くらやみで　へんなこと　しないでよね！$"

RockTunnel_1F_Text_LeahDefeat::
	.string "It was too dark…$"
	.string "{JPN}{JPN}くらすぎたわ‥$"

RockTunnel_1F_Text_LeahPostBattle::
	.string "I saw a MACHOP in this tunnel.$"
	.string "{JPN}{JPN}この　トンネルで　ワンリキーを　みかけたわ。$"

RockTunnel_1F_Text_DanaIntro::
	.string "I came this far looking for wild\n"
	.string "POKéMON.$"
	.string "{JPN}{JPN}やせいの　ポケモンを　さがして　ここまで\n"
	.string "きたの。$"

RockTunnel_1F_Text_DanaDefeat::
	.string "I'm out of POKéMON!$"
	.string "{JPN}{JPN}もう　ポケモンが　いないわ！$"

RockTunnel_1F_Text_DanaPostBattle::
	.string "You looked cute and harmless.\n"
	.string "Was I ever wrong!$"
	.string "{JPN}{JPN}かわいくて　よわそうに　みえたのに‥　とんだ\n"
	.string "みこみちがい　だったわ！$"

RockTunnel_1F_Text_ArianaIntro::
	.string "You have POKéMON!\n"
	.string "Let's start!$"
	.string "{JPN}{JPN}ポケモン　もってるのね！　はじめましょ！$"

RockTunnel_1F_Text_ArianaDefeat::
	.string "You play hard!$"
	.string "{JPN}{JPN}てごわいわね！$"

RockTunnel_1F_Text_ArianaPostBattle::
	.string "Whew!\n"
	.string "I'm all sweaty now.$"
	.string "{JPN}{JPN}ふう！　あせ　びっしょりだわ。$"

RockTunnel_1F_Text_RouteSign::
	.string "ROCK TUNNEL\n"
	.string "CERULEAN CITY - LAVENDER TOWN$"
	.string "{JPN}{JPN}イワヤマトンネル\n"
	.string "ハナダシティ　ー　シオンタウン$"

