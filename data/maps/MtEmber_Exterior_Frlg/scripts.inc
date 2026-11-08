MtEmber_Exterior_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtEmber_Exterior_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, MtEmber_Exterior_OnLoad
	.byte 0

MtEmber_Exterior_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_MT_EMBER_EXTERIOR
	call_if_eq VAR_MAP_SCENE_MT_EMBER_EXTERIOR, 2, MtEmber_Exterior_EventScript_RocketsFaceDown
	end

MtEmber_Exterior_EventScript_RocketsFaceDown::
	setobjectmovementtype LOCALID_MT_EMBER_GRUNT1, MOVEMENT_TYPE_FACE_DOWN
	setobjectmovementtype LOCALID_MT_EMBER_GRUNT2, MOVEMENT_TYPE_FACE_DOWN
	return

MtEmber_Exterior_OnLoad::
	call_if_ge VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, MtEmber_Exterior_EventScript_OpenCave
	end

MtEmber_Exterior_EventScript_OpenCave::
	setmetatile 42, 39, METATILE_MtEmber_CaveEntrance, 0
	return

MtEmber_Exterior_EventScript_Grunt1::
	lock
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_43, MtEmber_Exterior_EventScript_Grunt1Defeated
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, MtEmber_Exterior_EventScript_BattleGrunt1
	msgbox MtEmber_Exterior_Text_WellTryDiggingHere
	release
	end

MtEmber_Exterior_EventScript_Grunt1Defeated::
	msgbox MtEmber_Exterior_Text_Grunt1PostBattle
	release
	end

MtEmber_Exterior_EventScript_BattleGrunt1::
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_FacePlayer
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	msgbox MtEmber_Exterior_Text_Grunt1Intro
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_43, MtEmber_Exterior_Text_Grunt1Defeat
	msgbox MtEmber_Exterior_Text_Grunt1PostBattle
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_44, MtEmber_Exterior_EventScript_DefeatedBothGrunts
	release
	end

MtEmber_Exterior_EventScript_DefeatedBothGrunts::
	msgbox MtEmber_Exterior_Text_WellRegroupDontStepInsideThere
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_MT_EMBER_GRUNT1
	removeobject LOCALID_MT_EMBER_GRUNT2
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtEmber_Exterior_EventScript_Grunt2::
	lock
	faceplayer
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_44, MtEmber_Exterior_EventScript_DefeatedGrunt2
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, MtEmber_Exterior_EventScript_BattleGrunt2
	msgbox MtEmber_Exterior_Text_YoureInTheWayGetLost
	closemessage
	applymovement LOCALID_MT_EMBER_GRUNT2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MtEmber_Exterior_EventScript_DefeatedGrunt2::
	msgbox MtEmber_Exterior_Text_Grunt2PostBattle
	release
	end

MtEmber_Exterior_EventScript_BattleGrunt2::
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	msgbox MtEmber_Exterior_Text_Grunt2Intro
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_44, MtEmber_Exterior_Text_Grunt2Defeat
	msgbox MtEmber_Exterior_Text_Grunt2PostBattle
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_43, MtEmber_Exterior_EventScript_DefeatedBothGrunts
	release
	end

MtEmber_Exterior_EventScript_RocketPasswordScene::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox MtEmber_Exterior_Text_PunchedThroughAtLast
	message MtEmber_Exterior_Text_WhatsPasswordAgain
	waitmessage
	playse SE_PIN
	applymovement LOCALID_MT_EMBER_GRUNT2, Common_Movement_QuestionMark
	waitmovement 0
	waitbuttonpress
	msgbox MtEmber_Exterior_Text_FirstPasswordGoldeen
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_MT_EMBER_GRUNT2, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	call MtEmber_Exterior_EventScript_RocketsFaceDown
	msgbox MtEmber_Exterior_Text_SnoopsBeenListeningIn
	setvar VAR_MAP_SCENE_MT_EMBER_EXTERIOR, 2
	releaseall
	end

MtEmber_Exterior_EventScript_Logan::
	trainerbattle_single TRAINER_PKMN_RANGER_LOGAN, MtEmber_Exterior_Text_LoganIntro, MtEmber_Exterior_Text_LoganDefeat
	msgbox MtEmber_Exterior_Text_LoganPostBattle, MSGBOX_AUTOCLOSE
	end

MtEmber_Exterior_EventScript_Beth::
	trainerbattle_single TRAINER_PKMN_RANGER_BETH, MtEmber_Exterior_Text_BethIntro, MtEmber_Exterior_Text_BethDefeat
	msgbox MtEmber_Exterior_Text_BethPostBattle, MSGBOX_AUTOCLOSE
	end

