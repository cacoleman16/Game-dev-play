OneIsland_KindleRoad_EmberSpa_Frlg_MapScripts::
	.byte 0

OneIsland_KindleRoad_EmberSpa_EventScript_OldMan::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_WaterWarmsMeToCore, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BaldingMan1::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_EnjoyBowlOfChowder, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BaldingMan2::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_HotSpringIsTherapeutic, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_OldWoman::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_SeeHowSmoothMySkinIs, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BlackBelt::
	lock
	faceplayer
	famechecker FAMECHECKER_BRUNO, 3
	msgbox OneIsland_KindleRoad_EmberSpa_Text_BrunoVisitsSpaOnOccasion
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_RockSmashMan::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM06, OneIsland_KindleRoad_EmberSpa_EventScript_AlreadyGotHM06
	msgbox OneIsland_KindleRoad_EmberSpa_Text_UsedThisToMakeEmberSpa
	giveitem ITEM_HM06
	setflag FLAG_GOT_HM06
	msgbox OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_AlreadyGotHM06::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_SpaHeal::
	lockall
	fadescreen FADE_TO_WHITE
	special HealPlayerParty
	fadescreen FADE_FROM_WHITE
	msgbox OneIsland_KindleRoad_EmberSpa_Text_WaterExquisiteFullyRefreshed
	setvar VAR_TEMP_1, 1
	releaseall
	end

OneIsland_KindleRoad_EmberSpa_Text_WaterWarmsMeToCore::
	.string "Hm!\n"
	.string "Haaaaaaaaaaah!\p"
	.string "Aaaah! This is heavenly!\n"
	.string "The water warms me to my core!\p"
	.string "You should jump in, too.\p"
	.string "Go to the middle, relax, and get\n"
	.string "good and warm!$"
	.string "{JPN}{JPN}むっ！　はあああああああ！\p"
	.string "あああ‥　ごくらく　ごくらく！　からだの\n"
	.string "しんまで　あたたまるわい！\p"
	.string "おまえさんも　はいると　いい。\p"
	.string "まんなかに　いって　のんびり　しっかり\n"
	.string "あたたまるんじゃ！$"

OneIsland_KindleRoad_EmberSpa_Text_EnjoyBowlOfChowder::
	.string "My favorite part of bathing comes\n"
	.string "up right after this.\p"
	.string "I enjoy a bowl of chowder to\n"
	.string "warm me from the inside, too!$"
	.string "{JPN}{JPN}わしの　おふろの　いちばんの　たのしみは\n"
	.string "このあと　なんじゃ。\p"
	.string "あつあつの　スープを　のんで　からだの\n"
	.string "なかからも　あたたまるんじゃ！$"

OneIsland_KindleRoad_EmberSpa_Text_WaterExquisiteFullyRefreshed::
	.string "The water is perfectly warm.\n"
	.string "It feels exquisite…\p"
	.string "{PLAYER} and the POKéMON were\n"
	.string "fully refreshed!$"
	.string "{JPN}{JPN}おゆは　ちょうど　いい　あたたかさ。　とても\n"
	.string "きもちいい‥\p"
	.string "{PLAYER}も　ポケモンも　すっかり　げんきに\n"
	.string "なった！$"

OneIsland_KindleRoad_EmberSpa_Text_HotSpringIsTherapeutic::
	.string "Soaking in a hot spring pool is\n"
	.string "therapeutic, people say.$"
	.string "{JPN}{JPN}おんせんに　つかると　からだに　いいって　みんな\n"
	.string "いうわね。$"

OneIsland_KindleRoad_EmberSpa_Text_SeeHowSmoothMySkinIs::
	.string "Have a look at my face.\n"
	.string "See how smooth my skin is?\p"
	.string "The hot spring's water keeps my\n"
	.string "skin younger than my age.$"
	.string "{JPN}{JPN}わたしの　かおを　みて。　おはだ　つるつる\n"
	.string "でしょ？\p"
	.string "おんせんの　おかげで　としより　ずっと　わかい\n"
	.string "はだを　たもてるのよ。$"

OneIsland_KindleRoad_EmberSpa_Text_BrunoVisitsSpaOnOccasion::
	.string "After an honest day of training,\n"
	.string "nothing beats a relaxing soak in\l"
	.string "the hot spring pool.\p"
	.string "BRUNO, who's a senior ahead of me,\n"
	.string "visits the SPA on occasion.\p"
	.string "He comes to rehab injuries, both\n"
	.string "his own and his POKéMON's.$"
	.string "{JPN}{JPN}しゅぎょうを　した　あとは　おんせんで　のんびり\n"
	.string "つかるのが　いちばんだ。\p"
	.string "おれの　せんぱいの　シバさんも　ときどき　この\n"
	.string "おんせんに　くるんだ。\p"
	.string "じぶんと　ポケモンの　けがを　なおしに\n"
	.string "くるんだよ。$"

OneIsland_KindleRoad_EmberSpa_Text_UsedThisToMakeEmberSpa::
	.string "Hot springs go hand-in-hand with\n"
	.string "volcanoes.\p"
	.string "The EMBER SPA here, well,\n"
	.string "I made it years ago.\p"
	.string "I hewed the pool right out of a\n"
	.string "gigantic boulder by hand.\p"
	.string "I was using this at the time.\n"
	.string "I suppose I can part with it.$"
	.string "{JPN}{JPN}おんせんと　かざんは　つきものじゃ。\p"
	.string "ここの　ともしび　おんせんは　むかし　わしが\n"
	.string "つくったんじゃ。\p"
	.string "おおきな　いわを　てで　けずって　ゆぶねを\n"
	.string "つくったんじゃよ。\p"
	.string "そのとき　つかって　いたのが　これじゃ。\n"
	.string "おまえさんに　ゆずって　やろう。$"

OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06::
	.string "That shatters boulders as if they\n"
	.string "were made of crackers.$"
	.string "{JPN}{JPN}それを　つかえば　いわを　せんべいの　ように\n"
	.string "くだける　ぞ。$"

