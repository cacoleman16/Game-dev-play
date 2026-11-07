#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_player_avatar.h"
#include "follower_pokemon.h"
#include "link.h"
#include "overworld.h"
#include "pokemon.h"
#include "quest_log.h"
#include "string_util.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/quest_log.h"
#include "constants/species.h"
#include "constants/trainer_types.h"

// [QoL] Following Pokemon.
//
// The first non-egg, non-fainted party member is shown as an object event
// (local ID OBJ_EVENT_ID_FOLLOWER) that walks one tile behind the player.
//
//  * FollowerPokemon_Update() runs once per overworld frame. It spawns,
//    removes or re-skins the follower so it always matches the party (lead
//    swapped in the party menu, lead fainted in battle or from poison, first
//    Pokemon received, ...).
//  * Every time the player's tile changes, the tile the player just left is
//    queued. The follower walks to queued tiles at the player's speed
//    (walk/run/stairs/ice/currents) or ledge-jumps when a ledge separates
//    them. Walking toward the player keeps its facing direction in sync.
//  * Situations the queue can't express (warps, fly, cutscene teleports,
//    surfing, biking, an invisible player...) "resync" the follower: it is
//    hidden on the player's tile and pops back out on the player's next step.
//  * Vanilla FireRed only has overworld sprites for a few dozen species
//    (sFollowerGraphics). Everything else follows as a Poke Ball; add rows to
//    sFollowerGraphics once you import more overworld sprites.
//  * Setting FLAG_SYS_HIDE_FOLLOWER (e.g. from a cutscene script) removes the
//    follower until the flag is cleared.

#define FOLLOWER_QUEUE_SIZE 4

enum {
    FOLLOWER_SPEED_SLOWER,   // 32 frames per tile
    FOLLOWER_SPEED_SLOW,     // 24 frames per tile (rock stairs)
    FOLLOWER_SPEED_NORMAL,   // 16 frames per tile (walking)
    FOLLOWER_SPEED_FAST,     //  8 frames per tile (running)
    FOLLOWER_SPEED_CURRENT,  // water currents
    FOLLOWER_SPEED_FASTER,   //  4 frames per tile
    FOLLOWER_SPEED_SLIDE,    // ice
};

struct FollowerStep
{
    s16 x;
    s16 y;
    u8 speed;
};

struct FollowerState
{
    struct FollowerStep queue[FOLLOWER_QUEUE_SIZE];
    s16 lastPlayerX;
    s16 lastPlayerY;
    u16 species;    // species the follower currently represents
    u8 queueStart;
    u8 queueCount;
};

struct FollowerGraphics
{
    u16 species;
    u8 graphicsId;
};

static EWRAM_DATA struct FollowerState sFollower = {0};

