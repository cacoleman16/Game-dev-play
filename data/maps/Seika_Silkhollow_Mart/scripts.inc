Seika_Silkhollow_Mart_MapScripts::
	.byte 0

Seika_Silkhollow_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Silkhollow_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Silkhollow_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_QUICK_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_MOON_STONE
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_NONE
	release
	end

Seika_Silkhollow_Mart_EventScript_Npc0::
	msgbox Seika_Silkhollow_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Silkhollow_Mart_Text_Npc0::
	.string "SILK SCARVES are woven right here\n"
	.string "in town. They power up NORMAL-\l"
	.string "type moves!$"
