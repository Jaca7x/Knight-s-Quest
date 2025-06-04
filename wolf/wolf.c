#include "wolf.h"

/// Função de colisão AABB
bool CheckCollisionAABB(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitWolf(Wolf *wolf)
{
    wolf->position = (Vector2){800, 450};
    wolf->positionRunning = (Vector2){600, 450};
    wolf->start = (Vector2){800, 450};
    wolf->end = (Vector2){920, 450};

    wolf->life = 200;
    wolf->speed = 95.0f;

    wolf->currentFrame = 0;
    wolf->frameCounter = 0;

    wolf->spriteWalkWolf = LoadTexture("resources/sprites/wolf/walk.png");
    wolf->spriteAtkWolf  = LoadTexture("resources/sprites/wolf/Attack_1.png");
    wolf->spriteIdleWolf = LoadTexture("resources/sprites/wolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("resources/sprites/wolf/Dead.png");
    wolf->spriteRunAtkWolf = LoadTexture("resources/sprites/wolf/Run+Attack.png");

    wolf->frameWalk  = 11;
    wolf->frameAtk   = 6;
    wolf->frameIdle  = 8;
    wolf->frameDead  = 2;
    wolf->frameRunAtk = 7;

    wolf->frameWidth  = wolf->spriteWalkWolf.width / wolf->frameWalk;
    wolf->frameHeight = wolf->spriteWalkWolf.height;

    wolf->direction = 1;
    wolf->isMoving = true;
    wolf->isRuning = false;
    wolf->isIdle = true;
    wolf->isAttacking = false;
    wolf->hasHitPlayer = false;

    wolf->attackRange = 100.0f;
    wolf->attackCooldown = 0.0f;
    wolf->attackDamageTimer = 0.0f;
    wolf->damage = 50;
}

void UpdateWolf(Wolf *wolf, Player *player, float delta)
{
    // ====== PATRULHA ======
    if (wolf->isMoving)
    {
        wolf->position.x += wolf->speed * wolf->direction * delta;

        if (wolf->position.x <= wolf->start.x)
        {
            wolf->direction = 1;
        }
        else if (wolf->position.x >= wolf->end.x)
        {
            wolf->direction = -1;
        }
    }

    // ====== ANIMAÇÃO ======
    wolf->frameCounter++;
    if (wolf->frameCounter >= (60 / 10))
    {
        wolf->frameCounter = 0;

        if (wolf->isAttacking)
        {
            wolf->currentFrame = (wolf->currentFrame + 1) % wolf->frameAtk;
        }
        else if (wolf->isMoving)
        {
            wolf->currentFrame = (wolf->currentFrame + 1) % wolf->frameWalk;
        }
        else
        {
            wolf->currentFrame = (wolf->currentFrame + 1) % wolf->frameIdle;
        }
    }

    // ====== ATAQUE ======
    float distance = fabs(player->position.x - wolf->position.x);

    if (wolf->isAttacking)
    {
        wolf->attackDamageTimer -= delta;

        if (wolf->attackDamageTimer <= 0 && !wolf->hasHitPlayer)
        {
            if (distance <= wolf->attackRange)
            {
                player->life -= wolf->damage;
                wolf->hasHitPlayer = true;
                player->hasHit = true;

                if (player->position.x < wolf->position.x)
                {
                    player->position.x -= 70;
                }
                else
                {
                    player->position.x += 70;
                }
            }
        }

        wolf->attackCooldown -= delta;
        if (wolf->attackCooldown <= 0)
        {
            wolf->attackCooldown = 0;
            wolf->isAttacking = false;
            wolf->isMoving = true;
            wolf->hasHitPlayer = false;
            player->hasHit = false;
        }
    }
    else
    {
        if (distance <= wolf->attackRange)
        {
            wolf->isAttacking = true;
            wolf->isMoving = false;
            wolf->attackCooldown = 0.4f;
            wolf->attackDamageTimer = 0.4f;
            wolf->hasHitPlayer = false;
            player->hasHit = true;
        }
    }

    // ====== COLISÃO FÍSICA (BLOQUEAR PLAYER) ======
    if (CheckCollisionAABB(
            player->position.x, player->position.y, player->frameWidth, player->frameHeight,
            wolf->position.x, wolf->position.y, wolf->frameWidth, wolf->frameHeight))
    {
        if (player->position.x < wolf->position.x)
        {
            player->position.x = wolf->position.x - player->frameWidth;
        }
        else
        {
            player->position.x = wolf->position.x + wolf->frameWidth;
        }
    }
}

void DrawWolf(Wolf *wolf)
{
    Rectangle source = {
        wolf->currentFrame * wolf->frameWidth,
        0,
        wolf->frameWidth * wolf->direction,
        wolf->frameHeight};

    Rectangle dest = {
        wolf->position.x,
        wolf->position.y,
        wolf->frameWidth * 1.5f,
        wolf->frameHeight * 1.5f};

        Rectangle destRunning = {
        wolf->positionRunning.x,
        wolf->positionRunning.y,
        wolf->frameWidth * 1.5f,
        wolf->frameHeight * 1.5f};

    Vector2 origin = {0, 0};

    if (wolf->isAttacking)
    {
        DrawTexturePro(wolf->spriteAtkWolf, source, dest, origin, 0.0f, WHITE);
    }
    else  
    {
        DrawTexturePro(wolf->spriteWalkWolf, source, dest, origin, 0.0f, WHITE);
    }
    
    DrawTexturePro(wolf->spriteIdleWolf, source, destRunning, origin, 0.0f, WHITE);
}

void UnloadWolf(Wolf *wolf)
{
    UnloadTexture(wolf->spriteWalkWolf);
    UnloadTexture(wolf->spriteAtkWolf);
    UnloadTexture(wolf->spriteIdleWolf);
    UnloadTexture(wolf->spriteDeadWolf);
}
