Seika_Mossgrove_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Mossgrove_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Mossgrove_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_MOSSGROVE
	end

Seika_Mossgrove_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Mossgrove_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Mossgrove_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Mossgrove_PokemonCenter_1F_Text_Npc0::
	.string "I caught a HERACROSS in the woods!\p"
	.string "It tried to eat the honey in my\n"
	.string "bag. It has good taste!$"
	.string "{JPN}{JPN}もりで　ヘラクロスを　つかまえたよ！\p"
	.string "ぼくの　バッグの　ミツを　たべようと　したんだ。\n"
	.string "あじの　わかる　やつだよ！$"
