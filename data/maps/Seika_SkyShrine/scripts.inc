Seika_SkyShrine_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_SkyShrine_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, Seika_SkyShrine_OnResume
	.byte 0

Seika_SkyShrine_OnTransition::
	call_if_unset FLAG_QOL_SEIKA_SHRINE_NOVA, Seika_SkyShrine_EventScript_HideStar
	call_if_set FLAG_QOL_SEIKA_DONE, Seika_SkyShrine_EventScript_HideAll
	call_if_set FLAG_QOL_SEIKA_SHRINE_NOVA, Seika_SkyShrine_EventScript_SkipConfront
	end

Seika_SkyShrine_EventScript_HideStar::
	setflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	return

Seika_SkyShrine_EventScript_HideAll::
	setflag FLAG_QOL_SEIKA_HIDE_SHRINE_NOVA
	setflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	setvar VAR_TEMP_2, 1
	return

Seika_SkyShrine_EventScript_SkipConfront::
	setvar VAR_TEMP_2, 1
	return

Seika_SkyShrine_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, Seika_SkyShrine_EventScript_TryRemoveJirachi
	end

Seika_SkyShrine_EventScript_TryRemoveJirachi::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_SEIKA_SHRINE_JIRACHI
	return

Seika_SkyShrine_EventScript_Confront::
	lockall
	setvar VAR_TEMP_2, 1
	msgbox Seika_SkyShrine_Text_BellRings, MSGBOX_DEFAULT
	applymovement LOCALID_SEIKA_SHRINE_DENEB, Common_Movement_FaceDown
	applymovement LOCALID_SEIKA_SHRINE_ALTAIR, Common_Movement_FaceDown
	waitmovement 0
	msgbox Seika_SkyShrine_Text_DenebIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_DENEB_SHRINE, Seika_SkyShrine_Text_DenebDefeat
	msgbox Seika_SkyShrine_Text_AltairIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_ALTAIR_SHRINE, Seika_SkyShrine_Text_AltairDefeat
	setflag FLAG_QOL_SEIKA_SHRINE_NOVA
	msgbox Seika_SkyShrine_Text_NovaLeave, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_SHRINE_DENEB
	removeobject LOCALID_SEIKA_SHRINE_ALTAIR
	setflag FLAG_QOL_SEIKA_HIDE_SHRINE_NOVA
	fadescreen FADE_FROM_BLACK
	msgbox Seika_SkyShrine_Text_StarComes, MSGBOX_DEFAULT
	closemessage
	playse SE_M_DETECT
	clearflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	addobject LOCALID_SEIKA_SHRINE_JIRACHI
	delay 30
	releaseall
	end

Seika_SkyShrine_EventScript_Deneb::
	msgbox Seika_SkyShrine_Text_DenebIntro, MSGBOX_NPC
	end

Seika_SkyShrine_EventScript_Altair::
	msgbox Seika_SkyShrine_Text_AltairIntro, MSGBOX_NPC
	end

Seika_SkyShrine_EventScript_Jirachi::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_JIRACHI, CRY_MODE_ENCOUNTER
	msgbox Seika_SkyShrine_Text_Jirachi, MSGBOX_DEFAULT
	waitmoncry
	setwildbattle SPECIES_JIRACHI, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Seika_SkyShrine_EventScript_Ending
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Seika_SkyShrine_EventScript_StarFlewHome
	msgbox Seika_SkyShrine_Text_StillHere, MSGBOX_DEFAULT
	release
	end

Seika_SkyShrine_EventScript_StarFlewHome::
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_SHRINE_JIRACHI
	fadescreen FADE_FROM_BLACK
	msgbox Seika_SkyShrine_Text_FlewHome, MSGBOX_DEFAULT
Seika_SkyShrine_EventScript_Ending::
	setflag FLAG_QOL_SEIKA_HIDE_JIRACHI
	setflag FLAG_QOL_SEIKA_DONE
	msgbox Seika_SkyShrine_Text_Ending, MSGBOX_DEFAULT
	goto QoL_EventScript_SkyriftOpens

