Seika_Frostmere_Gym_MapScripts::
	.byte 0

Seika_Frostmere_Gym_EventScript_Shiro::
	trainerbattle_single TRAINER_SEIKA_FISHERMAN_SHIRO, Seika_Frostmere_Gym_Text_ShiroIntro, Seika_Frostmere_Gym_Text_ShiroDefeat
	msgbox Seika_Frostmere_Gym_Text_ShiroPost, MSGBOX_AUTOCLOSE
	end

Seika_Frostmere_Gym_Text_ShiroIntro::
	.string "Shh! You'll scare the fish...\n"
	.string "Too late! Battle me instead!$"

Seika_Frostmere_Gym_Text_ShiroDefeat::
	.string "The big one got away...$"

Seika_Frostmere_Gym_Text_ShiroPost::
	.string "On ice, you slide until something\n"
	.string "stops you. Use the rocks!$"

Seika_Frostmere_Gym_EventScript_Mizore::
	trainerbattle_single TRAINER_SEIKA_COOLTRAINER_MIZORE, Seika_Frostmere_Gym_Text_MizoreIntro, Seika_Frostmere_Gym_Text_MizoreDefeat
	msgbox Seika_Frostmere_Gym_Text_MizorePost, MSGBOX_AUTOCLOSE
	end

Seika_Frostmere_Gym_Text_MizoreIntro::
	.string "You made it across the ice?\n"
	.string "Then let's see you handle sleet!$"

Seika_Frostmere_Gym_Text_MizoreDefeat::
	.string "I melted...$"

Seika_Frostmere_Gym_Text_MizorePost::
	.string "RIME has fished this lake for\n"
	.string "forty winters. He never gives up.$"

Seika_Frostmere_Gym_EventScript_Touma::
	trainerbattle_single TRAINER_SEIKA_EXPERT_TOUMA, Seika_Frostmere_Gym_Text_ToumaIntro, Seika_Frostmere_Gym_Text_ToumaDefeat
	msgbox Seika_Frostmere_Gym_Text_ToumaPost, MSGBOX_AUTOCLOSE
	end

Seika_Frostmere_Gym_Text_ToumaIntro::
	.string "A cold mind is a clear mind.\n"
	.string "Show me yours!$"

Seika_Frostmere_Gym_Text_ToumaDefeat::
	.string "Hm. Clear as ice.$"

Seika_Frostmere_Gym_Text_ToumaPost::
	.string "FIRE melts ice, but RIME's\n"
	.string "POKéMON can take the heat.$"

Seika_Frostmere_Gym_EventScript_Rime::
	trainerbattle_single TRAINER_SEIKA_LEADER_RIME, Seika_Frostmere_Gym_Text_RimeIntro, Seika_Frostmere_Gym_Text_RimeDefeat, Seika_Frostmere_Gym_EventScript_RimeDefeated, NO_MUSIC
	msgbox Seika_Frostmere_Gym_Text_RimePost, MSGBOX_DEFAULT
	release
	end

Seika_Frostmere_Gym_EventScript_RimeDefeated::
	setflag FLAG_QOL_SEIKA_BADGE07
	msgbox Seika_Frostmere_Gym_Text_RimeGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_ICE_BEAM
	msgbox Seika_Frostmere_Gym_Text_RimePost, MSGBOX_DEFAULT
	release
	end

Seika_Frostmere_Gym_Text_RimeIntro::
	.string "...Shh. I was watching the ice.\n"
	.string "You need patience to catch\l"
	.string "anything up here.\p"
	.string "I am RIME. I have fished this\n"
	.string "lake through forty winters.\p"
	.string "Let us see if your fire can last\n"
	.string "through my winter!$"

Seika_Frostmere_Gym_Text_RimeDefeat::
	.string "The ice cracks... and spring\n"
	.string "comes after all.\p"
	.string "The FROST BADGE is yours. Wear it\n"
	.string "with a warm heart.\p"
	.string "{FONT_NORMAL}{PLAYER} received the FROST BADGE\n"
	.string "from RIME!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_Frostmere_Gym_Text_RimeGift::
	.string "Take this too. The coldest beam\n"
	.string "of ice I know.$"

Seika_Frostmere_Gym_Text_RimePost::
	.string "Go north along FROST ROAD to\n"
	.string "DUSKPEAK VILLAGE.\p"
	.string "TOMOE keeps the last shrine,\n"
	.string "below MT. SKYREACH.\p"
	.string "Mind the snow, and keep your\n"
	.string "POKéMON warm.$"

Seika_Frostmere_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE07, Seika_Frostmere_Gym_EventScript_GymGuyWon
	msgbox Seika_Frostmere_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_Frostmere_Gym_EventScript_GymGuyWon::
	msgbox Seika_Frostmere_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_Frostmere_Gym_Text_GymGuy::
	.string "Brr! RIME's ICE POKéMON are\n"
	.string "tough as glaciers.\p"
	.string "FIRE, FIGHTING, ROCK and STEEL\n"
	.string "moves crack them open!\p"
	.string "And watch your step. On the ice\n"
	.string "you slide until something stops\l"
	.string "you!$"

Seika_Frostmere_Gym_Text_GymGuyWon::
	.string "Seven BADGES! You're hotter than\n"
	.string "the ice is cold!$"
