Seika_TideRoad_MapScripts::
	.byte 0

Seika_TideRoad_EventScript_SignWest::
	msgbox Seika_TideRoad_Text_SignWest, MSGBOX_SIGN
	end

Seika_TideRoad_Text_SignWest::
	.string "TIDE ROAD\n"
	.string "KILN CITY - TIDEWATCH TOWN$"
	.string "{JPN}{JPN}シオサイかいどう　カマモトシティから\n"
	.string "シオミタウンへ$"

Seika_TideRoad_EventScript_SignEast::
	msgbox Seika_TideRoad_Text_SignEast, MSGBOX_SIGN
	end

Seika_TideRoad_Text_SignEast::
	.string "TIDE ROAD\n"
	.string "TIDEWATCH TOWN - KILN CITY$"
	.string "{JPN}{JPN}シオサイかいどう　シオミタウンから\n"
	.string "カマモトシティへ$"

Seika_TideRoad_EventScript_ItemBigPearl::
	finditem ITEM_BIG_PEARL
	end

Seika_TideRoad_EventScript_Woman::
	msgbox Seika_TideRoad_Text_Woman, MSGBOX_NPC
	end

Seika_TideRoad_Text_Woman::
	.string "The tide comes in twice a day.\n"
	.string "The people of TIDEWATCH can tell\l"
	.string "the hour by it.\p"
	.string "They say their shrine keeper can\n"
	.string "feel it in her bones!$"
	.string "{JPN}{JPN}しおは　1にちに　2かい　みちて　くるの。\n"
	.string "シオミタウンの　ひとは　それで　じかんが\l"
	.string "わかるのよ。\p"
	.string "やしろの　もりびとは　からだで　しおを\n"
	.string "かんじるんだって！$"

Seika_TideRoad_EventScript_Shige::
	trainerbattle_single TRAINER_SEIKA_FISHERMAN_SHIGE, Seika_TideRoad_Text_ShigeIntro, Seika_TideRoad_Text_ShigeDefeat
	msgbox Seika_TideRoad_Text_ShigePost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_ShigeIntro::
	.string "Shh! You'll scare the REMORAID!\n"
	.string "Oh, fine. Let's battle!$"
	.string "{JPN}{JPN}シーッ！　テッポウオが　にげちまう！　ああ　もう\n"
	.string "いい。　しょうぶだ！$"

Seika_TideRoad_Text_ShigeDefeat::
	.string "They all got away...$"
	.string "{JPN}{JPN}みんな　にげちまった‥$"

Seika_TideRoad_Text_ShigePost::
	.string "OCTILLERY love the rocks under\n"
	.string "this pier. I fish here every day.$"
	.string "{JPN}{JPN}オクタンは　この　さんばしの　したの　いわが\n"
	.string "だいすき。　おれは　まいにち　ここで　つりさ。$"

Seika_TideRoad_EventScript_Dan::
	trainerbattle_single TRAINER_SEIKA_SAILOR_DAN, Seika_TideRoad_Text_DanIntro, Seika_TideRoad_Text_DanDefeat
	msgbox Seika_TideRoad_Text_DanPost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_DanIntro::
	.string "Ahoy! I just came ashore and my\n"
	.string "legs want a fight!$"
	.string "{JPN}{JPN}ヨーソロー！　おかに　あがった　ばかりで　あしが\n"
	.string "うずうず　してるぜ！$"

Seika_TideRoad_Text_DanDefeat::
	.string "Sunk, by a landlubber!$"
	.string "{JPN}{JPN}りくの　やつに　しずめられるとは！$"

Seika_TideRoad_Text_DanPost::
	.string "The sea round TIDEWATCH is calm.\n"
	.string "NAGI the diver keeps it that way.$"
	.string "{JPN}{JPN}シオミタウンの　うみは　おだやかだ。　ダイバーの\n"
	.string "ナギサが　まもって　いるからな。$"

Seika_TideRoad_EventScript_Rin::
	trainerbattle_single TRAINER_SEIKA_SWIMMER_F_RIN, Seika_TideRoad_Text_RinIntro, Seika_TideRoad_Text_RinDefeat
	msgbox Seika_TideRoad_Text_RinPost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_RinIntro::
	.string "The water is warm today! Let's\n"
	.string "make some waves!$"
	.string "{JPN}{JPN}きょうは　みずが　あったかい！　なみを\n"
	.string "おこしちゃおう！$"

Seika_TideRoad_Text_RinDefeat::
	.string "I swallowed some seawater...$"
	.string "{JPN}{JPN}うみの　みず　のんじゃった‥$"

Seika_TideRoad_Text_RinPost::
	.string "I swim to TIDEWATCH and back\n"
	.string "every morning. It's good for you!$"
	.string "{JPN}{JPN}まいあさ　シオミタウンまで　およいで　かえって\n"
	.string "くるの。　からだに　いいのよ！$"

Seika_TideRoad_EventScript_Jun::
	trainerbattle_single TRAINER_SEIKA_SWIMMER_M_JUN, Seika_TideRoad_Text_JunIntro, Seika_TideRoad_Text_JunDefeat
	msgbox Seika_TideRoad_Text_JunPost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_JunIntro::
	.string "Nobody swims faster than me in\n"
	.string "the rain!$"
	.string "{JPN}{JPN}あめの　ひに　おれより　はやく　およげる　やつは\n"
	.string "いないぜ！$"

Seika_TideRoad_Text_JunDefeat::
	.string "The tide turned on me!$"
	.string "{JPN}{JPN}しおめが　かわっちまった！$"

Seika_TideRoad_Text_JunPost::
	.string "My KINGDRA was a HORSEA from\n"
	.string "this very beach.$"
	.string "{JPN}{JPN}おれの　キングドラは　この　はまべで　つかまえた\n"
	.string "タッツー　だったんだ。$"
