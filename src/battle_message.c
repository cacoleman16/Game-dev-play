#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_record.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "battle_setup.h"
#include "battle_special.h"
#include "battle_z_move.h"
#include "data.h"
#include "event_data.h"
#include "frontier_util.h"
#include "graphics.h"
#include "international_string_util.h"
#include "item.h"
#include "link.h"
#include "menu.h"
#include "palette.h"
#include "recorded_battle.h"
#include "string_util.h"
#include "strings.h"
#include "test_runner.h"
#include "text.h"
#include "trainer_hill.h"
#include "trainer_slide.h"
#include "trainer_tower.h"
#include "window.h"
#include "line_break.h"
#include "constants/abilities.h"
#include "constants/battle_dome.h"
#include "constants/battle_string_ids.h"
#include "constants/frontier_util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/trainer_hill.h"
#include "constants/weather.h"
#include "qol_language.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    union {
        struct {
            DEPRECATED("Use color.background instead") u8 bgColor;
            DEPRECATED("Use color.foreground instead") u8 fgColor;
            DEPRECATED("Use color.shadow instead") u8 shadowColor;
            DEPRECATED("Use color.accent instead") u8 accentColor;
        };
        union TextColor color;
    };
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
};

#if TESTING
EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#else
static EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#endif
EWRAM_DATA struct BattleMsgData *gBattleMsgDataPtr = NULL;

// todo: make some of those names less vague: attacker/target vs pkmn, etc.

static const u8 sText_EmptyString4[] = _("");

const u8 gText_PkmnShroudedInMist[] = _("{B_ATK_NAME_WITH_PREFIX} surrounds itself with a protective mist!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しろいきりに\nつつまれた！");
const u8 gText_PkmnGettingPumped[] = _("{B_DEF_NAME_WITH_PREFIX} is getting pumped!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　はりきっている！");
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1} is preventing switching out with its {B_LAST_ABILITY} Ability!\p${JPN}{JPN}{B_BUFF1}の　{B_LAST_ABILITY}で\nいれかえが　できない！\p");
const u8 gText_StatSharply[] = _(" sharply${JPN}{JPN}ぐーんと　");
const u8 gText_StatRose[] = _("rose!${JPN}{JPN}あがった！");
const u8 gText_StatFell[] = _("fell!${JPN}{JPN}さがった！");
const u8 gText_DefendersStatRose[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1} rose{B_BUFF2}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_BUFF1}が\n{B_BUFF2}あがった！");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}You got away safely!\p${JPN}{JPN}{PLAY_SE SE_FLEE}うまく　にげきれた！\p");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("You defeated {B_LINK_OPPONENT1_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}との　しょうぶに　かった！");
static const u8 sText_TwoLinkTrainersDefeated[] = _("You defeated {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}と　{B_LINK_OPPONENT2_NAME}との\nしょうぶに　かった！");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("You lost against {B_LINK_OPPONENT1_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}との　しょうぶに　まけた！");
static const u8 sText_PlayerLostToTwo[] = _("You lost to {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}と　{B_LINK_OPPONENT2_NAME}との\nしょうぶに　まけた！");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("You battled to a draw against {B_LINK_OPPONENT1_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}との　しょうぶは　ひきわけに\nなった！");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("You battled to a draw against {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}と　{B_LINK_OPPONENT2_NAME}との\nしょうぶは　ひきわけに　なった！");
static const u8 sText_WildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} fled!${JPN}{JPN}{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME}は　にげだした！"); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_TwoWildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME} fled!${JPN}{JPN}{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME}と　{B_LINK_OPPONENT2_NAME}は\nにげだした！"); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("You defeated {B_TRAINER1_NAME_WITH_CLASS}!\p${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}との　しょうぶに　かった！\p");
static const u8 sText_OpponentMon1Appeared[] = _("{B_OPPONENT_MON1_NAME} appeared!\p${JPN}{JPN}{B_OPPONENT_MON1_NAME}が　あらわれた！\p");
static const u8 sText_WildPkmnAppeared[] = _("You encountered a wild {B_OPPONENT_MON1_NAME}!\p${JPN}{JPN}あっ！　やせいの　{B_OPPONENT_MON1_NAME}が\nとびだしてきた！\p");
static const u8 sText_LegendaryPkmnAppeared[] = _("You encountered a wild {B_OPPONENT_MON1_NAME}!\p${JPN}{JPN}あっ！　やせいの　{B_OPPONENT_MON1_NAME}が\nとびだしてきた！\p");
static const u8 sText_WildPkmnAppearedPause[] = _("You encountered a wild {B_OPPONENT_MON1_NAME}!{PAUSE 127}${JPN}{JPN}あっ！　やせいの　{B_OPPONENT_MON1_NAME}が\nとびだしてきた！{PAUSE 127}");
static const u8 sText_TwoWildPkmnAppeared[] = _("Oh! A wild {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME} appeared!\p${JPN}{JPN}あっ！　やせいの　{B_OPPONENT_MON1_NAME}と\n{B_OPPONENT_MON2_NAME}が　とびだしてきた！\p");
static const u8 sText_GhostAppearedCantId[] = _("The GHOST appeared!\pDarn!\nThe GHOST can't be ID'd!\p${JPN}{JPN}ゆうれいが　あらわれた！\pダメだ！\nゆうれいの　しょうたいが　わからない！\p");
static const u8 sText_TheGhostAppeared[] = _("The GHOST appeared!\p${JPN}{JPN}ゆうれいが　あらわれた！\p");
static const u8 sText_Trainer1WantsToBattle[] = _("You are challenged by {B_TRAINER1_NAME_WITH_CLASS}!\p${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}が　しょうぶを\nしかけてきた！\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("You are challenged by {B_LINK_OPPONENT1_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}が　しょうぶを\nしかけてきた！");
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("You are challenged by {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}と　{B_LINK_OPPONENT2_NAME}が\nしょうぶを　しかけてきた！");
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} sent out {B_OPPONENT_MON1_NAME}!${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}は　{B_OPPONENT_MON1_NAME}を\nくりだした！");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} sent out {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}は　{B_OPPONENT_MON1_NAME}と\n{B_OPPONENT_MON2_NAME}を　くりだした！");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_NAME_WITH_CLASS} sent out {B_BUFF1}!${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}は　{B_BUFF1}を\nくりだした！");
static const u8 sText_LinkTrainerIntroSendOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_LINK_OPPONENT_MON1_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}は　{B_LINK_OPPONENT_MON1_NAME}を\nくりだした！");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_BUFF1}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}は　{B_BUFF1}を\nくりだした！");
static const u8 sText_LinkTrainer2SentOutPkmn2[] = _("{B_LINK_OPPONENT2_NAME} sent out {B_BUFF1}!${JPN}{JPN}{B_LINK_OPPONENT2_NAME}は　{B_BUFF1}を\nくりだした！");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}は　{B_OPPONENT_MON1_NAME}と\n{B_OPPONENT_MON2_NAME}を　くりだした！");
static const u8 sText_TwoLinkTrainersIntroSendOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_LINK_OPPONENT_MON1_NAME}! {B_LINK_OPPONENT2_NAME} sent out {B_LINK_OPPONENT_MON2_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}は　{B_LINK_OPPONENT_MON1_NAME}を\nくりだした！　{B_LINK_OPPONENT2_NAME}は\l{B_LINK_OPPONENT_MON2_NAME}を　くりだした！");
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_LINK_OPPONENT_MON2_NAME}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}は　{B_LINK_OPPONENT_MON2_NAME}を\nくりだした！");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} sent out {B_BUFF1}!${JPN}{JPN}{B_LINK_SCR_TRAINER_NAME}は　{B_BUFF1}を\nくりだした！");
static const u8 sText_GoPkmn[] = _("Go! {B_PLAYER_MON1_NAME}!${JPN}{JPN}ゆけっ！　{B_PLAYER_MON1_NAME}！");
static const u8 sText_GoTwoPkmn[] = _("Go! {B_PLAYER_MON1_NAME} and {B_PLAYER_MON2_NAME}!${JPN}{JPN}ゆけっ！　{B_PLAYER_MON1_NAME}！\n{B_PLAYER_MON2_NAME}！");
static const u8 sText_GoPkmn2[] = _("Go! {B_BUFF1}!${JPN}{JPN}ゆけっ！　{B_BUFF1}！");
static const u8 sText_DoItPkmn[] = _("You're in charge, {B_BUFF1}!${JPN}{JPN}いってこい！　{B_BUFF1}！");
static const u8 sText_GoForItPkmn[] = _("Go for it, {B_BUFF1}!${JPN}{JPN}がんばれ！　{B_BUFF1}！");
static const u8 sText_BeCarefulPkmn[] = _("Be careful, {B_PLAYER_MON1_NAME}!${JPN}{JPN}きを　つけろ！　{B_PLAYER_MON1_NAME}！");
static const u8 sText_JustALittleMorePkmn[] = _("Just a little more! Hang in there, {B_BUFF1}!${JPN}{JPN}あと　すこしだ！　がんばれ\n{B_BUFF1}！"); //currently unused, will require code changes
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("Your opponent's weak! Get 'em, {B_BUFF1}!${JPN}{JPN}あいては　よわっている！　チャンスだ\n{B_BUFF1}！");
static const u8 sText_LinkPartnerSentOutPkmn1GoPkmn[] = _("{B_LINK_PARTNER_NAME} sent out {B_LINK_PLAYER_MON1_NAME}! Go, {B_LINK_PLAYER_MON2_NAME}!${JPN}{JPN}{B_LINK_PARTNER_NAME}は　{B_LINK_PLAYER_MON1_NAME}を\nくりだした！　ゆけっ！　{B_LINK_PLAYER_MON2_NAME}！");
static const u8 sText_LinkPartnerSentOutPkmn2GoPkmn[] = _("{B_LINK_PARTNER_NAME} sent out {B_LINK_PLAYER_MON2_NAME}! Go, {B_LINK_PLAYER_MON1_NAME}!${JPN}{JPN}{B_LINK_PARTNER_NAME}は　{B_LINK_PLAYER_MON2_NAME}を\nくりだした！　ゆけっ！　{B_LINK_PLAYER_MON1_NAME}！");
static const u8 sText_LinkPartnerSentOutPkmn1[] = _("{B_LINK_PARTNER_NAME} sent out {B_BUFF1}!${JPN}{JPN}{B_LINK_PARTNER_NAME}は　{B_BUFF1}を\nくりだした！");
static const u8 sText_LinkPartnerSentOutPkmn2[] = _("{B_LINK_PARTNER_NAME} sent out {B_BUFF1}!${JPN}{JPN}{B_LINK_PARTNER_NAME}は　{B_BUFF1}を\nくりだした！");
static const u8 sText_LinkPartnerWithdrewPkmn1[] = _("{B_LINK_PARTNER_NAME} withdrew {B_LINK_PLAYER_MON1_NAME}!${JPN}{JPN}{B_LINK_PARTNER_NAME}は　{B_LINK_PLAYER_MON1_NAME}を\nひっこめた！");
static const u8 sText_LinkPartnerWithdrewPkmn2[] = _("{B_LINK_PARTNER_NAME} withdrew {B_LINK_PLAYER_MON2_NAME}!${JPN}{JPN}{B_LINK_PARTNER_NAME}は　{B_LINK_PLAYER_MON2_NAME}を\nひっこめた！");
static const u8 sText_PkmnSwitchOut[] = _("{B_BUFF1}, switch out! Come back!${JPN}{JPN}{B_BUFF1}　こうたいだ！　もどれ！"); //currently unused, I believe its used for when you switch on a Pokémon in shift mode
static const u8 sText_PkmnThatsEnough[] = _("{B_BUFF1}, that's enough! Come back!${JPN}{JPN}{B_BUFF1}　もういいぞ！　もどれ！");
static const u8 sText_PkmnComeBack[] = _("{B_BUFF1}, come back!${JPN}{JPN}{B_BUFF1}　もどれ！");
static const u8 sText_PkmnOkComeBack[] = _("OK, {B_BUFF1}! Come back!${JPN}{JPN}{B_BUFF1}　いいぞ！　もどれ！");
static const u8 sText_PkmnGoodComeBack[] = _("Good job, {B_BUFF1}! Come back!${JPN}{JPN}{B_BUFF1}　よくやった！　もどれ！");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} withdrew {B_BUFF1}!${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}は　{B_BUFF1}を\nひっこめた！");
static const u8 sText_Trainer2WithdrewPkmn[] = _("{B_TRAINER2_NAME_WITH_CLASS} withdrew {B_BUFF1}!${JPN}{JPN}{B_TRAINER2_NAME_WITH_CLASS}は　{B_BUFF1}を\nひっこめた！");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME} withdrew {B_BUFF1}!${JPN}{JPN}{B_LINK_OPPONENT1_NAME}は　{B_BUFF1}を\nひっこめた！");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_OPPONENT2_NAME} withdrew {B_BUFF1}!${JPN}{JPN}{B_LINK_OPPONENT2_NAME}は　{B_BUFF1}を\nひっこめた！");
static const u8 sText_WildPkmnPrefix[] = _("The wild ${JPN}{JPN}やせいの　");
static const u8 sText_FoePkmnPrefix[] = _("The opposing ${JPN}{JPN}あいての　");
static const u8 sText_WildPkmnPrefixLower[] = _("the wild ${JPN}{JPN}やせいの　");
static const u8 sText_FoePkmnPrefixLower[] = _("the opposing ${JPN}{JPN}あいての　");
static const u8 sText_EmptyString8[] = _("");
static const u8 sText_FoePkmnPrefix2[] = _("Opposing${JPN}{JPN}あいての");
static const u8 sText_AllyPkmnPrefix[] = _("Ally${JPN}{JPN}みかたの");
static const u8 sText_FoePkmnPrefix3[] = _("Opposing${JPN}{JPN}あいての");
static const u8 sText_AllyPkmnPrefix2[] = _("Ally${JPN}{JPN}みかたの");
static const u8 sText_FoePkmnPrefix4[] = _("Opposing${JPN}{JPN}あいての");
static const u8 sText_AllyPkmnPrefix3[] = _("Ally${JPN}{JPN}みかたの");
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX} used {B_BUFF3}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　{B_BUFF3}！");
static const u8 sText_ExclamationMark[] = _("!");
static const u8 sText_ExclamationMark2[] = _("!");
static const u8 sText_ExclamationMark3[] = _("!");
static const u8 sText_ExclamationMark4[] = _("!");
static const u8 sText_ExclamationMark5[] = _("!");
static const u8 sText_HP[] = _("HP${JPN}{JPN}HP");
static const u8 sText_Attack[] = _("Attack${JPN}{JPN}こうげき");
static const u8 sText_Defense[] = _("Defense${JPN}{JPN}ぼうぎょ");
static const u8 sText_Speed[] = _("Speed${JPN}{JPN}すばやさ");
static const u8 sText_SpAttack[] = _("Sp. Atk${JPN}{JPN}とくこう");
static const u8 sText_SpDefense[] = _("Sp. Def${JPN}{JPN}とくぼう");
static const u8 sText_Accuracy[] = _("accuracy${JPN}{JPN}めいちゅうりつ");
static const u8 sText_Evasiveness[] = _("evasiveness${JPN}{JPN}かいひりつ");

const u8 *const gStatNamesTable[NUM_BATTLE_STATS] =
{
    [STAT_HP]      = sText_HP,
    [STAT_ATK]     = sText_Attack,
    [STAT_DEF]     = sText_Defense,
    [STAT_SPEED]   = sText_Speed,
    [STAT_SPATK]   = sText_SpAttack,
    [STAT_SPDEF]   = sText_SpDefense,
    [STAT_ACC]     = sText_Accuracy,
    [STAT_EVASION] = sText_Evasiveness,
};
const u8 *const gPokeblockWasTooXStringTable[FLAVOR_COUNT] =
{
    [FLAVOR_SPICY]  = COMPOUND_STRING("was too spicy!${JPN}{JPN}からすぎた！"),
    [FLAVOR_DRY]    = COMPOUND_STRING("was too dry!${JPN}{JPN}しぶすぎた！"),
    [FLAVOR_SWEET]  = COMPOUND_STRING("was too sweet!${JPN}{JPN}あますぎた！"),
    [FLAVOR_BITTER] = COMPOUND_STRING("was too bitter!${JPN}{JPN}にがすぎた！"),
    [FLAVOR_SOUR]   = COMPOUND_STRING("was too sour!${JPN}{JPN}すっぱすぎた！"),
};

static const u8 sText_Someones[] = _("someone's${JPN}{JPN}だれかの");
static const u8 sText_Lanettes[] = _("LANETTE's${JPN}{JPN}マユミの"); //no decapitalize until it is everywhere
static const u8 sText_Bills[] = _("BILL's${JPN}{JPN}マサキの");
static const u8 sText_EnigmaBerry[] = _("ENIGMA BERRY${JPN}{JPN}ナゾのみ"); //no decapitalize until it is everywhere
static const u8 sText_BerrySuffix[] = _(" BERRY${JPN}{JPN}のみ"); //no decapitalize until it is everywhere
const u8 gText_EmptyString3[] = _("");

static const u8 sText_TwoInGameTrainersDefeated[] = _("You defeated {B_TRAINER1_NAME_WITH_CLASS} and {B_TRAINER2_NAME_WITH_CLASS}!\p${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}と　{B_TRAINER2_NAME_WITH_CLASS}との\nしょうぶに　かった！\p");

// New battle strings.
const u8 gText_drastically[] = _(" drastically${JPN}{JPN}ぐぐーんと　");
const u8 gText_severely[] = _("severely ${JPN}{JPN}がくーんと　");
static const u8 sText_TerrainReturnedToNormal[] = _("The terrain returned to normal!${JPN}{JPN}フィールドが　もとに　もどった！"); // Unused

