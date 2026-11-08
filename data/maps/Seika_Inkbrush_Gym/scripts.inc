Seika_Inkbrush_Gym_MapScripts::
	.byte 0

Seika_Inkbrush_Gym_EventScript_Statue::
	lockall
	goto_if_set FLAG_QOL_SEIKA_BADGE02, Seika_Inkbrush_Gym_EventScript_StatueWon
	msgbox Seika_Inkbrush_Gym_Text_Statue, MSGBOX_DEFAULT
	releaseall
	end

Seika_Inkbrush_Gym_EventScript_StatueWon::
	msgbox Seika_Inkbrush_Gym_Text_StatueWon, MSGBOX_DEFAULT
	releaseall
	end

Seika_Inkbrush_Gym_Text_Statue::
	.string "INKBRUSH POKéMON GYM\n"
	.string "LEADER: SUMI$"
	.string "{JPN}{JPN}スミフデ　ポケモンジム　リーダー　スミ$"

Seika_Inkbrush_Gym_Text_StatueWon::
	.string "INKBRUSH POKéMON GYM\n"
	.string "LEADER: SUMI\p"
	.string "WINNING TRAINERS:\n"
	.string "{PLAYER}$"
	.string "{JPN}{JPN}スミフデ　ポケモンジム　リーダー　スミ\p"
	.string "しょうりした　トレーナー:　{PLAYER}$"

Seika_Inkbrush_Gym_EventScript_Haku::
	trainerbattle_single TRAINER_SEIKA_PSYCHIC_HAKU, Seika_Inkbrush_Gym_Text_HakuIntro, Seika_Inkbrush_Gym_Text_HakuDefeat
	msgbox Seika_Inkbrush_Gym_Text_HakuPost, MSGBOX_AUTOCLOSE
	end

Seika_Inkbrush_Gym_Text_HakuIntro::
	.string "I read your next move in the\n"
	.string "shape of your shadow!$"
	.string "{JPN}{JPN}おまえの　かげの　かたちで　つぎの　てを\n"
	.string "よんだ！$"

Seika_Inkbrush_Gym_Text_HakuDefeat::
	.string "My reading was... smudged.$"
	.string "{JPN}{JPN}よみが‥　にじんで　しまった。$"

Seika_Inkbrush_Gym_Text_HakuPost::
	.string "SUMI never says a word in battle.\n"
	.string "She just writes. Then she wins.$"
	.string "{JPN}{JPN}スミさまは　バトルちゅう　なにも　いわない。\n"
	.string "ただ　ふでを　はしらせ‥　そして　かつのだ。$"

Seika_Inkbrush_Gym_EventScript_Shizu::
	trainerbattle_single TRAINER_SEIKA_PAINTER_SHIZU, Seika_Inkbrush_Gym_Text_ShizuIntro, Seika_Inkbrush_Gym_Text_ShizuDefeat
	msgbox Seika_Inkbrush_Gym_Text_ShizuPost, MSGBOX_AUTOCLOSE
	end

Seika_Inkbrush_Gym_Text_ShizuIntro::
	.string "My SMEARGLE paints with its tail.\n"
	.string "Let's paint you a defeat!$"
	.string "{JPN}{JPN}わたしの　ドーブルは　しっぽで　えを　かくの。\n"
	.string "あなたの　まけを　えがいて　あげる！$"

Seika_Inkbrush_Gym_Text_ShizuDefeat::
	.string "Oh! You ruined my picture!$"
	.string "{JPN}{JPN}ああっ！　わたしの　えが　だいなしよ！$"

Seika_Inkbrush_Gym_Text_ShizuPost::
	.string "SUMI's ESPEON is her last brush\n"
	.string "stroke. Keep something for it!$"
	.string "{JPN}{JPN}スミさまの　エーフィは　さいごの　ひとふで。\n"
	.string "その　ぶんの　ちからを　のこして　おきなさい！$"

Seika_Inkbrush_Gym_EventScript_Sumi::
	trainerbattle_single TRAINER_SEIKA_LEADER_SUMI, Seika_Inkbrush_Gym_Text_SumiIntro, Seika_Inkbrush_Gym_Text_SumiDefeat, Seika_Inkbrush_Gym_EventScript_SumiDefeated, NO_MUSIC
	msgbox Seika_Inkbrush_Gym_Text_SumiPost, MSGBOX_DEFAULT
	release
	end

