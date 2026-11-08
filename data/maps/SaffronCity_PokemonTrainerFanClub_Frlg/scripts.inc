SaffronCity_PokemonTrainerFanClub_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SaffronCity_PokemonTrainerFanClub_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, SaffronCity_PokemonTrainerFanClub_OnTransition
	.byte 0

SaffronCity_PokemonTrainerFanClub_OnFrame::
	map_script_2 VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 1, SaffronCity_PokemonTrainerFanClub_EventScript_MeetFirstFans
	.2byte 0

SaffronCity_PokemonTrainerFanClub_EventScript_MeetFirstFans::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	applymovement LOCALID_TRAINER_FAN_CLUB_BATTLE_GIRL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_TRAINER_FAN_CLUB_BATTLE_GIRL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_TRAINER_FAN_CLUB_BATTLE_GIRL, Common_Movement_Delay48
	waitmovement 0
	msgbox SaffronCity_PokemonTrainerFanClub_Text_HuhYou
	closemessage
	applymovement LOCALID_TRAINER_FAN_CLUB_BATTLE_GIRL, SaffronCity_PokemonTrainerFanClub_Movement_FanApproachPlayer
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SaffronCity_PokemonTrainerFanClub_Text_YourePlayerWereYourFansNow
	setvar VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 2
	releaseall
	end

SaffronCity_PokemonTrainerFanClub_Movement_FanApproachPlayer::
	delay_16
	delay_8
	walk_down
	delay_16
	walk_down
	delay_8
	walk_left
	step_end

SaffronCity_PokemonTrainerFanClub_OnTransition::
	goto_if_eq VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 1, SaffronCity_PokemonTrainerFanClub_EventScript_SetMemberPosForFirstMeeting
	goto_if_eq VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 2, SaffronCity_PokemonTrainerFanClub_EventScript_UpdateFanMemberPositions
	end

SaffronCity_PokemonTrainerFanClub_EventScript_UpdateFanMemberPositions::
	special Script_TryLoseFansFromPlayTime
	setvar VAR_0x8004, FANCLUB_MEMBER1
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember1ToFarTable
	setvar VAR_0x8004, FANCLUB_MEMBER2
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember2ToFarTable
	setvar VAR_0x8004, FANCLUB_MEMBER3
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember3ToFarTable
	setvar VAR_0x8004, FANCLUB_MEMBER4
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember4ToFarTable
	setvar VAR_0x8004, FANCLUB_MEMBER5
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember5ToFarTable
	setvar VAR_0x8004, FANCLUB_MEMBER6
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember6ToFarTable
	setvar VAR_0x8004, FANCLUB_MEMBER7
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember7ToFarTable
	setvar VAR_0x8004, FANCLUB_MEMBER8
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	call_if_eq VAR_RESULT, FALSE, SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember8ToFarTable
	end

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember1ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_BATTLE_GIRL, 5, 2
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_BATTLE_GIRL, MOVEMENT_TYPE_FACE_UP
	return

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember2ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_YOUNGSTER, 7, 4
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_YOUNGSTER, MOVEMENT_TYPE_FACE_LEFT
	return

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember3ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_GENTLEMAN, 2, 2
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_GENTLEMAN, MOVEMENT_TYPE_FACE_UP
	return

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember4ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_LITTLE_GIRL, 3, 4
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_LITTLE_GIRL, MOVEMENT_TYPE_FACE_RIGHT
	return

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember5ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_ROCKER, 4, 6
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_ROCKER, MOVEMENT_TYPE_FACE_UP
	return

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember6ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_WOMAN, 10, 3
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_WOMAN, MOVEMENT_TYPE_FACE_UP
	return

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember7ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_BEAUTY, 7, 5
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_BEAUTY, MOVEMENT_TYPE_FACE_LEFT
	return

SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember8ToFarTable::
	setobjectxyperm LOCALID_TRAINER_FAN_CLUB_BLACK_BELT, 9, 6
	setobjectmovementtype LOCALID_TRAINER_FAN_CLUB_BLACK_BELT, MOVEMENT_TYPE_FACE_LEFT
	return