Seika_SkyShrine_Text_BellRings::
	.string "A clear note rings out over the\n"
	.string "clouds. The STARGLASS BELL!$"
	.string "{JPN}{JPN}すんだ　ねいろが　くもの　うえに\n"
	.string "ひびきわたった。　ほしガラスのかねだ！$"

Seika_SkyShrine_Text_DenebIntro::
	.string "DENEB: Darling, you are late.\n"
	.string "The bell is ringing.\p"
	.string "When the WISHING STAR comes down,\n"
	.string "our wish brings VEGA back to\l"
	.string "lead us. Stay out of it!{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}$"
	.string "{JPN}{JPN}デネブ『おそかったわね　ダーリン。　かねは　もう\n"
	.string "なって　いるわ。\p"
	.string "ねがいぼしが　おりて　きたら　わたしたちの\n"
	.string "ねがいで　ベガさまを　よびもどすの。　じゃまは\l"
	.string "させないわよ！』{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}$"

Seika_SkyShrine_Text_DenebDefeat::
	.string "Not again... Not on the last\n"
	.string "day!$"
	.string "{JPN}{JPN}また　なの‥？　さいごの　ひに　なって！$"

Seika_SkyShrine_Text_AltairIntro::
	.string "ALTAIR: I dug that star stone\n"
	.string "out of the ground with my hands.\p"
	.string "I won't let a kid from KANTO\n"
	.string "waste it. Come on!{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}$"
	.string "{JPN}{JPN}アルタイル『あの　スターストーンは　おれが　この\n"
	.string "てで　ほりだしたんだ。\p"
	.string "カントーの　ガキに　むだに　されて　たまるか。\n"
	.string "かかって　こい！』{PLAY_BGM}{MUS_ENCOUNTER_SUSPICIOUS}$"

Seika_SkyShrine_Text_AltairDefeat::
	.string "The bell... it stopped for\n"
	.string "you, not for us.$"
	.string "{JPN}{JPN}かねが‥　とまった。　おれたちじゃ　なく\n"
	.string "おまえの　ために。$"

Seika_SkyShrine_Text_NovaLeave::
	.string "DENEB: The star turned away from\n"
	.string "us. It is listening to you.\p"
	.string "ALTAIR: Let's go, DENEB.\n"
	.string "There is nothing left for us\l"
	.string "here.$"
	.string "{JPN}{JPN}デネブ『ほしが　そっぽを　むいたわ。　あなたの\n"
	.string "ねがいを　きいて　いるのね。』\p"
	.string "アルタイル『いくぞ　デネブ。　ここには　もう\n"
	.string "おれたちの　ばしょは　ない。』$"

Seika_SkyShrine_Text_StarComes::
	.string "The last note of the bell fades.\p"
	.string "A small light falls from the sky\n"
	.string "and lands on the shrine...$"
	.string "{JPN}{JPN}かねの　さいごの　ねいろが　きえて　いった。\p"
	.string "ちいさな　ひかりが　そらから　ふって　きて\n"
	.string "やしろに　おりたった‥$"

Seika_SkyShrine_Text_Jirachi::
	.string "Jiraaa!$"
	.string "{JPN}{JPN}ジラーー！$"

Seika_SkyShrine_Text_StillHere::
	.string "The WISHING STAR is still\n"
	.string "floating above the shrine...$"
	.string "{JPN}{JPN}ねがいぼしは　まだ　やしろの　うえに　うかんで\n"
	.string "いる‥$"

Seika_SkyShrine_Text_FlewHome::
	.string "The WISHING STAR rose into the\n"
	.string "sky and was gone.\p"
	.string "But it looked back at you, just\n"
	.string "once, before it went.$"
	.string "{JPN}{JPN}ねがいぼしは　そらへ　のぼり　みえなく　なった。\p"
	.string "でも　いく　まえに　いちどだけ　こちらを\n"
	.string "ふりかえった。$"

