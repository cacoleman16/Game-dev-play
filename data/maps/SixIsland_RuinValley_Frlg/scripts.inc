SixIsland_RuinValley_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SixIsland_RuinValley_OnLoad
	.byte 0

SixIsland_RuinValley_OnLoad::
	call_if_set FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE, SixIsland_RuinValley_EventScript_OpenDottedHoleDoor
	end

SixIsland_RuinValley_EventScript_OpenDottedHoleDoor::
	setmetatile 24, 24, METATILE_SeviiIslands67_DottedHoleDoor_Open, 0
	return

SixIsland_RuinValley_EventScript_Scientist::
	lock
	msgbox SixIsland_RuinValley_Text_CantFigureOutHowToGetInside
	applymovement LOCALID_RUIN_VALLEY_SCIENTIST, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SixIsland_RuinValley_Text_IFoundThisPlace
	applymovement LOCALID_RUIN_VALLEY_SCIENTIST, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SixIsland_RuinValley_EventScript_DottedHoleDoor::
	lockall
	goto_if_set FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE, SixIsland_RuinValley_EventScript_DottedHoleDoorOpen
	msgbox SixIsland_RuinValley_Text_CheckDoorMoreThoroughly, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SixIsland_RuinValley_EventScript_IgnoreDottedHoleDoor
	msgbox SixIsland_RuinValley_Text_SeveralDotsOnTheDoor
	braillemsgbox Braille_Text_Cut
	releaseall
	end

SixIsland_RuinValley_EventScript_DottedHoleDoorOpen::
	msgbox SixIsland_RuinValley_Text_DoorIsOpen
	releaseall
	end

SixIsland_RuinValley_EventScript_IgnoreDottedHoleDoor::
	msgbox SixIsland_RuinValley_Text_LeftDoorAlone
	releaseall
	end

SixIsland_RuinValley_Text_CantFigureOutHowToGetInside::
	.string "I can't figure out how I'm supposed\n"
	.string "to get inside.$"
	.string "{JPN}{JPN}どう　やって　なかに　はいれば　いいのか\n"
	.string "わからないんだ。$"

SixIsland_RuinValley_Text_IFoundThisPlace::
	.string "Let me tell you, I found this\n"
	.string "place.\p"
	.string "Don't look so envious, will you?$"
	.string "{JPN}{JPN}いっとくけど　ここを　みつけたのは　わたし\n"
	.string "だからね。\p"
	.string "そんなに　うらやましそうな　かお　しないでよ。$"

SixIsland_RuinValley_Text_CheckDoorMoreThoroughly::
	.string "The door doesn't budge at all.\n"
	.string "Check it more thoroughly?$"
	.string "{JPN}{JPN}とびらは　びくとも　しない。　もっと　よく\n"
	.string "しらべて　みますか？$"

SixIsland_RuinValley_Text_LeftDoorAlone::
	.string "{PLAYER} left the door alone.$"
	.string "{JPN}{JPN}{PLAYER}は　とびらから　はなれた。$"

SixIsland_RuinValley_Text_SeveralDotsOnTheDoor::
	.string "On closer inspection, there are\n"
	.string "several dots on the door…$"
	.string "{JPN}{JPN}よく　みると　とびらに　いくつか　てんてんが\n"
	.string "ある‥$"

SixIsland_RuinValley_Text_DoorIsOpen::
	.string "The door is open.$"
	.string "{JPN}{JPN}とびらは　ひらいて　いる。$"

SixIsland_RuinValley_Text_StanlyIntro::
	.string "There appear to be many secrets\n"
	.string "still unsolved in this world.$"
	.string "{JPN}{JPN}この　よのなかには　まだ　とかれて　いない\n"
	.string "なぞが　たくさん　ある　ようだ。$"

SixIsland_RuinValley_Text_StanlyDefeat::
	.string "Hmm…$"
	.string "{JPN}{JPN}ふむ‥$"