SaffronCity_PokemonTrainerFanClub_EventScript_SetMemberPosForFirstMeeting::
	call SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember4ToFarTable
	call SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember5ToFarTable
	call SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember6ToFarTable
	call SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember7ToFarTable
	call SaffronCity_PokemonTrainerFanClub_EventScript_MoveMember8ToFarTable
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirl::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER1
	special Script_BufferFanClubTrainerName
	goto_if_eq VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 0, SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlPlayerNotChampion
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_WasYourFanNotAnymore
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_AlwaysCheerForYou
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_IllAlwaysBelieveInYou
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_EveryonesYourFanButICantBe
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BattleGirlPlayerNotChampion::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_TrainersCoolWhenBattling
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirl::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER4
	special Script_BufferFanClubTrainerName
	goto_if_eq VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 0, SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlPlayerNotChampion
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_WantToBeLikeSabrina
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_WantToBeLikeYouOneDay
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_EveryoneButMeStoppedBeingYourFan
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_WontStopBeingSabrinasFan
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_LittleGirlPlayerNotChampion::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_CanIBecomeCoolTrainerOneDay
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_Youngster::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER2
	special Script_BufferFanClubTrainerName
	goto_if_eq VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 0, SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterPlayerNotChampion
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_BrocksMyHero
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_CanYouAutographShorts
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_CountingOnYou
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_BrocksFanToBitterEnd
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_YoungsterPlayerNotChampion::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_BrocksLastWordOnCool
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_Gentleman::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER3
	special Script_BufferFanClubTrainerName
	goto_if_eq VAR_MAP_SCENE_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB, 0, SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanPlayerNotChampion
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_HmmAndYouAre
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_HadPleasureOfWatchingYouBattle
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_NeverSeenTrainerOfYourMagnificence
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_YouveStillAWaysToGo
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_GentlemanPlayerNotChampion::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_YoullBeTalkedAboutIfYouKeepWinning
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_Woman::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER6
	special Script_BufferFanClubTrainerName
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_WomanPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_WomanOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_TrainerHasBeenOnFire
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_WomanPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_WomanOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_AdoreWayYouBattle
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_WomanOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_ImYourNumberOneFan
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_WomanOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_EveryoneTalksAboutYou
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_Rocker::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER5
	special Script_BufferFanClubTrainerName
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_RockerPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_RockerOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_LoveWayTrainerTalks
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_RockerPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_RockerOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_TheWayYouBattleIsCool
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_RockerOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_ImOnlyOneLeftOfYourFans
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_RockerOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_ImLoneTrainerFan
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_Beauty::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER7
	special Script_BufferFanClubTrainerName
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_BeautyPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_BeautyOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_WhyCantOthersSeeMastersDignity
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BeautyPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_BeautyOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_YouReallyAreAmazing
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BeautyOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_ImYourOnlyFan
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BeautyOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_BelieveInMasterWithAllMyHeart
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BlackBelt::
	lock
	faceplayer
	setvar VAR_0x8004, FANCLUB_MEMBER8
	special Script_BufferFanClubTrainerName
	specialvar VAR_RESULT, Script_IsFanClubMemberFanOfPlayer
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_PokemonTrainerFanClub_EventScript_BlackBeltPlayersFan
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, (NUM_TRAINER_FAN_CLUB_MEMBERS - 1), SaffronCity_PokemonTrainerFanClub_EventScript_BlackBeltOnlyNonFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_OnlyMasterHasMyRespect
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BlackBeltPlayersFan::
	specialvar VAR_RESULT, Script_GetNumFansOfPlayerInTrainerFanClub
	goto_if_eq VAR_RESULT, 1, SaffronCity_PokemonTrainerFanClub_EventScript_BlackBeltOnlyFan
	msgbox SaffronCity_PokemonTrainerFanClub_Text_YourBattleStyleIsEducational
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BlackBeltOnlyFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_WontStopBeingYourFan
	release
	end

SaffronCity_PokemonTrainerFanClub_EventScript_BlackBeltOnlyNonFan::
	msgbox SaffronCity_PokemonTrainerFanClub_Text_NeverBeFanOfAnyoneButMaster
	release
	end

