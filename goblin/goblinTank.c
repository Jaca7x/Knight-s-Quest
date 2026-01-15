#include "goblinTank.h"

void InitGoblinTank(GoblinTank *goblinTank)
{
    goblinTank->position = (Vector2){500, 500};

    goblinTank->speed = 60.0f;

    goblinTank->goblinTankSpriteWalk = LoadTexture("resources/sprites/goblinTank/goblinTank-walk.png");
    goblinTank->goblinTankSpriteHurt = LoadTexture("resources/sprites/goblinTank/goblinTank-hurt.png");
    goblinTank->goblinTankSpriteDead = LoadTexture("resources/sprites/goblinTank/goblinTank-dead.png");
    goblinTank->goblinTankSpriteIdle = LoadTexture("resources/sprites/goblinTank/goblinTank-idle.png");
    goblinTank->goblinTankSpriteAtk = LoadTexture("resources/sprites/goblinTank/goblinTank-attack.png");

    goblinTank->frameWalk = 6;
    goblinTank->frameHurt = 4;
    goblinTank->frameDead = 9;
    goblinTank->frameIdle = 8;
    goblinTank->frameAtk = 10;

    goblinTank->currentFrame = 0;
    goblinTank->frameCounter = 0;

    goblinTank->frameWidthWalk = goblinTank->goblinTankSpriteWalk.width / goblinTank->frameWalk;
    goblinTank->frameHeightWalk = goblinTank->goblinTankSpriteWalk.height;

    goblinTank->frameWidthIdle = goblinTank->goblinTankSpriteIdle.width / goblinTank->frameIdle;
    goblinTank->frameHeightIdle = goblinTank->goblinTankSpriteIdle.height;

    goblinTank->frameWidthAttack = goblinTank->goblinTankSpriteAtk.width / goblinTank->frameAtk;
    goblinTank->frameHeightAttack = goblinTank->goblinTankSpriteAtk.height;

    goblinTank->frameWidthDead = goblinTank->goblinTankSpriteDead.width / goblinTank->frameDead;
    goblinTank->frameHeightDead = goblinTank->goblinTankSpriteDead.height;

    goblinTank->frameWidthHurt = goblinTank->goblinTankSpriteHurt.width / goblinTank->frameHurt;
    goblinTank->frameHeightHurt = goblinTank->goblinTankSpriteHurt.height;

    goblinTank->isIdle = true;
    goblinTank->isWalking = false;
    goblinTank->isAtacking = false;
    goblinTank->isDead = false;
    
    goblinTank->goblinTankHasHurt = false;
    goblinTank->goblinTankHasHit = false;
    goblinTank->deathAnimationDone = false;

    goblinTank->deathAnimTimer = 0.0f;

    goblinTank->direction = 1;
    goblinTank->goblinTankAttackRange = 100.0f;
    goblinTank->life = 100.0f;

    goblinTank->attackTime = 0.0f;
    goblinTank->attackCooldown = 1.0f;
}

