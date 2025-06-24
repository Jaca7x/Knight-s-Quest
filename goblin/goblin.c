#include "goblin.h"

/// Função de colisão AABB
bool CheckCollisionGoblin(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitGoblin(Goblin *goblin) 
{
    goblin->position = (Vector2){780, 567};

    goblin->goblinSpriteWalk = LoadTexture("resources/sprites/goblin/goblin-walk.png");
    goblin->goblinSpriteHurt = LoadTexture("resources/sprites/goblin/goblin-hurt.png");
    goblin->goblinSpriteDead = LoadTexture("resources/sprites/goblin/goblin-dead.png");
    goblin->goblinSpriteIdle = LoadTexture("resources/sprites/goblin/goblin-idle.png");
    goblin->goblinSpriteAtk = LoadTexture("resources/sprites/goblin/goblin-atk.png");

    goblin->frameWalk = 6;
    goblin->frameHurt = 4;
    goblin->frameDead = 6;
    goblin->frameIdle = 8;
    goblin->frameAtk = 6;

    goblin->currentFrame = 0;
    goblin->frameCounter = 8;
    goblin->frameWidth = goblin->goblinSpriteIdle.width / goblin->frameIdle;
    goblin->frameHeight = goblin->goblinSpriteIdle.height;

    goblin->isIdle = true;
    goblin->isWalking = false;
    goblin->isAtacking = false;

    goblin->scale = 0.02f;

    goblin->life = 100;

    goblin->damage = 20;
}

void UpdateGoblin(Goblin *goblin, Player *player, float delta)
{
    goblin->frameCounter++;
    if (goblin->frameCounter >= (60 / 10))
    {
        goblin->frameCounter = 0;

        if (goblin->isIdle)
        {
            goblin->currentFrame = (goblin->currentFrame + 1) % goblin->frameIdle;
        }
        else if (goblin->isWalking)
        {
            goblin->currentFrame = (goblin->currentFrame + 1) % goblin->frameWalk;
        }
        else
        {
            goblin->currentFrame = (goblin->currentFrame + 1) % goblin->frameAtk;
        }    
    }

    float push = 50.0f;

    if (CheckCollisionGoblin(
            player->position.x, player->position.y, player->frameWidth , player->frameHeight,
            goblin->position.x, goblin->position.y, goblin->frameWidth * goblin->scale, goblin->frameHeight * goblin->scale))
    {
        if (player->position.x < goblin->position.x)
        
            player->position.x -= push;
        else
            player->position.x -= push;
    }   

}

void DrawGoblin(Goblin *goblin) 
{
    Rectangle source = {
        goblin->currentFrame * goblin->frameWidth,
        0,
        goblin->frameWidth,
        goblin->frameHeight
    };

    Rectangle dest = {
        goblin->position.x,
        goblin->position.y,
        goblin->frameWidth * 0.15f,
        goblin->frameHeight * 0.15f
    };

    Vector2 origin = {0, 0};

    if (goblin->isIdle)
    {
        DrawTexturePro(goblin->goblinSpriteIdle, source, dest, origin, 0.0f, WHITE);
    } 
    else if (goblin->isWalking)
    {
        DrawTexturePro(goblin->goblinSpriteWalk, source, dest, origin, 0.0f, WHITE);
    } 
    else if (goblin->isAtacking)
    {
        DrawTexturePro(goblin->goblinSpriteAtk, source, dest, origin, 0.0f, WHITE);
    }
}

void UnloadGoblin(Goblin *goblin) 
{
    UnloadTexture(goblin->goblinSpriteWalk);
    UnloadTexture(goblin->goblinSpriteHurt);
    UnloadTexture(goblin->goblinSpriteDead);
    UnloadTexture(goblin->goblinSpriteIdle);
    UnloadTexture(goblin->goblinSpriteAtk);
}