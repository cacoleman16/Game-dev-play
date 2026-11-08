Seika_League_DragonRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Seika_League_DragonRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, Seika_League_DragonRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Seika_League_DragonRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Seika_League_DragonRoom_OnFrame
	.byte 0

Seika_League_DragonRoom_OnResume::
	setvar VAR_0x8004, 3
	call PokemonLeague_EventScript_DoLightingEffect
	end

Seika_League_DragonRoom_OnLoad::
	call_if_set FLAG_QOL_SEIKA_E4_4, PokemonLeague_EventScript_SetDoorOpenLance
	call_if_eq VAR_TEMP_1, 1, PokemonLeague_LancesRoom_EventScript_SetEntryClosed
	end

Seika_League_DragonRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, Seika_League_DragonRoom_EventScript_TurnNorth
	.2byte 0

Seika_League_DragonRoom_EventScript_TurnNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

Seika_League_DragonRoom_OnFrame::
	map_script_2 VAR_TEMP_1, 0, Seika_League_DragonRoom_EventScript_Enter
	.2byte 0

Seika_League_DragonRoom_EventScript_Enter::
	lockall
	applymovement LOCALID_PLAYER, PokemonLeague_LancesRoom_Movement_WalkThroughCorridor
	waitmovement 0
	playse SE_UNLOCK
	call PokemonLeague_LancesRoom_EventScript_SetEntryClosed
	special DrawWholeMapView
	setvar VAR_TEMP_1, 1
	releaseall
	end

Seika_League_DragonRoom_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_E4_4, Seika_League_DragonRoom_EventScript_Post
	msgbox Seika_League_DragonRoom_Text_Intro
	setflag FLAG_TEMP_3
	trainerbattle_no_intro TRAINER_SEIKA_E4_RYOKO, Seika_League_DragonRoom_Text_Defeat
	setflag FLAG_QOL_SEIKA_E4_4
	applymovement LOCALID_PLAYER, Common_Movement_Delay32
	waitmovement 0
	playse SE_DOOR
	call PokemonLeague_EventScript_SetDoorOpenLance
	special DrawWholeMapView
	msgbox Seika_League_DragonRoom_Text_Post
	release
	end

Seika_League_DragonRoom_EventScript_Post::
	msgbox Seika_League_DragonRoom_Text_Post
	release
	end

Seika_League_DragonRoom_Text_Intro::
	.string "I am RYOKO, last of the ELITE\n"
	.string "FOUR. My dragons were here before\l"
	.string "the first STARFALL.\p"
	.string "They will not fall to a\n"
	.string "traveler. Show me otherwise!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}リョウコ『わたしは　してんのう　さいごの　ひとり\n"
	.string "リョウコ。　わが　ドラゴンは　さいしょの\l"
	.string "スターフォールより　まえから　ここに　おった。\p"
	.string "たびびとなどに　たおされは　せぬ。　ちがうと\n"
	.string "いうなら　みせて　みよ！』{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

Seika_League_DragonRoom_Text_Defeat::
	.string "The dragons bow to you.$"
	.string "{JPN}{JPN}ドラゴンたちが　そなたに　ひれふした。$"

Seika_League_DragonRoom_Text_Post::
	.string "The CHAMPION is waiting.\n"
	.string "He has waited a long time.$"
	.string "{JPN}{JPN}リョウコ『チャンピオンが　まって　おる。　ながい\n"
	.string "あいだ　まち　つづけて　きたのじゃ。』$"
