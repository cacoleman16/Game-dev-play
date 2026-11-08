Seika_KilnCity_PotteryShop_MapScripts::
	.byte 0

Seika_KilnCity_PotteryShop_EventScript_Clerk::
	lock
	faceplayer
	message Seika_KilnCity_PotteryShop_Text_Clerk
	waitmessage
	pokemart Seika_KilnCity_PotteryShop_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_KilnCity_PotteryShop_Items::
	.2byte ITEM_HEAT_ROCK
	.2byte ITEM_DAMP_ROCK
	.2byte ITEM_SMOOTH_ROCK
	.2byte ITEM_ICY_ROCK
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_HARD_STONE
	.2byte ITEM_NONE
	release
	end

Seika_KilnCity_PotteryShop_Text_Clerk::
	.string "Welcome! These stones were fired\n"
	.string "in our kilns.\p"
	.string "Each one holds a little weather.\n"
	.string "Take a look!$"

Seika_KilnCity_PotteryShop_EventScript_Potter::
	msgbox Seika_KilnCity_PotteryShop_Text_Potter, MSGBOX_NPC
	end

Seika_KilnCity_PotteryShop_Text_Potter::
	.string "A pot remembers the fire that\n"
	.string "made it. So does a stone.\p"
	.string "A HEAT ROCK from our kilns makes\n"
	.string "the sun shine longer in battle.$"