MtEmber_Exterior_EventScript_Jocelyn::
	trainerbattle_single TRAINER_CRUSH_GIRL_JOCELYN, MtEmber_Exterior_Text_JocelynIntro, MtEmber_Exterior_Text_JocelynDefeat
	msgbox MtEmber_Exterior_Text_JocelynPostBattle, MSGBOX_AUTOCLOSE
	end

Text_ExplosionTeach::
	.string "Can you imagine?\n"
	.string "If this volcano were to erupt?\p"
	.string "The explosion would be the end of\n"
	.string "us. How terrifying is that?\p"
	.string "While we're terrified, would you\n"
	.string "like me to teach EXPLOSION?$"
	.string "{JPN}{JPN}かんがえて　みて！　もし　この　かざんが　ふんか\n"
	.string "したら？\p"
	.string "その　だいばくはつで　おしまいよ。　なんて\n"
	.string "おそろしいの！\p"
	.string "こわい　ついでに　だいばくはつを　おしえて\n"
	.string "あげようか？$"

Text_ExplosionDeclined::
	.string "Yeah, you're right.\n"
	.string "It is too terrifying.$"
	.string "{JPN}{JPN}そうよね。　やっぱり　こわすぎるわよね。$"

Text_ExplosionWhichMon::
	.string "You're terribly brave!\p"
	.string "Which POKéMON should I teach\n"
	.string "EXPLOSION?$"
	.string "{JPN}{JPN}なんて　だいたんなの！\p"
	.string "どの　ポケモンに　だいばくはつを　おしえる？$"

Text_ExplosionTaught::
	.string "Using EXPLOSION while on this\n"
	.string "volcano…\p"
	.string "What a terrifying thrill!$"
	.string "{JPN}{JPN}この　かざんで　だいばくはつを　つかうなんて‥\p"
	.string "なんて　おそろしい　スリル　なの！$"

MtEmber_Exterior_Text_WellTryDiggingHere::
	.string "Over here.\n"
	.string "We'll try digging here.\p"
	.string "That treasure the ADMINS've been\n"
	.string "talking about should be here.$"
	.string "{JPN}{JPN}こっちだ。　ここを　ほって　みるぞ。\p"
	.string "かんぶたちが　はなしてた　おたからは　ここに\n"
	.string "あるはずだ。$"

MtEmber_Exterior_Text_YoureInTheWayGetLost::
	.string "What the…?\n"
	.string "You're in the way. Get lost.$"
	.string "{JPN}{JPN}なんだ‥？　じゃまだ。　あっちへ　いけ。$"

MtEmber_Exterior_Text_PunchedThroughAtLast::
	.string "Whew, punched through at last.\n"
	.string "Let's go treasure hunting!$"
	.string "{JPN}{JPN}ふう　やっと　ほりぬいたぞ。　おたからを\n"
	.string "さがしに　いこうぜ！$"

MtEmber_Exterior_Text_WhatsPasswordAgain::
	.string "Any treasure we find, we haul back\n"
	.string "to the WAREHOUSE, understood?\p"
	.string "…Oh, and what're the passwords?\n"
	.string "At the ROCKET WAREHOUSE, I mean.$"
	.string "{JPN}{JPN}みつけた　おたからは　ぜんぶ　そうこに\n"
	.string "はこぶんだぞ　わかったな？\p"
	.string "‥ところで　あいことばは　なんだっけ？\n"
	.string "ロケットだんの　そうこの　だよ。$"

MtEmber_Exterior_Text_FirstPasswordGoldeen::
	.string "What, you forgot the password?\n"
	.string "There're actually two.\p"
	.string "The first one's “GOLDEEN need\n"
	.string "log.”\p"
	.string "And the second one is…$"
	.string "{JPN}{JPN}なんだ　あいことばを　わすれたのか？　じつは\n"
	.string "2つ　あるんだ。\p"
	.string "ひとつめは　『トサキントに　まるたが\n"
	.string "ひつよう』。\p"
	.string "で　ふたつめは‥$"

MtEmber_Exterior_Text_SnoopsBeenListeningIn::
	.string "Hey!\n"
	.string "This snoop's been listening in!$"
	.string "{JPN}{JPN}おい！　こいつ　ぬすみぎき　してたぞ！$"

MtEmber_Exterior_Text_Grunt1Intro::
	.string "You've been eavesdropping on us,\n"
	.string "haven't you?$"
	.string "{JPN}{JPN}おまえ　おれたちの　はなしを　ぬすみぎき\n"
	.string "してたな？$"

