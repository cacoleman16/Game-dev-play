Skyrift_RiftwakeHarbor_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Skyrift_RiftwakeHarbor_QoLFly_OnTransition
	.byte 0

Skyrift_RiftwakeHarbor_EventScript_TownSign::
	msgbox Skyrift_RiftwakeHarbor_Text_TownSign, MSGBOX_SIGN
	end

Skyrift_RiftwakeHarbor_Text_TownSign::
	.string "RIFTWAKE HARBOR\n"
	.string "The last port under the torn sky$"

Skyrift_RiftwakeHarbor_EventScript_HutSign::
	msgbox Skyrift_RiftwakeHarbor_Text_HutSign, MSGBOX_SIGN
	end

Skyrift_RiftwakeHarbor_Text_HutSign::
	.string "DR. HALLEY'S RIFT HUT\p"
	.string "Knock first. If the door hums,\n"
	.string "knock twice.$"

Skyrift_RiftwakeHarbor_EventScript_InnSign::
	msgbox Skyrift_RiftwakeHarbor_Text_InnSign, MSGBOX_SIGN
	end

Skyrift_RiftwakeHarbor_Text_InnSign::
	.string "RIFTWAKE INN\n"
	.string "Rest under a quiet roof.$"

Skyrift_RiftwakeHarbor_EventScript_OldMan::
	msgbox Skyrift_RiftwakeHarbor_Text_OldMan, MSGBOX_NPC
	end

Skyrift_RiftwakeHarbor_Text_OldMan::
	.string "See that crack in the sky, over\n"
	.string "the hills to the east?\p"
	.string "It opened the night the WISHING\n"
	.string "STAR left SEIKA. Ripped open like\l"
	.string "an old sail.\p"
	.string "Sixty years I've fished here.\n"
	.string "Never seen the stars bleed before.$"

Skyrift_RiftwakeHarbor_EventScript_Woman::
	msgbox Skyrift_RiftwakeHarbor_Text_Woman, MSGBOX_NPC
	end

Skyrift_RiftwakeHarbor_Text_Woman::
	.string "At night the rift glows violet.\n"
	.string "Nobody in town can sleep.\p"
	.string "The inn is full of folk who came\n"
	.string "just to watch it.$"

Skyrift_RiftwakeHarbor_EventScript_Boy::
	msgbox Skyrift_RiftwakeHarbor_Text_Boy, MSGBOX_NPC
	end

Skyrift_RiftwakeHarbor_Text_Boy::
	.string "Something with tentacles floated\n"
	.string "down onto SHARDFALL BEACH!\p"
	.string "It looked like a jellyfish made\n"
	.string "of glass. Mom says I dreamed it.$"

Skyrift_RiftwakeHarbor_EventScript_Fisher::
	msgbox Skyrift_RiftwakeHarbor_Text_Fisher, MSGBOX_NPC
	end

Skyrift_RiftwakeHarbor_Text_Fisher::
	.string "Since the sky tore, the sea\n"
	.string "glitters at night.\p"
	.string "Little shards keep falling into\n"
	.string "the water. STARYU love them!$"

Skyrift_RiftwakeHarbor_EventScript_Worker::
	msgbox Skyrift_RiftwakeHarbor_Text_Worker, MSGBOX_NPC
	end

Skyrift_RiftwakeHarbor_Text_Worker::
	.string "East of town is SHARDFALL BEACH.\p"
	.string "Past it, the crater where the big\n"
	.string "meteorite hit, and the cliffs up\l"
	.string "to that OBSERVATORY.\p"
	.string "The people in the strange coats\n"
	.string "came off a private boat. They\l"
	.string "went up there and never left.$"

Skyrift_EventScript_HarborSailor::
	lock
	faceplayer
	message QoL_Skyrift_Ferry_Text_Where
	waitmessage
	dynmultipush QoL_Skyrift_Ferry_Text_Lanternport, 0
	dynmultipush QoL_Text_DestCancel, 1
	dynmultipush QoL_Skyrift_Ferry_Text_Vermilion, 2
	dynmultistack 0, 0, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, QoL_Skyrift_Ferry_ToLanternport
	case 2, QoL_EventScript_SailToKanto
	msgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Ferry_ToLanternport::
	call QoL_EventScript_SetSail
	warp MAP_SEIKA_LANTERNPORT, 20, 28
	waitstate
	release
	end

QoL_EventScript_SailToSkyrift::
	call QoL_EventScript_SetSail
	warp MAP_SKYRIFT_RIFTWAKE_HARBOR, 18, 24
	waitstate
	release
	end

QoL_EventScript_LanternportSailor::
	lock
	faceplayer
	goto_if_unset FLAG_QOL_SKYRIFT_CHOSEN, QoL_Skyrift_Ferry_LanternportKanto
	message QoL_Skyrift_Ferry_Text_Where
	waitmessage
	dynmultipush QoL_Skyrift_Ferry_Text_Skyrift, 0
	dynmultipush QoL_Skyrift_Ferry_Text_Vermilion, 1
	dynmultipush QoL_Text_DestCancel, 2
	dynmultistack 0, 0, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, QoL_EventScript_SailToSkyrift
	case 1, QoL_EventScript_SailToKanto
	msgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Ferry_LanternportKanto::
	msgbox QoL_Text_OfferKanto, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, QoL_EventScript_SailToKanto
	msgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT
	release
	end

QoL_EventScript_PushSkyrift::
	goto_if_unset FLAG_QOL_SKYRIFT_CHOSEN, Common_EventScript_NopReturn
	dynmultipush QoL_Skyrift_Ferry_Text_Skyrift, 7
	return

QoL_Skyrift_Ferry_Text_Where::
	.string "Where would you like to sail?$"

QoL_Skyrift_Ferry_Text_Lanternport::
	.string "LANTERNPORT$"

QoL_Skyrift_Ferry_Text_Vermilion::
	.string "VERMILION CITY$"

QoL_Skyrift_Ferry_Text_Skyrift::
	.string "SKYRIFT ISLES$"

Skyrift_RiftwakeHarbor_QoLFly_OnTransition::
	setflag FLAG_QOL_SKYRIFT_FLY_RIFTWAKE
	end
