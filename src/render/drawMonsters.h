#ifndef DRAW_MONSTERS_H
#define DRAW_MONSTERS_H

#include "../../lib/raylib.h"
#include "stdio.h"

typedef struct monsters
{
    Texture2D spriteAtack;
    Texture2D spriteDead;
    Texture2D spriteWalk;
    Texture2D spriteIdle;
    Texture2D spriteHurt;

    Vector2 position;

    bool monsterHasHit;
    bool isAtacking;
    bool isDead;
    bool isWalking;
    bool isIdle;

    int currentFrame;

    int frameWidthHurt;
    int frameHeightHurt;

    int frameWidthWalk;
    int frameHeightWalk;

    int frameWidthIdle;
    int frameHeightIdle;

    int frameWidthDead;
    int frameHeightDead;

    int frameWidthAtack;
    int frameHeightAtack;

    int direction;

    float scale;
    float scaleIdle;

} Monsters;

void DrawMonsters(Monsters *monstersint, int rowBase, int offsetHurtY, int offsetDeadY, int offsetAtackY, int offsetWalkY, int offsetIdleY);

#endif // DRAW_MONSTERS_H