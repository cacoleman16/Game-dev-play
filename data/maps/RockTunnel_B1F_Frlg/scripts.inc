RockTunnel_B1F_Frlg_MapScripts::
	.byte 0

RockTunnel_B1F_EventScript_Sofia::
	trainerbattle_single TRAINER_PICNICKER_SOFIA, RockTunnel_B1F_Text_SofiaIntro, RockTunnel_B1F_Text_SofiaDefeat
	msgbox RockTunnel_B1F_Text_SofiaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Martha::
	trainerbattle_single TRAINER_PICNICKER_MARTHA, RockTunnel_B1F_Text_MarthaIntro, RockTunnel_B1F_Text_MarthaDefeat
	msgbox RockTunnel_B1F_Text_MarthaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Dudley::
	trainerbattle_single TRAINER_HIKER_DUDLEY, RockTunnel_B1F_Text_DudleyIntro, RockTunnel_B1F_Text_DudleyDefeat
	msgbox RockTunnel_B1F_Text_DudleyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Allen::
	trainerbattle_single TRAINER_HIKER_ALLEN, RockTunnel_B1F_Text_AllenIntro, RockTunnel_B1F_Text_AllenDefeat
	msgbox RockTunnel_B1F_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Eric::
	trainerbattle_single TRAINER_HIKER_ERIC, RockTunnel_B1F_Text_EricIntro, RockTunnel_B1F_Text_EricDefeat
	msgbox RockTunnel_B1F_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Cooper::
	trainerbattle_single TRAINER_POKEMANIAC_COOPER, RockTunnel_B1F_Text_CooperIntro, RockTunnel_B1F_Text_CooperDefeat
	msgbox RockTunnel_B1F_Text_CooperPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Steve::
	trainerbattle_single TRAINER_POKEMANIAC_STEVE, RockTunnel_B1F_Text_SteveIntro, RockTunnel_B1F_Text_SteveDefeat
	msgbox RockTunnel_B1F_Text_StevePostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Winston::
	trainerbattle_single TRAINER_POKEMANIAC_WINSTON, RockTunnel_B1F_Text_WinstonIntro, RockTunnel_B1F_Text_WinstonDefeat
	msgbox RockTunnel_B1F_Text_WinstonPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_Text_SofiaIntro::
	.string "Do you know how you can avoid\n"
	.string "getting lost in the mountains?\p"
	.string "You can bend twigs as trail\n"
	.string "markers.$"
	.string "{JPN}{JPN}やまで　まよわない　ほうほうを　しってる？\p"
	.string "えだを　おって　みちしるべに　するのよ。$"

RockTunnel_B1F_Text_SofiaDefeat::
	.string "Ohhh!\n"
	.string "I did my best!$"
	.string "{JPN}{JPN}ああっ！　せいいっぱい　やったのに！$"

RockTunnel_B1F_Text_SofiaPostBattle::
	.string "I want to go home!$"
	.string "{JPN}{JPN}もう　おうちに　かえりたい！$"

RockTunnel_B1F_Text_DudleyIntro::
	.string "Hahaha!\n"
	.string "Can you beat my power?$"
	.string "{JPN}{JPN}はっはっは！　おれの　パワーに　かてるかな？$"

RockTunnel_B1F_Text_DudleyDefeat::
	.string "Oops!\n"
	.string "Outmuscled!$"
	.string "{JPN}{JPN}おっと！　ちからまけだ！$"

RockTunnel_B1F_Text_DudleyPostBattle::
	.string "I go for power because I hate\n"
	.string "thinking.$"
	.string "{JPN}{JPN}かんがえるのは　きらいだから　パワー　いっぽんで\n"
	.string "いくのさ。$"

RockTunnel_B1F_Text_CooperIntro::
	.string "You have a POKéDEX?\n"
	.string "I want one, too.$"
	.string "{JPN}{JPN}ポケモンずかん　もってるの？　ぼくも\n"
	.string "ほしいなあ。$"

RockTunnel_B1F_Text_CooperDefeat::
	.string "Shoot!\n"
	.string "I am so jealous!$"
	.string "{JPN}{JPN}ちぇっ！　うらやましいなあ！$"

