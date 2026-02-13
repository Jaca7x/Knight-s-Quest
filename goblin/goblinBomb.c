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
    /* Textures */
    goblinBomb->base.spriteIdle    = LoadTexture("resources/sprites/goblinBomb/Idle.png");
    goblinBomb->base.spriteAtack   = LoadTexture("resources/sprites/goblinBomb/Attack1.png");
    goblinBomb->spriteAttackBomb   = LoadTexture("resources/sprites/goblinBomb/Attack3.png");
    goblinBomb->base.spriteHurt    = LoadTexture("resources/sprites/goblinBomb/Hurt.png");
    goblinBomb->base.spriteDead    = LoadTexture("resources/sprites/goblinBomb/Death.png");
    goblinBomb->base.spriteWalk    = LoadTexture("resources/sprites/goblinBomb/Run.png");
    goblinBomb->spriteBomb         = LoadTexture("resources/sprites/goblinBomb/Bomb_sprite.png");

    /* Position */
    goblinBomb->base.position = (Vector2){800, 440};
    goblinBomb->bomb.pos      = goblinBomb->base.position;
    goblinBomb->base.direction = 1;

    /* Stats */
    goblinBomb->maxLife = 70.0f;
    goblinBomb->life    = goblinBomb->maxLife;
    goblinBomb->speed   = 30.0f;
    goblinBomb->damage  = 20;
    goblinBomb->damageBomb = 50;
    goblinBomb->droppedHeart = false;

    /* Animation frame count */
    goblinBomb->frameIdle       = 4;
    goblinBomb->frameRun        = 8;
    goblinBomb->frameAttack     = 8;
    goblinBomb->frameAttackBomb = 12;
    goblinBomb->frameHurt       = 4;
    goblinBomb->frameDeath      = 4;

    goblinBomb->base.scale      = 0.5f;
    goblinBomb->base.scaleIdle  = 0.5f;

    /* Frame size calculation */
    goblinBomb->base.frameWidthIdle  = goblinBomb->base.spriteIdle.width / goblinBomb->frameIdle;
    goblinBomb->base.frameHeightIdle = goblinBomb->base.spriteIdle.height;

    goblinBomb->base.frameWidthWalk  = goblinBomb->base.spriteWalk.width / goblinBomb->frameRun;
    goblinBomb->base.frameHeightWalk = goblinBomb->base.spriteWalk.height;

    goblinBomb->base.frameWidthAtack  = goblinBomb->base.spriteAtack.width / goblinBomb->frameAttack;
    goblinBomb->base.frameHeightAtack = goblinBomb->base.spriteAtack.height;

    goblinBomb->frameWidthAttackBomb  = goblinBomb->spriteAttackBomb.width / goblinBomb->frameAttackBomb;
    goblinBomb->frameHeightAttackBomb = goblinBomb->spriteAttackBomb.height;

    goblinBomb->base.frameWidthHurt  = goblinBomb->base.spriteHurt.width / goblinBomb->frameHurt;
    goblinBomb->base.frameHeightHurt = goblinBomb->base.spriteHurt.height;

    goblinBomb->base.frameWidthDead  = goblinBomb->base.spriteDead.width / goblinBomb->frameDeath;
    goblinBomb->base.frameHeightDead = goblinBomb->base.spriteDead.height;

    goblinBomb->bomb.frameBomb       = 19;
    goblinBomb->bomb.frameWidthBomb  = goblinBomb->spriteBomb.width / goblinBomb->bomb.frameBomb;
    goblinBomb->bomb.frameHeightBomb = goblinBomb->spriteBomb.height;

    goblinBomb->base.currentFrame = 0;
    goblinBomb->frameCounter      = 0;

    /* Combat / Range */
    goblinBomb->timerForExplosion = 0.0f;
    goblinBomb->radiusToDamage    = 50.0f;
    goblinBomb->bombRange         = 200.0f;
    goblinBomb->atackRange       = 35.0f;
    goblinBomb->push              = 50;
    goblinBomb->viewPlayer        = 150.0f;

    goblinBomb->timerAttackBomb   = 0.0f;
    goblinBomb->animAttackTimer   = 0.0f;
    goblinBomb->animAttackBombEnd = false;

    goblinBomb->hurtDuration = 2.0f;
    goblinBomb->hurtTimer    = 0.0f;

    /* Bomb system */
    goblinBomb->bomb.isActive        = false;
    goblinBomb->bombExplode          = false;
    goblinBomb->hasThrownBomb        = false;
    goblinBomb->bomb.currentFrameBomb = 0;
    goblinBomb->bomb.frameCounterBomb = 0;
    goblinBomb->bomb.speed            = 65.0f;
    goblinBomb->bomb.playerIsDamage   = false;

    /* States */
    goblinBomb->base.isIdle        = true;
    goblinBomb->base.isWalking     = false;
    goblinBomb->base.isAtacking    = false;
    goblinBomb->base.monsterHasHit = false;

    goblinBomb->isAttackBomb      = false;
    goblinBomb->isDead            = false;
    goblinBomb->wasWalking        = false;
    goblinBomb->attackSoundPlayed = false;

    /* Sounds */
    goblinBomb->bomb.explosion =
        LoadSound("resources/sounds/sound_effects/bomb/explosion.wav");

    goblinBomb->bomb.timer =
        LoadSound("resources/sounds/sound_effects/bomb/timer.wav");

    goblinBomb->soundAttack =
        LoadSound("resources/sounds/sound_effects/wolf/red-wolf-scratch.wav");

    goblinBomb->soundBagGoblin =
        LoadSound("resources/sounds/sound_effects/goblin/bag.wav");
}

