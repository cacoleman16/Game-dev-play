SaffronCity_Dojo_Frlg_MapScripts::
	.byte 0

SaffronCity_Dojo_EventScript_TriggerMasterBattleLeft::
	lockall
	setvar VAR_MAP_SCENE_SAFFRON_CITY_DOJO, 1
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	releaseall
	end

SaffronCity_Dojo_EventScript_TriggerMasterBattleRight::
	lockall
	setvar VAR_MAP_SCENE_SAFFRON_CITY_DOJO, 1
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	releaseall
	end

SaffronCity_Dojo_EventScript_HitmonleeBall::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HITMON_FROM_DOJO, SaffronCity_Dojo_EventScript_AlreadyGotHitmon
	showmonpic SPECIES_HITMONLEE, 10, 3
	setvar VAR_TEMP_1, SPECIES_HITMONLEE
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SaffronCity_Dojo_Text_YouWantHitmonlee, MSGBOX_YESNO
	call EventScript_RestorePrevTextColor
	goto_if_eq VAR_RESULT, YES, SaffronCity_Dojo_EventScript_GiveHitmon
	hidemonpic
	release
	end

SaffronCity_Dojo_EventScript_AlreadyGotHitmon::
	msgbox SaffronCity_Dojo_Text_BetterNotGetGreedy
	release
	end

SaffronCity_Dojo_EventScript_HitmonchanBall::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HITMON_FROM_DOJO, SaffronCity_Dojo_EventScript_AlreadyGotHitmon
	showmonpic SPECIES_HITMONCHAN, 10, 3
	setvar VAR_TEMP_1, SPECIES_HITMONCHAN
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SaffronCity_Dojo_Text_YouWantHitmonchan, MSGBOX_YESNO
	call EventScript_RestorePrevTextColor
	goto_if_eq VAR_RESULT, YES, SaffronCity_Dojo_EventScript_GiveHitmon
	hidemonpic
	release
	end

SaffronCity_Dojo_EventScript_GiveHitmon::
	hidemonpic
	givemon VAR_TEMP_1, 25
	goto_if_eq VAR_RESULT, 0, SaffronCity_Dojo_EventScript_ReceivedHitmonParty
	goto_if_eq VAR_RESULT, 1, SaffronCity_Dojo_EventScript_ReceivedHitmonPC
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	release
	end

SaffronCity_Dojo_EventScript_ReceivedHitmonParty::
	removeobject VAR_LAST_TALKED
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster
	waitmessage
	waitfanfare
	setflag FLAG_GOT_HITMON_FROM_DOJO
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SaffronCity_Dojo_EventScript_EndGiveMon
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto SaffronCity_Dojo_EventScript_EndGiveMon
	end

SaffronCity_Dojo_EventScript_ReceivedHitmonPC::
	removeobject VAR_LAST_TALKED
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster
	waitmessage
	waitfanfare
	setflag FLAG_GOT_HITMON_FROM_DOJO
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SaffronCity_Dojo_EventScript_TransferredHitmonToPC
	call Common_EventScript_NameReceivedBoxMon
	goto SaffronCity_Dojo_EventScript_TransferredHitmonToPC
	end

SaffronCity_Dojo_EventScript_TransferredHitmonToPC::
	call Common_EventScript_TransferredToPC
	goto SaffronCity_Dojo_EventScript_EndGiveMon
	end

SaffronCity_Dojo_EventScript_EndGiveMon::
	release
	end

