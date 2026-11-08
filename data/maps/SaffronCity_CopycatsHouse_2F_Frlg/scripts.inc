SaffronCity_CopycatsHouse_2F_Frlg_MapScripts::
	.byte 0

SaffronCity_CopycatsHouse_2F_EventScript_Doduo::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_DODUO, CRY_MODE_NORMAL
	msgbox SaffronCity_CopycatsHouse_2F_Text_Doduo
	waitmoncry
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_Doll::
	lock
	msgbox SaffronCity_CopycatsHouse_2F_Text_RareMonOnlyDoll
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_Copycat::
	lock
	faceplayer
	checkitem ITEM_POKE_DOLL
	goto_if_eq VAR_RESULT, TRUE, EventScript_MimicTutor
	checkplayergender
	call_if_eq VAR_RESULT, MALE, SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerMale
	call_if_eq VAR_RESULT, FEMALE, SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerFemale
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerMale::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyMale
	return

SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerFemale::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyFemale
	return

SaffronCity_CopycatsHouse_2F_EventScript_Computer::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MySecrets, MSGBOX_SIGN
	end

SaffronCity_CopycatsHouse_2F_EventScript_Game::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MarioWearingABucket, MSGBOX_SIGN
	end

SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyMale::
	.string "{PLAYER}: Hi!\n"
	.string "Do you like POKéMON?\p"
	.string "{PLAYER}: Uh, no, I just asked you.\p"
	.string "{PLAYER}: Huh?\n"
	.string "You're strange!\p"
	.string "COPYCAT: Hmm? Quit mimicking?\n"
	.string "But, that's my favorite hobby!$"
	.string "{JPN}{JPN}{PLAYER}『やあ！　ポケモン　すき？』\p"
	.string "{PLAYER}『いや　ぼくが　きいてるん\n"
	.string "だけど。』\p"
	.string "{PLAYER}『えっ？　へんな　こ　だなあ！』\p"
	.string "ものまねむすめ『ん？　まねを　やめろって？\n"
	.string "だって　ものまねが　いちばんの　しゅみ\l"
	.string "なんだもん！』$"

SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyFemale::
	.string "{PLAYER}: Hi.\n"
	.string "You must like POKéMON.\p"
	.string "{PLAYER}: No, not me.\n"
	.string "I asked you.\p"
	.string "{PLAYER}: Pardon?\n"
	.string "You're weird!\l"
	.string " \l"
	.string "COPYCAT: Hmm? Quit mimicking?\l"
	.string "But that's my favorite hobby!$"
	.string "{JPN}{JPN}{PLAYER}『こんにちは。　ポケモンが　すき\n"
	.string "なのね。』\p"
	.string "{PLAYER}『ちがうわ。　わたしが　あなたに\n"
	.string "きいたの。』\p"
	.string "{PLAYER}『えっ？　へんな　こ！』\n"
	.string "ものまねむすめ『ん？　まねを　やめろって？\l"
	.string "だって　ものまねが　いちばんの　しゅみ\l"
	.string "なんだもん！』$"

Text_MimicTeach::
	.string "Oh wow!\n"
	.string "A POKé DOLL!\p"
	.string "For me?\n"
	.string "Thank you!\p"
	.string "You know what?\n"
	.string "I can teach the move MIMIC.$"
	.string "{JPN}{JPN}わあ！　ピッピにんぎょう！\p"
	.string "わたしに　くれるの？　ありがとう！\p"
	.string "そうだ！　わたし　ものまね　っていう　わざを\n"
	.string "おしえて　あげられるの。$"

Text_MimicDeclined::
	.string "Don't you like MIMIC?$"
	.string "{JPN}{JPN}ものまねは　きらい？$"

Text_MimicWhichMon::
	.string "I really love MIMIC!\n"
	.string "Who's going to learn it?$"
	.string "{JPN}{JPN}わたし　ものまねが　だいすき！　だれが\n"
	.string "おぼえるの？$"

Text_MimicTaughtMale::
	.string "{PLAYER}: Hi!\n"
	.string "Thanks for earlier!\p"
	.string "{PLAYER}: Pardon?\p"
	.string "{PLAYER}: Is it that fun to mimic my\n"
	.string "every move?\p"
	.string "COPYCAT: You bet!\n"
	.string "It's a scream!$"
	.string "{JPN}{JPN}{PLAYER}『やあ！　さっきは　ありがとう！』\p"
	.string "{PLAYER}『えっ？』\p"
	.string "{PLAYER}『ぼくの　まねを　するのが\n"
	.string "そんなに　たのしいの？』\p"
	.string "ものまねむすめ『もちろん！　おもしろくて\n"
	.string "たまらないわ！』$"

Text_MimicTaughtFemale::
	.string "{PLAYER}: Hi!\n"
	.string "Thanks for what you did for us.\p"
	.string "{PLAYER}: Pardon?\p"
	.string "{PLAYER}: Is it really that fun to\n"
	.string "copy what I say and do?\p"
	.string "COPYCAT: You bet!\n"
	.string "It's a scream!$"
	.string "{JPN}{JPN}{PLAYER}『こんにちは！　この　まえは\n"
	.string "ありがとう。』\p"
	.string "{PLAYER}『えっ？』\p"
	.string "{PLAYER}『わたしの　まねを　するのって\n"
	.string "ほんとに　そんなに　たのしいの？』\p"
	.string "ものまねむすめ『もちろん！　おもしろくて\n"
	.string "たまらないわ！』$"

SaffronCity_CopycatsHouse_2F_Text_Doduo::
	.string "DODUO: Giiih!\p"
	.string "PRETTY MIRROR LOOK AT ME,\n"
	.string "SEE HOW PRETTY I CAN BE?$"
	.string "{JPN}{JPN}ドードー『ギーッ！』\p"
	.string "かがみよ　かがみ　わたしを　みて！　わたしって\n"
	.string "こんなに　きれい　でしょ？$"

SaffronCity_CopycatsHouse_2F_Text_RareMonOnlyDoll::
	.string "This is a rare POKéMON!\n"
	.string "Huh? It's only a doll!$"
	.string "{JPN}{JPN}めずらしい　ポケモンだ！　あれ？　ただの\n"
	.string "にんぎょう　だ！$"

SaffronCity_CopycatsHouse_2F_Text_MarioWearingABucket::
	.string "A game with MARIO wearing a bucket\n"
	.string "on his head!$"
	.string "{JPN}{JPN}バケツを　かぶった　マリオの　ゲームだ！$"

SaffronCity_CopycatsHouse_2F_Text_MySecrets::
	.string "…\p"
	.string "My Secrets!\p"
	.string "Skill: Mimicry!\n"
	.string "Hobby: Collecting dolls!\l"
	.string "Favorite POKéMON: CLEFAIRY!$"
	.string "{JPN}{JPN}‥\p"
	.string "わたしの　ひみつ！\p"
	.string "とくぎ:　ものまね！\n"
	.string "しゅみ:　にんぎょう　あつめ！\l"
	.string "すきな　ポケモン:　ピッピ！$"

