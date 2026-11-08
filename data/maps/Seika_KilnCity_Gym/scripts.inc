Seika_KilnCity_Gym_MapScripts::
	.byte 0

Seika_KilnCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_QOL_SEIKA_BADGE04, Seika_KilnCity_Gym_EventScript_GymStatueWon
	msgbox Seika_KilnCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Gym_EventScript_GymStatueWon::
	msgbox Seika_KilnCity_Gym_Text_GymStatueWon, MSGBOX_DEFAULT
	releaseall
	end

Seika_KilnCity_Gym_Text_GymStatue::
	.string "KILN CITY POKéMON GYM\n"
	.string "LEADER: TOBIN\p"
	.string "WINNING TRAINERS:\n"
	.string "(none yet)$"
	.string "{JPN}{JPN}カマモト　ポケモンジム　リーダー　トビン\p"
	.string "しょうりした　トレーナー:　まだ　いない$"

Seika_KilnCity_Gym_Text_GymStatueWon::
	.string "KILN CITY POKéMON GYM\n"
	.string "LEADER: TOBIN\p"
	.string "WINNING TRAINERS:\n"
	.string "{PLAYER}$"
	.string "{JPN}{JPN}カマモト　ポケモンジム　リーダー　トビン\p"
	.string "しょうりした　トレーナー:　{PLAYER}$"

Seika_KilnCity_Gym_EventScript_Daiki::
	trainerbattle_single TRAINER_SEIKA_CAMPER_DAIKI, Seika_KilnCity_Gym_Text_DaikiIntro, Seika_KilnCity_Gym_Text_DaikiDefeat
	msgbox Seika_KilnCity_Gym_Text_DaikiPost, MSGBOX_AUTOCLOSE
	end

Seika_KilnCity_Gym_Text_DaikiIntro::
	.string "TOBIN taught me to dig my own\n"
	.string "clay. I dig my own wins, too!$"
	.string "{JPN}{JPN}ししょうに　ねんどは　じぶんで　ほれと\n"
	.string "おそわった。　かちも　じぶんで　ほりあてるぞ！$"

Seika_KilnCity_Gym_Text_DaikiDefeat::
	.string "I dug myself into a hole...$"
	.string "{JPN}{JPN}じぶんで　ほった　あなに　おちちゃった‥$"

Seika_KilnCity_Gym_Text_DaikiPost::
	.string "The walls in here are built like\n"
	.string "kilns. TOBIN says it keeps us warm.$"
	.string "{JPN}{JPN}この　ジムの　かべは　かまと　おなじ　つくり。\n"
	.string "あったかいって　ししょうは　いうよ。$"

Seika_KilnCity_Gym_EventScript_Kazu::
	trainerbattle_single TRAINER_SEIKA_POKEMANIAC_KAZU, Seika_KilnCity_Gym_Text_KazuIntro, Seika_KilnCity_Gym_Text_KazuDefeat
	msgbox Seika_KilnCity_Gym_Text_KazuPost, MSGBOX_AUTOCLOSE
	end

Seika_KilnCity_Gym_Text_KazuIntro::
	.string "Bones and clay! Clay and bones!\n"
	.string "My POKéMON love the ground!$"
	.string "{JPN}{JPN}ホネと　ねんど！　ねんどと　ホネ！　ぼくの\n"
	.string "ポケモンは　じめんが　だいすき！$"

Seika_KilnCity_Gym_Text_KazuDefeat::
	.string "My bones are rattled!$"
	.string "{JPN}{JPN}ホネまで　ガタガタだ！$"

Seika_KilnCity_Gym_Text_KazuPost::
	.string "TOBIN's GOLURK was shaped by his\n"
	.string "own hands. It's a real clay giant!$"
	.string "{JPN}{JPN}トビンさんの　ゴルーグは　てづくり　なんだ。\n"
	.string "ほんものの　ねんどの　きょじんだよ！$"

Seika_KilnCity_Gym_EventScript_Tetsu::
	trainerbattle_single TRAINER_SEIKA_RUIN_MANIAC_TETSU, Seika_KilnCity_Gym_Text_TetsuIntro, Seika_KilnCity_Gym_Text_TetsuDefeat
	msgbox Seika_KilnCity_Gym_Text_TetsuPost, MSGBOX_AUTOCLOSE
	end

Seika_KilnCity_Gym_Text_TetsuIntro::
	.string "I found old pots under the city.\n"
	.string "And a few old moves, too!$"
	.string "{JPN}{JPN}まちの　したで　ふるい　つぼを　ほりあてた。\n"
	.string "ふるい　わざも　いくつか　な！$"

Seika_KilnCity_Gym_Text_TetsuDefeat::
	.string "Shattered like a pot!$"
	.string "{JPN}{JPN}つぼみたいに　こなごなだ！$"

