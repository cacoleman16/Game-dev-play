TrainerTower_Lobby_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, TrainerTower_Lobby_OnResume
	map_script MAP_SCRIPT_ON_RETURN_TO_FIELD, TrainerTower_Lobby_OnReturnToField
	map_script MAP_SCRIPT_ON_TRANSITION, TrainerTower_Lobby_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, TrainerTower_Lobby_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, TrainerTower_Lobby_OnFrame
	.byte 0

TrainerTower_Lobby_OnResume::
	setvar VAR_TEMP_2, 0
	ttower_resumetimer
	ttower_shouldexit
	goto_if_eq VAR_RESULT, FALSE, TrainerTower_Lobby_OnResumeEnd
	@ Never reached, above always FALSE
	setvar VAR_TEMP_0, 0
	setobjectxy LOCALID_PLAYER, 9, 7
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
TrainerTower_Lobby_OnResumeEnd::
	end

TrainerTower_Lobby_OnReturnToField::
	addobject LOCALID_TOWER_NURSE
	addobject LOCALID_TOWER_MART_CLERK
	addobject LOCALID_TOWER_RECEPTIONIST
	addobject LOCALID_TOWER_COOLTRAINER_F
	addobject LOCALID_TOWER_BALDING_MAN
	end

TrainerTower_Lobby_OnLoad::
	call_if_eq VAR_TEMP_D, 17, TrainerTower_Lobby_OpenCounterBarrier
	end

TrainerTower_Lobby_OpenCounterBarrier::
	setmetatile 17, 10, METATILE_TrainerTower_Floor_ShadeBottomLeft, 0
	return

TrainerTower_Lobby_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_TRAINER_TOWER_LOBBY
	getplayerxy VAR_TEMP_D, VAR_RESULT
	end

TrainerTower_Lobby_OnFrame::
	map_script_2 VAR_TEMP_0, 0, TrainerTower_Lobby_EventScript_Enter
	map_script_2 VAR_TEMP_D, 17, TrainerTower_Lobby_EventScript_ExitElevator
	.2byte 0

TrainerTower_Lobby_EventScript_ExitElevator::
	lockall
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_ExitElevator
	waitmovement 0
	setmetatile 17, 10, METATILE_TrainerTower_CounterBarrier, 1
	special DrawWholeMapView
	playse SE_CLICK
	waitse
	setvar VAR_TEMP_D, 0
	releaseall
	end

TrainerTower_Lobby_Movement_ExitElevator::
	walk_down
	walk_down
	step_end

TrainerTower_Lobby_EventScript_Enter::
	setvar VAR_TEMP_0, 1
	ttower_getchallengestatus
	switch VAR_RESULT
	case TT_CHALLENGE_STATUS_LOST, TrainerTower_Lobby_EventScript_LostChallenge
	case TT_CHALLENGE_STATUS_UNK, TrainerTower_Lobby_EventScript_ExitChallengeSpeakToReceptionist
	case TT_CHALLENGE_STATUS_NORMAL, TrainerTower_Lobby_EventScript_EnterEnd

TrainerTower_Lobby_EventScript_LostChallenge::
	lock
	faceplayer
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox TrainerTower_Lobby_Text_TooBadComeBackTryAgain
	goto TrainerTower_Lobby_EventScript_ExitChallenge

@ Presumably E-Reader related
TrainerTower_Lobby_EventScript_ExitChallengeSpeakToReceptionist::
	lock
	faceplayer
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox TrainerTower_Lobby_Text_MoveCounterHereWhenTrainersSwitch
TrainerTower_Lobby_EventScript_ExitChallenge::
	closemessage
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_WalkDown
	waitmovement 0
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 0
	release
TrainerTower_Lobby_EventScript_EnterEnd::
	end

TrainerTower_Lobby_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

TrainerTower_Lobby_EventScript_Receptionist::
	lock
	faceplayer
	ttower_getbeatchallenge
	goto_if_eq VAR_RESULT, TRUE, TrainerTower_Lobby_EventScript_ThanksForCompeting
	msgbox TrainerTower_Lobby_Text_GiveItYourBest
	goto TrainerTower_Lobby_EventScript_ReceptionistEnd

TrainerTower_Lobby_EventScript_ThanksForCompeting::
	msgbox TrainerTower_Lobby_Text_ThanksForCompeting
