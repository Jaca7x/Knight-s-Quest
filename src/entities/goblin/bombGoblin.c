#include "bombGoblin.h"

bool CheckColisionBombGoblin(float x1, float y1, float w1, float h1,
                             float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void InitBombGoblin(BombGoblin *bombGoblin)
{
    // Textures
    bombGoblin->base.spriteIdle = LoadTexture("resources/sprites/bombGoblin/Idle.png");
    bombGoblin->base.spriteAtack = LoadTexture("resources/sprites/bombGoblin/Attack1.png");
    bombGoblin->spriteAttackBomb = LoadTexture("resources/sprites/bombGoblin/Attack3.png");
    bombGoblin->base.spriteHurt = LoadTexture("resources/sprites/bombGoblin/Hurt.png");
    bombGoblin->base.spriteDead = LoadTexture("resources/sprites/bombGoblin/Death.png");
    bombGoblin->base.spriteWalk = LoadTexture("resources/sprites/bombGoblin/Run.png");
    bombGoblin->spriteBomb = LoadTexture("resources/sprites/bombGoblin/Bomb_sprite.png");

    // Position
    bombGoblin->base.position = (Vector2){800, 440};
    bombGoblin->bomb.pos = bombGoblin->base.position;
    bombGoblin->base.direction = 1;

    // Stats
    bombGoblin->maxLife = 70.0f;
    bombGoblin->life = bombGoblin->maxLife;
    bombGoblin->speed = 30.0f;
    bombGoblin->damage  = 20;
    bombGoblin->damageBomb = 50;
    bombGoblin->droppedHeart = false;

    //Animation frame count
    bombGoblin->frameIdle = 4;
    bombGoblin->frameRun = 8;
    bombGoblin->frameAttack = 8;
    bombGoblin->frameAttackBomb = 12;
    bombGoblin->frameHurt = 4;
    bombGoblin->frameDeath = 4;

    bombGoblin->base.scale = 0.5f;
    bombGoblin->base.scaleIdle = 0.5f;

    // Frame size calculation 
    bombGoblin->base.frameWidthIdle  = bombGoblin->base.spriteIdle.width / bombGoblin->frameIdle;
    bombGoblin->base.frameHeightIdle = bombGoblin->base.spriteIdle.height;

    bombGoblin->base.frameWidthWalk  = bombGoblin->base.spriteWalk.width / bombGoblin->frameRun;
    bombGoblin->base.frameHeightWalk = bombGoblin->base.spriteWalk.height;

    bombGoblin->base.frameWidthAtack  = bombGoblin->base.spriteAtack.width / bombGoblin->frameAttack;
    bombGoblin->base.frameHeightAtack = bombGoblin->base.spriteAtack.height;

    bombGoblin->frameWidthAttackBomb  = bombGoblin->spriteAttackBomb.width / bombGoblin->frameAttackBomb;
    bombGoblin->frameHeightAttackBomb = bombGoblin->spriteAttackBomb.height;

    bombGoblin->base.frameWidthHurt  = bombGoblin->base.spriteHurt.width / bombGoblin->frameHurt;
    bombGoblin->base.frameHeightHurt = bombGoblin->base.spriteHurt.height;

    bombGoblin->base.frameWidthDead  = bombGoblin->base.spriteDead.width / bombGoblin->frameDeath;
    bombGoblin->base.frameHeightDead = bombGoblin->base.spriteDead.height;

    bombGoblin->bomb.frameBomb = 19;
    bombGoblin->bomb.frameWidthBomb  = bombGoblin->spriteBomb.width / bombGoblin->bomb.frameBomb;
    bombGoblin->bomb.frameHeightBomb = bombGoblin->spriteBomb.height;

    bombGoblin->base.currentFrame = 0;
    bombGoblin->frameCounter = 0;

    // Combat / Range 
    bombGoblin->timerForExplosion = 0.0f;
    bombGoblin->radiusToDamage = 50.0f;
    bombGoblin->bombRange = 200.0f;
    bombGoblin->atackRange = 35.0f;
    bombGoblin->push = 50;
    bombGoblin->viewPlayer = 150.0f;

    bombGoblin->timerAttackBomb = 0.0f;
    bombGoblin->animAttackTimer = 0.0f;
    bombGoblin->animAttackBombEnd = false;

    bombGoblin->hurtDuration = 2.0f;
    bombGoblin->hurtTimer = 0.0f;

    // Bomb system 
    bombGoblin->bomb.isActive = false;
    bombGoblin->bombExplode = false;
    bombGoblin->hasThrownBomb = false;
    bombGoblin->bomb.currentFrameBomb = 0;
    bombGoblin->bomb.frameCounterBomb = 0;
    bombGoblin->bomb.speed = 65.0f;
    bombGoblin->bomb.playerIsDamage = false;

    // States
    bombGoblin->base.isIdle = true;
    bombGoblin->base.isDead = false;
    bombGoblin->base.isWalking = false;
    bombGoblin->base.isAtacking = false;
    bombGoblin->base.monsterHasHit = false;

    bombGoblin->isAttackBomb = false;
    bombGoblin->wasWalking = false;
    bombGoblin->attackSoundPlayed = false;

    // Sounds 
    bombGoblin->bomb.explosion =
        LoadSound("resources/sounds/sound_effects/bomb/explosion.wav");

    bombGoblin->bomb.timer =
        LoadSound("resources/sounds/sound_effects/bomb/timer.wav");

    bombGoblin->soundAttack =
        LoadSound("resources/sounds/sound_effects/wolf/red-wolf-scratch.wav");

    bombGoblin->soundBagGoblin =
        LoadSound("resources/sounds/sound_effects/goblin/bag.wav");
}

void UpdateBombGoblin(BombGoblin *bombGoblin, float delta, Player *player)
{
    if (bombGoblin->base.isWalking)
    {
        bombGoblin->base.position.x += bombGoblin->speed * bombGoblin->base.direction * delta;
    }
    
    bombGoblin->frameCounter++;
    bombGoblin->bomb.frameCounterBomb++;

    if (bombGoblin->life <= LIFE_ZERO && !bombGoblin->base.isDead)
    {
        bombGoblin->base.isDead = true;
        bombGoblin->base.isIdle = false;
        bombGoblin->isAttackBomb = false;
        bombGoblin->base.isWalking = false;
        bombGoblin->base.isAtacking = false;
        bombGoblin->base.currentFrame = CURRENT_FRAME_ZERO;
        bombGoblin->base.monsterHasHit = false;
        StopSound(bombGoblin->bomb.timer);
        
        bombGoblin->bomb.isActive = false;
        
        return;
    }

    if (bombGoblin->base.monsterHasHit && !bombGoblin->base.isDead)
    {
        bombGoblin->base.isIdle = false;
        bombGoblin->isAttackBomb = false;
        bombGoblin->base.isAtacking = false;
        bombGoblin->base.isWalking = false;

        bombGoblin->hurtTimer += delta;

        if (bombGoblin->hurtTimer >= bombGoblin->hurtDuration)
        {
            bombGoblin->base.monsterHasHit = false;
            bombGoblin->hurtTimer = HURT_TIMER_ZERO;
            bombGoblin->base.isIdle = true;
        }
    }

    if (bombGoblin->base.isDead)
    {
        if (bombGoblin->frameCounter >= BOMB_GOBLIN_FRAME_DELAY)
        {
            bombGoblin->frameCounter = FRAME_COUNTER_ZERO;
            bombGoblin->base.currentFrame++;

            if (bombGoblin->base.currentFrame >= bombGoblin->frameDeath)
                bombGoblin->base.currentFrame = bombGoblin->frameDeath - PREVIOUS_FRAME;
        }

        return;
    }
    else if (bombGoblin->frameCounter >= BOMB_GOBLIN_FRAME_DELAY)
    {
        bombGoblin->frameCounter = FRAME_COUNTER_ZERO;

        if (bombGoblin->base.isIdle)
        {
            bombGoblin->base.currentFrame++;
            if (bombGoblin->base.currentFrame >= bombGoblin->frameIdle)
            {
                bombGoblin->base.currentFrame = FRAME_COUNTER_ZERO;
            }
        }
        else if (bombGoblin->isAttackBomb)
        {
            bombGoblin->base.currentFrame++;

            if (bombGoblin->base.currentFrame == FRAME_TO_SOUND_BAG && !bombGoblin->soundBagPlayed)
            {
                PlaySound(bombGoblin->soundBagGoblin);
                bombGoblin->soundBagPlayed = true;
            }
            
            if (bombGoblin->base.currentFrame >= bombGoblin->frameAttackBomb)
            {
                bombGoblin->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
        else if (bombGoblin->base.isAtacking)
        {
            bombGoblin->base.currentFrame++;
            
            if (bombGoblin->base.currentFrame == FRAME_TO_SOUND_ATTACK_BOMB_GOBLIN 
                && !bombGoblin->attackSoundPlayed)
            {
                PlaySound(bombGoblin->soundAttack);
                bombGoblin->attackSoundPlayed = true;
            }
            
            if (bombGoblin->base.currentFrame >= bombGoblin->frameAttack)
            {
                bombGoblin->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
        else if (bombGoblin->base.monsterHasHit)
        {
            bombGoblin->base.currentFrame++;

            if (bombGoblin->base.currentFrame >= bombGoblin->frameHurt)
            {
                bombGoblin->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
        else if (bombGoblin->base.isWalking)
        {
            bombGoblin->base.currentFrame++;
        
            if (bombGoblin->base.currentFrame >= bombGoblin->frameRun)
            {
                bombGoblin->base.currentFrame = CURRENT_FRAME_ZERO;
            }
        }
    }

    if (bombGoblin->bomb.frameCounterBomb >= BOMB_GOBLIN_FRAME_DELAY)
    {
        bombGoblin->bomb.frameCounterBomb = FRAME_COUNTER_BOMB_ZERO;

        if (bombGoblin->bomb.isActive)
        {
            bombGoblin->bomb.currentFrameBomb++;

            if (bombGoblin->bomb.currentFrameBomb == FRAME_TO_SOUND_EXPLOSION)
            {
                PlaySound(bombGoblin->bomb.explosion);
            }
            
            if (bombGoblin->bomb.currentFrameBomb >= bombGoblin->bomb.frameBomb)
            {
                bombGoblin->bomb.currentFrameBomb = FRAME_CURRENT_BOMB_ZERO;
                bombGoblin->bomb.frameCounterBomb = FRAME_COUNTER_BOMB_ZERO;
            }
        }
    }

    float disatanceToAttack = fabs(player->position.x - bombGoblin->base.position.x);

    bombGoblin->base.direction = (player->position.x > bombGoblin->base.position.x) ? DIRECTION_RIGHT: DIRECTION_LEFT;

    if (disatanceToAttack <= bombGoblin->viewPlayer && bombGoblin->hasThrownBomb)
    {
        if (disatanceToAttack > bombGoblin->atackRange)
        {
            bombGoblin->base.isWalking = true;
            bombGoblin->base.isIdle = false;
            bombGoblin->base.isAtacking = false;
        }
        else
        {  
            bombGoblin->attackSoundPlayed = false;
            bombGoblin->base.isWalking = false;
            bombGoblin->base.isAtacking = true;
            bombGoblin->base.isIdle = false;
        }
    }
    else
    {
        bombGoblin->base.isWalking = false;
        bombGoblin->base.isAtacking = false;
        bombGoblin->base.isIdle = true;
    }

    static bool wasAttacking = false;

    if (!bombGoblin->hasThrownBomb)
    {
        if (bombGoblin->bombRange > disatanceToAttack)
        {
            if (bombGoblin->atackRange > disatanceToAttack)
            {
                bombGoblin->base.isAtacking = true;
                bombGoblin->isAttackBomb = false;
                bombGoblin->base.isIdle = false;

                if (!wasAttacking)
                {
                    PlaySound(bombGoblin->soundAttack);
                    bombGoblin->base.currentFrame = CURRENT_FRAME_ZERO; 
                }
            }
            else
            {
                bombGoblin->isAttackBomb = true;
                bombGoblin->soundBagPlayed = false;
                bombGoblin->base.isAtacking = false;
                bombGoblin->base.isIdle = false;
            }
        }
        else
        {
            bombGoblin->base.isIdle = true;
            bombGoblin->base.isAtacking = false;
            bombGoblin->isAttackBomb = false;
        }

        if (bombGoblin->isAttackBomb)
        {
            bombGoblin->timerAttackBomb += delta;

            if (bombGoblin->timerAttackBomb >= ATTACK_BOMB_TIMER)
            {
                bombGoblin->isAttackBomb = false;
                bombGoblin->base.isIdle = true;

                bombGoblin->bomb.isActive = true;
                PlaySound(bombGoblin->bomb.timer);
                bombGoblin->timerAttackBomb = ATTACK_BOMB_TIMER_ZERO;
            }
        }
    }
    else
    {
        if (bombGoblin->atackRange > disatanceToAttack && bombGoblin->base.direction == DIRECTION_LEFT)
        {
            bombGoblin->base.isAtacking = true;
            bombGoblin->base.isIdle = false;
            bombGoblin->base.isWalking = false;
        }
        else if (bombGoblin->atackRangeRight > disatanceToAttack && bombGoblin->base.direction == DIRECTION_RIGHT)
        {
            bombGoblin->base.isAtacking = true;
            bombGoblin->base.isIdle = false;
            bombGoblin->base.isWalking = false;
        }
        bombGoblin->isAttackBomb = false;
    }

    wasAttacking = bombGoblin->base.isAtacking;

    Rectangle playerRec =
        {
            player->position.x + PLAYER_HITBOX_OFFSET_X,
            player->position.y + PLAYER_HITBOX_OFFSET_Y,
            player->frameWidth,
            player->frameHeight
        };

    Rectangle goblinRec =
        {
            bombGoblin->base.position.x + BOMB_GOBLIN_HITBOX_OFFSET_X,
            bombGoblin->base.position.y + BOMB_GOBLIN_HITBOX_OFFSET_Y,
            bombGoblin->base.frameWidthAtack / BOMB_GOBLIN_HITBOX_SCALE,
            bombGoblin->base.frameHeightAtack / BOMB_GOBLIN_HITBOX_SCALE
        };

    Vector2 centerCircle =
        {
            bombGoblin->bomb.pos.x +
            bombGoblin->bomb.frameWidthBomb / BOMB_CENTER_SCALE +
            BOMB_CENTER_OFFSET_X,

            bombGoblin->bomb.pos.y +
            bombGoblin->bomb.frameHeightBomb / BOMB_CENTER_SCALE +
            BOMB_CENTER_OFFSET_Y
        };

    if (bombGoblin->bomb.isActive)
    {
        bombGoblin->timerForExplosion += delta;

        if (bombGoblin->timerForExplosion < BOMB_EXPLOSION_DELAY)
        {
            bombGoblin->bomb.pos.x += bombGoblin->bomb.speed * bombGoblin->base.direction * delta;

            if (!bombGoblin->bomb.playerIsDamage 
                && CheckCollisionCircleRec(centerCircle, bombGoblin->radiusToDamage, playerRec) 
                && bombGoblin->bomb.currentFrameBomb == FRAME_TO_DAMAGE_EXPLOSION)
            {
                player->life -= bombGoblin->damageBomb;
                bombGoblin->bomb.playerIsDamage = true;
                player->hasHit = true;
                player->hitTimer = HIT_TIMER;
            }
        }
        else
        {
            bombGoblin->bombExplode = true;
            bombGoblin->bomb.isActive = false;
            bombGoblin->hasThrownBomb = true;

            bombGoblin->base.isIdle = true;
            bombGoblin->isAttackBomb = false;
            bombGoblin->timerForExplosion = ATTACK_BOMB_TIMER_ZERO;
            bombGoblin->bomb.playerIsDamage = false;
        }
    }

    bombGoblin->animAttackTimer -= delta;

    if (bombGoblin->base.isAtacking 
        && bombGoblin->base.currentFrame == FRAME_TO_ATTACK_BOMB_GOBLIN 
        && bombGoblin->animAttackTimer <= ANIM_ATTACK_ZERO)
    {
        if (CheckCollisionRecs(playerRec, goblinRec))
        {
            player->life -= bombGoblin->damage;
            player->hasHit = true;
            player->hitTimer = HIT_TIMER;

            bombGoblin->animAttackTimer = ANIM_ATTACK;
        }
    }

    if (!bombGoblin->base.isDead && CheckColisionBombGoblin(
        player->position.x + PLAYER_HITBOX_OFFSET_X, 
        player->position.y + PLAYER_HITBOX_OFFSET_Y, 
        player->frameWidth / PLAYER_HITBOX_WIDTH_DIV, 
        player->frameHeight,

        bombGoblin->base.position.x + BOMB_GOBLIN_HITBOX_OFFSET_X, 
        bombGoblin->base.position.y + BOMB_GOBLIN_HITBOX_OFFSET_Y, 
        bombGoblin->base.frameWidthHurt / BOMB_GOBLIN_SCALE_WIDTH, 
        bombGoblin->base.frameHeightHurt / BOMB_GOBLIN_SCALE_HEIGHT
        ))
    {
        player->position.x += (player->position.x < bombGoblin->base.position.x) ? -bombGoblin->push : bombGoblin->push;
    }
}

void DrawBombGoblin(BombGoblin *bombGoblin, Player *player)
{
    bombGoblin->entity.isDead = bombGoblin->base.isDead;
    bombGoblin->entity.life = bombGoblin->life;
    bombGoblin->entity.maxLife = bombGoblin->maxLife;
    bombGoblin->entity.position.x = bombGoblin->base.position.x;
    bombGoblin->entity.position.y = bombGoblin->base.position.y;

    DrawBar(&bombGoblin->entity, BOMB_GOBLIN_OFFSET_X_LIFE_BAR, BOMB_GOBLIN_OFFSET_Y_LIFE_BAR);

    Rectangle source;
    Rectangle dest;
    Vector2 origin = (Vector2){0, 0};
    float rotation = 0.0f;

    DrawMonsters(&bombGoblin->base, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO, OFFSET_ZERO);

    if (bombGoblin->isAttackBomb)
    {
        source = (Rectangle) 
        {
            bombGoblin->base.currentFrame * bombGoblin->frameWidthAttackBomb, 
            SPRITE_ROW_BASE,
            bombGoblin->frameWidthAttackBomb * bombGoblin->base.direction, 
            bombGoblin->frameHeightAttackBomb
        };

        dest = (Rectangle)
        {
            bombGoblin->base.position.x, 
            bombGoblin->base.position.y, 
            bombGoblin->frameWidthAttackBomb / bombGoblin->base.scale, 
            bombGoblin->frameHeightAttackBomb / bombGoblin->base.scale
        };

        DrawTexturePro(bombGoblin->spriteAttackBomb, source, dest, origin, rotation, RAYWHITE);
    }

    if (bombGoblin->bomb.isActive)
    {
        source = (Rectangle)
        {
            bombGoblin->bomb.currentFrameBomb * bombGoblin->bomb.frameWidthBomb, 
            SPRITE_ROW_BASE, 
            bombGoblin->bomb.frameWidthBomb * bombGoblin->base.direction, 
            bombGoblin->bomb.frameHeightBomb
        };

        dest = (Rectangle)
        {
            bombGoblin->bomb.pos.x,
            bombGoblin->bomb.pos.y + BOMB_CENTER_OFFSET_Y,
            bombGoblin->bomb.frameWidthBomb / bombGoblin->base.scale, 
            bombGoblin->bomb.frameHeightBomb / bombGoblin->base.scale
        };

        DrawTexturePro(bombGoblin->spriteBomb, source, dest, origin, rotation, RAYWHITE);
    }
}

void UnloadBombGoblin(BombGoblin *bombGoblin)
{
    UnloadTexture(bombGoblin->base.spriteAtack);
    UnloadTexture(bombGoblin->spriteAttackBomb);
    UnloadTexture(bombGoblin->spriteBomb);
    UnloadTexture(bombGoblin->base.spriteIdle);
    UnloadTexture(bombGoblin->base.spriteDead);
    UnloadTexture(bombGoblin->base.spriteHurt);
    UnloadTexture(bombGoblin->base.spriteWalk);

    UnloadSound(bombGoblin->soundAttack);
    UnloadSound(bombGoblin->bomb.explosion);
}
