.set SPOKE_TO_WOMAN_LAST,   FLAG_TEMP_2
.set SPOKE_TO_FAT_MAN_LAST, FLAG_TEMP_3

VermilionCity_PokemonFanClub_Frlg_MapScripts::
	.byte 0

VermilionCity_PokemonFanClub_EventScript_Chairman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_BIKE_VOUCHER, VermilionCity_PokemonFanClub_EventScript_AlreadyHeardStory
	msgbox VermilionCity_PokemonFanClub_Text_DidYouComeToHearAboutMyMons, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, VermilionCity_PokemonFanClub_EventScript_ChairmanStory
	msgbox VermilionCity_PokemonFanClub_Text_ComeBackToHearStory
	release
	end

VermilionCity_PokemonFanClub_EventScript_AlreadyHeardStory::
	msgbox VermilionCity_PokemonFanClub_Text_DidntComeToSeeAboutMonsAgain
	release
	end

VermilionCity_PokemonFanClub_EventScript_ChairmanStory::
	msgbox VermilionCity_PokemonFanClub_Text_ChairmansStory
	checkitemspace ITEM_BIKE_VOUCHER
	goto_if_eq VAR_RESULT, FALSE, VermilionCity_PokemonFanClub_EventScript_NoRoomForBikeVoucher
	setflag FLAG_GOT_BIKE_VOUCHER
	giveitem_msg VermilionCity_PokemonFanClub_Text_ReceivedBikeVoucherFromChairman, ITEM_BIKE_VOUCHER, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox VermilionCity_PokemonFanClub_Text_ExplainBikeVoucher
	release
	end

VermilionCity_PokemonFanClub_EventScript_NoRoomForBikeVoucher::
	msgbox VermilionCity_PokemonFanClub_Text_MakeRoomForThis
	release
	end

VermilionCity_PokemonFanClub_EventScript_WorkerF::
	lock
	faceplayer
	goto_if_set FLAG_SYS_GAME_CLEAR, VermilionCity_PokemonFanClub_EventScript_WorkerFGameClear
	msgbox VermilionCity_PokemonFanClub_Text_ChairmanVeryVocalAboutPokemon
	release
	end

VermilionCity_PokemonFanClub_EventScript_WorkerFGameClear::
	famechecker FAMECHECKER_DAISY, 1
	msgbox VermilionCity_PokemonFanClub_Text_ChairmanReallyAdoresHisMons
	release
	end

VermilionCity_PokemonFanClub_EventScript_Woman::
	lock
	faceplayer
	goto_if_set SPOKE_TO_FAT_MAN_LAST, VermilionCity_PokemonFanClub_EventScript_WomanSpokeToFatMan
	msgbox VermilionCity_PokemonFanClub_Text_AdoreMySeel
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag SPOKE_TO_WOMAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_WomanSpokeToFatMan::
	msgbox VermilionCity_PokemonFanClub_Text_SeelFarMoreAttractive
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	clearflag SPOKE_TO_FAT_MAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_FatMan::
	lock
	faceplayer
	goto_if_set SPOKE_TO_WOMAN_LAST, VermilionCity_PokemonFanClub_EventScript_FatManSpokeToWoman
	msgbox VermilionCity_PokemonFanClub_Text_AdmirePikachusTail
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag SPOKE_TO_FAT_MAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_FatManSpokeToWoman::
	msgbox VermilionCity_PokemonFanClub_Text_PikachuTwiceAsCute
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	clearflag SPOKE_TO_WOMAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_Pikachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox VermilionCity_PokemonFanClub_Text_Pikachu
	waitmoncry
	release
	end

VermilionCity_PokemonFanClub_EventScript_Seel::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SEEL, CRY_MODE_NORMAL
	msgbox VermilionCity_PokemonFanClub_Text_Seel
	waitmoncry
	release
	end

VermilionCity_PokemonFanClub_EventScript_RulesSign1::
	msgbox VermilionCity_PokemonFanClub_Text_ListenPolitelyToOtherTrainers, MSGBOX_SIGN
	end

VermilionCity_PokemonFanClub_EventScript_RulesSign2::
	msgbox VermilionCity_PokemonFanClub_Text_SomeoneBragsBragBack, MSGBOX_SIGN
	end

