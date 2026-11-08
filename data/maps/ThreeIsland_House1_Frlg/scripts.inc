ThreeIsland_House1_Frlg_MapScripts::
	.byte 0

ThreeIsland_House1_EventScript_Lostelle::
	lock
	faceplayer
	special GetPlayerBigGuyGirlString
	msgbox ThreeIsland_House1_Text_YoureAlwaysSoCool
	release
	end

ThreeIsland_House1_EventScript_DisplayCase::
	msgbox ThreeIsland_House1_Text_RareRocksGemsDisplayed, MSGBOX_SIGN
	end

ThreeIsland_House1_Text_YoureAlwaysSoCool::
	.string "LOSTELLE: Hi, {PLAYER}!\n"
	.string "You're always so cool!\p"
	.string "I'll make lunch for you, too, one\n"
	.string "day, {PLAYER}.$"
	.string "{JPN}{JPN}ユウコ『こんにちは　{PLAYER}！　いつも\n"
	.string "かっこいいね！\p"
	.string "いつか　{PLAYER}にも　おべんとう　つくって\n"
	.string "あげるね。』$"

ThreeIsland_House1_Text_RareRocksGemsDisplayed::
	.string "Rare rocks and gems are displayed.$"
	.string "{JPN}{JPN}めずらしい　いしや　ほうせきが　かざって　ある。$"

