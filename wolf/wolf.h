#ifndef WOLF_H
#define WOLF_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>

#define MAP_WOLF_AREA 1

typedef struct player Player;

typedef struct wolf
{
    Vector2 position;
    Vector2 start;
    Vector2 end;

    int life;

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

    bool isAttacking;
    float attackRange;
    float attackCooldown;
    bool hasHitPlayer;
    bool wolfHasHit;
    float attackDamageTimer;
    int damage;

} Wolf;

void InitWolf(Wolf *wolf);
void UpdateWolf(Wolf *wolf, Player *player, float delta);
void DrawWolf(Wolf *wolf);
void UnloadWolf(Wolf *wolf);

#endif // WOLF_H