VermilionCity_PokemonFanClub_Text_AdmirePikachusTail::
	.string "Won't you admire my PIKACHU's\n"
	.string "adorable tail?$"
	.string "{JPN}{JPN}わたしの　ピカチュウの　かわいい　しっぽ　みて\n"
	.string "くれない？$"

VermilionCity_PokemonFanClub_Text_PikachuTwiceAsCute::
	.string "Humph!\p"
	.string "My PIKACHU is twice as cute as\n"
	.string "that one!$"
	.string "{JPN}{JPN}ふん！\p"
	.string "わたしの　ピカチュウの　ほうが　その　こより\n"
	.string "2ばいは　かわいいわ！$"

VermilionCity_PokemonFanClub_Text_AdoreMySeel::
	.string "I just adore my SEEL!\n"
	.string "It's so lovable!\p"
	.string "It squeals, “Kyuuuh,” when I\n"
	.string "hug it!$"
	.string "{JPN}{JPN}ぼくは　パウワウが　だいすき！　とっても\n"
	.string "かわいいんだ！\p"
	.string "だきしめると　『キューッ』って　なくんだよ！$"

VermilionCity_PokemonFanClub_Text_SeelFarMoreAttractive::
	.string "Oh, dear!\p"
	.string "My SEEL is far more attractive.\n"
	.string "By double, I would say.$"
	.string "{JPN}{JPN}まったく！\p"
	.string "ぼくの　パウワウの　ほうが　ずっと　ステキさ\n"
	.string "2ばいは　ね$"

VermilionCity_PokemonFanClub_Text_Pikachu::
	.string "PIKACHU: Chu! Pikachu!$"
	.string "{JPN}{JPN}ピカチュウ『チュウ！　ピカチュウ！』$"

VermilionCity_PokemonFanClub_Text_Seel::
	.string "SEEL: Kyuoo!$"
	.string "{JPN}{JPN}パウワウ『キュオー！』$"

VermilionCity_PokemonFanClub_Text_DidYouComeToHearAboutMyMons::
	.string "I chair the POKéMON Fan Club!\p"
	.string "I raise more than a hundred\n"
	.string "POKéMON!\p"
	.string "I'm very fussy when it comes to\n"
	.string "POKéMON! I surely am!\p"
	.string "So…\p"
	.string "Did you come visit to hear about\n"
	.string "my POKéMON?$"
	.string "{JPN}{JPN}わしは　ポケモンだいすきクラブの　かいちょう\n"
	.string "じゃ！\p"
	.string "ポケモンを　100ぴき　いじょう　そだてて\n"
	.string "おる！\p"
	.string "ポケモンの　ことには　うるさいぞ！　ほんとうに\n"
	.string "うるさいぞ！\p"
	.string "それで‥\p"
	.string "わしの　ポケモンの　はなしを　ききに\n"
	.string "きたのかね？$"

VermilionCity_PokemonFanClub_Text_ChairmansStory::
	.string "Good!\n"
	.string "Then listen up!\p"
	.string "My favorite RAPIDASH…\p"
	.string "It's…cute…lovely…smart… \n"
	.string "plus…amazing…you think so?…\l"
	.string "oh yes…it's…stunning…\l"
	.string "kindly…love it!\p"
	.string "Hug it…when…sleeping…warm\n"
	.string "and cuddly…spectacular…\l"
	.string "ravishing…\l"
	.string "…Oops! Look at the time!\l"
	.string "I kept you too long!\p"
	.string "Thanks for hearing me out!\n"
	.string "I want you to have this!$"
	.string "{JPN}{JPN}よし！　では　きくが　よい！\p"
	.string "わしの　おきにいりの　ギャロップは‥\p"
	.string "かわいくて‥　かれいで‥　かしこくて‥\n"
	.string "そのうえ‥　すごくて‥　そう　おもわんか？‥\l"
	.string "そうじゃ‥　まさに‥　みごと‥　やさしくて‥\l"
	.string "だいすき！\p"
	.string "ねている　ときに‥　だきしめると‥　あったかくて\n"
	.string "ふかふかで‥　すばらしくて‥　うっとり‥\l"
	.string "‥おっと！　もう　こんな　じかんか！　ながく\l"
	.string "ひきとめて　しまったのう！\p"
	.string "はなしを　きいて　くれて　ありがとう！　これを\n"
	.string "あげよう！$"

