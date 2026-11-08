SilphCo_7F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_7F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, SilphCo_7F_OnTransition
	.byte 0

SilphCo_7F_OnLoad::
	call_if_unset FLAG_SILPH_7F_DOOR_1, EventScript_Close7FDoor1
	call_if_unset FLAG_SILPH_7F_DOOR_2, EventScript_Close7FDoor2
	call_if_unset FLAG_SILPH_7F_DOOR_3, EventScript_Close7FDoor3
	end

SilphCo_7F_OnTransition::
	call_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_SetObjRocketsGone
	end

SilphCo_7F_EventScript_SetObjRocketsGone::
	setobjectmovementtype LOCALID_SILPH_CO_7F_WORKER_M2, MOVEMENT_TYPE_FACE_UP
	return

SilphCo_7F_EventScript_RivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto SilphCo_7F_EventScript_RivalScene
	end

SilphCo_7F_EventScript_RivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 1
	goto SilphCo_7F_EventScript_RivalScene
	end

SilphCo_7F_EventScript_RivalScene::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	applymovement LOCALID_SILPH_CO_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_SILPH_CO_RIVAL, Common_Movement_Delay48
	waitmovement 0
	msgbox SilphCo_7F_Text_RivalWhatKeptYou
	call_if_eq VAR_TEMP_1, 0, SilphCo_7F_EventScript_RivalApproachTop
	call_if_eq VAR_TEMP_1, 1, SilphCo_7F_EventScript_RivalApproachBottom
	msgbox SilphCo_7F_Text_RivalIntro
	setvar VAR_LAST_TALKED, LOCALID_SILPH_CO_RIVAL
	call_if_eq VAR_KANTO_STARTER, 2, SilphCo_7F_EventScript_RivalSquirtle
	call_if_eq VAR_KANTO_STARTER, 1, SilphCo_7F_EventScript_RivalBulbasaur
	call_if_eq VAR_KANTO_STARTER, 0, SilphCo_7F_EventScript_RivalCharmander
	msgbox SilphCo_7F_Text_RivalPostBattle
	closemessage
	playbgm MUS_RG_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, SilphCo_7F_EventScript_RivalExitTop
	call_if_eq VAR_TEMP_1, 1, SilphCo_7F_EventScript_RivalExitBottom
	playse SE_WARP_IN
	fadedefaultbgm
	removeobject LOCALID_SILPH_CO_RIVAL
	waitse
	setvar VAR_MAP_SCENE_SILPH_CO_7F, 1
	releaseall
	end

SilphCo_7F_EventScript_RivalApproachTop::
	closemessage
	applymovement LOCALID_SILPH_CO_RIVAL, SilphCo_7F_Movement_RivalApproachTop
	waitmovement 0
	return

SilphCo_7F_EventScript_RivalApproachBottom::
	return

SilphCo_7F_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_SILPH_SQUIRTLE, SilphCo_7F_Text_RivalDefeat
	return

SilphCo_7F_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_SILPH_BULBASAUR, SilphCo_7F_Text_RivalDefeat
	return

SilphCo_7F_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_SILPH_CHARMANDER, SilphCo_7F_Text_RivalDefeat
	return

SilphCo_7F_EventScript_RivalExitTop::
	applymovement LOCALID_SILPH_CO_RIVAL, SilphCo_7F_Movement_RivalExitTop
	waitmovement 0
	return

SilphCo_7F_EventScript_RivalExitBottom::
	applymovement LOCALID_SILPH_CO_RIVAL, SilphCo_7F_Movement_RivalExitBottom
	waitmovement 0
	return

SilphCo_7F_Movement_RivalApproachTop::
	walk_up
	step_end

SilphCo_7F_Movement_RivalExitTop::
	walk_right
	walk_right
	walk_right
	walk_up
	delay_16
	step_end

SilphCo_7F_Movement_RivalExitBottom::
	walk_left
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	delay_16
	step_end

SilphCo_7F_EventScript_LaprasGuy::
	lock
	faceplayer
	goto_if_set FLAG_GOT_LAPRAS_FROM_SILPH, SilphCo_7F_EventScript_AlreadyGotLapras
	msgbox SilphCo_7F_Text_HaveMonForSavingUs
	setvar VAR_TEMP_1, SPECIES_LAPRAS
	givemon SPECIES_LAPRAS, 25
	goto_if_eq VAR_RESULT, 0, SilphCo_7F_EventScript_ReceiveLaprasParty
	goto_if_eq VAR_RESULT, 1, SilphCo_7F_EventScript_ReceiveLaprasPC
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	release
	end