Seika_SkyShrine_Text_Ending::
	.string "Far below, the lanterns of\n"
	.string "SEIKA glow in the night.\p"
	.string "The second STARFALL is over.\n"
	.string "Its wish, whatever it was, is\l"
	.string "yours to keep.$"
	.string "{JPN}{JPN}はるか　したで　セイカの　ともしびが　よるの\n"
	.string "なかに　かがやいて　いる。\p"
	.string "2どめの　スターフォールは　おわった。　その\n"
	.string "ねがいが　なんで　あれ　それは　あなただけの\l"
	.string "ものだ。$"

QoL_EventScript_SkyriftWish::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SKYRIFT_WISH_DONE, Seika_SkyShrine_EventScript_Jirachi
	playmoncry SPECIES_JIRACHI, CRY_MODE_ENCOUNTER
	msgbox QoL_Skyrift_Wish_Text_Intro, MSGBOX_DEFAULT
	waitmoncry
QoL_Skyrift_Wish_Ask::
	message QoL_Skyrift_Wish_Text_Ask
	waitmessage
	dynmultipush QoL_Skyrift_Wish_Text_Strength, 0
	dynmultipush QoL_Skyrift_Wish_Text_Friend, 1
	dynmultipush QoL_Skyrift_Wish_Text_Fortune, 2
	dynmultipush QoL_Skyrift_Wish_Text_Peace, 3
	dynmultistack 0, 0, TRUE, 4, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, QoL_Skyrift_Wish_Strength
	case 1, QoL_Skyrift_Wish_Friend
	case 2, QoL_Skyrift_Wish_Fortune
	case 3, QoL_Skyrift_Wish_Peace
	goto QoL_Skyrift_Wish_Ask

QoL_Skyrift_Wish_Strength::
	msgbox QoL_Skyrift_Wish_Text_Granted, MSGBOX_DEFAULT
	giveitem ITEM_ABILITY_PATCH
	giveitem ITEM_GOLD_BOTTLE_CAP
	giveitem ITEM_RARE_CANDY, 5
	goto QoL_Skyrift_Wish_Test

QoL_Skyrift_Wish_Fortune::
	msgbox QoL_Skyrift_Wish_Text_Granted, MSGBOX_DEFAULT
	giveitem ITEM_SHINY_CHARM
	goto QoL_Skyrift_Wish_Test

QoL_Skyrift_Wish_Peace::
	setflag FLAG_QOL_SKYRIFT_WISH_PEACE
	msgbox QoL_Skyrift_Wish_Text_PeaceGranted, MSGBOX_DEFAULT
	giveitem ITEM_COMET_SHARD
	goto QoL_Skyrift_Wish_Test

QoL_Skyrift_Wish_Test::
	setflag FLAG_QOL_SKYRIFT_WISH_DONE
	msgbox QoL_Skyrift_Wish_Text_Test, MSGBOX_DEFAULT
	goto Seika_SkyShrine_EventScript_Jirachi

QoL_Skyrift_Wish_Friend::
	setflag FLAG_QOL_SKYRIFT_WISH_DONE
	msgbox QoL_Skyrift_Wish_Text_FriendGranted, MSGBOX_DEFAULT
	givemon SPECIES_JIRACHI, 70, ITEM_STAR_PIECE, ITEM_CHERISH_BALL
	bufferspeciesname STR_VAR_1, SPECIES_JIRACHI
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message QoL_Skyrift_Wish_Text_GotJirachi
	waitmessage
	waitfanfare
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_SHRINE_JIRACHI
	fadescreen FADE_FROM_BLACK
	goto Seika_SkyShrine_EventScript_Ending

QoL_EventScript_SkyriftOpens::
	setflag FLAG_QOL_SKYRIFT_OPEN
	delay 30
	playse SE_THUNDER
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 12
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	msgbox QoL_Skyrift_Wish_Text_SkyTears, MSGBOX_DEFAULT
	release
	end

