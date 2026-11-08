Skyrift_Observatory_Roof_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, QoL_Skyrift_Roof_OnTransition
	.byte 0

Skyrift_Observatory_Roof_EventScript_Engine::
	msgbox Skyrift_Observatory_Roof_Text_Engine, MSGBOX_SIGN
	end

Skyrift_Observatory_Roof_Text_Engine::
	.string "The SKYRIFT ENGINE. Its masts\n"
	.string "point at the tear in the sky.\p"
	.string "Deep inside, something throbs\n"
	.string "like a heartbeat.$"

QoL_Skyrift_Roof_OnTransition::
	setvar VAR_TEMP_4, 0
	call_if_set FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Roof_Off
	setflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_VEGA
	setflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_SIRIUS
	call_if_set FLAG_QOL_SKYRIFT_VEGA_SAVED, QoL_Skyrift_Roof_ShowVega
	call_if_set FLAG_QOL_SKYRIFT_VEGA_LOST, QoL_Skyrift_Roof_ShowSirius
	end

QoL_Skyrift_Roof_Off::
	setvar VAR_TEMP_4, 1
	return

QoL_Skyrift_Roof_ShowVega::
	clearflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_VEGA
	return

QoL_Skyrift_Roof_ShowSirius::
	clearflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_SIRIUS
	return

QoL_Skyrift_Roof_EventScript_Climax::
	lockall
	setvar VAR_TEMP_4, 1
	msgbox QoL_Skyrift_Roof_Text_Vega1, MSGBOX_DEFAULT
	call_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_TalkALTAIR
	call_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_TalkDENEB
	call_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_TalkMIRA
	msgbox QoL_Skyrift_Roof_Text_Vega2, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Roof_Text_Sirius1, MSGBOX_DEFAULT
	goto_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_BattleALTAIR
	goto_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_BattleDENEB
	goto_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_BattleMIRA
	trainerbattle_two_trainers TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_TalkALTAIR::
	msgbox QoL_Skyrift_Roof_Text_TalkALTAIR, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_BattleALTAIR::
	multi_fixed_2_vs_2 TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat, PARTNER_QOL_ALTAIR
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_TalkDENEB::
	msgbox QoL_Skyrift_Roof_Text_TalkDENEB, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_BattleDENEB::
	multi_fixed_2_vs_2 TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat, PARTNER_QOL_DENEB
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_TalkMIRA::
	msgbox QoL_Skyrift_Roof_Text_TalkMIRA, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_BattleMIRA::
	multi_fixed_2_vs_2 TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat, PARTNER_QOL_MIRA
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_AfterBattle::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_WON, QoL_Skyrift_Roof_Lost
	msgbox QoL_Skyrift_Roof_Text_Overload, MSGBOX_DEFAULT
	playse SE_THUNDER
	setvar VAR_0x8004, 3
	setvar VAR_0x8005, 3
	setvar VAR_0x8006, 20
	setvar VAR_0x8007, 4
	special ShakeCamera
	waitstate
	msgbox QoL_Skyrift_Roof_Text_Pulled, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Roof_Text_Choice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_LetGo
	setflag FLAG_QOL_SKYRIFT_VEGA_SAVED
	call_if_set FLAG_QOL_SKYRIFT_WISH_PEACE, QoL_Skyrift_Roof_PeaceSave
	msgbox QoL_Skyrift_Roof_Text_Saved, MSGBOX_DEFAULT
	call_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_SavedALTAIR
	call_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_SavedDENEB
	call_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_SavedMIRA
	msgbox QoL_Skyrift_Roof_Text_VegaGift, MSGBOX_DEFAULT
	givemon SPECIES_POIPOLE, 60, ITEM_NONE, ITEM_BEAST_BALL
	bufferspeciesname STR_VAR_1, SPECIES_POIPOLE
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message QoL_Skyrift_Roof_Text_Received
	waitmessage
	waitfanfare
	goto QoL_Skyrift_Roof_Epilogue

