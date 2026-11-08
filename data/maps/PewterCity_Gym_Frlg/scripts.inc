PewterCity_Gym_Frlg_MapScripts::
	.byte 0

PewterCity_Gym_EventScript_Brock::
	famechecker FAMECHECKER_BROCK, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_BROCK, PewterCity_Gym_Text_BrockIntro, PewterCity_Gym_Text_BrockDefeat, PewterCity_Gym_EventScript_DefeatedBrock, NO_MUSIC
	goto_if_unset FLAG_GOT_TM39_FROM_BROCK, PewterCity_Gym_EventScript_GiveTM39
	msgbox PewterCity_Gym_Text_BrockPostBattle
	release
	end

PewterCity_Gym_EventScript_DefeatedBrock::
	famechecker FAMECHECKER_BROCK, 1
	setflag FLAG_DEFEATED_BROCK
	setflag FLAG_BADGE01_GET
	setvar VAR_MAP_SCENE_PEWTER_CITY, 1
	setflag FLAG_HIDE_PEWTER_CITY_GYM_GUIDE
	clearflag FLAG_HIDE_PEWTER_CITY_RUNNING_SHOES_GUY
	set_gym_trainers_frlg 1
	goto PewterCity_Gym_EventScript_GiveTM39
	end

PewterCity_Gym_EventScript_GiveTM39::
	msgbox PewterCity_Gym_Text_TakeThisWithYou
	checkitemspace ITEM_TM39
	goto_if_eq VAR_RESULT, FALSE, PewterCity_Gym_EventScript_NoRoomForTM39
	giveitem_msg PewterCity_Gym_Text_ReceivedTM39FromBrock, ITEM_TM39
	setflag FLAG_GOT_TM39_FROM_BROCK
	msgbox PewterCity_Gym_Text_ExplainTM39
	release
	end

PewterCity_Gym_EventScript_NoRoomForTM39::
	msgbox PewterCity_Gym_Text_DontHaveRoomForThis
	release
	end

PewterCity_Gym_EventScript_Liam::
	trainerbattle_single TRAINER_CAMPER_LIAM, PewterCity_Gym_Text_LiamIntro, PewterCity_Gym_Text_LiamDefeat
	msgbox PewterCity_Gym_Text_LiamPostBattle, MSGBOX_AUTOCLOSE
	end

PewterCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_BROCK, PewterCity_Gym_EventScript_GymGuyPostVictory
	msgbox PewterCity_Gym_Text_LetMeTakeYouToTheTop, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PewterCity_Gym_EventScript_GymGuyTakeMeToTop
	goto_if_eq VAR_RESULT, NO, PewterCity_Gym_EventScript_GymGuyDontTakeMeToTop
	end

PewterCity_Gym_EventScript_GymGuyPostVictory::
	msgbox PewterCity_Gym_Text_YoureChampMaterial
	release
	end

PewterCity_Gym_EventScript_GymGuyTakeMeToTop::
	msgbox PewterCity_Gym_Text_LetsGetHappening
	goto PewterCity_Gym_EventScript_GymGuyAdvice
	end

PewterCity_Gym_EventScript_GymGuyDontTakeMeToTop::
	msgbox PewterCity_Gym_Text_ItsFreeLetsGetHappening
	goto PewterCity_Gym_EventScript_GymGuyAdvice
	end

PewterCity_Gym_EventScript_GymGuyAdvice::
	msgbox PewterCity_Gym_Text_TryDifferentPartyOrders
	release
	end

PewterCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, PewterCity_Gym_EventScript_GymStatuePostVictory
	msgbox PewterCity_Gym_Text_GymStatue
	releaseall
	end

PewterCity_Gym_EventScript_GymStatuePostVictory::
	msgbox PewterCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

