// [QoL] Merged regions: storage for the Hoenn flags and vars of the FireRed
// build, and helpers for Hoenn maps. See include/qol_hoenn.h.
#include "global.h"
#include "event_data.h"
#include "qol_hoenn.h"
#include "script.h"
#include "constants/maps.h"

extern const u8 EventScript_ResetAllMapFlags[];

#if IS_FRLG

// Saves from before merged regions have no Hoenn block yet: start it empty.
static struct QoLHoennState *GetState(void)
{
    struct QoLHoennState *state = &gSaveBlock3Ptr->qolHoenn;
    if (state->magic != QOL_HOENN_MAGIC)
    {
        memset(state, 0, sizeof(*state));
        state->magic = QOL_HOENN_MAGIC;
    }
    return state;
}

u8 *QoL_HoennFlagPointer(u16 id)
{
    return &GetState()->flags[(id - QOL_HOENN_FLAGS_START) / 8];
}

u8 *QoL_ExtraFlagPointer(u16 id)
{
    return &GetState()->extraFlags[(id - QOL_EXTRA_FLAGS_START) / 8];
}

u16 *QoL_HoennVarPointer(u16 id)
{
    return &GetState()->vars[id - QOL_HOENN_VARS_START];
}

void QoL_HoennResetState(void)
{
    struct QoLHoennState *state = &gSaveBlock3Ptr->qolHoenn;
    memset(state, 0, sizeof(*state));
    state->magic = QOL_HOENN_MAGIC;
}

// Emerald's daily flags (FLAG_DAILY_*), 0x920-0x95F in Emerald numbering.
void QoL_HoennClearDailyFlags(void)
{
    memset(&GetState()->flags[0x920 / 8], 0, (0x960 - 0x920) / 8);
}

#endif // IS_FRLG

bool32 QoL_BadgeGet(u32 kantoBadgeFlag)
{
    if (FlagGet(kantoBadgeFlag))
        return TRUE;
#if IS_FRLG
    return FlagGet(kantoBadgeFlag - FLAG_BADGE01_GET + FLAG_HOENN_BADGE01_GET);
#else
    return FALSE;
#endif
}

// Special: before the first trip to Hoenn, set up Hoenn's map flags the way
// an Emerald new game does.
void QoL_HoennSetup(void)
{
#if IS_FRLG
    if (!FlagGet(FLAG_QOL_HOENN_SETUP))
    {
        RunScriptImmediately(EventScript_ResetAllMapFlags);
        FlagSet(FLAG_QOL_HOENN_SETUP);
    }
#endif
}

// Hoenn maps are in the Emerald map groups, which come before the FRLG ones.
bool32 QoL_IsHoennMap(void)
{
#if IS_FRLG
    return gSaveBlock1Ptr->location.mapGroup < MAP_GROUP(MAP_BATTLE_COLOSSEUM_2P_FRLG);
#else
    return TRUE;
#endif
}
