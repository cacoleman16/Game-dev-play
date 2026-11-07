PalletTown_PlayersHouse_2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_PlayersHouse_2F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PalletTown_PlayersHouse_2F_OnWarp
	.byte 0

PalletTown_PlayersHouse_2F_OnTransition::
	call_if_eq VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 0, PalletTown_PlayersHouse_2F_EventScript_SetRespawn
	end

PalletTown_PlayersHouse_2F_EventScript_SetRespawn::
	setrespawn HEAL_LOCATION_PALLET_TOWN
	return

PalletTown_PlayersHouse_2F_OnWarp::
	map_script_2 VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 0, PalletTown_PlayersHouse_2F_FirstWarpIn
	.2byte 0

PalletTown_PlayersHouse_2F_FirstWarpIn::
	turnobject LOCALID_PLAYER, DIR_NORTH
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 1
	end

@ [QoL] Challenge settings: the NES in your bedroom toggles the optional
@ Kanto level caps and the following Pokémon. Come back any time (Fly to Pallet).
PalletTown_PlayersHouse_2F_EventScript_NES::
	lockall
	msgbox PalletTown_PlayersHouse_2F_Text_PlayedWithNES, MSGBOX_DEFAULT
	msgbox PalletTown_PlayersHouse_2F_Text_ChangeSettings, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_NESDone
	setvar VAR_0x8005, 0
	setvar VAR_0x8006, 0
	call_if_unset FLAG_QOL_LEVEL_CAPS, PalletTown_PlayersHouse_2F_EventScript_AskCapsOn
	call_if_set FLAG_QOL_LEVEL_CAPS, PalletTown_PlayersHouse_2F_EventScript_AskCapsOff
	call_if_unset FLAG_QOL_FOLLOWERS_OFF, PalletTown_PlayersHouse_2F_EventScript_AskFollowersOff
	call_if_set FLAG_QOL_FOLLOWERS_OFF, PalletTown_PlayersHouse_2F_EventScript_AskFollowersOn
	msgbox PalletTown_PlayersHouse_2F_Text_SettingsSaved, MSGBOX_DEFAULT
PalletTown_PlayersHouse_2F_EventScript_NESDone::
	releaseall
	end

@ Each question flips its flag only on YES. VAR_0x8005 marks a flag already
@ handled, so the matching "set" check right after doesn't ask again.
PalletTown_PlayersHouse_2F_EventScript_AskCapsOn::
	msgbox PalletTown_PlayersHouse_2F_Text_CapsOffTurnOn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_SkipCaps
	setflag FLAG_QOL_LEVEL_CAPS
	setvar VAR_0x8005, 1
	return

PalletTown_PlayersHouse_2F_EventScript_AskCapsOff::
	goto_if_eq VAR_0x8005, 1, PalletTown_PlayersHouse_2F_EventScript_CapsDone
	msgbox PalletTown_PlayersHouse_2F_Text_CapsOnTurnOff, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_CapsDone
	clearflag FLAG_QOL_LEVEL_CAPS
PalletTown_PlayersHouse_2F_EventScript_CapsDone::
	setvar VAR_0x8005, 0
	return

PalletTown_PlayersHouse_2F_EventScript_SkipCaps::
	setvar VAR_0x8005, 1
	return

PalletTown_PlayersHouse_2F_EventScript_AskFollowersOff::
	msgbox PalletTown_PlayersHouse_2F_Text_FollowersOnTurnOff, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_SkipFollowers
	setflag FLAG_QOL_FOLLOWERS_OFF
	setvar VAR_0x8006, 1
	return

PalletTown_PlayersHouse_2F_EventScript_AskFollowersOn::
	goto_if_eq VAR_0x8006, 1, PalletTown_PlayersHouse_2F_EventScript_FollowersDone
	msgbox PalletTown_PlayersHouse_2F_Text_FollowersOffTurnOn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_FollowersDone
	clearflag FLAG_QOL_FOLLOWERS_OFF
PalletTown_PlayersHouse_2F_EventScript_FollowersDone::
	setvar VAR_0x8006, 0
	return

PalletTown_PlayersHouse_2F_EventScript_SkipFollowers::
	setvar VAR_0x8006, 1
	return

PalletTown_PlayersHouse_2F_EventScript_Sign::
	msgbox PalletTown_PlayersHouse_2F_Text_PressLRForHelp, MSGBOX_SIGN
	end

PalletTown_PlayersHouse_2F_Text_PlayedWithNES::
	.string "{PLAYER} played with the NES.\p"
	.string "…Okay!\n"
	.string "It's time to go!$"

PalletTown_PlayersHouse_2F_Text_PressLRForHelp::
	.string "It's a posted notice…\p"
	.string "L Button: auto-run on/off.\n"
	.string "R Button: DexNav search.\p"
	.string "Challenge settings are on the NES.$"

PalletTown_PlayersHouse_2F_Text_ChangeSettings::
	.string "Change the challenge settings?$"

PalletTown_PlayersHouse_2F_Text_CapsOffTurnOn::
	.string "LEVEL CAPS are OFF.\p"
	.string "With them ON, POKéMON stop gaining\n"
	.string "EXP. at the next GYM LEADER's top\l"
	.string "level. Turn LEVEL CAPS ON?$"

PalletTown_PlayersHouse_2F_Text_CapsOnTurnOff::
	.string "LEVEL CAPS are ON.\n"
	.string "Turn them OFF?$"

PalletTown_PlayersHouse_2F_Text_FollowersOnTurnOff::
	.string "Your lead POKéMON follows you.\n"
	.string "Turn following POKéMON OFF?$"

PalletTown_PlayersHouse_2F_Text_FollowersOffTurnOn::
	.string "Following POKéMON are OFF.\n"
	.string "Turn them back ON?$"

PalletTown_PlayersHouse_2F_Text_SettingsSaved::
	.string "Settings updated. Have fun!$"

