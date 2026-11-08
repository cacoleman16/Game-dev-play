SafariZone_EventScript_OutOfBallsMidBattle::
#if IS_FRLG
	setvar VAR_MAP_SCENE_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 3
#else
	setvar VAR_SAFARI_ZONE_STATE, 1
#endif
	special ExitSafariMode
#if IS_FRLG
	setwarp MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 4, 1
#else
	setwarp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
#endif
	end

SafariZone_EventScript_Exit::
#if IS_FRLG
	setvar VAR_MAP_SCENE_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 1
#else
	setvar VAR_SAFARI_ZONE_STATE, 1
#endif
	special ExitSafariMode
#if IS_FRLG
	warp MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 4, 1
#else
	warp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
#endif
	waitstate
	end

SafariZone_EventScript_RetirePrompt::
	lockall
	msgbox SafariZone_Text_WouldYouLikeToExit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_Retire
	releaseall
	end

SafariZone_EventScript_Retire::
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_TimesUp::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_TimesUp
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_OutOfBalls::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_OutOfBalls
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

EventScript_PokeBlockFeeder::
	lockall
	special GetPokeblockFeederInFront
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPresent
	msgbox SafariZone_Text_PlacePokeblockOnFeeder, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_ChoosePokeblock
	releaseall
	end

SafariZone_EventScript_ChoosePokeblock::
	fadescreen FADE_TO_BLACK
	special OpenPokeblockCaseOnFeeder
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPlaced
#ifdef BUGFIX
	releaseall  @ Only gets called from EventScript_PokeBlockFeeder which uses lockall.
#endif
	end

SafariZone_EventScript_PokeblockPlaced::
	message SafariZone_Text_PokeblockWasPlaced
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_EventScript_PokeblockPresent::
	message SafariZone_Text_PokeblockStillHere
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_Text_WouldYouLikeToExit:
	.string "Would you like to exit the SAFARI\n"
	.string "ZONE right now?$"
	.string "{JPN}{JPN}サファリゾーンから　いますぐ　でますか？$"

SafariZone_Text_TimesUp:
	.string "Ding-dong! Time's up!\n"
	.string "Your SAFARI Game is over.$"
	.string "{JPN}{JPN}ピンポーン！　じかんぎれ　です！\n"
	.string "サファリゲームは　しゅうりょう　です$"

SafariZone_Text_OutOfBalls:
	.string "You've run out of SAFARI BALLS.\n"
	.string "Your SAFARI Game is over.$"
	.string "{JPN}{JPN}サファリボールが　なくなりました\n"
	.string "サファリゲームは　しゅうりょう　です$"

SafariZone_Text_PlacePokeblockOnFeeder:
	.string "Would you like to place a {POKEBLOCK}\n"
	.string "on the {POKEBLOCK} FEEDER?$"
	.string "{JPN}{JPN}ポロックおきばに　ポロックを　おきますか？$"

SafariZone_Text_PokeblockStillHere:
	.string "The {STR_VAR_1} you left\n"
	.string "before is still here.$"
	.string "{JPN}{JPN}まえに　おいた　{STR_VAR_1}が　まだ\n"
	.string "のこっている$"

SafariZone_Text_PokeblockWasPlaced:
	.string "The {STR_VAR_1} was placed\n"
	.string "on the {POKEBLOCK} FEEDER.$"
	.string "{JPN}{JPN}{STR_VAR_1}を　ポロックおきばに　おいた$"

