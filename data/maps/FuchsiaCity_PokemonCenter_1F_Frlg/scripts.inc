FuchsiaCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FuchsiaCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

FuchsiaCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_FUCHSIA_CITY
	end

FuchsiaCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

FuchsiaCity_PokemonCenter_1F_EventScript_Man::
	msgbox FuchsiaCity_PokemonCenter_1F_Text_CantBecomeGoodTrainerWithOneMon, MSGBOX_NPC
	end

FuchsiaCity_PokemonCenter_1F_EventScript_CooltrainerF::
	msgbox FuchsiaCity_PokemonCenter_1F_Text_PokemonLeagueWestOfViridian, MSGBOX_NPC
	end

FuchsiaCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox FuchsiaCity_PokemonCenter_1F_Text_VisitSafariZoneForPokedex, MSGBOX_NPC
	end

FuchsiaCity_PokemonCenter_1F_Text_CantBecomeGoodTrainerWithOneMon::
	.string "You can't become a good TRAINER\n"
	.string "with just one strong POKéMON.\p"
	.string "But raising many POKéMON evenly\n"
	.string "is no easy task, either.$"
	.string "{JPN}{JPN}つよい　ポケモン　1ぴき　だけでは　いい\n"
	.string "トレーナーには　なれない。\p"
	.string "でも　たくさんの　ポケモンを　バランスよく\n"
	.string "そだてるのも　かんたんじゃ　ないんだ。$"

FuchsiaCity_PokemonCenter_1F_Text_PokemonLeagueWestOfViridian::
	.string "There's a narrow trail west of\n"
	.string "VIRIDIAN CITY.\p"
	.string "It goes to the POKéMON LEAGUE HQ.\n"
	.string "The HQ governs all TRAINERS.$"
	.string "{JPN}{JPN}トキワシティの　にしに　ほそい　みちが\n"
	.string "あるんだ。\p"
	.string "その　さきが　ポケモンリーグ　ほんぶ。　すべての\n"
	.string "トレーナーを　たばねる　ところさ。$"

FuchsiaCity_PokemonCenter_1F_Text_VisitSafariZoneForPokedex::
	.string "If you're working on a POKéDEX,\n"
	.string "visit the SAFARI ZONE.\p"
	.string "All sorts of rare POKéMON breed\n"
	.string "there.$"
	.string "{JPN}{JPN}ずかんを　うめているなら　サファリゾーンに\n"
	.string "いって　みて。\p"
	.string "めずらしい　ポケモンが　いろいろ　すんでいるよ。$"

