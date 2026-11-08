SilphCo_3F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_3F_OnLoad
	.byte 0

SilphCo_3F_OnLoad::
	call_if_unset FLAG_SILPH_3F_DOOR_1, EventScript_Close3FDoor1
	call_if_unset FLAG_SILPH_3F_DOOR_2, EventScript_Close3FDoor2
	end

SilphCo_3F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_3F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_3F_Text_WhatAmIToDo
	release
	end

SilphCo_3F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_3F_Text_YouAndYourMonsSavedUs
	release
	end

SilphCo_3F_EventScript_FloorSign::
	msgbox SilphCo_3F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_3F_EventScript_Jose::
	trainerbattle_single TRAINER_SCIENTIST_JOSE, SilphCo_3F_Text_JoseIntro, SilphCo_3F_Text_JoseDefeat
	msgbox SilphCo_3F_Text_JosePostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_3F_EventScript_Grunt::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_25, SilphCo_3F_Text_GruntIntro, SilphCo_3F_Text_GruntDefeat
	msgbox SilphCo_3F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_3F_Text_WhatAmIToDo::
	.string "I work for SILPH CO.\n"
	.string "What am I to do?$"
	.string "{JPN}{JPN}ぼくは　シルフカンパニーの　しゃいん　なんだ。\n"
	.string "どう　したら　いいんだ？$"

SilphCo_3F_Text_YouAndYourMonsSavedUs::
	.string "{PLAYER}!\n"
	.string "You and your POKéMON saved us!$"
	.string "{JPN}{JPN}{PLAYER}！　きみと　きみの　ポケモンが\n"
	.string "ぼくたちを　すくって　くれた！$"

SilphCo_3F_Text_GruntIntro::
	.string "Quit messing with us, kid!$"
	.string "{JPN}{JPN}ガキが　ちょろちょろ　するんじゃ　ねえ！$"

SilphCo_3F_Text_GruntDefeat::
	.string "I give up!$"
	.string "{JPN}{JPN}まいった！$"

SilphCo_3F_Text_GruntPostBattle::
	.string "A hint? You can open doors with\n"
	.string "a CARD KEY!$"
	.string "{JPN}{JPN}ヒントか？　ドアは　カードキーで　あけられるぜ！$"

SilphCo_3F_Text_JoseIntro::
	.string "I support TEAM ROCKET more than\n"
	.string "I support SILPH CO.!$"
	.string "{JPN}{JPN}ぼくは　シルフカンパニーより　ロケットだんを\n"
	.string "おうえん　してるんだ！$"

SilphCo_3F_Text_JoseDefeat::
	.string "You really got me!$"
	.string "{JPN}{JPN}まいったよ！$"

SilphCo_3F_Text_JosePostBattle::
	.string "Humph…\p"
	.string "TEAM ROCKET said that if I helped\n"
	.string "them, they'd let me study POKéMON.$"
	.string "{JPN}{JPN}ふん‥\p"
	.string "ロケットだんに　きょうりょく　すれば　ポケモンの\n"
	.string "けんきゅうを　させて　くれるって　いうんだ。$"

SilphCo_3F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "3F$"
	.string "{JPN}{JPN}シルフカンパニー　ほんしゃ\n"
	.string "3かい$"

