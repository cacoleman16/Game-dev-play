VictoryRoad_2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, VictoryRoad_2F_OnLoad
	.byte 0

VictoryRoad_2F_OnLoad::
	call_if_ne VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 100, VictoryRoad_2F_EventScript_SetRockBarrier1
	call_if_ne VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 100, VictoryRoad_2F_EventScript_SetRockBarrier2
	end

VictoryRoad_2F_EventScript_SetRockBarrier1::
	setmetatile 13, 10, METATILE_Cave_RockBarrier_Top, 1
	setmetatile 13, 11, METATILE_Cave_RockBarrier_Bottom, 1
	return

VictoryRoad_2F_EventScript_SetRockBarrier2::
	setmetatile 33, 16, METATILE_Cave_RockBarrier_Top, 1
	setmetatile 33, 17, METATILE_Cave_RockBarrier_Bottom, 1
	return

VictoryRoad_2F_EventScript_FloorSwitch1::
	lockall
	goto_if_eq VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 100, VictoryRoad_2F_EventScript_FloorSwitch1AlreadyPressed
	setmetatile 13, 10, METATILE_Cave_Floor_Ledge_Top, 0
	setmetatile 13, 11, METATILE_Cave_Floor_Ledge_Bottom, 0
	playse SE_ICE_BREAK
	special DrawWholeMapView
	waitse
	copyobjectxytoperm LOCALID_VICTORY_ROAD_2F_BOULDER1
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 100
	releaseall
	end

VictoryRoad_2F_EventScript_FloorSwitch1AlreadyPressed::
	releaseall
	end

VictoryRoad_2F_EventScript_FloorSwitch2::
	lockall
	goto_if_eq VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 100, VictoryRoad_2F_EventScript_FloorSwitch2AlreadyPressed
	setmetatile 33, 16, METATILE_Cave_Floor_Ledge_Top, 0
	setmetatile 33, 17, METATILE_Cave_Floor_Ledge_Bottom, 0
	playse SE_ICE_BREAK
	special DrawWholeMapView
	waitse
	copyobjectxytoperm LOCALID_VICTORY_ROAD_2F_BOULDER2
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 100
	releaseall
	end

VictoryRoad_2F_EventScript_FloorSwitch2AlreadyPressed::
	releaseall
	end

VictoryRoad_2F_EventScript_Dawson::
	trainerbattle_single TRAINER_POKEMANIAC_DAWSON, VictoryRoad_2F_Text_DawsonIntro, VictoryRoad_2F_Text_DawsonDefeat
	msgbox VictoryRoad_2F_Text_DawsonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Daisuke::
	trainerbattle_single TRAINER_BLACK_BELT_DAISUKE, VictoryRoad_2F_Text_DaisukeIntro, VictoryRoad_2F_Text_DaisukeDefeat
	msgbox VictoryRoad_2F_Text_DaisukePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Nelson::
	trainerbattle_single TRAINER_JUGGLER_NELSON, VictoryRoad_2F_Text_NelsonIntro, VictoryRoad_2F_Text_NelsonDefeat
	msgbox VictoryRoad_2F_Text_NelsonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Gregory::
	trainerbattle_single TRAINER_JUGGLER_GREGORY, VictoryRoad_2F_Text_GregoryIntro, VictoryRoad_2F_Text_GregoryDefeat
	msgbox VictoryRoad_2F_Text_GregoryPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Vincent::
	trainerbattle_single TRAINER_TAMER_VINCENT, VictoryRoad_2F_Text_VincentIntro, VictoryRoad_2F_Text_VincentDefeat
	msgbox VictoryRoad_2F_Text_VincentPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_Text_DawsonIntro::
	.string "If you can get through here,\n"
	.string "you can go meet the ELITE FOUR.$"
	.string "{JPN}{JPN}ここを　ぬければ　してんのうに　あえるぞ$"

VictoryRoad_2F_Text_DawsonDefeat::
	.string "No!\n"
	.string "Unbelievable!$"
	.string "{JPN}{JPN}そんな！　しんじられない！$"

VictoryRoad_2F_Text_DawsonPostBattle::
	.string "I can beat you when it comes to\n"
	.string "knowledge about POKéMON!$"
	.string "{JPN}{JPN}ポケモンの　ちしき　なら　きみには　まけないぞ！$"

VictoryRoad_2F_Text_DaisukeIntro::
	.string "This here is VICTORY ROAD.\n"
	.string "It's the final test for TRAINERS!$"
	.string "{JPN}{JPN}ここは　チャンピオンロード　トレーナーの\n"
	.string "さいごの　しれんだ！$"

VictoryRoad_2F_Text_DaisukeDefeat::
	.string "Atcho!$"
	.string "{JPN}{JPN}あちょー！$"

VictoryRoad_2F_Text_DaisukePostBattle::
	.string "If you get stuck, try moving some\n"
	.string "boulders around.$"
	.string "{JPN}{JPN}すすめなく　なったら　いわを　うごかして　みると\n"
	.string "いい$"

VictoryRoad_2F_Text_NelsonIntro::
	.string "Ah, so you wish to challenge the\n"
	.string "ELITE FOUR?$"
	.string "{JPN}{JPN}ほう　してんのうに　ちょうせん　するのか？$"

VictoryRoad_2F_Text_NelsonDefeat::
	.string "You got me!$"
	.string "{JPN}{JPN}やられた！$"

VictoryRoad_2F_Text_NelsonPostBattle::
	.string "{RIVAL} also came through here.$"
	.string "{JPN}{JPN}{RIVAL}も　ここを　とおって　いったよ$"

VictoryRoad_2F_Text_VincentIntro::
	.string "Come on!\n"
	.string "I'll whip you!$"
	.string "{JPN}{JPN}かかって　こい！　けちょんけちょんに　してやる！$"

VictoryRoad_2F_Text_VincentDefeat::
	.string "I got whipped!$"
	.string "{JPN}{JPN}けちょんけちょんに　された！$"

VictoryRoad_2F_Text_VincentPostBattle::
	.string "You earned the right to be on\n"
	.string "VICTORY ROAD…$"
	.string "{JPN}{JPN}きみには　チャンピオンロードを　すすむ　しかくが\n"
	.string "ある‥$"

VictoryRoad_2F_Text_GregoryIntro::
	.string "Is VICTORY ROAD too tough?$"
	.string "{JPN}{JPN}チャンピオンロードは　きびしすぎるかい？$"

VictoryRoad_2F_Text_GregoryDefeat::
	.string "Well done!$"
	.string "{JPN}{JPN}おみごと！$"

VictoryRoad_2F_Text_GregoryPostBattle::
	.string "Many TRAINERS give up their\n"
	.string "challenge here and go home.$"
	.string "{JPN}{JPN}ここで　ちょうせんを　あきらめて　かえる\n"
	.string "トレーナーも　おおいんだ$"

@ Unused, old text for Moltres
VictoryRoad_2F_Text_Gyaoo::
	.string "ギヤーオ！$"
	.string "{JPN}{JPN}ギヤーオ！$"

