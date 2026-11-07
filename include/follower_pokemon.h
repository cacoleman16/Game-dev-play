#ifndef GUARD_FOLLOWER_POKEMON_H
#define GUARD_FOLLOWER_POKEMON_H

// [QoL] Following Pokemon.
// The first non-egg, non-fainted party member walks behind the player on the
// overworld as an object event with local ID OBJ_EVENT_ID_FOLLOWER.
// See src/follower_pokemon.c for how it works.

// Called from overworld.c
void FollowerPokemon_OnMapLoad(void);       // after a map's object events are spawned
void FollowerPokemon_OnReturnToField(void); // after returning from a battle/menu
void FollowerPokemon_Update(void);          // once per overworld frame

// Called from event_object_movement.c when crossing into a connected map
void FollowerPokemon_OnCameraShift(s16 dx, s16 dy);

// Collision / interaction helpers
bool8 FollowerPokemon_IsFollowerObject(const struct ObjectEvent *objectEvent);
bool8 FollowerPokemon_ShouldIgnoreCollision(const struct ObjectEvent *mover, const struct ObjectEvent *other);
const u8 *FollowerPokemon_GetInteractionScript(u8 objectEventId);

// Special used by EventScript_FollowerPokemon
void FollowerPokemon_BufferInfo(void);

#endif // GUARD_FOLLOWER_POKEMON_H
