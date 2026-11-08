Route22_Frlg_MapScripts::
	.byte 0

Route22_EventScript_EarlyRivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRival::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	addobject LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_EarlyRivalApproach
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_EarlyRivalApproach
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_EarlyRivalApproachBottom
	delay 6
	msgbox Route22_Text_EarlyRivalIntro
	call_if_eq VAR_KANTO_STARTER, 2, Route22_EventScript_EarlyRivalSquirtle
	call_if_eq VAR_KANTO_STARTER, 1, Route22_EventScript_EarlyRivalBulbasaur
	call_if_eq VAR_KANTO_STARTER, 0, Route22_EventScript_EarlyRivalCharmander
	msgbox Route22_Text_EarlyRivalPostBattle
	closemessage
	delay 10
	playbgm MUS_RG_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_EarlyRivalExit
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_EarlyRivalExit
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_EarlyRivalExitBottom
	fadedefaultbgm
	removeobject LOCALID_ROUTE22_RIVAL
	setvar VAR_MAP_SCENE_ROUTE22, 2
	releaseall
	end

Route22_EventScript_EarlyRivalApproach::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproach
	waitmovement 0
	return

Route22_EventScript_EarlyRivalApproachBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproachBottom
	applymovement LOCALID_PLAYER, Route22_Movement_PlayerFaceRival
	waitmovement 0
	return

Route22_EventScript_EarlyRivalSquirtle::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE, 0, Route22_Text_EarlyRivalDefeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalBulbasaur::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_BULBASAUR, 0, Route22_Text_EarlyRivalDefeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalCharmander::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_CHARMANDER, 0, Route22_Text_EarlyRivalDefeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalExit::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_EarlyRivalExit
	waitmovement 0
	return

Route22_EventScript_EarlyRivalExitBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_EarlyRivalExitBottom
	waitmovement 0
	return

Route22_Movement_UnusedRivalExit:: @ 8
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route22_Movement_EarlyRivalExit::
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route22_Movement_EarlyRivalExitBottom::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route22_Movement_RivalApproach::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route22_Movement_RivalApproachBottom::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_down
	step_end

Route22_Movement_PlayerFaceRival::
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_up
	step_end

Route22_EventScript_LateRivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRival::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	addobject LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_LateRivalApproach
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_LateRivalApproach
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_LateRivalApproachBottom
	msgbox Route22_Text_LateRivalIntro
	setvar VAR_LAST_TALKED, LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_KANTO_STARTER, 2, Route22_EventScript_LateRivalSquirtle
	call_if_eq VAR_KANTO_STARTER, 1, Route22_EventScript_LateRivalBulbasaur
	call_if_eq VAR_KANTO_STARTER, 0, Route22_EventScript_LateRivalCharmander
	msgbox Route22_Text_LateRivalPostBattle
	closemessage
	delay 10
	playbgm MUS_RG_RIVAL_EXIT, 0
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_LateRivalExit
	waitmovement 0
	fadedefaultbgm
	removeobject LOCALID_ROUTE22_RIVAL
	setvar VAR_MAP_SCENE_ROUTE22, 4
	releaseall
	end

Route22_EventScript_LateRivalApproach::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproach
	waitmovement 0
	return

Route22_EventScript_LateRivalApproachBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproachBottom
	applymovement LOCALID_PLAYER, Route22_Movement_PlayerFaceRival
	waitmovement 0
	return

Route22_EventScript_LateRivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE, Route22_Text_LateRivalDefeat
	return

Route22_EventScript_LateRivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_BULBASAUR, Route22_Text_LateRivalDefeat
	return

Route22_EventScript_LateRivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER, Route22_Text_LateRivalDefeat
	return

Route22_Movement_LateRivalExit::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route22_EventScript_LeagueGateSign::
	msgbox Route22_Text_LeagueGateSign, MSGBOX_SIGN
	end

Route22_Text_EarlyRivalIntro::
	.string "{RIVAL}: Hey! {PLAYER}!\p"
	.string "You're off to the POKéMON LEAGUE?\n"
	.string "Forget about it!\p"
	.string "You probably don't have any\n"
	.string "BADGES, do you?\p"
	.string "The guard won't let you through\n"
	.string "without them.\p"
	.string "By the way, did your POKéMON get\n"
	.string "any stronger?$"
	.string "{JPN}{JPN}{RIVAL}『おっ！\n"
	.string "{PLAYER}じゃないか！\p"
	.string "ポケモンリーグに　いくのか？　やめとけって！\p"
	.string "どうせ　バッジ　もってないんだろ？\p"
	.string "バッジが　なけりゃ　みはりの　ひとが　とおして\n"
	.string "くれないぜ。\p"
	.string "ところで　おまえの　ポケモン　すこしは　つよく\n"
	.string "なったか？$"

