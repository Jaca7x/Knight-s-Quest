#include "goblinTank.h"

void DrawGoblinTankLifeBar(GoblinTank *goblinTank)
{
    if(goblinTank->isDead) return;

    float barWidth = 60.0f;
    float barHeight = 8.0f;
    float x = goblinTank->position.x + 140; 
    float y = goblinTank->position.y - 15;

    float lifePercent = goblinTank->life / goblinTank->maxLife;

    if (lifePercent < 0) lifePercent = 0;
    if (lifePercent > 1) lifePercent = 1;

    float currentBarWidth = barWidth * lifePercent;

    DrawRectangle(x, y, barWidth, barHeight, RED);

    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN);

    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
}

void InitGoblinTank(GoblinTank *goblinTank)
{
    goblinTank->position = (Vector2){800, 500};

    goblinTank->speed = 40.0f;
    goblinTank->maxLife = 200.0f;
    goblinTank->life = goblinTank->maxLife;

    
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

    goblinTank->damage = 35;

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

    goblinTank->growlSoundPlay = false;

    goblinTank->droppedHeart = false;

    goblinTank->deathAnimTimer = 0.0f;

    goblinTank->viewPlayer = 400.0f;

    goblinTank->direction = 1;
    goblinTank->goblinTankAttackRange = 60.0f;
    goblinTank->goblinTankAttackRangeLeft = 100.0f;

    goblinTank->attackTime = 0.0f;
    goblinTank->attackCooldown = 3.0f;

    goblinTank->attackCooldownTimer = 0.0f;

    goblinTank->attackAnimTimer = 0.0f;

    goblinTank->soundAttackGoblinTank = LoadSound("resources/sounds/sound_effects/goblin/attack-goblinTank.wav");
    goblinTank->soundGrowlGoblinTank = LoadSound("resources/sounds/sound_effects/goblin/idle-goblinTank.wav");
    goblinTank->soundHurtGoblinTank = LoadSound("resources/sounds/sound_effects/goblin/hurt-goblinTank.wav");
    goblinTank->soundDeathGolbinTank = LoadSound("resources/sounds/sound_effects/goblin/death-goblinTank.wav");
}

void UpdateGoblinTank(GoblinTank *goblinTank, float deltaTime, Player *player)
{
    if (goblinTank->life <= 0 && !goblinTank->isDead)
    {
        StopSound(goblinTank->soundGrowlGoblinTank);
        PlaySound(goblinTank->soundDeathGolbinTank);
        goblinTank->isDead = true;
        goblinTank->isWalking = false;
        goblinTank->isIdle = false;
        goblinTank->isAtacking = false;
        goblinTank->deathAnimationDone = false;
        goblinTank->goblinTankHasHurt = false;
        goblinTank->goblinTankHasHit = false;
        goblinTank->currentFrame = 0;
        goblinTank->frameDead = 7;
    }

    if (goblinTank->isDead && goblinTank->deathAnimationDone)
        return;
    
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
        else if (goblinTank->isDead && !goblinTank->deathAnimationDone)
        {
            goblinTank->deathAnimTimer += deltaTime;

            if (goblinTank->deathAnimTimer >= 0.2f)
            {   
                goblinTank->deathAnimTimer = 0.0f;
                goblinTank->currentFrame++;

                if (goblinTank->currentFrame >= goblinTank->frameDead - 1)
                {
                    goblinTank->currentFrame = goblinTank->frameDead - 1;
                    goblinTank->deathAnimationDone = true;
                }
            }
        }
        else if (goblinTank->isAtacking && goblinTank->frameCounter >= 10)
        {
            if (goblinTank->currentFrame == 5)
            {
                PlaySound(goblinTank->soundAttackGoblinTank);
            }
            
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameAtk;
        }
        else if (goblinTank->isWalking && goblinTank->frameCounter >= 30)
        {
            PlaySound(player->walkingInGrass);
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameWalk;
        }
        else if (goblinTank->isIdle && goblinTank->frameCounter >= 30)
        {
            if (!goblinTank->growlSoundPlay)
            {
                PlaySound(goblinTank->soundGrowlGoblinTank);
                goblinTank->growlSoundPlay = true;
            }
            
            goblinTank->frameCounter = 0;
            goblinTank->currentFrame = (goblinTank->currentFrame + 1) % goblinTank->frameIdle;
        }
   
    float push = 80.0f;
    float distanceToViewPlayer = fabs(player->position.x - goblinTank->position.x);

    float turnOffset = 100.0f;

    if (player->position.x > goblinTank->position.x + turnOffset)
    {
        goblinTank->direction = 1;  
    }
    else if (player->position.x < goblinTank->position.x - turnOffset)
    {
        goblinTank->direction = -1; 
    }

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
                    player->life -= goblinTank->damage;
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
                player->life -= goblinTank->damage;
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
   }

void DrawGoblinTank(GoblinTank *goblinTank)
{ 
    Rectangle source = {0, 0, 0, 0};
    Texture2D currentTexture;
    Rectangle dest = {0, 0, 0, 0};

    if (goblinTank->goblinTankHasHurt && !goblinTank->isDead)
    {
        float hurtOfSetY = -15;
        currentTexture = goblinTank->goblinTankSpriteHurt;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthHurt, 0, goblinTank->frameWidthHurt * goblinTank->direction, goblinTank->frameHeightHurt};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / 4, source.height / 4};

        dest.y += hurtOfSetY;
    }
    else if (goblinTank->isDead)
    {
        float hurtOfSetY = -5;

        currentTexture = goblinTank->goblinTankSpriteDead;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthDead, 0, goblinTank->frameWidthDead * goblinTank->direction, goblinTank->frameHeightDead};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / 4, source.height / 4};

        dest.y += hurtOfSetY;
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

    DrawGoblinTankLifeBar(goblinTank);
    
}

void UnloadGoblinTank(GoblinTank *goblinTank)
{
    UnloadTexture(goblinTank->goblinTankSpriteWalk);
    UnloadTexture(goblinTank->goblinTankSpriteHurt);
    UnloadTexture(goblinTank->goblinTankSpriteDead);
    UnloadTexture(goblinTank->goblinTankSpriteIdle);
    UnloadTexture(goblinTank->goblinTankSpriteAtk);
    UnloadSound(goblinTank->soundAttackGoblinTank);
    UnloadSound(goblinTank->soundGrowlGoblinTank);
    UnloadSound(goblinTank->soundHurtGoblinTank);
    UnloadSound(goblinTank->soundDeathGolbinTank);
}