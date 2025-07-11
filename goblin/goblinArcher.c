#include "goblinArcher.h"

void InitGoblinArcher(GoblinArcher *goblinArcher)
{
    goblinArcher->position = (Vector2){1120, 567};
    goblinArcher->life = 100;

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

    goblinArcher->arrowSpeed = 450.0f;
    goblinArcher->arrowTolerance = 90; // Tolerância para considerar acerto
    goblinArcher->arrowDamage = 10;
    goblinArcher->arrowFired = false; // Inicialmente a flecha não foi disparada

    goblinArcher->direction = 1;
    goblinArcher->isWalking = false;
    goblinArcher->isRunning = false;
    goblinArcher->isIdle = true;
    goblinArcher->goblinHasHit = false;
    goblinArcher->isDead = false;
    goblinArcher->isAtacking = false;

    goblinArcher->deathAnimTimer = 0.0f;
    goblinArcher->deathAnimationDone = false;

    // Ataque
    goblinArcher->goblinView = 600.0f;
    goblinArcher->attackRange = 400.0f; // Distância de ataque
    goblinArcher->attackCooldown = 1.5f;
    goblinArcher->hasHitPlayer = false;
}

void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta)
{

    if (goblinArcher->life <= 0 && !goblinArcher->isDead) 
    {
        goblinArcher->isDead = true;
        goblinArcher->goblinHasHit = false;
        goblinArcher->currentFrame = 0;
        goblinArcher->hasHitPlayer = false;
        goblinArcher->deathAnimTimer = 0.0f; 
        goblinArcher->frameDead = 7;
        goblinArcher->deathAnimationDone = false;
        goblinArcher->isWalking = false;
        goblinArcher->isAtacking = false;
        goblinArcher->isIdle = false;
        goblinArcher->arrowFired = false;
    }

    if (goblinArcher->isDead && goblinArcher->deathAnimationDone)
        return;
    
    goblinArcher->frameCounter++;
    if (goblinArcher->frameCounter >= (60 / 10)) {
        goblinArcher->frameCounter = 0;

        if (goblinArcher->isDead && !goblinArcher->deathAnimationDone) {
            // animação de morte
        }
        else if (goblinArcher->goblinHasHit && !goblinArcher->isDead) {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameHurt;
        }
        else if (goblinArcher->isAtacking) {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameAtk;
        }
        else if (goblinArcher->isWalking || goblinArcher->isRunning) {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameWalk;
        }
        else if (goblinArcher->isIdle) {
            goblinArcher->currentFrame = (goblinArcher->currentFrame + 1) % goblinArcher->frameIdle;
        }
    }

   float distanceToPlayer = fabs(player->position.x - goblinArcher->position.x);

    if (distanceToPlayer <= goblinArcher->goblinView)
    {
        // Definir direção para o player
        if (player->position.x < goblinArcher->position.x)
            goblinArcher->direction = -1;
        else
            goblinArcher->direction = 1;

        // Se estiver fora da range de ataque, anda até o player
        if (distanceToPlayer <= goblinArcher->attackRange)
        {
             // Está dentro da range: ataca
            goblinArcher->isAtacking = true;
            goblinArcher->isWalking = false;
            goblinArcher->isIdle = false;

            // Se flecha não foi disparada ainda
            if (!goblinArcher->arrow.active)
            {
                goblinArcher->arrow.active = true;
                goblinArcher->hasHitPlayer = false;
                goblinArcher->arrow.position = (Vector2){
                    goblinArcher->position.x,
                    goblinArcher->position.y + goblinArcher->frameHeight * 0.05f // ajusta para sair da mão
                };
                goblinArcher->arrow.direction = goblinArcher->direction;
                goblinArcher->arrow.speed = goblinArcher->arrowSpeed;
            }
        }
        else
        {
            goblinArcher->isWalking = true;
            goblinArcher->isIdle = false;
            goblinArcher->isAtacking = false;
            goblinArcher->hasHitPlayer = false;

            goblinArcher->position.x += goblinArcher->speed * goblinArcher->direction * delta;
        }
    }
    else
    {
        // Fora da visão, fica parado
        goblinArcher->isIdle = true;
        goblinArcher->isWalking = false;
        goblinArcher->isAtacking = false;
    }

     // Atualiza flecha se ativa
    if (goblinArcher->arrow.active)
    {
        goblinArcher->arrow.position.x += goblinArcher->arrow.speed * goblinArcher->arrow.direction * delta;
        goblinArcher->hasHitPlayer = false;

        // Colisão simples: se chegou perto do player
        if (fabs(goblinArcher->arrow.position.x - player->position.x) < goblinArcher->arrowTolerance) // ajuste tolerância
        {
            player->life -= goblinArcher->arrowDamage;
            goblinArcher->arrow.active = false;
            goblinArcher->hasHitPlayer = true;
        }

        // Se saiu da tela, desativa
        if (goblinArcher->arrow.position.x < 0 || goblinArcher->arrow.position.x > GetScreenWidth())
        {
            goblinArcher->arrow.active = false;
            goblinArcher->hasHitPlayer = false;
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
        goblinArcher->frameWidth * 0.15f,
        goblinArcher->frameHeight * 0.15f
    };

    Vector2 origin = {0, 0};

   if (goblinArcher->isDead)
    {
        DrawTexturePro(goblinArcher->spriteDeadGoblinArcher, source, dest, origin, 0.0f, WHITE);
    }
    else if (goblinArcher->goblinHasHit)
    {
       DrawTexturePro(goblinArcher->spriteHurtGoblinArcher, source, dest, origin, 0.0f, WHITE); 
    }
    else if (goblinArcher->isIdle)
    {
        DrawTexturePro(goblinArcher->spriteIdleGoblinArcher, source, dest, origin, 0.0f, WHITE);
    } 
    else if (goblinArcher->isWalking)
    {
        DrawTexturePro(goblinArcher->spriteWalkGoblinArcher, source, dest, origin, 0.0f, WHITE);
    } 
    else if (goblinArcher->isAtacking)
    {
        DrawTexturePro(goblinArcher->spriteAtkGoblinArcher, source, dest, origin, 0.0f, WHITE);
    }

    if (goblinArcher->arrow.active)
{
    DrawTextureEx(
        goblinArcher->arrowTexture,
        goblinArcher->arrow.position,
        0.0f,
        0.2f,  // escala (ajuste se quiser maior)
        WHITE
    );
}
}

void UnloadGoblinArcher(GoblinArcher *goblinArcher)
{
    UnloadTexture(goblinArcher->spriteWalkGoblinArcher);
    UnloadTexture(goblinArcher->spriteAtkGoblinArcher);
    UnloadTexture(goblinArcher->spriteIdleGoblinArcher);
    UnloadTexture(goblinArcher->spriteDeadGoblinArcher);
    UnloadTexture(goblinArcher->spriteHurtGoblinArcher);
}
