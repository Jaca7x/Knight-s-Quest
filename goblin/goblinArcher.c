#include "goblinArcher.h"

void InitGoblinArcher(GoblinArcher *goblinArcher)
{
    goblinArcher->position = (Vector2){1120, 520};
    goblinArcher->life = 100;

    goblinArcher->speed = 95.0f;

    goblinArcher->currentFrame = 0;
    goblinArcher->frameCounter = 0;

    goblinArcher->spriteWalkGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-walk.png");
    goblinArcher->spriteAtkGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-atk.png");
    goblinArcher->spriteIdleGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-idle.png");
    goblinArcher->spriteDeadGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-dead.png");
    goblinArcher->spriteHurtGoblinArcher = LoadTexture("resources/sprites/goblinArcher/goblinArcher-hurt.png");
    goblinArcher->arrow = LoadTexture("resources/sprites/goblinArcher/Arrow.png");

    goblinArcher->frameWalk = 6;
    goblinArcher->frameAtk = 9;
    goblinArcher->frameIdle = 8;
    goblinArcher->frameDead = 9;
    goblinArcher->frameHurt = 4;

    goblinArcher->frameWidth = goblinArcher->spriteIdleGoblinArcher.width / goblinArcher->frameIdle;
    goblinArcher->frameHeight = goblinArcher->spriteIdleGoblinArcher.height;

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
    goblinArcher->hasRunAttack = false;
    goblinArcher->attackRange = 100.0f;
    goblinArcher->attackCooldown = 1.5f;
    goblinArcher->hasHitPlayer = false;
}

void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta)
{
    if (goblinArcher->isWalking || goblinArcher->isRunning)
    {
        goblinArcher->position.x += goblinArcher->speed * delta * goblinArcher->direction;
    }

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

    printf("currentFrame: %d, frameIdle: %d\n", goblinArcher->currentFrame, goblinArcher->frameIdle);
    printf("isIdle: %d, isWalking: %d, isAtacking: %d, isDead: %d\n", goblinArcher->isIdle, goblinArcher->isWalking, goblinArcher->isAtacking, goblinArcher->isDead);
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

    DrawTexturePro(goblinArcher->spriteIdleGoblinArcher, source, dest, origin, 0.0f, WHITE);
}

void UnloadGoblinArcher(GoblinArcher *goblinArcher)
{
    UnloadTexture(goblinArcher->spriteWalkGoblinArcher);
    UnloadTexture(goblinArcher->spriteAtkGoblinArcher);
    UnloadTexture(goblinArcher->spriteIdleGoblinArcher);
    UnloadTexture(goblinArcher->spriteDeadGoblinArcher);
    UnloadTexture(goblinArcher->spriteHurtGoblinArcher);
}
