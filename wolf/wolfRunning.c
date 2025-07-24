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

void DrawWolfRunningLifeBar(WolfRun *wolfRun)
{
    float barWidth = 60.0f;
    float barHeight = 8.0f;
    float x = wolfRun->position.x + 80;  // ajuste horizontal
    float y = wolfRun->position.y + 30;  // acima do sprite

    float lifePercent = wolfRun->life / 100.0f;
    float currentBarWidth = barWidth * lifePercent;

    // Fundo (vermelho)
    DrawRectangle(x, y, barWidth, barHeight, RED);

    // Vida atual (verde)
    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN);

    // Contorno
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
}


void InitRunningWolf(WolfRun *wolfRun)
{
    wolfRun->position = (Vector2){1120, 450};
    wolfRun->start = (Vector2){500, 450};
    wolfRun->end = (Vector2){600, 450};

    wolfRun->life = 100;
    wolfRun->speedRun = 120.0f;
    wolfRun->speed = 95.0f;

    wolfRun->currentFrame = 0;
    wolfRun->frameCounter = 0;

    wolfRun->spriteWalkWolf  = LoadTexture("resources/sprites/wolf/WALK.png");
    wolfRun->spriteAtkWolf  = LoadTexture("resources/sprites/wolf/Attack_1.png");
    wolfRun->spriteIdleWolf = LoadTexture("resources/sprites/wolf/Idle.png");
    wolfRun->spriteDeadWolf = LoadTexture("resources/sprites/wolf/Dead.png");
    wolfRun->spriteRunWolf = LoadTexture("resources/sprites/wolf/Run.png");
    wolfRun->spriteHurtWolf = LoadTexture("resources/sprites/wolf/Hurt.png");

    wolfRun->frameWalk   = 11;
    wolfRun->frameAtk   = 6;
    wolfRun->frameIdle  = 8;
    wolfRun->frameDead  = 2;
    wolfRun->frameRun = 9;
    wolfRun->frameHurt  = 2;

    wolfRun->frameWidth  = wolfRun->spriteWalkWolf.width / wolfRun->frameWalk;
    wolfRun->frameHeight = wolfRun->spriteWalkWolf.height;

    wolfRun->direction = 1;
    wolfRun->isWalking = false;
    wolfRun->isRunning = false;
    wolfRun->isIdle = true;
    wolfRun->isAttacking = false;
    wolfRun->hasHitPlayer = false;
    wolfRun->wolfHasHit = false;
    wolfRun->isDead = false;

    wolfRun->deathAnimTimer = 0.0f;
    wolfRun->deathAnimationDone = false;

    wolfRun->attackRange = 100.0f;
    wolfRun->attackCooldown = 0.0f;
    wolfRun->attackDamageTimer = 0.0f;
    wolfRun->damage = 20;
    wolfRun->viewPlayer = 300.0f;
}

void UpdateRunningWolf(WolfRun *wolfRun, Player *player, float delta)
{

    if (wolfRun->life <= 0 && !wolfRun->isDead)
    {
        wolfRun->isDead = true;
        wolfRun->isRunning = false;
        wolfRun->isWalking = false;
        wolfRun->hasHitPlayer = false;
        wolfRun->wolfHasHit = false;
        wolfRun->frameDead = 1;
        wolfRun->deathAnimTimer = 0.0f;
        wolfRun->deathAnimationDone = false;
        wolfRun->currentFrame = 0;
        wolfRun->isIdle = false;
        wolfRun->isAttacking = false;
        wolfRun->speed = 0.0f;
        wolfRun->speedRun = 0.0f;

        wolfRun->droppedHeart = false; // Reseta o estado do coração
    }

    if (wolfRun->isDead && wolfRun->deathAnimationDone)
        return;

    wolfRun->frameCounter++;
    if (wolfRun->frameCounter >= (60 / 10))
    {
        wolfRun->frameCounter = 0;
        if (wolfRun->wolfHasHit && !wolfRun->isDead)
        {
            wolfRun->currentFrame = (wolfRun->currentFrame + 1) % wolfRun->frameHurt;
        }
        else if (wolfRun->isDead && !wolfRun->deathAnimationDone)
        {
            wolfRun->deathAnimTimer += delta;

            if (wolfRun->deathAnimTimer >= 0.2f)
            {   
            wolfRun->frameDead++;
            wolfRun->deathAnimTimer = 0.3f;

            if (wolfRun->frameDead >= 2)
            {
                wolfRun->frameDead = 1;
                wolfRun->deathAnimationDone = true;
                wolfRun->speed = 0.0f;
            }
            }
            wolfRun->currentFrame = wolfRun->frameDead;
        } 
        else if (!wolfRun->isDead)
        {
            if (wolfRun->isRunning)
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
    }

    if (wolfRun->wolfHasHit)
    {
        wolfRun->speed = 0.0f;
        wolfRun->speedRun = 0.0f;
    } 
    else 
    {
        wolfRun->speed = 95.0f;
        wolfRun->speedRun = 120.0f;
    }

    float distanceToPlayer = fabs(player->position.x - wolfRun->position.x);

    if (!wolfRun->isDead && distanceToPlayer <= wolfRun->viewPlayer)
    {
        wolfRun->direction = (player->position.x > wolfRun->position.x) ? 1 : -1;
        player->hasHit = false;

        if (!wolfRun->hasRunAttack)
        {
            // Corre até o player antes do ataque
            wolfRun->isRunning = true;
            wolfRun->isWalking = false;
            wolfRun->isAttacking = false;
            //Verifica hit no player quando corre 
            player->hasHit = true;
            wolfRun->hasHitPlayer = true;

            wolfRun->position.x += wolfRun->speedRun * wolfRun->direction * delta;

            if (!wolfRun->isDead && distanceToPlayer <= wolfRun->attackRange)
            {
                if (!wolfRun->hasHitPlayer)
                {
                    player->life -= wolfRun->damage;
                    //Verifica hit no player quando está batendo
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
                    player->hitTimer = 0.4f; 

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

    if (!wolfRun->isDead && CheckCollision(
            player->position.x, player->position.y, player->frameWidth, player->frameHeight,
            wolfRun->position.x, wolfRun->position.y, wolfRun->frameWidth, wolfRun->frameHeight))
    {
        player->hasHit = true;
        player->hitTimer = 0.3f;

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
    if (wolfRun->isDead)
    {
        currentSprite = wolfRun->spriteDeadWolf;
    }
    else if (wolfRun->wolfHasHit)
    {
        currentSprite = wolfRun->spriteHurtWolf;
    }
    else if (wolfRun->isAttacking)
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

    if (!wolfRun->isDead)
    {
        DrawWolfRunningLifeBar(wolfRun);
    }
}

void UnloadRunningWolf(WolfRun *wolfRun) 
{
    UnloadTexture(wolfRun->spriteWalkWolf);
    UnloadTexture(wolfRun->spriteAtkWolf);
    UnloadTexture(wolfRun->spriteIdleWolf);
    UnloadTexture(wolfRun->spriteDeadWolf);
    UnloadTexture(wolfRun->spriteRunWolf);
    UnloadTexture(wolfRun->spriteHurtWolf);
}