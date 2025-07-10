#ifndef GOBLINTANK_H
#define GOBLINTANK_H

#include "../librays/raylib.h"
#include "../player/player.h"

#define MAP_GOBLIN_TANK_AREA 2

typedef struct player Player; // declaração antecipada

typedef struct goblinTank
{
    Vector2 position;

    int life;

    float speed;

    int currentFrame;
    int frameCounter;

    Texture2D spriteWalkGoblinTank;
    Texture2D spriteAtkGoblinTank;
    Texture2D spriteIdleGoblinTank;
    Texture2D spriteDeadGoblinTank;
    Texture2D spriteHurtGoblinTank;

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
} GoblinTank;

void InitGoblinTank(GoblinTank *goblinTank);
void UpdateGoblinTank(GoblinTank *goblinTank, Player *player, float delta);
void DrawGoblinTank(GoblinTank *goblinTank);    
void UnloadGoblinTank(GoblinTank *goblinTank);

#endif // GOBLINTANK_H