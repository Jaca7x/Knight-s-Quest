#include "wolf.h"

void InitWolf(Wolf *wolf) {
    wolf->position = (Vector2){800, 450};
    wolf->start = (Vector2){800, 450};
    wolf->end = (Vector2){920, 450};

    wolf->life = 200;
    wolf->speed = 95.0f;

    wolf->currentFrame = 0;
    wolf->frameCounter = 0;

    // Corrigindo os caminhos e os sprites certos
    wolf->spriteWalkWolf = LoadTexture("resources/sprites/wolf/walk.png");
    wolf->spriteAtkWolf  = LoadTexture("resources/sprites/wolf/Attack_1.png");
    wolf->spriteIdleWolf = LoadTexture("resources/sprites/wolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("resources/sprites/wolf/Dead.png");

    // Frame sizes
    wolf->frameWalk  = 11;
    wolf->frameAtk   = 6;
    wolf->frameIdle  = 8;
    wolf->frameDead  = 2;

    wolf->frameWidth  = wolf->spriteWalkWolf.width / wolf->frameWalk;
    wolf->frameHeight = wolf->spriteWalkWolf.height;

    wolf->direction = 1;
}

void UpdateWolf(Wolf *wolf, float delta) {
    // Movimento simples em direção ao destino
    wolf->position.x += wolf->speed * wolf->direction * delta;

    if (wolf->position.x <= wolf->start.x) 
    {
        wolf->direction = 1;
    } 
    else if (wolf->position.x >= wolf->end.x) 
    {
        wolf->direction = -1;
    }
 
    // Animação (10 FPS)
    wolf->frameCounter++;
    if (wolf->frameCounter >= (60 / 10)) {
        wolf->frameCounter = 0;
        wolf->currentFrame = (wolf->currentFrame + 1) % wolf->frameWalk;
    }
}

void DrawWolf(Wolf *wolf) {
    Rectangle source = {
        wolf->currentFrame * wolf->frameWidth,
        0,
        wolf->frameWidth * wolf->direction,
        wolf->frameHeight
    };

    Rectangle dest = {
        wolf->position.x,
        wolf->position.y,
        wolf->frameWidth * 1.5,
        wolf->frameHeight * 1.5
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(wolf->spriteWalkWolf, source, dest, origin, 0.0f, WHITE);
}

void UnloadWolf(Wolf *wolf) {
    UnloadTexture(wolf->spriteWalkWolf);
    UnloadTexture(wolf->spriteAtkWolf);
    UnloadTexture(wolf->spriteIdleWolf);
    UnloadTexture(wolf->spriteDeadWolf);
}
