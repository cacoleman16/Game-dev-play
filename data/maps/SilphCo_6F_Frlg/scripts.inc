SilphCo_6F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_6F_OnLoad
	.byte 0

SilphCo_6F_OnLoad::
	call_if_unset FLAG_SILPH_6F_DOOR, EventScript_Close6FDoor
	end

SilphCo_6F_EventScript_WorkerM2::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerM2RocketsGone
	msgbox SilphCo_6F_Text_TargetedSilphForOurMonProducts
	release
	end

SilphCo_6F_EventScript_WorkerM2RocketsGone::
	msgbox SilphCo_6F_Text_ComeWorkForSilphWhenYoureOlder
	release
	end

SilphCo_6F_EventScript_WorkerM3::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerM3RocketsGone
	msgbox SilphCo_6F_Text_RocketsTookOverBuilding
	release
	end

SilphCo_6F_EventScript_WorkerM3RocketsGone::
	msgbox SilphCo_6F_Text_BetterGetBackToWork
	release
	end

SilphCo_6F_EventScript_WorkerM1::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerM1RocketsGone
	msgbox SilphCo_6F_Text_HelpMePlease
	release
	end

SilphCo_6F_EventScript_WorkerM1RocketsGone::
	msgbox SilphCo_6F_Text_WeGotEngaged
	release
	end

SilphCo_6F_EventScript_WorkerF1::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerF1RocketsGone
	msgbox SilphCo_6F_Text_ThatManIsSuchACoward
	release
	end

SilphCo_6F_EventScript_WorkerF1RocketsGone::
	msgbox SilphCo_6F_Text_NeedsMeToLookAfterHim
	release
	end

SilphCo_6F_EventScript_WorkerF2::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerF2RocketsGone
	msgbox SilphCo_6F_Text_RocketsTryingToConquerWorld
	release
	end

SilphCo_6F_EventScript_WorkerF2RocketsGone::
	msgbox SilphCo_6F_Text_RocketsRanAwayBecauseOfYou
	release
	end

SilphCo_6F_EventScript_FloorSign::
	msgbox SilphCo_6F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_6F_EventScript_Taylor::
	trainerbattle_single TRAINER_SCIENTIST_TAYLOR, SilphCo_6F_Text_TaylorIntro, SilphCo_6F_Text_TaylorDefeat
	msgbox SilphCo_6F_Text_TaylorPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_6F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_30, SilphCo_6F_Text_Grunt1Intro, SilphCo_6F_Text_Grunt1Defeat
	msgbox SilphCo_6F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_6F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_31, SilphCo_6F_Text_Grunt2Intro, SilphCo_6F_Text_Grunt2Defeat
	msgbox SilphCo_6F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_6F_Text_RocketsTookOverBuilding::
	.string "The ROCKETS came and took over\n"
	.string "the building!$"
	.string "{JPN}{JPN}ロケットだんが　きて　ビルを　のっとったんだ！$"

SilphCo_6F_Text_BetterGetBackToWork::
	.string "Well, better get back to work.$"
	.string "{JPN}{JPN}さて　しごとに　もどらないと。$"

SilphCo_6F_Text_HelpMePlease::
	.string "Oh dear, oh dear.\n"
	.string "Help me, please!$"
	.string "{JPN}{JPN}ああ　どうしよう　どうしよう。　たすけて\n"
	.string "ください！$"

SilphCo_6F_Text_WeGotEngaged::
	.string "We got engaged.\n"
	.string "Heheh!$"
	.string "{JPN}{JPN}ぼくたち　こんやく　したんだ。　えへへ！$"

SilphCo_6F_Text_ThatManIsSuchACoward::
	.string "That man next to me…\n"
	.string "He's such a coward!$"
	.string "{JPN}{JPN}となりの　あの　ひと‥　ほんとうに　いくじなし\n"
	.string "なんだから！$"

