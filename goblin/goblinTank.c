#include "goblinTank.h"

void InitGoblinTank(GoblinTank *goblinTank)
{
    goblinTank->position = (Vector2){1120, 520};
    goblinTank->life = 100;

    goblinTank->speed = 95.0f;

    goblinTank->currentFrame = 0;
    goblinTank->frameCounter = 0;

    goblinTank->spriteWalkGoblinTank = LoadTexture("resources/sprites/goblinTank/goblinTank-walk.png");
    goblinTank->spriteAtkGoblinTank = LoadTexture("resources/sprites/goblinTank/goblinTank-atk.png");
    goblinTank->spriteIdleGoblinTank = LoadTexture("resources/sprites/goblinTank/goblinTank-idle.png");
    goblinTank->spriteDeadGoblinTank = LoadTexture("resources/sprites/goblinTank/goblinTank-dead.png");
    goblinTank->spriteHurtGoblinTank = LoadTexture("resources/sprites/goblinTank/goblinTank-hurt.png");

    goblinTank->frameWalk = 11;
    goblinTank->frameAtk = 6;
    goblinTank->frameIdle = 8;
    goblinTank->frameDead = 2;
    goblinTank->frameHurt = 2;

    goblinTank->frameWidth = goblinTank->spriteIdleGoblinTank.width / goblinTank->frameIdle;
    goblinTank->frameHeight = goblinTank->spriteIdleGoblinTank.height;

    goblinTank->direction = 1;
    goblinTank->isWalking = false;
    goblinTank->isRunning = false;
    goblinTank->isIdle = true;
    goblinTank->goblinHasHit = false;
    goblinTank->isDead = false;
    goblinTank->isAtacking = false;

    goblinTank->deathAnimTimer = 0.0f;
    goblinTank->deathAnimationDone = false;

    // Ataque
    goblinTank->hasRunAttack = false;
    goblinTank->attackRange = 100.0f; // Distância do ataque
    goblinTank->attackCooldown = 1.5f; // Tempo de recarga do ataque
    goblinTank->hasHitPlayer = false; // Verifica se o jogador foi atingid
}

void UpdateGoblinTank(GoblinTank *goblinTank, Player *player, float delta)
{   

    if (goblinTank->isWalking || goblinTank->isRunning) 
    {
        goblinTank->position.x += goblinTank->speed * delta * goblinTank->direction;
    }

    goblinTank->frameCounter++;
    if (goblinTank->frameCounter >= (60 / 10)) {
    goblinTank->frameCounter = 0;

    if (goblinTank->isDead && !goblinTank->deathAnimationDone) {
        // animação de morte
    }
    else if (goblinTank->goblinHasHit && !goblinTank->isDead) {
        goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameHurt;
    }
    else if (goblinTank->isAtacking) {
        goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameAtk;
    }
    else if (goblinTank->isWalking || goblinTank->isRunning) {
        goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameWalk;
    }
    else if (goblinTank->isIdle) {
        goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameIdle;
    }
}

    printf("currentFrame: %d, frameIdle: %d\n", goblinTank->currentFrame, goblinTank->frameIdle);
    printf("isIdle: %d, isWalking: %d, isAtacking: %d, isDead: %d\n", goblinTank->isIdle, goblinTank->isWalking, goblinTank->isAtacking, goblinTank->isDead);
}

void DrawGoblinTank(GoblinTank *goblinTank)
{
     Rectangle source = {
        goblinTank->currentFrame * goblinTank->frameWidth,
        0,
        goblinTank->frameWidth * goblinTank->direction,
        goblinTank->frameHeight
    };

    Rectangle dest = {
        goblinTank->position.x,
        goblinTank->position.y,
        goblinTank->frameWidth * 0.15f,
        goblinTank->frameHeight * 0.15f
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(goblinTank->spriteIdleGoblinTank, source, dest, origin, 0.0f, WHITE);
}

void UnloadGoblinTank(GoblinTank *goblinTank)
{
    UnloadTexture(goblinTank->spriteWalkGoblinTank);
    UnloadTexture(goblinTank->spriteAtkGoblinTank);
    UnloadTexture(goblinTank->spriteIdleGoblinTank);
    UnloadTexture(goblinTank->spriteDeadGoblinTank);
    UnloadTexture(goblinTank->spriteHurtGoblinTank);
}