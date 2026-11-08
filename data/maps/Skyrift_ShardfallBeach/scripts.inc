Skyrift_ShardfallBeach_MapScripts::
	.byte 0

Skyrift_ShardfallBeach_EventScript_Sign::
	msgbox Skyrift_ShardfallBeach_Text_Sign, MSGBOX_SIGN
	end

Skyrift_ShardfallBeach_Text_Sign::
	.string "SHARDFALL BEACH\n"
	.string "RIFTWAKE HARBOR - METEOR BASIN$"
	.string "{JPN}{JPN}カケラはま\n"
	.string "ハザマみなとから　インセキのくぼちへ$"

Skyrift_ShardfallBeach_EventScript_ItemStarPiece::
	finditem ITEM_STAR_PIECE
	end

Skyrift_ShardfallBeach_EventScript_Kai::
	trainerbattle_single TRAINER_SKYRIFT_RANGER_KAI, Skyrift_ShardfallBeach_Text_KaiIntro, Skyrift_ShardfallBeach_Text_KaiDefeat
	msgbox Skyrift_ShardfallBeach_Text_KaiPost, MSGBOX_AUTOCLOSE
	end

Skyrift_ShardfallBeach_Text_KaiIntro::
	.string "Shards fell all over this beach.\n"
	.string "The POKéMON here went wild!$"
	.string "{JPN}{JPN}この　はまじゅうに　かけらが　ふって　きたんだ。\n"
	.string "ここの　ポケモンたちが　あばれだしちゃった！$"

Skyrift_ShardfallBeach_Text_KaiDefeat::
	.string "Okay, okay! You win!$"
	.string "{JPN}{JPN}わかった　わかった！　きみの　かちだ！$"

Skyrift_ShardfallBeach_Text_KaiPost::
	.string "Those pale rocks? They fell out\n"
	.string "of the rift. They're warm to\l"
	.string "the touch.$"
	.string "{JPN}{JPN}あの　しろっぽい　いし？　さけめから　おちて\n"
	.string "きたんだ。　さわると　あったかいよ。$"

Skyrift_ShardfallBeach_EventScript_NovaA::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_BEACH_1, Skyrift_ShardfallBeach_Text_NovaAIntro, Skyrift_ShardfallBeach_Text_NovaADefeat
	msgbox Skyrift_ShardfallBeach_Text_NovaAPost, MSGBOX_AUTOCLOSE
	end

Skyrift_ShardfallBeach_Text_NovaAIntro::
	.string "Halt! This beach belongs to\n"
	.string "TEAM NOVA now.\p"
	.string "VEGA's orders: no one goes\n"
	.string "past the crystal rock!$"
	.string "{JPN}{JPN}とまれ！　この　はまは　もう　ノバだんの\n"
	.string "ものだ。\p"
	.string "ベガさまの　めいれいだ！　すいしょうの　いわより\n"
	.string "さきへは　だれも　とおさん！$"

Skyrift_ShardfallBeach_Text_NovaADefeat::
	.string "The stars... turned on me.$"
	.string "{JPN}{JPN}ほしが‥　おれを　みはなした‥$"

Skyrift_ShardfallBeach_Text_NovaAPost::
	.string "You'll never reach the\n"
	.string "OBSERVATORY. SIRIUS is up there.$"
	.string "{JPN}{JPN}ノバてんもんだいには　ぜったいに　いけないぞ。\n"
	.string "うえには　シリウスが　いるんだ。$"

Skyrift_ShardfallBeach_EventScript_NovaB::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_BEACH_2, Skyrift_ShardfallBeach_Text_NovaBIntro, Skyrift_ShardfallBeach_Text_NovaBDefeat
	msgbox Skyrift_ShardfallBeach_Text_NovaBPost, MSGBOX_AUTOCLOSE
	end

Skyrift_ShardfallBeach_Text_NovaBIntro::
	.string "Two of you? Then two of us!\n"
	.string "The rift gives us strength!$"
	.string "{JPN}{JPN}ふたりがかり？　なら　こっちも　ふたりよ！\n"
	.string "さけめが　わたしたちに　ちからを　くれるわ！$"

Skyrift_ShardfallBeach_Text_NovaBDefeat::
	.string "Our dream... is it slipping\n"
	.string "away?$"
	.string "{JPN}{JPN}わたしたちの　ゆめが‥　とおざかって　いく‥？$"

Skyrift_ShardfallBeach_Text_NovaBPost::
	.string "VEGA says the ENGINE will bring\n"
	.string "the sky's guardian back to us.$"
	.string "{JPN}{JPN}エンジンが　そらの　まもりがみを　よびもどして\n"
	.string "くれるって　ベガさまは　いうのよ。$"

Skyrift_ShardfallBeach_EventScript_Leilani::
	trainerbattle_single TRAINER_SKYRIFT_SWIMMER_LEILANI, Skyrift_ShardfallBeach_Text_LeilaniIntro, Skyrift_ShardfallBeach_Text_LeilaniDefeat
	msgbox Skyrift_ShardfallBeach_Text_LeilaniPost, MSGBOX_AUTOCLOSE
	end

Skyrift_ShardfallBeach_Text_LeilaniIntro::
	.string "The water sparkles at night now!\n"
	.string "It's full of tiny stars!$"
	.string "{JPN}{JPN}よるの　うみが　キラキラ　ひかるの！　ちいさな\n"
	.string "ほしで　いっぱいなのよ！$"

Skyrift_ShardfallBeach_Text_LeilaniDefeat::
	.string "I sank like a stone...$"
	.string "{JPN}{JPN}いしみたいに　しずんじゃった‥$"

Skyrift_ShardfallBeach_Text_LeilaniPost::
	.string "The shards dissolve in the sea.\n"
	.string "The STARYU eat them, I think!$"
	.string "{JPN}{JPN}かけらは　うみで　とけちゃうの。　ヒトデマンが\n"
	.string "たべてるんじゃ　ないかしら！$"

QoL_Skyrift_EventScript_RiftNIHILEGO::
	lock
	faceplayer
	msgbox QoL_Skyrift_Text_RiftHum, MSGBOX_DEFAULT
	playmoncry SPECIES_NIHILEGO, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_NIHILEGO, 75
	settotemboost B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, QoL_Skyrift_RiftNIHILEGO_Caught
	msgbox QoL_Skyrift_Text_RiftBack, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_RiftNIHILEGO_Caught::
	setflag FLAG_QOL_SKYRIFT_UB_NIHILEGO
	removeobject LOCALID_SKYRIFT_RIFT_NIHILEGO
	msgbox QoL_Skyrift_Text_RiftClosed, MSGBOX_DEFAULT
	release
	end
