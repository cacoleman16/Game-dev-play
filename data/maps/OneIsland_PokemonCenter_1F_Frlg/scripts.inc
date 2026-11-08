OneIsland_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OneIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	map_script MAP_SCRIPT_ON_LOAD, OneIsland_PokemonCenter_1F_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, OneIsland_PokemonCenter_1F_OnFrame
	.byte 0

OneIsland_PokemonCenter_1F_OnLoad::
	call_if_ge VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5, OneIsland_PokemonCenter_1F_EventScript_SetRubyMetatile
	call_if_set FLAG_IS_CHAMPION, OneIsland_PokemonCenter_1F_EventScript_SetNetworkMachineOn
	end

OneIsland_PokemonCenter_1F_EventScript_SetRubyMetatile::
	setmetatile 13, 4, METATILE_PokemonCenterFrlg_NetworkMachine_Ruby, 1
	return

OneIsland_PokemonCenter_1F_EventScript_SetSapphireMetatile::
	setmetatile 16, 4, METATILE_PokemonCenterFrlg_NetworkMachine_Sapphire, 1
	return

OneIsland_PokemonCenter_1F_EventScript_SetNetworkMachineOn::
	call OneIsland_PokemonCenter_1F_EventScript_SetSapphireMetatile
	setmetatile 14, 4, METATILE_PokemonCenterFrlg_NetworkMachine_ScreenLeft_On, 1
	setmetatile 15, 4, METATILE_PokemonCenterFrlg_NetworkMachine_ScreenRight_On, 1
	return

OneIsland_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_ONE_ISLAND
	call_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 6, OneIsland_PokemonCenter_1F_EventScript_SetCelioQuestDone
	call_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 0, OneIsland_PokemonCenter_1F_EventScript_SetBillCelioFirstMeetingPos
	call_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 2, OneIsland_PokemonCenter_1F_EventScript_SetBillCelioReadyToLeavePos
	end

OneIsland_PokemonCenter_1F_EventScript_SetCelioQuestDone::
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 7
	return

OneIsland_PokemonCenter_1F_EventScript_SetBillCelioFirstMeetingPos::
	setobjectxyperm LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, 9, 8
	setobjectmovementtype LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, MOVEMENT_TYPE_FACE_UP
	setobjectxyperm LOCALID_CELIO, 15, 6
	setobjectmovementtype LOCALID_CELIO, MOVEMENT_TYPE_FACE_UP
	return

OneIsland_PokemonCenter_1F_EventScript_SetBillCelioReadyToLeavePos::
	setobjectxyperm LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, 14, 7
	setobjectmovementtype LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, MOVEMENT_TYPE_FACE_LEFT
	setobjectxyperm LOCALID_CELIO, 14, 8
	setobjectmovementtype LOCALID_CELIO, MOVEMENT_TYPE_FACE_LEFT
	return

OneIsland_PokemonCenter_1F_OnFrame::
	map_script_2 VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 0, OneIsland_PokemonCenter_1F_EventScript_MeetCelioScene
	.2byte 0

OneIsland_PokemonCenter_1F_EventScript_MeetCelioScene::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToCelio
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, OneIsland_PokemonCenter_1F_Movement_BillWalkToCelio
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillHeyThereCelio
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_CELIO, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_Delay48
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioCantBelieveYouCameOut
	msgbox OneIsland_PokemonCenter_1F_Text_BillHowsYourResearchComing
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_ThisIsMyBuddyCelio
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call_if_set FLAG_SYS_GAME_CLEAR, OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerChamp
	call_if_unset FLAG_SYS_GAME_CLEAR, OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerNotChamp
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioThatsReallyImpressive
	msgbox OneIsland_PokemonCenter_1F_Text_BillBringMeUpToSpeed
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioPCsCantLinkWithYours
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillLetMeHelpYou
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CanYouDeliverThisMeteoritePlayer
	msgreceiveditem OneIsland_PokemonCenter_1F_Text_AcceptedMeteoriteFromBill, ITEM_METEORITE, 1, MUS_RG_OBTAIN_KEY_ITEM
	additem ITEM_METEORITE
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioPleaseTakeThis
	giveitem_msg OneIsland_PokemonCenter_1F_Text_ObtainedTriPass, ITEM_TRI_PASS, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox OneIsland_PokemonCenter_1F_Text_PassLetsYouTravelBetweenIslands
	call_if_ge VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMapPage
	call_if_lt VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMap
	setflag FLAG_SYS_SEVII_MAP_123
	msgbox OneIsland_PokemonCenter_1F_Text_BillCatchYouLater
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	setflag FLAG_SYS_PC_STORAGE_DISABLED
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 1
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerChamp::
	msgbox OneIsland_PokemonCenter_1F_Text_PlayerIsReigningPokemonChamp
	return

OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerNotChamp::
	msgbox OneIsland_PokemonCenter_1F_Text_PlayerIsRisingPokemonChamp
	return

OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMapPage::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message OneIsland_PokemonCenter_1F_Text_ReceivedExtraPageForTownMap
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	return

OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMap::
	setflag FLAG_HIDE_TOWN_MAP
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	giveitem_msg OneIsland_PokemonCenter_1F_Text_ReceivedTownMap, ITEM_TOWN_MAP, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToCelio::
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

OneIsland_PokemonCenter_1F_Movement_BillWalkToCelio::
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_in_place_faster_right
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_WalkUp:
	walk_up
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_StepAsideLeft::
	walk_left
	walk_in_place_faster_right
	step_end

OneIsland_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

OneIsland_PokemonCenter_1F_EventScript_Bill::
	lock
	goto_if_set FLAG_SEVII_DETOUR_FINISHED, OneIsland_PokemonCenter_1F_EventScript_BillGoTakeStroll
	msgbox OneIsland_PokemonCenter_1F_Text_HmmHowAboutLikeThis
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BillGoTakeStroll::
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_GotPCWorkingStrollAWhileMore
	release
	end

