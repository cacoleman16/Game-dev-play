SixIsland_PatternBush_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_PatternBush_OnTransition
	.byte 0

SixIsland_PatternBush_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND_PATTERN_BUSH
	getplayerxy VAR_TEMP_1, VAR_TEMP_2
	call_if_ge VAR_TEMP_1, 50, SixIsland_PatternBush_EventScript_SetEscapeRightExit
	call_if_le VAR_TEMP_1, 49, SixIsland_PatternBush_EventScript_SetEscapeLeftExit
	end

SixIsland_PatternBush_EventScript_SetEscapeRightExit::
	setescapewarp MAP_SIX_ISLAND_GREEN_PATH, 255, 64, 10
	return

SixIsland_PatternBush_EventScript_SetEscapeLeftExit::
	setescapewarp MAP_SIX_ISLAND_GREEN_PATH, 255, 45, 10
	return

SixIsland_PatternBush_EventScript_Bethany::
	trainerbattle_single TRAINER_PKMN_BREEDER_BETHANY, SixIsland_PatternBush_Text_BethanyIntro, SixIsland_PatternBush_Text_BethanyDefeat
	msgbox SixIsland_PatternBush_Text_BethanyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Allison::
	trainerbattle_single TRAINER_PKMN_BREEDER_ALLISON, SixIsland_PatternBush_Text_AllisonIntro, SixIsland_PatternBush_Text_AllisonDefeat
	msgbox SixIsland_PatternBush_Text_AllisonPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Garret::
	trainerbattle_single TRAINER_BUG_CATCHER_GARRET, SixIsland_PatternBush_Text_GarretIntro, SixIsland_PatternBush_Text_GarretDefeat
	msgbox SixIsland_PatternBush_Text_GarretPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Jonah::
	trainerbattle_single TRAINER_BUG_CATCHER_JONAH, SixIsland_PatternBush_Text_JonahIntro, SixIsland_PatternBush_Text_JonahDefeat
	msgbox SixIsland_PatternBush_Text_JonahPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Vance::
	trainerbattle_single TRAINER_BUG_CATCHER_VANCE, SixIsland_PatternBush_Text_VanceIntro, SixIsland_PatternBush_Text_VanceDefeat
	msgbox SixIsland_PatternBush_Text_VancePostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Nash::
	trainerbattle_single TRAINER_YOUNGSTER_NASH, SixIsland_PatternBush_Text_NashIntro, SixIsland_PatternBush_Text_NashDefeat
	msgbox SixIsland_PatternBush_Text_NashPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Cordell::
	trainerbattle_single TRAINER_YOUNGSTER_CORDELL, SixIsland_PatternBush_Text_CordellIntro, SixIsland_PatternBush_Text_CordellDefeat
	msgbox SixIsland_PatternBush_Text_CordellPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Dalia::
	trainerbattle_single TRAINER_LASS_DALIA, SixIsland_PatternBush_Text_DaliaIntro, SixIsland_PatternBush_Text_DaliaDefeat
	msgbox SixIsland_PatternBush_Text_DaliaPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Joana::
	trainerbattle_single TRAINER_LASS_JOANA, SixIsland_PatternBush_Text_JoanaIntro, SixIsland_PatternBush_Text_JoanaDefeat
	msgbox SixIsland_PatternBush_Text_JoanaPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Riley::
	trainerbattle_single TRAINER_CAMPER_RILEY, SixIsland_PatternBush_Text_RileyIntro, SixIsland_PatternBush_Text_RileyDefeat
	msgbox SixIsland_PatternBush_Text_RileyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Marcy::
	trainerbattle_single TRAINER_PICNICKER_MARCY, SixIsland_PatternBush_Text_MarcyIntro, SixIsland_PatternBush_Text_MarcyDefeat
	msgbox SixIsland_PatternBush_Text_MarcyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Layton::
	trainerbattle_single TRAINER_RUIN_MANIAC_LAYTON, SixIsland_PatternBush_Text_LaytonIntro, SixIsland_PatternBush_Text_LaytonDefeat
	msgbox SixIsland_PatternBush_Text_LaytonPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_Text_BethanyIntro::
	.string "I'm curious, how do you raise your\n"
	.string "POKéMON?$"
	.string "{JPN}{JPN}あなたは　どうやって　ポケモンを　そだてて\n"
	.string "いるの？$"

SixIsland_PatternBush_Text_BethanyDefeat::
	.string "You raise your POKéMON with a\n"
	.string "whole lot of love!$"
	.string "{JPN}{JPN}あなたは　あふれる　あいじょうで　ポケモンを\n"
	.string "そだてて　いるのね！$"

SixIsland_PatternBush_Text_BethanyPostBattle::
	.string "In the same way your mother raised\n"
	.string "you full of love, you should raise\l"
	.string "your POKéMON.$"
	.string "{JPN}{JPN}おかあさんが　あなたを　あいじょう　いっぱいに\n"
	.string "そだてた　ように　あなたも　ポケモンを　そだてて\l"
	.string "あげてね。$"

