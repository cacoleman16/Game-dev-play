SaffronCity_Gym_Frlg_MapScripts::
	.byte 0

SaffronCity_Gym_EventScript_Sabrina::
	famechecker FAMECHECKER_SABRINA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_SABRINA, SaffronCity_Gym_Text_SabrinaIntro, SaffronCity_Gym_Text_SabrinaDefeat, SaffronCity_Gym_EventScript_DefeatedSabrina, NO_MUSIC
	goto_if_unset FLAG_GOT_TM04_FROM_SABRINA, SaffronCity_Gym_EventScript_GiveTM04
	msgbox SaffronCity_Gym_Text_ExplainTM04
	release
	end

SaffronCity_Gym_EventScript_DefeatedSabrina::
	famechecker FAMECHECKER_SABRINA, 2
	famechecker FAMECHECKER_SABRINA, 3
	clearflag FLAG_HIDE_SAFFRON_CITY_POKECENTER_SABRINA_JOURNALS
	setflag FLAG_DEFEATED_SABRINA
	setflag FLAG_BADGE06_GET
	set_gym_trainers_frlg 6
	goto SaffronCity_Gym_EventScript_GiveTM04
	end

SaffronCity_Gym_EventScript_GiveTM04::
	msgbox SaffronCity_Gym_Text_ExplainMarshBadgeTakeThis
	checkitemspace ITEM_TM04
	goto_if_eq VAR_RESULT, FALSE, SaffronCity_Gym_EventScript_NoRoomForTM04
	giveitem_msg SaffronCity_Gym_Text_ReceivedTM04FromSabrina, ITEM_TM04
	setflag FLAG_GOT_TM04_FROM_SABRINA
	msgbox SaffronCity_Gym_Text_SabrinaPostBattle
	release
	end

SaffronCity_Gym_EventScript_NoRoomForTM04::
	msgbox SaffronCity_Gym_Text_BagFullOfOtherItems
	release
	end

SaffronCity_Gym_EventScript_Johan::
	trainerbattle_single TRAINER_PSYCHIC_JOHAN, SaffronCity_Gym_Text_JohanIntro, SaffronCity_Gym_Text_JohanDefeat
	msgbox SaffronCity_Gym_Text_JohanPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Tyron::
	trainerbattle_single TRAINER_PSYCHIC_TYRON, SaffronCity_Gym_Text_TyronIntro, SaffronCity_Gym_Text_TyronDefeat
	famechecker FAMECHECKER_SABRINA, 5
	msgbox SaffronCity_Gym_Text_TyronPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Cameron::
	trainerbattle_single TRAINER_PSYCHIC_CAMERON, SaffronCity_Gym_Text_CameronIntro, SaffronCity_Gym_Text_CameronDefeat
	msgbox SaffronCity_Gym_Text_CameronPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Preston::
	trainerbattle_single TRAINER_PSYCHIC_PRESTON, SaffronCity_Gym_Text_PrestonIntro, SaffronCity_Gym_Text_PrestonDefeat
	msgbox SaffronCity_Gym_Text_PrestonPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Amanda::
	trainerbattle_single TRAINER_CHANNELER_AMANDA, SaffronCity_Gym_Text_AmandaIntro, SaffronCity_Gym_Text_AmandaDefeat
	msgbox SaffronCity_Gym_Text_AmandaPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Stacy::
	trainerbattle_single TRAINER_CHANNELER_STACY, SaffronCity_Gym_Text_StacyIntro, SaffronCity_Gym_Text_StacyDefeat
	msgbox SaffronCity_Gym_Text_StacyPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Tasha::
	trainerbattle_single TRAINER_CHANNELER_TASHA, SaffronCity_Gym_Text_TashaIntro, SaffronCity_Gym_Text_TashaDefeat
	msgbox SaffronCity_Gym_Text_TashaPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SABRINA, SaffronCity_Gym_EventScript_GymGuyPostVictory
	msgbox SaffronCity_Gym_Text_GymGuyAdvice
	release
	end

SaffronCity_Gym_EventScript_GymGuyPostVictory::
	msgbox SaffronCity_Gym_Text_GymGuyPostVictory
	release
	end

SaffronCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, SaffronCity_Gym_EventScript_GymStatuePostVictory
	msgbox SaffronCity_Gym_Text_GymStatue
	releaseall
	end

SaffronCity_Gym_EventScript_GymStatuePostVictory::
	msgbox SaffronCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

SaffronCity_Gym_Text_SabrinaIntro::
	.string "I had a vision of your arrival.\p"
	.string "I have had psychic powers since\n"
	.string "I was a child.\p"
	.string "It started when a spoon I\n"
	.string "carelessly tossed, bent.\p"
	.string "I dislike battling, but if you wish,\n"
	.string "I will show you my powers!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"
	.string "{JPN}{JPN}あなたが　くることは　わかっていたわ。\p"
	.string "わたしは　こどもの　ころから　エスパーの\n"
	.string "ちからを　もっていたの。\p"
	.string "なにげなく　なげた　スプーンが　まがったのが\n"
	.string "はじまり　だった。\p"
	.string "たたかうのは　すきでは　ないけど　あなたが\n"
	.string "のぞむなら　わたしの　ちからを　みせて\l"
	.string "あげる！{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

SaffronCity_Gym_Text_SabrinaDefeat::
	.string "This loss shocks me!\n"
	.string "But, a loss is a loss.\p"
	.string "I admit, I didn't work hard enough\n"
	.string "to win.\p"
	.string "Your victory has earned you the\n"
	.string "MARSHBADGE.$"
	.string "{JPN}{JPN}まけるなんて‥　ショックだわ！　でも　まけは\n"
	.string "まけ。\p"
	.string "かつための　どりょくが　たりなかったと\n"
	.string "みとめるわ。\p"
	.string "あなたの　しょうりの　あかしに　ゴールドバッジを\n"
	.string "あげる。$"

SaffronCity_Gym_Text_SabrinaPostBattle::
	.string "Psychic power isn't something that\n"
	.string "only a few people have.\p"
	.string "Everyone has psychic power.\n"
	.string "People just don't realize it.$"
	.string "{JPN}{JPN}エスパーの　ちからは　いちぶの　ひとだけが　もつ\n"
	.string "ものでは　ないの。\p"
	.string "だれもが　エスパーの　ちからを　もっている。\n"
	.string "ただ　きづいて　いない　だけ。$"

SaffronCity_Gym_Text_ExplainMarshBadgeTakeThis::
	.string "The MARSHBADGE makes POKéMON up\n"
	.string "to Lv. 70 obey you.\p"
	.string "Stronger POKéMON will become wild,\n"
	.string "ignoring your orders in battle.\p"
	.string "Just don't raise your POKéMON too\n"
	.string "much to avoid that problem.\p"
	.string "Wait, please take this TM with you.$"
	.string "{JPN}{JPN}ゴールドバッジが　あれば　レベル70　までの\n"
	.string "ポケモンが　いうことを　きくわ。\p"
	.string "それより　つよい　ポケモンは　バトルで\n"
	.string "めいれいを　きかなく　なるの。\p"
	.string "そう　ならないよう　ポケモンを　そだてすぎない\n"
	.string "ことね。\p"
	.string "まって　この　わざマシンを　もっていって。$"

SaffronCity_Gym_Text_ReceivedTM04FromSabrina::
	.string "{PLAYER} received TM04 from\n"
	.string "SABRINA.$"
	.string "{JPN}{JPN}{PLAYER}は　ナツメから　わざマシン04を\n"
	.string "もらった！$"

SaffronCity_Gym_Text_ExplainTM04::
	.string "TM04 is CALM MIND.\p"
	.string "It makes the POKéMON concentrate\n"
	.string "to raise both SP. ATK and SP. DEF.$"
	.string "{JPN}{JPN}わざマシン04は　めいそう。\p"
	.string "こころを　しずめて　とくこうと　とくぼうを\n"
	.string "あげる　わざよ。$"

SaffronCity_Gym_Text_BagFullOfOtherItems::
	.string "Your BAG is full of other items.$"
	.string "{JPN}{JPN}バッグが　ほかの　どうぐで　いっぱいね。$"