OneIsland_PokemonCenter_1F_EventScript_Celio::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 7, OneIsland_PokemonCenter_1F_EventScript_CelioGiveBillFact
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 6, OneIsland_PokemonCenter_1F_EventScript_CelioJustGivenSapphire
	goto_if_set FLAG_RECOVERED_SAPPHIRE, OneIsland_PokemonCenter_1F_EventScript_GiveCelioSapphire
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5, OneIsland_PokemonCenter_1F_EventScript_ExplainRainbowPass
	goto_if_set FLAG_GOT_RUBY, OneIsland_PokemonCenter_1F_EventScript_GiveCelioRuby
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, OneIsland_PokemonCenter_1F_EventScript_CelioWaitingForRuby
	specialvar VAR_RESULT, IsNationalPokedexEnabled
	goto_if_eq VAR_RESULT, TRUE, OneIsland_PokemonCenter_1F_EventScript_CelioRequestRuby
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 3, OneIsland_PokemonCenter_1F_EventScript_CelioPlayerMissingNationalDex
	msgbox OneIsland_PokemonCenter_1F_Text_SorryForBeingPoorHost
	closemessage
	applymovement LOCALID_CELIO, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioGiveBillFact::
	msgbox OneIsland_PokemonCenter_1F_Text_CelioHearingRumorsAboutYou
	random 3
	goto_if_eq VAR_RESULT, 0, OneIsland_PokemonCenter_1F_EventScript_BillFact1
	goto_if_eq VAR_RESULT, 1, OneIsland_PokemonCenter_1F_EventScript_BillFact2
	famechecker FAMECHECKER_BILL, 5
	msgbox OneIsland_PokemonCenter_1F_Text_BillCantStomachMilk
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BillFact1::
	famechecker FAMECHECKER_BILL, 3
	msgbox OneIsland_PokemonCenter_1F_Text_BillsFirstMonWasAbra
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BillFact2::
	famechecker FAMECHECKER_BILL, 4
	msgbox OneIsland_PokemonCenter_1F_Text_BillsHometownInGoldenrod
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioJustGivenSapphire::
	msgbox OneIsland_PokemonCenter_1F_Text_WishYouBestOfLuck
	release
	end

OneIsland_PokemonCenter_1F_EventScript_GiveCelioSapphire::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message OneIsland_PokemonCenter_1F_Text_HandedSapphireToCelio
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	removeitem ITEM_SAPPHIRE
	msgbox OneIsland_PokemonCenter_1F_Text_ThankYouGiveMeTime
	closemessage
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioPutGemInMachine
	waitmovement 0
	playse SE_CLICK
	call OneIsland_PokemonCenter_1F_EventScript_SetSapphireMetatile
	special DrawWholeMapView
	msgbox OneIsland_PokemonCenter_1F_Text_OkayThisIsGood
	closemessage
	delay 60
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioCheckMachine
	waitmovement 0
	delay 30
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioCheckMachine
	waitmovement 0
	delay 40
	playse SE_PC_ON
	call OneIsland_PokemonCenter_1F_EventScript_SetNetworkMachineOn
	special DrawWholeMapView
	msgbox OneIsland_PokemonCenter_1F_Text_LinkedUpWithLanette
	applymovement LOCALID_CELIO, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_ManagedToLinkWithHoennThankYou
	special SetPostgameFlags
	setflag FLAG_IS_CHAMPION
	setflag FLAG_HIDE_CERULEAN_CAVE_GUARD
	setflag FLAG_HIDE_LORELEI_IN_HER_HOUSE
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 6
	special InitRoamer
	release
	end

OneIsland_PokemonCenter_1F_EventScript_ExplainRainbowPass::
	msgbox OneIsland_PokemonCenter_1F_Text_PassLetYouGetToAllIslands
	release
	end

OneIsland_PokemonCenter_1F_EventScript_GiveCelioRuby::
	msgbox OneIsland_PokemonCenter_1F_Text_OhThats
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message OneIsland_PokemonCenter_1F_Text_HandedRubyToCelio
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	closemessage
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioPutGemInMachine
	waitmovement 0
	playse SE_CLICK
	call OneIsland_PokemonCenter_1F_EventScript_SetRubyMetatile
	special DrawWholeMapView
	delay 30
	applymovement LOCALID_CELIO, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_MayIAskOneMoreFavor, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OneIsland_PokemonCenter_1F_EventScript_DeclineHelpCelio
	goto OneIsland_PokemonCenter_1F_EventScript_GiveRainbowPass
	end

OneIsland_PokemonCenter_1F_EventScript_DeclineHelpCelio::
	msgbox OneIsland_PokemonCenter_1F_Text_PleaseINeedYourHelp, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OneIsland_PokemonCenter_1F_EventScript_DeclineHelpCelio
	goto OneIsland_PokemonCenter_1F_EventScript_GiveRainbowPass
	end

OneIsland_PokemonCenter_1F_EventScript_GiveRainbowPass::
	msgbox OneIsland_PokemonCenter_1F_Text_AnotherGemstoneInSeviiIslands
	removeitem ITEM_RUBY
	removeitem ITEM_TRI_PASS
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5
	additem ITEM_RAINBOW_PASS
	setflag FLAG_SYS_SEVII_MAP_4567
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message OneIsland_PokemonCenter_1F_Text_ReturnedTriPassForRainbowPass
	waitmessage
	waitfanfare
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message OneIsland_PokemonCenter_1F_Text_ObtainedExtraMapPage
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	goto OneIsland_PokemonCenter_1F_EventScript_ExplainRainbowPass
	end

