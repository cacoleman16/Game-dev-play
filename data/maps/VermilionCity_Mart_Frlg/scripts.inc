VermilionCity_Mart_Frlg_MapScripts::
	.byte 0

VermilionCity_Mart_EventScript_CooltrainerF::
	msgbox VermilionCity_Mart_Text_MonsGoodOrBadDependingOnTrainer, MSGBOX_NPC
	end

VermilionCity_Mart_EventScript_BaldingMan::
	msgbox VermilionCity_Mart_Text_TeamRocketAreWickedPeople, MSGBOX_NPC
	end

VermilionCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart VermilionCity_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
VermilionCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

VermilionCity_Mart_Text_TeamRocketAreWickedPeople::
	.string "There are wicked people who will\n"
	.string "use POKéMON for criminal acts.\p"
	.string "TEAM ROCKET traffics in rare\n"
	.string "POKéMON, for example.\p"
	.string "They also abandon POKéMON that\n"
	.string "they consider unpopular or useless.\p"
	.string "That's the sort of horrid people\n"
	.string "they are, TEAM ROCKET.$"
	.string "{JPN}{JPN}ポケモンを　はんざいに　つかう　わるい\n"
	.string "ひとたちが　いるの\p"
	.string "たとえば　ロケットだんは　めずらしい　ポケモンを\n"
	.string "みつばい　してるわ\p"
	.string "にんきが　ない　ポケモンや　やくに　たたない\n"
	.string "ポケモンは　すてて　しまうの\p"
	.string "ロケットだんは　そんな　ひどい　れんちゅう\n"
	.string "なのよ$"

VermilionCity_Mart_Text_MonsGoodOrBadDependingOnTrainer::
	.string "I think POKéMON can be good or\n"
	.string "bad. It depends on the TRAINER.$"
	.string "{JPN}{JPN}ポケモンが　いい　こに　なるか　わるい　こに\n"
	.string "なるかは　トレーナー　しだい　だと　おもうな$"

