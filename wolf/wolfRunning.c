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
    wolfRun->speedRun = 250.0f;
    wolfRun->speed = 95.0f;

    wolfRun->currentFrame = 0;
    wolfRun->frameCounter = 0;

    wolfRun->spriteWalkWolf  = LoadTexture("resources/sprites/wolf/WALK.png");
    wolfRun->spriteAtkWolf  = LoadTexture("resources/sprites/wolf/Attack_1.png");
    wolfRun->spriteIdleWolf = LoadTexture("resources/sprites/wolf/Idle.png");
    wolfRun->spriteDeadWolf = LoadTexture("resources/sprites/wolf/Dead.png");
    wolfRun->spriteRunAtkWolf = LoadTexture("resources/sprites/wolf/Run+Attack.png");

    wolfRun->frameWalk   = 11;
    wolfRun->frameAtk   = 6;
    wolfRun->frameIdle  = 8;
    wolfRun->frameDead  = 2;
    wolfRun->frameRunAtk = 7;

    wolfRun->frameWidth  = wolfRun->spriteWalkWolf.width / wolfRun->frameWalk;
    wolfRun->frameHeight = wolfRun->spriteWalkWolf.height;

    wolfRun->direction = 1;
    wolfRun->isWalking = false;
    wolfRun->isRunning = false;
    wolfRun->isIdle = true;
    wolfRun->isAttacking = false;
    wolfRun->hasHitPlayer = false;
    wolfRun->hasRunAttack = false;

    wolfRun->attackRange = 100.0f;
    wolfRun->attackCooldown = 0.0f;
    wolfRun->attackDamageTimer = 0.0f;
    wolfRun->damage = 20;
    wolfRun->damageJump = 40;
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
            wolfRun->currentFrame = (wolfRun->currentFrame + 1) % wolfRun->frameRunAtk;
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

    // Verifica se player está na visão
    if (distanceToPlayer <= wolfRun->viewPlayer)
    {
        wolfRun->direction = (player->position.x > wolfRun->position.x) ? 1 : -1;

        // Se ainda não iniciou o ataque correndo
        if (!wolfRun->hasRunAttack)
        {
            wolfRun->isRunning = true;
            wolfRun->isWalking = false;
            wolfRun->isAttacking = false;

            // Movimenta durante o ataque de corrida
            wolfRun->position.x += wolfRun->speedRun * wolfRun->direction * delta;

            // Se entrou na range de dano da corrida
            if (distanceToPlayer <= wolfRun->attackRange)
            {
                if (!wolfRun->hasHitPlayer)
                {
                    player->life -= wolfRun->damageJump;
                    wolfRun->hasHitPlayer = true;

                    // Empurra o player
                    if (player->position.x < wolfRun->position.x)
                        player->position.x -= 70;
                    else
                        player->position.x += 70;
                }

                // Finaliza o ataque de corrida
                wolfRun->hasRunAttack = true;
                wolfRun->isRunning = false;
                wolfRun->isWalking = true;
                wolfRun->hasHitPlayer = false;
            }
        }
        else
        {
            // Depois do ataque de corrida começa a andar e atacar normal
            float move = wolfRun->speed * wolfRun->direction * delta;
            wolfRun->position.x += move;

            if (distanceToPlayer <= wolfRun->attackRange)
            {
                // Ataca normalmente
                wolfRun->isAttacking = true;
                wolfRun->isWalking = false;

                wolfRun->attackDamageTimer -= delta;
                if (wolfRun->attackDamageTimer <= 0 && !wolfRun->hasHitPlayer)
                {
                    player->life -= wolfRun->damage;
                    wolfRun->hasHitPlayer = true;

                    // Empurra o player
                    if (player->position.x < wolfRun->position.x)
                        player->position.x -= 50;
                    else
                        player->position.x += 50;
                }

                wolfRun->attackCooldown -= delta;
                if (wolfRun->attackCooldown <= 0)
                {
                    wolfRun->attackCooldown = 0.6f;
                    wolfRun->attackDamageTimer = 0.3f;
                    wolfRun->hasHitPlayer = false;
                }
            }
            else
            {
                wolfRun->isAttacking = false;
                wolfRun->isWalking = true;
            }
        }
    }
    else
    {
        // Fora da visão → volta para Idle
        wolfRun->isRunning = false;
        wolfRun->isWalking = false;
        wolfRun->isAttacking = false;
        wolfRun->hasRunAttack = false;
    }

        // ====== COLISÃO FÍSICA (BLOQUEAR PLAYER) ======
    if (CheckCollision(
            player->position.x, player->position.y, player->frameWidth, player->frameHeight,
            wolfRun->position.x, wolfRun->position.y, wolfRun->frameWidth, wolfRun->frameHeight))
    {
        if (player->position.x < wolfRun->position.x)
        {
            player->position.x = wolfRun->position.x - player->frameWidth;
        }
        else
        {
            player->position.x = wolfRun->position.x + wolfRun->frameWidth;
        }
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
        currentSprite = wolfRun->spriteRunAtkWolf;
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