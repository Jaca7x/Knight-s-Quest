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

void DrawWolfLifeBar(Wolf *wolf)
{
    if (wolf->isDead) return;  // não desenha se estiver morto

    float barWidth = 60.0f;
    float barHeight = 8.0f;
    float x = wolf->position.x + 20;  // ajuste horizontal
    float y = wolf->position.y + 30;  // acima do sprite

    // Define vida máxima do lobo
    float maxLife = (wolf->life > 100) ? 200.0f : 100.0f;

    float lifePercent = wolf->life / maxLife;
    if (lifePercent < 0) lifePercent = 0;

    float currentBarWidth = barWidth * lifePercent;

    // Fundo (vermelho)
    DrawRectangle(x, y, barWidth, barHeight, RED);

    // Vida atual (verde)
    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN);

    // Contorno
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
}



void InitWolfBase(Wolf *wolf, Vector2 pos)
{
    wolf->position = pos;
    wolf->start = pos;
    wolf->end = (Vector2){pos.x + 100, pos.y};

    wolf->life = 100;
    wolf->maxLife = 100;
    wolf->speed = 95.0f;

    wolf->currentFrame = 0;
    wolf->frameCounter = 0;

    wolf->spriteWalkWolf = LoadTexture("resources/sprites/wolf/walk.png");
    wolf->spriteAtkWolf  = LoadTexture("resources/sprites/wolf/Attack_1.png");
    wolf->spriteIdleWolf = LoadTexture("resources/sprites/wolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("resources/sprites/wolf/Dead.png");
    wolf->spriteHurtWolf = LoadTexture("resources/sprites/wolf/Hurt.png");

    wolf->frameWalk  = 11;
    wolf->frameAtk   = 6;
    wolf->frameIdle  = 8;
    wolf->frameDead  = 2;
    wolf->frameHurt  = 2;

    wolf->frameWidth  = wolf->spriteWalkWolf.width / wolf->frameWalk;
    wolf->frameHeight = wolf->spriteWalkWolf.height;

    wolf->direction = 1;
    wolf->isMoving = true;

    wolf->isDead = false;
    wolf->deathAnimationDone = false;
    wolf->deathAnimTimer = 0.0f;

    wolf->isPatrolling = true;

    wolf->isAttacking = false;
    wolf->hasHitPlayer = false;
    wolf->wolfHasHit = false;

    wolf->attackRange = 100.0f;
    wolf->attackCooldown = 0.0f;
    wolf->attackDamageTimer = 0.0f;
    wolf->damage = 20;

    wolf->droppedHeart = false;

    wolf->wolfHitSound = LoadSound("resources/sounds/sound_effects/wolf/wolf-hit.wav");
    wolf->wolfHitSoundHeavy = LoadSound("resources/sounds/sound_effects/wolf/wolf-hit-heavy.wav");
    wolf->wolfDeathSound = LoadSound("resources/sounds/sound_effects/wolf/wolf-death.wav");
}

void InitWhiteWolf(Wolf *wolf, Vector2 pos) 
{
    InitWolfBase(wolf, pos);

    wolf->speed = 140.0f;
    wolf->life = 100;
    wolf->maxLife = 100;

    wolf->spriteWalkWolf = LoadTexture("resources/sprites/whiteWolf/Walk.png");
    wolf->spriteAtkWolf  = LoadTexture("resources/sprites/whiteWolf/Attack.png");
    wolf->spriteIdleWolf = LoadTexture("resources/sprites/whiteWolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("resources/sprites/whiteWolf/Dead.png");
    wolf->spriteHurtWolf = LoadTexture("resources/sprites/whiteWolf/Hurt.png");
    wolf->spriteWalkWolf = LoadTexture("resources/sprites/whiteWolf/Walk.png");

    wolf->frameAtk = 4;
}

void InitRedWolf(Wolf *wolf, Vector2 pos) 
{
    InitWolfBase(wolf, pos);

    wolf->speed = 70.0f;
    wolf->damage = 40;
    wolf->life = 200;
    wolf->maxLife = 200;

    wolf->spriteWalkWolf = LoadTexture("resources/sprites/redWolf/Walk.png");
    wolf->spriteAtkWolf  = LoadTexture("resources/sprites/redWolf/Attack.png");
    wolf->spriteIdleWolf = LoadTexture("resources/sprites/redWolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("resources/sprites/redWolf/Dead.png");
    wolf->spriteHurtWolf = LoadTexture("resources/sprites/redWolf/Hurt.png");
    wolf->spriteWalkWolf = LoadTexture("resources/sprites/redWolf/Walk.png");
    
    wolf->frameAtk   = 5;
}

void UpdateWolf(Wolf *wolf, Player *player, float delta)
{
if (wolf->life <= 0 && !wolf->isDead)
    {
        PlaySound(wolf->wolfDeathSound);
        wolf->isDead = true;
        wolf->isMoving = false;
        wolf->hasHitPlayer = false;
        wolf->wolfHasHit = false;
        wolf->frameDead = 1;
        wolf->deathAnimTimer = 0.0f;
        wolf->deathAnimationDone = false;
        wolf->currentFrame = 0;
        wolf->isPatrolling = false;
        wolf->isAttacking = false;
    }

    if (wolf->isDead && wolf->deathAnimationDone)
        return;

    // ====== PATRULHA ======
if (wolf->isPatrolling)
{
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
}

    // ====== ANIMAÇÃO ======
    wolf->frameCounter++;
    if (wolf->frameCounter >= (60 / 10))
    {
        wolf->frameCounter = 0;
        
        if (wolf->wolfHasHit && !wolf->isDead)
        {
            wolf->currentFrame = (wolf->currentFrame + 1) % wolf->frameHurt;
        }
        else if (wolf->isDead && !wolf->deathAnimationDone)
        {
            wolf->deathAnimTimer += delta;

            if (wolf->deathAnimTimer >= 0.2f)
            {   
            wolf->frameDead++;
            wolf->deathAnimTimer = 0.3f;

            if (wolf->frameDead >= 2)
            {
                wolf->frameDead = 1;
                wolf->deathAnimationDone = true;
                wolf->speed = 0.0f;
            }
            }
            wolf->currentFrame = wolf->frameDead;
        } 
        else if (!wolf->isDead && wolf->isPatrolling) 
        {
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
    }

    
    // ====== ATAQUE ======
    float distance = fabs(player->position.x - wolf->position.x);

    if (wolf->wolfHasHit)
    {
        wolf->speed = 0.0f;
    } else 
    {
        wolf->speed = 95.0f;
    }
    
    if (!wolf->isDead && wolf->isAttacking)
    {
        wolf->attackDamageTimer -= delta;

        if (wolf->attackDamageTimer <= 0 && !wolf->hasHitPlayer)
        {
            if (distance <= wolf->attackRange)
            {
                player->life -= wolf->damage;
                wolf->hasHitPlayer = true;
                player->hasHit = true;
                player->hitTimer = 0.5f; 

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
        }
    }

    // ====== COLISÃO FÍSICA (BLOQUEAR PLAYER) ======
   if (!wolf->isDead && CheckCollisionAABB(
            player->position.x, player->position.y, player->frameWidth, player->frameHeight,
            wolf->position.x, wolf->position.y, wolf->frameWidth, wolf->frameHeight))
    {
        player->hasHit = true;
        player->hitTimer = 0.5f;

        if (player->position.x < wolf->position.x)
            player->position.x = wolf->position.x - player->frameWidth;
        else
            player->position.x = wolf->position.x + wolf->frameWidth;
    }
}

void DrawWolf(Wolf *wolf)
{
    Rectangle source = {
        wolf->currentFrame * wolf->frameWidth,
        0,
        wolf->frameWidth * wolf->direction,
        wolf->frameHeight
    };

    Rectangle dest = {
        wolf->position.x,
        wolf->position.y,
        wolf->frameWidth * 1.5f,
        wolf->frameHeight * 1.5f
    };

    Vector2 origin = {0, 0};

    if (wolf->isDead)
    {
        DrawTexturePro(wolf->spriteDeadWolf, source, dest, origin, 0.0f, WHITE);
    }
    else if (wolf->wolfHasHit)
    {
        DrawTexturePro(wolf->spriteHurtWolf, source, dest, origin, 0.0f, WHITE);
    }
    else if (wolf->isAttacking)
    {
        DrawTexturePro(wolf->spriteAtkWolf, source, dest, origin, 0.0f, WHITE);
    }
    else  
    {
        DrawTexturePro(wolf->spriteWalkWolf, source, dest, origin, 0.0f, WHITE);
    }

    DrawWolfLifeBar(wolf);
}

void UnloadWolf(Wolf *wolf)
{
    UnloadTexture(wolf->spriteWalkWolf);
    UnloadTexture(wolf->spriteAtkWolf);
    UnloadTexture(wolf->spriteIdleWolf);
    UnloadTexture(wolf->spriteDeadWolf);
    UnloadTexture(wolf->spriteHurtWolf);
    UnloadSound(wolf->wolfHitSound);
    UnloadSound(wolf->wolfHitSoundHeavy);
    UnloadSound(wolf->wolfDeathSound);
}
