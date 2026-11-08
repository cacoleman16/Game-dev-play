@ [QoL] STARFALL: this hack's story, tying Kanto and Hoenn together.
@ See docs/STARFALL.md for the plot. Each scene is called from the map's own
@ script; cutscene characters are map objects that only appear here (each map
@ hides them on load: QoL_EventScript_Starfall*OnTransition).

@ ---------------------------------------------------------------------------
@ Chapter 0: Oak's Lab. Called from PalletTown_ProfessorOaksLab_ChooseStarterScene
@ once Oak and the player have walked in (player at 6,4; Oak 6,3; Blue 5,4).
@ ---------------------------------------------------------------------------
.equ QOL_RIVAL_STARTER_SPECIES, VAR_TEMP_3

QoL_EventScript_StarfallPrologue::
	msgbox PalletTown_ProfessorOaksLab_Text_RivalFedUpWithWaiting
	closemessage
	delay 40
	msgbox QoL_Text_OakLookAtMeteorite
	closemessage
	applymovement LOCALID_OAKS_LAB_PROF_OAK, Common_Movement_FaceRight
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	playse SE_M_DETECT
	addobject LOCALID_STARFALL_METEORITE
	waitse
	msgbox QoL_Text_OakSentByStevenStone
	closemessage
	delay 30
	playse SE_M_ROCK_THROW
	waitse
	msgbox QoL_Text_MeteoriteGlowing
	closemessage
	playse SE_M_REFLECT
	fadescreen FADE_TO_WHITE
	removeobject LOCALID_STARFALL_METEORITE
	addobject LOCALID_STARFALL_BELDUM
	removeobject LOCALID_BULBASAUR_BALL
	removeobject LOCALID_SQUIRTLE_BALL
	addobject LOCALID_STARFALL_BULBASAUR
	addobject LOCALID_STARFALL_SQUIRTLE
	delay 20
	fadescreen FADE_FROM_WHITE
	playmoncry SPECIES_BELDUM, CRY_MODE_ENCOUNTER
	waitmoncry
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox QoL_Text_BallsBurstOpen
	closemessage
	playmoncry SPECIES_BULBASAUR, CRY_MODE_NORMAL
	applymovement LOCALID_STARFALL_BULBASAUR, QoL_Movement_StarfallBulbasaurFlees
	waitmovement 0
	removeobject LOCALID_STARFALL_BULBASAUR
	playmoncry SPECIES_SQUIRTLE, CRY_MODE_NORMAL
	applymovement LOCALID_STARFALL_SQUIRTLE, QoL_Movement_StarfallSquirtleFlees
	waitmovement 0
	removeobject LOCALID_STARFALL_SQUIRTLE
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_OakTheyRanOff
	closemessage
	applymovement LOCALID_STARFALL_BELDUM, Common_Movement_FaceLeft
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox QoL_Text_OakItsABeldum
	closemessage
	@ Beldum joins: the player's partner for the whole story.
	removeobject LOCALID_STARFALL_BELDUM
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_PALLET_LADY_NOT_BLOCKING_SIGN
	setflag FLAG_QOL_STARFALL_PROLOGUE
	givemon SPECIES_BELDUM, 5, ITEM_NONE, ITEM_MOON_BALL, NATURE_ADAMANT, move1 = MOVE_TAKE_DOWN, move2 = MOVE_METAL_CLAW, move3 = MOVE_CONFUSION, move4 = MOVE_IRON_DEFENSE
	setvar VAR_KANTO_STARTER, 0 @ Blue gets the Charmander line
	bufferspeciesname STR_VAR_1, SPECIES_BELDUM
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message PalletTown_ProfessorOaksLab_Text_ReceivedMonFromOak
	waitmessage
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	waitfanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, QoL_EventScript_StarfallNameBeldum
	closemessage
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_OakTakeCareOfBeldum
	closemessage
	@ Blue takes the last Poké Ball.
	setvar QOL_RIVAL_STARTER_SPECIES, SPECIES_CHARMANDER
	call_if_set FLAG_QOL_RANDOMIZER, QoL_EventScript_StarfallRandomizeRival
	bufferspeciesname STR_VAR_1, QOL_RIVAL_STARTER_SPECIES
	applymovement LOCALID_OAKS_LAB_RIVAL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox QoL_Text_RivalWhatAboutMe
	msgbox QoL_Text_OakOneLeft
	closemessage
	applymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_RivalWalksToCharmander
	waitmovement 0
	msgbox QoL_Text_RivalTakesLastBall
	removeobject LOCALID_CHARMANDER_BALL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message PalletTown_ProfessorOaksLab_Text_RivalReceivedMonFromOak
	waitmessage
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	waitfanfare
	call EventScript_RestorePrevTextColor
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 3
	call_if_set FLAG_OPENED_START_MENU, PalletTown_ProfessorOaksLab_EventScript_ReadyEndSignLadyScene
	releaseall
	end

QoL_EventScript_StarfallNameBeldum::
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	return