SaffronCity_PokemonTrainerFanClub_Text_HuhYou::
	.string "Huh?\n"
	.string "You…$"
	.string "{JPN}{JPN}あれ？　きみは‥$"

SaffronCity_PokemonTrainerFanClub_Text_YourePlayerWereYourFansNow::
	.string "Are you maybe…\n"
	.string "… …\p"
	.string "You are!\n"
	.string "You are {PLAYER}!\p"
	.string "We were just talking about you!\p"
	.string "We were talking about how an\n"
	.string "incredibly good TRAINER appeared.\p"
	.string "And that someone is you!\n"
	.string "You're so cool!\p"
	.string "So, we decided that we'll become\n"
	.string "your FAN CLUB!\p"
	.string "We all hope you'll keep battling\n"
	.string "and show us how cool you are.\p"
	.string "That way, maybe other people will\n"
	.string "become fans just like me.$"
	.string "{JPN}{JPN}もしかして‥　‥　‥\p"
	.string "やっぱり！　{PLAYER}だ！\p"
	.string "ちょうど　きみの　はなしを　してたんだ！\p"
	.string "すごく　つよい　トレーナーが　あらわれたって\n"
	.string "はなしを　してたんだよ。\p"
	.string "それが　きみ　なんだ！　かっこいい！\p"
	.string "だから　みんなで　きみの　ファンクラブに　なる\n"
	.string "ことに　したんだ！\p"
	.string "これからも　バトルを　つづけて　かっこいい\n"
	.string "ところを　みせてね。\p"
	.string "そしたら　ぼく　みたいな　ファンが　もっと\n"
	.string "ふえる　かも　しれない。$"

SaffronCity_PokemonTrainerFanClub_Text_AlwaysCheerForYou::
	.string "I'll always be cheering for you!\p"
	.string "Go, {PLAYER}, go!$"
	.string "{JPN}{JPN}いつでも　おうえん　してるよ！\p"
	.string "がんばれ　{PLAYER}！$"

SaffronCity_PokemonTrainerFanClub_Text_IllAlwaysBelieveInYou::
	.string "I'll always believe in you,\n"
	.string "{PLAYER}.\p"
	.string "Please show the others what you're\n"
	.string "like when you battle seriously.$"
	.string "{JPN}{JPN}{PLAYER}　ずっと　しんじてるよ。\p"
	.string "ほんきで　たたかう　すがたを　みんなに　みせて\n"
	.string "あげてね。$"

SaffronCity_PokemonTrainerFanClub_Text_WasYourFanNotAnymore::
	.string "I'm sorry.\n"
	.string "I was your fan before.\p"
	.string "But now, you seem to have lost\n"
	.string "the drive you had before.\p"
	.string "Lately, I think {STR_VAR_1} has been\n"
	.string "a lot cooler.$"
	.string "{JPN}{JPN}ごめんね。　まえは　きみの　ファン　だったんだ。\p"
	.string "でも　いまの　きみは　まえの　ような　いきおいが\n"
	.string "ない　みたい。\p"
	.string "さいきんは　{STR_VAR_1}の　ほうが\n"
	.string "ずっと　かっこいいと　おもうんだ。$"

SaffronCity_PokemonTrainerFanClub_Text_EveryonesYourFanButICantBe::
	.string "Oh!\n"
	.string "{PLAYER}!\p"
	.string "Everyone's been all excited about\n"
	.string "you recently.\p"
	.string "I can understand why.\n"
	.string "I know that you're strong.\p"
	.string "But, do you know what?\p"
	.string "Once I became {STR_VAR_1}'s fan,\n"
	.string "I can't just stop being a fan.$"
	.string "{JPN}{JPN}あっ！　{PLAYER}！\p"
	.string "さいきん　みんな　きみに　むちゅう　なんだ。\p"
	.string "きもちは　わかるよ。　きみが　つよいのは\n"
	.string "しってる。\p"
	.string "でもね‥\p"
	.string "いちど　{STR_VAR_1}の　ファンに\n"
	.string "なったら　かんたんには　やめられないんだ。$"

