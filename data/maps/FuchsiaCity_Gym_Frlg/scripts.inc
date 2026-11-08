FuchsiaCity_Gym_Frlg_MapScripts::
	.byte 0

FuchsiaCity_Gym_EventScript_Koga::
	famechecker FAMECHECKER_KOGA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_KOGA, FuchsiaCity_Gym_Text_KogaIntro, FuchsiaCity_Gym_Text_KogaDefeat, FuchsiaCity_Gym_EventScript_DefeatedKoga, NO_MUSIC
	goto_if_unset FLAG_GOT_TM06_FROM_KOGA, FuchsiaCity_Gym_EventScript_GiveTM06
	msgbox FuchsiaCity_Gym_Text_KogaPostBattle
	release
	end

FuchsiaCity_Gym_EventScript_DefeatedKoga::
	famechecker FAMECHECKER_KOGA, 1
	clearflag FLAG_HIDE_FAME_CHECKER_KOGA_JOURNAL
	setflag FLAG_DEFEATED_KOGA
	setflag FLAG_BADGE05_GET
	set_gym_trainers_frlg 5
	goto FuchsiaCity_Gym_EventScript_GiveTM06
	end

FuchsiaCity_Gym_EventScript_GiveTM06::
	msgbox FuchsiaCity_Gym_Text_KogaExplainSoulBadge
	checkitemspace ITEM_TM06
	goto_if_eq VAR_RESULT, FALSE, FuchsiaCity_Gym_EventScript_NoRoomForTM06
	giveitem_msg FuchsiaCity_Gym_Text_ReceivedTM06FromKoga, ITEM_TM06
	setflag FLAG_GOT_TM06_FROM_KOGA
	msgbox FuchsiaCity_Gym_Text_KogaExplainTM06
	release
	end

FuchsiaCity_Gym_EventScript_NoRoomForTM06::
	msgbox FuchsiaCity_Gym_Text_MakeSpaceForThis
	release
	end

FuchsiaCity_Gym_EventScript_Phil::
	trainerbattle_single TRAINER_TAMER_PHIL, FuchsiaCity_Gym_Text_PhilIntro, FuchsiaCity_Gym_Text_PhilDefeat
	msgbox FuchsiaCity_Gym_Text_PhilPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Edgar::
	trainerbattle_single TRAINER_TAMER_EDGAR, FuchsiaCity_Gym_Text_EdgarIntro, FuchsiaCity_Gym_Text_EdgarDefeat
	msgbox FuchsiaCity_Gym_Text_EdgarPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Kirk::
	trainerbattle_single TRAINER_JUGGLER_KIRK, FuchsiaCity_Gym_Text_KirkIntro, FuchsiaCity_Gym_Text_KirkDefeat
	famechecker FAMECHECKER_KOGA, 2
	msgbox FuchsiaCity_Gym_Text_KirkPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Shawn::
	trainerbattle_single TRAINER_JUGGLER_SHAWN, FuchsiaCity_Gym_Text_ShawnIntro, FuchsiaCity_Gym_Text_ShawnDefeat
	msgbox FuchsiaCity_Gym_Text_ShawnPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Kayden::
	trainerbattle_single TRAINER_JUGGLER_KAYDEN, FuchsiaCity_Gym_Text_KaydenIntro, FuchsiaCity_Gym_Text_KaydenDefeat
	msgbox FuchsiaCity_Gym_Text_KaydenPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Nate::
	trainerbattle_single TRAINER_JUGGLER_NATE, FuchsiaCity_Gym_Text_NateIntro, FuchsiaCity_Gym_Text_NateDefeat
	msgbox FuchsiaCity_Gym_Text_NatePostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_KOGA, FuchsiaCity_Gym_EventScript_GymGuyPostVictory
	msgbox FuchsiaCity_Gym_Text_GymGuyAdvice
	release
	end

