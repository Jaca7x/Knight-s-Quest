#include "boss.h"

bool CheckCollisionBoss(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

float targetY = 340;

void InitBoss(Boss *boss)
{
    // Position
    boss->base.position = (Vector2){600, -320};
    boss->base.direction = 1.0f;

    // Core stats
    boss->push = 120.0f;
    boss->speed = 10.0f;
    boss->maxLife = 2000.0f;
    boss->life = boss->maxLife;
    boss->damage = 40;

    // Animation frame count
    boss->base.currentFrame = 0;
    boss->frameCounter = 0;

    boss->frameAtk = 9;
    boss->frameDead = 4;
    boss->frameHurt = 2;
    boss->frameIdle = 4;
    boss->frameWalk = 6;

    // Frame size calculation
    boss->base.spriteAtack = LoadTexture("assets/resources/sprites/boss/boss_attack.png");
    boss->base.spriteDead = LoadTexture("assets/resources/sprites/boss/boss_dead.png");
    boss->base.spriteIdle = LoadTexture("assets/resources/sprites/boss/boss_idle.png");
    boss->base.spriteHurt = LoadTexture("assets/resources/sprites/boss/boss_hurt.png");
    boss->base.spriteWalk = LoadTexture("assets/resources/sprites/boss/boss_walk.png");

    boss->base.frameWidthWalk = boss->base.spriteWalk.width / boss->frameWalk;
    boss->base.frameHeightWalk = boss->base.spriteWalk.height;

    boss->base.frameWidthIdle = boss->base.spriteIdle.width / boss->frameIdle;
    boss->base.frameHeightIdle = boss->base.spriteIdle.height;

    boss->base.frameWidthAtack = boss->base.spriteAtack.width / boss->frameAtk;
    boss->base.frameHeightAtack = boss->base.spriteAtack.height;

    boss->base.frameWidthHurt = boss->base.spriteHurt.width / boss->frameHurt;
    boss->base.frameHeightHurt = boss->base.spriteHurt.height;

    boss->base.frameWidthDead = boss->base.spriteDead.width / boss->frameDead;
    boss->base.frameHeightDead = boss->base.spriteDead.height;

    // Hurt
    boss->hurtTimer = 0.0f;
    boss->hurtDuration = 1.0f;

    // Atack 
    boss->attackTime = 0.0f;
    boss->attackCooldown = 0.0f;
    boss->hitFrame = 4;
    boss->hasAppliedDamage = false;

    // States
    boss->base.isAtacking = false;
    boss->base.isWalking = false;
    boss->base.isIdle = true;
    boss->base.isDead = false;
    boss->base.monsterHasHit = false;
    boss->musicStarted = false;

    // View
    boss->attackRange = 100.0f;
    boss->viewPlayer = 300.0f;

    // Sounds
    boss->bossMusic = 
        LoadMusicStream("assets/resources/music/boss-soundtrack.mp3");

    boss->bossGrounImpact = 
    LoadSound("assets/resources/sounds/sound_effects/boss/boss-impact.wav");
    
    boss->bossWalkSound = 
    LoadSound("assets/resources/sounds/sound_effects/boss/boss-walk.wav");
    
    boss->bossHurtSound = 
    LoadSound("assets/resources/sounds/sound_effects/boss/boss-hurt.wav");
    
    boss->bossDeathSound = 
    LoadSound("assets/resources/sounds/sound_effects/boss/boss-death.wav");
    
    boss->bossAttackSound = 
    LoadSound("assets/resources/sounds/sound_effects/boss/boss-attack.wav");
}

void UpdateBoss(Boss *boss, Player *player, float delta, bool *bossTriggered)
{
    float speedY = DESCENT_SPEED;

    if (boss->base.position.y < targetY && *bossTriggered)
    {
        boss->base.position.y += speedY * delta;
        boss->base.isIdle = false;
        boss->base.isWalking = false;
        boss->base.isAtacking = false;

        if (boss->base.position.y > targetY)
        {
            PlaySound(boss->bossGrounImpact);
            boss->base.position.y = targetY;
        }
    }

    if (boss->base.isDead)
    {
        boss->frameCounter++;

        if (boss->frameCounter >= BOSS_BASE_FRAME_ANIMATION)
        {
            boss->frameCounter = FRAME_COUNTER_ZERO;
            boss->base.currentFrame++;

            if (boss->base.currentFrame >= boss->frameDead)
                boss->base.currentFrame = boss->frameDead - PREVIOUS_FRAME;
        }

        return;
    }

    if (boss->life <= LIFE_ZERO && !boss->base.isDead)
    {
        PlaySound(boss->bossDeathSound);
        boss->base.isDead = true;
        boss->base.monsterHasHit = false;
        boss->base.currentFrame = CURRENT_FRAME_ZERO;
        boss->frameDead = BOSS_FRAME_DEAD;
        boss->base.isWalking = false;
        boss->base.isAtacking = false;
        boss->base.isIdle = false;
    }

    if (boss->base.monsterHasHit)
    {
        boss->base.isIdle = false;
        boss->base.isWalking = false;
        boss->base.isAtacking = false;

        boss->hurtTimer += delta;

        if (boss->hurtTimer >= boss->hurtDuration)
        {
            boss->base.monsterHasHit = false;
            boss->hurtTimer = TIMER_ZERO;
            boss->base.isIdle = true;
        }
    }

    boss->frameCounter++;

    if (boss->base.monsterHasHit && boss->frameCounter >= BOSS_HURT_FRAME_ANIMATION)
    {
        boss->frameCounter = CURRENT_FRAME_ZERO;
        boss->base.currentFrame = (boss->base.currentFrame + NEXT_FRAME) % boss->frameHurt;
    }
    else if (boss->base.isAtacking && boss->frameCounter >= BOSS_ATACK_FRAME_ANIMATION)
    {
        boss->frameCounter = FRAME_COUNTER_ZERO;
        boss->base.currentFrame = (boss->base.currentFrame + NEXT_FRAME) % boss->frameAtk;
    }
    else if (boss->base.isIdle && boss->frameCounter >= BOSS_BASE_FRAME_ANIMATION)
    {
        boss->frameCounter = FRAME_COUNTER_ZERO;
        boss->base.currentFrame = (boss->base.currentFrame + NEXT_FRAME) % boss->frameIdle;
    }
    else if (boss->base.isWalking && boss->frameCounter >= BOSS_BASE_FRAME_ANIMATION)
    {
        PlaySound(boss->bossWalkSound);
        boss->frameCounter = FRAME_COUNTER_ZERO;
        boss->base.currentFrame = (boss->base.currentFrame + NEXT_FRAME) % boss->frameWalk;
    }
    else if (boss->base.isDead && boss->frameCounter >= BOSS_BASE_FRAME_ANIMATION)
    {
        boss->frameCounter = FRAME_COUNTER_ZERO;
        boss->base.currentFrame = (boss->base.currentFrame + NEXT_FRAME);

        if (boss->base.currentFrame >= boss->frameDead)
            boss->base.currentFrame = boss->frameDead - PREVIOUS_FRAME;
    }

    float distance = fabs(player->position.x - boss->base.position.x);

    if (distance >= boss->viewPlayer)
    {
        boss->base.isIdle = true;
        boss->base.isWalking = false;
        boss->base.isAtacking = false;
        return;
    }

    boss->base.direction = (player->position.x > boss->base.position.x) ? DIRECTION_RIGHT : DIRECTION_LEFT;

    if (boss->base.isAtacking)
    {
        PlaySound(boss->bossAttackSound);
        boss->frameCounter++;

        if (boss->frameCounter >= BOSS_ATACK_FRAME_ANIMATION)
        {
            boss->frameCounter = FRAME_COUNTER_ZERO;
            boss->base.currentFrame++;

            if (boss->base.currentFrame >= boss->frameAtk)
            {
                boss->base.currentFrame = FRAME_COUNTER_ZERO;
                boss->base.isAtacking = false;
                boss->hasAppliedDamage = false;
                boss->attackCooldown = BOSS_ATACK_COOLDOWN;
            }
        }

        if (boss->base.currentFrame == boss->hitFrame && !boss->hasAppliedDamage)
        {
            if (CheckCollisionBoss(
                    player->position.x, player->position.y, player->frameWidth, player->frameHeight,
                    boss->base.position.x, boss->base.position.y, boss->base.frameWidthAtack, boss->base.frameHeightAtack))
            {
                player->life -= boss->damage;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;
                boss->hasAppliedDamage = true;
                player->position.x -= boss->push;
            }
        }
        return;
    }

    if (distance <= boss->attackRange && boss->attackCooldown <= COOLDOWN_ZERO)
    {
        boss->base.isAtacking = true;
        boss->base.currentFrame = CURRENT_FRAME_ZERO;
        boss->frameCounter = FRAME_COUNTER_ZERO;
        boss->hasAppliedDamage = false;
        return;
    }

    boss->attackCooldown -= delta;

    boss->base.isWalking = true;
    boss->base.isIdle = false;

    if (boss->base.isWalking)
    {
        boss->base.position.x += boss->speed * boss->base.direction * delta;
    }
}

void DrawBoss(Boss *boss)
{
    boss->entity.isDead = boss->base.isDead;
    boss->entity.life = boss->life;
    boss->entity.maxLife = boss->maxLife;
    boss->entity.position.x = BOSS_POSITION_X_LIFE_BAR;
    boss->entity.position.y = BOSS_POSITION_Y_LIFE_BAR;

    DrawText("Brakkor, o Dourado", BOSS_NAME_WIDTH, BOSS_NAME_HEIGHT, BOSS_NAME_FONT_SIZE, BLACK);
    DrawBar(&boss->entity, OFFSET_ZERO, OFFSET_ZERO, BOSS_WIDTH_LIFE_BAR, BOSS_HEIGHT_LIFE_BAR);

    DrawBossAnim(&boss->base);
}

void UnloadBoss(Boss *boss)
{
    UnloadTexture(boss->base.spriteAtack);
    UnloadTexture(boss->base.spriteDead);
    UnloadTexture(boss->base.spriteHurt);
    UnloadTexture(boss->base.spriteIdle);
    UnloadTexture(boss->base.spriteWalk);

    UnloadMusicStream(boss->bossMusic);

    UnloadSound(boss->bossHurtSound);
    UnloadSound(boss->bossWalkSound);
    UnloadSound(boss->bossDeathSound);
    UnloadSound(boss->bossAttackSound);
    UnloadSound(boss->bossGrounImpact);
}
