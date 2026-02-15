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
    wolf->position = pos;
    wolf->start = pos;
    wolf->end = (Vector2){pos.x + 100, pos.y};

    wolf->life = 100;
    wolf->maxLife = 100;
    wolf->speed = 95.0f;

    wolf->currentFrame = 0;
    wolf->frameCounter = 0;

    wolf->spriteWalkWolf = LoadTexture("assets/resources/sprites/wolf/walk.png");
    wolf->spriteAtkWolf = LoadTexture("assets/resources/sprites/wolf/Attack_1.png");
    wolf->spriteIdleWolf = LoadTexture("assets/resources/sprites/wolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("assets/resources/sprites/wolf/Dead.png");
    wolf->spriteHurtWolf = LoadTexture("assets/resources/sprites/wolf/Hurt.png");

    wolf->frameWalk = 11;
    wolf->frameAtk = 6;
    wolf->frameIdle = 8;
    wolf->frameDead = 2;
    wolf->frameHurt = 2;

    wolf->frameWidth  = wolf->spriteWalkWolf.width / wolf->frameWalk;
    wolf->frameHeight = wolf->spriteWalkWolf.height;

    wolf->scale = 1.5f;
    wolf->direction = 1;
    wolf->isMoving = true;

    wolf->isDead = false;
    wolf->deathAnimationDone = false;
    wolf->deathAnimTimer = 0.0f;

    wolf->isPatrolling = true;

    wolf->isAttacking = false;
    wolf->hasHitPlayer = false;
    wolf->wolfHasHit = false;

    wolf->attackRange = 100.0f;
    wolf->attackCooldown = 0.0f;
    wolf->attackDamageTimer = 0.0f;
    wolf->damage = 20;

    wolf->droppedHeart = false;

    wolf->wolfHitSound = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit.wav");
    wolf->wolfHitSoundHeavy = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit-heavy.wav");
    wolf->wolfDeathSound = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-death.wav");
    wolf->wolfScratch = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-scratch.wav");

    wolf->wolfGrowl = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-growl.wav");
    wolf->growlPlayed = true;
    wolf->timeSinceLastGrowl = 0.0f;
}

