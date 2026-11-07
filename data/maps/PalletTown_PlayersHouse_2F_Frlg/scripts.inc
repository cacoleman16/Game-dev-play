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

@ [QoL] Challenge settings: the NES in your bedroom opens a menu that switches
@ the optional rules on and off. Each entry shows its current state; pick one
@ to flip it, or DONE / B to leave. Come back any time (Fly to Pallet).
PalletTown_PlayersHouse_2F_EventScript_NES::
	lockall
	msgbox PalletTown_PlayersHouse_2F_Text_PlayedWithNES, MSGBOX_DEFAULT
PalletTown_PlayersHouse_2F_EventScript_NESMenu::
	message PalletTown_PlayersHouse_2F_Text_PickSetting
	waitmessage
	call_if_set FLAG_QOL_NUZLOCKE, PalletTown_PlayersHouse_2F_EventScript_PushNuzlockeOn
	call_if_unset FLAG_QOL_NUZLOCKE, PalletTown_PlayersHouse_2F_EventScript_PushNuzlockeOff
	call_if_set FLAG_QOL_LEVEL_CAPS, PalletTown_PlayersHouse_2F_EventScript_PushCapsOn
	call_if_unset FLAG_QOL_LEVEL_CAPS, PalletTown_PlayersHouse_2F_EventScript_PushCapsOff
	call_if_unset FLAG_QOL_FOLLOWERS_OFF, PalletTown_PlayersHouse_2F_EventScript_PushFollowersOn
	call_if_set FLAG_QOL_FOLLOWERS_OFF, PalletTown_PlayersHouse_2F_EventScript_PushFollowersOff
	dynmultipush PalletTown_PlayersHouse_2F_Text_Done, 9
	dynmultistack 0, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, PalletTown_PlayersHouse_2F_EventScript_ToggleNuzlocke
	case 1, PalletTown_PlayersHouse_2F_EventScript_ToggleCaps
	case 2, PalletTown_PlayersHouse_2F_EventScript_ToggleFollowers
	msgbox PalletTown_PlayersHouse_2F_Text_SettingsSaved, MSGBOX_DEFAULT
	releaseall
	end

PalletTown_PlayersHouse_2F_EventScript_PushNuzlockeOn::
	dynmultipush PalletTown_PlayersHouse_2F_Text_NuzlockeOn, 0
	return

PalletTown_PlayersHouse_2F_EventScript_PushNuzlockeOff::
	dynmultipush PalletTown_PlayersHouse_2F_Text_NuzlockeOff, 0
	return

PalletTown_PlayersHouse_2F_EventScript_PushCapsOn::
	dynmultipush PalletTown_PlayersHouse_2F_Text_CapsOn, 1
	return

PalletTown_PlayersHouse_2F_EventScript_PushCapsOff::
	dynmultipush PalletTown_PlayersHouse_2F_Text_CapsOff, 1
	return

PalletTown_PlayersHouse_2F_EventScript_PushFollowersOn::
	dynmultipush PalletTown_PlayersHouse_2F_Text_FollowersOn, 2
	return

PalletTown_PlayersHouse_2F_EventScript_PushFollowersOff::
	dynmultipush PalletTown_PlayersHouse_2F_Text_FollowersOff, 2
	return

@ Nuzlocke: explain the rules before turning it on.
PalletTown_PlayersHouse_2F_EventScript_ToggleNuzlocke::
	goto_if_set FLAG_QOL_NUZLOCKE, PalletTown_PlayersHouse_2F_EventScript_NuzlockeOff
	msgbox PalletTown_PlayersHouse_2F_Text_NuzlockeRules, MSGBOX_DEFAULT
	msgbox PalletTown_PlayersHouse_2F_Text_TurnNuzlockeOn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_NESMenu
	setflag FLAG_QOL_NUZLOCKE
	goto PalletTown_PlayersHouse_2F_EventScript_NESMenu

