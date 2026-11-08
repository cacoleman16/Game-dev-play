Seika_League_Lobby_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_League_Lobby_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Seika_League_Lobby_EventScript_GymGuy::
	msgbox Seika_League_Lobby_Text_GymGuy, MSGBOX_NPC
	end

Seika_League_Lobby_Text_GymGuy::
	.string "Yo! This is it, the SEIKA LEAGUE!\p"
	.string "Four ELITE trainers, then the\n"
	.string "CHAMPION. No healing between\l"
	.string "rooms, so stock up here!\p"
	.string "TETSU uses STEEL. FIRE and GROUND\n"
	.string "melt him down. Good luck, champ!$"
	.string "{JPN}{JPN}おーす！　ついに　きたな　セイカリーグ！\p"
	.string "してんのう　4にん　そして　チャンピオン。\n"
	.string "とちゅうで　かいふく　できないから　ここで\l"
	.string "そなえて　おけよ！\p"
	.string "テツは　はがね。　ほのおと　じめんで　とかして\n"
	.string "やれ！　がんばれよ　チャンピオン！$"

Seika_League_Lobby_EventScript_Cooltrainer::
	msgbox Seika_League_Lobby_Text_Cooltrainer, MSGBOX_NPC
	end

Seika_League_Lobby_Text_Cooltrainer::
	.string "They say CHAMPION SEIJI guards\n"
	.string "the path to the SKY SHRINE.\p"
	.string "Only a CHAMPION may climb to the\n"
	.string "top of MT. SKYREACH.$"
	.string "{JPN}{JPN}セイジさんは　そらのやしろへの　みちを　まもる\n"
	.string "チャンピオン　なんだって。\p"
	.string "ソラトドキやまの　てっぺんに　のぼれるのは\n"
	.string "チャンピオン　だけなんだ。$"

Seika_League_Lobby_OnTransition::
	setrespawn HEAL_LOCATION_SEIKA_LEAGUE
	clearflag FLAG_QOL_SEIKA_E4_1
	clearflag FLAG_QOL_SEIKA_E4_2
	clearflag FLAG_QOL_SEIKA_E4_3
	clearflag FLAG_QOL_SEIKA_E4_4
	end

Seika_League_Lobby_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Seika_League_Lobby_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_League_Lobby_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_League_Lobby_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_ETHER
	.2byte ITEM_MAX_ELIXIR
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end