Route121_SafariZoneEntrance_Text_WelcomeToSafariZone:
	.string "Welcome to the SAFARI ZONE!\p"
	.string "Here, you may witness many kinds of\n"
	.string "POKéMON rarely seen in HOENN.\p"
	.string "And, you'll see them in their wild,\n"
	.string "untamed state in natural habitats.\p"
	.string "What's more, we've thrown open the\n"
	.string "gates to POKéMON TRAINERS.\p"
	.string "You're encouraged to catch POKéMON\n"
	.string "for keeps!\p"
	.string "Come in and enjoy the SAFARI ZONE!$"
	.string "{JPN}{JPN}サファリゾーンへ　ようこそ！\p"
	.string "ここでは　ホウエンでは　めずらしい　ポケモンを\n"
	.string "たくさん　みる　ことが　できます\p"
	.string "しかも　しぜんの　なかで　くらす　やせいの\n"
	.string "すがたの　ままです！\p"
	.string "さらに　ポケモントレーナーの　みなさんにも\n"
	.string "かいほう　しています\p"
	.string "ぜひ　ポケモンを　つかまえて　おもちかえり\n"
	.string "ください！\p"
	.string "サファリゾーンを　おたのしみ　ください！$"

Route121_SafariZoneEntrance_Text_WelcomeFirstTime:
	.string "Welcome to the SAFARI ZONE!\n"
	.string "Is it your first time here?$"
	.string "{JPN}{JPN}サファリゾーンへ　ようこそ！　ここに　くるのは\n"
	.string "はじめて　ですか？$"

Route121_SafariZoneEntrance_Text_ComeInAndEnjoy:
	.string "Come in and enjoy the SAFARI ZONE!$"
	.string "{JPN}{JPN}どうぞ　サファリゾーンを　おたのしみ　ください！$"

Route121_SafariZoneEntrance_Text_FirstTimeInfo:
	.string "When you enter the SAFARI ZONE, you\n"
	.string "start with 30 SAFARI BALLS for\l"
	.string "catching POKéMON.\p"
	.string "The SAFARI Game is over when you run\n"
	.string "out of SAFARI BALLS, or when you've\l"
	.string "walked 500 steps.\p"
	.string "Come in and enjoy the SAFARI ZONE!$"
	.string "{JPN}{JPN}サファリゾーンに　はいると　ポケモンを\n"
	.string "つかまえる　ための　サファリボールを　30こ\l"
	.string "おわたし　します\p"
	.string "サファリボールが　なくなるか　500ぽ　あるくと\n"
	.string "サファリゲームは　しゅうりょう　です\p"
	.string "どうぞ　サファリゾーンを　おたのしみ　ください！$"

Route121_SafariZoneEntrance_Text_WouldYouLikeToPlay:
	.string "Welcome to the SAFARI ZONE!\p"
	.string "All you can catch for just ¥500!\n"
	.string "Would you like to play a SAFARI Game?$"
	.string "{JPN}{JPN}サファリゾーンへ　ようこそ！\p"
	.string "500円で　つかまえ　ほうだい！\n"
	.string "サファリゲームで　あそびますか？$"

Route121_SafariZoneEntrance_Text_PlayAnotherTime:
	.string "Okay.\n"
	.string "Please play another time!$"
	.string "{JPN}{JPN}そうですか　また　あそびに　きて　くださいね！$"

Route121_SafariZoneEntrance_Text_NotEnoughMoney:
	.string "You don't have enough money.\n"
	.string "Sorry.$"
	.string "{JPN}{JPN}おかねが　たりない　ようです　ごめんなさい$"

Route121_SafariZoneEntrance_Text_ThatWillBe500Please:
	.string "That will be ¥500, please.$"
	.string "{JPN}{JPN}500円　いただきます$"

Route121_SafariZoneEntrance_Text_HereAreYourSafariBalls:
	.string "Here are your SAFARI BALLS.$"
	.string "{JPN}{JPN}こちらが　サファリボール　です$"

Route121_SafariZoneEntrance_Text_Received30SafariBalls:
	.string "{PLAYER} received 30 SAFARI BALLS.$"
	.string "{JPN}{JPN}{PLAYER}は　サファリボールを　30こ\n"
	.string "うけとった$"

