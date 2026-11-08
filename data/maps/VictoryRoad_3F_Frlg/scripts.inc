VictoryRoad_3F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, VictoryRoad_3F_OnLoad
	.byte 0

VictoryRoad_3F_OnLoad::
	call_if_ne VAR_MAP_SCENE_VICTORY_ROAD_3F, 100, VictoryRoad_3F_EventScript_SetRockBarrier
	end

VictoryRoad_3F_EventScript_SetRockBarrier::
	setmetatile 12, 12, METATILE_Cave_RockBarrier_Top, 1
	setmetatile 12, 13, METATILE_Cave_RockBarrier_Bottom, 1
	return

VictoryRoad_3F_EventScript_FloorSwitch::
	lockall
	goto_if_eq VAR_MAP_SCENE_VICTORY_ROAD_3F, 100, VictoryRoad_3F_EventScript_FloorSwitchAlreadyPressed
	setmetatile 12, 12, METATILE_Cave_Floor_Ledge_Top, 0
	setmetatile 12, 13, METATILE_Cave_Floor_Ledge_Bottom, 0
	playse SE_ICE_BREAK
	special DrawWholeMapView
	waitse
	copyobjectxytoperm LOCALID_VICTORY_ROAD_3F_BOULDER1
	copyobjectxytoperm LOCALID_VICTORY_ROAD_3F_BOULDER2
	setvar VAR_MAP_SCENE_VICTORY_ROAD_3F, 100
	releaseall
	end

VictoryRoad_3F_EventScript_FloorSwitchAlreadyPressed::
	releaseall
	end

VictoryRoad_3F_EventScript_George::
	trainerbattle_single TRAINER_COOLTRAINER_GEORGE, VictoryRoad_3F_Text_GeorgeIntro, VictoryRoad_3F_Text_GeorgeDefeat
	msgbox VictoryRoad_3F_Text_GeorgePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_3F_EventScript_Colby::
	trainerbattle_single TRAINER_COOLTRAINER_COLBY, VictoryRoad_3F_Text_ColbyIntro, VictoryRoad_3F_Text_ColbyDefeat
	msgbox VictoryRoad_3F_Text_ColbyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_3F_EventScript_Caroline::
	trainerbattle_single TRAINER_COOLTRAINER_CAROLINE, VictoryRoad_3F_Text_CarolineIntro, VictoryRoad_3F_Text_CarolineDefeat
	msgbox VictoryRoad_3F_Text_CarolinePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_3F_EventScript_Alexa::
	trainerbattle_single TRAINER_COOLTRAINER_ALEXA, VictoryRoad_3F_Text_AlexaIntro, VictoryRoad_3F_Text_AlexaDefeat
	msgbox VictoryRoad_3F_Text_AlexaPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_3F_EventScript_Ray::
	trainerbattle_double TRAINER_COOL_COUPLE_RAY_TYRA, VictoryRoad_3F_Text_RayIntro, VictoryRoad_3F_Text_RayDefeat, VictoryRoad_3F_Text_RayNotEnoughMons
	msgbox VictoryRoad_3F_Text_RayPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_3F_EventScript_Tyra::
	trainerbattle_double TRAINER_COOL_COUPLE_RAY_TYRA, VictoryRoad_3F_Text_TyraIntro, VictoryRoad_3F_Text_TyraDefeat, VictoryRoad_3F_Text_TyraNotEnoughMons
	msgbox VictoryRoad_3F_Text_TyraPostBattle, MSGBOX_AUTOCLOSE
	end

Text_DoubleEdgeTeach::
	.string "You should be proud of yourself,\n"
	.string "having battled your way through\l"
	.string "VICTORY ROAD so courageously.\p"
	.string "In recognition of your feat,\n"
	.string "I'll teach you DOUBLE-EDGE.\p"
	.string "Would you like me to teach that\n"
	.string "technique?$"
	.string "{JPN}{JPN}チャンピオンロードを　ゆうかんに　たたかいぬいて\n"
	.string "きた　きみは　ほこって　いいぞ\p"
	.string "その　がんばりを　たたえて　すてみタックルを\n"
	.string "おしえて　あげよう\p"
	.string "この　わざを　おしえて　ほしいかね？$"

Text_DoubleEdgeDeclined::
	.string "I'll teach you the technique\n"
	.string "anytime.$"
	.string "{JPN}{JPN}いつでも　おしえて　あげるよ$"

Text_DoubleEdgeWhichMon::
	.string "Which POKéMON should I teach\n"
	.string "DOUBLE-EDGE?$"
	.string "{JPN}{JPN}どの　ポケモンに　すてみタックルを\n"
	.string "おしえるかね？$"

Text_DoubleEdgeTaught::
	.string "Keep that drive going for the\n"
	.string "POKéMON LEAGUE!\p"
	.string "Take a run at them and knock 'em\n"
	.string "out!$"
	.string "{JPN}{JPN}その　いきおいで　ポケモンリーグに　いどむんだ！\p"
	.string "おもいきり　ぶつかって　たおして　こい！$"

