#ifndef GOBLIN_TANK_H
#define GOBLIN_TANK_H

#include "../librays/raylib.h"
#include "../player/player.h"

#define GOBLIN_TANK_MAP 0

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

typedef struct Player player;

typedef struct goblinTank
{
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
    float goblinTankAttackRange;
    float goblinTankAttackRangeRight;

    float attackTime;
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