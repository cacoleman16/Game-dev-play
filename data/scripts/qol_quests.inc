@ [QoL] Side quests.
@
@ FIELD RESEARCH: a researcher in PROF. OAK's lab (map object at 1,6) hands out
@ tasks about POKéMON from faraway regions, one at a time, with a reward for
@ each (VAR_QOL_FIELD_RESEARCH, QOL_RESEARCH_* in include/constants/vars.h,
@ progress from src/qol_quests.c). After the CHAMPION fight, the research
@ reveals the GALARIAN legendary birds:
@   GALARIAN ARTICUNO  SEAFOAM ISLANDS B4F, near ARTICUNO's spot
@   GALARIAN ZAPDOS    POWER PLANT, near ZAPDOS's spot
@   GALARIAN MOLTRES   MT. EMBER summit, near MOLTRES's spot
@ A bird that isn't caught stays where it is, so the player can try again.

PalletTown_ProfessorOaksLab_EventScript_FieldResearcher::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_POKEDEX_GET, QoL_EventScript_ResearcherBusy
	goto_if_ge VAR_QOL_FIELD_RESEARCH, QOL_RESEARCH_DONE, QoL_EventScript_ResearchAllDone
	special QoL_Research_Status
	goto_if_eq VAR_RESULT, TRUE, QoL_EventScript_ResearchTaskDone
	call QoL_EventScript_DescribeTask
	release
	end

QoL_EventScript_ResearcherBusy::
	msgbox QoL_Text_ResearcherBusy
	release
	end

QoL_EventScript_ResearchAllDone::
	msgbox QoL_Text_ResearchAllDone
	release
	end

@ Explain the current task and how far along the player is.
QoL_EventScript_DescribeTask::
	switch VAR_QOL_FIELD_RESEARCH
	case QOL_RESEARCH_SHOW_FOREIGN, QoL_EventScript_DescribeShowForeign
	case QOL_RESEARCH_CATCH_10_FOREIGN, QoL_EventScript_DescribeCatch10
	case QOL_RESEARCH_WIN_50_BATTLES, QoL_EventScript_DescribeWin50
	case QOL_RESEARCH_HATCH_EGG, QoL_EventScript_DescribeHatchEgg
	case QOL_RESEARCH_CATCH_25_FOREIGN, QoL_EventScript_DescribeCatch25
	case QOL_RESEARCH_CATCH_100_KANTO, QoL_EventScript_DescribeCatch100Kanto
	case QOL_RESEARCH_GALAR_LEAD, QoL_EventScript_DescribeGalarLead
	case QOL_RESEARCH_CATCH_GALAR_BIRDS, QoL_EventScript_DescribeGalarBirds
	return

QoL_EventScript_DescribeShowForeign::
	msgbox QoL_Text_ResearchIntro
	msgbox QoL_Text_TaskShowForeign
	return

QoL_EventScript_DescribeCatch10::
	msgbox QoL_Text_TaskCatch10
	goto QoL_EventScript_ShowProgress

QoL_EventScript_DescribeWin50::
	msgbox QoL_Text_TaskWin50
	goto QoL_EventScript_ShowProgress

QoL_EventScript_DescribeHatchEgg::
	msgbox QoL_Text_TaskHatchEgg
	return

QoL_EventScript_DescribeCatch25::
	msgbox QoL_Text_TaskCatch25
	goto QoL_EventScript_ShowProgress

QoL_EventScript_DescribeCatch100Kanto::
	msgbox QoL_Text_TaskCatch100Kanto
	goto QoL_EventScript_ShowProgress

QoL_EventScript_DescribeGalarLead::
	msgbox QoL_Text_TaskGalarLead
	return

QoL_EventScript_DescribeGalarBirds::
	msgbox QoL_Text_TaskGalarBirds
	goto QoL_EventScript_ShowProgress

QoL_EventScript_ShowProgress::
	buffernumberstring STR_VAR_1, VAR_0x8005
	buffernumberstring STR_VAR_2, VAR_0x8006
	msgbox QoL_Text_ResearchProgress
	return

