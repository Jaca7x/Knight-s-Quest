#include "wolfRunning.h"

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
    wolfRun->isRuning = false;
    wolfRun->isIdle = true;
    wolfRun->isAttacking = false;
    wolfRun->hasHitPlayer = false;

    wolfRun->attackRange = 100.0f;
    wolfRun->attackCooldown = 0.0f;
    wolfRun->attackDamageTimer = 0.0f;
    wolfRun->damage = 50;
    wolfRun->damageJump = 80;
    wolfRun->viewPlayer = 200.0f;
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
        else if (wolfRun->isRuning)
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
}

void DrawRunningWolf(WolfRun *wolfRun) 
{
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

    DrawTexturePro(wolfRun->spriteIdleWolf, source, dest, origin, 0.0f, WHITE);
}

void UnloadRunningWolf(WolfRun *wolfRun) 
{
    UnloadTexture(wolfRun->spriteAtkWolf);
    UnloadTexture(wolfRun->spriteIdleWolf);
    UnloadTexture(wolfRun->spriteDeadWolf);
}