#include "goblinBomb.h"

void InitGoblinBomb(GoblinBomb *goblinBomb)
{
    goblinBomb->spriteIdle = LoadTexture("resources/sprites/goblinBomb/Idle.png");
    goblinBomb->spriteAttackBomb = LoadTexture("resources/sprites/goblinBomb/Attack3.png");

    goblinBomb->position = (Vector2){800, 440};
    goblinBomb->bomb.pos = (Vector2){800, 440};
    goblinBomb->frameIdle = 4;
    goblinBomb->frameAttackBomb = 12;

    goblinBomb->frameWidthIdle = goblinBomb->spriteIdle.width / goblinBomb->frameIdle;
    goblinBomb->frameHeightIdle = goblinBomb->spriteIdle.height;
    
    goblinBomb->frameWidthAttackBomb = goblinBomb->spriteAttackBomb.width / goblinBomb->frameAttackBomb;
    goblinBomb->frameHeightAttackBomb = goblinBomb->spriteAttackBomb.height;

    goblinBomb->currentFrame = 0;
    goblinBomb->frameCounter = 0;

    goblinBomb->isIdle = true;
    goblinBomb->isAttackBomb = false;

    goblinBomb->bomb.isActive = false;
    goblinBomb->bombExplode = false;
    goblinBomb->hasThrownBomb = false;
    
    goblinBomb->spriteBomb = LoadTexture("resources/sprites/goblinBomb/Bomb_sprite.png");
    goblinBomb->damageBomb = 30;
    goblinBomb->timerForExplosion = 0.0f;
    goblinBomb->radiusToDamage = 100.0f;
    goblinBomb->bombRange = 200.0f;

    goblinBomb->bomb.frameBomb = 19;
    goblinBomb->bomb.frameWidthBomb = goblinBomb->spriteBomb.width / goblinBomb->bomb.frameBomb;
    goblinBomb->bomb.frameHeightBomb = goblinBomb->spriteBomb.height;

    goblinBomb->timerAttackBomb = 0.0f;
    goblinBomb->animAttackBombEnd = false;
    goblinBomb->bomb.currentFrameBomb = 0;
    goblinBomb->bomb.frameCounterBomb = 0;
    goblinBomb->bomb.speed = 30.0f;
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

    }

        if (goblinBomb->bomb.frameCounterBomb >= 20)
        {
            goblinBomb->bomb.frameCounterBomb = 0;

            if (goblinBomb->bomb.isActive)
            {
                goblinBomb->bomb.currentFrameBomb++;
                if (goblinBomb->bomb.currentFrameBomb >= goblinBomb->bomb.frameBomb)
                {
                    goblinBomb->bomb.currentFrameBomb = 0;
                }
            } 
        }
    
    float disatanceToAttack = fabs(player->position.x - goblinBomb->position.x);

    if (!goblinBomb->hasThrownBomb)
    {
        if (goblinBomb->bombRange > disatanceToAttack)
        {
            if (!goblinBomb->bomb.isActive && !goblinBomb->isAttackBomb)
            {
                goblinBomb->isAttackBomb = true;
                goblinBomb->isIdle = false;
                goblinBomb->timerAttackBomb = 0.0f;
            }
        }
        else
        {
            goblinBomb->isAttackBomb = false;
            goblinBomb->isIdle = true;
        }
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

    if (goblinBomb->bomb.isActive)
    {
        goblinBomb->timerForExplosion += delta;

        if (goblinBomb->timerForExplosion < 5.5f)
        {
            goblinBomb->bomb.pos.x -= goblinBomb->bomb.speed * delta;
        }
        else
        {
            goblinBomb->bombExplode = true;
            goblinBomb->bomb.isActive = false;
            goblinBomb->hasThrownBomb = true;

            goblinBomb->isIdle = true;
            goblinBomb->isAttackBomb = false;
            goblinBomb->timerForExplosion = 0.0f;
        }
    }
}

void DrawGoblinBomb(GoblinBomb *goblinBomb)
{
    Rectangle dest;
    Rectangle source;

    if (goblinBomb->isIdle)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthIdle, 0, goblinBomb->frameWidthIdle, goblinBomb->frameHeightIdle};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthIdle * 2.0f, goblinBomb->frameHeightIdle * 2.0f};

        DrawTexturePro(goblinBomb->spriteIdle, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    else if (goblinBomb->isAttackBomb)
    {
        source = (Rectangle){goblinBomb->currentFrame * goblinBomb->frameWidthAttackBomb, 0, goblinBomb->frameWidthAttackBomb, goblinBomb->frameHeightAttackBomb};
        dest = (Rectangle){goblinBomb->position.x, goblinBomb->position.y, goblinBomb->frameWidthAttackBomb * 2.0f, goblinBomb->frameHeightAttackBomb * 2.0f};

        DrawTexturePro(goblinBomb->spriteAttackBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
    
    if (goblinBomb->bomb.isActive)
    {
        source = (Rectangle){goblinBomb->bomb.currentFrameBomb * goblinBomb->bomb.frameWidthBomb, 0, goblinBomb->bomb.frameWidthBomb, goblinBomb->bomb.frameHeightBomb};
        dest = (Rectangle){goblinBomb->bomb.pos.x, goblinBomb->bomb.pos.y + 50, goblinBomb->bomb.frameWidthBomb * 2.0f, goblinBomb->bomb.frameHeightBomb * 2.0f};

        DrawTexturePro(goblinBomb->spriteBomb, source, dest, (Vector2){0, 0}, 0.0f, RAYWHITE);
    }
}
