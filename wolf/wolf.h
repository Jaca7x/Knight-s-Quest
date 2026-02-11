#ifndef WOLF_H
#define WOLF_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>

#define MAP_WOLF_RUNNING_AREA 2
#define MAP_WOLF_WHITE_AREA 4

typedef struct player Player;

typedef struct wolf
{
    Entity entity;
    
    Vector2 position;
    Vector2 start;
    Vector2 end;

    int life;
    int maxLife;

    float speed;

    int currentFrame;
    int frameCounter;

    Texture2D spriteWalkWolf;
    Texture2D spriteAtkWolf;
    Texture2D spriteIdleWolf;
    Texture2D spriteDeadWolf;
    Texture2D spriteHurtWolf;
    
    int frameWidth;
    int frameHeight;

    int frameWalk;
    int frameAtk;
    int frameIdle;
    int frameDead;
    int frameHurt;

    int direction;
    bool isMoving;

    bool isDead;
    float deathAnimTimer;
    bool deathAnimationDone;

    bool isPatrolling;

    bool isAttacking;
    float attackRange;
    float attackCooldown;
    bool hasHitPlayer;
    bool wolfHasHit;
    float attackDamageTimer;
    int damage;

    bool droppedHeart; 

    Sound wolfHitSound;
    Sound wolfHitSoundHeavy;
    Sound wolfDeathSound;
    Sound wolfScratch;

    Sound wolfGrowl;
    bool growlPlayed;
    float timeSinceLastGrowl;
    
} Wolf;

void InitWolfBase(Wolf *wolf, Vector2 pos);

void InitWhiteWolf(Wolf *wolf, Vector2 pos);

void InitRedWolf(Wolf *wolf, Vector2 pos);

void UpdateWolf(Wolf *wolf, Player *player, float delta, int currentMapIndex);
void DrawWolf(Wolf *wolf);
void UnloadWolf(Wolf *wolf);

#endif // WOLF_H