TrainerTower_Lobby_EventScript_ReceptionistEnd::
	release
	end

TrainerTower_Lobby_EventScript_MartClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart TrainerTower_Lobby_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
TrainerTower_Lobby_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

TrainerTower_Lobby_EventScript_EntryTrigger::
	lockall
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox TrainerTower_Lobby_Text_WelcomeToTrainerTower
	ttower_getnumfloors
	goto_if_eq VAR_RESULT, FALSE, TrainerTower_Lobby_EventScript_AllFloorsUsed
	msgbox TrainerTower_Lobby_Text_TrainersUpToFloorNum
	goto TrainerTower_Lobby_EventScript_AskEnterChallenge

TrainerTower_Lobby_EventScript_AllFloorsUsed::
	msgbox TrainerTower_Lobby_Text_TrainersUpEighthFloor
TrainerTower_Lobby_EventScript_AskEnterChallenge::
	message TrainerTower_Lobby_Text_LikeToChallengeTrainers
	waitmessage
	multichoice 18, 6, MULTI_YESNOINFO, FALSE
	switch VAR_RESULT
	case 0, TrainerTower_Lobby_EventScript_ChooseChallenge
	case 1, TrainerTower_Lobby_EventScript_DeclineChallenge
	case 2, TrainerTower_Lobby_EventScript_ChallengeInfo
	case MULTI_B_PRESSED, TrainerTower_Lobby_EventScript_DeclineChallenge
	end

TrainerTower_Lobby_EventScript_ChallengeInfo::
	msgbox TrainerTower_Lobby_Text_ExplainTrainerTower
	goto TrainerTower_Lobby_EventScript_AskEnterChallenge
	end

TrainerTower_Lobby_EventScript_ChooseChallenge::
	multichoice 13, 3, MULTI_TRAINER_TOWER_MODE, FALSE
	switch VAR_RESULT
	case 0, TrainerTower_Lobby_EventScript_BeginChallenge
	case 1, TrainerTower_Lobby_EventScript_BeginChallenge
	case 2, TrainerTower_Lobby_EventScript_BeginChallenge
	case 3, TrainerTower_Lobby_EventScript_BeginChallenge
	case 4, TrainerTower_Lobby_EventScript_DeclineChallenge
	case MULTI_B_PRESSED, TrainerTower_Lobby_EventScript_DeclineChallenge
	end

TrainerTower_Lobby_EventScript_BeginChallenge::
	copyvar VAR_0x8005, VAR_RESULT
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 1
	special HealPlayerParty
	msgbox TrainerTower_Lobby_Text_StartClockGetSetGo
	ttower_startchallenge
	releaseall
	end

TrainerTower_Lobby_EventScript_DeclineChallenge::
	msgbox TrainerTower_Lobby_Text_PleaseVisitUsAgain
	closemessage
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_WalkDown
	waitmovement 0
	releaseall
	end

TrainerTower_Lobby_Movement_FaceReceptionist::
	face_right
	step_end

TrainerTower_Lobby_Movement_WalkDown::
	walk_down
	step_end

TrainerTower_Lobby_EventScript_ShowRecords::
	lockall
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, 1
	special ShowTrainerHillRecords
	releaseall
	end

TrainerTower_Lobby_EventScript_CooltrainerF::
	msgbox TrainerTower_Lobby_Text_WonderWhatKindsOfTrainers, MSGBOX_NPC
	end

TrainerTower_Lobby_EventScript_BaldingMan::
	msgbox TrainerTower_Lobby_Text_StairsTougherThanAnyBattle, MSGBOX_NPC
	end

@ All Trainer Tower text (Lobby and Roof) is interleaved here

TrainerTower_Lobby_Text_WelcomeToTrainerTower::
	.string "Hello!\p"
	.string "Welcome to TRAINER TOWER where\n"
	.string "TRAINERS gather from all over!$"
	.string "{JPN}{JPN}こんにちは！\p"
	.string "せかいじゅうから　トレーナーが　あつまる\n"
	.string "トレーナータワーへ　ようこそ！$"