QoL_Skyrift_Roof_LetGo::
	setflag FLAG_QOL_SKYRIFT_VEGA_LOST
	msgbox QoL_Skyrift_Roof_Text_Falls, MSGBOX_DEFAULT
	closemessage
	playse SE_THUNDER
	fadescreen FADE_TO_WHITE
	removeobject LOCALID_SKYRIFT_VEGA
	fadescreen FADE_FROM_WHITE
	call_if_set FLAG_QOL_SKYRIFT_WISH_PEACE, QoL_Skyrift_Roof_PeaceLost
	call_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_LostALTAIR
	call_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_LostDENEB
	call_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_LostMIRA
	msgbox QoL_Skyrift_Roof_Text_SiriusVow, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Roof_Text_Cosmog, MSGBOX_DEFAULT
	givemon SPECIES_COSMOG, 60, ITEM_NONE, ITEM_BEAST_BALL
	bufferspeciesname STR_VAR_1, SPECIES_COSMOG
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message QoL_Skyrift_Roof_Text_Received
	waitmessage
	waitfanfare
	goto QoL_Skyrift_Roof_Epilogue

QoL_Skyrift_Roof_PeaceSave::
	msgbox QoL_Skyrift_Roof_Text_PeaceSave, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_PeaceLost::
	msgbox QoL_Skyrift_Roof_Text_PeaceLost, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_Epilogue::
	setflag FLAG_QOL_SKYRIFT_CLIMAX
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SKYRIFT_VEGA
	removeobject LOCALID_SKYRIFT_SIRIUS
	fadescreen FADE_FROM_BLACK
	goto_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_AskALTAIR
	goto_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_AskDENEB
	goto_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_AskMIRA
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_TheEnd::
	playbgm MUS_RG_CREDITS, FALSE
	msgbox QoL_Skyrift_Roof_Text_TheEnd, MSGBOX_DEFAULT
	fadedefaultbgm
	releaseall
	end

QoL_Skyrift_Roof_Lost::
	releaseall
	end

QoL_Skyrift_Roof_EventScript_RematchVega::
	lock
	faceplayer
	msgbox QoL_Skyrift_Roof_Text_RematchVega, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_RematchNo
	trainerbattle_no_intro TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat
	release
	end

QoL_Skyrift_Roof_EventScript_RematchSirius::
	lock
	faceplayer
	msgbox QoL_Skyrift_Roof_Text_RematchSirius, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_RematchNo
	trainerbattle_no_intro TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat
	release
	end

QoL_Skyrift_Roof_RematchNo::
	release
	end

QoL_Skyrift_Roof_AskALTAIR::
	msgbox QoL_Skyrift_Roof_Text_KeepALTAIR, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_PartALTAIR
	msgbox QoL_Skyrift_Roof_Text_KeepYesALTAIR, MSGBOX_DEFAULT
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_PartALTAIR::
	msgbox QoL_Skyrift_Roof_Text_KeepNoALTAIR, MSGBOX_DEFAULT
	call QoL_Skyrift_Inn_Clear
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_SavedALTAIR::
	msgbox QoL_Skyrift_Roof_Text_SavedALTAIR, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_LostALTAIR::
	msgbox QoL_Skyrift_Roof_Text_LostALTAIR, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_AskDENEB::
	msgbox QoL_Skyrift_Roof_Text_KeepDENEB, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_PartDENEB
	msgbox QoL_Skyrift_Roof_Text_KeepYesDENEB, MSGBOX_DEFAULT
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_PartDENEB::
	msgbox QoL_Skyrift_Roof_Text_KeepNoDENEB, MSGBOX_DEFAULT
	call QoL_Skyrift_Inn_Clear
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_SavedDENEB::
	msgbox QoL_Skyrift_Roof_Text_SavedDENEB, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_LostDENEB::
	msgbox QoL_Skyrift_Roof_Text_LostDENEB, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_AskMIRA::
	msgbox QoL_Skyrift_Roof_Text_KeepMIRA, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_PartMIRA
	msgbox QoL_Skyrift_Roof_Text_KeepYesMIRA, MSGBOX_DEFAULT
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_PartMIRA::
	msgbox QoL_Skyrift_Roof_Text_KeepNoMIRA, MSGBOX_DEFAULT
	call QoL_Skyrift_Inn_Clear
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_SavedMIRA::
	msgbox QoL_Skyrift_Roof_Text_SavedMIRA, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_LostMIRA::
	msgbox QoL_Skyrift_Roof_Text_LostMIRA, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_Text_Vega1::
	.string "VEGA: So the CHAMPION of two... no,\n"
	.string "three regions came all this way.\p"
	.string "And you brought a friend.$"

