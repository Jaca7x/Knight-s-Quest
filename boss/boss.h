#ifndef BOSS_H
#define BOSS_H

#include "../librays/raylib.h"
#include "../player/player.h"

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
    int frameWidthAtk;        
    int frameHeightAtk; 
    int frameWidth;        
    int frameHeight; 

    float scaleAtk;
    float scaleIdle;
    
    float direction;

    bool isWalking;
    bool isAttacking;
    bool isIdle;

    float attackRange;
} Boss;

void InitBoss(Boss *boss);
void UpdateBoss(Boss *boss, Player *player);
void DrawBoss(Boss *boss);

#endif //BOSS_H
