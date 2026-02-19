#include "goblinTank.h"

//AABB
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
    // Position
    goblinTank->base.position = (Vector2){800, 500};
    goblinTank->base.direction = 1;

    // Core stats
    goblinTank->speed = 40.0f;
    goblinTank->maxLife = 200.0f;
    goblinTank->life = goblinTank->maxLife;
    goblinTank->damage = 25;

    // Vision & movement ranges
    goblinTank->viewPlayer = 400.0f;
    goblinTank->distanceToTurn = 100.0f;
    goblinTank->goblinTankAttackRange = 40.0f;
    goblinTank->goblinTankAttackRangeRight = 180.0f;
    goblinTank->push = 50;

    // Animation frame counts
    goblinTank->frameWalk = 6;
    goblinTank->frameHurt = 4;
    goblinTank->frameDead = 9;
    goblinTank->frameIdle = 8;
    goblinTank->frameAtk = 10;

    // Animation control
    goblinTank->base.currentFrame = 0;
    goblinTank->frameCounter = 0;
    goblinTank->deathAnimTimer = 0.0f;

    // Hurt system
    goblinTank->hurtTimer = 0.0f;
    goblinTank->hurtDuration = 0.2f;

    // Attack system
    goblinTank->atackCooldown = 1.0f;
    goblinTank->atackCooldownTimer = 0.0f;
    goblinTank->atackAnimTimer = 0.0f;
    goblinTank->hitApplied = false;

    // State flags
    goblinTank->base.isIdle = true;
    goblinTank->base.isWalking = false;
    goblinTank->base.isAtacking = false;
    goblinTank->base.isDead = false;
    goblinTank->base.monsterHasHit = false;

    goblinTank->hitPlayer = false;
    goblinTank->deathAnimationDone = false;
    goblinTank->growlSoundPlay = false;
    goblinTank->droppedHeart = false;

    // Sprites
    goblinTank->base.spriteWalk   = LoadTexture("assets/resources/sprites/goblinTank/goblinTank-walk.png");
    goblinTank->base.spriteHurt   = LoadTexture("assets/resources/sprites/goblinTank/goblinTank-hurt.png");
    goblinTank->base.spriteDead   = LoadTexture("assets/resources/sprites/goblinTank/goblinTank-dead.png");
    goblinTank->base.spriteIdle   = LoadTexture("assets/resources/sprites/goblinTank/goblinTank-idle.png");
    goblinTank->base.spriteAtack  = LoadTexture("assets/resources/sprites/goblinTank/goblinTank-attack.png");

    // Scale
    goblinTank->base.scale = 4;
    goblinTank->base.scaleIdle = 5;

    // Frame sizes
    goblinTank->base.frameWidthWalk  = goblinTank->base.spriteWalk.width / goblinTank->frameWalk;
    goblinTank->base.frameHeightWalk = goblinTank->base.spriteWalk.height;

    goblinTank->base.frameWidthIdle  = goblinTank->base.spriteIdle.width / goblinTank->frameIdle;
    goblinTank->base.frameHeightIdle = goblinTank->base.spriteIdle.height;

    goblinTank->base.frameWidthAtack  = goblinTank->base.spriteAtack.width / goblinTank->frameAtk;
    goblinTank->base.frameHeightAtack = goblinTank->base.spriteAtack.height;

    goblinTank->base.frameWidthDead  = goblinTank->base.spriteDead.width / goblinTank->frameDead;
    goblinTank->base.frameHeightDead = goblinTank->base.spriteDead.height;

    goblinTank->base.frameWidthHurt  = goblinTank->base.spriteHurt.width / goblinTank->frameHurt;
    goblinTank->base.frameHeightHurt = goblinTank->base.spriteHurt.height;

    // Sounds
    goblinTank->soundAttackGoblinTank =
        LoadSound("assets/resources/sounds/sound_effects/goblin/attack-goblinTank.wav");

    goblinTank->soundGrowlGoblinTank  = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/idle-goblinTank.wav");

    goblinTank->soundHurtGoblinTank   = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/hurt-goblinTank.wav");

    goblinTank->soundDeathGolbinTank  = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/death-goblinTank.wav");
}
 
