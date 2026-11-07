// [QoL] Species randomizer. See include/qol_randomizer.h.
#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "qol_randomizer.h"
#include "constants/species.h"

// The randomizer looks for a stand-in species by hashing (seed, species, try)
// into a species ID and keeping the first one that fits. Each step widens the
// allowed base stat total difference, so every species finds a partner.
#define TRIES_PER_STEP 64
static const u8 sBstWindowPercent[] = {10, 20, 35, 60};

bool32 QoL_RandomizerActive(void)
{
#if IS_FRLG
    return FlagGet(FLAG_QOL_RANDOMIZER);
#else
    return FALSE;
#endif
}

// 32-bit integer hash (a "lowbias32" finalizer): small, fast and well mixed.
static u32 HashMix(u32 x)
{
    x ^= x >> 16;
    x *= 0x7FEB352D;
    x ^= x >> 15;
    x *= 0x846CA68B;
    x ^= x >> 16;
    return x;
}

// The Trainer ID is random for every new game and never changes, so each save
// file gets its own fixed randomization.
static u32 GetRandomizerSeed(void)
{
    const u8 *id = gSaveBlock2Ptr->playerTrainerId;
    return id[0] | (id[1] << 8) | (id[2] << 16) | ((u32)id[3] << 24);
}

static u32 GetBaseStatTotal(enum Species species)
{
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    return info->baseHP + info->baseAttack + info->baseDefense
         + info->baseSpeed + info->baseSpAttack + info->baseSpDefense;
}

static bool32 IsLegendLike(enum Species species)
{
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    return info->isRestrictedLegendary || info->isSubLegendary
        || info->isMythical || info->isUltraBeast;
}

// Only base forms that exist in this build: no Megas, Gigantamax, Totems,
// battle-only forms or cosmetic variants.
static bool32 IsRandomizerCandidate(enum Species species, bool32 wantLegend)
{
    const struct SpeciesInfo *info;

    if (species == SPECIES_NONE || species >= NUM_SPECIES || !IsSpeciesEnabled(species))
        return FALSE;
    info = &gSpeciesInfo[species];
    if (info->isMegaEvolution || info->isPrimalReversion || info->isUltraBurst
     || info->isGigantamax || info->isTotem || info->isTeraForm)
        return FALSE;
    if (GET_BASE_SPECIES_ID(species) != species)
        return FALSE;
    return IsLegendLike(species) == wantLegend;
}

enum Species QoL_RandomizeSpecies(enum Species species)
{
    u32 seed, bst, step, try;
    bool32 wantLegend;

    if (!QoL_RandomizerActive() || species == SPECIES_NONE || species >= NUM_SPECIES)
        return species;

    seed = GetRandomizerSeed() ^ HashMix(species * 0x9E3779B9);
    bst = GetBaseStatTotal(species);
    wantLegend = IsLegendLike(species);

    for (step = 0; step < ARRAY_COUNT(sBstWindowPercent); step++)
    {
        u32 window = bst * sBstWindowPercent[step] / 100;

        for (try = 0; try < TRIES_PER_STEP; try++)
        {
            u32 hash = HashMix(seed + (step * TRIES_PER_STEP + try) * 0x85EBCA6B);
            enum Species candidate = 1 + hash % (NUM_SPECIES - 1);
            u32 candidateBst;

            if (!IsRandomizerCandidate(candidate, wantLegend))
                continue;
            candidateBst = GetBaseStatTotal(candidate);
            if (candidateBst + window >= bst && candidateBst <= bst + window)
                return candidate;
        }
    }
    return species; // Nothing close enough: keep the original.
}

// Script helper (special QoL_RandomizeSpecialVar): VAR_RESULT = the randomized
// form of the species in VAR_0x8004.
void QoL_RandomizeSpecialVar(void)
{
    gSpecialVar_Result = QoL_RandomizeSpecies(gSpecialVar_0x8004);
}
