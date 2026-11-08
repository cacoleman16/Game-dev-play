Skyrift_Riftwake_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Skyrift_Riftwake_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Skyrift_Riftwake_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SKYRIFT_RIFTWAKE
	end

Skyrift_Riftwake_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Skyrift_Riftwake_PokemonCenter_1F_EventScript_Npc0::
	msgbox Skyrift_Riftwake_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Skyrift_Riftwake_PokemonCenter_1F_Text_Npc0::
	.string "I came all this way to see the\n"
	.string "SKYRIFT up close.\p"
	.string "Now I can't stop looking up.\n"
	.string "My neck hurts!$"
