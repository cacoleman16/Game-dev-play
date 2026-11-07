#ifndef GUARD_QOL_NUZLOCKE_H
#define GUARD_QOL_NUZLOCKE_H

// [QoL] Nuzlocke mode for the FireRed/LeafGreen build.
//
// Switched on and off at the NES in the player's bedroom (FLAG_QOL_NUZLOCKE).
// The rules apply once the player has the Pokédex, which is when Oak hands
// out the first Poké Balls:
//   * Only the first random wild Pokémon met in each area (map section) can be
//     caught. Species already caught don't use up the area (dupes clause), and
//     shiny Pokémon can always be caught (shiny clause). Scripted encounters
//     (legendaries, Snorlax, gifts) and the Safari Zone are exempt.
//   * A Pokémon that faints is dead: Pokémon Centers, revives and Revival
//     Blessing no longer bring it back.
//   * If every party Pokémon is dead, wild Pokémon and trainers leave the player
//     alone so they can reach a PC and swap in living Pokémon.

bool32 QoL_NuzlockeActive(void);
bool32 QoL_NuzlockeIsDead(struct Pokemon *mon);
bool32 QoL_NuzlockeIsBoxMonDead(struct BoxPokemon *boxMon);
bool32 QoL_NuzlockePartyWiped(void);
void QoL_NuzlockeOnWildEncounter(void);
void QoL_NuzlockeEndWildBattle(void);

#endif // GUARD_QOL_NUZLOCKE_H