Route121_SafariZoneEntrance_Text_PleaseEnjoyYourself:
	.string "We'll let you know when your game\n"
	.string "is over.\p"
	.string "So, until then, enjoy yourself, please!\n"
	.string "Off you go on your wild excursion!$"
	.string "{JPN}{JPN}ゲームが　おわったら　おしらせ　します\p"
	.string "それまで　どうぞ　おたのしみ　ください！　では\n"
	.string "やせいの　ぼうけんへ　いってらっしゃい！$"

Route121_SafariZoneEntrance_Text_PCIsFull:
	.string "Excuse me!\n"
	.string "Your PC BOX is full.$"
	.string "{JPN}{JPN}すみません！　パソコンの　ボックスが　いっぱい\n"
	.string "です$"

Route121_SafariZoneEntrance_Text_YouNeedPokeblockCase:
	.string "Excuse me!\n"
	.string "You seem to be without a {POKEBLOCK} CASE.\p"
	.string "Your SAFARI Game will be much more\n"
	.string "fruitful if you use {POKEBLOCK}S.\p"
	.string "Please come back with a {POKEBLOCK} CASE.\p"
	.string "You may obtain a {POKEBLOCK} CASE from\n"
	.string "the LILYCOVE CONTEST HALL.$"
	.string "{JPN}{JPN}すみません！　ポロックケースを　おもちで　ない\n"
	.string "ようですね\p"
	.string "ポロックを　つかうと　サファリゲームが　ずっと\n"
	.string "たのしく　なりますよ\p"
	.string "ポロックケースを　もって　また　きて　ください\p"
	.string "ポロックケースは　ミナモシティの\n"
	.string "コンテストかいじょうで　もらえます$"

SafariZone_South_Text_StillHaveTimeExit:
	.string "You still have time left. Would you like\n"
	.string "to exit the SAFARI ZONE now?$"
	.string "{JPN}{JPN}まだ　じかんが　ありますが　サファリゾーンから\n"
	.string "でますか？$"

SafariZone_South_Text_EnjoyTheRestOfYourAdventure:
	.string "Please enjoy the rest of your wild\n"
	.string "adventure!$"
	.string "{JPN}{JPN}では　ひきつづき　やせいの　ぼうけんを\n"
	.string "おたのしみ　ください！$"

SafariZone_South_Text_ExitEarlyThankYouForPlaying:
	.string "Okay.\p"
	.string "I'll take back your remaining SAFARI\n"
	.string "BALLS.\p"
	.string "Thank you for playing.\n"
	.string "We hope to see you again.$"
	.string "{JPN}{JPN}わかりました\p"
	.string "のこりの　サファリボールを　おあずかり　します\p"
	.string "ごりよう　ありがとう　ございました　また　おこし\n"
	.string "ください$"

SafariZone_South_Text_GoodLuck:
	.string "Good luck!\p"
	.string "If you need anything, don't hesitate\n"
	.string "to tell me, please!$"
	.string "{JPN}{JPN}がんばって　ください！\p"
	.string "なにか　あったら　えんりょなく　いって\n"
	.string "くださいね！$"

SafariZone_South_Text_Boy:
	.string "Did you know?\p"
	.string "If you put a {POKEBLOCK} in that square box,\n"
	.string "POKéMON gather around.$"
	.string "{JPN}{JPN}しってる？\p"
	.string "あの　しかくい　はこに　ポロックを　いれると\n"
	.string "ポケモンが　あつまって　くるんだよ$"

SafariZone_South_Text_Man:
	.string "I want to keep going deeper, but I\n"
	.string "forgot to bring a BIKE.\p"
	.string "Something tells me that rare POKéMON\n"
	.string "live in the outlying areas.$"
	.string "{JPN}{JPN}もっと　おくへ　いきたいけど　じてんしゃを\n"
	.string "もってくるのを　わすれたんだ\p"
	.string "なんとなく　はずれの　ほうに　めずらしい\n"
	.string "ポケモンが　いる　きが　するんだ$"