// Species that have a walking overworld sprite in vanilla FireRed.
// Ho-Oh is left out on purpose: its sprite uses a special animation table
// without walking frames.
static const struct FollowerGraphics sFollowerGraphics[] =
{
    { SPECIES_PIDGEY,     OBJ_EVENT_GFX_PIDGEY },
    { SPECIES_PIDGEOT,    OBJ_EVENT_GFX_PIDGEOT },
    { SPECIES_SPEAROW,    OBJ_EVENT_GFX_SPEAROW },
    { SPECIES_FEAROW,     OBJ_EVENT_GFX_FEAROW },
    { SPECIES_PIKACHU,    OBJ_EVENT_GFX_PIKACHU },
    { SPECIES_NIDORAN_F,  OBJ_EVENT_GFX_NIDORAN_F },
    { SPECIES_NIDORINO,   OBJ_EVENT_GFX_NIDORINO },
    { SPECIES_CLEFAIRY,   OBJ_EVENT_GFX_CLEFAIRY },
    { SPECIES_JIGGLYPUFF, OBJ_EVENT_GFX_JIGGLYPUFF },
    { SPECIES_WIGGLYTUFF, OBJ_EVENT_GFX_WIGGLYTUFF },
    { SPECIES_MEOWTH,     OBJ_EVENT_GFX_MEOWTH },
    { SPECIES_PSYDUCK,    OBJ_EVENT_GFX_PSYDUCK },
    { SPECIES_POLIWRATH,  OBJ_EVENT_GFX_POLIWRATH },
    { SPECIES_MACHOP,     OBJ_EVENT_GFX_MACHOP },
    { SPECIES_MACHOKE,    OBJ_EVENT_GFX_MACHOKE },
    { SPECIES_SLOWPOKE,   OBJ_EVENT_GFX_SLOWPOKE },
    { SPECIES_SLOWBRO,    OBJ_EVENT_GFX_SLOWBRO },
    { SPECIES_DODUO,      OBJ_EVENT_GFX_DODUO },
    { SPECIES_SEEL,       OBJ_EVENT_GFX_SEEL },
    { SPECIES_VOLTORB,    OBJ_EVENT_GFX_VOLTORB },
    { SPECIES_CUBONE,     OBJ_EVENT_GFX_CUBONE },
    { SPECIES_CHANSEY,    OBJ_EVENT_GFX_CHANSEY },
    { SPECIES_KANGASKHAN, OBJ_EVENT_GFX_KANGASKHAN },
    { SPECIES_LAPRAS,     OBJ_EVENT_GFX_LAPRAS },
    { SPECIES_OMANYTE,    OBJ_EVENT_GFX_OMANYTE },
    { SPECIES_KABUTO,     OBJ_EVENT_GFX_KABUTO },
    { SPECIES_SNORLAX,    OBJ_EVENT_GFX_SNORLAX },
    { SPECIES_ARTICUNO,   OBJ_EVENT_GFX_ARTICUNO },
    { SPECIES_ZAPDOS,     OBJ_EVENT_GFX_ZAPDOS },
    { SPECIES_MOLTRES,    OBJ_EVENT_GFX_MOLTRES },
    { SPECIES_MEWTWO,     OBJ_EVENT_GFX_MEWTWO },
    { SPECIES_MEW,        OBJ_EVENT_GFX_MEW },
    { SPECIES_RAIKOU,     OBJ_EVENT_GFX_RAIKOU },
    { SPECIES_ENTEI,      OBJ_EVENT_GFX_ENTEI },
    { SPECIES_SUICUNE,    OBJ_EVENT_GFX_SUICUNE },
    { SPECIES_LUGIA,      OBJ_EVENT_GFX_LUGIA },
    { SPECIES_CELEBI,     OBJ_EVENT_GFX_CELEBI },
    { SPECIES_DEOXYS,     OBJ_EVENT_GFX_DEOXYS_N },
};

// Fallback for every species not listed above.
#define FOLLOWER_GFX_DEFAULT OBJ_EVENT_GFX_ITEM_BALL

static struct ObjectEvent *GetPlayerObjectEvent(void)
{
    return &gObjectEvents[gPlayerAvatar.objectEventId];
}

bool8 FollowerPokemon_IsFollowerObject(const struct ObjectEvent *objectEvent)
{
    return objectEvent->active && objectEvent->localId == OBJ_EVENT_ID_FOLLOWER;
}

static struct ObjectEvent *GetFollowerObjectEvent(void)
{
    u8 i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (FollowerPokemon_IsFollowerObject(&gObjectEvents[i]))
            return &gObjectEvents[i];
    }
    return NULL;
}

// The lead that should follow: first party member that is not an egg and has HP left.
static u16 GetFollowerSpeciesFromParty(void)
{
    u8 i;
    u16 species;

    if (FlagGet(FLAG_SYS_HIDE_FOLLOWER))
        return SPECIES_NONE;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_HP) == 0)
            continue; // fainted, or an empty slot
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        if (species != SPECIES_NONE && species != SPECIES_EGG)
            return species;
    }
    return SPECIES_NONE;
}

