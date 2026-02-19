#include "lifeBar.h"

void DrawLifeBar(Texture2D barLifeSprite, float life, Vector2 position, float scale)
{
    int frame;

    if (life == LIFE_ZERO)
    {
        frame = LIFE_FRAME_CONT - 2;
    }
    else
    {
        frame = (int)((1.0f - life / MAX_LIFE) * (LIFE_FRAME_CONT - 1));
    }

    Rectangle source = 
    {
        SPRITE_ROW_BASE,                       
        frame * LIFE_FRAME_WIDTH, 
        LIFE_FRAME_WIDTH,        
        LIFE_FRAME_HEIGHT        
    };

    Rectangle dest = 
    {
        position.x,                     
        position.y,                     
        LIFE_FRAME_WIDTH * scale,
        LIFE_FRAME_HEIGHT * scale
    };

    Vector2 origin = ORIGIN_TOPLEFT;

    DrawTexturePro(barLifeSprite, source, dest, origin, ROTATION, WHITE);
}
