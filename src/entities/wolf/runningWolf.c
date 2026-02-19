#include "runningWolf.h"

bool CheckCollision(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitRunningWolf(RunningWolf *runningWolf)
{
    //Position
    runningWolf->base.position = (Vector2){1120, 450};
    runningWolf->base.direction = 1;

    //Core stats
    runningWolf->maxLife = 100.0f;
    runningWolf->life = runningWolf->maxLife;
    runningWolf->speedRun = 120.0f;
    runningWolf->speed = 95.0f;
    runningWolf->push = 70.0f;
    runningWolf->damage = 20;

    //Vision 
    runningWolf->attackRange = 100.0f;
    runningWolf->viewPlayer = 300.0f;

    //Animation frame counts
    runningWolf->frameWalk = 11;
    runningWolf->frameAtk = 6;
    runningWolf->frameIdle = 8;
    runningWolf->frameDead = 2;
    runningWolf->frameRun = 9;
    runningWolf->frameHurt = 2;

    //Animation control
    runningWolf->base.currentFrame = 0;
    runningWolf->frameCounter = 0;
    runningWolf->deathAnimationDone = false;

    //State flags
    runningWolf->base.isWalking = false;
    runningWolf->isRunning = false;
    runningWolf->base.isIdle = true;
    runningWolf->base.isAtacking = false;
    runningWolf->hasHitPlayer = false;
    runningWolf->base.monsterHasHit = false;
    runningWolf->base.isDead = false;
   
    //Sprites
    runningWolf->base.spriteWalk = LoadTexture("assets/resources/sprites/wolf/WALK.png");
    runningWolf->base.spriteAtack = LoadTexture("assets/resources/sprites/wolf/Attack_1.png");
    runningWolf->base.spriteIdle = LoadTexture("assets/resources/sprites/wolf/Idle.png");
    runningWolf->base.spriteDead = LoadTexture("assets/resources/sprites/wolf/Dead.png");
    runningWolf->spriteRunWolf = LoadTexture("assets/resources/sprites/wolf/Run.png");
    runningWolf->base.spriteHurt = LoadTexture("assets/resources/sprites/wolf/Hurt.png");

    //Scale
    runningWolf->base.scale = 0.66f;
    runningWolf->base.scaleIdle = 0.66f;

    //Sprites size
    runningWolf->base.frameWidthWalk = runningWolf->base.spriteWalk.width / runningWolf->frameWalk;
    runningWolf->base.frameHeightWalk = runningWolf->base.spriteWalk.height;

    runningWolf->base.frameWidthAtack = runningWolf->base.spriteWalk.width / runningWolf->frameWalk;
    runningWolf->base.frameHeightAtack = runningWolf->base.spriteWalk.height;

    runningWolf->base.frameWidthDead = runningWolf->base.spriteWalk.width / runningWolf->frameWalk;
    runningWolf->base.frameHeightDead = runningWolf->base.spriteWalk.height;

    runningWolf->base.frameWidthIdle = runningWolf->base.spriteWalk.width / runningWolf->frameWalk;
    runningWolf->base.frameHeightIdle = runningWolf->base.spriteWalk.height;

    runningWolf->base.frameWidthHurt = runningWolf->base.spriteWalk.width / runningWolf->frameWalk;
    runningWolf->base.frameHeightHurt = runningWolf->base.spriteWalk.height;

    //Cooldown & Timer
    runningWolf->deathAnimTimer = 0.0f;
    runningWolf->attackCooldown = 0.0f;
    runningWolf->attackDamageTimer = 0.0f;
    
    //Sounds
    runningWolf->wolfHitSound =
    LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit.wav");
    
    runningWolf->wolfHitSoundHeavy = 
    LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit-heavy.wav");
    
    runningWolf->wolfDeathSound = 
    LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-death.wav");
   
    runningWolf->wolfScratch = 
    LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-scratch.wav");
    
    runningWolf->wolfGrowl = 
    LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-growl2.wav");
}

void UpdateRunningWolf(RunningWolf *runningWolf, Player *player, float delta)
{

    if (runningWolf->life <= LIFE_ZERO && !runningWolf->base.isDead)
    {
        StopSound(runningWolf->wolfGrowl);
        PlaySound(runningWolf->wolfDeathSound);
        runningWolf->base.isDead = true;
        runningWolf->isRunning = false;
        runningWolf->base.isWalking = false;
        runningWolf->hasHitPlayer = false;
        runningWolf->base.monsterHasHit = false;
        runningWolf->frameDead = WOLFS_FRAME_DEAD;
        runningWolf->deathAnimTimer = ANIM_DEAD_ZERO;
        runningWolf->deathAnimationDone = false;
        runningWolf->base.currentFrame = CURRENT_FRAME_ZERO;
        runningWolf->base.isIdle = false;
        runningWolf->base.isAtacking = false;
        runningWolf->speed = SPEED_ZERO;
        runningWolf->speedRun = SPEED_ZERO;

        runningWolf->droppedHeart = false;
    }

    if (runningWolf->base.isDead && runningWolf->deathAnimationDone)
        return;

    runningWolf->frameCounter++;
    if (runningWolf->frameCounter >= (GAME_FPS / ANIMATION_FPS))
    {
        runningWolf->frameCounter = FRAME_COUNTER_ZERO;
        if (runningWolf->base.monsterHasHit && !runningWolf->base.isDead)
        {
            runningWolf->base.currentFrame = (runningWolf->base.currentFrame + NEXT_FRAME) % runningWolf->frameHurt;
        }
        else if (runningWolf->base.isDead && !runningWolf->deathAnimationDone)
        {
            runningWolf->deathAnimTimer += delta;

            if (runningWolf->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
            {
                runningWolf->frameDead++;
                runningWolf->deathAnimTimer = DEATH_ANIM_FRAME_TIME;

                if (runningWolf->frameDead >= WOLFS_MAX_FRAMES_DEAD)
                {
                    runningWolf->frameDead = WOLFS_FRAME_DEAD;
                    runningWolf->deathAnimationDone = true;
                    runningWolf->speed = SPEED_ZERO;
                }
            }
            runningWolf->base.currentFrame = runningWolf->frameDead;
        }
        else if (!runningWolf->base.isDead)
        {
            if (runningWolf->isRunning)
            {
                runningWolf->base.currentFrame = (runningWolf->base.currentFrame + NEXT_FRAME) % runningWolf->frameRun;
            }
            else if (runningWolf->base.isWalking)
            {
                runningWolf->base.currentFrame = (runningWolf->base.currentFrame + NEXT_FRAME) % runningWolf->frameWalk;
            }
            else
            {
                runningWolf->base.currentFrame = (runningWolf->base.currentFrame + NEXT_FRAME) % runningWolf->frameIdle;
            }
        }
    }

    if (runningWolf->base.monsterHasHit)
    {
        runningWolf->speed = SPEED_ZERO;
        runningWolf->speedRun = SPEED_ZERO;
    }
    else
    {
        runningWolf->speed = 95.0f;
        runningWolf->speedRun = 120.0f;
    }

    float distanceToPlayer = fabs(player->position.x - runningWolf->base.position.x);

    if (!runningWolf->base.isDead && distanceToPlayer <= runningWolf->viewPlayer)
    {
        runningWolf->base.direction = (player->position.x > runningWolf->base.position.x) ? DIRECTION_RIGHT : DIRECTION_LEFT;
        player->hasHit = false;

        if (!runningWolf->hasRunAttack)
        {
            runningWolf->isRunning = true;
            runningWolf->base.isWalking = false;
            runningWolf->base.isAtacking = false;
            runningWolf->base.isIdle = false;

            player->hasHit = false;
            runningWolf->hasHitPlayer = true;

            runningWolf->base.position.x += runningWolf->speedRun * runningWolf->base.direction * delta;

            if (!runningWolf->base.isDead && distanceToPlayer <= runningWolf->attackRange)
            {
                if (!runningWolf->hasHitPlayer)
                {
                    player->life -= runningWolf->damage;
                    runningWolf->hasHitPlayer = true;
                    player->hasHit = true;

                    if (player->position.x < runningWolf->base.position.x)
                        player->position.x -= runningWolf->push;
                    else
                        player->position.x += runningWolf->push;
                }

                runningWolf->hasRunAttack = true;

                runningWolf->isRunning = false;
                runningWolf->base.isWalking = true;
                runningWolf->base.isAtacking = false;
                runningWolf->hasHitPlayer = false;
                player->hasHit = false;
            }
        }
        else
        {
            runningWolf->isRunning = false;
            runningWolf->base.isWalking = true;
            runningWolf->base.isAtacking = false;
            runningWolf->base.isIdle = false;

            runningWolf->base.position.x += runningWolf->speed * runningWolf->base.direction * delta;

            if (distanceToPlayer <= runningWolf->attackRange)
            {
                runningWolf->base.isAtacking = true;
                runningWolf->base.isWalking = false;

                runningWolf->attackDamageTimer -= delta;

                if (runningWolf->attackDamageTimer <= TIMER_ZERO && !runningWolf->hasHitPlayer)
                {
                    player->life -= runningWolf->damage;
                    runningWolf->hasHitPlayer = true;
                    player->hasHit = true;
                    player->hitTimer = HIT_TIMER;

                    if (player->position.x < runningWolf->base.position.x)
                        player->position.x -= runningWolf->push;
                    else
                        player->position.x += runningWolf->push;
                }

                runningWolf->attackCooldown -= delta;
                if (runningWolf->attackCooldown <= COOLDOWN_ZERO)
                {
                    runningWolf->attackCooldown = WOLFS_ATTACK_COOLDOWN_AND_DAMAGE;
                    runningWolf->attackDamageTimer = WOLFS_ATTACK_COOLDOWN_AND_DAMAGE;
                    runningWolf->hasHitPlayer = false;
                }
            }
            else
            {
                runningWolf->base.isAtacking = false;
                runningWolf->hasHitPlayer = false;
            }
        }
    }
    else
    {
        runningWolf->isRunning = false;
        runningWolf->base.isWalking = false;
        runningWolf->base.isAtacking = false;
        runningWolf->hasRunAttack = false;
        runningWolf->hasHitPlayer = false;
        player->hasHit = false;
        runningWolf->base.isIdle = true;
    }

    if (!runningWolf->base.isDead && CheckCollision(
                                         player->position.x, player->position.y, player->frameWidth, player->frameHeight,
                                         runningWolf->base.position.x, runningWolf->base.position.y, runningWolf->base.frameWidthWalk, runningWolf->base.frameHeightWalk))
    {
        player->hasHit = true;
        player->hitTimer = HIT_TIMER;

        if (player->position.x < runningWolf->base.position.x)
            player->position.x = runningWolf->base.position.x - player->frameWidth;
        else
            player->position.x = runningWolf->base.position.x + runningWolf->base.frameWidthWalk;
    }
}

void DrawRunningWolf(RunningWolf *runningWolf)
{
    runningWolf->entity.isDead = runningWolf->base.isDead;
    runningWolf->entity.life = runningWolf->life;
    runningWolf->entity.maxLife = runningWolf->maxLife;
    runningWolf->entity.position.x = runningWolf->base.position.x;
    runningWolf->entity.position.y = runningWolf->base.position.y;

    DrawBar(&runningWolf->entity, RUNNING_WOLF_OFFSET_X_LIFE_BAR, RUNNING_WOLF_OFFSET_Y_LIFE_BAR, WIDTH_LIFE_BAR, HEIGHT_LIFE_BAR);

    Texture2D currentSprite;

    if (runningWolf->isRunning)
    {
        currentSprite = runningWolf->spriteRunWolf;
    }

    Rectangle source = 
    {
        runningWolf->base.currentFrame * runningWolf->base.frameWidthWalk,
        SPRITE_ROW_BASE,
        runningWolf->base.frameWidthWalk * runningWolf->base.direction,
        runningWolf->base.frameHeightWalk
    };

    Rectangle dest = 
    {
        runningWolf->base.position.x,
        runningWolf->base.position.y,
        runningWolf->base.frameWidthWalk / runningWolf->base.scale,
        runningWolf->base.frameHeightWalk / runningWolf->base.scale
    };

    Vector2 origin = ORIGIN_TOPLEFT;

    DrawMonsters(&runningWolf->base, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO);

    DrawTexturePro(currentSprite, source, dest, origin, ROTATION, WHITE);
}

void UnloadRunningWolf(RunningWolf *runningWolf)
{
    UnloadTexture(runningWolf->base.spriteWalk);
    UnloadTexture(runningWolf->base.spriteAtack);
    UnloadTexture(runningWolf->base.spriteIdle);
    UnloadTexture(runningWolf->base.spriteDead);
    UnloadTexture(runningWolf->spriteRunWolf);
    UnloadTexture(runningWolf->base.spriteHurt);

    UnloadSound(runningWolf->wolfHitSound);
    UnloadSound(runningWolf->wolfHitSoundHeavy);
    UnloadSound(runningWolf->wolfDeathSound);
    UnloadSound(runningWolf->wolfScratch);
}