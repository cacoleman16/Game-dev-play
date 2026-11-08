FourIsland_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FourIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

FourIsland_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_FOUR_ISLAND
	end

FourIsland_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

FourIsland_PokemonCenter_1F_EventScript_Man::
	msgbox FourIsland_PokemonCenter_1F_Text_RareIceMonsInCave, MSGBOX_NPC
	end

FourIsland_PokemonCenter_1F_EventScript_Woman::
	msgbox FourIsland_PokemonCenter_1F_Text_DayCarePeopleFoundEggBefore, MSGBOX_NPC
	end

FourIsland_PokemonCenter_1F_EventScript_Gentleman::
	msgbox FourIsland_PokemonCenter_1F_Text_NewPokemonJournalHasntArrived, MSGBOX_NPC
	end

@ The 5 texts below are supposed to be in FourIsland_House1
Text_BodySlamTeach::
	.string "Ready?\n"
	.string "Boing!\p"
	.string "We're having a BODY SLAM match\n"
	.string "to see who wimps out first.\p"
	.string "If you were to join us, you'd be\n"
	.string "squashed like a bug, though…\p"
	.string "How about I teach BODY SLAM to\n"
	.string "a POKéMON of yours instead?$"
	.string "{JPN}{JPN}いくぞ？　ボヨーン！\p"
	.string "いま　どっちが　さきに　ねをあげるか\n"
	.string "のしかかりで　しょうぶ　してるんだ。\p"
	.string "きみが　くわわったら　むしみたいに　つぶれちゃう\n"
	.string "だろうけど‥\p"
	.string "かわりに　きみの　ポケモンに　のしかかりを\n"
	.string "おしえて　あげようか？$"

Text_BodySlamDeclined::
	.string "Come again!\n"
	.string "Boing!$"
	.string "{JPN}{JPN}また　おいで！　ボヨーン！$"

Text_BodySlamWhichMon::
	.string "Which POKéMON wants to learn how\n"
	.string "to BODY SLAM?$"
	.string "{JPN}{JPN}どの　ポケモンに　のしかかりを　おしえる？$"

Text_BodySlamTaught::
	.string "Boioing!$"
	.string "{JPN}{JPN}ボヨヨーン！$"

FourIsland_House1_Text_FatMan::
	.string "それ　ぼよーん！\n"
	.string "おまえ　おもいなあー！$"
	.string "{JPN}{JPN}それ　ぼよーん！　おまえ　おもいなあー！$"

FourIsland_PokemonCenter_1F_Text_RareIceMonsInCave::
	.string "There's a rare kind of ICE POKéMON\n"
	.string "that lives in the cave.\p"
	.string "We get a lot of visitors hoping to\n"
	.string "see that POKéMON.$"
	.string "{JPN}{JPN}どうくつには　めずらしい　こおりタイプの\n"
	.string "ポケモンが　すんでいるんだ。\p"
	.string "その　ポケモンを　みたくて　たくさんの　ひとが\n"
	.string "やってくるよ。$"

FourIsland_PokemonCenter_1F_Text_DayCarePeopleFoundEggBefore::
	.string "Oh? A POKéMON EGG?\n"
	.string "…I have no idea.\p"
	.string "…Oh, wait. The DAY-CARE people\n"
	.string "in town were saying something.\p"
	.string "They said that they'd found EGGS\n"
	.string "before on their property.$"
	.string "{JPN}{JPN}えっ？　ポケモンの　タマゴ？　‥さあ\n"
	.string "しらないなあ。\p"
	.string "‥あ　まって。　まちの　そだてやの　ひとが\n"
	.string "なにか　いってたっけ。\p"
	.string "まえに　にわで　タマゴを　みつけた　ことが\n"
	.string "あるって。$"

FourIsland_PokemonCenter_1F_Text_NewPokemonJournalHasntArrived::
	.string "What's this?\p"
	.string "The new POKéMON JOURNAL hasn't\n"
	.string "arrived on this island?$"
	.string "{JPN}{JPN}あれ？\p"
	.string "この　しまには　まだ　ポケモンジャーナルの\n"
	.string "さいしんごうが　とどいて　いないの？$"