SilphCo_7F_EventScript_ReceiveLaprasParty::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message SilphCo_7F_Text_ObtainedLaprasFromEmployee
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_LAPRAS
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SilphCo_7F_EventScript_EndReceiveLapras
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto SilphCo_7F_EventScript_EndReceiveLapras
	end

SilphCo_7F_EventScript_ReceiveLaprasPC::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message SilphCo_7F_Text_ObtainedLaprasFromEmployee
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_LAPRAS
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SilphCo_7F_EventScript_LaprasTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto SilphCo_7F_EventScript_LaprasTransferredToPC
	end

SilphCo_7F_EventScript_LaprasTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto SilphCo_7F_EventScript_EndReceiveLapras
	end

SilphCo_7F_EventScript_EndReceiveLapras::
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SilphCo_7F_Text_ExplainLapras
	setflag FLAG_GOT_LAPRAS_FROM_SILPH
	release
	end

SilphCo_7F_EventScript_AlreadyGotLapras::
	msgbox SilphCo_7F_Text_RocketBossWentToBoardroom
	release
	end

SilphCo_7F_EventScript_WorkerM1::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_WorkerM1RocketsGone
	msgbox SilphCo_7F_Text_RocketsAfterMasterBall
	release
	end

SilphCo_7F_EventScript_WorkerM1RocketsGone::
	msgbox SilphCo_7F_Text_CanceledMasterBallProject
	release
	end

SilphCo_7F_EventScript_WorkerM2::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_WorkerM2RocketsGone
	msgbox SilphCo_7F_Text_BadIfTeamRocketTookOver
	release
	end

SilphCo_7F_EventScript_WorkerM2RocketsGone::
	msgbox SilphCo_7F_Text_WowYouChasedOffTeamRocket
	release
	end

SilphCo_7F_EventScript_WorkerF::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_WorkerFRocketsGone
	msgbox SilphCo_7F_Text_ReallyDangerousHere
	release
	end

SilphCo_7F_EventScript_WorkerFRocketsGone::
	msgbox SilphCo_7F_Text_ThankYouSoMuch
	release
	end

SilphCo_7F_EventScript_FloorSign::
	msgbox SilphCo_7F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_7F_EventScript_Joshua::
	trainerbattle_single TRAINER_SCIENTIST_JOSHUA, SilphCo_7F_Text_JoshuaIntro, SilphCo_7F_Text_JoshuaDefeat
	msgbox SilphCo_7F_Text_JoshuaPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_33, SilphCo_7F_Text_Grunt1Intro, SilphCo_7F_Text_Grunt1Defeat
	msgbox SilphCo_7F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_34, SilphCo_7F_Text_Grunt2Intro, SilphCo_7F_Text_Grunt2Defeat
	msgbox SilphCo_7F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_35, SilphCo_7F_Text_Grunt3Intro, SilphCo_7F_Text_Grunt3Defeat
	msgbox SilphCo_7F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_Text_HaveMonForSavingUs::
	.string "Oh! Hi! You're not a ROCKET!\n"
	.string "You came to save us?\l"
	.string "Why, thank you!\p"
	.string "I want you to have this POKéMON\n"
	.string "for saving us.$"
	.string "{JPN}{JPN}あっ！　こんにちは！　ロケットだんじゃ\n"
	.string "ないのね！　たすけに　きて　くれたの？　まあ\l"
	.string "ありがとう！\p"
	.string "たすけて　くれた　おれいに　この　ポケモンを\n"
	.string "もらって　ほしいの。$"

SilphCo_7F_Text_ObtainedLaprasFromEmployee::
	.string "{PLAYER} obtained a LAPRAS from\n"
	.string "the SILPH employee!$"
	.string "{JPN}{JPN}{PLAYER}は　シルフの　しゃいんから\n"
	.string "ラプラスを　もらった！$"

