#ifndef LIFE_H
#define LIFE_H

typedef struct player Player; 
typedef struct wolf Wolf;

#include "raylib.h"  
#include "entities/player/player.h"
#include "entities/wolf/wolf.h"     

#define MAX_LIFE 100

#define LIFE_FRAME_WIDTH 32
#define LIFE_FRAME_HEIGHT 32

#define LIFE_FRAME_CONT 8

typedef struct life
{
    Texture2D barLifeSprite;
} Life;

void DrawLifeBar(Texture2D barLifeSprite, float life, Vector2 position, float scale);

#endif //LIFE_H