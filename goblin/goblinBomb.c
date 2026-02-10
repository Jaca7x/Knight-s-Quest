#include "goblinBomb.h"

bool CheckColisionGoblinBomb(float x1, float y1, float w1, float h1,
                             float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void DrawGoblinBombLifeBar(GoblinBomb *goblinBomb)
{
    if(goblinBomb->isDead) 
    return;

    float barWidth = 60.0f;
    float barHeight = 8.0f;
    float x = goblinBomb->position.x + 120;
    float y = goblinBomb->position.y + 80;

    float lifePercent = goblinBomb->life / goblinBomb->maxLife;

    if (lifePercent < 0) lifePercent = 0;
    if (lifePercent > 1) lifePercent = 1;

    float currentBarWidth = barWidth * lifePercent;

    DrawRectangle(x, y, barWidth, barHeight, RED);

    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN);

    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
}

void InitGoblinBomb(GoblinBomb *goblinBomb)
{
    goblinBomb->spriteIdle = LoadTexture("resources/sprites/goblinBomb/Idle.png");
    goblinBomb->spriteAttackBomb = LoadTexture("resources/sprites/goblinBomb/Attack3.png");
    goblinBomb->spriteAttack = LoadTexture("resources/sprites/goblinBomb/Attack1.png");
    goblinBomb->spriteHurt = LoadTexture("resources/sprites/goblinBomb/Hurt.png");
    goblinBomb->spriteDeath = LoadTexture("resources/sprites/goblinBomb/Death.png");
    goblinBomb->spriteRun = LoadTexture("resources/sprites/goblinBomb/Run.png");

    goblinBomb->position = (Vector2){800, 440};
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
    goblinBomb->frameScale = 2.0f;

    goblinBomb->frameWidthIdle = goblinBomb->spriteIdle.width / goblinBomb->frameIdle;
    goblinBomb->frameHeightIdle = goblinBomb->spriteIdle.height;

    goblinBomb->frameWidthAttackBomb = goblinBomb->spriteAttackBomb.width / goblinBomb->frameAttackBomb;
    goblinBomb->frameHeightAttackBomb = goblinBomb->spriteAttackBomb.height;

    goblinBomb->frameWidthAttack = goblinBomb->spriteAttack.width / goblinBomb->frameAttack;
    goblinBomb->frameHeightAttack = goblinBomb->spriteAttack.height;

    goblinBomb->frameWidthHurt = goblinBomb->spriteHurt.width / goblinBomb->frameHurt;
    goblinBomb->frameHeightHurt = goblinBomb->spriteHurt.height;

    goblinBomb->frameWidthDeath = goblinBomb->spriteDeath.width / goblinBomb->frameDeath;
    goblinBomb->frameHeightDeath = goblinBomb->spriteDeath.height;

    goblinBomb->frameWidthRun = goblinBomb->spriteRun.width / goblinBomb->frameRun;
    goblinBomb->frameHeightRun = goblinBomb->spriteRun.height;

    goblinBomb->currentFrame = 0;
    goblinBomb->frameCounter = 0;

    goblinBomb->isIdle = true;
    goblinBomb->isAttackBomb = false;
    goblinBomb->isWalking = false;

    goblinBomb->bomb.isActive = false;
    goblinBomb->bombExplode = false;
    goblinBomb->hasThrownBomb = false;
    goblinBomb->isAttack = false;

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

    goblinBomb->direction = 1;

    goblinBomb->animAttackTimer = 0.0f;

    goblinBomb->goblinHasHit = false;
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
    if (goblinBomb->isWalking)
    {
        goblinBomb->position.x += goblinBomb->speed * goblinBomb->direction * delta;
    }
    
    goblinBomb->frameCounter++;
    goblinBomb->bomb.frameCounterBomb++;

    if (goblinBomb->life <= 0 && !goblinBomb->isDead)
    {
        goblinBomb->isDead = true;
        goblinBomb->isIdle = false;
        goblinBomb->isAttackBomb = false;
        goblinBomb->isWalking = false;
        goblinBomb->isAttack = false;
        goblinBomb->currentFrame = 0;
        
        goblinBomb->bomb.isActive = false;
        
        return;
    }

    if (goblinBomb->goblinHasHit && !goblinBomb->isDead)
    {
        goblinBomb->isIdle = false;
        goblinBomb->isAttackBomb = false;
        goblinBomb->isAttack = false;
        goblinBomb->isWalking = false;

        goblinBomb->hurtTimer += delta;

        if (goblinBomb->hurtTimer >= goblinBomb->hurtDuration)
        {
            goblinBomb->goblinHasHit = false;
            goblinBomb->hurtTimer = 0.0f;
            goblinBomb->isIdle = true;
        }
    }

    if (goblinBomb->isDead)
    {
        if (goblinBomb->frameCounter >= 10)
        {
            goblinBomb->frameCounter = 0;
            goblinBomb->currentFrame++;

            if (goblinBomb->currentFrame >= goblinBomb->frameDeath)
                goblinBomb->currentFrame = goblinBomb->frameDeath - 1;
        }

        return;
    }
    else if (goblinBomb->frameCounter >= 10)
    {
        goblinBomb->frameCounter = 0;

        if (goblinBomb->isIdle)
        {
            goblinBomb->currentFrame++;
            if (goblinBomb->currentFrame >= goblinBomb->frameIdle)
            {
                goblinBomb->currentFrame = 0;
            }
        }
        else if (goblinBomb->isAttackBomb)
        {
            goblinBomb->currentFrame++;

            if (goblinBomb->currentFrame == 2 && !goblinBomb->soundBagPlayed)
            {
                PlaySound(goblinBomb->soundBagGoblin);
                goblinBomb->soundBagPlayed = true;
            }
            
            if (goblinBomb->currentFrame >= goblinBomb->frameAttackBomb)
            {
                goblinBomb->currentFrame = 0;
            }
        }
        else if (goblinBomb->isAttack)
        {
            goblinBomb->currentFrame++;
            
            if (goblinBomb->currentFrame == 6 && !goblinBomb->attackSoundPlayed)
            {
                PlaySound(goblinBomb->soundAttack);
                goblinBomb->attackSoundPlayed = true;
            }
            
            if (goblinBomb->currentFrame >= goblinBomb->frameAttack)
            {
                goblinBomb->currentFrame = 0;
            }
        }
        else if (goblinBomb->goblinHasHit)
        {
            goblinBomb->currentFrame++;
            if (goblinBomb->currentFrame >= goblinBomb->frameHurt)
            {
                goblinBomb->currentFrame = 0;
            }
        }
        else if (goblinBomb->isWalking)
        {
            goblinBomb->currentFrame++;
        
            if (goblinBomb->currentFrame >= goblinBomb->frameRun)
            {
                goblinBomb->currentFrame = 0;
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

    float disatanceToAttack = fabs(player->position.x - goblinBomb->position.x);

    goblinBomb->direction = (player->position.x > goblinBomb->position.x) ? 1 : -1;

    if (disatanceToAttack <= goblinBomb->viewPlayer && goblinBomb->hasThrownBomb)
    {
        if (disatanceToAttack > goblinBomb->attackRange)
        {
            goblinBomb->isWalking = true;
            goblinBomb->isIdle = false;
            goblinBomb->isAttack = false;
        }
        else
        {  
            goblinBomb->attackSoundPlayed = false;
            goblinBomb->isWalking = false;
            goblinBomb->isAttack = true;
            goblinBomb->isIdle = false;
        }
    }
    else
    {
        goblinBomb->isWalking = false;
        goblinBomb->isAttack = false;
        goblinBomb->isIdle = true;
    }

    static bool wasAttacking = false;

    if (!goblinBomb->hasThrownBomb)
    {
        if (goblinBomb->bombRange > disatanceToAttack)
        {
            if (goblinBomb->attackRange > disatanceToAttack)
            {
                goblinBomb->isAttack = true;
                goblinBomb->isAttackBomb = false;
                goblinBomb->isIdle = false;

                if (!wasAttacking)
                {
                    PlaySound(goblinBomb->soundAttack);
                    goblinBomb->currentFrame = 0; 
                }
            }
            else
            {
                goblinBomb->isAttackBomb = true;
                goblinBomb->soundBagPlayed = false;
                goblinBomb->isAttack = false;
                goblinBomb->isIdle = false;
            }
        }
        else
        {
            goblinBomb->isIdle = true;
            goblinBomb->isAttack = false;
            goblinBomb->isAttackBomb = false;
        }

        if (goblinBomb->isAttackBomb)
        {
            goblinBomb->timerAttackBomb += delta;

            if (goblinBomb->timerAttackBomb >= 1.5f)
            {
                goblinBomb->isAttackBomb = false;
                goblinBomb->isIdle = true;

                goblinBomb->bomb.isActive = true;
                PlaySound(goblinBomb->bomb.timer);
                goblinBomb->timerAttackBomb = 0.0f;
            }
        }
    }
    else
    {
        float right = 200.0f;
        
        if (goblinBomb->attackRange > disatanceToAttack && goblinBomb->direction == -1)
        {
            goblinBomb->isAttack = true;
            goblinBomb->isIdle = false;
            goblinBomb->isWalking = false;
        }
        else if (right > disatanceToAttack && goblinBomb->direction == 1)
        {
            goblinBomb->isAttack = true;
            goblinBomb->isIdle = false;
            goblinBomb->isWalking = false;
        }
        goblinBomb->isAttackBomb = false;
    }

    wasAttacking = goblinBomb->isAttack;

    Rectangle playerRec =
        {
            player->position.x + PLAYER_HITBOX_OFFSET_X,
            player->position.y + PLAYER_HITBOX_OFFSET_Y,
            player->frameWidth,
            player->frameHeight};

    Rectangle goblinRec =
        {
            goblinBomb->position.x + GOBLIN_HITBOX_OFFSET_X,
            goblinBomb->position.y + GOBLIN_HITBOX_OFFSET_Y,
            goblinBomb->frameWidthAttack / GOBLIN_HITBOX_SCALE,
            goblinBomb->frameHeightAttack / GOBLIN_HITBOX_SCALE};

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
            goblinBomb->bomb.pos.x += goblinBomb->bomb.speed * goblinBomb->direction * delta;

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

            goblinBomb->isIdle = true;
            goblinBomb->isAttackBomb = false;
            goblinBomb->timerForExplosion = 0.0f;
            goblinBomb->bomb.playerIsDamage = false;
        }
    }

    goblinBomb->animAttackTimer -= delta;

    if (goblinBomb->isAttack && goblinBomb->currentFrame == 7 && goblinBomb->animAttackTimer <= 0.0)
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

        goblinBomb->position.x + GOBLIN_HITBOX_OFFSET_X, 
        goblinBomb->position.y + GOBLIN_HITBOX_OFFSET_Y, 
        goblinBomb->frameWidthHurt / GOBLIN_SCALE_WIDTH, 
        goblinBomb->frameHeightHurt / GOBLIN_SCALE_HEIGHT
        ))
    {
        player->position.x += (player->position.x < goblinBomb->position.x) ? -push : push;
    }
}

