PokemonTower_4F_Frlg_MapScripts::
	.byte 0

PokemonTower_4F_EventScript_Paula::
	trainerbattle_single TRAINER_CHANNELER_PAULA, PokemonTower_4F_Text_PaulaIntro, PokemonTower_4F_Text_PaulaDefeat
	msgbox PokemonTower_4F_Text_PaulaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_4F_EventScript_Laurel::
	trainerbattle_single TRAINER_CHANNELER_LAUREL, PokemonTower_4F_Text_LaurelIntro, PokemonTower_4F_Text_LaurelDefeat
	msgbox PokemonTower_4F_Text_LaurelPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_4F_EventScript_Jody::
	trainerbattle_single TRAINER_CHANNELER_JODY, PokemonTower_4F_Text_JodyIntro, PokemonTower_4F_Text_JodyDefeat
	msgbox PokemonTower_4F_Text_JodyPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_4F_Text_PaulaIntro::
	.string "Ghost! No!\n"
	.string "Kwaaah!$"
	.string "{JPN}{JPN}おばけ！　いやー！　きええー！$"

PokemonTower_4F_Text_PaulaDefeat::
	.string "Where is the ghost?$"
	.string "{JPN}{JPN}おばけは　どこ？$"

PokemonTower_4F_Text_PaulaPostBattle::
	.string "I must have been dreaming…$"
	.string "{JPN}{JPN}ゆめでも　みていたのかしら‥$"

PokemonTower_4F_Text_LaurelIntro::
	.string "Be cursed with me!\n"
	.string "Kwaaah!$"
	.string "{JPN}{JPN}わたしと　いっしょに　のろわれなさい！\n"
	.string "きええー！$"

PokemonTower_4F_Text_LaurelDefeat::
	.string "What!$"
	.string "{JPN}{JPN}なにっ！$"

PokemonTower_4F_Text_LaurelPostBattle::
	.string "We can't determine the identity of\n"
	.string "the ghosts…$"
	.string "{JPN}{JPN}ゆうれいの　しょうたいは　わたしたちにも\n"
	.string "わからないの‥$"

PokemonTower_4F_Text_JodyIntro::
	.string "Huhuhu…\n"
	.string "Beat me not!$"
	.string "{JPN}{JPN}ふふふ‥　わたしに　かたないで！$"

PokemonTower_4F_Text_JodyDefeat::
	.string "Huh?\n"
	.string "Who? What?$"
	.string "{JPN}{JPN}はっ？　だれ？　なに？$"

PokemonTower_4F_Text_JodyPostBattle::
	.string "May the departed POKéMON rest in\n"
	.string "peace…$"
	.string "{JPN}{JPN}なくなった　ポケモンたちが　やすらかに\n"
	.string "ねむれますように‥$"

