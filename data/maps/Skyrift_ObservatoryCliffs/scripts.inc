Skyrift_ObservatoryCliffs_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, QoL_Skyrift_Gate_OnTransition
	.byte 0

Skyrift_ObservatoryCliffs_EventScript_Sign::
	msgbox Skyrift_ObservatoryCliffs_Text_Sign, MSGBOX_SIGN
	end

Skyrift_ObservatoryCliffs_Text_Sign::
	.string "OBSERVATORY CLIFFS\n"
	.string "NOVA OBSERVATORY ahead$"
	.string "{JPN}{JPN}テンモンがけ\n"
	.string "この　さき　ノバてんもんだい$"

Skyrift_ObservatoryCliffs_EventScript_ObsSign::
	msgbox Skyrift_ObservatoryCliffs_Text_ObsSign, MSGBOX_SIGN
	end

Skyrift_ObservatoryCliffs_Text_ObsSign::
	.string "NOVA OBSERVATORY\p"
	.string "We watch the sky so the sky\n"
	.string "will answer us.  - VEGA$"
	.string "{JPN}{JPN}ノバてんもんだい\p"
	.string "われらは　そらを　みる。　そらが　われらに\n"
	.string "こたえる　ように。　ー　ベガ$"

Skyrift_ObservatoryCliffs_EventScript_ItemPPMax::
	finditem ITEM_PP_MAX
	end

Skyrift_ObservatoryCliffs_EventScript_Kanoa::
	trainerbattle_single TRAINER_SKYRIFT_BLACK_BELT_KANOA, Skyrift_ObservatoryCliffs_Text_KanoaIntro, Skyrift_ObservatoryCliffs_Text_KanoaDefeat
	msgbox Skyrift_ObservatoryCliffs_Text_KanoaPost, MSGBOX_AUTOCLOSE
	end

Skyrift_ObservatoryCliffs_Text_KanoaIntro::
	.string "I climb these cliffs every\n"
	.string "morning. Two at a time? Fine!$"
	.string "{JPN}{JPN}まいあさ　この　がけを　のぼって　いるんだ。\n"
	.string "ふたり　いっぺんに？　いいとも！$"

Skyrift_ObservatoryCliffs_Text_KanoaDefeat::
	.string "My legs... are jelly...$"
	.string "{JPN}{JPN}あしが‥　ガクガクだ‥$"

Skyrift_ObservatoryCliffs_Text_KanoaPost::
	.string "Use the steps going up. Going\n"
	.string "down, the ledge is quicker.$"
	.string "{JPN}{JPN}のぼりは　かいだんを　つかいな。　くだりは\n"
	.string "だんさの　ほうが　はやいぞ。$"

Skyrift_ObservatoryCliffs_EventScript_Maile::
	trainerbattle_single TRAINER_SKYRIFT_COOLTRAINER_MAILE, Skyrift_ObservatoryCliffs_Text_MaileIntro, Skyrift_ObservatoryCliffs_Text_MaileDefeat
	msgbox Skyrift_ObservatoryCliffs_Text_MailePost, MSGBOX_AUTOCLOSE
	end

Skyrift_ObservatoryCliffs_Text_MaileIntro::
	.string "I came to see the OBSERVATORY.\n"
	.string "They wouldn't let me in!\p"
	.string "So battle me instead!$"
	.string "{JPN}{JPN}ノバてんもんだいを　みに　きたのに　なかに\n"
	.string "いれて　もらえなかったの！\p"
	.string "だから　かわりに　しょうぶして！$"

Skyrift_ObservatoryCliffs_Text_MaileDefeat::
	.string "That cheered me up!$"
	.string "{JPN}{JPN}おかげで　すっきり　したわ！$"

Skyrift_ObservatoryCliffs_Text_MailePost::
	.string "The big man at the top says the\n"
	.string "door is closed. For everyone.$"
	.string "{JPN}{JPN}てっぺんの　おおきな　おとこの　ひとが　とびらは\n"
	.string "しまってるって。　だれで　あってもね。$"

Skyrift_ObservatoryCliffs_EventScript_Nova::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_CLIFFS, Skyrift_ObservatoryCliffs_Text_NovaIntro, Skyrift_ObservatoryCliffs_Text_NovaDefeat
	msgbox Skyrift_ObservatoryCliffs_Text_NovaPost, MSGBOX_AUTOCLOSE
	end

Skyrift_ObservatoryCliffs_Text_NovaIntro::
	.string "You climbed all this way? Then\n"
	.string "you'll fall all this way!$"
	.string "{JPN}{JPN}ここまで　のぼって　きたのか？　なら　ここから\n"
	.string "まっさかさまに　おちて　もらうぜ！$"

Skyrift_ObservatoryCliffs_Text_NovaDefeat::
	.string "Fell... right off the cliff...$"
	.string "{JPN}{JPN}おちた‥　がけから　まっさかさま‥$"

