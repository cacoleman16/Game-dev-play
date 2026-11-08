SevenIsland_TrainerTower_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_TrainerTower_OnTransition
	.byte 0

SevenIsland_TrainerTower_OnTransition::
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 0
	end

SevenIsland_TrainerTower_EventScript_TrainerTowerSign::
	msgbox SevenIsland_TrainerTower_Text_TrainerTowerSign, MSGBOX_SIGN
	end

SevenIsland_TrainerTower_EventScript_TrainerTowerAheadSign::
	msgbox SevenIsland_TrainerTower_Text_TrainerTowerAhead, MSGBOX_SIGN
	end

SevenIsland_TrainerTower_Text_DarioIntro::
	.string "I sensed your approach.$"
	.string "{JPN}{JPN}きみが　くるのを　かんじて　いた。$"

SevenIsland_TrainerTower_Text_DarioDefeat::
	.string "Give up…$"
	.string "{JPN}{JPN}あきらめよう‥$"

SevenIsland_TrainerTower_Text_DarioPostBattle::
	.string "In your future, I sense that you\n"
	.string "will meet many others.\p"
	.string "I think you will be linked to each\n"
	.string "other by a strange power.$"
	.string "{JPN}{JPN}きみの　みらいに　おおくの　ひとたちとの\n"
	.string "であいが　みえる。\p"
	.string "きみたちは　ふしぎな　ちからで　むすばれる\n"
	.string "ことに　なるだろう。$"

SevenIsland_TrainerTower_Text_RodetteIntro::
	.string "Somewhere on this island, peculiar\n"
	.string "POKéMON are sleeping.$"
	.string "{JPN}{JPN}この　しまの　どこかに　ふしぎな　ポケモンが\n"
	.string "ねむって　いるの。$"

SevenIsland_TrainerTower_Text_RodetteDefeat::
	.string "What an amazing TRAINER!$"
	.string "{JPN}{JPN}なんて　すごい　トレーナーなの！$"

SevenIsland_TrainerTower_Text_RodettePostBattle::
	.string "Somewhere on this island, peculiar\n"
	.string "POKéMON are sleeping.\p"
	.string "Could it be that they're waiting\n"
	.string "for you?$"
	.string "{JPN}{JPN}この　しまの　どこかに　ふしぎな　ポケモンが\n"
	.string "ねむって　いるの。\p"
	.string "もしかして　あなたを　まって　いるのかも？$"

SevenIsland_TrainerTower_Text_TrainerTowerSign::
	.string "TRAINER TOWER ENTRANCE\n"
	.string "Rise to the Challenge, TRAINERS!$"
	.string "{JPN}{JPN}トレーナータワー　いりぐち\n"
	.string "トレーナーよ　ちょうせん　せよ！$"

SevenIsland_TrainerTower_Text_TrainerTowerAhead::
	.string "TRAINER TOWER AHEAD$"
	.string "{JPN}{JPN}このさき　トレーナータワー$"

