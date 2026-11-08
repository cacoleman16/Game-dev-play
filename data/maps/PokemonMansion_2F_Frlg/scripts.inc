PokemonMansion_2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, PokemonMansion_2F_OnLoad
	.byte 0

PokemonMansion_2F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, PokemonMansion_EventScript_PressSwitch_2F
	end

PokemonMansion_2F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 1
	call PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

PokemonMansion_2F_EventScript_DiaryJuly5th::
	msgbox PokemonMansion_1F_Text_NewMonDiscoveredInGuyanaJungle, MSGBOX_SIGN
	end

PokemonMansion_2F_EventScript_DiaryJuly10th::
	msgbox PokemonMansion_1F_Text_ChristenedDiscoveredMonMew, MSGBOX_SIGN
	end

PokemonMansion_2F_EventScript_Arnie::
	trainerbattle_single TRAINER_BURGLAR_ARNIE, PokemonMansion_1F_Text_ArnieIntro, PokemonMansion_1F_Text_ArnieDefeat
	msgbox PokemonMansion_1F_Text_ArniePostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_Text_PressSecretSwitch::
	.string "A secret switch!\p"
	.string "Press it?$"
	.string "{JPN}{JPN}ひみつの　スイッチだ！\p"
	.string "おして　みますか？$"

PokemonMansion_Text_WhoWouldnt::
	.string "Who wouldn't?$"
	.string "{JPN}{JPN}おすに　きまってる！$"

PokemonMansion_Text_NotQuiteYet::
	.string "Not quite yet!$"
	.string "{JPN}{JPN}いまは　やめて　おこう！$"

PokemonMansion_1F_Text_ArnieIntro::
	.string "I can't get out!\n"
	.string "This old place is one big puzzle.$"
	.string "{JPN}{JPN}でられない！　この　やしきは　まるで　おおきな\n"
	.string "パズルだ。$"

PokemonMansion_1F_Text_ArnieDefeat::
	.string "Oh, no!\n"
	.string "My bag of loot!$"
	.string "{JPN}{JPN}ああっ！　おれの　おたからが！$"

PokemonMansion_1F_Text_ArniePostBattle::
	.string "The switches here open and close\n"
	.string "alternating sets of doors.$"
	.string "{JPN}{JPN}ここの　スイッチは　おす　たびに　べつの\n"
	.string "とびらが　あいたり　しまったり　するんだ。$"

PokemonMansion_1F_Text_NewMonDiscoveredInGuyanaJungle::
	.string "Diary: July 5\n"
	.string "Guyana, South America\p"
	.string "A new POKéMON was discovered deep\n"
	.string "in the jungle.$"
	.string "{JPN}{JPN}にっし:7がつ　5か\n"
	.string "みなみアメリカ　ギアナ\p"
	.string "ジャングルの　おくちで　あたらしい　ポケモンが\n"
	.string "はっけん　された。$"

PokemonMansion_1F_Text_ChristenedDiscoveredMonMew::
	.string "Diary: July 10\n"
	.string "We christened the newly discovered\l"
	.string "POKéMON, MEW.$"
	.string "{JPN}{JPN}にっし:7がつ　10か\n"
	.string "はっけん　した　しんしゅの　ポケモンを　ミュウと\l"
	.string "なづけた。$"

