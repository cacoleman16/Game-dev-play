Skyrift_Observatory_Roof_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, QoL_Skyrift_Roof_OnTransition
	.byte 0

Skyrift_Observatory_Roof_EventScript_Engine::
	msgbox Skyrift_Observatory_Roof_Text_Engine, MSGBOX_SIGN
	end

Skyrift_Observatory_Roof_Text_Engine::
	.string "The SKYRIFT ENGINE. Its masts\n"
	.string "point at the tear in the sky.\p"
	.string "Deep inside, something throbs\n"
	.string "like a heartbeat.$"
	.string "{JPN}{JPN}ソラサケエンジンだ。　いくつもの　はしらが\n"
	.string "そらの　さけめに　むけられて　いる。\p"
	.string "おくふかくで　なにかが　しんぞうの　ように\n"
	.string "どくどくと　みゃくうって　いる。$"

QoL_Skyrift_Roof_OnTransition::
	setvar VAR_TEMP_4, 0
	call_if_set FLAG_QOL_SKYRIFT_CLIMAX, QoL_Skyrift_Roof_Off
	setflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_VEGA
	setflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_SIRIUS
	call_if_set FLAG_QOL_SKYRIFT_VEGA_SAVED, QoL_Skyrift_Roof_ShowVega
	call_if_set FLAG_QOL_SKYRIFT_VEGA_LOST, QoL_Skyrift_Roof_ShowSirius
	end

QoL_Skyrift_Roof_Off::
	setvar VAR_TEMP_4, 1
	return

QoL_Skyrift_Roof_ShowVega::
	clearflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_VEGA
	return

QoL_Skyrift_Roof_ShowSirius::
	clearflag FLAG_QOL_SKYRIFT_HIDE_REMATCH_SIRIUS
	return

QoL_Skyrift_Roof_EventScript_Climax::
	lockall
	setvar VAR_TEMP_4, 1
	msgbox QoL_Skyrift_Roof_Text_Vega1, MSGBOX_DEFAULT
	call_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_TalkALTAIR
	call_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_TalkDENEB
	call_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_TalkMIRA
	msgbox QoL_Skyrift_Roof_Text_Vega2, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Roof_Text_Sirius1, MSGBOX_DEFAULT
	goto_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_BattleALTAIR
	goto_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_BattleDENEB
	goto_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_BattleMIRA
	trainerbattle_two_trainers TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_TalkALTAIR::
	msgbox QoL_Skyrift_Roof_Text_TalkALTAIR, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_BattleALTAIR::
	multi_fixed_2_vs_2 TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat, PARTNER_QOL_ALTAIR
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_TalkDENEB::
	msgbox QoL_Skyrift_Roof_Text_TalkDENEB, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_BattleDENEB::
	multi_fixed_2_vs_2 TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat, PARTNER_QOL_DENEB
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_TalkMIRA::
	msgbox QoL_Skyrift_Roof_Text_TalkMIRA, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_BattleMIRA::
	multi_fixed_2_vs_2 TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat, TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat, PARTNER_QOL_MIRA
	goto QoL_Skyrift_Roof_AfterBattle

QoL_Skyrift_Roof_AfterBattle::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_WON, QoL_Skyrift_Roof_Lost
	msgbox QoL_Skyrift_Roof_Text_Overload, MSGBOX_DEFAULT
	playse SE_THUNDER
	setvar VAR_0x8004, 3
	setvar VAR_0x8005, 3
	setvar VAR_0x8006, 20
	setvar VAR_0x8007, 4
	special ShakeCamera
	waitstate
	msgbox QoL_Skyrift_Roof_Text_Pulled, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Roof_Text_Choice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_LetGo
	setflag FLAG_QOL_SKYRIFT_VEGA_SAVED
	call_if_set FLAG_QOL_SKYRIFT_WISH_PEACE, QoL_Skyrift_Roof_PeaceSave
	msgbox QoL_Skyrift_Roof_Text_Saved, MSGBOX_DEFAULT
	call_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_SavedALTAIR
	call_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_SavedDENEB
	call_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_SavedMIRA
	msgbox QoL_Skyrift_Roof_Text_VegaGift, MSGBOX_DEFAULT
	givemon SPECIES_POIPOLE, 60, ITEM_NONE, ITEM_BEAST_BALL
	bufferspeciesname STR_VAR_1, SPECIES_POIPOLE
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message QoL_Skyrift_Roof_Text_Received
	waitmessage
	waitfanfare
	goto QoL_Skyrift_Roof_Epilogue

