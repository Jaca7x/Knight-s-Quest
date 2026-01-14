#ifndef GOBLIN_TANK_H
#define GOBLIN_TANK_H

#include "../librays/raylib.h"

typedef struct goblinTank
{
    Vector2 position;

    Texture2D goblinTankSpriteWalk;
    Texture2D goblinTankSpriteHurt;
    Texture2D goblinTankSpriteDead;
    Texture2D goblinTankSpriteIdle;
    Texture2D goblinTankSpriteAtk;

    int frameWalk;
    int frameHurt;
    int frameDead;
    int frameIdle;
    int frameAtk;

    int currentFrame;      
    int frameCounter;      
    int frameWidth;       
    int frameHeight;       
    float frameFactor;     
    
    bool isIdle;
    bool isWalking;
    bool isAtacking;
    bool isDead;
    bool goblinTankHasHit;
    bool deathAnimationDone;

} GoblinTank;


void InitGoblinTank(GoblinTank *goblinTank);
void UpdateGoblinTank(GoblinTank *goblinTank, float deltaTime);
void DrawGoblinTank(GoblinTank *goblinTank);
void UnloadGoblinTank(GoblinTank *goblinTank);

#endif // GOBLIN_TANK_H