RocketHideout_B3F_Frlg_MapScripts::
	.byte 0

RocketHideout_B3F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_14, RocketHideout_B3F_Text_Grunt1Intro, RocketHideout_B3F_Text_Grunt1Defeat
	msgbox RocketHideout_B3F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B3F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_15, RocketHideout_B3F_Text_Grunt2Intro, RocketHideout_B3F_Text_Grunt2Defeat
	msgbox RocketHideout_B3F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B3F_Text_Grunt1Intro::
	.string "Stop meddling in TEAM ROCKET's\n"
	.string "affairs or else!$"
	.string "{JPN}{JPN}ロケットだんの　じゃまを　するんじゃ　ねえ！$"

RocketHideout_B3F_Text_Grunt1Defeat::
	.string "Oof!\n"
	.string "Taken down!$"
	.string "{JPN}{JPN}ぐはっ！　やられた！$"

RocketHideout_B3F_Text_Grunt1PostBattle::
	.string "SILPH SCOPE?\n"
	.string "Oh, that machine the BOSS stole.\p"
	.string "It's here somewhere.$"
	.string "{JPN}{JPN}シルフスコープ？　ああ　ボスが　ぬすんだ\n"
	.string "きかいの　ことか。\p"
	.string "この　どこかに　あるぜ。$"

RocketHideout_B3F_Text_Grunt2Intro::
	.string "We got word from upstairs that you\n"
	.string "were coming!$"
	.string "{JPN}{JPN}おまえが　くることは　うえから　きいてるぜ！$"

RocketHideout_B3F_Text_Grunt2Defeat::
	.string "What?\n"
	.string "I lost? No!$"
	.string "{JPN}{JPN}なに？　まけた？　うそだ！$"

RocketHideout_B3F_Text_Grunt2PostBattle::
	.string "Go ahead and go!\p"
	.string "But you can't run the elevator\n"
	.string "without the LIFT KEY.$"
	.string "{JPN}{JPN}いきたきゃ　いけよ！\p"
	.string "でも　エレベータのキーが　なけりゃ\n"
	.string "エレベーターは　うごかないぜ。$"