void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player)
{
    if (goblinBomb->base.isWalking)
    {
        goblinBomb->base.position.x += goblinBomb->speed * goblinBomb->base.direction * delta;
    }
    
    goblinBomb->frameCounter++;
    goblinBomb->bomb.frameCounterBomb++;

    if (goblinBomb->life <= LIFE_ZERO && !goblinBomb->isDead)
    {
        goblinBomb->isDead = true;
        goblinBomb->base.isIdle = false;
        goblinBomb->isAttackBomb = false;
        goblinBomb->base.isWalking = false;
        goblinBomb->base.isAtacking = false;
        goblinBomb->base.currentFrame = CURRENT_FRAME_ZERO;
        
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
            goblinBomb->hurtTimer = HURT_TIMER_ZERO;
            goblinBomb->base.isIdle = true;
        }
    }

    if (goblinBomb->isDead)
    {
        if (goblinBomb->frameCounter >= FRAME_DELAY_GOBLIN_BOMB)
        {
            goblinBomb->frameCounter = FRAME_COUNTER_ZERO;
            goblinBomb->base.currentFrame++;

            if (goblinBomb->base.currentFrame >= goblinBomb->frameDeath)
                goblinBomb->base.currentFrame = goblinBomb->frameDeath - PREVIOUS_FRAME;
        }

        return;
    }
    else if (goblinBomb->frameCounter >= FRAME_DELAY_GOBLIN_BOMB)
    {
        goblinBomb->frameCounter = FRAME_COUNTER_ZERO;

        if (goblinBomb->base.isIdle)
        {
            goblinBomb->base.currentFrame++;
            if (goblinBomb->base.currentFrame >= goblinBomb->frameIdle)
            {
                goblinBomb->base.currentFrame = FRAME_COUNTER_ZERO;
            }
        }
        else if (goblinBomb->isAttackBomb)
        {
            goblinBomb->base.currentFrame++;

            if (goblinBomb->base.currentFrame == FRAME_TO_SOUND_BAG && !goblinBomb->soundBagPlayed)
            {
                PlaySound(goblinBomb->soundBagGoblin);
                goblinBomb->soundBagPlayed = true;
            }
            
            if (goblinBomb->base.currentFrame >= goblinBomb->frameAttackBomb)
            {
                goblinBomb->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
        else if (goblinBomb->base.isAtacking)
        {
            goblinBomb->base.currentFrame++;
            
            if (goblinBomb->base.currentFrame == FRAME_TO_SOUND_ATTACK_GOBLIN_BOMB && !goblinBomb->attackSoundPlayed)
            {
                PlaySound(goblinBomb->soundAttack);
                goblinBomb->attackSoundPlayed = true;
            }
            
            if (goblinBomb->base.currentFrame >= goblinBomb->frameAttack)
            {
                goblinBomb->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
        else if (goblinBomb->base.monsterHasHit)
        {
            goblinBomb->base.currentFrame++;
            if (goblinBomb->base.currentFrame >= goblinBomb->frameHurt)
            {
                goblinBomb->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
        else if (goblinBomb->base.isWalking)
        {
            goblinBomb->base.currentFrame++;
        
            if (goblinBomb->base.currentFrame >= goblinBomb->frameRun)
            {
                goblinBomb->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
    }

    if (goblinBomb->bomb.frameCounterBomb >= FRAME_DELAY_GOBLIN_BOMB)
    {
        goblinBomb->bomb.frameCounterBomb = FRAME_COUNTER_BOMB_ZERO;

        if (goblinBomb->bomb.isActive)
        {
            goblinBomb->bomb.currentFrameBomb++;

            if (goblinBomb->bomb.currentFrameBomb == FRAME_TO_SOUND_EXPLOSION)
            {
                PlaySound(goblinBomb->bomb.explosion);
            }
            
            if (goblinBomb->bomb.currentFrameBomb >= goblinBomb->bomb.frameBomb)
            {
                goblinBomb->bomb.currentFrameBomb = FRAME_CURRENT_BOMB_ZERO;
                goblinBomb->bomb.frameCounterBomb = FRAME_COUNTER_BOMB_ZERO;
            }
        }
    }

    float disatanceToAttack = fabs(player->position.x - goblinBomb->base.position.x);

    goblinBomb->base.direction = (player->position.x > goblinBomb->base.position.x) ? DIRECTION_RIGHT: DIRECTION_LEFT;

    if (disatanceToAttack <= goblinBomb->viewPlayer && goblinBomb->hasThrownBomb)
    {
        if (disatanceToAttack > goblinBomb->atackRange)
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
            if (goblinBomb->atackRange > disatanceToAttack)
            {
                goblinBomb->base.isAtacking = true;
                goblinBomb->isAttackBomb = false;
                goblinBomb->base.isIdle = false;

                if (!wasAttacking)
                {
                    PlaySound(goblinBomb->soundAttack);
                    goblinBomb->base.currentFrame = CURRENT_FRAME_ZERO; 
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
                goblinBomb->timerAttackBomb = ATTACK_BOMB_TIMER_ZERO;
            }
        }
    }
    else
    {
        if (goblinBomb->atackRange > disatanceToAttack && goblinBomb->base.direction == DIRECTION_LEFT)
        {
            goblinBomb->base.isAtacking = true;
            goblinBomb->base.isIdle = false;
            goblinBomb->base.isWalking = false;
        }
        else if (goblinBomb->atackRangeRight > disatanceToAttack && goblinBomb->base.direction == DIRECTION_RIGHT)
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
            goblinBomb->base.position.x + GOBLIN_BOMB_HITBOX_OFFSET_X,
            goblinBomb->base.position.y + GOBLIN_BOMB_HITBOX_OFFSET_Y,
            goblinBomb->base.frameWidthAtack / GOBLIN_BOMB_HITBOX_SCALE,
            goblinBomb->base.frameHeightAtack / GOBLIN_BOMB_HITBOX_SCALE};

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
            && goblinBomb->bomb.currentFrameBomb == FRAME_TO_DAMAGE_EXPLOSION)
            {
                player->life -= goblinBomb->damageBomb;
                goblinBomb->bomb.playerIsDamage = true;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;
            }
        }
        else
        {
            goblinBomb->bombExplode = true;
            goblinBomb->bomb.isActive = false;
            goblinBomb->hasThrownBomb = true;

            goblinBomb->base.isIdle = true;
            goblinBomb->isAttackBomb = false;
            goblinBomb->timerForExplosion = ATTACK_BOMB_TIMER_ZERO;
            goblinBomb->bomb.playerIsDamage = false;
        }
    }

    goblinBomb->animAttackTimer -= delta;

    if (goblinBomb->base.isAtacking && goblinBomb->base.currentFrame == FRAME_TO_ATTACK_GOBLIN_BOMB && goblinBomb->animAttackTimer <= ANIM_ATTACK_ZERO)
    {
        if (CheckCollisionRecs(playerRec, goblinRec))
        {
            player->life -= goblinBomb->damage;
            player->hasHit = true;
            player->hitTimer = HIT_TIMER;

            goblinBomb->animAttackTimer = ANIM_ATTACK;
        }
    }

    if (!goblinBomb->isDead && CheckColisionGoblinBomb(
        player->position.x + PLAYER_HITBOX_OFFSET_X, 
        player->position.y + PLAYER_HITBOX_OFFSET_Y, 
        player->frameWidth / PLAYER_HITBOX_WIDTH_DIV, 
        player->frameHeight,

        goblinBomb->base.position.x + GOBLIN_BOMB_HITBOX_OFFSET_X, 
        goblinBomb->base.position.y + GOBLIN_BOMB_HITBOX_OFFSET_Y, 
        goblinBomb->base.frameWidthHurt / GOBLIN_BOMB_SCALE_WIDTH, 
        goblinBomb->base.frameHeightHurt / GOBLIN_BOMB_SCALE_HEIGHT
        ))
    {
        player->position.x += (player->position.x < goblinBomb->base.position.x) ? -goblinBomb->push : goblinBomb->push;
    }
}

void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player)
{
    goblinBomb->entity.isDead = goblinBomb->isDead;
    goblinBomb->entity.life = goblinBomb->life;
    goblinBomb->entity.maxLife = goblinBomb->maxLife;
    goblinBomb->entity.position.x = goblinBomb->base.position.x;
    goblinBomb->entity.position.y = goblinBomb->base.position.y;

    DrawBar(&goblinBomb->entity, OFFSET_BAR_LIFE_X_GOBLIN_BOMB, OFFSET_BAR_LIFE_Y_GOBLIN_BOMB);

    Rectangle source;
    Rectangle dest;

    DrawMonsters(&goblinBomb->base, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO);

    if (goblinBomb->isAttackBomb)
    {
        source = (Rectangle){goblinBomb->base.currentFrame * goblinBomb->frameWidthAttackBomb, SPRITE_ROW_BASE, goblinBomb->frameWidthAttackBomb * goblinBomb->base.direction, goblinBomb->frameHeightAttackBomb};
        dest = (Rectangle){goblinBomb->base.position.x, goblinBomb->base.position.y, goblinBomb->frameWidthAttackBomb / goblinBomb->base.scale, goblinBomb->frameHeightAttackBomb / goblinBomb->base.scale};

        DrawTexturePro(goblinBomb->spriteAttackBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }

    if (goblinBomb->bomb.isActive)
    {
        source = (Rectangle){goblinBomb->bomb.currentFrameBomb * goblinBomb->bomb.frameWidthBomb, SPRITE_ROW_BASE, goblinBomb->bomb.frameWidthBomb * goblinBomb->base.direction, goblinBomb->bomb.frameHeightBomb};
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
