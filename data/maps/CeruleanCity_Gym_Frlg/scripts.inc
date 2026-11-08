CeruleanCity_Gym_Frlg_MapScripts::
	.byte 0

CeruleanCity_Gym_EventScript_Misty::
	famechecker FAMECHECKER_MISTY, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, CeruleanCity_Gym_Text_MistyDefeat, CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC
	goto_if_unset FLAG_GOT_TM03_FROM_MISTY, CeruleanCity_Gym_EventScript_GiveTM03
	msgbox CeruleanCity_Gym_Text_ExplainTM03
	release
	end

CeruleanCity_Gym_EventScript_MistyDefeated::
	famechecker FAMECHECKER_MISTY, 1
	setflag FLAG_DEFEATED_MISTY
	setflag FLAG_BADGE02_GET
	setrespawn HEAL_LOCATION_CERULEAN_CITY
	set_gym_trainers_frlg 2
	goto CeruleanCity_Gym_EventScript_GiveTM03
	end

CeruleanCity_Gym_EventScript_GiveTM03::
	msgbox CeruleanCity_Gym_Text_ExplainCascadeBadge
	checkitemspace ITEM_TM03
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_Gym_EventScript_NoRoomForTM03
	giveitem_msg CeruleanCity_Gym_Text_ReceivedTM03FromMisty, ITEM_TM03
	setflag FLAG_GOT_TM03_FROM_MISTY
	msgbox CeruleanCity_Gym_Text_ExplainTM03
	release
	end

CeruleanCity_Gym_EventScript_NoRoomForTM03::
	msgbox CeruleanCity_Gym_Text_BetterMakeRoomForThis
	release
	end

CeruleanCity_Gym_EventScript_Diana::
	trainerbattle_single TRAINER_PICNICKER_DIANA, CeruleanCity_Gym_Text_DianaIntro, CeruleanCity_Gym_Text_DianaDefeat
	msgbox CeruleanCity_Gym_Text_DianaPostBattle, MSGBOX_AUTOCLOSE
	end

CeruleanCity_Gym_EventScript_Luis::
	trainerbattle_single TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, CeruleanCity_Gym_Text_LuisDefeat
	famechecker FAMECHECKER_MISTY, 2
	msgbox CeruleanCity_Gym_Text_LuisPostBattle, MSGBOX_AUTOCLOSE
	end

CeruleanCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MISTY, CeruleanCity_Gym_EventScript_GymGuyPostVictory
	msgbox CeruleanCity_Gym_Text_GymGuyAdvice
	release
	end

CeruleanCity_Gym_EventScript_GymGuyPostVictory::
	msgbox CeruleanCity_Gym_Text_WeMakePrettyGoodTeam
	release
	end

CeruleanCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, CeruleanCity_Gym_EventScript_GymStatuePostVictory
	msgbox CeruleanCity_Gym_Text_GymStatue
	releaseall
	end

CeruleanCity_Gym_EventScript_GymStatuePostVictory::
	msgbox CeruleanCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CeruleanCity_Gym_Text_MistyIntro::
	.string "Hi, you're a new face!\p"
	.string "Only those TRAINERS who have a\n"
	.string "policy about POKéMON can turn pro.\p"
	.string "What is your approach when you\n"
	.string "catch and train POKéMON?\p"
	.string "My policy is an all-out offensive\n"
	.string "with WATER-type POKéMON!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}あら　みない　かおね！\p"
	.string "ポケモンに　たいする　ポリシーを　もった\n"
	.string "トレーナーだけが　プロに　なれるのよ\p"
	.string "あなたは　どんな　やりかたで　ポケモンを\n"
	.string "つかまえて　そだててるの？\p"
	.string "わたしの　ポリシーは‥　みずタイプの　ポケモンで\n"
	.string "ガンガン　せめる　ことよ！{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

CeruleanCity_Gym_Text_ExplainTM03::
	.string "TM03 teaches WATER PULSE.\p"
	.string "Use it on an aquatic POKéMON!$"
	.string "{JPN}{JPN}わざマシン03は　みずのはどう\p"
	.string "みずの　ポケモンに　つかってね！$"

CeruleanCity_Gym_Text_ExplainCascadeBadge::
	.string "The CASCADEBADGE makes all\n"
	.string "POKéMON up to Lv. 30 obey.\p"
	.string "That includes even outsiders you\n"
	.string "got in trades.\p"
	.string "There's more. You can now use CUT\n"
	.string "anytime, even out of battle.\p"
	.string "You can CUT down small trees to\n"
	.string "open new pathways.\p"
	.string "You can also have my favorite TM.$"
	.string "{JPN}{JPN}ブルーバッジが　あれば　レベル30までの\n"
	.string "ポケモンが　いうことを　きくように　なるわ\p"
	.string "こうかんで　もらった　ポケモンも　ふくめてね\p"
	.string "それに　せんとうの　そとでも　いつでも\n"
	.string "いあいぎりが　つかえるの\p"
	.string "ちいさな　きを　いあいぎりで　きれば　あたらしい\n"
	.string "みちが　ひらけるわ\p"
	.string "わたしの　おきにいりの　わざマシンも　あげる$"

