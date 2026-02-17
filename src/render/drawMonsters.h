#ifndef DRAW_MONSTERS_H
#define DRAW_MONSTERS_H

#include "raylib.h"
#include "stdio.h"
#include "core/define.h"

#define BOSS_SCALE_ATK_AND_WALK_AND_DEATH 1.5f
#define BOSS_SCALE_IDLE 1.0f
#define BOSS_SCALE_HURT 0.6f

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
void DrawBossAnim(Monsters *monsterint);

#endif // DRAW_MONSTERS_H