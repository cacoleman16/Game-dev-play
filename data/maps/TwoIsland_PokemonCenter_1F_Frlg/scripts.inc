TwoIsland_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, TwoIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

TwoIsland_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_TWO_ISLAND
	end

TwoIsland_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

TwoIsland_PokemonCenter_1F_EventScript_GBAKid::
	msgbox TwoIsland_PokemonCenter_1F_Text_HaveYouVisitedGameCorner, MSGBOX_NPC
	end

TwoIsland_PokemonCenter_1F_EventScript_BugCatcher::
	msgbox TwoIsland_PokemonCenter_1F_Text_OldLadyLivesOutOnCape, MSGBOX_NPC
	end

TwoIsland_PokemonCenter_1F_Text_HaveYouVisitedGameCorner::
	.string "Have you visited the GAME CORNER\n"
	.string "already?\p"
	.string "It's no good to go alone.\n"
	.string "You have to take a friend along.$"
	.string "{JPN}{JPN}もう　ゲームコーナーには　いって　みた？\p"
	.string "ひとりで　いっても　だめなのよ　ともだちと\n"
	.string "いっしょに　いかなくちゃ$"

TwoIsland_PokemonCenter_1F_Text_OldLadyLivesOutOnCape::
	.string "Not many people live on\n"
	.string "TWO ISLAND.\p"
	.string "There is this old lady who lives\n"
	.string "out on the cape.\p"
	.string "She's been there since I don't know\n"
	.string "when.$"
	.string "{JPN}{JPN}2のしまに　すんでる　ひとは　あまり　いないの\p"
	.string "みさきの　ほうに　おばあさんが　ひとり　すんで\n"
	.string "いてね\p"
	.string "いつから　いるのか　わからない　くらい\n"
	.string "むかしから　いるのよ$"