const u8 *const gBattleStringsTable[STRINGID_COUNT] =
{
    [STRINGID_TRAINER1LOSETEXT]                     = COMPOUND_STRING("{B_TRAINER1_LOSE_TEXT}"),
    [STRINGID_PKMNGAINEDEXP]                        = COMPOUND_STRING("{B_BUFF1} gained{B_BUFF2} {B_BUFF3} Exp. Points!\p${JPN}{JPN}{B_BUFF1}は　{B_BUFF2}\n{B_BUFF3}けいけんちを　もらった！\p"),
    [STRINGID_PKMNGREWTOLV]                         = COMPOUND_STRING("{B_BUFF1} grew to Lv. {B_BUFF2}!{WAIT_SE}\p${JPN}{JPN}{B_BUFF1}は\nレベル{B_BUFF2}に　あがった！{WAIT_SE}\p"),
    [STRINGID_PKMNLEARNEDMOVE]                      = COMPOUND_STRING("{B_BUFF1} learned {B_BUFF2}!{WAIT_SE}\p${JPN}{JPN}{B_BUFF1}は　{B_BUFF2}を\nおぼえた！{WAIT_SE}\p"),
    [STRINGID_TRYTOLEARNMOVE1]                      = COMPOUND_STRING("{B_BUFF1} wants to learn the move {B_BUFF2}.\p${JPN}{JPN}{B_BUFF1}は　あたらしく\n{B_BUFF2}を　おぼえたい‥\p"),
    [STRINGID_TRYTOLEARNMOVE2]                      = COMPOUND_STRING("However, {B_BUFF1} already knows four moves.\p${JPN}{JPN}しかし　{B_BUFF1}は　わざを　4つ\nおぼえるので　せいいっぱいだ！\p"),
    [STRINGID_TRYTOLEARNMOVE3]                      = COMPOUND_STRING("Should another move be forgotten and replaced with {B_BUFF2}?${JPN}{JPN}{B_BUFF2}を　おぼえるために　ほかの\nわざを　わすれさせますか？"),
    [STRINGID_PKMNFORGOTMOVE]                       = COMPOUND_STRING("{B_BUFF1} forgot {B_BUFF2}…\p${JPN}{JPN}{B_BUFF1}は　{B_BUFF2}を\nきれいに　わすれた\p"),
    [STRINGID_STOPLEARNINGMOVE]                     = COMPOUND_STRING("{PAUSE 32}Do you want to give up on having {B_BUFF1} learn {B_BUFF2}?${JPN}{JPN}{PAUSE 32}それでは‥　{B_BUFF1}に\n{B_BUFF2}を　おぼえさせるのを\lあきらめますか？"),
    [STRINGID_DIDNOTLEARNMOVE]                      = COMPOUND_STRING("{B_BUFF1} did not learn {B_BUFF2}.\p${JPN}{JPN}{B_BUFF1}は　{B_BUFF2}を\nおぼえずに　おわった！\p"),
    [STRINGID_PKMNLEARNEDMOVE2]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} learned {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}を\nおぼえた！"),
    [STRINGID_PKMNPROTECTEDITSELF]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} protected itself!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　こうげきから　みを\nまもった！"),
    [STRINGID_ITDOESNTAFFECT]                       = COMPOUND_STRING("It doesn't affect {B_DEF_NAME_WITH_PREFIX2}…${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}には　こうかが\nないみたいだ‥"),
    [STRINGID_ITDOESNTAFFECTSCR]                    = COMPOUND_STRING("It doesn't affect {B_SCR_NAME_WITH_PREFIX2}…${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX2}には　こうかが\nないみたいだ‥"),
    [STRINGID_BATTLERFAINTED]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} fainted!\p${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　たおれた！\p"),
    [STRINGID_PLAYERGOTMONEY]                       = COMPOUND_STRING("You got ¥{B_BUFF1} for winning!\p${JPN}{JPN}しょうきんとして　{B_BUFF1}円\nてにいれた！\p"),
    [STRINGID_PLAYERWHITEOUT]                       = COMPOUND_STRING("You have no more Pokémon that can fight!\p${JPN}{JPN}たたかえる　ポケモンが　もう　いない！\p"),
    [STRINGID_PLAYERWHITEOUT2_WILD]                 = COMPOUND_STRING("You panicked and dropped ¥{B_BUFF1}…${JPN}{JPN}あわてて　{B_BUFF1}円\nおとしてしまった‥"),
    [STRINGID_PLAYERWHITEOUT2_TRAINER]              = COMPOUND_STRING("You gave ¥{B_BUFF1} to the winner…${JPN}{JPN}しょうきんとして　{B_BUFF1}円\nあいてに　わたした‥"),
    [STRINGID_PLAYERWHITEOUT3]                      = COMPOUND_STRING("You were overwhelmed by your defeat!${JPN}{JPN}めのまえが　まっくらに　なった！"),
    [STRINGID_PREVENTSESCAPE]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} prevents escape with {B_SCR_ABILITY}!\p${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\nにげられない！\p"),
    [STRINGID_HITXTIMES]                            = COMPOUND_STRING("The Pokémon was hit {B_BUFF1} time{B_BUFF2}!${JPN}{JPN}{B_BUFF1}かい\nあたった！{B_BUFF2}"),
    [STRINGID_PKMNFELLASLEEP]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} fell asleep!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　ねむってしまった！"),
    [STRINGID_PKMNMADESLEEP]                        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} made {B_EFF_NAME_WITH_PREFIX2} sleep!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は　ねむってしまった！"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYASLEEP]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is already asleep!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　すでに　ねむっている！"),
    [STRINGID_PKMNALREADYASLEEP2]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is already asleep!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　すでに　ねむっている！"),
    [STRINGID_PKMNWASPOISONED]                      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was poisoned!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　どくを　あびた！"),
    [STRINGID_PKMNPOISONEDBY]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was poisoned by {B_SCR_NAME_WITH_PREFIX2}'s {B_BUFF1}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX2}の　{B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX}は　どくを　あびた！"), //not in gen 5+, ability popup
    [STRINGID_PKMNHURTBYPOISON]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt by its poisoning!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　どくの　ダメージを\nうけている！"),
    [STRINGID_PKMNALREADYPOISONED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is already poisoned!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　すでに　どくを\nあびている！"),
    [STRINGID_PKMNBADLYPOISONED]                    = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was badly poisoned!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　もうどくを　あびた！"),
    [STRINGID_PKMNENERGYDRAINED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} had its energy drained!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}から　たいりょくを\nすいとった！"),
    [STRINGID_PKMNWASBURNED]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was burned!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　やけどを　おった！"),
    [STRINGID_PKMNBURNEDBY]                         = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} burned {B_EFF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は　やけどを　おった！"), //not in gen 5+, ability popup
    [STRINGID_PKMNHURTBYBURN]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt by its burn!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　やけどの　ダメージを\nうけている！"),
    [STRINGID_PKMNWASFROZEN]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was frozen solid!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　こおりづけに　なった！"),
    [STRINGID_PKMNFROZENBY]                         = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} froze {B_EFF_NAME_WITH_PREFIX2} solid!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は　こおりづけに　なった！"), //not in gen 5+, ability popup
    [STRINGID_PKMNISFROZEN]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is frozen solid!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　こおって　しまって\nうごかない！"),
    [STRINGID_PKMNWASDEFROSTED]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} thawed out!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　こおりが　とけた！"),
    [STRINGID_PKMNWASDEFROSTEDBY]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE} melted the ice!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_CURRENT_MOVE}で\nこおりが　とけた！"),
    [STRINGID_PKMNWASPARALYZED]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is paralyzed, so it may be unable to move!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　まひして　わざが\nでにくくなった！"),
    [STRINGID_PKMNWASPARALYZEDBY]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} paralyzed {B_EFF_NAME_WITH_PREFIX2}, so it may be unable to move!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は　まひして　わざが\lでにくくなった！"), //not in gen 5+, ability popup
    [STRINGID_PKMNISPARALYZED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} couldn't move because it's paralyzed!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　からだが　しびれて\nうごけない！"),
    [STRINGID_PKMNISALREADYPARALYZED]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is already paralyzed!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　すでに　まひしている！"),
    [STRINGID_PKMNHEALEDPARALYSIS]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was cured of paralysis!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　まひが　なおった！"),
    [STRINGID_STATSWONTINCREASE]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} won't go any higher!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}は\nもう　あがらない！"),
    [STRINGID_STATSWONTDECREASE]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} won't go any lower!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}は\nもう　さがらない！"),
    [STRINGID_PKMNISCONFUSED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is confused!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　こんらんしている！"),
    [STRINGID_PKMNHEALEDCONFUSION]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} snapped out of its confusion!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　こんらんが　とけた！"),
    [STRINGID_PKMNWASCONFUSED]                      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became confused!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　こんらんした！"),
    [STRINGID_PKMNALREADYCONFUSED]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is already confused!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　すでに\nこんらんしている！"),
    [STRINGID_PKMNFELLINLOVE]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fell in love!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　メロメロに　なった！"),
    [STRINGID_PKMNINLOVE]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is in love with {B_SCR_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_SCR_NAME_WITH_PREFIX2}に\nメロメロだ！"),
    [STRINGID_PKMNIMMOBILIZEDBYLOVE]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is immobilized by love!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　メロメロで　わざが\nだせなかった！"),
    [STRINGID_PKMNCHANGEDTYPE]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} transformed into the {B_BUFF1} type!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}タイプに　なった！"),
    [STRINGID_PKMNFLINCHED]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} flinched and couldn't move!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ひるんで　わざが\nだせない！"),
    [STRINGID_PKMNREGAINEDHEALTH]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s HP was restored.${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　たいりょくが\nかいふくした！"),
    [STRINGID_PKMNHPFULL]                           = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s HP is full!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　たいりょくは\nまんたんだ！"),
    [STRINGID_PKMNRAISEDSPDEF]                      = COMPOUND_STRING("Light Screen made {B_ATK_TEAM2} side stronger against special moves!${JPN}{JPN}ひかりのかべで　{B_ATK_TEAM2}の\nとくしゅこうげきに　つよくなった！"),
    [STRINGID_PKMNRAISEDDEF]                        = COMPOUND_STRING("Reflect made {B_ATK_TEAM2} side stronger against physical moves!${JPN}{JPN}リフレクターで　{B_ATK_TEAM2}の\nぶつりこうげきに　つよくなった！"),
    [STRINGID_PKMNAURORAVEIL]                       = COMPOUND_STRING("Aurora Veil made {B_ATK_TEAM2} side stronger against physical and special moves!${JPN}{JPN}オーロラベールで　{B_ATK_TEAM2}の\nぶつりこうげきと　とくしゅこうげきに\lつよくなった！"),
    [STRINGID_PKMNCOVEREDBYVEIL]                    = COMPOUND_STRING("{B_ATK_TEAM1} side became cloaked in a mystical veil!${JPN}{JPN}{B_ATK_TEAM1}は　しんぴの　ベールに\nつつまれた！"),
    [STRINGID_PKMNUSEDSAFEGUARD]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is protected by Safeguard!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　しんぴの　ベールに\nまもられている！"),
    [STRINGID_PKMNSAFEGUARDEXPIRED]                 = COMPOUND_STRING("{B_ATK_TEAM1} side is no longer protected by the mystical veil!${JPN}{JPN}{B_ATK_TEAM1}を　つつんでいた　しんぴの\nベールが　なくなった！"),
    [STRINGID_PKMNWENTTOSLEEP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} went to sleep!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ねむりはじめた！"), //not in gen 5+
    [STRINGID_PKMNSLEPTHEALTHY]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} slept and restored its HP!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ねむって　げんきに\nなった！"),
    [STRINGID_PKMNWHIPPEDWHIRLWIND]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} whipped up a whirlwind!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　まわりで　くうきが\nうずを　まく！"),
    [STRINGID_PKMNTOOKSUNLIGHT]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbed light!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ひかりを\nきゅうしゅうした！"),
    [STRINGID_PKMNLOWEREDHEAD]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} tucked in its head!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　くびを　ひっこめた！"),
    [STRINGID_PKMNFLEWHIGH]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} flew up high!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　そらたかく\nとびあがった！"),
    [STRINGID_PKMNDUGHOLE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} burrowed its way under the ground!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　じめんに　もぐった！"),
    [STRINGID_PKMNSQUEEZEDBYBIND]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was squeezed by {B_SCR_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　{B_SCR_NAME_WITH_PREFIX2}に\nしめつけられた！"),
    [STRINGID_PKMNTRAPPEDINVORTEX]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became trapped in the vortex!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　うずの　なかに\nとじこめられた！"),
    [STRINGID_PKMNWRAPPEDBY]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was wrapped by {B_SCR_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　{B_SCR_NAME_WITH_PREFIX2}に\nまきつかれた！"),
    [STRINGID_PKMNCLAMPED]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} clamped down on {B_EFF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_EFF_NAME_WITH_PREFIX2}を\nからに　はさんだ！"),
    [STRINGID_PKMNHURTBY]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}の\nダメージを　うけている！"),
    [STRINGID_PKMNFREEDFROM]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was freed from {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}から\nかいほうされた！"),
    [STRINGID_PKMNCRASHED]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} kept going and crashed!${JPN}{JPN}いきおい　あまって　{B_ATK_NAME_WITH_PREFIX}は\nじめんに　ぶつかった！"),
    [STRINGID_PKMNSHROUDEDINMIST]                   = gText_PkmnShroudedInMist,
    [STRINGID_PKMNPROTECTEDBYMIST]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is protected by the mist!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　しろいきりに\nまもられている！"),
    [STRINGID_PKMNGETTINGPUMPED]                    = gText_PkmnGettingPumped,
    [STRINGID_PKMNHITWITHRECOIL]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was damaged by the recoil!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　はんどうによる\nダメージを　うけた！"),
    [STRINGID_PKMNPROTECTEDITSELF2]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} protected itself!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　まもりの　たいせいに\nはいった！"),
    [STRINGID_PKMNBUFFETEDBYSANDSTORM]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is buffeted by the sandstorm!${JPN}{JPN}すなあらしが　{B_ATK_NAME_WITH_PREFIX}を　おそう！"),
    [STRINGID_PKMNPELTEDBYHAIL]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is buffeted by the hail!${JPN}{JPN}あられが　{B_ATK_NAME_WITH_PREFIX}を　おそう！"),
    [STRINGID_PKMNSEEDED]                           = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was seeded!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}に　たねを　うえつけた！"),
    [STRINGID_PKMNAVOIDEDATTACK]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} avoided the attack!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　こうげきを　かわした！"),
    [STRINGID_BATTLERAVOIDEDATTACK]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} avoided the attack!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　こうげきを　かわした！"),
    [STRINGID_PKMNSAPPEDBYLEECHSEED]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s health is sapped by Leech Seed!${JPN}{JPN}やどりぎが　{B_SCR_NAME_WITH_PREFIX}の　たいりょくを\nうばう！"),
    [STRINGID_PKMNFASTASLEEP]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is fast asleep.${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ぐうぐう　ねむっている"),
    [STRINGID_PKMNWOKEUP]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} woke up!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　めを　さました！"),
    [STRINGID_PKMNWOKEUPINUPROAR]                   = COMPOUND_STRING("The uproar woke {B_EFF_NAME_WITH_PREFIX2}!${JPN}{JPN}さわぐ　こえで　{B_EFF_NAME_WITH_PREFIX2}は　めを\nさました！"),
    [STRINGID_PKMNCAUSEDUPROAR]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} caused an uproar!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　さわぎだした！"),
    [STRINGID_PKMNMAKINGUPROAR]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is making an uproar!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　さわいでいる！"),
    [STRINGID_PKMNCALMEDDOWN]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} calmed down.${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　おとなしくなった"),
    [STRINGID_PKMNSTOCKPILED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} stockpiled {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}つ\nたくわえた！"),
    [STRINGID_PKMNCANTSLEEPINUPROAR2]               = COMPOUND_STRING("But {B_DEF_NAME_WITH_PREFIX2} can't sleep in an uproar!${JPN}{JPN}しかし　{B_DEF_NAME_WITH_PREFIX2}は　さわいでいて\nねむれない！"),
    [STRINGID_UPROARKEPTPKMNAWAKE]                  = COMPOUND_STRING("But the uproar kept {B_DEF_NAME_WITH_PREFIX2} awake!${JPN}{JPN}しかし　さわがしくて　{B_DEF_NAME_WITH_PREFIX2}は\nねむれない！"),
    [STRINGID_PKMNSTAYEDAWAKEUSING]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} stayed awake!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　ねむらない！"),
    [STRINGID_PKMNSTORINGENERGY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is storing energy!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　がまんを　している！"),
    [STRINGID_PKMNUNLEASHEDENERGY]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} unleashed its energy!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　がまんが　とかれた！"),
    [STRINGID_PKMNFATIGUECONFUSION]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} became confused due to fatigue!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　つかれはてて\nこんらんした！"),
    [STRINGID_PLAYERPICKEDUPMONEY]                  = COMPOUND_STRING("You picked up ¥{B_BUFF1}!\p${JPN}{JPN}{B_BUFF1}円　ひろった！\p"),
    [STRINGID_PKMNUNAFFECTED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is unaffected!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}には　こうかが　ない！"),
    [STRINGID_PKMNTRANSFORMEDINTO]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} transformed into {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}に\nへんしんした！"),
    [STRINGID_PKMNMADESUBSTITUTE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} put in a substitute!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　ぶんしんが　あらわれた！"),
    [STRINGID_PKMNHASSUBSTITUTE]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} already has a substitute!${JPN}{JPN}しかし　{B_ATK_NAME_WITH_PREFIX}の　ぶんしんは\nすでに　でていた！"),
    [STRINGID_SUBSTITUTEDAMAGED]                    = COMPOUND_STRING("The substitute took damage for {B_DEF_NAME_WITH_PREFIX2}!\p${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}に　かわって　ぶんしんが\nこうげきを　うけた！\p"),
    [STRINGID_PKMNSUBSTITUTEFADED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s substitute faded!\p${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　ぶんしんは\nきえてしまった‥\p"),
    [STRINGID_PKMNMUSTRECHARGE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} must recharge!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　はんどうで　うごけない！"),
    [STRINGID_PKMNRAGEBUILDING]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s rage is building!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　いかりの　ボルテージが\nあがっていく！"),
    [STRINGID_PKMNMOVEWASDISABLED]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1} was disabled!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_BUFF1}を\nふうじこめた！"),
    [STRINGID_PKMNMOVEISDISABLED]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE} is disabled!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　かなしばりで\n{B_CURRENT_MOVE}が　だせない！\p"),
    [STRINGID_PKMNMOVEDISABLEDNOMORE]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s move is no longer disabled!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　かなしばりが　とけた！"),
    [STRINGID_PKMNGOTENCORE]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} must do an encore!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　アンコールを　うけた！"),
    [STRINGID_PKMNGOTENCOREDMOVE]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} can only use {B_CURRENT_MOVE}!\p${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_CURRENT_MOVE}しか\nだせない！\p"),
    [STRINGID_PKMNENCOREENDED]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} ended its encore!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　アンコールじょうたいが\nとけた！"),
    [STRINGID_PKMNTOOKAIM]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} took aim at {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}に\nねらいを　さだめた！"),
    [STRINGID_PKMNSKETCHEDMOVE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} sketched {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}を\nスケッチした！"),
    [STRINGID_PKMNTRYINGTOTAKEFOE]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hoping to take its attacker down with it!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいてを　みちづれに\nしようとしている！"),
    [STRINGID_PKMNTOOKFOE]                          = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} took its attacker down with it!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　あいてを　みちづれに\nした！"),
    [STRINGID_PKMNREDUCEDPP]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} lost {B_BUFF2} PP from {B_BUFF1}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_BUFF1}を\n{B_BUFF2}けずった！"),
    [STRINGID_PKMNSTOLEITEM]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} stole {B_BUFF2}'s {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF2}から\n{B_LAST_ITEM}を　うばいとった！"),
    [STRINGID_TARGETCANTESCAPENOW]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} can no longer escape!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　もう　にげられない！"),
    [STRINGID_PKMNFELLINTONIGHTMARE]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} began having a nightmare!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　あくむを　みはじめた！"),
    [STRINGID_PKMNLOCKEDINNIGHTMARE]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is locked in a nightmare!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あくむに\nうなされている！"),
    [STRINGID_PKMNLAIDCURSE]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cut its own HP and put a curse on {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　じぶんの　たいりょくを\nけずって　{B_DEF_NAME_WITH_PREFIX2}に　のろいを\lかけた！"),
    [STRINGID_PKMNAFFLICTEDBYCURSE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is afflicted by the curse!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　のろわれている！"),
    [STRINGID_SPIKESSCATTERED]                      = COMPOUND_STRING("Spikes were scattered on the ground all around {B_DEF_TEAM2} side!${JPN}{JPN}{B_DEF_TEAM2}の　あしもとに　まきびしが\nちらばった！"),
    [STRINGID_PKMNHURTBYSPIKES]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} was hurt by the spikes!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　まきびしの　ダメージを\nうけた！"),
    [STRINGID_PKMNIDENTIFIED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was identified!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　しょうたいを\nみやぶった！"),
    [STRINGID_PKMNPERISHCOUNTFELL]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s perish count fell to {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　ほろびの　カウントが\n{B_BUFF1}に　なった！"),
    [STRINGID_PKMNBRACEDITSELF]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} braced itself!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　こらえる　たいせいに\nはいった！"),
    [STRINGID_PKMNENDUREDHIT]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} endured the hit!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　こうげきを　こらえた！"),
    [STRINGID_MAGNITUDESTRENGTH]                    = COMPOUND_STRING("Magnitude {B_BUFF1}!${JPN}{JPN}マグニチュード{B_BUFF1}！"),
    [STRINGID_PKMNCUTHPMAXEDATTACK]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cut its own HP and maximized its Attack!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　たいりょくを　けずって\nパワーぜんかい！"),
    [STRINGID_PKMNCOPIEDSTATCHANGES]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} copied {B_EFF_NAME_WITH_PREFIX2}'s stat changes!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_EFF_NAME_WITH_PREFIX2}の\nのうりょくへんかを　コピーした！"),
    [STRINGID_PKMNGOTFREE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was freed from {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}から\nかいほうされた！"),
    [STRINGID_PKMNSHEDLEECHSEED]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was freed from Leech Seed!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　やどりぎの　タネから\nかいほうされた！"),
    [STRINGID_PKMNBLEWAWAYSPIKES]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away Spikes!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　まきびしを\nふきとばした！"), // Not in gen 5+
    [STRINGID_PKMNFLEDFROMBATTLE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fled from battle!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　にげだした！"),
    [STRINGID_PKMNFORESAWATTACK]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} foresaw an attack!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　みらいに　こうげきを\nよちした！"),
    [STRINGID_PKMNTOOKATTACK]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} took the {B_BUFF1} attack!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_BUFF1}の\nこうげきを　うけた！"),
    [STRINGID_PKMNATTACK]                           = COMPOUND_STRING("{B_BUFF1}'s attack!${JPN}{JPN}{B_BUFF1}の　こうげき！"), // Not in gen 5+
    [STRINGID_PKMNCENTERATTENTION]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} became the center of attention!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　ちゅうもくの　まとに\nなった！"),
    [STRINGID_PKMNCHARGINGPOWER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} began charging power!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　じゅうでんを　はじめた！"),
    [STRINGID_NATUREPOWERTURNEDINTO]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s Nature Power turned into {B_CURRENT_MOVE}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　しぜんのちからは\n{B_CURRENT_MOVE}に　なった！"),
    [STRINGID_PKMNSTATUSNORMAL]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s status returned to normal!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　じょうたいが　もとに\nもどった！"),
    [STRINGID_PKMNHASNOMOVESLEFT]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} has no moves left that it can use!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　だせる　わざが　ない！\p"),
    [STRINGID_PKMNSUBJECTEDTOTORMENT]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was subjected to torment!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　いちゃもんを\nつけられた！"),
    [STRINGID_PKMNCANTUSEMOVETORMENT]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use the same move twice in a row due to the torment!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　いちゃもんを　つけられて\n2かい　つづけて　おなじ　わざが　だせない！\p"),
    [STRINGID_PKMNTIGHTENINGFOCUS]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is tightening its focus!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　きあいを　ためている！"),
    [STRINGID_PKMNFELLFORTAUNT]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fell for the taunt!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　ちょうはつに\nのってしまった！"),
    [STRINGID_PKMNCANTUSEMOVETAUNT]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use {B_CURRENT_MOVE} after the taunt!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ちょうはつされて\n{B_CURRENT_MOVE}が　だせない！\p"),
    [STRINGID_PKMNREADYTOHELP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is ready to help {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}を\nてだすけする　たいせいに　はいった！"),
    [STRINGID_PKMNSWITCHEDITEMS]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} switched items with its target!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　おたがいの　どうぐを\nこうかんした！"),
    [STRINGID_PKMNCOPIEDFOE]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} copied {B_DEF_NAME_WITH_PREFIX2}'s Ability!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}の\nとくせいを　コピーした！"),
    [STRINGID_PKMNWISHCAMETRUE]                     = COMPOUND_STRING("{B_BUFF1}'s wish came true!${JPN}{JPN}{B_BUFF1}の　ねがいごとが　かなった！"),
    [STRINGID_PKMNPLANTEDROOTS]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} planted its roots!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ねを　はった！"),
    [STRINGID_PKMNABSORBEDNUTRIENTS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbed nutrients with its roots!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ねから　ようぶんを\nすいとった！"),
    [STRINGID_PKMNANCHOREDITSELF]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is anchored in place with its roots!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　ねを　はって\nふんばっている！"),
    [STRINGID_PKMNWASMADEDROWSY]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} grew drowsy!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　ねむけを　さそった！"),
    [STRINGID_PKMNKNOCKEDOFF]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} knocked off {B_EFF_NAME_WITH_PREFIX2}'s {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_EFF_NAME_WITH_PREFIX2}の\n{B_LAST_ITEM}を　はたきおとした！"),
    [STRINGID_PKMNSWAPPEDABILITIES]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} swapped Abilities with its target!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　おたがいの　とくせいを\nいれかえた！"),
    [STRINGID_PKMNSEALEDOPPONENTMOVE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} sealed any moves its target shares with it!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいての　わざを\nふういんした！"),
    [STRINGID_PKMNCANTUSEMOVESEALED]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use its sealed {B_CURRENT_MOVE}!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ふういんされた\n{B_CURRENT_MOVE}を　だせない！\p"),
    [STRINGID_PKMNWANTSGRUDGE]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} wants its target to bear a grudge!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいてに　おんねんを\nかけようとしている！"),
    [STRINGID_PKMNLOSTPPGRUDGE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} lost all of {B_BUFF1}'s PP due to the grudge!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　{B_BUFF1}は\nおんねんで　PPが　0に　なった！"),
    [STRINGID_PKMNSHROUDEDITSELF]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} shrouded itself with Magic Coat!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　マジックコートに\nつつまれた！"),
    [STRINGID_PKMNMOVEBOUNCED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} bounced the {B_CURRENT_MOVE} back!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_CURRENT_MOVE}を\nはねかえした！"),
    [STRINGID_PKMNWAITSFORTARGET]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is waiting for a target to make a move!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいての　うごきを\nまっている！"),
    [STRINGID_PKMNSNATCHEDMOVE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} snatched {B_SCR_NAME_WITH_PREFIX2}'s move!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_SCR_NAME_WITH_PREFIX2}の\nわざを　よこどりした！"),
    [STRINGID_PKMNMADEITRAIN]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} made it rain!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\nあめが　ふりはじめた！"), //not in gen 5+, ability popup
    [STRINGID_PKMNPROTECTEDBY]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was protected by {B_DEF_ABILITY}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_DEF_ABILITY}で\nまもられている！"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSUSAGE]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} prevents {B_ATK_NAME_WITH_PREFIX2} from using {B_CURRENT_MOVE}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_DEF_ABILITY}で\n{B_ATK_NAME_WITH_PREFIX2}は　{B_CURRENT_MOVE}が\lだせない！"), //not in gen 5+, ability popup
    [STRINGID_PKMNRESTOREDHPUSING]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} had its HP restored.${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　たいりょくが\nかいふくした！"),
    [STRINGID_PKMNCHANGEDTYPEWITH]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s type changed to {B_BUFF1}!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は\n{B_BUFF1}タイプに　なった！"),
    [STRINGID_PKMNPREVENTSROMANCEWITH]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} prevents romance!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_DEF_ABILITY}で\nメロメロに　ならない！"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSCONFUSIONWITH]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} cannot be confused!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　こんらんしない！"),
    [STRINGID_PKMNRAISEDFIREPOWERWITH]              = COMPOUND_STRING("The power of {B_SCR_NAME_WITH_PREFIX}'s Fire-type moves rose!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　ほのおタイプの　わざの\nいりょくが　あがった！"),
    [STRINGID_PKMNANCHORSITSELFWITH]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is anchored in place with its suction cups!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　きゅうばんで\nはりついている！"),
    [STRINGID_PKMNPREVENTSSTATLOSSWITH]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s stats were not lowered!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　のうりょくは\nさがらない！"),
    [STRINGID_PKMNHURTSWITH]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt by {B_DEF_NAME_WITH_PREFIX2}'s {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}の\n{B_BUFF1}で　ダメージを　うけた！"),
    [STRINGID_PKMNTRACED]                           = COMPOUND_STRING("It traced {B_BUFF1}'s {B_BUFF2}!${JPN}{JPN}{B_BUFF1}の　{B_BUFF2}を\nトレースした！"),
    [STRINGID_STATSHARPLY]                          = gText_StatSharply,
    [STRINGID_STATHARSHLY]                          = COMPOUND_STRING("harshly ${JPN}{JPN}がくっと　"),
    [STRINGID_STATROSE]                             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} rose{B_BUFF2}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}が\n{B_BUFF2}あがった！"),
    [STRINGID_STATFELL]                             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} {B_BUFF2}fell!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}が\n{B_BUFF2}さがった！"),
    [STRINGID_CRITICALHIT]                          = COMPOUND_STRING("A critical hit!${JPN}{JPN}きゅうしょに　あたった！"),
    [STRINGID_ONEHITKO]                             = COMPOUND_STRING("It's a one-hit KO!${JPN}{JPN}いちげき　ひっさつ！"),
    [STRINGID_123POOF]                              = COMPOUND_STRING("One…{PAUSE 10}two…{PAUSE 10}and…{PAUSE 10}{PAUSE 20}{PLAY_SE SE_BALL_BOUNCE_1}ta-da!\p${JPN}{JPN}1‥{PAUSE 10}　2の‥{PAUSE 10}‥{PAUSE 10}{PAUSE 20}{PLAY_SE SE_BALL_BOUNCE_1}　ポカン！\p"),
    [STRINGID_ANDELLIPSIS]                          = COMPOUND_STRING("And…\p${JPN}{JPN}そして‥\p"),
    [STRINGID_NOTVERYEFFECTIVE]                     = COMPOUND_STRING("It's not very effective…${JPN}{JPN}こうかは　いまひとつの　ようだ‥"),
    [STRINGID_SUPEREFFECTIVE]                       = COMPOUND_STRING("It's super effective!${JPN}{JPN}こうかは　ばつぐんだ！"),
    [STRINGID_GOTAWAYSAFELY]                        = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED]                         = COMPOUND_STRING("{PLAY_SE SE_FLEE}The wild {B_BUFF1} fled!${JPN}{JPN}{PLAY_SE SE_FLEE}やせいの　{B_BUFF1}は　にげだした！"),
    [STRINGID_NORUNNINGFROMTRAINERS]                = COMPOUND_STRING("No! There's no running from a Trainer battle!\p${JPN}{JPN}ダメだ！　トレーナーとの　しょうぶの\nさいちゅうに　あいてに　せなかを　みせられない！\p"),
    [STRINGID_CANTESCAPE]                           = COMPOUND_STRING("You can't escape!\p${JPN}{JPN}にげられない！\p"),
    [STRINGID_DONTLEAVEBIRCH]                       = COMPOUND_STRING("PROF. BIRCH: Don't leave me like this!\p${JPN}{JPN}オダマキはかせ『そんな‥　みすてないで\nくれー！』\p"), //no decapitalize until it is everywhere
    [STRINGID_BUTNOTHINGHAPPENED]                   = COMPOUND_STRING("But nothing happened!${JPN}{JPN}しかし　なにも　おこらない！"),
    [STRINGID_BUTITFAILED]                          = COMPOUND_STRING("But it failed!${JPN}{JPN}しかし　うまく　きまらなかった！"),
    [STRINGID_ITHURTCONFUSION]                      = COMPOUND_STRING("It hurt itself in its confusion!${JPN}{JPN}わけも　わからず　じぶんを　こうげきした！"),
    [STRINGID_STARTEDTORAIN]                        = COMPOUND_STRING("It started to rain!${JPN}{JPN}あめが　ふりだした！"),
    [STRINGID_DOWNPOURSTARTED]                      = COMPOUND_STRING("A downpour started!${JPN}{JPN}おおあめが　ふりだした！"), // corresponds to DownpourText in pokegold and pokecrystal and is used by Rain Dance in GSC
    [STRINGID_RAINCONTINUES]                        = COMPOUND_STRING("Rain continues to fall.${JPN}{JPN}あめが　ふりつづいている"), //not in gen 5+
    [STRINGID_DOWNPOURCONTINUES]                    = COMPOUND_STRING("The downpour continues.${JPN}{JPN}おおあめが　ふりつづいている"), // unused
    [STRINGID_RAINSTOPPED]                          = COMPOUND_STRING("The rain stopped.${JPN}{JPN}あめが　やんだ！"),
    [STRINGID_SANDSTORMBREWED]                      = COMPOUND_STRING("A sandstorm kicked up!${JPN}{JPN}すなあらしが　ふきはじめた！"),
    [STRINGID_SANDSTORMRAGES]                       = COMPOUND_STRING("The sandstorm is raging.${JPN}{JPN}すなあらしが　ふきあれる"),
    [STRINGID_SANDSTORMSUBSIDED]                    = COMPOUND_STRING("The sandstorm subsided.${JPN}{JPN}すなあらしが　おさまった！"),
    [STRINGID_SUNLIGHTGOTBRIGHT]                    = COMPOUND_STRING("The sunlight turned harsh!${JPN}{JPN}ひざしが　つよくなった！"),
    [STRINGID_SUNLIGHTSTRONG]                       = COMPOUND_STRING("The sunlight is strong.${JPN}{JPN}ひざしが　つよい"), //not in gen 5+
    [STRINGID_SUNLIGHTFADED]                        = COMPOUND_STRING("The sunlight faded.${JPN}{JPN}ひざしが　もとに　もどった！"),
    [STRINGID_STARTEDHAIL]                          = COMPOUND_STRING("It started to hail!${JPN}{JPN}あられが　ふりだした！"),
    [STRINGID_HAILCONTINUES]                        = COMPOUND_STRING("The hail is crashing down.${JPN}{JPN}あられが　ふりつづいている"),
    [STRINGID_HAILSTOPPED]                          = COMPOUND_STRING("The hail stopped.${JPN}{JPN}あられが　やんだ！"),
    [STRINGID_STATCHANGESGONE]                      = COMPOUND_STRING("All stat changes were eliminated!${JPN}{JPN}すべての　のうりょくへんかが　もとに　もどった！"),
    [STRINGID_COINSSCATTERED]                       = COMPOUND_STRING("Coins were scattered everywhere!${JPN}{JPN}こばんが　あたりに　ちらばった！"),
    [STRINGID_TOOWEAKFORSUBSTITUTE]                 = COMPOUND_STRING("But it does not have enough HP left to make a substitute!${JPN}{JPN}しかし　ぶんしんを　だすには　たいりょくが\nたりなかった！"),
    [STRINGID_SHAREDPAIN]                           = COMPOUND_STRING("The battlers shared their pain!${JPN}{JPN}おたがいの　たいりょくを　わけあった！"),
    [STRINGID_BELLCHIMED]                           = COMPOUND_STRING("A bell chimed!${JPN}{JPN}すずの　おとが　ひびきわたった！"),
    [STRINGID_FAINTINTHREE]                         = COMPOUND_STRING("All Pokémon that heard the song will faint in three turns!${JPN}{JPN}ほろびのうたを　きいた　ポケモンは　3ターンごに\nほろびてしまう！"),
    [STRINGID_NOPPLEFT]                             = COMPOUND_STRING("There's no PP left for this move!\p${JPN}{JPN}わざの　のこりポイントが　ない！\p"), //not in gen 5+
    [STRINGID_BUTNOPPLEFT]                          = COMPOUND_STRING("But there was no PP left for the move!${JPN}{JPN}しかし　わざの　のこりポイントが　なかった！"),
    [STRINGID_PLAYERUSEDITEM]                       = COMPOUND_STRING("You used {B_LAST_ITEM}!${JPN}{JPN}{B_LAST_ITEM}を　つかった！"),
    [STRINGID_TRAINERBLOCKEDBALL]                   = COMPOUND_STRING("The Trainer blocked your Poké Ball!${JPN}{JPN}トレーナーが　ボールを　はじいた！"),
    [STRINGID_DONTBEATHIEF]                         = COMPOUND_STRING("Don't be a thief!${JPN}{JPN}ひとの　ものを　とったら　どろぼう！"),
    [STRINGID_ITDODGEDBALL]                         = COMPOUND_STRING("It dodged your thrown Poké Ball! This Pokémon can't be caught!${JPN}{JPN}ボールを　よけられた！　こいつは\nつかまりそうにないぞ！"),
    [STRINGID_PKMNBROKEFREE]                        = COMPOUND_STRING("Oh no! The Pokémon broke free!${JPN}{JPN}ダメだ！　ポケモンが　ボールから　でてしまった！"),
    [STRINGID_ITAPPEAREDCAUGHT]                     = COMPOUND_STRING("Aww! It appeared to be caught!${JPN}{JPN}ああ！　つかまえたと　おもったのに！"),
    [STRINGID_AARGHALMOSTHADIT]                     = COMPOUND_STRING("Aargh! Almost had it!${JPN}{JPN}ざんねん！　もうすこしで　つかまえられたのに！"),
    [STRINGID_SHOOTSOCLOSE]                         = COMPOUND_STRING("Gah! It was so close, too!${JPN}{JPN}おしい！　あと　すこしの　ところだったのに！"),
    [STRINGID_GOTCHAPKMNCAUGHTPLAYER]               = COMPOUND_STRING("Gotcha! {B_DEF_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p${JPN}{JPN}やったー！　{B_DEF_NAME}を\nつかまえたぞ！{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p"),
    [STRINGID_GOTCHAPKMNCAUGHTWALLY]                = COMPOUND_STRING("Gotcha! {B_DEF_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}${JPN}{JPN}やったー！　{B_DEF_NAME}を\nつかまえたぞ！{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}"),
    [STRINGID_GIVENICKNAMECAPTURED]                 = COMPOUND_STRING("Would you like to give {B_DEF_NAME} a nickname?${JPN}{JPN}つかまえた　{B_DEF_NAME}に\nニックネームを　つけますか？"),
    [STRINGID_PKMNDATAADDEDTODEX]                   = COMPOUND_STRING("{B_DEF_NAME}'s data has been added to the Pokédex!\p${JPN}{JPN}{B_DEF_NAME}の　データが　あたらしく\nポケモンずかんに　とうろくされます！\p"),
    [STRINGID_ITISRAINING]                          = COMPOUND_STRING("It's raining!${JPN}{JPN}あめが　ふっている！"),
    [STRINGID_SANDSTORMISRAGING]                    = COMPOUND_STRING("The sandstorm is raging!${JPN}{JPN}すなあらしが　ふきあれている！"),
    [STRINGID_CANTESCAPE2]                          = COMPOUND_STRING("You couldn't get away!\p${JPN}{JPN}にげられなかった！\p"),
    [STRINGID_PKMNIGNORESASLEEP]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ignored orders and kept sleeping!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　めいれいを　むしして\nねている！"),
    [STRINGID_PKMNIGNOREDORDERS]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ignored orders!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　めいれいを　むしした！"),
    [STRINGID_PKMNBEGANTONAP]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} began to nap!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ひるねを　はじめた！"),
    [STRINGID_PKMNLOAFING]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is loafing around!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　なまけている"),
    [STRINGID_PKMNWONTOBEY]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} won't obey!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　いうことを　きかない！"),
    [STRINGID_PKMNTURNEDAWAY]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} turned away!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　そっぽを　むいた！"),
    [STRINGID_PKMNPRETENDNOTNOTICE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} pretended not to notice!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しらんぷりを　した！"),
    [STRINGID_ENEMYABOUTTOSWITCHPKMN]               = COMPOUND_STRING("{B_TRAINER1_NAME_WITH_CLASS} is about to send out {B_BUFF2}. Will you switch your Pokémon?${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}は　{B_BUFF2}を\nくりだそうと　している！　ポケモンを\lいれかえますか？"),
    [STRINGID_CREPTCLOSER]                          = COMPOUND_STRING("{B_PLAYER_NAME} crept closer to {B_OPPONENT_MON1_NAME}!${JPN}{JPN}{B_PLAYER_NAME}は　{B_OPPONENT_MON1_NAME}に\nちかづいた！"), //safari
    [STRINGID_CANTGETCLOSER]                        = COMPOUND_STRING("{B_PLAYER_NAME} can't get any closer!${JPN}{JPN}{B_PLAYER_NAME}は　これいじょう\nちかづけない！"), //safari
    [STRINGID_PKMNWATCHINGCAREFULLY]                = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is watching carefully!${JPN}{JPN}{B_OPPONENT_MON1_NAME}は　ようすを\nうかがっている！"), //safari
    [STRINGID_PKMNCURIOUSABOUTX]                    = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is curious about the {B_BUFF1}!${JPN}{JPN}{B_OPPONENT_MON1_NAME}は　{B_BUFF1}に\nきょうみを　しめした！"), //safari
    [STRINGID_PKMNENTHRALLEDBYX]                    = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is enthralled by the {B_BUFF1}!${JPN}{JPN}{B_OPPONENT_MON1_NAME}は　{B_BUFF1}に\nむちゅうだ！"), //safari
    [STRINGID_PKMNIGNOREDX]                         = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} completely ignored the {B_BUFF1}!${JPN}{JPN}{B_OPPONENT_MON1_NAME}は　{B_BUFF1}を\nまったく　きにしていない！"), //safari
    [STRINGID_THREWPOKEBLOCKATPKMN]                 = COMPOUND_STRING("{B_PLAYER_NAME} threw a {POKEBLOCK} at the {B_OPPONENT_MON1_NAME}!${JPN}{JPN}{B_PLAYER_NAME}は　{B_OPPONENT_MON1_NAME}に\nポロックを　なげた！"), //safari
    [STRINGID_OUTOFSAFARIBALLS]                     = COMPOUND_STRING("{PLAY_SE SE_DING_DONG}ANNOUNCER: You're out of Safari Balls! Game over!\p${JPN}{JPN}{PLAY_SE SE_DING_DONG}アナウンス『サファリボールが　なくなりました！\nしゅうりょうです！』\p"), //safari
    [STRINGID_PKMNSITEMCUREDPARALYSIS]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} cured its paralysis!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nまひが　なおった！"),
    [STRINGID_PKMNSITEMCUREDPOISON]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} cured its poison!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nどくが　なおった！"),
    [STRINGID_PKMNSITEMHEALEDBURN]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} cured its burn!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nやけどが　なおった！"),
    [STRINGID_PKMNSITEMDEFROSTEDIT]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} defrosted it!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nこおりが　とけた！"),
    [STRINGID_PKMNSITEMWOKEIT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} woke it up!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nめを　さました！"),
    [STRINGID_PKMNSITEMSNAPPEDOUT]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} snapped it out of its confusion!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nこんらんが　とけた！"),
    [STRINGID_PKMNSITEMCUREDPROBLEM]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} cured its {B_BUFF1} problem!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\n{B_BUFF1}が　なおった！"), // Not in Gen 5+
    [STRINGID_PKMNSITEMRESTOREDHEALTH]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} restored its health using its {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nたいりょくを　かいふくした！"),
    [STRINGID_PKMNSITEMRESTOREDPP]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} restored PP to its move {B_BUFF1} using its {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\n{B_BUFF1}の　PPを　かいふくした！"),
    [STRINGID_PKMNSITEMRESTOREDSTATUS]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} returned its stats to normal using its {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nのうりょくへんかを　もとに　もどした！"),
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE]           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} restored a little HP using its {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nすこし　たいりょくを　かいふくした！"),
    [STRINGID_ITEMALLOWSONLYYMOVE]                  = COMPOUND_STRING("{B_LAST_ITEM} only allows the use of {B_CURRENT_MOVE}!\p${JPN}{JPN}{B_LAST_ITEM}の　こうかで\n{B_CURRENT_MOVE}しか　だせない！\p"),
    [STRINGID_PKMNHUNGONWITHX]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} hung on using its {B_LAST_ITEM}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nもちこたえた！"),
    [STRINGID_EMPTYSTRING3]                         = gText_EmptyString3,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} had its HP restored.${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　たいりょくが\nかいふくした！"),
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} whipped up a sandstorm!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\nすなあらしが　おきた！"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXPREVENTSYLOSS]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} was not lowered!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}は\nさがらない！"),
    [STRINGID_PKMNSXINFATUATEDY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fell in love!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　メロメロに　なった！"),
    [STRINGID_PKMNSXMADEYINEFFECTIVE]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s item cannot be removed!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　どうぐは　うばえない！"),
    [STRINGID_ITSUCKEDLIQUIDOOZE]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} sucked up the liquid ooze!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　ヘドロえきを\nすいとった！"),
    [STRINGID_PKMNTRANSFORMED]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} transformed!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　へんしんした！"),
    [STRINGID_ELECTRICITYWEAKENED]                  = COMPOUND_STRING("Electricity's power was weakened!${JPN}{JPN}でんきの　いりょくが　よわまった！"),
    [STRINGID_FIREWEAKENED]                         = COMPOUND_STRING("Fire's power was weakened!${JPN}{JPN}ほのおの　いりょくが　よわまった！"),
    [STRINGID_PKMNHIDUNDERWATER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} hid underwater!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　すいちゅうに　みを\nひそめた！"),
    [STRINGID_PKMNSPRANGUP]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} sprang up!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　たかく　とびはねた！"),
    [STRINGID_HMMOVESCANTBEFORGOTTEN]               = COMPOUND_STRING("HM moves can't be forgotten now.\p${JPN}{JPN}ひでんわざは　わすれさせることが　できません！\p"),
    [STRINGID_XFOUNDONEY]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} found one {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_LAST_ITEM}を\n1こ　みつけた！"),
    [STRINGID_PLAYERDEFEATEDTRAINER1]               = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA]                        = COMPOUND_STRING("A soothing aroma wafted through the area!${JPN}{JPN}ここちよい　かおりが　ひろがった！"),
    [STRINGID_ITEMSCANTBEUSEDNOW]                   = COMPOUND_STRING("Items can't be used now.{PAUSE 64}${JPN}{JPN}いまは　どうぐを　つかえない！{PAUSE 64}"), // Not present in Gen 5+
    [STRINGID_USINGITEMSTATOFPKMNROSE]              = COMPOUND_STRING("The {B_LAST_ITEM}{B_BUFF2} boosted {B_SCR_NAME_WITH_PREFIX2}'s {B_BUFF1}!${JPN}{JPN}{B_LAST_ITEM}で　{B_SCR_NAME_WITH_PREFIX2}の\n{B_BUFF1}が\l{B_BUFF2}あがった！"),
    [STRINGID_USINGITEMSTATOFPKMNFELL]              = COMPOUND_STRING("The {B_LAST_ITEM}{B_BUFF2} lowered {B_SCR_NAME_WITH_PREFIX2}'s {B_BUFF1}!${JPN}{JPN}{B_LAST_ITEM}で　{B_SCR_NAME_WITH_PREFIX2}の\n{B_BUFF1}が\l{B_BUFF2}さがった！"), // This string does not exist in Gen 5+. Used to print more info that's otherwise obscured such as using Room Service
    [STRINGID_PKMNUSEDXTOGETPUMPED]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} used the {B_LAST_ITEM} to get pumped!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}を\nつかって　はりきっている！"),
    [STRINGID_PKMNSXMADEYUSELESS]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} made {B_CURRENT_MOVE} useless!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\n{B_CURRENT_MOVE}は　こうかが　なくなった！"), //not in gen 5+, ability popup
    [STRINGID_PKMNTRAPPEDBYSANDTOMB]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became trapped by the quicksand!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　すなじごくに\nとらわれた！"),
    [STRINGID_EMPTYSTRING4]                         = COMPOUND_STRING(""),
    [STRINGID_ABOOSTED]                             = COMPOUND_STRING(" a boosted${JPN}{JPN}おおめに　"),
    [STRINGID_PKMNSXINTENSIFIEDSUN]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} intensified the sun's rays!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\nひざしが　つよくなった！"), //not in gen 5+, ability popup
    [STRINGID_YOUTHROWABALLNOWRIGHT]                = COMPOUND_STRING("You throw a Ball now, right? I… I'll do my best!${JPN}{JPN}ボールを　なげるんですよね？　ぼ‥　ぼく\nがんばります！"),
    [STRINGID_PKMNSXTOOKATTACK]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} took the attack!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　こうげきを　うけた！"),
    [STRINGID_PKMNCHOSEXASDESTINY]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} chose Doom Desire as its destiny!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　はめつのねがいを\nみらいに　たくした！"),
    [STRINGID_PKMNLOSTFOCUS]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} lost its focus and couldn't move!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しゅうちゅうが　とぎれて\nわざが　だせない！"),
    [STRINGID_USENEXTPKMN]                          = COMPOUND_STRING("Use next Pokémon?${JPN}{JPN}つぎの　ポケモンを　つかいますか？"),
    [STRINGID_PKMNFLEDUSINGITS]                     = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled using its {B_LAST_ITEM}!\p${JPN}{JPN}{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX}は　{B_LAST_ITEM}を\nつかって　にげだした！\p"),
    [STRINGID_PKMNFLEDUSING]                        = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled using {B_ATK_ABILITY}!\p${JPN}{JPN}{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX}は　{B_ATK_ABILITY}で\nにげだした！\p"), //not in gen 5+
    [STRINGID_PKMNWASDRAGGEDOUT]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was dragged out!\p${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　ひきずりだされた！\p"),
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} normalized its status!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nじょうたいが　もとに　もどった！"), // Not in Gen 5+
    [STRINGID_TRAINER1USEDITEM]                     = COMPOUND_STRING("{B_ATK_TRAINER_NAME_WITH_CLASS} used {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_TRAINER_NAME_WITH_CLASS}は　{B_LAST_ITEM}を\nつかった！"),
    [STRINGID_BOXISFULL]                            = COMPOUND_STRING("The Box is full! You can't catch any more!\p${JPN}{JPN}ボックスが　いっぱいで　これいじょう\nつかまえられない！\p"),
    [STRINGID_PKMNSXMADEITINEFFECTIVE]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} made it ineffective!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　こうかを　うちけした！"),
    [STRINGID_PKMNSXPREVENTSFLINCHING]              = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY} prevents flinching!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}の　{B_EFF_ABILITY}で\nひるまない！"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYHASBURN]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is already burned!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　すでに　やけどを\nおっている！"),
    [STRINGID_PKMNSXBLOCKSY]                        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} blocks {B_CURRENT_MOVE}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\n{B_CURRENT_MOVE}は　ふせがれた！"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXWOREOFF]                        = COMPOUND_STRING("{B_ATK_TEAM1} side's {B_BUFF1} wore off!${JPN}{JPN}{B_ATK_TEAM1}の　{B_BUFF1}の\nこうかが　きれた！"),
    [STRINGID_THEWALLSHATTERED]                     = COMPOUND_STRING("The wall shattered!${JPN}{JPN}かべを　はかいした！"), //not in gen5+, uses "your teams light screen wore off!" etc instead
    [STRINGID_PKMNSXCUREDITSYPROBLEM]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} cured its {B_BUFF1} problem!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\n{B_BUFF1}が　なおった！"), //not in gen 5+, ability popup
    [STRINGID_ATTACKERCANTESCAPE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't escape!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　にげられない！"),
    [STRINGID_PKMNOBTAINEDX]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} obtained {B_BUFF1}.${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}を\nてにいれた！"),
    [STRINGID_PKMNOBTAINEDX2]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} obtained {B_BUFF2}.${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_BUFF2}を\nてにいれた！"),
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} obtained {B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtained {B_BUFF2}.${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}を\nてにいれた！\p{B_DEF_NAME_WITH_PREFIX}は　{B_BUFF2}を\nてにいれた！"),
    [STRINGID_BUTNOEFFECT]                          = COMPOUND_STRING("But it had no effect!${JPN}{JPN}しかし　こうかが　なかった！"),
    [STRINGID_TWOENEMIESDEFEATED]                   = sText_TwoInGameTrainersDefeated,
    [STRINGID_TRAINER2LOSETEXT]                     = COMPOUND_STRING("{B_TRAINER2_LOSE_TEXT}"),
    [STRINGID_PKMNINCAPABLEOFPOWER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} appears incapable of using its power!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ちからを　はっき\nできないようだ！"),
    [STRINGID_GLINTAPPEARSINEYE]                    = COMPOUND_STRING("A glint appears in {B_SCR_NAME_WITH_PREFIX2}'s eyes!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX2}の　めが　ひかった！"),
    [STRINGID_PKMNGETTINGINTOPOSITION]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is getting into position!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　みがまえている！"),
    [STRINGID_PKMNBEGANGROWLINGDEEPLY]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} began growling deeply!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ひくく　うなりはじめた！"),
    [STRINGID_PKMNEAGERFORMORE]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is eager for more!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　もっと\nたたかいたがっている！"),
    [STRINGID_DEFEATEDOPPONENTBYREFEREE]            = COMPOUND_STRING("{B_PLAYER_MON1_NAME} defeated the opponent {B_OPPONENT_MON1_NAME} in a REFEREE's decision!${JPN}{JPN}しんぱんの　はんていで　{B_PLAYER_MON1_NAME}は\nあいての　{B_OPPONENT_MON1_NAME}に　かった！"),
    [STRINGID_LOSTTOOPPONENTBYREFEREE]              = COMPOUND_STRING("{B_PLAYER_MON1_NAME} lost to the opponent {B_OPPONENT_MON1_NAME} in a REFEREE's decision!${JPN}{JPN}しんぱんの　はんていで　{B_PLAYER_MON1_NAME}は\nあいての　{B_OPPONENT_MON1_NAME}に　まけた！"),
    [STRINGID_TIEDOPPONENTBYREFEREE]                = COMPOUND_STRING("{B_PLAYER_MON1_NAME} tied the opponent {B_OPPONENT_MON1_NAME} in a REFEREE's decision!${JPN}{JPN}しんぱんの　はんていで　{B_PLAYER_MON1_NAME}は\nあいての　{B_OPPONENT_MON1_NAME}と　ひきわけた！"),
    [STRINGID_QUESTIONFORFEITMATCH]                 = COMPOUND_STRING("Would you like to forfeit the match and quit now?${JPN}{JPN}しあいを　きけんして　いますぐ　やめますか？"),
    [STRINGID_FORFEITEDMATCH]                       = COMPOUND_STRING("The match was forfeited.${JPN}{JPN}しあいは　きけんと　なりました"),
    [STRINGID_PKMNTRANSFERREDSOMEONESPC]            = gText_PkmnTransferredSomeonesPC,
    [STRINGID_PKMNTRANSFERREDLANETTESPC]            = gText_PkmnTransferredLanettesPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL]                = gText_PkmnTransferredSomeonesPCBoxFull,
    [STRINGID_PKMNBOXLANETTESPCFULL]                = gText_PkmnTransferredLanettesPCBoxFull,
    [STRINGID_TRAINER1WINTEXT]                      = COMPOUND_STRING("{B_TRAINER1_WIN_TEXT}"),
    [STRINGID_TRAINER2WINTEXT]                      = COMPOUND_STRING("{B_TRAINER2_WIN_TEXT}"),
    [STRINGID_ENDUREDSTURDY]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} endured the hit using {B_DEF_ABILITY}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_DEF_ABILITY}で\nこうげきを　こらえた！"),
    [STRINGID_POWERHERB]                            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became fully charged due to its {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nパワーが　みなぎった！"),
    [STRINGID_HURTBYITEM]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt by the {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nダメージを　うけた！"),
    [STRINGID_GRAVITYINTENSIFIED]                   = COMPOUND_STRING("Gravity intensified!${JPN}{JPN}じゅうりょくが　つよくなった！"),
    [STRINGID_TARGETWOKEUP]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} woke up!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　めを　さました！"),
    [STRINGID_TAILWINDBLEW]                         = COMPOUND_STRING("A tailwind started blowing on {B_ATK_TEAM2} side!${JPN}{JPN}{B_ATK_TEAM2}の　うしろから　おいかぜが\nふきはじめた！"),
    [STRINGID_PKMNWENTBACK]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} went back to {B_ATK_TRAINER_NAME}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_ATK_TRAINER_NAME}の\nもとへ　もどっていく！"),
    [STRINGID_PKMNCANTUSEITEMSANYMORE]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} can't use items anymore!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　どうぐが\nつかえなくなった！"),
    [STRINGID_PKMNFLUNG]                            = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} flung its {B_LAST_ITEM}!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　{B_LAST_ITEM}を\nなげつけた！"),
    [STRINGID_PKMNPREVENTEDFROMHEALING]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was prevented from healing!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　かいふくふうじを\nうけた！"),
    [STRINGID_PKMNSWITCHEDATKANDDEF]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} switched its Attack and Defense!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　こうげきと　ぼうぎょを\nいれかえた！"),
    [STRINGID_PKMNSABILITYSUPPRESSED]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s Ability was suppressed!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　とくせいが\nきかなくなった！"),
    [STRINGID_SHIELDEDFROMCRITICALHITS]             = COMPOUND_STRING("Lucky Chant shielded {B_ATK_TEAM2} side from critical hits!${JPN}{JPN}おまじないで　{B_ATK_TEAM2}は\nきゅうしょに　あたらなくなった！"), // Currently not present in Champions
    [STRINGID_PKMNACQUIREDABILITY]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} acquired {B_DEF_ABILITY}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　とくせいが\n{B_DEF_ABILITY}に　なった！"),
    [STRINGID_POISONSPIKESSCATTERED]                = COMPOUND_STRING("Toxic spikes were scattered on the ground all around {B_DEF_TEAM2} side!${JPN}{JPN}{B_DEF_TEAM2}の　あしもとに　どくびしが\nちらばった！"),
    [STRINGID_PKMNSWITCHEDSTATCHANGES]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} switched stat changes with its target!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいてと\nのうりょくへんかを　いれかえた！"),
    [STRINGID_PKMNSURROUNDEDWITHVEILOFWATER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} surrounded itself with a veil of water!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　みずの　リングを\nまとった！"),
    [STRINGID_PKMNLEVITATEDONELECTROMAGNETISM]      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} levitated with electromagnetism!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　でんじりょくで\nうきあがった！"),
    [STRINGID_PKMNTWISTEDDIMENSIONS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} twisted the dimensions!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　じくうを　ゆがめた！"),
    [STRINGID_POINTEDSTONESFLOAT]                   = COMPOUND_STRING("Pointed stones float in the air on {B_DEF_TEAM2} side!${JPN}{JPN}{B_DEF_TEAM2}の　まわりに　とがった\nいわが　ただよいはじめた！"),
    [STRINGID_TRAPPEDBYSWIRLINGMAGMA]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became trapped by swirling magma!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　マグマの　うずに\nとじこめられた！"),
    [STRINGID_VANISHEDINSTANTLY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} vanished instantly!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　すがたが\nいっしゅんにして　きえた！"),
    [STRINGID_PROTECTEDTEAM]                        = COMPOUND_STRING("{B_CURRENT_MOVE} now protects {B_ATK_TEAM2} side!${JPN}{JPN}{B_CURRENT_MOVE}が　{B_ATK_TEAM2}を\nまもる！"),
    [STRINGID_SHAREDITSGUARD]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} shared its guard with the target!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいてと　ぼうぎょを\nわけあった！"),
    [STRINGID_SHAREDITSPOWER]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} shared its power with the target!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいてと　ちからを\nわけあった！"),
    [STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON]         = COMPOUND_STRING("It created a bizarre area in which Defense and Sp. Def stats are swapped!${JPN}{JPN}ぼうぎょと　とくぼうが　いれかわる　ふしぎな\nくうかんを　つくりだした！"),
    [STRINGID_BECAMENIMBLE]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became nimble!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}の　からだが　かるくなった！"),
    [STRINGID_HURLEDINTOTHEAIR]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was hurled into the air!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　うきあがった！"),
    [STRINGID_HELDITEMSLOSEEFFECTS]                 = COMPOUND_STRING("It created a bizarre area in which Pokémon's held items lose their effects!${JPN}{JPN}どうぐの　こうかが　なくなる　ふしぎな\nくうかんを　つくりだした！"),
    [STRINGID_FELLSTRAIGHTDOWN]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} fell straight down!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　うちおとされて　じめんに\nおちた！"),
    [STRINGID_TARGETCHANGEDTYPE]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} transformed into the {B_BUFF1} type!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は\n{B_BUFF1}タイプに　なった！"),
    [STRINGID_KINDOFFER]                            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} took the kind offer!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　おことばに　あまえた！"),
    [STRINGID_RESETSTARGETSSTATLEVELS]              = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s stat changes were removed!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}の　のうりょくへんかが\nもとに　もどった！"),
    [STRINGID_ALLYSWITCHPOSITION]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} and {B_SCR_NAME_WITH_PREFIX2} switched places!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}と　{B_SCR_NAME_WITH_PREFIX2}の\nいちが　いれかわった！"),
    [STRINGID_REFLECTTARGETSTYPE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became the same type as {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}と\nおなじ　タイプに　なった！"),
    [STRINGID_EMBARGOENDS]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can use items again!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　また　どうぐが\nつかえるように　なった！"),
    [STRINGID_ELECTROMAGNETISM]                     = COMPOUND_STRING("electromagnetism${JPN}{JPN}でんじふゆう"),
    [STRINGID_BUFFERENDS]                           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} wore off!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}の\nこうかが　きれた！"),
    [STRINGID_TELEKINESISENDS]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was freed from the telekinesis!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　テレキネシスから\nかいほうされた！"),
    [STRINGID_TAILWINDENDS]                         = COMPOUND_STRING("{B_ATK_TEAM1} side's tailwind petered out!${JPN}{JPN}{B_ATK_TEAM1}の　おいかぜが　やんだ！"),
    [STRINGID_LUCKYCHANTENDS]                       = COMPOUND_STRING("{B_ATK_TEAM1} side's Lucky Chant wore off!${JPN}{JPN}{B_ATK_TEAM1}の　おまじないの　こうかが\nきれた！"),
    [STRINGID_TRICKROOMENDS]                        = COMPOUND_STRING("The twisted dimensions returned to normal!${JPN}{JPN}ゆがんだ　じくうが　もとに　もどった！"),
    [STRINGID_WONDERROOMENDS]                       = COMPOUND_STRING("Wonder Room wore off, and Defense and Sp. Def stats returned to normal!${JPN}{JPN}ワンダールームが　とけて　ぼうぎょと　とくぼうが\nもとに　もどった！"),
    [STRINGID_MAGICROOMENDS]                        = COMPOUND_STRING("Magic Room wore off, and held items' effects returned to normal!${JPN}{JPN}マジックルームが　とけて　どうぐの　こうかが\nもとに　もどった！"),
    [STRINGID_MUDSPORTENDS]                         = COMPOUND_STRING("The effects of Mud Sport have faded.${JPN}{JPN}どろあそびの　こうかが　きれた！"),
    [STRINGID_WATERSPORTENDS]                       = COMPOUND_STRING("The effects of Water Sport have faded.${JPN}{JPN}みずあそびの　こうかが　きれた！"),
    [STRINGID_GRAVITYENDS]                          = COMPOUND_STRING("Gravity returned to normal!${JPN}{JPN}じゅうりょくが　もとに　もどった！"),
    [STRINGID_AQUARINGHEAL]                         = COMPOUND_STRING("A veil of water restored {B_ATK_NAME_WITH_PREFIX2}'s HP!${JPN}{JPN}みずの　リングが　{B_ATK_NAME_WITH_PREFIX2}の\nたいりょくを　かいふくした！"),
    [STRINGID_ELECTRICTERRAINENDS]                  = COMPOUND_STRING("The electricity disappeared from the battlefield.${JPN}{JPN}あしもとの　でんきが　きえさった！"),
    [STRINGID_MISTYTERRAINENDS]                     = COMPOUND_STRING("The mist disappeared from the battlefield.${JPN}{JPN}あしもとの　きりが　きえさった！"),
    [STRINGID_PSYCHICTERRAINENDS]                   = COMPOUND_STRING("The weirdness disappeared from the battlefield!${JPN}{JPN}あしもとの　ふしぎな　かんじが　きえさった！"),
    [STRINGID_GRASSYTERRAINENDS]                    = COMPOUND_STRING("The grass disappeared from the battlefield.${JPN}{JPN}あしもとの　くさが　きえさった！"),
    [STRINGID_TARGETABILITYSTATRAISE]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} {B_BUFF2}raised its {B_BUFF1}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_DEF_ABILITY}で\n{B_BUFF1}が\l{B_BUFF2}あがった！"), // Not in Gen 5+
    [STRINGID_STATWASMAXEDOUT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} maxed its {B_BUFF1}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}が\nさいだいまで　あがった！"),
    [STRINGID_ATTACKERABILITYSTATRAISE]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} {B_BUFF2}raised its {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　{B_ATK_ABILITY}で\n{B_BUFF1}が\l{B_BUFF2}あがった！"), // Not in Gen 5+
    [STRINGID_POISONHEALHPUP]                       = COMPOUND_STRING("The poisoning healed {B_ATK_NAME_WITH_PREFIX2} a little bit!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX2}は　どくで　すこし\nかいふくした！"), // Not in Gen 5+
    [STRINGID_BADDREAMSDMG]                         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is tormented!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　いちゃもんを\nつけられている！"),
    [STRINGID_MOLDBREAKERENTERS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} breaks the mold!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　かたやぶりだ！"),
    [STRINGID_TERAVOLTENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is radiating a bursting aura!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ほとばしる　オーラを\nはなっている！"),
    [STRINGID_TURBOBLAZEENTERS]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is radiating a blazing aura!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　もえたぎる　オーラを\nはなっている！"),
    [STRINGID_SLOWSTARTENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is slow to get going!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ちょうしが　あがらない！"),
    [STRINGID_SLOWSTARTEND]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} finally got its act together!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ちょうしを\nとりもどした！"),
    [STRINGID_SOLARPOWERHPDROP]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} takes its toll!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_ATK_ABILITY}で\nたいりょくを　けずられた！"), // Not in Gen 5+
    [STRINGID_PKMNWASHURT]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ダメージを　うけた！"),
    [STRINGID_ANTICIPATIONACTIVATES]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} shuddered!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　みぶるいした！"),
    [STRINGID_FOREWARNACTIVATES]                    = COMPOUND_STRING("{B_SCR_ABILITY} alerted {B_SCR_NAME_WITH_PREFIX2} to {B_EFF_NAME_WITH_PREFIX2}'s {B_BUFF1}!${JPN}{JPN}{B_SCR_ABILITY}で　{B_SCR_NAME_WITH_PREFIX2}は\n{B_EFF_NAME_WITH_PREFIX2}の　{B_BUFF1}を\lよちした！"),
    [STRINGID_ICEBODYHPGAIN]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} healed it a little bit!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_ATK_ABILITY}で\nすこし　かいふくした！"), // Not in Gen 5+
    [STRINGID_SNOWWARNINGHAIL]                      = COMPOUND_STRING("It started to hail!${JPN}{JPN}あられが　ふりだした！"),
    [STRINGID_FRISKACTIVATES]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} was frisked, revealing its {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_LAST_ITEM}を\nおみとおしだ！"),
    [STRINGID_UNNERVEENTERS]                        = COMPOUND_STRING("{B_EFF_TEAM1} side is too nervous to eat Berries!${JPN}{JPN}{B_EFF_TEAM1}は　きんちょうして　きのみが\nたべられなくなった！"),
    [STRINGID_HARVESTBERRY]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} harvested its {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_LAST_ITEM}を\nしゅうかくした！"),
    [STRINGID_PROTEANTYPECHANGE]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} transformed it into the {B_BUFF1} type!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_ATK_ABILITY}で\n{B_BUFF1}タイプに　なった！"),
    [STRINGID_SYMBIOSISITEMPASS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} shared its {B_LAST_ITEM} with {B_EFF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_EFF_NAME_WITH_PREFIX2}に\n{B_LAST_ITEM}を　わたした！"),
    [STRINGID_STEALTHROCKDMG]                       = COMPOUND_STRING("Pointed stones dug into {B_SCR_NAME_WITH_PREFIX2}!${JPN}{JPN}とがった　いわが　{B_SCR_NAME_WITH_PREFIX2}に\nくいこんだ！"),
    [STRINGID_TOXICSPIKESABSORBED]                  = COMPOUND_STRING("The toxic spikes disappeared from the ground around {B_EFF_TEAM2} side!${JPN}{JPN}{B_EFF_TEAM2}の　あしもとの　どくびしが\nきえさった！"),
    [STRINGID_TOXICSPIKESPOISONED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} was poisoned!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　どくを　あびた！"),
    [STRINGID_TOXICSPIKESBADLYPOISONED]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} was badly poisoned!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　もうどくを　あびた！"),
    [STRINGID_STICKYWEBSWITCHIN]                    = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was caught in a sticky web!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　ねばねばネットに\nひっかかった！"),
    [STRINGID_HEALINGWISHCAMETRUE]                  = COMPOUND_STRING("The healing wish came true for {B_SCR_NAME_WITH_PREFIX2}!${JPN}{JPN}いやしのねがいが　{B_SCR_NAME_WITH_PREFIX2}に\nとどいた！"),
    [STRINGID_HEALINGWISHHEALED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} regained health!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　げんきに　なった！"),
    [STRINGID_LUNARDANCECAMETRUE]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} became cloaked in mystical moonlight!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　しんぴの　つきあかりに\nつつまれた！"),
    [STRINGID_CURSEDBODYDISABLED]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1} was disabled!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　{B_BUFF1}は\nふうじこめられた！"),
    [STRINGID_ATTACKERACQUIREDABILITY]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} acquired {B_ATK_ABILITY}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　とくせいが\n{B_ATK_ABILITY}に　なった！"), // Not in Gen 5+
    [STRINGID_TARGETABILITYSTATLOWER]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} {B_BUFF2}lowered its {B_BUFF1}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_DEF_ABILITY}で\n{B_BUFF1}が\l{B_BUFF2}さがった！"), // Not in Gen 5+
    [STRINGID_TARGETSTATWONTGOHIGHER]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1} won't go any higher!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_BUFF1}は\nもう　あがらない！"),
    [STRINGID_PKMNMOVEBOUNCEDABILITY]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE} was bounced back!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　{B_CURRENT_MOVE}は\nはねかえされた！"),
    [STRINGID_IMPOSTERTRANSFORM]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} transformed into {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}に\nへんしんした！"),
    [STRINGID_ASSAULTVESTDOESNTALLOW]               = COMPOUND_STRING("The effects of the {B_LAST_ITEM} prevent status moves from being used!\p${JPN}{JPN}{B_LAST_ITEM}の　こうかで　へんかわざが\nだせない！\p"),
    [STRINGID_GRAVITYPREVENTSUSAGE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use {B_CURRENT_MOVE} because of gravity!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　じゅうりょくが　つよくて\n{B_CURRENT_MOVE}が　だせない！\p"),
    [STRINGID_HEALBLOCKPREVENTSUSAGE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was prevented from healing!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　かいふくふうじで\nかいふくできない！\p"),
    [STRINGID_NOTDONEYET]                           = COMPOUND_STRING("This move effect is not done yet!\p${JPN}{JPN}この　わざは　まだ　つかえない！\p"),
    [STRINGID_STICKYWEBUSED]                        = COMPOUND_STRING("A sticky web has been laid out on the ground on {B_DEF_TEAM2} side!${JPN}{JPN}{B_DEF_TEAM2}の　あしもとに\nねばねばネットが　ひろがった！"),
    [STRINGID_QUASHSUCCESS]                         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s move was postponed!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　わざは　あとまわしに\nされた！"),
    [STRINGID_PKMNBLEWAWAYTOXICSPIKES]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away Toxic Spikes!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　どくびしを\nふきとばした！"),
    [STRINGID_PKMNBLEWAWAYSTICKYWEB]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away Sticky Web!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ねばねばネットを\nふきとばした！"),
    [STRINGID_PKMNBLEWAWAYSTEALTHROCK]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away Stealth Rock!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ステルスロックを\nふきとばした！"),
    [STRINGID_IONDELUGEON]                          = COMPOUND_STRING("A deluge of ions showers the battlefield!${JPN}{JPN}プラズマの　シャワーが　ふりそそいだ！"),
    [STRINGID_TOPSYTURVYSWITCHEDSTATS]              = COMPOUND_STRING("All stat changes on {B_DEF_NAME_WITH_PREFIX2} were inverted!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}の　のうりょくへんかが\nぎゃくてんした！"),
    [STRINGID_TERRAINBECOMESMISTY]                  = COMPOUND_STRING("Mist swirled around the battlefield!${JPN}{JPN}あしもとに　きりが　たちこめた！"),
    [STRINGID_TERRAINBECOMESGRASSY]                 = COMPOUND_STRING("Grass grew to cover the battlefield!${JPN}{JPN}あしもとに　くさが　おいしげった！"),
    [STRINGID_TERRAINBECOMESELECTRIC]               = COMPOUND_STRING("An electric current ran across the battlefield!${JPN}{JPN}あしもとに　でんきが　かけめぐる！"),
    [STRINGID_TERRAINBECOMESPSYCHIC]                = COMPOUND_STRING("The battlefield got weird!${JPN}{JPN}あしもとが　ふしぎな　かんじに　なった！"),
    [STRINGID_TARGETELECTRIFIED]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s moves have been electrified!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　わざが　でんきタイプに\nなった！"),
    [STRINGID_MEGAEVOREACTING]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_LAST_ITEM} is reacting to {B_ATK_TRAINER_NAME}'s Mega Ring!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　{B_LAST_ITEM}と\n{B_ATK_TRAINER_NAME}の　メガリングが\lはんのうした！"), //actually displays the type of mega ring in inventory, but we didnt implement them :(
    [STRINGID_MEGAEVOEVOLVED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} has Mega Evolved into Mega {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は\nメガ{B_BUFF1}に　メガシンカした！"),
    [STRINGID_DRASTICALLY]                          = gText_drastically,
    [STRINGID_SEVERELY]                             = gText_severely,
    [STRINGID_INFESTATION]                          = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} has been afflicted with an infestation by {B_SCR_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　{B_SCR_NAME_WITH_PREFIX2}に\nまとわりつかれた！"),
    [STRINGID_NOEFFECTONTARGET]                     = COMPOUND_STRING("It won't have any effect on {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}には　こうかが　ない！"),
    [STRINGID_BURSTINGFLAMESHIT]                    = COMPOUND_STRING("The bursting flames hit {B_EFF_NAME_WITH_PREFIX2}!${JPN}{JPN}はじけた　ほのおが　{B_EFF_NAME_WITH_PREFIX2}に\nあたった！"),
    [STRINGID_BESTOWITEMGIVING]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} received {B_LAST_ITEM} from {B_ATK_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_ATK_NAME_WITH_PREFIX2}から\n{B_LAST_ITEM}を　もらった！"),
    [STRINGID_THIRDTYPEADDED]                       = COMPOUND_STRING("{B_BUFF1} type was added to {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}に\n{B_BUFF1}タイプが　ついかされた！"),
    [STRINGID_FELLFORFEINT]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} fell for the feint!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　フェイントを　うけた！"),
    [STRINGID_POKEMONCANNOTUSEMOVE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cannot use {B_CURRENT_MOVE}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_CURRENT_MOVE}を\nだせない！"),
    [STRINGID_COVEREDINPOWDER]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is covered in powder!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　ふんじんを　かぶった！"),
    [STRINGID_POWDEREXPLODES]                       = COMPOUND_STRING("When the flame touched the powder on the Pokémon, it exploded!${JPN}{JPN}ほのおが　ふんじんに　うつって　ばくはつした！"),
    [STRINGID_BELCHCANTSELECT]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} hasn't eaten any held Berries, so it can't possibly belch!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　きのみを\nたべていないので　ゲップが　できない！\p"),
    [STRINGID_SPECTRALTHIEFSTEAL]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} stole the target's boosted stats!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいての\nのうりょくアップを　うばいとった！"),
    [STRINGID_GRAVITYGROUNDING]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fell from the sky due to the gravity!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　じゅうりょくで　そらから\nおちた！"),
    [STRINGID_MISTYTERRAINPREVENTS]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} surrounds itself with a protective mist!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　しろいきりに\nつつまれた！"),
    [STRINGID_GRASSYTERRAINHEALS]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is healed by the grassy terrain!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　グラスフィールドで\nかいふくした！"),
    [STRINGID_ELECTRICTERRAINPREVENTS]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} surrounds itself with electrified terrain!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　エレキフィールドに\nつつまれた！"),
    [STRINGID_PSYCHICTERRAINPREVENTS]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is protected by the Psychic Terrain!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　サイコフィールドに\nまもられている！"),
    [STRINGID_SAFETYGOGGLESPROTECTED]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is not affected thanks to its {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}の\nおかげで　こうかを　うけない！"),
    [STRINGID_FLOWERVEILPROTECTED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} surrounded itself with a veil of petals!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　はなの　ベールに\nつつまれた！"),
    [STRINGID_FLOWERVEILPROTECTEDTARGET]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} surrounded itself with a veil of petals!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　はなの　ベールに\nつつまれた！"),
    [STRINGID_AROMAVEILPROTECTED]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is protected by an aromatic veil!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　アロマベールに\nまもられている！"),
    [STRINGID_CELEBRATEMESSAGE]                     = COMPOUND_STRING("Congratulations, {B_PLAYER_NAME}!${JPN}{JPN}おめでとう　{B_PLAYER_NAME}！"),
    [STRINGID_USEDINSTRUCTEDMOVE]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} followed {B_ATK_NAME_WITH_PREFIX2}'s instructions!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_ATK_NAME_WITH_PREFIX2}の\nさしずを　うけた！"),
    [STRINGID_THROATCHOPENDS]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can use sound-based moves again!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　また　おとの　わざが\nだせるように　なった！"),
    [STRINGID_PKMNCANTUSEMOVETHROATCHOP]            = COMPOUND_STRING("The effects of Throat Chop prevent {B_ATK_NAME_WITH_PREFIX2} from using certain moves!\p${JPN}{JPN}じごくづきの　こうかで　{B_ATK_NAME_WITH_PREFIX2}は\nいくつかの　わざが　だせない！\p"),
    [STRINGID_LASERFOCUS]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} concentrated intensely!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ふかく\nしゅうちゅうしている！"),
    [STRINGID_GEMACTIVATES]                         = COMPOUND_STRING("The {B_LAST_ITEM} strengthened {B_ATK_NAME_WITH_PREFIX2}'s power!${JPN}{JPN}{B_LAST_ITEM}で　{B_ATK_NAME_WITH_PREFIX2}の\nわざの　いりょくが　あがった！"),
    [STRINGID_BERRYDMGREDUCES]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} lessened the damage it took!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nダメージを　へらした！"),
    [STRINGID_AIRBALLOONFLOAT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} floats in the air with its Air Balloon!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ふうせんで\nうかんでいる！"),
    [STRINGID_AIRBALLOONPOP]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s Air Balloon popped!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　ふうせんが　われた！"),
    [STRINGID_INCINERATEBURN]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s {B_LAST_ITEM} was burnt up!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}の　{B_LAST_ITEM}は\nもえてしまった！"),
    [STRINGID_BUGBITE]                              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} stole and ate its target's {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいての\n{B_LAST_ITEM}を　うばって　たべた！"),
    [STRINGID_ILLUSIONWOREOFF]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s illusion wore off!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　イリュージョンが\nとけた！"),
    [STRINGID_ATTACKERCUREDTARGETSTATUS]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cured {B_DEF_NAME_WITH_PREFIX2}'s problem!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}の\nじょうたいいじょうを　なおした！"),
    [STRINGID_ATTACKERLOSTFIRETYPE]                 = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} burned itself out!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　もえつきた！"),
    [STRINGID_HEALERCURE]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was cured of {B_SCR_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX2}の　おかげで\n{B_ATK_NAME_WITH_PREFIX}の　じょうたいいじょうが\lなおった！"),
    [STRINGID_SCRIPTINGABILITYSTATRAISE]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} {B_BUFF2}raised its {B_BUFF1}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\n{B_BUFF1}が\l{B_BUFF2}あがった！"),
    [STRINGID_RECEIVERABILITYTAKEOVER]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} was taken over!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}が\nのっとられた！"),
    [STRINGID_PKNMABSORBINGPOWER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is absorbing power!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ちからを\nためこんでいる！"),
    [STRINGID_NOONEWILLBEABLETORUNAWAY]             = COMPOUND_STRING("No one will be able to leave the battlefield during the next turn!${JPN}{JPN}つぎの　ターン　だれも　にげられなくなった！"),
    [STRINGID_DESTINYKNOTACTIVATES]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fell in love because of the {B_LAST_ITEM}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nメロメロに　なった！"),
    [STRINGID_CLOAKEDINAFREEZINGLIGHT]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became cloaked in a freezing light!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　つめたい　ひかりに\nつつまれた！"),
    [STRINGID_CLEARAMULETWONTLOWERSTATS]            = COMPOUND_STRING("The effects of the {B_LAST_ITEM} held by {B_SCR_NAME_WITH_PREFIX2} prevents its stats from being lowered!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX2}の　もっている\n{B_LAST_ITEM}の　こうかで　のうりょくが\lさがらない！"),
    [STRINGID_FERVENTWISHREACHED]                   = COMPOUND_STRING("{B_ATK_TRAINER_NAME}'s fervent wish has reached {B_ATK_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_TRAINER_NAME}の　ねがいが\n{B_ATK_NAME_WITH_PREFIX2}に　とどいた！"),
    [STRINGID_AIRLOCKACTIVATES]                     = COMPOUND_STRING("The effects of the weather disappeared.${JPN}{JPN}てんきの　えいきょうが　なくなった！"),
    [STRINGID_PRESSUREENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is exerting its pressure!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　プレッシャーを\nはなっている！"),
    [STRINGID_DARKAURAENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is radiating a dark aura!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ダークオーラを\nはなっている！"),
    [STRINGID_FAIRYAURAENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is radiating a fairy aura!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　フェアリーオーラを\nはなっている！"),
    [STRINGID_AURABREAKENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} reversed all other Pokémon's auras!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ぜんいんの　オーラを\nぎゃくてんさせた！"),
    [STRINGID_COMATOSEENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is drowsing!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　うとうと　している！"),
    [STRINGID_SCREENCLEANERENTERS]                  = COMPOUND_STRING("All screens on the field were cleansed!${JPN}{JPN}ばの　かべが　すべて　なくなった！"),
    [STRINGID_FETCHEDPOKEBALL]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} found a {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}を\nみつけた！"),
    [STRINGID_ASANDSTORMKICKEDUP]                   = COMPOUND_STRING("A sandstorm kicked up!${JPN}{JPN}すなあらしが　ふきはじめた！"),
    [STRINGID_PKMNSWILLPERISHIN3TURNS]              = COMPOUND_STRING("Both Pokémon will faint in three turns!${JPN}{JPN}おたがいの　ポケモンは　3ターンごに\nほろびてしまう！"),
    [STRINGID_AURAFLAREDTOLIFE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s aura flared to life!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　オーラが　ほとばしる！"),
    [STRINGID_ASONEENTERS]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} has two Abilities!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　2つの　とくせいを\nもっている！"),
    [STRINGID_CURIOUSMEDICINEENTERS]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s stat changes were removed!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}の　のうりょくへんかが\nもとに　もどった！"),
    [STRINGID_CANACTFASTERTHANKSTO]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can act faster than normal, thanks to its {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}で\nこうどうが　はやくなった！"),
    [STRINGID_MICLEBERRYACTIVATES]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} boosted the accuracy of its next move using {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nつぎの　わざが　あたりやすくなった！"),
    [STRINGID_PKMNSHOOKOFFTHETAUNT]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} shook off the taunt!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　ちょうはつが　とけた！"),
    [STRINGID_PKMNGOTOVERITSINFATUATION]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} got over its infatuation!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　メロメロが　とけた！"),
    [STRINGID_ITEMCANNOTBEREMOVED]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s item cannot be removed!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　どうぐは　うばえない！"),
    [STRINGID_STICKYBARBTRANSFER]                   = COMPOUND_STRING("The {B_LAST_ITEM} attached itself to {B_ATK_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_LAST_ITEM}が　{B_ATK_NAME_WITH_PREFIX2}に\nくっついた！"),
    [STRINGID_PKMNBURNHEALED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s burn was cured!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　やけどが　なおった！"),
    [STRINGID_REDCARDACTIVATE]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} held up its Red Card against {B_ATK_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_ATK_NAME_WITH_PREFIX2}に\nレッドカードを　つきつけた！"),
    [STRINGID_EJECTBUTTONACTIVATE]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is switched out with the {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nもどっていく！"),
    [STRINGID_ATKGOTOVERINFATUATION]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} cured its infatuation status using its {B_LAST_ITEM}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nメロメロが　なおった！"),
    [STRINGID_TORMENTEDNOMORE]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is no longer tormented!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　いちゃもんが　とけた！"),
    [STRINGID_HEALBLOCKEDNOMORE]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} is no longer prevented from healing!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　かいふくふうじが\nとけた！"),
    [STRINGID_ATTACKERBECAMEFULLYCHARGED]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became fully charged due to its bond with its trainer!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　トレーナーとの　きずなで\nちからが　みなぎった！\p"),
    [STRINGID_ATTACKERBECAMEASHSPECIES]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became Ash-Greninja!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　サトシゲッコウガに\nなった！\p"),
    [STRINGID_EXTREMELYHARSHSUNLIGHT]               = COMPOUND_STRING("The sunlight turned extremely harsh!${JPN}{JPN}ひざしが　とても　つよくなった！"),
    [STRINGID_EXTREMESUNLIGHTFADED]                 = COMPOUND_STRING("The extremely harsh sunlight faded!${JPN}{JPN}ひざしが　もとに　もどった！"),
    [STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT]     = COMPOUND_STRING("The Water-type attack evaporated in the extremely harsh sunlight!${JPN}{JPN}つよい　ひざしで　みずタイプの　こうげきが\nじょうはつした！"),
    [STRINGID_EXTREMELYHARSHSUNLIGHTWASNOTLESSENED] = COMPOUND_STRING("The extremely harsh sunlight was not lessened at all!${JPN}{JPN}ひざしが　つよいままだ！"),
    [STRINGID_HEAVYRAIN]                            = COMPOUND_STRING("A heavy rain began to fall!${JPN}{JPN}つよい　あめが　ふりはじめた！"),
    [STRINGID_HEAVYRAINLIFTED]                      = COMPOUND_STRING("The heavy rain has lifted!${JPN}{JPN}つよい　あめが　あがった！"),
    [STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN]         = COMPOUND_STRING("The Fire-type attack fizzled out in the heavy rain!${JPN}{JPN}つよい　あめで　ほのおタイプの　こうげきが\nしょうめつした！"),
    [STRINGID_NORELIEFROMHEAVYRAIN]                 = COMPOUND_STRING("There is no relief from this heavy rain!${JPN}{JPN}つよい　あめは　やむ　けはいが　ない！"),
    [STRINGID_MYSTERIOUSAIRCURRENT]                 = COMPOUND_STRING("Mysterious strong winds are protecting Flying-type Pokémon!${JPN}{JPN}なぞの　らんきりゅうが　ひこうタイプの\nポケモンを　まもっている！"),
    [STRINGID_STRONGWINDSDISSIPATED]                = COMPOUND_STRING("The mysterious strong winds have dissipated!${JPN}{JPN}なぞの　らんきりゅうが　おさまった！"),
    [STRINGID_MYSTERIOUSAIRCURRENTBLOWSON]          = COMPOUND_STRING("The mysterious strong winds blow on regardless!${JPN}{JPN}なぞの　らんきりゅうは　ふきやまない！"),
    [STRINGID_ATTACKWEAKENEDBSTRONGWINDS]           = COMPOUND_STRING("The mysterious strong winds weakened the attack!${JPN}{JPN}なぞの　らんきりゅうが　こうげきを　よわめた！"),
    [STRINGID_STUFFCHEEKSCANTSELECT]                = COMPOUND_STRING("It can't use the move because it doesn't have a Berry!\p${JPN}{JPN}きのみを　もっていないので　わざが　だせない！\p"),
    [STRINGID_PKMNREVERTEDTOPRIMAL]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s Primal Reversion! It reverted to its primal state!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　ゲンシカイキ！　げんしの\nすがたを　とりもどした！"),
    [STRINGID_BUTPOKEMONCANTUSETHEMOVE]             = COMPOUND_STRING("But {B_ATK_NAME_WITH_PREFIX2} can't use the move!${JPN}{JPN}しかし　{B_ATK_NAME_WITH_PREFIX2}には　この　わざが\nつかえない！"),
    [STRINGID_BUTHOOPACANTUSEIT]                    = COMPOUND_STRING("But {B_ATK_NAME_WITH_PREFIX2} can't use it the way it is now!${JPN}{JPN}しかし　いまの　{B_ATK_NAME_WITH_PREFIX2}では　その\nわざは　つかえない！"),
    [STRINGID_BROKETHROUGHPROTECTION]               = COMPOUND_STRING("It broke through {B_EFF_NAME_WITH_PREFIX2}'s protection!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX2}の　まもりを　やぶった！"),
    [STRINGID_ABILITYALLOWSONLYMOVE]                = COMPOUND_STRING("{B_ATK_ABILITY} only allows the use of {B_CURRENT_MOVE}!\p${JPN}{JPN}{B_ATK_ABILITY}の　こうかで\n{B_CURRENT_MOVE}しか　だせない！\p"),
    [STRINGID_SWAPPEDABILITIES]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} swapped Abilities with its target!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　おたがいの　とくせいを\nいれかえた！"),
    [STRINGID_PKMNHEALEDPOISON]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was cured of its poisoning!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}の　どくが　なおった！"),
    [STRINGID_BATTLERTYPECHANGEDTO]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s type changed to {B_BUFF1}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は\n{B_BUFF1}タイプに　なった！"),
    [STRINGID_BOTHCANNOLONGERESCAPE]                = COMPOUND_STRING("Neither Pokémon can run away!${JPN}{JPN}おたがいに　にげられなくなった！"),
    [STRINGID_CANTESCAPEDUETOUSEDMOVE]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can no longer escape because it used No Retreat!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　はいすいのじんで\nにげられなくなった！"),
    [STRINGID_PKMNBECAMEWEAKERTOFIRE]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} became weaker to fire!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　ほのおに　よわくなった！"),
    [STRINGID_ABOUTTOUSEPOLTERGEIST]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is about to be attacked by its {B_LAST_ITEM}!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　{B_LAST_ITEM}に\nおそわれそうだ！"),
    [STRINGID_CANTESCAPEBECAUSEOFCURRENTMOVE]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} can no longer escape because of Octolock!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　たこがためで\nにげられなくなった！"),
    [STRINGID_NEUTRALIZINGGASENTERS]                = COMPOUND_STRING("Neutralizing gas filled the area!${JPN}{JPN}かがくへんかガスが　あたりに　たちこめた！"),
    [STRINGID_NEUTRALIZINGGASOVER]                  = COMPOUND_STRING("The effects of the neutralizing gas wore off!${JPN}{JPN}かがくへんかガスの　こうかが　きえた！"),
    [STRINGID_TARGETTOOHEAVY]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is too heavy to be lifted!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　おもすぎて\nもちあげられない！"),
    [STRINGID_PKMNTOOKTARGETHIGH]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} took {B_DEF_NAME_WITH_PREFIX2} into the sky!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}を\nそらへ　つれさった！"),
    [STRINGID_PKMNINSNAPTRAP]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} got trapped by a snap trap!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　トラバサミに\nとらわれた！"),
    [STRINGID_METEORBEAMCHARGING]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is overflowing with space power!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}に　うちゅうの　ちからが\nあふれだす！"),
    [STRINGID_HEATUPBEAK]                           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} started heating up its beak!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　クチバシを\nかねつしはじめた！"),
    [STRINGID_COURTCHANGE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} swapped the battle effects affecting each side of the field!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　おたがいの　ばの\nこうかを　いれかえた！"),
    [STRINGID_ZPOWERSURROUNDS]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} surrounded itself with its Z-Power!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　Zパワーを　みに\nまとった！"),
    [STRINGID_ZMOVEUNLEASHED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} unleashes its full-force Z-Move!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ぜんりょくの　Zわざを\nはなつ！"),
    [STRINGID_ZMOVERESETSSTATS]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} returned its decreased stats to normal using its Z-Power!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　Zパワーで　さがった\nのうりょくを　もとに　もどした！"),
    [STRINGID_ZMOVEALLSTATSUP]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} boosted its stats using its Z-Power!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　Zパワーで　のうりょくが\nあがった！"),
    [STRINGID_ZMOVEZBOOSTCRIT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} boosted its critical-hit ratio using its Z-Power!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　Zパワーで　きゅうしょに\nあたりやすくなった！"),
    [STRINGID_ZMOVERESTOREHP]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} restored its HP using its Z-Power!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　Zパワーで　たいりょくを\nかいふくした！"),
    [STRINGID_ZMOVESTATUP]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} boosted its stats using its Z-Power!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　Zパワーで　のうりょくが\nあがった！"),
    [STRINGID_ZMOVEHPTRAP]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s HP was restored by the Z-Power!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　Zパワーで　たいりょくが\nかいふくした！"),
    [STRINGID_ATTACKEREXPELLEDTHEPOISON]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} managed to expel the poison so you wouldn't worry!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しんぱい　かけまいと\nどくを　なおした！"),
    [STRINGID_ATTACKERSHOOKITSELFAWAKE]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} shook itself awake so you wouldn't worry!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しんぱい　かけまいと\nめを　さました！"),
    [STRINGID_ATTACKERBROKETHROUGHPARALYSIS]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} gathered all its energy to break through its paralysis so you wouldn't worry!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しんぱい　かけまいと\nちからを　ふりしぼって　まひを　なおした！"),
    [STRINGID_ATTACKERHEALEDITSBURN]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cured its burn through sheer determination so you wouldn't worry!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しんぱい　かけまいと\nきあいで　やけどを　なおした！"),
    [STRINGID_ATTACKERMELTEDTHEICE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} melted the ice with its fiery determination so you wouldn't worry!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しんぱい　かけまいと\nあつい　きあいで　こおりを　とかした！"),
    [STRINGID_TARGETTOUGHEDITOUT]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} toughed it out so you wouldn't feel sad!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　かなしませまいと\nもちこたえた！"),
    [STRINGID_ATTACKERLOSTELECTRICTYPE]             = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} used up all its electricity!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　でんきを\nつかいはたした！"),
    [STRINGID_ATTACKERSWITCHEDSTATWITHTARGET]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} switched {B_BUFF1} with its target!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　あいてと\n{B_BUFF1}を　いれかえた！"),
    [STRINGID_BEINGHITCHARGEDPKMNWITHPOWER]         = COMPOUND_STRING("Being hit by {B_CURRENT_MOVE} charged {B_EFF_NAME_WITH_PREFIX2} with power!${JPN}{JPN}{B_CURRENT_MOVE}を　うけて\n{B_EFF_NAME_WITH_PREFIX2}に　パワーが　みなぎった！"),
    [STRINGID_SUNLIGHTACTIVATEDABILITY]             = COMPOUND_STRING("The harsh sunlight activated {B_SCR_NAME_WITH_PREFIX2}'s Protosynthesis!${JPN}{JPN}ひざしで　{B_SCR_NAME_WITH_PREFIX2}の\nこだいかっせいが　はつどうした！"),
    [STRINGID_ORICHALCUMPULSEACTIVATES]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} turned the sunlight harsh, sending its ancient pulse into a frenzy!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ひざしを　つよくして\nこだいの　こどうが　たかなった！"),
    [STRINGID_ORICHALCUMPULSEACTIVATESINSUN]        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} basked in the sunlight, sending its ancient pulse into a frenzy!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ひざしを　うけて\nこだいの　こどうが　たかなった！"),
    [STRINGID_STATWASHEIGHTENED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_BUFF1} was heightened!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_BUFF1}が\nたかまった！"),
    [STRINGID_ELECTRICTERRAINACTIVATEDABILITY]      = COMPOUND_STRING("The Electric Terrain activated {B_SCR_NAME_WITH_PREFIX2}'s Quark Drive!${JPN}{JPN}エレキフィールドで　{B_SCR_NAME_WITH_PREFIX2}の\nクォークチャージが　はつどうした！"),
    [STRINGID_HADRONENGINEACTIVATES]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} turned the ground into Electric Terrain, energizing its futuristic engine!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　エレキフィールドを\nはって　みらいの　きかんを　かどうさせた！"),
    [STRINGID_HADRONENGINEACTIVATESINTERRAIN]       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} used the Electric Terrain to energize its futuristic engine!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　エレキフィールドで\nみらいの　きかんを　かどうさせた！"),
    [STRINGID_ABILITYWEAKENEDSURROUNDINGMONSSTAT]   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} weakened the {B_BUFF1} of all surrounding Pokémon!\p${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\nまわりの　ポケモンの　{B_BUFF1}が\lよわまった！\p"),
    [STRINGID_ATTACKERGAINEDSTRENGTHFROMTHEFALLEN]  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} gained strength from the fallen!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　たおれた　なかまから\nちからを　うけとった！"),
    [STRINGID_PKMNSABILITYPREVENTSABILITY]          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} prevents {B_DEF_NAME_WITH_PREFIX2}'s {B_DEF_ABILITY} from working!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　{B_SCR_ABILITY}で\n{B_DEF_NAME_WITH_PREFIX2}の　{B_DEF_ABILITY}が\lはたらかない！"), //not in gen 5+, ability popup
    [STRINGID_PREPARESHELLTRAP]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} set a shell trap!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　トラップシェルを\nしかけた！"),
    [STRINGID_SHELLTRAPDIDNTWORK]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s shell trap didn't work!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　トラップシェルは\nふはつに　おわった！"),
    [STRINGID_SPIKESDISAPPEAREDFROMTEAM]            = COMPOUND_STRING("The spikes disappeared from the ground around {B_ATK_TEAM2} side!${JPN}{JPN}{B_ATK_TEAM2}の　まわりの　まきびしが\nきえさった！"),
    [STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("The toxic spikes disappeared from the ground around {B_ATK_TEAM2} side!${JPN}{JPN}{B_ATK_TEAM2}の　まわりの　どくびしが\nきえさった！"),
    [STRINGID_STICKYWEBDISAPPEAREDFROMTEAM]         = COMPOUND_STRING("The sticky web has disappeared from the ground on {B_ATK_TEAM2} side!${JPN}{JPN}{B_ATK_TEAM2}の　まわりの\nねばねばネットが　きえさった！"),
    [STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("The pointed stones disappeared from {B_ATK_TEAM2} side!${JPN}{JPN}{B_ATK_TEAM2}の　まわりの　とがった\nいわが　きえさった！"),
    [STRINGID_COULDNTFULLYPROTECT]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} couldn't fully protect itself and got hurt!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　まもりきれずに\nダメージを　うけた！"),
    [STRINGID_STOCKPILEDEFFECTWOREOFF]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s stockpiled effect wore off!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　たくわえた　こうかが\nきえた！"),
    [STRINGID_PKMNREVIVEDREADYTOFIGHT]              = COMPOUND_STRING("{B_BUFF1} was revived and is ready to fight again!${JPN}{JPN}{B_BUFF1}は　げんきを　とりもどして\nたたかえるように　なった！"),
    [STRINGID_ITEMRESTOREDSPECIESHEALTH]            = COMPOUND_STRING("{B_BUFF1} had its HP restored.${JPN}{JPN}{B_BUFF1}の　たいりょくが\nかいふくした！"),
    [STRINGID_ITEMCUREDSPECIESSTATUS]               = COMPOUND_STRING("{B_BUFF1} had its status healed!${JPN}{JPN}{B_BUFF1}の　じょうたいいじょうが\nなおった！"), // Not in Gen 5+
    [STRINGID_ITEMRESTOREDSPECIESPP]                = COMPOUND_STRING("The PP of {B_BUFF1}'s {B_BUFF2} was restored!${JPN}{JPN}{B_BUFF1}の　{B_BUFF2}の\nPPが　かいふくした！"),
    [STRINGID_THUNDERCAGETRAPPED]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} trapped {B_EFF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_EFF_NAME_WITH_PREFIX2}を\nとじこめた！"),
    [STRINGID_PKMNHURTBYFROSTBITE]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt by its frostbite!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しもやけの　ダメージを\nうけている！"),
    [STRINGID_PKMNGOTFROSTBITE]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} got frostbite!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　しもやけに　なった！"),
    [STRINGID_PKMNSITEMHEALEDFROSTBITE]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_LAST_ITEM} cured its frostbite!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}で\nしもやけが　なおった！"),
    [STRINGID_ATTACKERHEALEDITSFROSTBITE]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cured its frostbite through sheer determination so you wouldn't worry!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しんぱい　かけまいと\nきあいで　しもやけを　なおした！"),
    [STRINGID_PKMNFROSTBITEHEALED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s frostbite was cured!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　しもやけが　なおった！"),
    [STRINGID_PKMNFROSTBITEHEALEDBY]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE} cured its frostbite!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_CURRENT_MOVE}で\nしもやけが　なおった！"),
    [STRINGID_MIRRORHERBCOPIED]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} used its Mirror Herb to mirror its opponent's stat changes!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ものまねハーブで\nあいての　のうりょくへんかを　まねした！"),
    [STRINGID_STARTEDSNOW]                          = COMPOUND_STRING("It started to snow!${JPN}{JPN}ゆきが　ふりはじめた！"),
    [STRINGID_SNOWCONTINUES]                        = COMPOUND_STRING("Snow continues to fall.${JPN}{JPN}ゆきが　ふりつづいている"), //not in gen 5+ (lol)
    [STRINGID_SNOWSTOPPED]                          = COMPOUND_STRING("The snow stopped.${JPN}{JPN}ゆきが　やんだ！"),
    [STRINGID_SNOWWARNINGSNOW]                      = COMPOUND_STRING("It started to snow!${JPN}{JPN}ゆきが　ふりはじめた！"),
    [STRINGID_PKMNITEMMELTED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} corroded {B_DEF_NAME_WITH_PREFIX2}'s {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_DEF_NAME_WITH_PREFIX2}の\n{B_LAST_ITEM}を　とかした！"),
    [STRINGID_ULTRABURSTREACTING]                   = COMPOUND_STRING("Bright light is about to burst out of {B_ATK_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX2}から　まばゆい　ひかりが\nあふれだそうと　している！"),
    [STRINGID_ULTRABURSTCOMPLETED]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} regained its true power through Ultra Burst!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ウルトラバーストで\nほんらいの　ちからを　とりもどした！"),
    [STRINGID_TEAMGAINEDEXP]                        = COMPOUND_STRING("The rest of your team gained Exp. Points thanks to the Exp. Share!\p${JPN}{JPN}てもちの　ポケモンも　がくしゅうそうちで\nけいけんちを　もらった！\p"),
    [STRINGID_CURRENTMOVECANTSELECT]                = COMPOUND_STRING("{B_BUFF1} cannot be used!\p${JPN}{JPN}{B_BUFF1}は　つかえない！\p"),
    [STRINGID_TARGETISBEINGSALTCURED]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is being salt cured!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　しおづけに　なった！"),
    [STRINGID_TARGETISHURTBYSALTCURE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by {B_BUFF1}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_BUFF1}の\nダメージを　うけている！"),
    [STRINGID_TARGETCOVEREDINSTICKYCANDYSYRUP]      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} got covered in sticky candy syrup!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　ねばねばの　アメまみれに\nなった！"),
    [STRINGID_SHARPSTEELFLOATS]                     = COMPOUND_STRING("Sharp-pointed pieces of steel started floating around {B_DEF_TEAM2} Pokémon!${JPN}{JPN}{B_DEF_TEAM2}の　まわりに　とがった\nはがねが　ただよいはじめた！"),
    [STRINGID_SHARPSTEELDMG]                        = COMPOUND_STRING("The sharp steel bit into {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}とがった　はがねが　{B_DEF_NAME_WITH_PREFIX2}に\nくいこんだ！"),
    [STRINGID_PKMNBLEWAWAYSHARPSTEEL]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away sharp steel!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　とがった　はがねを\nふきとばした！"),
    [STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM]        = COMPOUND_STRING("The pieces of steel surrounding {B_ATK_TEAM2} Pokémon disappeared!${JPN}{JPN}{B_ATK_TEAM2}の　まわりの　はがねが\nきえさった！"),
    [STRINGID_TEAMTRAPPEDWITHVINES]                 = COMPOUND_STRING("{B_EFF_TEAM1} Pokémon got trapped with vines!${JPN}{JPN}{B_EFF_TEAM1}の　ポケモンは　つるに\nとらわれた！"),
    [STRINGID_PKMNHURTBYVINES]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by G-Max Vine Lash's ferocious beating!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　キョダイベンタツの\nはげしい　ムチうちを　うけている！"),
    [STRINGID_TEAMCAUGHTINVORTEX]                   = COMPOUND_STRING("{B_EFF_TEAM1} Pokémon got caught in a vortex of water!${JPN}{JPN}{B_EFF_TEAM1}の　ポケモンは　みずの\nうずに　まきこまれた！"),
    [STRINGID_PKMNHURTBYVORTEX]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by G-Max Cannonade's vortex!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　キョダイホウゲキの\nうずで　ダメージを　うけている！"),
    [STRINGID_TEAMSURROUNDEDBYFIRE]                 = COMPOUND_STRING("{B_EFF_TEAM1} Pokémon were surrounded by fire!${JPN}{JPN}{B_EFF_TEAM1}の　ポケモンは　ほのおに\nかこまれた！"),
    [STRINGID_PKMNBURNINGUP]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is burning up within G-Max Wildfire's flames!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　キョダイゴクエンの\nほのおで　やかれている！"),
    [STRINGID_TEAMSURROUNDEDBYROCKS]                = COMPOUND_STRING("{B_EFF_TEAM1} Pokémon became surrounded by rocks!${JPN}{JPN}{B_EFF_TEAM1}の　ポケモンは　いわに\nかこまれた！"),
    [STRINGID_PKMNHURTBYROCKSTHROWN]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by rocks thrown out by G-Max Volcalith!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　キョダイフンセキの\nいわで　ダメージを　うけている！"),
    [STRINGID_MOVEBLOCKEDBYDYNAMAX]                 = COMPOUND_STRING("The move was blocked by the power of Dynamax!${JPN}{JPN}ダイマックスの　ちからで　わざが　ふせがれた！"),
    [STRINGID_ZEROTOHEROTRANSFORMATION]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} underwent a heroic transformation!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　ヒーローへの　へんしんを\nとげた！"),
    [STRINGID_THETWOMOVESBECOMEONE]                 = COMPOUND_STRING("The two moves have become one! It's a combined move!{PAUSE 16}${JPN}{JPN}2つの　わざが　ひとつに！　これは\nがったいわざだ！{PAUSE 16}"),
    [STRINGID_ARAINBOWAPPEAREDONSIDE]               = COMPOUND_STRING("A rainbow appeared in the sky on {B_EFF_TEAM2} side!${JPN}{JPN}{B_EFF_TEAM2}の　そらに　にじが\nかかった！"),
    [STRINGID_THERAINBOWDISAPPEARED]                = COMPOUND_STRING("The rainbow on {B_ATK_TEAM2} side disappeared!${JPN}{JPN}{B_ATK_TEAM2}の　にじが　きえた！"),
    [STRINGID_WAITINGFORPARTNERSMOVE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is waiting for {B_ATK_PARTNER_NAME}'s move…{PAUSE 16}${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　{B_ATK_PARTNER_NAME}の\nわざを　まっている‥{PAUSE 16}"),
    [STRINGID_SEAOFFIREENVELOPEDSIDE]               = COMPOUND_STRING("A sea of fire enveloped {B_EFF_TEAM2} side!${JPN}{JPN}{B_EFF_TEAM2}は　ひのうみに　つつまれた！"),
    [STRINGID_HURTBYTHESEAOFFIRE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt by the sea of fire!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ひのうみの　ダメージを\nうけた！"),
    [STRINGID_THESEAOFFIREDISAPPEARED]              = COMPOUND_STRING("The sea of fire around {B_ATK_TEAM2} side disappeared!${JPN}{JPN}{B_ATK_TEAM2}の　まわりの　ひのうみが\nきえた！"),
    [STRINGID_SWAMPENVELOPEDSIDE]                   = COMPOUND_STRING("A swamp enveloped {B_EFF_TEAM2} side!${JPN}{JPN}{B_EFF_TEAM2}は　しつげんに　つつまれた！"),
    [STRINGID_THESWAMPDISAPPEARED]                  = COMPOUND_STRING("The swamp around {B_ATK_TEAM2} side disappeared!${JPN}{JPN}{B_ATK_TEAM2}の　まわりの　しつげんが\nきえた！"),
    [STRINGID_PKMNTELLCHILLINGRECEPTIONJOKE]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is preparing to tell a chillingly bad joke!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　さむい　ギャグを\nいおうとしている！"),
    [STRINGID_HOSPITALITYRESTORATION]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} drank down all the matcha that {B_SCR_NAME_WITH_PREFIX2} made!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は　{B_SCR_NAME_WITH_PREFIX2}が\nたてた　おちゃを　のみほした！"),
    [STRINGID_ELECTROSHOTCHARGING]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbed electricity!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　でんきを\nきゅうしゅうした！"),
    [STRINGID_ITEMWASUSEDUP]                        = COMPOUND_STRING("The {B_LAST_ITEM} was used up…${JPN}{JPN}{B_LAST_ITEM}を　つかいきった‥"),
    [STRINGID_ATTACKERLOSTITSTYPE]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} lost its {B_BUFF1} type!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX}は\n{B_BUFF1}タイプを　うしなった！"),
    [STRINGID_SHEDITSTAIL]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} shed its tail to create a decoy!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　しっぽを　きって\nみがわりに　した！"),
    [STRINGID_CLOAKEDINAHARSHLIGHT]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became cloaked in a harsh light!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　はげしい　ひかりに\nつつまれた！"),
    [STRINGID_SUPERSWEETAROMAWAFTS]                 = COMPOUND_STRING("A supersweet aroma is wafting from the syrup covering {B_EFF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_EFF_NAME_WITH_PREFIX2}を　おおう　シロップから\nあまい　かおりが　ただよう！"),
    [STRINGID_DIMENSIONSWERETWISTED]                = COMPOUND_STRING("The dimensions were twisted!${JPN}{JPN}じくうが　ゆがんだ！"),
    [STRINGID_BIZARREARENACREATED]                  = COMPOUND_STRING("A bizarre area was created in which Pokémon's held items lose their effects!${JPN}{JPN}どうぐの　こうかが　なくなる　ふしぎな\nくうかんが　できた！"),
    [STRINGID_BIZARREAREACREATED]                   = COMPOUND_STRING("A bizarre area was created in which Defense and Sp. Def stats are swapped!${JPN}{JPN}ぼうぎょと　とくぼうが　いれかわる　ふしぎな\nくうかんが　できた！"),
    [STRINGID_TIDYINGUPCOMPLETE]                    = COMPOUND_STRING("Tidying up complete!${JPN}{JPN}おかたづけ　かんりょう！"),
    [STRINGID_PKMNTERASTALLIZEDINTO]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} terastallized into the {B_BUFF1} type!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}タイプに　テラスタルした！"), // Does not exist, meant to mimic form change strings
    [STRINGID_BOOSTERENERGYACTIVATES]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} used its {B_LAST_ITEM} to activate {B_SCR_ABILITY}!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_LAST_ITEM}を\nつかって　{B_SCR_ABILITY}を　はつどうした！"),
    [STRINGID_FOGCREPTUP]                           = COMPOUND_STRING("Fog crept up as thick as soup!${JPN}{JPN}こい　きりが　たちこめた！"),
    [STRINGID_FOGISDEEP]                            = COMPOUND_STRING("The fog is deep…${JPN}{JPN}きりが　ふかい‥"),
    [STRINGID_FOGLIFTED]                            = COMPOUND_STRING("The fog lifted.${JPN}{JPN}きりが　はれた！"),
    [STRINGID_PKMNMADESHELLGLEAM]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} made its shell gleam! It's distorting type matchups!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX}は　こうらを　かがやかせて\nタイプあいしょうを　くるわせた！"),
    [STRINGID_FICKLEBEAMDOUBLED]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is going all out for this attack!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　ぜんりょくで　こうげきを\nしかける！"),
    [STRINGID_COMMANDERACTIVATES]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} was swallowed by {B_BUFF1} and became {B_BUFF1}'s commander!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　{B_BUFF1}に\nのみこまれて　{B_BUFF1}の\lしれいとうに　なった！"),
    [STRINGID_POKEFLUTECATCHY]                      = COMPOUND_STRING("{B_PLAYER_NAME} played the {B_LAST_ITEM}.\pNow, that's a catchy tune!${JPN}{JPN}{B_PLAYER_NAME}は　{B_LAST_ITEM}を\nふいた！\pなんとも　ここちよい　ねいろだ！"),
    [STRINGID_POKEFLUTE]                            = COMPOUND_STRING("{B_PLAYER_NAME} played the {B_LAST_ITEM}.${JPN}{JPN}{B_PLAYER_NAME}は　{B_LAST_ITEM}を\nふいた！"),
    [STRINGID_MONHEARINGFLUTEAWOKE]                 = COMPOUND_STRING("The Pokémon hearing the flute awoke!${JPN}{JPN}ふえの　ねいろを　きいた　ポケモンは　めを\nさました！"),
    [STRINGID_SUNLIGHTISHARSH]                      = COMPOUND_STRING("The sunlight is harsh!${JPN}{JPN}ひざしが　つよい！"),
    [STRINGID_ITISHAILING]                          = COMPOUND_STRING("It's hailing!${JPN}{JPN}あられが　ふっている！"),
    [STRINGID_ITISSNOWING]                          = COMPOUND_STRING("It's snowing!${JPN}{JPN}ゆきが　ふっている！"),
    [STRINGID_ISCOVEREDWITHGRASS]                   = COMPOUND_STRING("The battlefield is covered with grass!${JPN}{JPN}あしもとに　くさが　おいしげっている！"),
    [STRINGID_MISTSWIRLSAROUND]                     = COMPOUND_STRING("Mist swirls around the battlefield!${JPN}{JPN}あしもとに　きりが　たちこめている！"),
    [STRINGID_ELECTRICCURRENTISRUNNING]             = COMPOUND_STRING("An electric current is running across the battlefield!${JPN}{JPN}あしもとに　でんきが　かけめぐっている！"),
    [STRINGID_SEEMSWEIRD]                           = COMPOUND_STRING("The battlefield seems weird!${JPN}{JPN}あしもとが　ふしぎな　かんじだ！"),
    [STRINGID_WAGGLINGAFINGER]                      = COMPOUND_STRING("Waggling a finger let it use {B_CURRENT_MOVE}!${JPN}{JPN}ゆびを　ふったら　{B_CURRENT_MOVE}が　でた！"),
    [STRINGID_BLOCKEDBYSLEEPCLAUSE]                 = COMPOUND_STRING("Sleep Clause kept {B_DEF_NAME_WITH_PREFIX2} awake!${JPN}{JPN}ねむり　きんしルールで　{B_DEF_NAME_WITH_PREFIX2}は\nねむらなかった！"),
    [STRINGID_SUPEREFFECTIVETWOFOES]                = COMPOUND_STRING("It's super effective on {B_DEF_NAME_WITH_PREFIX2} and {B_DEF_PARTNER_NAME}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}と　{B_DEF_PARTNER_NAME}には\nこうかは　ばつぐんだ！"),
    [STRINGID_NOTVERYEFFECTIVETWOFOES]              = COMPOUND_STRING("It's not very effective on {B_DEF_NAME_WITH_PREFIX2} and {B_DEF_PARTNER_NAME}.${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}と　{B_DEF_PARTNER_NAME}には\nこうかは　いまひとつの　ようだ‥"),
    [STRINGID_ITDOESNTAFFECTTWOFOES]                = COMPOUND_STRING("It doesn't affect {B_DEF_NAME_WITH_PREFIX2} and {B_DEF_PARTNER_NAME}…${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}と　{B_DEF_PARTNER_NAME}には\nこうかが　ないみたいだ‥"),
    [STRINGID_SENDCAUGHTMONPARTYORBOX]              = COMPOUND_STRING("Add {B_DEF_NAME} to your party?${JPN}{JPN}{B_DEF_NAME}を　てもちに　くわえますか？"),
    [STRINGID_PKMNSENTTOPCAFTERCATCH]               = gText_PkmnSentToPCAfterCatch,
    [STRINGID_PKMNDYNAMAXED]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} grew huge into its Dynamax form!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　きょだいな\nダイマックスの　すがたに　なった！"),
    [STRINGID_PKMNGIGANTAMAXED]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} grew huge into its Gigantamax form!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　きょだいな\nキョダイマックスの　すがたに　なった！"),
    [STRINGID_TIMETODYNAMAX]                        = COMPOUND_STRING("Time to Dynamax!${JPN}{JPN}ダイマックスの　じかんだ！"),
    [STRINGID_TIMETOGIGANTAMAX]                     = COMPOUND_STRING("Time to Gigantamax!${JPN}{JPN}キョダイマックスの　じかんだ！"),
    [STRINGID_QUESTIONFORFEITBATTLE]                = COMPOUND_STRING("Would you like to give up on this battle and quit now? Quitting the battle is the same as losing the battle.${JPN}{JPN}しょうぶを　あきらめて　やめますか？　やめると\nまけたことに　なります"),
    [STRINGID_POWERCONSTRUCTPRESENCEOFMANY]         = COMPOUND_STRING("You sense the presence of many!${JPN}{JPN}たくさんの　けはいを　かんじる！"),
    [STRINGID_POWERCONSTRUCTTRANSFORM]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} transformed into its Complete Forme!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　パーフェクトフォルムに\nへんかした！"),
    [STRINGID_ABILITYSHIELDPROTECTS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s Ability is protected by the effects of its {B_LAST_ITEM}!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}の　とくせいは\n{B_LAST_ITEM}の　こうかで\lまもられている！"),
    [STRINGID_MONTOOSCAREDTOMOVE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is too scared to move!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　こわくて　うごけない！"),
    [STRINGID_GHOSTGETOUTGETOUT]                    = COMPOUND_STRING("GHOST: Get out…… Get out……${JPN}{JPN}ゆうれい『たちされ‥‥　たちされ‥‥』"),
    [STRINGID_SILPHSCOPEUNVEILED]                   = COMPOUND_STRING("SILPH SCOPE unveiled the GHOST's\nidentity!${JPN}{JPN}シルフスコープで　ゆうれいの\nしょうたいを　みやぶった！"),
    [STRINGID_GHOSTWASMAROWAK]                      = COMPOUND_STRING("The GHOST was MAROWAK!\p${JPN}{JPN}ゆうれいの　しょうたいは　ガラガラ　だった！\p"),
    [STRINGID_TRAINER1MON1COMEBACK]                 = COMPOUND_STRING("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME}, come back!${JPN}{JPN}{B_TRAINER1_NAME}『{B_OPPONENT_MON1_NAME}\nもどれ！』"),
    [STRINGID_THREWROCK]                            = COMPOUND_STRING("{B_PLAYER_NAME} threw a ROCK\nat the {B_OPPONENT_MON1_NAME}!${JPN}{JPN}{B_PLAYER_NAME}は　{B_OPPONENT_MON1_NAME}に\nいしを　なげた！"),
    [STRINGID_THREWBAIT]                            = COMPOUND_STRING("{B_PLAYER_NAME} threw some BAIT\nat the {B_OPPONENT_MON1_NAME}!${JPN}{JPN}{B_PLAYER_NAME}は　{B_OPPONENT_MON1_NAME}に\nエサを　なげた！"),
    [STRINGID_PKMNANGRY]                            = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is angry!${JPN}{JPN}{B_OPPONENT_MON1_NAME}は　おこっている！"),
    [STRINGID_PKMNEATING]                           = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is eating!${JPN}{JPN}{B_OPPONENT_MON1_NAME}は　エサを　たべている！"),
    [STRINGID_PKMNDISGUISEWASBUSTED]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s disguise was busted!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　ばけのかわが　はがれた！"),
    [STRINGID_ZENMODETRIGGERED]                     = COMPOUND_STRING("{B_SCR_ABILITY} triggered!${JPN}{JPN}{B_SCR_ABILITY}が　はつどうした！"),
    [STRINGID_ZENMODEENDED]                         = COMPOUND_STRING("{B_SCR_ABILITY} ended!${JPN}{JPN}{B_SCR_ABILITY}の　こうかが　きれた！"),
    [STRINGID_SCRCUREDPARALYSIS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} was cured of paralysis!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　まひが　なおった！"),
    [STRINGID_SCRCUREDPOISON]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} was cured of its poisoning!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　どくが　なおった！"),
    [STRINGID_SCRCUREDBURN]                         = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s burn was cured!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　やけどが　なおった！"),
    [STRINGID_SCRCUREDSLEEP]                        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} woke up!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}は　めを　さました！"),
    [STRINGID_SCRCUREDCONFUSION]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} snapped out of its confusion!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　こんらんが　とけた！"),
    [STRINGID_PARTYCUREDPARALYSIS]                  = COMPOUND_STRING("{B_BUFF1} was cured of paralysis!${JPN}{JPN}{B_BUFF1}の　まひが　なおった！"),
    [STRINGID_PARTYCUREDPOISON]                     = COMPOUND_STRING("{B_BUFF1} was cured of its poisoning!${JPN}{JPN}{B_BUFF1}の　どくが　なおった！"),
    [STRINGID_PARTYCUREDBURN]                       = COMPOUND_STRING("{B_BUFF1}'s burn was cured!${JPN}{JPN}{B_BUFF1}の　やけどが　なおった！"),
    [STRINGID_PARTYCUREDSLEEP]                      = COMPOUND_STRING("{B_BUFF1} woke up!${JPN}{JPN}{B_BUFF1}は　めを　さました！"),
    [STRINGID_PARTYCUREDFREEZE]                     = COMPOUND_STRING("{B_BUFF1} thawed out!${JPN}{JPN}{B_BUFF1}の　こおりが　とけた！"),
    [STRINGID_PARTYCUREDFROSTBITE]                  = COMPOUND_STRING("{B_BUFF1}'s frostbite was cured!${JPN}{JPN}{B_BUFF1}の　しもやけが　なおった！"),
    [STRINGID_PKMNATKNOTLOWERED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s Attack was not lowered!${JPN}{JPN}{B_SCR_NAME_WITH_PREFIX}の　こうげきは　さがらない！"),
    [STRINGID_VICTORYCATCH]                         = COMPOUND_STRING("{B_DEF_NAME} is weak!\nThrow a Poké Ball now!${JPN}{JPN}{B_DEF_NAME}は　よわっている！\nいまこそ　ボールを　なげるんだ！"),
    [STRINGID_CANTUSEMOVE]                          = COMPOUND_STRING("This move can't be used!\p${JPN}{JPN}この　わざは　つかえない！\p"),
    [STRINGID_REFLECTWOREOFF]                       = COMPOUND_STRING("{B_DEF_TEAM1} side's Reflect wore off!${JPN}{JPN}{B_DEF_TEAM1}の　リフレクターの　こうかが\nきれた！"),
    [STRINGID_LIGHTSCREENWOREOFF]                   = COMPOUND_STRING("{B_DEF_TEAM1} side's Light Screen wore off!${JPN}{JPN}{B_DEF_TEAM1}の　ひかりのかべの　こうかが\nきれた！"),
    [STRINGID_AURORAVEILWOREOFF]                    = COMPOUND_STRING("{B_DEF_TEAM1} side's Aurora Veil wore off!${JPN}{JPN}{B_DEF_TEAM1}の　オーロラベールの\nこうかが　きれた！"),
    [STRINGID_MOSTLYINEFFECTIVE]                    = COMPOUND_STRING("It's mostly ineffective…${JPN}{JPN}こうかは　かなり　いまひとつの　ようだ‥"),
    [STRINGID_EXTREMELYEFFECTIVE]                   = COMPOUND_STRING("It's extremely effective!${JPN}{JPN}こうかは　ものすごく　ばつぐんだ！"),
    [STRINGID_NOTVERYEFFECTIVEONDEF]                = COMPOUND_STRING("It's not very effective on {B_DEF_NAME_WITH_PREFIX2}.${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}には　こうかは　いまひとつの\nようだ‥"),
    [STRINGID_SUPEREFFECTIVEONDEF]                  = COMPOUND_STRING("It's super effective on {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}には　こうかは　ばつぐんだ！"),
    [STRINGID_MOSTLYINEFFECTIVEONDEF]               = COMPOUND_STRING("It's mostly ineffective on {B_DEF_NAME_WITH_PREFIX2}.${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}には　こうかは　かなり\nいまひとつの　ようだ‥"),
    [STRINGID_EXTREMELYEFFECTIVEONDEF]              = COMPOUND_STRING("It's extremely effective on {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}には　こうかは　ものすごく\nばつぐんだ！"),
    [STRINGID_EXTREMELYEFFECTIVETWOFOES]            = COMPOUND_STRING("It's extremely effective on {B_DEF_NAME_WITH_PREFIX2} and {B_DEF_PARTNER_NAME}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}と　{B_DEF_PARTNER_NAME}には\nこうかは　ものすごく　ばつぐんだ！"),
    [STRINGID_MOSTLYINEFFECTIVETWOFOES]             = COMPOUND_STRING("It's mostly ineffective on {B_DEF_NAME_WITH_PREFIX2} and {B_DEF_PARTNER_NAME}.${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}と　{B_DEF_PARTNER_NAME}には\nこうかは　かなり　いまひとつの　ようだ‥"),
    [STRINGID_CRITICALHITONDEF]                     = COMPOUND_STRING("A critical hit on {B_DEF_NAME_WITH_PREFIX2}!${JPN}{JPN}{B_DEF_NAME_WITH_PREFIX2}の　きゅうしょに　あたった！"),
    [STRINGID_S]                                    = COMPOUND_STRING("s${JPN}{JPN}{PAUSE 1}"),
    [STRINGID_LOSTSOMEOFITSHP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} lost some of its HP!${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　たいりょくを　すこし\nうしなった！"),
    [STRINGID_BELCHCANTUSE]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} hasn't eaten any held Berries, so it can't possibly belch!\p${JPN}{JPN}{B_ATK_NAME_WITH_PREFIX}は　きのみを\nたべていないので　ゲップが　できない！\p"),
};