OneIsland_PokemonCenter_1F_EventScript_CelioWaitingForRuby::
	msgbox OneIsland_PokemonCenter_1F_Text_TryingToFindGem
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioRequestRuby::
	msgbox OneIsland_PokemonCenter_1F_Text_CelioCaughtMoreMonMaybeICanBeUseful
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_YoullBeTradingFromTrainersFarAway
	applymovement LOCALID_CELIO, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_NeedsSpecialGemstone
	setvar VAR_MAP_SCENE_MT_EMBER_EXTERIOR, 1
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioPlayerMissingNationalDex::
	msgbox OneIsland_PokemonCenter_1F_Text_CelioImModifyingMyNetworkMachine
	release
	end

OneIsland_PokemonCenter_1F_Movement_CelioCheckMachine::
	walk_in_place_up
	delay_8
	walk_in_place_up
	step_end

OneIsland_PokemonCenter_1F_Movement_CelioPutGemInMachine::
	walk_in_place_faster_up
	walk_in_place_up
	delay_16
	walk_in_place_up
	walk_in_place_up
	step_end

OneIsland_PokemonCenter_1F_EventScript_LittleBoy::
	msgbox OneIsland_PokemonCenter_1F_Text_CameFromPalletDontKnowIt, MSGBOX_NPC
	end

OneIsland_PokemonCenter_1F_EventScript_Hiker::
	msgbox OneIsland_PokemonCenter_1F_Text_EnormousVolcanoOnIsland, MSGBOX_NPC
	end

OneIsland_PokemonCenter_1F_EventScript_BattleGirl::
	lock
	faceplayer
	goto_if_set FLAG_IS_CHAMPION, OneIsland_PokemonCenter_1F_EventScript_BattleGirlHoennLinked
	msgbox OneIsland_PokemonCenter_1F_Text_WishICouldTradeWithBoyfriend
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BattleGirlHoennLinked::
	msgbox OneIsland_PokemonCenter_1F_Text_TradedWithFarAwayBoyfriend
	release
	end

OneIsland_PokemonCenter_1F_EventScript_NetworkMachine::
	lockall
	goto_if_set FLAG_IS_CHAMPION, OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithHoenn
	goto_if_set FLAG_SEVII_DETOUR_FINISHED, OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithKanto
	msgbox OneIsland_PokemonCenter_1F_Text_MachineUnderAdjustment
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithKanto::
	msgbox OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKanto
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithHoenn::
	msgbox OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKantoAndHoenn
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerTop::
	lockall
	setvar VAR_TEMP_1, 1
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerMidTop::
	lockall
	setvar VAR_TEMP_1, 2
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerMidBottom::
	lockall
	setvar VAR_TEMP_1, 3
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 4
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene::
	textcolor NPC_TEXT_COLOR_MALE
	playse SE_PIN
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillOhHeyPlayer
	closemessage
	call_if_eq VAR_TEMP_1, 1, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillTop
	call_if_eq VAR_TEMP_1, 2, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidTop
	call_if_eq VAR_TEMP_1, 3, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidBottom
	call_if_eq VAR_TEMP_1, 4, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillBottom
	delay 10
	msgbox OneIsland_PokemonCenter_1F_Text_BillWeGotItDone
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioJobWentQuick
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillYouveLearnedALot
	msgbox OneIsland_PokemonCenter_1F_Text_CelioOhReallyEhehe
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 10
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 15
	msgbox OneIsland_PokemonCenter_1F_Text_BillWeShouldHeadBackToKanto
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 15
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioPromiseIllShowYouAroundSometime
	closemessage
	delay 25
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerExit
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, OneIsland_PokemonCenter_1F_Movement_BillExit
	waitmovement 0
	playse SE_EXIT
	setflag FLAG_HIDE_ONE_ISLAND_POKECENTER_BILL
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 3
	clearflag FLAG_HIDE_CINNABAR_BILL
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND, 3
	setvar VAR_0x8004, SEAGALLOP_ONE_ISLAND
	setvar VAR_0x8006, SEAGALLOP_CINNABAR_ISLAND
	goto EventScript_SetSail
	end

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillTop::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillTop
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidTop::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidTop
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidBottom::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidBottom
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillBottom::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillBottom
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillTop::
	walk_right
	walk_down
	walk_in_place_faster_right
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidTop::
	walk_right
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidBottom::
	walk_right
	walk_up
	walk_in_place_faster_right
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillBottom::
	walk_right
	walk_up
	walk_up
	walk_in_place_faster_right
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_DelayWalkInPlaceLeft::
	delay_16
	delay_16
	delay_4
	walk_in_place_faster_left
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_WalkInPlaceDown::
	walk_in_place_fast_down
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerExit::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

OneIsland_PokemonCenter_1F_Movement_BillExit::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

OneIsland_PokemonCenter_1F_Text_BillHeyThereCelio::
	.string "BILL: Hey, there!\n"
	.string "CELIO!$"
	.string "{JPN}{JPN}マサキ:　よう！　ニシキ！$"

OneIsland_PokemonCenter_1F_Text_CelioCantBelieveYouCameOut::
	.string "CELIO: BILL!\n"
	.string "I can't believe you came out here.$"
	.string "{JPN}{JPN}ニシキ:　マサキさん！　ほんとうに　きて\n"
	.string "くれたんですね。$"

OneIsland_PokemonCenter_1F_Text_BillHowsYourResearchComing::
	.string "BILL: Well, absolutely!\n"
	.string "How's your research coming along?\p"
	.string "…Oh, wait a sec.$"
	.string "{JPN}{JPN}マサキ:　もちろんや！　けんきゅうの　ほうは\n"
	.string "どないや？\p"
	.string "‥あ　ちょい　まち。$"

