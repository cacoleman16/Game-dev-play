Seika_League_SkyRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Seika_League_SkyRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, Seika_League_SkyRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Seika_League_SkyRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Seika_League_SkyRoom_OnFrame
	.byte 0

Seika_League_SkyRoom_OnResume::
	setvar VAR_0x8004, 0
	call PokemonLeague_EventScript_DoLightingEffect
	end

Seika_League_SkyRoom_OnLoad::
	call_if_set FLAG_QOL_SEIKA_E4_3, PokemonLeague_EventScript_SetDoorOpen
	call_if_eq VAR_TEMP_1, 1, PokemonLeague_EventScript_CloseEntry
	end

Seika_League_SkyRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, Seika_League_SkyRoom_EventScript_TurnNorth
	.2byte 0

Seika_League_SkyRoom_EventScript_TurnNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

Seika_League_SkyRoom_OnFrame::
	map_script_2 VAR_TEMP_1, 0, Seika_League_SkyRoom_EventScript_Enter
	.2byte 0

Seika_League_SkyRoom_EventScript_Enter::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp5
	waitmovement 0
	playse SE_UNLOCK
	call PokemonLeague_EventScript_CloseEntry
	special DrawWholeMapView
	setvar VAR_TEMP_1, 1
	releaseall
	end

Seika_League_SkyRoom_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_E4_3, Seika_League_SkyRoom_EventScript_Post
	msgbox Seika_League_SkyRoom_Text_Intro
	setflag FLAG_TEMP_3
	trainerbattle_no_intro TRAINER_SEIKA_E4_SORA, Seika_League_SkyRoom_Text_Defeat
	setflag FLAG_QOL_SEIKA_E4_3
	applymovement LOCALID_PLAYER, Common_Movement_Delay32
	waitmovement 0
	playse SE_DOOR
	call PokemonLeague_EventScript_SetDoorOpen
	special DrawWholeMapView
	msgbox Seika_League_SkyRoom_Text_Post
	release
	end

Seika_League_SkyRoom_EventScript_Post::
	msgbox Seika_League_SkyRoom_Text_Post
	release
	end

Seika_League_SkyRoom_Text_Intro::
	.string "I am SORA. I have flown higher\n"
	.string "than MT. SKYREACH.\p"
	.string "I saw the STARFALL from above\n"
	.string "the clouds. Can you keep up?{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

Seika_League_SkyRoom_Text_Defeat::
	.string "You flew right past me!$"

Seika_League_SkyRoom_Text_Post::
	.string "The last of us waits beyond\n"
	.string "that door. Breathe, then go.$"
