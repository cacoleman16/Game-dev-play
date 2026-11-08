Seika_HollowPath_MapScripts::
	.byte 0

Seika_HollowPath_EventScript_Sign::
	msgbox Seika_HollowPath_Text_Sign, MSGBOX_SIGN
	end

Seika_HollowPath_Text_Sign::
	.string "HOLLOW PATH\n"
	.string "TIDEWATCH TOWN - SILKHOLLOW TOWN$"
	.string "{JPN}{JPN}ウツロのみち　シオミタウンから\n"
	.string "キヌウツロタウンへ$"

Seika_HollowPath_EventScript_Iwao::
	trainerbattle_single TRAINER_SEIKA_HIKER_IWAO, Seika_HollowPath_Text_IwaoIntro, Seika_HollowPath_Text_IwaoDefeat
	msgbox Seika_HollowPath_Text_IwaoPost, MSGBOX_AUTOCLOSE
	end

Seika_HollowPath_Text_IwaoIntro::
	.string "These walls echo like a drum.\n"
	.string "Let's make some noise!$"
	.string "{JPN}{JPN}この　かべは　たいこみたいに　ひびくんだ。\n"
	.string "はでに　いこうぜ！$"

Seika_HollowPath_Text_IwaoDefeat::
	.string "My ears are ringing!$"
	.string "{JPN}{JPN}みみが　キーンと　する！$"

Seika_HollowPath_Text_IwaoPost::
	.string "Up ahead the cliffs close in.\n"
	.string "That's why they call it HOLLOW.$"
	.string "{JPN}{JPN}このさきは　がけが　せまって　くるんだ。　だから\n"
	.string "ウツロのみちって　よばれてるのさ。$"

Seika_HollowPath_EventScript_Kenta::
	trainerbattle_single TRAINER_SEIKA_BLACK_BELT_KENTA, Seika_HollowPath_Text_KentaIntro, Seika_HollowPath_Text_KentaDefeat
	msgbox Seika_HollowPath_Text_KentaPost, MSGBOX_AUTOCLOSE
	end

Seika_HollowPath_Text_KentaIntro::
	.string "I train by punching boulders!\n"
	.string "You look softer than a boulder!$"
	.string "{JPN}{JPN}おれは　いわを　なぐって　きたえて　いる！\n"
	.string "おまえは　いわより　やわらかそうだな！$"

Seika_HollowPath_Text_KentaDefeat::
	.string "Ouch! Harder than a boulder!$"
	.string "{JPN}{JPN}いてっ！　いわより　かたい！$"

Seika_HollowPath_Text_KentaPost::
	.string "HERACROSS can lift a hundred\n"
	.string "times its own weight. Amazing!$"
	.string "{JPN}{JPN}ヘラクロスは　じぶんの　100ばいの　おもさを\n"
	.string "もちあげられるんだ。　すごいぞ！$"

Seika_HollowPath_EventScript_Saki::
	trainerbattle_single TRAINER_SEIKA_PICNICKER_SAKI, Seika_HollowPath_Text_SakiIntro, Seika_HollowPath_Text_SakiDefeat
	msgbox Seika_HollowPath_Text_SakiPost, MSGBOX_AUTOCLOSE
	end

Seika_HollowPath_Text_SakiIntro::
	.string "I'm off to see the silk dancers!\n"
	.string "But first, a battle!$"
	.string "{JPN}{JPN}きぬの　おどりこを　みに　いく　ところなの！\n"
	.string "でも　その　まえに　しょうぶよ！$"

Seika_HollowPath_Text_SakiDefeat::
	.string "You danced circles around me!$"
	.string "{JPN}{JPN}すっかり　おどらされちゃった！$"

Seika_HollowPath_Text_SakiPost::
	.string "SILKHOLLOW is just north of here.\n"
	.string "Its GYM LEADER is a weaver.$"
	.string "{JPN}{JPN}キヌウツロタウンは　この　すぐ　きただよ。\n"
	.string "ジムリーダーは　はたおりの　ひとなの。$"

Seika_HollowPath_EventScript_Altair::
	trainerbattle_single TRAINER_SEIKA_NOVA_ALTAIR_HOLLOW, Seika_HollowPath_Text_AltairIntro, Seika_HollowPath_Text_AltairDefeat, Seika_HollowPath_EventScript_AltairLeaves
	end

Seika_HollowPath_EventScript_AltairLeaves::
	msgbox Seika_HollowPath_Text_AltairAfter, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SEIKA_HOLLOW_PATH_ALTAIR, Seika_HollowPath_Movement_AltairLeave
	waitmovement 0
	removeobject LOCALID_SEIKA_HOLLOW_PATH_ALTAIR
	setflag FLAG_QOL_SEIKA_HIDE_ALTAIR_HOLLOW
	release
	end

Seika_HollowPath_Movement_AltairLeave:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

Seika_HollowPath_Text_AltairIntro::
	.string "Well, well. The kid from KANTO,\n"
	.string "all the way out here.\p"
	.string "Look what these old rocks gave\n"
	.string "me. A STAR STONE, left by the\l"
	.string "first STARFALL!\p"
	.string "DENEB has her STARGLASS BELL.\n"
	.string "But the bell can't ring without\l"
	.string "a stone like this one.\p"
	.string "Now it can. And you're not\n"
	.string "taking it from me!$"
	.string "{JPN}{JPN}アルタイル『ほう。　カントーの　ガキが　こんな\n"
	.string "ところまで　きたか。\p"
	.string "みろよ　この　ふるい　いわが　くれた　ものを。\n"
	.string "さいしょの　スターフォールが　のこした\l"
	.string "スターストーンだ！\p"
	.string "デネブは　ほしガラスのかねを　てに　いれた。\n"
	.string "だが　こういう　いしが　なけりゃ　かねは\l"
	.string "ならないのさ。\p"
	.string "これで　かねは　なる。　おまえなんかに\n"
	.string "わたさないぜ！』$"

Seika_HollowPath_Text_AltairDefeat::
	.string "Bah! Rocks never lie...\n"
	.string "but they don't win battles.$"
	.string "{JPN}{JPN}ちっ！　いわは　うそを　つかないが‥　バトルには\n"
	.string "かてないってか。$"

Seika_HollowPath_Text_AltairAfter::
	.string "Keep your BADGES, kid. I have\n"
	.string "what I came for.\p"
	.string "When that bell rings, the WISHING\n"
	.string "STAR comes down to us. Not you!$"
	.string "{JPN}{JPN}アルタイル『バッジは　とっときな　ガキ。　おれは\n"
	.string "ほしい　ものを　てに　いれたからな。\p"
	.string "かねが　なれば　ねがいぼしは　おれたちの　もとに\n"
	.string "おりる。　おまえじゃ　なくな！』$"