PewterCity_Gym_Text_BrockIntro::
	.string "So, you're here. I'm BROCK.\n"
	.string "I'm PEWTER's GYM LEADER.\p"
	.string "My rock-hard willpower is evident\n"
	.string "even in my POKéMON.\p"
	.string "My POKéMON are all rock hard, and\n"
	.string "have true-grit determination.\p"
	.string "That's right - my POKéMON are all\n"
	.string "the ROCK type!\p"
	.string "Fuhaha! You're going to challenge\n"
	.string "me knowing that you'll lose?\p"
	.string "That's the TRAINER's honor that\n"
	.string "compels you to challenge me.\p"
	.string "Fine, then!\n"
	.string "Show me your best!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}よく　きたな！　おれは　タケシ。　ニビの\n"
	.string "ジムリーダーだ！\p"
	.string "おれの　かたい　いしは　おれの　ポケモンにも\n"
	.string "あらわれて　いる！\p"
	.string "おれの　ポケモンは　みんな　いしのように　かたく\n"
	.string "ねばりづよい！\p"
	.string "そう！　おれの　ポケモンは　みんな　いわタイプ\n"
	.string "なのだ！\p"
	.string "ふはは！　まけると　わかって　いて　おれに\n"
	.string "いどむのか？\p"
	.string "それも　トレーナーの　プライドが　そうさせる\n"
	.string "のだろう。\p"
	.string "よし　いいだろう！　おまえの　ちからを　みせて\n"
	.string "みろ！{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PewterCity_Gym_Text_BrockDefeat::
	.string "I took you for granted, and so\n"
	.string "I lost.\p"
	.string "As proof of your victory, I confer\n"
	.string "on you this…the official POKéMON\l"
	.string "LEAGUE BOULDERBADGE.\p"
	.string "{FONT_NORMAL}{PLAYER} received the BOULDERBADGE\n"
	.string "from BROCK!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}\p"
	.string "{FONT_MALE}Just having the BOULDERBADGE makes\n"
	.string "your POKéMON more powerful.\p"
	.string "It also enables the use of the\n"
	.string "move FLASH outside of battle.\p"
	.string "Of course, a POKéMON must know the\n"
	.string "move FLASH to use it.$"
	.string "{JPN}{JPN}おまえを　あまく　みて　いた。　だから\n"
	.string "まけたのだ。\p"
	.string "かった　しるしに　この　ポケモンリーグ\n"
	.string "こうにんの　グレーバッジを　わたそう。\p"
	.string "{FONT_NORMAL}{PLAYER}は　タケシから　グレーバッジを\n"
	.string "もらった！{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}\p"
	.string "{FONT_MALE}グレーバッジを　もって　いるだけで　ポケモンが\n"
	.string "つよく　なる。\p"
	.string "それに　せんとう　いがいでも　フラッシュが\n"
	.string "つかえるように　なるぞ。\p"
	.string "もちろん　フラッシュを　おぼえた　ポケモンが\n"
	.string "ひつよう　だがな。$"

PewterCity_Gym_Text_TakeThisWithYou::
	.string "Wait!\n"
	.string "Take this with you.$"
	.string "{JPN}{JPN}まて！　これも　もって　いけ。$"

PewterCity_Gym_Text_ReceivedTM39FromBrock::
	.string "{PLAYER} received TM39\n"
	.string "from BROCK.$"
	.string "{JPN}{JPN}{PLAYER}は　タケシから　わざマシン39を\n"
	.string "もらった！$"

PewterCity_Gym_Text_ExplainTM39::
	.string "A TM, Technical Machine, contains a\n"
	.string "technique for POKéMON.\p"
	.string "Using a TM teaches the move it\n"
	.string "contains to a POKéMON.\p"
	.string "A TM is good for only one use.\p"
	.string "So, when you use one, pick the\n"
	.string "POKéMON carefully.\p"
	.string "Anyways…\n"
	.string "TM39 contains ROCK TOMB.\p"
	.string "It hurls boulders at the foe and\n"
	.string "lowers its SPEED.$"
	.string "{JPN}{JPN}わざマシンには　ポケモンの　わざが　はいって\n"
	.string "いる。\p"
	.string "わざマシンを　つかうと　その　わざを　ポケモンに\n"
	.string "おぼえさせられる。\p"
	.string "わざマシンは　1かい　つかうと　なくなる。\p"
	.string "だから　つかう　ときは　ポケモンを　よく\n"
	.string "えらぶんだぞ。\p"
	.string "ところで‥　わざマシン39の　なかみは\n"
	.string "がんせきふうじだ。\p"
	.string "あいてに　いわを　なげつけて　すばやさを　さげる\n"
	.string "わざだ。$"

PewterCity_Gym_Text_BrockPostBattle::
	.string "There are all kinds of TRAINERS in\n"
	.string "this huge world of ours.\p"
	.string "You appear to be very gifted as a\n"
	.string "POKéMON TRAINER.\p"
	.string "So let me make a suggestion.\p"
	.string "Go to the GYM in CERULEAN and test\n"
	.string "your abilities.$"
	.string "{JPN}{JPN}この　ひろい　せかいには　いろんな　トレーナーが\n"
	.string "いる。\p"
	.string "おまえには　ポケモントレーナーとしての\n"
	.string "すばらしい　さいのうが　あるようだ。\p"
	.string "そこで　ひとつ　アドバイスだ。\p"
	.string "ハナダの　ジムに　いって　じぶんの　ちからを\n"
	.string "ためして　みろ。$"

