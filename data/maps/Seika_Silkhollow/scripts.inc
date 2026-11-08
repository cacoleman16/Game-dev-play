Seika_Silkhollow_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Silkhollow_QoLFly_OnTransition
	.byte 0

Seika_Silkhollow_EventScript_TownSign::
	msgbox Seika_Silkhollow_Text_TownSign, MSGBOX_SIGN
	end

Seika_Silkhollow_Text_TownSign::
	.string "SILKHOLLOW TOWN\n"
	.string "Where silk is spun from\l"
	.string "moonlight.$"
	.string "{JPN}{JPN}キヌウツロタウン　つきの　いとを　つむぐ　まち$"

Seika_Silkhollow_EventScript_GymSign::
	msgbox Seika_Silkhollow_Text_GymSign, MSGBOX_SIGN
	end

Seika_Silkhollow_Text_GymSign::
	.string "SILKHOLLOW POKéMON GYM\n"
	.string "LEADER: AYAME\p"
	.string "The weaver who spins every\n"
	.string "battle into silk!$"
	.string "{JPN}{JPN}キヌウツロ　ポケモンジム　リーダー　アヤメ\p"
	.string "どんな　バトルも　きぬに　おりあげる\n"
	.string "はたおりびと！$"

Seika_Silkhollow_EventScript_RoadSign::
	msgbox Seika_Silkhollow_Text_RoadSign, MSGBOX_SIGN
	end

Seika_Silkhollow_Text_RoadSign::
	.string "WEAVER'S ROAD\n"
	.string "West to FROSTMERE VILLAGE$"
	.string "{JPN}{JPN}ハタオリかいどう　にしへ　シモミズむら$"

Seika_Silkhollow_EventScript_Dancer::
	msgbox Seika_Silkhollow_Text_Dancer, MSGBOX_NPC
	end

Seika_Silkhollow_Text_Dancer::
	.string "We dance on this square every\n"
	.string "night of the full moon.\p"
	.string "AYAME dances with us. Her steps\n"
	.string "are as light as a JUMPLUFF!$"
	.string "{JPN}{JPN}まんげつの　よるは　いつも　この　ひろばで\n"
	.string "おどるの。\p"
	.string "アヤメさんも　いっしょに　おどるのよ。\n"
	.string "ワタッコみたいに　かるい　ステップなの！$"

Seika_Silkhollow_EventScript_Weaver::
	msgbox Seika_Silkhollow_Text_Weaver, MSGBOX_NPC
	end

Seika_Silkhollow_Text_Weaver::
	.string "Our silk comes from ARIADOS.\n"
	.string "Their thread never breaks.\p"
	.string "We dye it with flowers and weave\n"
	.string "it by lantern light.$"
	.string "{JPN}{JPN}ここの　きぬは　アリアドスの　いとから　できて\n"
	.string "いるの。　ぜったいに　きれないのよ。\p"
	.string "はなで　そめて　ランタンの　あかりで　おるの。$"

Seika_Silkhollow_EventScript_OldMan::
	msgbox Seika_Silkhollow_Text_OldMan, MSGBOX_NPC
	end

Seika_Silkhollow_Text_OldMan::
	.string "WEAVER'S ROAD runs west to\n"
	.string "FROSTMERE VILLAGE.\p"
	.string "The wind off the lake there cuts\n"
	.string "like a knife. Bring a scarf!$"
	.string "{JPN}{JPN}ハタオリかいどうは　にしの　シモミズむらへ\n"
	.string "つづいて　おる。\p"
	.string "あそこの　みずうみの　かぜは　ナイフのように\n"
	.string "するどいぞ。　マフラーを　わすれるでないぞ！$"

Seika_Silkhollow_EventScript_Girl::
	msgbox Seika_Silkhollow_Text_Girl, MSGBOX_NPC
	end

Seika_Silkhollow_Text_Girl::
	.string "A man with muddy gloves ran\n"
	.string "through town and out to the west.\p"
	.string "He was holding a shiny stone,\n"
	.string "and laughing to himself!$"
	.string "{JPN}{JPN}どろだらけの　てぶくろの　おじさんが　まちを\n"
	.string "かけぬけて　にしへ　いったよ。\p"
	.string "ぴかぴかの　いしを　もって　ひとりで\n"
	.string "わらってたの！$"

Seika_Silkhollow_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_SILKHOLLOW
	end
