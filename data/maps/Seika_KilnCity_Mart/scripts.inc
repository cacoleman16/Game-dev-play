Seika_KilnCity_Mart_MapScripts::
	.byte 0

Seika_Kiln_City_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Kiln_City_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Kiln_City_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_QUICK_BALL
	.2byte ITEM_DUSK_BALL
	.2byte ITEM_TIMER_BALL
	.2byte ITEM_HEAL_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	.2byte ITEM_X_SP_ATK
	.2byte ITEM_X_SP_DEF
	.2byte ITEM_X_SPEED
	.2byte ITEM_NONE
	release
	end

Seika_Kiln_City_Mart_EventScript_Npc0::
	msgbox Seika_Kiln_City_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Kiln_City_Mart_Text_Npc0::
	.string "This MART has more than any other\n"
	.string "in SEIKA. Merchants come from\l"
	.string "every town to KILN CITY.$"
