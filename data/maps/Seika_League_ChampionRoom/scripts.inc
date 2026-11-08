Seika_League_ChampionRoom_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Seika_League_ChampionRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Seika_League_ChampionRoom_OnFrame
	.byte 0

Seika_League_ChampionRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, Seika_League_ChampionRoom_EventScript_TurnNorth
	.2byte 0

Seika_League_ChampionRoom_EventScript_TurnNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

Seika_League_ChampionRoom_OnFrame::
	map_script_2 VAR_TEMP_1, 0, Seika_League_ChampionRoom_EventScript_Enter
	.2byte 0

Seika_League_ChampionRoom_EventScript_Enter::
	lockall
	setvar VAR_TEMP_1, 1
	applymovement LOCALID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerEnter
	waitmovement 0
	delay 20
	goto_if_set FLAG_QOL_SEIKA_CHAMPION, Seika_League_ChampionRoom_EventScript_Rematch
	msgbox Seika_League_ChampionRoom_Text_Intro
	goto Seika_League_ChampionRoom_EventScript_Battle

Seika_League_ChampionRoom_EventScript_Rematch::
	msgbox Seika_League_ChampionRoom_Text_Rematch
Seika_League_ChampionRoom_EventScript_Battle::
	setflag FLAG_TEMP_3
	savebgm MUS_VICTORY_GYM_LEADER
	trainerbattle_no_intro TRAINER_SEIKA_CHAMPION_SEIJI, Seika_League_ChampionRoom_Text_Defeat
	msgbox Seika_League_ChampionRoom_Text_Post
	goto_if_set FLAG_QOL_SEIKA_CHAMPION, Seika_League_ChampionRoom_EventScript_Leave
	setflag FLAG_QOL_SEIKA_CHAMPION
	playfanfare MUS_OBTAIN_BADGE
	message Seika_League_ChampionRoom_Text_Champion
	waitmessage
	waitfanfare
	msgbox Seika_League_ChampionRoom_Text_Shrine
Seika_League_ChampionRoom_EventScript_Leave::
	closemessage
	fadescreen FADE_TO_BLACK
	warp MAP_SEIKA_STARWATCH_PLATEAU, 11, 7
	waitstate
	releaseall
	end

Seika_League_ChampionRoom_Text_Intro::
	.string "So you are the one the lanterns\n"
	.string "have been guiding.\p"
	.string "I am SEIJI, CHAMPION of SEIKA.\n"
	.string "I keep the path to the SKY SHRINE.\p"
	.string "A thousand years ago my family\n"
	.string "swore to let only the strongest\l"
	.string "heart walk that path.\p"
	.string "Show me your heart, trainer from\n"
	.string "KANTO!{PLAY_BGM}{MUS_RG_VS_CHAMPION}$"
	.string "{JPN}{JPN}セイジ『そうか‥　ともしびが　みちびいて\n"
	.string "きたのは　きみだったか。\p"
	.string "わたしは　セイカの　チャンピオン　セイジ。\n"
	.string "そらのやしろへの　みちを　まもって　いる。\p"
	.string "せんねん　まえ　わが　いちぞくは　ちかった。\n"
	.string "その　みちを　あゆめるのは　もっとも　つよい\l"
	.string "こころの　もちぬし　だけだと。\p"
	.string "カントーから　きた　トレーナーよ！　きみの\n"
	.string "こころを　みせて　くれ！』{PLAY_BGM}{MUS_RG_VS_CHAMPION}$"

Seika_League_ChampionRoom_Text_Rematch::
	.string "Back again? Good. The sky never\n"
	.string "tires of a strong heart.{PLAY_BGM}{MUS_RG_VS_CHAMPION}$"
	.string "{JPN}{JPN}セイジ『また　きたか。　よし。　そらは　つよい\n"
	.string "こころに　あきは　しない。』{PLAY_BGM}{MUS_RG_VS_CHAMPION}$"

Seika_League_ChampionRoom_Text_Defeat::
	.string "Remarkable... The lanterns chose\n"
	.string "well.$"
	.string "{JPN}{JPN}みごとだ‥　ともしびの　めに　くるいは\n"
	.string "なかったな。$"

Seika_League_ChampionRoom_Text_Post::
	.string "You are stronger than any trainer\n"
	.string "I have faced.$"
	.string "{JPN}{JPN}セイジ『きみは　わたしが　たたかった　どの\n"
	.string "トレーナーよりも　つよい。』$"

Seika_League_ChampionRoom_Text_Champion::
	.string "{PLAYER} became the CHAMPION\n"
	.string "of the SEIKA LEAGUE!$"
	.string "{JPN}{JPN}{PLAYER}は　セイカリーグの　チャンピオンに\n"
	.string "なった！$"

Seika_League_ChampionRoom_Text_Shrine::
	.string "Listen. Team NOVA carried the\n"
	.string "STARGLASS BELL up to the SKY\l"
	.string "SHRINE.\p"
	.string "If they ring it, the WISHING\n"
	.string "STAR will grant THEIR wish.\p"
	.string "The path above STARWATCH PLATEAU\n"
	.string "is yours now. Hurry!$"
	.string "{JPN}{JPN}セイジ『きいて　くれ。　ノバだんが\n"
	.string "ほしガラスのかねを　そらのやしろへ\l"
	.string "はこびあげた。\p"
	.string "かねを　ならされたら　ねがいぼしは　やつらの\n"
	.string "ねがいを　かなえて　しまう。\p"
	.string "ホシミこうげんの　うえへの　みちは　もう　きみの\n"
	.string "ものだ。　いそいで　くれ！』$"
