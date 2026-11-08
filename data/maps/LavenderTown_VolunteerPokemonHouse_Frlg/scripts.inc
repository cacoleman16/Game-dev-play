LavenderTown_VolunteerPokemonHouse_Frlg_MapScripts::
	.byte 0

LavenderTown_VolunteerPokemonHouse_EventScript_MrFuji::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POKE_FLUTE, LavenderTown_VolunteerPokemonHouse_EventScript_AlreadyHavePokeFlute
	msgbox LavenderTown_VolunteerPokemonHouse_Text_IdLikeYouToHaveThis
	checkitemspace ITEM_POKE_FLUTE
	goto_if_eq VAR_RESULT, FALSE, LavenderTown_VolunteerPokemonHouse_EventScript_NoRoomForPokeFlute
	setflag FLAG_GOT_POKE_FLUTE
	giveitem_msg LavenderTown_VolunteerPokemonHouse_Text_ReceivedPokeFluteFromMrFuji, ITEM_POKE_FLUTE, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox LavenderTown_VolunteerPokemonHouse_Text_ExplainPokeFlute
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_AlreadyHavePokeFlute::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_HasPokeFluteHelpedYou
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_NoRoomForPokeFlute::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MustMakeRoomForThis
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_LittleGirl::
	lock
	faceplayer
	famechecker FAMECHECKER_MRFUJI, 0
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MrFujiLooksAfterOrphanedMons
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_LittleBoy::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MonsNiceToHug, MSGBOX_NPC
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Youngster::
	lock
	faceplayer
	goto_if_set FLAG_RESCUED_MR_FUJI, LavenderTown_VolunteerPokemonHouse_EventScript_YoungsterFujiBack
	msgbox LavenderTown_VolunteerPokemonHouse_Text_WhereDidMrFujiGo
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_YoungsterFujiBack::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MrFujiWasPrayingForCubonesMother
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Nidorino::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_NIDORINO, CRY_MODE_NORMAL
	msgbox LavenderTown_VolunteerPokemonHouse_Text_Nidorino
	waitmoncry
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Psyduck::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PSYDUCK, CRY_MODE_NORMAL
	msgbox LavenderTown_VolunteerPokemonHouse_Text_Psyduck
	waitmoncry
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_PokemonFanMagazine::
	lockall
	famechecker FAMECHECKER_MRFUJI, 2
	msgbox LavenderTown_VolunteerPokemonHouse_Text_GrandPrizeDrawingClipped
	releaseall
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Bookshelf::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_PokemonMagazinesLineShelf, MSGBOX_SIGN
	end

LavenderTown_PokemonCenter_1F_Text_HearMrFujiNotFromAroundHere::
	.string "I recently moved to this town.\p"
	.string "I hear that MR. FUJI's not from\n"
	.string "these parts originally, either.$"
	.string "{JPN}{JPN}さいきん　この　まちに　ひっこして　きたんだ。\p"
	.string "フジろうじんも　もともと　この　あたりの\n"
	.string "ひとじゃ　ないらしいよ。$"

LavenderTown_VolunteerPokemonHouse_Text_WhereDidMrFujiGo::
	.string "That's odd, MR. FUJI isn't here.\n"
	.string "Where'd he go?$"
	.string "{JPN}{JPN}へんね　フジろうじんが　いないわ。　どこへ\n"
	.string "いったのかしら？$"

LavenderTown_VolunteerPokemonHouse_Text_MrFujiWasPrayingForCubonesMother::
	.string "MR. FUJI had been praying alone\n"
	.string "for CUBONE's mother.$"
	.string "{JPN}{JPN}フジろうじんは　ひとりで　カラカラの\n"
	.string "おかあさんの　ために　いのって　いたの。$"

LavenderTown_VolunteerPokemonHouse_Text_MrFujiLooksAfterOrphanedMons::
	.string "This is really MR. FUJI's house.\p"
	.string "He's really kind.\p"
	.string "He looks after abandoned and\n"
	.string "orphaned POKéMON.$"
	.string "{JPN}{JPN}ここは　ほんとうは　フジろうじんの　いえ\n"
	.string "なんだ。\p"
	.string "とても　やさしい　ひと　なんだよ。\p"
	.string "すてられたり　おやを　なくしたり　した\n"
	.string "ポケモンの　せわを　しているんだ。$"

