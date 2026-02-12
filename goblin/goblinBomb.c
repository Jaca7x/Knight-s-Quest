#include "goblinBomb.h"

bool CheckColisionGoblinBomb(float x1, float y1, float w1, float h1,
                             float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitGoblinBomb(GoblinBomb *goblinBomb)
{
    goblinBomb->base.spriteIdle = LoadTexture("resources/sprites/goblinBomb/Idle.png");
    goblinBomb->spriteAttackBomb = LoadTexture("resources/sprites/goblinBomb/Attack3.png");
    goblinBomb->base.spriteAtack = LoadTexture("resources/sprites/goblinBomb/Attack1.png");
    goblinBomb->base.spriteHurt = LoadTexture("resources/sprites/goblinBomb/Hurt.png");
    goblinBomb->base.spriteDead= LoadTexture("resources/sprites/goblinBomb/Death.png");
    goblinBomb->base.spriteWalk = LoadTexture("resources/sprites/goblinBomb/Run.png");

    goblinBomb->base.position = (Vector2){800, 440};
    goblinBomb->bomb.pos = (Vector2){800, 440};

    goblinBomb->maxLife = 70.0f;
    goblinBomb->life = goblinBomb->maxLife;
    goblinBomb->speed = 30.0f;
    goblinBomb->damage = 20;

    goblinBomb->frameIdle = 4;
    goblinBomb->frameAttackBomb = 12;
    goblinBomb->frameAttack = 8;
    goblinBomb->frameHurt = 4;
    goblinBomb->frameDeath = 4;
    goblinBomb->frameRun = 8;
    goblinBomb->base.scale = 0.5;
    goblinBomb->base.scaleIdle = 0.5;

    goblinBomb->base.frameWidthIdle = goblinBomb->base.spriteIdle.width / goblinBomb->frameIdle;
    goblinBomb->base.frameHeightIdle = goblinBomb->base.spriteIdle.height;

    goblinBomb->frameWidthAttackBomb = goblinBomb->spriteAttackBomb.width / goblinBomb->frameAttackBomb;
    goblinBomb->frameHeightAttackBomb = goblinBomb->spriteAttackBomb.height;

    goblinBomb->base.frameWidthAtack = goblinBomb->base.spriteAtack.width / goblinBomb->frameAttack;
    goblinBomb->base.frameHeightAtack = goblinBomb->base.spriteAtack.height;

    goblinBomb->base.frameWidthHurt = goblinBomb->base.spriteHurt.width / goblinBomb->frameHurt;
    goblinBomb->base.frameHeightHurt = goblinBomb->base.spriteHurt.height;

    goblinBomb->base.frameWidthDead = goblinBomb->base.spriteDead.width / goblinBomb->frameDeath;
    goblinBomb->base.frameHeightDead = goblinBomb->base.spriteDead.height;

    goblinBomb->base.frameWidthWalk = goblinBomb->base.spriteWalk.width / goblinBomb->frameRun;
    goblinBomb->base.frameHeightWalk = goblinBomb->base.spriteWalk.height;

    goblinBomb->base.currentFrame = 0;
    goblinBomb->frameCounter = 0;

    goblinBomb->base.isIdle = true;
    goblinBomb->isAttackBomb = false;
    goblinBomb->base.isWalking = false;

    goblinBomb->bomb.isActive = false;
    goblinBomb->bombExplode = false;
    goblinBomb->hasThrownBomb = false;
    goblinBomb->base.isAtacking = false;

    goblinBomb->droppedHeart = false;

    goblinBomb->spriteBomb = LoadTexture("resources/sprites/goblinBomb/Bomb_sprite.png");
    goblinBomb->damageBomb = 50;
    goblinBomb->timerForExplosion = 0.0f;
    goblinBomb->radiusToDamage = 50.0f;
    goblinBomb->bombRange = 200.0f;
    goblinBomb->attackRange = 35.0f;
    goblinBomb->viewPlayer = 150.0f;

    goblinBomb->bomb.frameBomb = 19;
    goblinBomb->bomb.frameWidthBomb = goblinBomb->spriteBomb.width / goblinBomb->bomb.frameBomb;
    goblinBomb->bomb.frameHeightBomb = goblinBomb->spriteBomb.height;

    goblinBomb->timerAttackBomb = 0.0f;
    goblinBomb->animAttackBombEnd = false;
    goblinBomb->bomb.currentFrameBomb = 0;
    goblinBomb->bomb.frameCounterBomb = 0;
    goblinBomb->bomb.speed = 65.0f;

    goblinBomb->bomb.playerIsDamage = false;

    goblinBomb->base.direction = 1;

    goblinBomb->animAttackTimer = 0.0f;

    goblinBomb->base.monsterHasHit = false;
    goblinBomb->isDead = false;
    goblinBomb->wasWalking = false;
    goblinBomb->attackSoundPlayed = false;

    goblinBomb->hurtDuration = 2.0f;
    goblinBomb->hurtTimer = 0.0f;

    goblinBomb->bomb.explosion = LoadSound("resources/sounds/sound_effects/bomb/explosion.wav");
    goblinBomb->bomb.timer = LoadSound("resources/sounds/sound_effects/bomb/timer.wav");
    goblinBomb->soundAttack = LoadSound("resources/sounds/sound_effects/wolf/red-wolf-scratch.wav");
    goblinBomb->soundBagGoblin = LoadSound("resources/sounds/sound_effects/goblin/bag.wav");
}

void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player)
{
    if (goblinBomb->base.isWalking)
    {
        goblinBomb->base.position.x += goblinBomb->speed * goblinBomb->base.direction * delta;
    }
    
    goblinBomb->frameCounter++;
    goblinBomb->bomb.frameCounterBomb++;

    if (goblinBomb->life <= 0 && !goblinBomb->isDead)
    {
        goblinBomb->isDead = true;
        goblinBomb->base.isIdle = false;
        goblinBomb->isAttackBomb = false;
        goblinBomb->base.isWalking = false;
        goblinBomb->base.isAtacking = false;
        goblinBomb->base.currentFrame = 0;
        
        goblinBomb->bomb.isActive = false;
        
        return;
    }

    if (goblinBomb->base.monsterHasHit && !goblinBomb->isDead)
    {
        goblinBomb->base.isIdle = false;
        goblinBomb->isAttackBomb = false;
        goblinBomb->base.isAtacking = false;
        goblinBomb->base.isWalking = false;

        goblinBomb->hurtTimer += delta;

        if (goblinBomb->hurtTimer >= goblinBomb->hurtDuration)
        {
            goblinBomb->base.monsterHasHit = false;
            goblinBomb->hurtTimer = 0.0f;
            goblinBomb->base.isIdle = true;
        }
    }

    if (goblinBomb->isDead)
    {
        if (goblinBomb->frameCounter >= 10)
        {
            goblinBomb->frameCounter = 0;
            goblinBomb->base.currentFrame++;

            if (goblinBomb->base.currentFrame >= goblinBomb->frameDeath)
                goblinBomb->base.currentFrame = goblinBomb->frameDeath - 1;
        }

        return;
    }
    else if (goblinBomb->frameCounter >= 10)
    {
        goblinBomb->frameCounter = 0;

        if (goblinBomb->base.isIdle)
        {
            goblinBomb->base.currentFrame++;
            if (goblinBomb->base.currentFrame >= goblinBomb->frameIdle)
            {
                goblinBomb->base.currentFrame = 0;
            }
        }
        else if (goblinBomb->isAttackBomb)
        {
            goblinBomb->base.currentFrame++;

            if (goblinBomb->base.currentFrame == 2 && !goblinBomb->soundBagPlayed)
            {
                PlaySound(goblinBomb->soundBagGoblin);
                goblinBomb->soundBagPlayed = true;
            }
            
            if (goblinBomb->base.currentFrame >= goblinBomb->frameAttackBomb)
            {
                goblinBomb->base.currentFrame = 0;
            }
        }
        else if (goblinBomb->base.isAtacking)
        {
            goblinBomb->base.currentFrame++;
            
            if (goblinBomb->base.currentFrame == 6 && !goblinBomb->attackSoundPlayed)
            {
                PlaySound(goblinBomb->soundAttack);
                goblinBomb->attackSoundPlayed = true;
            }
            
            if (goblinBomb->base.currentFrame >= goblinBomb->frameAttack)
            {
                goblinBomb->base.currentFrame = 0;
            }
        }
        else if (goblinBomb->base.monsterHasHit)
        {
            goblinBomb->base.currentFrame++;
            if (goblinBomb->base.currentFrame >= goblinBomb->frameHurt)
            {
                goblinBomb->base.currentFrame = 0;
            }
        }
        else if (goblinBomb->base.isWalking)
        {
            goblinBomb->base.currentFrame++;
        
            if (goblinBomb->base.currentFrame >= goblinBomb->frameRun)
            {
                goblinBomb->base.currentFrame = 0;
            }
        }
    }

    if (goblinBomb->bomb.frameCounterBomb >= 10)
    {
        goblinBomb->bomb.frameCounterBomb = 0;

        if (goblinBomb->bomb.isActive)
        {
            goblinBomb->bomb.currentFrameBomb++;

            if (goblinBomb->bomb.currentFrameBomb == 14)
            {
                PlaySound(goblinBomb->bomb.explosion);
            }
            
            if (goblinBomb->bomb.currentFrameBomb >= goblinBomb->bomb.frameBomb)
            {
                goblinBomb->bomb.currentFrameBomb = 0;
                goblinBomb->bomb.frameCounterBomb = 0;
            }
        }
    }

    float disatanceToAttack = fabs(player->position.x - goblinBomb->base.position.x);

    goblinBomb->base.direction = (player->position.x > goblinBomb->base.position.x) ? 1 : -1;

    if (disatanceToAttack <= goblinBomb->viewPlayer && goblinBomb->hasThrownBomb)
    {
        if (disatanceToAttack > goblinBomb->attackRange)
        {
            goblinBomb->base.isWalking = true;
            goblinBomb->base.isIdle = false;
            goblinBomb->base.isAtacking = false;
        }
        else
        {  
            goblinBomb->attackSoundPlayed = false;
            goblinBomb->base.isWalking = false;
            goblinBomb->base.isAtacking = true;
            goblinBomb->base.isIdle = false;
        }
    }
    else
    {
        goblinBomb->base.isWalking = false;
        goblinBomb->base.isAtacking = false;
        goblinBomb->base.isIdle = true;
    }

    static bool wasAttacking = false;

    if (!goblinBomb->hasThrownBomb)
    {
        if (goblinBomb->bombRange > disatanceToAttack)
        {
            if (goblinBomb->attackRange > disatanceToAttack)
            {
                goblinBomb->base.isAtacking = true;
                goblinBomb->isAttackBomb = false;
                goblinBomb->base.isIdle = false;

                if (!wasAttacking)
                {
                    PlaySound(goblinBomb->soundAttack);
                    goblinBomb->base.currentFrame = 0; 
                }
            }
            else
            {
                goblinBomb->isAttackBomb = true;
                goblinBomb->soundBagPlayed = false;
                goblinBomb->base.isAtacking = false;
                goblinBomb->base.isIdle = false;
            }
        }
        else
        {
            goblinBomb->base.isIdle = true;
            goblinBomb->base.isAtacking = false;
            goblinBomb->isAttackBomb = false;
        }

        if (goblinBomb->isAttackBomb)
        {
            goblinBomb->timerAttackBomb += delta;

            if (goblinBomb->timerAttackBomb >= 1.5f)
            {
                goblinBomb->isAttackBomb = false;
                goblinBomb->base.isIdle = true;

                goblinBomb->bomb.isActive = true;
                PlaySound(goblinBomb->bomb.timer);
                goblinBomb->timerAttackBomb = 0.0f;
            }
        }
    }
    else
    {
        float right = 200.0f;
        
        if (goblinBomb->attackRange > disatanceToAttack && goblinBomb->base.direction == -1)
        {
            goblinBomb->base.isAtacking = true;
            goblinBomb->base.isIdle = false;
            goblinBomb->base.isWalking = false;
        }
        else if (right > disatanceToAttack && goblinBomb->base.direction == 1)
        {
            goblinBomb->base.isAtacking = true;
            goblinBomb->base.isIdle = false;
            goblinBomb->base.isWalking = false;
        }
        goblinBomb->isAttackBomb = false;
    }

    wasAttacking = goblinBomb->base.isAtacking;

    Rectangle playerRec =
        {
            player->position.x + PLAYER_HITBOX_OFFSET_X,
            player->position.y + PLAYER_HITBOX_OFFSET_Y,
            player->frameWidth,
            player->frameHeight};

    Rectangle goblinRec =
        {
            goblinBomb->base.position.x + GOBLIN_HITBOX_OFFSET_X,
            goblinBomb->base.position.y + GOBLIN_HITBOX_OFFSET_Y,
            goblinBomb->base.frameWidthAtack / GOBLIN_HITBOX_SCALE,
            goblinBomb->base.frameHeightAtack / GOBLIN_HITBOX_SCALE};

    Vector2 centerCircle =
        {
            goblinBomb->bomb.pos.x +
                goblinBomb->bomb.frameWidthBomb / BOMB_CENTER_SCALE +
                BOMB_CENTER_OFFSET_X,

            goblinBomb->bomb.pos.y +
                goblinBomb->bomb.frameHeightBomb / BOMB_CENTER_SCALE +
                BOMB_CENTER_OFFSET_Y};

    if (goblinBomb->bomb.isActive)
    {
        goblinBomb->timerForExplosion += delta;

        if (goblinBomb->timerForExplosion < BOMB_EXPLOSION_DELAY)
        {
            goblinBomb->bomb.pos.x += goblinBomb->bomb.speed * goblinBomb->base.direction * delta;

            if (!goblinBomb->bomb.playerIsDamage && CheckCollisionCircleRec(centerCircle, goblinBomb->radiusToDamage, playerRec) 
            && goblinBomb->bomb.currentFrameBomb == 14)
            {
                player->life -= goblinBomb->damageBomb;
                goblinBomb->bomb.playerIsDamage = true;
                player->hasHit = true;
                player->hitTimer = 0.4f;
            }
        }
        else
        {
            goblinBomb->bombExplode = true;
            goblinBomb->bomb.isActive = false;
            goblinBomb->hasThrownBomb = true;

            goblinBomb->base.isIdle = true;
            goblinBomb->isAttackBomb = false;
            goblinBomb->timerForExplosion = 0.0f;
            goblinBomb->bomb.playerIsDamage = false;
        }
    }

    goblinBomb->animAttackTimer -= delta;

    if (goblinBomb->base.isAtacking && goblinBomb->base.currentFrame == 7 && goblinBomb->animAttackTimer <= 0.0)
    {
        if (CheckCollisionRecs(playerRec, goblinRec))
        {
            player->life -= goblinBomb->damage;
            player->hasHit = true;
            player->hitTimer = 0.4f;

            goblinBomb->animAttackTimer = 1.0f;
        }
    }

    int push = 50;

    if (!goblinBomb->isDead && CheckColisionGoblinBomb(
        player->position.x + PLAYER_HITBOX_OFFSET_X, 
        player->position.y + PLAYER_HITBOX_OFFSET_Y, 
        player->frameWidth / PLAYER_HITBOX_WIDTH_DIV, 
        player->frameHeight,

        goblinBomb->base.position.x + GOBLIN_HITBOX_OFFSET_X, 
        goblinBomb->base.position.y + GOBLIN_HITBOX_OFFSET_Y, 
        goblinBomb->base.frameWidthHurt / GOBLIN_SCALE_WIDTH, 
        goblinBomb->base.frameHeightHurt / GOBLIN_SCALE_HEIGHT
        ))
    {
        player->position.x += (player->position.x < goblinBomb->base.position.x) ? -push : push;
    }
}

