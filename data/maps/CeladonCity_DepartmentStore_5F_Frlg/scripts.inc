CeladonCity_DepartmentStore_5F_Frlg_MapScripts::
	.byte 0

@ [QoL] Hyper Training: the gentleman maxes a POKéMON's IVs for Bottle Caps.
@ A BOTTLE CAP trains one stat, a GOLD BOTTLE CAP trains all six.
@ Stat numbers follow the STAT_ order: 0 HP, 1 Atk, 2 Def, 3 Speed, 4 Sp. Atk, 5 Sp. Def.
CeladonCity_DepartmentStore_5F_EventScript_Gentleman::
	lock
	faceplayer
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainingIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainBye
	checkitem ITEM_BOTTLE_CAP
	copyvar VAR_0x8005, VAR_RESULT
	checkitem ITEM_GOLD_BOTTLE_CAP
	goto_if_eq VAR_RESULT, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainChooseMon
	goto_if_eq VAR_0x8005, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainChooseMon
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainNoCaps, MSGBOX_DEFAULT
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainChooseMon::
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainWhichMon, MSGBOX_DEFAULT
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainBye
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainEgg
	message CeladonCity_DepartmentStore_5F_Text_HyperTrainWhichStat
	waitmessage
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatHP, 0
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatAtk, 1
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatDef, 2
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatSpe, 3
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatSpA, 4
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatSpD, 5
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatAll, 6
	dynmultipush CeladonCity_DepartmentStore_5F_Text_StatCancel, 7
	dynmultistack 0, 0, FALSE, 8, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainHP
	case 1, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAtk
	case 2, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDef
	case 3, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainSpe
	case 4, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainSpA
	case 5, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainSpD
	case 6, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAll
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainBye

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainHP::
	checkitem ITEM_BOTTLE_CAP
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedCap
	canhypertrain 0, VAR_0x8004
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax
	removeitem ITEM_BOTTLE_CAP
	hypertrain 0, VAR_0x8004
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAtk::
	checkitem ITEM_BOTTLE_CAP
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedCap
	canhypertrain 1, VAR_0x8004
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax
	removeitem ITEM_BOTTLE_CAP
	hypertrain 1, VAR_0x8004
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDef::
	checkitem ITEM_BOTTLE_CAP
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedCap
	canhypertrain 2, VAR_0x8004
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax
	removeitem ITEM_BOTTLE_CAP
	hypertrain 2, VAR_0x8004
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainSpe::
	checkitem ITEM_BOTTLE_CAP
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedCap
	canhypertrain 3, VAR_0x8004
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax
	removeitem ITEM_BOTTLE_CAP
	hypertrain 3, VAR_0x8004
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainSpA::
	checkitem ITEM_BOTTLE_CAP
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedCap
	canhypertrain 4, VAR_0x8004
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax
	removeitem ITEM_BOTTLE_CAP
	hypertrain 4, VAR_0x8004
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainSpD::
	checkitem ITEM_BOTTLE_CAP
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedCap
	canhypertrain 5, VAR_0x8004
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax
	removeitem ITEM_BOTTLE_CAP
	hypertrain 5, VAR_0x8004
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone

@ Gold Bottle Cap: train every stat that isn't maxed yet. VAR_0x8006 counts them.
CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAll::
	checkitem ITEM_GOLD_BOTTLE_CAP
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedGoldCap
	setvar VAR_0x8006, 0
	canhypertrain 0, VAR_0x8004
	call_if_eq VAR_RESULT, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllHP
	canhypertrain 1, VAR_0x8004
	call_if_eq VAR_RESULT, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllAtk
	canhypertrain 2, VAR_0x8004
	call_if_eq VAR_RESULT, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllDef
	canhypertrain 3, VAR_0x8004
	call_if_eq VAR_RESULT, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllSpe
	canhypertrain 4, VAR_0x8004
	call_if_eq VAR_RESULT, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllSpA
	canhypertrain 5, VAR_0x8004
	call_if_eq VAR_RESULT, TRUE, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllSpD
	goto_if_eq VAR_0x8006, 0, CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax
	removeitem ITEM_GOLD_BOTTLE_CAP
	goto CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllHP::
	hypertrain 0, VAR_0x8004
	addvar VAR_0x8006, 1
	return

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllAtk::
	hypertrain 1, VAR_0x8004
	addvar VAR_0x8006, 1
	return

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllDef::
	hypertrain 2, VAR_0x8004
	addvar VAR_0x8006, 1
	return

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllSpe::
	hypertrain 3, VAR_0x8004
	addvar VAR_0x8006, 1
	return

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllSpA::
	hypertrain 4, VAR_0x8004
	addvar VAR_0x8006, 1
	return

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAllSpD::
	hypertrain 5, VAR_0x8004
	addvar VAR_0x8006, 1
	return

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainDone::
	playfanfare MUS_LEVEL_UP
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainDone, MSGBOX_DEFAULT
	waitfanfare
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainAlreadyMax::
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainAlreadyMax, MSGBOX_DEFAULT
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedCap::
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainNeedCap, MSGBOX_DEFAULT
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainNeedGoldCap::
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainNeedGoldCap, MSGBOX_DEFAULT
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainEgg::
	msgbox CeladonCity_DepartmentStore_5F_Text_HyperTrainEgg, MSGBOX_DEFAULT
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_HyperTrainBye::
	msgbox CeladonCity_DepartmentStore_5F_Text_ExplainStatEnhancers, MSGBOX_DEFAULT
	release
	end