SaffronCity_Gym_Text_AmandaIntro::
	.string "SABRINA is far younger than I,\n"
	.string "but she has earned my respect.$"
	.string "{JPN}{JPN}ナツメさまは　わたしより　ずっと　わかいけど\n"
	.string "そんけい　しているわ。$"

SaffronCity_Gym_Text_AmandaDefeat::
	.string "Not good enough!$"
	.string "{JPN}{JPN}まだまだ　だったわ！$"

SaffronCity_Gym_Text_AmandaPostBattle::
	.string "In a battle of equals, the one\n"
	.string "with the stronger will wins.\p"
	.string "If you wish to beat SABRINA, focus\n"
	.string "and will yourself to victory.$"
	.string "{JPN}{JPN}じつりょくが　おなじなら　いしの　つよい　ほうが\n"
	.string "かつの。\p"
	.string "ナツメさまに　かちたいなら　しゅうちゅうして\n"
	.string "かつと　ねんじなさい。$"

SaffronCity_Gym_Text_JohanIntro::
	.string "Does our unseen power scare you?$"
	.string "{JPN}{JPN}みえない　ちからが　こわいかい？$"

SaffronCity_Gym_Text_JohanDefeat::
	.string "I never foresaw this!$"
	.string "{JPN}{JPN}これは　よち　できなかった！$"

SaffronCity_Gym_Text_JohanPostBattle::
	.string "PSYCHIC POKéMON fear only ghosts\n"
	.string "and bugs!$"
	.string "{JPN}{JPN}エスパーポケモンが　おそれるのは　ゴーストと\n"
	.string "むし　だけだ！$"

SaffronCity_Gym_Text_StacyIntro::
	.string "POKéMON take on the appearance of\n"
	.string "their TRAINERS.\p"
	.string "Your POKéMON must be tough, then!$"
	.string "{JPN}{JPN}ポケモンは　トレーナーに　にてくる　ものよ。\p"
	.string "あなたの　ポケモンは　きっと　たくましい　のね！$"

SaffronCity_Gym_Text_StacyDefeat::
	.string "I knew it!$"
	.string "{JPN}{JPN}やっぱりね！$"

SaffronCity_Gym_Text_StacyPostBattle::
	.string "I have much to learn still…\p"
	.string "I must master PSYCHIC and teach it\n"
	.string "to my POKéMON…$"
	.string "{JPN}{JPN}まだまだ　まなぶ　ことが　たくさん　ある‥\p"
	.string "サイコキネシスを　きわめて　ポケモンに\n"
	.string "おしえなくちゃ‥$"

SaffronCity_Gym_Text_TyronIntro::
	.string "You understand, don't you?\p"
	.string "That power alone isn't enough to\n"
	.string "win in the world of POKéMON?$"
	.string "{JPN}{JPN}わかるだろう？\p"
	.string "ポケモンの　せかいでは　ちから　だけでは\n"
	.string "かてない　ことが？$"

SaffronCity_Gym_Text_TyronDefeat::
	.string "I don't believe this!$"
	.string "{JPN}{JPN}しんじられない！$"

SaffronCity_Gym_Text_TyronPostBattle::
	.string "SABRINA just wiped out the KARATE\n"
	.string "MASTER next door.$"
	.string "{JPN}{JPN}ナツメさまは　となりの　カラテ　だいおうを\n"
	.string "こてんぱんに　したんだ。$"

SaffronCity_Gym_Text_TashaIntro::
	.string "You and I, our POKéMON shall\n"
	.string "battle!$"
	.string "{JPN}{JPN}わたしと　あなた　ポケモンで　しょうぶよ！$"

SaffronCity_Gym_Text_TashaDefeat::
	.string "I lost after all!$"
	.string "{JPN}{JPN}やっぱり　まけちゃった！$"

SaffronCity_Gym_Text_TashaPostBattle::
	.string "I knew that this was going to take\n"
	.string "place.$"
	.string "{JPN}{JPN}こう　なることは　わかっていたわ。$"

