SaffronCity_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SaffronCity_OnTransition
	.byte 0

SaffronCity_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SAFFRON_CITY
	call_if_unset FLAG_RESCUED_MR_FUJI, SaffronCity_EventScript_MoveDoorGuardGrunt
	end

SaffronCity_EventScript_MoveDoorGuardGrunt::
	setobjectxyperm LOCALID_SAFFRON_DOOR_GUARD_GRUNT, 33, 31
	return

SaffronCity_EventScript_RocketGrunt1::
	msgbox SaffronCity_Text_WhatDoYouWantGetLost, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt2::
	msgbox SaffronCity_Text_BossTakeTownForTeamRocket, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt3::
	msgbox SaffronCity_Text_DontGetDefiantOrIllHurtYou, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt4::
	msgbox SaffronCity_Text_SaffronBelongsToTeamRocket, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt5::
	msgbox SaffronCity_Text_CriminalLifeMakesMeFeelAlive, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt6::
	msgbox SaffronCity_Text_WatchWhereYoureWalking, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt7::
	msgbox SaffronCity_Text_WeCanExploitMonsAroundWorld, MSGBOX_NPC
	end

SaffronCity_EventScript_DoorGuardGrunt::
	lock
	goto_if_set FLAG_RESCUED_MR_FUJI, SaffronCity_EventScript_DoorGuardAsleep
	applymovement LOCALID_SAFFRON_DOOR_GUARD_GRUNT, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SaffronCity_Text_ImASecurityGuard
	release
	end

SaffronCity_EventScript_DoorGuardAsleep::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox SaffronCity_Text_HesTakingASnooze
	release
	end

SaffronCity_EventScript_WorkerM::
	msgbox SaffronCity_Text_YouBeatTeamRocket, MSGBOX_NPC
	end

SaffronCity_EventScript_Youngster::
	msgbox SaffronCity_Text_SafeToGoOutAgain, MSGBOX_NPC
	end

SaffronCity_EventScript_Lass::
	msgbox SaffronCity_Text_PeopleComingBackToSaffron, MSGBOX_NPC
	end

SaffronCity_EventScript_Boy::
	msgbox SaffronCity_Text_FlewHereOnPidgeot, MSGBOX_NPC
	end

SaffronCity_EventScript_Pidgeot::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIDGEOT, CRY_MODE_NORMAL
	msgbox SaffronCity_Text_Pidgeot
	waitmoncry
	release
	end

SaffronCity_EventScript_Man::
	msgbox SaffronCity_Text_SawRocketBossEscaping, MSGBOX_NPC
	end

SaffronCity_EventScript_CitySign::
	msgbox SaffronCity_Text_CitySign, MSGBOX_SIGN
	end

SaffronCity_EventScript_DojoSign::
	msgbox SaffronCity_Text_FightingDojo, MSGBOX_SIGN
	end

SaffronCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_SABRINA, 0
	msgbox SaffronCity_Text_GymSign
	releaseall
	end

SaffronCity_EventScript_TrainerTips1::
	msgbox SaffronCity_Text_FullHealCuresStatus, MSGBOX_SIGN
	end

SaffronCity_EventScript_TrainerTips2::
	msgbox SaffronCity_Text_GreatBallImprovedCatchRate, MSGBOX_SIGN
	end

SaffronCity_EventScript_SilphCoSign::
	msgbox SaffronCity_Text_SilphCoSign, MSGBOX_SIGN
	end

SaffronCity_EventScript_MrPsychicsHouseSign::
	msgbox SaffronCity_Text_MrPsychicsHouse, MSGBOX_SIGN
	end

SaffronCity_EventScript_SilphProductSign::
	msgbox SaffronCity_Text_SilphsLatestProduct, MSGBOX_SIGN
	end

SaffronCity_EventScript_TrainerFanClubSign::
	msgbox SaffronCity_Text_TrainerFanClubSign, MSGBOX_SIGN
	end

SaffronCity_Text_WhatDoYouWantGetLost::
	.string "What do you want?\n"
	.string "Get lost!$"
	.string "{JPN}{JPN}なんだ？　あっちへ　いけ！$"

