Skyrift_Riftwake_Mart_MapScripts::
	.byte 0

Skyrift_Riftwake_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Skyrift_Riftwake_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Skyrift_Riftwake_Mart_Items::
	.2byte ITEM_BEAST_BALL
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_DUSK_BALL
	.2byte ITEM_QUICK_BALL
	.2byte ITEM_TIMER_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

Skyrift_Riftwake_Mart_EventScript_Npc0::
	msgbox Skyrift_Riftwake_Mart_Text_Npc0, MSGBOX_NPC
	end

Skyrift_Riftwake_Mart_Text_Npc0::
	.string "BEAST BALLS! They only make them\n"
	.string "for what comes out of the rift.\p"
	.string "On anything else, they barely\n"
	.string "work at all. Choose wisely!$"
	.string "{JPN}{JPN}ウルトラボールです！　さけめから　でてくる\n"
	.string "ものの　ためだけに　つくられて　いるのですよ。\p"
	.string "ほかの　ポケモンには　ほとんど　ききません。\n"
	.string "よく　えらんで　くださいね！$"
