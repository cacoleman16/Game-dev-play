Seika_Silkhollow_Gym_MapScripts::
	.byte 0

Seika_Silkhollow_Gym_EventScript_Midori::
	trainerbattle_single TRAINER_SEIKA_AROMA_LADY_MIDORI, Seika_Silkhollow_Gym_Text_MidoriIntro, Seika_Silkhollow_Gym_Text_MidoriDefeat
	msgbox Seika_Silkhollow_Gym_Text_MidoriPost, MSGBOX_AUTOCLOSE
	end

Seika_Silkhollow_Gym_Text_MidoriIntro::
	.string "Breathe in... the scent of\n"
	.string "flowers in silk. Now battle!$"
	.string "{JPN}{JPN}すーっと　すって‥　きぬに　しみた　はなの\n"
	.string "かおり。　さあ　しょうぶよ！$"

Seika_Silkhollow_Gym_Text_MidoriDefeat::
	.string "Oh! My petals scattered!$"
	.string "{JPN}{JPN}ああ！　はなびらが　ちっちゃった！$"

Seika_Silkhollow_Gym_Text_MidoriPost::
	.string "AYAME dyes her silk with flowers\n"
	.string "from the town gardens.$"
	.string "{JPN}{JPN}アヤメさまは　まちの　はなばたけの　はなで\n"
	.string "きぬを　そめて　いるの。$"

Seika_Silkhollow_Gym_EventScript_Chiyo::
	trainerbattle_single TRAINER_SEIKA_PARASOL_LADY_CHIYO, Seika_Silkhollow_Gym_Text_ChiyoIntro, Seika_Silkhollow_Gym_Text_ChiyoDefeat
	msgbox Seika_Silkhollow_Gym_Text_ChiyoPost, MSGBOX_AUTOCLOSE
	end

Seika_Silkhollow_Gym_Text_ChiyoIntro::
	.string "One, two, three, turn!\n"
	.string "Keep in step with me!$"
	.string "{JPN}{JPN}ワン　ツー　スリー　ターン！　わたしに　あわせて\n"
	.string "ステップよ！$"

Seika_Silkhollow_Gym_Text_ChiyoDefeat::
	.string "You stepped on my toes!$"
	.string "{JPN}{JPN}あしを　ふまれちゃった！$"

Seika_Silkhollow_Gym_Text_ChiyoPost::
	.string "Every dancer here learned from\n"
	.string "AYAME. She never misses a step.$"
	.string "{JPN}{JPN}おどりこは　みんな　アヤメさまの　でしなの。\n"
	.string "あの　かたは　ステップを　まちがえないわ。$"

Seika_Silkhollow_Gym_EventScript_Yuka::
	trainerbattle_single TRAINER_SEIKA_BEAUTY_YUKA, Seika_Silkhollow_Gym_Text_YukaIntro, Seika_Silkhollow_Gym_Text_YukaDefeat
	msgbox Seika_Silkhollow_Gym_Text_YukaPost, MSGBOX_AUTOCLOSE
	end

Seika_Silkhollow_Gym_Text_YukaIntro::
	.string "Silk looks soft, doesn't it?\n"
	.string "FAIRY POKéMON look soft, too!$"
	.string "{JPN}{JPN}きぬって　やわらかそうでしょ？\n"
	.string "フェアリーポケモンも　そう　みえるわよね！$"

Seika_Silkhollow_Gym_Text_YukaDefeat::
	.string "You saw right through me!$"
	.string "{JPN}{JPN}すっかり　みすかされちゃった！$"

Seika_Silkhollow_Gym_Text_YukaPost::
	.string "The thread that looks weakest\n"
	.string "is often the strongest.$"
	.string "{JPN}{JPN}いちばん　よわそうな　いとが　いちばん　つよい\n"
	.string "ことも　あるのよ。$"

Seika_Silkhollow_Gym_EventScript_Ayame::
	trainerbattle_single TRAINER_SEIKA_LEADER_AYAME, Seika_Silkhollow_Gym_Text_AyameIntro, Seika_Silkhollow_Gym_Text_AyameDefeat, Seika_Silkhollow_Gym_EventScript_AyameDefeated, NO_MUSIC
	msgbox Seika_Silkhollow_Gym_Text_AyamePost, MSGBOX_DEFAULT
	release
	end