QoL_EventScript_StarfallRandomizeRival::
	copyvar VAR_0x8004, QOL_RIVAL_STARTER_SPECIES
	special QoL_RandomizeSpecialVar
	copyvar QOL_RIVAL_STARTER_SPECIES, VAR_RESULT
	return

@ The cutscene objects only appear during the scene.
QoL_EventScript_StarfallLabOnTransition::
	setflag FLAG_QOL_HIDE_STARFALL_LAB
	return

QoL_Movement_StarfallBulbasaurFlees::
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

QoL_Movement_StarfallSquirtleFlees::
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

@ ---------------------------------------------------------------------------
@ Pallet Town: Mom talks about Dad in Hoenn; after the move she's gone.
@ ---------------------------------------------------------------------------
QoL_EventScript_MomTalksAboutHoenn::
	goto_if_ne VAR_LITTLEROOT_INTRO_STATE, 0, Common_EventScript_NopReturn
	msgbox QoL_Text_MomDadIsInHoenn
	return

QoL_EventScript_PalletHouseOnTransition::
	call_if_ne VAR_LITTLEROOT_INTRO_STATE, 0, QoL_EventScript_HidePalletMom
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 0, QoL_EventScript_ShowPalletMom
	return

QoL_EventScript_HidePalletMom::
	setflag FLAG_QOL_HIDE_PALLET_MOM
	return

QoL_EventScript_ShowPalletMom::
	clearflag FLAG_QOL_HIDE_PALLET_MOM
	return

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------
QoL_Text_OakLookAtMeteorite::
	.string "OAK: Now, {PLAYER}, before\n"
	.string "anything else, look at this!$"

QoL_Text_OakSentByStevenStone::
	.string "OAK: Last night's STARFALL…\n"
	.string "Did you see it?\p"
	.string "Meteors rained over KANTO and\n"
	.string "HOENN on the very same night!\p"
	.string "A friend of mine in HOENN, a\n"
	.string "stone collector named STEVEN\l"
	.string "STONE, sent me this meteorite.\p"
	.string "It has been humming ever since\n"
	.string "the STARFALL began…$"

QoL_Text_MeteoriteGlowing::
	.string "OAK: Huh? It's glowing!$"

QoL_Text_BallsBurstOpen::
	.string "The flash startled the POKéMON on\n"
	.string "the table!\p"
	.string "Two POKé BALLS burst open!$"

QoL_Text_OakTheyRanOff::
	.string "OAK: BULBASAUR! SQUIRTLE!\n"
	.string "Come back!\p"
	.string "…They're gone. The flash must\n"
	.string "have frightened them.\p"
	.string "Don't worry. BULBASAUR loves the\n"
	.string "forest, and SQUIRTLE the water.\l"
	.string "They'll be safe out there.\p"
	.string "Who knows? You might meet them\n"
	.string "again on your journey!$"

QoL_Text_OakItsABeldum::
	.string "OAK: And that… That POKéMON came\n"
	.string "out of the meteorite!\p"
	.string "It's a BELDUM! They live in\n"
	.string "HOENN, and they're said to be\l"
	.string "drawn to strong hearts.\p"
	.string "Look, {PLAYER}! It's floating\n"
	.string "right up to you!\p"
	.string "It came all the way from the sky,\n"
	.string "and it has chosen you!$"

QoL_Text_OakTakeCareOfBeldum::
	.string "OAK: Take good care of BELDUM,\n"
	.string "{PLAYER}.\p"
	.string "When you can, show it to STEVEN\n"
	.string "in HOENN. He'll want to see what\l"
	.string "his meteorite turned into!$"

QoL_Text_RivalWhatAboutMe::
	.string "{RIVAL}: Hey! Gramps!\n"
	.string "What about me?$"

QoL_Text_OakOneLeft::
	.string "OAK: Be patient, {RIVAL}.\n"
	.string "There's still one POKé BALL on\l"
	.string "the table. It's yours.$"

QoL_Text_RivalTakesLastBall::
	.string "{RIVAL}: A floating hunk of metal?\n"
	.string "Ha!\p"
	.string "I'll take this one. It's way\n"
	.string "tougher than yours, {PLAYER}!$"

QoL_Text_MomDadIsInHoenn::
	.string "MOM: Oh, and {PLAYER}…\p"
	.string "Your father wrote again. He's a\n"
	.string "GYM LEADER in HOENN now, in a\l"
	.string "place called PETALBURG CITY.\p"
	.string "Once you have an S.S. TICKET, we'll\n"
	.string "sail to HOENN together on the\l"
	.string "S.S. TIDAL from VERMILION CITY.\p"
	.string "The movers have packed our\n"
	.string "things already!$"

QoL_Text_MomsNoteOnTV::
	.string "There's a note taped to the TV.\p"
	.string "“{PLAYER}, we've moved to\n"
	.string "LITTLEROOT TOWN in HOENN.\l"
	.string "Come home anytime! Love, MOM”$"