OneIsland_PokemonCenter_1F_Text_ThisIsMyBuddyCelio::
	.string "{PLAYER}, this is my buddy CELIO.\n"
	.string "He's one dedicated PC MANIAC!$"
	.string "{JPN}{JPN}{PLAYER}　こいつは　わいの　なかまの\n"
	.string "ニシキや。　パソコンに　めっちゃ　くわしい\l"
	.string "マニア　なんやで！$"

OneIsland_PokemonCenter_1F_Text_PlayerIsRisingPokemonChamp::
	.string "CELIO, this is {PLAYER}, a rising\n"
	.string "contender as the POKéMON CHAMP!$"
	.string "{JPN}{JPN}ニシキ　こっちは　{PLAYER}や。　いま\n"
	.string "のりにのっとる　チャンピオン　こうほ　なんやで！$"

OneIsland_PokemonCenter_1F_Text_PlayerIsReigningPokemonChamp::
	.string "CELIO, this is {PLAYER}, the\n"
	.string "reigning POKéMON CHAMP!$"
	.string "{JPN}{JPN}ニシキ　こっちは　{PLAYER}や。　げんえきの\n"
	.string "ポケモン　チャンピオン　なんやで！$"

OneIsland_PokemonCenter_1F_Text_CelioThatsReallyImpressive::
	.string "CELIO: That's really impressive.\p"
	.string "I hate to say it, but I have zero\n"
	.string "aptitude for battling.\p"
	.string "Anyways, I'm glad to meet you.$"
	.string "{JPN}{JPN}ニシキ:　それは　すごいですね。\p"
	.string "はずかしい　はなし　ぼくは　バトルの　さいのうが\n"
	.string "まったく　なくて。\p"
	.string "とにかく　あえて　うれしいです。$"

OneIsland_PokemonCenter_1F_Text_BillBringMeUpToSpeed::
	.string "BILL: So, bring me up to speed.\n"
	.string "How's your machine running?$"
	.string "{JPN}{JPN}マサキ:　で　じょうきょうは　どうや？　マシンの\n"
	.string "ちょうしは？$"

OneIsland_PokemonCenter_1F_Text_CelioPCsCantLinkWithYours::
	.string "CELIO: It's running fine, but we're\n"
	.string "too remote out here.\p"
	.string "The PCs on this island just can't\n"
	.string "link with your PC, BILL.$"
	.string "{JPN}{JPN}ニシキ:　うごいては　いるんですが　ここは\n"
	.string "とおすぎて。\p"
	.string "この　しまの　パソコンは　マサキさんの\n"
	.string "パソコンと　どうしても　つながらないんです。$"

OneIsland_PokemonCenter_1F_Text_BillLetMeHelpYou::
	.string "BILL: Oh, yeah?\n"
	.string "Okay, let me take a look-see.\p"
	.string "…Hang on here…\n"
	.string "I think we can make it work.\l"
	.string "Let me help you, okay?$"
	.string "{JPN}{JPN}マサキ:　ほう　そうか？　ほな　ちょっと\n"
	.string "みせてみ。\p"
	.string "‥まてよ‥　これなら　なんとか　なりそうや。\n"
	.string "わいが　てつだったるわ！$"

OneIsland_PokemonCenter_1F_Text_CanYouDeliverThisMeteoritePlayer::
	.string "{PLAYER}, can I get you to wait for\n"
	.string "me just a bit?\p"
	.string "…Actually, can I get you to do\n"
	.string "me a favor?\p"
	.string "The island next to this one's\n"
	.string "called TWO ISLAND.\p"
	.string "There's a guy there that runs\n"
	.string "a GAME CORNER.\p"
	.string "He has this thing for rare rocks\n"
	.string "and gems.\p"
	.string "We keep in touch, being fellow\n"
	.string "maniacs.\p"
	.string "So, can I get you to deliver this\n"
	.string "METEORITE to him?$"
	.string "{JPN}{JPN}{PLAYER}　ちょっと　まっとって　くれるか？\p"
	.string "‥いや　そや　ひとつ　たのみを　きいて\n"
	.string "くれへんか？\p"
	.string "この　となりの　しまは　2のしまって　いうんや。\p"
	.string "そこに　ゲームコーナーを　やっとる　おっちゃんが\n"
	.string "おってな。\p"
	.string "めずらしい　いしや　ほうせきに　めが　ないんや。\p"
	.string "マニア　どうし　れんらくを　とりおうとる\n"
	.string "なかでな。\p"
	.string "で　この　いんせきを　その　ひとに　とどけて\n"
	.string "くれへんか？$"

OneIsland_PokemonCenter_1F_Text_AcceptedMeteoriteFromBill::
	.string "{PLAYER} accepted the METEORITE\n"
	.string "from BILL.$"
	.string "{JPN}{JPN}{PLAYER}は　マサキから　いんせきを\n"
	.string "あずかった。$"

OneIsland_PokemonCenter_1F_Text_CelioPleaseTakeThis::
	.string "CELIO: {PLAYER}, if you are going\n"
	.string "to TWO ISLAND, please take this.$"
	.string "{JPN}{JPN}ニシキ:　{PLAYER}さん　2のしまに\n"
	.string "いくなら　これを　どうぞ。$"

OneIsland_PokemonCenter_1F_Text_PassLetsYouTravelBetweenIslands::
	.string "It's a PASS for the ferry service\n"
	.string "serving the local islands.\p"
	.string "It will let you travel between the\n"
	.string "ISLANDS ONE, TWO, and THREE.\p"
	.string "Oh, you should have this, too.$"
	.string "{JPN}{JPN}この　あたりの　しまを　むすぶ　ふねの　パス\n"
	.string "です。\p"
	.string "1のしま　2のしま　3のしまの　あいだを\n"
	.string "いききできます。\p"
	.string "そうだ　これも　どうぞ。$"

