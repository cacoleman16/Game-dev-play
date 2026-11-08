Seika_SkyreachPass_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_SkyreachPass_OnTransition
	.byte 0

Seika_SkyreachPass_EventScript_Sign::
	msgbox Seika_SkyreachPass_Text_Sign, MSGBOX_SIGN
	end

Seika_SkyreachPass_Text_Sign::
	.string "SKYREACH PASS\n"
	.string "DUSKPEAK - STARWATCH PLATEAU$"
	.string "{JPN}{JPN}ソラトドキとうげ　タソガレむらから\n"
	.string "ホシミこうげんへ$"

Seika_SkyreachPass_OnTransition::
	setvar VAR_TEMP_1, 0
	goto_if_unset FLAG_QOL_SEIKA_BADGE01, Seika_SkyreachPass_EventScript_GateShut
	goto_if_unset FLAG_QOL_SEIKA_BADGE02, Seika_SkyreachPass_EventScript_GateShut
	goto_if_unset FLAG_QOL_SEIKA_BADGE03, Seika_SkyreachPass_EventScript_GateShut
	goto_if_unset FLAG_QOL_SEIKA_BADGE04, Seika_SkyreachPass_EventScript_GateShut
	goto_if_unset FLAG_QOL_SEIKA_BADGE05, Seika_SkyreachPass_EventScript_GateShut
	goto_if_unset FLAG_QOL_SEIKA_BADGE06, Seika_SkyreachPass_EventScript_GateShut
	goto_if_unset FLAG_QOL_SEIKA_BADGE07, Seika_SkyreachPass_EventScript_GateShut
	goto_if_unset FLAG_QOL_SEIKA_BADGE08, Seika_SkyreachPass_EventScript_GateShut
	setvar VAR_TEMP_1, 1
Seika_SkyreachPass_EventScript_GateShut::
	end

Seika_SkyreachPass_EventScript_GateTrigger::
	lockall
	applymovement LOCALID_SEIKA_SKYREACH_GUARD, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_SEIKA_SKYREACH_GUARD, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Seika_SkyreachPass_Text_GuardStop, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	releaseall
	end

Seika_SkyreachPass_EventScript_Guard::
	lock
	faceplayer
	goto_if_eq VAR_TEMP_1, 1, Seika_SkyreachPass_EventScript_GuardPass
	msgbox Seika_SkyreachPass_Text_GuardStop, MSGBOX_DEFAULT
	release
	end

Seika_SkyreachPass_EventScript_GuardPass::
	msgbox Seika_SkyreachPass_Text_GuardPass, MSGBOX_DEFAULT
	release
	end

Seika_SkyreachPass_Text_GuardStop::
	.string "Halt! Past here is the climb to\n"
	.string "the SEIKA LEAGUE.\p"
	.string "Only trainers with all eight\n"
	.string "SEIKA BADGES may go up.\p"
	.string "Come back when you have them!$"
	.string "{JPN}{JPN}まて！　ここから　さきは　セイカリーグへの\n"
	.string "のぼりみちだ。\p"
	.string "セイカの　バッジを　8つ　ぜんぶ　もつ\n"
	.string "トレーナーしか　とおれん！\p"
	.string "そろえてから　でなおして　こい！$"

Seika_SkyreachPass_Text_GuardPass::
	.string "Eight SEIKA BADGES! Go on up.\p"
	.string "The path is steep. Use the\n"
	.string "steps in the cliffs, and mind\l"
	.string "the ledges.$"
	.string "{JPN}{JPN}セイカの　バッジ　8つ！　よし　とおれ！\p"
	.string "みちは　けわしいぞ。　がけの　かいだんを\n"
	.string "つかって　だんさに　きを　つけて　いけ。$"

Seika_SkyreachPass_EventScript_Ryusei::
	trainerbattle_single TRAINER_SEIKA_COOLTRAINER_RYUSEI, Seika_SkyreachPass_Text_RyuseiIntro, Seika_SkyreachPass_Text_RyuseiDefeat
	msgbox Seika_SkyreachPass_Text_RyuseiPost, MSGBOX_AUTOCLOSE
	end

Seika_SkyreachPass_Text_RyuseiIntro::
	.string "Eight BADGES, huh? Let's see if\n"
	.string "you earned them!$"
	.string "{JPN}{JPN}バッジ　8つか。　だてじゃ　ないか　ためさせて\n"
	.string "もらうぜ！$"

