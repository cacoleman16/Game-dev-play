Seika_StarwatchPlateau_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_StarwatchPlateau_QoLFly_OnTransition
	.byte 0

Seika_StarwatchPlateau_EventScript_LeagueSign::
	msgbox Seika_StarwatchPlateau_Text_LeagueSign, MSGBOX_SIGN
	end

Seika_StarwatchPlateau_Text_LeagueSign::
	.string "STARWATCH PLATEAU\p"
	.string "The SEIKA LEAGUE: the last\n"
	.string "watch below the stars.$"
	.string "{JPN}{JPN}ホシミこうげん\p"
	.string "セイカリーグ　ほしの　したの　さいごの　みはり$"

Seika_StarwatchPlateau_EventScript_CaveSign::
	msgbox Seika_StarwatchPlateau_Text_CaveSign, MSGBOX_SIGN
	end

Seika_StarwatchPlateau_Text_CaveSign::
	.string "MT. SKYREACH: SKY SHRINE\p"
	.string "Only the CHAMPION of the SEIKA\n"
	.string "LEAGUE may enter.$"
	.string "{JPN}{JPN}ソラトドキやま　そらのやしろ\p"
	.string "セイカリーグの　チャンピオンしか　はいれません$"

Seika_StarwatchPlateau_OnTransition::
	call_if_set FLAG_QOL_SEIKA_CHAMPION, Seika_StarwatchPlateau_EventScript_MonkAside
	end

Seika_StarwatchPlateau_EventScript_MonkAside::
	setobjectxyperm LOCALID_SEIKA_STARWATCH_MONK, 21, 8
	setobjectmovementtype LOCALID_SEIKA_STARWATCH_MONK, MOVEMENT_TYPE_FACE_LEFT
	return

Seika_StarwatchPlateau_EventScript_Monk::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_CHAMPION, Seika_StarwatchPlateau_EventScript_MonkChampion
	msgbox Seika_StarwatchPlateau_Text_MonkBlock, MSGBOX_DEFAULT
	release
	end

Seika_StarwatchPlateau_EventScript_MonkChampion::
	msgbox Seika_StarwatchPlateau_Text_MonkChampion, MSGBOX_DEFAULT
	release
	end

Seika_StarwatchPlateau_Text_MonkBlock::
	.string "This cave climbs to the summit of\n"
	.string "MT. SKYREACH and the SKY SHRINE.\p"
	.string "By an oath a thousand years old,\n"
	.string "only the CHAMPION of the SEIKA\l"
	.string "LEAGUE may enter.\p"
	.string "The LEAGUE is behind you, child.$"
	.string "{JPN}{JPN}この　どうくつは　ソラトドキやまの　いただき\n"
	.string "そらのやしろへと　つづいて　おる。\p"
	.string "せんねんの　ちかいに　より　ここに　はいれるのは\n"
	.string "セイカリーグの　チャンピオン　だけじゃ。\p"
	.string "リーグは　おまえさんの　うしろに　あるぞ。$"

Seika_StarwatchPlateau_Text_MonkChampion::
	.string "CHAMPION {PLAYER}. The mountain\n"
	.string "has been waiting for you.\p"
	.string "The SKY SHRINE is above. Go\n"
	.string "with the stars.$"
	.string "{JPN}{JPN}チャンピオン　{PLAYER}よ。　やまが\n"
	.string "おまえさんを　まって　おった。\p"
	.string "そらのやしろは　この　うえじゃ。　ほしと　ともに\n"
	.string "ゆくが　よい。$"

Seika_StarwatchPlateau_EventScript_Cooltrainer::
	msgbox Seika_StarwatchPlateau_Text_Cooltrainer, MSGBOX_NPC
	end

Seika_StarwatchPlateau_Text_Cooltrainer::
	.string "The SEIKA LEAGUE! I climbed all\n"
	.string "the way up here...\p"
	.string "And lost to the first ELITE\n"
	.string "FOUR member. Back down I go.$"
	.string "{JPN}{JPN}セイカリーグ！　はるばる　ここまで　のぼって\n"
	.string "きたのに‥\p"
	.string "してんのうの　ひとりめに　まけちゃった。　また\n"
	.string "したまで　おりるよ。$"

Seika_StarwatchPlateau_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_LEAGUE
	goto Seika_StarwatchPlateau_OnTransition
