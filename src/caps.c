#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "caps.h"
#include "pokemon.h"


u32 GetCurrentLevelCap(void)
{
#if IS_FRLG
    // [QoL] Optional Kanto level caps: the level of each gym leader's
    // strongest Pokémon, then the Champion's. Off unless FLAG_QOL_LEVEL_CAPS
    // is set (toggled at the NES in the player's bedroom).
    static const u32 sLevelCapFlagMapFrlg[][2] =
    {
        {FLAG_BADGE01_GET, 14}, // Brock
        {FLAG_BADGE02_GET, 21}, // Misty
        {FLAG_BADGE03_GET, 24}, // Lt. Surge
        {FLAG_BADGE04_GET, 29}, // Erika
        {FLAG_BADGE05_GET, 43}, // Koga
        {FLAG_BADGE06_GET, 43}, // Sabrina
        {FLAG_BADGE07_GET, 47}, // Blaine
        {FLAG_BADGE08_GET, 50}, // Giovanni
        {FLAG_SYS_GAME_CLEAR, 63}, // Champion
    };
    // [QoL] Merged regions: Hoenn's gym leaders (Emerald levels). The cap is
    // the higher of the two regions', so progress in either raises it.
    static const u32 sLevelCapFlagMapHoenn[][2] =
    {
        {FLAG_HOENN_BADGE01_GET, 15}, // Roxanne
        {FLAG_HOENN_BADGE02_GET, 19}, // Brawly
        {FLAG_HOENN_BADGE03_GET, 24}, // Wattson
        {FLAG_HOENN_BADGE04_GET, 29}, // Flannery
        {FLAG_HOENN_BADGE05_GET, 31}, // Norman
        {FLAG_HOENN_BADGE06_GET, 33}, // Winona
        {FLAG_HOENN_BADGE07_GET, 42}, // Tate & Liza
        {FLAG_HOENN_BADGE08_GET, 46}, // Juan
        {FLAG_HOENN_GAME_CLEAR, 58},  // Champion
    };
    u32 j, kanto = MAX_LEVEL, hoenn = MAX_LEVEL;

    if (FLAG_QOL_LEVEL_CAPS == 0 || !FlagGet(FLAG_QOL_LEVEL_CAPS))
        return MAX_LEVEL;
    for (j = 0; j < ARRAY_COUNT(sLevelCapFlagMapFrlg); j++)
    {
        if (!FlagGet(sLevelCapFlagMapFrlg[j][0]))
        {
            kanto = sLevelCapFlagMapFrlg[j][1];
            break;
        }
    }
    for (j = 0; j < ARRAY_COUNT(sLevelCapFlagMapHoenn); j++)
    {
        if (!FlagGet(sLevelCapFlagMapHoenn[j][0]))
        {
            hoenn = sLevelCapFlagMapHoenn[j][1];
            break;
        }
    }
    return max(kanto, hoenn);
#endif

    static const u32 sLevelCapFlagMap[][2] =
    {
        {FLAG_BADGE01_GET, 15},
        {FLAG_BADGE02_GET, 19},
        {FLAG_BADGE03_GET, 24},
        {FLAG_BADGE04_GET, 29},
        {FLAG_BADGE05_GET, 31},
        {FLAG_BADGE06_GET, 33},
        {FLAG_BADGE07_GET, 42},
        {FLAG_BADGE08_GET, 46},
        {FLAG_IS_CHAMPION, 58},
    };

    u32 i;

    if (B_LEVEL_CAP_TYPE == LEVEL_CAP_FLAG_LIST)
    {
        for (i = 0; i < ARRAY_COUNT(sLevelCapFlagMap); i++)
        {
            if (!FlagGet(sLevelCapFlagMap[i][0]))
                return sLevelCapFlagMap[i][1];
        }
    }
    else if (B_LEVEL_CAP_TYPE == LEVEL_CAP_VARIABLE)
    {
        return VarGet(B_LEVEL_CAP_VARIABLE);
    }

    return MAX_LEVEL;
}

u32 GetSoftLevelCapExpValue(u32 level, u32 expValue)
{
    static const u32 sExpScalingDown[5] = { 4, 8, 16, 32, 64 };
    static const u32 sExpScalingUp[5]   = { 16, 8, 4, 2, 1 };

    u32 levelDifference;
    u32 currentLevelCap = GetCurrentLevelCap();

    if (B_EXP_CAP_TYPE == EXP_CAP_NONE)
        return expValue;

    if (level < currentLevelCap)
    {
        if (B_LEVEL_CAP_EXP_UP)
        {
            levelDifference = currentLevelCap - level;
            if (levelDifference > ARRAY_COUNT(sExpScalingUp) - 1)
                return expValue + (expValue / sExpScalingUp[ARRAY_COUNT(sExpScalingUp) - 1]);
            else
                return expValue + (expValue / sExpScalingUp[levelDifference]);
        }
        else
        {
            return expValue;
        }
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_HARD)
    {
        return 0;
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_SOFT)
    {
        levelDifference = level - currentLevelCap;
        if (levelDifference > ARRAY_COUNT(sExpScalingDown) - 1)
            return expValue / sExpScalingDown[ARRAY_COUNT(sExpScalingDown) - 1];
        else
            return expValue / sExpScalingDown[levelDifference];
    }
    else
    {
       return expValue;
    }
}

u32 GetCurrentEVCap(void)
{
    static const u16 sEvCapFlagMap[][2] = {
        // Define EV caps for each milestone
        {FLAG_BADGE01_GET, MAX_TOTAL_EVS *  1 / 17},
        {FLAG_BADGE02_GET, MAX_TOTAL_EVS *  3 / 17},
        {FLAG_BADGE03_GET, MAX_TOTAL_EVS *  5 / 17},
        {FLAG_BADGE04_GET, MAX_TOTAL_EVS *  7 / 17},
        {FLAG_BADGE05_GET, MAX_TOTAL_EVS *  9 / 17},
        {FLAG_BADGE06_GET, MAX_TOTAL_EVS * 11 / 17},
        {FLAG_BADGE07_GET, MAX_TOTAL_EVS * 13 / 17},
        {FLAG_BADGE08_GET, MAX_TOTAL_EVS * 15 / 17},
        {FLAG_IS_CHAMPION, MAX_TOTAL_EVS},
    };

    if (B_EV_CAP_TYPE == EV_CAP_FLAG_LIST)
    {
        for (u32 evCap = 0; evCap < ARRAY_COUNT(sEvCapFlagMap); evCap++)
        {
            if (!FlagGet(sEvCapFlagMap[evCap][0]))
                return sEvCapFlagMap[evCap][1];
        }
    }
    else if (B_EV_CAP_TYPE == EV_CAP_VARIABLE)
    {
        return VarGet(B_EV_CAP_VARIABLE);
    }
    else if (B_EV_CAP_TYPE == EV_CAP_NO_GAIN)
    {
        return 0;
    }

    return MAX_TOTAL_EVS;
}
