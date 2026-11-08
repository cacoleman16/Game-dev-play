Seika_Tidewatch_Gym_MapScripts::
	.byte 0

Seika_Tidewatch_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_QOL_SEIKA_BADGE05, Seika_Tidewatch_Gym_EventScript_GymStatueWon
	msgbox Seika_Tidewatch_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

Seika_Tidewatch_Gym_EventScript_GymStatueWon::
	msgbox Seika_Tidewatch_Gym_Text_GymStatueWon, MSGBOX_DEFAULT
	releaseall
	end

Seika_Tidewatch_Gym_Text_GymStatue::
	.string "TIDEWATCH POKéMON GYM\n"
	.string "LEADER: NAGI\p"
	.string "WINNING TRAINERS:\n"
	.string "(none yet)$"
	.string "{JPN}{JPN}シオミ　ポケモンジム　リーダー　ナギサ\p"
	.string "しょうりした　トレーナー:　まだ　いない$"

Seika_Tidewatch_Gym_Text_GymStatueWon::
	.string "TIDEWATCH POKéMON GYM\n"
	.string "LEADER: NAGI\p"
	.string "WINNING TRAINERS:\n"
	.string "{PLAYER}$"
	.string "{JPN}{JPN}シオミ　ポケモンジム　リーダー　ナギサ\p"
	.string "しょうりした　トレーナー:　{PLAYER}$"

Seika_Tidewatch_Gym_EventScript_Mina::
	trainerbattle_single TRAINER_SEIKA_SWIMMER_F_MINA, Seika_Tidewatch_Gym_Text_MinaIntro, Seika_Tidewatch_Gym_Text_MinaDefeat
	msgbox Seika_Tidewatch_Gym_Text_MinaPost, MSGBOX_AUTOCLOSE
	end

Seika_Tidewatch_Gym_Text_MinaIntro::
	.string "Splash! You walked right past my\n"
	.string "lane. Time to swim for it!$"
	.string "{JPN}{JPN}バシャーン！　わたしの　コースを　とおったわね。\n"
	.string "およいで　しょうぶよ！$"

Seika_Tidewatch_Gym_Text_MinaDefeat::
	.string "I sank like a stone...$"
	.string "{JPN}{JPN}いしみたいに　しずんじゃった‥$"

Seika_Tidewatch_Gym_Text_MinaPost::
	.string "NAGI dives deeper than anyone in\n"
	.string "SEIKA. She says it's very quiet.$"
	.string "{JPN}{JPN}ナギサさまは　セイカの　だれよりも　ふかく\n"
	.string "もぐれるの。　とても　しずかなんだって。$"

Seika_Tidewatch_Gym_EventScript_Kaito::
	trainerbattle_single TRAINER_SEIKA_SWIMMER_M_KAITO, Seika_Tidewatch_Gym_Text_KaitoIntro, Seika_Tidewatch_Gym_Text_KaitoDefeat
	msgbox Seika_Tidewatch_Gym_Text_KaitoPost, MSGBOX_AUTOCLOSE
	end

Seika_Tidewatch_Gym_Text_KaitoIntro::
	.string "The tide is with me today!$"
	.string "{JPN}{JPN}きょうの　しおは　おれの　みかただ！$"

Seika_Tidewatch_Gym_Text_KaitoDefeat::
	.string "The tide went out...$"
	.string "{JPN}{JPN}しおが　ひいちまった‥$"

Seika_Tidewatch_Gym_Text_KaitoPost::
	.string "NAGI's KINGDRA calls the rain.\n"
	.string "Then it gets really fast!$"
	.string "{JPN}{JPN}ナギサさまの　キングドラは　あめを　よぶんだ。\n"
	.string "そうすると　すごく　はやく　なるんだぜ！$"

Seika_Tidewatch_Gym_EventScript_Sora::
	trainerbattle_single TRAINER_SEIKA_SWIMMER_M_SORA, Seika_Tidewatch_Gym_Text_SoraIntro, Seika_Tidewatch_Gym_Text_SoraDefeat
	msgbox Seika_Tidewatch_Gym_Text_SoraPost, MSGBOX_AUTOCLOSE
	end

Seika_Tidewatch_Gym_Text_SoraIntro::
	.string "No one gets to NAGI without\n"
	.string "getting wet first!$"
	.string "{JPN}{JPN}ずぶぬれに　ならずに　ナギサさまの　ところへは\n"
	.string "いけないぜ！$"

Seika_Tidewatch_Gym_Text_SoraDefeat::
	.string "Soaked through!$"
	.string "{JPN}{JPN}びしょぬれだ！$"

Seika_Tidewatch_Gym_Text_SoraPost::
	.string "Electric and Grass moves work\n"
	.string "well here. Just saying.$"
	.string "{JPN}{JPN}ここでは　でんきと　くさの　わざが　よく\n"
	.string "きくぜ。　ひとりごと　だけどな。$"