VermilionCity_PokemonFanClub_Text_ReceivedBikeVoucherFromChairman::
	.string "{PLAYER} received a BIKE VOUCHER\n"
	.string "from the CHAIRMAN.$"
	.string "{JPN}{JPN}{PLAYER}は　かいちょうから\n"
	.string "じてんしゃひきかえけんを　もらった$"

VermilionCity_PokemonFanClub_Text_ExplainBikeVoucher::
	.string "Take that BIKE VOUCHER to the\n"
	.string "BIKE SHOP in CERULEAN CITY.\p"
	.string "Exchange that for a BICYCLE\n"
	.string "free of charge!\p"
	.string "Don't worry, my favorite FEAROW\n"
	.string "will FLY me anywhere I need to go.\p"
	.string "So, I have no need for a BICYCLE.\p"
	.string "I hope you like cycling!$"
	.string "{JPN}{JPN}その　ひきかえけんを　ハナダシティの\n"
	.string "じてんしゃやに　もって　いきなさい\p"
	.string "ただで　じてんしゃと　こうかん　して\n"
	.string "もらえるぞ！\p"
	.string "しんぱい　いらん　わしは　おきにいりの\n"
	.string "オニドリルに　そらをとぶで　どこへでも　つれて\l"
	.string "いって　もらうからの\p"
	.string "だから　じてんしゃは　いらんのじゃ\p"
	.string "サイクリングを　たのしんで　おくれ！$"

VermilionCity_PokemonFanClub_Text_ComeBackToHearStory::
	.string "Oh. Come back when you want to\n"
	.string "hear my story!$"
	.string "{JPN}{JPN}そうか　わしの　はなしを　ききたく　なったら\n"
	.string "また　くるが　よい！$"

VermilionCity_PokemonFanClub_Text_DidntComeToSeeAboutMonsAgain::
	.string "Hello, {PLAYER}!\p"
	.string "Did you come see me about my\n"
	.string "POKéMON again?\p"
	.string "No?\n"
	.string "Too bad!$"
	.string "{JPN}{JPN}おお　{PLAYER}！\p"
	.string "また　わしの　ポケモンの　はなしを　ききに\n"
	.string "きたのかね？\p"
	.string "ちがう？　ざんねんじゃ！$"

VermilionCity_PokemonFanClub_Text_MakeRoomForThis::
	.string "Make room for this!$"
	.string "{JPN}{JPN}もちものを　へらして　おくのじゃ！$"

VermilionCity_PokemonFanClub_Text_ChairmanVeryVocalAboutPokemon::
	.string "Our CHAIRMAN is very vocal about\n"
	.string "POKéMON.$"
	.string "{JPN}{JPN}うちの　かいちょうは　ポケモンの　こととなると\n"
	.string "はなしが　とまらないの$"

VermilionCity_PokemonFanClub_Text_ListenPolitelyToOtherTrainers::
	.string "Let's all listen politely to other\n"
	.string "TRAINERS!$"
	.string "{JPN}{JPN}ほかの　トレーナーの　はなしは　ちゃんと\n"
	.string "きこうね！$"

VermilionCity_PokemonFanClub_Text_SomeoneBragsBragBack::
	.string "If someone brags, brag right back!$"
	.string "{JPN}{JPN}じまん　されたら　じまんし　かえせ！$"

VermilionCity_PokemonFanClub_Text_ChairmanReallyAdoresHisMons::
	.string "Our CHAIRMAN really does adore his\n"
	.string "POKéMON.\p"
	.string "But the person who is most liked by\n"
	.string "POKéMON is DAISY, I think.$"
	.string "{JPN}{JPN}かいちょうは　ほんとうに　ポケモンを\n"
	.string "かわいがってるわ\p"
	.string "でも　いちばん　ポケモンに　すかれてるのは\n"
	.string "ナナミさん　だと　おもうの$"
