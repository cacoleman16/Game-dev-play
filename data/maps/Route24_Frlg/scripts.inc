Route24_Frlg_MapScripts::
	.byte 0

Route24_EventScript_Rocket::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_ROUTE24, 1, Route24_EventScript_RocketPostBattle
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNugget
	call Route24_EventScript_BattleRocket
	release
	end

Route24_EventScript_NoRoomForNugget::
	msgbox Route24_Text_YouDontHaveAnyRoom
	release
	end

Route24_EventScript_RocketPostBattle::
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	release
	end

Route24_EventScript_RocketTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTriggerRight::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketApproachPlayer
	call_if_eq VAR_TEMP_1, 1, Route24_EventScript_RocketMotionToPlayer
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNuggetTrigger
	call Route24_EventScript_BattleRocket
	releaseall
	end

Route24_EventScript_BattleRocket::
	giveitem_msg Route24_Text_ReceivedNuggetFromMysteryTrainer, ITEM_NUGGET
	message Route24_Text_JoinTeamRocket
	waitmessage
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	waitbuttonpress
	setvar VAR_LAST_TALKED, LOCALID_ROUTE24_ROCKET
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_6, Route24_Text_RocketDefeat
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	setvar VAR_MAP_SCENE_ROUTE24, 1
	return

Route24_EventScript_RocketApproachPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketApproachPlayer
	waitmovement 0
	return

Route24_EventScript_RocketMotionToPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

Route24_EventScript_NoRoomForNuggetTrigger::
	msgbox Route24_Text_YouDontHaveAnyRoom
	closemessage
	applymovement LOCALID_PLAYER, Route24_Movement_WalkDown
	waitmovement 0
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketWalkBackToPos
	release
	end

Route24_EventScript_RocketWalkBackToPos::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketWalkBackToPos
	waitmovement 0
	return

Route24_Movement_RocketApproachPlayer::
	walk_left
	step_end

Route24_Movement_RocketWalkBackToPos::
	walk_right
	walk_in_place_faster_left
	step_end

Route24_Movement_WalkDown::
	walk_down
	step_end

Route24_Text_JustEarnedFabulousPrize::
	.string "Congratulations! You beat our\n"
	.string "five contest TRAINERS!\p"
	.string "You just earned a fabulous prize!$"
	.string "{JPN}{JPN}おめでとう！　5にんの　トレーナーに\n"
	.string "かちぬいたね！\p"
	.string "すばらしい　しょうひんを　あげよう！$"

Route24_Text_ReceivedNuggetFromMysteryTrainer::
	.string "{PLAYER} received a NUGGET\n"
	.string "from the mystery TRAINER!$"
	.string "{JPN}{JPN}{PLAYER}は　なぞの　トレーナーから\n"
	.string "きんのたまを　もらった！$"

Route24_Text_YouDontHaveAnyRoom::
	.string "You don't have any room!$"
	.string "{JPN}{JPN}にもつが　いっぱいだな！$"

Route24_Text_JoinTeamRocket::
	.string "By the way, how would you like to\n"
	.string "join TEAM ROCKET?\p"
	.string "We're a group of professional\n"
	.string "criminals specializing in POKéMON!\p"
	.string "Want to join?\p"
	.string "Are you sure?\p"
	.string "Come on, join us!\p"
	.string "I'm telling you to join!\p"
	.string "…Okay, you need convincing!\p"
	.string "I'll make you an offer you can't\n"
	.string "refuse!$"
	.string "{JPN}{JPN}ところで　ロケットだんに　はいる　きは　ないか？\p"
	.string "おれたちは　ポケモンを　つかって　わるさを　する\n"
	.string "プロの　あくとう　なんだ！\p"
	.string "はいりたいか？\p"
	.string "ほんとうに　いいのか？\p"
	.string "なあ　はいれよ！\p"
	.string "はいれって　いってるだろ！\p"
	.string "‥しかたない　ちからずくで　いくか！\p"
	.string "いやとは　いわせないぜ！$"

Route24_Text_RocketDefeat::
	.string "Arrgh!\n"
	.string "You are good!$"
	.string "{JPN}{JPN}ぐはっ！　おまえ　つよいな！$"

