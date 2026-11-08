Seika_Duskpeak_Mart_MapScripts::
	.byte 0

Seika_Duskpeak_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Seika_Duskpeak_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
Seika_Duskpeak_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_DUSK_BALL
	.2byte ITEM_QUICK_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_DUSK_STONE
	.2byte ITEM_NONE
	release
	end

Seika_Duskpeak_Mart_EventScript_Npc0::
	msgbox Seika_Duskpeak_Mart_Text_Npc0, MSGBOX_NPC
	end

Seika_Duskpeak_Mart_Text_Npc0::
	.string "DUSK BALLS work best in caves\n"
	.string "and at night.\p"
	.string "Here in DUSKPEAK it always\n"
	.string "feels like dusk.$"
	.string "{JPN}{JPN}ダークボールは　どうくつや　よるに　つかうと\n"
	.string "いちばん　よく　きくのよ。\p"
	.string "ここ　タソガレむらは　いつも　たそがれどき\n"
	.string "みたいだけどね。$"
