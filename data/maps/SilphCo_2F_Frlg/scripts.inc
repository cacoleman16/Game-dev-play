SilphCo_2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_2F_OnLoad
	.byte 0

SilphCo_2F_OnLoad::
	call_if_unset FLAG_SILPH_2F_DOOR_1, EventScript_Close2FDoor1
	call_if_unset FLAG_SILPH_2F_DOOR_2, EventScript_Close2FDoor2
	end

SilphCo_2F_EventScript_ThunderWaveTutor::
	goto EventScript_ThunderWaveTutor
	end

SilphCo_2F_EventScript_FloorSign::
	msgbox SilphCo_2F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_2F_EventScript_Connor::
	trainerbattle_single TRAINER_SCIENTIST_CONNOR, SilphCo_2F_Text_ConnorIntro, SilphCo_2F_Text_ConnorDefeat
	msgbox SilphCo_2F_Text_ConnorPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_EventScript_Jerry::
	trainerbattle_single TRAINER_SCIENTIST_JERRY, SilphCo_2F_Text_JerryIntro, SilphCo_2F_Text_JerryDefeat
	msgbox SilphCo_2F_Text_JerryPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_23, SilphCo_2F_Text_Grunt1Intro, SilphCo_2F_Text_Grunt1Defeat
	msgbox SilphCo_2F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_24, SilphCo_2F_Text_Grunt2Intro, SilphCo_2F_Text_Grunt2Defeat
	msgbox SilphCo_2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_Text_ConnorIntro::
	.string "Help!\n"
	.string "I'm a SILPH employee.$"
	.string "{JPN}{JPN}たすけて！　ぼくは　シルフの　しゃいん　なんだ。$"

SilphCo_2F_Text_ConnorDefeat::
	.string "How did you know\n"
	.string "I was a ROCKET?$"
	.string "{JPN}{JPN}なぜ　ロケットだんだと　わかった？$"

SilphCo_2F_Text_ConnorPostBattle::
	.string "I work for both SILPH and TEAM\n"
	.string "ROCKET.$"
	.string "{JPN}{JPN}おれは　シルフと　ロケットだんの　かけもち\n"
	.string "なのさ。$"

SilphCo_2F_Text_JerryIntro::
	.string "It's off-limits here!\n"
	.string "Go home!$"
	.string "{JPN}{JPN}ここは　たちいり　きんしだ！　かえれ！$"

SilphCo_2F_Text_JerryDefeat::
	.string "You're good.$"
	.string "{JPN}{JPN}やるね。$"

SilphCo_2F_Text_JerryPostBattle::
	.string "Can you solve the maze in here?\n"
	.string "It's not easy, I tell you!$"
	.string "{JPN}{JPN}ここの　めいろが　とけるかな？　かんたんじゃ\n"
	.string "ないぞ！$"

SilphCo_2F_Text_Grunt1Intro::
	.string "No kids are allowed in here!$"
	.string "{JPN}{JPN}ここは　こどもは　たちいり　きんしだ！$"

SilphCo_2F_Text_Grunt1Defeat::
	.string "Tough!$"
	.string "{JPN}{JPN}つよい！$"

SilphCo_2F_Text_Grunt1PostBattle::
	.string "Diamond-shaped tiles are teleport\n"
	.string "blocks.\p"
	.string "They're transporters for this\n"
	.string "high-tech building.$"
	.string "{JPN}{JPN}ひしがたの　ゆかは　ワープパネルだ。\p"
	.string "この　ハイテク　ビルの　いどう　そうち　なのさ。$"

SilphCo_2F_Text_Grunt2Intro::
	.string "Hey, kid!\n"
	.string "What are you wandering here for?$"
	.string "{JPN}{JPN}おい　ガキ！　こんな　ところで　なにを\n"
	.string "うろついて　いる？$"

SilphCo_2F_Text_Grunt2Defeat::
	.string "I goofed!$"
	.string "{JPN}{JPN}しくじった！$"

SilphCo_2F_Text_Grunt2PostBattle::
	.string "SILPH CO. will be associated with\n"
	.string "TEAM ROCKET!$"
	.string "{JPN}{JPN}シルフカンパニーは　ロケットだんの　ものに\n"
	.string "なるのさ！$"

Text_ThunderWaveTeach::
	.string "Eeek!\n"
	.string "No! Stop! Help!\p"
	.string "Oh, you're not with TEAM ROCKET.\n"
	.string "I'm sorry, I thought…\p"
	.string "Will you forgive me if I teach\n"
	.string "you the THUNDER WAVE technique?$"
	.string "{JPN}{JPN}きゃー！　やめて！　たすけてー！\p"
	.string "あら　ロケットだんじゃ　ないのね。　ごめんなさい\n"
	.string "てっきり‥\p"
	.string "おわびに　でんじはを　おしえるから　ゆるして\n"
	.string "くれる？$"

Text_ThunderWaveDeclined::
	.string "Oh…\n"
	.string "But THUNDER WAVE is convenient…$"
	.string "{JPN}{JPN}そう‥　でんじはって　べんりなのに‥$"

Text_ThunderWaveWhichMon::
	.string "Which POKéMON should I teach\n"
	.string "THUNDER WAVE?$"
	.string "{JPN}{JPN}どの　ポケモンに　でんじはを　おしえる？$"

Text_ThunderWaveTaught::
	.string "THUNDER WAVE causes paralysis in\n"
	.string "the target POKéMON.\p"
	.string "A paralyzed POKéMON may be unable\n"
	.string "to move in battle.\p"
	.string "It's an ELECTRIC-type move, so it\n"
	.string "won't work on some POKéMON.$"
	.string "{JPN}{JPN}でんじはは　あいての　ポケモンを　まひ\n"
	.string "させるの。\p"
	.string "まひした　ポケモンは　バトルで　うごけない\n"
	.string "ことが　あるのよ。\p"
	.string "でんきタイプの　わざ　だから　きかない\n"
	.string "ポケモンも　いるわ。$"

SilphCo_2F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "2F$"
	.string "{JPN}{JPN}シルフカンパニー　ほんしゃ\n"
	.string "2かい$"