SixIsland_RuinValley_Text_StanlyPostBattle::
	.string "I suggest to you that you, too,\n"
	.string "give a thought to the secrets and\l"
	.string "mysteries of our world once a day.$"
	.string "{JPN}{JPN}きみも　1にち　1かいは　この　せかいの\n"
	.string "ひみつや　なぞに　ついて　かんがえて　みると\l"
	.string "いい。$"

SixIsland_RuinValley_Text_FosterIntro::
	.string "I'm curious, what compelled you to\n"
	.string "come here?$"
	.string "{JPN}{JPN}きみは　なにに　ひかれて　ここへ　きたのかね？$"

SixIsland_RuinValley_Text_FosterDefeat::
	.string "It's been so long since I battled,\n"
	.string "my instincts are off.$"
	.string "{JPN}{JPN}ひさしぶりの　バトルで　かんが　にぶって\n"
	.string "いたよ。$"

SixIsland_RuinValley_Text_FosterPostBattle::
	.string "There are several ruins on the\n"
	.string "SEVII ISLANDS.\p"
	.string "The majority of them date back\n"
	.string "a long way.$"
	.string "{JPN}{JPN}ナナシマには　いくつかの　いせきが　あるんだ。\p"
	.string "その　ほとんどは　とても　ふるい　じだいの\n"
	.string "ものだよ。$"

SixIsland_RuinValley_Text_LarryIntro::
	.string "It's been said that there are\n"
	.string "mysterious stones on this island.\p"
	.string "I suspect it has something to do\n"
	.string "with the ruins here.$"
	.string "{JPN}{JPN}この　しまには　ふしぎな　いしが　あると\n"
	.string "いわれて　いる。\p"
	.string "ここの　いせきと　なにか　かんけいが　あるに\n"
	.string "ちがいない。$"

SixIsland_RuinValley_Text_LarryDefeat::
	.string "Oh, interesting.$"
	.string "{JPN}{JPN}ほう　おもしろい。$"

SixIsland_RuinValley_Text_LarryPostBattle::
	.string "Those ruins over there…\p"
	.string "I haven't had any luck in finding\n"
	.string "how I can get inside.$"
	.string "{JPN}{JPN}あそこの　いせき‥\p"
	.string "どう　すれば　なかに　はいれるのか　まだ\n"
	.string "わからないんだ。$"

SixIsland_RuinValley_Text_DarylIntro::
	.string "A battle with you, so high up!$"
	.string "{JPN}{JPN}こんな　たかい　ところで　きみと　バトルだ！$"

SixIsland_RuinValley_Text_DarylDefeat::
	.string "Oh, the heartbreak of losing so\n"
	.string "easily to you!$"
	.string "{JPN}{JPN}あっさり　まけて　しまうとは‥　くやしい！$"

SixIsland_RuinValley_Text_DarylPostBattle::
	.string "Experience the joy of climbing!$"
	.string "{JPN}{JPN}やまのぼりの　よろこびを　あじわおう！$"

SixIsland_RuinValley_Text_HectorIntro::
	.string "I'm pretty familiar with the land\n"
	.string "around these parts.$"
	.string "{JPN}{JPN}この　あたりの　ちりには　かなり　くわしいんだ。$"

SixIsland_RuinValley_Text_HectorDefeat::
	.string "You don't need to be so pushy.\n"
	.string "I'll be happy to tell you.$"
	.string "{JPN}{JPN}そんなに　つよく　でなくても　いいだろう。\n"
	.string "よろこんで　おしえるよ。$"

SixIsland_RuinValley_Text_HectorPostBattle::
	.string "The door to the ruins…\n"
	.string "It has these strange patterns\l"
	.string "engraved on the outside.\p"
	.string "I don't know what they mean,\n"
	.string "though.$"
	.string "{JPN}{JPN}いせきの　とびら‥　そとがわに　ふしぎな\n"
	.string "もようが　きざまれて　いるんだ。\p"
	.string "なにを　いみ　して　いるのかは　わからない\n"
	.string "けどね。$"