SixIsland_PatternBush_Text_AllisonIntro::
	.string "I'm working to preserve the natural\n"
	.string "ecology of POKéMON here.$"
	.string "{JPN}{JPN}わたしは　ここで　ポケモンの　しぜんの\n"
	.string "せいたいけいを　まもる　かつどうを　して\l"
	.string "いるの。$"

SixIsland_PatternBush_Text_AllisonDefeat::
	.string "Oh, for someone so young, you are\n"
	.string "tremendous!$"
	.string "{JPN}{JPN}まあ　わかいのに　すごいわね！$"

SixIsland_PatternBush_Text_AllisonPostBattle::
	.string "I'm not saying that you shouldn't\n"
	.string "catch POKéMON.\p"
	.string "I just want people to raise their\n"
	.string "POKéMON responsibly.$"
	.string "{JPN}{JPN}ポケモンを　つかまえるなって　いってる　わけじゃ\n"
	.string "ないのよ。\p"
	.string "ただ　せきにんを　もって　ポケモンを　そだてて\n"
	.string "ほしいの。$"

SixIsland_PatternBush_Text_GarretIntro::
	.string "I caught a BUG POKéMON that lives\n"
	.string "only around here!$"
	.string "{JPN}{JPN}この　あたりにしか　いない　むしポケモンを\n"
	.string "つかまえたんだ！$"

SixIsland_PatternBush_Text_GarretDefeat::
	.string "Heheh…\n"
	.string "Isn't my POKéMON awesome?$"
	.string "{JPN}{JPN}へへ‥　ぼくの　ポケモン　かっこいい　だろ？$"

SixIsland_PatternBush_Text_GarretPostBattle::
	.string "There's a girl near the BUSH who\n"
	.string "measures HERACROSS for TRAINERS.$"
	.string "{JPN}{JPN}しげみの　ちかくに　トレーナーの　ヘラクロスの\n"
	.string "おおきさを　はかって　くれる　おんなのこが\l"
	.string "いるよ。$"

SixIsland_PatternBush_Text_JonahIntro::
	.string "You know, it just doesn't feel right\n"
	.string "if I don't use BUG POKéMON.$"
	.string "{JPN}{JPN}やっぱり　むしポケモンを　つかわないと　しっくり\n"
	.string "こないんだよね。$"

SixIsland_PatternBush_Text_JonahDefeat::
	.string "It's okay, losing is a fun part of\n"
	.string "POKéMON's appeal, too.$"
	.string "{JPN}{JPN}いいんだ。　まける　ことも　ポケモンの\n"
	.string "みりょくの　ひとつ　だから。$"

SixIsland_PatternBush_Text_JonahPostBattle::
	.string "By the way, “appeal” is a weird\n"
	.string "word, if you think about it.\p"
	.string "Like, is it like an orange peel?\n"
	.string "Or like a ringing bell?$"
	.string "{JPN}{JPN}ところで　『みりょく』って　よく　かんがえると\n"
	.string "へんな　ことば　だよね。\p"
	.string "みる　ちからって　こと？　それとも　ミリの\n"
	.string "ちから？$"

SixIsland_PatternBush_Text_VanceIntro::
	.string "Yeah, yeah, yeah!\n"
	.string "Look at all the BUG POKéMON!$"
	.string "{JPN}{JPN}やった　やった　やったー！　むしポケモンが\n"
	.string "いっぱい！$"

SixIsland_PatternBush_Text_VanceDefeat::
	.string "I got beat while I was still busy\n"
	.string "celebrating!$"
	.string "{JPN}{JPN}まだ　よろこんでる　さいちゅうに　まけちゃった！$"

SixIsland_PatternBush_Text_VancePostBattle::
	.string "I'm going to bring my little brother\n"
	.string "here next time.$"
	.string "{JPN}{JPN}こんどは　おとうとを　ここに　つれて　こよう。$"

SixIsland_PatternBush_Text_NashIntro::
	.string "Look, look! There are funny\n"
	.string "patterns on the ground.$"
	.string "{JPN}{JPN}みて　みて！　じめんに　へんな　もようが\n"
	.string "あるよ。$"

SixIsland_PatternBush_Text_NashDefeat::
	.string "Super awesome!$"
	.string "{JPN}{JPN}ちょー　すごい！$"

SixIsland_PatternBush_Text_NashPostBattle::
	.string "The funny patterns on the ground…\p"
	.string "They look like the patterns on my\n"
	.string "grandpa's clothes.$"
	.string "{JPN}{JPN}じめんの　へんな　もよう‥\p"
	.string "おじいちゃんの　ふくの　もように　にてるんだ。$"

SixIsland_PatternBush_Text_CordellIntro::
	.string "I bet you think I'm just some guy,\n"
	.string "don't you, eh?$"
	.string "{JPN}{JPN}おめえ　おらの　こと　ただの　おっさんだと\n"
	.string "おもってるべ？$"

SixIsland_PatternBush_Text_CordellDefeat::
	.string "I bet you think I'm weak,\n"
	.string "don't you, eh?$"
	.string "{JPN}{JPN}おめえ　おらの　こと　よわいと　おもってるべ？$"

