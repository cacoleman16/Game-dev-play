CeladonCity_DepartmentStore_4F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_4F_EventScript_Man::
	msgbox CeladonCity_DepartmentStore_4F_Text_GettingPokeDollAsPresent, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_Youngster::
	msgbox CeladonCity_DepartmentStore_4F_Text_CanRunAwayWithPokeDoll, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_4F_Text_FloorSign, MSGBOX_SIGN
	end

@ [QoL] The 4F clerk also sells every evolution item, Mega Stone and Z-Crystal,
@ so modern evolutions and battle gimmicks are available in Kanto.
CeladonCity_DepartmentStore_4F_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	dynmultipush CeladonCity_DepartmentStore_4F_Text_ChoiceGifts, 0
	dynmultipush CeladonCity_DepartmentStore_4F_Text_ChoiceEvolution, 1
	dynmultipush CeladonCity_DepartmentStore_4F_Text_ChoiceMegaStones, 2
	dynmultipush CeladonCity_DepartmentStore_4F_Text_ChoiceZCrystals, 3
	dynmultipush CeladonCity_DepartmentStore_4F_Text_ChoiceCancel, 4
	dynmultistack 0, 0, FALSE, 5, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_4F_EventScript_MartGifts
	case 1, CeladonCity_DepartmentStore_4F_EventScript_MartEvolution
	case 2, CeladonCity_DepartmentStore_4F_EventScript_MartMegaStones
	case 3, CeladonCity_DepartmentStore_4F_EventScript_MartZCrystals
	msgbox gText_PleaseComeAgain
	release
	end

CeladonCity_DepartmentStore_4F_EventScript_MartGifts::
	pokemart CeladonCity_DepartmentStore_4F_Items
	goto CeladonCity_DepartmentStore_4F_EventScript_ClerkDone

CeladonCity_DepartmentStore_4F_EventScript_MartEvolution::
	pokemart CeladonCity_DepartmentStore_4F_EvolutionItems
	goto CeladonCity_DepartmentStore_4F_EventScript_ClerkDone

CeladonCity_DepartmentStore_4F_EventScript_MartMegaStones::
	pokemart CeladonCity_DepartmentStore_4F_MegaStones
	goto CeladonCity_DepartmentStore_4F_EventScript_ClerkDone

CeladonCity_DepartmentStore_4F_EventScript_MartZCrystals::
	pokemart CeladonCity_DepartmentStore_4F_ZCrystals
	goto CeladonCity_DepartmentStore_4F_EventScript_ClerkDone

CeladonCity_DepartmentStore_4F_EventScript_ClerkDone::
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_4F_Items::
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_RETRO_MAIL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_NONE
	release
	end

	.align 2
CeladonCity_DepartmentStore_4F_EvolutionItems::
	.2byte ITEM_ICE_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_SHINY_STONE
	.2byte ITEM_DUSK_STONE
	.2byte ITEM_DAWN_STONE
	.2byte ITEM_SWEET_APPLE
	.2byte ITEM_TART_APPLE
	.2byte ITEM_CRACKED_POT
	.2byte ITEM_CHIPPED_POT
	.2byte ITEM_GALARICA_CUFF
	.2byte ITEM_GALARICA_WREATH
	.2byte ITEM_AUSPICIOUS_ARMOR
	.2byte ITEM_MALICIOUS_ARMOR
	.2byte ITEM_BLACK_AUGURITE
	.2byte ITEM_LINKING_CORD
	.2byte ITEM_PEAT_BLOCK
	.2byte ITEM_SYRUPY_APPLE
	.2byte ITEM_UNREMARKABLE_TEACUP
	.2byte ITEM_MASTERPIECE_TEACUP
	.2byte ITEM_METAL_ALLOY
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_UPGRADE
	.2byte ITEM_PROTECTOR
	.2byte ITEM_ELECTIRIZER
	.2byte ITEM_MAGMARIZER
	.2byte ITEM_DUBIOUS_DISC
	.2byte ITEM_REAPER_CLOTH
	.2byte ITEM_PRISM_SCALE
	.2byte ITEM_WHIPPED_DREAM
	.2byte ITEM_SACHET
	.2byte ITEM_OVAL_STONE
	.2byte ITEM_DEEP_SEA_SCALE
	.2byte ITEM_DEEP_SEA_TOOTH
	.2byte ITEM_METAL_COAT
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_RAZOR_CLAW
	.2byte ITEM_RAZOR_FANG
	.2byte ITEM_NONE
	release
	end

	.align 2
