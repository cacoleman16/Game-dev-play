ThreeIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ThreeIsland_OnTransition
	.byte 0

ThreeIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_THREE_ISLAND
	call_if_set FLAG_RESCUED_LOSTELLE, ThreeIsland_EventScript_HideAntiBikers
	call_if_eq VAR_MAP_SCENE_THREE_ISLAND, 4, ThreeIsland_EventScript_SetAntiBikersMovementAfterBikers
	end

ThreeIsland_EventScript_HideAntiBikers::
	setflag FLAG_HIDE_THREE_ISLAND_ANTIBIKERS
	return

ThreeIsland_EventScript_SetAntiBikersMovementAfterBikers::
	setobjectmovementtype LOCALID_THREE_ISLAND_ANTIBIKER1, MOVEMENT_TYPE_LOOK_AROUND
	setobjectmovementtype LOCALID_THREE_ISLAND_ANTIBIKER2, MOVEMENT_TYPE_LOOK_AROUND
	return

ThreeIsland_EventScript_Biker::
	lock
	setvar VAR_TEMP_1, 2
	call ThreeIsland_EventScript_BikerArgumentScene
	release
	end

ThreeIsland_EventScript_AntiBiker1::
	lock
	goto_if_set FLAG_GOT_FULL_RESTORE_FROM_THREE_ISLAND_DEFENDER, ThreeIsland_EventScript_AntiBiker1GotFullRestore
	goto_if_eq VAR_MAP_SCENE_THREE_ISLAND, 4, ThreeIsland_EventScript_GiveFullRestore
	setvar VAR_TEMP_1, 0
	call ThreeIsland_EventScript_BikerArgumentScene
	release
	end

ThreeIsland_EventScript_AntiBiker1GotFullRestore::
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER1, Common_Movement_FacePlayer
	waitmovement 0
	msgbox ThreeIsland_Text_YouveGotImpressiveMons
	release
	end

ThreeIsland_EventScript_GiveFullRestore::
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER1, Common_Movement_FacePlayer
	waitmovement 0
	msgbox ThreeIsland_Text_ThankYouOhYourMonGotHurt
	checkitemspace ITEM_FULL_RESTORE
	goto_if_eq VAR_RESULT, FALSE, ThreeIsland_EventScript_NoRoomForFullRestore
	msgreceiveditem ThreeIsland_Text_GivenFullRestore, ITEM_FULL_RESTORE
	additem ITEM_FULL_RESTORE
	setflag FLAG_GOT_FULL_RESTORE_FROM_THREE_ISLAND_DEFENDER
	msgbox ThreeIsland_Text_YouveGotImpressiveMons
	release
	end

ThreeIsland_EventScript_NoRoomForFullRestore::
	msgbox ThreeIsland_Text_OhYourBagIsFull
	release
	end

ThreeIsland_EventScript_BikerArgumentScene::
	call_if_eq VAR_TEMP_1, 1, ThreeIsland_EventScript_PlayerFaceLeft
	call_if_eq VAR_TEMP_1, 2, ThreeIsland_EventScript_PlayerFaceLeft
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER1, ThreeIsland_Movement_SpeakRight
	waitmovement 0
	msgbox ThreeIsland_Text_GoBackToKanto
	call_if_eq VAR_TEMP_1, 0, ThreeIsland_EventScript_PlayerFaceRight
	call_if_eq VAR_TEMP_1, 1, ThreeIsland_EventScript_PlayerFaceRight
	call_if_eq VAR_TEMP_1, 2, ThreeIsland_EventScript_PlayerFaceBiker
	applymovement LOCALID_THREE_ISLAND_BIKER1, ThreeIsland_Movement_SpeakLeft
	waitmovement 0
	msgbox ThreeIsland_Text_BossIsOnHisWay
	call_if_eq VAR_TEMP_1, 0, ThreeIsland_EventScript_PlayerFaceUp
	call_if_eq VAR_TEMP_1, 1, ThreeIsland_EventScript_PlayerFaceUp
	call_if_eq VAR_TEMP_1, 2, ThreeIsland_EventScript_PlayerFaceAntiBiker
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER2, ThreeIsland_Movement_SpeakRight
	waitmovement 0
	msgbox ThreeIsland_Text_GetOffIslandNow
	call_if_eq VAR_TEMP_1, 0, ThreeIsland_EventScript_PlayerFaceRight
	call_if_eq VAR_TEMP_1, 1, ThreeIsland_EventScript_PlayerFaceRight
	call_if_eq VAR_TEMP_1, 2, ThreeIsland_EventScript_PlayerFaceBiker
	applymovement LOCALID_THREE_ISLAND_BIKER3, ThreeIsland_Movement_SpeakLeft
	waitmovement 0
	msgbox ThreeIsland_Text_WhosGonnaMakeMe
	return