const u16 gTrainerUsedItemStringIds[] =
{
    STRINGID_PLAYERUSEDITEM, STRINGID_TRAINER1USEDITEM
};

const u16 gZEffectStringIds[] =
{
    [B_MSG_Z_RESET_STATS] = STRINGID_ZMOVERESETSSTATS,
    [B_MSG_Z_ALL_STATS_UP]= STRINGID_ZMOVEALLSTATSUP,
    [B_MSG_Z_BOOST_CRITS] = STRINGID_ZMOVEZBOOSTCRIT,
    [B_MSG_Z_FOLLOW_ME]   = STRINGID_PKMNCENTERATTENTION,
    [B_MSG_Z_RECOVER_HP]  = STRINGID_ZMOVERESTOREHP,
    [B_MSG_Z_STAT_UP]     = STRINGID_ZMOVESTATUP,
    [B_MSG_Z_HP_TRAP]     = STRINGID_ZMOVEHPTRAP,
};

const u16 gMentalHerbCureStringIds[] =
{
    [B_MSG_MENTALHERBCURE_INFATUATION] = STRINGID_ATKGOTOVERINFATUATION,
    [B_MSG_MENTALHERBCURE_TORMENT]     = STRINGID_TORMENTEDNOMORE,
    [B_MSG_MENTALHERBCURE_DISABLE]     = STRINGID_PKMNMOVEDISABLEDNOMORE,
    [B_MSG_MENTALHERBCURE_HEALBLOCK]   = STRINGID_HEALBLOCKEDNOMORE,
    [B_MSG_MENTALHERBCURE_ENCORE]      = STRINGID_PKMNENCOREENDED,
    [B_MSG_MENTALHERBCURE_TAUNT]       = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gStartingStatusStringIds[B_MSG_STARTING_STATUS_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_SET_TRICK_ROOM]       = STRINGID_DIMENSIONSWERETWISTED,
    [B_MSG_SET_MAGIC_ROOM]       = STRINGID_BIZARREARENACREATED,
    [B_MSG_SET_WONDER_ROOM]      = STRINGID_BIZARREAREACREATED,
    [B_MSG_SET_TAILWIND]         = STRINGID_TAILWINDBLEW,
    [B_MSG_SET_RAINBOW]          = STRINGID_ARAINBOWAPPEAREDONSIDE,
    [B_MSG_SET_SEA_OF_FIRE]      = STRINGID_SEAOFFIREENVELOPEDSIDE,
    [B_MSG_SET_SWAMP]            = STRINGID_SWAMPENVELOPEDSIDE,
    [B_MSG_SET_SPIKES]           = STRINGID_SPIKESSCATTERED,
    [B_MSG_SET_POISON_SPIKES]    = STRINGID_POISONSPIKESSCATTERED,
    [B_MSG_SET_STICKY_WEB]       = STRINGID_STICKYWEBUSED,
    [B_MSG_SET_STEALTH_ROCK]     = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SET_SHARP_STEEL]      = STRINGID_SHARPSTEELFLOATS,
};