static struct Pokemon *GetFollowerMon(void)
{
    u8 i;
    u16 species;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_HP) == 0)
            continue;
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        if (species != SPECIES_NONE && species != SPECIES_EGG)
            return &gPlayerParty[i];
    }
    return NULL;
}

static u8 GetFollowerGraphicsId(u16 species)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sFollowerGraphics); i++)
    {
        if (sFollowerGraphics[i].species == species)
            return sFollowerGraphics[i].graphicsId;
    }
    return FOLLOWER_GFX_DEFAULT;
}

// FALSE when the follower logic should not touch anything this frame.
static bool8 IsFollowerLogicActive(void)
{
    if (gMain.callback1 != CB1_Overworld) // link rooms, cable club
        return FALSE;
    if (QL_IS_PLAYBACK_STATE || QL_GetPlaybackState() == QL_PLAYBACK_STATE_RUNNING)
        return FALSE;
    if (!gObjectEvents[gPlayerAvatar.objectEventId].active || !gObjectEvents[gPlayerAvatar.objectEventId].isPlayer)
        return FALSE;
    return TRUE;
}

static bool8 IsFollowerAllowedOnMap(void)
{
    if (InUnionRoom() == TRUE)
        return FALSE;
    return TRUE;
}

static void ResetFollowerQueue(void)
{
    sFollower.queueStart = 0;
    sFollower.queueCount = 0;
}

static void SyncFollowerToPlayer(void)
{
    struct ObjectEvent *player = GetPlayerObjectEvent();

    sFollower.lastPlayerX = player->currentCoords.x;
    sFollower.lastPlayerY = player->currentCoords.y;
    ResetFollowerQueue();
}

static bool8 IsFollowerMoving(struct ObjectEvent *follower)
{
    return follower->heldMovementActive && !follower->heldMovementFinished;
}

// Hides the follower on the player's tile; it reappears behind the player on their next step.
static void HideFollowerOnPlayer(struct ObjectEvent *follower)
{
    struct ObjectEvent *player = GetPlayerObjectEvent();

    follower->invisible = TRUE;
    if (follower->currentCoords.x != player->currentCoords.x || follower->currentCoords.y != player->currentCoords.y)
        MoveObjectEventToMapCoords(follower, player->currentCoords.x, player->currentCoords.y); // also clears its held movement
    else
        ObjectEventClearHeldMovementIfActive(follower);
    follower->currentElevation = player->currentElevation;
    follower->previousElevation = player->previousElevation;
    ObjectEventTurn(follower, player->facingDirection);
    SyncFollowerToPlayer();
}

static struct ObjectEvent *SpawnFollower(u16 species)
{
    struct ObjectEvent *player = GetPlayerObjectEvent();
    struct ObjectEvent *follower;
    int objectEventId;

    objectEventId = SpawnSpecialObjectEventParameterized(GetFollowerGraphicsId(species), MOVEMENT_TYPE_NONE, OBJ_EVENT_ID_FOLLOWER,
                                                         player->currentCoords.x, player->currentCoords.y, player->currentElevation);
    if (objectEventId >= OBJECT_EVENTS_COUNT)
        return NULL; // every object event slot on this map is in use

    follower = &gObjectEvents[objectEventId];
    sFollower.species = species;
    HideFollowerOnPlayer(follower);
    return follower;
}

static void RemoveFollower(struct ObjectEvent *follower)
{
    RemoveObjectEventWithoutTemplate(follower);
    sFollower.species = SPECIES_NONE;
    ResetFollowerQueue();
}

static void UpdateFollowerGraphics(struct ObjectEvent *follower, u16 species)
{
    u8 graphicsId = GetFollowerGraphicsId(species);

    if (follower->graphicsId != graphicsId)
    {
        ObjectEventSetGraphicsId(follower, graphicsId);
        ObjectEventTurn(follower, follower->facingDirection);
    }
    sFollower.species = species;
}

static bool8 ShouldHideFollower(struct ObjectEvent *player)
{
    if (player->invisible)
        return TRUE;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING | PLAYER_AVATAR_FLAG_UNDERWATER
                            | PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        return TRUE;
    return FALSE;
}

