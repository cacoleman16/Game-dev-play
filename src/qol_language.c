// [QoL] English / Japanese text. See include/qol_language.h.
#include "global.h"
#include "battle_main.h"
#include "data.h"
#include "pokemon.h"
#include "qol_language.h"
#include "constants/abilities.h"
#include "constants/characters.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokedex.h"

#include "data/text/qol_ja_names.h"
#include "data/text/qol_ja_descriptions.h"

#define ROM_START_ADDR 0x08000000
#define ROM_END_ADDR   0x0A000000

bool32 QoL_IsJapanese(void)
{
    return gSaveBlock2Ptr->optionsLanguage == QOL_LANGUAGE_JAPANESE;
}

// Names stored in fixed-size arrays inside ROM tables can't carry a Japanese
// alternate, so recognize a pointer to one and return the Japanese name.
static const u8 *JapaneseTableName(const u8 *str)
{
    u32 offset, i;

    offset = (u32)str - (u32)gSpeciesInfo;
    if (offset < (NUM_SPECIES + 1) * sizeof(struct SpeciesInfo))
    {
        i = offset / sizeof(struct SpeciesInfo);
        if (gSpeciesInfo[i].natDexNum > NATIONAL_DEX_COUNT)
            return NULL;
        if (str == gSpeciesInfo[i].speciesName)
            return sJaSpeciesNames[gSpeciesInfo[i].natDexNum];
        if (str == gSpeciesInfo[i].categoryName)
            return sJaCategoryNames[gSpeciesInfo[i].natDexNum];
        return NULL;
    }

    offset = (u32)str - (u32)gAbilitiesInfo;
    if (offset < ABILITIES_COUNT * sizeof(struct AbilityInfo))
    {
        i = offset / sizeof(struct AbilityInfo);
        if (str == gAbilitiesInfo[i].name)
            return sJaAbilityNames[i];
        return NULL;
    }

    offset = (u32)str - (u32)gTypesInfo;
    if (offset < NUMBER_OF_MON_TYPES * sizeof(struct TypeInfo))
    {
        i = offset / sizeof(struct TypeInfo);
        if (str == gTypesInfo[i].name)
            return sJaTypeNames[i];
        return NULL;
    }
    return NULL;
}

// Returns the Japanese version of a ROM string in Japanese mode, else the
// string itself. A Japanese alternate sits right after the English text's
// EOS, marked by two {JPN} control codes (FC 15 FC 15).
const u8 *QoL_LangStr(const u8 *str)
{
    const u8 *p;
    const u8 *name;

    if (str == NULL || gSaveBlock2Ptr->optionsLanguage != QOL_LANGUAGE_JAPANESE)
        return str;
    if ((u32)str < ROM_START_ADDR || (u32)str >= ROM_END_ADDR)
        return str; // buffers in RAM are already in the right language

    name = JapaneseTableName(str);
    if (name != NULL)
        return name;

    for (p = str; *p != EOS; p++)
        ;
    if (p[1] == EXT_CTRL_CODE_BEGIN && p[2] == EXT_CTRL_CODE_JPN
     && p[3] == EXT_CTRL_CODE_BEGIN && p[4] == EXT_CTRL_CODE_JPN)
        return p + 5;
    return str;
}

const u8 *QoL_JapaneseSpeciesName(enum Species species)
{
    u32 dex;

    if (species > NUM_SPECIES)
        return NULL;
    dex = gSpeciesInfo[species].natDexNum;
    if (dex == 0 || dex > NATIONAL_DEX_COUNT)
        return NULL;
    return sJaSpeciesNames[dex];
}

const u8 *QoL_LangMoveName(enum Move move, const u8 *english)
{
    if (gSaveBlock2Ptr->optionsLanguage == QOL_LANGUAGE_JAPANESE
     && move < MOVES_COUNT_ALL && sJaMoveNames[move] != NULL)
        return sJaMoveNames[move];
    return english;
}

const u8 *QoL_LangItemName(enum Item item, const u8 *english)
{
    if (gSaveBlock2Ptr->optionsLanguage == QOL_LANGUAGE_JAPANESE
     && item < ITEMS_COUNT && sJaItemNames[item] != NULL)
        return sJaItemNames[item];
    return english;
}

// Descriptions: Japanese text from src/data/text/qol_ja_descriptions.h when
// it exists, else the English one.
const u8 *QoL_LangMoveDescription(enum Move move, const u8 *english)
{
    if (QoL_IsJapanese() && move < MOVES_COUNT_ALL && sJaMoveDescriptions[move] != NULL)
        return sJaMoveDescriptions[move];
    return english;
}

const u8 *QoL_LangItemDescription(enum Item item, const u8 *english)
{
    if (QoL_IsJapanese() && item < ITEMS_COUNT && sJaItemDescriptions[item] != NULL)
        return sJaItemDescriptions[item];
    return english;
}

const u8 *QoL_LangAbilityDescription(enum Ability ability, const u8 *english)
{
    if (QoL_IsJapanese() && ability < ABILITIES_COUNT && sJaAbilityDescriptions[ability] != NULL)
        return sJaAbilityDescriptions[ability];
    return english;
}

const u8 *QoL_LangPokedexText(enum Species species, const u8 *english)
{
    u32 dex;

    if (!QoL_IsJapanese() || species > NUM_SPECIES)
        return english;
    dex = gSpeciesInfo[species].natDexNum;
    if (dex <= NATIONAL_DEX_COUNT && sJaPokedexText[dex] != NULL)
        return sJaPokedexText[dex];
    return english;
}
