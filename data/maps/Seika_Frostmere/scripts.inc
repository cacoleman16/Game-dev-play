Seika_Frostmere_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Frostmere_QoLFly_OnTransition
	.byte 0

Seika_Frostmere_EventScript_TownSign::
	msgbox Seika_Frostmere_Text_TownSign, MSGBOX_SIGN
	end

Seika_Frostmere_Text_TownSign::
	.string "FROSTMERE VILLAGE\n"
	.string "Home of the coldest lake\l"
	.string "in SEIKA.$"
	.string "{JPN}{JPN}シモミズむら　セイカいち　つめたい　みずうみ$"

Seika_Frostmere_EventScript_GymSign::
	msgbox Seika_Frostmere_Text_GymSign, MSGBOX_SIGN
	end

Seika_Frostmere_Text_GymSign::
	.string "FROSTMERE POKéMON GYM\n"
	.string "LEADER: RIME\p"
	.string "The ice fisher who waits out\n"
	.string "the longest winter!$"
	.string "{JPN}{JPN}シモミズ　ポケモンジム　リーダー　ツララ\p"
	.string "いちばん　ながい　ふゆを　たえぬく　こおりの\n"
	.string "つりびと！$"

Seika_Frostmere_EventScript_Fisher::
	msgbox Seika_Frostmere_Text_Fisher, MSGBOX_NPC
	end

Seika_Frostmere_Text_Fisher::
	.string "In winter we cut holes in the ice\n"
	.string "and fish all night long.\p"
	.string "RIME can sit by one hole for a\n"
	.string "whole day without moving!$"
	.string "{JPN}{JPN}ふゆは　こおりに　あなを　あけて　ひとばんじゅう\n"
	.string "つりを　するんだ。\p"
	.string "ツララさんなんか　ひとつの　あなの　まえで　まる\n"
	.string "1にち　うごかずに　いられるんだぜ！$"

Seika_Frostmere_EventScript_OldWoman::
	msgbox Seika_Frostmere_Text_OldWoman, MSGBOX_NPC
	end

Seika_Frostmere_Text_OldWoman::
	.string "Strange folk in dark coats came\n"
	.string "through here last night.\p"
	.string "They were hauling something big,\n"
	.string "wrapped in cloth. Up north.$"
	.string "{JPN}{JPN}ゆうべ　くろい　コートの　あやしい　れんちゅうが\n"
	.string "ここを　とおって　いったんじゃ。\p"
	.string "ぬのに　つつんだ　おおきな　ものを　ひっぱって\n"
	.string "きたの　ほうへ　いったのじゃ。$"

Seika_Frostmere_EventScript_Boy::
	msgbox Seika_Frostmere_Text_Boy, MSGBOX_NPC
	end

Seika_Frostmere_Text_Boy::
	.string "I saw a LAPRAS in the lake once!\n"
	.string "It was singing to the snow!$"
	.string "{JPN}{JPN}ぼく　みずうみで　ラプラスを　みた　ことが\n"
	.string "あるんだ！　ゆきに　むかって　うたってたよ！$"

Seika_Frostmere_EventScript_Grunts::
	lock
	faceplayer
	playbgm MUS_ENCOUNTER_INTENSE, FALSE
	msgbox Seika_Frostmere_Text_GruntsTalk, MSGBOX_DEFAULT
	goto_if_defeated TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_1, Seika_Frostmere_EventScript_Grunt2
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_1, Seika_Frostmere_Text_Grunt1Defeat
	goto Seika_Frostmere_EventScript_Grunt2
	end

Seika_Frostmere_EventScript_Grunt2::
	msgbox Seika_Frostmere_Text_Grunt2Intro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SEIKA_NOVA_GRUNT_FROSTMERE_2, Seika_Frostmere_Text_Grunt2Defeat
	msgbox Seika_Frostmere_Text_GruntsAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_FROSTMERE_GRUNT1
	removeobject LOCALID_SEIKA_FROSTMERE_GRUNT2
	setflag FLAG_QOL_SEIKA_HIDE_GRUNTS_FROSTMERE
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	release
	end

Seika_Frostmere_Text_GruntsTalk::
	.string "...So we haul the bell all the\n"
	.string "way north? To the mountain?\p"
	.string "Orders are orders. The bell goes\n"
	.string "up MT. SKYREACH, and we-\p"
	.string "Huh? How long has this kid been\n"
	.string "standing there?\p"
	.string "You didn't hear a thing! Nothing\n"
	.string "about any bell!$"
	.string "{JPN}{JPN}‥で　この　かねを　きたまで　ずっと\n"
	.string "はこぶのか？　やまの　うえまで？\p"
	.string "めいれいは　めいれいだ。　かねを\n"
	.string "ソラトドキやまへ　はこんだら　おれたちは‥\p"
	.string "ん？　この　ガキ　いつから　そこに　いたんだ？\p"
	.string "おまえは　なにも　きいて　ない！　かねの\n"
	.string "ことなんか　なにも　な！$"

Seika_Frostmere_Text_Grunt1Defeat::
	.string "Ugh! Not in the snow!$"
	.string "{JPN}{JPN}ぐわっ！　ゆきの　なかで　まけるなんて！$"

Seika_Frostmere_Text_Grunt2Intro::
	.string "My turn! I'll freeze you out!$"
	.string "{JPN}{JPN}つぎは　おれだ！　こおりづけに　して　やるぜ！$"

Seika_Frostmere_Text_Grunt2Defeat::
	.string "Beaten by a kid... again?$"
	.string "{JPN}{JPN}ガキに　まけた‥　また　かよ！$"

Seika_Frostmere_Text_GruntsAfter::
	.string "Forget it! The bell's already on\n"
	.string "its way up the mountain.\p"
	.string "DENEB and ALTAIR will have it\n"
	.string "ringing before you get there!\p"
	.string "Come on, let's go!$"
	.string "{JPN}{JPN}もう　いい！　かねは　とっくに　やまを　のぼって\n"
	.string "いる　ところだ。\p"
	.string "おまえが　つく　まえに　デネブさまと\n"
	.string "アルタイルさまが　ならして　くれるぜ！\p"
	.string "おい　いくぞ！$"

Seika_Frostmere_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_FROSTMERE
	end
