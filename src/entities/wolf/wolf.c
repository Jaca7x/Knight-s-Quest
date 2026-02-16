#include "wolf.h"

bool CheckCollisionAABB(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitWolfBase(Wolf *wolf, Vector2 pos)
{
    // Position
    wolf->base.position = pos;
    wolf->start = pos;
    wolf->end = (Vector2){pos.x + 100, pos.y};
    wolf->base.direction = 1;

    //Core stats
    wolf->life = 100;
    wolf->maxLife = 100;
    wolf->speed = 95.0f;
    wolf->damage = 20;
    wolf->push = 70.0f;

    //Vision 
    wolf->attackRange = 100.0f;

    //Animation frame counts
    wolf->frameWalk = 11;
    wolf->frameAtk = 6;
    wolf->frameIdle = 8;
    wolf->frameDead = 2;
    wolf->frameHurt = 2;

    //Animation control
    wolf->base.currentFrame = 0;
    wolf->frameCounter = 0;
    wolf->deathAnimationDone = false;

    //State flags
    wolf->base.isDead = false;
    wolf->isMoving = true;
    wolf->base.isAtacking = false;
    wolf->hasHitPlayer = false;
    wolf->base.monsterHasHit = false;
    wolf->growlPlayed = true;
    wolf->isPatrolling = true;

    wolf->droppedHeart = false;

    //Sprites
    wolf->base.spriteWalk = LoadTexture("assets/resources/sprites/wolf/walk.png");
    wolf->base.spriteAtack = LoadTexture("assets/resources/sprites/wolf/Attack_1.png");
    wolf->base.spriteIdle = LoadTexture("assets/resources/sprites/wolf/Idle.png");
    wolf->base.spriteDead = LoadTexture("assets/resources/sprites/wolf/Dead.png");
    wolf->base.spriteHurt = LoadTexture("assets/resources/sprites/wolf/Hurt.png");

   //Scale
    wolf->base.scale = 0.66f;
    wolf->base.scaleIdle = 0.66f;

    //Sprites size
    wolf->base.frameWidthWalk  = wolf->base.spriteWalk.width / wolf->frameWalk;
    wolf->base.frameHeightWalk = wolf->base.spriteWalk.height;

    wolf->base.frameWidthAtack = wolf->base.spriteWalk.width / wolf->frameWalk;
    wolf->base.frameHeightAtack = wolf->base.spriteWalk.height;
    
    wolf->base.frameWidthHurt = wolf->base.spriteWalk.width / wolf->frameWalk;
    wolf->base.frameHeightHurt = wolf->base.spriteWalk.height;

    wolf->base.frameWidthDead = wolf->base.spriteWalk.width / wolf->frameWalk;
    wolf->base.frameHeightDead = wolf->base.spriteWalk.height;

    wolf->base.frameWidthIdle = wolf->base.spriteWalk.width / wolf->frameWalk;
    wolf->base.frameHeightIdle = wolf->base.spriteWalk.height;
    
    //Cooldown & timer
    wolf->deathAnimTimer = 0.0f;
    wolf->attackCooldown = 0.0f;
    wolf->attackDamageTimer = 0.0f;
    wolf->timeSinceLastGrowl = 0.0f;

    //Sounds
    wolf->wolfHitSound = 
        LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit.wav");
        
    wolf->wolfHitSoundHeavy = 
        LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit-heavy.wav");

    wolf->wolfDeathSound =
        LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-death.wav");

    wolf->wolfScratch = 
        LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-scratch.wav");
        
    wolf->wolfGrowl = 
        LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-growl.wav");
}

void InitWhiteWolf(Wolf *wolf, Vector2 pos)
{
    InitWolfBase(wolf, pos);

    wolf->speed = 140.0f;
    wolf->life = 100;
    wolf->maxLife = 100;

    wolf->base.spriteWalk = LoadTexture("assets/resources/sprites/whiteWolf/Walk.png");
    wolf->base.spriteAtack = LoadTexture("assets/resources/sprites/whiteWolf/Attack.png");
    wolf->base.spriteIdle = LoadTexture("assets/resources/sprites/whiteWolf/Idle.png");
    wolf->base.spriteDead = LoadTexture("assets/resources/sprites/whiteWolf/Dead.png");
    wolf->base.spriteHurt = LoadTexture("assets/resources/sprites/whiteWolf/Hurt.png");
    wolf->base.spriteWalk = LoadTexture("assets/resources/sprites/whiteWolf/Walk.png");

    wolf->frameAtk = 4;

    wolf->wolfScratch = LoadSound("assets/resources/sounds/sound_effects/wolf/white-wolf-scratch.wav");
}

void InitRedWolf(Wolf *wolf, Vector2 pos)
{
    InitWolfBase(wolf, pos);

    wolf->speed = 70.0f;
    wolf->damage = 40;
    wolf->life = 200;
    wolf->maxLife = 200;

    wolf->base.spriteWalk = LoadTexture("assets/resources/sprites/redWolf/Walk.png");
    wolf->base.spriteAtack = LoadTexture("assets/resources/sprites/redWolf/Attack.png");
    wolf->base.spriteIdle = LoadTexture("assets/resources/sprites/redWolf/Idle.png");
    wolf->base.spriteDead = LoadTexture("assets/resources/sprites/redWolf/Dead.png");
    wolf->base.spriteHurt = LoadTexture("assets/resources/sprites/redWolf/Hurt.png");
    wolf->base.spriteWalk = LoadTexture("assets/resources/sprites/redWolf/Walk.png");

    wolf->frameAtk = 5;

    wolf->wolfHitSound = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit-deep.wav");
    wolf->wolfHitSoundHeavy = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit-heavy-deep.wav");
    wolf->wolfDeathSound = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-death-deep.wav");
    wolf->wolfScratch = LoadSound("assets/resources/sounds/sound_effects/wolf/red-wolf-scratch.wav");
}

void UpdateWolf(Wolf *wolf, Player *player, float delta, int currentMapIndex)
{
    if (wolf->life <= LIFE_ZERO && !wolf->base.isDead)
    {
        StopSound(wolf->wolfGrowl);
        PlaySound(wolf->wolfDeathSound);
        wolf->base.isDead = true;
        wolf->isMoving = false;
        wolf->hasHitPlayer = false;
        wolf->base.monsterHasHit = false;
        wolf->frameDead = WOLFS_FRAME_DEAD;
        wolf->deathAnimTimer = TIMER_ZERO;
        wolf->deathAnimationDone = false;
        wolf->base.currentFrame = CURRENT_FRAME_ZERO;
        wolf->isPatrolling = false;
        wolf->base.isAtacking = false;
    }

    if (wolf->base.isDead && wolf->deathAnimationDone)
        return;

    if (wolf->isPatrolling)
    {
        if (wolf->isMoving)
        {
            if (wolf->growlPlayed)
            {
                PlaySound(wolf->wolfGrowl);
                wolf->growlPlayed = false;
                wolf->timeSinceLastGrowl = TIME_SINCE_LAST_GROWL;
            }
            else
            {
                wolf->timeSinceLastGrowl -= delta;

                if (wolf->timeSinceLastGrowl <= COOLDOWN_ZERO)
                {
                    wolf->growlPlayed = true;
                }
            }

            wolf->base.position.x += wolf->speed * wolf->base.direction * delta;

            if (wolf->base.position.x <= wolf->start.x)
            {
                wolf->base.direction = DIRECTION_RIGHT;
            }
            else if (wolf->base.position.x >= wolf->end.x)
            {
                wolf->base.direction = DIRECTION_LEFT;
            }
        }
    }

    wolf->frameCounter++;

    if (wolf->frameCounter >= (GAME_FPS / ANIMATION_FPS))
    {
        wolf->frameCounter = FRAME_COUNTER_ZERO;

        if (wolf->base.monsterHasHit && !wolf->base.isDead)
        {
            wolf->base.currentFrame = (wolf->base.currentFrame + NEXT_FRAME) % wolf->frameHurt;
        }
        else if (wolf->base.isDead && !wolf->deathAnimationDone)
        {
            wolf->deathAnimTimer += delta;

            if (wolf->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
            {
                wolf->frameDead++;
                wolf->deathAnimTimer = TIMER_ZERO;

                if (wolf->frameDead >= WOLFS_MAX_FRAMES_DEAD)
                {
                    wolf->frameDead = WOLFS_FRAME_DEAD;
                    wolf->deathAnimationDone = true;
                    wolf->speed = SPEED_ZERO;
                }
            }
            wolf->base.currentFrame = wolf->frameDead;
        }
        else if (!wolf->base.isDead && wolf->isPatrolling)
        {
            if (wolf->base.isAtacking)
            {
                wolf->base.currentFrame = (wolf->base.currentFrame + NEXT_FRAME) % wolf->frameAtk;
            }
            else if (wolf->isMoving)
            {
                wolf->base.currentFrame = (wolf->base.currentFrame + NEXT_FRAME) % wolf->frameWalk;
            }
            else
            {
                wolf->base.currentFrame = (wolf->base.currentFrame + NEXT_FRAME) % wolf->frameIdle;
            }
        }
    }

    float distance = fabs(player->position.x - wolf->base.position.x);

    if (wolf->base.monsterHasHit)
    {
        wolf->speed = SPEED_ZERO;
    }
    else
    {
        wolf->speed = WOLF_BASE_SPEED;
    }

    if (!wolf->base.isDead && wolf->base.isAtacking)
    {
        PlaySound(wolf->wolfScratch);
        wolf->attackDamageTimer -= delta;

        if (wolf->attackDamageTimer <= TIMER_ZERO && !wolf->hasHitPlayer)
        {
            if (distance <= wolf->attackRange)
            {
                player->life -= wolf->damage;
                wolf->hasHitPlayer = true;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;

                if (player->position.x < wolf->base.position.x)
                {
                    player->position.x -= wolf->push;
                }
                else
                {
                    player->position.x += wolf->push;
                }
            }
        }

        wolf->attackCooldown -= delta;

        if (wolf->attackCooldown <= COOLDOWN_ZERO)
        {
            wolf->attackCooldown = COOLDOWN_ZERO;
            wolf->base.isAtacking = false;
            wolf->isMoving = true;
            wolf->hasHitPlayer = false;
        }
    }
    else
    {
        if (distance <= wolf->attackRange)
        {
            wolf->base.isAtacking = true;
            wolf->isMoving = false;
            wolf->attackCooldown = WOLFS_ATTACK_COOLDOWN_AND_DAMAGE;
            wolf->attackDamageTimer = WOLFS_ATTACK_COOLDOWN_AND_DAMAGE;
            wolf->hasHitPlayer = false;
        }
    }

    if (!wolf->base.isDead && CheckCollisionAABB(
                             player->position.x, player->position.y, player->frameWidth, player->frameHeight,
                             wolf->base.position.x, wolf->base.position.y, wolf->base.frameWidthWalk, wolf->base.frameHeightWalk))
    {
        player->hasHit = true;
        player->hitTimer = HIT_TIMER;

        if (player->position.x < wolf->base.position.x)
            player->position.x = wolf->base.position.x - player->frameWidth;
        else
            player->position.x = wolf->base.position.x + wolf->base.frameWidthWalk;
    }
}

void DrawWolf(Wolf *wolf)
{
    wolf->entity.isDead = wolf->base.isDead;
    wolf->entity.life = wolf->life;
    wolf->entity.maxLife = wolf->maxLife;
    wolf->entity.position.x = wolf->base.position.x;
    wolf->entity.position.y = wolf->base.position.y;

    DrawBar(&wolf->entity, WOLF_OFFSET_X_LIFE_BAR, WOLF_OFFSET_Y_LIFE_BAR, WIDTH_LIFE_BAR, HEIGHT_LIFE_BAR);

    DrawMonsters(&wolf->base, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO);
}

void UnloadWolf(Wolf *wolf)
{
    UnloadTexture(wolf->base.spriteWalk);
    UnloadTexture(wolf->base.spriteAtack);
    UnloadTexture(wolf->base.spriteIdle);
    UnloadTexture(wolf->base.spriteDead);
    UnloadTexture(wolf->base.spriteHurt);
    
    UnloadSound(wolf->wolfHitSound);
    UnloadSound(wolf->wolfHitSoundHeavy);
    UnloadSound(wolf->wolfDeathSound);
    UnloadSound(wolf->wolfScratch);
}
