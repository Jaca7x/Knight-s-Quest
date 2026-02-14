#include "goblinArcher.h"

bool CheckCollisionArrow (float x1, float y1, float w1, float h1,
                          float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}
                     
void InitGoblinArcher(GoblinArcher *goblinArcher)
{
    // Position
    goblinArcher->base.position = (Vector2){1120, 567};
    goblinArcher->base.direction = 1;

    // Cores stats
    goblinArcher->maxLife = 50.0f;
    goblinArcher->life = goblinArcher->maxLife;
    goblinArcher->speed = 95.0f;

    // Vision & movement ranges
    goblinArcher->goblinView = 600.0f;
    goblinArcher->attackRange = 400.0f;

    // Animation frame counts
    goblinArcher->frameIdle = 8;
    goblinArcher->frameWalk = 6;
    goblinArcher->frameAtk  = 9;
    goblinArcher->frameHurt = 4;
    goblinArcher->frameDead = 9;

    // Animation control
    goblinArcher->base.currentFrame = 0;
    goblinArcher->frameCounter = 0;
    goblinArcher->deathAnimTimer = 0.0f;
    goblinArcher->attackAnimTimer = 0.0f;

    // State flags
    goblinArcher->base.isWalking = false;
    goblinArcher->base.isIdle = true;
    goblinArcher->base.isAtacking = false;
    goblinArcher->base.isDead = false;
    goblinArcher->base.monsterHasHit = false;

    goblinArcher->deathAnimationDone = false;
    goblinArcher->droppedHeart = false;

    goblinArcher->hasHitPlayer = false;

    goblinArcher->arrow.active = false;

    // Sprites
    goblinArcher->base.spriteIdle  = LoadTexture("assets/resources/sprites/goblinArcher/goblinArcher-idle.png");
    goblinArcher->base.spriteWalk  = LoadTexture("assets/resources/sprites/goblinArcher/goblinArcher-walk.png");
    goblinArcher->base.spriteAtack = LoadTexture("assets/resources/sprites/goblinArcher/goblinArcher-atk.png");
    goblinArcher->base.spriteHurt  = LoadTexture("assets/resources/sprites/goblinArcher/goblinArcher-hurt.png");
    goblinArcher->base.spriteDead  = LoadTexture("assets/resources/sprites/goblinArcher/goblinArcher-dead.png");

    goblinArcher->arrowTexture     = LoadTexture("assets/resources/sprites/goblinArcher/Arrow.png");

    // Scales
    const float defaultScale = 6.67f;
    goblinArcher->base.scale = defaultScale;
    goblinArcher->base.scaleIdle = defaultScale;

    // Frame sizes
    goblinArcher->base.frameWidthIdle  = goblinArcher->base.spriteIdle.width  / goblinArcher->frameIdle;
    goblinArcher->base.frameHeightIdle = goblinArcher->base.spriteIdle.height;

    goblinArcher->base.frameWidthWalk  = goblinArcher->base.spriteWalk.width  / goblinArcher->frameWalk;
    goblinArcher->base.frameHeightWalk = goblinArcher->base.spriteWalk.height;

    goblinArcher->base.frameWidthAtack  = goblinArcher->base.spriteAtack.width / goblinArcher->frameAtk;
    goblinArcher->base.frameHeightAtack = goblinArcher->base.spriteAtack.height;

    goblinArcher->base.frameWidthHurt  = goblinArcher->base.spriteHurt.width  / goblinArcher->frameHurt;
    goblinArcher->base.frameHeightHurt = goblinArcher->base.spriteHurt.height;

    goblinArcher->base.frameWidthDead  = goblinArcher->base.spriteDead.width  / goblinArcher->frameDead;
    goblinArcher->base.frameHeightDead = goblinArcher->base.spriteDead.height;

    //Arrow core stats
    goblinArcher->arrowSpeed = 380.0f;
    goblinArcher->arrowTolerance = 90;
    goblinArcher->arrowDamage = 15;

    //Arrow cooldown
    goblinArcher->arrowCooldown = 0.0f;
    goblinArcher->attackCooldown = 1.5f;

    //Sounds
    goblinArcher->goblinArcherDeathSound =
        LoadSound("assets/resources/sounds/sound_effects/goblin/goblin-death.wav");

    goblinArcher->goblinArcherLoadingSound =
        LoadSound("assets/resources/sounds/sound_effects/goblin/bow-loading.wav");

    goblinArcher->arrowHitSound =
        LoadSound("assets/resources/sounds/sound_effects/goblin/arrow-hit.wav");
}