void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player)
{
    goblinBomb->entity.isDead = goblinBomb->isDead;
    goblinBomb->entity.life = goblinBomb->life;
    goblinBomb->entity.maxLife = goblinBomb->maxLife;
    goblinBomb->entity.position.x = goblinBomb->base.position.x;
    goblinBomb->entity.position.y = goblinBomb->base.position.y;

    DrawBar(&goblinBomb->entity, 120, -90);

    Rectangle source;
    Rectangle dest;

    DrawMonsters(&goblinBomb->base, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO);

    if (goblinBomb->isAttackBomb)
    {
        source = (Rectangle){goblinBomb->base.currentFrame * goblinBomb->frameWidthAttackBomb, ROW_BASE, goblinBomb->frameWidthAttackBomb * goblinBomb->base.direction, goblinBomb->frameHeightAttackBomb};
        dest = (Rectangle){goblinBomb->base.position.x, goblinBomb->base.position.y, goblinBomb->frameWidthAttackBomb / goblinBomb->base.scale, goblinBomb->frameHeightAttackBomb / goblinBomb->base.scale};

        DrawTexturePro(goblinBomb->spriteAttackBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }

    if (goblinBomb->bomb.isActive)
    {
        source = (Rectangle){goblinBomb->bomb.currentFrameBomb * goblinBomb->bomb.frameWidthBomb, ROW_BASE, goblinBomb->bomb.frameWidthBomb * goblinBomb->base.direction, goblinBomb->bomb.frameHeightBomb};
        dest = (Rectangle){goblinBomb->bomb.pos.x, goblinBomb->bomb.pos.y + BOMB_CENTER_OFFSET_Y, goblinBomb->bomb.frameWidthBomb / goblinBomb->base.scale, goblinBomb->bomb.frameHeightBomb / goblinBomb->base.scale};

        DrawTexturePro(goblinBomb->spriteBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
}

void UnloadGoblinBomb(GoblinBomb *goblinBomb)
{
    UnloadTexture(goblinBomb->base.spriteAtack);
    UnloadTexture(goblinBomb->spriteAttackBomb);
    UnloadTexture(goblinBomb->spriteBomb);
    UnloadTexture(goblinBomb->base.spriteIdle);
    UnloadTexture(goblinBomb->base.spriteDead);
    UnloadTexture(goblinBomb->base.spriteHurt);
    UnloadTexture(goblinBomb->base.spriteWalk);

    UnloadSound(goblinBomb->soundAttack);
    UnloadSound(goblinBomb->bomb.explosion);
}
