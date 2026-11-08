Seika_Lanternport_Gym_MapScripts::
	.byte 0

Seika_Lanternport_Gym_EventScript_Homura::
	trainerbattle_single TRAINER_SEIKA_KINDLER_HOMURA, Seika_Lanternport_Gym_Text_HomuraIntro, Seika_Lanternport_Gym_Text_HomuraDefeat
	msgbox Seika_Lanternport_Gym_Text_HomuraPost, MSGBOX_AUTOCLOSE
	end

Seika_Lanternport_Gym_Text_HomuraIntro::
	.string "WICK's fire burned for a thousand\n"
	.string "nights. Mine burns for him!$"

Seika_Lanternport_Gym_Text_HomuraDefeat::
	.string "My flame... flickered out.$"

Seika_Lanternport_Gym_Text_HomuraPost::
	.string "WICK lights every lantern in town\n"
	.string "with one breath. Really!$"

Seika_Lanternport_Gym_EventScript_Enka::
	trainerbattle_single TRAINER_SEIKA_KINDLER_ENKA, Seika_Lanternport_Gym_Text_EnkaIntro, Seika_Lanternport_Gym_Text_EnkaDefeat
	msgbox Seika_Lanternport_Gym_Text_EnkaPost, MSGBOX_AUTOCLOSE
	end

Seika_Lanternport_Gym_Text_EnkaIntro::
	.string "Too close to the fire? Too bad!$"

Seika_Lanternport_Gym_Text_EnkaDefeat::
	.string "Whoa, I got burned!$"

Seika_Lanternport_Gym_Text_EnkaPost::
	.string "WICK opens with dark fire. Watch\n"
	.string "out for his HOUNDOOM!$"

Seika_Lanternport_Gym_EventScript_Wick::
	trainerbattle_single TRAINER_SEIKA_LEADER_WICK, Seika_Lanternport_Gym_Text_WickIntro, Seika_Lanternport_Gym_Text_WickDefeat, Seika_Lanternport_Gym_EventScript_WickDefeated, NO_MUSIC
	msgbox Seika_Lanternport_Gym_Text_WickPost, MSGBOX_DEFAULT
	release
	end

Seika_Lanternport_Gym_EventScript_WickDefeated::
	setflag FLAG_QOL_SEIKA_BADGE01
	msgbox Seika_Lanternport_Gym_Text_WickGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_FLAMETHROWER
	msgbox Seika_Lanternport_Gym_Text_WickPost, MSGBOX_DEFAULT
	release
	end

Seika_Lanternport_Gym_Text_WickIntro::
	.string "So the traveler from KANTO has\n"
	.string "come to my shrine.\p"
	.string "I am WICK. I make the lanterns\n"
	.string "that guide the WISHING STAR home.\p"
	.string "A lantern that fears the wind is\n"
	.string "no lantern at all! Show me your\l"
	.string "fire!$"

Seika_Lanternport_Gym_Text_WickDefeat::
	.string "Ha! My flame bows to yours.\p"
	.string "Take the LANTERN BADGE. Let it\n"
	.string "light your way through SEIKA.\p"
	.string "{FONT_NORMAL}{PLAYER} received the LANTERN BADGE\n"
	.string "from WICK!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_Lanternport_Gym_Text_WickGift::
	.string "And take this, too. A flame that\n"
	.string "never goes out.$"

Seika_Lanternport_Gym_Text_WickPost::
	.string "Go north to INKBRUSH TOWN.\p"
	.string "SUMI keeps the STARFALL SCROLL\n"
	.string "there. If anyone knows where the\l"
	.string "WISHING STAR sleeps, she does.$"

Seika_Lanternport_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE01, Seika_Lanternport_Gym_EventScript_GymGuyWon
	msgbox Seika_Lanternport_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_Lanternport_Gym_EventScript_GymGuyWon::
	msgbox Seika_Lanternport_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_Lanternport_Gym_Text_GymGuy::
	.string "Yo! Welcome to SEIKA's first GYM!\p"
	.string "WICK burns hot. WATER, GROUND\n"
	.string "and ROCK moves cool him down.\p"
	.string "His TYPHLOSION can erupt at full\n"
	.string "health, so hit it hard and fast!$"

Seika_Lanternport_Gym_Text_GymGuyWon::
	.string "That was blazing! One BADGE down,\n"
	.string "seven to go!$"
