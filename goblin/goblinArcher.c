#include "goblinArcher.h"

bool CheckCollisionArrow (float x1, float y1, float w1, float h1,
                          float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void DrawGoblinArcherLifeBar(GoblinArcher *goblinArcher)
{
    float barWidth = 60.0f;
    float barHeight = 8.0f;
    float x = goblinArcher->position.x + 10;  // ajuste horizontal
    float y = goblinArcher->position.y - 10;  // acima do sprite

    float lifePercent = goblinArcher->life / 50.0f;
    float currentBarWidth = barWidth * lifePercent;

    // Fundo (vermelho)
    DrawRectangle(x, y, barWidth, barHeight, RED);

    // Vida atual (verde)
    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN);

    // Contorno
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
}
                     
void InitGoblinArcher(GoblinArcher *goblinArcher)
{
    goblinArcher->position = (Vector2){1120, 567};
    goblinArcher->life = 50;

    goblinArcher->speed = 95.0f;

    goblinArcher->currentFrame = 0;
    goblinArcher->frameCounter = 0;

    goblinArcher->spriteWalkGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-walk.png");
    goblinArcher->spriteAtkGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-atk.png");
    goblinArcher->spriteIdleGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-idle.png");
    goblinArcher->spriteDeadGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-dead.png");
    goblinArcher->spriteHurtGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-hurt.png");
    goblinArcher->arrowTexture = LoadTexture("resources/sprites/goblinArcher/Arrow.png");

    goblinArcher->frameWalk = 6;
    goblinArcher->frameAtk = 9;
    goblinArcher->frameIdle = 8;
    goblinArcher->frameDead = 9;
    goblinArcher->frameHurt = 4;

    goblinArcher->frameWidth = goblinArcher->spriteIdleGoblinArcher.width / goblinArcher->frameIdle;
    goblinArcher->frameHeight = goblinArcher->spriteIdleGoblinArcher.height;
    goblinArcher->frameFactor = 0.15f; // Fator de escala para os frames

    goblinArcher->arrowSpeed = 380.0f;
    goblinArcher->arrowTolerance = 90;
    goblinArcher->arrowDamage = 15;
    goblinArcher->arrow.active = false;

    goblinArcher->direction = 1;
    goblinArcher->isWalking = false;
    goblinArcher->isRunning = false;
    goblinArcher->isIdle = true;
    goblinArcher->goblinHasHit = false;
    goblinArcher->isDead = false;
    goblinArcher->isAtacking = false;

    goblinArcher->deathAnimTimer = 0.0f;
    goblinArcher->deathAnimationDone = false;

    goblinArcher->goblinView = 600.0f;
    goblinArcher->attackRange = 400.0f;
    goblinArcher->attackCooldown = 1.5f;
    goblinArcher->hasHitPlayer = false;
    goblinArcher->arrowCooldown = 0.0f;

    goblinArcher->attackAnimTimer = 0.0f;
}