@ The current task is done: reward, then the next task.
QoL_EventScript_ResearchTaskDone::
	goto_if_eq VAR_QOL_FIELD_RESEARCH, QOL_RESEARCH_GALAR_LEAD, QoL_EventScript_RevealGalarBirds
	msgbox QoL_Text_ResearchTaskDone
	switch VAR_QOL_FIELD_RESEARCH
	case QOL_RESEARCH_SHOW_FOREIGN, QoL_EventScript_RewardAbilityCapsule
	case QOL_RESEARCH_CATCH_10_FOREIGN, QoL_EventScript_RewardLuckyEgg
	case QOL_RESEARCH_WIN_50_BATTLES, QoL_EventScript_RewardRareCandies
	case QOL_RESEARCH_HATCH_EGG, QoL_EventScript_RewardOvalCharm
	case QOL_RESEARCH_CATCH_25_FOREIGN, QoL_EventScript_RewardGoldBottleCap
	case QOL_RESEARCH_CATCH_100_KANTO, QoL_EventScript_RewardShinyCharm
	case QOL_RESEARCH_CATCH_GALAR_BIRDS, QoL_EventScript_RewardMasterBall
	release
	end

QoL_EventScript_RewardAbilityCapsule::
	giveitem ITEM_ABILITY_CAPSULE
	goto QoL_EventScript_AfterReward

QoL_EventScript_RewardLuckyEgg::
	giveitem ITEM_LUCKY_EGG
	goto QoL_EventScript_AfterReward

QoL_EventScript_RewardRareCandies::
	giveitem ITEM_RARE_CANDY, 3
	goto QoL_EventScript_AfterReward

QoL_EventScript_RewardOvalCharm::
	giveitem ITEM_OVAL_CHARM
	goto QoL_EventScript_AfterReward

QoL_EventScript_RewardGoldBottleCap::
	giveitem ITEM_GOLD_BOTTLE_CAP
	goto QoL_EventScript_AfterReward

QoL_EventScript_RewardShinyCharm::
	giveitem ITEM_SHINY_CHARM
	goto QoL_EventScript_AfterReward

QoL_EventScript_RewardMasterBall::
	giveitem ITEM_MASTER_BALL
	goto_if_eq VAR_RESULT, FALSE, QoL_EventScript_BagFull
	giveitem ITEM_ABILITY_PATCH
	goto QoL_EventScript_AfterReward

@ giveitem leaves VAR_RESULT FALSE when the bag is full: keep the task open.
QoL_EventScript_AfterReward::
	goto_if_eq VAR_RESULT, FALSE, QoL_EventScript_BagFull
	addvar VAR_QOL_FIELD_RESEARCH, 1
	goto_if_ge VAR_QOL_FIELD_RESEARCH, QOL_RESEARCH_DONE, QoL_EventScript_ResearchAllDone
	msgbox QoL_Text_ResearchNextTask
	special QoL_Research_Status
	call QoL_EventScript_DescribeTask
	release
	end

QoL_EventScript_BagFull::
	msgbox QoL_Text_ResearchBagFull
	release
	end

QoL_EventScript_RevealGalarBirds::
	msgbox QoL_Text_GalarBirdsRevealed
	clearflag FLAG_QOL_HIDE_GALAR_ARTICUNO
	clearflag FLAG_QOL_HIDE_GALAR_ZAPDOS
	clearflag FLAG_QOL_HIDE_GALAR_MOLTRES
	setvar VAR_QOL_FIELD_RESEARCH, QOL_RESEARCH_CATCH_GALAR_BIRDS
	release
	end

@ ------------------------------------------------------------ Galarian birds

SeafoamIslands_B4F_EventScript_GalarianArticuno::
	lock
	faceplayer
	setwildbattle SPECIES_ARTICUNO_GALAR, 70
	setvar VAR_0x8004, SPECIES_ARTICUNO_GALAR
	call QoL_EventScript_GalarBirdBattle
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, QoL_EventScript_GalarBirdGotAway
	setflag FLAG_QOL_CAUGHT_GALAR_ARTICUNO
	goto EventScript_RemoveStaticMon
	end

PowerPlant_EventScript_GalarianZapdos::
	lock
	faceplayer
	setwildbattle SPECIES_ZAPDOS_GALAR, 70
	setvar VAR_0x8004, SPECIES_ZAPDOS_GALAR
	call QoL_EventScript_GalarBirdBattle
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, QoL_EventScript_GalarBirdGotAway
	setflag FLAG_QOL_CAUGHT_GALAR_ZAPDOS
	goto EventScript_RemoveStaticMon
	end