// Maps the movement the player just started to the follower's step speed.
static u8 GetFollowerSpeedForPlayerAction(u8 action)
{
    if ((action >= MOVEMENT_ACTION_WALK_SLOWER_DOWN && action <= MOVEMENT_ACTION_WALK_SLOWER_RIGHT)
     || (action >= MOVEMENT_ACTION_WALK_SLOWEST_DOWN && action <= MOVEMENT_ACTION_WALK_SLOWEST_RIGHT))
        return FOLLOWER_SPEED_SLOWER;
    if (action >= MOVEMENT_ACTION_WALK_SLOW_DOWN && action <= MOVEMENT_ACTION_WALK_SLOW_RIGHT)
        return FOLLOWER_SPEED_SLOW;
    if ((action >= MOVEMENT_ACTION_WALK_FAST_DOWN && action <= MOVEMENT_ACTION_WALK_FAST_RIGHT)
     || (action >= MOVEMENT_ACTION_PLAYER_RUN_DOWN && action <= MOVEMENT_ACTION_PLAYER_RUN_RIGHT)
     || (action >= MOVEMENT_ACTION_SPIN_DOWN && action <= MOVEMENT_ACTION_SPIN_RIGHT))
        return FOLLOWER_SPEED_FAST;
    if (action >= MOVEMENT_ACTION_RIDE_WATER_CURRENT_DOWN && action <= MOVEMENT_ACTION_RIDE_WATER_CURRENT_RIGHT)
        return FOLLOWER_SPEED_CURRENT;
    if (action >= MOVEMENT_ACTION_WALK_FASTER_DOWN && action <= MOVEMENT_ACTION_WALK_FASTER_RIGHT)
        return FOLLOWER_SPEED_FASTER;
    if (action >= MOVEMENT_ACTION_SLIDE_DOWN && action <= MOVEMENT_ACTION_SLIDE_RIGHT)
        return FOLLOWER_SPEED_SLIDE;
    // Normal walking, rock-stair running, ledge jumps (the follower only walks
    // to the take-off tile), scripted movement, ...
    return FOLLOWER_SPEED_NORMAL;
}

// Only movements that use the standard NPC animations are used here, so any
// overworld sprite with sAnimTable_Standard (or an inanimate one) is safe.
static u8 GetFollowerWalkAction(u8 speed, u8 direction)
{
    switch (speed)
    {
    case FOLLOWER_SPEED_SLOWER:
        return GetWalkSlowerMovementAction(direction);
    case FOLLOWER_SPEED_SLOW:
        return GetWalkSlowMovementAction(direction);
    case FOLLOWER_SPEED_FAST:
        return GetWalkFastMovementAction(direction);
    case FOLLOWER_SPEED_CURRENT:
        return GetRideWaterCurrentMovementAction(direction);
    case FOLLOWER_SPEED_FASTER:
        return GetWalkFasterMovementAction(direction);
    case FOLLOWER_SPEED_SLIDE:
        return GetSlideMovementAction(direction);
    case FOLLOWER_SPEED_NORMAL:
    default:
        return GetWalkNormalMovementAction(direction);
    }
}

static u8 GetDirectionFromDelta(s16 dx, s16 dy)
{
    if (dx > 0)
        return DIR_EAST;
    if (dx < 0)
        return DIR_WEST;
    if (dy > 0)
        return DIR_SOUTH;
    return DIR_NORTH;
}

static void QueueFollowerStep(s16 x, s16 y, u8 speed)
{
    struct FollowerStep *step = &sFollower.queue[(sFollower.queueStart + sFollower.queueCount) % FOLLOWER_QUEUE_SIZE];

    step->x = x;
    step->y = y;
    step->speed = speed;
    sFollower.queueCount++;
}