VictoryRoad_3F_Text_GeorgeIntro::
	.string "I heard rumors of a child prodigy.$"
	.string "{JPN}{JPN}てんさい　こどもが　いるって　うわさを　きいたぞ$"

VictoryRoad_3F_Text_GeorgeDefeat::
	.string "The rumors were true!$"
	.string "{JPN}{JPN}うわさは　ほんとう　だった！$"

VictoryRoad_3F_Text_GeorgePostBattle::
	.string "So, it was you who beat GIOVANNI\n"
	.string "of TEAM ROCKET?$"
	.string "{JPN}{JPN}じゃあ　ロケットだんの　サカキを　たおしたのは\n"
	.string "きみ　だったのか？$"

VictoryRoad_3F_Text_AlexaIntro::
	.string "TRAINERS live to seek stronger\n"
	.string "opponents.$"
	.string "{JPN}{JPN}トレーナーは　より　つよい　あいてを　もとめて\n"
	.string "いきるのよ$"

VictoryRoad_3F_Text_AlexaDefeat::
	.string "Oh!\n"
	.string "So strong!$"
	.string "{JPN}{JPN}ああ！　つよい！$"

VictoryRoad_3F_Text_AlexaPostBattle::
	.string "By fighting tough battles, you get\n"
	.string "stronger.$"
	.string "{JPN}{JPN}きびしい　たたかいを　かさねて　つよく　なるのよ$"

VictoryRoad_3F_Text_CarolineIntro::
	.string "I'll show you just how good you\n"
	.string "are. Not!$"
	.string "{JPN}{JPN}あなたの　じつりょくを　おしえて　あげる‥\n"
	.string "なんてね！$"

VictoryRoad_3F_Text_CarolineDefeat::
	.string "I'm furious!$"
	.string "{JPN}{JPN}くやしーい！$"

VictoryRoad_3F_Text_CarolinePostBattle::
	.string "You showed me just how good\n"
	.string "I was…$"
	.string "{JPN}{JPN}わたしの　じつりょくを　おしえられちゃった‥$"

VictoryRoad_3F_Text_ColbyIntro::
	.string "Only the chosen can pass here!$"
	.string "{JPN}{JPN}ここを　とおれるのは　えらばれた　ものだけだ！$"

VictoryRoad_3F_Text_ColbyDefeat::
	.string "I don't believe it!$"
	.string "{JPN}{JPN}しんじられん！$"

VictoryRoad_3F_Text_ColbyPostBattle::
	.string "All TRAINERS here are headed to\n"
	.string "the POKéMON LEAGUE.\p"
	.string "Don't let down your guard.$"
	.string "{JPN}{JPN}ここに　いる　トレーナーは　みんな\n"
	.string "ポケモンリーグを　めざして　いる\p"
	.string "ゆだん　するなよ$"

VictoryRoad_3F_Text_RayIntro::
	.string "RAY: Together, the two of us are\n"
	.string "destined for greatness!$"
	.string "{JPN}{JPN}レイ『ぼくたち　ふたりは　いだいな　トレーナーに\n"
	.string "なる　うんめい　なんだ！』$"

VictoryRoad_3F_Text_RayDefeat::
	.string "RAY: Ludicrous!\n"
	.string "This can't be!$"
	.string "{JPN}{JPN}レイ『ばかな！　こんな　はずは！』$"

VictoryRoad_3F_Text_RayPostBattle::
	.string "RAY: You've beaten us.\n"
	.string "Greatness remains elusive…$"
	.string "{JPN}{JPN}レイ『まけたよ‥　いだいな　トレーナーへの\n"
	.string "みちは　とおいな‥』$"

VictoryRoad_3F_Text_RayNotEnoughMons::
	.string "RAY: Together, the two of us are\n"
	.string "striving for the pinnacle.\p"
	.string "We need you to bring two POKéMON\n"
	.string "into battle with us.$"
	.string "{JPN}{JPN}レイ『ぼくたち　ふたりで　ちょうてんを　めざして\n"
	.string "いるんだ\p"
	.string "ポケモンを　2ひき　だして　たたかって\n"
	.string "もらうよ』$"

VictoryRoad_3F_Text_TyraIntro::
	.string "TYRA: We're trying to become\n"
	.string "champions together.$"
	.string "{JPN}{JPN}タイラ『わたしたち　ふたりで　チャンピオンを\n"
	.string "めざして　いるの』$"

VictoryRoad_3F_Text_TyraDefeat::
	.string "TYRA: Oh, but…$"
	.string "{JPN}{JPN}タイラ『そんな‥』$"

VictoryRoad_3F_Text_TyraPostBattle::
	.string "TYRA: You've taught me that power\n"
	.string "can be infinite in shape and form.$"
	.string "{JPN}{JPN}タイラ『つよさの　かたちは　ひとつじゃ　ないって\n"
	.string "おしえられたわ』$"

VictoryRoad_3F_Text_TyraNotEnoughMons::
	.string "TYRA: You can't battle with us if\n"
	.string "you have only one POKéMON.$"
	.string "{JPN}{JPN}タイラ『ポケモンが　1ぴき　だけじゃ\n"
	.string "わたしたちとは　たたかえないわ』$"

