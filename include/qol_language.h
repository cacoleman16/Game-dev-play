#ifndef GUARD_QOL_LANGUAGE_H
#define GUARD_QOL_LANGUAGE_H

// [QoL] English / Japanese text.
//
// The language is an option (OPTION menu, header row), stored in
// gSaveBlock2Ptr->optionsLanguage. In Japanese mode every text printer uses the
// Japanese (kana) font, and strings are swapped for their Japanese versions:
//   * Script and C strings carry their Japanese text right after the English
//     text's end-of-string, behind two {JPN} codes (dev_scripts/qol_japanese.py
//     writes these from text/ja/).
//   * Pokemon, ability and type names live in fixed-size arrays, and move and
//     item names are looked up by ID; their Japanese names come from
//     src/data/text/qol_ja_names.h (dev_scripts/qol_ja_names.py).
// The core string functions (StringCopy, StringExpandPlaceholders, the text
// printer...) call QoL_LangStr(), so most text switches without other changes.

#define QOL_LANGUAGE_ENGLISH  0
#define QOL_LANGUAGE_JAPANESE 1

bool32 QoL_IsJapanese(void);
const u8 *QoL_LangStr(const u8 *str);
const u8 *QoL_LangMoveName(enum Move move, const u8 *english);
const u8 *QoL_LangItemName(enum Item item, const u8 *english);
const u8 *QoL_JapaneseSpeciesName(enum Species species); // NULL if unknown
const u8 *QoL_LangMoveDescription(enum Move move, const u8 *english);
const u8 *QoL_LangItemDescription(enum Item item, const u8 *english);
const u8 *QoL_LangAbilityDescription(enum Ability ability, const u8 *english);
const u8 *QoL_LangPokedexText(enum Species species, const u8 *english);

#endif // GUARD_QOL_LANGUAGE_H
