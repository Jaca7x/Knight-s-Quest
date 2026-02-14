#ifndef BOSS_H
#define BOSS_H

#include "raylib.h"
#include "entities/player/player.h"

#define BOSS_MAP 8

typedef struct boss
{
    Vector2 position;

    Texture2D spriteWalk;
    Texture2D spriteHurt;
    Texture2D spriteAtk;
    Texture2D spriteIdle;
    Texture2D spriteDead;

    int frameIdle;
    int frameWalk;
    int frameDead;
    int frameHurt;
    int frameAtk;

    int currentFrame;     
    int frameCounter;    

    int frameWidthAtk;        
    int frameHeightAtk; 
    float attackTime;
    float attackCooldown;
    int hitFrame; 
    bool hasAppliedDamage;


    int frameWidthWalk;
    int frameHeightWalk; 

    int frameWidth;        
    int frameHeight; 

    int frameWidthHurt;
    int frameHeightHurt;

    int frameHeightDead;
    int frameWidthDead;

    float scaleAtk;
    float scaleIdle;
    float scaleWalk;
    float scaleHurt;

    float hurtTimer;
    float hurtDuration;
    
    float direction;

    bool isWalking;
    bool isAtacking;
    bool isIdle;
    bool isDead;

    float attackRange;

    float speed;

    int life;
    bool bossHasHit;

    float viewPlayer;

    int damage;

    Music bossMusic;

    bool musicStarted;  

    Sound bossGrounImpact;

    Sound bossWalkSound;
    Sound bossHurtSound;
    Sound bossDeathSound;
    Sound bossAttackSound;
} Boss;

void InitBoss(Boss *boss);
void UpdateBoss(Boss *boss, Player *player, float delta, bool *bossTriggered);
void DrawBoss(Boss *boss);
void UnloadBoss(Boss *boss);

#endif //BOSS_H
