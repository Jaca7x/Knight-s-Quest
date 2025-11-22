#include "boss.h"

void InitBoss(Boss *boss) 
{
    boss->position = (Vector2){300, 500};

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
}

void UpdateBoss(Boss *boss) 
{
    boss->frameCounter++;

    if (boss->frameCounter >= 30)
    {
        boss->frameCounter = 0;
        boss->currentFrame = (boss->currentFrame + 1) % boss->frameIdle;
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

    DrawTexturePro(boss->spriteIdle, source, dest, (Vector2){0,0}, 0.0f, WHITE);
}
