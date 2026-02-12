#ifndef GOBLIN_TANK_H
#define GOBLIN_TANK_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "../src/render/drawMonsters.h"

/* ===================== MAP ===================== */

#define GOBLIN_TANK_MAP 5                // Map index where the Goblin Tank appears

/* ===================== DIRECTION ===================== */

#define DIRECTION_LEFT  -1               // Facing left
#define DIRECTION_RIGHT  1               // Facing right

/* ===================== HITBOX / OFFSETS ===================== */

#define GOBLIN_TANK_OFFSET_Y              10   // Vertical offset for hitbox alignment
#define GOBLIN_TANK_HITBOX_WITH_DIV       11   // Width divisor for hitbox scaling
#define GOBLIN_TANK_HITBOX_HEIGHT_DIV     6    // Height divisor for hitbox scaling
#define GOBLIN_TANK_HURTBOX_OFFSET_X      115  // Horizontal offset for hurtbox

#define GOBLIN_ATTACK_LEFT_IDLE           125  // Attack hitbox offset (left, idle)
#define GOBLIN_ATTACK_LEFT_STRIKE         190  // Attack hitbox offset (left, strike frame)

#define GOBLIN_ATTACK_RIGHT_IDLE          65   // Attack hitbox offset (right, idle)
#define GOBLIN_ATTACK_RIGHT_STRIKE        0    // Attack hitbox offset (right, strike frame)

#define HURT_OFFSET_HURT_Y               -15   // Sprite offset while hurt
#define HURT_OFFSET_DEAD_Y                -5   // Sprite offset while dead
#define HURT_OFFSET_ATTACK_Y             -55   // Sprite offset while attacking
#define HURT_OFFSET_WALKING_Y            -10   // Sprite offset while walking
#define HURT_OFFSET_IDLE_Y               -15   // Sprite offset while idle

#define ATTACK_HITBOX_START_FRAME         3    // Frame where attack hitbox becomes active
#define SPRITE_ROW_BASE                   0    // Base row in sprite sheet

/* ===================== FRAME / ANIMATION ===================== */

#define GOBLIN_TANK_FRAME_DELAY           30   // Default animation frame delay
#define GOBLIN_TANK_FRAME_DELAY_HURT      60   // Hurt animation frame delay
#define GOBLIN_TANK_ATTACK_FRAME_DELAY    10   // Attack animation frame delay

#define FRAME_TO_DAMAGE                   8    // Frame that applies damage
#define FRAME_TO_SOUND_ATTACK             5    // Frame that plays attack sound
#define FRAME_OF_DEATH                    7    // Total frames in death animation

#define NEXT_FRAME                        1    // Frame increment step
#define PREVIOUS_FRAME                    1    // Used to clamp last frame

#define CURRENT_FRAME_ZERO                0    // Initial frame value
#define FRAME_COUNTER_ZERO                0    // Initial frame counter value

#define DEATH_ANIM_FRAME_TIME             0.2f // Time per death animation frame

/* ===================== COMBAT / COOLDOWN ===================== */

#define COOL_DOWN_ATTACK                  1.0f // Attack cooldown duration
#define COOL_DOWN_ZERO                    0.0f // Zero value for cooldown checks
#define LIFE_ZERO                         0    // Zero life threshold

/* ===================== STRUCT ===================== */

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
