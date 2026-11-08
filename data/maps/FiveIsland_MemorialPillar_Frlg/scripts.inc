FiveIsland_MemorialPillar_Frlg_MapScripts::
	.byte 0

FiveIsland_MemorialPillar_EventScript_MemorialMan::
	lock
	goto_if_set FLAG_GOT_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_AlreadyGotTM42
	goto_if_set FLAG_NO_ROOM_FOR_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_ReturnedForTM42
	msgbox FiveIsland_MemorialPillar_Text_ScrubScrub
	closemessage
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_FacePlayer
	waitmovement 0
	delay 45
	msgbox FiveIsland_MemorialPillar_Text_YourMonsLookHealthy
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox FiveIsland_MemorialPillar_Text_ThisIsWhereIBuriedMyOnix
	release
	end

FiveIsland_MemorialPillar_EventScript_AlreadyGotTM42::
	msgbox FiveIsland_MemorialPillar_Text_ScrubScrubTectonix
	release
	end

FiveIsland_MemorialPillar_EventScript_ReturnedForTM42::
	msgbox FiveIsland_MemorialPillar_Text_StillHaveThingAsMyThanks
	giveitem ITEM_TM42
	goto_if_eq VAR_RESULT, FALSE, FiveIsland_MemorialPillar_EventScript_NoRoomForTM42
	call FiveIsland_MemorialPillar_EventScript_ReceivedTM42
	release
	end

FiveIsland_MemorialPillar_EventScript_Memorial::
	lockall
	goto_if_set FLAG_GOT_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_MemorialLemonadeAlreadyPlaced
	goto_if_set FLAG_NO_ROOM_FOR_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_MemorialLemonadeAlreadyPlaced
	msgbox FiveIsland_MemorialPillar_Text_HereLiesTectonixLemonadeOffering
	checkitem ITEM_LEMONADE
	goto_if_eq VAR_RESULT, TRUE, FiveIsland_MemorialPillar_EventScript_AskPlaceLemonade
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_AskPlaceLemonade::
	msgbox FiveIsland_MemorialPillar_Text_LeaveAnotherLemonadeOffering, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FiveIsland_MemorialPillar_EventScript_PlaceLemonade
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_PlaceLemonade::
	removeitem ITEM_LEMONADE
	msgbox FiveIsland_MemorialPillar_Text_PlacedCanOfLemonade
	closemessage
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 45
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox FiveIsland_MemorialPillar_Text_ThankYouPleaseTakeThis
	giveitem ITEM_TM42
	goto_if_eq VAR_RESULT, FALSE, FiveIsland_MemorialPillar_EventScript_NoRoomForTM42
	call FiveIsland_MemorialPillar_EventScript_ReceivedTM42
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_ReceivedTM42::
	msgbox FiveIsland_MemorialPillar_Text_BeGoodToYourMonsToo
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	setflag FLAG_GOT_TM42_AT_MEMORIAL_PILLAR
	return

FiveIsland_MemorialPillar_EventScript_NoRoomForTM42::
	setflag FLAG_NO_ROOM_FOR_TM42_AT_MEMORIAL_PILLAR
	msgbox FiveIsland_MemorialPillar_Text_DontHaveRoomForIt
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_MemorialLemonadeAlreadyPlaced::
	msgbox FiveIsland_MemorialPillar_Text_HereLiesTectonix
	releaseall
	end

FiveIsland_MemorialPillar_Text_MiloIntro::
	.string "I'm the eldest of the BIRD\n"
	.string "BROTHERS.\p"
	.string "The best thing about birds…\n"
	.string "It has to be the beak.$"
	.string "{JPN}{JPN}おれは　とり　きょうだいの　ちょうなん！\p"
	.string "とりの　いちばんの　みりょくは‥　やっぱり\n"
	.string "くちばしだ！$"

FiveIsland_MemorialPillar_Text_MiloDefeat::
	.string "Oh, so sharp!$"
	.string "{JPN}{JPN}うわ　するどい！$"

FiveIsland_MemorialPillar_Text_MiloPostBattle::
	.string "You can fly to FOUR ISLAND from\n"
	.string "here.$"
	.string "{JPN}{JPN}ここからなら　4のしまへ　そらをとぶで　いけるぞ$"

FiveIsland_MemorialPillar_Text_ChazIntro::
	.string "I'm the middle kid of the BIRD\n"
	.string "BROTHERS.\p"
	.string "The best thing about birds…\n"
	.string "It has to be the wings.$"
	.string "{JPN}{JPN}おれは　とり　きょうだいの　じなん！\p"
	.string "とりの　いちばんの　みりょくは‥　やっぱり\n"
	.string "つばさだ！$"

FiveIsland_MemorialPillar_Text_ChazDefeat::
	.string "Flap, flap!$"
	.string "{JPN}{JPN}バサッ　バサッ！$"

FiveIsland_MemorialPillar_Text_ChazPostBattle::
	.string "There was a really somber guy\n"
	.string "over that way.$"
	.string "{JPN}{JPN}あっちに　すごく　しずんだ　かおの　ひとが\n"
	.string "いたよ$"