FuchsiaCity_Gym_EventScript_GymGuyPostVictory::
	msgbox FuchsiaCity_Gym_Text_GymGuyPostVictory
	release
	end

FuchsiaCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE05_GET, FuchsiaCity_Gym_EventScript_GymStatuePostVictory
	msgbox FuchsiaCity_Gym_Text_GymStatue
	releaseall
	end

FuchsiaCity_Gym_EventScript_GymStatuePostVictory::
	msgbox FuchsiaCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

FuchsiaCity_Gym_Text_KogaIntro::
	.string "KOGA: Fwahahaha!\p"
	.string "A mere child like you dares to\n"
	.string "challenge me?\p"
	.string "The very idea makes me shiver\n"
	.string "with mirth!\p"
	.string "Very well, I shall show you true\n"
	.string "terror as a ninja master.\p"
	.string "Poison brings steady doom.\n"
	.string "Sleep renders foes helpless.\p"
	.string "Despair to the creeping horror of\n"
	.string "POISON-type POKéMON!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}キョウ:　ファファファ！\p"
	.string "こわっぱの　ぶんざいで　せっしゃに　いどむ\n"
	.string "とは！\p"
	.string "おもわず　わらいで　みぶるいが　するわ！\p"
	.string "よかろう　にんじゃ　マスターの　ほんとうの\n"
	.string "おそろしさを　みせて　やろう。\p"
	.string "どくは　じわじわと　ほろびを　もたらし　ねむりは\n"
	.string "あいてを　むりょくに　する。\p"
	.string "どくタイプ　ポケモンの　しのびよる　きょうふに\n"
	.string "ぜつぼう　するが　よい！{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

FuchsiaCity_Gym_Text_KogaDefeat::
	.string "Humph!\n"
	.string "You have proven your worth!\p"
	.string "Here!\n"
	.string "Take the SOULBADGE!$"
	.string "{JPN}{JPN}むう！　おぬしの　じつりょく　みとめよう！\p"
	.string "さあ！　ピンクバッジを　うけとるが　よい！$"

FuchsiaCity_Gym_Text_KogaPostBattle::
	.string "When afflicted by TOXIC, a POKéMON\n"
	.string "suffers more and more.\p"
	.string "It suffers worsening damage as the\n"
	.string "battle wears on!\p"
	.string "It will surely terrorize foes!$"
	.string "{JPN}{JPN}どくどくを　うけた　ポケモンは　どんどん\n"
	.string "くるしむ。\p"
	.string "たたかいが　ながびくほど　ダメージが　ふえて\n"
	.string "いくのだ！\p"
	.string "てきを　きょうふに　おとしいれる　こと\n"
	.string "まちがいなし！$"

FuchsiaCity_Gym_Text_KogaExplainSoulBadge::
	.string "Now that you have the SOULBADGE,\n"
	.string "the DEFENSE of your POKéMON rises.\p"
	.string "It also lets you SURF outside of\n"
	.string "battle.\p"
	.string "Ah!\n"
	.string "Take this, too!$"
	.string "{JPN}{JPN}ピンクバッジを　もっていれば　ポケモンの\n"
	.string "ぼうぎょが　あがる。\p"
	.string "さらに　たたかい　いがいでも　なみのりが\n"
	.string "つかえるように　なるぞ。\p"
	.string "おお！　これも　もっていけ！$"

FuchsiaCity_Gym_Text_ReceivedTM06FromKoga::
	.string "{PLAYER} received TM06\n"
	.string "from KOGA.$"
	.string "{JPN}{JPN}{PLAYER}は　キョウから　わざマシン06を\n"
	.string "もらった！$"

FuchsiaCity_Gym_Text_KogaExplainTM06::
	.string "Sealed within that TM06 lies\n"
	.string "TOXIC!\p"
	.string "It is a secret technique dating\n"
	.string "back some four hundred years.$"
	.string "{JPN}{JPN}その　わざマシン06には　どくどくが\n"
	.string "ふうじこめられて　おる！\p"
	.string "およそ　400ねんの　むかしから　つたわる\n"
	.string "ひじゅつ　なのだ。$"

