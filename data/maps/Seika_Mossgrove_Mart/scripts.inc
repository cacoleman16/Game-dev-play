Seika_Mossgrove_Mart_MapScripts::
	.byte 0

Seika_Mossgrove_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Mossgrove_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Mossgrove_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_NET_BALL
	.2byte ITEM_NEST_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_HONEY
	.2byte ITEM_NONE
	release
	end

Seika_Mossgrove_Mart_EventScript_Npc0::
	msgbox Seika_Mossgrove_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Mossgrove_Mart_Text_Npc0::
	.string "MOSSGROVE HONEY is famous.\p"
	.string "Spread some in tall grass and\n"
	.string "wild POKéMON come running!$"
	.string "{JPN}{JPN}コケムスの　あまいミツは　ゆうめいなのよ。\p"
	.string "くさむらで　つかうと　やせいの　ポケモンが\n"
	.string "とんで　くるわ！$"
