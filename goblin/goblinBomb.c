#include "goblinBomb.h"

void InitGoblinBomb(GoblinBomb *goblinBomb)
{
    goblinBomb->spriteIdle = LoadTexture("resources/sprites/goblinBomb/Idle.png");
    goblinBomb->spriteAttackBomb = LoadTexture("resources/sprites/goblinBomb/Attack3.png");
    goblinBomb->spriteAttack = LoadTexture("resources/sprites/goblinBomb/Attack1.png");

    goblinBomb->position = (Vector2){800, 440};
    goblinBomb->bomb.pos = (Vector2){800, 440};
    goblinBomb->frameIdle = 4;
    goblinBomb->frameAttackBomb = 12;
    goblinBomb->frameAttack = 8;

    goblinBomb->frameWidthIdle = goblinBomb->spriteIdle.width / goblinBomb->frameIdle;
    goblinBomb->frameHeightIdle = goblinBomb->spriteIdle.height;
    
    goblinBomb->frameWidthAttackBomb = goblinBomb->spriteAttackBomb.width / goblinBomb->frameAttackBomb;
    goblinBomb->frameHeightAttackBomb = goblinBomb->spriteAttackBomb.height;

    goblinBomb->frameWidthAttack = goblinBomb->spriteAttack.width / goblinBomb->frameAttack;
    goblinBomb->frameHeightAttack = goblinBomb->spriteAttack.height;

    goblinBomb->currentFrame = 0;
    goblinBomb->frameCounter = 0;

    goblinBomb->isIdle = true;
    goblinBomb->isAttackBomb = false;

    goblinBomb->bomb.isActive = false;
    goblinBomb->bombExplode = false;
    goblinBomb->hasThrownBomb = false;
    goblinBomb->isAttack = false;
    
    goblinBomb->spriteBomb = LoadTexture("resources/sprites/goblinBomb/Bomb_sprite.png");
    goblinBomb->damageBomb = 30;
    goblinBomb->timerForExplosion = 0.0f;
    goblinBomb->radiusToDamage = 50.0f;
    goblinBomb->bombRange = 200.0f;
    goblinBomb->attackRange = 70.0f;

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
}


void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player)
{
    goblinBomb->frameCounter++;
    goblinBomb->bomb.frameCounterBomb++;

    if (goblinBomb->frameCounter >= 10)
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
            if (goblinBomb->currentFrame >= goblinBomb->frameAttackBomb)
            {
                goblinBomb->currentFrame = 0;
            }
        }
        else if (goblinBomb->isAttack)
        {
            goblinBomb->currentFrame++;
            if (goblinBomb->currentFrame >= goblinBomb->frameAttack)
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
                if (goblinBomb->bomb.currentFrameBomb >= goblinBomb->bomb.frameBomb)
                {
                    goblinBomb->bomb.currentFrameBomb = 0;
                    goblinBomb->bomb.frameCounterBomb = 0;
                }
            } 
        }
    
    float disatanceToAttack = fabs(player->position.x - goblinBomb->position.x);

    goblinBomb->direction = (player->position.x - 60 > goblinBomb->position.x) ? 1 : -1;

    if (!goblinBomb->hasThrownBomb)
    {
        if (goblinBomb->bombRange > disatanceToAttack)
        {
            if (goblinBomb->attackRange > disatanceToAttack)
            {
                goblinBomb->isAttack = true;
                goblinBomb->isAttackBomb = false;
                goblinBomb->isIdle = false;
            }
            else
            {
                goblinBomb->isAttackBomb = true;
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
                goblinBomb->timerAttackBomb = 0.0f;
            }
        }
    }
    else
    {
        if (goblinBomb->attackRange > disatanceToAttack)
        {
            goblinBomb->isAttack = true;
            goblinBomb->isIdle = false;
        }
        else
        {
            goblinBomb->isIdle = true;
            goblinBomb->isAttack = false;
        }
        goblinBomb->isAttackBomb = false;
    }

    Rectangle playerRec = 
    {
        player->position.x + 50,
        player->position.y + 40,
        player->frameWidth,
        player->frameHeight
    };
    
    Rectangle goblinRec =
    {
        goblinBomb->position.x + 110,
        goblinBomb->position.y + 130,
        goblinBomb->frameWidthAttack / 2,
        goblinBomb->frameHeightAttack / 2
    };

    Vector2 centerCircle =
    {
        goblinBomb->bomb.pos.x + (goblinBomb->bomb.frameWidthBomb * 1.2f) / 1.2f + 5,
        goblinBomb->bomb.pos.y + (goblinBomb->bomb.frameHeightBomb * 1.2f) / 1.2f + 50
    };

    if (goblinBomb->bomb.isActive)
    {
        goblinBomb->timerForExplosion += delta;

        if (goblinBomb->timerForExplosion < 3.140f)
        {
            if (goblinBomb->direction == 1)
            {
                goblinBomb->bomb.pos.x -= goblinBomb->bomb.speed * goblinBomb->direction * delta;
            }
            else if (goblinBomb->direction == -1)
            {
                goblinBomb->bomb.pos.x += goblinBomb->bomb.speed * goblinBomb->direction * delta;
            }
                       
            if (!goblinBomb->bomb.playerIsDamage && CheckCollisionCircleRec(centerCircle, goblinBomb->radiusToDamage, playerRec) && goblinBomb->bomb.currentFrameBomb == 14)
            {
                player->life -= 50;
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
            player->life -= 15;
            player->hasHit = true;
            player->hitTimer = 0.4f;

            goblinBomb->animAttackTimer = 1.0f;
        }   
    }
}

