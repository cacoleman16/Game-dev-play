PewterCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PewterCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_PEWTER_CITY
	end

PewterCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

PewterCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox PewterCity_PokemonCenter_1F_Text_TeamRocketMtMoonImOnPhone, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_Jigglypuff::
	lock
	faceplayer
	fadeoutbgm 0
	playbgm MUS_RG_JIGGLYPUFF, 0
	message PewterCity_PokemonCenter_1F_Text_Jigglypuff
	waitmessage
	delay 455
	playbgm MUS_POKE_CENTER, 1
	release
	end

PewterCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox PewterCity_PokemonCenter_1F_Text_WhenJiggylypuffSingsMonsGetDrowsy, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_EventScript_GBAKid1::
	lock
	msgbox PewterCity_PokemonCenter_1F_Text_TradingMyClefairyForPikachu
	release
	end

PewterCity_PokemonCenter_1F_EventScript_GBAKid2::
	msgbox PewterCity_PokemonCenter_1F_Text_TradingPikachuWithKid, MSGBOX_NPC
	end

PewterCity_PokemonCenter_1F_Text_TeamRocketMtMoonImOnPhone::
	.string "What!?\p"
	.string "TEAM ROCKET is at MT. MOON?\n"
	.string "Huh?\p"
	.string "I'm on the phone!\n"
	.string "Scram!$"
	.string "{JPN}{JPN}なんだと！？\p"
	.string "ロケットだんが　おつきみやまに？　ん？\p"
	.string "いま　でんわちゅう　なんだ！　あっちへ　いけ！$"

PewterCity_PokemonCenter_1F_Text_Jigglypuff::
	.string "JIGGLYPUFF: Puu pupuu!$"
	.string "{JPN}{JPN}プリン『プー　ププー！』$"

PewterCity_PokemonCenter_1F_Text_WhenJiggylypuffSingsMonsGetDrowsy::
	.string "Yawn!\p"
	.string "When JIGGLYPUFF sings, POKéMON get\n"
	.string "drowsy…\p"
	.string "…Me, too…\n"
	.string "Snore…$"
	.string "{JPN}{JPN}ふわーあ！\p"
	.string "プリンが　うたうと　ポケモンは　ねむく\n"
	.string "なるんだ‥\p"
	.string "‥ぼくも‥　ぐうぐう‥$"

PewterCity_PokemonCenter_1F_Text_TradingMyClefairyForPikachu::
	.string "I really want a PIKACHU, so I'm\n"
	.string "trading my CLEFAIRY for one.$"
	.string "{JPN}{JPN}どうしても　ピカチュウが　ほしいから　ぼくの\n"
	.string "ピッピと　こうかん　するんだ。$"

PewterCity_PokemonCenter_1F_Text_TradingPikachuWithKid::
	.string "I'm trading POKéMON with that kid\n"
	.string "there.\p"
	.string "I had two PIKACHU, so I figured\n"
	.string "I might as well trade one.$"
	.string "{JPN}{JPN}あそこの　こと　ポケモンを　こうかん　するんだ。\p"
	.string "ピカチュウが　2ひき　いたから　1ぴき　こうかん\n"
	.string "しても　いいかなって。$"

