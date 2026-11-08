PokemonTower_6F_Frlg_MapScripts::
	.byte 0

PokemonTower_6F_EventScript_MarowakGhost::
	lockall
	textcolor NPC_TEXT_COLOR_MON
	msgbox PokemonTower_6F_Text_BeGoneIntruders
	setwildbattle SPECIES_MAROWAK, 30
	special StartMarowakBattle
	waitstate
	goto_if_eq VAR_RESULT, FALSE, PokemonTower_6F_EventScript_DefeatedMarowakGhost  @ VAR_RESULT set by CB2_EndMarowakBattle
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerUp
	waitmovement 0
	releaseall
	end

PokemonTower_6F_EventScript_DefeatedMarowakGhost::
	message PokemonTower_6F_Text_GhostWasCubonesMother
	waitmessage
	waitse
	playmoncry SPECIES_MAROWAK, CRY_MODE_NORMAL
	waitbuttonpress
	waitmoncry
	msgbox PokemonTower_6F_Text_MothersSpiritWasCalmed
	setvar VAR_MAP_SCENE_POKEMON_TOWER_6F, 1
	releaseall
	end

PokemonTower_6F_Movement_ForcePlayerUp::
	walk_up
	step_end

PokemonTower_6F_EventScript_Angelica::
	trainerbattle_single TRAINER_CHANNELER_ANGELICA, PokemonTower_6F_Text_AngelicaIntro, PokemonTower_6F_Text_AngelicaDefeat
	msgbox PokemonTower_6F_Text_AngelicaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Emilia::
	trainerbattle_single TRAINER_CHANNELER_EMILIA, PokemonTower_6F_Text_EmiliaIntro, PokemonTower_6F_Text_EmiliaDefeat
	msgbox PokemonTower_6F_Text_EmiliaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Jennifer::
	trainerbattle_single TRAINER_CHANNELER_JENNIFER, PokemonTower_6F_Text_JenniferIntro, PokemonTower_6F_Text_JenniferDefeat
	msgbox PokemonTower_6F_Text_JenniferPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_Text_AngelicaIntro::
	.string "Give…me…\n"
	.string "blood…$"
	.string "{JPN}{JPN}ち‥を‥　よこせ‥$"

PokemonTower_6F_Text_AngelicaDefeat::
	.string "Groan!$"
	.string "{JPN}{JPN}うぐぐっ！$"

PokemonTower_6F_Text_AngelicaPostBattle::
	.string "I feel anemic and weak…$"
	.string "{JPN}{JPN}ひんけつで　ふらふら　するわ‥$"

PokemonTower_6F_Text_EmiliaIntro::
	.string "Urff…\n"
	.string "Kwaah!$"
	.string "{JPN}{JPN}うふぅ‥　きええっ！$"

PokemonTower_6F_Text_EmiliaDefeat::
	.string "Something fell out!$"
	.string "{JPN}{JPN}なにかが　ぬけた！$"

PokemonTower_6F_Text_EmiliaPostBattle::
	.string "My hair didn't fall out!\n"
	.string "It was a malevolent spirit!$"
	.string "{JPN}{JPN}かみのけが　ぬけたんじゃ　ないわよ！\n"
	.string "あくりょうが　ぬけたの！$"

PokemonTower_6F_Text_JenniferIntro::
	.string "Ke…ke…ke…\n"
	.string "ke…ke…ke!$"
	.string "{JPN}{JPN}け‥け‥け‥　け‥け‥けっ！$"

PokemonTower_6F_Text_JenniferDefeat::
	.string "Keee!$"
	.string "{JPN}{JPN}けえええっ！$"

PokemonTower_6F_Text_JenniferPostBattle::
	.string "What's going on here?$"
	.string "{JPN}{JPN}いったい　どうなってるの？$"

PokemonTower_6F_Text_BeGoneIntruders::
	.string "Be gone…\n"
	.string "Intruders…$"
	.string "{JPN}{JPN}たちされ‥　よそものよ‥$"

PokemonTower_6F_Text_GhostWasCubonesMother::
	.string "The ghost was the restless spirit\n"
	.string "of CUBONE's mother!$"
	.string "{JPN}{JPN}ゆうれいの　しょうたいは　カラカラの　ははおやの\n"
	.string "さまよう　たましい　だった！$"

PokemonTower_6F_Text_MothersSpiritWasCalmed::
	.string "The mother's spirit was calmed.\p"
	.string "It departed to the afterlife…$"
	.string "{JPN}{JPN}ははおやの　たましいは　しずまった。\p"
	.string "そして　あのよへ　たびだって　いった‥$"

