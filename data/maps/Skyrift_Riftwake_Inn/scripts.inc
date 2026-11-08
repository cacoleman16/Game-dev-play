Skyrift_Riftwake_Inn_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, QoL_Skyrift_Inn_OnTransition
	.byte 0

Skyrift_Riftwake_Inn_EventScript_Ledger::
	msgbox Skyrift_Riftwake_Inn_Text_Ledger, MSGBOX_SIGN
	end

Skyrift_Riftwake_Inn_Text_Ledger::
	.string "The guest book. Every name in\n"
	.string "it wrote the same thing:\p"
	.string "“Came to see the sky.”$"

Skyrift_Riftwake_Inn_EventScript_Guest::
	msgbox Skyrift_Riftwake_Inn_Text_Guest, MSGBOX_NPC
	end

Skyrift_Riftwake_Inn_Text_Guest::
	.string "I sailed out from VERMILION just\n"
	.string "to watch the rift at night.\p"
	.string "It's beautiful. And a little bit\n"
	.string "frightening.$"

QoL_Skyrift_Inn_Text_JoinedALTAIR::
	.string "ALTAIR: Back on the road? Fine by\n"
	.string "me.$"

QoL_Skyrift_Inn_Text_JoinedDENEB::
	.string "DENEB: Shall we, darling?$"

QoL_Skyrift_Inn_Text_JoinedMIRA::
	.string "MIRA: Yay! Where are we going next?$"

QoL_Skyrift_EventScript_Inn::
	lock
	faceplayer
	msgbox QoL_Skyrift_Inn_Text_Welcome, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Inn_Companion
	msgbox QoL_Skyrift_Inn_Text_Rest, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	fadescreen FADE_FROM_BLACK
	msgbox QoL_Skyrift_Inn_Text_Morning, MSGBOX_DEFAULT
QoL_Skyrift_Inn_Companion::
	goto_if_unset FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Inn_Bye
	msgbox QoL_Skyrift_Inn_Text_Lounge, MSGBOX_DEFAULT
	message QoL_Skyrift_Inn_Text_Who
	waitmessage
	dynmultipush QoL_Skyrift_Pier_Text_OptAltair, 0
	dynmultipush QoL_Skyrift_Pier_Text_OptDeneb, 1
	dynmultipush QoL_Skyrift_Pier_Text_OptMira, 2
	dynmultipush QoL_Skyrift_Inn_Text_OptAlone, 3
	dynmultipush QoL_Skyrift_Inn_Text_OptKeep, 4
	dynmultistack 0, 0, FALSE, 5, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, QoL_Skyrift_Inn_SetALTAIR
	case 1, QoL_Skyrift_Inn_SetDENEB
	case 2, QoL_Skyrift_Inn_SetMIRA
	case 3, QoL_Skyrift_Inn_Alone
QoL_Skyrift_Inn_Bye::
	msgbox QoL_Skyrift_Inn_Text_Bye, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_Clear::
	checkfollowernpc
	goto_if_eq VAR_RESULT, FALSE, QoL_Skyrift_Inn_ClearFlags
	destroyfollowernpc
QoL_Skyrift_Inn_ClearFlags::
	clearflag FLAG_QOL_SKYRIFT_WITH_ALTAIR
	clearflag FLAG_QOL_SKYRIFT_WITH_DENEB
	clearflag FLAG_QOL_SKYRIFT_WITH_MIRA
	return

