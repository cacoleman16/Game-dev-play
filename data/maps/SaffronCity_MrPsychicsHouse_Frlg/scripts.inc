SaffronCity_MrPsychicsHouse_Frlg_MapScripts::
	.byte 0

SaffronCity_MrPsychicsHouse_EventScript_MrPsychic::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM29_FROM_MR_PSYCHIC, SaffronCity_MrPsychicsHouse_EventScript_AlreadyGotTM29
	msgbox SaffronCity_MrPsychicsHouse_Text_YouWantedThis
	checkitemspace ITEM_TM29
	goto_if_eq VAR_RESULT, FALSE, SaffronCity_MrPsychicsHouse_EventScript_NoRoomForTM29
	giveitem_msg SaffronCity_MrPsychicsHouse_Text_ReceivedTM29FromMrPsychic, ITEM_TM29
	msgbox SaffronCity_MrPsychicsHouse_Text_ExplainTM29
	setflag FLAG_GOT_TM29_FROM_MR_PSYCHIC
	release
	end

SaffronCity_MrPsychicsHouse_EventScript_NoRoomForTM29::
	msgbox SaffronCity_MrPsychicsHouse_Text_YouveNoRoom
	release
	end

SaffronCity_MrPsychicsHouse_EventScript_AlreadyGotTM29::
	msgbox SaffronCity_MrPsychicsHouse_Text_ExplainTM29
	release
	end

SaffronCity_MrPsychicsHouse_Text_YouWantedThis::
	.string "…Wait! Don't say a word!\p"
	.string "You wanted this!$"
	.string "{JPN}{JPN}‥まった！　なにも　いうな！\p"
	.string "きみが　ほしいのは　これだろう！$"

SaffronCity_MrPsychicsHouse_Text_ReceivedTM29FromMrPsychic::
	.string "{PLAYER} received TM29\n"
	.string "from MR. PSYCHIC.$"
	.string "{JPN}{JPN}{PLAYER}は　エスパーおやじから\n"
	.string "わざマシン29を　もらった！$"

SaffronCity_MrPsychicsHouse_Text_ExplainTM29::
	.string "You already know, don't you?\n"
	.string "TM29 is PSYCHIC.\p"
	.string "It's an attack that may also lower\n"
	.string "the target's special stats.$"
	.string "{JPN}{JPN}もう　わかって　いるだろう？　わざマシン29は\n"
	.string "サイコキネシス。\p"
	.string "あいての　とくぼうを　さげる　ことも　ある\n"
	.string "こうげきわざ　だ。$"

SaffronCity_MrPsychicsHouse_Text_YouveNoRoom::
	.string "Where do you plan to put this?\n"
	.string "You've no room.$"
	.string "{JPN}{JPN}どこに　しまう　つもりだ？　もちものが　いっぱい\n"
	.string "じゃないか。$"

