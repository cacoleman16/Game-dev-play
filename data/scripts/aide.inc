Aide_EventScript_HaventCaughtEnough::
	msgbox Aide_Text_HaventCaughtEnoughMonsForItem
	release
	end

Aide_EventScript_NoRoomForItem::
	msgbox Aide_Text_DontHaveAnyRoomForItem
	release
	end

Aide_EventScript_DeclineCheckMons::
	msgbox Aide_Text_GetEnoughMonsComeBackForItem
	release
	end

Aide_Text_HaventCaughtEnoughMonsForItem::
	.string "Let's see…\n"
	.string "Uh-oh! You have caught only\l"
	.string "{STR_VAR_3} kinds of POKéMON!\p"
	.string "You need {STR_VAR_1} kinds\n"
	.string "if you want the {STR_VAR_2}.$"
	.string "{JPN}{JPN}どれどれ‥　あらら！　つかまえた　ポケモンは\n"
	.string "まだ　{STR_VAR_3}しゅるい　だけですね！\p"
	.string "{STR_VAR_2}が　ほしいなら\n"
	.string "{STR_VAR_1}しゅるい　ひつよう　ですよ$"

Aide_Text_GetEnoughMonsComeBackForItem::
	.string "…Oh. I see.\p"
	.string "When you get {STR_VAR_1} kinds of POKéMON,\n"
	.string "come back for the {STR_VAR_2}.$"
	.string "{JPN}{JPN}‥あ　そうですか\p"
	.string "ポケモンを　{STR_VAR_1}しゅるい\n"
	.string "つかまえたら　{STR_VAR_2}を　とりに\l"
	.string "きて　ください$"

Aide_Text_DontHaveAnyRoomForItem::
	.string "Oh! I see you don't have any\n"
	.string "room for the {STR_VAR_2}.$"
	.string "{JPN}{JPN}あれ！　{STR_VAR_2}を　いれる　ばしょが\n"
	.string "ない　みたいですね$"