QoL_Skyrift_Inn_Alone::
	call QoL_Skyrift_Inn_Clear
	call QoL_Skyrift_Inn_Lobby
	call QoL_Skyrift_Inn_ShowALTAIR
	call QoL_Skyrift_Inn_ShowDENEB
	call QoL_Skyrift_Inn_ShowMIRA
	msgbox QoL_Skyrift_Inn_Text_AloneDone, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_SetALTAIR::
	call QoL_Skyrift_Inn_Clear
	setflag FLAG_QOL_SKYRIFT_WITH_ALTAIR
	call QoL_Skyrift_Inn_Lobby
	removeobject LOCALID_SKYRIFT_INN_ALTAIR
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_DENEB, QoL_Skyrift_Inn_ShowDENEB
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_MIRA, QoL_Skyrift_Inn_ShowMIRA
	createfollowernpc OBJ_EVENT_GFX_HIKER_FRLG, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_ALTAIR
	msgbox QoL_Skyrift_Inn_Text_JoinedALTAIR, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_SetDENEB::
	call QoL_Skyrift_Inn_Clear
	setflag FLAG_QOL_SKYRIFT_WITH_DENEB
	call QoL_Skyrift_Inn_Lobby
	removeobject LOCALID_SKYRIFT_INN_DENEB
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_ALTAIR, QoL_Skyrift_Inn_ShowALTAIR
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_MIRA, QoL_Skyrift_Inn_ShowMIRA
	createfollowernpc OBJ_EVENT_GFX_HEX_MANIAC, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_DENEB
	msgbox QoL_Skyrift_Inn_Text_JoinedDENEB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_SetMIRA::
	call QoL_Skyrift_Inn_Clear
	setflag FLAG_QOL_SKYRIFT_WITH_MIRA
	call QoL_Skyrift_Inn_Lobby
	removeobject LOCALID_SKYRIFT_INN_MIRA
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_ALTAIR, QoL_Skyrift_Inn_ShowALTAIR
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_DENEB, QoL_Skyrift_Inn_ShowDENEB
	createfollowernpc OBJ_EVENT_GFX_LASS_FRLG, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_MIRA
	msgbox QoL_Skyrift_Inn_Text_JoinedMIRA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_OnTransition::
	call QoL_Skyrift_Inn_Lobby
	end

QoL_Skyrift_Inn_Lobby::
	setflag FLAG_QOL_SKYRIFT_HIDE_INN_ALTAIR
	call_if_unset FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Inn_SeatALTAIR
	setflag FLAG_QOL_SKYRIFT_HIDE_INN_DENEB
	call_if_unset FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Inn_SeatDENEB
	setflag FLAG_QOL_SKYRIFT_HIDE_INN_MIRA
	call_if_unset FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Inn_SeatMIRA
	goto_if_set FLAG_QOL_SKYRIFT_CLIMAX, Common_EventScript_NopReturn
	setflag FLAG_QOL_SKYRIFT_HIDE_INN_ALTAIR
	setflag FLAG_QOL_SKYRIFT_HIDE_INN_DENEB
	setflag FLAG_QOL_SKYRIFT_HIDE_INN_MIRA
	return

QoL_Skyrift_Inn_SeatALTAIR::
	clearflag FLAG_QOL_SKYRIFT_HIDE_INN_ALTAIR
	return

QoL_Skyrift_Inn_ShowALTAIR::
	addobject LOCALID_SKYRIFT_INN_ALTAIR
	return

QoL_Skyrift_Inn_EventScript_LobbyALTAIR::
	lock
	faceplayer
	msgbox QoL_Skyrift_Inn_Text_LobbyALTAIR, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Inn_LobbyNoALTAIR
	call QoL_Skyrift_Inn_Clear
	setflag FLAG_QOL_SKYRIFT_WITH_ALTAIR
	removeobject LOCALID_SKYRIFT_INN_ALTAIR
	call QoL_Skyrift_Inn_Lobby
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_DENEB, QoL_Skyrift_Inn_ShowDENEB
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_MIRA, QoL_Skyrift_Inn_ShowMIRA
	createfollowernpc OBJ_EVENT_GFX_HIKER_FRLG, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_ALTAIR
	msgbox QoL_Skyrift_Inn_Text_JoinedALTAIR, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_LobbyNoALTAIR::
	msgbox QoL_Skyrift_Inn_Text_LobbyNoALTAIR, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_SeatDENEB::
	clearflag FLAG_QOL_SKYRIFT_HIDE_INN_DENEB
	return

QoL_Skyrift_Inn_ShowDENEB::
	addobject LOCALID_SKYRIFT_INN_DENEB
	return