SaffronCity_Text_BossTakeTownForTeamRocket::
	.string "BOSS said he'll take this town\n"
	.string "in the name of TEAM ROCKET!$"
	.string "{JPN}{JPN}ボスは　ロケットだんの　なに　かけて　この\n"
	.string "まちを　のっとると　いったぞ！$"

SaffronCity_Text_DontGetDefiantOrIllHurtYou::
	.string "Don't get defiant!\n"
	.string "Or I'll have to hurt you!$"
	.string "{JPN}{JPN}さからうなよ！　さもないと　いたい　めに\n"
	.string "あうぞ！$"

SaffronCity_Text_SaffronBelongsToTeamRocket::
	.string "SAFFRON belongs to TEAM ROCKET!$"
	.string "{JPN}{JPN}ヤマブキは　ロケットだんの　ものだ！$"

SaffronCity_Text_CriminalLifeMakesMeFeelAlive::
	.string "My life as a criminal makes me feel\n"
	.string "so alive!$"
	.string "{JPN}{JPN}わるの　せいかつは　いきてるって　かんじが\n"
	.string "するぜ！$"

SaffronCity_Text_WatchWhereYoureWalking::
	.string "Ow!\n"
	.string "Watch where you're walking!$"
	.string "{JPN}{JPN}いてっ！　どこ　みて　あるいてんだ！$"

SaffronCity_Text_WeCanExploitMonsAroundWorld::
	.string "With SILPH under control, we can\n"
	.string "exploit POKéMON around the world!\p"
	.string "We'll get stinking rich, yeahah!$"
	.string "{JPN}{JPN}シルフを　のっとれば　せかいじゅうの　ポケモンを\n"
	.string "りよう　できる！\p"
	.string "おおがねもちに　なれるぜ！　イエーイ！$"

SaffronCity_Text_YouBeatTeamRocket::
	.string "You beat TEAM ROCKET all alone?\n"
	.string "That's amazing!$"
	.string "{JPN}{JPN}ひとりで　ロケットだんを　たおしたの？　すごい！$"

SaffronCity_Text_SafeToGoOutAgain::
	.string "Yeah! TEAM ROCKET is gone!\n"
	.string "It's safe to go out again!$"
	.string "{JPN}{JPN}やった！　ロケットだんが　いなくなった！　これで\n"
	.string "また　そとに　でられるわ！$"

SaffronCity_Text_PeopleComingBackToSaffron::
	.string "People fled from here in droves\n"
	.string "when those ROCKETS came.\p"
	.string "They should be flocking back to\n"
	.string "SAFFRON now.$"
	.string "{JPN}{JPN}ロケットだんが　きた　とき　おおぜいの　ひとが\n"
	.string "にげだしたんだ。\p"
	.string "これから　みんな　ヤマブキに　もどって　くる\n"
	.string "だろう。$"

SaffronCity_Text_FlewHereOnPidgeot::
	.string "I flew here on my PIDGEOT when\n"
	.string "I read about SILPH.\p"
	.string "It's already over?\n"
	.string "I missed the media action…$"
	.string "{JPN}{JPN}シルフの　きじを　よんで　ピジョットで　とんで\n"
	.string "きたんだ。\p"
	.string "もう　おわっちゃったの？　しゅざいに\n"
	.string "まにあわなかった‥$"

SaffronCity_Text_Pidgeot::
	.string "PIDGEOT: Bi bibii!$"
	.string "{JPN}{JPN}ピジョット『ピピピー！』$"

SaffronCity_Text_SawRocketBossEscaping::
	.string "I saw the ROCKET BOSS escaping\n"
	.string "SILPH's building.$"
	.string "{JPN}{JPN}ロケットだんの　ボスが　シルフの　ビルから\n"
	.string "にげていくのを　みたよ。$"

SaffronCity_Text_ImASecurityGuard::
	.string "I'm a security guard.\p"
	.string "Suspicious kids I don't allow in!$"
	.string "{JPN}{JPN}わたしは　けいびいん　だ。\p"
	.string "あやしい　こどもは　いれないぞ！$"

