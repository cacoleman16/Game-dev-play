Seika_Mossgrove_Gym_MapScripts::
	.byte 0

Seika_Mossgrove_Gym_EventScript_Statue::
	lockall
	goto_if_set FLAG_QOL_SEIKA_BADGE03, Seika_Mossgrove_Gym_EventScript_StatueWon
	msgbox Seika_Mossgrove_Gym_Text_Statue, MSGBOX_DEFAULT
	releaseall
	end

Seika_Mossgrove_Gym_EventScript_StatueWon::
	msgbox Seika_Mossgrove_Gym_Text_StatueWon, MSGBOX_DEFAULT
	releaseall
	end

Seika_Mossgrove_Gym_Text_Statue::
	.string "MOSSGROVE POKéMON GYM\n"
	.string "LEADER: KAYA$"
	.string "{JPN}{JPN}コケムス　ポケモンジム　リーダー　カヤ$"

Seika_Mossgrove_Gym_Text_StatueWon::
	.string "MOSSGROVE POKéMON GYM\n"
	.string "LEADER: KAYA\p"
	.string "WINNING TRAINERS:\n"
	.string "{PLAYER}$"
	.string "{JPN}{JPN}コケムス　ポケモンジム　リーダー　カヤ\p"
	.string "しょうりした　トレーナー:　{PLAYER}$"

Seika_Mossgrove_Gym_EventScript_Tetsu::
	trainerbattle_single TRAINER_SEIKA_BUG_MANIAC_TETSU, Seika_Mossgrove_Gym_Text_TetsuIntro, Seika_Mossgrove_Gym_Text_TetsuDefeat
	msgbox Seika_Mossgrove_Gym_Text_TetsuPost, MSGBOX_AUTOCLOSE
	end

Seika_Mossgrove_Gym_Text_TetsuIntro::
	.string "Bzzz! You walked right into my\n"
	.string "web!$"
	.string "{JPN}{JPN}ブーン！　ぼくの　くものすに　まんまと\n"
	.string "かかったな！$"

Seika_Mossgrove_Gym_Text_TetsuDefeat::
	.string "My web... it tore!$"
	.string "{JPN}{JPN}くものすが‥　やぶれた！$"

Seika_Mossgrove_Gym_Text_TetsuPost::
	.string "KAYA hums to her bugs before a\n"
	.string "battle. It makes them brave!$"
	.string "{JPN}{JPN}カヤさんは　バトルの　まえに　むしたちに　うたを\n"
	.string "きかせるの。　すると　ゆうかんに　なるんだ！$"

Seika_Mossgrove_Gym_EventScript_Mitsu::
	trainerbattle_single TRAINER_SEIKA_PKMN_BREEDER_MITSU, Seika_Mossgrove_Gym_Text_MitsuIntro, Seika_Mossgrove_Gym_Text_MitsuDefeat
	msgbox Seika_Mossgrove_Gym_Text_MitsuPost, MSGBOX_AUTOCLOSE
	end

Seika_Mossgrove_Gym_Text_MitsuIntro::
	.string "Careful! The hives are buzzing\n"
	.string "today. So are my POKéMON!$"
	.string "{JPN}{JPN}きをつけて！　きょうは　すが　ざわざわ\n"
	.string "してるの。　わたしの　ポケモンもよ！$"

Seika_Mossgrove_Gym_Text_MitsuDefeat::
	.string "Oh, the poor dears.$"
	.string "{JPN}{JPN}まあ　かわいそうに。$"

Seika_Mossgrove_Gym_Text_MitsuPost::
	.string "KAYA's VESPIQUEN rules the hive.\n"
	.string "It heals itself, so hit it hard!$"
	.string "{JPN}{JPN}カヤさんの　ビークインは　すの　じょおう。\n"
	.string "じぶんで　なおすから　いっきに　たたいて！$"

Seika_Mossgrove_Gym_EventScript_Kaya::
	trainerbattle_single TRAINER_SEIKA_LEADER_KAYA, Seika_Mossgrove_Gym_Text_KayaIntro, Seika_Mossgrove_Gym_Text_KayaDefeat, Seika_Mossgrove_Gym_EventScript_KayaDefeated, NO_MUSIC
	msgbox Seika_Mossgrove_Gym_Text_KayaPost, MSGBOX_DEFAULT
	release
	end

