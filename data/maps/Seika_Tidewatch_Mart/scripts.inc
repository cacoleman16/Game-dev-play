Seika_Tidewatch_Mart_MapScripts::
	.byte 0

Seika_Tidewatch_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Tidewatch_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Tidewatch_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_NET_BALL
	.2byte ITEM_DIVE_BALL
	.2byte ITEM_LURE_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_WATER_STONE
	.2byte ITEM_NONE
	release
	end

Seika_Tidewatch_Mart_EventScript_Npc0::
	msgbox Seika_Tidewatch_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Tidewatch_Mart_Text_Npc0::
	.string "A LURE BALL works best on POKéMON\n"
	.string "you hook with a ROD.$"
