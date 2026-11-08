CeladonCity_Restaurant_Frlg_MapScripts::
	.byte 0

CeladonCity_Restaurant_EventScript_Chef::
	msgbox CeladonCity_Restaurant_Text_TakingBreakRightNow, MSGBOX_NPC
	end

CeladonCity_Restaurant_EventScript_Woman::
	msgbox CeladonCity_Restaurant_Text_OftenGoToDrugstore, MSGBOX_NPC
	end

CeladonCity_Restaurant_EventScript_CoinCaseMan::
	lock
	faceplayer
	goto_if_set FLAG_GOT_COIN_CASE, CeladonCity_Restaurant_EventScript_AlreadyGotCoinCase
	msgbox CeladonCity_Restaurant_Text_TakeThisImBusted
	checkitemspace ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_Restaurant_EventScript_NoRoomForCoinCase
	giveitem_msg CeladonCity_Restaurant_Text_ReceivedCoinCaseFromMan, ITEM_COIN_CASE
	setflag FLAG_GOT_COIN_CASE
	release
	end

CeladonCity_Restaurant_EventScript_NoRoomForCoinCase::
	msgbox CeladonCity_Restaurant_Text_MakeRoomForThis
	release
	end

CeladonCity_Restaurant_EventScript_AlreadyGotCoinCase::
	msgbox CeladonCity_Restaurant_Text_ThoughtIdWinItBack
	release
	end

CeladonCity_Restaurant_EventScript_WorkerM::
	msgbox CeladonCity_Restaurant_Text_PsstBasementUnderGameCorner, MSGBOX_NPC
	end

CeladonCity_Restaurant_EventScript_FatMan::
	msgbox CeladonCity_Restaurant_Text_ManLostItAllAtSlots, MSGBOX_NPC
	end

CeladonCity_Restaurant_Text_TakingBreakRightNow::
	.string "Hi!\p"
	.string "Sorry, but we're taking a break\n"
	.string "right now.$"
	.string "{JPN}{JPN}いらっしゃい！\p"
	.string "ごめんなさい　いまは　きゅうけい　ちゅうなんです$"

CeladonCity_Restaurant_Text_OftenGoToDrugstore::
	.string "My POKéMON are weak, so I often\n"
	.string "have to go to the DRUGSTORE.$"
	.string "{JPN}{JPN}ぼくの　ポケモン　よわいから　しょっちゅう\n"
	.string "ドラッグストアに　いかなきゃ　ならないんだ$"

CeladonCity_Restaurant_Text_PsstBasementUnderGameCorner::
	.string "Psst! There's a basement under the\n"
	.string "GAME CORNER, I hear.$"
	.string "{JPN}{JPN}ねえ　ねえ！　ゲームコーナーの　したには\n"
	.string "ちかしつが　あるんだって$"

CeladonCity_Restaurant_Text_ManLostItAllAtSlots::
	.string "Munch…\p"
	.string "The man at that table lost it all\n"
	.string "at the slots.$"
	.string "{JPN}{JPN}もぐもぐ‥\p"
	.string "あっちの　テーブルの　ひと　スロットで　ぜんぶ\n"
	.string "すっちゃったんだって$"

CeladonCity_Restaurant_Text_TakeThisImBusted::
	.string "Go ahead! Laugh!\n"
	.string "I'm flat-out busted!\p"
	.string "No more slots for me!\n"
	.string "I'm going straight!\p"
	.string "Here!\n"
	.string "I won't be needing this anymore!$"
	.string "{JPN}{JPN}わらえよ！　わらいたきゃ　わらえ！\n"
	.string "すっからかんだ！\p"
	.string "もう　スロットは　やめだ！　まじめに　いきるぜ！\p"
	.string "ほら！　これは　もう　いらねえ！$"

CeladonCity_Restaurant_Text_ReceivedCoinCaseFromMan::
	.string "{PLAYER} received a COIN CASE\n"
	.string "from the man.$"
	.string "{JPN}{JPN}{PLAYER}は　おとこから　コインケースを\n"
	.string "もらった！$"

CeladonCity_Restaurant_Text_MakeRoomForThis::
	.string "Make room for this!$"
	.string "{JPN}{JPN}にもつを　へらして　こいよ！$"

CeladonCity_Restaurant_Text_ThoughtIdWinItBack::
	.string "I always thought I was going to\n"
	.string "win it back…$"
	.string "{JPN}{JPN}いつかは　とりかえせると　おもってたんだがな‥$"