Skyrift_ObservatoryCliffs_Text_NovaPost::
	.string "SIRIUS guards the gate up there.\n"
	.string "He has never lost. Not once.$"
	.string "{JPN}{JPN}うえの　もんは　シリウスが　まもって　いる。\n"
	.string "あいつは　いちども　まけた　ことが　ないんだ。$"

QoL_Skyrift_EventScript_RiftXURKITREE::
	lock
	faceplayer
	msgbox QoL_Skyrift_Text_RiftHum, MSGBOX_DEFAULT
	playmoncry SPECIES_XURKITREE, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_XURKITREE, 75
	settotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, QoL_Skyrift_RiftXURKITREE_Caught
	msgbox QoL_Skyrift_Text_RiftBack, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_RiftXURKITREE_Caught::
	setflag FLAG_QOL_SKYRIFT_UB_XURKITREE
	removeobject LOCALID_SKYRIFT_RIFT_XURKITREE
	msgbox QoL_Skyrift_Text_RiftClosed, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_EventScript_RiftKARTANA::
	lock
	faceplayer
	msgbox QoL_Skyrift_Text_RiftHum, MSGBOX_DEFAULT
	playmoncry SPECIES_KARTANA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_KARTANA, 75
	settotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, QoL_Skyrift_RiftKARTANA_Caught
	msgbox QoL_Skyrift_Text_RiftBack, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_RiftKARTANA_Caught::
	setflag FLAG_QOL_SKYRIFT_UB_KARTANA
	removeobject LOCALID_SKYRIFT_RIFT_KARTANA
	msgbox QoL_Skyrift_Text_RiftClosed, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Text_RiftHum::
	.string "The air around it bends and\n"
	.string "shimmers. Rift energy pours\l"
	.string "out of the tear!\p"
	.string "An ULTRA BEAST, wrapped in\n"
	.string "the rift's power!$"
	.string "{JPN}{JPN}まわりの　くうきが　ゆがんで　ゆらめいて　いる。\n"
	.string "さけめから　エネルギーが　あふれだす！\p"
	.string "さけめの　ちからを　まとった\n"
	.string "ウルトラビーストだ！$"

QoL_Skyrift_Text_RiftBack::
	.string "It slipped back toward the\n"
	.string "rift... It's still here.$"
	.string "{JPN}{JPN}それは　さけめの　ほうへ　すべるように\n"
	.string "もどった‥　まだ　ここに　いる。$"

QoL_Skyrift_Text_RiftClosed::
	.string "With the ULTRA BEAST gone,\n"
	.string "the little rift closed.$"
	.string "{JPN}{JPN}ウルトラビーストが　いなく　なり　ちいさな\n"
	.string "さけめは　とじた。$"

QoL_Skyrift_Gate_OnTransition::
	setvar VAR_TEMP_4, 0
	call_if_set FLAG_QOL_SKYRIFT_SIRIUS_GATE, QoL_Skyrift_Gate_Off
	end

QoL_Skyrift_Gate_Off::
	setvar VAR_TEMP_4, 1
	return

QoL_Skyrift_Gate_EventScript_Sirius::
	lockall
	setvar VAR_TEMP_4, 1
	applymovement LOCALID_SKYRIFT_SIRIUS_GATE, Common_Movement_FaceDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_SKYRIFT_SIRIUS_GATE, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox QoL_Skyrift_Gate_Text_Intro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SKYRIFT_SIRIUS_GATE, QoL_Skyrift_Gate_Text_Defeat
	msgbox QoL_Skyrift_Gate_Text_After, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SKYRIFT_SIRIUS_GATE
	setflag FLAG_QOL_SKYRIFT_SIRIUS_GATE
	fadescreen FADE_FROM_BLACK
	releaseall
	end

QoL_Skyrift_Gate_Text_Intro::
	.string "SIRIUS: Stop right there.\p"
	.string "I am SIRIUS. VEGA's orders are\n"
	.string "simple: no one reaches the\l"
	.string "OBSERVATORY.\p"
	.string "Not a CHAMPION from SEIKA,\n"
	.string "and not whoever walks\l"
	.string "behind you.$"
	.string "{JPN}{JPN}シリウス『そこで　とまれ。\p"
	.string "おれは　シリウス。　ベガさまの　めいれいは\n"
	.string "ひとつだ。　だれも　ノバてんもんだいには\l"
	.string "いかせない。\p"
	.string "セイカの　チャンピオンだろうと　おまえの\n"
	.string "うしろを　あるく　ものだろうと　だ。』$"

QoL_Skyrift_Gate_Text_Defeat::
	.string "Tch. Fine.$"
	.string "{JPN}{JPN}ちっ。　いいだろう。$"

QoL_Skyrift_Gate_Text_After::
	.string "SIRIUS: She's on the roof, starting\n"
	.string "the SKYRIFT ENGINE.\p"
	.string "Come up if you must. I will\n"
	.string "be waiting at her side.$"
	.string "{JPN}{JPN}シリウス『ベガさまは　おくじょうで\n"
	.string "ソラサケエンジンを　うごかして　いる。\p"
	.string "くるなら　こい。　おれは　ベガさまの　そばで\n"
	.string "まって　いる。』$"
