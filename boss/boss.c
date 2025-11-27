#include "boss.h"

bool CheckCollisionBoss(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

Rectangle GetSourceValueRec(int frame, int frameWidth, float direction, int frameHeigth)
{
    Rectangle r;
    r.x = frame * frameWidth;
    r.y = 0;
    r.width = frameWidth * direction;
    r.height = frameHeigth;

    return r;
}

Rectangle GetDestValueRec(Vector2 position, int frameWidth, int frameHeight, float scale)
{
    Rectangle r;

    r.x = position.x;
    r.y = position.y;
    r.width = frameWidth * scale;
    r.height = frameHeight * scale;

    return r;
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

    boss->scaleAtk = 1.5f;
    boss->scaleIdle = 1.0f;
    boss->scaleWalk = 1.5f;

    boss->frameWidth  = boss->spriteIdle.width / boss->frameIdle;
    boss->frameHeight = boss->spriteIdle.height;
    
    boss->frameWidthAtk = boss->spriteAtk.width / boss->frameAtk;
    boss->frameHeightAtk = boss->spriteAtk.height;

    boss->frameWidthWalk = boss->spriteWalk.width / boss->frameWalk;
    boss->frameHeightWalk = boss->spriteWalk.height; 

    boss->direction = 1.0f;

    boss->isAttacking = false;
    boss->isWalking = false;
    boss->isIdle = true;

    boss->attackRange = 100.0f;
    boss->viewPlayer = 900.0f;

    boss->speed = 10.0f;
}

void UpdateBoss(Boss *boss, Player *player, float delta) 
{
    boss->frameCounter++;

    if (boss->isAttacking && boss->frameCounter >= 10)
    {
        boss->frameCounter = 0;
        boss->currentFrame = (boss->currentFrame + 1) % boss->frameAtk;
    }
    else if (boss->isIdle && boss->frameCounter >= 40)
    {
        boss->frameCounter = 0;
        boss->currentFrame = (boss->currentFrame + 1) % boss->frameIdle;
    }
    else if (boss->isWalking && boss->frameCounter >= 40)
    {
        boss->frameCounter = 0;
        boss->currentFrame = (boss->currentFrame + 1) % boss->frameWalk;
    }

    float distance = fabs(player->position.x - boss->position.x);

    if (distance <= boss->viewPlayer)
    {
        boss->direction = (player->position.x > boss->position.x) ? 1 : -1;

        boss->isAttacking = false;
        boss->isIdle = false;
        boss->isWalking = true;


        if (distance <= boss->attackRange)
        {
            boss->isAttacking = true;
            boss->isWalking = false;
        }
    }
    else
    {
        boss->isIdle = true;
        boss->isWalking = false;
    }
    
    if (boss->isWalking)
    {
        boss->position.x += boss->speed * boss->direction * delta;
    } 
}   

Rectangle source;
Rectangle dest;

void DrawBoss(Boss *boss) 
{
    if (boss->isIdle)
    {
        source = GetSourceValueRec(boss->currentFrame, boss->frameWidth, boss->direction, boss->frameHeight);
        dest = GetDestValueRec(boss->position, boss->frameWidth, boss->frameHeight, boss->scaleIdle);

        DrawTexturePro(boss->spriteIdle, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    else if (boss->isAttacking)
    {
        source = GetSourceValueRec(boss->currentFrame, boss->frameWidthAtk, boss->direction, boss->frameHeightAtk);
        dest = GetDestValueRec(boss->position, boss->frameWidthAtk, boss->frameHeightAtk, boss->scaleAtk);

        DrawTexturePro(boss->spriteAtk, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    else if (boss->isWalking)
    {
        source = GetSourceValueRec(boss->currentFrame, boss->frameWidthWalk, boss->direction, boss->frameHeightWalk);
        dest = GetDestValueRec(boss->position, boss->frameWidthWalk, boss->frameHeightWalk, boss->scaleWalk);

        DrawTexturePro(boss->spriteWalk, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
}