ThreeIsland_EventScript_PlayerFaceUp::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

@ Unused
ThreeIsland_EventScript_PlayerFaceDown::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

ThreeIsland_EventScript_PlayerFaceLeft::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

ThreeIsland_EventScript_PlayerFaceRight::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

ThreeIsland_EventScript_PlayerFaceBiker::
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_ge VAR_0x8004, 9, ThreeIsland_EventScript_PlayerFaceUp
	goto ThreeIsland_EventScript_PlayerFaceRight
	end

ThreeIsland_EventScript_PlayerFaceAntiBiker::
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_ge VAR_0x8004, 9, ThreeIsland_EventScript_PlayerFaceLeft
	goto ThreeIsland_EventScript_PlayerFaceUp
	end

ThreeIsland_EventScript_AntiBiker2::
	lock
	goto_if_eq VAR_MAP_SCENE_THREE_ISLAND, 4, ThreeIsland_EventScript_AntiBiker2BikersGone
	setvar VAR_TEMP_1, 1
	call ThreeIsland_EventScript_BikerArgumentScene
	release
	end

ThreeIsland_EventScript_AntiBiker2BikersGone::
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER2, Common_Movement_FacePlayer
	waitmovement 0
	msgbox ThreeIsland_Text_LostelleWentOffTowardsBondBridge
	release
	end

ThreeIsland_EventScript_BikerBossIntroTrigger::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER1, ThreeIsland_Movement_SpeakRight
	waitmovement 0
	msgbox ThreeIsland_Text_AreYouBossGoBackToKanto
	applymovement LOCALID_PAXTON, ThreeIsland_Movement_SpeakLeft
	waitmovement 0
	msgbox ThreeIsland_Text_JustGotHerePal
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER1, ThreeIsland_Movement_SpeakRight
	waitmovement 0
	msgbox ThreeIsland_Text_FollowersRaisingHavoc
	applymovement LOCALID_PAXTON, ThreeIsland_Movement_SpeakLeft
	waitmovement 0
	msgbox ThreeIsland_Text_OughtToBeThankingUs
	applymovement LOCALID_THREE_ISLAND_ANTIBIKER1, ThreeIsland_Movement_SpeakRight
	waitmovement 0
	msgbox ThreeIsland_Text_YouCowardsToughInPack
	setvar VAR_MAP_SCENE_THREE_ISLAND, 3
	releaseall
	end

ThreeIsland_Movement_SpeakLeft::
	walk_in_place_left
	step_end

ThreeIsland_Movement_SpeakRight::
	walk_in_place_right
	step_end

ThreeIsland_EventScript_BattleBikersTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto ThreeIsland_EventScript_BattleBikersScene
	end

ThreeIsland_EventScript_BattleBikersTriggerMidLeft::
	lockall
	setvar VAR_TEMP_1, 1
	goto ThreeIsland_EventScript_BattleBikersScene
	end

ThreeIsland_EventScript_BattleBikersTriggerMid::
	lockall
	setvar VAR_TEMP_1, 2
	goto ThreeIsland_EventScript_BattleBikersScene
	end

ThreeIsland_EventScript_BattleBikersTriggerMidRight::
	lockall
	setvar VAR_TEMP_1, 3
	goto ThreeIsland_EventScript_BattleBikersScene
	end