CeladonCity_DepartmentStore_4F_MegaStones::
	.2byte ITEM_VENUSAURITE
	.2byte ITEM_CHARIZARDITE_X
	.2byte ITEM_CHARIZARDITE_Y
	.2byte ITEM_BLASTOISINITE
	.2byte ITEM_BEEDRILLITE
	.2byte ITEM_PIDGEOTITE
	.2byte ITEM_ALAKAZITE
	.2byte ITEM_SLOWBRONITE
	.2byte ITEM_GENGARITE
	.2byte ITEM_KANGASKHANITE
	.2byte ITEM_PINSIRITE
	.2byte ITEM_GYARADOSITE
	.2byte ITEM_AERODACTYLITE
	.2byte ITEM_MEWTWONITE_X
	.2byte ITEM_MEWTWONITE_Y
	.2byte ITEM_AMPHAROSITE
	.2byte ITEM_STEELIXITE
	.2byte ITEM_SCIZORITE
	.2byte ITEM_HERACRONITE
	.2byte ITEM_HOUNDOOMINITE
	.2byte ITEM_TYRANITARITE
	.2byte ITEM_SCEPTILITE
	.2byte ITEM_BLAZIKENITE
	.2byte ITEM_SWAMPERTITE
	.2byte ITEM_GARDEVOIRITE
	.2byte ITEM_SABLENITE
	.2byte ITEM_MAWILITE
	.2byte ITEM_AGGRONITE
	.2byte ITEM_MEDICHAMITE
	.2byte ITEM_MANECTITE
	.2byte ITEM_SHARPEDONITE
	.2byte ITEM_CAMERUPTITE
	.2byte ITEM_ALTARIANITE
	.2byte ITEM_BANETTITE
	.2byte ITEM_ABSOLITE
	.2byte ITEM_GLALITITE
	.2byte ITEM_SALAMENCITE
	.2byte ITEM_METAGROSSITE
	.2byte ITEM_LATIASITE
	.2byte ITEM_LATIOSITE
	.2byte ITEM_LOPUNNITE
	.2byte ITEM_GARCHOMPITE
	.2byte ITEM_LUCARIONITE
	.2byte ITEM_ABOMASITE
	.2byte ITEM_GALLADITE
	.2byte ITEM_AUDINITE
	.2byte ITEM_DIANCITE
	.2byte ITEM_CLEFABLITE
	.2byte ITEM_VICTREEBELITE
	.2byte ITEM_STARMINITE
	.2byte ITEM_DRAGONINITE
	.2byte ITEM_MEGANIUMITE
	.2byte ITEM_FERALIGITE
	.2byte ITEM_SKARMORITE
	.2byte ITEM_FROSLASSITE
	.2byte ITEM_EMBOARITE
	.2byte ITEM_EXCADRITE
	.2byte ITEM_SCOLIPITE
	.2byte ITEM_SCRAFTINITE
	.2byte ITEM_EELEKTROSSITE
	.2byte ITEM_CHANDELURITE
	.2byte ITEM_CHESNAUGHTITE
	.2byte ITEM_DELPHOXITE
	.2byte ITEM_GRENINJITE
	.2byte ITEM_PYROARITE
	.2byte ITEM_FLOETTITE
	.2byte ITEM_MALAMARITE
	.2byte ITEM_BARBARACITE
	.2byte ITEM_DRAGALGITE
	.2byte ITEM_HAWLUCHANITE
	.2byte ITEM_ZYGARDITE
	.2byte ITEM_DRAMPANITE
	.2byte ITEM_FALINKSITE
	.2byte ITEM_HEATRANITE
	.2byte ITEM_DARKRANITE
	.2byte ITEM_ZERAORITE
	.2byte ITEM_RAICHUNITE_X
	.2byte ITEM_RAICHUNITE_Y
	.2byte ITEM_CHIMECHITE
	.2byte ITEM_ABSOLITE_Z
	.2byte ITEM_STARAPTITE
	.2byte ITEM_GARCHOMPITE_Z
	.2byte ITEM_LUCARIONITE_Z
	.2byte ITEM_GOLURKITE
	.2byte ITEM_MEOWSTICITE
	.2byte ITEM_CRABOMINITE
	.2byte ITEM_GOLISOPITE
	.2byte ITEM_MAGEARNITE
	.2byte ITEM_SCOVILLAINITE
	.2byte ITEM_BAXCALIBRITE
	.2byte ITEM_TATSUGIRINITE
	.2byte ITEM_GLIMMORANITE
	.2byte ITEM_NONE
	release
	end

	.align 2
