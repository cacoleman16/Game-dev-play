FuchsiaCity_WardensHouse_Frlg_MapScripts::
	.byte 0

FuchsiaCity_WardensHouse_EventScript_Warden::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM04, FuchsiaCity_WardensHouse_EventScript_ExplainStrength
	goto_if_set FLAG_HIDE_SAFARI_ZONE_WEST_GOLD_TEETH, FuchsiaCity_WardensHouse_EventScript_GiveGoldTeeth
	msgbox FuchsiaCity_WardensHouse_Text_HifFuffHefifoo, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, FuchsiaCity_WardensHouse_EventScript_WardenYes
	call_if_eq VAR_RESULT, NO, FuchsiaCity_WardensHouse_EventScript_WardenNo
	release
	end

FuchsiaCity_WardensHouse_EventScript_GiveGoldTeeth::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message FuchsiaCity_WardensHouse_Text_GaveGoldTeethToWarden
	waitmessage
	waitfanfare
	msgbox FuchsiaCity_WardensHouse_Text_WardenPoppedInHisTeeth
	call EventScript_RestorePrevTextColor
	checkplayergender
	call_if_eq VAR_RESULT, MALE, FuchsiaCity_WardensHouse_EventScript_WardenThanksMale
	call_if_eq VAR_RESULT, FEMALE, FuchsiaCity_WardensHouse_EventScript_WardenThanksFemale
	giveitem_msg FuchsiaCity_WardensHouse_Text_ReceivedHM04FromWarden, ITEM_HM04
	setflag FLAG_GOT_HM04
	removeitem ITEM_GOLD_TEETH
	release
	end

FuchsiaCity_WardensHouse_EventScript_WardenThanksMale::
	msgbox FuchsiaCity_WardensHouse_Text_ThanksSonGiveYouSomething
	return

FuchsiaCity_WardensHouse_EventScript_WardenThanksFemale::
	msgbox FuchsiaCity_WardensHouse_Text_ThanksLassieGiveYouSomething
	return

FuchsiaCity_WardensHouse_EventScript_WardenYes::
	msgbox FuchsiaCity_WardensHouse_Text_AhHowheeHoHoo
	return

FuchsiaCity_WardensHouse_EventScript_WardenNo::
	msgbox FuchsiaCity_WardensHouse_Text_HeOhayHeHaHoo
	return

FuchsiaCity_WardensHouse_EventScript_ExplainStrength::
	msgbox FuchsiaCity_WardensHouse_Text_ExplainStrength
	release
	end

FuchsiaCity_WardensHouse_EventScript_DisplaySign1::
	msgbox FuchsiaCity_WardensHouse_Text_MonPhotosFossilsOnDisplay, MSGBOX_SIGN
	end

FuchsiaCity_WardensHouse_EventScript_DisplaySign2::
	msgbox FuchsiaCity_WardensHouse_Text_OldMonMerchandiseOnDisplay, MSGBOX_SIGN
	end

FuchsiaCity_WardensHouse_Text_HifFuffHefifoo::
	.string "WARDEN: Hif fuff hefifoo!\p"
	.string "Ha lof ha feef ee hafahi ho.\n"
	.string "Heff hee fwee!$"
	.string "{JPN}{JPN}えんちょう:　ふがふが　ほがほが！\p"
	.string "ふぁ　ふがふが　ほがほが　ふが。　ふが　ふがー！$"

FuchsiaCity_WardensHouse_Text_AhHowheeHoHoo::
	.string "Ah howhee ho hoo!\n"
	.string "Eef ee hafahi ho!$"
	.string "{JPN}{JPN}ふぁ　ふがふが　ほが！　ふがふが　ほがほが！$"

FuchsiaCity_WardensHouse_Text_HeOhayHeHaHoo::
	.string "Ha?\n"
	.string "He ohay heh ha hoo ee haheh!$"
	.string "{JPN}{JPN}ふぁ？　ふが　ほがほが　ふが　ほがー！$"