ThreeIsland_EventScript_BattleBikersTriggerRight::
	lockall
	setvar VAR_TEMP_1, 4
	goto ThreeIsland_EventScript_BattleBikersScene
	end

ThreeIsland_EventScript_BattleBikersScene::
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_THREE_ISLAND_BIKER1, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_THREE_ISLAND_BIKER1, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_THREE_ISLAND_BIKER1, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_THREE_ISLAND_BIKER1, ThreeIsland_Movement_BikerApproach
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_BOY, 0
	msgbox ThreeIsland_Text_WannaMakeSomethingOfYourStaring, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ThreeIsland_EventScript_LeaveBikersAlone
	msgbox ThreeIsland_Text_Biker1Intro
	setvar VAR_LAST_TALKED, LOCALID_THREE_ISLAND_BIKER1
	trainerbattle_no_intro TRAINER_BIKER_GOON, ThreeIsland_Text_Biker1Defeat
	applymovement LOCALID_THREE_ISLAND_BIKER1, ThreeIsland_Movement_BikerSpeak
	waitmovement 0
	msgbox ThreeIsland_Text_Biker1PostBattle
	closemessage
	applymovement LOCALID_THREE_ISLAND_BIKER2, ThreeIsland_Movement_BikerApproach
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_BOY, 0
	msgbox ThreeIsland_Text_Biker2Intro
	setvar VAR_LAST_TALKED, LOCALID_THREE_ISLAND_BIKER2
	trainerbattle_no_intro TRAINER_BIKER_GOON_2, ThreeIsland_Text_Biker2Defeat
	applymovement LOCALID_THREE_ISLAND_BIKER2, ThreeIsland_Movement_BikerSpeak
	waitmovement 0
	msgbox ThreeIsland_Text_Biker2PostBattle
	closemessage
	applymovement LOCALID_THREE_ISLAND_BIKER3, ThreeIsland_Movement_BikerApproach
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_BOY, 0
	msgbox ThreeIsland_Text_Biker3Intro
	setvar VAR_LAST_TALKED, LOCALID_THREE_ISLAND_BIKER3
	trainerbattle_no_intro TRAINER_BIKER_GOON_3, ThreeIsland_Text_Biker3Defeat
	applymovement LOCALID_THREE_ISLAND_BIKER3, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_THREE_ISLAND_BIKER4, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_THREE_ISLAND_BIKER5, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox ThreeIsland_Text_Biker3PostBattle
	closemessage
	delay 45
	call_if_eq VAR_TEMP_1, 0, ThreeIsland_EventScript_PaxtonApproachLeft
	call_if_eq VAR_TEMP_1, 1, ThreeIsland_EventScript_PaxtonApproachMidLeft
	call_if_eq VAR_TEMP_1, 2, ThreeIsland_EventScript_PaxtonApproachMid
	call_if_eq VAR_TEMP_1, 3, ThreeIsland_EventScript_PaxtonApproachMidRight
	call_if_eq VAR_TEMP_1, 4, ThreeIsland_EventScript_PaxtonApproachRight
	playbgm MUS_RG_ENCOUNTER_BOY, 0
	msgbox ThreeIsland_Text_PaxtonIntro
	setvar VAR_LAST_TALKED, LOCALID_PAXTON
	trainerbattle_no_intro TRAINER_CUE_BALL_PAXTON, ThreeIsland_Text_PaxtonDefeat
	msgbox ThreeIsland_Text_PaxtonPostBattle
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_PAXTON
	removeobject LOCALID_THREE_ISLAND_BIKER1
	removeobject LOCALID_THREE_ISLAND_BIKER2
	removeobject LOCALID_THREE_ISLAND_BIKER3
	removeobject LOCALID_THREE_ISLAND_BIKER4
	removeobject LOCALID_THREE_ISLAND_BIKER5
	removeobject LOCALID_THREE_ISLAND_BIKER6
	fadescreenswapbuffers FADE_FROM_BLACK
	setvar VAR_MAP_SCENE_THREE_ISLAND, 4
	releaseall
	end