CeladonCity_DepartmentStore_4F_ZCrystals::
	.2byte ITEM_NORMALIUM_Z
	.2byte ITEM_FIRIUM_Z
	.2byte ITEM_WATERIUM_Z
	.2byte ITEM_ELECTRIUM_Z
	.2byte ITEM_GRASSIUM_Z
	.2byte ITEM_ICIUM_Z
	.2byte ITEM_FIGHTINIUM_Z
	.2byte ITEM_POISONIUM_Z
	.2byte ITEM_GROUNDIUM_Z
	.2byte ITEM_FLYINIUM_Z
	.2byte ITEM_PSYCHIUM_Z
	.2byte ITEM_BUGINIUM_Z
	.2byte ITEM_ROCKIUM_Z
	.2byte ITEM_GHOSTIUM_Z
	.2byte ITEM_DRAGONIUM_Z
	.2byte ITEM_DARKINIUM_Z
	.2byte ITEM_STEELIUM_Z
	.2byte ITEM_FAIRIUM_Z
	.2byte ITEM_PIKANIUM_Z
	.2byte ITEM_EEVIUM_Z
	.2byte ITEM_SNORLIUM_Z
	.2byte ITEM_MEWNIUM_Z
	.2byte ITEM_DECIDIUM_Z
	.2byte ITEM_INCINIUM_Z
	.2byte ITEM_PRIMARIUM_Z
	.2byte ITEM_LYCANIUM_Z
	.2byte ITEM_MIMIKIUM_Z
	.2byte ITEM_KOMMONIUM_Z
	.2byte ITEM_TAPUNIUM_Z
	.2byte ITEM_SOLGANIUM_Z
	.2byte ITEM_LUNALIUM_Z
	.2byte ITEM_MARSHADIUM_Z
	.2byte ITEM_ALORAICHIUM_Z
	.2byte ITEM_PIKASHUNIUM_Z
	.2byte ITEM_ULTRANECROZIUM_Z
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_4F_Text_ChoiceGifts::
	.string "Gifts & stones$"
	.string "{JPN}{JPN}おくりもの・いし$"

CeladonCity_DepartmentStore_4F_Text_ChoiceEvolution::
	.string "Evolution items$"
	.string "{JPN}{JPN}しんかの　どうぐ$"

CeladonCity_DepartmentStore_4F_Text_ChoiceMegaStones::
	.string "Mega Stones$"
	.string "{JPN}{JPN}メガストーン$"

CeladonCity_DepartmentStore_4F_Text_ChoiceZCrystals::
	.string "Z-Crystals$"
	.string "{JPN}{JPN}Zクリスタル$"

CeladonCity_DepartmentStore_4F_Text_ChoiceCancel::
	.string "Cancel$"
	.string "{JPN}{JPN}やめる$"

CeladonCity_DepartmentStore_4F_Text_GettingPokeDollAsPresent::
	.string "I'm getting a present for my\n"
	.string "girlfriend.\p"
	.string "I guess a POKé DOLL will be it.\n"
	.string "It's the popular thing.$"
	.string "{JPN}{JPN}かのじょに　プレゼントを　かうんだ\p"
	.string "ピッピにんぎょうに　しようかな　いま\n"
	.string "にんきだしね$"

CeladonCity_DepartmentStore_4F_Text_CanRunAwayWithPokeDoll::
	.string "I heard something useful.\p"
	.string "If a wild POKéMON appears, you can\n"
	.string "distract it with a POKé DOLL.\p"
	.string "You can run away while the wild\n"
	.string "POKéMON is distracted.$"
	.string "{JPN}{JPN}いいこと　きいちゃった\p"
	.string "やせいの　ポケモンが　でてきたら\n"
	.string "ピッピにんぎょうで　きを　そらせるんだって\p"
	.string "やせいの　ポケモンが　きを　とられてる　すきに\n"
	.string "にげられるのよ$"

CeladonCity_DepartmentStore_4F_Text_FloorSign::
	.string "Express yourself with gifts!\n"
	.string "4F: WISE MAN GIFTS\p"
	.string "Evolution Special!\n"
	.string "Element STONES on sale now!$"
	.string "{JPN}{JPN}おくりもので　きもちを　つたえよう！　4F:\n"
	.string "かしこい　おくりもの\p"
	.string "しんか　とくしゅう！　しんかのいし\n"
	.string "はつばいちゅう！$"