OneIsland_PokemonCenter_1F_Text_ReceivedExtraPageForTownMap::
	.string "{PLAYER} received an extra page\n"
	.string "for the TOWN MAP!$"
	.string "{JPN}{JPN}{PLAYER}は　タウンマップの　ついか\n"
	.string "ページを　もらった！$"

OneIsland_PokemonCenter_1F_Text_ReceivedTownMap::
	.string "{PLAYER} received\n"
	.string "a TOWN MAP!$"
	.string "{JPN}{JPN}{PLAYER}は　タウンマップを　もらった！$"

OneIsland_PokemonCenter_1F_Text_BillCatchYouLater::
	.string "BILL: I'll catch you later!\n"
	.string "Say hi to the guy for me!$"
	.string "{JPN}{JPN}マサキ:　ほな　また　あとでな！　おっちゃんに\n"
	.string "よろしゅう　いうといてや！$"

OneIsland_PokemonCenter_1F_Text_HmmHowAboutLikeThis::
	.string "Hmm…\p"
	.string "How about we try this like this…$"
	.string "{JPN}{JPN}うーん‥\p"
	.string "ここを　こう　してみたら　どうや‥$"

OneIsland_PokemonCenter_1F_Text_GotPCWorkingStrollAWhileMore::
	.string "Oh, hey, {PLAYER}!\p"
	.string "Did you see?\n"
	.string "We got the PC working!\p"
	.string "I've got a few things to show\n"
	.string "CELIO here.\p"
	.string "Can you go out on a stroll or\n"
	.string "something for a while more?$"
	.string "{JPN}{JPN}おっ　{PLAYER}やないか！\p"
	.string "みたか？　パソコンが　うごいたで！\p"
	.string "ニシキに　いくつか　みせたい　ことが　あってな。\p"
	.string "もう　しばらく　そのへんを　さんぽ　でも\n"
	.string "しとって　くれへんか？$"

OneIsland_PokemonCenter_1F_Text_SorryForBeingPoorHost::
	.string "I'm sorry for taking up so much of\n"
	.string "BILL's time.\p"
	.string "I'm also sorry for being such a \n"
	.string "poor host on your visit here.$"
	.string "{JPN}{JPN}マサキさんの　じかんを　たくさん　とって\n"
	.string "しまって　すみません。\p"
	.string "せっかく　きて　いただいたのに　おもてなしも\n"
	.string "できなくて　ごめんなさい。$"

OneIsland_PokemonCenter_1F_Text_UsualPCServicesUnavailable::
	.string "The usual PC services aren't\n"
	.string "available…$"
	.string "{JPN}{JPN}いつもの　パソコンの　サービスは　つかえない\n"
	.string "ようだ‥$"

OneIsland_PokemonCenter_1F_Text_BillOhHeyPlayer::
	.string "BILL: Oh, hey!\n"
	.string "{PLAYER}!$"
	.string "{JPN}{JPN}マサキ:　おっ！　{PLAYER}！$"

OneIsland_PokemonCenter_1F_Text_BillWeGotItDone::
	.string "BILL: What kept you so long?\n"
	.string "Been out having a good time?\p"
	.string "We got it done.\n"
	.string "The PCs are up and running!$"
	.string "{JPN}{JPN}マサキ:　えらい　おそかったなあ？　たのしんで\n"
	.string "きたんか？\p"
	.string "こっちは　おわったで。　パソコンは　ばっちり\n"
	.string "うごいとる！$"

OneIsland_PokemonCenter_1F_Text_CelioJobWentQuick::
	.string "CELIO: The job went incredibly\n"
	.string "quick.\p"
	.string "BILL is one amazing guy…$"
	.string "{JPN}{JPN}ニシキ:　しんじられないほど　はやく\n"
	.string "おわりました。\p"
	.string "マサキさんは　ほんとうに　すごい　ひとです‥$"

OneIsland_PokemonCenter_1F_Text_BillYouveLearnedALot::
	.string "BILL: No, no! There was almost\n"
	.string "nothing left for me to do.\p"
	.string "CELIO, I have to hand it to you.\n"
	.string "You've learned a lot.$"
	.string "{JPN}{JPN}マサキ:　いやいや！　わいが　やる　ことなんて\n"
	.string "ほとんど　のこってへんかったで。\p"
	.string "ニシキ　たいした　もんや。　よう　べんきょう\n"
	.string "しとるわ。$"

OneIsland_PokemonCenter_1F_Text_CelioOhReallyEhehe::
	.string "CELIO: Oh, really?\n"
	.string "Ehehe…$"
	.string "{JPN}{JPN}ニシキ:　ほんとう　ですか？　えへへ‥$"

OneIsland_PokemonCenter_1F_Text_BillWeShouldHeadBackToKanto::
	.string "BILL: Well, there you have it.\n"
	.string "I'm finished with the job.\l"
	.string "We should head back to KANTO.\p"
	.string "CELIO, I'll be seeing you again.$"
	.string "{JPN}{JPN}マサキ:　ほな　そういう　わけで　わいの\n"
	.string "しごとは　おわりや。　カントーに　もどろか。\p"
	.string "ニシキ　また　あおな。$"

OneIsland_PokemonCenter_1F_Text_CelioPromiseIllShowYouAroundSometime::
	.string "CELIO: {PLAYER}, I'm really sorry\n"
	.string "that we sent you off alone today.\p"
	.string "I promise, I will show you around\n"
	.string "these islands sometime.$"
	.string "{JPN}{JPN}ニシキ:　{PLAYER}さん　きょうは　ひとりで\n"
	.string "いかせて　しまって　ほんとうに　すみません。\p"
	.string "いつか　かならず　この　しまじまを　あんない\n"
	.string "しますね。$"