PewterCity_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.$"
	.string "{JPN}{JPN}これを　しまう　ばしょが　ないようだな。$"

PewterCity_Gym_Text_LiamIntro::
	.string "Stop right there, kid!\p"
	.string "You're ten thousand light-years \n"
	.string "from facing BROCK!$"
	.string "{JPN}{JPN}ちょっと　まった！\p"
	.string "タケシに　いどむ　なんて　1まんこうねん\n"
	.string "はやいぜ！$"

PewterCity_Gym_Text_LiamDefeat::
	.string "Darn!\p"
	.string "Light-years isn't time…\n"
	.string "It measures distance!$"
	.string "{JPN}{JPN}くそー！\p"
	.string "こうねんは　じかんじゃ　なくて‥　きょりの\n"
	.string "たんい　だった！$"

PewterCity_Gym_Text_LiamPostBattle::
	.string "You're pretty hot.\n"
	.string "…But not as hot as BROCK!$"
	.string "{JPN}{JPN}なかなか　やるな。　‥でも　タケシは　もっと\n"
	.string "すごいぞ！$"

PewterCity_Gym_Text_LetMeTakeYouToTheTop::
	.string "Hiya!\n"
	.string "Do you want to dream big?\p"
	.string "Do you dare to dream of becoming\n"
	.string "the POKéMON champ?\p"
	.string "I'm no TRAINER, but I can advise\n"
	.string "you on how to win.\p"
	.string "Let me take you to the top!$"
	.string "{JPN}{JPN}よう！　でっかい　ゆめを　もってるかい？\p"
	.string "ポケモン　チャンピオンに　なる　ゆめを　みる\n"
	.string "ゆうきは　あるかい？\p"
	.string "おれは　トレーナーじゃ　ないが　かつ　ための\n"
	.string "アドバイスなら　できるぜ。\p"
	.string "おまえを　てっぺんまで　つれてって　やるよ！$"

PewterCity_Gym_Text_LetsGetHappening::
	.string "All right!\n"
	.string "Let's get happening!$"
	.string "{JPN}{JPN}よっしゃ！　はじめようぜ！$"

PewterCity_Gym_Text_TryDifferentPartyOrders::
	.string "The first POKéMON out in a match is\n"
	.string "at the left of the POKéMON LIST.\p"
	.string "By changing the order of POKéMON,\n"
	.string "you may gain an advantage.\p"
	.string "Try different orders to suit your\n"
	.string "opponent's party.$"
	.string "{JPN}{JPN}しょうぶで　さいしょに　でる　ポケモンは\n"
	.string "ポケモンリストの　ひだりに　いる　ポケモンだ。\p"
	.string "ポケモンの　じゅんばんを　かえれば　ゆうりに\n"
	.string "たたかえる　ことも　ある。\p"
	.string "あいての　てもちに　あわせて　いろいろな\n"
	.string "じゅんばんを　ためして　みな。$"

PewterCity_Gym_Text_ItsFreeLetsGetHappening::
	.string "It's a free service!\n"
	.string "Let's get happening!$"
	.string "{JPN}{JPN}アドバイスは　タダだ！　はじめようぜ！$"

PewterCity_Gym_Text_YoureChampMaterial::
	.string "Just as I thought!\n"
	.string "You're POKéMON champ material!$"
	.string "{JPN}{JPN}おもった　とおりだ！　おまえは　チャンピオンの\n"
	.string "うつわだぜ！$"

PewterCity_Gym_Text_GymStatue::
	.string "PEWTER POKéMON GYM\n"
	.string "LEADER: BROCK\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"
	.string "{JPN}{JPN}ニビ　ポケモンジム\n"
	.string "リーダー　タケシ\p"
	.string "しょうりした　トレーナー:\n"
	.string "{RIVAL}$"

PewterCity_Gym_Text_GymStatuePlayerWon::
	.string "PEWTER POKéMON GYM\n"
	.string "LEADER: BROCK\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"
	.string "{JPN}{JPN}ニビ　ポケモンジム\n"
	.string "リーダー　タケシ\p"
	.string "しょうりした　トレーナー:\n"
	.string "{RIVAL}　{PLAYER}$"

