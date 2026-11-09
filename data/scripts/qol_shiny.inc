@ [QoL] The Shiny Painter: in the Viridian City and Lanternport Pokémon
@ Centers. She paints a party Pokémon shiny (the first one is free, so your
@ starter can shine from the start), washes the shine off again for free,
@ and sells the SHINY CHARM. See HACK_NOTES.md, "Shiny hunting".

	.set QOL_SHINY_PAINT_PRICE, 10000
	.set QOL_SHINY_CHARM_PRICE, 30000

QoL_EventScript_ShinyPainter::
	lock
	faceplayer
	msgbox QoL_Text_PainterHello, MSGBOX_DEFAULT
QoL_EventScript_PainterMenu::
	message QoL_Text_PainterWhat
	waitmessage
	dynmultipush QoL_Text_PainterOptPaint, 0
	call QoL_EventScript_PainterPushCharm
	dynmultipush QoL_Text_PainterOptBye, 2
	dynmultistack 0, 0, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, QoL_EventScript_PainterPaint
	case 1, QoL_EventScript_PainterCharm
QoL_EventScript_PainterBye::
	msgbox QoL_Text_PainterBye, MSGBOX_DEFAULT
	release
	end

QoL_EventScript_PainterPushCharm::
	checkitem ITEM_SHINY_CHARM
	goto_if_eq VAR_RESULT, TRUE, Common_EventScript_NopReturn
	dynmultipush QoL_Text_PainterOptCharm, 1
	return

QoL_EventScript_PainterPaint::
	msgbox QoL_Text_PainterWhich, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	special QoL_PainterCheck
	goto_if_eq VAR_RESULT, 2, QoL_EventScript_PainterMenu
	goto_if_eq VAR_RESULT, 1, QoL_EventScript_PainterWash
	goto_if_unset FLAG_QOL_SHINY_FREE_PAINT, QoL_EventScript_PainterFree
	showmoneybox 0, 0
	msgbox QoL_Text_PainterPrice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_EventScript_PainterNoDeal
	checkmoney QOL_SHINY_PAINT_PRICE
	goto_if_eq VAR_RESULT, FALSE, QoL_EventScript_PainterNoMoney
	removemoney QOL_SHINY_PAINT_PRICE
	updatemoneybox
	playse SE_SHOP
	waitse
	hidemoneybox
	goto QoL_EventScript_PainterDoPaint

QoL_EventScript_PainterFree::
	msgbox QoL_Text_PainterFree, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_EventScript_PainterMenu
	setflag FLAG_QOL_SHINY_FREE_PAINT
QoL_EventScript_PainterDoPaint::
	msgbox QoL_Text_PainterWork, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_WHITE
	setvar VAR_0x8005, TRUE
	special QoL_PainterSet
	updatefollowingmon
	delay 20
	fadescreen FADE_FROM_WHITE
	playse SE_SHINY
	waitse
	msgbox QoL_Text_PainterDone, MSGBOX_DEFAULT
	goto QoL_EventScript_PainterMenu

QoL_EventScript_PainterWash::
	msgbox QoL_Text_PainterAlready, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_EventScript_PainterMenu
	closemessage
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8005, FALSE
	special QoL_PainterSet
	updatefollowingmon
	delay 20
	fadescreen FADE_FROM_BLACK
	msgbox QoL_Text_PainterWashed, MSGBOX_DEFAULT
	goto QoL_EventScript_PainterMenu

QoL_EventScript_PainterNoDeal::
	hidemoneybox
	goto QoL_EventScript_PainterMenu

QoL_EventScript_PainterNoMoney::
	hidemoneybox
	msgbox QoL_Text_PainterNoMoney, MSGBOX_DEFAULT
	goto QoL_EventScript_PainterMenu

QoL_EventScript_PainterCharm::
	showmoneybox 0, 0
	msgbox QoL_Text_PainterCharmPrice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_EventScript_PainterNoDeal
	checkmoney QOL_SHINY_CHARM_PRICE
	goto_if_eq VAR_RESULT, FALSE, QoL_EventScript_PainterNoMoney
	removemoney QOL_SHINY_CHARM_PRICE
	updatemoneybox
	playse SE_SHOP
	waitse
	hidemoneybox
	giveitem ITEM_SHINY_CHARM
	msgbox QoL_Text_PainterCharmInfo, MSGBOX_DEFAULT
	goto QoL_EventScript_PainterMenu

