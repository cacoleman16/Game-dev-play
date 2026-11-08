Seika_KilnCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Kiln_City_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Kiln_City_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_KILN_CITY
	end

Seika_Kiln_City_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Kiln_City_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Kiln_City_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Kiln_City_PokemonCenter_1F_Text_Npc0::
	.string "I came all the way from KANTO for\n"
	.string "a KILN CITY teapot.\p"
	.string "Now I need a second bag to carry\n"
	.string "it home!$"
	.string "{JPN}{JPN}カマモトシティの　きゅうすが　ほしくて　はるばる\n"
	.string "カントーから　きたのです。\p"
	.string "おかげで　もちかえる　かばんが　もう　ひとつ\n"
	.string "いりますな！$"
