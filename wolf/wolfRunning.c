#include "wolfRunning.h"

/// Função de colisão AABB
bool CheckCollision(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}


void InitRunningWolf(WolfRun *wolfRun)
{
    wolfRun->position = (Vector2){1120, 450};
    wolfRun->start = (Vector2){500, 450};
    wolfRun->end = (Vector2){600, 450};

    wolfRun->life = 200;
    wolfRun->speedRun = 120.0f;
    wolfRun->speed = 95.0f;

    wolfRun->currentFrame = 0;
    wolfRun->frameCounter = 0;

    wolfRun->spriteWalkWolf  = LoadTexture("resources/sprites/wolf/WALK.png");
    wolfRun->spriteAtkWolf  = LoadTexture("resources/sprites/wolf/Attack_1.png");
    wolfRun->spriteIdleWolf = LoadTexture("resources/sprites/wolf/Idle.png");
    wolfRun->spriteDeadWolf = LoadTexture("resources/sprites/wolf/Dead.png");
    wolfRun->spriteRunWolf = LoadTexture("resources/sprites/wolf/Run.png");

    wolfRun->frameWalk   = 11;
    wolfRun->frameAtk   = 6;
    wolfRun->frameIdle  = 8;
    wolfRun->frameDead  = 2;
    wolfRun->frameRun = 9;

    wolfRun->frameWidth  = wolfRun->spriteWalkWolf.width / wolfRun->frameWalk;
    wolfRun->frameHeight = wolfRun->spriteWalkWolf.height;

    wolfRun->direction = 1;
    wolfRun->isWalking = false;
    wolfRun->isRunning = false;
    wolfRun->isIdle = true;
    wolfRun->isAttacking = false;
    wolfRun->hasHitPlayer = false;

    wolfRun->attackRange = 100.0f;
    wolfRun->attackCooldown = 0.0f;
    wolfRun->attackDamageTimer = 0.0f;
    wolfRun->damage = 20;
    wolfRun->viewPlayer = 300.0f;
}

void UpdateRunningWolf(WolfRun *wolfRun, Player *player, float delta)
{
    wolfRun->frameCounter++;
    if (wolfRun->frameCounter >= (60 / 10))
    {
        wolfRun->frameCounter = 0;

        if (wolfRun->isAttacking)
        {
            wolfRun->currentFrame = (wolfRun->currentFrame + 1) % wolfRun->frameAtk;
        }
        else if (wolfRun->isRunning)
        {
            wolfRun->currentFrame = (wolfRun->currentFrame + 1) % wolfRun->frameRun;
        }
        else if (wolfRun->isWalking)
        {
            wolfRun->currentFrame = (wolfRun->currentFrame + 1) % wolfRun->frameWalk;
        }
        else
        {
            wolfRun->currentFrame = (wolfRun->currentFrame + 1) % wolfRun->frameIdle;
        }
    }

    float distanceToPlayer = fabs(player->position.x - wolfRun->position.x);

    if (distanceToPlayer <= wolfRun->viewPlayer)
    {
        wolfRun->direction = (player->position.x > wolfRun->position.x) ? 1 : -1;
        player->hasHit = false;

        if (!wolfRun->hasRunAttack)
        {
            // Corre até o player antes do ataque
            wolfRun->isRunning = true;
            wolfRun->isWalking = false;
            wolfRun->isAttacking = false;

            wolfRun->position.x += wolfRun->speedRun * wolfRun->direction * delta;

            if (distanceToPlayer <= wolfRun->attackRange)
            {
                if (!wolfRun->hasHitPlayer)
                {
                    player->life -= wolfRun->damage;
                    wolfRun->hasHitPlayer = true;
                    player->hasHit = true;

                    if (player->position.x < wolfRun->position.x)
                        player->position.x -= 70;
                    else
                        player->position.x += 70;
                }

                wolfRun->hasRunAttack = true;

                // Para de correr, começa a andar após ataque
                wolfRun->isRunning = false;
                wolfRun->isWalking = true;
                wolfRun->isAttacking = false;
                wolfRun->hasHitPlayer = false;
                player->hasHit = false;
            }
        }
        else
        {
            // Após ataque correndo → andar atrás do player
            wolfRun->isRunning = false;
            wolfRun->isWalking = true;
            wolfRun->isAttacking = false;

            wolfRun->position.x += wolfRun->speed * wolfRun->direction * delta;

            if (distanceToPlayer <= wolfRun->attackRange)
            {
                wolfRun->isAttacking = true;
                wolfRun->isWalking = false;

                wolfRun->attackDamageTimer -= delta;

                if (wolfRun->attackDamageTimer <= 0 && !wolfRun->hasHitPlayer)
                {
                    player->life -= wolfRun->damage;
                    wolfRun->hasHitPlayer = true;
                    player->hasHit = true;

                    if (player->position.x < wolfRun->position.x)
                        player->position.x -= 70;
                    else
                        player->position.x += 70;
                }

                wolfRun->attackCooldown -= delta;
                if (wolfRun->attackCooldown <= 0)
                {
                    wolfRun->attackCooldown = 0.4f;
                    wolfRun->attackDamageTimer = 0.4f;
                    wolfRun->hasHitPlayer = false;
                }
            }
            else
            {
                wolfRun->isAttacking = false;
                wolfRun->hasHitPlayer = false;
            }
        }
    }
    else
    {
        wolfRun->isRunning = false;
        wolfRun->isWalking = false;
        wolfRun->isAttacking = false;
        wolfRun->hasRunAttack = false;
        wolfRun->hasHitPlayer = false;
        player->hasHit = false;
    }

    if (CheckCollision(
            player->position.x, player->position.y, player->frameWidth, player->frameHeight,
            wolfRun->position.x, wolfRun->position.y, wolfRun->frameWidth, wolfRun->frameHeight))
    {
        player->hasHit = true;

        if (player->position.x < wolfRun->position.x)
            player->position.x = wolfRun->position.x - player->frameWidth;
        else
            player->position.x = wolfRun->position.x + wolfRun->frameWidth;
    }
}

void DrawRunningWolf(WolfRun *wolfRun) 
{
    Texture2D currentSprite;

    // Escolhe o sprite baseado no estado atual
    if (wolfRun->isAttacking)
    {
        currentSprite = wolfRun->spriteAtkWolf;
    }
    else if (wolfRun->isRunning)
    {
        currentSprite = wolfRun->spriteRunWolf;
    }
    else if (wolfRun->isWalking)
    {
        currentSprite = wolfRun->spriteWalkWolf;
    }
    else
    {
        currentSprite = wolfRun->spriteIdleWolf;
    }

    Rectangle source = {
        wolfRun->currentFrame * wolfRun->frameWidth,
        0,
        wolfRun->frameWidth * wolfRun->direction,
        wolfRun->frameHeight
    };

    Rectangle dest = {
        wolfRun->position.x,
        wolfRun->position.y,
        wolfRun->frameWidth * 1.5f,
        wolfRun->frameHeight * 1.5f
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(currentSprite, source, dest, origin, 0.0f, WHITE);
}

void UnloadRunningWolf(WolfRun *wolfRun) 
{
    UnloadTexture(wolfRun->spriteAtkWolf);
    UnloadTexture(wolfRun->spriteIdleWolf);
    UnloadTexture(wolfRun->spriteDeadWolf);
}