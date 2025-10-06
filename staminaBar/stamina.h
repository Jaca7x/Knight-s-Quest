#ifndef STAMINA_H
#define STAMINA_H

#include "../librays/raylib.h"   
#include "../player/player.h"    

#define MAX_STAMINA 150

#define STAMINA_FRAME_WIDTH 32
#define STAMINA_FRAME_HEIGHT 32

#define STAMINA_FRAME_COUNT 8

typedef struct staminaBar
{
    Texture2D barStamina;   
} Stamina;

void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale, Player *player);
void UpdateStaminaBar(Player *player, float delta);

#endif // STAMINA_H