FuchsiaCity_Gym_Text_MakeSpaceForThis::
	.string "Make space for this, child!$"
	.string "{JPN}{JPN}こわっぱよ　もちものを　かたづけて　まいれ！$"

FuchsiaCity_Gym_Text_KaydenIntro::
	.string "Strength isn't the key for POKéMON.\n"
	.string "Do you understand this?\p"
	.string "POKéMON is about strategy!\p"
	.string "I'll show you how strategy can\n"
	.string "beat brute strength.$"
	.string "{JPN}{JPN}ポケモンは　ちからだけじゃ　ない。　わかるかい？\p"
	.string "ポケモンは　さくせんが　だいじ　なんだ！\p"
	.string "さくせんで　ちからまかせを　うちまかす　ところを\n"
	.string "みせて　やるよ。$"

FuchsiaCity_Gym_Text_KaydenDefeat::
	.string "What?\n"
	.string "Extraordinary!$"
	.string "{JPN}{JPN}なに？　すごい！$"

FuchsiaCity_Gym_Text_KaydenPostBattle::
	.string "So, you mix brawn with brains?\n"
	.string "Good strategy!\p"
	.string "That's remarkable for a child\n"
	.string "TRAINER.$"
	.string "{JPN}{JPN}ちからと　あたまを　くみあわせて　いるのか？\n"
	.string "いい　さくせんだ！\p"
	.string "こどもの　トレーナーに　しては　たいした\n"
	.string "もんだよ。$"

FuchsiaCity_Gym_Text_KirkIntro::
	.string "I was a magician once upon a time.\p"
	.string "But I dreamt of becoming a ninja,\n"
	.string "so I joined this GYM.$"
	.string "{JPN}{JPN}むかしは　てじなし　だったんだ。\p"
	.string "でも　にんじゃに　あこがれて　この　ジムに\n"
	.string "はいったのさ。$"

FuchsiaCity_Gym_Text_KirkDefeat::
	.string "I'm done for!$"
	.string "{JPN}{JPN}まいった！$"

FuchsiaCity_Gym_Text_KirkPostBattle::
	.string "Even though I've lost, I will keep\n"
	.string "training according to the teachings\l"
	.string "of KOGA, my ninja master.$"
	.string "{JPN}{JPN}まけても　にんじゃ　マスター　キョウさまの\n"
	.string "おしえを　まもって　しゅぎょうを　つづけるぞ。$"

FuchsiaCity_Gym_Text_NateIntro::
	.string "Let's see you beat my special\n"
	.string "techniques!$"
	.string "{JPN}{JPN}おれの　とくべつな　わざを　やぶれるかな！$"

FuchsiaCity_Gym_Text_NateDefeat::
	.string "You had me fooled!$"
	.string "{JPN}{JPN}いっぱい　くわされた！$"

FuchsiaCity_Gym_Text_NatePostBattle::
	.string "I like poison and sleep techniques,\n"
	.string "as they linger after battle!$"
	.string "{JPN}{JPN}どくと　ねむりの　わざは　バトルの　あとも\n"
	.string "のこるから　すきなんだ！$"

FuchsiaCity_Gym_Text_PhilIntro::
	.string "Stop right there!\p"
	.string "The famed invisible walls of\n"
	.string "FUCHSIA GYM have you frustrated?$"
	.string "{JPN}{JPN}まて！\p"
	.string "セキチクジム　じまんの　みえない　かべに\n"
	.string "てこずって　いるかい？$"

FuchsiaCity_Gym_Text_PhilDefeat::
	.string "Whoa!\n"
	.string "You've got it!$"
	.string "{JPN}{JPN}おお！　やるな！$"

