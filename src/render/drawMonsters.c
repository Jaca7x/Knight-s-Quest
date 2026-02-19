#include "drawMonsters.h"

Rectangle GetSourceValueRec(int frame, int frameWidth, float direction, int frameHeigth)
{
    Rectangle r;
    r.x = frame * frameWidth;
    r.y = 0;
    r.width = frameWidth * direction;
    r.height = frameHeigth;

    return r;
}

Rectangle GetDestValueRec(Vector2 position, int frameWidth, int frameHeight, float scale)
{
    Rectangle r;

    r.x = position.x;
    r.y = position.y;
    r.width = frameWidth * scale;
    r.height = frameHeight * scale;

    return r;
}


void DrawMonsters(Monsters *monsters, int rowBase, int offsetHurtY, int offsetDeadY, int offsetAtackY, int offsetWalkY, int offsetIdleY)
{
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

    Vector2 origin = ORIGIN_TOPLEFT;
    float rotation = ROTATION;

    DrawTexturePro(currentTexture, source, dest, origin, rotation, WHITE);
}

void DrawBossAnim(Monsters *monsters)
{
    Rectangle source;
    Rectangle dest;

    if (monsters->isDead)
    {
        float hurtOffsetY = 45.0f;

        source = GetSourceValueRec(monsters->currentFrame, monsters->frameWidthDead,
                               monsters->direction, monsters->frameHeightDead);
        dest = GetDestValueRec(monsters->position, monsters->frameWidthDead,
                           monsters->frameHeightDead, BOSS_SCALE_ATK_AND_WALK_AND_DEATH);

        dest.y += hurtOffsetY;

        DrawTexturePro(monsters->spriteDead, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
    }
    else if (monsters->monsterHasHit)
    {
        float hurtOffsetY = -15.0f;

        source = GetSourceValueRec(monsters->currentFrame, monsters->frameWidthHurt,
                               monsters->direction, monsters->frameHeightHurt);
        dest = GetDestValueRec(monsters->position, monsters->frameWidthHurt,
                           monsters->frameHeightHurt, BOSS_SCALE_HURT);

        dest.y += hurtOffsetY;

        DrawTexturePro(monsters->spriteHurt, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
    }
    else if (monsters->isAtacking)
    {
        float attackOffsetY = -35.0f;

        source = GetSourceValueRec(monsters->currentFrame, monsters->frameWidthAtack,
                               monsters->direction, monsters->frameHeightAtack);
        dest = GetDestValueRec(monsters->position, monsters->frameWidthAtack,
                           monsters->frameHeightAtack, BOSS_SCALE_ATK_AND_WALK_AND_DEATH);

        dest.y += attackOffsetY;

        DrawTexturePro(monsters->spriteAtack, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
    }
    else if (monsters->isWalking)
    {
        source = GetSourceValueRec(monsters->currentFrame, monsters->frameWidthWalk,
                               monsters->direction, monsters->frameHeightWalk);
        dest = GetDestValueRec(monsters->position, monsters->frameWidthWalk,
                           monsters->frameHeightWalk, BOSS_SCALE_ATK_AND_WALK_AND_DEATH);

        DrawTexturePro(monsters->spriteWalk, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
    }
    else
    {
        source = GetSourceValueRec(monsters->currentFrame, monsters->frameWidthIdle,
                               monsters->direction, monsters->frameHeightIdle);
        dest = GetDestValueRec(monsters->position, monsters->frameWidthIdle,
                           monsters->frameHeightIdle, BOSS_SCALE_IDLE);

        DrawTexturePro(monsters->spriteIdle, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
    }
}