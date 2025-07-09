#include "goblinTank.h"

void InitGoblinTank(GoblinTank *goblinTank)
{
    goblinTank->position = (Vector2){1120, 450};
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

    goblinTank->frameWidth = goblinTank->spriteWalkGoblinTank.width / goblinTank->frameWalk;
    goblinTank->frameHeight = goblinTank->spriteWalkGoblinTank.height;

    goblinTank->direction = 1;
    goblinTank->isWalking = false;
    goblinTank->isRunning = false;
    goblinTank->isIdle = true;
    goblinTank->goblinHasHit = false;
    goblinTank->isDead = false;

    goblinTank->deathAnimTimer = 0.0f;
    goblinTank->deathAnimationDone = false;

    // Ataque
    goblinTank->isAttacking = false;
    goblinTank->hasRunAttack = false;
    goblinTank->attackRange = 100.0f; // Distância do ataque
    goblinTank->attackCooldown = 1.5f; // Tempo de recarga do ataque
    goblinTank->hasHitPlayer = false; // Verifica se o jogador foi atingid
}

void UpdateGoblinTank(GoblinTank *goblinTank, Player *player, float delta)
{

}

void DrawGoblinTank(GoblinTank *goblinTank)
{

}

void UnloadGoblinTank(GoblinTank *goblinTank)
{
    UnloadTexture(goblinTank->spriteWalkGoblinTank);
    UnloadTexture(goblinTank->spriteAtkGoblinTank);
    UnloadTexture(goblinTank->spriteIdleGoblinTank);
    UnloadTexture(goblinTank->spriteDeadGoblinTank);
    UnloadTexture(goblinTank->spriteHurtGoblinTank);
}