const u16 gTerrainStringIds[B_MSG_TERRAIN_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY] = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC] = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY] = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_TERRAIN_END_MISTY] = STRINGID_MISTYTERRAINENDS,
    [B_MSG_TERRAIN_END_ELECTRIC] = STRINGID_ELECTRICTERRAINENDS,
    [B_MSG_TERRAIN_END_PSYCHIC] = STRINGID_PSYCHICTERRAINENDS,
    [B_MSG_TERRAIN_END_GRASSY] = STRINGID_GRASSYTERRAINENDS,
};

const u16 gTerrainPreventsStringIds[] =
{
    [B_MSG_TERRAINPREVENTS_MISTY]    = STRINGID_MISTYTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_ELECTRIC] = STRINGID_ELECTRICTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_PSYCHIC]  = STRINGID_PSYCHICTERRAINPREVENTS
};

const u16 gHealingWishStringIds[] =
{
    STRINGID_HEALINGWISHCAMETRUE,
    STRINGID_LUNARDANCECAMETRUE
};

const u16 gDmgHazardsStringIds[] =
{
    [B_MSG_PKMNHURTBYSPIKES]   = STRINGID_PKMNHURTBYSPIKES,
    [B_MSG_STEALTHROCKDMG]     = STRINGID_STEALTHROCKDMG,
    [B_MSG_SHARPSTEELDMG]      = STRINGID_SHARPSTEELDMG,
    [B_MSG_POINTEDSTONESFLOAT] = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SPIKESSCATTERED]    = STRINGID_SPIKESSCATTERED,
    [B_MSG_SHARPSTEELFLOATS]   = STRINGID_SHARPSTEELFLOATS,
};

