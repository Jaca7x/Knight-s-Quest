#ifndef WOLF_H
#define WOLF_H

#include "../librays/raylib.h"

typedef struct wolf
{
    Vector2 position;
    Vector2 start;
    Vector2 end;

    int life;

    float speed;

    int currentFrame;
    int frameCounter;

    Texture2D spriteWalkWolf;
    Texture2D spriteAtkWolf;
    Texture2D spriteIdleWolf;
    Texture2D spriteDeadWolf;
    
    int frameWidth;
    int frameHeight;

    int frameWalk;
    int frameAtk;
    int frameIdle;
    int frameDead;

    int direction;

} Wolf;

void InitWolf(Wolf *wolf);
void UpdateWolf(Wolf *wolf, float delta);
void DrawWolf(Wolf *wolf);
void UnloadWolf(Wolf *wolf);

#endif // WOLF_H