QoL_Skyrift_Wish_Text_Intro::
	.string "The WISHING STAR opens its eyes\n"
	.string "and looks straight at you.\p"
	.string "It is waiting for your wish.$"
	.string "{JPN}{JPN}ねがいぼしが　めを　ひらき　まっすぐ　こちらを\n"
	.string "みつめた。\p"
	.string "あなたの　ねがいを　まって　いる。$"

QoL_Skyrift_Wish_Text_Ask::
	.string "What will you wish for?$"
	.string "{JPN}{JPN}なにを　ねがう？$"

QoL_Skyrift_Wish_Text_Strength::
	.string "STRENGTH$"
	.string "{JPN}{JPN}つよさ$"

QoL_Skyrift_Wish_Text_Friend::
	.string "A FRIEND$"
	.string "{JPN}{JPN}ともだち$"

QoL_Skyrift_Wish_Text_Fortune::
	.string "FORTUNE$"
	.string "{JPN}{JPN}こううん$"

QoL_Skyrift_Wish_Text_Peace::
	.string "PEACE FOR NOVA$"
	.string "{JPN}{JPN}ノバに　へいわを$"

QoL_Skyrift_Wish_Text_Granted::
	.string "The star glows brightly...\n"
	.string "Your wish is granted!$"
	.string "{JPN}{JPN}ほしが　まばゆく　かがやいた‥　ねがいが\n"
	.string "かなった！$"

QoL_Skyrift_Wish_Text_PeaceGranted::
	.string "The star is quiet for a long time.\p"
	.string "Then its light flies away across\n"
	.string "the sea, looking for someone.\p"
	.string "Something small falls into your\n"
	.string "hands.$"
	.string "{JPN}{JPN}ほしは　ながい　あいだ　だまって　いた。\p"
	.string "やがて　その　ひかりは　だれかを　さがす　ように\n"
	.string "うみの　むこうへ　とんで　いった。\p"
	.string "てのひらに　ちいさな　ものが　ぽとりと　おちて\n"
	.string "きた。$"

QoL_Skyrift_Wish_Text_Test::
	.string "The WISHING STAR spins in the air.\n"
	.string "It wants to see how strong you\l"
	.string "are!$"
	.string "{JPN}{JPN}ねがいぼしが　くうちゅうで　くるりと　まわった。\n"
	.string "あなたの　つよさを　たしかめたい　ようだ！$"

QoL_Skyrift_Wish_Text_FriendGranted::
	.string "The WISHING STAR laughs and floats\n"
	.string "right into your arms!$"
	.string "{JPN}{JPN}ねがいぼしは　わらって　あなたの　うでの　なかへ\n"
	.string "とびこんで　きた！$"

QoL_Skyrift_Wish_Text_GotJirachi::
	.string "{PLAYER} made a new friend:\n"
	.string "{STR_VAR_1}!$"
	.string "{JPN}{JPN}{PLAYER}に　あたらしい　ともだちが\n"
	.string "できた！　{STR_VAR_1}だ！$"

QoL_Skyrift_Wish_Text_SkyTears::
	.string "Far to the east, over the sea,\n"
	.string "the night sky TEARS open like\l"
	.string "paper.\p"
	.string "A crack of strange light hangs\n"
	.string "above the water.\p"
	.string "Someone will be waiting on\n"
	.string "LANTERNPORT's pier...$"
	.string "{JPN}{JPN}はるか　ひがしの　うみの　うえで　よぞらが\n"
	.string "かみの　ように　ビリビリと　さけた！\p"
	.string "ふしぎな　ひかりの　ひびわれが　うみの　うえに\n"
	.string "うかんで　いる。\p"
	.string "トモシビタウンの　さんばしで　だれかが　まって\n"
	.string "いる　はずだ‥$"
