Route16_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route16_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, Route16_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route16_OnWarp
	.byte 0

Route16_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, Route16_EventScript_RemoveSnorlax
	end

Route16_EventScript_RemoveSnorlax::
	removeobject VAR_LAST_TALKED
	return

Route16_OnTransition::
	call_if_eq VAR_MAP_SCENE_ROUTE16, 1, Route16_OnTransitionCyclingRoad
	end

Route16_OnTransitionCyclingRoad::
	setflag FLAG_SYS_ON_CYCLING_ROAD
	return

Route16_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route16_OnWarpCyclingRoad
	.2byte 0

Route16_OnWarpCyclingRoad::
	special ForcePlayerOntoBike
	end

Route16_EventScript_Snorlax::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_POKE_FLUTE, Route16_EventScript_SnorlaxNoPokeFlute
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route16_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
	setwildbattle SPECIES_SNORLAX, 30
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_16_SNORLAX
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route16_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route16_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route16_EventScript_FoughtSnorlax
	release
	end

Route16_EventScript_DontUsePokeFlute::
	release
	end

Route16_EventScript_FoughtSnorlax::
	msgbox Text_SnorlaxReturnedToMountains
	release
	end

Route16_EventScript_SnorlaxNoPokeFlute::
	msgbox Route16_Text_MonSprawledOutInSlumber
	release
	end

Route16_EventScript_CyclingRoadSign::
	msgbox Route16_Text_CyclingRoadSign, MSGBOX_SIGN
	end

Route16_EventScript_RouteSign::
	msgbox Route16_Text_RouteSign, MSGBOX_SIGN
	end

Route16_Text_LaoIntro::
	.string "What do you want?$"
	.string "{JPN}{JPN}なんの　ようだ？$"

Route16_Text_LaoDefeat::
	.string "Don't you dare laugh!$"
	.string "{JPN}{JPN}わらうんじゃ　ねえぞ！$"

Route16_Text_LaoPostBattle::
	.string "We like just hanging here.\n"
	.string "What's it to you?$"
	.string "{JPN}{JPN}おれたちは　ここで　たむろ　するのが\n"
	.string "すきなんだ。　もんく　あるか？$"

Route16_Text_KojiIntro::
	.string "Nice BIKE!\n"
	.string "Hand it over!$"
	.string "{JPN}{JPN}いい　じてんしゃ　だな！　よこしな！$"

Route16_Text_KojiDefeat::
	.string "Knockout!$"
	.string "{JPN}{JPN}ノックアウト！$"

Route16_Text_KojiPostBattle::
	.string "Forget it, who needs your BIKE!$"
	.string "{JPN}{JPN}もう　いいや！　おまえの　じてんしゃなんか\n"
	.string "いらねえよ！$"

Route16_Text_LukeIntro::
	.string "Come out and play, little mouse!$"
	.string "{JPN}{JPN}でてきて　あそぼうぜ　ネズミちゃん！$"

Route16_Text_LukeDefeat::
	.string "You little rat!$"
	.string "{JPN}{JPN}この　ちびネズミめ！$"

Route16_Text_LukePostBattle::
	.string "I hate losing!\n"
	.string "Get out of my face!$"
	.string "{JPN}{JPN}まけるのは　だいきらいだ！　とっとと　うせろ！$"

Route16_Text_HideoIntro::
	.string "Hey, you just bumped me!$"
	.string "{JPN}{JPN}おい　いま　ぶつかった　だろ！$"

Route16_Text_HideoDefeat::
	.string "Kaboom!$"
	.string "{JPN}{JPN}どかーん！$"

Route16_Text_HideoPostBattle::
	.string "We'll always be hanging around here\n"
	.string "even if you don't like it.\p"
	.string "You can detour to VERMILION from\n"
	.string "FUCHSIA going along the coast.$"
	.string "{JPN}{JPN}いやがられても　おれたちは　ずっと　ここに\n"
	.string "いるぜ。\p"
	.string "セキチクから　クチバへは　かいがんぞいを\n"
	.string "まわって　いけるぞ。$"

Route16_Text_CamronIntro::
	.string "I'm feeling hungry and mean!\n"
	.string "I need a punching bag!$"
	.string "{JPN}{JPN}はらぺこで　いらいら　してるんだ！\n"
	.string "サンドバッグが　ほしいぜ！$"

Route16_Text_CamronDefeat::
	.string "Bad, bad, bad!$"
	.string "{JPN}{JPN}くそ　くそ　くそっ！$"

