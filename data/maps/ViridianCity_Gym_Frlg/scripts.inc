ViridianCity_Gym_Frlg_MapScripts::
	.byte 0

ViridianCity_Gym_EventScript_Giovanni::
	famechecker FAMECHECKER_GIOVANNI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_GIOVANNI, ViridianCity_Gym_Text_GiovanniIntro, ViridianCity_Gym_Text_GiovanniDefeat, ViridianCity_Gym_EventScript_DefeatedGiovanni, NO_MUSIC
	goto_if_unset FLAG_GOT_TM26_FROM_GIOVANNI, ViridianCity_Gym_EventScript_GiveTM26
	msgbox ViridianCity_Gym_Text_GiovanniPostBattle
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_GIOVANNI
	fadescreen FADE_FROM_BLACK
	release
	end

ViridianCity_Gym_EventScript_DefeatedGiovanni::
	famechecker FAMECHECKER_GIOVANNI, 3
	setflag FLAG_HIDE_MISC_KANTO_ROCKETS
	setflag FLAG_DEFEATED_LEADER_GIOVANNI
	setflag FLAG_BADGE08_GET
	setvar VAR_MAP_SCENE_ROUTE22, 3
	set_gym_trainers_frlg 8
	goto ViridianCity_Gym_EventScript_GiveTM26
	end

ViridianCity_Gym_EventScript_GiveTM26::
	msgbox ViridianCity_Gym_Text_ExplainEarthBadgeTakeThis
	checkitemspace ITEM_TM26
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_Gym_EventScript_NoRoomForTM26
	giveitem_msg ViridianCity_Gym_Text_ReceivedTM26FromGiovanni, ITEM_TM26
	setflag FLAG_GOT_TM26_FROM_GIOVANNI
	msgbox ViridianCity_Gym_Text_ExplainTM26
	release
	end

ViridianCity_Gym_EventScript_NoRoomForTM26::
	msgbox ViridianCity_Gym_Text_YouDoNotHaveSpace
	release
	end

ViridianCity_Gym_EventScript_Jason::
	trainerbattle_single TRAINER_TAMER_JASON, ViridianCity_Gym_Text_JasonIntro, ViridianCity_Gym_Text_JasonDefeat
	msgbox ViridianCity_Gym_Text_JasonPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Cole::
	trainerbattle_single TRAINER_TAMER_COLE, ViridianCity_Gym_Text_ColeIntro, ViridianCity_Gym_Text_ColeDefeat
	msgbox ViridianCity_Gym_Text_ColePostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Atsushi::
	trainerbattle_single TRAINER_BLACK_BELT_ATSUSHI, ViridianCity_Gym_Text_AtsushiIntro, ViridianCity_Gym_Text_AtsushiDefeat
	msgbox ViridianCity_Gym_Text_AtsushiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Kiyo::
	trainerbattle_single TRAINER_BLACK_BELT_KIYO, ViridianCity_Gym_Text_KiyoIntro, ViridianCity_Gym_Text_KiyoDefeat
	msgbox ViridianCity_Gym_Text_KiyoPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Takashi::
	trainerbattle_single TRAINER_BLACK_BELT_TAKASHI, ViridianCity_Gym_Text_TakashiIntro, ViridianCity_Gym_Text_TakashiDefeat
	msgbox ViridianCity_Gym_Text_TakashiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Samuel::
	trainerbattle_single TRAINER_COOLTRAINER_SAMUEL, ViridianCity_Gym_Text_SamuelIntro, ViridianCity_Gym_Text_SamuelDefeat
	msgbox ViridianCity_Gym_Text_SamuelPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Yuji::
	trainerbattle_single TRAINER_COOLTRAINER_YUJI, ViridianCity_Gym_Text_YujiIntro, ViridianCity_Gym_Text_YujiDefeat
	msgbox ViridianCity_Gym_Text_YujiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Warren::
	trainerbattle_single TRAINER_COOLTRAINER_WARREN, ViridianCity_Gym_Text_WarrenIntro, ViridianCity_Gym_Text_WarrenDefeat
	msgbox ViridianCity_Gym_Text_WarrenPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LEADER_GIOVANNI, ViridianCity_Gym_EventScript_GymGuyPostVictory
	msgbox ViridianCity_Gym_Text_GymGuyAdvice
	release
	end