// Detects a new player step and queues the tile the player just left.
// Returns FALSE if the follower had to be resynced.
static bool8 TrackPlayerSteps(struct ObjectEvent *follower, struct ObjectEvent *player)
{
    s16 dx = player->currentCoords.x - sFollower.lastPlayerX;
    s16 dy = player->currentCoords.y - sFollower.lastPlayerY;
    s16 absX = dx < 0 ? -dx : dx;
    s16 absY = dy < 0 ? -dy : dy;

    if (dx == 0 && dy == 0)
        return TRUE;

    // One tile, or two tiles in a straight line (ledge jump). Anything else is
    // a warp or a scripted teleport.
    if ((absX != 0 && absY != 0) || absX > 2 || absY > 2 || sFollower.queueCount >= FOLLOWER_QUEUE_SIZE)
    {
        HideFollowerOnPlayer(follower);
        return FALSE;
    }

    QueueFollowerStep(sFollower.lastPlayerX, sFollower.lastPlayerY, GetFollowerSpeedForPlayerAction(player->movementActionId));
    sFollower.lastPlayerX = player->currentCoords.x;
    sFollower.lastPlayerY = player->currentCoords.y;
    return TRUE;
}

// When the follower is idle with nothing queued it must be on the player's
// tile (hidden), next to the player, or two tiles away across a ledge.
static void CheckFollowerPosition(struct ObjectEvent *follower, struct ObjectEvent *player)
{
    s16 dx = player->currentCoords.x - follower->currentCoords.x;
    s16 dy = player->currentCoords.y - follower->currentCoords.y;
    s16 absX = dx < 0 ? -dx : dx;
    s16 absY = dy < 0 ? -dy : dy;

    if (follower->invisible)
    {
        if (dx != 0 || dy != 0)
            HideFollowerOnPlayer(follower);
        return;
    }

    if ((absX == 0 && absY == 0) || (absX != 0 && absY != 0) || absX > 2 || absY > 2)
        HideFollowerOnPlayer(follower);
}

static void ProcessFollowerQueue(struct ObjectEvent *follower, struct ObjectEvent *player)
{
    struct FollowerStep *step;
    s16 dx, dy, absX, absY;
    u8 direction;
    u8 action;

    if (IsFollowerMoving(follower))
        return;
    ObjectEventClearHeldMovementIfActive(follower);

    if (sFollower.queueCount == 0)
    {
        CheckFollowerPosition(follower, player);
        return;
    }

    step = &sFollower.queue[sFollower.queueStart];
    sFollower.queueStart = (sFollower.queueStart + 1) % FOLLOWER_QUEUE_SIZE;
    sFollower.queueCount--;

    dx = step->x - follower->currentCoords.x;
    dy = step->y - follower->currentCoords.y;
    absX = dx < 0 ? -dx : dx;
    absY = dy < 0 ? -dy : dy;

    if (dx == 0 && dy == 0)
    {
        // The player stepped off the follower's tile (e.g. right after a
        // warp): just reveal it where it is.
        follower->invisible = FALSE;
        return;
    }

    direction = GetDirectionFromDelta(dx, dy);
    if (absX + absY == 1)
        action = GetFollowerWalkAction(step->speed, direction);
    else if ((absX == 0 || absY == 0) && absX + absY == 2)
        action = GetJump2MovementAction(direction); // hop down the ledge after the player
    else
    {
        HideFollowerOnPlayer(follower);
        return;
    }

    follower->invisible = FALSE;
    ObjectEventSetHeldMovement(follower, action);
}

void FollowerPokemon_Update(void)
{
    struct ObjectEvent *player;
    struct ObjectEvent *follower;
    u16 species;

    if (!IsFollowerLogicActive())
        return;

    follower = GetFollowerObjectEvent();
    species = IsFollowerAllowedOnMap() ? GetFollowerSpeciesFromParty() : SPECIES_NONE;

    if (species == SPECIES_NONE)
    {
        if (follower != NULL)
            RemoveFollower(follower);
        return;
    }

    if (follower == NULL)
    {
        SpawnFollower(species);
        return;
    }

    player = GetPlayerObjectEvent();

    // The lead changed: party reorder, the lead fainted, a new Pokemon...
    if (species != sFollower.species && !IsFollowerMoving(follower))
        UpdateFollowerGraphics(follower, species);

    if (ShouldHideFollower(player))
    {
        if (!follower->invisible || follower->currentCoords.x != player->currentCoords.x || follower->currentCoords.y != player->currentCoords.y)
            HideFollowerOnPlayer(follower);
        SyncFollowerToPlayer();
        return;
    }

    if (TrackPlayerSteps(follower, player))
        ProcessFollowerQueue(follower, player);
}

