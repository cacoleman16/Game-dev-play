Seika_KilnCity_Workshop_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_KilnCity_Workshop_OnTransition
	.byte 0

Seika_KilnCity_Workshop_EventScript_Apprentice::
	msgbox Seika_KilnCity_Workshop_Text_Apprentice, MSGBOX_NPC
	end

Seika_KilnCity_Workshop_Text_Apprentice::
	.string "We fire the glaze three times.\n"
	.string "Once for the clay, once for the\l"
	.string "color, once for luck.$"
	.string "{JPN}{JPN}うわぐすりは　3ど　やくんです。　ねんどの\n"
	.string "ために　1ど　いろの　ために　1ど　うんの\l"
	.string "ために　1ど。$"

Seika_KilnCity_Workshop_EventScript_DisplayBowls::
	msgbox Seika_KilnCity_Workshop_Text_DisplayBowls, MSGBOX_SIGN
	end

Seika_KilnCity_Workshop_EventScript_DisplayCase::
	lockall
	goto_if_set FLAG_QOL_SEIKA_DENEB_KILN, Seika_KilnCity_Workshop_EventScript_DisplayEmpty
	msgbox Seika_KilnCity_Workshop_Text_DisplayBell, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Workshop_EventScript_DisplayEmpty::
	msgbox Seika_KilnCity_Workshop_Text_DisplayEmpty, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Workshop_Text_DisplayBowls::
	.string "Tea bowls glazed the color of the\n"
	.string "night sky.\p"
	.string "A tag on them says: NOT FOR SALE.$"
	.string "{JPN}{JPN}よぞらの　いろの　ちゃわんが　ならんで　いる。\p"
	.string "ふだには　「ひばいひん」と　かいて　ある。$"

Seika_KilnCity_Workshop_Text_DisplayBell::
	.string "A small bell of clear, glittering\n"
	.string "glass. It hums when you lean close.$"
	.string "{JPN}{JPN}きらきら　ひかる　ちいさな　ガラスの　かね。\n"
	.string "かおを　よせると　かすかに　うなって　いる。$"

Seika_KilnCity_Workshop_Text_DisplayEmpty::
	.string "The case is empty. A ring in the\n"
	.string "dust shows where a bell once sat.$"
	.string "{JPN}{JPN}ケースは　からっぽだ。　ほこりの　なかに　かねが\n"
	.string "おかれて　いた　わの　あとが　ある。$"

Seika_KilnCity_Workshop_OnTransition::
	call_if_set FLAG_QOL_SEIKA_DENEB_KILN, Seika_KilnCity_Workshop_EventScript_SceneDone
	end

Seika_KilnCity_Workshop_EventScript_SceneDone::
	setvar VAR_TEMP_1, 1
	return

Seika_KilnCity_Workshop_EventScript_DenebScene::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox Seika_KilnCity_Workshop_Text_DenebBuys, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Seika_KilnCity_Workshop_Text_MasterWorried, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_SEIKA_KILN_CITY_WORKSHOP_DENEB, Common_Movement_FaceDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_SEIKA_KILN_CITY_WORKSHOP_DENEB, Common_Movement_ExclamationMark
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox Seika_KilnCity_Workshop_Text_DenebIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_DENEB_KILN, Seika_KilnCity_Workshop_Text_DenebDefeat
	msgbox Seika_KilnCity_Workshop_Text_DenebAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_KILN_CITY_WORKSHOP_DENEB
	setflag FLAG_QOL_SEIKA_HIDE_DENEB_KILN
	setflag FLAG_QOL_SEIKA_DENEB_KILN
	setvar VAR_TEMP_1, 1
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Seika_KilnCity_Workshop_Text_MasterAfter, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Workshop_EventScript_Deneb::
	lock
	faceplayer
	msgbox Seika_KilnCity_Workshop_Text_DenebBuys, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Workshop_EventScript_Master::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_DENEB_KILN, Seika_KilnCity_Workshop_EventScript_MasterLater
	msgbox Seika_KilnCity_Workshop_Text_MasterWorried, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Workshop_EventScript_MasterLater::
	msgbox Seika_KilnCity_Workshop_Text_MasterLater, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Workshop_Text_DenebBuys::
	.string "DENEB: Then it is settled.\p"
	.string "The STARGLASS BELL belongs to\n"
	.string "TEAM NOVA now. The gold is on\l"
	.string "your table, master.$"
	.string "{JPN}{JPN}デネブ『それじゃ　きまりね。\p"
	.string "ほしガラスのかねは　これから　ノバだんの　もの。\n"
	.string "おかねは　テーブルの　うえよ　おやかたさん。』$"