ViridianCity_Gym_EventScript_GymGuyPostVictory::
	famechecker FAMECHECKER_GIOVANNI, 4
	msgbox ViridianCity_Gym_Text_GymGuyPostVictory
	release
	end

ViridianCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, ViridianCity_Gym_EventScript_GymStatuePostVictory
	msgbox ViridianCity_Gym_Text_GymStatue
	releaseall
	end

ViridianCity_Gym_EventScript_GymStatuePostVictory::
	msgbox ViridianCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

ViridianCity_Gym_Text_GiovanniIntro::
	.string "Fwahahaha!\n"
	.string "Welcome to my hideout!\p"
	.string "It shall be so until I can restore\n"
	.string "TEAM ROCKET to its former glory.\p"
	.string "But, you have found me again.\n"
	.string "So be it.\l"
	.string "This time, I'm not holding back!\p"
	.string "Once more, you shall face\n"
	.string "GIOVANNI, the greatest TRAINER!{PLAY_BGM}{MUS_RG_ENCOUNTER_ROCKET}$"
	.string "{JPN}{JPN}ふはははは！　わたしの　かくれがへ　ようこそ！\p"
	.string "ロケットだんを　ふっかつ　させる　その　ときまで\n"
	.string "ここに　いる　つもりだった\p"
	.string "だが　また　みつかって　しまったか　まあ　いい\n"
	.string "こんどは　ようしゃ　しないぞ！\p"
	.string "ふたたび　さいきょうの　トレーナー　サカキが\n"
	.string "あいてを　しよう！{PLAY_BGM}{MUS_RG_ENCOUNTER_ROCKET}$"

ViridianCity_Gym_Text_GiovanniDefeat::
	.string "Ha!\n"
	.string "That was a truly intense fight.\l"
	.string "You have won!\p"
	.string "As proof, here is the EARTHBADGE!\n"
	.string "{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"
	.string "{JPN}{JPN}ふっ！　じつに　はげしい　たたかい　だった\n"
	.string "きみの　かちだ！\p"
	.string "その　あかしに　グリーンバッジを\n"
	.string "わたそう！{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

ViridianCity_Gym_Text_GiovanniPostBattle::
	.string "Having lost in this fashion, \n"
	.string "I can't face my followers.\l"
	.string "I have betrayed their trust.\p"
	.string "As of today, TEAM ROCKET is\n"
	.string "finished forever!\p"
	.string "As for myself, I shall dedicate\n"
	.string "my life to training again.\p"
	.string "Let us meet again someday!\n"
	.string "Farewell!$"
	.string "{JPN}{JPN}こんな　まけかたを　しては　ぶかに　あわせる\n"
	.string "かおが　ない　かれらの　しんらいを　うらぎって\l"
	.string "しまった\p"
	.string "きょうを　かぎりに　ロケットだんは　かいさん\n"
	.string "する！\p"
	.string "わたしは　もう　いちど　しゅぎょうに　うちこむ\n"
	.string "つもりだ\p"
	.string "いつか　また　あおう！　さらばだ！$"

ViridianCity_Gym_Text_ExplainEarthBadgeTakeThis::
	.string "The EARTHBADGE makes POKéMON of\n"
	.string "any level obey without question.\p"
	.string "It is evidence of your mastery as\n"
	.string "a POKéMON TRAINER.\p"
	.string "With it, you can challenge the\n"
	.string "POKéMON LEAGUE.\p"
	.string "Also, take this TM.\p"
	.string "Consider it a gift for your POKéMON\n"
	.string "LEAGUE challenge.$"
	.string "{JPN}{JPN}グリーンバッジが　あれば　どんな　レベルの\n"
	.string "ポケモンも　きみの　いう　ことを　きく\p"
	.string "ポケモントレーナーとして　いちにんまえの\n"
	.string "あかしだ\p"
	.string "それが　あれば　ポケモンリーグに　ちょうせん\n"
	.string "できる\p"
	.string "それと　この　わざマシンも　もって　いけ\p"
	.string "ポケモンリーグに　いどむ　きみへの　はなむけだ$"

ViridianCity_Gym_Text_ReceivedTM26FromGiovanni::
	.string "{PLAYER} received TM26\n"
	.string "from GIOVANNI.$"
	.string "{JPN}{JPN}{PLAYER}は　サカキから　わざマシン26を\n"
	.string "もらった$"