CeladonCity_DepartmentStore_5F_Text_HyperTrainingIntro::
	.string "I'm a HYPER TRAINING coach.\p"
	.string "Bring me a BOTTLE CAP and I'll max\n"
	.string "out one stat's potential. A GOLD\l"
	.string "BOTTLE CAP maxes all of them.\p"
	.string "Shall I train one of your POKéMON?$"
	.string "{JPN}{JPN}わたしは　すごいとっくんの　コーチだ\p"
	.string "ぎんのおうかんを　もってくれば　のうりょくを\n"
	.string "ひとつ　さいこうまで　きたえて　やろう\l"
	.string "きんのおうかんなら　ぜんぶだ\p"
	.string "きみの　ポケモンを　きたえて　みるか？$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainNoCaps::
	.string "You don't have any BOTTLE CAPS.\n"
	.string "The clerk here sells them.$"
	.string "{JPN}{JPN}おうかんを　もって　いないようだな　ここの\n"
	.string "てんいんが　うっているぞ$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainWhichMon::
	.string "Which POKéMON should I train?$"
	.string "{JPN}{JPN}どの　ポケモンを　きたえる？$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainWhichStat::
	.string "Which stat?$"
	.string "{JPN}{JPN}どの　のうりょく？$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainDone::
	.string "Done! Its potential is maxed out.\n"
	.string "Keep training hard!$"
	.string "{JPN}{JPN}かんりょう！　さいこうまで　きたえたぞ！\n"
	.string "これからも　がんばれよ！$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainAlreadyMax::
	.string "That's already at its best.\n"
	.string "No need to spend a cap on it.$"
	.string "{JPN}{JPN}そこは　もう　さいこうだ　おうかんを　つかう\n"
	.string "ひつようは　ないぞ$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainNeedCap::
	.string "That needs a BOTTLE CAP.$"
	.string "{JPN}{JPN}それには　ぎんのおうかんが　いるぞ$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainNeedGoldCap::
	.string "Training every stat at once needs\n"
	.string "a GOLD BOTTLE CAP.$"
	.string "{JPN}{JPN}ぜんぶを　いっきに　きたえるには\n"
	.string "きんのおうかんが　いるぞ$"

CeladonCity_DepartmentStore_5F_Text_HyperTrainEgg::
	.string "An EGG can't be trained yet.$"
	.string "{JPN}{JPN}タマゴは　まだ　きたえられないぞ$"

CeladonCity_DepartmentStore_5F_Text_StatHP::
	.string "HP$"
	.string "{JPN}{JPN}HP$"

CeladonCity_DepartmentStore_5F_Text_StatAtk::
	.string "ATTACK$"
	.string "{JPN}{JPN}こうげき$"

CeladonCity_DepartmentStore_5F_Text_StatDef::
	.string "DEFENSE$"
	.string "{JPN}{JPN}ぼうぎょ$"

CeladonCity_DepartmentStore_5F_Text_StatSpe::
	.string "SPEED$"
	.string "{JPN}{JPN}すばやさ$"

CeladonCity_DepartmentStore_5F_Text_StatSpA::
	.string "SP. ATK$"
	.string "{JPN}{JPN}とくこう$"

CeladonCity_DepartmentStore_5F_Text_StatSpD::
	.string "SP. DEF$"
	.string "{JPN}{JPN}とくぼう$"