SaffronCity_PokemonTrainerFanClub_Text_TrainersCoolWhenBattling::
	.string "I think TRAINERS are so cool when\n"
	.string "they're battling.\p"
	.string "Don't you think so, too?$"
	.string "{JPN}{JPN}バトルを　してる　ときの　トレーナーって　すごく\n"
	.string "かっこいいと　おもうの。\p"
	.string "あなたも　そう　おもわない？$"

SaffronCity_PokemonTrainerFanClub_Text_CanYouAutographShorts::
	.string "Oh! It's {PLAYER}!\n"
	.string "Too cool!\p"
	.string "Oh!\n"
	.string "Oh, yeah!\p"
	.string "Can you autograph something?\n"
	.string "How about my shorts?$"
	.string "{JPN}{JPN}あっ！　{PLAYER}だ！　かっこいい！\p"
	.string "あっ！　そうだ！\p"
	.string "サインして　くれない？　ぼくの　たんパンに\n"
	.string "なんて　どう？$"

SaffronCity_PokemonTrainerFanClub_Text_CountingOnYou::
	.string "However hot it's been…\p"
	.string "And however cold it may get…\p"
	.string "I've always believed in, and worn,\n"
	.string "shorts!\p"
	.string "I'm counting on you to keep\n"
	.string "winning!\p"
	.string "I believe in you just as much as\n"
	.string "I believe in shorts!$"
	.string "{JPN}{JPN}どんなに　あつい　ひも‥\p"
	.string "どんなに　さむい　ひも‥\p"
	.string "ぼくは　たんパンを　しんじて　はきつづけて\n"
	.string "きた！\p"
	.string "これからも　かちつづけて　くれよ！\p"
	.string "たんパンと　おなじ　くらい　きみを　しんじてる\n"
	.string "からね！$"

SaffronCity_PokemonTrainerFanClub_Text_BrocksMyHero::
	.string "BROCK's my hero!\n"
	.string "He's a man among men!\p"
	.string "Heheh, I got my mom to do\n"
	.string "something for me.\p"
	.string "I got her to embroider “BROCK”\n"
	.string "on my shorts!$"
	.string "{JPN}{JPN}タケシは　ぼくの　ヒーロー！　おとこの　なかの\n"
	.string "おとこ　だ！\p"
	.string "へへっ　ママに　おねがい　したんだ。\p"
	.string "ぼくの　たんパンに　『タケシ』って　ししゅう\n"
	.string "してもらったんだ！$"

SaffronCity_PokemonTrainerFanClub_Text_BrocksFanToBitterEnd::
	.string "…  …\p"
	.string "…  …\n"
	.string "I'm BROCK's fan!\p"
	.string "Even if I were his only fan,\n"
	.string "I swear it on my rock-hard will,\l"
	.string "and on my shorts!\p"
	.string "I'll be his fan to the bitter end!$"
	.string "{JPN}{JPN}‥　‥\p"
	.string "‥　‥　ぼくは　タケシの　ファンだ！\p"
	.string "たとえ　ファンが　ぼく　ひとり　でも　いしの\n"
	.string "ように　かたい　いしと　たんパンに　ちかって‥\p"
	.string "さいごの　さいごまで　タケシの　ファンで\n"
	.string "いるぞ！$"

SaffronCity_PokemonTrainerFanClub_Text_BrocksLastWordOnCool::
	.string "BROCK's the last word on cool,\n"
	.string "end of story!\p"
	.string "Now there's a guy who I want to\n"
	.string "be like!$"
	.string "{JPN}{JPN}かっこいいと　いえば　タケシ！　いじょう！\p"
	.string "ああいう　おとこに　ぼくは　なりたいんだ！$"

SaffronCity_PokemonTrainerFanClub_Text_HadPleasureOfWatchingYouBattle::
	.string "{PLAYER}, I've had the pleasure of\n"
	.string "watching you battle.\p"
	.string "It brings back sweet memories of\n"
	.string "my own long-lost youth.\p"
	.string "May you continue to find success.\n"
	.string "You have a fan in me!$"
	.string "{JPN}{JPN}{PLAYER}　きみの　バトルを　たのしく\n"
	.string "みせて　もらったよ。\p"
	.string "わしの　とおい　わかき　ひの　おもいでが\n"
	.string "よみがえる　ようじゃ。\p"
	.string "これからも　かつやく　しておくれ。　わしは\n"
	.string "きみの　ファン　じゃよ！$"