TrainerTower_Lobby_Text_TrainersUpToFloorNum::
	.string "TRAINERS from all over the world\n"
	.string "gather here to battle.\p"
	.string "Let me see…\p"
	.string "Right now, there are TRAINERS only\n"
	.string "up to Floor {STR_VAR_1}.$"
	.string "{JPN}{JPN}ここには　せかいじゅうの　トレーナーが\n"
	.string "しょうぶを　しに　あつまって　きます\p"
	.string "えーと‥\p"
	.string "いまは　{STR_VAR_1}かいまで　しか\n"
	.string "トレーナーが　いません$"

TrainerTower_Lobby_Text_TrainersUpEighthFloor::
	.string "TRAINERS are awaiting your\n"
	.string "challenge up to the eighth floor.$"
	.string "{JPN}{JPN}8かいまで　トレーナーが　ちょうせんを　まって\n"
	.string "います$"

TrainerTower_Lobby_Text_LikeToChallengeTrainers::
	.string "Would you like to challenge the\n"
	.string "waiting TRAINERS?$"
	.string "{JPN}{JPN}まって　いる　トレーナーたちに　ちょうせん\n"
	.string "しますか？$"

TrainerTower_Lobby_Text_StartClockGetSetGo::
	.string "Okay, I'll get the clock started,\n"
	.string "so give it everything you have.\p"
	.string "On your marks…\p"
	.string "Get set…\p"
	.string "Go!$"
	.string "{JPN}{JPN}では　タイムを　はかり　はじめます！\n"
	.string "ぜんりょくで　がんばって　ください\p"
	.string "いちについて‥\p"
	.string "よーい‥\p"
	.string "スタート！$"

TrainerTower_Lobby_Text_PleaseVisitUsAgain::
	.string "Please do visit us again!$"
	.string "{JPN}{JPN}また　ぜひ　おこし　ください！$"

TrainerTower_Lobby_Text_TooBadComeBackTryAgain::
	.string "That was too bad.\p"
	.string "I think you put in a tremendous\n"
	.string "effort in your battling.\p"
	.string "Please come back and try again!$"
	.string "{JPN}{JPN}ざんねん　でしたね\p"
	.string "でも　とても　すばらしい　たたかいぶり　だったと\n"
	.string "おもいます\p"
	.string "また　ちょうせんしに　きて　くださいね！$"

TrainerTower_Lobby_Text_GiveItYourBest::
	.string "I hope you give it your best.$"
	.string "{JPN}{JPN}がんばって　くださいね$"

TrainerTower_Lobby_Text_MoveCounterHereWhenTrainersSwitch::
	.string "When the TRAINERS switch places,\n"
	.string "the movement can be hectic.\p"
	.string "To avoid the stampede, we moved\n"
	.string "the reception counter here.\p"
	.string "I'm sorry for the inconvenience.$"
	.string "{JPN}{JPN}トレーナーが　いれかわる　ときは　とても\n"
	.string "さわがしく　なります\p"
	.string "まきこまれない　ように　うけつけを　ここへ\n"
	.string "うつしました\p"
	.string "ごめいわくを　おかけ　します$"

TrainerTower_Roof_Text_ImOwnerBattledPerfectly::
	.string "Hello…\p"
	.string "I am the owner of this TOWER…\p"
	.string "How the sky soars above this\n"
	.string "rooftop…\p"
	.string "The caress of the winds up here…\p"
	.string "It's all so perfect…\p"
	.string "The way you battled…\n"
	.string "It, too, was perfection…$"
	.string "{JPN}{JPN}こんにちは‥\p"
	.string "わたしは　この　タワーの　オーナー‥\p"
	.string "この　おくじょうから　みる　そらの　たかさ‥\p"
	.string "ふきぬける　かぜの　ここちよさ‥\p"
	.string "すべてが　かんぺき‥\p"
	.string "そして　きみの　たたかいぶりも‥　また　かんぺき\n"
	.string "だった‥$"

TrainerTower_Roof_Text_ThisIsForYou::
	.string "This is for you…$"
	.string "{JPN}{JPN}これを　きみに‥$"

TrainerTower_Roof_Text_DoneItInRecordTime::
	.string "Oh!\n"
	.string "Stupendous!\p"
	.string "It's marvelous how you've come up\n"
	.string "here so quickly.\p"
	.string "The fact is, you've done it in\n"
	.string "record time…\p"
	.string "I'll have your record posted at\n"
	.string "the reception counter.$"
	.string "{JPN}{JPN}おお！　すばらしい！\p"
	.string "こんなに　はやく　ここまで　くるとは　みごとだ\p"
	.string "じつは　きみの　タイムは　しんきろく　なのだ‥\p"
	.string "きみの　きろくを　うけつけに　はりだして　おこう$"

