#include "goblinTank.h"

bool CheckColisionGoblinTank(float x1, float y1, float w1, float h1,
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
    goblinTank->distanceToTurn = 100.0f;
    goblinTank->goblinTankAttackRange = 40.0f;
    goblinTank->goblinTankAttackRangeRight = 180.0f;

    goblinTank->push = 50;
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
    if (goblinTank->life <= LIFE_ZERO && !goblinTank->isDead)
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
        goblinTank->currentFrame = CURRENT_FRAME_ZERO;
        goblinTank->frameDead = FRAME_OF_DEATH;
    }

    if (goblinTank->isDead && goblinTank->deathAnimationDone)
        return;
    
    if (goblinTank->attackCooldownTimer > COOL_DOWN_ZERO) goblinTank->attackCooldownTimer -= deltaTime;

        if (goblinTank->attackAnimTimer > COOL_DOWN_ZERO)
        {
            goblinTank->attackAnimTimer -= deltaTime;
            goblinTank->isAttacking = true;
        }
        else
        {
            goblinTank->isAttacking = false;
        }
        
     goblinTank->frameCounter++;

        if (goblinTank->goblinTankHasHurt && !goblinTank->isDead && goblinTank->frameCounter >= GOBLIN_TANK_FRAME_DELAY)
        {
            goblinTank->frameCounter = FRAME_COUNTER_ZERO;
            goblinTank->currentFrame = (goblinTank->currentFrame + NEXT_FRAME) % goblinTank->frameHurt;
        }
        else if (goblinTank->isDead && !goblinTank->deathAnimationDone)
        {
            goblinTank->deathAnimTimer += deltaTime;

            if (goblinTank->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
            {   
                goblinTank->deathAnimTimer = COOL_DOWN_ZERO;
                goblinTank->currentFrame++;

                if (goblinTank->currentFrame >= goblinTank->frameDead - PREVIOUS_FRAME)
                {
                    goblinTank->currentFrame = goblinTank->frameDead - PREVIOUS_FRAME;
                    goblinTank->deathAnimationDone = true;
                }
            }
        }
        else if (goblinTank->isAttacking && goblinTank->frameCounter >= GOBLIN_TANK_ATTACK_FRAME_DELAY)
        {
            if (goblinTank->currentFrame == FRAME_TO_SOUND_ATTACK)
            {
                PlaySound(goblinTank->soundAttackGoblinTank);
            }
            
            goblinTank->frameCounter = FRAME_COUNTER_ZERO;
            goblinTank->currentFrame = (goblinTank->currentFrame + NEXT_FRAME) % goblinTank->frameAtk;
        }
        else if (goblinTank->isWalking && goblinTank->frameCounter >= GOBLIN_TANK_FRAME_DELAY)
        {
            goblinTank->frameCounter = FRAME_COUNTER_ZERO;
            goblinTank->currentFrame = (goblinTank->currentFrame + NEXT_FRAME) % goblinTank->frameWalk;
        }
        else if (goblinTank->isIdle && goblinTank->frameCounter >= GOBLIN_TANK_FRAME_DELAY)
        {
            if (!goblinTank->growlSoundPlay)
            {
                PlaySound(goblinTank->soundGrowlGoblinTank);
                goblinTank->growlSoundPlay = true;
            }
            
            goblinTank->frameCounter = FRAME_COUNTER_ZERO;
            goblinTank->currentFrame = (goblinTank->currentFrame + NEXT_FRAME) % goblinTank->frameIdle;
        }
   
    float distanceToViewPlayer = fabs(player->position.x - goblinTank->position.x);

    if (player->position.x > goblinTank->position.x + goblinTank->distanceToTurn)
    {
        goblinTank->direction = DIRECTION_RIGHT;  
    }
    else if (player->position.x < goblinTank->position.x - goblinTank->distanceToTurn)
    {
        goblinTank->direction = DIRECTION_LEFT; 
    }

        if (!goblinTank->isDead && !goblinTank->goblinTankHasHurt && distanceToViewPlayer <= goblinTank->viewPlayer) 
        {
            goblinTank->isWalking = true;
            goblinTank->isIdle = false;
        
            goblinTank->position.x += goblinTank->speed * goblinTank->direction * deltaTime;

            if (distanceToViewPlayer <= goblinTank->goblinTankAttackRange && goblinTank->direction == DIRECTION_LEFT && goblinTank->attackCooldownTimer <= COOL_DOWN_ZERO)
            {
                goblinTank->attackAnimTimer = COOL_DOWN_ATTACK;

                if (goblinTank->isAttacking && !goblinTank->hitApplied && goblinTank->currentFrame == FRAME_TO_DAMAGE)
                {
                    player->life -= goblinTank->damage;
                    goblinTank->goblinTankHasHit = true;
                    player->hasHit = true;
                    player->hitTimer = HIT_TIMER;

                    goblinTank->attackCooldownTimer = goblinTank->attackCooldown;
                }
            }
            else if (distanceToViewPlayer <= goblinTank->goblinTankAttackRangeRight && goblinTank->direction == DIRECTION_RIGHT && goblinTank->attackCooldownTimer <= COOL_DOWN_ZERO)
            {
                goblinTank->attackAnimTimer = COOL_DOWN_ATTACK;

                if (goblinTank->isAttacking && !goblinTank->hitApplied && goblinTank->currentFrame == FRAME_TO_DAMAGE)
                {
                    player->life -= goblinTank->damage;
                    goblinTank->goblinTankHasHit = true;
                    player->hasHit = true;
                    player->hitTimer = HIT_TIMER;

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

        if (goblinTank->direction == DIRECTION_LEFT) 
        {
            if (goblinTank->isAttacking)
            {
                if (goblinTank->currentFrame >= ATTACK_HITBOX_START_FRAME)
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
                if (goblinTank->currentFrame >= ATTACK_HITBOX_START_FRAME)
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
            player->position.x += (goblinTank->direction == DIRECTION_LEFT ? -goblinTank->push : goblinTank->push);
        }       
}
        
void DrawGoblinTank(GoblinTank *goblinTank, Player *player)
{ 
    goblinTank->entity.life = goblinTank->life;
    goblinTank->entity.maxLife = goblinTank->maxLife;
    goblinTank->entity.position.x = goblinTank->position.x;
    goblinTank->entity.position.y = goblinTank->position.y;
    goblinTank->entity.isDead = goblinTank->isDead;

    DrawBar(&goblinTank->entity, 140, 15); 

    Rectangle source = {0, 0, 0, 0};
    Texture2D currentTexture;
    Rectangle dest = {0, 0, 0, 0};

    if (goblinTank->goblinTankHasHurt && !goblinTank->isDead)
    {
        float hurtOffSetY = HURT_OFFSET_HURT_Y;
        currentTexture = goblinTank->goblinTankSpriteHurt;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthHurt, SPRITE_ROW_BASE, goblinTank->frameWidthHurt * goblinTank->direction, goblinTank->frameHeightHurt};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOffSetY;
    }
    else if (goblinTank->isDead)
    {
        float hurtOffSetY = HURT_OFFSET_DEAD_Y;

        currentTexture = goblinTank->goblinTankSpriteDead;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthDead, SPRITE_ROW_BASE, goblinTank->frameWidthDead * goblinTank->direction, goblinTank->frameHeightDead};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOffSetY;
    }
    else if (goblinTank->isAttacking)
    {
        float hurtOffSetY = HURT_OFFSET_ATTACK_Y;

        currentTexture = goblinTank->goblinTankSpriteAtk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthAttack, SPRITE_ROW_BASE, goblinTank->frameWidthAttack * goblinTank->direction, goblinTank->frameHeightAttack};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOffSetY;
    }
    else if (goblinTank->isWalking)
    {
        float hurtOffSetY = HURT_OFFSET_WALKING_Y;

        currentTexture = goblinTank->goblinTankSpriteWalk;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthWalk, SPRITE_ROW_BASE, goblinTank->frameWidthWalk * goblinTank->direction, goblinTank->frameHeightWalk};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scale, source.height / goblinTank->scale};

        dest.y += hurtOffSetY;
    }   
    else if (goblinTank->isIdle)
    {
        float hurtOffSetY = HURT_OFFSET_IDLE_Y;

        currentTexture = goblinTank->goblinTankSpriteIdle;
        source = (Rectangle){goblinTank->currentFrame * goblinTank->frameWidthIdle, SPRITE_ROW_BASE, goblinTank->frameWidthIdle * goblinTank->direction, goblinTank->frameHeightIdle};
        dest = (Rectangle){goblinTank->position.x, goblinTank->position.y, source.width / goblinTank->scaleIdle , source.height / goblinTank->scaleIdle};

        dest.y += hurtOffSetY;
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
    UnloadSound(goblinTank->soundAttackGoblinTank);
    UnloadSound(goblinTank->soundGrowlGoblinTank);
    UnloadSound(goblinTank->soundHurtGoblinTank);
    UnloadSound(goblinTank->soundDeathGolbinTank);
}