void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta)
{
    if (goblinArcher->life <= LIFE_ZERO && !goblinArcher->base.isDead)
    {
        PlaySound(goblinArcher->goblinArcherDeathSound);
        goblinArcher->base.isDead = true;
        goblinArcher->base.currentFrame = CURRENT_FRAME_ZERO;
        goblinArcher->hasHitPlayer = false;  
        goblinArcher->base.monsterHasHit = false;
        player->hasHit = false;
        goblinArcher->frameDead = GOBLIN_ARCHER_FRAME_DEAD;
        goblinArcher->deathAnimTimer = ANIM_DEAD_ZERO;
        goblinArcher->deathAnimationDone = false;
        goblinArcher->base.isWalking = false;
        goblinArcher->base.isAtacking = false;
        goblinArcher->base.isIdle = false;
        goblinArcher->arrow.active = false;
        goblinArcher->attackAnimTimer = ANIM_ATTACK_ZERO;
        goblinArcher->arrowDamage = ARROW_DAMAGE_ZERO;
    }

    if (goblinArcher->base.isDead && goblinArcher->deathAnimationDone)
        return;

    goblinArcher->frameCounter++;
    if (goblinArcher->frameCounter >= (GAME_FPS / ANIMATION_FPS))
    {
        goblinArcher->frameCounter = FRAME_COUNTER_ZERO;

        if (goblinArcher->base.isDead && !goblinArcher->deathAnimationDone)
        {
            goblinArcher->deathAnimTimer += delta;

            if (goblinArcher->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
            {   
                goblinArcher->frameDead++;
                goblinArcher->deathAnimTimer = DEATH_ANIM_FRAME_TIME;

                if (goblinArcher->frameDead >= GOBLIN_ARCHER_FRAME_DEAD)
                {
                    goblinArcher->frameDead = GOBLIN_ARCHER_FRAME_DEAD;
                    goblinArcher->deathAnimationDone = true;
                    goblinArcher->speed = SPEED_ZERO;
        
                }
            }
            goblinArcher->base.currentFrame = goblinArcher->frameDead;  
        }
        else if (goblinArcher->base.monsterHasHit && !goblinArcher->base.isDead)
        {
            goblinArcher->base.currentFrame = (goblinArcher->base.currentFrame + NEXT_FRAME) % goblinArcher->frameHurt;
        }
        else if (goblinArcher->base.isAtacking)
        {
            if (goblinArcher->base.currentFrame == FRAME_FOR_LOADING_SOUND)
            {
               PlaySound(goblinArcher->goblinArcherLoadingSound);
            }
            goblinArcher->base.currentFrame = (goblinArcher->base.currentFrame + NEXT_FRAME) % goblinArcher->frameAtk;
        }
        else if (goblinArcher->base.isWalking)
        {
            goblinArcher->base.currentFrame = (goblinArcher->base.currentFrame + NEXT_FRAME) % goblinArcher->frameWalk;
        }
        else if (goblinArcher->base.isIdle)
        {
            goblinArcher->base.currentFrame = (goblinArcher->base.currentFrame + NEXT_FRAME) % goblinArcher->frameIdle;
        }
    }

    if (goblinArcher->arrowCooldown > ARROW_COOLDOWN_ZERO)
    {
        goblinArcher->arrowCooldown -= delta;
        if (goblinArcher->arrowCooldown < ARROW_COOLDOWN_ZERO)
            goblinArcher->arrowCooldown = ARROW_COOLDOWN_ZERO;
    }

    if (goblinArcher->attackAnimTimer > ANIM_ATTACK_ZERO)
    {
        goblinArcher->attackAnimTimer -= delta;
        if (goblinArcher->attackAnimTimer < ANIM_ATTACK_ZERO)
            goblinArcher->attackAnimTimer = ANIM_ATTACK_ZERO;
    }

    float distanceToPlayer = fabs(player->position.x - goblinArcher->base.position.x);

    if (distanceToPlayer <= goblinArcher->goblinView && player->life > LIFE_ZERO && !goblinArcher->base.isDead)
    {
        goblinArcher->base.direction = (player->position.x < goblinArcher->base.position.x) ? DIRECTION_LEFT : DIRECTION_RIGHT;

        if (distanceToPlayer <= goblinArcher->attackRange)
        {
            goblinArcher->base.isIdle = false;
            goblinArcher->base.isWalking = false;
            goblinArcher->base.isAtacking = true;

            if (goblinArcher->arrowCooldown <= ARROW_COOLDOWN_ZERO)
            {
                goblinArcher->arrow.active = true;
                goblinArcher->arrow.arrowYOffset = ARROW_OFFSET_Y;
                goblinArcher->arrowCooldown = ARROW_COOLDOWN;
                goblinArcher->attackAnimTimer = GOBLIN_ARCHER_ATTACK_ANIM_TIMER;
                goblinArcher->arrow.position = (Vector2){
                    goblinArcher->base.position.x,
                    goblinArcher->base.position.y + goblinArcher->base.frameHeightIdle * goblinArcher->arrow.arrowYOffset
                };
                goblinArcher->arrow.scale = ARROW_SCALE;
                goblinArcher->arrow.rotation = ARROW_ROTATION;
                goblinArcher->arrow.direction = goblinArcher->base.direction;
                goblinArcher->arrow.speed = goblinArcher->arrowSpeed;
            }

            if (goblinArcher->attackAnimTimer > ANIM_ATTACK_ZERO)
            {
                goblinArcher->base.isIdle = false;
                goblinArcher->base.isAtacking = true;
            }
            else
            {
                goblinArcher->base.isIdle = true;
                goblinArcher->base.isAtacking = false;
            }
        }
        else
        {
            goblinArcher->base.isWalking = true;
            goblinArcher->base.isAtacking = false;
            goblinArcher->base.isIdle = false;

            goblinArcher->base.position.x += goblinArcher->speed * goblinArcher->base.direction * delta;
        }
    } else 
    {
        goblinArcher->base.isWalking = false;
        goblinArcher->base.isAtacking = false;
        goblinArcher->base.isIdle = true;
    }

    if (goblinArcher->arrow.active)
    {
        goblinArcher->arrow.position.x += goblinArcher->arrow.speed * goblinArcher->arrow.direction * delta;

        Rectangle hitbox = GetPlayerHitbox(player);

        if (CheckCollisionArrow(
            goblinArcher->arrow.position.x,
            goblinArcher->arrow.position.y,
            goblinArcher->arrowTexture.width * goblinArcher->arrow.scale,
            goblinArcher->arrowTexture.height * goblinArcher->arrow.scale,
            hitbox.x,
            hitbox.y,
            hitbox.width,
            hitbox.height))
        {
            PlaySound(goblinArcher->arrowHitSound);
            player->life -= goblinArcher->arrowDamage;
            goblinArcher->arrow.active = false;
            player->hitTimer = HIT_TIMER;   
            goblinArcher->hasHitPlayer = true;
            player->hasHit = true; 
        }
    }

    if (goblinArcher->arrow.active)
    {
        if (goblinArcher->arrow.position.x < POSITION_ZERO||
            goblinArcher->arrow.position.x > GetScreenWidth())
        {
            goblinArcher->arrow.active = false;
        }
    }
}

void DrawGoblinArcher(GoblinArcher *goblinArcher)
{
    goblinArcher->entity.isDead = goblinArcher->base.isDead;
    goblinArcher->entity.life = goblinArcher->life;
    goblinArcher->entity.maxLife = goblinArcher->maxLife;
    goblinArcher->entity.position.x = goblinArcher->base.position.x;
    goblinArcher->entity.position.y = goblinArcher->base.position.y;

    DrawBar(&goblinArcher->entity, GOBLIN_ARCHER_OFFSET_BAR_LIFE_X, GOBLIN_ARCHER_OFFSET_BAR_LIFE_Y);

    DrawMonsters(&goblinArcher->base, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO);

    if (goblinArcher->arrow.active)
    {
        DrawTextureEx(
            goblinArcher->arrowTexture,
            goblinArcher->arrow.position,
            goblinArcher->arrow.rotation,
            goblinArcher->arrow.scale,
            WHITE
        );
    }
}   

void UnloadGoblinArcher(GoblinArcher *goblinArcher)
{
    UnloadTexture(goblinArcher->base.spriteWalk);
    UnloadTexture(goblinArcher->base.spriteAtack);
    UnloadTexture(goblinArcher->base.spriteIdle);
    UnloadTexture(goblinArcher->base.spriteDead);
    UnloadTexture(goblinArcher->base.spriteHurt);
    UnloadTexture(goblinArcher->arrowTexture);
}
