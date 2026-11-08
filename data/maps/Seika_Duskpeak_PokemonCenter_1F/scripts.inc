Seika_Duskpeak_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Duskpeak_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Duskpeak_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_DUSKPEAK
	end

Seika_Duskpeak_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Duskpeak_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Duskpeak_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Duskpeak_PokemonCenter_1F_Text_Npc0::
	.string "I came all the way from KANTO\n"
	.string "to see the top of MT. SKYREACH.\p"
	.string "Only the CHAMPION may climb it.\n"
	.string "Can you believe that?$"
