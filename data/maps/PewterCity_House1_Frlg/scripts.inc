PewterCity_House1_Frlg_MapScripts::
	.byte 0

PewterCity_House1_EventScript_BaldingMan::
	msgbox PewterCity_House1_Text_TradeMonsAreFinicky, MSGBOX_NPC
	end

PewterCity_House1_EventScript_LittleBoy::
	lock
	msgbox PewterCity_House1_Text_NidoranSit
	call PewterCity_House1_EventScript_DoNidoranCry
	release
	end

PewterCity_House1_EventScript_Nidoran::
	lock
	faceplayer
	call PewterCity_House1_EventScript_DoNidoranCry
	applymovement LOCALID_PEWTER_HOUSE_NIDORAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PewterCity_House1_EventScript_DoNidoranCry::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	waitse
	playmoncry SPECIES_NIDORAN_M, CRY_MODE_NORMAL
	msgbox PewterCity_House1_Text_Nidoran
	waitmoncry
	call EventScript_RestorePrevTextColor
	return

PewterCity_House1_Text_Nidoran::
	.string "NIDORAN♂: Bowbow!$"
	.string "{JPN}{JPN}ニドラン♂『ニドッ！』$"

PewterCity_House1_Text_NidoranSit::
	.string "NIDORAN, sit!$"
	.string "{JPN}{JPN}ニドラン　おすわり！$"

PewterCity_House1_Text_TradeMonsAreFinicky::
	.string "Our POKéMON's an outsider, so it's\n"
	.string "finicky and hard to handle.\p"
	.string "An outsider is a POKéMON that you\n"
	.string "get in a trade.\p"
	.string "It grows fast, but it may ignore an\n"
	.string "unskilled TRAINER in battle.\p"
	.string "If only we had some BADGES…$"
	.string "{JPN}{JPN}うちの　ポケモンは　よそもの　だから\n"
	.string "きむずかしくて　あつかいにくいんだ。\p"
	.string "よそものって　いうのは　こうかんで　もらった\n"
	.string "ポケモンの　ことさ。\p"
	.string "そだつのは　はやいけど　うでの　ない\n"
	.string "トレーナーだと　せんとうで　いうことを　きかない\l"
	.string "ことも　ある。\p"
	.string "バッジさえ　もってれば　なあ‥$"