ThreeIsland_EventScript_PaxtonApproachLeft::
	applymovement LOCALID_THREE_ISLAND_BIKER3, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PAXTON, ThreeIsland_Movement_PaxtonApproachLeft
	waitmovement 0
	return

ThreeIsland_EventScript_PaxtonApproachMidLeft::
	applymovement LOCALID_THREE_ISLAND_BIKER3, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PAXTON, ThreeIsland_Movement_PaxtonApproachMidLeft
	waitmovement 0
	return

ThreeIsland_EventScript_PaxtonApproachMid::
	applymovement LOCALID_THREE_ISLAND_BIKER3, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PAXTON, ThreeIsland_Movement_PaxtonApproachMid
	applymovement LOCALID_PLAYER, ThreeIsland_Movement_PlayerFacePaxton
	waitmovement 0
	return

ThreeIsland_EventScript_PaxtonApproachMidRight::
	applymovement LOCALID_THREE_ISLAND_BIKER3, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, ThreeIsland_Movement_PlayerFacePaxton
	applymovement LOCALID_PAXTON, ThreeIsland_Movement_PaxtonApproachMidRight
	waitmovement 0
	return

ThreeIsland_EventScript_PaxtonApproachRight::
	applymovement LOCALID_THREE_ISLAND_BIKER3, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, ThreeIsland_Movement_PlayerFacePaxton
	applymovement LOCALID_PAXTON, ThreeIsland_Movement_PaxtonApproachRight
	waitmovement 0
	return

ThreeIsland_EventScript_LeaveBikersAlone::
	msgbox ThreeIsland_Text_ThatsSmart
	closemessage
	applymovement LOCALID_PLAYER, ThreeIsland_Movement_PlayerLeaveBikers
	waitmovement 0
	applymovement LOCALID_THREE_ISLAND_BIKER1, ThreeIsland_Movement_Biker1ReturnToPack
	waitmovement 0
	fadedefaultbgm
	releaseall
	end

ThreeIsland_Movement_PlayerLeaveBikers::
	walk_down
	step_end

ThreeIsland_Movement_BikerApproach::
	walk_down
	step_end

ThreeIsland_Movement_Biker1ReturnToPack::
	walk_up
	walk_in_place_faster_left
	step_end

ThreeIsland_Movement_BikerSpeak::
	walk_in_place_down
	step_end

ThreeIsland_Movement_PaxtonApproachLeft::
	walk_down
	walk_left
	walk_in_place_faster_down
	step_end

ThreeIsland_Movement_PaxtonApproachMidLeft::
	walk_down
	step_end

ThreeIsland_Movement_PaxtonApproachMid::
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

ThreeIsland_Movement_PlayerFacePaxton::
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

ThreeIsland_Movement_PaxtonApproachMidRight::
	walk_down
	walk_down
	walk_right
	walk_in_place_faster_right
	step_end

ThreeIsland_Movement_PaxtonApproachRight::
	walk_down
	walk_down
	walk_right
	walk_right
	walk_in_place_faster_right
	step_end

ThreeIsland_EventScript_Woman::
	msgbox ThreeIsland_Text_WouldntWantToSeeBikersHereAgain, MSGBOX_NPC
	end

ThreeIsland_EventScript_LittleBoy::
	msgbox ThreeIsland_Text_WhenDodouEvolvesGoingToPlayGame, MSGBOX_NPC
	end

ThreeIsland_EventScript_Doduo::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_DODUO, CRY_MODE_NORMAL
	msgbox ThreeIsland_Text_Doduo
	waitmoncry
	release
	end

ThreeIsland_EventScript_IslandSign::
	msgbox ThreeIsland_Text_IslandSign, MSGBOX_SIGN
	end

ThreeIsland_EventScript_Biker6::
	msgbox ThreeIsland_Text_IslandBelongsToUs, MSGBOX_NPC
	end

