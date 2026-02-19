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
        0,                       
        frame * LIFE_FRAME_WIDTH, 
        LIFE_FRAME_WIDTH,        
        LIFE_FRAME_HEIGHT        
    };

    Rectangle dest = 
    {
        position.x,                     
        position.y,                     
        LIFE_FRAME_WIDTH * scale* 2,
        LIFE_FRAME_HEIGHT * scale* 2 
    };

    Vector2 origin = ORIGIN_TOPLEFT;

    DrawTexturePro(barLifeSprite, source, dest, origin, ROTATION, WHITE);
}