QoL_Skyrift_Roof_Text_TalkALTAIR::
	.string "ALTAIR: VEGA, stop! The star stone\n"
	.string "wasn't for this!\p"
	.string "VEGA: ALTAIR. Always loyal. Too\n"
	.string "loyal to ask questions.$"

QoL_Skyrift_Roof_Text_TalkDENEB::
	.string "DENEB: VEGA... I paid for every\n"
	.string "bolt of this machine. Let me help\l"
	.string "you turn it off.\p"
	.string "VEGA: My clever DENEB. You never\n"
	.string "understood what we were buying.$"

QoL_Skyrift_Roof_Text_TalkMIRA::
	.string "MIRA: Grandpa says the sky answers\n"
	.string "anyone who listens. Not anyone who\l"
	.string "shouts!\p"
	.string "VEGA: SEIJI's little star reader.\n"
	.string "How sweet.$"

QoL_Skyrift_Roof_Text_Vega2::
	.string "VEGA: The SKYRIFT ENGINE will call\n"
	.string "DEOXYS back.\p"
	.string "And this time it will answer\n"
	.string "to me. SIRIUS!$"

QoL_Skyrift_Roof_Text_Sirius1::
	.string "SIRIUS: Ma'am.$"

QoL_Skyrift_Roof_Text_VegaDefeat::
	.string "No... not again...$"

QoL_Skyrift_Roof_Text_SiriusDefeat::
	.string "Forgive me, VEGA.$"

QoL_Skyrift_Roof_Text_Overload::
	.string "VEGA: The ENGINE! It's pulling too\n"
	.string "much power... it's overloading!$"

QoL_Skyrift_Roof_Text_Pulled::
	.string "The rift above the roof tears\n"
	.string "wider and pulls at VEGA!\p"
	.string "VEGA: Help... me...!$"

QoL_Skyrift_Roof_Text_Choice::
	.string "Take VEGA's hand?$"

QoL_Skyrift_Roof_Text_PeaceSave::
	.string "A warm light falls from the sky.\n"
	.string "The WISHING STAR's light!\p"
	.string "It wraps around VEGA and pulls\n"
	.string "her back with you.$"

QoL_Skyrift_Roof_Text_Saved::
	.string "You grab VEGA's hand and pull!\p"
	.string "The rift snaps shut above the\n"
	.string "roof. The ENGINE goes quiet.\p"
	.string "VEGA: ...You saved me. Even after\n"
	.string "everything.$"

QoL_Skyrift_Roof_Text_SavedALTAIR::
	.string "ALTAIR: Told you, VEGA. The kid's\n"
	.string "the real thing.\p"
	.string "VEGA: Will you... help me take this\n"
	.string "machine apart, ALTAIR?\p"
	.string "ALTAIR: Rock by rock, boss.$"

QoL_Skyrift_Roof_Text_SavedDENEB::
	.string "DENEB: Oh, VEGA. You frightened me.\p"
	.string "VEGA: DENEB... sell it all. The\n"
	.string "engine, the island. Give the money\l"
	.string "to SEIKA's shrines.\p"
	.string "DENEB: Finally, a purchase I\n"
	.string "approve of.$"

QoL_Skyrift_Roof_Text_SavedMIRA::
	.string "MIRA: You see? The sky DOES answer\n"
	.string "if you listen!\p"
	.string "VEGA: Perhaps I should have asked a\n"
	.string "star reader before I bought a\l"
	.string "telescope.$"

QoL_Skyrift_Roof_Text_VegaGift::
	.string "VEGA: This little one came through\n"
	.string "the rift with me. It wants to go\l"
	.string "with you.$"

