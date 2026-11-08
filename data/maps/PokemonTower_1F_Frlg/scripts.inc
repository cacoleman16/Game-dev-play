PokemonTower_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_1F_OnTransition
	.byte 0

PokemonTower_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POKEMON_TOWER_1F
	end

PokemonTower_1F_EventScript_Channeler::
	msgbox PokemonTower_1F_Text_SenseSpiritsUpToMischief, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman1::
	msgbox PokemonTower_1F_Text_GrowlitheWhyDidYouDie, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_BaldingMan::
	msgbox PokemonTower_1F_Text_CameToPrayForDepartedClefairy, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman2::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, PokemonTower_1F_EventScript_Woman2MalePlayer
	msgbox PokemonTower_1F_Text_ComeToPayRespectsGirl
	release
	end

PokemonTower_1F_EventScript_Woman2MalePlayer::
	msgbox PokemonTower_1F_Text_ComeToPayRespectsSon
	release
	end

PokemonTower_1F_EventScript_WorkerF::
	msgbox PokemonTower_1F_Text_ErectedInMemoryOfDeadMons, MSGBOX_NPC
	end

PokemonTower_1F_Text_ErectedInMemoryOfDeadMons::
	.string "POKéMON TOWER was erected in the\n"
	.string "memory of POKéMON that died.$"
	.string "{JPN}{JPN}ポケモンタワーは　なくなった　ポケモンたちを\n"
	.string "とむらう　ために　たてられたんだ。$"

PokemonTower_1F_Text_ComeToPayRespectsSon::
	.string "Did you come to pay respects?\p"
	.string "Bless your POKéMON-loving heart,\n"
	.string "son.$"
	.string "{JPN}{JPN}おまいりに　きたのかい？\p"
	.string "ポケモンを　だいじに　する　やさしい　こだね\n"
	.string "ぼうや。$"

PokemonTower_1F_Text_ComeToPayRespectsGirl::
	.string "Did you come to pay respects?\p"
	.string "Bless your POKéMON-loving heart,\n"
	.string "girl.$"
	.string "{JPN}{JPN}おまいりに　きたのかい？\p"
	.string "ポケモンを　だいじに　する　やさしい　こだね\n"
	.string "おじょうちゃん。$"

PokemonTower_1F_Text_CameToPrayForDepartedClefairy::
	.string "I came to pray for my dearly\n"
	.string "departed CLEFAIRY.\p"
	.string "Sniff!\n"
	.string "I'm awash in tears…$"
	.string "{JPN}{JPN}しんで　しまった　ピッピの　ために　おいのりに\n"
	.string "きたの。\p"
	.string "ぐすん！　なみだが　とまらないわ‥$"

PokemonTower_1F_Text_GrowlitheWhyDidYouDie::
	.string "My GROWLITHE…\n"
	.string "Why did you die?$"
	.string "{JPN}{JPN}わたしの　ガーディ‥　どうして　しんじゃったの？$"

PokemonTower_1F_Text_SenseSpiritsUpToMischief::
	.string "I am a CHANNELER.\p"
	.string "There are spirits up to mischief.\n"
	.string "I sense them high in the TOWER.$"
	.string "{JPN}{JPN}わたしは　きとうしです。\p"
	.string "いたずらを　する　れいが　いますね。　タワーの\n"
	.string "うえの　ほうに　けはいを　かんじます。$"

