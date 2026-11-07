@ [QoL] Following Pokemon: talking to the Pokemon walking behind you.
@ FollowerPokemon_BufferInfo sets STR_VAR_1 (nickname), VAR_0x8004 (species)
@ and VAR_RESULT (FOLLOWER_MOOD_*, based on friendship).
EventScript_FollowerPokemon::
	lock
	faceplayer
	special FollowerPokemon_BufferInfo
	goto_if_eq VAR_RESULT, FOLLOWER_MOOD_NONE, EventScript_FollowerPokemon_End
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
	call_if_eq VAR_RESULT, FOLLOWER_MOOD_OVERJOYED, EventScript_FollowerPokemon_Overjoyed
	call_if_eq VAR_RESULT, FOLLOWER_MOOD_HAPPY, EventScript_FollowerPokemon_Happy
	call_if_eq VAR_RESULT, FOLLOWER_MOOD_NEUTRAL, EventScript_FollowerPokemon_Neutral
	waitmoncry
EventScript_FollowerPokemon_End::
	release
	end

EventScript_FollowerPokemon_Overjoyed::
	msgbox Text_FollowerPokemon_Overjoyed, MSGBOX_DEFAULT
	return

EventScript_FollowerPokemon_Happy::
	msgbox Text_FollowerPokemon_Happy, MSGBOX_DEFAULT
	return

EventScript_FollowerPokemon_Neutral::
	msgbox Text_FollowerPokemon_Neutral, MSGBOX_DEFAULT
	return

Text_FollowerPokemon_Overjoyed::
	.string "{STR_VAR_1} is overjoyed to be\n"
	.string "traveling with you!$"

Text_FollowerPokemon_Happy::
	.string "{STR_VAR_1} seems happy to be\n"
	.string "following you!$"

Text_FollowerPokemon_Neutral::
	.string "{STR_VAR_1} is keeping up\n"
	.string "with you.$"