const u16 gSwitchInAbilityStringIds[] =
{
    [B_MSG_SWITCHIN_MOLDBREAKER] = STRINGID_MOLDBREAKERENTERS,
    [B_MSG_SWITCHIN_TERAVOLT] = STRINGID_TERAVOLTENTERS,
    [B_MSG_SWITCHIN_TURBOBLAZE] = STRINGID_TURBOBLAZEENTERS,
    [B_MSG_SWITCHIN_SLOWSTART] = STRINGID_SLOWSTARTENTERS,
    [B_MSG_SWITCHIN_UNNERVE] = STRINGID_UNNERVEENTERS,
    [B_MSG_SWITCHIN_ANTICIPATION] = STRINGID_ANTICIPATIONACTIVATES,
    [B_MSG_SWITCHIN_FOREWARN] = STRINGID_FOREWARNACTIVATES,
    [B_MSG_SWITCHIN_PRESSURE] = STRINGID_PRESSUREENTERS,
    [B_MSG_SWITCHIN_DARKAURA] = STRINGID_DARKAURAENTERS,
    [B_MSG_SWITCHIN_FAIRYAURA] = STRINGID_FAIRYAURAENTERS,
    [B_MSG_SWITCHIN_AURABREAK] = STRINGID_AURABREAKENTERS,
    [B_MSG_SWITCHIN_COMATOSE] = STRINGID_COMATOSEENTERS,
    [B_MSG_SWITCHIN_SCREENCLEANER] = STRINGID_SCREENCLEANERENTERS,
    [B_MSG_SWITCHIN_ASONE] = STRINGID_ASONEENTERS,
    [B_MSG_SWITCHIN_CURIOUS_MEDICINE] = STRINGID_CURIOUSMEDICINEENTERS,
    [B_MSG_SWITCHIN_PASTEL_VEIL] = STRINGID_PKMNHEALEDPOISON,
    [B_MSG_SWITCHIN_NEUTRALIZING_GAS] = STRINGID_NEUTRALIZINGGASENTERS,
};