CeruleanCity_Gym_Text_ReceivedTM03FromMisty::
	.string "{PLAYER} received TM03\n"
	.string "from MISTY.$"
	.string "{JPN}{JPN}{PLAYER}は　カスミから　わざマシン03を\n"
	.string "もらった！$"

CeruleanCity_Gym_Text_BetterMakeRoomForThis::
	.string "You better make room for this!$"
	.string "{JPN}{JPN}にもつを　へらして　きなさいよ！$"

CeruleanCity_Gym_Text_MistyDefeat::
	.string "Wow!\n"
	.string "You're too much!\p"
	.string "All right!\p"
	.string "You can have the CASCADEBADGE to\n"
	.string "show you beat me.$"
	.string "{JPN}{JPN}うそっ！　あなた　つよすぎ！\p"
	.string "わかったわ！\p"
	.string "わたしに　かった　しるしに　ブルーバッジを\n"
	.string "あげるわ$"

CeruleanCity_Gym_Text_DianaIntro::
	.string "What? You?\n"
	.string "I'm more than good enough for you!\p"
	.string "MISTY won't have to be bothered.$"
	.string "{JPN}{JPN}なに？　あなた？　あなたの　あいては　わたしで\n"
	.string "じゅうぶんよ！\p"
	.string "カスミの　てを　わずらわせる　までも　ないわ$"

CeruleanCity_Gym_Text_DianaDefeat::
	.string "You overwhelmed me!$"
	.string "{JPN}{JPN}あっとう　されたわ！$"

CeruleanCity_Gym_Text_DianaPostBattle::
	.string "You have to face other TRAINERS\n"
	.string "to see how good you really are.$"
	.string "{JPN}{JPN}ほかの　トレーナーと　たたかって　こそ　じぶんの\n"
	.string "ほんとうの　じつりょくが　わかるのよ$"

CeruleanCity_Gym_Text_LuisIntro::
	.string "Splash!\p"
	.string "I'm first up!\n"
	.string "Let's do it!$"
	.string "{JPN}{JPN}ザブーン！\p"
	.string "まずは　ぼくが　あいてだ！　いくぞ！$"

CeruleanCity_Gym_Text_LuisDefeat::
	.string "That can't be!$"
	.string "{JPN}{JPN}そんな　ばかな！$"

CeruleanCity_Gym_Text_LuisPostBattle::
	.string "MISTY is a TRAINER who's going to\n"
	.string "keep improving.\p"
	.string "She won't lose to someone like you!$"
	.string "{JPN}{JPN}カスミは　これからも　どんどん　つよく　なる\n"
	.string "トレーナーなんだ\p"
	.string "きみなんかに　まける　もんか！$"

CeruleanCity_Gym_Text_GymGuyAdvice::
	.string "Yo!\n"
	.string "Champ in the making!\p"
	.string "Let me give you some advice!\p"
	.string "The LEADER, MISTY, is a pro who\n"
	.string "uses WATER-type POKéMON.\p"
	.string "You can drain all their water with\n"
	.string "GRASS-type POKéMON.\p"
	.string "Or, you might use ELECTRIC-type\n"
	.string "POKéMON and zap them!$"
	.string "{JPN}{JPN}おーす！　みらいの　チャンピオン！\p"
	.string "ちょっと　アドバイス　させてくれ！\p"
	.string "リーダーの　カスミは　みずタイプの　ポケモンを\n"
	.string "つかう　プロだ\p"
	.string "くさタイプの　ポケモンなら　みずを　すいとって\n"
	.string "しまえるぞ\p"
	.string "でんきタイプの　ポケモンで　しびれさせるのも\n"
	.string "ありだ！$"

CeruleanCity_Gym_Text_WeMakePrettyGoodTeam::
	.string "You beat MISTY!\n"
	.string "See, what'd I tell ya?\p"
	.string "You and me, kid, we make a pretty\n"
	.string "darn-good team!$"
	.string "{JPN}{JPN}カスミに　かったな！　ほら　おれの　いった\n"
	.string "とおりだろ？\p"
	.string "おれと　きみ　なかなか　いい　コンビじゃ\n"
	.string "ないか！$"

CeruleanCity_Gym_Text_GymStatue::
	.string "CERULEAN POKéMON GYM\n"
	.string "LEADER: MISTY\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"
	.string "{JPN}{JPN}ハナダ　ポケモンジム\n"
	.string "リーダー　カスミ\p"
	.string "かったひと:\n"
	.string "{RIVAL}$"

CeruleanCity_Gym_Text_GymStatuePlayerWon::
	.string "CERULEAN POKéMON GYM\n"
	.string "LEADER: MISTY\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"
	.string "{JPN}{JPN}ハナダ　ポケモンジム\n"
	.string "リーダー　カスミ\p"
	.string "かったひと:\n"
	.string "{RIVAL}　{PLAYER}$"

