#ifndef GUARD_QOL_HOENN_H
#define GUARD_QOL_HOENN_H

// [QoL] Merged regions: the FireRed build also contains the Hoenn maps and
// scripts. The Emerald-only flags and vars are stored in their own block
// (SaveBlock3) so Hoenn and Kanto progress never overwrite each other:
//   * flags QOL_HOENN_FLAGS_START .. + QOL_HOENN_FLAGS_COUNT (constants/flags.h)
//   * vars  QOL_HOENN_VARS_START  .. + QOL_HOENN_VARS_COUNT  (constants/vars.h)
//   * Hoenn trainers come after the Kanto ones (HOENN_TRAINER, opponents.h)

#define QOL_HOENN_MAGIC 0x484F454E // "HOEN"

#if IS_FRLG
static inline bool32 QoL_IsHoennFlag(u32 id)
{
    return id >= QOL_HOENN_FLAGS_START && id < QOL_HOENN_FLAGS_START + QOL_HOENN_FLAGS_COUNT;
}
static inline bool32 QoL_IsExtraFlag(u32 id)
{
    return id >= QOL_EXTRA_FLAGS_START && id < QOL_EXTRA_FLAGS_START + QOL_EXTRA_FLAGS_COUNT;
}
static inline bool32 QoL_IsHoennVar(u32 id)
{
    return id >= QOL_HOENN_VARS_START && id < QOL_HOENN_VARS_START + QOL_HOENN_VARS_COUNT;
}
u8 *QoL_HoennFlagPointer(u16 id);
u8 *QoL_ExtraFlagPointer(u16 id);
u16 *QoL_HoennVarPointer(u16 id);
void QoL_HoennResetState(void);
void QoL_HoennClearDailyFlags(void);
#endif

bool32 QoL_IsHoennMap(void);
// FlagGet for a (Kanto) FLAG_BADGE0x_GET that is also TRUE when the player has
// Hoenn's badge with the same number.
bool32 QoL_BadgeGet(u32 kantoBadgeFlag);
void QoL_HoennSetup(void);
// [QoL] Visible wild Pokémon (the expansion's Overworld Wild Encounters):
// everywhere with the NES option, always on Starfall Isle. Where they're on,
// the usual random grass encounters are off.
bool32 QoL_VisibleWildActive(void);

#endif // GUARD_QOL_HOENN_H