CeladonCity_DepartmentStore_5F_Text_StatAll::
	.string "All (GOLD CAP)$"
	.string "{JPN}{JPN}ぜんぶ　きんのおうかん$"

CeladonCity_DepartmentStore_5F_Text_StatCancel::
	.string "Cancel$"
	.string "{JPN}{JPN}やめる$"


CeladonCity_DepartmentStore_5F_EventScript_Sailor::
	msgbox CeladonCity_DepartmentStore_5F_Text_HereForStatEnhancers, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_5F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_5F_Text_Drugstore, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_5F_EventScript_ClerkXItems::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart CeladonCity_DepartmentStore_5F_XItems
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_5F_XItems::
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFEND
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_SPECIAL
	.2byte ITEM_X_ACCURACY
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_ClerkVitamins::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart CeladonCity_DepartmentStore_5F_Vitamins
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_5F_Vitamins::
	.2byte ITEM_HP_UP
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_CALCIUM
	.2byte ITEM_ZINC
	.2byte ITEM_CARBOS
	@ [QoL] Stat management: Bottle Caps (used by the Hyper Training gentleman
	@ on this floor), Ability Capsule/Patch, all Nature Mints and the six
	@ EV-lowering Berries.
	.2byte ITEM_BOTTLE_CAP
	.2byte ITEM_GOLD_BOTTLE_CAP
	.2byte ITEM_ABILITY_CAPSULE
	.2byte ITEM_ABILITY_PATCH
	.2byte ITEM_LONELY_MINT
	.2byte ITEM_ADAMANT_MINT
	.2byte ITEM_NAUGHTY_MINT
	.2byte ITEM_BRAVE_MINT
	.2byte ITEM_BOLD_MINT
	.2byte ITEM_IMPISH_MINT
	.2byte ITEM_LAX_MINT
	.2byte ITEM_RELAXED_MINT
	.2byte ITEM_MODEST_MINT
	.2byte ITEM_MILD_MINT
	.2byte ITEM_RASH_MINT
	.2byte ITEM_QUIET_MINT
	.2byte ITEM_CALM_MINT
	.2byte ITEM_GENTLE_MINT
	.2byte ITEM_CAREFUL_MINT
	.2byte ITEM_SASSY_MINT
	.2byte ITEM_TIMID_MINT
	.2byte ITEM_HASTY_MINT
	.2byte ITEM_JOLLY_MINT
	.2byte ITEM_NAIVE_MINT
	.2byte ITEM_SERIOUS_MINT
	.2byte ITEM_POMEG_BERRY
	.2byte ITEM_KELPSY_BERRY
	.2byte ITEM_QUALOT_BERRY
	.2byte ITEM_HONDEW_BERRY
	.2byte ITEM_GREPA_BERRY
	.2byte ITEM_TAMATO_BERRY
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_5F_Text_ExplainStatEnhancers::
	.string "POKéMON stat enhancers can be\n"
	.string "bought only here.\p"
	.string "HP UP increases the base HP of a\n"
	.string "POKéMON.\p"
	.string "CALCIUM raises the base SP. ATK\n"
	.string "stat of one POKéMON.\p"
	.string "ZINC boosts the base SP. DEF stat\n"
	.string "of one POKéMON.\p"
	.string "CARBOS enhances the base SPEED\n"
	.string "stat.$"
	.string "{JPN}{JPN}ポケモンの　のうりょくを　あげる　くすりは\n"
	.string "ここでしか　かえません\p"
	.string "マックスアップは　ポケモンの　HPを　あげます\p"
	.string "リゾチウムは　ポケモンの　とくこうを　あげます\p"
	.string "キトサンは　ポケモンの　とくぼうを　あげます\p"
	.string "インドメタシンは　すばやさを　あげます$"

CeladonCity_DepartmentStore_5F_Text_HereForStatEnhancers::
	.string "I'm here for POKéMON stat\n"
	.string "enhancers.\p"
	.string "PROTEIN increases the base ATTACK\n"
	.string "power.\p"
	.string "IRON increases the base DEFENSE\n"
	.string "stat.$"
	.string "{JPN}{JPN}ぼくは　ポケモンの　のうりょくを　あげる\n"
	.string "くすりを　かいに　きたんだ\p"
	.string "タウリンは　こうげきを　あげるんだ\p"
	.string "ブロムヘキシンは　ぼうぎょを　あげるよ$"

CeladonCity_DepartmentStore_5F_Text_Drugstore::
	.string "5F: DRUGSTORE$"
	.string "{JPN}{JPN}5F:　ドラッグストア$"