MtEmber_Summit_EventScript_GalarianMoltres::
	lock
	faceplayer
	setwildbattle SPECIES_MOLTRES_GALAR, 70
	setvar VAR_0x8004, SPECIES_MOLTRES_GALAR
	call QoL_EventScript_GalarBirdBattle
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, QoL_EventScript_GalarBirdGotAway
	setflag FLAG_QOL_CAUGHT_GALAR_MOLTRES
	goto EventScript_RemoveStaticMon
	end

@ Cry, battle, and VAR_RESULT = the battle outcome. VAR_0x8004 = species.
QoL_EventScript_GalarBirdBattle::
	waitse
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	message Text_Gyaoo
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	return

@ Not caught (fainted or the player ran): it stays put for another try.
QoL_EventScript_GalarBirdGotAway::
	bufferspeciesname STR_VAR_1, VAR_0x8004
	msgbox QoL_Text_GalarBirdStillHere
	release
	end

@ ---------------------------------------------------------------------- text

QoL_Text_ResearcherBusy::
	.string "I'm one of PROF. OAK's researchers.\n"
	.string "I study POKéMON from far away.\p"
	.string "Once you have a POKéDEX, I could\n"
	.string "use your help with some research!$"
	.string "{JPN}{JPN}わたしは　オーキドはかせの　けんきゅういん。\p"
	.string "ポケモンずかんを　もらったら　けんきゅうを\n"
	.string "てつだってほしいの！$"

QoL_Text_ResearchIntro::
	.string "Hi! I'm doing FIELD RESEARCH for\n"
	.string "PROF. OAK.\p"
	.string "POKéMON from faraway regions, the\n"
	.string "ones after No. 386, have shown up\l"
	.string "all over KANTO lately.\p"
	.string "Help me study them, and I'll\n"
	.string "reward you with rare items!$"
	.string "{JPN}{JPN}こんにちは！　わたしは　オーキドはかせの\n"
	.string "フィールドリサーチを　しているの。\p"
	.string "さいきん　とおい　ちほうの　ポケモン‥　ずかん\n"
	.string "387ばんより　あとの　ポケモンが　カントーの\l"
	.string "あちこちに　あらわれているの。\p"
	.string "しらべるのを　てつだってくれたら　めずらしい\n"
	.string "どうぐを　あげるわ！$"

QoL_Text_TaskShowForeign::
	.string "First task: show me a POKéMON from\n"
	.string "a faraway region. Bring it in your\l"
	.string "party, and talk to me again!$"
	.string "{JPN}{JPN}さいしょの　おねがい！　とおい　ちほうの\n"
	.string "ポケモンを　てもちに　いれて　みせにきてね！$"

QoL_Text_TaskCatch10::
	.string "Next task: catch 10 different\n"
	.string "kinds of POKéMON from faraway\l"
	.string "regions.$"
	.string "{JPN}{JPN}つぎの　おねがい！　とおい　ちほうの　ポケモンを\n"
	.string "10しゅるい　つかまえてきて！$"

QoL_Text_TaskWin50::
	.string "Next task: win 50 battles against\n"
	.string "TRAINERS. I want to see how these\l"
	.string "POKéMON fight!$"
	.string "{JPN}{JPN}つぎの　おねがい！　トレーナーとの　しょうぶに\n"
	.string "50かい　かってきて！　どんな　たたかいかたを\l"
	.string "するのか　みたいの！$"

QoL_Text_TaskHatchEgg::
	.string "Next task: hatch a POKéMON EGG.\n"
	.string "The DAY CARE can help with that.$"
	.string "{JPN}{JPN}つぎの　おねがい！　ポケモンの　タマゴを\n"
	.string "かえしてきて！　そだてやさんが\l"
	.string "たすけてくれるわ。$"

QoL_Text_TaskCatch25::
	.string "Next task: catch 25 different\n"
	.string "kinds of POKéMON from faraway\l"
	.string "regions.$"
	.string "{JPN}{JPN}つぎの　おねがい！　とおい　ちほうの　ポケモンを\n"
	.string "25しゅるい　つかまえてきて！$"

QoL_Text_TaskCatch100Kanto::
	.string "Next task: catch 100 different\n"
	.string "kinds of KANTO POKéMON, No. 1 to\l"
	.string "No. 151, for comparison.$"
	.string "{JPN}{JPN}つぎの　おねがい！　くらべるために　カントーの\n"
	.string "ポケモン‥　1ばんから　151ばんまでを\l"
	.string "100しゅるい　つかまえてきて！$"