SaffronCity_PokemonTrainerFanClub_Text_NeverSeenTrainerOfYourMagnificence::
	.string "I have journeyed far and wide\n"
	.string "across the lands.\p"
	.string "But never have I seen a TRAINER\n"
	.string "of your magnificence.\p"
	.string "Take courage in your abilities\n"
	.string "and keep battling on!\p"
	.string "Make that a promise to me,\n"
	.string "my young idol!$"
	.string "{JPN}{JPN}わしは　せかいじゅうを　たびして　きた。\p"
	.string "だが　きみほど　すばらしい　トレーナーは　みた\n"
	.string "ことが　ない。\p"
	.string "じしんを　もって　これからも　たたかい\n"
	.string "つづけなさい！\p"
	.string "わしの　わかき　アイドルよ　やくそく　じゃぞ！$"

SaffronCity_PokemonTrainerFanClub_Text_HmmAndYouAre::
	.string "Hmm…\n"
	.string "And you are?\p"
	.string "Were you hoping to sign up for\n"
	.string "a membership in our FAN CLUB?$"
	.string "{JPN}{JPN}ふむ‥　きみは？\p"
	.string "わたしたちの　ファンクラブに　にゅうかい\n"
	.string "したいのかね？$"

SaffronCity_PokemonTrainerFanClub_Text_YouveStillAWaysToGo::
	.string "Ah, so you are the much-\n"
	.string "talked-about {PLAYER}.\p"
	.string "You certainly do have the look of\n"
	.string "a winner.\p"
	.string "But for me to accept you as\n"
	.string "a true POKéMON great…\p"
	.string "… …\p"
	.string "You've still a ways to go!$"
	.string "{JPN}{JPN}ほう　きみが　うわさの　{PLAYER}か。\p"
	.string "たしかに　しょうしゃの　かおつきを　しているな。\p"
	.string "だが　わたしが　ほんものの　ポケモン　たつじんと\n"
	.string "みとめるには‥\p"
	.string "‥　‥\p"
	.string "まだまだ　だな！$"

SaffronCity_PokemonTrainerFanClub_Text_YoullBeTalkedAboutIfYouKeepWinning::
	.string "All of us gathered here are fans\n"
	.string "of POKéMON TRAINERS.\p"
	.string "The way they look in battle…\n"
	.string "They are the picture of coolness.\p"
	.string "Perhaps you will be talked about\n"
	.string "here, too, if you keep winning.$"
	.string "{JPN}{JPN}ここに　あつまっている　のは　みんな\n"
	.string "ポケモントレーナーの　ファンだ。\p"
	.string "バトルでの　すがた‥　かっこよさ　そのもの\n"
	.string "だよ。\p"
	.string "きみも　かちつづければ　ここで　うわさに　なる\n"
	.string "かも　しれないね。$"

SaffronCity_PokemonTrainerFanClub_Text_WantToBeLikeYouOneDay::
	.string "It's {PLAYER}!\n"
	.string "It's {PLAYER}!\p"
	.string "I want to be like {PLAYER} one day!$"
	.string "{JPN}{JPN}{PLAYER}だ！　{PLAYER}だ！\p"
	.string "いつか　{PLAYER}みたいに　なりたいな！$"

SaffronCity_PokemonTrainerFanClub_Text_EveryoneButMeStoppedBeingYourFan::
	.string "Everyone stopped being {PLAYER}'s\n"
	.string "fan…\p"
	.string "But that means I can have {PLAYER}\n"
	.string "all to myself!\p"
	.string "Yay! Go for it!\n"
	.string "My one and only {PLAYER}!$"
	.string "{JPN}{JPN}みんな　{PLAYER}の　ファンを\n"
	.string "やめちゃった‥\p"
	.string "でも　それって　{PLAYER}を　ひとりじめ\n"
	.string "できるって　こと！\p"
	.string "わーい！　がんばって！　わたしだけの\n"
	.string "{PLAYER}！$"

