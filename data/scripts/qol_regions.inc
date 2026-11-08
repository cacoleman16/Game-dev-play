@ [QoL] Merged regions: the S.S. TIDAL links Kanto and Hoenn.
@ Vermilion City (Kanto): a sailor on the pier sails to Hoenn once the player
@ has the S.S. TICKET, and to the Battle Frontier once the player is a Champion.
@ Hoenn: the Slateport, Lilycove and Battle Frontier ferry desks and a shuttle
@ driver in Littleroot Town take the player back to Vermilion City.
@ The first trip to Hoenn starts Emerald's arrival: the moving truck to Littleroot.

VermilionCity_EventScript_TidalSailor::
	lock
	faceplayer
	msgbox QoL_Text_TidalGreeting, MSGBOX_DEFAULT
	checkitem ITEM_SS_TICKET
	goto_if_eq VAR_RESULT, FALSE, QoL_EventScript_TidalNoTicket
	special QoL_HoennSetup
	@ The Battle Frontier is open to Champions of either region.
	setvar VAR_0x8005, FALSE
	call_if_set FLAG_SYS_GAME_CLEAR, QoL_EventScript_FrontierOpen
	call_if_set FLAG_HOENN_GAME_CLEAR, QoL_EventScript_FrontierOpen
	goto_if_eq VAR_0x8005, TRUE, QoL_EventScript_TidalMenu
	goto_if_eq VAR_LITTLEROOT_INTRO_STATE, 0, QoL_EventScript_TidalFirstTrip
QoL_EventScript_TidalMenu::
	message QoL_Text_TidalWhereTo
	waitmessage
	dynmultipush QoL_Text_DestLittleroot, 0
	call_if_set FLAG_VISITED_SLATEPORT_CITY, QoL_EventScript_PushSlateport
	call_if_set FLAG_VISITED_LILYCOVE_CITY, QoL_EventScript_PushLilycove
	call_if_eq VAR_0x8005, TRUE, QoL_EventScript_PushFrontier
	dynmultipush QoL_Text_DestStarfallIsle, 5 @ [QoL] Visible wild POKéMON island
	call QoL_EventScript_PushBirthIsland @ [QoL] Starfall finale
	call QoL_EventScript_PushSeika @ [QoL] Seika region (after the finale)
	call QoL_EventScript_PushSkyrift @ [QoL] Skyrift Isles (docs/SKYRIFT.md)
	dynmultipush QoL_Text_DestCancel, 9
	dynmultistack 0, 0, FALSE, 5, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, QoL_EventScript_SailToLittleroot
	case 1, QoL_EventScript_SailToSlateport
	case 2, QoL_EventScript_SailToLilycove
	case 3, QoL_EventScript_SailToFrontier
	case 4, QoL_EventScript_SailToBirthIsland
	case 5, QoL_EventScript_SailToStarfallIsle
	case 6, QoL_EventScript_SailToSeika
	case 7, QoL_EventScript_SailToSkyrift
	goto QoL_EventScript_TidalCancel
	end

QoL_EventScript_PushSlateport::
	dynmultipush QoL_Text_DestSlateport, 1
	return

QoL_EventScript_PushLilycove::
	dynmultipush QoL_Text_DestLilycove, 2
	return

QoL_EventScript_PushFrontier::
	dynmultipush QoL_Text_DestFrontier, 3
	return

QoL_EventScript_PushBirthIsland::
	call QoL_EventScript_StarfallFinaleOpen
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_NopReturn
	dynmultipush QoL_Text_DestBirthIsland, 4
	return

QoL_EventScript_SailToBirthIsland::
	call QoL_EventScript_SetSail
	warp MAP_BIRTH_ISLAND_HARBOR, 8, 4
	waitstate
	release
	end

