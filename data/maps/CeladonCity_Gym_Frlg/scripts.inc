CeladonCity_Gym_Frlg_MapScripts::
	.byte 0

CeladonCity_Gym_EventScript_Erika::
	famechecker FAMECHECKER_ERIKA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_ERIKA, CeladonCity_Gym_Text_ErikaIntro, CeladonCity_Gym_Text_ErikaDefeat, CeladonCity_Gym_EventScript_DefeatedErika, NO_MUSIC
	goto_if_unset FLAG_GOT_TM19_FROM_ERIKA, CeladonCity_Gym_EventScript_GiveTM19
	famechecker FAMECHECKER_ERIKA, 4
	msgbox CeladonCity_Gym_Text_ErikaPostBattle
	release
	end

CeladonCity_Gym_EventScript_DefeatedErika::
	famechecker FAMECHECKER_ERIKA, 1
	clearflag FLAG_HIDE_FAME_CHECKER_ERIKA_JOURNALS
	setflag FLAG_DEFEATED_ERIKA
	setflag FLAG_BADGE04_GET
	set_gym_trainers_frlg 4
	goto CeladonCity_Gym_EventScript_GiveTM19
	end

CeladonCity_Gym_EventScript_GiveTM19::
	msgbox CeladonCity_Gym_Text_ExplainRainbowBadgeTakeThis
	checkitemspace ITEM_TM19
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_Gym_EventScript_NoRoomForTM19
	giveitem_msg CeladonCity_Gym_Text_ReceivedTM19FromErika, ITEM_TM19
	setflag FLAG_GOT_TM19_FROM_ERIKA
	msgbox CeladonCity_Gym_Text_ExplainTM19
	release
	end

CeladonCity_Gym_EventScript_NoRoomForTM19::
	msgbox CeladonCity_Gym_Text_ShouldMakeRoomForThis
	release
	end

CeladonCity_Gym_EventScript_Kay::
	trainerbattle_single TRAINER_LASS_KAY, CeladonCity_Gym_Text_KayIntro, CeladonCity_Gym_Text_KayDefeat
	msgbox CeladonCity_Gym_Text_KayPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Lisa::
	trainerbattle_single TRAINER_LASS_LISA, CeladonCity_Gym_Text_LisaIntro, CeladonCity_Gym_Text_LisaDefeat
	famechecker FAMECHECKER_ERIKA, 2
	msgbox CeladonCity_Gym_Text_LisaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Tina::
	trainerbattle_single TRAINER_PICNICKER_TINA, CeladonCity_Gym_Text_TinaIntro, CeladonCity_Gym_Text_TinaDefeat
	msgbox CeladonCity_Gym_Text_TinaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Bridget::
	trainerbattle_single TRAINER_BEAUTY_BRIDGET, CeladonCity_Gym_Text_BridgetIntro, CeladonCity_Gym_Text_BridgetDefeat
	msgbox CeladonCity_Gym_Text_BridgetPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Tamia::
	trainerbattle_single TRAINER_BEAUTY_TAMIA, CeladonCity_Gym_Text_TamiaIntro, CeladonCity_Gym_Text_TamiaDefeat
	famechecker FAMECHECKER_ERIKA, 3
	msgbox CeladonCity_Gym_Text_TamiaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Lori::
	trainerbattle_single TRAINER_BEAUTY_LORI, CeladonCity_Gym_Text_LoriIntro, CeladonCity_Gym_Text_LoriDefeat
	msgbox CeladonCity_Gym_Text_LoriPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Mary::
	trainerbattle_single TRAINER_COOLTRAINER_MARY, CeladonCity_Gym_Text_MaryIntro, CeladonCity_Gym_Text_MaryDefeat
	msgbox CeladonCity_Gym_Text_MaryPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, CeladonCity_Gym_EventScript_GymStatuePostVictory
	msgbox CeladonCity_Gym_Text_GymStatue
	releaseall
	end

CeladonCity_Gym_EventScript_GymStatuePostVictory::
	msgbox CeladonCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CeladonCity_Gym_Text_ErikaIntro::
	.string "Hello…\n"
	.string "Lovely weather, isn't it?\l"
	.string "It's so pleasant…\p"
	.string "…Oh, dear…\n"
	.string "I must have dozed off. Welcome.\p"
	.string "My name is ERIKA.\n"
	.string "I am the LEADER of CELADON GYM.\p"
	.string "I am a student of the art of\n"
	.string "flower arranging.\p"
	.string "My POKéMON are solely of the\n"
	.string "GRASS type.\p"
	.string "…Oh, I'm sorry, I had no idea that\n"
	.string "you wished to challenge me.\p"
	.string "Very well, but I shall not lose.{PLAY_BGM MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}こんにちは‥　いい　おてんきですわね‥　ぽかぽか\n"
	.string "して‥\p"
	.string "‥あら　いけない‥　うとうと　して\n"
	.string "しまいましたわ　ようこそ\p"
	.string "わたくしは　エリカ　タマムシジムの　リーダーです\p"
	.string "いけばなを　たしなんで　おりますの\p"
	.string "わたくしの　ポケモンは　くさタイプ　ばかりですわ\p"
	.string "‥まあ　ごめんなさい　ちょうせん　しに\n"
	.string "いらしたとは　ぞんじませんでしたわ\p"
	.string "よろしいですわ　でも　まけませんわよ{PLAY_BGM MUS_RG_ENCOUNTER_GYM_LEADER}$"