@ ---------------------------------------------------------------------------
@ Shared: Deneb's first line depends on whether you've met her before.
@ ---------------------------------------------------------------------------
QoL_EventScript_DenebGreeting::
	goto_if_set FLAG_QOL_STARFALL_SILPH, QoL_EventScript_DenebGreetingAgain
	goto_if_set FLAG_QOL_STARFALL_CINNABAR, QoL_EventScript_DenebGreetingAgain
	goto_if_set FLAG_QOL_STARFALL_MT_CHIMNEY, QoL_EventScript_DenebGreetingAgain
	msgbox QoL_Text_DenebFirstMeeting
	return

QoL_EventScript_DenebGreetingAgain::
	msgbox QoL_Text_DenebAgain
	return

@ ---------------------------------------------------------------------------
@ Chapter 1: Mt. Moon B2F, right after the player takes a fossil.
@ ---------------------------------------------------------------------------
QoL_EventScript_StarfallMtMoon::
	goto_if_set FLAG_QOL_STARFALL_MT_MOON, Common_EventScript_NopReturn
	getplayerxy VAR_0x8004, VAR_0x8005
	call_if_eq VAR_0x8004, 14, QoL_EventScript_MtMoonAltairRight
	delay 20
	playse SE_PIN
	addobject LOCALID_STARFALL_MT_MOON_ALTAIR
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_STARFALL_MT_MOON_ALTAIR, QoL_Movement_WalkUp4
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_AltairMtMoonIntro
	trainerbattle_no_intro TRAINER_NOVA_ALTAIR_MT_MOON, QoL_Text_AltairMtMoonDefeat
	msgbox QoL_Text_AltairMtMoonAfter
	closemessage
	applymovement LOCALID_STARFALL_MT_MOON_ALTAIR, QoL_Movement_WalkDown4
	waitmovement 0
	removeobject LOCALID_STARFALL_MT_MOON_ALTAIR
	fadedefaultbgm
	setflag FLAG_QOL_STARFALL_MT_MOON
	return

QoL_EventScript_MtMoonAltairRight::
	setobjectxyperm LOCALID_STARFALL_MT_MOON_ALTAIR, 14, 13
	return

QoL_EventScript_StarfallMtMoonOnTransition::
	setflag FLAG_QOL_HIDE_NOVA_MT_MOON
	return

@ ---------------------------------------------------------------------------
@ Chapter 2: Silph Co. 11F, after Giovanni leaves.
@ ---------------------------------------------------------------------------
QoL_EventScript_StarfallSilph::
	goto_if_set FLAG_QOL_STARFALL_SILPH, Common_EventScript_NopReturn
	delay 30
	addobject LOCALID_STARFALL_SILPH_DENEB
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_STARFALL_SILPH_DENEB, QoL_Movement_WalkDown2
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	call QoL_EventScript_DenebGreeting
	msgbox QoL_Text_DenebSilph
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STARFALL_SILPH_DENEB
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	setflag FLAG_QOL_STARFALL_SILPH
	return

QoL_EventScript_StarfallSilphOnTransition::
	setflag FLAG_QOL_HIDE_NOVA_SILPH
	return

@ ---------------------------------------------------------------------------
@ Chapter 3: Cinnabar Lab, the fossil room. Called when the player first
@ talks to the fossil scientist.
@ ---------------------------------------------------------------------------
QoL_EventScript_StarfallCinnabar::
	goto_if_set FLAG_QOL_STARFALL_CINNABAR, Common_EventScript_NopReturn
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_CinnabarScientistShards
	closemessage
	playse SE_DOOR
	addobject LOCALID_STARFALL_CINNABAR_GRUNT
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_STARFALL_CINNABAR_GRUNT, Common_Movement_WalkRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox QoL_Text_CinnabarGruntIntro
	trainerbattle_no_intro TRAINER_NOVA_GRUNT_CINNABAR, QoL_Text_CinnabarGruntDefeat
	addobject LOCALID_STARFALL_CINNABAR_DENEB
	applymovement LOCALID_STARFALL_CINNABAR_DENEB, Common_Movement_WalkRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	call QoL_EventScript_DenebGreeting
	msgbox QoL_Text_DenebCinnabarIntro
	trainerbattle_no_intro TRAINER_NOVA_DENEB_CINNABAR, QoL_Text_DenebCinnabarDefeat
	msgbox QoL_Text_DenebCinnabarAfter
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STARFALL_CINNABAR_GRUNT
	removeobject LOCALID_STARFALL_CINNABAR_DENEB
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_CinnabarScientistDeoxys
	closemessage
	setflag FLAG_QOL_STARFALL_CINNABAR
	return

QoL_EventScript_StarfallCinnabarOnTransition::
	setflag FLAG_QOL_HIDE_NOVA_CINNABAR
	return