QoL_Text_TaskGalarLead::
	.string "Last lead: reports of strange\n"
	.string "birds that look like ARTICUNO,\l"
	.string "ZAPDOS and MOLTRES.\p"
	.string "They're too strong to chase\n"
	.string "until you're the CHAMPION.\l"
	.string "Come back after that!$"
	.string "{JPN}{JPN}さいごの　てがかり！　フリーザー　サンダー\n"
	.string "ファイヤーに　にた　ふしぎな　とりの　うわさが\l"
	.string "あるの。\p"
	.string "でも　とても　つよいから　チャンピオンに\n"
	.string "なってから　またきてね！$"

QoL_Text_TaskGalarBirds::
	.string "Find the GALARIAN forms of\n"
	.string "ARTICUNO, ZAPDOS and MOLTRES.\p"
	.string "They're near where the KANTO\n"
	.string "birds live: SEAFOAM ISLANDS, the\l"
	.string "POWER PLANT and MT. EMBER.$"
	.string "{JPN}{JPN}ガラルの　すがたの　フリーザー　サンダー\n"
	.string "ファイヤーを　さがして！\p"
	.string "カントーの　とりたちが　すむ　ふたごじま\n"
	.string "むじんはつでんしょ　ともしびやまの　ちかくに\l"
	.string "いるわ！$"

QoL_Text_ResearchProgress::
	.string "So far: {STR_VAR_1} of {STR_VAR_2}.$"
	.string "{JPN}{JPN}いまは　{STR_VAR_1}　/\n"
	.string "{STR_VAR_2}　よ！$"

QoL_Text_ResearchTaskDone::
	.string "Oh, that's wonderful! Task\n"
	.string "complete! Here's your reward.$"
	.string "{JPN}{JPN}まあ　すごい！　おねがい　たっせい！　おれいを\n"
	.string "どうぞ！$"

QoL_Text_ResearchNextTask::
	.string "There's more to find out!$"
	.string "{JPN}{JPN}まだまだ しらべることが あるわ！$"

QoL_Text_ResearchBagFull::
	.string "Your BAG is full. Make some room,\n"
	.string "then come back for your reward.$"
	.string "{JPN}{JPN}バッグが　いっぱいね。　あきを　つくってから\n"
	.string "おれいを　とりにきて！$"

QoL_Text_GalarBirdsRevealed::
	.string "The CHAMPION! Then you can handle\n"
	.string "this.\p"
	.string "The strange birds are GALARIAN\n"
	.string "ARTICUNO, ZAPDOS and MOLTRES!\p"
	.string "They've settled near the KANTO\n"
	.string "birds' homes: SEAFOAM ISLANDS,\l"
	.string "the POWER PLANT and MT. EMBER.\p"
	.string "Catch all three, and I'll give\n"
	.string "you my best reward!$"
	.string "{JPN}{JPN}チャンピオン！　それなら　だいじょうぶね。\p"
	.string "ふしぎな　とりの　しょうたいは　ガラルの\n"
	.string "すがたの　フリーザー　サンダー　ファイヤーよ！\p"
	.string "カントーの　とりたちの　すみか‥　ふたごじま\n"
	.string "むじんはつでんしょ　ともしびやまの　ちかくに\l"
	.string "いるわ！\p"
	.string "3びき　ぜんぶ　つかまえたら　とっておきの\n"
	.string "おれいを　あげる！$"

QoL_Text_ResearchAllDone::
	.string "Thanks to you, my research is\n"
	.string "complete! PROF. OAK is thrilled.\p"
	.string "You're a true POKéMON expert!$"
	.string "{JPN}{JPN}あなたの　おかげで　わたしの　けんきゅうは\n"
	.string "かんせいよ！　オーキドはかせも　おおよろこび！\p"
	.string "あなたは　ほんものの　ポケモン　はかせね！$"

QoL_Text_GalarBirdStillHere::
	.string "{STR_VAR_1} is still watching you.\n"
	.string "You can try again!$"
	.string "{JPN}{JPN}{STR_VAR_1}は　まだ　こちらを\n"
	.string "みている‥　もういちど　ちょうせん　できる！$"