OneIsland_PokemonCenter_1F_Text_CelioImModifyingMyNetworkMachine::
	.string "CELIO: Hello!\n"
	.string "You look awfully busy as always.\p"
	.string "How am I doing?\p"
	.string "Well, I'm modifying my Network\n"
	.string "Machine.\p"
	.string "When I get done with the machine,\n"
	.string "I hope you'll be first to use it,\l"
	.string "{PLAYER}.$"
	.string "{JPN}{JPN}ニシキ:　こんにちは！　あいかわらず\n"
	.string "いそがしそう　ですね。\p"
	.string "ぼくですか？\p"
	.string "いま　ネットワークマシンを　かいぞう\n"
	.string "しているんです。\p"
	.string "マシンが　かんせい　したら　{PLAYER}さんに\n"
	.string "さいしょに　つかって　ほしいです。$"

OneIsland_PokemonCenter_1F_Text_CelioCaughtMoreMonMaybeICanBeUseful::
	.string "CELIO: {PLAYER}, how have things\n"
	.string "been for you?\p"
	.string "Oh, is that right?\n"
	.string "You've caught more POKéMON.\p"
	.string "Do you know what?\n"
	.string "Maybe I can be useful to you.$"
	.string "{JPN}{JPN}ニシキ:　{PLAYER}さん　その　あと\n"
	.string "どうですか？\p"
	.string "へえ　そうなんですか？　ポケモンを　もっと\n"
	.string "つかまえたんですね。\p"
	.string "そうだ！　ぼく　おやくに　たてる\n"
	.string "かもしれません。$"

OneIsland_PokemonCenter_1F_Text_YoullBeTradingFromTrainersFarAway::
	.string "I'm modifying the Network Machine\n"
	.string "right now.\p"
	.string "I'm changing it so it can handle\n"
	.string "trades over long distances.\p"
	.string "When I get finished, you'll be\n"
	.string "trading for exotic POKéMON from\l"
	.string "TRAINERS far away.$"
	.string "{JPN}{JPN}いま　ネットワークマシンを　かいぞう　している\n"
	.string "ところ　なんです。\p"
	.string "とおくの　ひととも　こうかん　できるように　して\n"
	.string "いるんです。\p"
	.string "かんせい　したら　とおくの　トレーナーと\n"
	.string "めずらしい　ポケモンを　こうかん　できますよ。$"

OneIsland_PokemonCenter_1F_Text_NeedsSpecialGemstone::
	.string "But, there is a slight catch.\p"
	.string "For the link to work, the Machine\n"
	.string "needs a special gemstone.\p"
	.string "It's supposed to be on ONE ISLAND,\n"
	.string "but I haven't found one yet.\p"
	.string "Who knows where it could be.$"
	.string "{JPN}{JPN}でも　ひとつ　もんだいが　あるんです。\p"
	.string "つなぐ　ためには　マシンに　とくべつな\n"
	.string "ほうせきが　ひつよう　なんです。\p"
	.string "1のしまに　ある　はず　なんですが　まだ\n"
	.string "みつけられて　いなくて。\p"
	.string "いったい　どこに　あるんでしょう。$"

OneIsland_PokemonCenter_1F_Text_TryingToFindGem::
	.string "I was trying to find the gem\n"
	.string "even while I was studying.\p"
	.string "As a result, I've made no headway\n"
	.string "in both my search and studies…\p"
	.string "If I relied on BILL, I'm sure my\n"
	.string "research would progress.\p"
	.string "But this time, I want to try to do\n"
	.string "things by myself.$"
	.string "{JPN}{JPN}べんきょう　しながら　ほうせきも　さがして\n"
	.string "いたんです。\p"
	.string "その　せいで　さがしものも　べんきょうも\n"
	.string "ちっとも　すすまなくて‥\p"
	.string "マサキさんを　たよれば　きっと　けんきゅうは\n"
	.string "すすむと　おもいます。\p"
	.string "でも　こんどは　じぶんの　ちからで　やって\n"
	.string "みたいんです。$"

OneIsland_PokemonCenter_1F_Text_OhThats::
	.string "Oh!\n"
	.string "Th-that's…$"
	.string "{JPN}{JPN}あっ！　そ　それは‥$"

OneIsland_PokemonCenter_1F_Text_HandedRubyToCelio::
	.string "{PLAYER} handed the RUBY\n"
	.string "to CELIO.$"
	.string "{JPN}{JPN}{PLAYER}は　ニシキに　ルビーを　わたした。$"

OneIsland_PokemonCenter_1F_Text_MayIAskOneMoreFavor::
	.string "Thank you!\n"
	.string "{PLAYER}, you're simply amazing.\p"
	.string "… … …  … … …\p"
	.string "Um… May I ask one more giant favor\n"
	.string "of you?$"
	.string "{JPN}{JPN}ありがとう　ございます！　{PLAYER}さん\n"
	.string "ほんとうに　すごいです。\p"
	.string "‥　‥　‥　‥　‥　‥\p"
	.string "あの‥　もうひとつ　おおきな　おねがいを　しても\n"
	.string "いいですか？$"

OneIsland_PokemonCenter_1F_Text_PleaseINeedYourHelp::
	.string "It… It's not anything weird.\n"
	.string "Please, I need your help.$"
	.string "{JPN}{JPN}へ　へんな　ことじゃ　ないんです。　おねがいです\n"
	.string "ちからを　かして　ください。$"

