#ifndef GOBLINBOMB_H
#define GOBLINBOMB_H

#include "../librays/raylib.h"
#include "../player/player.h"


typedef struct player Player;

typedef struct bomb
{
    Vector2 pos;
    float speed;

    bool isActive;

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
    Vector2 position;

    int frameIdle;
    int frameAttackBomb;

    int frameWidthIdle;
    int frameHeightIdle;

    int frameWidthAttackBomb;
    int frameHeightAttackBomb;

    int frameCounter;
    int currentFrame;

    bool isIdle;
    bool isAttackBomb;
    bool bombExplode;
    bool hasThrownBomb;

    Texture2D spriteBomb;

    Bomb bomb;
    int damageBomb;
    float timerForExplosion;
    float radiusToDamage;
    float bombRange;

    float timerAttackBomb;
    bool animAttackBombEnd;
} GoblinBomb;




void InitGoblinBomb(GoblinBomb *goblinBomb);
void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player);
void DrawGoblinBomb(GoblinBomb *goblinBomb);

#endif // GOBLINBOMB_H