SaffronCity_PokemonTrainerFanClub_Text_WantToBeLikeSabrina::
	.string "Do you know what?\n"
	.string "I want to be like SABRINA!\p"
	.string "I want to be a psychic girl just\n"
	.string "like her!$"
	.string "{JPN}{JPN}ねえ　しってる？　わたし　ナツメさん　みたいに\n"
	.string "なりたいの！\p"
	.string "ナツメさん　みたいな　エスパーしょうじょに\n"
	.string "なりたいの！$"

SaffronCity_PokemonTrainerFanClub_Text_WontStopBeingSabrinasFan::
	.string "Even if I'm the only one, I won't\n"
	.string "stop being SABRINA's fan.\p"
	.string "My dream is to be like SABRINA,\n"
	.string "cute, but cool.\p"
	.string "I want to be a psychic girl just\n"
	.string "like her!$"
	.string "{JPN}{JPN}たとえ　わたし　ひとりに　なっても　ナツメさんの\n"
	.string "ファンは　やめないわ。\p"
	.string "わたしの　ゆめは　ナツメさん　みたいに\n"
	.string "かわいくて　かっこいい　ひとに　なること。\p"
	.string "ナツメさん　みたいな　エスパーしょうじょに\n"
	.string "なりたいの！$"

SaffronCity_PokemonTrainerFanClub_Text_CanIBecomeCoolTrainerOneDay::
	.string "I wonder if I can become a cool\n"
	.string "TRAINER one day?$"
	.string "{JPN}{JPN}わたしも　いつか　かっこいい　トレーナーに\n"
	.string "なれる　かしら？$"

SaffronCity_PokemonTrainerFanClub_Text_TheWayYouBattleIsCool::
	.string "Oh, man, oh, man!\n"
	.string "Cool! Too cool!\p"
	.string "…Huh?\n"
	.string "No, not you.\p"
	.string "Just the way you battle.\n"
	.string "That's cool.$"
	.string "{JPN}{JPN}うわあ　うわあ！　かっこいい！　かっこよすぎ！\p"
	.string "‥えっ？　ちがう　ちがう　きみの　ことじゃ\n"
	.string "ないよ。\p"
	.string "きみの　たたかいかたの　ことさ。　あれは\n"
	.string "かっこいい。$"

SaffronCity_PokemonTrainerFanClub_Text_ImOnlyOneLeftOfYourFans::
	.string "Yo, {PLAYER}!\p"
	.string "Looks like I'm the only one left\n"
	.string "out of your fans.\p"
	.string "But, you know, I like being an only\n"
	.string "fan… It's less stressful.\p"
	.string "Keep up and at it!$"
	.string "{JPN}{JPN}よう　{PLAYER}！\p"
	.string "きみの　ファンは　おれ　ひとりに　なっちまった\n"
	.string "みたいだな。\p"
	.string "でも　ひとりだけの　ファンって　わるくないぜ‥\n"
	.string "きらくで　いい。\p"
	.string "その　ちょうしで　がんばれよ！$"

SaffronCity_PokemonTrainerFanClub_Text_LoveWayTrainerTalks::
	.string "{STR_VAR_1}'s cool, huh?\n"
	.string "I love the way he talks.\p"
	.string "I'd really relish a chance to meet\n"
	.string "him in person once.$"
	.string "{JPN}{JPN}{STR_VAR_1}って　かっこいいよな？　あの\n"
	.string "しゃべりかたが　たまらないんだ。\p"
	.string "いちど　じかに　あって　みたいなあ。$"

SaffronCity_PokemonTrainerFanClub_Text_ImLoneTrainerFan::
	.string "Huh, can you believe it?\n"
	.string "Everyone turned into your fans.\p"
	.string "So after all we've been through,\n"
	.string "I'm the lone {STR_VAR_1} fan?\p"
	.string "Eh, being the one and only\n"
	.string "{STR_VAR_1} fan's my style, anyway.$"
	.string "{JPN}{JPN}まいったな　みんな　きみの　ファンに\n"
	.string "なっちまった。\p"
	.string "いろいろ　あったのに　{STR_VAR_1}の\n"
	.string "ファンは　おれ　ひとり　だけか？\p"
	.string "ま　{STR_VAR_1}の　ただ　ひとりの\n"
	.string "ファンって　いうのも　おれらしくて　いいけどな。$"