ViridianCity_Gym_Text_ExplainTM26::
	.string "TM26 contains EARTHQUAKE.\p"
	.string "It is a powerful attack that causes\n"
	.string "a massive tremor.\p"
	.string "I made it when I ran the GYM here,\n"
	.string "far too long ago… $"
	.string "{JPN}{JPN}わざマシン26は　じしんだ\p"
	.string "おおきな　ゆれを　おこす　きょうりょくな\n"
	.string "こうげきだ\p"
	.string "わたしが　ここの　ジムを　まかされて　いた\n"
	.string "ころに　つくった　ものだ‥　ずいぶん　むかしの\l"
	.string "ことだがな‥$"

ViridianCity_Gym_Text_YouDoNotHaveSpace::
	.string "You do not have space for this!$"
	.string "{JPN}{JPN}もちものが　いっぱいで　うけとれない　ようだな！$"

ViridianCity_Gym_Text_YujiIntro::
	.string "Heh!\n"
	.string "You must be running out of steam\l"
	.string "by now!$"
	.string "{JPN}{JPN}へっ！　そろそろ　ばてて　きた　ころだろう！$"

ViridianCity_Gym_Text_YujiDefeat::
	.string "I ran out of gas!$"
	.string "{JPN}{JPN}ガス　ぎれだ！$"

ViridianCity_Gym_Text_YujiPostBattle::
	.string "You'll need power to keep up with\n"
	.string "our GYM LEADER.$"
	.string "{JPN}{JPN}うちの　リーダーに　ついて　いくには　パワーが\n"
	.string "ひつようだぞ$"

ViridianCity_Gym_Text_AtsushiIntro::
	.string "Rrrroar!\n"
	.string "I'm working myself into a rage!$"
	.string "{JPN}{JPN}うおおおー！　いかりが　こみあげて　きたぞ！$"

ViridianCity_Gym_Text_AtsushiDefeat::
	.string "Wargh!$"
	.string "{JPN}{JPN}ぐわー！$"

ViridianCity_Gym_Text_AtsushiPostBattle::
	.string "I'm still not worthy!$"
	.string "{JPN}{JPN}おれは　まだまだ　みじゅくだ！$"

ViridianCity_Gym_Text_JasonIntro::
	.string "POKéMON and I, we make wonderful\n"
	.string "music together!$"
	.string "{JPN}{JPN}ポケモンと　ぼくは　すばらしい　おんがくを\n"
	.string "かなでるのさ！$"

ViridianCity_Gym_Text_JasonDefeat::
	.string "You are in perfect harmony!$"
	.string "{JPN}{JPN}かんぺきな　ハーモニーだ！$"

ViridianCity_Gym_Text_JasonPostBattle::
	.string "Do you know the identity of our\n"
	.string "GYM LEADER?$"
	.string "{JPN}{JPN}うちの　ジムリーダーの　しょうたいを\n"
	.string "しってるかい？$"

ViridianCity_Gym_Text_KiyoIntro::
	.string "Karate is the ultimate form of\n"
	.string "martial arts!$"
	.string "{JPN}{JPN}からては　さいきょうの　ぶじゅつ　なのだ！$"

ViridianCity_Gym_Text_KiyoDefeat::
	.string "Aiyah!$"
	.string "{JPN}{JPN}あいやー！$"

ViridianCity_Gym_Text_KiyoPostBattle::
	.string "If my POKéMON were as good at\n"
	.string "karate as I…$"
	.string "{JPN}{JPN}ポケモンが　おれ　くらい　からてが　つよければ‥$"

ViridianCity_Gym_Text_WarrenIntro::
	.string "The truly talented win with style.$"
	.string "{JPN}{JPN}ほんとうに　つよい　ものは　かっこよく　かつのさ$"

ViridianCity_Gym_Text_WarrenDefeat::
	.string "I lost my grip!$"
	.string "{JPN}{JPN}ちからが　ぬけた！$"

ViridianCity_Gym_Text_WarrenPostBattle::
	.string "The LEADER will scold me for\n"
	.string "losing this way…$"
	.string "{JPN}{JPN}こんな　まけかたを　したら　リーダーに\n"
	.string "しかられる‥$"