Route24_Text_YoudBecomeTopRocketLeader::
	.string "With your ability, you'd become\n"
	.string "a top leader in TEAM ROCKET.\p"
	.string "Come on, think of the opportunity!\n"
	.string "Don't let this chance go to waste.$"
	.string "{JPN}{JPN}おまえの　うでなら　ロケットだんの　かんぶにも\n"
	.string "なれるぜ。\p"
	.string "よく　かんがえろ！　こんな　チャンスを　のがす\n"
	.string "てはないぞ。$"

Route24_Text_ShaneIntro::
	.string "I saw your feat from the grass!$"
	.string "{JPN}{JPN}くさむらから　きみの　かつやくを　みてたよ！$"

Route24_Text_ShaneDefeat::
	.string "I thought not!$"
	.string "{JPN}{JPN}やっぱり　だめか！$"

Route24_Text_ShanePostBattle::
	.string "I hid because the people on the\n"
	.string "bridge frightened me.$"
	.string "{JPN}{JPN}はしの　うえの　ひとたちが　こわくて\n"
	.string "かくれてたんだ。$"

Route24_Text_EthanIntro::
	.string "Okay! I'm No. 5!\n"
	.string "I'll stomp you!$"
	.string "{JPN}{JPN}よし！　ぼくが　5にんめだ！　ふみつぶして\n"
	.string "やる！$"

Route24_Text_EthanDefeat::
	.string "Whoa!\n"
	.string "Too much!$"
	.string "{JPN}{JPN}うわっ！　つよすぎ！$"

Route24_Text_EthanPostBattle::
	.string "I did my best. I have no regrets!$"
	.string "{JPN}{JPN}せいいっぱい　やったんだ。　くいは　ないよ！$"

Route24_Text_ReliIntro::
	.string "I'm No. 4!\n"
	.string "Getting tired?$"
	.string "{JPN}{JPN}わたしが　4にんめ！　つかれて　きた？$"

Route24_Text_ReliDefeat::
	.string "I lost, too!$"
	.string "{JPN}{JPN}わたしも　まけちゃった！$"

Route24_Text_ReliPostBattle::
	.string "I did my best, so I've no regrets!$"
	.string "{JPN}{JPN}せいいっぱい　やったから　くいは　ないわ！$"

Route24_Text_TimmyIntro::
	.string "Here's No. 3!\n"
	.string "I won't be easy!$"
	.string "{JPN}{JPN}3にんめは　ぼくだ！　かんたんには　いかないぞ！$"

Route24_Text_TimmyDefeat::
	.string "Ow!\n"
	.string "Stomped flat!$"
	.string "{JPN}{JPN}いてっ！　ぺしゃんこだ！$"

Route24_Text_TimmyPostBattle::
	.string "I did my best. I have no regrets!$"
	.string "{JPN}{JPN}せいいっぱい　やったんだ。　くいは　ないよ！$"

Route24_Text_AliIntro::
	.string "I'm second!\n"
	.string "Now it's serious!$"
	.string "{JPN}{JPN}わたしが　2ばんめよ！　ここからが　ほんばん！$"

Route24_Text_AliDefeat::
	.string "How could I lose?$"
	.string "{JPN}{JPN}どうして　まけたの？$"

Route24_Text_AliPostBattle::
	.string "I did my best. I have no regrets!$"
	.string "{JPN}{JPN}せいいっぱい　やったわ。　くいは　ないわ！$"

Route24_Text_CaleIntro::
	.string "People call this the NUGGET\n"
	.string "BRIDGE!\p"
	.string "Beat us five TRAINERS and win\n"
	.string "a fabulous prize!\p"
	.string "Think you got what it takes?$"
	.string "{JPN}{JPN}ここは　ゴールデンボールブリッジ！\p"
	.string "5にんの　トレーナーに　かちぬけば　すばらしい\n"
	.string "しょうひんが　もらえるぞ！\p"
	.string "きみに　できるかな？$"

Route24_Text_CaleDefeat::
	.string "Whoo!\n"
	.string "Good stuff!$"
	.string "{JPN}{JPN}おおっ！　やるな！$"

Route24_Text_CalePostBattle::
	.string "I did my best. I have no regrets!$"
	.string "{JPN}{JPN}せいいっぱい　やったんだ。　くいは　ないよ！$"
