#include "goblin.h"

bool CheckCollisionGoblin(float x1, float y1, float w1, float h1,
                          float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitGoblinBase(Goblin *goblin, Vector2 pos)
{
    // Position
    goblin->base.position = pos;
    goblin->base.direction = 1;

    // Core stats
    goblin->damage = 20;
    goblin->life = 50;
    goblin->maxLife = 50.0f;
    goblin->speed = 100.0f;
    goblin->baseSpeed = 100.0f;

    // Vision & movement ranges
    goblin->viewPlayer = 300.0f;
    goblin->goblinAttackRangeRight = 100.0f;
    goblin->goblinAttackRangeLeft = 15.0f;

    // Animation frame counts
    goblin->frameWalk = 6;
    goblin->frameHurt = 4;
    goblin->frameDead = 8;
    goblin->frameIdle = 8;
    goblin->frameAtk = 6;

    // Animation Control
    goblin->base.currentFrame = 0;
    goblin->frameCounter = 0;
    goblin->deathAnimationDone = false;

    //State flags
    goblin->base.isIdle = true;
    goblin->base.isWalking = false;
    goblin->base.isAtacking = false;
    goblin->base.isDead = false;
    goblin->base.monsterHasHit = false;

    goblin->droppedHeart = false;

    goblin->goblinHasHitPlayer = false;

    // Sprites
    goblin->base.spriteWalk = LoadTexture("assets/resources/sprites/goblin/goblin-walk.png");
    goblin->base.spriteHurt = LoadTexture("assets/resources/sprites/goblin/goblin-hurt.png");
    goblin->base.spriteDead = LoadTexture("assets/resources/sprites/goblin/goblin-dead.png");
    goblin->base.spriteIdle = LoadTexture("assets/resources/sprites/goblin/goblin-idle.png");
    goblin->base.spriteAtack = LoadTexture("assets/resources/sprites/goblin/goblin-atk.png");

    // Scale
    goblin->scaleHitbox = 0.02f;
    goblin->base.scale = 6.67f;
    goblin->base.scaleIdle = 6.67f;

    // Sprites sizes
    goblin->base.frameWidthIdle = goblin->base.spriteIdle.width / goblin->frameIdle;
    goblin->base.frameHeightIdle = goblin->base.spriteIdle.height;

    goblin->base.frameWidthWalk = goblin->base.spriteWalk.width / goblin->frameWalk;
    goblin->base.frameHeightWalk = goblin->base.spriteWalk.height;

    goblin->base.frameWidthAtack = goblin->base.spriteAtack.width / goblin->frameAtk;
    goblin->base.frameHeightAtack = goblin->base.spriteAtack.height;

    goblin->base.frameWidthDead = goblin->base.spriteDead.width / goblin->frameDead;
    goblin->base.frameHeightDead = goblin->base.spriteDead.height;

    goblin->base.frameWidthHurt = goblin->base.spriteHurt.width / goblin->frameHurt;
    goblin->base.frameHeightHurt = goblin->base.spriteHurt.height;

    // Atack & hurt
    goblin->push = 50.0f;
    goblin->hurtTimer = 0.0f;
    goblin->hurtDuration = 0.4f;
    goblin->attackTimer = 0.0f;
    goblin->attackCooldown = 1.0f;

    // Sounds
    goblin->goblinDeathSound = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/goblin-death.wav");
    goblin->goblinCutSound = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/goblin-cut.wav");

    goblin->RedGoblinHitSound = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/red-goblin-hit.wav");
}

void InitRedGoblin(Goblin *goblin, Vector2 pos)
{
    InitGoblinBase(goblin, pos);

    // Cores stats red goblin
    goblin->life = 70;
    goblin->maxLife = 70.0f;
    goblin->speed = 80.0f;
    goblin->baseSpeed = 80.0f;
    goblin->damage = 40;

    // Sprites red goblin
    goblin->base.spriteWalk = LoadTexture("assets/resources/sprites/redGoblin/Walk.png");
    goblin->base.spriteHurt = LoadTexture("assets/resources/sprites/redGoblin/Hurt.png");
    goblin->base.spriteDead = LoadTexture("assets/resources/sprites/redGoblin/Dead.png");
    goblin->base.spriteIdle = LoadTexture("assets/resources/sprites/redGoblin/Idle.png");
    goblin->base.spriteAtack = LoadTexture("assets/resources/sprites/redGoblin/Attack.png");

    // Sounds red goblin
    goblin->goblinDeathSound = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/red-goblin-death.wav");
    goblin->RedGoblinHitSound = 
        LoadSound("assets/resources/sounds/sound_effects/goblin/red-goblin-hit.wav");
}

void UpdateGoblin(Goblin *goblin, Player *player, int currentMapIndex, float delta)
{
    if (goblin->life <= LIFE_ZERO && !goblin->base.isDead)
    {
        PlaySound(goblin->goblinDeathSound);
        goblin->base.isDead = true;
        goblin->base.monsterHasHit = false;
        goblin->base.currentFrame = CURRENT_FRAME_ZERO;
        goblin->goblinHasHitPlayer = false;
        goblin->attackTimer = COOLDOWN_ZERO;
        goblin->frameDead = GOBLIN_FRAME_DEATH;
        goblin->deathAnimationDone = false;
        goblin->base.isWalking = false;
        goblin->base.isAtacking = false;
        goblin->base.isIdle = false;
    }

    if (goblin->base.isDead && goblin->deathAnimationDone)
        return;

    if (goblin->base.monsterHasHit)
    {
        goblin->base.isIdle = false;
        goblin->base.isWalking = false;
        goblin->base.isAtacking = false;

        goblin->hurtTimer += delta;

        if (goblin->hurtTimer >= goblin->hurtDuration)
        {
            goblin->base.monsterHasHit = false;
            goblin->hurtTimer = HURT_TIMER_ZERO;
            goblin->base.isIdle = true;
        }
    }

    goblin->frameCounter++;
    if (goblin->frameCounter >= (GAME_FPS / ANIMATION_FPS))
    {
        goblin->frameCounter = FRAME_COUNTER_ZERO;
        if (goblin->base.monsterHasHit && !goblin->base.isDead)
        {
            goblin->base.currentFrame = (goblin->base.currentFrame + NEXT_FRAME) % goblin->frameHurt;
        }
        else if (goblin->base.isDead && !goblin->deathAnimationDone)
        {
            goblin->deathAnimTimer += delta;
            if (goblin->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
            {
                goblin->frameDead++;
                goblin->deathAnimTimer = COOLDOWN_ZERO;

                if (goblin->frameDead >= GOBLIN_TOTAL_FRAME_DEATH)
                {
                    goblin->frameDead = GOBLIN_FRAME_DEATH;
                    goblin->deathAnimationDone = true;
                }
            }
            goblin->base.currentFrame = goblin->frameDead;
        }
        else if (goblin->base.isAtacking)
        {
            if (goblin->base.currentFrame == GOBLIN_FRAME_FOR_ATTACK_SOUND)
            {
                PlaySound(goblin->goblinCutSound);
            }
            goblin->base.currentFrame = (goblin->base.currentFrame + NEXT_FRAME) % goblin->frameAtk;
        }
        else if (goblin->base.isWalking)
        {
            goblin->base.currentFrame = (goblin->base.currentFrame + NEXT_FRAME) % goblin->frameWalk;
        }
        else if (goblin->base.isIdle)
        {
            goblin->base.currentFrame = (goblin->base.currentFrame + NEXT_FRAME) % goblin->frameIdle;
        }
    }

    if (goblin->attackTimer > COOLDOWN_ZERO)
    {
        goblin->attackTimer -= delta;
        goblin->base.isAtacking = true;
        goblin->base.isWalking = false;
        goblin->base.isIdle = false;
    }
    else
    {
        float distanceToPlayer = fabs(player->position.x - goblin->base.position.x);

        if (!goblin->base.isDead && distanceToPlayer <= goblin->viewPlayer && !goblin->base.monsterHasHit)
        {
            goblin->base.direction = (player->position.x > goblin->base.position.x) ? DIRECTION_RIGHT : DIRECTION_LEFT;

            goblin->base.isAtacking = false;
            goblin->base.isWalking = true;
            goblin->base.isIdle = false;

            goblin->base.position.x += goblin->speed * goblin->base.direction * delta;

            if (distanceToPlayer <= goblin->goblinAttackRangeRight && goblin->base.direction == DIRECTION_LEFT)
            {
                goblin->base.isAtacking = true;
                goblin->attackTimer = goblin->attackCooldown;
                player->life -= goblin->damage;
                goblin->goblinHasHitPlayer = true;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;

                player->position.x += (player->position.x < goblin->base.position.x) ? -goblin->push : goblin->push;
            }
            else if (distanceToPlayer <= goblin->goblinAttackRangeLeft && goblin->base.direction == DIRECTION_RIGHT)
            {
                goblin->base.isAtacking = true;
                goblin->attackTimer = goblin->attackCooldown;
                player->life -= goblin->damage;
                goblin->goblinHasHitPlayer = true;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;

                player->position.x += (player->position.x < goblin->base.position.x) ? -goblin->push : goblin->push;
            }
        }
        else
        {
            goblin->base.isIdle = true;
            goblin->base.isWalking = false;
            goblin->base.isAtacking = false;
            goblin->goblinHasHitPlayer = false;
        }

        if (!goblin->base.isDead && CheckCollisionGoblin(
                                        player->position.x,
                                        player->position.y,
                                        player->frameWidth,
                                        player->frameHeight,

                                        goblin->base.position.x,
                                        goblin->base.position.y,
                                        goblin->base.frameWidthIdle * goblin->scaleHitbox,
                                        goblin->base.frameHeightIdle * goblin->scaleHitbox))
        {
            player->position.x += (player->position.x < goblin->base.position.x) ? -goblin->push : goblin->push;
        }
    }
}

void DrawGoblin(Goblin *goblin)
{
    goblin->entity.isDead = goblin->base.isDead;
    goblin->entity.life = goblin->life;
    goblin->entity.maxLife = goblin->maxLife;
    goblin->entity.position.x = goblin->base.position.x;
    goblin->entity.position.y = goblin->base.position.y;

    DrawBar(&goblin->entity, GOBLIN_BAR_LIFE_OFFSET_X, GOBLIN_BAR_LIFE_OFFSET_Y);

    DrawMonsters(&goblin->base, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO);
}

void UnloadGoblin(Goblin *goblin)
{
    UnloadTexture(goblin->base.spriteWalk);
    UnloadTexture(goblin->base.spriteHurt);
    UnloadTexture(goblin->base.spriteDead);
    UnloadTexture(goblin->base.spriteIdle);
    UnloadTexture(goblin->base.spriteAtack);
}