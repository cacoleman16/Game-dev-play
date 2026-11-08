VermilionCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

VermilionCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VERMILION_CITY
	end

VermilionCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_Man::
	msgbox VermilionCity_PokemonCenter_1F_Text_PoisonedMonFaintedWhileWalking, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Hiker::
	msgbox VermilionCity_PokemonCenter_1F_Text_TrainerMonsStrongerThanWild, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox VermilionCity_PokemonCenter_1F_Text_AllMonWeakToSpecificTypes, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_VSSeekerWoman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_VS_SEEKER, VermilionCity_PokemonCenter_1F_EventScript_ExplainVSSeeker
	msgbox VermilionCity_PokemonCenter_1F_Text_UrgeToBattleSomeoneAgain
	setflag FLAG_GOT_VS_SEEKER
	giveitem ITEM_VS_SEEKER
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	msgbox VermilionCity_PokemonCenter_1F_Text_UseDeviceForRematches
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_ExplainVSSeeker::
	msgbox VermilionCity_PokemonCenter_1F_Text_ExplainVSSeeker
	release
	end

VermilionCity_PokemonCenter_1F_Text_TrainerMonsStrongerThanWild::
	.string "Even if they are the same level,\n"
	.string "POKéMON can have very different\l"
	.string "stats and abilities.\p"
	.string "A POKéMON raised by a TRAINER is\n"
	.string "stronger than one in the wild.$"
	.string "{JPN}{JPN}おなじ　レベルでも　ポケモンの　のうりょくは\n"
	.string "ずいぶん　ちがう　ことが　あるんだ\p"
	.string "トレーナーが　そだてた　ポケモンは　やせいの\n"
	.string "ポケモンより　つよいのさ$"

VermilionCity_PokemonCenter_1F_Text_PoisonedMonFaintedWhileWalking::
	.string "My POKéMON was poisoned!\n"
	.string "It fainted while we were walking!$"
	.string "{JPN}{JPN}ぼくの　ポケモンが　どくを　うけちゃって\n"
	.string "あるいてる　とちゅうで　ひんしに　なっちゃった！$"

VermilionCity_PokemonCenter_1F_Text_AllMonWeakToSpecificTypes::
	.string "It is true that a higher-level\n"
	.string "POKéMON will be more powerful…\p"
	.string "But, all POKéMON will have weak\n"
	.string "points against specific types.\p"
	.string "So, there appears to be no\n"
	.string "universally strong POKéMON.$"
	.string "{JPN}{JPN}たしかに　レベルが　たかい　ポケモンほど\n"
	.string "つよい‥\p"
	.string "でも　どの　ポケモンにも　にがてな　タイプが\n"
	.string "あるんだ\p"
	.string "だから　どんな　あいてにも　つよい　ポケモンは\n"
	.string "いない　みたいだね$"

VermilionCity_PokemonCenter_1F_Text_UrgeToBattleSomeoneAgain::
	.string "The urge to battle with someone\n"
	.string "you've tangled with before…\p"
	.string "Have you ever had that urge?\n"
	.string "I'm sure you have.\p"
	.string "I wanted to battle certain people\n"
	.string "again over and over, too.\p"
	.string "So, I've been giving these away.\n"
	.string "Please, take one!$"
	.string "{JPN}{JPN}まえに　たたかった　あいてと　もう　いちど\n"
	.string "たたかいたい‥\p"
	.string "そう　おもった　こと　ない？　きっと　あるよね\p"
	.string "ぼくも　なんども　たたかいたい　あいてが\n"
	.string "いたんだ\p"
	.string "だから　これを　くばってるんだ　ひとつ　どうぞ！$"

VermilionCity_PokemonCenter_1F_Text_UseDeviceForRematches::
	.string "Use that device and you'll find\n"
	.string "TRAINERS looking for a rematch.\p"
	.string "You have to charge its battery to\n"
	.string "use it, though.$"
	.string "{JPN}{JPN}それを　つかうと　さいせんを　したがってる\n"
	.string "トレーナーが　みつかるよ\p"
	.string "でも　つかうには　じゅうでん　しないと\n"
	.string "だめなんだ$"

VermilionCity_PokemonCenter_1F_Text_ExplainVSSeeker::
	.string "How do you use the VS SEEKER?\n"
	.string "There's nothing to it.\p"
	.string "Use it like beep-beep-beep, and\n"
	.string "TRAINERS around you will notice.\p"
	.string "If any TRAINER wants a rematch,\n"
	.string "it will let you know immediately.\p"
	.string "Charge its battery and use it\n"
	.string "while you're on a road.$"
	.string "{JPN}{JPN}バトルサーチャーの　つかいかた？　かんたんだよ\p"
	.string "ピピピッて　つかうと　まわりの　トレーナーが\n"
	.string "きづくんだ\p"
	.string "さいせんを　したい　トレーナーが　いたら　すぐに\n"
	.string "わかるよ\p"
	.string "じゅうでん　してから　どうろで　つかってね$"
