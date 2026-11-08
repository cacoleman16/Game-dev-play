CeladonCity_DepartmentStore_1F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_1F_EventScript_Receptionist::
	msgbox CeladonCity_DepartmentStore_1F_Text_WelcomeToDeptStore, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_1F_EventScript_LayoutSign::
	msgbox CeladonCity_DepartmentStore_1F_Text_FloorDescriptions, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_1F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_1F_Text_ServiceCounter, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_1F_Text_WelcomeToDeptStore::
	.string "Hello!\n"
	.string "Welcome to CELADON DEPT. STORE.\p"
	.string "The board on the right describes\n"
	.string "the store layout.$"
	.string "{JPN}{JPN}いらっしゃいませ！　タマムシデパートへ　ようこそ\p"
	.string "みぎの　あんないばんに　うりばの　ごあんないが\n"
	.string "ございます$"

CeladonCity_DepartmentStore_1F_Text_FloorDescriptions::
	.string "1F: SERVICE COUNTER\p"
	.string "2F: TRAINER'S MARKET\p"
	.string "3F: TV GAME SHOP\p"
	.string "4F: WISE MAN GIFTS\p"
	.string "5F: DRUGSTORE\p"
	.string "ROOFTOP SQUARE: VENDING MACHINES$"
	.string "{JPN}{JPN}1F:　サービスカウンター\p"
	.string "2F:　トレーナーズマーケット\p"
	.string "3F:　テレビゲームショップ\p"
	.string "4F:　かしこい　おくりもの\p"
	.string "5F:　ドラッグストア\p"
	.string "おくじょう　ひろば:　じどうはんばいき$"

CeladonCity_DepartmentStore_1F_Text_ServiceCounter::
	.string "1F: SERVICE COUNTER$"
	.string "{JPN}{JPN}1F:　サービスカウンター$"

