CeladonCity_Condominiums_1F_Frlg_MapScripts::
	.byte 0

CeladonCity_Condominiums_1F_EventScript_TeaWoman::
	lock
	faceplayer
	goto_if_set FLAG_TALKED_TO_TEA_LADY_AFTER_HOF, CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea
	goto_if_set FLAG_SYS_GAME_CLEAR, CeladonCity_Condominiums_1F_EventScript_TeaWomanMentionDaisy
	goto_if_set FLAG_GOT_TEA, CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea
	msgbox CeladonCity_Condominiums_1F_Text_TryThisDrinkInstead
	setflag FLAG_GOT_TEA
	giveitem ITEM_TEA
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	msgbox CeladonCity_Condominiums_1F_Text_NothingBeatsThirstLikeTea
	release
	end

CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea::
	msgbox CeladonCity_Condominiums_1F_Text_MyDearMonsKeepMeCompany
	release
	end

CeladonCity_Condominiums_1F_EventScript_TeaWomanMentionDaisy::
	famechecker FAMECHECKER_DAISY, 4
	setflag FLAG_TALKED_TO_TEA_LADY_AFTER_HOF
	msgbox CeladonCity_Condominiums_1F_Text_DaisyComesToBuyTea
	release
	end

CeladonCity_Condominiums_1F_EventScript_Meowth::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Meowth
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_Clefairy::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CLEFAIRY, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Clefairy
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_Nidoran::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_NIDORAN_F, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Nidoran
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_SuiteSign::
	msgbox CeladonCity_Condominiums_1F_Text_ManagersSuite, MSGBOX_SIGN
	end

CeladonCity_Condominiums_1F_Text_Meowth::
	.string "MEOWTH: Meow!$"
	.string "{JPN}{JPN}ニャース『ニャーオ！』$"

CeladonCity_Condominiums_1F_Text_TryThisDrinkInstead::
	.string "You shouldn't spend all your money\n"
	.string "on drinks.\p"
	.string "Try this instead.$"
	.string "{JPN}{JPN}のみものばかりに　おかねを　つかっちゃ　だめよ\p"
	.string "かわりに　これを　あげるわ$"

CeladonCity_Condominiums_1F_Text_NothingBeatsThirstLikeTea::
	.string "Nothing beats thirst like some hot\n"
	.string "TEA.\p"
	.string "It really is the best.$"
	.string "{JPN}{JPN}のどが　かわいたら　あつい　おちゃが　いちばん！\p"
	.string "ほんとに　さいこうよ$"

CeladonCity_Condominiums_1F_Text_MyDearMonsKeepMeCompany::
	.string "My dear POKéMON keep me company.\n"
	.string "MEOWTH even brings money home!$"
	.string "{JPN}{JPN}かわいい　ポケモンたちが　いつも　そばに\n"
	.string "いてくれるの　ニャースは　おかねまで\l"
	.string "ひろってくるのよ！$"

CeladonCity_Condominiums_1F_Text_DaisyComesToBuyTea::
	.string "Oh, hello, dearie.\n"
	.string "Did you enjoy my TEA?\p"
	.string "By the way, dear. Are you, by any\n"
	.string "chance, from PALLET TOWN?\p"
	.string "A girl from PALLET TOWN, DAISY,\n"
	.string "she enjoys TEA every day.\p"
	.string "She visits the CELADON DEPT. STORE\n"
	.string "to buy some TEA.$"
	.string "{JPN}{JPN}あら　こんにちは　おちゃは　おいしかった？\p"
	.string "ところで　あなた　ひょっとして　マサラタウンの\n"
	.string "ひと？\p"
	.string "マサラタウンの　ナナミって　おんなのこがね\n"
	.string "まいにち　おちゃを　たのしんでるの\p"
	.string "タマムシデパートに　おちゃを　かいに　くるのよ$"

CeladonCity_Condominiums_1F_Text_Clefairy::
	.string "CLEFAIRY: Pi pippippi!$"
	.string "{JPN}{JPN}ピッピ『ピッピッピッピー！』$"

CeladonCity_Condominiums_1F_Text_Nidoran::
	.string "NIDORAN♀: Kya kyaoo!$"
	.string "{JPN}{JPN}ニドラン♀『キャッ　キャオー！』$"

CeladonCity_Condominiums_1F_Text_ManagersSuite::
	.string "CELADON MANSION\n"
	.string "Manager's Suite$"
	.string "{JPN}{JPN}タマムシマンション\n"
	.string "かんりにんしつ$"

