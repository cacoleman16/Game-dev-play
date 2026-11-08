CeladonCity_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeladonCity_OnTransition
	.byte 0

CeladonCity_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CELADON_CITY
	end

CeladonCity_EventScript_LittleGirl::
	msgbox CeladonCity_Text_GotMyKoffingInCinnabar, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan1::
	lock
	msgbox CeladonCity_Text_GymIsGreatFullOfWomen
	release
	end

CeladonCity_EventScript_Woman::
	msgbox CeladonCity_Text_GameCornerIsBadForCitysImage, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan2::
	msgbox CeladonCity_Text_BlewItAllAtSlots, MSGBOX_NPC
	end

CeladonCity_EventScript_SoftboiledTutor::
	goto EventScript_SoftboiledTutor
	end

CeladonCity_EventScript_FatMan::
	lock
	faceplayer
	msgbox CeladonCity_Text_MyTrustedPalPoliwrath
	closemessage
	applymovement LOCALID_CELADON_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_Poliwrath::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_POLIWRATH, CRY_MODE_NORMAL
	msgbox CeladonCity_Text_Poliwrath
	closemessage
	waitmoncry
	applymovement LOCALID_CELADON_POLIWRATH, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_RocketGrunt1::
	msgbox CeladonCity_Text_KeepOutOfTeamRocketsWay, MSGBOX_NPC
	end

CeladonCity_EventScript_RocketGrunt2::
	msgbox CeladonCity_Text_GetLostOrIllPunchYou, MSGBOX_NPC
	end

CeladonCity_EventScript_Boy::
	msgbox CeladonCity_Text_ScaldedTongueOnTea, MSGBOX_NPC
	end

CeladonCity_EventScript_SilphCoScientist::
	msgbox CeladonCity_Text_SomeoneStoleSilphScope, MSGBOX_NPC
	end

CeladonCity_EventScript_TrainerTips1::
	msgbox CeladonCity_Text_ExplainXAccuracyDireHit, MSGBOX_SIGN
	end

CeladonCity_EventScript_CitySign::
	msgbox CeladonCity_Text_CitySign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_ERIKA, 0
	msgbox CeladonCity_Text_GymSign
	releaseall
	end

CeladonCity_EventScript_MansionSign::
	msgbox CeladonCity_Text_MansionSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_DeptStoreSign::
	msgbox CeladonCity_Text_DeptStoreSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_TrainerTips2::
	msgbox CeladonCity_Text_GuardSpecProtectsFromStatus, MSGBOX_SIGN
	end

CeladonCity_EventScript_PrizeExchangeSign::
	msgbox CeladonCity_Text_PrizeExchangeSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GameCornerSign::
	msgbox CeladonCity_Text_GameCornerSign, MSGBOX_SIGN
	end

CeladonCity_Text_GotMyKoffingInCinnabar::
	.string "I got my KOFFING in CINNABAR.\p"
	.string "It's usually nice, but it breathes\n"
	.string "poison when it's angry.$"
	.string "{JPN}{JPN}ぼくの　ドガースは　グレンじまで　つかまえたんだ\p"
	.string "ふだんは　おとなしいけど　おこると　どくガスを\n"
	.string "はくんだ$"

CeladonCity_Text_GymIsGreatFullOfWomen::
	.string "Heheh! This GYM is great!\n"
	.string "It's full of women!$"
	.string "{JPN}{JPN}うへへ！　この　ジム　さいこう！　おんなのひと\n"
	.string "ばっかりだ！$"

CeladonCity_Text_GameCornerIsBadForCitysImage::
	.string "CELADON takes pride in its efforts\n"
	.string "to keep the city beautiful.\p"
	.string "That's why the new GAME CORNER is\n"
	.string "bad for our city's image.$"
	.string "{JPN}{JPN}タマムシは　まちを　きれいに　する　ことに\n"
	.string "ちからを　いれているの\p"
	.string "だから　あたらしい　ゲームコーナーは　まちの\n"
	.string "イメージに　よくないのよ$"

CeladonCity_Text_BlewItAllAtSlots::
	.string "Moan…\n"
	.string "I blew it all at the slots!\p"
	.string "I knew I should have cashed in my\n"
	.string "COINS for prizes!$"
	.string "{JPN}{JPN}うう‥　スロットで　ぜんぶ　すっちゃった！\p"
	.string "コインを　けいひんに　かえて　おけば　よかった！$"

CeladonCity_Text_MyTrustedPalPoliwrath::
	.string "This is my trusted pal, POLIWRATH.\p"
	.string "It evolved from a POLIWHIRL when\n"
	.string "I used a WATER STONE.$"
	.string "{JPN}{JPN}これが　ぼくの　あいぼう　ニョロボン\p"
	.string "ニョロゾに　みずのいしを　つかったら　しんか\n"
	.string "したんだ$"

CeladonCity_Text_Poliwrath::
	.string "POLIWRATH: Ribi ribit!$"
	.string "{JPN}{JPN}ニョロボン『ニョロッ　ニョロー！』$"

CeladonCity_Text_GetLostOrIllPunchYou::
	.string "What are you staring at?\n"
	.string "Get lost, or I'll punch you.$"
	.string "{JPN}{JPN}なに　じろじろ　みてんだ？　あっち　いけ！\n"
	.string "なぐるぞ$"

CeladonCity_Text_KeepOutOfTeamRocketsWay::
	.string "Keep out of TEAM ROCKET's way!$"
	.string "{JPN}{JPN}ロケットだんの　じゃまを　するんじゃねえ！$"