Seika_Silkhollow_Gym_EventScript_AyameDefeated::
	setflag FLAG_QOL_SEIKA_BADGE06
	msgbox Seika_Silkhollow_Gym_Text_AyameGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_SAFEGUARD
	msgbox Seika_Silkhollow_Gym_Text_AyamePost, MSGBOX_DEFAULT
	release
	end

Seika_Silkhollow_Gym_Text_AyameIntro::
	.string "Welcome to my weaving hall.\p"
	.string "I am AYAME. Every battle is a\n"
	.string "thread, and I weave them all\l"
	.string "into silk.\p"
	.string "Let us see what pattern you and\n"
	.string "your POKéMON will make!$"
	.string "{JPN}{JPN}アヤメ『はたおりの　やかたへ　ようこそ。\p"
	.string "わたしは　アヤメ。　バトルは　ひとつ　ひとつが\n"
	.string "いと。　わたしは　それを　ぜんぶ　きぬに\l"
	.string "おりあげるの。\p"
	.string "あなたと　ポケモンが　どんな　もようを\n"
	.string "おりなすのか　みせて　いただくわ！』$"

Seika_Silkhollow_Gym_Text_AyameDefeat::
	.string "Oh... what a beautiful pattern.\n"
	.string "I could never have woven it.\p"
	.string "Please take the SILK BADGE. It is\n"
	.string "light as thread, and as strong.\p"
	.string "{FONT_NORMAL}{PLAYER} received the SILK BADGE\n"
	.string "from AYAME!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"
	.string "{JPN}{JPN}まあ‥　なんて　うつくしい　もよう。　わたしには\n"
	.string "とても　おれないわ。\p"
	.string "キヌバッジを　どうぞ。　いとのように　かるくて\n"
	.string "いとのように　つよいの。\p"
	.string "{FONT_NORMAL}{PLAYER}は　アヤメから　キヌバッジを\n"
	.string "もらった！{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_Silkhollow_Gym_Text_AyameGift::
	.string "And this, too. A veil of silk to\n"
	.string "keep your POKéMON safe.$"
	.string "{JPN}{JPN}アヤメ『それと　これも。　あなたの　ポケモンを\n"
	.string "まもる　きぬの　ベールよ。』$"

Seika_Silkhollow_Gym_Text_AyamePost::
	.string "Go west along WEAVER'S ROAD to\n"
	.string "FROSTMERE VILLAGE.\p"
	.string "RIME keeps the shrine by the\n"
	.string "cold lake there. He is patient,\l"
	.string "like the ice itself.$"
	.string "{JPN}{JPN}アヤメ『ハタオリかいどうを　にしへ　いけば\n"
	.string "シモミズむらよ。\p"
	.string "そこの　つめたい　みずうみの　ほとりで\n"
	.string "ツララさんが　やしろを　まもって　いるわ。\l"
	.string "こおりのように　しんぼうづよい　ひとよ。』$"

Seika_Silkhollow_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE06, Seika_Silkhollow_Gym_EventScript_GymGuyWon
	msgbox Seika_Silkhollow_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_Silkhollow_Gym_EventScript_GymGuyWon::
	msgbox Seika_Silkhollow_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_Silkhollow_Gym_Text_GymGuy::
	.string "Yo! AYAME weaves her FAIRY\n"
	.string "POKéMON into one strong cloth.\p"
	.string "POISON and STEEL moves cut right\n"
	.string "through it!\p"
	.string "Her SYLVEON is the last thread.\n"
	.string "Don't let it CALM MIND too long!$"
	.string "{JPN}{JPN}おーす！　アヤメは　フェアリーポケモンを\n"
	.string "1まいの　つよい　ぬのに　おりあげるんだ。\p"
	.string "どくと　はがねの　わざなら　スパッと\n"
	.string "きりさけるぞ！\p"
	.string "さいごの　いとは　ニンフィア。　めいそうを\n"
	.string "つまれすぎるなよ！$"

Seika_Silkhollow_Gym_Text_GymGuyWon::
	.string "What a show! That's six BADGES.\n"
	.string "Two more to go!$"
	.string "{JPN}{JPN}みごとな　ショー　だったぜ！　これで　バッジ\n"
	.string "6つ。　あと　2つだ！$"
