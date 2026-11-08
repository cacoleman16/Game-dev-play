Seika_Frostmere_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Frostmere_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Frostmere_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_FROSTMERE
	end

Seika_Frostmere_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Frostmere_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Frostmere_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Frostmere_PokemonCenter_1F_Text_Npc0::
	.string "Heal up before FROST ROAD.\n"
	.string "The wind up there bites!$"
	.string "{JPN}{JPN}シモかいどうに　いく　まえに　かいふく\n"
	.string "しときな。　あそこの　かぜは　みに　しみるぞ！$"
