#ifndef STAMINA_H
#define STAMINA_H

#include "raylib.h"  
#include "entities/player/player.h"

#define MAX_STAMINA 150

#define STAMINA_FRAME_WIDTH 32
#define STAMINA_FRAME_HEIGHT 32

#define STAMINA_FRAME_COUNT 7

typedef struct staminaBar
{
    Texture2D barStamina;   
    bool isRegenerating;
} Stamina;

void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale, Player *player, bool button_pressed);
void UpdateStaminaBar(Player *player, float delta);

#endif // STAMINA_H
