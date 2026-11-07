@ [QoL] Smart Repel loop.
@ When a Repel wears off, look in the bag for another one (the kind used last
@ first, then REPEL > SUPER REPEL > MAX REPEL) and offer to use it right away.
@ GetRepelToReuse sets VAR_RESULT (TRUE if one was found), VAR_0x8004 (its item
@ ID) and STR_VAR_1 (its name); UseRepelToReuse consumes it and restarts the
@ step counter.
EventScript_RepelWoreOff::
	lockall
	special GetRepelToReuse
	goto_if_eq VAR_RESULT, FALSE, EventScript_RepelWoreOff_NoneLeft
	msgbox Text_RepelWoreOffUseAnother, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_RepelWoreOff_Done
	special UseRepelToReuse
	playse SE_REPEL
	msgbox Text_UsedAnotherRepel, MSGBOX_DEFAULT
	waitse
EventScript_RepelWoreOff_Done::
	releaseall
	end

EventScript_RepelWoreOff_NoneLeft::
	msgbox Text_RepelWoreOff, MSGBOX_DEFAULT
	releaseall
	end

Text_RepelWoreOff::
	.string "REPEL's effect wore off…$"

Text_RepelWoreOffUseAnother::
	.string "REPEL's effect wore off…\p"
	.string "Would you like to use another\n"
	.string "{STR_VAR_1}?$"

Text_UsedAnotherRepel::
	.string "{PLAYER} used the\n"
	.string "{STR_VAR_1}.$"