const u16 gNoEscapeStringIds[] =
{
    [B_MSG_CANT_ESCAPE]          = STRINGID_CANTESCAPE,
    [B_MSG_DONT_LEAVE_BIRCH]     = STRINGID_DONTLEAVEBIRCH,
    [B_MSG_PREVENTS_ESCAPE]      = STRINGID_PREVENTSESCAPE,
    [B_MSG_CANT_ESCAPE_2]        = STRINGID_CANTESCAPE2,
    [B_MSG_ATTACKER_CANT_ESCAPE] = STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] =
{
    [B_MSG_STARTED_RAIN]      = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_DOWNPOUR]  = STRINGID_DOWNPOURSTARTED, // Unused
    [B_MSG_WEATHER_FAILED]    = STRINGID_BUTITFAILED,
    [B_MSG_STARTED_SANDSTORM] = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_SUNLIGHT]  = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL]      = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW]      = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_FOG]       = STRINGID_FOGCREPTUP, // Unused, can use for custom moves that set fog
};

const u16 gAbilityWeatherChangeStringId[] =
{
    [B_MSG_STARTED_DRIZZLE]        = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_SAND_STREAM]    = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_DROUGHT]        = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL_WARNING]   = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW_WARNING]   = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_DESOLATE_LAND]  = STRINGID_EXTREMELYHARSHSUNLIGHT,
    [B_MSG_STARTED_PRIMORDIAL_SEA] = STRINGID_HEAVYRAIN,
    [B_MSG_STARTED_STRONG_WINDS]   = STRINGID_MYSTERIOUSAIRCURRENT,
};

const u16 gWeatherEndsStringIds[B_MSG_WEATHER_END_COUNT] =
{
    [B_MSG_WEATHER_END_RAIN]                       = STRINGID_RAINSTOPPED,
    [B_MSG_WEATHER_END_SUN]                        = STRINGID_SUNLIGHTFADED,
    [B_MSG_WEATHER_END_SANDSTORM]                  = STRINGID_SANDSTORMSUBSIDED,
    [B_MSG_WEATHER_END_HAIL]                       = STRINGID_HAILSTOPPED,
    [B_MSG_WEATHER_END_SNOW]                       = STRINGID_SNOWSTOPPED,
    [B_MSG_WEATHER_END_FOG]                        = STRINGID_FOGLIFTED,
    [B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT]   = STRINGID_EXTREMESUNLIGHTFADED,
    [B_MSG_WEATHER_END_HEAVY_RAIN]                 = STRINGID_HEAVYRAINLIFTED,
    [B_MSG_WEATHER_END_STRONG_WINDS]               = STRINGID_STRONGWINDSDISSIPATED,
};

const u16 gWeatherTurnStringIds[] =
{
    [B_MSG_WEATHER_TURN_RAIN]         = STRINGID_RAINCONTINUES,
    [B_MSG_WEATHER_TURN_DOWNPOUR]     = STRINGID_DOWNPOURCONTINUES,
    [B_MSG_WEATHER_TURN_SUN]          = STRINGID_SUNLIGHTSTRONG,
    [B_MSG_WEATHER_TURN_SANDSTORM]    = STRINGID_SANDSTORMRAGES,
    [B_MSG_WEATHER_TURN_HAIL]         = STRINGID_HAILCONTINUES,
    [B_MSG_WEATHER_TURN_SNOW]         = STRINGID_SNOWCONTINUES,
    [B_MSG_WEATHER_TURN_FOG]          = STRINGID_FOGISDEEP,
    [B_MSG_WEATHER_TURN_STRONG_WINDS] = STRINGID_MYSTERIOUSAIRCURRENTBLOWSON,
};

const u16 gSandStormHailDmgStringIds[] =
{
    [B_MSG_SANDSTORM] = STRINGID_PKMNBUFFETEDBYSANDSTORM,
    [B_MSG_HAIL]      = STRINGID_PKMNPELTEDBYHAIL
};

const u16 gProtectLikeUsedStringIds[] =
{
    [B_MSG_PROTECTED_ITSELF] = STRINGID_PKMNPROTECTEDITSELF2,
    [B_MSG_BRACED_ITSELF]    = STRINGID_PKMNBRACEDITSELF,
    [B_MSG_PROTECTED_TEAM]   = STRINGID_PROTECTEDTEAM,
};

const u16 gBrokeProtectionStringIds[] =
{
    [B_MSG_FEINT]                 = STRINGID_FELLFORFEINT,
    [B_MSG_BROKE_THROUGH_PROTECT] = STRINGID_BROKETHROUGHPROTECTION,
};

const u16 gReflectLightScreenSafeguardStringIds[] =
{
    [B_MSG_SIDE_STATUS_FAILED]     = STRINGID_BUTITFAILED,
    [B_MSG_SET_REFLECT_SINGLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_REFLECT_DOUBLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_LIGHTSCREEN_SINGLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_LIGHTSCREEN_DOUBLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_SAFEGUARD]          = STRINGID_PKMNCOVEREDBYVEIL,
    [B_MSG_SET_AURORA_VEIL]        = STRINGID_PKMNAURORAVEIL,
};

const u16 gLeechSeedStringIds[] =
{
    [B_MSG_LEECH_SEED_SET]   = STRINGID_PKMNSEEDED,
    [B_MSG_LEECH_SEED_MISS]  = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_LEECH_SEED_FAIL]  = STRINGID_ITDOESNTAFFECT,
    [B_MSG_LEECH_SEED_DRAIN] = STRINGID_PKMNSAPPEDBYLEECHSEED,
    [B_MSG_LEECH_SEED_OOZE]  = STRINGID_ITSUCKEDLIQUIDOOZE,
};

const u16 gRestUsedStringIds[] =
{
    [B_MSG_REST]          = STRINGID_PKMNWENTTOSLEEP,
    [B_MSG_REST_STATUSED] = STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] =
{
    [B_MSG_UPROAR_CONTINUES] = STRINGID_PKMNMAKINGUPROAR,
    [B_MSG_UPROAR_ENDS]      = STRINGID_PKMNCALMEDDOWN
};

const u16 gWokeUpStringIds[] =
{
    [B_MSG_WOKE_UP]        = STRINGID_PKMNWOKEUP,
    [B_MSG_WOKE_UP_UPROAR] = STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gUproarAwakeStringIds[] =
{
    [B_MSG_CANT_SLEEP_UPROAR]  = STRINGID_PKMNCANTSLEEPINUPROAR2,
    [B_MSG_UPROAR_KEPT_AWAKE]  = STRINGID_UPROARKEPTPKMNAWAKE,
};

const u16 gStatUpStringIds[] =
{
    [B_MSG_STAT_CHANGED]            = STRINGID_STATROSE,
    [B_MSG_STAT_WONT_CHANGE]        = STRINGID_STATSWONTINCREASE,
    [B_MSG_STAT_MAXED]              = STRINGID_STATWASMAXEDOUT,
    [B_MSG_STAT_CHANGE_EMPTY]       = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]       = STRINGID_USINGITEMSTATOFPKMNROSE,
    [B_MSG_STAT_CHANGED_BELLY_DRUM] = STRINGID_PKMNCUTHPMAXEDATTACK,
    [B_MSG_USED_DIRE_HIT]           = STRINGID_PKMNUSEDXTOGETPUMPED,
};

// Mostly redundant, combine with above in a future pr
const u16 gStatDownStringIds[] =
{
    [B_MSG_STAT_CHANGED]            = STRINGID_STATFELL,
    [B_MSG_STAT_WONT_CHANGE]        = STRINGID_STATSWONTDECREASE,
    [B_MSG_STAT_CHANGE_EMPTY]       = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]       = STRINGID_USINGITEMSTATOFPKMNFELL,
    [B_MSG_STAT_CHANGED_BELLY_DRUM] = STRINGID_PKMNCUTHPMAXEDATTACK, // Message for contrary is still printed
};

// Index copied from move's index in sTrappingMoves
const u16 gWrappedStringIds[NUM_TRAPPING_MOVES] =
{
    [B_MSG_WRAPPED_BIND]        = STRINGID_PKMNSQUEEZEDBYBIND,     // MOVE_BIND
    [B_MSG_WRAPPED_WRAP]        = STRINGID_PKMNWRAPPEDBY,          // MOVE_WRAP
    [B_MSG_WRAPPED_FIRE_SPIN]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_FIRE_SPIN
    [B_MSG_WRAPPED_CLAMP]       = STRINGID_PKMNCLAMPED,            // MOVE_CLAMP
    [B_MSG_WRAPPED_WHIRLPOOL]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_WHIRLPOOL
    [B_MSG_WRAPPED_SAND_TOMB]   = STRINGID_PKMNTRAPPEDBYSANDTOMB,  // MOVE_SAND_TOMB
    [B_MSG_WRAPPED_MAGMA_STORM] = STRINGID_TRAPPEDBYSWIRLINGMAGMA, // MOVE_MAGMA_STORM
    [B_MSG_WRAPPED_INFESTATION] = STRINGID_INFESTATION,            // MOVE_INFESTATION
    [B_MSG_WRAPPED_SNAP_TRAP]   = STRINGID_PKMNINSNAPTRAP,         // MOVE_SNAP_TRAP
    [B_MSG_WRAPPED_THUNDER_CAGE]= STRINGID_THUNDERCAGETRAPPED,     // MOVE_THUNDER_CAGE
};

const u16 gMistUsedStringIds[] =
{
    [B_MSG_SET_MIST]    = STRINGID_PKMNSHROUDEDINMIST,
    [B_MSG_MIST_FAILED] = STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] =
{
    [B_MSG_GETTING_PUMPED]      = STRINGID_PKMNGETTINGPUMPED,
    [B_MSG_FOCUS_ENERGY_FAILED] = STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] =
{
    [B_MSG_TRANSFORMED]      = STRINGID_PKMNTRANSFORMEDINTO,
    [B_MSG_TRANSFORM_FAILED] = STRINGID_BUTITFAILED
};