CeladonCity_Text_ExplainXAccuracyDireHit::
	.string "TRAINER TIPS\p"
	.string "X ACCURACY boosts the accuracy of\n"
	.string "techniques.\p"
	.string "DIRE HIT jacks up the likelihood\n"
	.string "of critical hits.\p"
	.string "Get your items at the CELADON\n"
	.string "DEPT. STORE!$"
	.string "{JPN}{JPN}トレーナー　ワンポイント\p"
	.string "ヨクアタールは　わざの　めいちゅうりつを\n"
	.string "あげる！\p"
	.string "クリティカットは　わざが　きゅうしょに\n"
	.string "あたりやすく　なる！\p"
	.string "どうぐは　タマムシデパートで　そろえよう！$"

CeladonCity_Text_CitySign::
	.string "CELADON CITY\n"
	.string "The City of Rainbow Dreams$"
	.string "{JPN}{JPN}タマムシシティ\n"
	.string "にじいろの　ゆめの　まち$"

CeladonCity_Text_GymSign::
	.string "CELADON CITY POKéMON GYM\n"
	.string "LEADER: ERIKA\l"
	.string "The Nature-Loving Princess!$"
	.string "{JPN}{JPN}タマムシシティ　ポケモンジム\n"
	.string "リーダー　エリカ\l"
	.string "しぜんを　あいする　おじょうさま！$"

CeladonCity_Text_MansionSign::
	.string "CELADON MANSION$"
	.string "{JPN}{JPN}タマムシマンション$"

CeladonCity_Text_DeptStoreSign::
	.string "Find what you need at the\n"
	.string "CELADON DEPT. STORE!$"
	.string "{JPN}{JPN}ほしい　ものは　なんでも　そろう！\n"
	.string "タマムシデパート$"

CeladonCity_Text_GuardSpecProtectsFromStatus::
	.string "TRAINER TIPS\p"
	.string "GUARD SPEC. protects POKéMON\n"
	.string "from status-reduction moves during\l"
	.string "battle.\p"
	.string "Get your items at the\n"
	.string "CELADON DEPT. STORE!$"
	.string "{JPN}{JPN}トレーナー　ワンポイント\p"
	.string "エフェクトガードは　せんとうちゅう　のうりょくを\n"
	.string "さげる　わざから　ポケモンを　まもる！\p"
	.string "どうぐは　タマムシデパートで　そろえよう！$"

CeladonCity_Text_PrizeExchangeSign::
	.string "COINS exchanged for prizes!\n"
	.string "PRIZE EXCHANGE$"
	.string "{JPN}{JPN}コインを　けいひんと　こうかん！\n"
	.string "けいひん　こうかんじょ$"

CeladonCity_Text_GameCornerSign::
	.string "ROCKET GAME CORNER\n"
	.string "The playground for grown-ups!$"
	.string "{JPN}{JPN}ロケット　ゲームコーナー\n"
	.string "おとなの　あそびば！$"

CeladonCity_Text_ScaldedTongueOnTea::
	.string "Aaaagh, ow…\n"
	.string "I scalded my tongue!\p"
	.string "This nice old lady in the MANSION\n"
	.string "gave me some TEA.\p"
	.string "But it was boiling hot!\n"
	.string "Gotta cool it to drink it.$"
	.string "{JPN}{JPN}あちちっ‥　したを　やけど　しちゃった！\p"
	.string "マンションの　やさしい　おばあさんが　おちゃを\n"
	.string "くれたの\p"
	.string "でも　すごく　あつかったの！　さまさないと\n"
	.string "のめないわ$"

Text_SoftboiledTeach::
	.string "Hello, there!\p"
	.string "I've seen you about, but I never\n"
	.string "had a chance to chat.\p"
	.string "It must be good luck that brought\n"
	.string "us together finally.\p"
	.string "I'd like to celebrate by teaching\n"
	.string "you the move SOFTBOILED.$"
	.string "{JPN}{JPN}やあ　こんにちは！\p"
	.string "ときどき　みかけて　いたけど　はなす　きかいが\n"
	.string "なくてね\p"
	.string "やっと　こうして　であえたのも　なにかの\n"
	.string "えんだね\p"
	.string "おいわいに　タマゴうみを　おしえて　あげよう$"

Text_SoftboiledDeclined::
	.string "I'll always be keeping an eye out\n"
	.string "for you.\p"
	.string "I hope we can chat again.$"
	.string "{JPN}{JPN}いつでも　きみを　みまもって　いるよ\p"
	.string "また　はなせると　いいね$"

Text_SoftboiledWhichMon::
	.string "So, who's the POKéMON that gets\n"
	.string "the chance to learn SOFTBOILED?$"
	.string "{JPN}{JPN}それで　タマゴうみを　おぼえる　ポケモンは\n"
	.string "どれかな？$"

Text_SoftboiledTaught::
	.string "I wish you the best of luck!$"
	.string "{JPN}{JPN}きみの　こううんを　いのってるよ！$"

CeladonCity_Text_SomeoneStoleSilphScope::
	.string "Oh, what am I to do…\p"
	.string "Someone stole our SILPH SCOPE.\p"
	.string "The thief came running this way,\n"
	.string "I'm sure of it.\p"
	.string "But I lost sight of him!\n"
	.string "Where'd he go?$"
	.string "{JPN}{JPN}ああ　どうしよう‥\p"
	.string "だれかに　シルフスコープを　ぬすまれて　しまった\p"
	.string "どろぼうは　こっちに　にげて　きたんだ\n"
	.string "まちがいない\p"
	.string "でも　みうしなって　しまった！　どこへ\n"
	.string "いったんだ？$"