SaffronCity_Dojo_EventScript_Statue::
	msgbox SaffronCity_Dojo_Text_FightingDojo, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_LeftScroll::
	msgbox SaffronCity_Dojo_Text_EnemiesOnEverySide, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_RightScroll::
	msgbox SaffronCity_Dojo_Text_GoesAroundComesAround, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_Hitoshi::
	trainerbattle_single TRAINER_BLACK_BELT_HITOSHI, SaffronCity_Dojo_Text_HitoshiIntro, SaffronCity_Dojo_Text_HitoshiDefeat
	msgbox SaffronCity_Dojo_Text_HitoshiPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Hideki::
	trainerbattle_single TRAINER_BLACK_BELT_HIDEKI, SaffronCity_Dojo_Text_HidekiIntro, SaffronCity_Dojo_Text_HidekiDefeat
	msgbox SaffronCity_Dojo_Text_HidekiPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Aaron::
	trainerbattle_single TRAINER_BLACK_BELT_AARON, SaffronCity_Dojo_Text_AaronIntro, SaffronCity_Dojo_Text_AaronDefeat
	msgbox SaffronCity_Dojo_Text_AaronPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Mike::
	trainerbattle_single TRAINER_BLACK_BELT_MIKE, SaffronCity_Dojo_Text_MikeIntro, SaffronCity_Dojo_Text_MikeDefeat
	msgbox SaffronCity_Dojo_Text_MikePostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_MasterKoichi::
	trainerbattle_single TRAINER_BLACK_BELT_KOICHI, SaffronCity_Dojo_Text_MasterKoichiIntro, SaffronCity_Dojo_Text_MasterKoichiDefeat, SaffronCity_Dojo_EventScript_DefeatedMasterKoichi
	goto_if_set FLAG_GOT_HITMON_FROM_DOJO, SaffronCity_Dojo_EventScript_MasterKoichiAlreadyGotHitmon
	msgbox SaffronCity_Dojo_Text_ChoosePrizedFightingMon, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_MasterKoichiAlreadyGotHitmon::
	msgbox SaffronCity_Dojo_Text_StayAndTrainWithUs
	release
	end

SaffronCity_Dojo_EventScript_DefeatedMasterKoichi::
	setvar VAR_MAP_SCENE_SAFFRON_CITY_DOJO, 1
	release
	end

SaffronCity_Dojo_Text_MasterKoichiIntro::
	.string "Grunt!\p"
	.string "I am the KARATE MASTER!\n"
	.string "I am the LEADER here!\p"
	.string "You wish to challenge us?\n"
	.string "Expect no mercy!\p"
	.string "Fwaaa!$"
	.string "{JPN}{JPN}おっす！\p"
	.string "わしが　カラテ　だいおう！　ここの　リーダー\n"
	.string "じゃ！\p"
	.string "わしらに　ちょうせん　するとな？　ようしゃ\n"
	.string "せんぞ！\p"
	.string "はああーっ！$"

SaffronCity_Dojo_Text_MasterKoichiDefeat::
	.string "Hwa!\n"
	.string "Arrgh! Beaten!$"
	.string "{JPN}{JPN}ぬおっ！　ぐわあっ！　まけた！$"

SaffronCity_Dojo_Text_ChoosePrizedFightingMon::
	.string "Indeed, I have lost!\p"
	.string "But, I beseech you, do not take\n"
	.string "our emblem as your trophy!\p"
	.string "In return, I will give you a prized\n"
	.string "FIGHTING POKéMON!\p"
	.string "Choose whichever one you like!$"
	.string "{JPN}{JPN}うむ　わしの　まけじゃ！\p"
	.string "だが　たのむ！　どうじょうの　かんばん　だけは\n"
	.string "もっていかんで　くれ！\p"
	.string "かわりに　だいじな　かくとうポケモンを\n"
	.string "ゆずろう！\p"
	.string "すきな　ほうを　えらぶが　よい！$"

SaffronCity_Dojo_Text_StayAndTrainWithUs::
	.string "Grunt!\n"
	.string "Stay and train at karate with us!$"
	.string "{JPN}{JPN}おっす！　わしらと　いっしょに　カラテの\n"
	.string "しゅぎょうを　していかんか！$"

SaffronCity_Dojo_Text_MikeIntro::
	.string "Hoargh!\n"
	.string "Take your shoes off!$"
	.string "{JPN}{JPN}おりゃー！　くつを　ぬげ！$"

SaffronCity_Dojo_Text_MikeDefeat::
	.string "I give up!$"
	.string "{JPN}{JPN}まいった！$"

SaffronCity_Dojo_Text_MikePostBattle::
	.string "You wait 'til you see our MASTER!\n"
	.string "I'm a small fry compared to him!$"
	.string "{JPN}{JPN}だいおうに　あったら　おどろくぞ！　おれなんか\n"
	.string "ざこ　みたいな　もんだ！$"