SaffronCity_Gym_Text_CameronIntro::
	.string "SABRINA is young, but she's also\n"
	.string "our highly skilled LEADER.\p"
	.string "You won't reach her easily!$"
	.string "{JPN}{JPN}ナツメさまは　わかいが　うでの　たつ\n"
	.string "リーダーだ。\p"
	.string "かんたんには　たどりつけないぞ！$"

SaffronCity_Gym_Text_CameronDefeat::
	.string "Ow!\n"
	.string "Down and out!$"
	.string "{JPN}{JPN}うわっ！　ノックアウトだ！$"

SaffronCity_Gym_Text_CameronPostBattle::
	.string "There used to be two POKéMON GYMS\n"
	.string "in SAFFRON.\p"
	.string "The FIGHTING DOJO next door lost\n"
	.string "its GYM status, though.\p"
	.string "We simply creamed them when we\n"
	.string "decided to have just one GYM.$"
	.string "{JPN}{JPN}むかし　ヤマブキには　ポケモンジムが　ふたつ\n"
	.string "あったんだ。\p"
	.string "でも　となりの　かくとう　どうじょうは　ジムの\n"
	.string "しかくを　うしなった。\p"
	.string "ジムを　ひとつに　する　ことに　なった　とき\n"
	.string "われわれが　こてんぱんに　したのさ。$"

SaffronCity_Gym_Text_PrestonIntro::
	.string "SAFFRON POKéMON GYM is famous for\n"
	.string "training psychics.\p"
	.string "You want to see SABRINA, don't\n"
	.string "you? I can tell!$"
	.string "{JPN}{JPN}ヤマブキ　ポケモンジムは　エスパーの\n"
	.string "しゅぎょうで　ゆうめいだ。\p"
	.string "ナツメさまに　あいたいんだろう？　わかるぞ！$"

SaffronCity_Gym_Text_PrestonDefeat::
	.string "Arrrgh!$"
	.string "{JPN}{JPN}ぐわーっ！$"

SaffronCity_Gym_Text_PrestonPostBattle::
	.string "That's right! I used telepathy to\n"
	.string "read your mind!$"
	.string "{JPN}{JPN}そのとおり！　テレパシーで　きみの　こころを\n"
	.string "よんだのさ！$"

SaffronCity_Gym_Text_GymGuyAdvice::
	.string "Yo!\n"
	.string "Champ in the making!\p"
	.string "SABRINA's POKéMON use psychic\n"
	.string "power instead of force.\p"
	.string "FIGHTING POKéMON are especially\n"
	.string "weak against PSYCHIC POKéMON.\p"
	.string "They'll get destroyed before they\n"
	.string "can even aim a punch!$"
	.string "{JPN}{JPN}おーす！　みらいの　チャンピオン！\p"
	.string "ナツメの　ポケモンは　ちからでは　なく\n"
	.string "エスパーの　ちからで　たたかう。\p"
	.string "かくとうポケモンは　エスパーポケモンに　とくに\n"
	.string "よわいんだ。\p"
	.string "パンチを　くりだす　まえに　やられちまうぞ！$"

SaffronCity_Gym_Text_GymGuyPostVictory::
	.string "Psychic power, huh?\p"
	.string "If I had that, I'd make a bundle\n"
	.string "at the slots!$"
	.string "{JPN}{JPN}エスパーの　ちから　かあ‥\p"
	.string "おれに　あったら　スロットで　おおもうけ\n"
	.string "するのになあ！$"

SaffronCity_Gym_Text_GymStatue::
	.string "SAFFRON POKéMON GYM\n"
	.string "LEADER: SABRINA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"
	.string "{JPN}{JPN}ヤマブキ　ポケモンジム\n"
	.string "リーダー　ナツメ\p"
	.string "かったひと:\n"
	.string "{RIVAL}$"

SaffronCity_Gym_Text_GymStatuePlayerWon::
	.string "SAFFRON POKéMON GYM\n"
	.string "LEADER: SABRINA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"
	.string "{JPN}{JPN}ヤマブキ　ポケモンジム\n"
	.string "リーダー　ナツメ\p"
	.string "かったひと:\n"
	.string "{RIVAL}　{PLAYER}$"