void FollowerPokemon_OnMapLoad(void)
{
    sFollower.species = SPECIES_NONE;
    ResetFollowerQueue();
    // Spawning is left to FollowerPokemon_Update, which runs on the first
    // overworld frame after the map's own object events have been created.
}

void FollowerPokemon_OnReturnToField(void)
{
    struct ObjectEvent *follower = GetFollowerObjectEvent();

    ResetFollowerQueue();
    if (follower != NULL)
    {
        // The follower's sprite was rebuilt from its saved graphics; make sure
        // the graphics match the current lead on the next update.
        sFollower.species = SPECIES_NONE;
        if (gObjectEvents[gPlayerAvatar.objectEventId].active)
            SyncFollowerToPlayer();
    }
}

void FollowerPokemon_OnCameraShift(s16 dx, s16 dy)
{
    u8 i;

    // Map connection crossed: every object's coordinates were shifted, so
    // shift the remembered player tile and queued tiles too.
    sFollower.lastPlayerX -= dx;
    sFollower.lastPlayerY -= dy;
    for (i = 0; i < FOLLOWER_QUEUE_SIZE; i++)
    {
        sFollower.queue[i].x -= dx;
        sFollower.queue[i].y -= dy;
    }
}

// The follower never blocks the player (so it can be walked through/swapped
// with), trainers' line of sight, or pushed boulders. Wandering NPCs still
// bump into it so they don't overlap it.
bool8 FollowerPokemon_ShouldIgnoreCollision(const struct ObjectEvent *mover, const struct ObjectEvent *other)
{
    if (!FollowerPokemon_IsFollowerObject(other))
        return FALSE;
    if (other->invisible)
        return TRUE;
    if (mover->isPlayer || mover->trainerType != TRAINER_TYPE_NONE || mover->graphicsId == OBJ_EVENT_GFX_PUSHABLE_BOULDER)
        return TRUE;
    return FALSE;
}

// Script to run when the player talks to objectEventId, if it is the follower.
// Returns NULL for any other object, or when the follower is hidden.
const u8 *FollowerPokemon_GetInteractionScript(u8 objectEventId)
{
    if (!FollowerPokemon_IsFollowerObject(&gObjectEvents[objectEventId]))
        return NULL;
    if (gObjectEvents[objectEventId].invisible)
        return NULL;
    return EventScript_FollowerPokemon;
}

// Special for EventScript_FollowerPokemon.
// Out: STR_VAR_1 = nickname, VAR_0x8004 = species (for the cry),
//      VAR_RESULT = FOLLOWER_MOOD_* picked from the Pokemon's friendship.
void FollowerPokemon_BufferInfo(void)
{
    struct Pokemon *mon = GetFollowerMon();
    u8 friendship;

    if (mon == NULL)
    {
        gSpecialVar_0x8004 = SPECIES_NONE;
        gSpecialVar_Result = FOLLOWER_MOOD_NONE;
        return;
    }

    GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
    gSpecialVar_0x8004 = GetMonData(mon, MON_DATA_SPECIES);
    friendship = GetMonData(mon, MON_DATA_FRIENDSHIP);
    if (friendship >= 200)
        gSpecialVar_Result = FOLLOWER_MOOD_OVERJOYED;
    else if (friendship >= 100)
        gSpecialVar_Result = FOLLOWER_MOOD_HAPPY;
    else
        gSpecialVar_Result = FOLLOWER_MOOD_NEUTRAL;
}
