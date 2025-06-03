#ifndef LIFE_H
#define LIFE_H

#include "../librays/raylib.h"   // Biblioteca Raylib
#include "../player/player.h"    // Arquivo do player (usa a stamina do player)

#define MAX_LIFE 200

#define LIFE_FRAME_WIDTH 32
#define LIFE_FRAME_HEIGHT 32

#define LIFE_FRAME_CONT 8

typedef struct lifeBar
{
    Texture2D lifeBar;
} LifeBar;

void DrawLifeBar(Texture2D lifeBar, float life, Vector2 position, float scale);

void UpdateLifeBar(Player *player, float delta);

#endif //LIFE_H