TrainerTower_Roof_Text_TookSweetTimeGettingHere::
	.string "You seem to have taken your sweet\n"
	.string "time getting here…$"
	.string "{JPN}{JPN}ここまで　くるのに　ずいぶん　のんびり　して\n"
	.string "きたようだね‥$"

TrainerTower_Roof_Text_IdLikeToSeeBetterTime::
	.string "What I would like to see is a\n"
	.string "better time out of you…\p"
	.string "I'm counting on you.\p"
	.string "Until then, farewell…$"
	.string "{JPN}{JPN}つぎは　もっと　よい　タイムを　みせて\n"
	.string "もらいたい‥\p"
	.string "きたい　して　いるよ\p"
	.string "では　また　あう　ひまで‥$"

TrainerTower_Text_XMinYZSec::
	.string "{STR_VAR_1} min. {STR_VAR_2}.{STR_VAR_3} sec.$"
	.string "{JPN}{JPN}{STR_VAR_1}ふん\n"
	.string "{STR_VAR_2}.{STR_VAR_3}びょう$"

TrainerTower_Lobby_Text_HereAreTheResults::
	.string "せいせきひょうです$"
	.string "{JPN}{JPN}せいせきひょうです$"

TrainerTower_Lobby_Text_NeedTwoMonsForDouble::
	.string "This is a two-on-two battle.\p"
	.string "You may not battle unless you have\n"
	.string "at least two POKéMON.$"
	.string "{JPN}{JPN}ここは　2たい2の　ダブルバトルです\p"
	.string "ポケモンを　2ひき　いじょう　もって　いないと\n"
	.string "たたかえません$"

TrainerTower_Lobby_Text_ExplainTrainerTower::
	.string "Here at TRAINER TOWER, there is an\n"
	.string "event called TIME ATTACK.\p"
	.string "You will be timed on how quickly\n"
	.string "you can get from the reception\l"
	.string "counter to the OWNER on the roof.\p"
	.string "The best times will be recorded on\n"
	.string "the Time Board.\p"
	.string "Try competing with friends to see\n"
	.string "who can beat it the fastest.\p"
	.string "You will not earn any EXP. Points\n"
	.string "or money by beating TRAINERS here.$"
	.string "{JPN}{JPN}トレーナータワーでは　タイムアタック　という\n"
	.string "イベントを　おこなって　います\p"
	.string "うけつけから　おくじょうの　オーナーの\n"
	.string "ところまで　どれだけ　はやく　いけるかを\l"
	.string "はかります\p"
	.string "いちばん　はやい　タイムは　せいせきひょうに\n"
	.string "きろく　されます\p"
	.string "ともだちと　だれが　いちばん　はやいか　きそって\n"
	.string "みてください\p"
	.string "なお　ここで　トレーナーに　かっても\n"
	.string "けいけんちや　おかねは　もらえません$"

TrainerTower_Lobby_Text_ThanksForCompeting::
	.string "Thank you for competing!$"
	.string "{JPN}{JPN}ちょうせん　ありがとう　ございました！$"

TrainerTower_Lobby_Text_WonderWhatKindsOfTrainers::
	.string "I'm here to see how good I am.\p"
	.string "I wonder what kinds of TRAINERS\n"
	.string "are waiting for me?\p"
	.string "It's nerve-racking!$"
	.string "{JPN}{JPN}じぶんの　じつりょくを　ためしに　きたんだ\p"
	.string "どんな　トレーナーが　まって　いるのかな？\p"
	.string "ドキドキ　するよ！$"

TrainerTower_Lobby_Text_StairsTougherThanAnyBattle::
	.string "Gasp, gasp…\n"
	.string "Gasp…\p"
	.string "Never mind battling! These stairs…\n"
	.string "They're tougher than any battle…$"
	.string "{JPN}{JPN}ハァ　ハァ‥　ハァ‥\p"
	.string "バトル　どころじゃ　ない！　この　かいだん‥\n"
	.string "どんな　バトルより　きつい‥$"

