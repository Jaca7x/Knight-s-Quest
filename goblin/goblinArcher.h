#ifndef GOBLINARCHER_H
#define GOBLINARCHER_H

#include "../librays/raylib.h"
#include "../player/player.h"

#define MAP_GOBLIN_ARCHER_AREA 2

typedef struct player Player; // declaração antecipada

typedef struct goblinArcher
{
    Vector2 position;

    int life;

    float speed;

    int currentFrame;
    int frameCounter;

    Texture2D spriteWalkGoblinArcher;
    Texture2D spriteAtkGoblinArcher;
    Texture2D spriteIdleGoblinArcher;
    Texture2D spriteDeadGoblinArcher;
    Texture2D spriteHurtGoblinArcher;

    int frameWidth;
    int frameHeight;

    int frameWalk;
    int frameAtk;
    int frameIdle;
    int frameDead;
    int frameHurt;

    int direction;
    bool isWalking;
    bool isRunning;
    bool isIdle;
    bool goblinHasHit;
    bool isDead;
    bool isAtacking;

    float deathAnimTimer;
    bool deathAnimationDone;

    bool hasRunAttack;
    float attackRange;
    float attackCooldown;
    bool hasHitPlayer;
    float attackDamageTimer;
} GoblinArcher;

void InitGoblinArcher(GoblinArcher *goblinArcher);
void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta);
void DrawGoblinArcher(GoblinArcher *goblinArcher);    
void UnloadGoblinArcher(GoblinArcher *goblinArcher);

#endif // goblinArcher_H