Route22_Text_EarlyRivalDefeat::
	.string "Awww!\n"
	.string "You just lucked out!$"
	.string "{JPN}{JPN}なにー！　まぐれ　だろ！$"

Route22_Text_EarlyRivalPostBattle::
	.string "I heard the POKéMON LEAGUE is\n"
	.string "crawling with tough TRAINERS.\p"
	.string "I have to figure out how to get\n"
	.string "past them.\p"
	.string "You should quit dawdling and get a\n"
	.string "move on!$"
	.string "{JPN}{JPN}ポケモンリーグには　つよい　トレーナーが\n"
	.string "うじゃうじゃ　いるらしいぜ。\p"
	.string "そいつらを　どうやって　かちぬくか\n"
	.string "かんがえないとな。\p"
	.string "おまえも　ぐずぐず　してないで　さっさと\n"
	.string "いけよ！$"

@ Translated in RB as "{RIVAL}: What? Why do I have 2 POKéMON? You should catch some more too!"
Route22_Text_RivalShouldCatchSomeMons::
	.string "{RIVAL}“なんだ？\n"
	.string "ポケモン　2ひきも\l"
	.string "もってるの　なぜか　だって？\p"
	.string "おまえも\n"
	.string "つかまえれば　いい　じゃん！$"
	.string "{JPN}{JPN}{RIVAL}『なんだ？　ポケモン　2ひきも\n"
	.string "もってるの　なぜか　だって？\p"
	.string "おまえも　つかまえれば　いい　じゃん！$"

Route22_Text_LateRivalIntro::
	.string "{RIVAL}: What? {PLAYER}!\n"
	.string "What a surprise to see you here!\p"
	.string "So you're going to the POKéMON\n"
	.string "LEAGUE?\p"
	.string "You collected all the BADGES, too?\n"
	.string "That's cool!\p"
	.string "Then I'll whip you, {PLAYER}, as a\n"
	.string "warm-up for the POKéMON LEAGUE!\p"
	.string "Come on!$"
	.string "{JPN}{JPN}{RIVAL}『なに？　{PLAYER}！　こんな\n"
	.string "ところで　あうとはな！\p"
	.string "おまえも　ポケモンリーグに　いくのか？\p"
	.string "バッジも　ぜんぶ　あつめたのか？　やるじゃん！\p"
	.string "じゃあ　ポケモンリーグの　まえの　じゅんび\n"
	.string "うんどうに　{PLAYER}を　やっつけて\l"
	.string "やるぜ！\p"
	.string "かかってこい！$"

Route22_Text_LateRivalDefeat::
	.string "What!?\p"
	.string "I was just careless, you!$"
	.string "{JPN}{JPN}なにっ！？\p"
	.string "ちょっと　ゆだん　した　だけだ！$"

Route22_Text_LateRivalPostBattle::
	.string "That loosened me up.\n"
	.string "I'm ready for the POKéMON LEAGUE!\p"
	.string "{PLAYER}, you need more practice.\p"
	.string "But hey, you know that!\n"
	.string "I'm out of here. Smell ya!$"
	.string "{JPN}{JPN}おかげで　からだが　ほぐれたぜ。\n"
	.string "ポケモンリーグの　じゅんびは　ばっちりだ！\p"
	.string "{PLAYER}　おまえは　もっと　れんしゅう\n"
	.string "しなきゃな。\p"
	.string "ま　わかってる　だろうけどな！　おれは　いくぜ！\n"
	.string "あばよ！$"

@ Translated in RB as "{RIVAL}: Hahaha! {PLAYER}! That's your best? You're nowhere near as good as me, pal! Go train some more! You loser!"
Route22_Text_LateRivalVictory::
	.string "{RIVAL}“ひゃははッ　{PLAYER}ー！\n"
	.string "それで　がんばってるのかよ！\l"
	.string "おれの　さいのうに　くらべりゃ\l"
	.string "{PLAYER}は　まだまだ　だな！\p"
	.string "もっと　れんしゅう　こいよ！\n"
	.string "あははーッ！$"
	.string "{JPN}{JPN}{RIVAL}『ひゃははッ　{PLAYER}ー！\n"
	.string "それで　がんばってるのかよ！　おれの　さいのうに\l"
	.string "くらべりゃ　{PLAYER}は　まだまだ　だな！\p"
	.string "もっと　れんしゅう　こいよ！　あははーッ！$"

Route22_Text_LeagueGateSign::
	.string "POKéMON LEAGUE\n"
	.string "Front Gate$"
	.string "{JPN}{JPN}ポケモンリーグ\n"
	.string "しょうめん　ゲート$"

