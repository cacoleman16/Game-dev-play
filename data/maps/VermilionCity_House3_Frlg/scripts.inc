VermilionCity_House3_Frlg_MapScripts::
	.byte 0

VermilionCity_House3_EventScript_Boy::
	msgbox VermilionCity_House3_Text_PidgeyFlyLetterToSaffron, MSGBOX_NPC
	end

VermilionCity_House3_EventScript_Lass::
	msgbox VermilionCity_House3_Text_SendMyPidgeyToUnionRoom, MSGBOX_NPC
	end

VermilionCity_House3_EventScript_Pidgey::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIDGEY, CRY_MODE_NORMAL
	msgbox VermilionCity_House3_Text_Pidgey
	waitmoncry
	release
	end

VermilionCity_House3_EventScript_Letter::
	msgbox VermilionCity_House3_Text_DearPippiLetter, MSGBOX_SIGN
	end

VermilionCity_House3_Text_PidgeyFlyLetterToSaffron::
	.string "I'm getting my PIDGEY to fly a\n"
	.string "letter to SAFFRON in the north.$"
	.string "{JPN}{JPN}ポッポに　たのんで　きたの　ヤマブキまで\n"
	.string "てがみを　とどけて　もらうの$"

VermilionCity_House3_Text_Pidgey::
	.string "PIDGEY: Kurukkoo!$"
	.string "{JPN}{JPN}ポッポ『クルックー！』$"

VermilionCity_House3_Text_DearPippiLetter::
	.string "Dear PIPPI,\n"
	.string "I hope to see you soon.\p"
	.string "I heard SAFFRON has problems with\n"
	.string "TEAM ROCKET.\p"
	.string "VERMILION appears to be safe.$"
	.string "{JPN}{JPN}ピッピへ　はやく　あいたいな\p"
	.string "ヤマブキは　ロケットだんの　せいで　たいへん\n"
	.string "だって　きいたよ\p"
	.string "クチバは　だいじょうぶ　みたい$"

VermilionCity_House3_Text_SendMyPidgeyToUnionRoom::
	.string "I want to exchange MAIL with all\n"
	.string "sorts of people.\p"
	.string "I send my PIDGEY to a UNION ROOM\n"
	.string "to exchange MAIL for me.$"
	.string "{JPN}{JPN}いろんな　ひとと　メールを　こうかん　したいの\p"
	.string "だから　ポッポを　ユニオンルームに　おくって\n"
	.string "メールを　こうかん　して　もらってるの$"

