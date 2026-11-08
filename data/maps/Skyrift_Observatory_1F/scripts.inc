Skyrift_Observatory_1F_MapScripts::
	.byte 0

Skyrift_Observatory_1F_EventScript_Cases::
	msgbox Skyrift_Observatory_1F_Text_Cases, MSGBOX_SIGN
	end

Skyrift_Observatory_1F_Text_Cases::
	.string "Glass cases full of meteorite\n"
	.string "chips, each with a label:\p"
	.string "“METEOR BASIN, SECOND STARFALL.”$"

Skyrift_Observatory_1F_EventScript_NovaA::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_1F_1, Skyrift_Observatory_1F_Text_NovaAIntro, Skyrift_Observatory_1F_Text_NovaADefeat
	msgbox Skyrift_Observatory_1F_Text_NovaAPost, MSGBOX_AUTOCLOSE
	end

Skyrift_Observatory_1F_Text_NovaAIntro::
	.string "Intruders in the OBSERVATORY!\n"
	.string "Sound the alarm!$"

Skyrift_Observatory_1F_Text_NovaADefeat::
	.string "Alarm... off...$"

Skyrift_Observatory_1F_Text_NovaAPost::
	.string "The lab is upstairs. VEGA is\n"
	.string "above that, on the roof.$"

Skyrift_Observatory_1F_EventScript_NovaB::
	trainerbattle_single TRAINER_SKYRIFT_NOVA_1F_2, Skyrift_Observatory_1F_Text_NovaBIntro, Skyrift_Observatory_1F_Text_NovaBDefeat
	msgbox Skyrift_Observatory_1F_Text_NovaBPost, MSGBOX_AUTOCLOSE
	end

Skyrift_Observatory_1F_Text_NovaBIntro::
	.string "I built half the machines in\n"
	.string "this building. Let me show you!$"

Skyrift_Observatory_1F_Text_NovaBDefeat::
	.string "My calculations were off...$"

Skyrift_Observatory_1F_Text_NovaBPost::
	.string "The ENGINE draws its power from\n"
	.string "the rift itself. Brilliant, no?$"