SafariZone_Southwest_Text_Woman:
	.string "Sometimes, when I toss a {POKEBLOCK} at \n"
	.string "POKéMON, it gets ignored.\p"
	.string "Do POKéMON have likes and dislikes\n"
	.string "about what they eat?$"
	.string "{JPN}{JPN}ポケモンに　ポロックを　なげても　むしされる\n"
	.string "ことが　あるの\p"
	.string "ポケモンにも　たべものの　すききらいが\n"
	.string "あるのかしら？$"

SafariZone_Northwest_Text_Man:
	.string "Gasp… Gasp…\n"
	.string "I…made it out here…but…\p"
	.string "I'm exhausted… I don't have the\n"
	.string "energy to catch POKéMON…$"
	.string "{JPN}{JPN}ぜえ‥　ぜえ‥　ここまで‥　きたけど‥\p"
	.string "もう　へとへと‥　ポケモンを　つかまえる\n"
	.string "げんきも　ない‥$"

SafariZone_North_Text_Fisherman:
	.string "I'm on a mission to find WATER POKéMON\n"
	.string "you don't see in HOENN.\p"
	.string "Do you have any idea where the lake is?$"
	.string "{JPN}{JPN}ホウエンでは　みられない　みずポケモンを\n"
	.string "さがしに　きたんだ\p"
	.string "みずうみが　どこに　あるか　しらないかい？$"

SafariZone_North_Text_Man:
	.string "I'm going to catch a lot of rare POKéMON\n"
	.string "here and trade them with my friends!$"
	.string "{JPN}{JPN}ここで　めずらしい　ポケモンを　たくさん\n"
	.string "つかまえて　ともだちと　こうかん　するんだ！$"

SafariZone_South_Text_Youngster:
	.string "I put a {POKEBLOCK} on the {POKEBLOCK} FEEDER.\n"
	.string "But it seems to have disappeared.\p"
	.string "I guess POKéMON must have eaten it\n"
	.string "without me noticing.$"
	.string "{JPN}{JPN}ポロックおきばに　ポロックを　おいたんだ　でも\n"
	.string "いつのまにか　なくなってた\p"
	.string "きっと　ぼくが　きづかない　うちに　ポケモンが\n"
	.string "たべちゃったんだね$"

Route121_SafariZoneEntrance_Text_TrainerTip:
	.string "SAFARI ZONE TRAINER TIP!\p"
	.string "Throw {POKEBLOCK}S at wild POKéMON to make\n"
	.string "them less likely to flee.$"
	.string "{JPN}{JPN}サファリゾーンの　トレーナー　ヒント！\p"
	.string "やせいの　ポケモンに　ポロックを　なげると\n"
	.string "にげにくく　なるぞ$"

SafariZone_Southwest_Text_RestHouseSign:
	.string "“Relieve your tired feet.”\n"
	.string "REST HOUSE$"
	.string "{JPN}{JPN}『つかれた　あしを　やすめよう』　きゅうけいじょ$"

SafariZone_RestHouse_Text_Youngster:
	.string "I don't have any {POKEBLOCK}S, but I caught\n"
	.string "a good number of POKéMON.\p"
	.string "You can improve your chances of making\n"
	.string "a catch by getting closer to them\l"
	.string "before throwing a SAFARI BALL.$"
	.string "{JPN}{JPN}ポロックは　もってないけど　ポケモンは　けっこう\n"
	.string "つかまえたよ\p"
	.string "サファリボールを　なげる　まえに　ポケモンに\n"
	.string "ちかづくと　つかまえやすく　なるんだ$"

SafariZone_RestHouse_Text_PsychicM:
	.string "If you use {POKEBLOCK}S, wild POKéMON won't\n"
	.string "be so quick to run away.\p"
	.string "It's not much use to give {POKEBLOCK}S to\n"
	.string "POKéMON that don't flee easily.$"
	.string "{JPN}{JPN}ポロックを　つかうと　やせいの　ポケモンが\n"
	.string "すぐには　にげなく　なる\p"
	.string "でも　もともと　にげにくい　ポケモンに\n"
	.string "ポロックを　あげても　あまり　いみが　ないよ$"