const u16 gSubstituteUsedStringIds[] =
{
    [B_MSG_SET_SUBSTITUTE]    = STRINGID_PKMNMADESUBSTITUTE,
    [B_MSG_SUBSTITUTE_FAILED] = STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPOISONED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNPOISONEDBY
};

const u16 gGotParalyzedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPARALYZED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNWASPARALYZEDBY
};

const u16 gFellAsleepStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLASLEEP,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNMADESLEEP,
};

const u16 gGotBurnedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASBURNED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNBURNEDBY
};

const u16 gGotFrostbiteStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNGOTFROSTBITE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNGOTFROSTBITE,
};

const u16 gFrostbiteHealedStringIds[] =
{
    [B_MSG_FROSTBITE_HEALED]         = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_FROSTBITE_HEALED_BY_MOVE] = STRINGID_PKMNFROSTBITEHEALEDBY
};

const u16 gGotFrozenStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASFROZEN,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] =
{
    [B_MSG_DEFROSTED]         = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_DEFROSTED_BY_MOVE] = STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gKOFailedStringIds[] =
{
    [B_MSG_KO_MISS]       = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_KO_UNAFFECTED] = STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLINLOVE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNSXINFATUATEDY
};

const u16 gAbsorbDrainStringIds[] =
{
    [B_MSG_ABSORB]      = STRINGID_PKMNENERGYDRAINED,
    [B_MSG_ABSORB_OOZE] = STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] =
{
    [B_MSG_WEAKEN_ELECTRIC] = STRINGID_ELECTRICITYWEAKENED,
    [B_MSG_WEAKEN_FIRE]     = STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] =
{
    [B_MSG_BELL]                     = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_ATTACKER] = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_PARTNER]  = STRINGID_BELLCHIMED,
    [B_MSG_BELL_BOTH_SOUNDPROOF]     = STRINGID_BELLCHIMED,
    [B_MSG_SOOTHING_AROMA]           = STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] =
{
    [B_MSG_FUTURE_SIGHT] = STRINGID_PKMNFORESAWATTACK,
    [B_MSG_DOOM_DESIRE]  = STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] =
{
    [BALL_NO_SHAKES]     = STRINGID_PKMNBROKEFREE,
    [BALL_1_SHAKE]       = STRINGID_ITAPPEAREDCAUGHT,
    [BALL_2_SHAKES]      = STRINGID_AARGHALMOSTHADIT,
    [BALL_3_SHAKES_FAIL] = STRINGID_SHOOTSOCLOSE
};

// Overworld weathers that don't have an associated battle weather default to "It is raining."
const u16 gWeatherStartsStringIds[] =
{
    [WEATHER_NONE]               = STRINGID_ITISRAINING,
    [WEATHER_SUNNY_CLOUDS]       = STRINGID_ITISRAINING,
    [WEATHER_SUNNY]              = STRINGID_ITISRAINING,
    [WEATHER_RAIN]               = STRINGID_ITISRAINING,
    [WEATHER_SNOW]               = (B_OVERWORLD_SNOW >= GEN_9 ? STRINGID_ITISSNOWING : STRINGID_ITISHAILING),
    [WEATHER_RAIN_THUNDERSTORM]  = STRINGID_ITISRAINING,
    [WEATHER_FOG_HORIZONTAL]     = STRINGID_FOGISDEEP,
    [WEATHER_VOLCANIC_ASH]       = STRINGID_ITISRAINING,
    [WEATHER_SANDSTORM]          = STRINGID_SANDSTORMISRAGING,
    [WEATHER_FOG_DIAGONAL]       = STRINGID_FOGISDEEP,
    [WEATHER_UNDERWATER]         = STRINGID_ITISRAINING,
    [WEATHER_SHADE]              = STRINGID_ITISRAINING,
    [WEATHER_DROUGHT]            = STRINGID_SUNLIGHTISHARSH,
    [WEATHER_DOWNPOUR]           = STRINGID_ITISRAINING,
    [WEATHER_UNDERWATER_BUBBLES] = STRINGID_ITISRAINING,
    [WEATHER_ABNORMAL]           = STRINGID_ITISRAINING
};

const u16 gTerrainStartsStringIds[] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_MISTSWIRLSAROUND,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_ELECTRICCURRENTISRUNNING,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_SEEMSWEIRD,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_ISCOVEREDWITHGRASS,
};

const u16 gPrimalWeatherBlocksStringIds[] =
{
    [B_MSG_PRIMAL_WEATHER_FIZZLED_BY_RAIN]      = STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN,
    [B_MSG_PRIMAL_WEATHER_EVAPORATED_IN_SUN]    = STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT,
};

const u16 gInobedientStringIds[] =
{
    [B_MSG_LOAFING]            = STRINGID_PKMNLOAFING,
    [B_MSG_WONT_OBEY]          = STRINGID_PKMNWONTOBEY,
    [B_MSG_TURNED_AWAY]        = STRINGID_PKMNTURNEDAWAY,
    [B_MSG_PRETEND_NOT_NOTICE] = STRINGID_PKMNPRETENDNOTNOTICE,
    [B_MSG_INCAPABLE_OF_POWER] = STRINGID_PKMNINCAPABLEOFPOWER
};

const u16 gSafariReactionStringIds[NUM_SAFARI_REACTIONS] =
{
    [B_MSG_MON_WATCHING] = STRINGID_PKMNWATCHINGCAREFULLY,
    [B_MSG_MON_ANGRY]    = STRINGID_PKMNANGRY,
    [B_MSG_MON_EATING]   = STRINGID_PKMNEATING
};

const u16 gSafariGetNearStringIds[] =
{
    [B_MSG_CREPT_CLOSER]    = STRINGID_CREPTCLOSER,
    [B_MSG_CANT_GET_CLOSER] = STRINGID_CANTGETCLOSER
};

const u16 gSafariPokeblockResultStringIds[] =
{
    [B_MSG_MON_CURIOUS]    = STRINGID_PKMNCURIOUSABOUTX,
    [B_MSG_MON_ENTHRALLED] = STRINGID_PKMNENTHRALLEDBYX,
    [B_MSG_MON_IGNORED]    = STRINGID_PKMNIGNOREDX
};

const u16 CureStatusBerryEffectStringID[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PKMNSITEMCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PKMNSITEMCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PKMNSITEMHEALEDBURN,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNSITEMDEFROSTEDIT,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNSITEMHEALEDFROSTBITE,
    [B_MSG_CURED_SLEEP] = STRINGID_PKMNSITEMWOKEIT,
    [B_MSG_CURED_CONFUSION] = STRINGID_PKMNSITEMSNAPPEDOUT,
};

const u16 gItemSwapStringIds[] =
{
    [B_MSG_ITEM_SWAP_TAKEN] = STRINGID_PKMNOBTAINEDX,
    [B_MSG_ITEM_SWAP_GIVEN] = STRINGID_PKMNOBTAINEDX2,
    [B_MSG_ITEM_SWAP_BOTH]  = STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] =
{
    [B_MSG_FLASH_FIRE_BOOST]    = STRINGID_PKMNRAISEDFIREPOWERWITH,
    [B_MSG_FLASH_FIRE_NO_BOOST] = STRINGID_PKMNSXMADEITINEFFECTIVE
};

const u16 gCaughtMonStringIds[] =
{
    [B_MSG_SENT_SOMEONES_PC]   = STRINGID_PKMNTRANSFERREDSOMEONESPC,
    [B_MSG_SENT_LANETTES_PC]   = STRINGID_PKMNTRANSFERREDLANETTESPC,
    [B_MSG_SOMEONES_BOX_FULL]  = STRINGID_PKMNBOXSOMEONESPCFULL,
    [B_MSG_LANETTES_BOX_FULL]  = STRINGID_PKMNBOXLANETTESPCFULL,
    [B_MSG_SWAPPED_INTO_PARTY] = STRINGID_PKMNSENTTOPCAFTERCATCH,
};

const u16 gRoomsStringIds[] =
{
    STRINGID_PKMNTWISTEDDIMENSIONS, STRINGID_TRICKROOMENDS,
    STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON, STRINGID_WONDERROOMENDS,
    STRINGID_HELDITEMSLOSEEFFECTS, STRINGID_MAGICROOMENDS,
    STRINGID_EMPTYSTRING3
};

const u16 gStatusConditionsStringIds[] =
{
    STRINGID_PKMNWASPOISONED, STRINGID_PKMNBADLYPOISONED, STRINGID_PKMNWASBURNED, STRINGID_PKMNWASPARALYZED, STRINGID_PKMNFELLASLEEP, STRINGID_PKMNGOTFROSTBITE
};

const u16 gDamageNonTypesStartStringIds[] =
{
    [B_MSG_TRAPPED_WITH_VINES]  = STRINGID_TEAMTRAPPEDWITHVINES,
    [B_MSG_CAUGHT_IN_VORTEX]    = STRINGID_TEAMCAUGHTINVORTEX,
    [B_MSG_SURROUNDED_BY_FIRE]  = STRINGID_TEAMSURROUNDEDBYFIRE,
    [B_MSG_SURROUNDED_BY_ROCKS] = STRINGID_TEAMSURROUNDEDBYROCKS,
};

const u16 gDamageNonTypesDmgStringIds[] =
{
    [B_MSG_HURT_BY_VINES]        = STRINGID_PKMNHURTBYVINES,
    [B_MSG_HURT_BY_VORTEX]       = STRINGID_PKMNHURTBYVORTEX,
    [B_MSG_BURNING_UP]           = STRINGID_PKMNBURNINGUP,
    [B_MSG_HURT_BY_ROCKS_THROWN] = STRINGID_PKMNHURTBYROCKSTHROWN,
};

const u16 gRemoveHazardsStringIds[] =
{
    [HAZARDS_SPIKES] = STRINGID_SPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STICKY_WEB] = STRINGID_STICKYWEBDISAPPEAREDFROMTEAM,
    [HAZARDS_TOXIC_SPIKES] = STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STEALTH_ROCK] = STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM,
    [HAZARDS_STEELSURGE] = STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM,
};

const u16 gZenModeStringIds[] =
{
    [B_MSG_ZEN_MODE_TRIGGERED] = STRINGID_ZENMODETRIGGERED,
    [B_MSG_ZEN_MODE_ENDED] = STRINGID_ZENMODEENDED
};

const u16 gCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_SCRCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_SCRCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_SCRCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_SCRCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gPartyCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PARTYCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PARTYCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PARTYCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_PARTYCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PARTYCUREDFREEZE,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PARTYCUREDFROSTBITE,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gHurtByStringIds[] =
{
    [B_MSG_HURT] = STRINGID_PKMNWASHURT,
    [B_MSG_HURT_BY_ITEM] = STRINGID_PKMNHURTSWITH,
};

const u16 gBreakScreensStringIds[] =
{
    [B_MSG_BREAK_REFLECT] = STRINGID_REFLECTWOREOFF,
    [B_MSG_BREAK_LIGHT_SCREEN] = STRINGID_LIGHTSCREENWOREOFF,
    [B_MSG_BREAK_AURORA_VEIL] = STRINGID_AURORAVEILWOREOFF,
};

