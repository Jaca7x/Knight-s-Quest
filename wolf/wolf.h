#ifndef WOLF_H
#define WOLF_H

#include "../librays/raylib.h"

typedef struct wolf
{
    Vector2 position;

    int life;

    int currentFrame;
    int frameCounter;

    Texture2D spriteWalkWolf;
    Texture2D spriteAtkWolf;
    Texture2D spriteIdleWolf;
    Texture2D spriteDeadWolf;

    int frameWalk;
    int frameAtk;
    int frameIdle;
    int frameDead;

} Wolf;

void InitWolf(Wolf *wolf);
void UpdateWolf(Wolf *wolf, float delta);
void DrawWolf(Wolf *wolf);
void UnloadWolf(Wolf *wolf);

#endif // WOLF_H