SaffronCity_Text_HesTakingASnooze::
	.string "…\n"
	.string "Snore…\p"
	.string "Hah! He's taking a snooze!$"
	.string "{JPN}{JPN}‥　ぐうぐう‥\p"
	.string "はっ！　いねむり　してるぞ！$"

SaffronCity_Text_CitySign::
	.string "SAFFRON CITY\n"
	.string "Shining, Golden Land of Commerce$"
	.string "{JPN}{JPN}ヤマブキシティ\n"
	.string "こがねいろに　かがやく　しょうぎょうの　まち$"

SaffronCity_Text_FightingDojo::
	.string "FIGHTING DOJO$"
	.string "{JPN}{JPN}かくとう　どうじょう$"

SaffronCity_Text_GymSign::
	.string "SAFFRON CITY POKéMON GYM\n"
	.string "LEADER: SABRINA\l"
	.string "The Master of PSYCHIC POKéMON!$"
	.string "{JPN}{JPN}ヤマブキシティ　ポケモンジム\n"
	.string "リーダー　ナツメ\l"
	.string "エスパー　ポケモンの　マスター！$"

SaffronCity_Text_FullHealCuresStatus::
	.string "TRAINER TIPS\p"
	.string "FULL HEAL cures all ailments like\n"
	.string "burns, paralysis, poisoning,\l"
	.string "freezing, and sleep.\p"
	.string "It costs a bit more, but it's more\n"
	.string "convenient than buying other items.$"
	.string "{JPN}{JPN}トレーナー　ワンポイント\p"
	.string "なんでもなおしは　やけど　まひ　どく　こおり\n"
	.string "ねむり　などの　じょうたいを　すべて　なおす！\p"
	.string "すこし　たかいけど　ほかの　どうぐを\n"
	.string "そろえるより　べんり！$"

SaffronCity_Text_GreatBallImprovedCatchRate::
	.string "TRAINER TIPS\p"
	.string "The new GREAT BALL offers improved\n"
	.string "catching rates.\p"
	.string "Try it on those hard-to-catch\n"
	.string "POKéMON.$"
	.string "{JPN}{JPN}トレーナー　ワンポイント\p"
	.string "あたらしい　スーパーボールは　つかまえやすさ\n"
	.string "アップ！\p"
	.string "なかなか　つかまらない　ポケモンに　ためして\n"
	.string "みよう！$"

SaffronCity_Text_SilphCoSign::
	.string "SILPH CO. OFFICE BUILDING$"
	.string "{JPN}{JPN}シルフカンパニー　ほんしゃビル$"

SaffronCity_Text_MrPsychicsHouse::
	.string "MR. PSYCHIC'S HOUSE$"
	.string "{JPN}{JPN}エスパーおやじの　いえ$"

SaffronCity_Text_SilphsLatestProduct::
	.string "SILPH's latest product!\n"
	.string "Release to be determined…$"
	.string "{JPN}{JPN}シルフの　さいしん　せいひん！　はつばいび\n"
	.string "みてい‥$"

SaffronCity_Text_TrainerFanClubSign::
	.string "POKéMON TRAINER FAN CLUB\p"
	.string "Many TRAINERS have scribbled their\n"
	.string "names on this sign.$"
	.string "{JPN}{JPN}ポケモントレーナー　ファンクラブ\p"
	.string "おおくの　トレーナーが　この　かんばんに\n"
	.string "なまえを　らくがき　している。$"

SaffronCity_Text_HowCanClubNotRecognizeLance::
	.string "This FAN CLUB…\n"
	.string "No one here has a clue!\p"
	.string "How could they not recognize\n"
	.string "the brilliance that is LANCE?\p"
	.string "He stands for justice!\n"
	.string "He's cool, and yet passionate!\l"
	.string "He's the greatest, LANCE!$"
	.string "{JPN}{JPN}この　ファンクラブ‥　ここの　れんちゅうは\n"
	.string "なにも　わかっちゃ　いない！\p"
	.string "ワタルの　すばらしさが　なぜ　わからないんだ？\p"
	.string "せいぎの　みかた！　クールで　いて\n"
	.string "じょうねつてき！　さいこうだよ　ワタル！$"

