#ifndef GUARD_QOL_RANDOMIZER_H
#define GUARD_QOL_RANDOMIZER_H

// [QoL] Species randomizer for the FireRed/LeafGreen build.
//
// Switched on and off at the NES in the player's bedroom (FLAG_QOL_RANDOMIZER).
// Every species is swapped for another one with a similar base stat total, so
// early routes stay early-game and legendaries stay legendary. The swap is a
// fixed table per save file (seeded by the Trainer ID): every Pidgey in that
// save becomes the same Pokémon, and the same goes for trainers' teams.
//
// Randomized: wild Pokémon (grass, water, fishing, Rock Smash, DexNav),
// trainers' teams (their custom moves and abilities are dropped), scripted
// wild battles (Snorlax, legendaries...), and the starters (yours and your
// rival's). Gift Pokémon and in-game trades stay as they are, since their
// scripts name the species.

bool32 QoL_RandomizerActive(void);
enum Species QoL_RandomizeSpecies(enum Species species);
void QoL_RandomizeSpecialVar(void); // special: VAR_RESULT = randomized VAR_0x8004

#endif // GUARD_QOL_RANDOMIZER_H
