Seika_Lanternport_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Lanternport_QoLFly_OnTransition
	.byte 0

Seika_Lanternport_EventScript_TownSign::
	msgbox Seika_Lanternport_Text_TownSign, MSGBOX_SIGN
	end

Seika_Lanternport_Text_TownSign::
	.string "LANTERNPORT\n"
	.string "Where lanterns light the way\l"
	.string "for falling stars.$"
	.string "{JPN}{JPN}トモシビタウン　おちる　ほしの　みちを\n"
	.string "ランタンが　てらす　みなと$"

Seika_Lanternport_EventScript_GymSign::
	msgbox Seika_Lanternport_Text_GymSign, MSGBOX_SIGN
	end

Seika_Lanternport_Text_GymSign::
	.string "LANTERNPORT POKéMON GYM\n"
	.string "LEADER: WICK\p"
	.string "The lantern maker whose\n"
	.string "flame never goes out!$"
	.string "{JPN}{JPN}トモシビ　ポケモンジム　リーダー　ウィック\p"
	.string "けっして　きえない　ほのおの\n"
	.string "ランタンしょくにん！$"

Seika_Lanternport_EventScript_HallSign::
	msgbox Seika_Lanternport_Text_HallSign, MSGBOX_SIGN
	end

Seika_Lanternport_Text_HallSign::
	.string "LANTERN HALL\n"
	.string "Starwatch Festival office$"
	.string "{JPN}{JPN}ランタンホール　ほしみまつり　じむきょく$"

Seika_Lanternport_EventScript_Elder::
	msgbox Seika_Lanternport_Text_Elder, MSGBOX_NPC
	end

Seika_Lanternport_Text_Elder::
	.string "Welcome to SEIKA, traveler.\p"
	.string "A thousand years ago, the first\n"
	.string "STARFALL came down over this land.\p"
	.string "Our ancestors wrote that one small\n"
	.string "star fell on MT. SKYREACH and\l"
	.string "went to sleep there.\p"
	.string "The WISHING STAR, they called it.\n"
	.string "It wakes for seven days every\l"
	.string "thousand years.\p"
	.string "We light lanterns so it can find\n"
	.string "its way. This year... the sky\l"
	.string "fell again. Perhaps it is awake.$"
	.string "{JPN}{JPN}ようこそ　セイカへ　たびの　かた。\p"
	.string "せんねんまえ　さいしょの　スターフォールが　この\n"
	.string "ちに　ふりそそいだ。\p"
	.string "ちいさな　ほしが　ひとつ　ソラトドキやまに\n"
	.string "おちて　ねむりに　ついたと　ごせんぞさまは\l"
	.string "かきのこして　おる。\p"
	.string "それを　ねがいぼしと　よんだ。　せんねんに\n"
	.string "いちど　7かかん　だけ　めを　さますのじゃ。\p"
	.string "ほしが　まよわぬよう　わしらは　ランタンを\n"
	.string "ともす。　ことし‥　また　そらが　ふった。　もう\l"
	.string "めざめて　おるのかも　しれんのう。$"

Seika_Lanternport_EventScript_LanternGirl::
	msgbox Seika_Lanternport_Text_LanternGirl, MSGBOX_NPC
	end

Seika_Lanternport_Text_LanternGirl::
	.string "Every night of the festival we\n"
	.string "send lanterns up into the sky!\p"
	.string "WICK makes all of them. His\n"
	.string "lanterns never, ever go out!$"
	.string "{JPN}{JPN}おまつりの　あいだ　まいばん　ランタンを　そらに\n"
	.string "とばすの！\p"
	.string "ぜんぶ　ウィックさんが　つくるの。　その\n"
	.string "ランタンは　ぜったいに　きえないんだよ！$"

Seika_Lanternport_EventScript_Fisher::
	msgbox Seika_Lanternport_Text_Fisher, MSGBOX_NPC
	end

Seika_Lanternport_Text_Fisher::
	.string "The CHINCHOU out here light up\n"
	.string "the sea at night.\p"
	.string "That's how this town got its\n"
	.string "name, if you ask me.$"
	.string "{JPN}{JPN}この　あたりの　チョンチーは　よるに　なると\n"
	.string "うみを　てらすんだ。\p"
	.string "この　まちの　なまえも　そこから　きたんじゃ\n"
	.string "ないかな。$"

Seika_Lanternport_EventScript_Woman::
	msgbox Seika_Lanternport_Text_Woman, MSGBOX_NPC
	end

Seika_Lanternport_Text_Woman::
	.string "LANTERN ROAD goes north to\n"
	.string "INKBRUSH TOWN.\p"
	.string "The scribes there keep the oldest\n"
	.string "record of the STARFALL.$"
	.string "{JPN}{JPN}トモシビかいどうを　きたへ　いくと\n"
	.string "スミフデタウンよ。\p"
	.string "あそこの　まきものかんには　スターフォールの\n"
	.string "いちばん　ふるい　きろくが　あるの。$"

Seika_Lanternport_EventScript_Sailor::
	lock
	faceplayer
	msgbox QoL_Text_OfferKanto, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, QoL_EventScript_SailToKanto
	msgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT
	release
	end

Seika_Lanternport_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_LANTERNPORT
	end
