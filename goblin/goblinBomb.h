#ifndef GOBLINBOMB_H
#define GOBLINBOMB_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "../src/render/drawMonsters.h"

/* =========================
   MAP / GENERAL DEFINES
========================= */

#define OFFSET_ZERO 0
#define MAP_GOBLIN_BOMB 0
#define ROW_BASE 0

/* =========================
   GOBLIN HITBOX / SCALE
========================= */

#define GOBLIN_HITBOX_OFFSET_X 110
#define GOBLIN_HITBOX_OFFSET_Y 130
#define GOBLIN_HITBOX_SCALE    0.5f

#define GOBLIN_SCALE_WIDTH  1.98f
#define GOBLIN_SCALE_HEIGHT 1.87f

/* =========================
   BOMB DEFINES
========================= */

#define BOMB_CENTER_OFFSET_X 5
#define BOMB_CENTER_OFFSET_Y 50
#define BOMB_CENTER_SCALE    1.2f
#define BOMB_EXPLOSION_DELAY 3.14f


typedef struct player Player;

/* =========================
   BOMB STRUCT
========================= */

typedef struct bomb
{
    Vector2 pos;
    float speed;

    bool isActive;
    bool playerIsDamage;

    int frameBomb;
    int frameWidthBomb;
    int frameHeightBomb;

    int currentFrameBomb;
    int frameCounterBomb;

    Sound explosion;
    Sound timer;

} Bomb;

/* =========================
   GOBLIN BOMB STRUCT
========================= */

typedef struct goblinBomb
{
    /* Core systems */
    Entity entity;
    Monsters base;

    /* Textures */
    Texture2D spriteAttackBomb;
    Texture2D spriteBomb;

    /* Stats */
    float maxLife;
    int life;
    float speed;
    int damage;
    int damageBomb;

    /* Frame configuration */
    int frameIdle;
    int frameAttack;
    int frameAttackBomb;
    int frameHurt;
    int frameDeath;
    int frameRun;

    int frameWidthAttackBomb;
    int frameHeightAttackBomb;

    int frameCounter;

    /* State flags */
    bool isAttackBomb;
    bool bombExplode;
    bool hasThrownBomb;
    bool animAttackBombEnd;
    bool isDead;
    bool wasWalking;
    bool attackSoundPlayed;
    bool soundBagPlayed;
    bool droppedHeart;

    /* Bomb system */
    Bomb bomb;
    float timerForExplosion;
    float radiusToDamage;
    float bombRange;
    float attackRange;

    /* Timers */
    float timerAttackBomb;
    float animAttackTimer;
    float hurtDuration;
    float hurtTimer;
    float viewPlayer;

    /* Sounds */
    Sound soundAttack;
    Sound soundBagGoblin;

} GoblinBomb;


/* =========================
   FUNCTIONS
========================= */

void InitGoblinBomb(GoblinBomb *goblinBomb);
void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player);
void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player);
void UnloadGoblinBomb(GoblinBomb *goblinBomb);

#endif // GOBLINBOMB_H