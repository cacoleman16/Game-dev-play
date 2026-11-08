SSAnne_Kitchen_Frlg_MapScripts::
	.byte 0

SSAnne_Kitchen_EventScript_Chef1::
	msgbox SSAnne_Kitchen_Text_BusyOutOfTheWay, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef2::
	msgbox SSAnne_Kitchen_Text_SawOddBerryInTrash, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef3::
	msgbox SSAnne_Kitchen_Text_SoBusyImDizzy, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef4::
	lock
	faceplayer
	msgbox SSAnne_Kitchen_Text_IAmLeChefMainCourseIs
	random 3
	copyvar VAR_0x8008, VAR_RESULT
	call_if_eq VAR_0x8008, 0, SSAnne_Kitchen_EventScript_SalmonDuSalad
	call_if_eq VAR_0x8008, 1, SSAnne_Kitchen_EventScript_EelsAuBarbecue
	call_if_eq VAR_0x8008, 2, SSAnne_Kitchen_EventScript_PrimeBeefsteak
	release
	end

SSAnne_Kitchen_EventScript_SalmonDuSalad::
	msgbox SSAnne_Kitchen_Text_SalmonDuSalad
	return

SSAnne_Kitchen_EventScript_EelsAuBarbecue::
	msgbox SSAnne_Kitchen_Text_EelsAuBarbecue
	return

SSAnne_Kitchen_EventScript_PrimeBeefsteak::
	msgbox SSAnne_Kitchen_Text_PrimeBeefsteak
	return

SSAnne_Kitchen_EventScript_Chef5::
	msgbox SSAnne_Kitchen_Text_PeelSpudsEveryDay, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef6::
	msgbox SSAnne_Kitchen_Text_HearAboutSnorlaxItsAGlutton, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef7::
	msgbox SSAnne_Kitchen_Text_OnlyGetToPeelOnions, MSGBOX_NPC
	end

SSAnne_Kitchen_Text_BusyOutOfTheWay::
	.string "You, mon petit!\n"
	.string "We're busy here!\l"
	.string "Out of the way!$"
	.string "{JPN}{JPN}ちょっと　そこの　きみ！　いそがしいんだ！\n"
	.string "どいて　どいて！$"

SSAnne_Kitchen_Text_SawOddBerryInTrash::
	.string "I saw an odd BERRY in the trash.\n"
	.string "I wonder what that was?$"
	.string "{JPN}{JPN}ゴミばこに　へんな　きのみが　あったよ。\n"
	.string "なんだったん　だろう？$"

SSAnne_Kitchen_Text_SoBusyImDizzy::
	.string "I'm so busy I'm getting dizzy!\n"
	.string "You have to give me room here!$"
	.string "{JPN}{JPN}いそがしくて　めが　まわりそうだ！　そこを\n"
	.string "あけて　くれ！$"

SSAnne_Kitchen_Text_PeelSpudsEveryDay::
	.string "Hum-de-hum-de-ho…\p"
	.string "I peel spuds every day!\n"
	.string "Hum-hum…$"
	.string "{JPN}{JPN}ふんふん　ふーん‥\p"
	.string "まいにち　じゃがいもの　かわむき　だよ！\n"
	.string "ふんふん‥$"

SSAnne_Kitchen_Text_HearAboutSnorlaxItsAGlutton::
	.string "Did you hear about SNORLAX?\n"
	.string "It's a glutton.\p"
	.string "No other POKéMON eats and sleeps\n"
	.string "the way SNORLAX can and does!$"
	.string "{JPN}{JPN}カビゴンの　こと　しってる？　すごい\n"
	.string "くいしんぼう　なんだ。\p"
	.string "カビゴン　ほど　よく　たべて　よく　ねる\n"
	.string "ポケモンは　いないよ！$"

SSAnne_Kitchen_Text_OnlyGetToPeelOnions::
	.string "Snivel… Sniff…\p"
	.string "I only get to peel onions…\n"
	.string "Snivel…$"
	.string "{JPN}{JPN}ぐすっ‥　ぐすん‥\p"
	.string "ぼくは　たまねぎの　かわむき　ばっかり‥\n"
	.string "ぐすっ‥$"

SSAnne_Kitchen_Text_IAmLeChefMainCourseIs::
	.string "Er-hem!\n"
	.string "Indeed I am le CHEF!\p"
	.string "Le main course is$"
	.string "{JPN}{JPN}えっへん！　わたくしこそ　この　ふねの　シェフ\n"
	.string "で　ございます！\p"
	.string "きょうの　メインディッシュは$"

SSAnne_Kitchen_Text_SalmonDuSalad::
	.string "Salmon du Salad!\p"
	.string "Les guests may gripe it's fish\n"
	.string "again, however!$"
	.string "{JPN}{JPN}サーモンの　サラダ！\p"
	.string "おきゃくさまに　また　さかなか　と　いわれそう\n"
	.string "ですが！$"

SSAnne_Kitchen_Text_EelsAuBarbecue::
	.string "Eels au Barbecue!\p"
	.string "Les guests will mutiny, I fear.$"
	.string "{JPN}{JPN}ウナギの　バーベキュー！\p"
	.string "おきゃくさまが　はんらんを　おこしそう　ですな。$"

SSAnne_Kitchen_Text_PrimeBeefsteak::
	.string "Prime Beefsteak!\p"
	.string "But, have I enough fillets du\n"
	.string "beef?$"
	.string "{JPN}{JPN}とくせん　ビーフステーキ！\p"
	.string "しかし　ヒレにくは　たりるかな？$"

