// [QoL] Side quests: the Field Research chain (a researcher in Oak's Lab) and
// the post-game hunt for the Galarian legendary birds.
//
// VAR_QOL_FIELD_RESEARCH holds the current task (QOL_RESEARCH_* in
// include/constants/vars.h). The script asks QoL_Research_Status how far the
// player is: VAR_RESULT = TRUE when the task is done, VAR_0x8005 = progress,
// VAR_0x8006 = goal.
#include "global.h"
#include "event_data.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/species.h"
#include "constants/vars.h"

// The scripts and map objects use these copies of species-based values.
STATIC_ASSERT(OBJ_EVENT_GFX_QOL_ARTICUNO_GALAR == OBJ_EVENT_GFX_SPECIES(ARTICUNO_GALAR), QolArticunoGfx);
STATIC_ASSERT(OBJ_EVENT_GFX_QOL_ZAPDOS_GALAR == OBJ_EVENT_GFX_SPECIES(ZAPDOS_GALAR), QolZapdosGfx);
STATIC_ASSERT(OBJ_EVENT_GFX_QOL_MOLTRES_GALAR == OBJ_EVENT_GFX_SPECIES(MOLTRES_GALAR), QolMoltresGfx);

#define FIRST_FOREIGN_DEX_NUM 387 // Turtwig: Pokemon from Sinnoh and later

static u32 CountCaughtInDexRange(u32 first, u32 last)
{
    u32 n, count = 0;

    if (last > NATIONAL_DEX_COUNT)
        last = NATIONAL_DEX_COUNT;
    for (n = first; n <= last; n++)
    {
        if (GetSetPokedexFlag(n, FLAG_GET_CAUGHT))
            count++;
    }
    return count;
}

static bool32 PartyHasForeignMon(void)
{
    u32 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        enum Species species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (species != SPECIES_NONE && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG)
         && SpeciesToNationalPokedexNum(species) >= FIRST_FOREIGN_DEX_NUM)
            return TRUE;
    }
    return FALSE;
}

static u32 Min16(u32 x)
{
    return x > 0xFFFF ? 0xFFFF : x;
}

void QoL_Research_Status(void)
{
    u32 progress = 0, goal = 1;

    switch (VarGet(VAR_QOL_FIELD_RESEARCH))
    {
    case QOL_RESEARCH_SHOW_FOREIGN:
        progress = PartyHasForeignMon();
        break;
    case QOL_RESEARCH_CATCH_10_FOREIGN:
        progress = CountCaughtInDexRange(FIRST_FOREIGN_DEX_NUM, NATIONAL_DEX_COUNT);
        goal = 10;
        break;
    case QOL_RESEARCH_WIN_50_BATTLES:
        progress = GetGameStat(GAME_STAT_TRAINER_BATTLES);
        goal = 50;
        break;
    case QOL_RESEARCH_HATCH_EGG:
        progress = GetGameStat(GAME_STAT_HATCHED_EGGS);
        break;
    case QOL_RESEARCH_CATCH_25_FOREIGN:
        progress = CountCaughtInDexRange(FIRST_FOREIGN_DEX_NUM, NATIONAL_DEX_COUNT);
        goal = 25;
        break;
    case QOL_RESEARCH_CATCH_100_KANTO:
        progress = CountCaughtInDexRange(1, 151);
        goal = 100;
        break;
    case QOL_RESEARCH_GALAR_LEAD:
        progress = FlagGet(FLAG_SYS_GAME_CLEAR);
        break;
    case QOL_RESEARCH_CATCH_GALAR_BIRDS:
        progress = FlagGet(FLAG_QOL_CAUGHT_GALAR_ARTICUNO) + FlagGet(FLAG_QOL_CAUGHT_GALAR_ZAPDOS)
                 + FlagGet(FLAG_QOL_CAUGHT_GALAR_MOLTRES);
        goal = 3;
        break;
    default:
        break;
    }
    gSpecialVar_0x8005 = Min16(progress);
    gSpecialVar_0x8006 = goal;
    gSpecialVar_Result = (progress >= goal);
}