QoL_Skyrift_Roof_LetGo::
	setflag FLAG_QOL_SKYRIFT_VEGA_LOST
	msgbox QoL_Skyrift_Roof_Text_Falls, MSGBOX_DEFAULT
	closemessage
	playse SE_THUNDER
	fadescreen FADE_TO_WHITE
	removeobject LOCALID_SKYRIFT_VEGA
	fadescreen FADE_FROM_WHITE
	call_if_set FLAG_QOL_SKYRIFT_WISH_PEACE, QoL_Skyrift_Roof_PeaceLost
	call_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_LostALTAIR
	call_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_LostDENEB
	call_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_LostMIRA
	msgbox QoL_Skyrift_Roof_Text_SiriusVow, MSGBOX_DEFAULT
	msgbox QoL_Skyrift_Roof_Text_Cosmog, MSGBOX_DEFAULT
	givemon SPECIES_COSMOG, 60, ITEM_NONE, ITEM_BEAST_BALL
	bufferspeciesname STR_VAR_1, SPECIES_COSMOG
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message QoL_Skyrift_Roof_Text_Received
	waitmessage
	waitfanfare
	goto QoL_Skyrift_Roof_Epilogue

QoL_Skyrift_Roof_PeaceSave::
	msgbox QoL_Skyrift_Roof_Text_PeaceSave, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_PeaceLost::
	msgbox QoL_Skyrift_Roof_Text_PeaceLost, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_Epilogue::
	setflag FLAG_QOL_SKYRIFT_CLIMAX
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SKYRIFT_VEGA
	removeobject LOCALID_SKYRIFT_SIRIUS
	fadescreen FADE_FROM_BLACK
	goto_if_set FLAG_QOL_SKYRIFT_WITH_ALTAIR, QoL_Skyrift_Roof_AskALTAIR
	goto_if_set FLAG_QOL_SKYRIFT_WITH_DENEB, QoL_Skyrift_Roof_AskDENEB
	goto_if_set FLAG_QOL_SKYRIFT_WITH_MIRA, QoL_Skyrift_Roof_AskMIRA
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_TheEnd::
	playbgm MUS_RG_CREDITS, FALSE
	msgbox QoL_Skyrift_Roof_Text_TheEnd, MSGBOX_DEFAULT
	fadedefaultbgm
	releaseall
	end

QoL_Skyrift_Roof_Lost::
	releaseall
	end

QoL_Skyrift_Roof_EventScript_RematchVega::
	lock
	faceplayer
	msgbox QoL_Skyrift_Roof_Text_RematchVega, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_RematchNo
	trainerbattle_no_intro TRAINER_SKYRIFT_VEGA, QoL_Skyrift_Roof_Text_VegaDefeat
	release
	end

QoL_Skyrift_Roof_EventScript_RematchSirius::
	lock
	faceplayer
	msgbox QoL_Skyrift_Roof_Text_RematchSirius, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_RematchNo
	trainerbattle_no_intro TRAINER_SKYRIFT_SIRIUS, QoL_Skyrift_Roof_Text_SiriusDefeat
	release
	end

QoL_Skyrift_Roof_RematchNo::
	release
	end

QoL_Skyrift_Roof_AskALTAIR::
	msgbox QoL_Skyrift_Roof_Text_KeepALTAIR, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_PartALTAIR
	msgbox QoL_Skyrift_Roof_Text_KeepYesALTAIR, MSGBOX_DEFAULT
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_PartALTAIR::
	msgbox QoL_Skyrift_Roof_Text_KeepNoALTAIR, MSGBOX_DEFAULT
	call QoL_Skyrift_Inn_Clear
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_SavedALTAIR::
	msgbox QoL_Skyrift_Roof_Text_SavedALTAIR, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_LostALTAIR::
	msgbox QoL_Skyrift_Roof_Text_LostALTAIR, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_AskDENEB::
	msgbox QoL_Skyrift_Roof_Text_KeepDENEB, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_PartDENEB
	msgbox QoL_Skyrift_Roof_Text_KeepYesDENEB, MSGBOX_DEFAULT
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_PartDENEB::
	msgbox QoL_Skyrift_Roof_Text_KeepNoDENEB, MSGBOX_DEFAULT
	call QoL_Skyrift_Inn_Clear
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_SavedDENEB::
	msgbox QoL_Skyrift_Roof_Text_SavedDENEB, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_LostDENEB::
	msgbox QoL_Skyrift_Roof_Text_LostDENEB, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_AskMIRA::
	msgbox QoL_Skyrift_Roof_Text_KeepMIRA, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_Skyrift_Roof_PartMIRA
	msgbox QoL_Skyrift_Roof_Text_KeepYesMIRA, MSGBOX_DEFAULT
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_PartMIRA::
	msgbox QoL_Skyrift_Roof_Text_KeepNoMIRA, MSGBOX_DEFAULT
	call QoL_Skyrift_Inn_Clear
	goto QoL_Skyrift_Roof_TheEnd

