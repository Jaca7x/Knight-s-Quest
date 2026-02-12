#include "drawMonsters.h"

void DrawMonsters(Monsters *monsters, int rowBase, int offsetHurtY, int offsetDeadY, int offsetAtackY, int offsetWalkY, int offsetIdleY)
{
    printf("direction: %d\n", monsters->direction);
    
    Rectangle source = {0, 0, 0, 0};
    Texture2D currentTexture;
    Rectangle dest = {0, 0, 0, 0};

    if (monsters->monsterHasHit)
    {
        float hurtOffSetY = offsetHurtY;

        currentTexture = monsters->spriteHurt;
        
        source = (Rectangle)
        {
            monsters->currentFrame * monsters->frameWidthHurt, 
            rowBase, 
            monsters->frameWidthHurt * monsters->direction, 
            monsters->frameHeightHurt
        };
        
        dest = (Rectangle)
        {
            monsters->position.x, 
            monsters->position.y, 
            source.width / monsters->scale,
             source.height / monsters->scale
        };

        dest.y += hurtOffSetY;
    }
    else if (monsters->isDead)
    {
        float hurtOffSetY = offsetDeadY;

        currentTexture = monsters->spriteDead;

        source = (Rectangle){
            monsters->currentFrame * monsters->frameWidthDead, 
            rowBase, 
            monsters->frameWidthDead * monsters->direction, 
            monsters->frameHeightDead
        };
        
        dest = (Rectangle){monsters->position.x, monsters->position.y, source.width / monsters->scale, source.height / monsters->scale};

        dest.y += hurtOffSetY;
    }
    else if (monsters->isAtacking)
    {
        float hurtOffSetY = offsetAtackY;

        currentTexture = monsters->spriteAtack;
        source = (Rectangle){monsters->currentFrame * monsters->frameWidthAtack, rowBase, monsters->frameWidthAtack * monsters->direction, monsters->frameHeightAtack};
        dest = (Rectangle){monsters->position.x, monsters->position.y, source.width / monsters->scale, source.height / monsters->scale};

        dest.y += hurtOffSetY;
    }
    else if (monsters->isWalking)
    {
        float hurtOffSetY = offsetWalkY;

        currentTexture = monsters->spriteWalk;
        source = (Rectangle){monsters->currentFrame * monsters->frameWidthWalk, rowBase, monsters->frameWidthWalk * monsters->direction, monsters->frameHeightWalk};
        dest = (Rectangle){monsters->position.x, monsters->position.y, source.width / monsters->scale, source.height / monsters->scale};

        dest.y += hurtOffSetY;
    }
    else if (monsters->isIdle)
    {
        float hurtOffSetY = offsetIdleY;

        currentTexture = monsters->spriteIdle;
        source = (Rectangle){monsters->currentFrame * monsters->frameWidthIdle, rowBase, monsters->frameWidthIdle * monsters->direction, monsters->frameHeightIdle};
        dest = (Rectangle){monsters->position.x, monsters->position.y, source.width / monsters->scaleIdle, source.height / monsters->scaleIdle};

        dest.y += hurtOffSetY;
    }

    Vector2 origin = {0, 0};
    float rotation = 0.0f;

    DrawTexturePro(currentTexture, source, dest, origin, rotation, WHITE);
}