void UpdateGoblinTank(GoblinTank *goblinTank, float delta, Player *player)
{
    if (goblinTank->life <= LIFE_ZERO && !goblinTank->base.isDead)
    {
        StopSound(goblinTank->soundGrowlGoblinTank);
        PlaySound(goblinTank->soundDeathGolbinTank);
        goblinTank->base.isDead = true;
        goblinTank->base.isWalking = false;
        goblinTank->base.isIdle = false;
        goblinTank->base.isAtacking = false;
        goblinTank->deathAnimationDone = false;
        goblinTank->base.monsterHasHit = false;
        goblinTank->hitPlayer = false;
        goblinTank->base.currentFrame = CURRENT_FRAME_ZERO;
        goblinTank->frameDead = FRAME_OF_DEATH_GOBLIN_TANK;
    }

    if (goblinTank->base.isDead && goblinTank->deathAnimationDone)
        return;

    if (goblinTank->atackCooldownTimer > COOLDOWN_ZERO)
        goblinTank->atackCooldownTimer -= delta;

    if (goblinTank->atackAnimTimer > COOLDOWN_ZERO)
    {
        goblinTank->atackAnimTimer -= delta;
        goblinTank->base.isAtacking = true;
    }
    else
    {
        goblinTank->base.isAtacking = false;
    }

    if (goblinTank->base.monsterHasHit)
    {
        goblinTank->base.isIdle = false;
        goblinTank->base.isWalking = false;
        goblinTank->base.isAtacking = false;

        goblinTank->hurtTimer += delta;

        if (goblinTank->hurtTimer >= goblinTank->hurtDuration)
        {
            goblinTank->base.monsterHasHit = false;
            goblinTank->hurtTimer = HURT_TIMER_ZERO;
            goblinTank->base.isIdle = true;
        }
    }

    goblinTank->frameCounter++;

    if 
    (
        goblinTank->base.monsterHasHit 
        && !goblinTank->base.isDead 
        && goblinTank->frameCounter >= GOBLIN_TANK_FRAME_DELAY_HURT
    )
    {
        goblinTank->frameCounter = FRAME_COUNTER_ZERO;
        goblinTank->base.currentFrame = (goblinTank->base.currentFrame + NEXT_FRAME) % goblinTank->frameHurt;
    }
    else if (goblinTank->base.isDead && !goblinTank->deathAnimationDone)
    {
        goblinTank->deathAnimTimer += delta;

        if (goblinTank->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
        {
            goblinTank->deathAnimTimer = COOLDOWN_ZERO;
            goblinTank->base.currentFrame++;

            if (goblinTank->base.currentFrame >= goblinTank->frameDead - PREVIOUS_FRAME)
            {
                goblinTank->base.currentFrame = goblinTank->frameDead - PREVIOUS_FRAME;
                goblinTank->deathAnimationDone = true;
            }
        }
    }
    else if (goblinTank->base.isAtacking && goblinTank->frameCounter >= GOBLIN_TANK_ATTACK_FRAME_DELAY)
    {
        if (goblinTank->base.currentFrame == FRAME_TO_SOUND_ATTACK_GOBLIN_TANK)
        {
            PlaySound(goblinTank->soundAttackGoblinTank);
        }

        goblinTank->frameCounter = FRAME_COUNTER_ZERO;
        goblinTank->base.currentFrame = (goblinTank->base.currentFrame + NEXT_FRAME) % goblinTank->frameAtk;
    }
    else if (goblinTank->base.isWalking && goblinTank->frameCounter >= GOBLIN_TANK_FRAME_DELAY)
    {
        goblinTank->frameCounter = FRAME_COUNTER_ZERO;
        goblinTank->base.currentFrame = (goblinTank->base.currentFrame + NEXT_FRAME) % goblinTank->frameWalk;
    }
    else if (goblinTank->base.isIdle && goblinTank->frameCounter >= GOBLIN_TANK_FRAME_DELAY)
    {
        if (!goblinTank->growlSoundPlay)
        {
            PlaySound(goblinTank->soundGrowlGoblinTank);
            goblinTank->growlSoundPlay = true;
        }

        goblinTank->frameCounter = FRAME_COUNTER_ZERO;
        goblinTank->base.currentFrame = (goblinTank->base.currentFrame + NEXT_FRAME) % goblinTank->frameIdle;
    }

    float distanceToViewPlayer = fabs(player->position.x - goblinTank->base.position.x);

    if (player->position.x > goblinTank->base.position.x + goblinTank->distanceToTurn)
    {
        goblinTank->base.direction = DIRECTION_RIGHT;
    }
    else if (player->position.x < goblinTank->base.position.x - goblinTank->distanceToTurn)
    {
        goblinTank->base.direction = DIRECTION_LEFT;
    }

    if 
    (
        !goblinTank->base.isDead 
        && !goblinTank->base.monsterHasHit 
        && distanceToViewPlayer <= goblinTank->viewPlayer
    )
    {
        goblinTank->base.isWalking = true;
        goblinTank->base.isIdle = false;

        goblinTank->base.position.x += goblinTank->speed * goblinTank->base.direction * delta;

        if 
        (
            distanceToViewPlayer <= goblinTank->goblinTankAttackRange 
            && goblinTank->base.direction == DIRECTION_LEFT 
            && goblinTank->atackCooldownTimer <= COOLDOWN_ZERO
        )
        {
            goblinTank->atackAnimTimer = COOLDOWN_ATTACK;

            if 
            (
                goblinTank->base.isAtacking 
                && !goblinTank->hitApplied 
                && goblinTank->base.currentFrame == FRAME_TO_DAMAGE_GOBLIN_TANK
            )
            {
                player->life -= goblinTank->damage;
                goblinTank->hitPlayer = true;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;

                goblinTank->atackCooldownTimer = goblinTank->atackCooldown;
            }
        }
        else if 
        (
            distanceToViewPlayer <= goblinTank->goblinTankAttackRangeRight 
            && goblinTank->base.direction == DIRECTION_RIGHT 
            && goblinTank->atackCooldownTimer <= COOLDOWN_ZERO
        )
        {
            goblinTank->atackAnimTimer = COOLDOWN_ATTACK;

            if (
                goblinTank->base.isAtacking
                && !goblinTank->hitApplied 
                && goblinTank->base.currentFrame == FRAME_TO_DAMAGE_GOBLIN_TANK
            )
            {
                player->life -= goblinTank->damage;
                goblinTank->hitPlayer = true;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;

                goblinTank->atackCooldownTimer = goblinTank->atackCooldown;
            }
        }
    }
    else
    {
        goblinTank->base.isIdle = true;
        goblinTank->base.isWalking = false;
        goblinTank->base.isAtacking = false;
        goblinTank->hitPlayer = false;
    }

    float attackOffsetX;

    if (goblinTank->base.direction == DIRECTION_LEFT)
    {
        if (goblinTank->base.isAtacking)
        {
            if (goblinTank->base.currentFrame >= ATTACK_HITBOX_START_FRAME_GOBLIN_TANK)
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
        if (goblinTank->base.isAtacking)
        {
            if (goblinTank->base.currentFrame >= ATTACK_HITBOX_START_FRAME_GOBLIN_TANK)
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

    if (!goblinTank->base.isDead &&
        CheckColisionGoblinTank(
            player->position.x + PLAYER_HITBOX_OFFSET_X,
            player->position.y + PLAYER_HITBOX_OFFSET_Y,
            player->frameWidth / PLAYER_HITBOX_WIDTH_DIV,
            player->frameHeight,

            goblinTank->base.position.x + attackOffsetX,
            goblinTank->base.position.y + GOBLIN_TANK_OFFSET_Y,
            goblinTank->base.frameWidthAtack / GOBLIN_TANK_HITBOX_WITH_DIV,
            goblinTank->base.frameHeightAtack / GOBLIN_TANK_HITBOX_HEIGHT_DIV))
    {
        player->position.x += (goblinTank->base.direction == DIRECTION_LEFT ? -goblinTank->push : goblinTank->push);
    }
}

void DrawGoblinTank(GoblinTank *goblinTank, Player *player)
{
    goblinTank->entity.life = goblinTank->life;
    goblinTank->entity.maxLife = goblinTank->maxLife;
    goblinTank->entity.position.x = goblinTank->base.position.x;
    goblinTank->entity.position.y = goblinTank->base.position.y;
    goblinTank->entity.isDead = goblinTank->base.isDead;

    DrawBar(&goblinTank->entity, OFFSET_BAR_LIFE_X_GOBLIN_TANK, OFFSET_BAR_LIFE_Y_GOBLIN_TANK, WIDTH_LIFE_BAR, HEIGHT_LIFE_BAR);

    DrawMonsters
    (
        &goblinTank->base, 
        SPRITE_ROW_BASE, 
        HURT_GOBLIN_TANK_OFFSET_HURT_Y, 
        HURT_GOBLIN_TANK_OFFSET_DEAD_Y, 
        HURT_GOBLIN_TANK_OFFSET_ATTACK_Y, 
        HURT_GOBLIN_TANK_OFFSET_WALKING_Y,
        HURT_GOBLIN_TANK_OFFSET_IDLE_Y
    );
}

void UnloadGoblinTank(GoblinTank *goblinTank)
{
    UnloadTexture(goblinTank->base.spriteWalk);
    UnloadTexture(goblinTank->base.spriteHurt);
    UnloadTexture(goblinTank->base.spriteDead);
    UnloadTexture(goblinTank->base.spriteIdle);
    UnloadTexture(goblinTank->base.spriteAtack);
    
    UnloadSound(goblinTank->soundAttackGoblinTank);
    UnloadSound(goblinTank->soundGrowlGoblinTank);
    UnloadSound(goblinTank->soundHurtGoblinTank);
    UnloadSound(goblinTank->soundDeathGolbinTank);
}