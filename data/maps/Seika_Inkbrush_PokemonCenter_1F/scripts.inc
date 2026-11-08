Seika_Inkbrush_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Inkbrush_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_Inkbrush_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_INKBRUSH
	end

Seika_Inkbrush_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_Inkbrush_PokemonCenter_1F_EventScript_Npc0::
	msgbox Seika_Inkbrush_PokemonCenter_1F_Text_Npc0, MSGBOX_NPC
	end

Seika_Inkbrush_PokemonCenter_1F_Text_Npc0::
	.string "The ink here is made from soot\n"
	.string "and pine resin.\p"
	.string "It never fades. Some scrolls are\n"
	.string "a thousand years old!$"
	.string "{JPN}{JPN}この　まちの　すみは　すすと　まつやにから\n"
	.string "つくるの。\p"
	.string "ぜったいに　いろあせないわ。　せんねんまえの\n"
	.string "まきものだって　あるのよ！$"