FiveIsland_MemorialPillar_Text_HaroldIntro::
	.string "I'm the youngest of the BIRD\n"
	.string "BROTHERS.\p"
	.string "The best thing about birds…\n"
	.string "It has to be the down.$"
	.string "{JPN}{JPN}ぼくは　とり　きょうだいの　すえっこ！\p"
	.string "とりの　いちばんの　みりょくは‥　やっぱり\n"
	.string "うもうだね！$"

FiveIsland_MemorialPillar_Text_HaroldDefeat::
	.string "Down is so comfy…\n"
	.string "It makes me drowsy…$"
	.string "{JPN}{JPN}うもうって　ふかふか‥　ねむく　なっちゃう‥$"

FiveIsland_MemorialPillar_Text_HaroldPostBattle::
	.string "It fills me with happiness when\n"
	.string "bird POKéMON beg playfully…$"
	.string "{JPN}{JPN}とりポケモンが　じゃれて　あまえて　くると\n"
	.string "しあわせな　きもちに　なるんだ‥$"

FiveIsland_MemorialPillar_Text_ScrubScrub::
	.string "Scrub, scrub…$"
	.string "{JPN}{JPN}ゴシ　ゴシ‥$"

FiveIsland_MemorialPillar_Text_YourMonsLookHealthy::
	.string "Hello…\n"
	.string "Your POKéMON look healthy…$"
	.string "{JPN}{JPN}こんにちは‥　あなたの　ポケモン　げんきそう\n"
	.string "ですね‥$"

FiveIsland_MemorialPillar_Text_ThisIsWhereIBuriedMyOnix::
	.string "This is where I buried my ONIX…\n"
	.string "It was named TECTONIX…$"
	.string "{JPN}{JPN}ここに　わたしの　イワークを　ほうむったんです‥\n"
	.string "なまえは　イワオ　でした‥$"

FiveIsland_MemorialPillar_Text_HereLiesTectonixLemonadeOffering::
	.string "“Here lies TECTONIX” is engraved\n"
	.string "on a boulder.\p"
	.string "There is a LEMONADE placed as an\n"
	.string "offering in front of the grave.$"
	.string "{JPN}{JPN}『イワオ　ここに　ねむる』と　いわに\n"
	.string "きざまれている\p"
	.string "おはかの　まえに　ミックスオレが　おそなえ\n"
	.string "されている$"

FiveIsland_MemorialPillar_Text_LeaveAnotherLemonadeOffering::
	.string "Would you like to leave another\n"
	.string "LEMONADE as an offering?$"
	.string "{JPN}{JPN}ミックスオレを　おそなえ　しますか？$"

FiveIsland_MemorialPillar_Text_PlacedCanOfLemonade::
	.string "{PLAYER} placed a can of LEMONADE\n"
	.string "in front of the grave.$"
	.string "{JPN}{JPN}{PLAYER}は　おはかの　まえに\n"
	.string "ミックスオレを　おそなえ　した$"

FiveIsland_MemorialPillar_Text_ThankYouPleaseTakeThis::
	.string "Th-thank you…\p"
	.string "That was my TECTONIX's favorite\n"
	.string "drink…\p"
	.string "I don't even know you, but your\n"
	.string "act of kindness…\p"
	.string "It's uplifted me a little.\p"
	.string "Can you please take this as my\n"
	.string "thanks?$"
	.string "{JPN}{JPN}あ　ありがとう　ございます‥\p"
	.string "それは　わたしの　イワオの　だいすきな　のみもの\n"
	.string "でした‥\p"
	.string "みずしらずの　かたなのに　その　やさしさ‥\p"
	.string "すこし　げんきが　でました\p"
	.string "おれいに　これを　うけとって　もらえませんか？$"

FiveIsland_MemorialPillar_Text_BeGoodToYourMonsToo::
	.string "Please be good to your POKéMON,\n"
	.string "too.$"
	.string "{JPN}{JPN}あなたも　ポケモンを　たいせつに　して　あげて\n"
	.string "くださいね$"

FiveIsland_MemorialPillar_Text_DontHaveRoomForIt::
	.string "If you don't have room for it,\n"
	.string "I'll hold it for you till next time.$"
	.string "{JPN}{JPN}もちきれない　ようでしたら　つぎの　ときまで\n"
	.string "あずかって　おきますね$"

FiveIsland_MemorialPillar_Text_StillHaveThingAsMyThanks::
	.string "Oh, it's you from that time…\p"
	.string "I still have that thing I wanted\n"
	.string "you to have as my thanks.$"
	.string "{JPN}{JPN}ああ　あの　ときの‥\p"
	.string "おれいに　さしあげたかった　もの　まだ\n"
	.string "あずかって　いますよ$"

FiveIsland_MemorialPillar_Text_ScrubScrubTectonix::
	.string "Scrub, scrub…\p"
	.string "… … …\n"
	.string "TECTONIX…$"
	.string "{JPN}{JPN}ゴシ　ゴシ‥\p"
	.string "‥　‥　‥　イワオ‥$"

FiveIsland_MemorialPillar_Text_HereLiesTectonix::
	.string "Boulders and rocks are neatly piled\n"
	.string "up as a cairn.\p"
	.string "“Here lies TECTONIX” is engraved\n"
	.string "on a boulder.$"
	.string "{JPN}{JPN}いわや　いしが　きれいに　つみあげられている\p"
	.string "『イワオ　ここに　ねむる』と　いわに\n"
	.string "きざまれている$"

