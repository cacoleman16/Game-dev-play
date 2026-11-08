Seika_Tidewatch_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Tidewatch_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Tidewatch_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_TIDEWATCH
	end

Seika_Tidewatch_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Tidewatch_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Tidewatch_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Tidewatch_PokemonCenter_1F_Text_Npc0::
	.string "My ship brings fish from here to\n"
	.string "KILN CITY, and pots back again.\p"
	.string "The KILN CITY folk do love their\n"
	.string "fish on a nice plate!$"
