.equ RECEIVED_TOWN_MAP, VAR_TEMP_1

PalletTown_RivalsHouse_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_RivalsHouse_OnTransition
	.byte 0

PalletTown_RivalsHouse_OnTransition::
	call_if_lt VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_MoveDaisyToTable
	call_if_ge VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap
	end

PalletTown_RivalsHouse_EventScript_MoveDaisyToTable::
	setobjectmovementtype LOCALID_DAISY, MOVEMENT_TYPE_FACE_RIGHT
	setobjectxyperm LOCALID_DAISY, 5, 4
	return

PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap::
	setvar RECEIVED_TOWN_MAP, TRUE
	return

PalletTown_RivalsHouse_EventScript_Daisy::
	lock
	faceplayer
	famechecker FAMECHECKER_DAISY, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	goto_if_set FLAG_SYS_GAME_CLEAR, PalletTown_RivalsHouse_EventScript_GroomMon
	goto_if_eq RECEIVED_TOWN_MAP, TRUE, PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_ExplainTownMap
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 1, PalletTown_RivalsHouse_EventScript_GiveTownMap
	goto_if_ge VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 1, PalletTown_RivalsHouse_EventScript_HeardBattledRival
	msgbox PalletTown_RivalsHouse_Text_HiBrothersAtLab
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_RivalsHouse_EventScript_HeardBattledRival::
	msgbox PalletTown_RivalsHouse_Text_HeardYouBattledRival
	release
	end

PalletTown_RivalsHouse_EventScript_GroomMon::
	goto_if_lt VAR_MASSAGE_COOLDOWN_STEP_COUNTER, 500, PalletTown_RivalsHouse_EventScript_RateMonFriendship
	msgbox PalletTown_RivalsHouse_Text_LikeMeToGroomMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	msgbox PalletTown_RivalsHouse_Text_GroomWhichOne
	special ChoosePartyMon
	lock
	faceplayer
	goto_if_ge VAR_0x8004, PARTY_SIZE, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, PalletTown_RivalsHouse_EventScript_CantGroomEgg
	msgbox PalletTown_RivalsHouse_Text_LookingNiceInNoTime
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special DaisyMassageServices
	fadescreen FADE_FROM_BLACK
	special BufferMonNickname
	msgbox PalletTown_RivalsHouse_Text_ThereYouGoAllDone
	release
	end

PalletTown_RivalsHouse_EventScript_CantGroomEgg::
	msgbox PalletTown_RivalsHouse_Text_CantGroomAnEgg
	release
	end

PalletTown_RivalsHouse_EventScript_DeclineGrooming::
	msgbox PalletTown_RivalsHouse_Text_DontNeedAnyGrooming
	release
	end

PalletTown_RivalsHouse_EventScript_RateMonFriendship::
	msgbox PalletTown_RivalsHouse_Text_MayISeeFirstMon
	specialvar VAR_RESULT, GetLeadMonFriendship
	switch VAR_RESULT
	case 0, PalletTown_RivalsHouse_EventScript_MonFriendshipLowest
	case 1, PalletTown_RivalsHouse_EventScript_MonFriendshipLower
	case 2, PalletTown_RivalsHouse_EventScript_MonFriendshipLow
	case 3, PalletTown_RivalsHouse_EventScript_MonFriendshipMid
	case 4, PalletTown_RivalsHouse_EventScript_MonFriendshipHigh
	case 5, PalletTown_RivalsHouse_EventScript_MonFriendshipHigher
	case 6, PalletTown_RivalsHouse_EventScript_MonFriendshipHighest
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLowest::
	msgbox PalletTown_RivalsHouse_Text_WhyWouldMonHateYouSoMuch
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLower::
	msgbox PalletTown_RivalsHouse_Text_DontLikeWayItGlaresAtYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLow::
	msgbox PalletTown_RivalsHouse_Text_NotFamiliarWithYouYet
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipMid::
	msgbox PalletTown_RivalsHouse_Text_ItsWarmingUpToYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigh::
	msgbox PalletTown_RivalsHouse_Text_ItsQuiteFriendly
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigher::
	msgbox PalletTown_RivalsHouse_Text_ItLooksVeryHappy
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHighest::
	msgbox PalletTown_RivalsHouse_Text_CouldntLoveYouMore
	release
	end

PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest::
	msgbox PalletTown_RivalsHouse_Text_PleaseGiveMonsRest
	release
	end

PalletTown_RivalsHouse_EventScript_GiveTownMap::
	msgbox PalletTown_RivalsHouse_Text_ErrandForGrandpaThisWillHelp
	closemessage
	checkitemspace ITEM_TOWN_MAP
	goto_if_eq VAR_RESULT, FALSE, PalletTown_RivalsHouse_EventScript_NoRoomForTownMap
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	removeobject LOCALID_TOWN_MAP
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	delay 15
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	delay 12
	giveitem_msg PalletTown_RivalsHouse_Text_ReceivedTownMapFromDaisy, ITEM_TOWN_MAP, 1, MUS_RG_OBTAIN_KEY_ITEM
	release
	end

PalletTown_RivalsHouse_EventScript_NoRoomForTownMap::
	msgbox PalletTown_RivalsHouse_Text_DontHaveSpaceForThis
	release
	end

PalletTown_RivalsHouse_EventScript_ExplainTownMap::
	msgbox PalletTown_RivalsHouse_Text_ExplainTownMap
	release
	end

PalletTown_RivalsHouse_EventScript_TownMap::
	msgbox PalletTown_RivalsHouse_Text_ItsBigMapOfKanto, MSGBOX_NPC
	end

PalletTown_RivalsHouse_EventScript_Bookshelf::
	msgbox PalletTown_RivalsHouse_Text_ShelvesCrammedFullOfBooks, MSGBOX_SIGN
	end

PalletTown_RivalsHouse_EventScript_Picture::
	msgbox PalletTown_RivalsHouse_Text_LovelyAndSweetClefairy, MSGBOX_SIGN
	end

PalletTown_RivalsHouse_Text_HiBrothersAtLab::
	.string "DAISY: Hi, {PLAYER}!\p"
	.string "My brother, {RIVAL}, is out at\n"
	.string "Grandpa's LAB.$"
	.string "{JPN}{JPN}ナナミ『あら　{PLAYER}！\p"
	.string "おとうとの　{RIVAL}なら　おじいちゃんの\n"
	.string "けんきゅうじょに　いるわよ。』$"

PalletTown_RivalsHouse_Text_HeardYouBattledRival::
	.string "DAISY: {PLAYER}, I heard you had\n"
	.string "a battle against {RIVAL}.\p"
	.string "I wish I'd seen that!$"
	.string "{JPN}{JPN}ナナミ『{PLAYER}　{RIVAL}と\n"
	.string "しょうぶ　したんですって？\p"
	.string "わたしも　みたかったなあ！』$"

PalletTown_RivalsHouse_Text_ErrandForGrandpaThisWillHelp::
	.string "Grandpa asked you to run an\n"
	.string "errand?\p"
	.string "Gee, that's lazy of him.\n"
	.string "Here, this will help you.$"
	.string "{JPN}{JPN}おじいちゃんに　おつかいを　たのまれたの？\p"
	.string "もう　なまけものなんだから。　はい　これ　やくに\n"
	.string "たつわよ。$"

PalletTown_RivalsHouse_Text_ReceivedTownMapFromDaisy::
	.string "{PLAYER} received a TOWN MAP\n"
	.string "from DAISY.$"
	.string "{JPN}{JPN}{PLAYER}は　ナナミから　タウンマップを\n"
	.string "もらった！$"

PalletTown_RivalsHouse_Text_DontHaveSpaceForThis::
	.string "You don't have space for this in\n"
	.string "your BAG.$"
	.string "{JPN}{JPN}バッグに　はいる　ばしょが　ないみたいね。$"