MtEmber_Exterior_Text_Grunt1Defeat::
	.string "Huh, what?$"
	.string "{JPN}{JPN}えっ　なんだと？$"

MtEmber_Exterior_Text_Grunt1PostBattle::
	.string "Why'd you have to win?$"
	.string "{JPN}{JPN}なんで　かっちまうんだよ？$"

MtEmber_Exterior_Text_Grunt2Intro::
	.string "Trying to horn in on our treasure?\n"
	.string "Don't bet on it!$"
	.string "{JPN}{JPN}おれたちの　おたからを　よこどり　する　きか？\n"
	.string "そうは　いくか！$"

MtEmber_Exterior_Text_Grunt2Defeat::
	.string "Wait!\n"
	.string "But how?$"
	.string "{JPN}{JPN}まて！　なんでだ？$"

MtEmber_Exterior_Text_Grunt2PostBattle::
	.string "Develop amnesia conveniently and\n"
	.string "forget everything you heard!$"
	.string "{JPN}{JPN}つごうよく　きおくそうしつに　なって　きいた\n"
	.string "ことを　ぜんぶ　わすれろ！$"

MtEmber_Exterior_Text_WellRegroupDontStepInsideThere::
	.string "What a setback…\n"
	.string "We'll have to regroup.\p"
	.string "You! Don't even think about taking\n"
	.string "a step inside here!\p"
	.string "If you know what's good for you,\n"
	.string "don't even think about this!$"
	.string "{JPN}{JPN}なんて　こった‥　たてなおさないと。\p"
	.string "おまえ！　ここに　いっぽでも　はいろうなんて\n"
	.string "おもうなよ！\p"
	.string "みの　ためを　おもうなら　かんがえる　ことすら\n"
	.string "するな！$"

MtEmber_Exterior_Text_LoganIntro::
	.string "An active volcano is one that's\n"
	.string "erupting, or about to.$"
	.string "{JPN}{JPN}かつかざんとは　ふんか　している　または　ふんか\n"
	.string "しそうな　かざんの　ことだ。$"

MtEmber_Exterior_Text_LoganDefeat::
	.string "Whew… That was hot!\n"
	.string "Sweat's erupting from me!$"
	.string "{JPN}{JPN}ふう‥　あつかった！　あせが　ふきだしてるよ！$"

MtEmber_Exterior_Text_LoganPostBattle::
	.string "They say there're over 1,500\n"
	.string "active volcanoes in the world.\p"
	.string "Just thinking about them makes me\n"
	.string "feel all hot and gooey inside!$"
	.string "{JPN}{JPN}せかいには　1500いじょうの　かつかざんが\n"
	.string "あるらしい。\p"
	.string "かんがえる　だけで　からだの　なかが　あつく\n"
	.string "とろけそうだ！$"

MtEmber_Exterior_Text_BethIntro::
	.string "Do you know how caves form inside\n"
	.string "volcanoes?$"
	.string "{JPN}{JPN}かざんの　なかに　どうくつが　できる　しくみ\n"
	.string "しってる？$"

MtEmber_Exterior_Text_BethDefeat::
	.string "Atchah!\n"
	.string "You're shockingly good!$"
	.string "{JPN}{JPN}あちゃー！　びっくりするほど　つよいわね！$"

MtEmber_Exterior_Text_BethPostBattle::
	.string "When lava at the surface of magma\n"
	.string "hardens, and the magma inside flows\l"
	.string "out, caverns are left behind.$"
	.string "{JPN}{JPN}マグマの　ひょうめんの　ようがんが　かたまって\n"
	.string "なかの　マグマが　ながれでると　どうくつが\l"
	.string "のこるのよ。$"

MtEmber_Exterior_Text_JocelynIntro::
	.string "I've been in training so I can mend\n"
	.string "my broken heart…\p"
	.string "It toughened me up physically at\n"
	.string "least! Hi-yah!$"
	.string "{JPN}{JPN}しつれんの　きずを　いやす　ために　しゅぎょう\n"
	.string "してたの‥\p"
	.string "おかげで　からだは　きたえられたわ！　ハイヤー！$"

MtEmber_Exterior_Text_JocelynDefeat::
	.string "I think I tried to overdo it…$"
	.string "{JPN}{JPN}ちょっと　がんばりすぎた　かも‥$"

MtEmber_Exterior_Text_JocelynPostBattle::
	.string "The way you battle, it reminds me\n"
	.string "of the guy who dumped me…$"
	.string "{JPN}{JPN}あなたの　たたかいかた　わたしを　ふった\n"
	.string "あいつを　おもいだすわ‥$"

