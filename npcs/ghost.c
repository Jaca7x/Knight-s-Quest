#include "ghost.h"

bool InteractionWithGhost(const Ghost *ghost, Player *player)
{
    float dx = player->position.x - ghost->position.x;
    return (dx > -150 && dx < 60);
}

void InitGhost(Ghost *ghost) 
{
    ghost->position = (Vector2){800, 540}; 
    ghost->ghostIdle = LoadTexture("resources/sprites/npc/ghost-idle.png"); 
    ghost->ghostBtnE = LoadTexture("resources/sprites/btns/btn-E.png");
    ghost->ghostSppech = LoadTexture("resources/sprites/npc/ghost-speech.png");
    ghost->ghostExclamation = LoadTexture("resources/sprites/npc/exclamation.png");

    ghost->frameIdle = 3;

    ghost->frameWidth = ghost->ghostIdle.width / ghost->frameIdle; 
    ghost->frameHeight = ghost->ghostIdle.height; 
    ghost->currentFrame = 0; 
    ghost->frameCounter = 0;
}

void UpdateGhost(Ghost *ghost, float delta) 
{
    ghost->frameCounter++;
    if (ghost->frameCounter >= (200 / 5)) 
    {
        ghost->frameCounter = 0;
        ghost->currentFrame = (ghost->currentFrame + 1) % ghost->frameIdle;
    }
}

void DrawGhost(Ghost *ghost, Player *player) 
{
    Rectangle source = {
        ghost->currentFrame * ghost->frameWidth,
        0,
        ghost->frameWidth,
        ghost->frameHeight
    };
    Rectangle dest = {
        ghost->position.x,
        ghost->position.y,
        ghost->frameWidth / 2.5,
        ghost->frameHeight / 2.5
    };
    Vector2 origin = {0, 0};

    DrawTexturePro(ghost->ghostIdle, source, dest, origin, 0.0f, Fade(WHITE, 0.7f));

    if (InteractionWithGhost(ghost, player))
    {
        DrawTexture(ghost->ghostBtnE, ghost->position.x + 15, ghost->position.y - 35, WHITE);
    }
    else
    {
        DrawTexture(ghost->ghostExclamation, ghost->position.x + 25, ghost->position.y - 25, WHITE);
    }
}

void UnloadGhost(Ghost *ghost) 
{
    UnloadTexture(ghost->ghostIdle);
    UnloadTexture(ghost->ghostBtnE);
    UnloadTexture(ghost->ghostSppech);
}