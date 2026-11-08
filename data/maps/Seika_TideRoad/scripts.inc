Seika_TideRoad_MapScripts::
	.byte 0

Seika_TideRoad_EventScript_SignWest::
	msgbox Seika_TideRoad_Text_SignWest, MSGBOX_SIGN
	end

Seika_TideRoad_Text_SignWest::
	.string "TIDE ROAD\n"
	.string "KILN CITY - TIDEWATCH TOWN$"

Seika_TideRoad_EventScript_SignEast::
	msgbox Seika_TideRoad_Text_SignEast, MSGBOX_SIGN
	end

Seika_TideRoad_Text_SignEast::
	.string "TIDE ROAD\n"
	.string "TIDEWATCH TOWN - KILN CITY$"

Seika_TideRoad_EventScript_ItemBigPearl::
	finditem ITEM_BIG_PEARL
	end

Seika_TideRoad_EventScript_Woman::
	msgbox Seika_TideRoad_Text_Woman, MSGBOX_NPC
	end

Seika_TideRoad_Text_Woman::
	.string "The tide comes in twice a day.\n"
	.string "The people of TIDEWATCH can tell\l"
	.string "the hour by it.\p"
	.string "They say their shrine keeper can\n"
	.string "feel it in her bones!$"

Seika_TideRoad_EventScript_Shige::
	trainerbattle_single TRAINER_SEIKA_FISHERMAN_SHIGE, Seika_TideRoad_Text_ShigeIntro, Seika_TideRoad_Text_ShigeDefeat
	msgbox Seika_TideRoad_Text_ShigePost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_ShigeIntro::
	.string "Shh! You'll scare the REMORAID!\n"
	.string "Oh, fine. Let's battle!$"

Seika_TideRoad_Text_ShigeDefeat::
	.string "They all got away...$"

Seika_TideRoad_Text_ShigePost::
	.string "OCTILLERY love the rocks under\n"
	.string "this pier. I fish here every day.$"

Seika_TideRoad_EventScript_Dan::
	trainerbattle_single TRAINER_SEIKA_SAILOR_DAN, Seika_TideRoad_Text_DanIntro, Seika_TideRoad_Text_DanDefeat
	msgbox Seika_TideRoad_Text_DanPost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_DanIntro::
	.string "Ahoy! I just came ashore and my\n"
	.string "legs want a fight!$"

Seika_TideRoad_Text_DanDefeat::
	.string "Sunk, by a landlubber!$"

Seika_TideRoad_Text_DanPost::
	.string "The sea round TIDEWATCH is calm.\n"
	.string "NAGI the diver keeps it that way.$"

Seika_TideRoad_EventScript_Rin::
	trainerbattle_single TRAINER_SEIKA_SWIMMER_F_RIN, Seika_TideRoad_Text_RinIntro, Seika_TideRoad_Text_RinDefeat
	msgbox Seika_TideRoad_Text_RinPost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_RinIntro::
	.string "The water is warm today! Let's\n"
	.string "make some waves!$"

Seika_TideRoad_Text_RinDefeat::
	.string "I swallowed some seawater...$"

Seika_TideRoad_Text_RinPost::
	.string "I swim to TIDEWATCH and back\n"
	.string "every morning. It's good for you!$"

Seika_TideRoad_EventScript_Jun::
	trainerbattle_single TRAINER_SEIKA_SWIMMER_M_JUN, Seika_TideRoad_Text_JunIntro, Seika_TideRoad_Text_JunDefeat
	msgbox Seika_TideRoad_Text_JunPost, MSGBOX_AUTOCLOSE
	end

Seika_TideRoad_Text_JunIntro::
	.string "Nobody swims faster than me in\n"
	.string "the rain!$"

Seika_TideRoad_Text_JunDefeat::
	.string "The tide turned on me!$"

Seika_TideRoad_Text_JunPost::
	.string "My KINGDRA was a HORSEA from\n"
	.string "this very beach.$"