SafariZone_RestHouse_Text_FatMan:
	.string "If you put a {POKEBLOCK} on the FEEDER,\n"
	.string "POKéMON are attracted to it.\p"
	.string "I think POKéMON with the same sort of\n"
	.string "nature are drawn by a certain {POKEBLOCK}.$"
	.string "{JPN}{JPN}おきばに　ポロックを　おくと　ポケモンが　よって\n"
	.string "くるんだ\p"
	.string "おなじ　せいかくの　ポケモンは　おなじ\n"
	.string "ポロックに　ひかれる　んじゃないかな$"

SafariZone_South_Text_AreaOffLimits1:
	.string "This area is still under construction.\n"
	.string "It's off-limits, sorry!$"
	.string "{JPN}{JPN}この　エリアは　まだ　こうじ　ちゅう　です\n"
	.string "たちいり　きんし　なんです　ごめんなさい！$"

SafariZone_Southeast_Text_ExpansionIsFinished:
	.string "The SAFARI ZONE's expansion project\n"
	.string "is finished now.\p"
	.string "We hope you will enjoy the new area.$"
	.string "{JPN}{JPN}サファリゾーンの　かくちょう　こうじが\n"
	.string "おわりました\p"
	.string "あたらしい　エリアを　おたのしみ　ください$"

SafariZone_South_Text_AreaOffLimits2:
	.string "This area is still under construction.\n"
	.string "It's off-limits, sorry!$"
	.string "{JPN}{JPN}この　エリアは　まだ　こうじ　ちゅう　です\n"
	.string "たちいり　きんし　なんです　ごめんなさい！$"

SafariZone_Southeast_Text_LittleGirl:
	.string "Wow! Whee! I haven't seen any of\n"
	.string "these POKéMON before!$"
	.string "{JPN}{JPN}わあ！　すごーい！　はじめて　みる　ポケモン\n"
	.string "ばっかり！$"

SafariZone_Southeast_Text_FatMan:
	.string "The POKéMON in this area are all\n"
	.string "new to me.\p"
	.string "And I'm allowed to catch these rare\n"
	.string "POKéMON! Too cool!$"
	.string "{JPN}{JPN}この　エリアの　ポケモンは　ぜんぶ　はじめて\n"
	.string "みるよ\p"
	.string "しかも　この　めずらしい　ポケモンを　つかまえて\n"
	.string "いいなんて！　さいこう！$"

SafariZone_Southeast_Text_RichBoy:
	.string "The POKéMON around here seem to be\n"
	.string "from somewhere other than HOENN.$"
	.string "{JPN}{JPN}この　あたりの　ポケモンは　ホウエン　いがいの\n"
	.string "ところから　きた　みたいだね$"

SafariZone_Northeast_Text_Boy:
	.string "I only have a couple SAFARI BALLS left.\p"
	.string "I'm having a hard time trying to\n"
	.string "decide what I should catch.$"
	.string "{JPN}{JPN}サファリボールが　あと　ちょっとしか　ないんだ\p"
	.string "なにを　つかまえるか　なかなか　きめられないよ$"

SafariZone_Northeast_Text_Woman:
	.string "I heard that you can see PIKACHU here.\n"
	.string "Where might one be?$"
	.string "{JPN}{JPN}ここで　ピカチュウが　みられるって　きいたの\n"
	.string "どこに　いるのかしら？$"

SafariZone_Northeast_Text_Girl:
	.string "Oh, boo!\n"
	.string "I can't seem to catch anything!\p"
	.string "I'll end up wasting the admission\n"
	.string "price if I don't catch something!$"
	.string "{JPN}{JPN}もう！　ぜんぜん　つかまらない！\p"
	.string "なにか　つかまえないと　にゅうじょうりょうが\n"
	.string "むだに　なっちゃう！$"