Seika_Inkbrush_Gym_EventScript_SumiDefeated::
	setflag FLAG_QOL_SEIKA_BADGE02
	msgbox Seika_Inkbrush_Gym_Text_SumiGift, MSGBOX_DEFAULT
	giveitem ITEM_TM_CALM_MIND
	msgbox Seika_Inkbrush_Gym_Text_SumiPost, MSGBOX_DEFAULT
	release
	end

Seika_Inkbrush_Gym_Text_SumiIntro::
	.string "Welcome. I am SUMI. I keep the\n"
	.string "shrine of INKBRUSH, and its ink.\p"
	.string "A brush cannot lie. One stroke\n"
	.string "shows the whole heart of the\l"
	.string "one who holds it.\p"
	.string "Let us see what kind of stroke\n"
	.string "you are. Begin!$"
	.string "{JPN}{JPN}スミ『ようこそ。　わたくしは　スミ。　スミフデの\n"
	.string "やしろと　その　すみを　まもる　ものです。\p"
	.string "ふでは　うそを　つけません。　ひとふでに　もつ\n"
	.string "ひとの　こころが　すべて　あらわれるのです。\p"
	.string "あなたは　どんな　ふでを　はこぶのか　みせて\n"
	.string "もらいましょう。　いざ！』$"

Seika_Inkbrush_Gym_Text_SumiDefeat::
	.string "A firm stroke, and an honest\n"
	.string "one. You have earned this.\p"
	.string "Take the INK BADGE.\p"
	.string "{FONT_NORMAL}{PLAYER} received the INK BADGE\n"
	.string "from SUMI!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"
	.string "{JPN}{JPN}ちからづよく　そして　まっすぐな　ふで。　これを\n"
	.string "うけとる　しかくが　あります。\p"
	.string "スミバッジを　どうぞ。\p"
	.string "{FONT_NORMAL}{PLAYER}は　スミから　スミバッジを\n"
	.string "もらった！{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

Seika_Inkbrush_Gym_Text_SumiGift::
	.string "And this. A calm mind makes a\n"
	.string "steady hand.$"
	.string "{JPN}{JPN}スミ『それと　これも。　おちついた　こころが\n"
	.string "ぶれない　てを　つくるのです。』$"

Seika_Inkbrush_Gym_Text_SumiPost::
	.string "You came about the STARFALL\n"
	.string "SCROLL, didn't you?\p"
	.string "Our keeper will tell you what it\n"
	.string "says. His hall is just north of\l"
	.string "the main street.\p"
	.string "Then go west, through MOSSGROVE\n"
	.string "WOODS, to MOSSGROVE VILLAGE.\l"
	.string "KAYA keeps the shrine there.$"
	.string "{JPN}{JPN}スミ『あなたは　スターフォールのまきものの\n"
	.string "ことで　きたのでしょう？\p"
	.string "まきものかんの　あるじが　なにが　かいて　あるか\n"
	.string "おしえて　くれます。　おおどおりの　すぐ\l"
	.string "きたですよ。\p"
	.string "それから　にしへ　コケムスのもりを　ぬけて\n"
	.string "コケムスむらへ。　そこの　やしろは　カヤが\l"
	.string "まもって　います。』$"

Seika_Inkbrush_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_QOL_SEIKA_BADGE02, Seika_Inkbrush_Gym_EventScript_GymGuyWon
	msgbox Seika_Inkbrush_Gym_Text_GymGuy, MSGBOX_DEFAULT
	release
	end

Seika_Inkbrush_Gym_EventScript_GymGuyWon::
	msgbox Seika_Inkbrush_Gym_Text_GymGuyWon, MSGBOX_DEFAULT
	release
	end

Seika_Inkbrush_Gym_Text_GymGuy::
	.string "Hey! SUMI's POKéMON are all\n"
	.string "PSYCHIC types.\p"
	.string "BUG, GHOST and DARK moves will\n"
	.string "smudge her ink. Watch out for her\l"
	.string "CALM MIND, too!$"
	.string "{JPN}{JPN}よう！　スミの　ポケモンは　みんな\n"
	.string "エスパータイプだ。\p"
	.string "むし　ゴースト　あくの　わざで　すみを\n"
	.string "にじませて　やれ！　めいそうにも　きをつけろよ！$"

Seika_Inkbrush_Gym_Text_GymGuyWon::
	.string "What a finish! That INK BADGE\n"
	.string "won't ever wash off!$"
	.string "{JPN}{JPN}みごとな　フィニッシュ！　その　スミバッジは\n"
	.string "もう　ずっと　きえないぜ！$"