SaffronCity_PokemonTrainerFanClub_Text_AdoreWayYouBattle::
	.string "Oh, my goodness!\n"
	.string "Is it really you, {PLAYER}?\p"
	.string "I have to tell you, I adore the\n"
	.string "way you battle.\p"
	.string "I hope you'll keep at it.\n"
	.string "I'll be your number one fan!$"
	.string "{JPN}{JPN}まあ！　ほんとうに　{PLAYER}さん　なの？\p"
	.string "あなたの　たたかいかたが　だいすき　なのよ。\p"
	.string "これからも　がんばってね。　わたしが　ファン\n"
	.string "だいいちごう　よ！$"

SaffronCity_PokemonTrainerFanClub_Text_ImYourNumberOneFan::
	.string "Oh, my goodness!\n"
	.string "If it isn't {PLAYER}!\p"
	.string "It's so disappointing… I realized\n"
	.string "I'm your number-one-and-only fan!\p"
	.string "Well, it can't be helped!\n"
	.string "Let me cheer you on, at least!\p"
	.string "Hurray, hurray, {PLAYER}!$"
	.string "{JPN}{JPN}まあ！　{PLAYER}さん　じゃないの！\p"
	.string "ざんねんだわ‥　あなたの　ファンは　わたし\n"
	.string "ひとり　だけに　なって　しまったの！\p"
	.string "でも　しかたないわね！　せめて　おうえん　させて\n"
	.string "ちょうだい！\p"
	.string "フレー　フレー　{PLAYER}！$"

SaffronCity_PokemonTrainerFanClub_Text_TrainerHasBeenOnFire::
	.string "Recently, {STR_VAR_1} has been on\n"
	.string "fire.\p"
	.string "The way he battles… It's just \n"
	.string "the way that I like it!$"
	.string "{JPN}{JPN}さいきん　{STR_VAR_1}が　のりに\n"
	.string "のってるのよ。\p"
	.string "あの　たたかいかた‥　まさに　わたし　ごのみ\n"
	.string "なの！$"

SaffronCity_PokemonTrainerFanClub_Text_EveryoneTalksAboutYou::
	.string "Oh, hello!\n"
	.string "If it isn't {PLAYER}!\p"
	.string "Lately, all everyone talks about\n"
	.string "is you.\p"
	.string "I'm feeling lonesome because I\n"
	.string "prefer {STR_VAR_1}.$"
	.string "{JPN}{JPN}あら　こんにちは！　{PLAYER}さん\n"
	.string "じゃないの！\p"
	.string "さいきん　みんな　あなたの　はなし　ばかり。\p"
	.string "わたしは　{STR_VAR_1}の　ほうが\n"
	.string "すきだから　さみしいわ。$"

SaffronCity_PokemonTrainerFanClub_Text_YouReallyAreAmazing::
	.string "Amazing!\n"
	.string "{PLAYER}, you really are amazing!\p"
	.string "Instead of just watching, maybe\n"
	.string "I should become a TRAINER, too.$"
	.string "{JPN}{JPN}すごい！　{PLAYER}　あなた　ほんとうに\n"
	.string "すごいわ！\p"
	.string "みてる　だけじゃ　なくて　わたしも　トレーナーに\n"
	.string "なろうかしら。$"

SaffronCity_PokemonTrainerFanClub_Text_ImYourOnlyFan::
	.string "I'm your only fan…\n"
	.string "Sigh… This is lonely…\p"
	.string "Can't you try harder and make the\n"
	.string "others take notice of you?$"
	.string "{JPN}{JPN}あなたの　ファンは　わたし　だけ‥　はあ‥\n"
	.string "さみしいわ‥\p"
	.string "もっと　がんばって　みんなに　ちゅうもく　されて\n"
	.string "ちょうだい？$"