SilphCo_7F_Text_ExplainLapras::
	.string "It's a LAPRAS.\n"
	.string "It's a very intelligent POKéMON.\p"
	.string "We kept it in our lab, but it will\n"
	.string "be much better off with you.\p"
	.string "I think you will be a good TRAINER\n"
	.string "for LAPRAS!\p"
	.string "It's a good swimmer.\n"
	.string "It'll give you a lift across water!$"
	.string "{JPN}{JPN}それは　ラプラス。　とても　かしこい\n"
	.string "ポケモンよ。\p"
	.string "けんきゅうじょで　かって　いたけど　あなたと\n"
	.string "いる　ほうが　ずっと　しあわせだと　おもうの。\p"
	.string "あなたなら　ラプラスの　いい　トレーナーに\n"
	.string "なれるわ！\p"
	.string "およぎが　とくい　だから　みずの　うえを\n"
	.string "はこんで　くれるわよ！$"

SilphCo_7F_Text_RocketBossWentToBoardroom::
	.string "TEAM ROCKET's BOSS went to the\n"
	.string "boardroom!\p"
	.string "Is our PRESIDENT okay?\n"
	.string "I'm worried.$"
	.string "{JPN}{JPN}ロケットだんの　ボスが　しゃちょうしつに\n"
	.string "いったんだ！\p"
	.string "しゃちょうは　だいじょうぶ　だろうか？\n"
	.string "しんぱいだ。$"

SilphCo_7F_Text_RocketsAfterMasterBall::
	.string "TEAM ROCKET was after the MASTER\n"
	.string "BALL, which catches any POKéMON.$"
	.string "{JPN}{JPN}ロケットだんは　どんな　ポケモンでも\n"
	.string "つかまえられる　マスターボールを　ねらって\l"
	.string "いたんだ。$"

SilphCo_7F_Text_CanceledMasterBallProject::
	.string "We canceled the MASTER BALL\n"
	.string "project because of TEAM ROCKET.$"
	.string "{JPN}{JPN}ロケットだんの　せいで　マスターボールの\n"
	.string "けいかくは　ちゅうし　したんだ。$"

SilphCo_7F_Text_BadIfTeamRocketTookOver::
	.string "It would be bad if TEAM ROCKET\n"
	.string "took over SILPH or our POKéMON.$"
	.string "{JPN}{JPN}シルフや　ポケモンが　ロケットだんに\n"
	.string "のっとられたら　たいへんだ。$"

SilphCo_7F_Text_WowYouChasedOffTeamRocket::
	.string "Wow!\p"
	.string "You chased off TEAM ROCKET all by\n"
	.string "yourself?$"
	.string "{JPN}{JPN}すごい！\p"
	.string "たった　ひとりで　ロケットだんを\n"
	.string "おいはらったの？$"

SilphCo_7F_Text_ReallyDangerousHere::
	.string "You!\n"
	.string "It's really dangerous here!\p"
	.string "You came to save me?\n"
	.string "You can't!$"
	.string "{JPN}{JPN}きみ！　ここは　ほんとうに　あぶないよ！\p"
	.string "ぼくを　たすけに　きたって？　むりだよ！$"

SilphCo_7F_Text_ThankYouSoMuch::
	.string "Thank you so much!$"
	.string "{JPN}{JPN}ほんとうに　ありがとう！$"

SilphCo_7F_Text_Grunt3Intro::
	.string "Uh-oh!\n"
	.string "I smell a little mouse!$"
	.string "{JPN}{JPN}おっと！　ちいさな　ネズミの　においが　するぜ！$"

SilphCo_7F_Text_Grunt3Defeat::
	.string "Lights out!$"
	.string "{JPN}{JPN}ノックアウトだ！$"

SilphCo_7F_Text_Grunt3PostBattle::
	.string "You won't find my BOSS by just\n"
	.string "scurrying around!$"
	.string "{JPN}{JPN}ちょろちょろ　うごきまわっても　ボスは\n"
	.string "みつからないぜ！$"

SilphCo_7F_Text_JoshuaIntro::
	.string "Heheh!\p"
	.string "You mistook me for a SILPH worker?$"
	.string "{JPN}{JPN}ヘッヘッヘ！\p"
	.string "おれを　シルフの　しゃいんと　まちがえたか？$"

SilphCo_7F_Text_JoshuaDefeat::
	.string "I'm done!$"
	.string "{JPN}{JPN}まいった！$"

SilphCo_7F_Text_JoshuaPostBattle::
	.string "Despite your age, you are a\n"
	.string "skilled TRAINER!$"
	.string "{JPN}{JPN}わかいのに　なかなか　うでの　たつ\n"
	.string "トレーナーだ！$"