PalletTown_RivalsHouse_Text_ExplainTownMap::
	.string "You can use the TOWN MAP to find\n"
	.string "out where you are, or check the\l"
	.string "names of places.$"
	.string "{JPN}{JPN}タウンマップを　つかえば　いま　どこに　いるのか\n"
	.string "わかるし　ばしょの　なまえも　しらべられるわ。$"

PalletTown_RivalsHouse_Text_PleaseGiveMonsRest::
	.string "DAISY: Just like people, POKéMON\n"
	.string "are living things.\p"
	.string "When they get tired, please give\n"
	.string "them a rest.$"
	.string "{JPN}{JPN}ナナミ『ポケモンも　ひとと　おなじ　いきものよ。\p"
	.string "つかれたら　ちゃんと　やすませて　あげてね。』$"

PalletTown_RivalsHouse_Text_ItsBigMapOfKanto::
	.string "It's a big map of the KANTO region.\n"
	.string "Now this would be useful!$"
	.string "{JPN}{JPN}カントーちほうの　おおきな　ちずだ。　これは\n"
	.string "やくに　たちそう！$"

PalletTown_RivalsHouse_Text_ShelvesCrammedFullOfBooks::
	.string "The shelves are crammed full of\n"
	.string "books on POKéMON.$"
	.string "{JPN}{JPN}ほんだなには　ポケモンの　ほんが　ぎっしり\n"
	.string "つまっている。$"

PalletTown_RivalsHouse_Text_LikeMeToGroomMon::
	.string "DAISY: Hi, {PLAYER}!\n"
	.string "Good timing.\p"
	.string "I'm about to have some tea.\n"
	.string "Would you like to join me?\p"
	.string "Oh, but look.\n"
	.string "Your POKéMON are a little dirty.\p"
	.string "Would you like me to groom one?$"
	.string "{JPN}{JPN}ナナミ『あら　{PLAYER}！　いい　ところに\n"
	.string "きたわね。\p"
	.string "いまから　おちゃに　するところなの。　いっしょに\n"
	.string "どう？\p"
	.string "あら　でも　みて。　あなたの　ポケモン　ちょっと\n"
	.string "よごれてるわね。\p"
	.string "1ぴき　けづくろい　して　あげましょうか？』$"

PalletTown_RivalsHouse_Text_DontNeedAnyGrooming::
	.string "You don't need any grooming done?\n"
	.string "Okay, we'll just have tea.$"
	.string "{JPN}{JPN}けづくろいは　いいの？　じゃあ　おちゃだけ\n"
	.string "のみましょう。$"

PalletTown_RivalsHouse_Text_GroomWhichOne::
	.string "Which one should I groom?$"
	.string "{JPN}{JPN}どの　こを　けづくろい　しようかしら？$"

PalletTown_RivalsHouse_Text_LookingNiceInNoTime::
	.string "DAISY: Okay, I'll get it looking\n"
	.string "nice in no time.$"
	.string "{JPN}{JPN}ナナミ『まかせて！　すぐに　きれいに\n"
	.string "してあげるわ。』$"

PalletTown_RivalsHouse_Text_ThereYouGoAllDone::
#ifdef BUGFIX @ The localizers missed what should be a textcolor change in the localizations.
	.string "{COLOR DARK_GRAY}{STR_VAR_1} looks dreamily content…\p"
	.string "{COLOR RED}DAISY: There you go! All done.\n"
#else @ In the JP games, gender-based text used a different font instead of different colors.
	.string "{FONT_NORMAL}{STR_VAR_1} looks dreamily content…\p"
	.string "{FONT_FEMALE}DAISY: There you go! All done.\n"
#endif
	.string "See? Doesn't it look nice?\p"
	.string "Giggle…\n"
	.string "It's such a cute POKéMON.$"

PalletTown_RivalsHouse_Text_CantGroomAnEgg::
	.string "Oh, sorry. I honestly can't\n"
	.string "groom an EGG.$"
	.string "{JPN}{JPN}あら　ごめんなさい。　タマゴは　さすがに\n"
	.string "けづくろい　できないわ。$"

