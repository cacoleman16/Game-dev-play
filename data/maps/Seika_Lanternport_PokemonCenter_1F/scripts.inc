Seika_Lanternport_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Lanternport_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Lanternport_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_LANTERNPORT
	end

Seika_Lanternport_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Lanternport_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Lanternport_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Lanternport_PokemonCenter_1F_Text_Npc0::
	.string "POKéMON in SEIKA are strong.\n"
	.string "The sea air makes them tough!$"
