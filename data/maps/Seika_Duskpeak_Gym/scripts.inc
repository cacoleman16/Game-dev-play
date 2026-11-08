Seika_Duskpeak_Gym_MapScripts::
	.byte 0

Seika_Duskpeak_Gym_EventScript_Sayo::
	trainerbattle_single TRAINER_SEIKA_CHANNELER_SAYO, Seika_Duskpeak_Gym_Text_SayoIntro, Seika_Duskpeak_Gym_Text_SayoDefeat
	msgbox Seika_Duskpeak_Gym_Text_SayoPost, MSGBOX_AUTOCLOSE
	end

Seika_Duskpeak_Gym_Text_SayoIntro::
	.string "The stones are listening...\n"
	.string "And they do not like you!$"

Seika_Duskpeak_Gym_Text_SayoDefeat::
	.string "The spirits... went quiet.$"

Seika_Duskpeak_Gym_Text_SayoPost::
	.string "Each stone here is for a keeper\n"
	.string "who watched for the star.$"

Seika_Duskpeak_Gym_EventScript_Miyako::
	trainerbattle_single TRAINER_SEIKA_HEX_MANIAC_MIYAKO, Seika_Duskpeak_Gym_Text_MiyakoIntro, Seika_Duskpeak_Gym_Text_MiyakoDefeat
	msgbox Seika_Duskpeak_Gym_Text_MiyakoPost, MSGBOX_AUTOCLOSE
	end

Seika_Duskpeak_Gym_Text_MiyakoIntro::
	.string "Fufufu... A curse on your\n"
	.string "next turn!$"

Seika_Duskpeak_Gym_Text_MiyakoDefeat::
	.string "My curse came back to me...$"

Seika_Duskpeak_Gym_Text_MiyakoPost::
	.string "TOMOE never curses anyone.\n"
	.string "She just listens. It is scarier.$"

Seika_Duskpeak_Gym_EventScript_Kikue::
	trainerbattle_single TRAINER_SEIKA_CHANNELER_KIKUE, Seika_Duskpeak_Gym_Text_KikueIntro, Seika_Duskpeak_Gym_Text_KikueDefeat
	msgbox Seika_Duskpeak_Gym_Text_KikuePost, MSGBOX_AUTOCLOSE
	end

Seika_Duskpeak_Gym_Text_KikueIntro::
	.string "Step into the light of the circle,\n"
	.string "if you can reach it!$"

Seika_Duskpeak_Gym_Text_KikueDefeat::
	.string "The candle went out...$"

Seika_Duskpeak_Gym_Text_KikuePost::
	.string "TOMOE's MISMAGIUS sings before\n"
	.string "it strikes. Do not listen!$"

Seika_Duskpeak_Gym_EventScript_Tomoe::
	trainerbattle_single TRAINER_SEIKA_LEADER_TOMOE, Seika_Duskpeak_Gym_Text_TomoeIntro, Seika_Duskpeak_Gym_Text_TomoeDefeat, Seika_Duskpeak_Gym_EventScript_TomoeDefeated, NO_MUSIC
	msgbox Seika_Duskpeak_Gym_Text_TomoePost, MSGBOX_DEFAULT
	release
	end

Seika_Duskpeak_Gym_EventScript_TomoeDefeated::
	setflag FLAG_QOL_SEIKA_BADGE08
	msgbox Seika_Duskpeak_Gym_Text_TomoeGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_SHADOW_BALL
	msgbox Seika_Duskpeak_Gym_Text_TomoePost, MSGBOX_DEFAULT
	release
	end

Seika_Duskpeak_Gym_Text_TomoeIntro::
	.string "Welcome. I am TOMOE. I keep the\n"
	.string "shrine below MT. SKYREACH.\p"
	.string "The spirits of the old keepers\n"
	.string "speak to me. They have watched\l"
	.string "you since LANTERNPORT.\p"
	.string "They wish to know if your heart\n"
	.string "is ready for the peak.\p"
	.string "Show them!$"

Seika_Duskpeak_Gym_Text_TomoeDefeat::
	.string "The spirits are smiling...\n"
	.string "They are proud of you.\p"
	.string "Take the DUSK BADGE, the last\n"
	.string "of the eight.\p"
	.string "{FONT_NORMAL}{PLAYER} received the DUSK BADGE\n"
	.string "from TOMOE!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_Duskpeak_Gym_Text_TomoeGift::
	.string "Take this, too. Let the dark\n"
	.string "carry your strength.$"

Seika_Duskpeak_Gym_Text_TomoePost::
	.string "Listen well. The WISHING STAR's\n"
	.string "seven days are nearly over.\p"
	.string "Team NOVA carried the STARGLASS\n"
	.string "BELL up to the SKY SHRINE.\p"
	.string "But the shrine path opens only\n"
	.string "for the CHAMPION of the SEIKA\l"
	.string "LEAGUE.\p"
	.string "Cross SKYREACH PASS to STARWATCH\n"
	.string "PLATEAU, and win. Hurry!$"

Seika_Duskpeak_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE08, Seika_Duskpeak_Gym_EventScript_GymGuyWon
	msgbox Seika_Duskpeak_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_Duskpeak_Gym_EventScript_GymGuyWon::
	msgbox Seika_Duskpeak_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_Duskpeak_Gym_Text_GymGuy::
	.string "Yo! This is the last GYM in SEIKA,\n"
	.string "and the darkest!\p"
	.string "TOMOE's GHOST POKéMON laugh at\n"
	.string "NORMAL and FIGHTING moves.\p"
	.string "Hit them with DARK and GHOST\n"
	.string "moves instead.\p"
	.string "And her SPIRITOMB? Only FAIRY\n"
	.string "moves really hurt it!$"

Seika_Duskpeak_Gym_Text_GymGuyWon::
	.string "You did it! All eight SEIKA\n"
	.string "BADGES!\p"
	.string "SKYREACH PASS is open to you\n"
	.string "now. Go, champ!$"