Seika_Tidewatch_Gym_EventScript_Nagi::
	trainerbattle_single TRAINER_SEIKA_LEADER_NAGI, Seika_Tidewatch_Gym_Text_NagiIntro, Seika_Tidewatch_Gym_Text_NagiDefeat, Seika_Tidewatch_Gym_EventScript_NagiDefeated, NO_MUSIC
	msgbox Seika_Tidewatch_Gym_Text_NagiPost, MSGBOX_DEFAULT
	release
	end

Seika_Tidewatch_Gym_EventScript_NagiDefeated::
	setflag FLAG_QOL_SEIKA_BADGE05
	msgbox Seika_Tidewatch_Gym_Text_NagiGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_RAIN_DANCE
	msgbox Seika_Tidewatch_Gym_Text_NagiPost, MSGBOX_DEFAULT
	release
	end

Seika_Tidewatch_Gym_Text_NagiIntro::
	.string "Hello there. I am NAGI. I keep the\n"
	.string "TIDE SHRINE, and I dive.\p"
	.string "Down deep, the sea is very calm.\n"
	.string "You can hear your own heart.\p"
	.string "Let's see if yours stays calm\n"
	.string "when the tide comes in!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}ナギサ『こんにちは。　わたしは　ナギサ。\n"
	.string "しおのやしろの　もりびとで　ダイバーよ。\p"
	.string "うみの　ふかくは　とても　しずか。　じぶんの\n"
	.string "むねの　おとが　きこえるのよ。\p"
	.string "しおが　みちて　きても　あなたの　こころは\n"
	.string "しずかで　いられるかしら？』{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

Seika_Tidewatch_Gym_Text_NagiDefeat::
	.string "Whew... You rode the wave all the\n"
	.string "way in.\p"
	.string "The TIDE BADGE is yours. The sea\n"
	.string "will remember you.\p"
	.string "{FONT_NORMAL}{PLAYER} received the TIDE BADGE\n"
	.string "from NAGI!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"
	.string "{JPN}{JPN}ふう‥　なみに　のって　いっきに　ここまで\n"
	.string "きたわね。\p"
	.string "シオバッジは　あなたの　ものよ。　うみは\n"
	.string "あなたを　わすれないわ。\p"
	.string "{FONT_NORMAL}{PLAYER}は　ナギサから　シオバッジを\n"
	.string "もらった！{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_Tidewatch_Gym_Text_NagiGift::
	.string "Take this TM, too. RAIN DANCE\n"
	.string "brings the sea down from the sky.$"
	.string "{JPN}{JPN}ナギサ『この　わざマシンも　どうぞ。　あまごいは\n"
	.string "そらから　うみを　ふらせる　わざよ。』$"

Seika_Tidewatch_Gym_Text_NagiPost::
	.string "Go north from TIDEWATCH, through\n"
	.string "HOLLOW PATH, to SILKHOLLOW TOWN.\p"
	.string "AYAME, the weaver, keeps the\n"
	.string "shrine there.\p"
	.string "The stones under the bay all\n"
	.string "point to MT. SKYREACH. Hurry.$"
	.string "{JPN}{JPN}ナギサ『シオミタウンから　きたへ　ウツロのみちを\n"
	.string "ぬければ　キヌウツロタウンよ。\p"
	.string "そこでは　はたおりの　アヤメさんが　やしろを\n"
	.string "まもって　いるわ。\p"
	.string "いりえの　そこの　いしは　みんな\n"
	.string "ソラトドキやまを　さして　いるの。　いそいで。』$"

Seika_Tidewatch_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE05, Seika_Tidewatch_Gym_EventScript_GymGuyWon
	msgbox Seika_Tidewatch_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_Tidewatch_Gym_EventScript_GymGuyWon::
	msgbox Seika_Tidewatch_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_Tidewatch_Gym_Text_GymGuy::
	.string "Yo! Welcome to the TIDEWATCH GYM!\p"
	.string "NAGI uses WATER types. ELECTRIC\n"
	.string "and GRASS moves are your friends.\p"
	.string "Careful: her KINGDRA brings rain\n"
	.string "and gets super fast in it!$"
	.string "{JPN}{JPN}おーす！　シオミジムへ　ようこそ！\p"
	.string "ナギサは　みずタイプの　つかいてだ。　でんきと\n"
	.string "くさの　わざが　みかたに　なるぞ。\p"
	.string "ちゅういしろよ。　キングドラは　あめを　ふらせて\n"
	.string "すごく　はやく　なるからな！$"

Seika_Tidewatch_Gym_Text_GymGuyWon::
	.string "You made a splash! Five BADGES\n"
	.string "now. Way to go!$"
	.string "{JPN}{JPN}でっかい　しぶきを　あげたな！　これで　バッジ\n"
	.string "5つ。　その　ちょうしだ！$"
