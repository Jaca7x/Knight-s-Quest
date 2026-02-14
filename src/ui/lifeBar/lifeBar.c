#include "lifeBar.h"

void DrawLifeBar(Texture2D barLifeSprite, float life, Vector2 position, float scale) 
{

    int frame;

    if (life == 0)
    {
       frame = LIFE_FRAME_CONT - 2;
    } else 
    {
        frame = (int)((1.0f - life / MAX_LIFE) * (LIFE_FRAME_CONT - 1));
    }
    
    Rectangle source = {
        0,                                       // X inicial
        frame * LIFE_FRAME_WIDTH,            // Y inicial baseado no frame
        LIFE_FRAME_WIDTH,                     // Largura do frame
        LIFE_FRAME_HEIGHT                     // Altura do frame
    };


    Rectangle dest = {
        position.x,                           // Posição X na tela
        position.y,                         // Posição Y na tela
        LIFE_FRAME_WIDTH * scale * 1.2,             // Largura ajustada pela escala
        LIFE_FRAME_HEIGHT * scale * 1.2          // Altura ajustada pela escala
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(barLifeSprite, source, dest, origin, 0.0f, WHITE);
}