PalletTown_RivalsHouse_Text_MayISeeFirstMon::
	.string "DAISY: Your POKéMON grow to love\n"
	.string "you if you raise them with love.\p"
	.string "For example, {PLAYER}, may I see\n"
	.string "your first POKéMON?$"
	.string "{JPN}{JPN}ナナミ『ポケモンは　あいじょうを　こめて\n"
	.string "そだてると　あなたの　ことを　すきに　なって\l"
	.string "くれるのよ。\p"
	.string "たとえば　{PLAYER}の　せんとうの　ポケモン\n"
	.string "ちょっと　みせて　くれる？』$"

PalletTown_RivalsHouse_Text_CouldntLoveYouMore::
	.string "It couldn't possibly love you\n"
	.string "any more than it does now.\p"
	.string "Your POKéMON is happy beyond\n"
	.string "words.$"
	.string "{JPN}{JPN}これいじょう　ない　くらい　あなたの　ことが\n"
	.string "だいすき　みたい。\p"
	.string "あなたの　ポケモン　ことばに　できない　くらい\n"
	.string "しあわせそうよ。$"

PalletTown_RivalsHouse_Text_ItLooksVeryHappy::
	.string "It looks very happy.\p"
	.string "I wish {RIVAL} could see this and\n"
	.string "learn something from it.$"
	.string "{JPN}{JPN}とても　しあわせそうね。\p"
	.string "{RIVAL}にも　これを　みせて　すこしは\n"
	.string "みならって　ほしいわ。$"

PalletTown_RivalsHouse_Text_ItsQuiteFriendly::
	.string "It's quite friendly with you.\n"
	.string "Keep being good to it!$"
	.string "{JPN}{JPN}あなたに　とても　なついてるわね。　これからも\n"
	.string "かわいがって　あげてね！$"

PalletTown_RivalsHouse_Text_ItsWarmingUpToYou::
	.string "It's warming up to you.\n"
	.string "Trust must be growing between you.$"
	.string "{JPN}{JPN}だんだん　あなたに　なれて　きたみたい。\n"
	.string "ふたりの　あいだに　しんらいが　うまれて\l"
	.string "きてるのね。$"

PalletTown_RivalsHouse_Text_NotFamiliarWithYouYet::
	.string "It's not quite familiar with you\n"
	.string "yet.\p"
	.string "POKéMON are all quite wary when\n"
	.string "you first get them.$"
	.string "{JPN}{JPN}まだ　あなたに　あまり　なれて　いないみたい。\p"
	.string "ポケモンは　もらった　ばかりの　ころは　みんな\n"
	.string "けいかい　する　ものなのよ。$"

PalletTown_RivalsHouse_Text_DontLikeWayItGlaresAtYou::
	.string "{PLAYER}, I don't like the way it\n"
	.string "glares at you.\p"
	.string "Could you try being a little nicer\n"
	.string "to it?$"
	.string "{JPN}{JPN}{PLAYER}　この　こ　あなたを　にらんでる\n"
	.string "みたいで　ちょっと　きに　なるわ。\p"
	.string "もうすこし　やさしく　して　あげられない？$"

PalletTown_RivalsHouse_Text_WhyWouldMonHateYouSoMuch::
	.string "…Um, it's not easy for me to say\n"
	.string "this, but…\p"
	.string "Is there some reason why your\n"
	.string "POKéMON would hate you so much?$"
	.string "{JPN}{JPN}‥えっと　いいにくいん　だけど‥\p"
	.string "あなたの　ポケモン　どうして　こんなに　あなたの\n"
	.string "ことを　きらって　いるのかしら？$"

PalletTown_RivalsHouse_Text_LovelyAndSweetClefairy::
	.string "“The lovely and sweet\n"
	.string "CLEFAIRY”$"
	.string "{JPN}{JPN}『かわいくて　すてきな　ピッピ』$"

