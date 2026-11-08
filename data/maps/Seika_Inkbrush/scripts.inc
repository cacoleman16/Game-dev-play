Seika_Inkbrush_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Seika_Inkbrush_QoLFly_OnTransition
	.byte 0

Seika_Inkbrush_EventScript_TownSign::
	msgbox Seika_Inkbrush_Text_TownSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_TownSign::
	.string "INKBRUSH TOWN\n"
	.string "Every stroke tells a story.$"
	.string "{JPN}{JPN}スミフデタウン　ひとふでごとに　ものがたり$"

Seika_Inkbrush_EventScript_GymSign::
	msgbox Seika_Inkbrush_Text_GymSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_GymSign::
	.string "INKBRUSH POKéMON GYM\n"
	.string "LEADER: SUMI\p"
	.string "The calligrapher who reads\n"
	.string "the mind behind every stroke!$"
	.string "{JPN}{JPN}スミフデ　ポケモンジム　リーダー　スミ\p"
	.string "ひとふでに　こめた　こころを　よみとる\n"
	.string "しょどうか！$"

Seika_Inkbrush_EventScript_HallSign::
	msgbox Seika_Inkbrush_Text_HallSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_HallSign::
	.string "INKBRUSH SCRIPTORIUM\n"
	.string "Keepers of the STARFALL SCROLL$"
	.string "{JPN}{JPN}スミフデ　まきものかん\n"
	.string "スターフォールのまきものを　まもりつぐ　ところ$"

Seika_Inkbrush_EventScript_WestSign::
	msgbox Seika_Inkbrush_Text_WestSign, MSGBOX_SIGN
	end

Seika_Inkbrush_Text_WestSign::
	.string "WEST: MOSSGROVE WOODS\n"
	.string "MOSSGROVE VILLAGE lies beyond.$"
	.string "{JPN}{JPN}にしへ　コケムスのもり　その　さきに\n"
	.string "コケムスむら$"

Seika_Inkbrush_EventScript_OldScribe::
	msgbox Seika_Inkbrush_Text_OldScribe, MSGBOX_NPC
	end

Seika_Inkbrush_Text_OldScribe::
	.string "I have copied books in that\n"
	.string "SCRIPTORIUM for sixty years.\p"
	.string "In all that time, nobody asked to\n"
	.string "see the STARFALL SCROLL. Then\l"
	.string "last week, a stranger did.$"
	.string "{JPN}{JPN}わしは　あの　まきものかんで　60ねん　ほんを\n"
	.string "うつして　きた。\p"
	.string "その　あいだ　スターフォールのまきものを\n"
	.string "みせてくれと　いう　ものは　おらなんだ。　だが\l"
	.string "せんしゅう　よそものが　きてのう。$"

Seika_Inkbrush_EventScript_Boy::
	msgbox Seika_Inkbrush_Text_Boy, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Boy::
	.string "I'm practicing my brush strokes!\p"
	.string "SUMI says a good stroke comes\n"
	.string "from a calm mind. Mine wobble...$"
	.string "{JPN}{JPN}ぼく　ふでの　れんしゅう　してるんだ！\p"
	.string "スミさんが　いうには　いい　せんは　おちついた\n"
	.string "こころから　うまれるって。　ぼくのは　よれよれ‥$"

Seika_Inkbrush_EventScript_Woman::
	msgbox Seika_Inkbrush_Text_Woman, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Woman::
	.string "MOSSGROVE WOODS is just west of\n"
	.string "town. It's easy to get lost in\l"
	.string "there.\p"
	.string "A man with dirty gloves went in\n"
	.string "with a shovel. Digging for what,\l"
	.string "I wonder?$"
	.string "{JPN}{JPN}まちの　すぐ　にしが　コケムスのもりよ。　なかは\n"
	.string "とても　まよいやすいの。\p"
	.string "てぶくろを　どろだらけに　した　おとこの　ひとが\n"
	.string "スコップを　もって　はいって　いったわ。　なにを\l"
	.string "ほるのかしら？$"

Seika_Inkbrush_EventScript_Fisher::
	msgbox Seika_Inkbrush_Text_Fisher, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Fisher::
	.string "The scribes rinse their brushes\n"
	.string "in this pond.\p"
	.string "The POLIWAG here have ink-black\n"
	.string "swirls. Coincidence? Ha!$"
	.string "{JPN}{JPN}まきものかんの　ひとたちは　この　いけで　ふでを\n"
	.string "あらうんだ。\p"
	.string "ここの　ニョロモの　うずまきは　すみの　ように\n"
	.string "まっくろ。　ぐうぜん？　はっ！$"

Seika_Inkbrush_EventScript_Gentleman::
	msgbox Seika_Inkbrush_Text_Gentleman, MSGBOX_NPC
	end

Seika_Inkbrush_Text_Gentleman::
	.string "SUMI can read a person's heart\n"
	.string "from one line of their writing.\p"
	.string "I wrote her a letter once. She\n"
	.string "still laughs when she sees me.$"
	.string "{JPN}{JPN}スミさんは　もじを　1ぎょう　みるだけで　その\n"
	.string "ひとの　こころが　わかるのです。\p"
	.string "てがみを　かいた　ことが　ありましてな。\n"
	.string "いまだに　わたしを　みると　わらうのです。$"

Seika_Inkbrush_QoLFly_OnTransition::
	setflag FLAG_QOL_SEIKA_FLY_INKBRUSH
	end
