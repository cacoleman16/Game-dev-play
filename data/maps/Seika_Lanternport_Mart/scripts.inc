Seika_Lanternport_Mart_MapScripts::
	.byte 0

Seika_Lanternport_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Lanternport_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Lanternport_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_DUSK_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_NONE
	release
	end

Seika_Lanternport_Mart_EventScript_Npc0::
	msgbox Seika_Lanternport_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Lanternport_Mart_Text_Npc0::
	.string "A FIRE STONE for the festival\n"
	.string "bonfire? Why not!$"
