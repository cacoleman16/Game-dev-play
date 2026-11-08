Seika_MossgroveWoods_MapScripts::
	.byte 0

Seika_MossgroveWoods_EventScript_EastSign::
	msgbox Seika_MossgroveWoods_Text_EastSign, MSGBOX_SIGN
	end

Seika_MossgroveWoods_Text_EastSign::
	.string "MOSSGROVE WOODS\n"
	.string "EAST: INKBRUSH TOWN$"
	.string "{JPN}{JPN}コケムスのもり　ひがしへ　スミフデタウン$"

Seika_MossgroveWoods_EventScript_WestSign::
	msgbox Seika_MossgroveWoods_Text_WestSign, MSGBOX_SIGN
	end

Seika_MossgroveWoods_Text_WestSign::
	.string "MOSSGROVE WOODS\n"
	.string "WEST: MOSSGROVE VILLAGE$"
	.string "{JPN}{JPN}コケムスのもり　にしへ　コケムスむら$"

Seika_MossgroveWoods_EventScript_Altair::
	trainerbattle_single TRAINER_SEIKA_NOVA_ALTAIR_WOODS, Seika_MossgroveWoods_Text_AltairIntro, Seika_MossgroveWoods_Text_AltairDefeat, Seika_MossgroveWoods_EventScript_AltairDefeated
	msgbox Seika_MossgroveWoods_Text_AltairAfter, MSGBOX_AUTOCLOSE
	end

Seika_MossgroveWoods_EventScript_AltairDefeated::
	msgbox Seika_MossgroveWoods_Text_AltairAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SEIKA_MOSSGROVE_WOODS_ALTAIR
	setflag FLAG_QOL_SEIKA_ALTAIR_WOODS
	fadescreen FADE_FROM_BLACK
	release
	end

Seika_MossgroveWoods_Text_AltairIntro::
	.string "Hah! The kid from KANTO. Of\n"
	.string "course it's you.\p"
	.string "A thousand years ago the first\n"
	.string "STARFALL threw stones all over\l"
	.string "these woods.\p"
	.string "One of them is under my shovel\n"
	.string "right now. A star stone!\p"
	.string "DENEB needs it, and I won't let\n"
	.string "you dig it up first!$"
	.string "{JPN}{JPN}アルタイル『はっ！　カントーの　こどもか。\n"
	.string "やっぱり　おまえだったか。\p"
	.string "せんねんまえ　さいしょの　スターフォールが　この\n"
	.string "もりじゅうに　いしを　ばらまいた。\p"
	.string "その　ひとつが　いま　おれの　スコップの　したに\n"
	.string "ある。　スターストーンさ！\p"
	.string "デネブが　ほしがってるんでな。　おまえに　さきを\n"
	.string "こされて　たまるかよ！』$"

Seika_MossgroveWoods_Text_AltairDefeat::
	.string "Bah! Buried again!$"
	.string "{JPN}{JPN}ちっ！　また　うめられたぜ！$"

Seika_MossgroveWoods_Text_AltairAfter::
	.string "Fine. Keep your hole in the dirt.\p"
	.string "DENEB already has the scroll\n"
	.string "copied. She's going north to\l"
	.string "KILN CITY, for the bell.\p"
	.string "We'll wake that star whether you\n"
	.string "like it or not!$"
	.string "{JPN}{JPN}アルタイル『けっ　いいさ。　その　つちの　あなは\n"
	.string "くれてやる。\p"
	.string "デネブは　もう　まきものを　うつしおえた。\n"
	.string "かねを　もとめて　きたの　カマモトシティへ\l"
	.string "むかってるぜ。\p"
	.string "おまえが　どう　おもおうと　あの　ほしは\n"
	.string "おれたちが　おこして　やるさ！』$"

Seika_MossgroveWoods_EventScript_Riku::
	trainerbattle_single TRAINER_SEIKA_BUG_CATCHER_RIKU, Seika_MossgroveWoods_Text_RikuIntro, Seika_MossgroveWoods_Text_RikuDefeat
	msgbox Seika_MossgroveWoods_Text_RikuPost, MSGBOX_AUTOCLOSE
	end

Seika_MossgroveWoods_Text_RikuIntro::
	.string "The woods are full of bugs! And\n"
	.string "I caught the best ones!$"
	.string "{JPN}{JPN}もりは　むしで　いっぱい！　ぼくは　いちばん\n"
	.string "いいのを　つかまえたんだ！$"