QoL_Skyrift_Roof_SavedMIRA::
	msgbox QoL_Skyrift_Roof_Text_SavedMIRA, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_LostMIRA::
	msgbox QoL_Skyrift_Roof_Text_LostMIRA, MSGBOX_DEFAULT
	return

QoL_Skyrift_Roof_Text_Vega1::
	.string "VEGA: So the CHAMPION of two... no,\n"
	.string "three regions came all this way.\p"
	.string "And you brought a friend.$"
	.string "{JPN}{JPN}ベガ『ふたつ‥　いいえ　みっつの　ちほうの\n"
	.string "チャンピオンが　はるばる　ここまで　きたのね。\p"
	.string "しかも　おともだちまで　つれて。』$"

QoL_Skyrift_Roof_Text_TalkALTAIR::
	.string "ALTAIR: VEGA, stop! The star stone\n"
	.string "wasn't for this!\p"
	.string "VEGA: ALTAIR. Always loyal. Too\n"
	.string "loyal to ask questions.$"
	.string "{JPN}{JPN}アルタイル『ボス　やめろ！　スターストーンは\n"
	.string "こんな　ことの　ためじゃ　なかった！』\p"
	.string "ベガ『アルタイル。　いつも　ちゅうじつね。\n"
	.string "ちゅうじつすぎて　なにも　きこうと　しない。』$"

QoL_Skyrift_Roof_Text_TalkDENEB::
	.string "DENEB: VEGA... I paid for every\n"
	.string "bolt of this machine. Let me help\l"
	.string "you turn it off.\p"
	.string "VEGA: My clever DENEB. You never\n"
	.string "understood what we were buying.$"
	.string "{JPN}{JPN}デネブ『ベガさま‥　この　きかいは　ネジ\n"
	.string "1ぽんまで　わたしが　はらったの。　とめるのを\l"
	.string "てつだわせて。』\p"
	.string "ベガ『かしこい　デネブ。　なにを　かって\n"
	.string "いたのか　ついに　わからなかったのね。』$"

QoL_Skyrift_Roof_Text_TalkMIRA::
	.string "MIRA: Grandpa says the sky answers\n"
	.string "anyone who listens. Not anyone who\l"
	.string "shouts!\p"
	.string "VEGA: SEIJI's little star reader.\n"
	.string "How sweet.$"
	.string "{JPN}{JPN}ミラ『そらは　みみを　すます　ひとに\n"
	.string "こたえるって　おじいちゃんが　いってた。　さけぶ\l"
	.string "ひとじゃ　ないよ！』\p"
	.string "ベガ『セイジの　ちいさな　ほしよみさん。\n"
	.string "かわいらしい　こと。』$"

QoL_Skyrift_Roof_Text_Vega2::
	.string "VEGA: The SKYRIFT ENGINE will call\n"
	.string "DEOXYS back.\p"
	.string "And this time it will answer\n"
	.string "to me. SIRIUS!$"
	.string "{JPN}{JPN}ベガ『ソラサケエンジンが　デオキシスを\n"
	.string "よびもどすわ。\p"
	.string "こんどこそ　わたくしに　したがって　もらう。\n"
	.string "シリウス！』$"

QoL_Skyrift_Roof_Text_Sirius1::
	.string "SIRIUS: Ma'am.$"
	.string "{JPN}{JPN}シリウス『はっ。』$"

QoL_Skyrift_Roof_Text_VegaDefeat::
	.string "No... not again...$"
	.string "{JPN}{JPN}そんな‥　また‥$"