SilphCo_7F_Text_Grunt1Intro::
	.string "I am one of the four ROCKET\n"
	.string "BROTHERS!$"
	.string "{JPN}{JPN}おれは　ロケット　4きょうだいの　ひとり！$"

SilphCo_7F_Text_Grunt1Defeat::
	.string "Aack!\n"
	.string "Brothers, I lost!$"
	.string "{JPN}{JPN}ぐわっ！　きょうだい　まけちまった！$"

SilphCo_7F_Text_Grunt1PostBattle::
	.string "Doesn't matter.\n"
	.string "My brothers will repay the favor!$"
	.string "{JPN}{JPN}かまわねえ。　きょうだいたちが　おかえし　して\n"
	.string "くれるさ！$"

SilphCo_7F_Text_Grunt2Intro::
	.string "A child intruder?\n"
	.string "That must be you!$"
	.string "{JPN}{JPN}こどもの　しんにゅうしゃ？　おまえの　ことだな！$"

SilphCo_7F_Text_Grunt2Defeat::
	.string "Fine!\n"
	.string "I lost!$"
	.string "{JPN}{JPN}わかったよ！　おれの　まけだ！$"

SilphCo_7F_Text_Grunt2PostBattle::
	.string "Go on home before my BOSS gets\n"
	.string "ticked off!$"
	.string "{JPN}{JPN}ボスが　おこりだす　まえに　さっさと　かえりな！$"

SilphCo_7F_Text_RivalWhatKeptYou::
	.string "{RIVAL}: What kept you, {PLAYER}?$"
	.string "{JPN}{JPN}{RIVAL}『おそかったな　{PLAYER}！』$"

SilphCo_7F_Text_RivalIntro::
	.string "{RIVAL}: Hahaha! I thought you'd\n"
	.string "turn up if I waited here!\p"
	.string "I guess TEAM ROCKET slowed you\n"
	.string "down! Not that I care!\p"
	.string "I saw you in SAFFRON, so I decided\n"
	.string "to see if you got better!$"
	.string "{JPN}{JPN}{RIVAL}『ハッハッハ！　ここで　まってれば\n"
	.string "くると　おもってたぜ！\p"
	.string "ロケットだんに　てまどったん　だろ！　ま\n"
	.string "おれには　かんけい　ないけどな！\p"
	.string "ヤマブキで　おまえを　みかけたから　どれだけ\n"
	.string "つよく　なったか　ためして　やろうと\l"
	.string "おもってさ！』$"

SilphCo_7F_Text_RivalDefeat::
	.string "Oh, man!\n"
	.string "So, you are ready for BOSS\l"
	.string "ROCKET!$"
	.string "{JPN}{JPN}ちぇっ！　これなら　ロケットだんの　ボスとも\n"
	.string "たたかえるな！$"

SilphCo_7F_Text_RivalPostBattle::
	.string "Well, {PLAYER}!\n"
	.string "I'm moving on up and ahead!\p"
	.string "By checking my POKéDEX, I'm\n"
	.string "starting to see what's strong and\l"
	.string "how they evolve!\p"
	.string "Am I a genius or what?\p"
	.string "I'm going to the POKéMON LEAGUE to\n"
	.string "boot out the ELITE FOUR.\p"
	.string "I'll become the world's most\n"
	.string "powerful TRAINER!\p"
	.string "{PLAYER}, well, good luck to you!\n"
	.string "Don't sweat it!\l"
	.string "Smell ya!$"
	.string "{JPN}{JPN}じゃあな　{PLAYER}！　おれは　さきに\n"
	.string "いくぜ！\p"
	.string "ずかんを　みてたら　どの　ポケモンが　つよいのか\n"
	.string "どう　しんか　するのか　わかって　きたぜ！\p"
	.string "おれって　てんさい　だろ？\p"
	.string "おれは　ポケモンリーグに　いって　してんのうを\n"
	.string "けちらして　やる。\p"
	.string "そして　せかいで　いちばん　つよい　トレーナーに\n"
	.string "なるんだ！\p"
	.string "{PLAYER}　まあ　せいぜい　がんばれよ！\n"
	.string "きにすんなって！　あばよ！$"

SilphCo_7F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "7F$"
	.string "{JPN}{JPN}シルフカンパニー　ほんしゃ\n"
	.string "7かい$"