Seika_MossgroveWoods_Text_RikuDefeat::
	.string "Squashed...$"
	.string "{JPN}{JPN}つぶされた‥$"

Seika_MossgroveWoods_Text_RikuPost::
	.string "HERACROSS love the sap in these\n"
	.string "trees. Look in the tall grass!$"
	.string "{JPN}{JPN}ヘラクロスは　この　きの　みつが　だいすき。\n"
	.string "くさむらを　さがして　みて！$"

Seika_MossgroveWoods_EventScript_Nanami::
	trainerbattle_single TRAINER_SEIKA_PICNICKER_NANAMI, Seika_MossgroveWoods_Text_NanamiIntro, Seika_MossgroveWoods_Text_NanamiDefeat
	msgbox Seika_MossgroveWoods_Text_NanamiPost, MSGBOX_AUTOCLOSE
	end

Seika_MossgroveWoods_Text_NanamiIntro::
	.string "A picnic in the woods! Want to\n"
	.string "battle for my sandwich?$"
	.string "{JPN}{JPN}もりで　ピクニック！　わたしの　サンドイッチを\n"
	.string "かけて　バトル　しない？$"

Seika_MossgroveWoods_Text_NanamiDefeat::
	.string "Aww, you can have the sandwich.$"
	.string "{JPN}{JPN}あーあ　サンドイッチ　あげるわ。$"

Seika_MossgroveWoods_Text_NanamiPost::
	.string "URSARING smell honey from miles\n"
	.string "away. MOSSGROVE is full of it!$"
	.string "{JPN}{JPN}リングマは　とおくからでも　ミツの　においが\n"
	.string "わかるの。　コケムスむらは　ミツだらけよ！$"

Seika_MossgroveWoods_EventScript_Kaori::
	trainerbattle_single TRAINER_SEIKA_AROMA_LADY_KAORI, Seika_MossgroveWoods_Text_KaoriIntro, Seika_MossgroveWoods_Text_KaoriDefeat
	msgbox Seika_MossgroveWoods_Text_KaoriPost, MSGBOX_AUTOCLOSE
	end

Seika_MossgroveWoods_Text_KaoriIntro::
	.string "Breathe in... The woods smell of\n"
	.string "moss and flowers. Now, battle!$"
	.string "{JPN}{JPN}すって‥　はいて‥　もりは　コケと　はなの\n"
	.string "かおり。　さあ　バトルよ！$"

Seika_MossgroveWoods_Text_KaoriDefeat::
	.string "What a sweet battle.$"
	.string "{JPN}{JPN}なんて　あまい　バトルかしら。$"

Seika_MossgroveWoods_Text_KaoriPost::
	.string "The bees of MOSSGROVE drink from\n"
	.string "these flowers. Be gentle here.$"
	.string "{JPN}{JPN}コケムスむらの　ハチたちは　この　はなの　ミツを\n"
	.string "すうの。　ここでは　やさしくね。$"

Seika_MossgroveWoods_EventScript_Isamu::
	trainerbattle_single TRAINER_SEIKA_BUG_MANIAC_ISAMU, Seika_MossgroveWoods_Text_IsamuIntro, Seika_MossgroveWoods_Text_IsamuDefeat
	msgbox Seika_MossgroveWoods_Text_IsamuPost, MSGBOX_AUTOCLOSE
	end

Seika_MossgroveWoods_Text_IsamuIntro::
	.string "Shh! You'll scare the bugs!\n"
	.string "Oh, you want to battle? Fine!$"
	.string "{JPN}{JPN}しーっ！　むしが　にげちゃう！　え？　バトル\n"
	.string "したいの？　いいよ！$"

Seika_MossgroveWoods_Text_IsamuDefeat::
	.string "My bugs... buzzed off.$"
	.string "{JPN}{JPN}ぼくの　むしたち‥　とんで　いっちゃった。$"

Seika_MossgroveWoods_Text_IsamuPost::
	.string "Somebody's been digging holes all\n"
	.string "over the woods. It upsets the\l"
	.string "bugs!$"
	.string "{JPN}{JPN}だれかが　もりじゅうに　あなを　ほってるんだ。\n"
	.string "むしたちが　こまってるよ！$"

Seika_MossgroveWoods_EventScript_ItemStarPiece::
	finditem ITEM_STAR_PIECE
	end

Seika_MossgroveWoods_EventScript_ItemBigRoot::
	finditem ITEM_BIG_ROOT
	end
