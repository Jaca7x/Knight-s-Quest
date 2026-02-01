#ifndef GOBLINBOMB_H
#define GOBLINBOMB_H

#include "../librays/raylib.h"
#include "../player/player.h"

#define MAP_GOBLIN_BOMB 0

typedef struct player Player;

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
} Bomb;

typedef struct goblinBomb
{
    Texture2D spriteIdle;
    Texture2D spriteAttackBomb;
    Texture2D spriteAttack;
    Texture2D spriteHurt;

    Vector2 position;

    float maxLife;
    int life;

    int frameIdle;
    int frameAttackBomb;
    int frameAttack;
    int frameHurt;

    int frameWidthIdle;
    int frameHeightIdle;

    int frameWidthAttackBomb;
    int frameHeightAttackBomb;

    int frameWidthAttack;
    int frameHeightAttack;

    int frameWidthHurt;
    int frameHeightHurt;

    int frameCounter;
    int currentFrame;

    bool isIdle;
    bool isAttackBomb;
    bool bombExplode;
    bool hasThrownBomb;
    bool isAttack;

    Texture2D spriteBomb;

    Bomb bomb;
    int damageBomb;
    float timerForExplosion;
    float radiusToDamage;
    float bombRange;
    float attackRange;

    float timerAttackBomb;
    bool animAttackBombEnd;

    int direction;

    float animAttackTimer;

    bool goblinHasHit;
    bool isDead;

    float hurtDuration;
    float hurtTimer;
} GoblinBomb;




void InitGoblinBomb(GoblinBomb *goblinBomb);
void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player);
void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player);
void UnloadGoblinBomb(GoblinBomb *goblinBomb);

#endif // GOBLINBOMB_H