QoL_Text_PainterHello::
	.string "Ah, a customer! I'm the SHINY\n"
	.string "PAINTER.\p"
	.string "My star-dust paint makes any\n"
	.string "POKéMON shine like a shiny one.$"
	.string "{JPN}{JPN}おや　おきゃくさん！　わたしは　いろちがい\n"
	.string "ペインター。\p"
	.string "ほしの　こなの　えのぐで　どんな　ポケモンも\n"
	.string "いろちがいみたいに　かがやかせて　あげるわ。$"

QoL_Text_PainterWhat::
	.string "What can I do for you?$"
	.string "{JPN}{JPN}なにに　する？$"

QoL_Text_PainterOptPaint::
	.string "PAINT A POKéMON$"
	.string "{JPN}{JPN}ポケモンを　ぬる$"

QoL_Text_PainterOptCharm::
	.string "SHINY CHARM$"
	.string "{JPN}{JPN}ひかるおまもり$"

QoL_Text_PainterOptBye::
	.string "NO THANKS$"
	.string "{JPN}{JPN}やめておく$"

QoL_Text_PainterWhich::
	.string "Which POKéMON shall I paint?$"
	.string "{JPN}{JPN}どの　ポケモンを　ぬる？$"

QoL_Text_PainterFree::
	.string "Your first one is on the house!\n"
	.string "Paint {STR_VAR_1}?$"
	.string "{JPN}{JPN}さいしょの　1ぴきは　ただよ！\n"
	.string "{STR_VAR_1}を　ぬる？$"

QoL_Text_PainterPrice::
	.string "That will be ¥10,000 to paint\n"
	.string "{STR_VAR_1}. All right?$"
	.string "{JPN}{JPN}{STR_VAR_1}を　ぬるのは\n"
	.string "10000えんよ。\l"
	.string "いい？$"

QoL_Text_PainterWork::
	.string "Hold still, {STR_VAR_1}…\n"
	.string "A little star dust here…$"
	.string "{JPN}{JPN}{STR_VAR_1}　じっとしてて‥\n"
	.string "ほしの　こなを　ちょっと‥$"

QoL_Text_PainterDone::
	.string "There! {STR_VAR_1} is shiny now.\n"
	.string "It suits it!$"
	.string "{JPN}{JPN}できた！　{STR_VAR_1}は　いろちがいに\n"
	.string "なったわ。\l"
	.string "よく　にあってる！$"

QoL_Text_PainterAlready::
	.string "{STR_VAR_1} already shines!\p"
	.string "Shall I wash the shine off?\n"
	.string "That one's free.$"
	.string "{JPN}{JPN}{STR_VAR_1}は　もう　かがやいてるわ！\p"
	.string "かがやきを　あらいおとす？\n"
	.string "それは　ただで　いいわよ。$"

QoL_Text_PainterWashed::
	.string "All washed. {STR_VAR_1} looks just\n"
	.string "like it used to.$"
	.string "{JPN}{JPN}あらいおわったわ。　{STR_VAR_1}は\n"
	.string "もとどおりよ。$"

QoL_Text_PainterNoMoney::
	.string "Oh dear, you don't have enough\n"
	.string "money.$"
	.string "{JPN}{JPN}あら　おかねが　たりないみたい。$"

QoL_Text_PainterCharmPrice::
	.string "The SHINY CHARM makes shiny\n"
	.string "POKéMON easier to find in the wild.\p"
	.string "It's ¥30,000. Would you like it?$"
	.string "{JPN}{JPN}ひかるおまもりが　あると　やせいの　いろちがいに\n"
	.string "であいやすく　なるの。\p"
	.string "30000えんよ。　かう？$"

QoL_Text_PainterCharmInfo::
	.string "Keep it in your BAG and shiny\n"
	.string "POKéMON will turn up more often.$"
	.string "{JPN}{JPN}バッグに　いれて　おけば　いろちがいの\n"
	.string "ポケモンが　よく　でてくるわ。$"

QoL_Text_PainterBye::
	.string "Come back when you want some\n"
	.string "sparkle!$"
	.string "{JPN}{JPN}かがやきが　ほしく　なったら　また　きてね！$"