QoL_Skyrift_Inn_EventScript_LobbyDENEB::
	lock
	faceplayer
	msgbox QoL_Skyrift_Inn_Text_LobbyDENEB, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Inn_LobbyNoDENEB
	call QoL_Skyrift_Inn_Clear
	setflag FLAG_QOL_SKYRIFT_WITH_DENEB
	removeobject LOCALID_SKYRIFT_INN_DENEB
	call QoL_Skyrift_Inn_Lobby
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_ALTAIR, QoL_Skyrift_Inn_ShowALTAIR
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_MIRA, QoL_Skyrift_Inn_ShowMIRA
	createfollowernpc OBJ_EVENT_GFX_HEX_MANIAC, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_DENEB
	msgbox QoL_Skyrift_Inn_Text_JoinedDENEB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_LobbyNoDENEB::
	msgbox QoL_Skyrift_Inn_Text_LobbyNoDENEB, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_SeatMIRA::
	clearflag FLAG_QOL_SKYRIFT_HIDE_INN_MIRA
	return

QoL_Skyrift_Inn_ShowMIRA::
	addobject LOCALID_SKYRIFT_INN_MIRA
	return

QoL_Skyrift_Inn_EventScript_LobbyMIRA::
	lock
	faceplayer
	msgbox QoL_Skyrift_Inn_Text_LobbyMIRA, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Inn_LobbyNoMIRA
	call QoL_Skyrift_Inn_Clear
	setflag FLAG_QOL_SKYRIFT_WITH_MIRA
	removeobject LOCALID_SKYRIFT_INN_MIRA
	call QoL_Skyrift_Inn_Lobby
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_ALTAIR, QoL_Skyrift_Inn_ShowALTAIR
	call_if_unset FLAG_QOL_SKYRIFT_HIDE_INN_DENEB, QoL_Skyrift_Inn_ShowDENEB
	createfollowernpc OBJ_EVENT_GFX_LASS_FRLG, 0x3E, QoL_EventScript_CompanionTalk, PARTNER_QOL_MIRA
	msgbox QoL_Skyrift_Inn_Text_JoinedMIRA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_LobbyNoMIRA::
	msgbox QoL_Skyrift_Inn_Text_LobbyNoMIRA, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Inn_Text_LobbyALTAIR::
	.string "ALTAIR: Need a strong back on the\n"
	.string "road? I'm ready when you are.$"

QoL_Skyrift_Inn_Text_LobbyNoALTAIR::
	.string "ALTAIR: Suit yourself. I'll be\n"
	.string "here.$"

QoL_Skyrift_Inn_Text_LobbyDENEB::
	.string "DENEB: Bored of the inn already,\n"
	.string "darling. Take me with you?$"

QoL_Skyrift_Inn_Text_LobbyNoDENEB::
	.string "DENEB: Pity. I'll order more tea.$"

QoL_Skyrift_Inn_Text_LobbyMIRA::
	.string "MIRA: The stars are clear tonight!\n"
	.string "Can I come with you?$"

QoL_Skyrift_Inn_Text_LobbyNoMIRA::
	.string "MIRA: Okay! I'll keep drawing my\n"
	.string "star chart.$"

QoL_Skyrift_Inn_Text_Welcome::
	.string "Welcome to the RIFTWAKE INN!\n"
	.string "Would you like to rest?$"

QoL_Skyrift_Inn_Text_Rest::
	.string "Sleep well!$"

QoL_Skyrift_Inn_Text_Morning::
	.string "Good morning! Your POKéMON look\n"
	.string "full of energy.$"

QoL_Skyrift_Inn_Text_Lounge::
	.string "Your friends from the\n"
	.string "OBSERVATORY are staying here.\p"
	.string "They'd each love to travel with\n"
	.string "you, wherever you go.$"

QoL_Skyrift_Inn_Text_Who::
	.string "Who should travel with you?$"

QoL_Skyrift_Inn_Text_OptAlone::
	.string "TRAVEL ALONE$"

QoL_Skyrift_Inn_Text_OptKeep::
	.string "NO CHANGE$"

QoL_Skyrift_Inn_Text_AloneDone::
	.string "Your companion waves goodbye.\n"
	.string "They'll be here at the inn.$"

QoL_Skyrift_Inn_Text_Bye::
	.string "Come back anytime!$"
