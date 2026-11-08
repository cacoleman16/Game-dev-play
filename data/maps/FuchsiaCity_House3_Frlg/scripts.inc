FuchsiaCity_House3_Frlg_MapScripts::
	.byte 0

FuchsiaCity_House3_EventScript_MoveDeleter::
	lock
	faceplayer
	msgbox FuchsiaCity_House3_Text_WouldYouLikeToForgetMove, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FuchsiaCity_House3_EventScript_ChooseMonForMoveDeleter
	goto FuchsiaCity_House3_EventScript_CancelForgetMove
	end

FuchsiaCity_House3_EventScript_ChooseMonForMoveDeleter::
	msgbox FuchsiaCity_House3_Text_WhichMonShouldForgetMove
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, FuchsiaCity_House3_EventScript_CancelForgetMove
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, FuchsiaCity_House3_EventScript_CantForgetMoveEgg
	special GetNumMovesSelectedMonHas
	goto_if_eq VAR_RESULT, 1, FuchsiaCity_House3_EventScript_CantForgetOnlyMove
	msgbox FuchsiaCity_House3_Text_WhichMoveShouldBeForgotten
	fadescreen FADE_TO_BLACK
	special MoveDeleterChooseMoveToForget
	fadescreen FADE_FROM_BLACK
	goto_if_eq VAR_0x8005, MAX_MON_MOVES, FuchsiaCity_House3_EventScript_ChooseMonForMoveDeleter
	special BufferMoveDeleterNicknameAndMove
	msgbox FuchsiaCity_House3_Text_MonsMoveShouldBeForgotten, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FuchsiaCity_House3_EventScript_ForgetMove
	goto FuchsiaCity_House3_EventScript_CancelForgetMove
	end

FuchsiaCity_House3_EventScript_ForgetMove::
	special MoveDeleterForgetMove
	playfanfare MUS_MOVE_DELETED
	waitfanfare
	msgbox FuchsiaCity_House3_Text_MonHasForgottenMoveCompletely
	release
	end

FuchsiaCity_House3_EventScript_CantForgetOnlyMove::
	special BufferMoveDeleterNicknameAndMove
	msgbox FuchsiaCity_House3_Text_MonOnlyKnowsOneMove
	release
	end

FuchsiaCity_House3_EventScript_CantForgetMoveEgg::
	msgbox FuchsiaCity_House3_Text_NoEggShouldKnowMoves
	release
	end

FuchsiaCity_House3_EventScript_CancelForgetMove::
	msgbox FuchsiaCity_House3_Text_ComeAgainToForgetOtherMoves
	release
	end

FuchsiaCity_House3_Text_WouldYouLikeToForgetMove::
	.string "Uh…\n"
	.string "Oh, yes, I'm the MOVE DELETER.\p"
	.string "I can make POKéMON forget their\n"
	.string "moves.\p"
	.string "Would you like me to do that?$"
	.string "{JPN}{JPN}あー‥　そうそう　わしは　わざわすれオヤジ\n"
	.string "じゃ。\p"
	.string "ポケモンに　わざを　わすれさせる　ことが\n"
	.string "できる。\p"
	.string "わすれさせて　みるかね？$"

FuchsiaCity_House3_Text_WhichMonShouldForgetMove::
	.string "Which POKéMON should forget a\n"
	.string "move?$"
	.string "{JPN}{JPN}どの　ポケモンの　わざを　わすれさせる？$"

FuchsiaCity_House3_Text_WhichMoveShouldBeForgotten::
	.string "Which move should be forgotten?$"
	.string "{JPN}{JPN}どの　わざを　わすれさせる？$"

FuchsiaCity_House3_Text_MonOnlyKnowsOneMove::
	.string "{STR_VAR_1} seems to know only one\n"
	.string "move…$"
	.string "{JPN}{JPN}{STR_VAR_1}は　わざを　ひとつしか\n"
	.string "おぼえて　おらん　ようじゃ‥$"

FuchsiaCity_House3_Text_MonsMoveShouldBeForgotten::
	.string "Hm! {STR_VAR_1}'s {STR_VAR_2}?\n"
	.string "That move should be forgotten?$"
	.string "{JPN}{JPN}ふむ！　{STR_VAR_1}の\n"
	.string "{STR_VAR_2}じゃな？　その　わざを\l"
	.string "わすれさせるのか？$"

FuchsiaCity_House3_Text_MonHasForgottenMoveCompletely::
	.string "It worked to perfection!\p"
	.string "{STR_VAR_1} has forgotten\n"
	.string "{STR_VAR_2} completely.$"
	.string "{JPN}{JPN}かんぺきに　うまく　いったぞ！\p"
	.string "{STR_VAR_1}は　{STR_VAR_2}を\n"
	.string "きれいさっぱり　わすれた！$"

FuchsiaCity_House3_Text_ComeAgainToForgetOtherMoves::
	.string "Come again if there are other\n"
	.string "moves to be forgotten.$"
	.string "{JPN}{JPN}ほかにも　わすれさせたい　わざが　あったら　また\n"
	.string "おいで。$"

FuchsiaCity_House3_Text_NoEggShouldKnowMoves::
	.string "What?\n"
	.string "No EGG should know any moves.$"
	.string "{JPN}{JPN}なんと？　タマゴは　わざを　おぼえて　おらんぞ。$"

