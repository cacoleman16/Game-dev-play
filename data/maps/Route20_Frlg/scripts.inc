Route20_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route20_OnTransition
	.byte 0

Route20_OnTransition::
	call_if_unset FLAG_STOPPED_SEAFOAM_B3F_CURRENT, Route20_EventScript_ResetSeafoamBouldersForB3F
	call_if_unset FLAG_STOPPED_SEAFOAM_B4F_CURRENT, Route20_EventScript_ResetSeafoamBouldersForB4F
	end

Route20_EventScript_ResetSeafoamBouldersForB3F::
	clearflag FLAG_HIDE_SEAFOAM_1F_BOULDER_1
	clearflag FLAG_HIDE_SEAFOAM_1F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B1F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B1F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B2F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B2F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_2
	return

Route20_EventScript_ResetSeafoamBouldersForB4F::
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_3
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_4
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_5
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_6
	setflag FLAG_HIDE_SEAFOAM_B4F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B4F_BOULDER_2
	return

Route20_EventScript_SeafoamIslandsSign::
	msgbox Route20_Text_SeafoamIslands, MSGBOX_SIGN
	end

Route20_Text_BarryIntro::
	.string "The water is shallow here.\n"
	.string "There are many people swimming.$"
	.string "{JPN}{JPN}ここは　あさいから　およいでる　ひとが　たくさん\n"
	.string "いるんだ。$"

Route20_Text_BarryDefeat::
	.string "Splash!$"
	.string "{JPN}{JPN}ばしゃーん！$"

Route20_Text_BarryPostBattle::
	.string "I wish I could ride my POKéMON.\n"
	.string "I bet you're not tired.$"
	.string "{JPN}{JPN}ぼくも　ポケモンに　のれたらなあ。　きみは\n"
	.string "つかれて　ないだろうね。$"

Route20_Text_ShirleyIntro::
	.string "SEAFOAM is a quiet getaway.\n"
	.string "I'm vacationing here.$"
	.string "{JPN}{JPN}ふたごじまは　しずかな　かくれが　よ。　ここで\n"
	.string "バカンス　ちゅうなの。$"

Route20_Text_ShirleyDefeat::
	.string "Quit it!$"
	.string "{JPN}{JPN}やめてよ！$"

Route20_Text_ShirleyPostBattle::
	.string "There's a huge cavern underneath\n"
	.string "this island.$"
	.string "{JPN}{JPN}この　しまの　したには　おおきな　どうくつが\n"
	.string "あるのよ。$"

Route20_Text_TiffanyIntro::
	.string "I love floating with the fishes\n"
	.string "here among the waves.$"
	.string "{JPN}{JPN}なみの　なかで　さかなと　いっしょに　ぷかぷか\n"
	.string "うかぶのが　すきなの。$"

Route20_Text_TiffanyDefeat::
	.string "Yowch!$"
	.string "{JPN}{JPN}いたっ！$"

Route20_Text_TiffanyPostBattle::
	.string "Want to float with me?$"
	.string "{JPN}{JPN}いっしょに　うかばない？$"

Route20_Text_IreneIntro::
	.string "Are you on vacation, too?$"
	.string "{JPN}{JPN}あなたも　バカンス？$"

Route20_Text_IreneDefeat::
	.string "No mercy at all!$"
	.string "{JPN}{JPN}ようしゃ　ないわね！$"

Route20_Text_IrenePostBattle::
	.string "SEAFOAM used to be a single island\n"
	.string "in the distant past.$"
	.string "{JPN}{JPN}ふたごじまは　ずっと　むかし　ひとつの　しま\n"
	.string "だったのよ。$"

Route20_Text_DeanIntro::
	.string "Check out my buff physique!$"
	.string "{JPN}{JPN}おれの　きたえあげた　にくたいを　みろ！$"

Route20_Text_DeanDefeat::
	.string "Wimpy!$"
	.string "{JPN}{JPN}よわっ！$"

Route20_Text_DeanPostBattle::
	.string "I should've been buffing up my\n"
	.string "POKéMON, not me!$"
	.string "{JPN}{JPN}おれじゃなくて　ポケモンを　きたえるべき\n"
	.string "だった！$"