@ ---------------------------------------------------------------------------
@ Chapter 5: Meteor Falls 1F, after Team Magma takes Prof. Cozmo's meteorite.
@ ---------------------------------------------------------------------------
QoL_EventScript_StarfallMeteorFalls::
	goto_if_set FLAG_QOL_STARFALL_METEOR_FALLS, Common_EventScript_NopReturn
	delay 20
	addobject LOCALID_STARFALL_METEOR_FALLS_ALTAIR
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_STARFALL_METEOR_FALLS_ALTAIR, QoL_Movement_WalkUp3
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_AltairMeteorFallsIntro
	trainerbattle_no_intro TRAINER_NOVA_ALTAIR_METEOR_FALLS, QoL_Text_AltairMeteorFallsDefeat
	msgbox QoL_Text_AltairMeteorFallsAfter
	closemessage
	applymovement LOCALID_STARFALL_METEOR_FALLS_ALTAIR, QoL_Movement_WalkDown3
	waitmovement 0
	removeobject LOCALID_STARFALL_METEOR_FALLS_ALTAIR
	fadedefaultbgm
	setflag FLAG_QOL_STARFALL_METEOR_FALLS
	return

QoL_EventScript_StarfallMeteorFallsOnTransition::
	setflag FLAG_QOL_HIDE_NOVA_METEOR_FALLS
	return

@ ---------------------------------------------------------------------------
@ Chapter 6: Mt. Chimney, after Maxie and Archie leave.
@ ---------------------------------------------------------------------------
QoL_EventScript_StarfallMtChimney::
	goto_if_set FLAG_QOL_STARFALL_MT_CHIMNEY, Common_EventScript_NopReturn
	delay 20
	addobject LOCALID_STARFALL_MT_CHIMNEY_DENEB
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_STARFALL_MT_CHIMNEY_DENEB, QoL_Movement_DenebChimneyApproach
	waitmovement 0
	applymovement LOCALID_STARFALL_MT_CHIMNEY_DENEB, Common_Movement_FacePlayer
	applymovement LOCALID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	call QoL_EventScript_DenebGreeting
	msgbox QoL_Text_DenebChimneyIntro
	trainerbattle_no_intro TRAINER_NOVA_DENEB_MT_CHIMNEY, QoL_Text_DenebChimneyDefeat
	msgbox QoL_Text_DenebChimneyAfter
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STARFALL_MT_CHIMNEY_DENEB
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	setflag FLAG_QOL_STARFALL_MT_CHIMNEY
	return

QoL_EventScript_StarfallMtChimneyOnTransition::
	setflag FLAG_QOL_HIDE_NOVA_MT_CHIMNEY
	return

QoL_Movement_DenebChimneyApproach::
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	step_end

@ ---------------------------------------------------------------------------
@ Chapter 7: Mossdeep Space Center 2F, after Maxie and Tabitha. Steven is
@ still there.
@ ---------------------------------------------------------------------------
QoL_EventScript_StarfallSpaceCenter::
	goto_if_set FLAG_QOL_STARFALL_SPACE_CENTER, Common_EventScript_NopReturn
	delay 20
	addobject LOCALID_STARFALL_SPACE_CENTER_ALTAIR
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	applymovement LOCALID_STARFALL_SPACE_CENTER_ALTAIR, QoL_Movement_WalkLeft3
	waitmovement 0
	applymovement LOCALID_STARFALL_SPACE_CENTER_ALTAIR, Common_Movement_FacePlayer
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_AltairSpaceCenterIntro
	trainerbattle_no_intro TRAINER_NOVA_ALTAIR_SPACE_CENTER, QoL_Text_AltairSpaceCenterDefeat
	msgbox QoL_Text_AltairSpaceCenterAfter
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STARFALL_SPACE_CENTER_ALTAIR
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	msgbox QoL_Text_StevenAboutBeldum
	closemessage
	setflag FLAG_QOL_STARFALL_SPACE_CENTER
	return

QoL_EventScript_StarfallSpaceCenterOnTransition::
	setflag FLAG_QOL_HIDE_NOVA_SPACE_CENTER
	return

@ ---------------------------------------------------------------------------
@ Chapter 8: Steven's house, Mossdeep City
@ ---------------------------------------------------------------------------
@ After Hoenn's Hall of Fame Steven's letter explains the finale.
QoL_EventScript_StarfallStevensLetter::
	msgbox QoL_Text_StevensLetter
	setflag FLAG_QOL_STARFALL_LETTER
	return

@ The Poké Ball on Steven's table: in Emerald a Beldum; here, a Metagrossite
@ for the Beldum that came from his meteorite.
QoL_EventScript_StarfallStevensGift::
	lockall
	goto_if_set FLAG_QOL_STARFALL_STEVEN_GIFT, QoL_EventScript_StevensGiftTaken
	msgbox QoL_Text_StevensGiftNote
	giveitem ITEM_METAGROSSITE
	goto_if_eq VAR_RESULT, FALSE, QoL_EventScript_StevensGiftBagFull
	setflag FLAG_QOL_STARFALL_STEVEN_GIFT
	removeobject LOCALID_STEVENS_HOUSE_BALL
	setflag FLAG_RECEIVED_BELDUM
	releaseall
	end

QoL_EventScript_StevensGiftTaken::
	releaseall
	end

QoL_EventScript_StevensGiftBagFull::
	msgbox gText_TheBagIsFull
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Movements
@ ---------------------------------------------------------------------------
QoL_Movement_WalkUp4::
	walk_up
QoL_Movement_WalkUp3::
	walk_up
	walk_up
	walk_up
	step_end

