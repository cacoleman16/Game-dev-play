FuchsiaCity_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FuchsiaCity_OnTransition
	.byte 0

FuchsiaCity_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_FUCHSIA_CITY
	call_if_set FLAG_GOT_DOME_FOSSIL, FuchsiaCity_EventScript_SetOmanyteGfx
	call_if_unset FLAG_GOT_DOME_FOSSIL, FuchsiaCity_EventScript_SetKabutoGfx
	end

FuchsiaCity_EventScript_SetOmanyteGfx::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OMANYTE
	return

FuchsiaCity_EventScript_SetKabutoGfx::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_KABUTO
	return

FuchsiaCity_EventScript_LittleBoy::
	msgbox FuchsiaCity_Text_DidYouTrySafariGame, MSGBOX_NPC
	end

FuchsiaCity_EventScript_OldMan::
	msgbox FuchsiaCity_Text_SafariZoneZooInFront, MSGBOX_NPC
	end

FuchsiaCity_EventScript_Erik::
	msgbox FuchsiaCity_Text_WheresSara, MSGBOX_NPC
	end

FuchsiaCity_EventScript_Youngster::
	msgbox FuchsiaCity_Text_ItemBallInThere, MSGBOX_NPC
	end

FuchsiaCity_EventScript_Lass::
	lock
	faceplayer
	famechecker FAMECHECKER_KOGA, 3
	msgbox FuchsiaCity_Text_MyFatherIsGymLeader
	release
	end

FuchsiaCity_EventScript_CitySign::
	msgbox FuchsiaCity_Text_CitySign, MSGBOX_SIGN
	end

FuchsiaCity_EventScript_SafariZoneSign::
	msgbox FuchsiaCity_Text_SafariZoneSign, MSGBOX_SIGN
	end

FuchsiaCity_EventScript_SafariGameSign::
	msgbox FuchsiaCity_Text_SafariGameSign, MSGBOX_SIGN
	end

FuchsiaCity_EventScript_WardensHomeSign::
	msgbox FuchsiaCity_Text_WardensHomeSign, MSGBOX_SIGN
	end

@ Unused
FuchsiaCity_EventScript_SafariZoneOfficeSign::
	msgbox FuchsiaCity_Text_SafariZoneOfficeSign, MSGBOX_SIGN
	end

FuchsiaCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_KOGA, 0
	msgbox FuchsiaCity_Text_GymSign
	releaseall
	end

FuchsiaCity_EventScript_ChanseySign::
	lockall
	setvar VAR_0x8004, SPECIES_CHANSEY
	special SetSeenMon
	showmonpic SPECIES_CHANSEY, 10, 3
	msgbox FuchsiaCity_Text_ChanseySign
	hidemonpic
	releaseall
	end

FuchsiaCity_EventScript_VoltorbSign::
	lockall
	setvar VAR_0x8004, SPECIES_VOLTORB
	special SetSeenMon
	showmonpic SPECIES_VOLTORB, 10, 3
	msgbox FuchsiaCity_Text_VoltorbSign
	hidemonpic
	releaseall
	end

FuchsiaCity_EventScript_KangaskhanSign::
	lockall
	setvar VAR_0x8004, SPECIES_KANGASKHAN
	special SetSeenMon
	showmonpic SPECIES_KANGASKHAN, 10, 3
	msgbox FuchsiaCity_Text_KangaskhanSign
	hidemonpic
	releaseall
	end

FuchsiaCity_EventScript_SlowpokeSign::
	lockall
	setvar VAR_0x8004, SPECIES_SLOWPOKE
	special SetSeenMon
	showmonpic SPECIES_SLOWPOKE, 10, 3
	msgbox FuchsiaCity_Text_SlowpokeSign
	hidemonpic
	releaseall
	end

FuchsiaCity_EventScript_LaprasSign::
	lockall
	setvar VAR_0x8004, SPECIES_LAPRAS
	special SetSeenMon
	showmonpic SPECIES_LAPRAS, 10, 3
	msgbox FuchsiaCity_Text_LaprasSign
	hidemonpic
	releaseall
	end

FuchsiaCity_EventScript_FossilMonSign::
	lockall
	goto_if_set FLAG_GOT_DOME_FOSSIL, FuchsiaCity_EventScript_OmanyteSign
	setvar VAR_0x8004, SPECIES_KABUTO
	special SetSeenMon
	showmonpic SPECIES_KABUTO, 10, 3
	msgbox FuchsiaCity_Text_KabutoSign
	hidemonpic
	releaseall
	end