CeladonCity_Gym_Text_ErikaDefeat::
	.string "Oh!\n"
	.string "I concede defeat.\l"
	.string "You are remarkably strong.\p"
	.string "I must confer on you the\n"
	.string "RAINBOWBADGE.$"
	.string "{JPN}{JPN}まあ！　まいりましたわ　あなた　とても\n"
	.string "おつよいのね\p"
	.string "では　レインボーバッジを　さしあげますわ$"

CeladonCity_Gym_Text_ErikaPostBattle::
	.string "You are cataloging POKéMON?\n"
	.string "I must say I'm impressed.\p"
	.string "I would never collect POKéMON if\n"
	.string "they were unattractive.$"
	.string "{JPN}{JPN}ポケモンを　しらべて　いらっしゃるの？　かんしん\n"
	.string "いたしますわ\p"
	.string "わたくしなら　うつくしくない　ポケモンは\n"
	.string "あつめませんけれど$"

CeladonCity_Gym_Text_ExplainRainbowBadgeTakeThis::
	.string "The RAINBOWBADGE will make\n"
	.string "POKéMON up to Lv. 50 obey.\p"
	.string "It also allows POKéMON to use\n"
	.string "STRENGTH in and out of battle.\p"
	.string "Please also take this with you.$"
	.string "{JPN}{JPN}レインボーバッジが　あれば　レベル50までの\n"
	.string "ポケモンが　いうことを　きくように　なりますわ\p"
	.string "それに　せんとうの　そとでも　かいりきが\n"
	.string "つかえますの\p"
	.string "こちらも　おもちに　なって$"

CeladonCity_Gym_Text_ReceivedTM19FromErika::
	.string "{PLAYER} received TM19\n"
	.string "from ERIKA.$"
	.string "{JPN}{JPN}{PLAYER}は　エリカから　わざマシン19を\n"
	.string "もらった！$"

CeladonCity_Gym_Text_ExplainTM19::
	.string "TM19 contains GIGA DRAIN.\p"
	.string "Half the damage it inflicts is\n"
	.string "drained to heal your POKéMON.\p"
	.string "Wouldn't you agree that it's a\n"
	.string "wonderful move?$"
	.string "{JPN}{JPN}わざマシン19は　ギガドレイン\p"
	.string "あたえた　ダメージの　はんぶんを　すいとって\n"
	.string "ポケモンの　たいりょくを　かいふく　しますの\p"
	.string "すてきな　わざだと　おもいませんこと？$"

CeladonCity_Gym_Text_ShouldMakeRoomForThis::
	.string "You should make room for this.$"
	.string "{JPN}{JPN}おにもつを　へらして　くださいな$"

CeladonCity_Gym_Text_KayIntro::
	.string "I should tell you about this GYM.\p"
	.string "Only real ladies are allowed in\n"
	.string "here!$"
	.string "{JPN}{JPN}この　ジムの　こと　おしえて　あげる\p"
	.string "ここは　ほんものの　レディしか　はいれないのよ！$"

CeladonCity_Gym_Text_KayDefeat::
	.string "You're too rough!$"
	.string "{JPN}{JPN}らんぼう　すぎるわ！$"

CeladonCity_Gym_Text_KayPostBattle::
	.string "Bleaah!\n"
	.string "I hope ERIKA wipes you out!$"
	.string "{JPN}{JPN}べーだ！　エリカさまに　やられちゃえば\n"
	.string "いいのよ！$"

CeladonCity_Gym_Text_BridgetIntro::
	.string "Oh, welcome.\n"
	.string "I was getting bored.$"
	.string "{JPN}{JPN}あら　いらっしゃい　ちょうど　たいくつ\n"
	.string "してたのよ$"

CeladonCity_Gym_Text_BridgetDefeat::
	.string "My makeup!$"
	.string "{JPN}{JPN}おけしょうが！$"

CeladonCity_Gym_Text_BridgetPostBattle::
	.string "GRASS-type POKéMON are tough\n"
	.string "against the WATER type.\p"
	.string "They also have an edge on ROCK-\n"
	.string "and GROUND-type POKéMON.$"
	.string "{JPN}{JPN}くさタイプの　ポケモンは　みずタイプに　つよいの\p"
	.string "いわタイプや　じめんタイプにも　ゆうりなのよ$"

