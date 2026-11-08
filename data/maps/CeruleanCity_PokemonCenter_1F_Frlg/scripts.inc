CeruleanCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

CeruleanCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_CERULEAN_CITY
	end

CeruleanCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

CeruleanCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox CeruleanCity_PokemonCenter_1F_Text_EveryoneCallsBillPokemaniac, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_EventScript_Rocker::
	msgbox CeruleanCity_PokemonCenter_1F_Text_BillDoesWhateverForRareMons, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_EventScript_Youngster::
	lock
	faceplayer
	famechecker FAMECHECKER_BILL, 1
	msgbox CeruleanCity_PokemonCenter_1F_Text_BillCollectsRareMons
	release
	end

CeruleanCity_PokemonCenter_1F_EventScript_Lass::
	msgbox CeruleanCity_PokemonCenter_1F_Text_TryTradingUpstairs, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_Text_BillDoesWhateverForRareMons::
	.string "That BILL!\p"
	.string "I heard that he'll do whatever it\n"
	.string "takes to get rare POKéMON.\p"
	.string "He's not above doing all sorts\n"
	.string "of things, I've heard.$"
	.string "{JPN}{JPN}あの　マサキって　やつ！\p"
	.string "めずらしい　ポケモンの　ためなら　なんでも\n"
	.string "するらしいぜ\p"
	.string "どんな　ことでも　へいきで　やるって　うわさだ$"

CeruleanCity_PokemonCenter_1F_Text_EveryoneCallsBillPokemaniac::
	.string "Have you heard about BILL?\p"
	.string "Everyone calls him a POKéMANIAC!\p"
	.string "I think people are just jealous of\n"
	.string "BILL, though.\p"
	.string "Who wouldn't want to boast about\n"
	.string "their POKéMON?$"
	.string "{JPN}{JPN}マサキの　こと　きいた　ことある？\p"
	.string "みんな　あいつを　ポケモンマニアって　よぶんだ！\p"
	.string "でも　それって　マサキが　うらやましい　だけだと\n"
	.string "おもうな\p"
	.string "だれだって　じぶんの　ポケモンを　じまん\n"
	.string "したいもんね$"

CeruleanCity_PokemonCenter_1F_Text_BillCollectsRareMons::
	.string "BILL has lots of POKéMON!\n"
	.string "He collects rare ones, too!$"
	.string "{JPN}{JPN}マサキは　ポケモンを　たくさん　もってるんだ！\n"
	.string "めずらしい　ポケモンも　あつめてるよ！$"

CeruleanCity_PokemonCenter_1F_Text_TryTradingUpstairs::
	.string "Why don't you go upstairs and try\n"
	.string "trading POKéMON with your friends?\p"
	.string "You could get a lot more variety\n"
	.string "by trading.\p"
	.string "The POKéMON you get in trades grow\n"
	.string "quickly, too.$"
	.string "{JPN}{JPN}2かいに　いって　ともだちと　ポケモンを\n"
	.string "こうかん　してみたら？\p"
	.string "こうかん　すれば　いろんな　ポケモンが　てに\n"
	.string "はいるよ\p"
	.string "それに　こうかんで　もらった　ポケモンは\n"
	.string "そだつのも　はやいんだ$"

