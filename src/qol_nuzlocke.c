// [QoL] Nuzlocke mode. See include/qol_nuzlocke.h for the rules.
#include "global.h"
#include "event_data.h"
#include "pokedex.h"
#include "pokemon.h"
#include "qol_nuzlocke.h"
#include "constants/flags.h"
#include "constants/region_map_sections.h"

#if IS_FRLG
STATIC_ASSERT(FLAG_QOL_NUZLOCKE_AREAS_START + KANTO_MAPSEC_COUNT - 1 <= FLAG_0x3D7, NuzlockeAreaFlagsFitInFreeRange);
STATIC_ASSERT(FLAG_QOL_NUZLOCKE_HOENN_AREAS_START + KANTO_MAPSEC_START <= QOL_HOENN_FLAGS_START + QOL_HOENN_FLAGS_COUNT, NuzlockeHoennAreaFlagsFit);
#endif

bool32 QoL_NuzlockeActive(void)
{
#if IS_FRLG
    // The run starts with the first Poké Balls, which come with the Pokédex.
    return FlagGet(FLAG_QOL_NUZLOCKE) && FlagGet(FLAG_SYS_POKEDEX_GET);
#else
    return FALSE;
#endif
}

// A dead Pokémon is a fainted, hatched Pokémon while Nuzlocke is on.
bool32 QoL_NuzlockeIsDead(struct Pokemon *mon)
{
    if (!QoL_NuzlockeActive())
        return FALSE;
    if (!GetMonData(mon, MON_DATA_SANITY_HAS_SPECIES) || GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;
    return GetMonData(mon, MON_DATA_HP) == 0;
}

// Boxed Pokémon keep their HP (OW_PC_HEAL is Gen 8+), so rebuild the party
// form to read it.
bool32 QoL_NuzlockeIsBoxMonDead(struct BoxPokemon *boxMon)
{
    struct Pokemon mon;

    if (!QoL_NuzlockeActive() || !GetBoxMonData(boxMon, MON_DATA_SANITY_HAS_SPECIES))
        return FALSE;
    BoxMonToMon(boxMon, &mon);
    return QoL_NuzlockeIsDead(&mon);
}

// TRUE when Nuzlocke is on and no party Pokémon can fight.
bool32 QoL_NuzlockePartyWiped(void)
{
    u32 i;

    if (!QoL_NuzlockeActive())
        return FALSE;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];
        if (GetMonData(mon, MON_DATA_SANITY_HAS_SPECIES)
         && !GetMonData(mon, MON_DATA_IS_EGG)
         && GetMonData(mon, MON_DATA_HP) != 0)
            return FALSE;
    }
    return TRUE;
}

// Called when a random wild battle (grass, water, fishing, Rock Smash, Sweet
// Scent, DexNav) is about to start, after the wild Pokémon is generated.
// Decides whether it may be caught and uses up the area if this is its
// encounter.
void QoL_NuzlockeOnWildEncounter(void)
{
#if IS_FRLG
    u32 mapSec = gMapHeader.regionMapSectionId;
    u32 areaFlag;
    struct Pokemon *wildMon = &gEnemyParty[0];
    enum Species species = GetMonData(wildMon, MON_DATA_SPECIES);

    FlagClear(FLAG_QOL_NO_CATCHING);
    if (!QoL_NuzlockeActive())
        return;
    if (mapSec > KANTO_MAPSEC_END)
        return; // Not a Kanto, Sevii or Hoenn area: no tracking.

    // Shiny clause: always catchable, and the area stays open.
    if (IsMonShiny(wildMon))
        return;

    if (mapSec < KANTO_MAPSEC_START) // [QoL] Merged regions: a Hoenn area
        areaFlag = FLAG_QOL_NUZLOCKE_HOENN_AREAS_START + mapSec;
    else
        areaFlag = FLAG_QOL_NUZLOCKE_AREAS_START + (mapSec - KANTO_MAPSEC_START);
    if (FlagGet(areaFlag))
    {
        FlagSet(FLAG_QOL_NO_CATCHING); // This area's encounter is used up.
        return;
    }

    // Dupes clause: a species already caught can't be caught again, but it
    // doesn't use up the area either.
    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
    {
        FlagSet(FLAG_QOL_NO_CATCHING);
        return;
    }

    // This is the area's one encounter, whether the player catches it or not.
    FlagSet(areaFlag);
#endif
}

// Called after every wild battle so the "can't catch" state never leaks into
// the next one.
void QoL_NuzlockeEndWildBattle(void)
{
#if IS_FRLG
    FlagClear(FLAG_QOL_NO_CATCHING);
#endif
}