QoL_Movement_WalkDown4::
	walk_down
QoL_Movement_WalkDown3::
	walk_down
QoL_Movement_WalkDown2::
	walk_down
	walk_down
	step_end

QoL_Movement_WalkLeft3::
	walk_left
	walk_left
	walk_left
	step_end

@ ---------------------------------------------------------------------------
@ Text: Team Nova in Kanto
@ ---------------------------------------------------------------------------
QoL_Text_AltairMtMoonIntro::
	.string "???: So TEAM ROCKET finally dug\n"
	.string "down to the old crater.\l"
	.string "Good work, little thieves.\p"
	.string "Oh? You're no ROCKET.\n"
	.string "And that BELDUM…\p"
	.string "A child of the STARFALL! Who'd\n"
	.string "have thought?\p"
	.string "I'm ALTAIR, of TEAM NOVA.\n"
	.string "I hunt what falls from the sky.\p"
	.string "The MOON STONES here came down\n"
	.string "long ago. ROCKET wants the fossils.\l"
	.string "We want the STAR SHARD beneath.\p"
	.string "Now, let's see if that BELDUM\n"
	.string "remembers where it came from!$"

QoL_Text_AltairMtMoonDefeat::
	.string "It moves like it remembers the\n"
	.string "sky. Interesting.$"

QoL_Text_AltairMtMoonAfter::
	.string "ALTAIR: Keep it, then. For now.\p"
	.string "The STARFALL isn't over, kid.\n"
	.string "When the sky calls that BELDUM\l"
	.string "home, you'll understand.\p"
	.string "Tell ROCKET thanks for the\n"
	.string "digging!$"

QoL_Text_DenebFirstMeeting::
	.string "???: Hehe… So you're the one with\n"
	.string "the STARFALL BELDUM.\p"
	.string "I'm DENEB of TEAM NOVA.\n"
	.string "ALTAIR told me all about you.$"

QoL_Text_DenebAgain::
	.string "DENEB: You again, darling? And\n"
	.string "that BELDUM of yours, too.$"

QoL_Text_DenebSilph::
	.string "DENEB: GIOVANNI left in such a\n"
	.string "hurry. What a pity.\p"
	.string "Did you think TEAM ROCKET paid\n"
	.string "for all this on its own?\p"
	.string "Our leader funded every floor of\n"
	.string "this takeover. In return, we get…\p"
	.string "…this. SILPH's MASTER BALL data.\n"
	.string "A ball that catches anything.\p"
	.string "Even something that comes down\n"
	.string "from the stars. Ciao!$"

QoL_Text_CinnabarScientistShards::
	.string "Is that a BELDUM? From the\n"
	.string "STARFALL? Remarkable!\p"
	.string "We've been studying STAR SHARDS,\n"
	.string "the fragments the meteors left.\p"
	.string "The DNA inside them is like\n"
	.string "nothing on this planet…$"

QoL_Text_CinnabarGruntIntro::
	.string "TEAM NOVA! Nobody move!\p"
	.string "Hand over the STAR SHARDS!\n"
	.string "You, kid! Out of the way!$"

QoL_Text_CinnabarGruntDefeat::
	.string "The stars weren't with me…$"

QoL_Text_DenebCinnabarIntro::
	.string "DENEB: Those shards belong to\n"
	.string "TEAM NOVA. We paid for them,\l"
	.string "one way or another.\p"
	.string "Shall we dance?$"

QoL_Text_DenebCinnabarDefeat::
	.string "My, my. That BELDUM is quite the\n"
	.string "bodyguard.$"

QoL_Text_DenebCinnabarAfter::
	.string "DENEB: No matter. My grunt already\n"
	.string "slipped out with one shard.\p"
	.string "One is all we needed from KANTO.\n"
	.string "Ta-ta!$"

QoL_Text_CinnabarScientistDeoxys::
	.string "They took a sample… but I still\n"
	.string "have our results.\p"
	.string "The DNA in the shards matches a\n"
	.string "POKéMON sighted far to the south,\l"
	.string "on a place called BIRTH ISLAND.\p"
	.string "DEOXYS. A POKéMON from space.\p"
	.string "If TEAM NOVA is collecting the\n"
	.string "shards, they must be trying to\l"
	.string "call it down…\p"
	.string "Please, take care of that BELDUM.\n"
	.string "Now, how can I help you?$"

@ ---------------------------------------------------------------------------
@ Text: Team Nova in Hoenn
@ ---------------------------------------------------------------------------
QoL_Text_AltairMeteorFallsIntro::
	.string "ALTAIR: Well, well. The BELDUM kid,\n"
	.string "all the way out in HOENN.\p"
	.string "Did you see that? MAGMA thinks\n"
	.string "it stole that meteorite for its\l"
	.string "own little volcano plan.\p"
	.string "Who do you think told MAXIE where\n"
	.string "to find PROF. COZMO?\p"
	.string "Once MAGMA's machine has woken\n"
	.string "the meteorite up, TEAM NOVA will\l"
	.string "take it back. That's the deal.\p"
	.string "Now. Round two!$"