CeladonCity_Gym_Text_TinaIntro::
	.string "…Weren't you peeking in here\n"
	.string "earlier?$"
	.string "{JPN}{JPN}‥さっき　ここを　のぞいて　なかった？$"

CeladonCity_Gym_Text_TinaDefeat::
	.string "You're an eye-opener!$"
	.string "{JPN}{JPN}めが　さめる　おもいだわ！$"

CeladonCity_Gym_Text_TinaPostBattle::
	.string "Oh, you were looking at ERIKA…\n"
	.string "You weren't looking at me…$"
	.string "{JPN}{JPN}なんだ　エリカさまを　みてたのね‥　わたしじゃ\n"
	.string "なかったんだ‥$"

CeladonCity_Gym_Text_TamiaIntro::
	.string "Look, look!\n"
	.string "See my POKéMON!\p"
	.string "I like the GRASS type.\n"
	.string "I like how they're easy to raise.$"
	.string "{JPN}{JPN}みて　みて！　わたしの　ポケモン！\p"
	.string "くさタイプが　すきなの　そだてやすい　ところが\n"
	.string "いいのよね$"

CeladonCity_Gym_Text_TamiaDefeat::
	.string "No!$"
	.string "{JPN}{JPN}いやーん！$"

CeladonCity_Gym_Text_TamiaPostBattle::
	.string "We only use GRASS-type POKéMON at\n"
	.string "our GYM.\p"
	.string "Why? We also use them for making\n"
	.string "flower arrangements!$"
	.string "{JPN}{JPN}うちの　ジムでは　くさタイプの　ポケモンしか\n"
	.string "つかわないの\p"
	.string "なぜかって？　いけばなにも　つかうからよ！$"

CeladonCity_Gym_Text_LisaIntro::
	.string "Oh, hey!\p"
	.string "We don't like BUG- or FIRE-type\n"
	.string "POKéMON in here!$"
	.string "{JPN}{JPN}ちょっと　あなた！\p"
	.string "ここでは　むしタイプや　ほのおタイプの\n"
	.string "ポケモンは　おことわりよ！$"

CeladonCity_Gym_Text_LisaDefeat::
	.string "Oh!\n"
	.string "You!$"
	.string "{JPN}{JPN}まあ！　あなたって！$"

CeladonCity_Gym_Text_LisaPostBattle::
	.string "Our LEADER ERIKA might be quiet,\n"
	.string "but she's famous around here.$"
	.string "{JPN}{JPN}リーダーの　エリカさまは　おとなしいけど　この\n"
	.string "あたりでは　ゆうめいなのよ$"

CeladonCity_Gym_Text_LoriIntro::
	.string "Pleased to meet you.\n"
	.string "My hobby is POKéMON training.$"
	.string "{JPN}{JPN}はじめまして　わたくしの　しゅみは　ポケモンを\n"
	.string "そだてる　ことですの$"

CeladonCity_Gym_Text_LoriDefeat::
	.string "Oh!\n"
	.string "Splendid!$"
	.string "{JPN}{JPN}まあ！　おみごと！$"

CeladonCity_Gym_Text_LoriPostBattle::
	.string "I have a blind date coming up.\n"
	.string "I have to learn to be polite,\l"
	.string "especially if I have to battle.$"
	.string "{JPN}{JPN}こんど　おみあいが　ありますの　しょうぶの\n"
	.string "ときも　れいぎただしく　しなくては$"

CeladonCity_Gym_Text_MaryIntro::
	.string "Welcome to CELADON GYM!\p"
	.string "You'd better not underestimate\n"
	.string "the nice ladies here.$"
	.string "{JPN}{JPN}タマムシジムへ　ようこそ！\p"
	.string "ここの　すてきな　おねえさまたちを　あまく\n"
	.string "みない　ほうが　いいわよ$"

CeladonCity_Gym_Text_MaryDefeat::
	.string "Oh!\n"
	.string "Beaten!$"
	.string "{JPN}{JPN}ああ！　まけたわ！$"

CeladonCity_Gym_Text_MaryPostBattle::
	.string "I didn't bring my best POKéMON.\n"
	.string "Wait until next time!$"
	.string "{JPN}{JPN}きょうは　いちばんの　ポケモンを　つれて\n"
	.string "こなかったの　こんどは　まけないわ！$"

CeladonCity_Gym_Text_GymStatue::
	.string "CELADON POKéMON GYM\n"
	.string "LEADER: ERIKA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"
	.string "{JPN}{JPN}タマムシ　ポケモンジム\n"
	.string "リーダー　エリカ\p"
	.string "かったひと:\n"
	.string "{RIVAL}$"

CeladonCity_Gym_Text_GymStatuePlayerWon::
	.string "CELADON POKéMON GYM\n"
	.string "LEADER: ERIKA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"
	.string "{JPN}{JPN}タマムシ　ポケモンジム\n"
	.string "リーダー　エリカ\p"
	.string "かったひと:\n"
	.string "{RIVAL}　{PLAYER}$"