OneIsland_PokemonCenter_1F_Text_AnotherGemstoneInSeviiIslands::
	.string "While I was studying gemstones,\n"
	.string "I discovered something important.\p"
	.string "There is another gem that forms\n"
	.string "a pair with this RUBY.\p"
	.string "That other gemstone is supposed to\n"
	.string "be in the SEVII ISLANDS.\p"
	.string "{PLAYER}, please, I need you to go\n"
	.string "find the other gem.\p"
	.string "{PLAYER}, may I have your ferry\n"
	.string "PASS and the TOWN MAP?$"
	.string "{JPN}{JPN}ほうせきを　しらべて　いるうちに　だいじな\n"
	.string "ことが　わかったんです。\p"
	.string "この　ルビーと　ついに　なる　ほうせきが\n"
	.string "もうひとつ　あるんです。\p"
	.string "その　ほうせきは　ナナシマの　どこかに　ある\n"
	.string "はず　なんです。\p"
	.string "{PLAYER}さん　おねがいです。　もうひとつの\n"
	.string "ほうせきを　さがして　きて　ください。\p"
	.string "{PLAYER}さん　ふねの　パスと\n"
	.string "タウンマップを　かして　もらえますか？$"

OneIsland_PokemonCenter_1F_Text_ReturnedTriPassForRainbowPass::
	.string "{PLAYER} returned the TRI-PASS and\n"
	.string "received the RAINBOW PASS.$"
	.string "{JPN}{JPN}{PLAYER}は　トライパスを　かえして\n"
	.string "レインボーパスを　もらった！$"

OneIsland_PokemonCenter_1F_Text_ObtainedExtraMapPage::
	.string "Obtained an extra page for the\n"
	.string "TOWN MAP!$"
	.string "{JPN}{JPN}タウンマップの　ついか　ページを　てに　いれた！$"

OneIsland_PokemonCenter_1F_Text_PassLetYouGetToAllIslands::
	.string "This is my own ferry PASS.\p"
	.string "It will let you get to all the\n"
	.string "SEVII ISLANDS.\p"
	.string "{PLAYER}, please, I can't do\n"
	.string "it without your help.$"
	.string "{JPN}{JPN}これは　ぼくの　ふねの　パス　です。\p"
	.string "ナナシマの　すべての　しまに　いけます。\p"
	.string "{PLAYER}さん　おねがいします。　あなたの\n"
	.string "たすけが　ないと　できないんです。$"

OneIsland_PokemonCenter_1F_Text_HandedSapphireToCelio::
	.string "{PLAYER} handed the SAPPHIRE\n"
	.string "to CELIO.$"
	.string "{JPN}{JPN}{PLAYER}は　ニシキに　サファイアを\n"
	.string "わたした。$"

OneIsland_PokemonCenter_1F_Text_ThankYouGiveMeTime::
	.string "CELIO: So this is the gem that\n"
	.string "forms a pair with the RUBY…\p"
	.string "{PLAYER}, you've gone through a lot\n"
	.string "to get this, didn't you?\p"
	.string "You don't have to tell me. I know\n"
	.string "it wasn't easy.\p"
	.string "Thank you so much!\p"
	.string "Now it's my turn to work for you!\n"
	.string "Please give me a little time.$"
	.string "{JPN}{JPN}ニシキ:　これが　ルビーと　ついに　なる\n"
	.string "ほうせき‥\p"
	.string "{PLAYER}さん　これを　てに　いれるのに\n"
	.string "たいへんな　おもいを　したんでしょう？\p"
	.string "いわなくても　わかります。　かんたんじゃ\n"
	.string "なかった　はずです。\p"
	.string "ほんとうに　ありがとう　ございます！\p"
	.string "こんどは　ぼくが　がんばる　ばんです！　すこし\n"
	.string "じかんを　ください。$"

OneIsland_PokemonCenter_1F_Text_OkayThisIsGood::
	.string "Okay, this is good…$"
	.string "{JPN}{JPN}よし　これで　いい‥$"

OneIsland_PokemonCenter_1F_Text_LinkedUpWithLanette::
	.string "I did it!\n"
	.string "I linked up with LANETTE!$"
	.string "{JPN}{JPN}やった！　マユミさんと　つながったぞ！$"

OneIsland_PokemonCenter_1F_Text_ManagedToLinkWithHoennThankYou::
	.string "{PLAYER}…\n"
	.string "{PLAYER}, I did it!\p"
	.string "I've managed to link up with\n"
	.string "TRAINERS in the HOENN region!\p"
	.string "Finally, the Network Machine is\n"
	.string "fully operational!\p"
	.string "{PLAYER}, I owe it all to you!\p"
	.string "Thanks to you, my dream came\n"
	.string "true…$"
	.string "{JPN}{JPN}{PLAYER}さん‥　{PLAYER}さん\n"
	.string "やりました！\p"
	.string "ホウエンちほうの　トレーナーと　つながる　ことが\n"
	.string "できたんです！\p"
	.string "ついに　ネットワークマシンが　かんぜんに\n"
	.string "うごきました！\p"
	.string "{PLAYER}さん　ぜんぶ　あなたの\n"
	.string "おかげです！\p"
	.string "あなたの　おかげで　ぼくの　ゆめが\n"
	.string "かないました‥$"

OneIsland_PokemonCenter_1F_Text_WishYouBestOfLuck::
	.string "I…\n"
	.string "I'm not crying.\p"
	.string "That's enough about me!\p"
	.string "{PLAYER}, you're going to keep\n"
	.string "looking for exotic POKéMON, right?\p"
	.string "I wish you the best of luck!$"
	.string "{JPN}{JPN}ぼ　ぼくは　ないて　なんか　いませんよ。\p"
	.string "ぼくの　はなしは　もう　いいです！\p"
	.string "{PLAYER}さんは　これからも　めずらしい\n"
	.string "ポケモンを　さがし　つづけるん　ですよね？\p"
	.string "がんばって　ください！$"