SaffronCity_PokemonTrainerFanClub_Text_WhyCantOthersSeeMastersDignity::
	.string "Master {STR_VAR_1} is so dignified…\n"
	.string "He is so my type.\p"
	.string "Why can't the others see the\n"
	.string "dignity he simply oozes?$"
	.string "{JPN}{JPN}{STR_VAR_1}さまは　とても　いげんが\n"
	.string "あって‥　わたしの　このみ　なの。\p"
	.string "あの　にじみでる　いげんが　どうして　みんなには\n"
	.string "わからないの　かしら？$"

SaffronCity_PokemonTrainerFanClub_Text_BelieveInMasterWithAllMyHeart::
	.string "I believe in Master {STR_VAR_1}\n"
	.string "with all my heart.\p"
	.string "Even if I am the only one to\n"
	.string "believe in him.\p"
	.string "I won't ever feel lonesome, though.\p"
	.string "Not if I keep thinking about Master\n"
	.string "{STR_VAR_1}.$"
	.string "{JPN}{JPN}わたしは　こころから　{STR_VAR_1}さまを\n"
	.string "しんじて　いるわ。\p"
	.string "たとえ　しんじて　いるのが　わたし　ひとり　だけ\n"
	.string "でも。\p"
	.string "でも　さみしく　なんか　ないわ。\p"
	.string "{STR_VAR_1}さまの　ことを　おもって\n"
	.string "いれば。$"

SaffronCity_PokemonTrainerFanClub_Text_YourBattleStyleIsEducational::
	.string "Hiyah!\p"
	.string "Your battling style is most\n"
	.string "educational.\p"
	.string "I hope that you will keep plugging\n"
	.string "away at success, hiyah!$"
	.string "{JPN}{JPN}ハイヤー！\p"
	.string "きみの　たたかいかたは　たいへん　べんきょうに\n"
	.string "なる。\p"
	.string "これからも　しょうりを　めざして　がんばって\n"
	.string "くれ！　ハイヤー！$"

SaffronCity_PokemonTrainerFanClub_Text_WontStopBeingYourFan::
	.string "Instead of just watching others\n"
	.string "battle, battle yourself!\p"
	.string "…My DOJO's MASTER chewed me out\n"
	.string "like that…\p"
	.string "But I won't let that stop me from\n"
	.string "being your fan!\p"
	.string "I can't stop being your fan!$"
	.string "{JPN}{JPN}ひとの　バトルを　みてないで　じぶんで\n"
	.string "たたかえ！\p"
	.string "‥と　どうじょうの　だいおうに　しかられて\n"
	.string "しまった‥\p"
	.string "だが　それでも　きみの　ファンは　やめないぞ！\p"
	.string "きみの　ファンは　やめられないんだ！$"

SaffronCity_PokemonTrainerFanClub_Text_OnlyMasterHasMyRespect::
	.string "Hiyah! There is but one TRAINER\n"
	.string "who has my respect.\p"
	.string "And that is none other than my\n"
	.string "MASTER at the FIGHTING DOJO.\p"
	.string "You should become a fan of my\n"
	.string "MASTER like me! Hiyah!$"
	.string "{JPN}{JPN}ハイヤー！　おれが　そんけい　する　トレーナーは\n"
	.string "ただ　ひとり。\p"
	.string "それは　かくとう　どうじょうの　だいおう　だ！\p"
	.string "きみも　おれと　いっしょに　だいおうの　ファンに\n"
	.string "なれ！　ハイヤー！$"

SaffronCity_PokemonTrainerFanClub_Text_NeverBeFanOfAnyoneButMaster::
	.string "Hiyah! I respect my MASTER at the\n"
	.string "FIGHTING DOJO.\p"
	.string "I will never become a fan of\n"
	.string "anyone other than my MASTER!\p"
	.string "I will even sneak out of the DOJO\n"
	.string "to spread word of his greatness.\p"
	.string "Hiyah!$"
	.string "{JPN}{JPN}ハイヤー！　おれは　かくとう　どうじょうの\n"
	.string "だいおうを　そんけい　している。\p"
	.string "だいおう　いがいの　ファンには　ぜったいに\n"
	.string "ならない！\p"
	.string "どうじょうを　ぬけだして　でも　だいおうの\n"
	.string "すばらしさを　ひろめるのだ！\p"
	.string "ハイヤー！$"

