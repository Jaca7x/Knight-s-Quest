#include "boss.h"

bool CheckCollision(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitBoss(Boss *boss) 
{
    boss->position = (Vector2){300, 340};

    boss->currentFrame = 0;
    boss->frameCounter = 0;
    
    boss->spriteAtk = LoadTexture("resources/sprites/boss/boss_attack.png");
    boss->spriteDead = LoadTexture("resources/sprites/boss/boss_dead.png");
    boss->spriteIdle = LoadTexture("resources/sprites/boss/boss_idle.png");
    boss->spriteHurt = LoadTexture("resources/sprites/boss/boss_hurt.png");
    boss->spriteWalk = LoadTexture("resources/sprites/boss/boss_walk.png");

    boss->frameAtk  = 9;
    boss->frameDead = 4;
    boss->frameHurt = 2;
    boss->frameIdle = 4;
    boss->frameWalk = 6;

    boss->frameWidth  = boss->spriteIdle.width / boss->frameIdle;
    boss->frameHeight = boss->spriteIdle.height;                  

    boss->direction = 1.0f;

    boss->isAttacking = false;
    boss->isWalking = false;
    boss->isIdle = true;

    boss->attackRange = 100.0f;
}

void UpdateBoss(Boss *boss, Player *player) 
{
    boss->frameCounter++;

    if (boss->frameCounter >= 30)
    {
        if (boss->isIdle)
        {
            boss->frameCounter = 0;
            boss->currentFrame = (boss->currentFrame + 1) % boss->frameIdle;
        }
        else if (boss->isWalking)
        {
            boss->frameCounter = 0;
            boss->currentFrame = (boss->currentFrame + 1) % boss->frameWalk;
        }
        else if (boss->isAttacking)
        {
            boss->frameCounter = 0;
            boss->currentFrame = (boss->currentFrame + 1) % boss->frameAtk;

        }
    } 

    float distance = fabs(player->position.x - boss->position.x);

    if (boss->isAttacking)
    {
        if (distance <= boss->attackRange)
        {
            boss->isAttacking = true;
        }
    }
    


    if (CheckCollision(player->position.x, player->position.y, player->frameWidth, player->frameHeight,
                        boss->position.x, boss->position.y, boss->frameWidth, boss->frameHeight))
    {
        player->hasHit = true;
        player->hitTimer = 0.5f;

        if (player->position.x < boss->position.x)
            player->position.x = boss->position.x - player->frameWidth;
        else
            player->position.x = boss->position.x + boss->frameWidth;
    }
    
}

void DrawBoss(Boss *boss) 
{
    Rectangle source = {
        boss->currentFrame * boss->frameWidth,
        0,
        boss->frameWidth * boss->direction,
        boss->frameHeight
    };

    Rectangle dest = {
        boss->position.x,
        boss->position.y,
        boss->frameWidth ,
        boss->frameHeight 
    };

    if (boss->isIdle)
    {
        DrawTexturePro(boss->spriteIdle, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    else if (boss->isAttacking)
    {
        DrawTexturePro(boss->spriteAtk, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    
    
   

}