OneIsland_PokemonCenter_1F_Text_CelioHearingRumorsAboutYou::
	.string "CELIO: Hello!\p"
	.string "{PLAYER}, I've been hearing rumors\n"
	.string "about you.$"
	.string "{JPN}{JPN}ニシキ:　こんにちは！\p"
	.string "{PLAYER}さんの　うわさ　いろいろ　きいて\n"
	.string "いますよ。$"

OneIsland_PokemonCenter_1F_Text_BillsFirstMonWasAbra::
	.string "{PLAYER}, what's your favorite kind\n"
	.string "of POKéMON?\p"
	.string "BILL is a POKéMANIAC, so he loves\n"
	.string "every kind.\p"
	.string "Apparently, the first one he caught\n"
	.string "was an ABRA.$"
	.string "{JPN}{JPN}{PLAYER}さんは　どんな　ポケモンが　すき\n"
	.string "ですか？\p"
	.string "マサキさんは　ポケマニア　だから　どんな\n"
	.string "ポケモンも　だいすき　なんです。\p"
	.string "さいしょに　つかまえた　ポケモンは　ケーシィ\n"
	.string "だったそうですよ。$"

OneIsland_PokemonCenter_1F_Text_BillsHometownInGoldenrod::
	.string "By the way, {PLAYER}, you're from\n"
	.string "PALLET TOWN, aren't you?\p"
	.string "I've heard that it's a quiet and\n"
	.string "pleasant place.\p"
	.string "BILL's hometown is GOLDENROD CITY,\n"
	.string "where his folks still live.\p"
	.string "I've heard that it's quite the\n"
	.string "festive, bustling city.\p"
	.string "I'd like to go there one day.$"
	.string "{JPN}{JPN}ところで　{PLAYER}さんは　マサラタウン\n"
	.string "しゅっしん　ですよね？\p"
	.string "しずかで　いい　ところだと　ききました。\p"
	.string "マサキさんの　ふるさとは　コガネシティで　いまも\n"
	.string "かぞくが　すんでいるんです。\p"
	.string "とても　にぎやかな　まち　だそうです。\p"
	.string "いつか　いって　みたいなあ。$"

OneIsland_PokemonCenter_1F_Text_BillCantStomachMilk::
	.string "{PLAYER}, is there anything that\n"
	.string "you can't stand?\p"
	.string "Apparently, BILL simply can't\n"
	.string "stomach milk at all.$"
	.string "{JPN}{JPN}{PLAYER}さんは　にがてな　もの　って\n"
	.string "ありますか？\p"
	.string "マサキさんは　ぎゅうにゅうが　まったく　だめ\n"
	.string "なんだそうです。$"

OneIsland_PokemonCenter_1F_Text_CameFromPalletDontKnowIt::
	.string "Oh, you're a stranger here!\n"
	.string "Hi! Where did you come from?\p"
	.string "…PALLET TOWN?\n"
	.string "I don't know it!$"
	.string "{JPN}{JPN}あら　みない　かおね！　こんにちは！　どこから\n"
	.string "きたの？\p"
	.string "‥マサラタウン？　しらないわ！$"

OneIsland_PokemonCenter_1F_Text_EnormousVolcanoOnIsland::
	.string "On this island, there's an enormous\n"
	.string "volcano.\p"
	.string "It hasn't erupted lately, so why\n"
	.string "not enjoy a hike?$"
	.string "{JPN}{JPN}この　しまには　おおきな　かざんが　あるのよ。\p"
	.string "さいきんは　ふんか　して　いないから\n"
	.string "ハイキングを　たのしんで　みたら？$"

OneIsland_PokemonCenter_1F_Text_WishICouldTradeWithBoyfriend::
	.string "I wish I could trade POKéMON with\n"
	.string "my boyfriend who lives far away…$"
	.string "{JPN}{JPN}とおくに　すんでる　かれと　ポケモンを　こうかん\n"
	.string "できたら　いいのに‥$"

OneIsland_PokemonCenter_1F_Text_TradedWithFarAwayBoyfriend::
	.string "I traded POKéMON with my boyfriend\n"
	.string "far away!\p"
	.string "Everyone's saying that we can\n"
	.string "thank you and CELIO.\p"
	.string "So, thank you!$"
	.string "{JPN}{JPN}とおくの　かれと　ポケモンを　こうかん\n"
	.string "できたの！\p"
	.string "あなたと　ニシキさんの　おかげだって　みんな\n"
	.string "いってるわ。\p"
	.string "だから　ありがとう！$"

OneIsland_PokemonCenter_1F_Text_MachineUnderAdjustment::
	.string "Network Machine\n"
	.string "Link Level 0\p"
	.string "…POKéMON Storage System under\n"
	.string "adjustment…$"
	.string "{JPN}{JPN}ネットワークマシン\n"
	.string "リンクレベル　0\p"
	.string "‥ポケモン　あずかり　システム　ちょうせい\n"
	.string "ちゅう‥$"

OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKanto::
	.string "Network Machine\n"
	.string "Link Level 1\p"
	.string "Link established with the KANTO\n"
	.string "region.$"
	.string "{JPN}{JPN}ネットワークマシン\n"
	.string "リンクレベル　1\p"
	.string "カントーちほうと　せつぞく　しました。$"

OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKantoAndHoenn::
	.string "Network Machine\n"
	.string "Link Level 2\p"
	.string "Link established with the KANTO\n"
	.string "and HOENN regions.$"
	.string "{JPN}{JPN}ネットワークマシン\n"
	.string "リンクレベル　2\p"
	.string "カントーちほう　ホウエンちほうと　せつぞく\n"
	.string "しました。$"

OneIsland_PokemonCenter_1F_Text_ObtainedTriPass::
	.string "Obtained the TRI-PASS!$"
	.string "{JPN}{JPN}トライパスを　てに　いれた！$"