QoL_Text_AltairMeteorFallsDefeat::
	.string "Bah! It's grown.$"

QoL_Text_AltairMeteorFallsAfter::
	.string "ALTAIR: Go ahead, chase MAGMA up\n"
	.string "MT. CHIMNEY.\p"
	.string "Every step they take is a step\n"
	.string "for us!$"

QoL_Text_DenebChimneyIntro::
	.string "DENEB: You beat MAXIE before his\n"
	.string "machine could finish its work.\p"
	.string "That meteorite was supposed to\n"
	.string "be ours afterward. You're bad for\l"
	.string "business, darling!$"

QoL_Text_DenebChimneyDefeat::
	.string "Ugh! Not my best investment.$"

QoL_Text_DenebChimneyAfter::
	.string "DENEB: Keep your little rock, then.\p"
	.string "There are plenty more in the sky.\n"
	.string "And soon they'll all come to us.$"

QoL_Text_AltairSpaceCenterIntro::
	.string "ALTAIR: MAGMA wanted the rocket\n"
	.string "fuel. Such small dreams.\p"
	.string "TEAM NOVA came for the flight\n"
	.string "data. The path of the next\l"
	.string "STARFALL!\p"
	.string "You and that BELDUM again… Fine.\n"
	.string "Let's settle this!$"

QoL_Text_AltairSpaceCenterDefeat::
	.string "…It's ready to go home. I can feel\n"
	.string "it.$"

QoL_Text_AltairSpaceCenterAfter::
	.string "ALTAIR: I already sent the data to\n"
	.string "our leader. VEGA knows where the\l"
	.string "next STARFALL will land.\p"
	.string "When it does, the sky will answer\n"
	.string "her. Not you!$"

QoL_Text_StevenAboutBeldum::
	.string "STEVEN: TEAM NOVA… So they're the\n"
	.string "ones who've been buying meteorites\l"
	.string "all over HOENN.\p"
	.string "And that BELDUM… That's the\n"
	.string "meteorite I sent PROF. OAK!\p"
	.string "It hatched during the STARFALL and\n"
	.string "chose you. Incredible.\p"
	.string "I'll look into TEAM NOVA. Come and\n"
	.string "see me at my house.$"

QoL_Text_StevensLetter::
	.string "A letter from STEVEN.\p"
	.string "“{PLAYER}, congratulations on\n"
	.string "becoming HOENN's CHAMPION.\p"
	.string "I've learned what TEAM NOVA wants.\p"
	.string "Their leader, VEGA, paid TEAM\n"
	.string "ROCKET, MAGMA and AQUA to gather\l"
	.string "STAR SHARDS from both regions.\p"
	.string "She has taken them all to BIRTH\n"
	.string "ISLAND, where the next STARFALL\l"
	.string "will land. With the shards she\l"
	.string "means to call down DEOXYS.\p"
	.string "Once you are the CHAMPION of both\n"
	.string "KANTO and HOENN, ask the S.S.\l"
	.string "TIDAL in VERMILION CITY to take\l"
	.string "you to BIRTH ISLAND.\p"
	.string "Your BELDUM came from that sky.\n"
	.string "I believe it can stop her.\p"
	.string "I left something for it on my\n"
	.string "table. -STEVEN”$"

QoL_Text_StevensGiftNote::
	.string "There's a note by the POKé BALL.\p"
	.string "“For the POKéMON from my\n"
	.string "meteorite. -STEVEN”$"

@ ---------------------------------------------------------------------------
@ Chapter 9: the finale on BIRTH ISLAND. Open once Steven's letter is read and
@ the player is Champion of both regions.
@ ---------------------------------------------------------------------------
QoL_EventScript_StarfallFinaleOpen::
	setvar VAR_RESULT, FALSE
	goto_if_unset FLAG_QOL_STARFALL_LETTER, Common_EventScript_NopReturn
	goto_if_unset FLAG_SYS_GAME_CLEAR, Common_EventScript_NopReturn
	goto_if_unset FLAG_HOENN_GAME_CLEAR, Common_EventScript_NopReturn
	setvar VAR_RESULT, TRUE
	return

QoL_EventScript_StarfallBirthIslandOnTransition::
	setflag FLAG_QOL_HIDE_NOVA_BIRTH_ISLAND
	goto_if_set FLAG_QOL_STARFALL_VEGA, Common_EventScript_NopReturn
	call QoL_EventScript_StarfallFinaleOpen
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_NopReturn
	clearflag FLAG_QOL_HIDE_NOVA_BIRTH_ISLAND
	@ Deoxys only answers once Vega's plan has failed
	setflag FLAG_HIDE_BIRTH_ISLAND_DEOXYS_TRIANGLE
	return

BirthIsland_Exterior_EventScript_NovaGrunt1::
	trainerbattle_single TRAINER_NOVA_GRUNT_BIRTH_ISLAND_1, QoL_Text_BirthGrunt1Intro, QoL_Text_BirthGrunt1Defeat
	msgbox QoL_Text_BirthGrunt1After, MSGBOX_AUTOCLOSE
	end

