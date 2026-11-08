FiveIsland_Meadow_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, FiveIsland_Meadow_OnLoad
	.byte 0

FiveIsland_Meadow_OnLoad::
	call_if_set FLAG_UNLOCKED_ROCKET_WAREHOUSE, FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked
	end

FiveIsland_Meadow_EventScript_WarehouseDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_ROCKET_WAREHOUSE, FiveIsland_Meadow_EventScript_WarehouseDoorAlreadyOpen
	goto_if_set FLAG_LEARNED_YES_NAH_CHANSEY, FiveIsland_Meadow_EventScript_OpenWarehouseDoor
	msgbox FiveIsland_Meadow_Text_EnteredPasswordAnotherNeeded
	releaseall
	end

FiveIsland_Meadow_EventScript_OpenWarehouseDoor::
	setflag FLAG_UNLOCKED_ROCKET_WAREHOUSE
	message FiveIsland_Meadow_Text_EnteredPasswordDoorOpened
	waitmessage
	delay 60
	call FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked
	special DrawWholeMapView
	playse SE_PIN
	waitse
	releaseall
	end

FiveIsland_Meadow_EventScript_WarehouseDoorAlreadyOpen::
	msgbox FiveIsland_Meadow_Text_WarehouseDoorAlreadyOpen
	releaseall
	end

FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked::
	setmetatile 12, 21, METATILE_SeviiIslands45_RocketWarehouseDoor_Unlocked, 1
	return

FiveIsland_Meadow_EventScript_Rocket1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_49, FiveIsland_Meadow_Text_Rocket1Intro, FiveIsland_Meadow_Text_Rocket1Defeat
	msgbox FiveIsland_Meadow_Text_Rocket1PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_Meadow_EventScript_Rocket2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_50, FiveIsland_Meadow_Text_Rocket2Intro, FiveIsland_Meadow_Text_Rocket2Defeat
	msgbox FiveIsland_Meadow_Text_Rocket2PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_Meadow_EventScript_Rocket3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_51, FiveIsland_Meadow_Text_Rocket3Intro, FiveIsland_Meadow_Text_Rocket3Defeat
	msgbox FiveIsland_Meadow_Text_Rocket3PostBattle, MSGBOX_AUTOCLOSE
	end

@ Unused
FiveIsland_Meadow_Text_DoorNeedsTwoPasswords::
	.string "とびらを　あけるには\n"
	.string "パスワードが　2つ　いるようだ$"
	.string "{JPN}{JPN}とびらを　あけるには　パスワードが　2つ\n"
	.string "いるようだ$"

FiveIsland_Meadow_Text_EnteredPasswordAnotherNeeded::
	.string "{PLAYER} entered a password.\p"
	.string "“GOLDEEN need log.”\p"
	.string "… …  … …  … …\p"
	.string "Another password is needed to\n"
	.string "open the door…$"
	.string "{JPN}{JPN}{PLAYER}は　パスワードを　にゅうりょく\n"
	.string "した\p"
	.string "『トサキントに　まるたが　ひつよう』\p"
	.string "‥　‥　‥　‥　‥　‥\p"
	.string "とびらを　あけるには　もう　ひとつ　パスワードが\n"
	.string "いるようだ‥$"

FiveIsland_Meadow_Text_EnteredPasswordDoorOpened::
	.string "{PLAYER} entered two passwords.\p"
	.string "“GOLDEEN need log.”\n"
	.string "“Yes, nah, CHANSEY.”\p"
	.string "… …  … …  … …\p"
	.string "The WAREHOUSE's door opened!$"
	.string "{JPN}{JPN}{PLAYER}は　2つの　パスワードを\n"
	.string "にゅうりょく　した\p"
	.string "『トサキントに　まるたが　ひつよう』\n"
	.string "『うん　いや　ラッキー』\p"
	.string "‥　‥　‥　‥　‥　‥\p"
	.string "そうこの　とびらが　ひらいた！$"

FiveIsland_Meadow_Text_WarehouseDoorAlreadyOpen::
	.string "The WAREHOUSE's door is already\n"
	.string "open.$"
	.string "{JPN}{JPN}そうこの　とびらは　もう　ひらいている$"

FiveIsland_Meadow_Text_Rocket1Intro::
	.string "This area's strictly off-limits!\n"
	.string "Out, out!$"
	.string "{JPN}{JPN}ここは　たちいり　きんしだ！　でていけ\n"
	.string "でていけ！$"

FiveIsland_Meadow_Text_Rocket1Defeat::
	.string "Is this for real?$"
	.string "{JPN}{JPN}うそだろ？$"

FiveIsland_Meadow_Text_Rocket1PostBattle::
	.string "You don't have any business here!\n"
	.string "Go home!$"
	.string "{JPN}{JPN}おまえには　ようの　ない　ところだ！　かえれ！$"

FiveIsland_Meadow_Text_Rocket2Intro::
	.string "There's nothing past here except\n"
	.string "our WAREHOUSE.\p"
	.string "So what brings you here?$"
	.string "{JPN}{JPN}この　さきには　おれたちの　そうこしか　ないぞ\p"
	.string "なにしに　きたんだ？$"

FiveIsland_Meadow_Text_Rocket2Defeat::
	.string "That wasn't supposed to happen…$"
	.string "{JPN}{JPN}こんな　はずじゃ‥$"

FiveIsland_Meadow_Text_Rocket2PostBattle::
	.string "You're wanting to know what's inside\n"
	.string "this WAREHOUSE?\p"
	.string "It's better you didn't know.\n"
	.string "For your own sake, I mean.$"
	.string "{JPN}{JPN}そうこの　なかみが　しりたいのか？\p"
	.string "しらない　ほうが　いい　おまえの　ためにもな$"

FiveIsland_Meadow_Text_Rocket3Intro::
	.string "You'd be wasting time even if you\n"
	.string "were to get by me.$"
	.string "{JPN}{JPN}おれを　たおした　ところで　じかんの　むだだぜ$"

FiveIsland_Meadow_Text_Rocket3Defeat::
	.string "Oh, that makes me furious!$"
	.string "{JPN}{JPN}ああ　むかつく！$"

FiveIsland_Meadow_Text_Rocket3PostBattle::
	.string "You won't be able to get in without\n"
	.string "the passwords anyway.$"
	.string "{JPN}{JPN}どうせ　パスワードが　なけりゃ　なかには\n"
	.string "はいれないぜ$"