Seika_Mossgrove_Gym_EventScript_KayaDefeated::
	setflag FLAG_QOL_SEIKA_BADGE03
	msgbox Seika_Mossgrove_Gym_Text_KayaGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_GIGA_DRAIN
	msgbox Seika_Mossgrove_Gym_Text_KayaPost, MSGBOX_DEFAULT
	release
	end

Seika_Mossgrove_Gym_Text_KayaIntro::
	.string "Hello! I'm KAYA. I keep the bees,\n"
	.string "and the bees keep the shrine.\p"
	.string "A hive is a thousand small wings\n"
	.string "working as one.\p"
	.string "We'll swarm you, sting you, and\n"
	.string "then heal right up. Ready?$"
	.string "{JPN}{JPN}カヤ『こんにちは！　わたしは　カヤ。　わたしが\n"
	.string "ハチを　まもり　ハチが　やしろを　まもるの。\p"
	.string "すと　いうのは　1000の　ちいさな　はねが\n"
	.string "ひとつに　なって　はたらく　ところ。\p"
	.string "むらがって　さして　すぐに　かいふく！\n"
	.string "じゅんびは　いい？』$"

Seika_Mossgrove_Gym_Text_KayaDefeat::
	.string "Bzz... Our hive has never been\n"
	.string "beaten so sweetly!\p"
	.string "The GROVE BADGE is yours.\p"
	.string "{FONT_NORMAL}{PLAYER} received the GROVE BADGE\n"
	.string "from KAYA!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"
	.string "{JPN}{JPN}ブーン‥　わたしたちの　すが　こんなに　あまく\n"
	.string "まけるなんて　はじめて！\p"
	.string "モリバッジは　あなたの　ものよ。\p"
	.string "{FONT_NORMAL}{PLAYER}は　カヤから　モリバッジを\n"
	.string "もらった！{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_Mossgrove_Gym_Text_KayaGift::
	.string "Take this TM, too. It drinks your\n"
	.string "foe's strength, the way my bees\l"
	.string "drink nectar.$"
	.string "{JPN}{JPN}カヤ『この　わざマシンも　どうぞ。　ハチが\n"
	.string "ミツを　すうように　あいての　ちからを\l"
	.string "すいとるの。』$"

Seika_Mossgrove_Gym_Text_KayaPost::
	.string "The road north of the village\n"
	.string "goes to KILN CITY.\p"
	.string "TOBIN keeps the shrine there,\n"
	.string "and its kilns are the only ones\l"
	.string "that make the STARGLASS BELL.\p"
	.string "If that woman wants the bell,\n"
	.string "KILN CITY is where she's gone.\l"
	.string "Hurry, {PLAYER}!$"
	.string "{JPN}{JPN}カヤ『むらの　きたの　みちは　カマモトシティへ\n"
	.string "つづいて　いるわ。\p"
	.string "そこの　やしろは　トビンさんが　まもって　いて\n"
	.string "ほしガラスのかねを　つくれるのは　そこの\l"
	.string "かまだけなの。\p"
	.string "あの　おんなの　ひとが　かねを　ねらって\n"
	.string "いるなら　きっと　カマモトシティよ。　いそいで\l"
	.string "{PLAYER}！』$"

Seika_Mossgrove_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE03, Seika_Mossgrove_Gym_EventScript_GymGuyWon
	msgbox Seika_Mossgrove_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_Mossgrove_Gym_EventScript_GymGuyWon::
	msgbox Seika_Mossgrove_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_Mossgrove_Gym_Text_GymGuy::
	.string "Yo! KAYA's hive is all BUG\n"
	.string "POKéMON.\p"
	.string "FIRE, FLYING and ROCK moves\n"
	.string "smoke them out!\p"
	.string "Careful with her SCIZOR. Only\n"
	.string "FIRE really scares it.$"
	.string "{JPN}{JPN}よう！　カヤの　すは　むしポケモン　ばかりだ。\p"
	.string "ほのお　ひこう　いわの　わざで　いぶりだして\n"
	.string "やれ！\p"
	.string "ハッサムには　きをつけろ。　ほんとうに\n"
	.string "にがてなのは　ほのおだけだ。$"

Seika_Mossgrove_Gym_Text_GymGuyWon::
	.string "You out-buzzed the queen bee!\n"
	.string "Sweet!$"
	.string "{JPN}{JPN}じょおうバチに　かったな！　あまーい\n"
	.string "しょうりだぜ！$"