PalletTown_PlayersHouse_2F_EventScript_NuzlockeOff::
	msgbox PalletTown_PlayersHouse_2F_Text_TurnNuzlockeOff, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_PlayersHouse_2F_EventScript_NESMenu
	clearflag FLAG_QOL_NUZLOCKE
	goto PalletTown_PlayersHouse_2F_EventScript_NESMenu

PalletTown_PlayersHouse_2F_EventScript_ToggleCaps::
	goto_if_set FLAG_QOL_LEVEL_CAPS, PalletTown_PlayersHouse_2F_EventScript_CapsOff
	msgbox PalletTown_PlayersHouse_2F_Text_CapsExplain, MSGBOX_DEFAULT
	setflag FLAG_QOL_LEVEL_CAPS
	goto PalletTown_PlayersHouse_2F_EventScript_NESMenu

PalletTown_PlayersHouse_2F_EventScript_CapsOff::
	clearflag FLAG_QOL_LEVEL_CAPS
	goto PalletTown_PlayersHouse_2F_EventScript_NESMenu

PalletTown_PlayersHouse_2F_EventScript_ToggleFollowers::
	goto_if_set FLAG_QOL_FOLLOWERS_OFF, PalletTown_PlayersHouse_2F_EventScript_FollowersOn
	setflag FLAG_QOL_FOLLOWERS_OFF
	goto PalletTown_PlayersHouse_2F_EventScript_NESMenu

PalletTown_PlayersHouse_2F_EventScript_FollowersOn::
	clearflag FLAG_QOL_FOLLOWERS_OFF
	goto PalletTown_PlayersHouse_2F_EventScript_NESMenu

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

PalletTown_PlayersHouse_2F_Text_PickSetting::
	.string "Challenge settings.\n"
	.string "Pick one to switch it ON or OFF.$"

PalletTown_PlayersHouse_2F_Text_NuzlockeOn::
	.string "NUZLOCKE: ON$"

PalletTown_PlayersHouse_2F_Text_NuzlockeOff::
	.string "NUZLOCKE: OFF$"

PalletTown_PlayersHouse_2F_Text_CapsOn::
	.string "LEVEL CAPS: ON$"

PalletTown_PlayersHouse_2F_Text_CapsOff::
	.string "LEVEL CAPS: OFF$"

PalletTown_PlayersHouse_2F_Text_FollowersOn::
	.string "FOLLOWERS: ON$"

PalletTown_PlayersHouse_2F_Text_FollowersOff::
	.string "FOLLOWERS: OFF$"

PalletTown_PlayersHouse_2F_Text_Done::
	.string "DONE$"

PalletTown_PlayersHouse_2F_Text_NuzlockeRules::
	.string "NUZLOCKE rules start once PROF.\n"
	.string "OAK gives you POKé BALLS.\p"
	.string "You may catch only the first wild\n"
	.string "POKéMON you meet in each area.\p"
	.string "Species you already caught and\n"
	.string "shiny POKéMON don't count, and\l"
	.string "special encounters are free.\p"
	.string "A POKéMON that faints is gone:\n"
	.string "it can't be healed or revived.\l"
	.string "Put it in a BOX to say goodbye.$"

PalletTown_PlayersHouse_2F_Text_TurnNuzlockeOn::
	.string "Turn NUZLOCKE ON?$"

PalletTown_PlayersHouse_2F_Text_TurnNuzlockeOff::
	.string "Turn NUZLOCKE OFF?\n"
	.string "Fainted POKéMON can heal again.$"

PalletTown_PlayersHouse_2F_Text_CapsExplain::
	.string "With LEVEL CAPS ON, POKéMON stop\n"
	.string "gaining EXP. at the next GYM\l"
	.string "LEADER's top level.$"

PalletTown_PlayersHouse_2F_Text_SettingsSaved::
	.string "Settings updated. Have fun!$"