void UpdateGoblinTank(GoblinTank *goblinTank, float deltaTime, Player *player)
{
    goblinTank->frameCounter++;

        if (goblinTank->goblinTankHasHurt && !goblinTank->isDead && goblinTank->frameCounter >= 30)
        {
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameHurt;
        }
        else if (goblinTank->isDead && !goblinTank->deathAnimationDone && goblinTank->frameCounter >= 30)
        {
            goblinTank->deathAnimTimer += deltaTime;

            goblinTank->frameCounter = 0;

            if (goblinTank->deathAnimTimer >= 2.0f)
            {
                goblinTank->frameDead++;
                goblinTank->deathAnimTimer = 0.0f;

                if (goblinTank->currentFrame == 6)
                {
                    goblinTank->deathAnimationDone = true;
                }
            }
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameDead;
        }
        else if (goblinTank->isAtacking && goblinTank->frameCounter >= 10)
        {
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameAtk;
        }
        else if (goblinTank->isWalking && goblinTank->frameCounter >= 20)
        {
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameWalk;
        }
        else if (goblinTank->isIdle && goblinTank->frameCounter >= 30)
        {
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameIdle;
        }
   
    float push = 50.0f;
    float distanceToViewPlayer = fabs(player->position.x - goblinTank->position.x);
    goblinTank->direction = (player->position.x > goblinTank->position.x) ? 1 : -1;

   if (goblinTank->attackTime > 0.0f)
   {
        goblinTank->attackTime -= deltaTime;
        goblinTank->isAtacking = true;
        goblinTank->isWalking = false;
        goblinTank->isIdle = false;
   }
   else 
   {
        if (!goblinTank->isDead && !goblinTank->goblinTankHasHurt) 
        {
            goblinTank->isAtacking = false;
            goblinTank->isWalking = true;
            goblinTank->isIdle = false;

            goblinTank->position.x += goblinTank->speed * goblinTank->direction * deltaTime;

            if (distanceToViewPlayer <= goblinTank->goblinTankAttackRange && goblinTank->direction == -1)
            {
                goblinTank->isAtacking = true;
                goblinTank->attackTime = goblinTank->attackCooldown;
                goblinTank->goblinTankHasHit = true;
                player->life -= 15; 
                player->hasHit = true;
                player->hitTimer = 0.4f;

                player->position.x += (player->position.x < goblinTank->position.x) ? -push : push;
            }
            else if (distanceToViewPlayer <= goblinTank->goblinTankAttackRange && goblinTank->direction == 1)
            {
                goblinTank->isAtacking = true;
                goblinTank->attackTime = goblinTank->attackCooldown;
                player->life -= 15;
                goblinTank->goblinTankHasHit = true;
                player->hasHit = true;
                player->hitTimer = 0.4f;

                // Knockback
                player->position.x += (player->position.x < goblinTank->position.x) ? -push : push;
            }
        }
        else
        {
            goblinTank->isIdle = true;
            goblinTank->isWalking = false;
            goblinTank->isAtacking = false;
            goblinTank->goblinTankHasHit = false;
        }
   }
}

void DrawGoblinTank(GoblinTank *goblinTank)
{
    Rectangle source = {0, 0, 0, 0};
    Texture2D currentTexture;

    if (goblinTank->goblinTankHasHurt && !goblinTank->isDead)
    {
        currentTexture = goblinTank->goblinTankSpriteHurt;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthHurt, 0, goblinTank->frameWidthHurt * goblinTank->direction, goblinTank->frameHeightHurt};
    }
    else if (goblinTank->isDead)
    {
        currentTexture = goblinTank->goblinTankSpriteDead;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthDead, 0, goblinTank->frameWidthDead * goblinTank->direction, goblinTank->frameHeightDead};
    }
    else if (goblinTank->isAtacking)
    {
        currentTexture = goblinTank->goblinTankSpriteAtk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthAttack, 0, goblinTank->frameWidthAttack * goblinTank->direction, goblinTank->frameHeightAttack};
    }
    else if (goblinTank->isWalking)
    {
        currentTexture = goblinTank->goblinTankSpriteWalk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthWalk, 0, goblinTank->frameWidthWalk * goblinTank->direction, goblinTank->frameHeightWalk};
    }
    else if (goblinTank->isIdle)
    {
        currentTexture = goblinTank->goblinTankSpriteIdle;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthIdle, 0, goblinTank->frameWidthIdle * goblinTank->direction, goblinTank->frameHeightIdle};
    }

    Rectangle dest = {goblinTank->position.x, goblinTank->position.y, source.width / 4, source.height / 4};
    Vector2 origin = {0, 0};
    float rotation = 0.0f;

    DrawTexturePro(currentTexture, source, dest, origin, rotation, WHITE);
}

void UnloadGoblinTank(GoblinTank *goblinTank)
{
    UnloadTexture(goblinTank->goblinTankSpriteWalk);
    UnloadTexture(goblinTank->goblinTankSpriteHurt);
    UnloadTexture(goblinTank->goblinTankSpriteDead);
    UnloadTexture(goblinTank->goblinTankSpriteIdle);
    UnloadTexture(goblinTank->goblinTankSpriteAtk);
}