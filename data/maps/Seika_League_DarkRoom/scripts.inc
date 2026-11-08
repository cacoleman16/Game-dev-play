Seika_League_DarkRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Seika_League_DarkRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, Seika_League_DarkRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Seika_League_DarkRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Seika_League_DarkRoom_OnFrame
	.byte 0

Seika_League_DarkRoom_OnResume::
	setvar VAR_0x8004, 2
	call PokemonLeague_EventScript_DoLightingEffect
	end

Seika_League_DarkRoom_OnLoad::
	call_if_set FLAG_QOL_SEIKA_E4_2, PokemonLeague_EventScript_SetDoorOpen
	call_if_eq VAR_TEMP_1, 1, PokemonLeague_EventScript_CloseEntry
	end

Seika_League_DarkRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, Seika_League_DarkRoom_EventScript_TurnNorth
	.2byte 0

Seika_League_DarkRoom_EventScript_TurnNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

Seika_League_DarkRoom_OnFrame::
	map_script_2 VAR_TEMP_1, 0, Seika_League_DarkRoom_EventScript_Enter
	.2byte 0

Seika_League_DarkRoom_EventScript_Enter::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp5
	waitmovement 0
	playse SE_UNLOCK
	call PokemonLeague_EventScript_CloseEntry
	special DrawWholeMapView
	setvar VAR_TEMP_1, 1
	releaseall
	end

Seika_League_DarkRoom_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_E4_2, Seika_League_DarkRoom_EventScript_Post
	msgbox Seika_League_DarkRoom_Text_Intro
	setflag FLAG_TEMP_3
	trainerbattle_no_intro TRAINER_SEIKA_E4_KUROE, Seika_League_DarkRoom_Text_Defeat
	setflag FLAG_QOL_SEIKA_E4_2
	applymovement LOCALID_PLAYER, Common_Movement_Delay32
	waitmovement 0
	playse SE_DOOR
	call PokemonLeague_EventScript_SetDoorOpen
	special DrawWholeMapView
	msgbox Seika_League_DarkRoom_Text_Post
	release
	end

Seika_League_DarkRoom_EventScript_Post::
	msgbox Seika_League_DarkRoom_Text_Post
	release
	end

Seika_League_DarkRoom_Text_Intro::
	.string "I am KUROE. Stars are only bright\n"
	.string "because the night is so dark.\p"
	.string "Your light against my dark.\n"
	.string "Shall we?{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

Seika_League_DarkRoom_Text_Defeat::
	.string "So bright... I can hardly see.$"

Seika_League_DarkRoom_Text_Post::
	.string "Keep that light. The sky is\n"
	.string "waiting for you.$"
