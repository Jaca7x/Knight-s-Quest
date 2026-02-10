#include "goblinTank.h"

bool CheckColisionGoblinTank(float x1, float y1, float w1, float h1,
                             float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}


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

    goblinTank->scale = 4;
    goblinTank->scaleIdle = 5;

    goblinTank->frameWalk = 6;
    goblinTank->frameHurt = 4;
    goblinTank->frameDead = 9;
    goblinTank->frameIdle = 8;
    goblinTank->frameAtk = 10;

    goblinTank->currentFrame = 0;
    goblinTank->frameCounter = 0;

    goblinTank->damage = 25;

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
    goblinTank->isAttacking = false;
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
    goblinTank->goblinTankAttackRange = 40.0f;
    goblinTank->goblinTankAttackRangeRight = 180.0f;

    goblinTank->attackTime = 0.0f;
    goblinTank->attackCooldown = 1.0f;

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
        goblinTank->isAttacking = false;
        goblinTank->deathAnimationDone = false;
        goblinTank->goblinTankHasHurt = false;
        goblinTank->goblinTankHasHit = false;
        goblinTank->currentFrame = 0;
        goblinTank->frameDead = 7;
    }

    if (goblinTank->isDead && goblinTank->deathAnimationDone)
        return;
    
    if (goblinTank->attackCooldownTimer > 0.0f) goblinTank->attackCooldownTimer -= deltaTime;

        if (goblinTank->attackAnimTimer > 0.0f)
        {
            goblinTank->attackAnimTimer -= deltaTime;
            goblinTank->isAttacking = true;
        }
        else
        {
            goblinTank->isAttacking = false;
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
        else if (goblinTank->isAttacking && goblinTank->frameCounter >= 10)
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

                if (goblinTank->isAttacking && !goblinTank->hitApplied && goblinTank->currentFrame == 8)
                {
                    player->life -= goblinTank->damage;
                    goblinTank->goblinTankHasHit = true;
                    player->hasHit = true;
                    player->hitTimer = 0.4f;

                    goblinTank->attackCooldownTimer = goblinTank->attackCooldown;
                }
            }
            else if (distanceToViewPlayer <= goblinTank->goblinTankAttackRangeRight && goblinTank->direction == 1 && goblinTank->attackCooldownTimer <= 0.0f)
            {
                goblinTank->attackAnimTimer = 1.0f;

                if (goblinTank->isAttacking && !goblinTank->hitApplied && goblinTank->currentFrame == 8)
                {
                    player->life -= goblinTank->damage;
                    goblinTank->goblinTankHasHit = true;
                    player->hasHit = true;
                    player->hitTimer = 0.4f;

                    goblinTank->attackCooldownTimer = goblinTank->attackCooldown;
                }
            }
        }
        else 
        {
            goblinTank->isIdle = true;
            goblinTank->isWalking = false;
            goblinTank->isAttacking = false;
            goblinTank->goblinTankHasHit = false;
        }
        
        float attackOffsetX;

        if (goblinTank->direction == -1) 
        {
            if (goblinTank->isAttacking)
            {
                if (goblinTank->currentFrame >= 3)
                {
                    attackOffsetX = GOBLIN_ATTACK_LEFT_STRIKE;
                }
                else
                {
                    attackOffsetX = GOBLIN_ATTACK_LEFT_IDLE;
                }
            }
            else
            attackOffsetX = GOBLIN_ATTACK_LEFT_IDLE;
        }
        else
        {
            if (goblinTank->isAttacking)
            {
                if (goblinTank->currentFrame >= 3)
                {
                    attackOffsetX = GOBLIN_ATTACK_RIGHT_STRIKE;
                }
                else
                {
                    attackOffsetX = GOBLIN_ATTACK_RIGHT_IDLE;
                }
            }
            else
            attackOffsetX = GOBLIN_ATTACK_RIGHT_IDLE;
        }

        if (!goblinTank->isDead &&
        CheckColisionGoblinTank(
        player->position.x + PLAYER_HITBOX_OFFSET_X,
        player->position.y + PLAYER_HITBOX_OFFSET_Y,
        player->frameWidth / PLAYER_HITBOX_WIDTH_DIV,
        player->frameHeight,

        goblinTank->position.x + attackOffsetX,
        goblinTank->position.y + GOBLIN_TANK_OFFSET_Y,
        goblinTank->frameWidthAttack / GOBLIN_TANK_HITBOX_WITH_DIV,
        goblinTank->frameHeightAttack / GOBLIN_TANK_HITBOX_HEIGHT_DIV
        ))
        {
            player->position.x += (goblinTank->direction == -1 ? -push : push);
        }       
}
        
void DrawGoblinTank(GoblinTank *goblinTank, Player *player)
{ 
    Rectangle source = {0, 0, 0, 0};
    Texture2D currentTexture;
    Rectangle dest = {0, 0, 0, 0};

    if (goblinTank->goblinTankHasHurt && !goblinTank->isDead)
    {
        float hurtOfSetY = -15;
        currentTexture = goblinTank->goblinTankSpriteHurt;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthHurt, 0, goblinTank->frameWidthHurt * goblinTank->direction, goblinTank->frameHeightHurt};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOfSetY;
    }
    else if (goblinTank->isDead)
    {
        float hurtOfSetY = -5;

        currentTexture = goblinTank->goblinTankSpriteDead;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthDead, 0, goblinTank->frameWidthDead * goblinTank->direction, goblinTank->frameHeightDead};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOfSetY;
    }
    else if (goblinTank->isAttacking)
    {
        float hurtOfSetY = -55;

        currentTexture = goblinTank->goblinTankSpriteAtk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthAttack, 0, goblinTank->frameWidthAttack * goblinTank->direction, goblinTank->frameHeightAttack};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOfSetY;
    }
    else if (goblinTank->isWalking)
    {
        float hurtOfSetY = -10;

        currentTexture = goblinTank->goblinTankSpriteWalk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthWalk, 0, goblinTank->frameWidthWalk * goblinTank->direction, goblinTank->frameHeightWalk};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOfSetY;
    }   
    else if (goblinTank->isIdle)
    {
        float hurtOfSetY = -15;

        currentTexture = goblinTank->goblinTankSpriteIdle;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthIdle, 0, goblinTank->frameWidthIdle * goblinTank->direction, goblinTank->frameHeightIdle};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scaleIdle , source.height / goblinTank->scaleIdle};

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