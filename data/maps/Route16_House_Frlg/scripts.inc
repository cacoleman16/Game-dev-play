Route16_House_Frlg_MapScripts::
	.byte 0

Route16_House_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM02, Route16_House_EventScript_AlreadyGotHM02
	msgbox Route16_House_Text_FoundMySecretRetreat
	checkitemspace ITEM_HM02
	goto_if_eq VAR_RESULT, FALSE, Route16_House_EventScript_NoRoomForHM02
	giveitem_msg Route16_House_Text_ReceivedHM02FromGirl, ITEM_HM02
	msgbox Route16_House_Text_ExplainHM02
	setflag FLAG_GOT_HM02
	release
	end

Route16_House_EventScript_NoRoomForHM02::
	msgbox Route16_House_Text_DontHaveAnyRoomForThis
	release
	end

Route16_House_EventScript_AlreadyGotHM02::
	msgbox Route16_House_Text_ExplainHM02
	release
	end

Route16_House_EventScript_Fearow::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_FEAROW, CRY_MODE_NORMAL
	msgbox Route16_House_Text_Fearow
	waitmoncry
	release
	end

Route16_House_Text_FoundMySecretRetreat::
	.string "Oh, dear.\n"
	.string "You've found my secret retreat.\p"
	.string "Please don't tell anyone I'm here.\n"
	.string "I'll make it up to you with this!$"
	.string "{JPN}{JPN}あら　まあ。　わたしの　ひみつの　かくれがを\n"
	.string "みつけちゃったのね。\p"
	.string "わたしが　ここに　いること　だれにも\n"
	.string "いわないでね。　おれいに　これを　あげるわ！$"

Route16_House_Text_ReceivedHM02FromGirl::
	.string "{PLAYER} received HM02\n"
	.string "from the girl.$"
	.string "{JPN}{JPN}{PLAYER}は　おんなのこから\n"
	.string "ひでんマシン02を　もらった！$"

Route16_House_Text_ExplainHM02::
	.string "HM02 is FLY.\n"
	.string "It's a wonderfully convenient move.\p"
	.string "Please, put it to good use.$"
	.string "{JPN}{JPN}ひでんマシン02は　そらをとぶ　よ。　とっても\n"
	.string "べんりな　わざなの。\p"
	.string "ぜひ　やくだてて　ちょうだい。$"

Route16_House_Text_DontHaveAnyRoomForThis::
	.string "You don't have any room for this.$"
	.string "{JPN}{JPN}にもつが　いっぱいで　もてないみたいね。$"

Route16_House_Text_Fearow::
	.string "FEAROW: Kyueen!$"
	.string "{JPN}{JPN}オニドリル『キュエーン！$"

