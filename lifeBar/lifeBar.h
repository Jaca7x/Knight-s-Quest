#ifndef LIFE_H
#define LIFE_H

typedef struct player Player; // <- Declaração antecipada
typedef struct wolf Wolf; // <- Declaração antecipada

#include "../librays/raylib.h"   // Biblioteca Raylib
#include "../player/player.h"
#include "../wolf/wolf.h"     // Arquivo do player (usa a stamina do player)

#define MAX_LIFE 200

#define LIFE_FRAME_WIDTH 32
#define LIFE_FRAME_HEIGHT 32

#define LIFE_FRAME_CONT 8

typedef struct life
{
    Texture2D barLifeSprite;
} Life;

void DrawLifeBar(Texture2D barLifeSprite, float life, Vector2 position, float scale);

void UpdateLifeBar(Player *player, Wolf *wolf, float delta);
#endif //LIFE_H