void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta)
{
    if (goblinArcher->life <= 0 && !goblinArcher->isDead)
    {
        goblinArcher->isDead = true;
        goblinArcher->currentFrame = 0;
        goblinArcher->hasHitPlayer = false;  
        goblinArcher->goblinHasHit = false;
        player->hasHit = false;
        goblinArcher->frameDead = 7;
        goblinArcher->deathAnimTimer = 0.0f;
        goblinArcher->deathAnimationDone = false;
        goblinArcher->isWalking = false;
        goblinArcher->isAtacking = false;
        goblinArcher->isIdle = false;
        goblinArcher->arrow.active = false;
        goblinArcher->attackAnimTimer = 0.0f;
        goblinArcher->arrow.active = false;
        goblinArcher->arrowDamage = 0.0f;
    }

    if (goblinArcher->isDead && goblinArcher->deathAnimationDone)
        return;

    goblinArcher->frameCounter++;
    if (goblinArcher->frameCounter >= (60 / 10))
    {
        goblinArcher->frameCounter = 0;

        if (goblinArcher->isDead && !goblinArcher->deathAnimationDone)
        {
            goblinArcher->deathAnimTimer += delta;

            if (goblinArcher->deathAnimTimer >= 0.2f)
            {   
                goblinArcher->frameDead++;
                goblinArcher->deathAnimTimer = 0.3f;

                if (goblinArcher->frameDead >= 8)
                {
                    goblinArcher->frameDead = 8;
                    goblinArcher->deathAnimationDone = true;
                    goblinArcher->speed = 0.0f;
        
                }
            }
            goblinArcher->currentFrame = goblinArcher->frameDead;  
        }
        else if (goblinArcher->goblinHasHit && !goblinArcher->isDead)
        {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameHurt;
        }
        else if (goblinArcher->isAtacking)
        {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameAtk;
        }
        else if (goblinArcher->isWalking || goblinArcher->isRunning)
        {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameWalk;
        }
        else if (goblinArcher->isIdle)
        {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameIdle;
        }
    }

    // Cooldowns
    if (goblinArcher->arrowCooldown > 0.0f)
    {
        goblinArcher->arrowCooldown -= delta;
        if (goblinArcher->arrowCooldown < 0.0f)
            goblinArcher->arrowCooldown = 0.0f;
    }

    if (goblinArcher->attackAnimTimer > 0.0f)
    {
        goblinArcher->attackAnimTimer -= delta;
        if (goblinArcher->attackAnimTimer < 0.0f)
            goblinArcher->attackAnimTimer = 0.0f;
    }

    float distanceToPlayer = fabs(player->position.x - goblinArcher->position.x);

    if (distanceToPlayer <= goblinArcher->goblinView && player->life > 0 && !goblinArcher->isDead)
    {
        goblinArcher->direction = (player->position.x < goblinArcher->position.x) ? -1 : 1;

        if (distanceToPlayer <= goblinArcher->attackRange)
        {
            goblinArcher->isWalking = false;

            if (goblinArcher->arrowCooldown <= 0.0f)
            {
                // Lança flecha
                goblinArcher->arrow.active = true;
                goblinArcher->arrow.arrowYOffset = 0.05f;
                goblinArcher->arrowCooldown = 1.5f;
                goblinArcher->attackAnimTimer = 0.4f;
                goblinArcher->arrow.position = (Vector2){
                    goblinArcher->position.x,
                    goblinArcher->position.y + goblinArcher->frameHeight * goblinArcher->arrow.arrowYOffset
                };
                goblinArcher->arrow.scale = 0.2f;
                goblinArcher->arrow.rotation = 0.0f;
                goblinArcher->arrow.direction = goblinArcher->direction;
                goblinArcher->arrow.speed = goblinArcher->arrowSpeed;
            }

            // Controle da animação de ataque separado do cooldown
            if (goblinArcher->attackAnimTimer > 0.0f)
            {
                goblinArcher->isIdle = false;
                goblinArcher->isAtacking = true;
            }
            else
            {
                goblinArcher->isIdle = true;
                goblinArcher->isAtacking = false;
            }
        }
        else
        {
            goblinArcher->isWalking = true;
            goblinArcher->isAtacking = false;
            goblinArcher->isIdle = false;

            goblinArcher->position.x += goblinArcher->speed * goblinArcher->direction * delta;
        }
    }

    // Atualiza a posição da flecha
    if (goblinArcher->arrow.active)
    {
        goblinArcher->arrow.position.x += goblinArcher->arrow.speed * goblinArcher->arrow.direction * delta;

        Rectangle hitbox = GetPlayerHitbox(player);

        if (CheckCollisionArrow(
            goblinArcher->arrow.position.x,
            goblinArcher->arrow.position.y,
            goblinArcher->arrowTexture.width * goblinArcher->arrow.scale,
            goblinArcher->arrowTexture.height * goblinArcher->arrow.scale,
            hitbox.x,
            hitbox.y,
            hitbox.width,
            hitbox.height))
        {
            player->life -= goblinArcher->arrowDamage;
            goblinArcher->arrow.active = false;
            player->hitTimer = 0.4f;   
            goblinArcher->hasHitPlayer = true;
            player->hasHit = true; 
        }
    }

    // Verifica se a flecha saiu da tela
    if (goblinArcher->arrow.active)
    {
        if (goblinArcher->arrow.position.x < 0 ||
            goblinArcher->arrow.position.x > GetScreenWidth())
        {
            goblinArcher->arrow.active = false;
        }
    }
}



void DrawGoblinArcher(GoblinArcher *goblinArcher)
{
    Rectangle source = {
        goblinArcher->currentFrame * goblinArcher->frameWidth,
        0,
        goblinArcher->frameWidth * goblinArcher->direction,
        goblinArcher->frameHeight
    };

    Rectangle dest = {
        goblinArcher->position.x,
        goblinArcher->position.y,
        goblinArcher->frameWidth * goblinArcher->frameFactor,
        goblinArcher->frameHeight * goblinArcher->frameFactor
    };

    Vector2 origin = {0, 0};

    float rotation = 0.0f;

    if (goblinArcher->isDead)
    {
        DrawTexturePro(goblinArcher->spriteDeadGoblinArcher, source, dest, origin, rotation, WHITE);
    }
    else if (goblinArcher->goblinHasHit)
    {
        DrawTexturePro(goblinArcher->spriteHurtGoblinArcher, source, dest, origin, rotation, WHITE);
    }
    else if (goblinArcher->isIdle)
    {
        DrawTexturePro(goblinArcher->spriteIdleGoblinArcher, source, dest, origin, rotation, WHITE);
    }
    else if (goblinArcher->isWalking)
    {
        DrawTexturePro(goblinArcher->spriteWalkGoblinArcher, source, dest, origin, rotation, WHITE);
    }
    else if (goblinArcher->isAtacking)
    {
        DrawTexturePro(goblinArcher->spriteAtkGoblinArcher, source, dest, origin, rotation, WHITE);
    }

    if (goblinArcher->arrow.active)
    {
        DrawTextureEx(
            goblinArcher->arrowTexture,
            goblinArcher->arrow.position,
            goblinArcher->arrow.rotation,
            goblinArcher->arrow.scale,
            WHITE
        );
    }

    if (!goblinArcher->isDead && goblinArcher->life > 0)
    {
        DrawGoblinArcherLifeBar(goblinArcher);
    }
}   


void UnloadGoblinArcher(GoblinArcher *goblinArcher)
{
    UnloadTexture(goblinArcher->spriteWalkGoblinArcher);
    UnloadTexture(goblinArcher->spriteAtkGoblinArcher);
    UnloadTexture(goblinArcher->spriteIdleGoblinArcher);
    UnloadTexture(goblinArcher->spriteDeadGoblinArcher);
    UnloadTexture(goblinArcher->spriteHurtGoblinArcher);
    UnloadTexture(goblinArcher->arrowTexture);
}