RockTunnel_B1F_Text_CooperPostBattle::
	.string "When you finish your POKéDEX,\n"
	.string "can I have it?$"
	.string "{JPN}{JPN}ずかんが　かんせい　したら　ぼくに　くれない？$"

RockTunnel_B1F_Text_SteveIntro::
	.string "Um… Do you know about POKéMON\n"
	.string "cosplay?$"
	.string "{JPN}{JPN}あの‥　ポケモンの　コスプレって　しってる？$"

RockTunnel_B1F_Text_SteveDefeat::
	.string "Well, that's that.$"
	.string "{JPN}{JPN}まあ　しかたないか。$"

RockTunnel_B1F_Text_StevePostBattle::
	.string "POKéMON cosplay is dressing up as\n"
	.string "POKéMON for fun.\p"
	.string "CLEFAIRY is a favorite.$"
	.string "{JPN}{JPN}ポケモンの　コスプレって　いうのは　ポケモンの\n"
	.string "かっこうを　して　たのしむ　ことさ。\p"
	.string "ピッピが　いちばん　にんきだよ。$"

RockTunnel_B1F_Text_AllenIntro::
	.string "My POKéMON techniques will leave\n"
	.string "you crying!$"
	.string "{JPN}{JPN}おれの　ポケモンの　テクニックで　なかせて\n"
	.string "やるぜ！$"

RockTunnel_B1F_Text_AllenDefeat::
	.string "I give!\n"
	.string "You're a better technician!$"
	.string "{JPN}{JPN}まいった！　きみの　ほうが　うわてだ！$"

RockTunnel_B1F_Text_AllenPostBattle::
	.string "In mountains, you'll often find\n"
	.string "ROCK-type POKéMON.$"
	.string "{JPN}{JPN}やまには　いわタイプの　ポケモンが　おおいぞ。$"

RockTunnel_B1F_Text_MarthaIntro::
	.string "I don't often come here, but I will\n"
	.string "battle you.$"
	.string "{JPN}{JPN}ここへは　あまり　こないけど　しょうぶ\n"
	.string "してあげるわ。$"

RockTunnel_B1F_Text_MarthaDefeat::
	.string "Oh!\n"
	.string "I lost!$"
	.string "{JPN}{JPN}あら！　まけちゃった！$"

RockTunnel_B1F_Text_MarthaPostBattle::
	.string "I like tiny POKéMON.\n"
	.string "Big ones are too scary!$"
	.string "{JPN}{JPN}ちいさい　ポケモンが　すきなの。　おおきいのは\n"
	.string "こわすぎるわ！$"

RockTunnel_B1F_Text_EricIntro::
	.string "Hit me with your best shot!$"
	.string "{JPN}{JPN}おまえの　ぜんりょくで　かかって　こい！$"

RockTunnel_B1F_Text_EricDefeat::
	.string "Fired away!$"
	.string "{JPN}{JPN}うちつくした！$"

RockTunnel_B1F_Text_EricPostBattle::
	.string "I'll raise my POKéMON to beat\n"
	.string "yours, kid.$"
	.string "{JPN}{JPN}ポケモンを　きたえて　おまえに　かってやるぞ。$"

RockTunnel_B1F_Text_WinstonIntro::
	.string "I draw illustrations of POKéMON\n"
	.string "when I'm home.$"
	.string "{JPN}{JPN}いえでは　ポケモンの　イラストを　かいて\n"
	.string "いるんだ。$"

RockTunnel_B1F_Text_WinstonDefeat::
	.string "Whew…\n"
	.string "I'm exhausted…$"
	.string "{JPN}{JPN}ふう‥　つかれた‥$"

RockTunnel_B1F_Text_WinstonPostBattle::
	.string "I'm an artist, not a fighter.\n"
	.string "I'll go home to draw.$"
	.string "{JPN}{JPN}ぼくは　がかで　たたかう　ひとじゃ　ない。\n"
	.string "かえって　えを　かこう。$"