QoL_Skyrift_Roof_Text_SiriusDefeat::
	.string "Forgive me, VEGA.$"
	.string "{JPN}{JPN}おゆるしを　ベガさま。$"

QoL_Skyrift_Roof_Text_Overload::
	.string "VEGA: The ENGINE! It's pulling too\n"
	.string "much power... it's overloading!$"
	.string "{JPN}{JPN}ベガ『エンジンが！　ちからを　すいこみすぎて‥\n"
	.string "ぼうそう　して　いるわ！』$"

QoL_Skyrift_Roof_Text_Pulled::
	.string "The rift above the roof tears\n"
	.string "wider and pulls at VEGA!\p"
	.string "VEGA: Help... me...!$"
	.string "{JPN}{JPN}おくじょうの　うえの　さけめが　ひろがり　ベガを\n"
	.string "ひきこもうと　する！\p"
	.string "ベガ『たすけて‥！』$"

QoL_Skyrift_Roof_Text_Choice::
	.string "Take VEGA's hand?$"
	.string "{JPN}{JPN}ベガの　てを　つかむ？$"

QoL_Skyrift_Roof_Text_PeaceSave::
	.string "A warm light falls from the sky.\n"
	.string "The WISHING STAR's light!\p"
	.string "It wraps around VEGA and pulls\n"
	.string "her back with you.$"
	.string "{JPN}{JPN}そらから　あたたかい　ひかりが　ふって　きた。\n"
	.string "ねがいぼしの　ひかりだ！\p"
	.string "ひかりは　ベガを　つつみこみ　こちらへ\n"
	.string "ひきもどした。$"

QoL_Skyrift_Roof_Text_Saved::
	.string "You grab VEGA's hand and pull!\p"
	.string "The rift snaps shut above the\n"
	.string "roof. The ENGINE goes quiet.\p"
	.string "VEGA: ...You saved me. Even after\n"
	.string "everything.$"
	.string "{JPN}{JPN}ベガの　てを　つかんで　ひっぱった！\p"
	.string "おくじょうの　うえで　さけめが　ぴしゃりと\n"
	.string "とじた。　エンジンが　しずかに　なった。\p"
	.string "ベガ『‥わたくしを　たすけたのね。　あんな\n"
	.string "ことを　したのに。』$"

QoL_Skyrift_Roof_Text_SavedALTAIR::
	.string "ALTAIR: Told you, VEGA. The kid's\n"
	.string "the real thing.\p"
	.string "VEGA: Will you... help me take this\n"
	.string "machine apart, ALTAIR?\p"
	.string "ALTAIR: Rock by rock, boss.$"
	.string "{JPN}{JPN}アルタイル『いったろ　ボス。　こいつは\n"
	.string "ほんものだって。』\p"
	.string "ベガ『アルタイル‥　この　きかいを　ばらすのを\n"
	.string "てつだって　くれる？』\p"
	.string "アルタイル『いわ　ひとつずつな　ボス。』$"

QoL_Skyrift_Roof_Text_SavedDENEB::
	.string "DENEB: Oh, VEGA. You frightened me.\p"
	.string "VEGA: DENEB... sell it all. The\n"
	.string "engine, the island. Give the money\l"
	.string "to SEIKA's shrines.\p"
	.string "DENEB: Finally, a purchase I\n"
	.string "approve of.$"
	.string "{JPN}{JPN}デネブ『ああ　ベガさま。　ひやひや　したわ。』\p"
	.string "ベガ『デネブ‥　ぜんぶ　うりなさい。　エンジンも\n"
	.string "しまも。　おかねは　セイカの　やしろに\l"
	.string "きふするのよ。』\p"
	.string "デネブ『やっと　わたしも　なっとく　できる\n"
	.string "とりひきね。』$"

QoL_Skyrift_Roof_Text_SavedMIRA::
	.string "MIRA: You see? The sky DOES answer\n"
	.string "if you listen!\p"
	.string "VEGA: Perhaps I should have asked a\n"
	.string "star reader before I bought a\l"
	.string "telescope.$"
	.string "{JPN}{JPN}ミラ『ほらね？　みみを　すませば　そらは\n"
	.string "ちゃんと　こたえて　くれるんだよ！』\p"
	.string "ベガ『ぼうえんきょうを　かう　まえに　ほしよみに\n"
	.string "きいて　おくべきだったかしらね。』$"

