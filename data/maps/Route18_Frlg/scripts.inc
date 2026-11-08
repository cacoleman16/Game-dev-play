Route18_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route18_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route18_OnWarp
	.byte 0

Route18_OnTransition::
	call_if_eq VAR_MAP_SCENE_ROUTE16, 1, Route18_OnTransitionCyclingRoad
	end

Route18_OnTransitionCyclingRoad::
	setflag FLAG_SYS_ON_CYCLING_ROAD
	return

Route18_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route18_OnWarpCyclingRoad
	.2byte 0

Route18_OnWarpCyclingRoad::
	special ForcePlayerOntoBike
	end

Route18_EventScript_RouteSign::
	msgbox Route18_Text_RouteSign, MSGBOX_SIGN
	end

Route18_EventScript_CyclingRoadSign::
	msgbox Route18_Text_CyclingRoadSign, MSGBOX_SIGN
	end

Route18_Text_WiltonIntro::
	.string "I always check every grassy area\n"
	.string "for new POKéMON.$"
	.string "{JPN}{JPN}おれは　いつも　くさむらを　ぜんぶ　しらべて\n"
	.string "あたらしい　ポケモンを　さがすんだ。$"

Route18_Text_WiltonDefeat::
	.string "Tch!$"
	.string "{JPN}{JPN}ちっ！$"

Route18_Text_WiltonPostBattle::
	.string "I wish I had a BIKE!$"
	.string "{JPN}{JPN}じてんしゃが　あればなあ！$"

Route18_Text_RamiroIntro::
	.string "Kurukkoo!\n"
	.string "How do you like my birdcall?$"
	.string "{JPN}{JPN}クルックー！　おれの　とりの　なきまね\n"
	.string "どうだい？$"

Route18_Text_RamiroDefeat::
	.string "I had to bug you!$"
	.string "{JPN}{JPN}じゃま　しちゃったな！$"

Route18_Text_RamiroPostBattle::
	.string "I collect sea POKéMON on weekends\n"
	.string "because the sea is so close.$"
	.string "{JPN}{JPN}うみが　ちかいから　しゅうまつは　うみの\n"
	.string "ポケモンを　あつめてるんだ。$"

Route18_Text_JacobIntro::
	.string "This is my turf!\n"
	.string "Get out of here!$"
	.string "{JPN}{JPN}ここは　おれの　なわばりだ！　でていけ！$"

Route18_Text_JacobDefeat::
	.string "Darn!$"
	.string "{JPN}{JPN}くそっ！$"

Route18_Text_JacobPostBattle::
	.string "This is my fave area for catching\n"
	.string "POKéMON.$"
	.string "{JPN}{JPN}ここは　ポケモンを　つかまえるのに　おきにいりの\n"
	.string "ばしょ　なんだ。$"

Route18_Text_RouteSign::
	.string "ROUTE 18\n"
	.string "CELADON CITY - FUCHSIA CITY$"
	.string "{JPN}{JPN}18ばんどうろ\n"
	.string "タマムシシティ　ー　セキチクシティ$"

Route18_Text_CyclingRoadSign::
	.string "CYCLING ROAD\n"
	.string "No pedestrians permitted!$"
	.string "{JPN}{JPN}サイクリングロード\n"
	.string "ほこうしゃ　つうこうきんし！$"

