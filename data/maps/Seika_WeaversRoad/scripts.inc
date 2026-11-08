Seika_WeaversRoad_MapScripts::
	.byte 0

Seika_WeaversRoad_EventScript_SignEast::
	msgbox Seika_WeaversRoad_Text_SignEast, MSGBOX_SIGN
	end

Seika_WeaversRoad_Text_SignEast::
	.string "WEAVER'S ROAD\n"
	.string "SILKHOLLOW - FROSTMERE$"
	.string "{JPN}{JPN}ハタオリかいどう　キヌウツロタウンから\n"
	.string "シモミズむらへ$"

Seika_WeaversRoad_EventScript_SignWest::
	msgbox Seika_WeaversRoad_Text_SignWest, MSGBOX_SIGN
	end

Seika_WeaversRoad_Text_SignWest::
	.string "WEAVER'S ROAD\n"
	.string "FROSTMERE - SILKHOLLOW$"
	.string "{JPN}{JPN}ハタオリかいどう　シモミズむらから\n"
	.string "キヌウツロタウンへ$"

Seika_WeaversRoad_EventScript_Tsumugi::
	trainerbattle_single TRAINER_SEIKA_BUG_MANIAC_TSUMUGI, Seika_WeaversRoad_Text_TsumugiIntro, Seika_WeaversRoad_Text_TsumugiDefeat
	msgbox Seika_WeaversRoad_Text_TsumugiPost, MSGBOX_AUTOCLOSE
	end

Seika_WeaversRoad_Text_TsumugiIntro::
	.string "You walked right into my web!\n"
	.string "BUG POKéMON, go!$"
	.string "{JPN}{JPN}ぼくの　くものすに　まんまと　かかったな！　いけ\n"
	.string "むしポケモン！$"

Seika_WeaversRoad_Text_TsumugiDefeat::
	.string "My web... torn to pieces!$"
	.string "{JPN}{JPN}くものすが‥　ずたずただ！$"

Seika_WeaversRoad_Text_TsumugiPost::
	.string "ARIADOS silk is so strong, the\n"
	.string "weavers use it for their looms.$"
	.string "{JPN}{JPN}アリアドスの　いとは　とても　じょうぶ。\n"
	.string "はたおりきにも　つかわれて　いるんだ。$"

Seika_WeaversRoad_EventScript_Kinu::
	trainerbattle_single TRAINER_SEIKA_AROMA_LADY_KINU, Seika_WeaversRoad_Text_KinuIntro, Seika_WeaversRoad_Text_KinuDefeat
	msgbox Seika_WeaversRoad_Text_KinuPost, MSGBOX_AUTOCLOSE
	end

Seika_WeaversRoad_Text_KinuIntro::
	.string "The flowers on this road dye the\n"
	.string "silk of SILKHOLLOW. Lovely, no?$"
	.string "{JPN}{JPN}この　みちの　はなで　キヌウツロタウンの　きぬを\n"
	.string "そめるのよ。　すてきでしょ？$"

Seika_WeaversRoad_Text_KinuDefeat::
	.string "My bouquet is all wilted!$"
	.string "{JPN}{JPN}はなたばが　しおれちゃった！$"

Seika_WeaversRoad_Text_KinuPost::
	.string "Red petals for the festival silk,\n"
	.string "blue for the winter silk.$"
	.string "{JPN}{JPN}あかい　はなびらは　おまつりの　きぬ。　あおい\n"
	.string "はなびらは　ふゆの　きぬよ。$"

Seika_WeaversRoad_EventScript_Daiki::
	trainerbattle_single TRAINER_SEIKA_COOLTRAINER_DAIKI, Seika_WeaversRoad_Text_DaikiIntro, Seika_WeaversRoad_Text_DaikiDefeat
	msgbox Seika_WeaversRoad_Text_DaikiPost, MSGBOX_AUTOCLOSE
	end

Seika_WeaversRoad_Text_DaikiIntro::
	.string "Six BADGES? Let's see if you've\n"
	.string "earned them!$"
	.string "{JPN}{JPN}バッジ　6つか？　だてじゃ　ないか　ためさせて\n"
	.string "もらうぜ！$"

Seika_WeaversRoad_Text_DaikiDefeat::
	.string "Yeah, you've earned them.$"
	.string "{JPN}{JPN}ああ　だてじゃ　なかったな。$"

Seika_WeaversRoad_Text_DaikiPost::
	.string "It gets cold past here. FROSTMERE\n"
	.string "is where winter lives.$"
	.string "{JPN}{JPN}この　さきは　さむく　なるぞ。　シモミズむらは\n"
	.string "ふゆが　すむ　ところだ。$"

Seika_WeaversRoad_EventScript_Mayu::
	trainerbattle_single TRAINER_SEIKA_LASS_MAYU, Seika_WeaversRoad_Text_MayuIntro, Seika_WeaversRoad_Text_MayuDefeat
	msgbox Seika_WeaversRoad_Text_MayuPost, MSGBOX_AUTOCLOSE
	end

Seika_WeaversRoad_Text_MayuIntro::
	.string "My name means cocoon! Want to see\n"
	.string "my POKéMON break out?$"
	.string "{JPN}{JPN}わたしの　なまえ　マユって　まゆの　ことなの！\n"
	.string "ポケモンが　まゆを　やぶる　ところ　みたい？$"

Seika_WeaversRoad_Text_MayuDefeat::
	.string "Wrapped up and beaten...$"
	.string "{JPN}{JPN}まゆに　つつまれた　まま　まけちゃった‥$"

Seika_WeaversRoad_Text_MayuPost::
	.string "One day my LEDYBA all became\n"
	.string "LEDIAN. Now they never sit still!$"
	.string "{JPN}{JPN}ある　ひ　レディバが　みんな　レディアンに\n"
	.string "なったの。　いまでは　じっと　して　いないのよ！$"