ViridianCity_Gym_Text_TakashiIntro::
	.string "I'm the KARATE KING!\n"
	.string "Your fate rests with me!$"
	.string "{JPN}{JPN}おれは　からておう！　おまえの　うんめいは　おれ\n"
	.string "しだいだ！$"

ViridianCity_Gym_Text_TakashiDefeat::
	.string "Ayah!$"
	.string "{JPN}{JPN}あいやー！$"

ViridianCity_Gym_Text_TakashiPostBattle::
	.string "The POKéMON LEAGUE?\n"
	.string "You? Don't get cocky!$"
	.string "{JPN}{JPN}ポケモンリーグ？　おまえが？　ちょうしに\n"
	.string "のるなよ！$"

ViridianCity_Gym_Text_ColeIntro::
	.string "Your POKéMON will cower at the\n"
	.string "crack of my whip!$"
	.string "{JPN}{JPN}おれの　ムチの　ひとふりで　おまえの　ポケモンは\n"
	.string "ふるえあがるぞ！$"

ViridianCity_Gym_Text_ColeDefeat::
	.string "Yowch!\n"
	.string "Whiplash!$"
	.string "{JPN}{JPN}いてっ！　ムチが　はねかえった！$"

ViridianCity_Gym_Text_ColePostBattle::
	.string "Wait!\n"
	.string "I was just careless!$"
	.string "{JPN}{JPN}まて！　いまのは　ゆだん　しただけだ！$"

ViridianCity_Gym_Text_SamuelIntro::
	.string "VIRIDIAN GYM was closed for a\n"
	.string "long time.\p"
	.string "But now, our LEADER is back!$"
	.string "{JPN}{JPN}トキワジムは　ながい　あいだ　しまって　いた\p"
	.string "だが　いま　リーダーが　もどって　きたのだ！$"

ViridianCity_Gym_Text_SamuelDefeat::
	.string "I was beaten?$"
	.string "{JPN}{JPN}まけた　のか？$"

ViridianCity_Gym_Text_SamuelPostBattle::
	.string "You can go on to the POKéMON\n"
	.string "LEAGUE only by defeating our GYM\l"
	.string "LEADER!$"
	.string "{JPN}{JPN}うちの　ジムリーダーを　たおさなければ\n"
	.string "ポケモンリーグへは　すすめないぞ！$"

ViridianCity_Gym_Text_GymGuyAdvice::
	.string "Yo!\n"
	.string "Champ in the making!\p"
	.string "Even I don't know the VIRIDIAN\n"
	.string "LEADER's identity.\p"
	.string "But one thing's certain.\n"
	.string "This will be the toughest of all\l"
	.string "the GYM LEADERS.\p"
	.string "Also, I heard that the TRAINERS\n"
	.string "here like GROUND-type POKéMON.$"
	.string "{JPN}{JPN}オーッス！　みらいの　チャンピオン！\p"
	.string "トキワの　リーダーの　しょうたいは　おれも\n"
	.string "しらないんだ\p"
	.string "でも　ひとつ　たしかな　ことが　ある　ここの\n"
	.string "リーダーは　どの　ジムリーダーより　てごわいぞ\p"
	.string "それと　ここの　トレーナーは　じめんタイプの\n"
	.string "ポケモンが　すきらしいぞ$"

ViridianCity_Gym_Text_GymGuyPostVictory::
	.string "Blow me away! GIOVANNI was the\n"
	.string "GYM LEADER of VIRIDIAN?$"
	.string "{JPN}{JPN}ぶったまげた！　サカキが　トキワの　ジムリーダー\n"
	.string "だったなんて！$"

ViridianCity_Gym_Text_GymStatue::
	.string "VIRIDIAN POKéMON GYM\n"
	.string "LEADER: ?\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"
	.string "{JPN}{JPN}トキワ　ポケモンジム\n"
	.string "リーダー　？\p"
	.string "しょうりした　トレーナー\n"
	.string "{RIVAL}$"

ViridianCity_Gym_Text_GymStatuePlayerWon::
	.string "VIRIDIAN POKéMON GYM\n"
	.string "LEADER: GIOVANNI\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"
	.string "{JPN}{JPN}トキワ　ポケモンジム\n"
	.string "リーダー　サカキ\p"
	.string "しょうりした　トレーナー\n"
	.string "{RIVAL}　{PLAYER}$"

