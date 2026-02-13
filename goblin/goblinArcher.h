#ifndef GOBLIN_ARCHER_H
#define GOBLIN_ARCHER_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "../src/render/drawMonsters.h"
#include "../src/define/define.h"

/* =========================================================
   DEFINES
========================================================= */

#define MAP_GOBLIN_ARCHER_AREA 1

#define ARROW_DAMAGE_ZERO 0
#define FRAME_DEAD_GOBLIN_ARCHER 8
#define FRAME_TO_PLAY_SOUND_LOADING 5
#define ARROW_COOLDOWN_ZERO 0.0f
#define ARROW_COOLDOWN 1.5f
#define ATTACK_ANIM_TIMER_GOBLIN_ARCHER   0.4f
#define ARROW_SCALE 0.2f
#define ARROW_ROTATION 0.0f
#define ARROW_OFFSET_Y 0.05f

#define GOBLIN_ARCHER_BAR_LIFE_OFFSET_X 10
#define GOBLIN_ARCHER_BAR_LIFE_OFFSET_Y 10
/* =========================================================
   STRUCT: Arrow
========================================================= */

typedef struct arrow
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

typedef struct goblinArcher
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