QoL_Skyrift_Roof_Text_VegaGift::
	.string "VEGA: This little one came through\n"
	.string "the rift with me. It wants to go\l"
	.string "with you.$"
	.string "{JPN}{JPN}ベガ『この　こは　わたくしと　いっしょに\n"
	.string "さけめを　くぐって　きたの。　あなたと\l"
	.string "いきたいそうよ。』$"

QoL_Skyrift_Roof_Text_Received::
	.string "{PLAYER} received {STR_VAR_1}!$"
	.string "{JPN}{JPN}{PLAYER}は　{STR_VAR_1}を\n"
	.string "もらった！$"

QoL_Skyrift_Roof_Text_Falls::
	.string "You hesitate... and VEGA's hand\n"
	.string "slips away.\p"
	.string "The rift swallows her and snaps\n"
	.string "shut.$"
	.string "{JPN}{JPN}ためらった　しゅんかん‥　ベガの　てが\n"
	.string "すりぬけた。\p"
	.string "さけめは　ベガを　のみこみ　ぴしゃりと　とじた。$"

QoL_Skyrift_Roof_Text_PeaceLost::
	.string "As she fell, a warm light went\n"
	.string "with her. The WISHING STAR's\l"
	.string "light.\p"
	.string "Wherever she has gone, she\n"
	.string "is not alone.$"
	.string "{JPN}{JPN}おちて　いく　ベガに　あたたかい　ひかりが\n"
	.string "よりそって　いた。　ねがいぼしの　ひかりだ。\p"
	.string "どこへ　いったと　しても　ベガは　ひとりでは\n"
	.string "ない。$"

QoL_Skyrift_Roof_Text_LostALTAIR::
	.string "ALTAIR: ...Boss.\p"
	.string "...I'll keep digging. If there's\n"
	.string "a way to her, I'll find it.$"
	.string "{JPN}{JPN}アルタイル『‥ボス。\p"
	.string "‥おれは　ほりつづけるぜ。　ボスへの　みちが\n"
	.string "あるなら　かならず　みつけて　やる。』$"

QoL_Skyrift_Roof_Text_LostDENEB::
	.string "DENEB: She chose her sky.\p"
	.string "I suppose... I can stop paying\n"
	.string "for it now.$"
	.string "{JPN}{JPN}デネブ『じぶんの　そらを　えらんだのね。\p"
	.string "これで‥　もう　おかねを　はらわなくて\n"
	.string "いいのかしら。』$"

QoL_Skyrift_Roof_Text_LostMIRA::
	.string "MIRA: She's out there with the\n"
	.string "stars now.\p"
	.string "I'll find her on my chart one\n"
	.string "day. I promise.$"
	.string "{JPN}{JPN}ミラ『あのひとは　いま　ほしたちと\n"
	.string "いっしょなんだね。\p"
	.string "いつか　わたしの　せいざずで　みつけて　みせる。\n"
	.string "やくそくだよ。』$"

QoL_Skyrift_Roof_Text_SiriusVow::
	.string "SIRIUS: You let her go.\p"
	.string "I will find a way through that\n"
	.string "rift. I will wait here, on her\l"
	.string "roof, for as long as it takes.$"
	.string "{JPN}{JPN}シリウス『おまえは　てを　はなした。\p"
	.string "おれは　あの　さけめを　ぬける　みちを　さがす。\n"
	.string "ベガさまの　この　おくじょうで　いつまでも\l"
	.string "まつ。』$"

QoL_Skyrift_Roof_Text_Cosmog::
	.string "Something small drifts down from\n"
	.string "where the rift closed...\p"
	.string "It floats over to you.$"
	.string "{JPN}{JPN}さけめが　とじた　ところから　ちいさな　なにかが\n"
	.string "ふわりと　おりて　きた‥\p"
	.string "それは　こちらへ　ただよって　きた。$"

QoL_Skyrift_Roof_Text_KeepALTAIR::
	.string "ALTAIR: So... the boss is sorted,\n"
	.string "and I've got nowhere to be.\p"
	.string "Mind if I keep tagging along?$"
	.string "{JPN}{JPN}アルタイル『さて‥　ボスの　ことは　かたが\n"
	.string "ついたし　おれには　いく　あても　ない。\p"
	.string "このまま　おまえに　ついて　いっても　いいか？』$"