FuchsiaCity_EventScript_OmanyteSign::
	setvar VAR_0x8004, SPECIES_OMANYTE
	special SetSeenMon
	showmonpic SPECIES_OMANYTE, 10, 3
	msgbox FuchsiaCity_Text_OmanyteSign
	hidemonpic
	releaseall
	end

FuchsiaCity_Text_DidYouTrySafariGame::
	.string "Did you try the SAFARI GAME?\p"
	.string "There are some rare POKéMON that\n"
	.string "can only be caught there.$"
	.string "{JPN}{JPN}サファリゲームは　やって　みた？\p"
	.string "あそこでしか　つかまえられない　めずらしい\n"
	.string "ポケモンも　いるのよ。$"

FuchsiaCity_Text_SafariZoneZooInFront::
	.string "The SAFARI ZONE has a zoo in front\n"
	.string "of the entrance.\p"
	.string "Out back is the SAFARI GAME for\n"
	.string "catching POKéMON.$"
	.string "{JPN}{JPN}サファリゾーンの　いりぐちの　まえには\n"
	.string "どうぶつえんが　あるんだ。\p"
	.string "おくには　ポケモンを　つかまえる\n"
	.string "サファリゲームが　あるよ。$"

FuchsiaCity_Text_WheresSara::
	.string "ERIK: Where's SARA?\n"
	.string "I said I'd meet her here.$"
	.string "{JPN}{JPN}エリック:　サラは　どこだ？　ここで　まちあわせ\n"
	.string "したのに。$"

FuchsiaCity_Text_ItemBallInThere::
	.string "That item ball in there…\n"
	.string "Were you wanting that?\p"
	.string "Me, too!\n"
	.string "…Huh? That's a POKéMON?$"
	.string "{JPN}{JPN}あそこの　どうぐ‥　きみも　ほしかったの？\p"
	.string "ぼくも！　‥あれ？　あれって　ポケモン　なの？$"

FuchsiaCity_Text_CitySign::
	.string "FUCHSIA CITY\n"
	.string "Behold! It's Passion Pink!$"
	.string "{JPN}{JPN}セキチクシティ\n"
	.string "みよ！　これぞ　じょうねつの　ピンク！$"

FuchsiaCity_Text_SafariZoneSign::
	.string "POKéMON PARADISE\n"
	.string "SAFARI ZONE$"
	.string "{JPN}{JPN}ポケモンの　らくえん\n"
	.string "サファリゾーン$"

FuchsiaCity_Text_SafariGameSign::
	.string "SAFARI GAME\n"
	.string "POKéMON-U-CATCH!$"
	.string "{JPN}{JPN}サファリゲーム\n"
	.string "ポケモン　つかまえ　ほうだい！$"

FuchsiaCity_Text_WardensHomeSign::
	.string "SAFARI ZONE\n"
	.string "WARDEN'S HOME$"
	.string "{JPN}{JPN}サファリゾーン\n"
	.string "えんちょうの　いえ$"

FuchsiaCity_Text_SafariZoneOfficeSign::
	.string "POKéMON PARADISE!\n"
	.string "Welcome to the SAFARI ZONE!\l"
	.string "SAFARI ZONE OFFICE$"
	.string "{JPN}{JPN}ポケモンの　らくえん！\n"
	.string "サファリゾーンへ　ようこそ！\l"
	.string "サファリゾーン　じむしょ$"

FuchsiaCity_Text_GymSign::
	.string "FUCHSIA CITY POKéMON GYM\n"
	.string "LEADER: KOGA\l"
	.string "The Poisonous Ninja Master$"
	.string "{JPN}{JPN}セキチクシティ　ポケモンジム\n"
	.string "リーダー:　キョウ\l"
	.string "どくの　にんじゃ　マスター$"

FuchsiaCity_Text_ChanseySign::
	.string "Name: CHANSEY\n"
	.string "Catching one is all up to chance.$"
	.string "{JPN}{JPN}なまえ:　ラッキー\n"
	.string "つかまえられたら　ラッキー！$"