Seika_SkyreachPass_Text_RyuseiDefeat::
	.string "Yeah, you earned them.$"
	.string "{JPN}{JPN}ああ　だてじゃ　なかったな。$"

Seika_SkyreachPass_Text_RyuseiPost::
	.string "The pass has three cliffs. Look\n"
	.string "for the steps cut into each one.$"
	.string "{JPN}{JPN}この　とうげには　がけが　3つ　ある。\n"
	.string "それぞれに　きざまれた　かいだんを　さがせ。$"

Seika_SkyreachPass_EventScript_Hoshie::
	trainerbattle_single TRAINER_SEIKA_COOLTRAINER_HOSHIE, Seika_SkyreachPass_Text_HoshieIntro, Seika_SkyreachPass_Text_HoshieDefeat
	msgbox Seika_SkyreachPass_Text_HoshiePost, MSGBOX_AUTOCLOSE
	end

Seika_SkyreachPass_Text_HoshieIntro::
	.string "Halfway up and already tired?\n"
	.string "I'm not!$"
	.string "{JPN}{JPN}まだ　はんぶんなのに　もう　つかれた？　わたしは\n"
	.string "へっちゃらよ！$"

Seika_SkyreachPass_Text_HoshieDefeat::
	.string "Okay... now I'm tired.$"
	.string "{JPN}{JPN}うう‥　いまので　つかれちゃった。$"

Seika_SkyreachPass_Text_HoshiePost::
	.string "Going down is easy. Just hop the\n"
	.string "ledges! Going up is the hard part.$"
	.string "{JPN}{JPN}くだりは　かんたん。　だんさを　とびおりる\n"
	.string "だけ！　たいへんなのは　のぼりよ。$"

Seika_SkyreachPass_EventScript_Takeru::
	trainerbattle_single TRAINER_SEIKA_COOLTRAINER_TAKERU, Seika_SkyreachPass_Text_TakeruIntro, Seika_SkyreachPass_Text_TakeruDefeat
	msgbox Seika_SkyreachPass_Text_TakeruPost, MSGBOX_AUTOCLOSE
	end

Seika_SkyreachPass_Text_TakeruIntro::
	.string "My TYRANITAR grew up on this\n"
	.string "mountain. It knows every rock!$"
	.string "{JPN}{JPN}おれの　バンギラスは　この　やまそだち。　いわは\n"
	.string "ぜんぶ　しりつくしてるぜ！$"

Seika_SkyreachPass_Text_TakeruDefeat::
	.string "Crushed like gravel...$"
	.string "{JPN}{JPN}じゃりみたいに　くだかれた‥$"

Seika_SkyreachPass_Text_TakeruPost::
	.string "LARVITAR eat the mountain itself.\n"
	.string "One day there may be no pass!$"
	.string "{JPN}{JPN}ヨーギラスは　やまを　たべて　しまうんだ。\n"
	.string "いつか　とうげが　なくなるかもな！$"

Seika_SkyreachPass_EventScript_Misora::
	trainerbattle_single TRAINER_SEIKA_COOLTRAINER_MISORA, Seika_SkyreachPass_Text_MisoraIntro, Seika_SkyreachPass_Text_MisoraDefeat
	msgbox Seika_SkyreachPass_Text_MisoraPost, MSGBOX_AUTOCLOSE
	end

Seika_SkyreachPass_Text_MisoraIntro::
	.string "The LEAGUE is just ahead. You\n"
	.string "will have to beat me first!$"
	.string "{JPN}{JPN}リーグは　すぐ　そこ。　でも　まずは　わたしを\n"
	.string "たおして　いきなさい！$"

Seika_SkyreachPass_Text_MisoraDefeat::
	.string "Go on, then. You are ready.$"
	.string "{JPN}{JPN}さあ　いきなさい。　あなたなら　だいじょうぶ。$"

Seika_SkyreachPass_Text_MisoraPost::
	.string "From STARWATCH PLATEAU you can\n"
	.string "see every town in SEIKA.$"
	.string "{JPN}{JPN}ホシミこうげんからは　セイカの　まちが　ぜんぶ\n"
	.string "みえるのよ。$"
