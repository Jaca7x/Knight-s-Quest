#ifndef WOLFRUN_H
#define WOLFRUN_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "math.h"

#define MAP_WOLF_AREA 1

typedef struct player Player; // <- Declaração antecipada

typedef struct wolfRun
{
    Vector2 position;
    Vector2 start;
    Vector2 end;

    int life;

    float speed;
    float speedRun;

    int currentFrame;
    int frameCounter;

    Texture2D spriteWalkWolf;
    Texture2D spriteAtkWolf;
    Texture2D spriteIdleWolf;
    Texture2D spriteDeadWolf;
    Texture2D spriteRunWolf;
    Texture2D spriteHurtWolf;

    
    int frameWidth;
    int frameHeight;

    int frameWalk;
    int frameAtk;
    int frameIdle;
    int frameDead;
    int frameRun;
    int frameHurt;

    int direction;
    bool isWalking;
    bool isRunning;
    bool isIdle;
    bool wolfHasHit;
    bool isDead;

    float deathAnimTimer;
    bool deathAnimationDone;

    bool isAttacking;
    bool hasRunAttack;
    float attackRange;
    float attackCooldown;
    bool hasHitPlayer;
    float attackDamageTimer;
    int damage;
    float viewPlayer;

} WolfRun;


void InitRunningWolf(WolfRun *wolfRun);
void UpdateRunningWolf(WolfRun *wolfRun, Player *player, float delta);
void DrawRunningWolf(WolfRun *wolfRun);
void UnloadRunningWolf(WolfRun *wolfRun);

#endif // WOLFRUN_H