Seika_KilnCity_Gym_Text_TetsuPost::
	.string "Go on. The master is waiting at\n"
	.string "the back of the kiln.$"
	.string "{JPN}{JPN}さあ　いけ。　ししょうは　かまの　おくで　まって\n"
	.string "いるぞ。$"

Seika_KilnCity_Gym_EventScript_Tobin::
	trainerbattle_single TRAINER_SEIKA_LEADER_TOBIN, Seika_KilnCity_Gym_Text_TobinIntro, Seika_KilnCity_Gym_Text_TobinDefeat, Seika_KilnCity_Gym_EventScript_TobinDefeated, NO_MUSIC
	msgbox Seika_KilnCity_Gym_Text_TobinPost, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Gym_EventScript_TobinDefeated::
	setflag FLAG_QOL_SEIKA_BADGE04
	msgbox Seika_KilnCity_Gym_Text_TobinGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_EARTHQUAKE
	msgbox Seika_KilnCity_Gym_Text_TobinPost, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Gym_Text_TobinIntro::
	.string "So you have walked through my\n"
	.string "kilns. I am TOBIN, the potter.\p"
	.string "Clay remembers every hand that\n"
	.string "shaped it, and every fire that\l"
	.string "hardened it.\p"
	.string "Let us see what kind of fire\n"
	.string "shaped you, trainer!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}トビン『わしの　かまを　ぬけて　きたか。　わしは\n"
	.string "とうげいかの　トビン。\p"
	.string "ねんどは　こねた　ての　ことも　やいた　ほのおの\n"
	.string "ことも　すべて　おぼえて　おる。\p"
	.string "おまえさんを　きたえた　ほのおが　どんな　ものか\n"
	.string "みせて　もらおう！』{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

Seika_KilnCity_Gym_Text_TobinDefeat::
	.string "Hah! My clay has cracked, and\n"
	.string "that is no shame.\p"
	.string "You have earned the KILN BADGE.\n"
	.string "Wear it with care.\p"
	.string "{FONT_NORMAL}{PLAYER} received the KILN BADGE\n"
	.string "from TOBIN!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"
	.string "{JPN}{JPN}はっは！　わしの　ねんどが　われたか。　はじる\n"
	.string "ことでは　ない。\p"
	.string "カマバッジを　わたそう。　だいじに\n"
	.string "するんじゃぞ。\p"
	.string "{FONT_NORMAL}{PLAYER}は　トビンから　カマバッジを\n"
	.string "もらった！{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_KilnCity_Gym_Text_TobinGift::
	.string "And take this TM. It is the\n"
	.string "shaking of the earth itself.$"
	.string "{JPN}{JPN}トビン『それと　この　わざマシンも　やろう。\n"
	.string "だいち　そのものを　ゆるがす　わざじゃ。』$"

Seika_KilnCity_Gym_Text_TobinPost::
	.string "Go east along TIDE ROAD to\n"
	.string "TIDEWATCH TOWN.\p"
	.string "NAGI keeps the TIDE SHRINE there.\n"
	.string "She reads the sea the way I\l"
	.string "read clay.$"
	.string "{JPN}{JPN}トビン『シオサイかいどうを　ひがしへ　ゆけば\n"
	.string "シオミタウンじゃ。\p"
	.string "ナギサが　シオミの　やしろを　まもって　おる。\n"
	.string "わしが　ねんどを　よむように　あれは　うみを\l"
	.string "よむのじゃ。』$"

Seika_KilnCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE04, Seika_KilnCity_Gym_EventScript_GymGuyWon
	msgbox Seika_KilnCity_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Gym_EventScript_GymGuyWon::
	msgbox Seika_KilnCity_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_KilnCity_Gym_Text_GymGuy::
	.string "Yo! Welcome to the KILN GYM!\p"
	.string "TOBIN uses GROUND types. WATER,\n"
	.string "GRASS and ICE moves soften his\l"
	.string "clay.\p"
	.string "His ace is GOLURK, a clay giant.\n"
	.string "Its GHOST type dodges NORMAL\l"
	.string "and FIGHTING moves!$"
	.string "{JPN}{JPN}よう！　カマモトジムへ　ようこそ！\p"
	.string "トビンは　じめんタイプの　つかいて。　みず　くさ\n"
	.string "こおりの　わざで　ねんどを　やわらかく\l"
	.string "しちまえ！\p"
	.string "きりふだは　ねんどの　きょじん　ゴルーグ。\n"
	.string "ゴーストタイプだから　ノーマルと　かくとうの\l"
	.string "わざは　きかないぞ！$"

Seika_KilnCity_Gym_Text_GymGuyWon::
	.string "You cracked the KILN GYM!\n"
	.string "That's four BADGES now!$"
	.string "{JPN}{JPN}カマモトジムを　うちくだいたな！　これで\n"
	.string "バッジは　4つだ！$"
