#ifndef GOBLIN_TANK_H
#define GOBLIN_TANK_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "../src/render/drawMonsters.h"
#include "../src/define/define.h"

/* ===================== MAP ===================== */

#define GOBLIN_TANK_MAP                                5                // Map index where the Goblin Tank appears

/* ===================== HITBOX / OFFSETS ===================== */

#define GOBLIN_TANK_OFFSET_Y                          10   // Vertical offset for hitbox alignment
#define GOBLIN_TANK_HITBOX_WITH_DIV                   11   // Width divisor for hitbox scaling
#define GOBLIN_TANK_HITBOX_HEIGHT_DIV                 6    // Height divisor for hitbox scaling
#define GOBLIN_TANK_HURTBOX_OFFSET_X                  115  // Horizontal offset for hurtbox

#define GOBLIN_ATTACK_LEFT_IDLE                       125  // Attack hitbox offset (left, idle)
#define GOBLIN_ATTACK_LEFT_STRIKE                     190  // Attack hitbox offset (left, strike frame)

#define GOBLIN_ATTACK_RIGHT_IDLE                      65   // Attack hitbox offset (right, idle)
#define GOBLIN_ATTACK_RIGHT_STRIKE                    0    // Attack hitbox offset (right, strike frame)

#define HURT_GOBLIN_TANK_OFFSET_HURT_Y               -15   // Sprite offset while hurt
#define HURT_GOBLIN_TANK_OFFSET_DEAD_Y                -5   // Sprite offset while dead
#define HURT_GOBLIN_TANK_OFFSET_ATTACK_Y             -55   // Sprite offset while attacking
#define HURT_GOBLIN_TANK_OFFSET_WALKING_Y            -10   // Sprite offset while walking
#define HURT_GOBLIN_TANK_OFFSET_IDLE_Y               -15   // Sprite offset while idle

#define ATTACK_HITBOX_START_FRAME_GOBLIN_TANK         3    // Frame where attack hitbox becomes active

#define OFFSET_BAR_LIFE_X_GOBLIN_TANK                 140  // Bar life offset X
#define OFFSET_BAR_LIFE_Y_GOBLIN_TANK                 15   // Bar life offset Y

/* ===================== FRAME / ANIMATION ===================== */

#define GOBLIN_TANK_FRAME_DELAY           30   // Default animation frame delay
#define GOBLIN_TANK_FRAME_DELAY_HURT      60   // Hurt animation frame delay
#define GOBLIN_TANK_ATTACK_FRAME_DELAY    10   // Attack animation frame delay

#define FRAME_TO_DAMAGE_GOBLIN_TANK                  8    // Frame that applies damage
#define FRAME_TO_SOUND_ATTACK_GOBLIN_TANK            5    // Frame that plays attack sound
#define FRAME_OF_DEATH_GOBLIN_TANK                   7    // Total frames in death animation

typedef struct Player player;

typedef struct goblinTank
{
    Entity entity;  // Used for life bar rendering
    Monsters base;  // Base monster data used for rendering

    float speed;
    float viewPlayer;
    float distanceToTurn;

    float goblinTankAttackRange;
    float goblinTankAttackRangeRight;

    int life;
    float maxLife;
    int damage;
    int push;

    int frameWalk;
    int frameHurt;
    int frameDead;
    int frameIdle;
    int frameAtk;

    int frameCounter;

    float atackCooldown;
    float atackCooldownTimer;
    float atackAnimTimer;

    float deathAnimTimer;
    float hurtTimer;
    float hurtDuration;

    bool hitPlayer;
    bool hitApplied;
    bool deathAnimationDone;
    bool growlSoundPlay;
    bool droppedHeart;

    Sound soundAttackGoblinTank;
    Sound soundGrowlGoblinTank;
    Sound soundHurtGoblinTank;
    Sound soundDeathGolbinTank;

} GoblinTank;

/* ===================== FUNCTIONS ===================== */

void InitGoblinTank(GoblinTank *goblinTank);
void UpdateGoblinTank(GoblinTank *goblinTank, float deltaTime, Player *player);
void DrawGoblinTank(GoblinTank *goblinTank, Player *player);
void UnloadGoblinTank(GoblinTank *goblinTank);

#endif