FuchsiaCity_Text_VoltorbSign::
	.string "Name: VOLTORB\n"
	.string "The very image of a POKé BALL.$"
	.string "{JPN}{JPN}なまえ:　ビリリダマ\n"
	.string "モンスターボールに　そっくり。$"

FuchsiaCity_Text_KangaskhanSign::
	.string "Name: KANGASKHAN\p"
	.string "A maternal POKéMON that raises its\n"
	.string "young in a pouch on its belly.$"
	.string "{JPN}{JPN}なまえ:　ガルーラ\p"
	.string "おなかの　ふくろで　こどもを　そだてる\n"
	.string "おかあさん　ポケモン。$"

FuchsiaCity_Text_SlowpokeSign::
	.string "Name: SLOWPOKE\n"
	.string "Friendly and very slow moving.$"
	.string "{JPN}{JPN}なまえ:　ヤドン\n"
	.string "ひとなつっこくて　とても　のんびりや。$"

FuchsiaCity_Text_LaprasSign::
	.string "Name: LAPRAS\n"
	.string "AKA the king of the seas.$"
	.string "{JPN}{JPN}なまえ:　ラプラス\n"
	.string "またの　なを　うみの　おうじゃ。$"

FuchsiaCity_Text_OmanyteSign::
	.string "Name: OMANYTE\n"
	.string "An extremely rare POKéMON that was\l"
	.string "regenerated from a fossil.$"
	.string "{JPN}{JPN}なまえ:　オムナイト\n"
	.string "かせきから　ふっかつ　した　とても　めずらしい\l"
	.string "ポケモン。$"

FuchsiaCity_Text_KabutoSign::
	.string "Name: KABUTO\n"
	.string "An extremely rare POKéMON that was\l"
	.string "regenerated from a fossil.$"
	.string "{JPN}{JPN}なまえ:　カブト\n"
	.string "かせきから　ふっかつ　した　とても　めずらしい\l"
	.string "ポケモン。$"

@ Unused. May have been meant to be the placeholder text for the zoo mons, similar to RB's "!" text
FuchsiaCity_Text_Ellipsis::
	.string "……$"

Text_SubstituteTeach::
	.string "Aww, I wish I was a KANGASKHAN\n"
	.string "baby.\p"
	.string "I'd love to be a substitute for the\n"
	.string "baby…\p"
	.string "And snuggle in the mother\n"
	.string "KANGASKHAN's belly pouch.\p"
	.string "But only POKéMON can use the\n"
	.string "technique SUBSTITUTE…\p"
	.string "Want me to teach SUBSTITUTE to\n"
	.string "one of your POKéMON?$"
	.string "{JPN}{JPN}ああ　ガルーラの　あかちゃんに　なりたいなあ。\p"
	.string "あかちゃんの　みがわりに　なって‥\p"
	.string "おかあさん　ガルーラの　おなかの　ふくろで\n"
	.string "まるく　なりたい！\p"
	.string "でも　みがわりの　わざを　つかえるのは　ポケモン\n"
	.string "だけ‥\p"
	.string "きみの　ポケモンに　みがわりを　おしえて\n"
	.string "あげようか？$"

Text_SubstituteDeclined::
	.string "Oh, really?\n"
	.string "SUBSTITUTE seems so fun…$"
	.string "{JPN}{JPN}えっ　ほんとに？　みがわり　たのしそう　なのに‥$"

Text_SubstituteWhichMon::
	.string "Which POKéMON wants to learn\n"
	.string "SUBSTITUTE?$"
	.string "{JPN}{JPN}どの　ポケモンに　みがわりを　おしえる？$"

Text_SubstituteTaught::
	.string "Boy, what I'd give to crawl inside\n"
	.string "a KANGASKHAN belly pouch…$"
	.string "{JPN}{JPN}ああ　ガルーラの　おなかの　ふくろに\n"
	.string "もぐりこめたら　なあ‥$"

FuchsiaCity_Text_MyFatherIsGymLeader::
	.string "My father is the GYM LEADER of\n"
	.string "this town.\p"
	.string "I'm training to use POISON POKéMON\n"
	.string "as well as my father.$"
	.string "{JPN}{JPN}わたしの　ちちうえは　この　まちの　ジムリーダー\n"
	.string "です。\p"
	.string "わたしも　ちちうえの　ように　どくの　ポケモンを\n"
	.string "つかいこなせるよう　しゅぎょう　ちゅう　です。$"