Seika_KilnCity_Workshop_Text_MasterWorried::
	.string "MASTER: But that bell was never\n"
	.string "meant to be sold...\p"
	.string "Our first masters fired it to\n"
	.string "answer the star when it fell.$"
	.string "{JPN}{JPN}おやかた『しかし　あの　かねは　うる　ための\n"
	.string "ものでは　ない‥\p"
	.string "はじまりの　めいじんたちが　ほしが　おちた　とき\n"
	.string "こたえる　ために　やいた　かね　なのじゃ。』$"

Seika_KilnCity_Workshop_Text_DenebIntro::
	.string "Oh? {PLAYER}. Of course it is you.\p"
	.string "You followed us from KANTO all\n"
	.string "the way to SEIKA. How loyal.\p"
	.string "The old songs say this bell can\n"
	.string "wake the WISHING STAR.\p"
	.string "We paid a fair price for it.\n"
	.string "But if you insist on a quarrel...$"
	.string "{JPN}{JPN}デネブ『あら？　{PLAYER}。　やっぱり\n"
	.string "あなたね　ダーリン。\p"
	.string "カントーから　はるばる　セイカまで　おいかけて\n"
	.string "くるなんて。　いちずなのね。\p"
	.string "ふるい　うたでは　この　かねが　ねがいぼしを\n"
	.string "めざめさせるんですって。\p"
	.string "ちゃんと　おかねは　はらったのよ。　でも\n"
	.string "どうしても　けんかが　したいなら‥』$"

Seika_KilnCity_Workshop_Text_DenebDefeat::
	.string "Hmph. You fight as if the stars\n"
	.string "themselves were on your side.$"
	.string "{JPN}{JPN}ふん。　まるで　ほしが　あなたの　みかたを\n"
	.string "してる　みたいね。$"

Seika_KilnCity_Workshop_Text_DenebAfter::
	.string "It changes nothing, {PLAYER}.\p"
	.string "The bell is already on its way\n"
	.string "to the mountain.\p"
	.string "VEGA will have her wish.\n"
	.string "Good day.$"
	.string "{JPN}{JPN}デネブ『なにも　かわらないわよ　{PLAYER}。\p"
	.string "かねは　もう　やまへ　むかって　いるもの。\p"
	.string "ベガさまの　ねがいは　かなうわ。　ごきげんよう\n"
	.string "ダーリン。』$"

Seika_KilnCity_Workshop_Text_MasterAfter::
	.string "MASTER: She left with her people.\n"
	.string "Gold on my table, and the bell\l"
	.string "gone from its case.\p"
	.string "If they ring it on MT. SKYREACH,\n"
	.string "the star will answer them...\p"
	.string "Please, young one. Do what you\n"
	.string "can. Win the BADGES and follow\l"
	.string "her to the mountain.$"
	.string "{JPN}{JPN}おやかた『あの　おんなは　なかまと　でて　いって\n"
	.string "しもうた。　テーブルに　きんかを　のこし　かねは\l"
	.string "ケースから　きえた。\p"
	.string "ソラトドキやまで　あれを　ならされたら　ほしは\n"
	.string "やつらに　こたえて　しまう‥\p"
	.string "たのむ　わかいの。　バッジを　あつめて　あの\n"
	.string "おんなを　やままで　おって　くれ。　できる\l"
	.string "かぎりの　ことを。』$"

Seika_KilnCity_Workshop_Text_MasterLater::
	.string "MASTER: The STARGLASS BELL is\n"
	.string "gone. Our oldest treasure...\p"
	.string "Go on, young one. The road to\n"
	.string "MT. SKYREACH is long.$"
	.string "{JPN}{JPN}おやかた『ほしガラスのかねが　きえて　しもうた。\n"
	.string "わしらの　いちばん　ふるい　たからが‥\p"
	.string "さあ　ゆけ　わかいの。　ソラトドキやまへの\n"
	.string "みちは　ながいぞ。』$"