SixIsland_PatternBush_Text_CordellPostBattle::
	.string "Ayup, you townies come around all\n"
	.string "so dandy with a hat that flashy…\p"
	.string "How about giving me that?$"
	.string "{JPN}{JPN}んだ　まちの　もんは　そんな　はでな　ぼうし\n"
	.string "かぶって　きどって　くるだな‥\p"
	.string "その　ぼうし　おらに　くれねえか？$"

SixIsland_PatternBush_Text_DaliaIntro::
	.string "Take a deep breath.\n"
	.string "Isn't the air delicious?$"
	.string "{JPN}{JPN}しんこきゅう　して　ごらん。　くうきが\n"
	.string "おいしいでしょ？$"

SixIsland_PatternBush_Text_DaliaDefeat::
	.string "If you'd like, I can teach you how\n"
	.string "to breathe properly.$"
	.string "{JPN}{JPN}よかったら　ただしい　こきゅうの　しかたを\n"
	.string "おしえて　あげるわ。$"

SixIsland_PatternBush_Text_DaliaPostBattle::
	.string "First, exhale.\n"
	.string "Blow everything out.\p"
	.string "When you can't exhale anymore,\n"
	.string "inhale the clean air!\p"
	.string "Isn't it refreshing?$"
	.string "{JPN}{JPN}まずは　いきを　はいて。　ぜんぶ　はききって。\p"
	.string "もう　はけなく　なったら　きれいな　くうきを\n"
	.string "すいこむの！\p"
	.string "すっきり　するでしょ？$"

SixIsland_PatternBush_Text_JoanaIntro::
	.string "I love BUG POKéMON.\n"
	.string "That's why I'm here all the time.\p"
	.string "Am I the only girl like that?$"
	.string "{JPN}{JPN}わたし　むしポケモンが　だいすき。　だから\n"
	.string "いつも　ここに　いるの。\p"
	.string "こんな　おんなのこって　わたし　だけ？$"

SixIsland_PatternBush_Text_JoanaDefeat::
	.string "I lost, but I'm still laughing.\n"
	.string "Am I the only girl like that?$"
	.string "{JPN}{JPN}まけたけど　わらっちゃう。　こんな\n"
	.string "おんなのこって　わたし　だけ？$"

SixIsland_PatternBush_Text_JoanaPostBattle::
	.string "I'm going to keep on collecting\n"
	.string "BUG POKéMON.\p"
	.string "Am I the only girl like that?$"
	.string "{JPN}{JPN}これからも　むしポケモンを　あつめ　つづけるわ。\p"
	.string "こんな　おんなのこって　わたし　だけ？$"

SixIsland_PatternBush_Text_RileyIntro::
	.string "This is a good spot.\n"
	.string "I'll pitch my tent here.$"
	.string "{JPN}{JPN}ここは　いい　ばしょだ。　ここに　テントを\n"
	.string "はろう。$"

SixIsland_PatternBush_Text_RileyDefeat::
	.string "Wimped out…$"
	.string "{JPN}{JPN}へこたれた‥$"

SixIsland_PatternBush_Text_RileyPostBattle::
	.string "I'm going to observe the night sky\n"
	.string "here.$"
	.string "{JPN}{JPN}ここで　よぞらを　かんさつ　するんだ。$"

SixIsland_PatternBush_Text_MarcyIntro::
	.string "Oh, yuck!\n"
	.string "I think a bug stung me!$"
	.string "{JPN}{JPN}うえっ！　むしに　さされた　みたい！$"

SixIsland_PatternBush_Text_MarcyDefeat::
	.string "It wasn't a bug.\n"
	.string "I cut my shin on some grass.$"
	.string "{JPN}{JPN}むしじゃ　なかった。　くさで　すねを\n"
	.string "きっちゃった。$"

SixIsland_PatternBush_Text_MarcyPostBattle::
	.string "A little cut like that…\n"
	.string "A little spit's enough to cure it!$"
	.string "{JPN}{JPN}これくらいの　きず‥　つばでも　つけとけば\n"
	.string "なおるわ！$"

SixIsland_PatternBush_Text_LaytonIntro::
	.string "Have you noticed something odd\n"
	.string "about these parts?$"
	.string "{JPN}{JPN}この　あたりで　なにか　へんな　ことに\n"
	.string "きづかないか？$"

SixIsland_PatternBush_Text_LaytonDefeat::
	.string "Have you taken a good look around\n"
	.string "your feet?$"
	.string "{JPN}{JPN}あしもとを　よく　みて　ごらん？$"

SixIsland_PatternBush_Text_LaytonPostBattle::
	.string "There are places here in PATTERN\n"
	.string "BUSH where grass won't grow.\p"
	.string "What could be the cause of such\n"
	.string "a phenomenon?$"
	.string "{JPN}{JPN}この　もようのしげみには　くさが　はえない\n"
	.string "ばしょが　あるんだ。\p"
	.string "いったい　なにが　げんいんで　そんな\n"
	.string "げんしょうが　おきるんだろう？$"

