Seika_Silkhollow_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Silkhollow_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Silkhollow_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_SILKHOLLOW
	end

Seika_Silkhollow_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Silkhollow_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Silkhollow_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Silkhollow_PokemonCenter_1F_Text_Npc0::
	.string "Up north, the clouds never leave\n"
	.string "the top of MT. SKYREACH.\p"
	.string "They say the WISHING STAR sleeps\n"
	.string "up there, above the clouds.$"