BirthIsland_Exterior_EventScript_NovaGrunt2::
	trainerbattle_single TRAINER_NOVA_GRUNT_BIRTH_ISLAND_2, QoL_Text_BirthGrunt2Intro, QoL_Text_BirthGrunt2Defeat
	msgbox QoL_Text_BirthGrunt2After, MSGBOX_AUTOCLOSE
	end

BirthIsland_Exterior_EventScript_NovaDeneb::
	trainerbattle_single TRAINER_NOVA_DENEB_BIRTH_ISLAND, QoL_Text_BirthDenebIntro, QoL_Text_BirthDenebDefeat
	msgbox QoL_Text_BirthDenebAfter, MSGBOX_AUTOCLOSE
	end

BirthIsland_Exterior_EventScript_NovaAltair::
	trainerbattle_single TRAINER_NOVA_ALTAIR_BIRTH_ISLAND, QoL_Text_BirthAltairIntro, QoL_Text_BirthAltairDefeat
	msgbox QoL_Text_BirthAltairAfter, MSGBOX_AUTOCLOSE
	end

BirthIsland_Exterior_EventScript_NovaVega::
	lockall
	faceplayer
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	textcolor NPC_TEXT_COLOR_FEMALE
	@ Her admins step in if the player slipped past them
	goto_if_defeated TRAINER_NOVA_DENEB_BIRTH_ISLAND, QoL_EventScript_VegaCheckAltair
	msgbox QoL_Text_VegaCallsDeneb
	trainerbattle_no_intro TRAINER_NOVA_DENEB_BIRTH_ISLAND, QoL_Text_BirthDenebDefeat
QoL_EventScript_VegaCheckAltair::
	goto_if_defeated TRAINER_NOVA_ALTAIR_BIRTH_ISLAND, QoL_EventScript_VegaBattle
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox QoL_Text_VegaCallsAltair
	trainerbattle_no_intro TRAINER_NOVA_ALTAIR_BIRTH_ISLAND, QoL_Text_BirthAltairDefeat
QoL_EventScript_VegaBattle::
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox QoL_Text_VegaIntro
	trainerbattle_no_intro TRAINER_NOVA_VEGA, QoL_Text_VegaDefeat
	msgbox QoL_Text_VegaAfter
	closemessage
	playse SE_M_REFLECT
	fadescreen FADE_TO_WHITE
	delay 30
	fadescreen FADE_FROM_WHITE
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox QoL_Text_ShardsRise
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox QoL_Text_VegaFlees
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STARFALL_BIRTH_VEGA
	removeobject LOCALID_STARFALL_BIRTH_DENEB
	removeobject LOCALID_STARFALL_BIRTH_ALTAIR
	removeobject LOCALID_STARFALL_BIRTH_GRUNT_1
	removeobject LOCALID_STARFALL_BIRTH_GRUNT_2
	setflag FLAG_QOL_STARFALL_VEGA
	clearflag FLAG_HIDE_BIRTH_ISLAND_DEOXYS_TRIANGLE
	clearflag FLAG_DEOXYS_ROCK_COMPLETE
	addobject LOCALID_BIRTH_ISLAND_EXTERIOR_ROCK
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox QoL_Text_TriangleHint
	releaseall
	end

@ Deoxys defeated in the finale: it returns to the stars, then the ending.
QoL_EventScript_StarfallDeoxysFlewHome::
	goto_if_set FLAG_QOL_STARFALL_DONE, Common_EventScript_NopReturn
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_BIRTH_ISLAND_DEOXYS
	fadescreenswapbuffers FADE_FROM_BLACK
	msgbox QoL_Text_DeoxysWentHome
	call QoL_EventScript_StarfallEnding
	release
	end

@ Called after the Deoxys battle (caught or defeated) in the finale.
QoL_EventScript_StarfallEnding::
	goto_if_unset FLAG_QOL_STARFALL_VEGA, Common_EventScript_NopReturn
	goto_if_set FLAG_QOL_STARFALL_DONE, Common_EventScript_NopReturn
	setflag FLAG_QOL_STARFALL_DONE
	lockall
	delay 30
	fadescreen FADE_TO_BLACK
	playbgm MUS_RG_CREDITS, FALSE
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox QoL_Text_Ending1
	textcolor NPC_TEXT_COLOR_MALE
	msgbox QoL_Text_EndingSteven
	msgbox QoL_Text_EndingOak
	msgbox QoL_Text_EndingBirch
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox QoL_Text_EndingTheEnd
	closemessage
	fadedefaultbgm
	fadescreen FADE_FROM_BLACK
	releaseall
	return

@ ---------------------------------------------------------------------------
@ Text: the finale
@ ---------------------------------------------------------------------------
QoL_Text_BirthGrunt1Intro::
	.string "The STARFALL is tonight! No one\n"
	.string "gets past TEAM NOVA!$"

QoL_Text_BirthGrunt1Defeat::
	.string "A shooting star… of pain!$"

QoL_Text_BirthGrunt1After::
	.string "VEGA bought every shard in two\n"
	.string "regions. You can't stop her now!$"

