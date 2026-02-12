#ifndef GOBLIN_ARCHER_H
#define GOBLIN_ARCHER_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "../src/render/drawMonsters.h"

/* =========================================================
   DEFINES
========================================================= */

#define MAP_GOBLIN_ARCHER_AREA 1
#define OFFSET_ZERO 0

/* =========================================================
   STRUCT: Arrow
========================================================= */

typedef struct Arrow
{
    Vector2 position;

    float speed;
    float scale;
    float rotation;
    float arrowYOffset;

    int direction;
    bool active;

} Arrow;

/* =========================================================
   STRUCT: GoblinArcher
========================================================= */

typedef struct GoblinArcher
{
    /* ---------- Base ---------- */
    Entity entity;
    Monsters base;

    /* ---------- Movement ---------- */
    float speed;

    /* ---------- Health ---------- */
    int life;
    float maxLife;
    bool droppedHeart;

    /* ---------- Animation ---------- */
    int frameCounter;

    int frameIdle;
    int frameWalk;
    int frameAtk;
    int frameHurt;
    int frameDead;

    float attackAnimTimer;

    bool hasHitPlayer;

    float deathAnimTimer;
    bool deathAnimationDone;

    /* ---------- Attack ---------- */
    Texture2D arrowTexture;
    Arrow arrow;

    float arrowSpeed;
    float arrowCooldown;
    float attackCooldown;
    float attackDamageTimer;

    float goblinView;
    float attackRange;

    int arrowTolerance;
    int arrowDamage;

    bool arrowFired;

    /* ---------- Sounds ---------- */
    Sound goblinArcherDeathSound;
    Sound goblinArcherLoadingSound;
    Sound arrowHitSound;

} GoblinArcher;

/* =========================================================
   FUNCTIONS
========================================================= */

void InitGoblinArcher(GoblinArcher *goblinArcher);
void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta);
void DrawGoblinArcher(GoblinArcher *goblinArcher);
void UnloadGoblinArcher(GoblinArcher *goblinArcher);

#endif // GOBLIN_ARCHER_H