ThreeIsland_Text_IslandSign::
	.string "THREE ISLAND\n"
	.string "Kin Island of Family Bonding$"
	.string "{JPN}{JPN}3のしま\n"
	.string "かぞくの　きずなの　しま$"

ThreeIsland_Text_IslandBelongsToUs::
	.string "Hyahoo!\p"
	.string "From this day on, this island\n"
	.string "belongs to us!$"
	.string "{JPN}{JPN}ヒャッホー！\p"
	.string "きょうから　この　しまは　おれたちの　ものだ！$"

ThreeIsland_Text_GoBackToKanto::
	.string "We don't need you people bringing\n"
	.string "your noise and trouble here!\p"
	.string "We're asking you to go back to\n"
	.string "KANTO!$"
	.string "{JPN}{JPN}あんたたちが　さわぎや　もめごとを　もちこむのは\n"
	.string "めいわく　なんだ！\p"
	.string "カントーに　かえって　くれ！$"

ThreeIsland_Text_BossIsOnHisWay::
	.string "Hey, go cry somewhere else.\n"
	.string "Our boss is on his way.\p"
	.string "When he gets here, we'll give you\n"
	.string "a k-rad motorbike show you won't\l"
	.string "soon forget!$"
	.string "{JPN}{JPN}おい　なくなら　よそで　なきな。　もうすぐ\n"
	.string "ボスが　くるんだ。\p"
	.string "ボスが　きたら　わすれられない　ような　イカした\n"
	.string "バイクの　ショーを　みせて　やるぜ！$"

ThreeIsland_Text_GetOffIslandNow::
	.string "W-what!? Not on your life!\n"
	.string "Get off the island now!$"
	.string "{JPN}{JPN}な　なんだって！？　とんでもない！　いますぐ\n"
	.string "しまから　でていけ！$"

ThreeIsland_Text_WhosGonnaMakeMe::
	.string "Who's gonna make me?$"
	.string "{JPN}{JPN}だれが　おいだすって？$"

ThreeIsland_Text_AreYouBossGoBackToKanto::
	.string "Are you the boss?\n"
	.string "Go back to KANTO right now!$"
	.string "{JPN}{JPN}あんたが　ボスか？　いますぐ　カントーに\n"
	.string "かえれ！$"

ThreeIsland_Text_JustGotHerePal::
	.string "Hah?\p"
	.string "I just got here, pal.\p"
	.string "What's with the hostile attitude?\n"
	.string "It's mighty cold of you!$"
	.string "{JPN}{JPN}はあ？\p"
	.string "おれは　いま　きた　ところだぜ。\p"
	.string "なんだ　その　けんかごしの　たいどは？　ずいぶん\n"
	.string "つめたいじゃ　ねえか！$"

ThreeIsland_Text_FollowersRaisingHavoc::
	.string "Your gang of followers have been\n"
	.string "raising havoc on their bikes.\p"
	.string "Do you have any idea how much\n"
	.string "trouble they've caused us on the\l"
	.string "island?$"
	.string "{JPN}{JPN}あんたの　てしたどもが　バイクで　あばれまわって\n"
	.string "いるんだ。\p"
	.string "この　しまが　どれだけ　めいわく　してるか\n"
	.string "わかってるのか？$"

ThreeIsland_Text_OughtToBeThankingUs::
	.string "No, man, I don't get it at all.\p"
	.string "Look at this place.\n"
	.string "What do you do for entertainment?\p"
	.string "You ought to be thanking us for\n"
	.string "livening up this sleepy village.\p"
	.string "But hey, if you insist, you can try\n"
	.string "making us leave.$"
	.string "{JPN}{JPN}いや　さっぱり　わからねえな。\p"
	.string "この　ばしょを　みて　みろよ。　ここで　なにを\n"
	.string "して　たのしむんだ？\p"
	.string "こんな　いなかまちを　もりあげて　やってるんだ。\n"
	.string "かんしゃ　して　ほしい　くらいだぜ。\p"
	.string "でも　まあ　どうしてもって　いうなら　おれたちを\n"
	.string "おいだして　みな。$"

