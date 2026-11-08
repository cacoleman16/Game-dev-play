Seika_Inkbrush_Mart_MapScripts::
	.byte 0

Seika_Inkbrush_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Inkbrush_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Inkbrush_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_QUICK_BALL
	.2byte ITEM_TIMER_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

Seika_Inkbrush_Mart_EventScript_Npc0::
	msgbox Seika_Inkbrush_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Inkbrush_Mart_Text_Npc0::
	.string "QUICK BALLS are best thrown on\n"
	.string "the very first turn.\p"
	.string "Just like a brush stroke. No\n"
	.string "second chances!$"
	.string "{JPN}{JPN}クイックボールは　さいしょの　ターンに\n"
	.string "なげるのが　いちばん。\p"
	.string "ふでと　おなじさ。　にどめは　ないんだ！$"
