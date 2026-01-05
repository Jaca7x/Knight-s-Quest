#include "boss.h"

bool CheckCollisionBoss(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void DrawBossfLifeBar(Boss *boss)
{
    if (boss->isDead) return;

    float barWidth = 800.0f;
    float barHeight = 18.0f;
    float x = (GetScreenWidth() / 2) - (barWidth / 2); 
    float y = 20;

    float maxLife = 2000.0f;
    float lifePercent = boss->life / maxLife;
    if (lifePercent < 0) lifePercent = 0;

    float currentBarWidth = barWidth * lifePercent;

    DrawText("Boss Life", x + barWidth / 2 - MeasureText("Boss Life", 10) / 2, y - 18 , 20, BLACK);

    DrawRectangle(x, y, barWidth, barHeight, RED);

    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN);

    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
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

float targetY = 340;

void InitBoss(Boss *boss) 
{
    boss->position = (Vector2){600, -320};

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
    boss->scaleHurt = 0.6f;

    boss->hurtTimer = 0.0f;
    boss->hurtDuration= 1.0f;

    boss->frameWidth  = boss->spriteIdle.width / boss->frameIdle;
    boss->frameHeight = boss->spriteIdle.height;
    
    boss->frameWidthAtk = boss->spriteAtk.width / boss->frameAtk;
    boss->frameHeightAtk = boss->spriteAtk.height;

    boss->frameWidthHurt = boss->spriteHurt.width / boss->frameHurt;
    boss->frameHeightHurt = boss->spriteHurt.height;

    boss->frameWidthDead = boss->spriteDead.width / boss->frameDead;
    boss->frameHeightDead = boss->spriteDead.height;

    boss->attackTime = 0.0f;
    boss->attackCooldown = 0.0f;
    boss->hitFrame = 4;         
    boss->hasAppliedDamage = false;


    boss->frameWidthWalk = boss->spriteWalk.width / boss->frameWalk;
    boss->frameHeightWalk = boss->spriteWalk.height; 

    boss->direction = 1.0f;

    boss->isAtacking = false;
    boss->isWalking = false;
    boss->isIdle = true;
    boss->isDead = false;

    boss->attackRange = 100.0f;
    boss->viewPlayer = 300.0f;

    boss->speed = 10.0f;
    
    boss->life = 2000;
    boss->bossHasHit = false;
    
    boss->damage = 40;

    boss->bossMusic = LoadMusicStream("resources/music/boss-soundtrack.mp3");
}

void UpdateBoss(Boss *boss, Player *player, float delta, bool *bossTriggered) 
{
    float speedY = 250.0f;

    if (boss->position.y < targetY && *bossTriggered)
    {
        boss->position.y += speedY * delta;
        boss->isIdle = false;
        boss->isWalking = false;
        boss->isAtacking = false;

        if (boss->position.y > targetY)
        {
            boss->position.y = targetY;
        }
    }

    if (boss->isDead)
    {
    boss->frameCounter++;

    if (boss->frameCounter >= 40)
    {
        boss->frameCounter = 0;
        boss->currentFrame++;

        if (boss->currentFrame >= boss->frameDead)
            boss->currentFrame = boss->frameDead - 1; 
    }

    return;
}


    if (boss->life <= 0 && !boss->isDead) 
    {
        boss->isDead = true;
        boss->bossHasHit = false;
        boss->currentFrame = 0;
        boss->frameDead = 4;
        boss->isWalking = false;
        boss->isAtacking = false;
        boss->isIdle = false;
    }

     if (boss->bossHasHit) 
    {
        boss->isIdle = false;
        boss->isWalking = false;
        boss->isAtacking = false;

        boss->hurtTimer += delta;

        if (boss->hurtTimer >= boss->hurtDuration) 
        {
            boss->bossHasHit = false;
            boss->hurtTimer = 0.0f;
            boss->isIdle = true;
        }
    }

    boss->frameCounter++;
    
    if (boss->bossHasHit && boss->frameCounter >= 30)
    {
        boss->frameCounter = 0;
        boss->currentFrame = (boss->currentFrame + 1) % boss->frameHurt;
    }
    else if (boss->isAtacking && boss->frameCounter >= 10)
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
    else if (boss->isDead && boss->frameCounter >= 40)
    {
    boss->frameCounter = 0;
    boss->currentFrame = (boss->currentFrame + 1);

    if (boss->currentFrame >= boss->frameDead)
        boss->currentFrame = boss->frameDead - 1;
    }


    float distance = fabs(player->position.x - boss->position.x);

    if (distance >= boss->viewPlayer)
    {
        boss->isIdle = true;
        boss->isWalking = false;
        boss->isAtacking = false;
        return;
    }

    boss->direction = (player->position.x > boss->position.x) ? 1 : -1;

    if (boss->isAtacking)
    {

        boss->frameCounter++;

        if (boss->frameCounter >= 10)
        {
            boss->frameCounter = 0;
            boss->currentFrame++;

            if (boss->currentFrame >= boss->frameAtk)
            {
                boss->currentFrame = 0;
                boss->isAtacking = false;
                boss->hasAppliedDamage = false;  
                boss->attackCooldown = 1.5f;
            }
        }

        if (boss->currentFrame == boss->hitFrame && !boss->hasAppliedDamage)
        {
            if (CheckCollisionBoss(
                player->position.x, player->position.y, player->frameWidth, player->frameHeight,
                boss->position.x, boss->position.y, boss->frameWidthAtk, boss->frameHeightAtk))
            {
                player->life -= boss->damage;
                player->hasHit = true;
                player->hitTimer = 0.5f;
                boss->hasAppliedDamage = true;
                player->position.x -= 120;
            }
        }
        return; 
    }

    if (distance <= boss->attackRange && boss->attackCooldown <= 0)
    {
        boss->isAtacking = true;
        boss->currentFrame = 0;
        boss->frameCounter = 0;
        boss->hasAppliedDamage = false;
        return;
    }

    boss->attackCooldown -= delta;

    boss->isWalking = true;
    boss->isIdle = false;
    
    if (boss->isWalking)
    {
        boss->position.x += boss->speed * boss->direction * delta;
    } 
}

Rectangle source;
Rectangle dest;

void DrawBoss(Boss *boss) 
{
    if (boss->isDead)
    {
        float hurtOffsetY = 45.0f;

        source = GetSourceValueRec(boss->currentFrame, boss->frameWidthDead,
                               boss->direction, boss->frameHeightDead);
        dest = GetDestValueRec(boss->position, boss->frameWidthDead,
                           boss->frameHeightDead, boss->scaleWalk);

        dest.y += hurtOffsetY;

        DrawTexturePro(boss->spriteDead, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    else if (boss->bossHasHit)
    {
        float hurtOffsetY = -15.0f;

        source = GetSourceValueRec(boss->currentFrame, boss->frameWidthHurt,
                               boss->direction, boss->frameHeightHurt);
        dest = GetDestValueRec(boss->position, boss->frameWidthHurt,
                           boss->frameHeightHurt, boss->scaleHurt);

        dest.y += hurtOffsetY;

        DrawTexturePro(boss->spriteHurt, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    else if (boss->isAtacking)
    {
        float attackOffsetY = -35.0f;

        source = GetSourceValueRec(boss->currentFrame, boss->frameWidthAtk,
                               boss->direction, boss->frameHeightAtk);
        dest = GetDestValueRec(boss->position, boss->frameWidthAtk,
                           boss->frameHeightAtk, boss->scaleAtk);

        dest.y += attackOffsetY;

        DrawTexturePro(boss->spriteAtk, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    else if (boss->isWalking)
    {
        source = GetSourceValueRec(boss->currentFrame, boss->frameWidthWalk,
                               boss->direction, boss->frameHeightWalk);
        dest = GetDestValueRec(boss->position, boss->frameWidthWalk,
                           boss->frameHeightWalk, boss->scaleWalk);

        DrawTexturePro(boss->spriteWalk, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    else
    {
        source = GetSourceValueRec(boss->currentFrame, boss->frameWidth,
                               boss->direction, boss->frameHeight);
        dest = GetDestValueRec(boss->position, boss->frameWidth,
                           boss->frameHeight, boss->scaleIdle);

        DrawTexturePro(boss->spriteIdle, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }

    if (boss->position.y == targetY)
    {
        DrawBossfLifeBar(boss);
    }
    
   
}
