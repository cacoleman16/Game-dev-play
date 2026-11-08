CeladonCity_DepartmentStore_3F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_3F_EventScript_CounterTutor::
	goto EventScript_CounterTutor
	end

CeladonCity_DepartmentStore_3F_EventScript_GBAKid1::
	msgbox CeladonCity_DepartmentStore_3F_Text_OTStandsForOriginalTrainer, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_GBAKid2::
	msgbox CeladonCity_DepartmentStore_3F_Text_BuddyTradingKangaskhanForHaunter, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_GBAKid3::
	msgbox CeladonCity_DepartmentStore_3F_Text_HaunterEvolvedOnTrade, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_LittleGirl::
	msgbox CeladonCity_DepartmentStore_3F_Text_CanIdentifyTradeMonsByID, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_SuperNES::
	msgbox CeladonCity_DepartmentStore_3F_Text_ItsSuperNES, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV1::
	msgbox CeladonCity_DepartmentStore_3F_Text_AnRPG, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV2::
	msgbox CeladonCity_DepartmentStore_3F_Text_SportsGame, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV3::
	msgbox CeladonCity_DepartmentStore_3F_Text_PuzzleGame, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV4::
	msgbox CeladonCity_DepartmentStore_3F_Text_FightingGame, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_3F_Text_TVGameShop, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_Poster::
	msgbox CeladonCity_DepartmentStore_3F_Text_RedGreenBothArePokemon, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_Text_OTStandsForOriginalTrainer::
	.string "Captured POKéMON are registered\n"
	.string "with an ID No. and the OT.\p"
	.string "OT stands for “Original TRAINER” -\n"
	.string "the TRAINER that caught it first.$"
	.string "{JPN}{JPN}つかまえた　ポケモンには　IDナンバーと　おやが\n"
	.string "とうろく　されるんだ\p"
	.string "おやって　いうのは　さいしょに　つかまえた\n"
	.string "トレーナーの　ことさ$"

CeladonCity_DepartmentStore_3F_Text_BuddyTradingKangaskhanForHaunter::
	.string "All right!\p"
	.string "My buddy's going to trade me his\n"
	.string "KANGASKHAN for my HAUNTER!$"
	.string "{JPN}{JPN}やったー！\p"
	.string "ともだちが　ぼくの　ゴーストと　ガルーラを\n"
	.string "こうかん　してくれるんだ！$"

CeladonCity_DepartmentStore_3F_Text_HaunterEvolvedOnTrade::
	.string "Come on, HAUNTER!\p"
	.string "I love HAUNTER!\n"
	.string "I collect them!\p"
	.string "Huh?\p"
	.string "HAUNTER turned into a different\n"
	.string "POKéMON while being traded!$"
	.string "{JPN}{JPN}おいで　ゴースト！\p"
	.string "ぼく　ゴーストが　だいすき！　あつめてるんだ！\p"
	.string "あれ？\p"
	.string "こうかん　したら　ゴーストが　ちがう　ポケモンに\n"
	.string "なっちゃった！$"

CeladonCity_DepartmentStore_3F_Text_CanIdentifyTradeMonsByID::
	.string "You can identify POKéMON you get\n"
	.string "in trades by their ID Numbers.$"
	.string "{JPN}{JPN}こうかんで　もらった　ポケモンは　IDナンバーで\n"
	.string "みわけられるよ$"

CeladonCity_DepartmentStore_3F_Text_ItsSuperNES::
	.string "It's a Super NES.$"
	.string "{JPN}{JPN}スーパーファミコンだ$"

CeladonCity_DepartmentStore_3F_Text_AnRPG::
	.string "An RPG!\n"
	.string "There's no time for that!$"
	.string "{JPN}{JPN}RPGだ！　そんな　ひまは　ない！$"

CeladonCity_DepartmentStore_3F_Text_SportsGame::
	.string "A sports game!\n"
	.string "Dad'll like that!$"
	.string "{JPN}{JPN}スポーツゲームだ！　おとうさんが　よろこびそう！$"

CeladonCity_DepartmentStore_3F_Text_PuzzleGame::
	.string "A puzzle game!\n"
	.string "Looks addictive!$"
	.string "{JPN}{JPN}パズルゲームだ！　はまりそう！$"

CeladonCity_DepartmentStore_3F_Text_FightingGame::
	.string "A fighting game!\n"
	.string "Looks tough!$"
	.string "{JPN}{JPN}かくとうゲームだ！　むずかしそう！$"

CeladonCity_DepartmentStore_3F_Text_TVGameShop::
	.string "3F: TV GAME SHOP$"
	.string "{JPN}{JPN}3F:　テレビゲームショップ$"

CeladonCity_DepartmentStore_3F_Text_RedGreenBothArePokemon::
	.string "Red and Green!\n"
	.string "Both are POKéMON!$"
	.string "{JPN}{JPN}あかと　みどり！　どっちも　ポケモンだ！$"

Text_CounterTeach::
	.string "Oh, hi!\n"
	.string "I finally finished POKéMON.\p"
	.string "Not done yet? How about I teach\n"
	.string "you a good move?\p"
	.string "The move I have in mind is\n"
	.string "COUNTER.\p"
	.string "Not like the one I'm leaning on,\n"
	.string "mind you!$"
	.string "{JPN}{JPN}あ　どうも！　ぼく　やっと　ポケモンを　クリア\n"
	.string "したんだ\p"
	.string "きみは　まだ？　じゃあ　いい　わざを　おしえて\n"
	.string "あげようか？\p"
	.string "おしえたい　わざは　カウンター！\p"
	.string "ぼくが　よりかかってる　この　カウンターじゃ\n"
	.string "ないよ！$"

Text_CounterDeclined::
	.string "You're not interested? Come see\n"
	.string "me if you change your mind.$"
	.string "{JPN}{JPN}きょうみ　ない？　きが　かわったら　また　きてね$"

Text_CounterWhichMon::
	.string "Which POKéMON should I teach\n"
	.string "COUNTER to?$"
	.string "{JPN}{JPN}どの　ポケモンに　カウンターを　おしえる？$"

Text_CounterTaught::
	.string "Are you using that COUNTER move\n"
	.string "I taught your POKéMON?$"
	.string "{JPN}{JPN}ぼくが　おしえた　カウンター　つかってる？$"
