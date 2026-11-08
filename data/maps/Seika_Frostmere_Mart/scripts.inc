Seika_Frostmere_Mart_MapScripts::
	.byte 0

Seika_Frostmere_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Frostmere_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Frostmere_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_DIVE_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_ICE_STONE
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_NONE
	release
	end

Seika_Frostmere_Mart_EventScript_Npc0::
	msgbox Seika_Frostmere_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Frostmere_Mart_Text_Npc0::
	.string "NEVER-MELT ICE powers up ICE\n"
	.string "moves. It never melts, not even\l"
	.string "in my pocket!$"
