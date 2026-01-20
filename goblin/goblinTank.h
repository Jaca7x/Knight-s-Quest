#ifndef GOBLIN_TANK_H
#define GOBLIN_TANK_H

#include "../librays/raylib.h"
#include "../player/player.h"

#define GOBLIN_TANK_MAP 0

typedef struct Player player;

typedef struct goblinTank
{
    Vector2 position;

    float speed;

    Texture2D goblinTankSpriteWalk;
    Texture2D goblinTankSpriteHurt;
    Texture2D goblinTankSpriteDead;
    Texture2D goblinTankSpriteIdle;
    Texture2D goblinTankSpriteAtk;

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
    bool isAtacking;
    bool isDead;
    bool goblinTankHasHurt;
    bool goblinTankHasHit;
    bool deathAnimationDone;

    float deathAnimTimer;

    float viewPlayer;

    int direction;
    float goblinTankAttackRange;
    float goblinTankAttackRangeLeft;
    float life;

    float attackTime;
    float attackCooldown;
    float attackCooldownTimer;   

    float attackAnimTimer;  
    
    bool hitApplied;

} GoblinTank;


void InitGoblinTank(GoblinTank *goblinTank);
void UpdateGoblinTank(GoblinTank *goblinTank, float deltaTime, Player *player);
void DrawGoblinTank(GoblinTank *goblinTank);
void UnloadGoblinTank(GoblinTank *goblinTank);

#endif // GOBLIN_TANK_H