ThreeIsland_Text_YouCowardsToughInPack::
	.string "Grr… You cowards…\n"
	.string "So tough in a pack…$"
	.string "{JPN}{JPN}ぐぬぬ‥　ひきょうものめ‥　あつまると　つよきに\n"
	.string "なりやがって‥$"

ThreeIsland_Text_WannaMakeSomethingOfYourStaring::
	.string "You, what are you staring at?\n"
	.string "Don't you know it's not polite?\p"
	.string "You wanna make something of it\n"
	.string "or what?$"
	.string "{JPN}{JPN}おい　なに　じろじろ　みてんだ？　しつれいだと\n"
	.string "おもわねえのか？\p"
	.string "なんか　もんく　あんのか？$"

ThreeIsland_Text_ThatsSmart::
	.string "That's smart.\n"
	.string "Keep your nose out of this.$"
	.string "{JPN}{JPN}かしこいな。　くびを　つっこむんじゃ　ねえぞ。$"

ThreeIsland_Text_Biker1Intro::
	.string "Heh, I like your guts.\n"
	.string "You'll be losing money to me, but…$"
	.string "{JPN}{JPN}へっ　いい　どきょう　してるじゃ　ねえか。\n"
	.string "かねは　いただくけどな‥$"

ThreeIsland_Text_Biker1Defeat::
	.string "Wha…\n"
	.string "What is this kid?!$"
	.string "{JPN}{JPN}な‥　なんなんだ　この　ガキは！？$"

ThreeIsland_Text_Biker1PostBattle::
	.string "Aww, man…\n"
	.string "Don't you dare laugh!$"
	.string "{JPN}{JPN}ちくしょう‥　わらうんじゃ　ねえぞ！$"

ThreeIsland_Text_Biker2Intro::
	.string "Aren't you from KANTO?\n"
	.string "You should be on our side!$"
	.string "{JPN}{JPN}おまえ　カントーから　きたんだろ？　おれたちの\n"
	.string "みかたを　しろよ！$"

ThreeIsland_Text_Biker2Defeat::
	.string "Stop fooling around!$"
	.string "{JPN}{JPN}ふざけんなよ！$"

ThreeIsland_Text_Biker2PostBattle::
	.string "What's the matter with you,\n"
	.string "getting all hot like that?\p"
	.string "Totally uncool, man!$"
	.string "{JPN}{JPN}なに　そんなに　あつく　なってんだよ？\p"
	.string "まったく　ダサいぜ！$"

ThreeIsland_Text_Biker3Intro::
	.string "We invited the boss out here,\n"
	.string "but you had to mess it up!\p"
	.string "You embarrassed us, man!$"
	.string "{JPN}{JPN}せっかく　ボスを　よんだのに　おまえが　じゃまを\n"
	.string "しやがって！\p"
	.string "おれたちの　かおに　どろを　ぬりやがって！$"

ThreeIsland_Text_Biker3Defeat::
	.string "… … …   … … …$"
	.string "{JPN}{JPN}‥　‥　‥　　‥　‥　‥$"

ThreeIsland_Text_Biker3PostBattle::
	.string "Boss, I'm telling you, you've gotta\n"
	.string "do something about this kid!$"
	.string "{JPN}{JPN}ボス　この　ガキ　なんとか　して　くださいよ！$"

ThreeIsland_Text_PaxtonIntro::
	.string "I've been watching you, and I'd say\n"
	.string "you've done enough.\p"
	.string "What are you, their friend or\n"
	.string "something?\p"
	.string "Then I guess you'll be battling me\n"
	.string "in their place.$"
	.string "{JPN}{JPN}みてたぜ。　もう　じゅうぶん　だろう。\p"
	.string "おまえ　こいつらの　ともだちか　なにかか？\p"
	.string "じゃあ　こいつらの　かわりに　おれと　しょうぶ\n"
	.string "して　もらおうか。$"