QoL_Skyrift_Roof_Text_Received::
	.string "{PLAYER} received {STR_VAR_1}!$"

QoL_Skyrift_Roof_Text_Falls::
	.string "You hesitate... and VEGA's hand\n"
	.string "slips away.\p"
	.string "The rift swallows her and snaps\n"
	.string "shut.$"

QoL_Skyrift_Roof_Text_PeaceLost::
	.string "As she fell, a warm light went\n"
	.string "with her. The WISHING STAR's\l"
	.string "light.\p"
	.string "Wherever she has gone, she\n"
	.string "is not alone.$"

QoL_Skyrift_Roof_Text_LostALTAIR::
	.string "ALTAIR: ...Boss.\p"
	.string "...I'll keep digging. If there's\n"
	.string "a way to her, I'll find it.$"

QoL_Skyrift_Roof_Text_LostDENEB::
	.string "DENEB: She chose her sky.\p"
	.string "I suppose... I can stop paying\n"
	.string "for it now.$"

QoL_Skyrift_Roof_Text_LostMIRA::
	.string "MIRA: She's out there with the\n"
	.string "stars now.\p"
	.string "I'll find her on my chart one\n"
	.string "day. I promise.$"

QoL_Skyrift_Roof_Text_SiriusVow::
	.string "SIRIUS: You let her go.\p"
	.string "I will find a way through that\n"
	.string "rift. I will wait here, on her\l"
	.string "roof, for as long as it takes.$"

QoL_Skyrift_Roof_Text_Cosmog::
	.string "Something small drifts down from\n"
	.string "where the rift closed...\p"
	.string "It floats over to you.$"

QoL_Skyrift_Roof_Text_KeepALTAIR::
	.string "ALTAIR: So... the boss is sorted,\n"
	.string "and I've got nowhere to be.\p"
	.string "Mind if I keep tagging along?$"

QoL_Skyrift_Roof_Text_KeepYesALTAIR::
	.string "ALTAIR: Ha! Then let's go find\n"
	.string "something worth digging up.$"

QoL_Skyrift_Roof_Text_KeepNoALTAIR::
	.string "ALTAIR: Fair enough. I'll be at the\n"
	.string "RIFTWAKE INN if you need muscle.$"

QoL_Skyrift_Roof_Text_KeepDENEB::
	.string "DENEB: Well, darling. I'm out of a\n"
	.string "job now.\p"
	.string "May I keep traveling with you?$"

QoL_Skyrift_Roof_Text_KeepYesDENEB::
	.string "DENEB: Wonderful. I'll pack light.\n"
	.string "Three hats at most.$"

QoL_Skyrift_Roof_Text_KeepNoDENEB::
	.string "DENEB: Then I'll wait at the\n"
	.string "RIFTWAKE INN. Don't keep a lady\l"
	.string "waiting.$"

QoL_Skyrift_Roof_Text_KeepMIRA::
	.string "MIRA: Grandpa can wait a little\n"
	.string "longer, right?\p"
	.string "Can I keep traveling with you?$"

QoL_Skyrift_Roof_Text_KeepYesMIRA::
	.string "MIRA: Yay! I'll read the stars, you\n"
	.string "do the battling!$"

QoL_Skyrift_Roof_Text_KeepNoMIRA::
	.string "MIRA: Okay... I'll be at the\n"
	.string "RIFTWAKE INN. Come get me anytime!$"

QoL_Skyrift_Roof_Text_TheEnd::
	.string "The SKYRIFT is closed.\n"
	.string "The STARFALL is truly over.\p"
	.string "STARFALL - THE END\p"
	.string "Thank you for playing!$"

QoL_Skyrift_Roof_Text_RematchVega::
	.string "VEGA: I come up here to watch the\n"
	.string "stars now. Just watch.\p"
	.string "Would you battle me? For old\n"
	.string "times' sake.$"

QoL_Skyrift_Roof_Text_RematchSirius::
	.string "SIRIUS: You again. Every night I\n"
	.string "wait for the rift to open.\p"
	.string "Battle me. It passes the time.$"