LavenderTown_VolunteerPokemonHouse_Text_MonsNiceToHug::
	.string "It's so warm!\n"
	.string "POKéMON are so nice to hug.$"
	.string "{JPN}{JPN}あったかーい！　ポケモンって　だきしめると\n"
	.string "きもちいい！$"

LavenderTown_VolunteerPokemonHouse_Text_Nidorino::
	.string "NIDORINO: Gaoo!$"
	.string "{JPN}{JPN}ニドリーノ:　ガオー！$"

LavenderTown_VolunteerPokemonHouse_Text_Psyduck::
	.string "PSYDUCK: Gwappa!$"
	.string "{JPN}{JPN}コダック:　グワッパ！$"

LavenderTown_VolunteerPokemonHouse_Text_IdLikeYouToHaveThis::
	.string "MR. FUJI: {PLAYER}…\p"
	.string "Your POKéDEX quest is one that\n"
	.string "requires strong dedication.\p"
	.string "Without deep love for POKéMON,\n"
	.string "your quest may fail.\p"
	.string "I'm not sure if this will help you,\n"
	.string "but I'd like you to have it.$"
	.string "{JPN}{JPN}フジろうじん:　{PLAYER}‥\p"
	.string "ずかんを　かんせい　させるには　つよい　いしが\n"
	.string "ひつようじゃ。\p"
	.string "ポケモンへの　ふかい　あいじょうが　なければ\n"
	.string "その　たびは　うまく　いかんかも　しれん。\p"
	.string "やくに　たつか　わからんが　これを　うけとって\n"
	.string "ほしい。$"

LavenderTown_VolunteerPokemonHouse_Text_ReceivedPokeFluteFromMrFuji::
	.string "{PLAYER} received a POKé FLUTE\n"
	.string "from MR. FUJI.$"
	.string "{JPN}{JPN}{PLAYER}は　フジろうじんから\n"
	.string "ポケモンのふえを　もらった！$"

LavenderTown_VolunteerPokemonHouse_Text_ExplainPokeFlute::
	.string "Upon hearing the POKé FLUTE,\n"
	.string "sleeping POKéMON will spring awake.\p"
	.string "Try using it on POKéMON that are\n"
	.string "sleeping obstacles.$"
	.string "{JPN}{JPN}ポケモンのふえを　きくと　ねむっている\n"
	.string "ポケモンは　とびおきる。\p"
	.string "みちを　ふさいで　ねむっている　ポケモンに\n"
	.string "つかって　みなさい。$"

LavenderTown_VolunteerPokemonHouse_Text_MustMakeRoomForThis::
	.string "You must make room for this!$"
	.string "{JPN}{JPN}もちものを　あけて　おきなさい！$"

LavenderTown_VolunteerPokemonHouse_Text_HasPokeFluteHelpedYou::
	.string "MR. FUJI: Has my POKé FLUTE\n"
	.string "helped you?$"
	.string "{JPN}{JPN}フジろうじん:　わしの　ポケモンのふえは　やくに\n"
	.string "たったかね？$"

LavenderTown_VolunteerPokemonHouse_Text_GrandPrizeDrawingClipped::
	.string "POKéMON FAN MAGAZINE\n"
	.string "Monthly Grand Prize Drawing!\p"
	.string "The application form is…\p"
	.string "Gone! It's been clipped out.\n"
	.string "Someone must have applied already.$"
	.string "{JPN}{JPN}げっかん　ポケモンファン\n"
	.string "まいつきの　だい　ちゅうせんかい！\p"
	.string "おうぼようしは‥\p"
	.string "ない！　きりとられている。　だれかが　もう\n"
	.string "おうぼ　したらしい。$"

LavenderTown_VolunteerPokemonHouse_Text_PokemonMagazinesLineShelf::
	.string "POKéMON magazines line the shelf.\p"
	.string "POKéMON INSIDER…\p"
	.string "POKéMON FAN…$"
	.string "{JPN}{JPN}たなに　ポケモンの　ざっしが　ならんでいる。\p"
	.string "ポケモン　インサイダー‥\p"
	.string "ポケモンファン‥$"