SilphCo_6F_Text_NeedsMeToLookAfterHim::
	.string "He's so helpless, he needed\n"
	.string "someone like me to look after him.$"
	.string "{JPN}{JPN}あの　ひと　たよりないから　わたしが　めんどう\n"
	.string "みて　あげなきゃ　だめなのよ。$"

SilphCo_6F_Text_RocketsTryingToConquerWorld::
	.string "TEAM ROCKET is trying to conquer\n"
	.string "the world with POKéMON.$"
	.string "{JPN}{JPN}ロケットだんは　ポケモンで　せかいを　せいふく\n"
	.string "しようと　して　いるんだ。$"

SilphCo_6F_Text_RocketsRanAwayBecauseOfYou::
	.string "TEAM ROCKET ran away, and it's all\n"
	.string "because of you!$"
	.string "{JPN}{JPN}ロケットだんが　にげだしたのは　ぜんぶ　きみの\n"
	.string "おかげだよ！$"

SilphCo_6F_Text_TargetedSilphForOurMonProducts::
	.string "They must have targeted SILPH for\n"
	.string "our POKéMON products.$"
	.string "{JPN}{JPN}やつらは　うちの　ポケモン　せいひんが　めあてで\n"
	.string "シルフを　ねらったに　ちがいない。$"

SilphCo_6F_Text_ComeWorkForSilphWhenYoureOlder::
	.string "Come work for SILPH when you\n"
	.string "get older.$"
	.string "{JPN}{JPN}おおきく　なったら　シルフで　はたらきなよ。$"

SilphCo_6F_Text_Grunt1Intro::
	.string "I am one of the four ROCKET\n"
	.string "BROTHERS!$"
	.string "{JPN}{JPN}おれは　ロケット　4きょうだいの　ひとり！$"

SilphCo_6F_Text_Grunt1Defeat::
	.string "Flame out!$"
	.string "{JPN}{JPN}もえつきた！$"

SilphCo_6F_Text_Grunt1PostBattle::
	.string "No matter!\n"
	.string "My brothers will avenge me!$"
	.string "{JPN}{JPN}かまわん！　きょうだいたちが　かたきを　うって\n"
	.string "くれる！$"

SilphCo_6F_Text_TaylorIntro::
	.string "That rotten PRESIDENT!\p"
	.string "Serves him right for shipping me\n"
	.string "off to the TIKSI BRANCH!\p"
	.string "That's why TEAM ROCKET came after\n"
	.string "us, I'm sure of it!$"
	.string "{JPN}{JPN}あの　しゃちょうめ！\p"
	.string "おれを　ティクシ　してんに　とばした　ばちが\n"
	.string "あたったんだ！\p"
	.string "だから　ロケットだんに　ねらわれたんだ\n"
	.string "まちがいない！$"

SilphCo_6F_Text_TaylorDefeat::
	.string "Shoot!$"
	.string "{JPN}{JPN}くそっ！$"

SilphCo_6F_Text_TaylorPostBattle::
	.string "TIKSI BRANCH?\n"
	.string "It's in Russian no-man's-land!$"
	.string "{JPN}{JPN}ティクシ　してん？　ロシアの　さいはての\n"
	.string "ちだぞ！$"

SilphCo_6F_Text_Grunt2Intro::
	.string "You dare betray TEAM ROCKET?$"
	.string "{JPN}{JPN}ロケットだんを　うらぎる　きか？$"

SilphCo_6F_Text_Grunt2Defeat::
	.string "You traitor!$"
	.string "{JPN}{JPN}うらぎりものめ！$"

SilphCo_6F_Text_Grunt2PostBattle::
	.string "If you stand for justice, you\n"
	.string "betray us bad guys!$"
	.string "{JPN}{JPN}せいぎの　みかたを　するって　ことは　おれたち\n"
	.string "わるものを　うらぎるって　ことだ！$"

SilphCo_6F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "6F$"
	.string "{JPN}{JPN}シルフカンパニー　ほんしゃ\n"
	.string "6かい$"