SaffronCity_Dojo_Text_HidekiIntro::
	.string "I hear you're good!\n"
	.string "Show me!$"
	.string "{JPN}{JPN}つよい　らしいな！　みせて　もらおう！$"

SaffronCity_Dojo_Text_HidekiDefeat::
	.string "Judge!\n"
	.string "One point!$"
	.string "{JPN}{JPN}いっぽん！　それまで！$"

SaffronCity_Dojo_Text_HidekiPostBattle::
	.string "Our MASTER is a pro fighter.\n"
	.string "Be prepared to lose!$"
	.string "{JPN}{JPN}だいおうは　かくとうの　プロだ。　まける\n"
	.string "かくごを　しておけ！$"

SaffronCity_Dojo_Text_AaronIntro::
	.string "Hiyah!\n"
	.string "I have no fear for anything tough!\p"
	.string "I shatter boulders for training\n"
	.string "every day!$"
	.string "{JPN}{JPN}ハイヤー！　かたい　ものなど　こわくない！\p"
	.string "まいにち　いわを　くだいて　しゅぎょう\n"
	.string "しているのだ！$"

SaffronCity_Dojo_Text_AaronDefeat::
	.string "Yow!\n"
	.string "Stubbed fingers!$"
	.string "{JPN}{JPN}いてっ！　つきゆび　した！$"

SaffronCity_Dojo_Text_AaronPostBattle::
	.string "The only thing that frightens us is\n"
	.string "psychic power!$"
	.string "{JPN}{JPN}おれたちが　こわいのは　エスパーの　ちから\n"
	.string "だけだ！$"

SaffronCity_Dojo_Text_HitoshiIntro::
	.string "Hwaah!\p"
	.string "You're trespassing in our\n"
	.string "FIGHTING DOJO!$"
	.string "{JPN}{JPN}ハアーッ！\p"
	.string "かくとう　どうじょうに　かってに　はいる　とは！$"

SaffronCity_Dojo_Text_HitoshiDefeat::
	.string "Oof!\n"
	.string "I give up!$"
	.string "{JPN}{JPN}うぐっ！　まいった！$"

SaffronCity_Dojo_Text_HitoshiPostBattle::
	.string "The prime fighters across the land\n"
	.string "train here.$"
	.string "{JPN}{JPN}ここでは　くにじゅうの　いちりゅうの\n"
	.string "かくとうかが　しゅぎょう　しているのだ。$"

SaffronCity_Dojo_Text_YouWantHitmonlee::
	.string "You want the hard-kicking\n"
	.string "HITMONLEE?$"
	.string "{JPN}{JPN}けりの　めいじん　サワムラーが　ほしいのか？$"

SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster::
	.string "{PLAYER} received the {STR_VAR_1}\n"
	.string "from the KARATE MASTER.$"
	.string "{JPN}{JPN}{PLAYER}は　カラテ　だいおう　から\n"
	.string "{STR_VAR_1}を　もらった！$"

SaffronCity_Dojo_Text_YouWantHitmonchan::
	.string "You want the piston-punching\n"
	.string "HITMONCHAN?$"
	.string "{JPN}{JPN}パンチの　れんだ　エビワラーが　ほしいのか？$"

SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster2::
	.string "{PLAYER}は　カラテ　だいおう　から\n"
	.string "{STR_VAR_1}を　もらった！$"
	.string "{JPN}{JPN}{PLAYER}は　カラテ　だいおう　から\n"
	.string "{STR_VAR_1}を　もらった！$"

SaffronCity_Dojo_Text_BetterNotGetGreedy::
	.string "Better not get greedy…$"
	.string "{JPN}{JPN}よくばりは　いかんぞ‥$"

SaffronCity_Dojo_Text_EnemiesOnEverySide::
	.string "Enemies on every side!$"
	.string "{JPN}{JPN}しきいを　またげば　しちにんの　てき　あり！$"

SaffronCity_Dojo_Text_GoesAroundComesAround::
	.string "What goes around comes around.$"
	.string "{JPN}{JPN}いんが　おうほう。$"

SaffronCity_Dojo_Text_FightingDojo::
	.string "FIGHTING DOJO$"
	.string "{JPN}{JPN}かくとう　どうじょう$"

