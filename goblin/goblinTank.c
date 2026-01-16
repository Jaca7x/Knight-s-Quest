#include "goblinTank.h"

bool CheckCollisionGoblinTank(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitGoblinTank(GoblinTank *goblinTank)
{
    goblinTank->position = (Vector2){800, 500};

    goblinTank->speed = 40.0f;

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

    goblinTank->hitApplied = false;

    goblinTank->deathAnimTimer = 0.0f;

    goblinTank->viewPlayer = 400.0f;

    goblinTank->direction = 1;
    goblinTank->goblinTankAttackRange = 60.0f;
    goblinTank->goblinTankAttackRangeLeft = 180.0f;
    goblinTank->life = 100.0f;

    goblinTank->attackTime = 0.0f;
    goblinTank->attackCooldown = 2.0f;

    goblinTank->attackCooldownTimer = 0.0f;

    goblinTank->attackAnimTimer = 0.0f;
}

void UpdateGoblinTank(GoblinTank *goblinTank, float deltaTime, Player *player)
{
    if (goblinTank->attackCooldownTimer > 0.0f)
            goblinTank->attackCooldownTimer -= deltaTime;

        if (goblinTank->attackAnimTimer > 0.0f)
        {
            goblinTank->attackAnimTimer -= deltaTime;
            goblinTank->isAtacking = true;
        }
        else
        {
            goblinTank->isAtacking = false;
        }
        
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
        else if (goblinTank->isWalking && goblinTank->frameCounter >= 30)
        {
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameWalk;
        }
        else if (goblinTank->isIdle && goblinTank->frameCounter >= 30)
        {
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameIdle;
        }
   
    float push = 80.0f;
    float distanceToViewPlayer = fabs(player->position.x - goblinTank->position.x);
    goblinTank->direction = (player->position.x > goblinTank->position.x) ? 1 : -1;

        if (!goblinTank->isDead && !goblinTank->goblinTankHasHurt && distanceToViewPlayer <= goblinTank->viewPlayer) 
        {
            goblinTank->isWalking = true;
            goblinTank->isIdle = false;
        
            goblinTank->position.x += goblinTank->speed * goblinTank->direction * deltaTime;

            if (distanceToViewPlayer <= goblinTank->goblinTankAttackRange && goblinTank->direction == -1 && goblinTank->attackCooldownTimer <= 0.0f)
            {
                goblinTank->attackAnimTimer = 1.0f;

                if (goblinTank->isAtacking && !goblinTank->hitApplied && goblinTank->currentFrame == 8)
                {
                    goblinTank->goblinTankHasHit = true;
                    player->life -= 15; 
                    player->hasHit = true;
                    player->hitTimer = 0.4f;

                    player->position.x += (player->position.x < goblinTank->position.x) ? -push : push;

                    goblinTank->hitApplied = true;
                }
                
                
                if (goblinTank->attackAnimTimer <= 0)
                {
                    goblinTank->attackCooldownTimer = goblinTank->attackCooldown;

                    goblinTank->hitApplied = false;
                }
            }
            else if (distanceToViewPlayer <= goblinTank->goblinTankAttackRangeLeft && goblinTank->direction == 1 && goblinTank->attackCooldownTimer <= 0.0f)
            {
                
                player->life -= 15;
                goblinTank->goblinTankHasHit = true;
                player->hasHit = true;
                player->hitTimer = 0.4f;

                // Knockback
                player->position.x += (player->position.x < goblinTank->position.x) ? -push : push;

                goblinTank->attackCooldownTimer = goblinTank->attackCooldown;
                goblinTank->attackAnimTimer = 1.0f;
            }
        }
        else 
        {
            goblinTank->isIdle = true;
            goblinTank->isWalking = false;
            goblinTank->isAtacking = false;
            goblinTank->goblinTankHasHit = false;
        }

        if (!goblinTank->isDead && CheckCollisionGoblinTank(
                player->position.x, player->position.y, player->frameWidth , player->frameHeight,
                goblinTank->position.x, goblinTank->position.y, goblinTank->frameWidthHurt / 35 , goblinTank->frameHeightHurt / 35))
        {
            player->position.x += (player->position.x < goblinTank->position.x) ? -push : push;
        }
   }

void DrawGoblinTank(GoblinTank *goblinTank)
{ 
    Rectangle source = {0, 0, 0, 0};
    Texture2D currentTexture;
    Rectangle dest = {0, 0, 0, 0};

    if (goblinTank->goblinTankHasHurt && !goblinTank->isDead)
    {
        currentTexture = goblinTank->goblinTankSpriteHurt;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthHurt, 0, goblinTank->frameWidthHurt * goblinTank->direction, goblinTank->frameHeightHurt};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / 4, source.height / 4};
    }
    else if (goblinTank->isDead)
    {
        currentTexture = goblinTank->goblinTankSpriteDead;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthDead, 0, goblinTank->frameWidthDead * goblinTank->direction, goblinTank->frameHeightDead};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / 4, source.height / 4};
    }
    else if (goblinTank->isAtacking)
    {
        float hurtOfSetY = -55;

        currentTexture = goblinTank->goblinTankSpriteAtk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthAttack, 0, goblinTank->frameWidthAttack * goblinTank->direction, goblinTank->frameHeightAttack};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / 4, source.height / 4};

        dest.y += hurtOfSetY;
    }
    else if (goblinTank->isWalking)
    {
        float hurtOfSetY = -10;

        currentTexture = goblinTank->goblinTankSpriteWalk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthWalk, 0, goblinTank->frameWidthWalk * goblinTank->direction, goblinTank->frameHeightWalk};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / 4, source.height / 4};

        dest.y += hurtOfSetY;
    }   
    else if (goblinTank->isIdle)
    {
        float hurtOfSetY = -15;

        currentTexture = goblinTank->goblinTankSpriteIdle;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthIdle, 0, goblinTank->frameWidthIdle * goblinTank->direction, goblinTank->frameHeightIdle};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / 5, source.height / 5};

        dest.y += hurtOfSetY;
    }
    
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