FuchsiaCity_Gym_Text_PhilPostBattle::
	.string "You impressed me!\n"
	.string "Here's a hint!\p"
	.string "Look very closely for gaps in the\n"
	.string "invisible walls!$"
	.string "{JPN}{JPN}かんしん　したよ！　ヒントを　やろう！\p"
	.string "みえない　かべの　すきまを　よーく　さがすんだ！$"

FuchsiaCity_Gym_Text_EdgarIntro::
	.string "I also study the way of the ninja\n"
	.string "with Master KOGA!\p"
	.string "Ninja have a long history of using\n"
	.string "animals!$"
	.string "{JPN}{JPN}おれも　キョウさまの　もとで　にんじゃの　みちを\n"
	.string "まなんで　いるんだ！\p"
	.string "にんじゃは　むかしから　どうぶつを　つかって\n"
	.string "きたんだぞ！$"

FuchsiaCity_Gym_Text_EdgarDefeat::
	.string "Awoo!$"
	.string "{JPN}{JPN}ワオーン！$"

FuchsiaCity_Gym_Text_EdgarPostBattle::
	.string "I still have much to learn.$"
	.string "{JPN}{JPN}まだまだ　まなぶ　ことが　おおいな。$"

FuchsiaCity_Gym_Text_ShawnIntro::
	.string "Master KOGA comes from a long line\n"
	.string "of ninjas.\p"
	.string "What did you descend from?$"
	.string "{JPN}{JPN}キョウさまは　だいだい　つづく　にんじゃの\n"
	.string "いえがらだ。\p"
	.string "おまえは　なんの　いえがら　なんだ？$"

FuchsiaCity_Gym_Text_ShawnDefeat::
	.string "You're more skilled than I'd\n"
	.string "thought!$"
	.string "{JPN}{JPN}おもったより　うでが　たつな！$"

FuchsiaCity_Gym_Text_ShawnPostBattle::
	.string "Where there is light, there is\n"
	.string "shadow!\p"
	.string "Light and shadow!\n"
	.string "Which do you choose?$"
	.string "{JPN}{JPN}ひかり　あるところに　かげ　あり！\p"
	.string "ひかりと　かげ！　おまえは　どちらを　えらぶ？$"

FuchsiaCity_Gym_Text_GymGuyAdvice::
	.string "Yo!\n"
	.string "Champ in the making!\p"
	.string "FUCHSIA GYM is a tricked-up place.\n"
	.string "It's riddled with invisible walls!\p"
	.string "KOGA might appear close, but he's\n"
	.string "blocked off.\p"
	.string "You have to find gaps in the walls\n"
	.string "to reach him.$"
	.string "{JPN}{JPN}よう！　みらいの　チャンピオン！\p"
	.string "セキチクジムは　しかけだらけ。　みえない　かべが\n"
	.string "いっぱい　あるんだ！\p"
	.string "キョウは　ちかくに　みえても　かべで　ふさがれて\n"
	.string "いる。\p"
	.string "かべの　すきまを　みつけないと\n"
	.string "たどりつけないぞ。$"

FuchsiaCity_Gym_Text_GymGuyPostVictory::
	.string "It's amazing how ninja can terrify,\n"
	.string "even now!$"
	.string "{JPN}{JPN}にんじゃって　いまでも　ほんとに　こわいよな！$"

FuchsiaCity_Gym_Text_GymStatue::
	.string "FUCHSIA POKéMON GYM\n"
	.string "LEADER: KOGA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"
	.string "{JPN}{JPN}セキチク　ポケモンジム\n"
	.string "リーダー:　キョウ\p"
	.string "しょうりした　トレーナー:\n"
	.string "{RIVAL}$"

FuchsiaCity_Gym_Text_GymStatuePlayerWon::
	.string "FUCHSIA POKéMON GYM\n"
	.string "LEADER: KOGA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"
	.string "{JPN}{JPN}セキチク　ポケモンジム\n"
	.string "リーダー:　キョウ\p"
	.string "しょうりした　トレーナー:\n"
	.string "{RIVAL}　{PLAYER}$"

