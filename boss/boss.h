#ifndef BOSS_H
#define BOSS_H

#include "../librays/raylib.h"

typedef struct boss
{
    Vector2 position;

    Texture2D spriteWalk;
    Texture2D spriteHurt;
    Texture2D spriteAtk;
    Texture2D spriteIdle;
    Texture2D spriteDead;

    int frameIdle;
    int frameWalk;
    int frameDead;
    int frameHurt;
    int frameAtk;

    int currentFrame;     
    int frameCounter;      
    int frameWidth;        
    int frameHeight; 

    float direction;
} Boss;

void InitBoss(Boss *boss);
void UpdateBoss(Boss *boss);
void DrawBoss(Boss *boss);

#endif //BOSS_H