void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player)
{
    Rectangle source;
    Rectangle dest;

    if (goblinBomb->isDead)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthDeath, 0, goblinBomb->frameWidthDeath * goblinBomb->direction, goblinBomb->frameHeightDeath};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthDeath * goblinBomb->frameScale, goblinBomb->frameHeightDeath * goblinBomb->frameScale};

        DrawTexturePro(goblinBomb->spriteDeath, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->goblinHasHit)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthHurt, 0, goblinBomb->frameWidthHurt * goblinBomb->direction, goblinBomb->frameHeightHurt};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthHurt * goblinBomb->frameScale, goblinBomb->frameHeightHurt * goblinBomb->frameScale};

        DrawTexturePro(goblinBomb->spriteHurt, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->isIdle)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthIdle, 0, goblinBomb->frameWidthIdle * goblinBomb->direction, goblinBomb->frameHeightIdle};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthIdle * goblinBomb->frameScale, goblinBomb->frameHeightIdle * goblinBomb->frameScale};

        DrawTexturePro(goblinBomb->spriteIdle, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->isAttackBomb)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthAttackBomb, 0, goblinBomb->frameWidthAttackBomb * goblinBomb->direction, goblinBomb->frameHeightAttackBomb};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthAttackBomb * goblinBomb->frameScale, goblinBomb->frameHeightAttackBomb * goblinBomb->frameScale};

        DrawTexturePro(goblinBomb->spriteAttackBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->isAttack)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthAttack, 0, goblinBomb->frameWidthAttack * goblinBomb->direction, goblinBomb->frameHeightAttack};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthAttack * goblinBomb->frameScale, goblinBomb->frameHeightAttack * goblinBomb->frameScale};

        DrawTexturePro(goblinBomb->spriteAttack, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->isWalking)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthRun, 0, goblinBomb->frameWidthRun * goblinBomb->direction, goblinBomb->frameHeightRun};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthRun * goblinBomb->frameScale, goblinBomb->frameHeightRun * goblinBomb->frameScale};

        DrawTexturePro(goblinBomb->spriteRun, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }

    if (goblinBomb->bomb.isActive)
    {
        source = (Rectangle){goblinBomb->bomb.currentFrameBomb * goblinBomb->bomb.frameWidthBomb, 0, goblinBomb->bomb.frameWidthBomb * goblinBomb->direction, goblinBomb->bomb.frameHeightBomb};
        dest = (Rectangle){goblinBomb->bomb.pos.x, goblinBomb->bomb.pos.y + BOMB_CENTER_OFFSET_Y, goblinBomb->bomb.frameWidthBomb * goblinBomb->frameScale, goblinBomb->bomb.frameHeightBomb * goblinBomb->frameScale};

        DrawTexturePro(goblinBomb->spriteBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }

    DrawGoblinBombLifeBar(goblinBomb);
}

void UnloadGoblinBomb(GoblinBomb *goblinBomb)
{
    UnloadTexture(goblinBomb->spriteAttack);
    UnloadTexture(goblinBomb->spriteAttackBomb);
    UnloadTexture(goblinBomb->spriteBomb);
    UnloadTexture(goblinBomb->spriteIdle);
    UnloadTexture(goblinBomb->spriteDeath);
    UnloadTexture(goblinBomb->spriteHurt);
    UnloadTexture(goblinBomb->spriteRun);

    UnloadSound(goblinBomb->soundAttack);
    UnloadSound(goblinBomb->bomb.explosion);
}
