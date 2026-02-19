#ifndef STAMINA_H
#define STAMINA_H

#include "raylib.h"  
#include "entities/player/player.h"
#include "core/define.h"

//STATS
#define MAX_STAMINA 150
#define STAMINA_REGENERATION 20.0f
#define RUNNING_STAMINA_EXPENDITURE 25.0f

//SIZE & OFFSET & FRAMES
#define STAMINA_FRAME_WIDTH 32
#define STAMINA_FRAME_HEIGHT 32

#define STAMINA_OFFSET 20

#define STAMINA_FRAME_COUNT 7

typedef struct staminaBar
{
    Texture2D barStamina;   
    bool isRegenerating;
} Stamina;

void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale, Player *player, bool button_pressed);
void UpdateStaminaBar(Player *player, float delta);

#endif