QoL_Text_BirthGrunt2Intro::
	.string "Look up! The sky's already\n"
	.string "glowing!$"

QoL_Text_BirthGrunt2Defeat::
	.string "My stars are out…$"

QoL_Text_BirthGrunt2After::
	.string "When DEOXYS comes down, VEGA's\n"
	.string "MASTER BALL will be ready.$"

QoL_Text_BirthDenebIntro::
	.string "DENEB: Darling, you're late. The\n"
	.string "show's about to begin.\p"
	.string "VEGA paid for this night with\n"
	.string "every coin she has. I'm here to\l"
	.string "make sure she gets her money's\l"
	.string "worth!$"

QoL_Text_BirthDenebDefeat::
	.string "Bankrupt… by a child!$"

QoL_Text_BirthDenebAfter::
	.string "DENEB: Go on, then. VEGA doesn't\n"
	.string "lose. Not to people like you.$"

QoL_Text_BirthAltairIntro::
	.string "ALTAIR: Mt. MOON, METEOR FALLS,\n"
	.string "the SPACE CENTER… and now here.\p"
	.string "Every shard I dug up is in that\n"
	.string "circle behind me.\p"
	.string "Your BELDUM can feel them too,\n"
	.string "can't it? One last time, kid!$"

QoL_Text_BirthAltairDefeat::
	.string "…The sky picked you. Fine.$"

QoL_Text_BirthAltairAfter::
	.string "ALTAIR: Watch the sky, kid.\n"
	.string "It's coming.$"

QoL_Text_VegaCallsDeneb::
	.string "VEGA: Slipping past my admins?\n"
	.string "How rude. DENEB!$"

QoL_Text_VegaCallsAltair::
	.string "VEGA: ALTAIR! Don't just stand\n"
	.string "there!$"

QoL_Text_VegaIntro::
	.string "VEGA: So you're the child my\n"
	.string "admins can never seem to beat.\p"
	.string "Look up, darling. Tonight the\n"
	.string "second STARFALL lands right here,\l"
	.string "on BIRTH ISLAND.\p"
	.string "I paid TEAM ROCKET to steal\n"
	.string "SILPH's MASTER BALL.\p"
	.string "I paid MAXIE to warm my\n"
	.string "meteorites in his volcano, and\l"
	.string "ARCHIE never even knew whose\l"
	.string "money paid for his submarine.\p"
	.string "Every STAR SHARD in KANTO and\n"
	.string "HOENN is in this circle. When the\l"
	.string "stars fall, DEOXYS will come for\l"
	.string "them… and I'll be waiting.\p"
	.string "A POKéMON from beyond the sky,\n"
	.string "owned by the one person rich\l"
	.string "enough to deserve it.\p"
	.string "And that BELDUM? It fell with the\n"
	.string "first STARFALL. It belongs to the\l"
	.string "sky, so it belongs to me!$"

QoL_Text_VegaDefeat::
	.string "My shards… My stars!$"

QoL_Text_VegaAfter::
	.string "VEGA: No… The shards are glowing.\p"
	.string "They're not answering me.\n"
	.string "They're answering your BELDUM!$"

QoL_Text_ShardsRise::
	.string "The STAR SHARDS rose from the\n"
	.string "circle and shot into the sky!\p"
	.string "High above, the second STARFALL\n"
	.string "began…$"

QoL_Text_VegaFlees::
	.string "VEGA: Fine! Let the sky have you,\n"
	.string "then! TEAM NOVA, we're leaving!$"

QoL_Text_TriangleHint::
	.string "Something is shining at the top\n"
	.string "of the island.\p"
	.string "A strange triangle hums in the\n"
	.string "light of the falling stars…$"

QoL_Text_DeoxysWentHome::
	.string "DEOXYS rose into the sky and\n"
	.string "vanished among the falling stars.$"

QoL_Text_Ending1::
	.string "The second STARFALL lit up the\n"
	.string "skies over KANTO and HOENN.\p"
	.string "TEAM NOVA's shards had scattered\n"
	.string "back into the stars, and with\l"
	.string "them, VEGA's plan.$"

QoL_Text_EndingSteven::
	.string "STEVEN: {PLAYER}! I saw the sky\n"
	.string "from MOSSDEEP. It was beautiful.\p"
	.string "TEAM NOVA is finished. Without the\n"
	.string "shards, VEGA has nothing left to\l"
	.string "buy.\p"
	.string "That BELDUM came down with the\n"
	.string "STARFALL to find someone strong.\l"
	.string "It found you.$"

QoL_Text_EndingOak::
	.string "OAK: To think a meteorite on my\n"
	.string "desk started all this!\p"
	.string "{PLAYER}, you've filled your\n"
	.string "POKéDEX with two regions' worth\l"
	.string "of memories. I'm proud of you.$"

QoL_Text_EndingBirch::
	.string "BIRCH: Now that's what I call\n"
	.string "fieldwork! Come by the lab\l"
	.string "anytime. Your mom's cooking, too!$"

QoL_Text_EndingTheEnd::
	.string "STARFALL\n"
	.string "THE END\p"
	.string "Thank you for playing!$"
