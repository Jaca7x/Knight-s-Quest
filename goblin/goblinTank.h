#ifndef GOBLIN_TANK_H
#define GOBLIN_TANK_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"

#define GOBLIN_TANK_MAP 5

#define GOBLIN_TANK_OFFSET_Y 10
#define GOBLIN_TANK_HITBOX_WITH_DIV 11
#define GOBLIN_TANK_HITBOX_HEIGHT_DIV 6

#define GOBLIN_TANK_HURTBOX_OFFSET_X  115

// Goblin attack offsets - LEFT
#define GOBLIN_ATTACK_LEFT_IDLE       125
#define GOBLIN_ATTACK_LEFT_STRIKE     190

// Goblin attack offsets - RIGHT
#define GOBLIN_ATTACK_RIGHT_IDLE      65
#define GOBLIN_ATTACK_RIGHT_STRIKE    0

#define COOL_DOWN_ATTACK 1.0f
#define FRAME_TO_DAMAGE 8
#define FRAME_TO_SOUND_ATTACK 5

#define LIFE_ZERO 0
#define COOL_DOWN_ZERO 0.0f
#define FRAME_COUNTER_ZERO 0
#define CURRENT_FRAME_ZERO 0

#define GOBLIN_TANK_FRAME_DELAY 30
#define GOBLIN_TANK_ATTACK_FRAME_DELAY 10

#define NEXT_FRAME 1
#define PREVIOUS_FRAME 1

#define DEATH_ANIM_FRAME_TIME 0.2f
#define FRAME_OF_DEATH 7

#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1

#define ATTACK_HITBOX_START_FRAME 3

#define SPRITE_ROW_BASE 0

#define HURT_OFFSET_HURT_Y -15
#define HURT_OFFSET_DEAD_Y -5
#define HURT_OFFSET_ATTACK_Y -55
#define HURT_OFFSET_WALKING_Y -10
#define HURT_OFFSET_IDLE_Y -15

typedef struct Player player;

typedef struct goblinTank
{
    Entity entity;

    Vector2 position;

    float speed;
    int life;
    float maxLife;

    Texture2D goblinTankSpriteWalk;
    Texture2D goblinTankSpriteHurt;
    Texture2D goblinTankSpriteDead;
    Texture2D goblinTankSpriteIdle;
    Texture2D goblinTankSpriteAtk;

    int scale;
    int scaleIdle;

    int frameWalk;
    int frameHurt;
    int frameDead;
    int frameIdle;
    int frameAtk;

    int currentFrame;      
    int frameCounter;      
    
    int frameWidthWalk;       
    int frameHeightWalk;     

    int frameWidthIdle;       
    int frameHeightIdle;     
    
    int frameWidthAttack;       
    int frameHeightAttack;     

    int frameWidthDead;       
    int frameHeightDead;   

    int frameWidthHurt;       
    int frameHeightHurt;     

    int damage;

    bool isIdle;
    bool isWalking;
    bool isAttacking;
    bool isDead;
    bool goblinTankHasHurt;
    bool goblinTankHasHit;
    bool deathAnimationDone;
    bool growlSoundPlay;
    bool droppedHeart;

    float deathAnimTimer;

    float viewPlayer;

    int direction;
    float distanceToTurn;
    float goblinTankAttackRange;
    float goblinTankAttackRangeRight;

    int push;
    float attackCooldown;
    float attackCooldownTimer;   

    float attackAnimTimer;  
    
    bool hitApplied;

    Sound soundAttackGoblinTank;
    Sound soundGrowlGoblinTank;
    Sound soundHurtGoblinTank;
    Sound soundDeathGolbinTank;

} GoblinTank;


void InitGoblinTank(GoblinTank *goblinTank);
void UpdateGoblinTank(GoblinTank *goblinTank, float deltaTime, Player *player);
void DrawGoblinTank(GoblinTank *goblinTank, Player *player);
void UnloadGoblinTank(GoblinTank *goblinTank);

#endif // GOBLIN_TANK_H