Route16_Text_CamronPostBattle::
	.string "If I'm going to have POKéMON, they\n"
	.string "may as well be ferocious.\p"
	.string "I'd use them to leave my enemies\n"
	.string "in tatters.$"
	.string "{JPN}{JPN}どうせ　ポケモンを　もつなら　きょうぼうな\n"
	.string "やつが　いい。\p"
	.string "てきを　ずたずたに　してやるのさ。$"

Route16_Text_RubenIntro::
	.string "Hey, there!\n"
	.string "Let's have ourselves a good time!$"
	.string "{JPN}{JPN}よう！　いっしょに　たのしもうぜ！$"

Route16_Text_RubenDefeat::
	.string "Don't make me mad!$"
	.string "{JPN}{JPN}おこらせるなよ！$"

Route16_Text_RubenPostBattle::
	.string "I get my kicks by harassing people\n"
	.string "with my stinking POKéMON.\p"
	.string "They're great for startling people.\n"
	.string "And, they bite, too.$"
	.string "{JPN}{JPN}くさい　ポケモンで　ひとに　いやがらせ　するのが\n"
	.string "たのしいんだ。\p"
	.string "みんな　びっくり　するぜ。　それに\n"
	.string "かみつくしな。$"

Route16_Text_MonSprawledOutInSlumber::
	.string "A POKéMON is sprawled out in\n"
	.string "a deep and comfortable slumber.$"
	.string "{JPN}{JPN}ポケモンが　きもちよさそうに　ぐっすりと\n"
	.string "ねむっている。$"

Route16_Text_CyclingRoadSign::
	.string "Enjoy the slope!\n"
	.string "CYCLING ROAD$"
	.string "{JPN}{JPN}さかみちを　たのしもう！\n"
	.string "サイクリングロード$"

Route16_Text_RouteSign::
	.string "ROUTE 16\n"
	.string "CELADON CITY - FUCHSIA CITY$"
	.string "{JPN}{JPN}16ばんどうろ\n"
	.string "タマムシシティ　ー　セキチクシティ$"

Route16_Text_JedIntro::
	.string "JED: Our love knows no bounds.\n"
	.string "We're in love and we show it!$"
	.string "{JPN}{JPN}ジェド『ぼくらの　あいは　むげんだい！\n"
	.string "あいしあう　ふたりを　みせて　あげるよ！$"

Route16_Text_JedDefeat::
	.string "JED: Oh, no!\n"
	.string "My love has seen me as a loser!$"
	.string "{JPN}{JPN}ジェド『ああっ！　かのじょに　まける　すがたを\n"
	.string "みられた！$"

Route16_Text_JedPostBattle::
	.string "JED: Listen, LEA.\n"
	.string "You need to focus less on me.$"
	.string "{JPN}{JPN}ジェド『なあ　レア。　ぼくばかり　みてないで\n"
	.string "しょうぶに　しゅうちゅう　しようよ。$"

Route16_Text_JedNotEnoughMons::
	.string "JED: You have just one POKéMON?\n"
	.string "Is there no love in your heart?$"
	.string "{JPN}{JPN}ジェド『ポケモンが　1ぴき　だけ？　きみの\n"
	.string "こころに　あいは　ないのかい？$"

Route16_Text_LeaIntro::
	.string "LEA: Sometimes, the intensity of\n"
	.string "our love scares me.$"
	.string "{JPN}{JPN}レア『ときどき　わたしたちの　あいの　はげしさが\n"
	.string "こわく　なるの。$"

Route16_Text_LeaDefeat::
	.string "LEA: Ohh! But JED looks cool\n"
	.string "even in a loss!$"
	.string "{JPN}{JPN}レア『ああ！　でも　まけても　ジェドは\n"
	.string "かっこいいわ！$"

Route16_Text_LeaPostBattle::
	.string "LEA: Ehehe, I'm sorry.\n"
	.string "JED is so cool.$"
	.string "{JPN}{JPN}レア『えへへ　ごめんね。　ジェドって　ほんとに\n"
	.string "かっこいいの。$"

Route16_Text_LeaNotEnoughMons::
	.string "LEA: Oh, you don't have two\n"
	.string "POKéMON with you?\p"
	.string "Doesn't it feel lonely for you or\n"
	.string "your POKéMON?$"
	.string "{JPN}{JPN}レア『あら　ポケモンを　2ひき　もってないの？\p"
	.string "あなたも　ポケモンも　さびしく　ないの？$"
