#include "wolf.h"

void InitWolf(Wolf *wolf) {
    wolf->position = (Vector2){};

    wolf->life = 200;

    wolf->currentFrame = 0;
    wolf->frameCounter = 0;
    
    wolf->spriteWalkWolf = LoadTexture("../resouces/sprites/wolf/Attack_1.png");
    wolf->spriteAtkWolf = LoadTexture("../resouces/sprites/wolf/walk.png");
    wolf->spriteIdleWolf = LoadTexture("../resouces/sprites/wolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("../resouces/sprites/wolf/Dead.png");

    wolf->frameWalk = 11;
    wolf->frameAtk = 6;
    wolf->frameIdle = 8;
    wolf->frameDead = 2;
}

void UpdateWolf(Wolf *wolf, float delta) {
    
}

void DrawWolf(Wolf *wolf) {
    
}

void UnloadWolf(Wolf *wolf) {
    
}