QoL_Skyrift_Roof_Text_KeepYesALTAIR::
	.string "ALTAIR: Ha! Then let's go find\n"
	.string "something worth digging up.$"
	.string "{JPN}{JPN}アルタイル『へっ！　なら　ほりだす　かちの　ある\n"
	.string "もんを　さがしに　いこうぜ。』$"

QoL_Skyrift_Roof_Text_KeepNoALTAIR::
	.string "ALTAIR: Fair enough. I'll be at the\n"
	.string "RIFTWAKE INN if you need muscle.$"
	.string "{JPN}{JPN}アルタイル『わかったよ。　ちからが　いる　ときは\n"
	.string "ハザマのやどに　いるぜ。』$"

QoL_Skyrift_Roof_Text_KeepDENEB::
	.string "DENEB: Well, darling. I'm out of a\n"
	.string "job now.\p"
	.string "May I keep traveling with you?$"
	.string "{JPN}{JPN}デネブ『さて　ダーリン。　わたし　しつぎょう\n"
	.string "しちゃったわ。\p"
	.string "このまま　ごいっしょ　しても　いいかしら？』$"

QoL_Skyrift_Roof_Text_KeepYesDENEB::
	.string "DENEB: Wonderful. I'll pack light.\n"
	.string "Three hats at most.$"
	.string "{JPN}{JPN}デネブ『すてき。　にもつは　すくなめに　するわ。\n"
	.string "ぼうしは　3つまでね。』$"

QoL_Skyrift_Roof_Text_KeepNoDENEB::
	.string "DENEB: Then I'll wait at the\n"
	.string "RIFTWAKE INN. Don't keep a lady\l"
	.string "waiting.$"
	.string "{JPN}{JPN}デネブ『じゃあ　ハザマのやどで　まってるわ。\n"
	.string "レディを　あまり　またせないでね。』$"

QoL_Skyrift_Roof_Text_KeepMIRA::
	.string "MIRA: Grandpa can wait a little\n"
	.string "longer, right?\p"
	.string "Can I keep traveling with you?$"
	.string "{JPN}{JPN}ミラ『おじいちゃんは　もう　すこし　まって\n"
	.string "くれるよね？\p"
	.string "これからも　いっしょに　たびして　いい？』$"

QoL_Skyrift_Roof_Text_KeepYesMIRA::
	.string "MIRA: Yay! I'll read the stars, you\n"
	.string "do the battling!$"
	.string "{JPN}{JPN}ミラ『やったー！　わたしが　ほしを　よむから\n"
	.string "バトルは　まかせたよ！』$"

QoL_Skyrift_Roof_Text_KeepNoMIRA::
	.string "MIRA: Okay... I'll be at the\n"
	.string "RIFTWAKE INN. Come get me anytime!$"
	.string "{JPN}{JPN}ミラ『そっか‥　ハザマのやどに　いるね。\n"
	.string "いつでも　むかえに　きてね！』$"

QoL_Skyrift_Roof_Text_TheEnd::
	.string "The SKYRIFT is closed.\n"
	.string "The STARFALL is truly over.\p"
	.string "STARFALL - THE END\p"
	.string "Thank you for playing!$"
	.string "{JPN}{JPN}ソラサケは　とじた。　スターフォールは\n"
	.string "ほんとうに　おわったのだ。\p"
	.string "スターフォール　ー　おわり\p"
	.string "あそんで　くれて　ありがとう！$"

QoL_Skyrift_Roof_Text_RematchVega::
	.string "VEGA: I come up here to watch the\n"
	.string "stars now. Just watch.\p"
	.string "Would you battle me? For old\n"
	.string "times' sake.$"
	.string "{JPN}{JPN}ベガ『いまは　ほしを　ながめに　ここへ　くるの。\n"
	.string "ただ　ながめる　だけ。\p"
	.string "しょうぶ　して　くださる？　むかしの\n"
	.string "よしみで。』$"

QoL_Skyrift_Roof_Text_RematchSirius::
	.string "SIRIUS: You again. Every night I\n"
	.string "wait for the rift to open.\p"
	.string "Battle me. It passes the time.$"
	.string "{JPN}{JPN}シリウス『また　おまえか。　おれは　まいばん\n"
	.string "さけめが　ひらくのを　まって　いる。\p"
	.string "しょうぶしろ。　ひまつぶしに　なる。』$"