const u8 gText_PkmnIsEvolving[] = _("What?\n{STR_VAR_1} is evolving!${JPN}{JPN}おや‥！？\n{STR_VAR_1}の　ようすが‥！");
const u8 gText_CongratsPkmnEvolved[] = _("Congratulations! Your {STR_VAR_1}\nevolved into {STR_VAR_2}!{WAIT_SE}\p${JPN}{JPN}おめでとう！　{STR_VAR_1}は\n{STR_VAR_2}に　しんかした！{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("Huh? {STR_VAR_1}\nstopped evolving!\p${JPN}{JPN}あれ‥？　{STR_VAR_1}の\nへんかが　とまった！\p");
const u8 gText_EllipsisQuestionMark[] = _("……?\p${JPN}{JPN}‥‥？\p");
const u8 gText_WhatWillPkmnDo[] = _("What will\n{B_BUFF1} do?${JPN}{JPN}{B_BUFF1}は\nどうする？");
const u8 gText_WhatWillPkmnDo2[] = _("What will\n{B_PLAYER_NAME} do?${JPN}{JPN}{B_PLAYER_NAME}は\nどうする？");
const u8 gText_WhatWillWallyDo[] = _("What will\nWALLY do?${JPN}{JPN}ミツルは\nどうする？");
const u8 gText_LinkStandby[] = _("{PAUSE 16}Link standby…${JPN}{JPN}{PAUSE 16}つうしんたいきちゅう‥");
const u8 gText_BattleMenu[] = _("Battle{CLEAR_TO 56}Bag\nPokémon{CLEAR_TO 56}Run${JPN}{JPN}たたかう{CLEAR_TO 56}バッグ\nポケモン{CLEAR_TO 56}にげる");
const u8 gText_SafariZoneMenu[] = _("Ball{CLEAR_TO 56}{POKEBLOCK}\nGo Near{CLEAR_TO 56}Run${JPN}{JPN}ボール{CLEAR_TO 56}ポロック\nちかづく{CLEAR_TO 56}にげる");
const u8 gText_SafariZoneMenuFrlg[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}BALL{CLEAR_TO 56}BAIT\nROCK{CLEAR_TO 56}RUN${JPN}{JPN}{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}ボール{CLEAR_TO 56}エサ\nいし{CLEAR_TO 56}にげる");
const u8 gText_MoveInterfacePP[] = _("PP ");
const u8 gText_MoveInterfaceType[] = _("TYPE/${JPN}{JPN}タイプ/");
const u8 gText_MoveInterfacePPType[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}PP\nTYPE/${JPN}{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}PP\nタイプ/");
const u8 gText_MoveInterfaceDynamicColors[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}");
const u8 gText_WhichMoveToForget4[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Which move should\nbe forgotten?${JPN}{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}どの　わざを\nわすれさせる？");
const u8 gText_BattleCatchOrNot[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Catch\nDon't catch${JPN}{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}つかまえる\nつかまえない");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Yes\nNo${JPN}{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}はい\nいいえ");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Switch\nwhich?${JPN}{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}どれと\nいれかえる？");
const u8 gText_BattleSwitchWhich2[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}");
const u8 gText_BattleSwitchWhich3[] = _("{UP_ARROW}");
const u8 gText_BattleSwitchWhich4[] = _("{ESCAPE 4}");
const u8 gText_BattleSwitchWhich5[] = _("-");
const u8 gText_SafariBalls[] = _("Safari Balls${JPN}{JPN}サファリボール");
const u8 gText_SafariBallLeft[] = _("Left: $");
const u8 gText_Sleep[] = _("sleep${JPN}{JPN}ねむり");
const u8 gText_Poison[] = _("poison${JPN}{JPN}どく");
const u8 gText_Burn[] = _("burn${JPN}{JPN}やけど");
const u8 gText_Paralysis[] = _("paralysis${JPN}{JPN}まひ");
const u8 gText_Ice[] = _("ice${JPN}{JPN}こおり");
const u8 gText_Confusion[] = _("confusion${JPN}{JPN}こんらん");
const u8 gText_Love[] = _("love${JPN}{JPN}メロメロ");
const u8 gText_SpaceAndSpace[] = _(" and ${JPN}{JPN}と");
const u8 gText_CommaSpace[] = _(", ${JPN}{JPN}・");
const u8 gText_Space2[] = _(" ");
const u8 gText_LineBreak[] = _("\l");
const u8 gText_NewLine[] = _("\n");
const u8 gText_Are[] = _("are");
const u8 gText_Are2[] = _("are");
const u8 gText_BadEgg[] = _("Bad Egg${JPN}{JPN}ダメタマゴ");
const u8 gText_BattleWallyName[] = _("WALLY");
const u8 gText_Win[] = _("{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}Win${JPN}{JPN}{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}かち");
const u8 gText_Loss[] = _("{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}Loss${JPN}{JPN}{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}まけ");
const u8 gText_Draw[] = _("{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}Draw${JPN}{JPN}{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}ひきわけ");
static const u8 sText_SpaceIs[] = _(" is");
static const u8 sText_ApostropheS[] = _("'s");
const u8 gText_BattleTourney[] = _("BATTLE TOURNEY${JPN}{JPN}バトルトーナメント");

const u8 *const gRoundsStringTable[DOME_ROUNDS_COUNT] =
{
    [DOME_ROUND1]    = COMPOUND_STRING("Round 1${JPN}{JPN}1かいせん"),
    [DOME_ROUND2]    = COMPOUND_STRING("Round 2${JPN}{JPN}2かいせん"),
    [DOME_SEMIFINAL] = COMPOUND_STRING("Semifinal${JPN}{JPN}じゅんけっしょう"),
    [DOME_FINAL]     = COMPOUND_STRING("Final${JPN}{JPN}けっしょう"),
};

const u8 gText_TheGreatNewHope[] = _("The great new hope!\p${JPN}{JPN}きたいの　ルーキー！\p");
const u8 gText_WillChampionshipDreamComeTrue[] = _("Will the championship dream come true?!\p${JPN}{JPN}ゆうしょうの　ゆめは　かなうのか！？\p");
const u8 gText_AFormerChampion[] = _("A former champion!\p${JPN}{JPN}かつての　チャンピオン！\p");
const u8 gText_ThePreviousChampion[] = _("The previous champion!\p${JPN}{JPN}ぜんかいの　チャンピオン！\p");
const u8 gText_TheUnbeatenChampion[] = _("The unbeaten champion!\p${JPN}{JPN}むはいの　チャンピオン！\p");
const u8 gText_PlayerMon1Name[] = _("{B_PLAYER_MON1_NAME}");
const u8 gText_Vs[] = _("VS");
const u8 gText_OpponentMon1Name[] = _("{B_OPPONENT_MON1_NAME}");
const u8 gText_Mind[] = _("Mind${JPN}{JPN}こころ");
const u8 gText_Skill[] = _("Skill${JPN}{JPN}わざ");
const u8 gText_Body[] = _("Body${JPN}{JPN}からだ");
const u8 gText_Judgment[] = _("{B_BUFF1}{CLEAR 13}Judgment{CLEAR 13}{B_BUFF2}${JPN}{JPN}{B_BUFF1}{CLEAR 13}はんてい{CLEAR 13}{B_BUFF2}");
static const u8 sText_TwoTrainersSentPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} sent out {B_OPPONENT_MON1_NAME}!\p{B_TRAINER2_NAME_WITH_CLASS} sent out {B_OPPONENT_MON2_NAME}!${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}は　{B_OPPONENT_MON1_NAME}を\nくりだした！\p{B_TRAINER2_NAME_WITH_CLASS}は　{B_OPPONENT_MON2_NAME}を\nくりだした！");
static const u8 sText_Trainer2SentOutPkmn[] = _("{B_TRAINER2_NAME_WITH_CLASS} sent out {B_BUFF1}!${JPN}{JPN}{B_TRAINER2_NAME_WITH_CLASS}は　{B_BUFF1}を\nくりだした！");
static const u8 sText_TwoTrainersWantToBattle[] = _("You are challenged by {B_TRAINER1_NAME_WITH_CLASS} and {B_TRAINER2_NAME_WITH_CLASS}!\p${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}と　{B_TRAINER2_NAME_WITH_CLASS}が\nしょうぶを　しかけてきた！\p");
static const u8 sText_InGamePartnerSentOutZGoN[] = _("{B_PARTNER_NAME_WITH_CLASS} sent out {B_PLAYER_MON2_NAME}! Go, {B_PLAYER_MON1_NAME}!${JPN}{JPN}{B_PARTNER_NAME_WITH_CLASS}は　{B_PLAYER_MON2_NAME}を\nくりだした！　ゆけっ！　{B_PLAYER_MON1_NAME}！");
static const u8 sText_InGamePartnerSentOutNGoZ[] = _("{B_PARTNER_NAME_WITH_CLASS} sent out {B_PLAYER_MON1_NAME}! Go, {B_PLAYER_MON2_NAME}!${JPN}{JPN}{B_PARTNER_NAME_WITH_CLASS}は　{B_PLAYER_MON1_NAME}を\nくりだした！　ゆけっ！　{B_PLAYER_MON2_NAME}！");
static const u8 sText_InGamePartnerSentOutPkmn1[] = _("{B_PARTNER_NAME_WITH_CLASS} sent out {B_PLAYER_MON1_NAME}!${JPN}{JPN}{B_PARTNER_NAME_WITH_CLASS}は　{B_PLAYER_MON1_NAME}を\nくりだした！");
static const u8 sText_InGamePartnerSentOutPkmn2[] = _("{B_PARTNER_NAME_WITH_CLASS} sent out {B_PLAYER_MON2_NAME}!${JPN}{JPN}{B_PARTNER_NAME_WITH_CLASS}は　{B_PLAYER_MON2_NAME}を\nくりだした！");
static const u8 sText_InGamePartnerWithdrewPkmn1[] = _("{B_PARTNER_NAME_WITH_CLASS} withdrew {B_PLAYER_MON1_NAME}!${JPN}{JPN}{B_PARTNER_NAME_WITH_CLASS}は　{B_PLAYER_MON1_NAME}を\nひっこめた！");
static const u8 sText_InGamePartnerWithdrewPkmn2[] = _("{B_PARTNER_NAME_WITH_CLASS} withdrew {B_PLAYER_MON2_NAME}!${JPN}{JPN}{B_PARTNER_NAME_WITH_CLASS}は　{B_PLAYER_MON2_NAME}を\nひっこめた！");

const u16 gBattlePalaceFlavorTextTable[] =
{
    [B_MSG_GLINT_IN_EYE]   = STRINGID_GLINTAPPEARSINEYE,
    [B_MSG_GETTING_IN_POS] = STRINGID_PKMNGETTINGINTOPOSITION,
    [B_MSG_GROWL_DEEPLY]   = STRINGID_PKMNBEGANGROWLINGDEEPLY,
    [B_MSG_EAGER_FOR_MORE] = STRINGID_PKMNEAGERFORMORE,
};

const u8 *const gRefereeStringsTable[] =
{
    [B_MSG_REF_NOTHING_IS_DECIDED] = COMPOUND_STRING("REFEREE: If nothing is decided in 3 turns, we will go to judging!${JPN}{JPN}しんぱん『3ターンで　けっちゃくが　つかなければ\nはんていに　なります！』"),
    [B_MSG_REF_THATS_IT]           = COMPOUND_STRING("REFEREE: That's it! We will now go to judging to determine the winner!${JPN}{JPN}しんぱん『そこまで！　はんていで　しょうしゃを\nきめます！』"),
    [B_MSG_REF_JUDGE_MIND]         = COMPOUND_STRING("REFEREE: Judging category 1, Mind! The POKéMON showing the most guts!\p${JPN}{JPN}しんぱん『はんてい　こうもく　1　こころ！\nもっとも　きあいの　はいった　ポケモン！』\p"),
    [B_MSG_REF_JUDGE_SKILL]        = COMPOUND_STRING("REFEREE: Judging category 2, Skill! The POKéMON using moves the best!\p${JPN}{JPN}しんぱん『はんてい　こうもく　2　わざ！　わざを\nもっとも　うまく　つかった　ポケモン！』\p"),
    [B_MSG_REF_JUDGE_BODY]         = COMPOUND_STRING("REFEREE: Judging category 3, Body! The POKéMON with the most vitality!\p${JPN}{JPN}しんぱん『はんてい　こうもく　3　からだ！\nもっとも　たいりょくの　ある　ポケモン！』\p"),
    [B_MSG_REF_PLAYER_WON]         = COMPOUND_STRING("REFEREE: Judgment: {B_BUFF1} to {B_BUFF2}! The winner is {B_PLAYER_NAME}'s {B_PLAYER_MON1_NAME}!\p${JPN}{JPN}しんぱん『はんてい\n{B_BUFF1}たい{B_BUFF2}！\lしょうしゃは　{B_PLAYER_NAME}の\l{B_PLAYER_MON1_NAME}！』\p"),
    [B_MSG_REF_OPPONENT_WON]       = COMPOUND_STRING("REFEREE: Judgment: {B_BUFF1} to {B_BUFF2}! The winner is {B_TRAINER1_NAME}'s {B_OPPONENT_MON1_NAME}!\p${JPN}{JPN}しんぱん『はんてい\n{B_BUFF1}たい{B_BUFF2}！\lしょうしゃは　{B_TRAINER1_NAME}の\l{B_OPPONENT_MON1_NAME}！』\p"),
    [B_MSG_REF_DRAW]               = COMPOUND_STRING("REFEREE: Judgment: 3 to 3! We have a draw!\p${JPN}{JPN}しんぱん『はんてい　3たい3！　ひきわけです！』\p"),
    [B_MSG_REF_COMMENCE_BATTLE]    = COMPOUND_STRING("REFEREE: {B_PLAYER_MON1_NAME} VS {B_OPPONENT_MON1_NAME}! Commence battling!${JPN}{JPN}しんぱん『{B_PLAYER_MON1_NAME}　VS\n{B_OPPONENT_MON1_NAME}！　しあい　かいし！』"),
};

static const u8 sText_Trainer1Fled[] = _( "{PLAY_SE SE_FLEE}{B_TRAINER1_NAME_WITH_CLASS} fled!");
static const u8 sText_PlayerLostAgainstTrainer1[] = _("You lost to {B_TRAINER1_NAME_WITH_CLASS}!${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}との　しょうぶに　まけた！");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("You battled to a draw against {B_TRAINER1_NAME_WITH_CLASS}!${JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}との　しょうぶは　ひきわけに\nなった！");
const u8 gText_RecordBattleToPass[] = _("Would you like to record your battle\non your Frontier Pass?${JPN}{JPN}たたかいの　きろくを\nフロンティアパスに　のこしますか？");
const u8 gText_BattleRecordedOnPass[] = _("{B_PLAYER_NAME}'s battle result was recorded\non the Frontier Pass.${JPN}{JPN}{B_PLAYER_NAME}の　たたかいの　きろくを\nフロンティアパスに　のこしました");
static const u8 sText_LinkTrainerWantsToBattlePause[] = _("You are challenged by {B_LINK_OPPONENT1_NAME}!\p${JPN}{JPN}{B_LINK_OPPONENT1_NAME}が　しょうぶを\nしかけてきた！\p");
static const u8 sText_TwoLinkTrainersWantToBattlePause[] = _("You are challenged by {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!\p${JPN}{JPN}{B_LINK_OPPONENT1_NAME}と　{B_LINK_OPPONENT2_NAME}が\nしょうぶを　しかけてきた！\p");
static const u8 sText_Your1[] = _("Your${JPN}{JPN}みかた");
static const u8 sText_Opposing1[] = _("The opposing${JPN}{JPN}あいて");
static const u8 sText_Your2[] = _("your${JPN}{JPN}みかた");
static const u8 sText_Opposing2[] = _("the opposing${JPN}{JPN}あいて");
static const u8 sText_EmptyStatus[] = _("$$$$$$$");

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
    [B_CATCH_OR_NOT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_KantoTutorial[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
    [B_WIN_OAK_OLD_MAN] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 1,
        .speed = 1,
        .fgColor = 2,
        .bgColor = 1,
        .shadowColor = 3,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_Arena[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [ARENA_WIN_PLAYER_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_VS] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_OPPONENT_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_MIND] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_SKILL] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_BODY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TITLE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TEXT] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 2,
        .color.background = 1,
        .color.accent = 1,
        .color.shadow = 3,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
};

static const struct BattleWindowText *const sBattleTextOnWindowsInfo[] =
{
    [B_WIN_TYPE_NORMAL] = sTextOnWindowsInfo_Normal,
    [B_WIN_TYPE_ARENA]  = sTextOnWindowsInfo_Arena,
    [B_WIN_TYPE_KANTO_TUTORIAL] = sTextOnWindowsInfo_KantoTutorial,
};

static const u8 sRecordedBattleTextSpeeds[] = {8, 4, 1, 0};

void BufferStringBattle(enum StringID stringID, enum BattlerId battler)
{
    s32 i;
    const u8 *stringPtr = NULL;

    gBattleMsgDataPtr = (struct BattleMsgData *)(&gBattleResources->bufferA[battler][4]);
    gLastUsedItem = gBattleMsgDataPtr->lastItem;
    gLastUsedAbility = gBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = gBattleMsgDataPtr->scrActive;
    gBattleStruct->scriptPartyIdx = gBattleMsgDataPtr->bakScriptPartyIdx;
    gBattleStruct->hpScale = gBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = gBattleMsgDataPtr->itemEffectBattler;
    gBattleStruct->stringMoveType = gBattleMsgDataPtr->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = gBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = gBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = gBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = gBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringID)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                {
                    stringPtr = sText_TwoTrainersWantToBattle;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                {
                    stringPtr = sText_TwoLinkTrainersWantToBattle;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                    {
                        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                            stringPtr = sText_LinkTrainerWantsToBattle;
                        else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                            stringPtr = sText_TwoTrainersWantToBattle;
                        else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
                            stringPtr = sText_LinkTrainerWantsToBattlePause;
                        else
                            stringPtr = sText_TwoLinkTrainersWantToBattlePause;
                    }
                    else
                    {
                        stringPtr = sText_TwoLinkTrainersWantToBattle;
                    }
                }
                else
                {
                    if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                        stringPtr = sText_Trainer1WantsToBattle;
                    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                        stringPtr = sText_LinkTrainerWantsToBattlePause;
                    else
                        stringPtr = sText_LinkTrainerWantsToBattle;
                }
            }
            else
            {
                if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1WantsToBattle;
                else if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
                    stringPtr = sText_TwoTrainersWantToBattle;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = sText_TwoTrainersWantToBattle;
                else
                    stringPtr = sText_Trainer1WantsToBattle;
            }
        }
        else
        {
            if (IsGhostBattleWithoutScope())
                stringPtr = sText_GhostAppearedCantId;
            else if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
                stringPtr = sText_TheGhostAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
                stringPtr = sText_LegendaryPkmnAppeared;
            else if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))))
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_CATCH_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (IsOnPlayerSide(battler))
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetPartnerBattler(battler))))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI && (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK || gBattleTypeFlags & BATTLE_TYPE_LINK))
                {
                    if (BattlerIsPlayer(battler))
                        stringPtr = sText_LinkPartnerSentOutPkmn2GoPkmn; // Player is on left
                    else
                        stringPtr = sText_LinkPartnerSentOutPkmn1GoPkmn; // Link Partner on left
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                {
                    if (BattlerIsPlayer(battler) && (battler & BIT_FLANK) == B_FLANK_LEFT)
                        stringPtr = sText_InGamePartnerSentOutZGoN; // Player is on left
                    else
                        stringPtr = sText_InGamePartnerSentOutNGoZ; // Partner on left
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_RAID)
                {
                    stringPtr = sText_BeCarefulPkmn;
                }
                else
                {
                    stringPtr = sText_GoTwoPkmn;
                }
            }
            else
            {
                stringPtr = sText_GoPkmn;
            }
        }
        else
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetPartnerBattler(battler))))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
                else if ((gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) || (gBattleTypeFlags & BATTLE_TYPE_MULTI && (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK || gBattleTypeFlags & BATTLE_TYPE_LINK)))
                    stringPtr = sText_TwoLinkTrainersIntroSendOutPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS || gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                    stringPtr = sText_TwoTrainersSentPkmn;
                else if (BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler))) // Link Opponent doubles and test opponent
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
            }
            else if (BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler)))
            {
                if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
                    stringPtr = sText_LinkTrainerIntroSendOutPkmn;
            }
            else
            {
                stringPtr = sText_Trainer1SentOutPkmn;
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if ((GetBattlerPosition(battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(battler) || BattlerIsWally(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerWithdrewPkmn1;
                else
                    stringPtr = sText_InGamePartnerWithdrewPkmn1;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent A and test opponent
            {
                stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerWithdrewPkmn2;
                else
                    stringPtr = sText_InGamePartnerWithdrewPkmn2;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (BattleSideHasTwoTrainers(B_SIDE_OPPONENT))
                    stringPtr = sText_LinkTrainer2WithdrewPkmn;
                else
                    stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if ((GetBattlerPosition(gBattleScripting.battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(gBattleScripting.battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerSentOutPkmn1;
                else
                    stringPtr = sText_InGamePartnerSentOutPkmn1;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent A and test opponent
            {
                stringPtr = sText_LinkTrainerSentOutPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(gBattleScripting.battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerSentOutPkmn2;
                else
                    stringPtr = sText_InGamePartnerSentOutPkmn2;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (BattleSideHasTwoTrainers(B_SIDE_OPPONENT))
                    stringPtr = sText_LinkTrainer2SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2SentOutPkmn;
            }
            else // Single trainer double Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn2;
            }
        }
        break;
    case STRINGID_USEDMOVE: // Pokémon used a move msg
        if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
         && !IsZMove(gBattleMsgDataPtr->currentMove)
         && !IsMaxMove(gBattleMsgDataPtr->currentMove))
            StringCopy(gBattleTextBuff3, gTypesInfo[*(&gBattleStruct->stringMoveType)].generic);
        else
            StringCopy(gBattleTextBuff3, GetMoveName(gBattleMsgDataPtr->currentMove));
        stringPtr = sText_AttackerUsedX;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(GetPartnerBattler(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = sText_TwoWildFled;
            else
                stringPtr = sText_WildFled;
        }
        else
        {
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(GetPartnerBattler(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                        stringPtr = sText_TwoInGameTrainersDefeated;
                    else
                        stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
                    break;
                }
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    break;
                }
            }
        }
        break;
    case STRINGID_TRAINERSLIDE:
        stringPtr = gBattleStruct->trainerSlideMsg;
        break;
    default: // load a string from the table
        if (stringID >= STRINGID_COUNT)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
            stringPtr = gBattleStringsTable[stringID];
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8 *src)
{
#ifndef NDEBUG
    u32 j, strWidth;
    u32 dstID = BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
    for (j = 1;; j++)
    {
        strWidth = GetStringLineWidth(0, gDisplayedStringBattle, 0, j, sizeof(gDisplayedStringBattle));
        if (strWidth == 0)
            break;
    }
    return dstID;
#else
    return BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
#endif
}

static const u8 *TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[8];
    u32 chars1, chars2;
    u8 *statusPtr;

    memcpy(status, sText_EmptyStatus, min(ARRAY_COUNT(status), ARRAY_COUNT(sText_EmptyStatus)));

    statusPtr = status;
    for (i = 0; i < ARRAY_COUNT(status); i++)
    {
        if (*src == EOS) break; // one line required to match -g
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32 *)(&status[0]);
    chars2 = *(u32 *)(&status[4]);

    for (i = 0; i < ARRAY_COUNT(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32 *)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32 *)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

static void GetBattlerNick(enum BattlerId battler, u8 *dst)
{
    struct Pokemon *illusionMon = GetIllusionMonPtr(battler);
    struct Pokemon *mon = GetBattlerMon(battler);

    if (illusionMon != NULL)
        mon = illusionMon;
    GetMonData(mon, MON_DATA_NICKNAME, dst);
    StringGet_Nickname(dst);
}

#define HANDLE_NICKNAME_STRING_CASE(battler)                            \
    if (!IsOnPlayerSide(battler))                                       \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
        toCpy = QoL_LangStr(toCpy); /* [QoL] */                         \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
    }                                                                   \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;

#define HANDLE_NICKNAME_STRING_LOWERCASE(battler)                       \
    if (!IsOnPlayerSide(battler))                       \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefixLower;                           \
        else                                                            \
            toCpy = sText_WildPkmnPrefixLower;                          \
        toCpy = QoL_LangStr(toCpy); /* [QoL] */                         \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
    }                                                                   \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;

static const u8 *BattleStringGetOpponentNameByTrainerId(u16 trainerId, u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
    {
        u32 i;
        for (i = 0; i < ARRAY_COUNT(gBattleResources->secretBase->trainerName); i++)
            text[i] = gBattleResources->secretBase->trainerName[i];
        text[i] = EOS;
        ConvertInternationalString(text, gBattleResources->secretBase->language);
        toCpy = text;
    }
    else if (trainerId == TRAINER_UNION_ROOM)
    {
        toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_LINK_OPPONENT)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler)].name;
        else
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler) & BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
    {
        CopyFrontierBrainTrainerName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
    {
        GetFrontierTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
    {
        GetTrainerTowerOpponentName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
    {
        GetTrainerHillTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
    {
        GetEreaderTrainerName(text);
        toCpy = text;
    }
    else
    {
        enum TrainerClassID trainerClass = GetTrainerClassFromId(TRAINER_BATTLE_PARAM.opponentA);

        if (trainerClass == TRAINER_CLASS_RIVAL_EARLY_FRLG || trainerClass == TRAINER_CLASS_RIVAL_LATE_FRLG || trainerClass == TRAINER_CLASS_CHAMPION_FRLG)
            toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
        else
            toCpy = GetTrainerNameFromId(trainerId);
    }

    assertf(DoesStringProperlyTerminate(toCpy, TRAINER_NAME_LENGTH + 1),"Opponent needs a valid name")
    {
        return sText_EmptyString4;
    }

    return toCpy;
}

static const u8 *BattleStringGetOpponentName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_OPPONENT_LEFT:
        toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    case B_POSITION_OPPONENT_RIGHT:
        if (gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI) && !BATTLE_TWO_VS_ONE_OPPONENT)
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, battler);
        else
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetPlayerName(u8 *text, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_PLAYER_LEFT:
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            toCpy = gLinkPlayers[0].name;
        else
            toCpy = gSaveBlock2Ptr->playerName;
        break;
    case B_POSITION_PLAYER_RIGHT:
        if (((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER)))
            || gTestRunnerEnabled)
        {
            toCpy = gLinkPlayers[0].name;
        }
        else if ((gBattleTypeFlags & BATTLE_TYPE_LINK) && gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_MULTI))
        {
            toCpy = gLinkPlayers[2].name;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        {
            GetFrontierTrainerName(text, gPartnerTrainerId);
            toCpy = text;
        }
        else
        {
            toCpy = gSaveBlock2Ptr->playerName;
        }
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetTrainerName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    if (IsOnPlayerSide(battler))
        return BattleStringGetPlayerName(text, battler);
    else
        return BattleStringGetOpponentName(text, multiplayerId, battler);
}

static const u8 *BattleStringGetOpponentClassByTrainerId(u16 trainerId)
{
    const u8 *toCpy;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
        toCpy = gTrainerClasses[GetSecretBaseTrainerClass()].name;
    else if (trainerId == TRAINER_UNION_ROOM)
        toCpy = gTrainerClasses[GetUnionRoomTrainerClass()].name;
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
        toCpy = gTrainerClasses[GetFrontierBrainTrainerClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
        toCpy = gTrainerClasses[GetFrontierOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
        toCpy = gTrainerClasses[GetTrainerTowerOpponentClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
        toCpy = gTrainerClasses[GetTrainerHillOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
        toCpy = gTrainerClasses[GetEreaderTrainerClassId()].name;
    else if (trainerId == TRAINER_LINK_OPPONENT)
        toCpy = gTrainerClasses[TRAINER_NONE].name;
    else
        toCpy = gTrainerClasses[GetTrainerClassFromId(trainerId)].name;

    return toCpy;
}

// Ensure the defined length for an item name can contain the full defined length of a berry name.
// This ensures that custom Enigma Berry names will fit in the text buffer at the top of BattleStringExpandPlaceholders.
STATIC_ASSERT(BERRY_NAME_LENGTH + ARRAY_COUNT(sText_BerrySuffix) <= ITEM_NAME_LENGTH, BerryNameTooLong);

u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst, u32 dstSize)
{
    src = QoL_LangStr(src); // [QoL] Japanese text in Japanese mode
    u32 dstID = 0; // if they used dstID, why not use srcID as well?
    const u8 *toCpy = NULL;
    u8 text[max(max(max(32, TRAINER_NAME_LENGTH + 1), POKEMON_NAME_LENGTH + 1), ITEM_NAME_LENGTH)];
    u8 *textStart = &text[0];
    u8 multiplayerId;
    u8 fontId = FONT_NORMAL;

    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
        multiplayerId = gRecordedBattleMultiplayerId;
    else
        multiplayerId = GetMultiplayerId();

    // Clear destination first
    while (dstID < dstSize)
    {
        dst[dstID] = EOS;
        dstID++;
    }

    dstID = 0;
    while (*src != EOS)
    {
        toCpy = NULL;

        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            u32 classLength = 0;
            u32 nameLength = 0;
            const u8 *classString;
            const u8 *nameString;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                {
                    toCpy = gBattleTextBuff2;
                }
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                {
                    toCpy = gBattleTextBuff3;
                }
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // Unused, to change into sth else.
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                GetBattlerNick(GetPartnerBattler(gBattlerAttacker), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget)
                break;
            case B_TXT_DEF_NAME: // target name
                GetBattlerNick(gBattlerTarget, text);
                toCpy = text;
                break;
            case B_TXT_DEF_PARTNER_NAME: // partner target name
                GetBattlerNick(GetPartnerBattler(gBattlerTarget), text);
                toCpy = text;
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler)
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->currentMove);
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (gBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->originallyUsedMove);
                break;
            case B_TXT_LAST_ITEM: // last used item
                if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
                {
                    if (gLastUsedItem == ITEM_ENIGMA_BERRY_E_READER)
                    {
                        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        {
                            if ((gBattleScripting.multiplayerId != 0 && (gPotentialItemEffectBattler & BIT_SIDE))
                                || (gBattleScripting.multiplayerId == 0 && !(gPotentialItemEffectBattler & BIT_SIDE)))
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                        else
                        {
                            if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                    }
                    else
                    {
                        CopyItemName(gLastUsedItem, text);
                        toCpy = text;
                    }
                }
                else
                {
                    CopyItemName(gLastUsedItem, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = gAbilitiesInfo[gLastUsedAbility].name;
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerAttacker]].name;
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerTarget]].name;
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattleScripting.battler]].name;
                break;
            case B_TXT_EFF_ABILITY: // effect battler ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gEffectBattler]].name;
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                break;
            case B_TXT_TRAINER1_NAME_WITH_CLASS: // trainer1 name with trainer class
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(GetPartnerPosition(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(GetBattlerLeftFoe(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(GetBattlerRightFoe(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_26: // ?
                if (!IsOnPlayerSide(gBattleScripting.battler))
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        toCpy = sText_FoePkmnPrefix;
                    else
                        toCpy = sText_WildPkmnPrefix;
                    while (*toCpy != EOS)
                    {
                        dst[dstID] = *toCpy;
                        dstID++;
                        toCpy++;
                    }
                }
                GetMonData(&GetBattlerParty(gBattleScripting.battler)[gBattleStruct->scriptPartyIdx], MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_PC_LANETTE))
                    toCpy = IS_FRLG ? sText_Bills : sText_Lanettes;
                else
                    toCpy = sText_Someones;
                break;
            case B_TXT_ATK_PREFIX2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_DEF_PREFIX2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_ATK_PREFIX1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_DEF_PREFIX1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_ATK_PREFIX3:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_DEF_PREFIX3:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_TRAINER2_CLASS:
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                break;
            case B_TXT_TRAINER2_NAME:
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                break;
            case B_TXT_TRAINER2_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerBLoseText();
                }
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                break;
            case B_TXT_PARTNER_CLASS:
                toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                break;
            case B_TXT_PARTNER_NAME:
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                break;
            case B_TXT_PARTNER_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetPlayerName(textStart, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME:
                toCpy = BattleStringGetTrainerName(text, multiplayerId, gBattlerAttacker);
                break;
            case B_TXT_ATK_TRAINER_CLASS:
                switch (GetBattlerPosition(gBattlerAttacker))
                {
                case B_POSITION_PLAYER_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                        toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                    break;
                case B_POSITION_OPPONENT_LEFT:
                    toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                case B_POSITION_OPPONENT_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                    else
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                default:
                    break;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME_WITH_CLASS:
                toCpy = textStart;
                if (gBattleTypeFlags & BATTLE_TYPE_CATCH_TUTORIAL)
                {
                    if (IS_FRLG)
                        textStart = StringCopy(textStart, COMPOUND_STRING("The old man${JPN}{JPN}おじいさん"));
                    else
                        textStart = StringCopy(textStart, COMPOUND_STRING("WALLY"));
                }
                else if (GetBattlerPosition(gBattlerAttacker) == B_POSITION_PLAYER_LEFT)
                {
                    textStart = StringCopy(textStart, BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker));
                }
                else
                {
                    classString = NULL;
                    switch (GetBattlerPosition(gBattlerAttacker))
                    {
                    case B_POSITION_PLAYER_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                            classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                        break;
                    case B_POSITION_OPPONENT_LEFT:
                        classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    case B_POSITION_OPPONENT_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                        else
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    default:
                        break;
                    }
                    classLength = 0;
                    nameLength = 0;
                    while (classString[classLength] != EOS)
                    {
                        textStart[classLength] = classString[classLength];
                        classLength++;
                    }
                    textStart[classLength] = CHAR_SPACE;
                    textStart += 1 + classLength;
                    nameString = BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker);
                    if (nameString != textStart)
                    {
                        while (nameString[nameLength] != EOS)
                        {
                            textStart[nameLength] = nameString[nameLength];
                            nameLength++;
                        }
                        textStart[nameLength] = EOS;
                    }
                }
                break;
            case B_TXT_ATK_TEAM1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_ATK_TEAM2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_DEF_TEAM1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_DEF_TEAM2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_EFF_TEAM1:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_EFF_TEAM2:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerTarget)
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattleScripting.battler)
                break;
            }

            if (toCpy != NULL)
            {
                toCpy = QoL_LangStr(toCpy); // [QoL] Japanese prefixes, team words...
                while (*toCpy != EOS)
                {
                    // [QoL] NBSP is a kana in the Japanese font: keep real spaces there.
                    if (*toCpy == CHAR_SPACE && !QoL_IsJapanese())
                        dst[dstID] = CHAR_NBSP;
                    else
                        dst[dstID] = *toCpy;
                    dstID++;
                    toCpy++;
                }
            }

            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER2_LOSE_TEXT
                || *src == B_TXT_TRAINER1_WIN_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstID] = EXT_CTRL_CODE_BEGIN;
                dstID++;
                dst[dstID] = EXT_CTRL_CODE_PAUSE_UNTIL_PRESS;
                dstID++;
            }
        }
        else
        {
            dst[dstID] = *src;
            dstID++;
        }
        src++;
    }

    dst[dstID] = *src;
    dstID++;

    BreakStringAutomatic(dst, BATTLE_MSG_MAX_WIDTH, BATTLE_MSG_MAX_LINES, fontId, SHOW_SCROLL_PROMPT);

    return dstID;
}

static void IllusionNickHack(enum BattlerId battler, u32 partyId, u8 *dst)
{
    u32 id = PARTY_SIZE;
    struct Pokemon *party = GetBattlerParty(battler);
    struct Pokemon *mon = &party[partyId], *partnerMon;

    if (GetMonAbility(mon) == ABILITY_ILLUSION)
    {
        if (IsBattlerAlive(GetPartnerBattler(battler)))
            partnerMon = GetBattlerMon(GetPartnerBattler(battler));
        else
            partnerMon = mon;

        id = GetIllusionMonPartyId(party, mon, partnerMon, battler);
    }

    if (id != PARTY_SIZE)
        GetMonData(&party[id], MON_DATA_NICKNAME, dst);
    else
        GetMonData(mon, MON_DATA_NICKNAME, dst);
}

void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcID = 1;
    u32 value = 0;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u16 hword;

    *dst = EOS;
    while (src[srcID] != B_BUFF_EOS)
    {
        switch (src[srcID])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcID + 1]);
            StringAppend(dst, gBattleStringsTable[hword]);
            srcID += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcID + 1])
            {
            case 1:
                value = src[srcID + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcID + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcID + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcID + 2]);
            srcID += src[srcID + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            StringAppend(dst, GetMoveName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_TYPE: // type name
            StringAppend(dst, gTypesInfo[src[srcID + 1]].name);
            srcID += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
        case B_BUFF_MON_NICK_WITH_PREFIX_LOWER: // poke nick with lowercase prefix
            if (!IsOnPlayerSide(src[srcID + 1]))
            {
                if (src[srcID] == B_BUFF_MON_NICK_WITH_PREFIX_LOWER)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefixLower);
                    else
                        StringAppend(dst, sText_WildPkmnPrefixLower);
                }
                else
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefix);
                    else
                        StringAppend(dst, sText_WildPkmnPrefix);
                }
            }
            GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, nickname);
            StringGet_Nickname(nickname);
            StringAppend(dst, nickname);
            srcID += 3;
            break;
        case B_BUFF_STAT: // stats
            StringAppend(dst, gStatNamesTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_SPECIES: // species name
            StringCopy(dst, GetSpeciesName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (src[srcID + 2] == gBattlerPartyIndexes[src[srcID + 1]])
            {
                GetBattlerNick(src[srcID + 1], dst);
            }
            else if (gBattleScripting.illusionNickHack) // for STRINGID_ENEMYABOUTTOSWITCHPKMN
            {
                gBattleScripting.illusionNickHack = 0;
                IllusionNickHack(src[srcID + 1], src[srcID + 2], dst);
                StringGet_Nickname(dst);
            }
            else
            {
                GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, dst);
                StringGet_Nickname(dst);
            }
            srcID += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            StringAppend(dst, gAbilitiesInfo[T1_READ_16(&src[srcID + 1])].name);
            srcID += 3;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcID + 1]);
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (hword == ITEM_ENIGMA_BERRY_E_READER)
                {
                    if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                    {
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringAppend(dst, sText_BerrySuffix);
                    }
                    else
                    {
                        StringAppend(dst, sText_EnigmaBerry);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcID += 3;
            break;
        }
    }
}

void BattlePutTextOnWindow(const u8 *text, u8 windowId)
{
    const struct BattleWindowText *textInfo = sBattleTextOnWindowsInfo[gBattleScripting.windowsType];
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;

    if (windowId & B_WIN_COPYTOVRAM)
    {
        windowId &= ~B_WIN_COPYTOVRAM;
        copyToVram = FALSE;
    }
    else
    {
        FillWindowPixelBuffer(windowId, textInfo[windowId].fillValue);
        copyToVram = TRUE;
    }

    printerTemplate.currentChar = text;
    printerTemplate.type = WINDOW_TEXT_PRINTER;
    printerTemplate.windowId = windowId;
    printerTemplate.fontId = textInfo[windowId].fontId;
    printerTemplate.x = textInfo[windowId].x;
    printerTemplate.y = textInfo[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = textInfo[windowId].letterSpacing;
    printerTemplate.lineSpacing = textInfo[windowId].lineSpacing;
    printerTemplate.color = textInfo[windowId].color;

    if (B_WIN_MOVE_NAME_1 <= windowId && windowId <= B_WIN_MOVE_NAME_4)
    {
        // We cannot check the actual width of the window because
        // B_WIN_MOVE_NAME_1 and B_WIN_MOVE_NAME_3 are 16 wide for
        // Z-move details.
        if (gBattleStruct->zmove.viewing && windowId == B_WIN_MOVE_NAME_1)
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 16 * TILE_WIDTH);
        else
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 8 * TILE_WIDTH);
    }

    if (printerTemplate.x == 0xFF)
    {
        u32 width = GetBattleWindowTemplatePixelWidth(gBattleScripting.windowsType, windowId);
        s32 alignX = GetStringCenterAlignXOffsetWithLetterSpacing(printerTemplate.fontId, printerTemplate.currentChar, width, printerTemplate.letterSpacing);
        printerTemplate.x = printerTemplate.currentX = alignX;
    }

    if (windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)) || gTestRunnerEnabled || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != B_WIN_OAK_OLD_MAN))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == B_WIN_MSG || windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
    {
        if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            speed = 1;
        else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            speed = sRecordedBattleTextSpeeds[GetTextSpeedInRecordedBattle()];
        else
            speed = GetPlayerTextSpeedDelay();

        gTextFlags.canABSpeedUpPrint = 1;
    }
    else
    {
        speed = textInfo[windowId].speed;
        gTextFlags.canABSpeedUpPrint = 0;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);

    if (copyToVram)
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
    }
}

void SetPPNumbersPaletteInMoveSelection(enum BattlerId battler)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct *)(&gBattleResources->bufferA[battler][4]);
    const u16 *palPtr = gPPTextPalette;
    u8 var;

    if (!gBattleStruct->zmove.viewing)
        var = GetCurrentPPToMaxPPState(chooseMoveStruct->currentPP[gMoveSelectionCursor[battler]],
                         chooseMoveStruct->maxPP[gMoveSelectionCursor[battler]]);
    else
        var = 3;

    gPlttBufferUnfaded[BG_PLTT_ID(5) + 12] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[BG_PLTT_ID(5) + 11] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 12], &gPlttBufferFaded[BG_PLTT_ID(5) + 12], PLTT_SIZEOF(1));
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 11], &gPlttBufferFaded[BG_PLTT_ID(5) + 11], PLTT_SIZEOF(1));
}

u8 GetCurrentPPToMaxPPState(u8 currentPP, u8 maxPP)
{
    if (maxPP == currentPP)
    {
        return 3;
    }
    else if (maxPP <= 2)
    {
        if (currentPP > 1)
            return 3;
        else
            return 2 - currentPP;
    }
    else if (maxPP <= 7)
    {
        if (currentPP > 2)
            return 3;
        else
            return 2 - currentPP;
    }
    else
    {
        if (currentPP == 0)
            return 2;
        if (currentPP <= maxPP / 4)
            return 1;
        if (currentPP > maxPP / 2)
            return 3;
    }

    return 0;
}