void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player)
{
    Rectangle source;
    Rectangle dest;

    Rectangle goblinRec =
    {
        goblinBomb->position.x + 110,
        goblinBomb->position.y + 130,
        goblinBomb->frameWidthAttack / 2,
        goblinBomb->frameHeightAttack / 2
    };

    Rectangle playerRec = 
    {
        player->position.x + 50,
        player->position.y + 40,
        player->frameWidth,
        player->frameHeight
    };

    if (goblinBomb->isIdle)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthIdle, 0, goblinBomb->frameWidthIdle * goblinBomb->direction, goblinBomb->frameHeightIdle};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthIdle * 2.0f, goblinBomb->frameHeightIdle * 2.0f};

        DrawTexturePro(goblinBomb->spriteIdle, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->isAttackBomb)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthAttackBomb, 0, goblinBomb->frameWidthAttackBomb * goblinBomb->direction, goblinBomb->frameHeightAttackBomb};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthAttackBomb * 2.0f, goblinBomb->frameHeightAttackBomb * 2.0f};

        DrawTexturePro(goblinBomb->spriteAttackBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->isAttack)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthAttack, 0, goblinBomb->frameWidthAttack * goblinBomb->direction, goblinBomb->frameHeightAttack};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthAttack * 2.0f, goblinBomb->frameHeightAttack * 2.0f};

        DrawTexturePro(goblinBomb->spriteAttack, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    
    if (goblinBomb->bomb.isActive)
    {
        source = (Rectangle){goblinBomb->bomb.currentFrameBomb * goblinBomb->bomb.frameWidthBomb, 0, goblinBomb->bomb.frameWidthBomb * goblinBomb->direction, goblinBomb->bomb.frameHeightBomb};
        dest = (Rectangle){goblinBomb->bomb.pos.x, goblinBomb->bomb.pos.y + 50, goblinBomb->bomb.frameWidthBomb * 2.0f, goblinBomb->bomb.frameHeightBomb * 2.0f};

        DrawTexturePro(goblinBomb->spriteBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }

    DrawRectangleLinesEx(goblinRec, 1, GREEN);
    DrawRectangleLinesEx(playerRec, 1, RED);
}

void UnloadGoblinBomb(GoblinBomb *goblinBomb)
{
    UnloadTexture(goblinBomb->spriteAttack);
    UnloadTexture(goblinBomb->spriteAttackBomb);
    UnloadTexture(goblinBomb->spriteBomb);
    UnloadTexture(goblinBomb->spriteIdle);
}