@ STARFALL ISLE (Emerald's FARAWAY ISLAND): meteors from the STARFALL came
@ down here, and its wild POKéMON always roam in the open (src/qol_hoenn.c,
@ QoL_VisibleWildActive). Mew still hides in the grass.
QoL_EventScript_SailToStarfallIsle::
	call QoL_EventScript_SetSail
	warp MAP_FARAWAY_ISLAND_ENTRANCE, 13, 38
	waitstate
	release
	end

@ SEIKA (docs/SEIKA.md): opens after the Starfall finale on Birth Island.
QoL_EventScript_PushSeika::
	goto_if_unset FLAG_QOL_STARFALL_DONE, Common_EventScript_NopReturn
	dynmultipush QoL_Text_DestSeika, 6
	return

QoL_EventScript_SailToSeika::
	call QoL_EventScript_SetSail
	warp MAP_SEIKA_LANTERNPORT, 20, 28
	waitstate
	release
	end

QoL_EventScript_FrontierOpen::
	setvar VAR_0x8005, TRUE
	return

QoL_EventScript_TidalNoTicket::
	msgbox QoL_Text_TidalNeedTicket, MSGBOX_DEFAULT
	release
	end

QoL_EventScript_TidalCancel::
	msgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT
	release
	end

QoL_EventScript_TidalFirstTrip::
	msgbox QoL_Text_TidalFirstTrip, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, QoL_EventScript_TidalCancel
	call QoL_EventScript_SetSail
	warp MAP_INSIDE_OF_TRUCK, 2, 2
	waitstate
	release
	end

QoL_EventScript_SailToLittleroot::
	goto_if_eq VAR_LITTLEROOT_INTRO_STATE, 0, QoL_EventScript_TidalFirstTrip
	call QoL_EventScript_SetSail
	warp MAP_LITTLEROOT_TOWN, 10, 3
	waitstate
	release
	end

QoL_EventScript_SailToSlateport::
	call QoL_EventScript_SetSail
	warp MAP_SLATEPORT_CITY_HARBOR, 8, 11
	waitstate
	release
	end

QoL_EventScript_SailToLilycove::
	call QoL_EventScript_SetSail
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end

QoL_EventScript_SailToFrontier::
	call QoL_EventScript_SetSail
	warp MAP_BATTLE_FRONTIER_OUTSIDE_WEST, 19, 67
	waitstate
	release
	end

QoL_EventScript_SetSail::
	msgbox QoL_Text_TidalAllAboard, MSGBOX_DEFAULT
	closemessage
	delay 10
	playse SE_SHIP
	fadescreen FADE_TO_BLACK
	delay 60
	return

@ Hoenn side: asked first at the Slateport, Lilycove and Battle Frontier ferry
@ desks. NO returns to the desk's own menu.
QoL_EventScript_FerryOfferKanto::
	msgbox QoL_Text_OfferKanto, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, QoL_EventScript_SailToKanto
	return

QoL_EventScript_SailToKanto::
	call QoL_EventScript_SetSail
	warp MAP_VERMILION_CITY, 23, 32
	waitstate
	release
	end

LittlerootTown_EventScript_ShuttleDriver::
	lock
	faceplayer
	msgbox QoL_Text_ShuttleDriver, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, QoL_EventScript_SailToKanto
	msgbox QoL_Text_TidalComeBack, MSGBOX_DEFAULT
	release
	end

QoL_Text_TidalGreeting::
	.string "Ahoy! This pier is for the S.S. TIDAL.\p"
	.string "She sails across the sea to the\n"
	.string "HOENN region!$"
	.string "{JPN}{JPN}おーい！　ここは　タイドリップごうの　のりばだ。\p"
	.string "ホウエン　ちほうまで　うみを　わたって\n"
	.string "いくんだ！$"

QoL_Text_TidalNeedTicket::
	.string "You'll need an S.S. TICKET to board,\n"
	.string "though.$"
	.string "{JPN}{JPN}ただし　のるには　ふねのチケットが\n"
	.string "ひつようだぞ。$"

QoL_Text_TidalFirstTrip::
	.string "Oh, an S.S. TICKET! That'll do.\p"
	.string "First time going to HOENN?\p"
	.string "When we dock, a moving truck will\n"
	.string "take you to LITTLEROOT TOWN.\p"
	.string "PROF. BIRCH lives there. He studies\n"
	.string "POKéMON out in the wild.\p"
	.string "Shall we set sail?$"
	.string "{JPN}{JPN}おっ　ふねのチケット！　それなら\n"
	.string "だいじょうぶだ。\p"
	.string "ホウエンは　はじめてかい？\p"
	.string "ふねが　ついたら　ひっこしの　トラックで\n"
	.string "ミシロタウンまで　おくって　もらえるぞ。\p"
	.string "ミシロタウンには　やせいの　ポケモンを\n"
	.string "けんきゅうしている　オダマキはかせが　いるんだ。\p"
	.string "さあ　しゅっこう　するかい？$"

QoL_Text_TidalWhereTo::
	.string "Where would you like to sail?$"
	.string "{JPN}{JPN}どこへ いくかい？$"

QoL_Text_TidalAllAboard::
	.string "All aboard!\n"
	.string "The S.S. TIDAL is setting sail!$"
	.string "{JPN}{JPN}ごじょうせん　ください！　タイドリップごう\n"
	.string "しゅっこう！$"

QoL_Text_TidalComeBack::
	.string "Come back anytime!$"
	.string "{JPN}{JPN}また いつでも きてくれよな！$"

QoL_Text_OfferKanto::
	.string "The S.S. TIDAL also sails to KANTO.\p"
	.string "Would you like to go to\n"
	.string "VERMILION CITY?$"
	.string "{JPN}{JPN}タイドリップごうは　カントーへも　いくぞ。\p"
	.string "クチバシティへ　いくかい？$"

QoL_Text_ShuttleDriver::
	.string "I drive the shuttle truck to the\n"
	.string "S.S. TIDAL's port.\p"
	.string "The ship sails to VERMILION CITY in\n"
	.string "KANTO. Want a lift?$"
	.string "{JPN}{JPN}わたしは　タイドリップごうの　みなとへ　いく\n"
	.string "トラックの　うんてんしゅさ。\p"
	.string "ふねは　カントーの　クチバシティへ　いくんだ。\n"
	.string "のっていくかい？$"

QoL_Text_DestLittleroot::
	.string "LITTLEROOT TOWN$"
	.string "{JPN}{JPN}ミシロタウン$"

QoL_Text_DestSlateport::
	.string "SLATEPORT CITY$"
	.string "{JPN}{JPN}カイナシティ$"

QoL_Text_DestLilycove::
	.string "LILYCOVE CITY$"
	.string "{JPN}{JPN}ミナモシティ$"

QoL_Text_DestFrontier::
	.string "BATTLE FRONTIER$"
	.string "{JPN}{JPN}バトルフロンティア$"

QoL_Text_DestBirthIsland::
	.string "BIRTH ISLAND$"
	.string "{JPN}{JPN}たんじょうのしま$"

QoL_Text_DestStarfallIsle::
	.string "STARFALL ISLE$"
	.string "{JPN}{JPN}スターフォールとう$"

QoL_Text_StarfallIsleSign::
	.string "STARFALL ISLE\p"
	.string "Meteors from the STARFALL fell\n"
	.string "on this island.\p"
	.string "The POKéMON here roam in the open.\n"
	.string "Walk into one to battle it, or go\l"
	.string "around it if you'd rather not.$"
	.string "{JPN}{JPN}スターフォールとう\p"
	.string "スターフォールの　ながれぼしが　この　しまに\n"
	.string "おちた。\p"
	.string "ここの　ポケモンは　そとを　うろついている。\n"
	.string "ふれれば　バトル！　たたかいたく　なければ\l"
	.string "よけて　とおろう。$"

QoL_Text_DestSeika::
	.string "SEIKA$"
	.string "{JPN}{JPN}セイカちほう$"

QoL_Text_DestCancel::
	.string "CANCEL$"
	.string "{JPN}{JPN}やめる$"
