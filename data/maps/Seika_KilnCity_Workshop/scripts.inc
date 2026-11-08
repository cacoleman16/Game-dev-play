Seika_KilnCity_Workshop_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_KilnCity_Workshop_OnTransition
	.byte 0

Seika_KilnCity_Workshop_EventScript_Apprentice::
	msgbox Seika_KilnCity_Workshop_Text_Apprentice, MSGBOX_NPC
	end

Seika_KilnCity_Workshop_Text_Apprentice::
	.string "We fire the glaze three times.\n"
	.string "Once for the clay, once for the\l"
	.string "color, once for luck.$"

Seika_KilnCity_Workshop_EventScript_DisplayBowls::
	msgbox Seika_KilnCity_Workshop_Text_DisplayBowls, MSGBOX_SIGN
	end

Seika_KilnCity_Workshop_EventScript_DisplayCase::
	lockall
	goto_if_set FLAG_QOL_SEIKA_DENEB_KILN, Seika_KilnCity_Workshop_EventScript_DisplayEmpty
	msgbox Seika_KilnCity_Workshop_Text_DisplayBell, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Workshop_EventScript_DisplayEmpty::
	msgbox Seika_KilnCity_Workshop_Text_DisplayEmpty, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Workshop_Text_DisplayBowls::
	.string "Tea bowls glazed the color of the\n"
	.string "night sky.\p"
	.string "A tag on them says: NOT FOR SALE.$"

Seika_KilnCity_Workshop_Text_DisplayBell::
	.string "A small bell of clear, glittering\n"
	.string "glass. It hums when you lean close.$"

Seika_KilnCity_Workshop_Text_DisplayEmpty::
	.string "The case is empty. A ring in the\n"
	.string "dust shows where a bell once sat.$"

Seika_KilnCity_Workshop_OnTransition::
	call_if_set FLAG_QOL_SEIKA_DENEB_KILN, Seika_KilnCity_Workshop_EventScript_SceneDone
	end

Seika_KilnCity_Workshop_EventScript_SceneDone::
	setvar VAR_TEMP_1, 1
	return

Seika_KilnCity_Workshop_EventScript_DenebScene::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox Seika_KilnCity_Workshop_Text_DenebBuys, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Seika_KilnCity_Workshop_Text_MasterWorried, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_SEIKA_KILN_CITY_WORKSHOP_DENEB, Common_Movement_FaceDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_SEIKA_KILN_CITY_WORKSHOP_DENEB, Common_Movement_ExclamationMark
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox Seika_KilnCity_Workshop_Text_DenebIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_DENEB_KILN, Seika_KilnCity_Workshop_Text_DenebDefeat
	msgbox Seika_KilnCity_Workshop_Text_DenebAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_KILN_CITY_WORKSHOP_DENEB
	setflag FLAG_QOL_SEIKA_HIDE_DENEB_KILN
	setflag FLAG_QOL_SEIKA_DENEB_KILN
	setvar VAR_TEMP_1, 1
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Seika_KilnCity_Workshop_Text_MasterAfter, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Workshop_EventScript_Deneb::
	lock
	faceplayer
	msgbox Seika_KilnCity_Workshop_Text_DenebBuys, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Workshop_EventScript_Master::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_DENEB_KILN, Seika_KilnCity_Workshop_EventScript_MasterLater
	msgbox Seika_KilnCity_Workshop_Text_MasterWorried, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Workshop_EventScript_MasterLater::
	msgbox Seika_KilnCity_Workshop_Text_MasterLater, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Workshop_Text_DenebBuys::
	.string "DENEB: Then it is settled.\p"
	.string "The STARGLASS BELL belongs to\n"
	.string "TEAM NOVA now. The gold is on\l"
	.string "your table, master.$"

Seika_KilnCity_Workshop_Text_MasterWorried::
	.string "MASTER: But that bell was never\n"
	.string "meant to be sold...\p"
	.string "Our first masters fired it to\n"
	.string "answer the star when it fell.$"

Seika_KilnCity_Workshop_Text_DenebIntro::
	.string "Oh? {PLAYER}. Of course it is you.\p"
	.string "You followed us from KANTO all\n"
	.string "the way to SEIKA. How loyal.\p"
	.string "The old songs say this bell can\n"
	.string "wake the WISHING STAR.\p"
	.string "We paid a fair price for it.\n"
	.string "But if you insist on a quarrel...$"

Seika_KilnCity_Workshop_Text_DenebDefeat::
	.string "Hmph. You fight as if the stars\n"
	.string "themselves were on your side.$"

Seika_KilnCity_Workshop_Text_DenebAfter::
	.string "It changes nothing, {PLAYER}.\p"
	.string "The bell is already on its way\n"
	.string "to the mountain.\p"
	.string "VEGA will have her wish.\n"
	.string "Good day.$"

Seika_KilnCity_Workshop_Text_MasterAfter::
	.string "MASTER: She left with her people.\n"
	.string "Gold on my table, and the bell\l"
	.string "gone from its case.\p"
	.string "If they ring it on MT. SKYREACH,\n"
	.string "the star will answer them...\p"
	.string "Please, young one. Do what you\n"
	.string "can. Win the BADGES and follow\l"
	.string "her to the mountain.$"

Seika_KilnCity_Workshop_Text_MasterLater::
	.string "MASTER: The STARGLASS BELL is\n"
	.string "gone. Our oldest treasure...\p"
	.string "Go on, young one. The road to\n"
	.string "MT. SKYREACH is long.$"