Route20_Text_DarrinIntro::
	.string "Why are you riding a POKéMON?\n"
	.string "Can't you swim?$"
	.string "{JPN}{JPN}なんで　ポケモンに　のってるんだ？\n"
	.string "およげないのか？$"

Route20_Text_DarrinDefeat::
	.string "Ouch!\n"
	.string "Torpedoed!$"
	.string "{JPN}{JPN}いてっ！　ぎょらいを　くらった！$"

Route20_Text_DarrinPostBattle::
	.string "Riding a POKéMON sure looks fun!$"
	.string "{JPN}{JPN}ポケモンに　のるのも　たのしそうだな！$"

Route20_Text_RogerIntro::
	.string "I rode my bird POKéMON here.$"
	.string "{JPN}{JPN}とりポケモンに　のって　ここまで　きたんだ。$"

Route20_Text_RogerDefeat::
	.string "Oh, no!\n"
	.string "Now what am I to do?$"
	.string "{JPN}{JPN}しまった！　これから　どうしよう？$"

Route20_Text_RogerPostBattle::
	.string "My birds are exhausted.\n"
	.string "They can't FLY me back!$"
	.string "{JPN}{JPN}とりたちが　つかれきってる。　そらをとぶで\n"
	.string "かえれないよ！$"

Route20_Text_NoraIntro::
	.string "My boyfriend gave me big pearls.$"
	.string "{JPN}{JPN}かれしに　おおきな　しんじゅを　もらったの。$"

Route20_Text_NoraDefeat::
	.string "Oh, no!\n"
	.string "My pearls were in them!$"
	.string "{JPN}{JPN}ああっ！　しんじゅが　なかに　はいってたのに！$"

Route20_Text_NoraPostBattle::
	.string "Will my pearls grow bigger inside\n"
	.string "CLOYSTER?$"
	.string "{JPN}{JPN}パルシェンの　なかで　しんじゅは　おおきく\n"
	.string "なるかしら？$"

Route20_Text_MissyIntro::
	.string "I swam here from CINNABAR ISLAND.\n"
	.string "It wasn't easy, I tell you.$"
	.string "{JPN}{JPN}グレンじまから　およいで　きたの。　たいへん\n"
	.string "だったわよ。$"

Route20_Text_MissyDefeat::
	.string "I'm so disappointed!$"
	.string "{JPN}{JPN}がっかり　だわ！$"

Route20_Text_MissyPostBattle::
	.string "POKéMON have taken over an\n"
	.string "abandoned mansion on CINNABAR.\p"
	.string "They call it the POKéMON MANSION\n"
	.string "now.$"
	.string "{JPN}{JPN}グレンの　あきやしきに　ポケモンが\n"
	.string "すみついたの。\p"
	.string "いまでは　ポケモンやしきって　よばれてるわ。$"

Route20_Text_MelissaIntro::
	.string "CINNABAR, in the west, has a LAB\n"
	.string "for POKéMON.\p"
	.string "My daddy works there.$"
	.string "{JPN}{JPN}にしの　グレンには　ポケモンの　けんきゅうじょが\n"
	.string "あるの。\p"
	.string "パパが　そこで　はたらいてるのよ。$"

Route20_Text_MelissaDefeat::
	.string "Wait!\n"
	.string "You're supposed to wait!$"
	.string "{JPN}{JPN}まって！　まってって　いってるでしょ！$"

Route20_Text_MelissaPostBattle::
	.string "CINNABAR is a volcanic island.\p"
	.string "I heard it rose out of the sea\n"
	.string "when a volcano erupted.$"
	.string "{JPN}{JPN}グレンは　かざんの　しま　なの。\p"
	.string "かざんが　ふんか　したとき　うみから\n"
	.string "うまれたんですって。$"

Route20_Text_SeafoamIslands::
	.string "SEAFOAM ISLANDS$"
	.string "{JPN}{JPN}ふたごじま$"

Route20_Text_MistyTrainsHere::
	.string "Strong TRAINERS and WATER POKéMON\n"
	.string "are common sights in these parts.\p"
	.string "They say that MISTY of the\n"
	.string "CERULEAN GYM trains here.$"
	.string "{JPN}{JPN}この　あたりでは　つよい　トレーナーや\n"
	.string "みずポケモンを　よく　みかける。\p"
	.string "ハナダジムの　カスミも　ここで　とっくん\n"
	.string "している　らしい。$"