ThreeIsland_Text_PaxtonDefeat::
	.string "All right, enough!\n"
	.string "We'll leave like you wanted!\p"
	.string "We'll be happy to see the last of\n"
	.string "this boring island!$"
	.string "{JPN}{JPN}わかった　もう　いい！　おのぞみ　どおり\n"
	.string "でていって　やるよ！\p"
	.string "こんな　たいくつな　しま　せいせい　するぜ！$"

ThreeIsland_Text_PaxtonPostBattle::
	.string "Humph! Yeah, go right on hanging\n"
	.string "around with these hayseeds!$"
	.string "{JPN}{JPN}ふん！　せいぜい　この　いなかものたちと\n"
	.string "つるんでな！$"

ThreeIsland_Text_ThankYouOhYourMonGotHurt::
	.string "Thank you! Those goons were\n"
	.string "nothing but bad trouble.\p"
	.string "Oh, look, your POKéMON got hurt\n"
	.string "on my account.$"
	.string "{JPN}{JPN}ありがとう！　あの　ならずものたちには\n"
	.string "ほんとうに　こまって　いたんだ。\p"
	.string "おや　わたしの　せいで　きみの　ポケモンが\n"
	.string "けがを　して　しまったね。$"

ThreeIsland_Text_GivenFullRestore::
	.string "{PLAYER} was given\n"
	.string "a FULL RESTORE.$"
	.string "{JPN}{JPN}{PLAYER}は　かいふくのくすりを　もらった。$"

ThreeIsland_Text_OhYourBagIsFull::
	.string "Oh?\n"
	.string "Your BAG is full.$"
	.string "{JPN}{JPN}おや？　バッグが　いっぱい　だね。$"

ThreeIsland_Text_YouveGotImpressiveMons::
	.string "You've got seriously impressive\n"
	.string "POKéMON with you.$"
	.string "{JPN}{JPN}きみは　ほんとうに　すごい　ポケモンを　つれて\n"
	.string "いるね。$"

ThreeIsland_Text_LostelleWentOffTowardsBondBridge::
	.string "It'd be fantastic if someone as\n"
	.string "strong as you lived here.\p"
	.string "I hope you'll at least stay here\n"
	.string "a while.\p"
	.string "…I beg your pardon?\n"
	.string "You're looking for LOSTELLE?\p"
	.string "LOSTELLE went off towards BOND\n"
	.string "BRIDGE a while ago.$"
	.string "{JPN}{JPN}きみ　みたいな　つよい　ひとが　ここに　すんで\n"
	.string "くれたら　すばらしいのに。\p"
	.string "せめて　しばらくは　ここに　いて　ほしいな。\p"
	.string "‥え？　ユウコを　さがしてる？\p"
	.string "ユウコなら　さっき　きずなばしの　ほうへ\n"
	.string "いったよ。$"

ThreeIsland_Text_WouldntWantToSeeBikersHereAgain::
	.string "THREE ISLAND is actually the most\n"
	.string "populous of the islands here.\p"
	.string "Still, it could be less sleepy.\p"
	.string "But I wouldn't want to see goons\n"
	.string "like those BIKERS here again.$"
	.string "{JPN}{JPN}3のしまは　じつは　この　あたりの　しまで\n"
	.string "いちばん　ひとが　おおいんだ。\p"
	.string "でも　もうすこし　にぎやかでも　いいのに。\p"
	.string "だからって　あんな　ぼうそうぞくの\n"
	.string "れんちゅうには　もう　きて　ほしく　ないけどね。$"

ThreeIsland_Text_WhenDodouEvolvesGoingToPlayGame::
	.string "I'm going to train my DODUO in the\n"
	.string "BERRY FOREST.\p"
	.string "When it evolves, I'm going to play\n"
	.string "a game on TWO ISLAND.$"
	.string "{JPN}{JPN}きのみのもりで　ドードーを　きたえるんだ。\p"
	.string "しんか　したら　2のしまで　ゲームを　するんだ。$"

ThreeIsland_Text_Doduo::
	.string "DODUO: Gigiih!$"
	.string "{JPN}{JPN}ドードー『ギギィー！』$"

