#include "goblinBomb.h"

void InitGoblinBomb(GoblinBomb *goblinBomb)
{
    goblinBomb->spriteIdle = LoadTexture("resources/sprites/goblinBomb/Idle.png");
    goblinBomb->spriteAttackBomb = LoadTexture("resources/sprites/goblinBomb/Attack3.png");

    goblinBomb->position = (Vector2){800, 300};

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

    goblinBomb->spriteBomb = LoadTexture("resources/sprites/goblinBomb/Bomb_sprite.png");
    goblinBomb->damageBomb = 30;
    goblinBomb->timerForExplosion = 0.0f;
    goblinBomb->radiusToDamage = 100.0f;
    goblinBomb->bombRange = 200.0f;
}


void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player)
{
    goblinBomb->frameCounter++;

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
    
    float disatanceToAttack = fabs(player->position.x - goblinBomb->position.x);

    if (goblinBomb->bombRange > disatanceToAttack)
    {
        goblinBomb->isIdle = false;
        goblinBomb->isAttackBomb = true;
    }
    else
    {
        goblinBomb->isIdle = true;
        goblinBomb->isAttackBomb = false;
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
    

}
