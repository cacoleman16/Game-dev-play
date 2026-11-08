SaffronCity_CopycatsHouse_1F_Frlg_MapScripts::
	.byte 0

SaffronCity_CopycatsHouse_1F_EventScript_CopycatsMom::
	msgbox SaffronCity_CopycatsHouse_1F_Text_DaughterIsSelfCentered, MSGBOX_NPC
	end

SaffronCity_CopycatsHouse_1F_EventScript_CopycatsDad::
	msgbox SaffronCity_CopycatsHouse_1F_Text_DaughterLikesToMimicPeople, MSGBOX_NPC
	end

SaffronCity_CopycatsHouse_1F_EventScript_Chansey::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CHANSEY, CRY_MODE_NORMAL
	msgbox SaffronCity_CopycatsHouse_1F_Text_Chansey
	waitmoncry
	release
	end

SaffronCity_CopycatsHouse_1F_Text_DaughterIsSelfCentered::
	.string "My daughter is so self-centered.\n"
	.string "She has but a few friends.$"
	.string "{JPN}{JPN}うちの　むすめは　じぶんかって　でね。\n"
	.string "ともだちが　すくないの。$"

SaffronCity_CopycatsHouse_1F_Text_DaughterLikesToMimicPeople::
	.string "My daughter likes to mimic people.\n"
	.string "It's rather vexing.\p"
	.string "Her mimicry has earned her the\n"
	.string "nickname COPYCAT around here.$"
	.string "{JPN}{JPN}うちの　むすめは　ひとの　まねを　するのが\n"
	.string "すきでね。　ちょっと　こまって　いるんだ。\p"
	.string "ものまねが　うまいから　この　あたりでは\n"
	.string "ものまねむすめ　と　よばれているよ。$"

SaffronCity_CopycatsHouse_1F_Text_Chansey::
	.string "CHANSEY: Chaan! Sii!$"
	.string "{JPN}{JPN}ラッキー『ラッ　ラッキー！』$"