FuchsiaCity_WardensHouse_Text_GaveGoldTeethToWarden::
	.string "{PLAYER} gave the GOLD TEETH\n"
	.string "to the WARDEN.$"
	.string "{JPN}{JPN}{PLAYER}は　えんちょうに　きんのいればを\n"
	.string "わたした。$"

FuchsiaCity_WardensHouse_Text_WardenPoppedInHisTeeth::
	.string "The WARDEN popped in his teeth!$"
	.string "{JPN}{JPN}えんちょうは　いればを　はめた！$"

FuchsiaCity_WardensHouse_Text_ThanksSonGiveYouSomething::
	.string "WARDEN: Thanks, son!\n"
	.string "You're a real lifesaver!\p"
	.string "No one could understand a word\n"
	.string "that I said, not a one!\p"
	.string "I was too ashamed to show my face\n"
	.string "around the OFFICE, even.\p"
	.string "Let me give you something for your\n"
	.string "trouble.$"
	.string "{JPN}{JPN}えんちょう:　おおきに　ぼうず！　ほんま\n"
	.string "たすかったわ！\p"
	.string "わしの　いうこと　だれも　わからへんかったんや！\p"
	.string "はずかしゅうて　じむしょにも　かお\n"
	.string "だされへんかった。\p"
	.string "おれいに　これ　やるわ。$"

FuchsiaCity_WardensHouse_Text_ThanksLassieGiveYouSomething::
	.string "WARDEN: Thank you, lassie!\n"
	.string "You're a real lifesaver!\p"
	.string "No one could understand a word\n"
	.string "that I said, not a one!\p"
	.string "I was too ashamed to show my face\n"
	.string "around the OFFICE, even.\p"
	.string "Let me give you something for your\n"
	.string "trouble.$"
	.string "{JPN}{JPN}えんちょう:　おおきに　じょうちゃん！　ほんま\n"
	.string "たすかったわ！\p"
	.string "わしの　いうこと　だれも　わからへんかったんや！\p"
	.string "はずかしゅうて　じむしょにも　かお\n"
	.string "だされへんかった。\p"
	.string "おれいに　これ　やるわ。$"

FuchsiaCity_WardensHouse_Text_ReceivedHM04FromWarden::
	.string "{PLAYER} received HM04\n"
	.string "from the WARDEN.$"
	.string "{JPN}{JPN}{PLAYER}は　えんちょうから\n"
	.string "ひでんマシン04を　もらった！$"

FuchsiaCity_WardensHouse_Text_ExplainStrength::
	.string "WARDEN: Inside HM04 you'll find\n"
	.string "STRENGTH.\p"
	.string "It lets POKéMON move boulders when\n"
	.string "you're outside of battle.\p"
	.string "Oh yes, did you find the SECRET\n"
	.string "HOUSE in the SAFARI ZONE?$"
	.string "{JPN}{JPN}えんちょう:　ひでんマシン04の　なかみは\n"
	.string "かいりきや。\p"
	.string "たたかい　いがいで　ポケモンが　おおきな　いわを\n"
	.string "うごかせる　ように　なるで。\p"
	.string "そや　サファリゾーンの　ひみつの　いえは\n"
	.string "みつけたか？$"

@ Unused
FuchsiaCity_WardensHouse_Text_YouHaveTooMuchStuff::
	.string "なんや　にもつ\n"
	.string "いっぱいやんけ！$"
	.string "{JPN}{JPN}なんや　にもつ　いっぱいやんけ！$"

FuchsiaCity_WardensHouse_Text_MonPhotosFossilsOnDisplay::
	.string "POKéMON photos and fossils are on\n"
	.string "display.$"
	.string "{JPN}{JPN}ポケモンの　しゃしんや　かせきが\n"
	.string "かざられている。$"

FuchsiaCity_WardensHouse_Text_OldMonMerchandiseOnDisplay::
	.string "Old POKéMON merchandise is on\n"
	.string "display.$"
	.string "{JPN}{JPN}むかしの　ポケモン　グッズが　かざられている。$"

