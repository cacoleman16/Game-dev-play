Route1_Frlg_MapScripts::
	.byte 0

Route1_EventScript_MartClerk::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POTION_ON_ROUTE_1, Route1_EventScript_AlreadyGotPotion
	msgbox Route1_Text_WorkAtPokeMartTakeSample
	textcolor NPC_TEXT_COLOR_NEUTRAL
	checkitemspace ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	bufferitemname STR_VAR_2, ITEM_POTION
	playfanfare MUS_LEVEL_UP
	message gText_ObtainedTheItem
	waitmessage
	waitfanfare
	additem ITEM_POTION
	msgbox Route1_Text_PutPotionAway
	call EventScript_RestorePrevTextColor
	setflag FLAG_GOT_POTION_ON_ROUTE_1
	release
	end

Route1_EventScript_AlreadyGotPotion::
	msgbox Route1_Text_ComeSeeUsIfYouNeedPokeBalls
	release
	end

Route1_EventScript_Boy::
	msgbox Route1_Text_CanJumpFromLedges, MSGBOX_NPC
	end

Route1_EventScript_RouteSign::
	msgbox Route1_Text_RouteSign, MSGBOX_SIGN
	end

Route1_Text_WorkAtPokeMartTakeSample::
	.string "Hi!\n"
	.string "I work at a POKéMON MART.\p"
	.string "It's part of a convenient chain\n"
	.string "selling all sorts of items.\p"
	.string "Please, visit us in VIRIDIAN CITY.\p"
	.string "I know, I'll give you a sample.\n"
	.string "Here you go!$"
	.string "{JPN}{JPN}こんにちは！　わたし　フレンドリィショップの\n"
	.string "てんいん　です。\p"
	.string "いろんな　どうぐを　うっている　べんりな　おみせ\n"
	.string "なんですよ。\p"
	.string "トキワシティの　おみせに　ぜひ　きてくださいね。\p"
	.string "そうだ！　おためしに　これを　どうぞ！$"

Route1_Text_ComeSeeUsIfYouNeedPokeBalls::
	.string "Please come see us if you need\n"
	.string "POKé BALLS for catching POKéMON.$"
	.string "{JPN}{JPN}ポケモンを　つかまえる　モンスターボールが\n"
	.string "ひつような　ときは　ぜひ　おみせに\l"
	.string "きてくださいね。$"

Route1_Text_PutPotionAway::
	.string "{PLAYER} put the POTION away in\n"
	.string "the BAG's ITEMS POCKET.$"
	.string "{JPN}{JPN}{PLAYER}は　キズぐすりを　バッグの\n"
	.string "どうぐポケットに　しまった。$"

Route1_Text_CanJumpFromLedges::
	.string "See those ledges along the road?\p"
	.string "It's a bit scary, but you can jump\n"
	.string "from them.\p"
	.string "You can get back to PALLET TOWN\n"
	.string "quicker that way.$"
	.string "{JPN}{JPN}みちに　だんさが　あるでしょ？\p"
	.string "ちょっと　こわいけど　とびおりる　ことが\n"
	.string "できるんだ。\p"
	.string "そうすれば　マサラタウンに　はやく　もどれるよ。$"

Route1_Text_RouteSign::
	.string "ROUTE 1\n"
	.string "PALLET TOWN - VIRIDIAN CITY$"
	.string "{JPN}{JPN}1ばんどうろ\n"
	.string "マサラタウン　ー　トキワシティ$"