void InitWhiteWolf(Wolf *wolf, Vector2 pos)
{
    InitWolfBase(wolf, pos);

    wolf->speed = 140.0f;
    wolf->life = 100;
    wolf->maxLife = 100;

    wolf->spriteWalkWolf = LoadTexture("assets/resources/sprites/whiteWolf/Walk.png");
    wolf->spriteAtkWolf = LoadTexture("assets/resources/sprites/whiteWolf/Attack.png");
    wolf->spriteIdleWolf = LoadTexture("assets/resources/sprites/whiteWolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("assets/resources/sprites/whiteWolf/Dead.png");
    wolf->spriteHurtWolf = LoadTexture("assets/resources/sprites/whiteWolf/Hurt.png");
    wolf->spriteWalkWolf = LoadTexture("assets/resources/sprites/whiteWolf/Walk.png");

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

    wolf->spriteWalkWolf = LoadTexture("assets/resources/sprites/redWolf/Walk.png");
    wolf->spriteAtkWolf = LoadTexture("assets/resources/sprites/redWolf/Attack.png");
    wolf->spriteIdleWolf = LoadTexture("assets/resources/sprites/redWolf/Idle.png");
    wolf->spriteDeadWolf = LoadTexture("assets/resources/sprites/redWolf/Dead.png");
    wolf->spriteHurtWolf = LoadTexture("assets/resources/sprites/redWolf/Hurt.png");
    wolf->spriteWalkWolf = LoadTexture("assets/resources/sprites/redWolf/Walk.png");

    wolf->frameAtk = 5;

    wolf->wolfHitSound = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit-deep.wav");
    wolf->wolfHitSoundHeavy = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-hit-heavy-deep.wav");
    wolf->wolfDeathSound = LoadSound("assets/resources/sounds/sound_effects/wolf/wolf-death-deep.wav");
    wolf->wolfScratch = LoadSound("assets/resources/sounds/sound_effects/wolf/red-wolf-scratch.wav");
}

void UpdateWolf(Wolf *wolf, Player *player, float delta, int currentMapIndex)
{
    if (wolf->life <= LIFE_ZERO && !wolf->isDead)
    {
        StopSound(wolf->wolfGrowl);
        PlaySound(wolf->wolfDeathSound);
        wolf->isDead = true;
        wolf->isMoving = false;
        wolf->hasHitPlayer = false;
        wolf->wolfHasHit = false;
        wolf->frameDead = 1;
        wolf->deathAnimTimer = TIMER_ZERO;
        wolf->deathAnimationDone = false;
        wolf->currentFrame = CURRENT_FRAME_ZERO;
        wolf->isPatrolling = false;
        wolf->isAttacking = false;
    }

    if (wolf->isDead && wolf->deathAnimationDone)
        return;

    if (wolf->isPatrolling)
    {
        if (wolf->isMoving)
        {
            if (wolf->growlPlayed)
            {
                PlaySound(wolf->wolfGrowl);
                wolf->growlPlayed = false;
                wolf->timeSinceLastGrowl = 8.0f;
            }
            else
            {
                wolf->timeSinceLastGrowl += delta;
                if (wolf->timeSinceLastGrowl <= COOLDOWN_ZERO)
                {
                    wolf->growlPlayed = true;
                }
            }

            wolf->position.x += wolf->speed * wolf->direction * delta;

            if (wolf->position.x <= wolf->start.x)
            {
                wolf->direction = DIRECTION_RIGHT;
            }
            else if (wolf->position.x >= wolf->end.x)
            {
                wolf->direction = DIRECTION_LEFT;
            }
        }
    }

    wolf->frameCounter++;
    if (wolf->frameCounter >= (GAME_FPS / ANIMATION_FPS))
    {
        wolf->frameCounter = FRAME_COUNTER_ZERO;

        if (wolf->wolfHasHit && !wolf->isDead)
        {
            wolf->currentFrame = (wolf->currentFrame + NEXT_FRAME) % wolf->frameHurt;
        }
        else if (wolf->isDead && !wolf->deathAnimationDone)
        {
            wolf->deathAnimTimer += delta;

            if (wolf->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
            {
                wolf->frameDead++;
                wolf->deathAnimTimer = TIMER_ZERO;

                if (wolf->frameDead >= 2)
                {
                    wolf->frameDead = 1;
                    wolf->deathAnimationDone = true;
                    wolf->speed = SPEED_ZERO;
                }
            }
            wolf->currentFrame = wolf->frameDead;
        }
        else if (!wolf->isDead && wolf->isPatrolling)
        {
            if (wolf->isAttacking)
            {
                wolf->currentFrame = (wolf->currentFrame + NEXT_FRAME) % wolf->frameAtk;
            }
            else if (wolf->isMoving)
            {
                wolf->currentFrame = (wolf->currentFrame + NEXT_FRAME) % wolf->frameWalk;
            }
            else
            {
                wolf->currentFrame = (wolf->currentFrame + NEXT_FRAME) % wolf->frameIdle;
            }
        }
    }

    // ====== ATAQUE ======
    float distance = fabs(player->position.x - wolf->position.x);

    if (wolf->wolfHasHit)
    {
        wolf->speed = SPEED_ZERO;
    }
    else
    {
        wolf->speed = 95.0f;
    }

    if (!wolf->isDead && wolf->isAttacking)
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

                if (player->position.x < wolf->position.x)
                {
                    player->position.x -= 70;
                }
                else
                {
                    player->position.x += 70;
                }
            }
        }

        wolf->attackCooldown -= delta;
        if (wolf->attackCooldown <= COOLDOWN_ZERO)
        {
            wolf->attackCooldown = COOLDOWN_ZERO;
            wolf->isAttacking = false;
            wolf->isMoving = true;
            wolf->hasHitPlayer = false;
        }
    }
    else
    {
        if (distance <= wolf->attackRange)
        {
            wolf->isAttacking = true;
            wolf->isMoving = false;
            wolf->attackCooldown = 0.4f;
            wolf->attackDamageTimer = 0.4f;
            wolf->hasHitPlayer = false;
        }
    }

    // ====== COLISÃO FÍSICA (BLOQUEAR PLAYER) ======
    if (!wolf->isDead && CheckCollisionAABB(
                             player->position.x, player->position.y, player->frameWidth, player->frameHeight,
                             wolf->position.x, wolf->position.y, wolf->frameWidth, wolf->frameHeight))
    {
        player->hasHit = true;
        player->hitTimer = HIT_TIMER;

        if (player->position.x < wolf->position.x)
            player->position.x = wolf->position.x - player->frameWidth;
        else
            player->position.x = wolf->position.x + wolf->frameWidth;
    }
}

void DrawWolf(Wolf *wolf)
{
    wolf->entity.isDead = wolf->isDead;
    wolf->entity.life = wolf->life;
    wolf->entity.maxLife = wolf->maxLife;
    wolf->entity.position.x = wolf->position.x;
    wolf->entity.position.y = wolf->position.y;

    DrawBar(&wolf->entity, 20, -30);

    Rectangle source = {
        wolf->currentFrame * wolf->frameWidth,
        SPRITE_ROW_BASE,
        wolf->frameWidth * wolf->direction,
        wolf->frameHeight};

    Rectangle dest = {
        wolf->position.x,
        wolf->position.y,
        wolf->frameWidth * wolf->scale,
        wolf->frameHeight * wolf->scale};

    Vector2 origin = ORIGIN_TOPLEFT;

    if (wolf->isDead)
    {
        DrawTexturePro(wolf->spriteDeadWolf, source, dest, origin, ROTATION, WHITE);
    }
    else if (wolf->wolfHasHit)
    {
        DrawTexturePro(wolf->spriteHurtWolf, source, dest, origin, ROTATION, WHITE);
    }
    else if (wolf->isAttacking)
    {
        DrawTexturePro(wolf->spriteAtkWolf, source, dest, origin, ROTATION, WHITE);
    }
    else
    {
        DrawTexturePro(wolf->spriteWalkWolf, source, dest, origin, ROTATION, WHITE);
    }
}

void UnloadWolf(Wolf *wolf)
{
    UnloadTexture(wolf->spriteWalkWolf);
    UnloadTexture(wolf->spriteAtkWolf);
    UnloadTexture(wolf->spriteIdleWolf);
    UnloadTexture(wolf->spriteDeadWolf);
    UnloadTexture(wolf->spriteHurtWolf);
    
    UnloadSound(wolf->wolfHitSound);
    UnloadSound(wolf->wolfHitSoundHeavy);
    UnloadSound(wolf->wolfDeathSound);
    UnloadSound(wolf->wolfScratch);
}
