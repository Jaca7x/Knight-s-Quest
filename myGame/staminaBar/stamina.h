#ifndef STAMINA_H
#define STAMINA_H

#include "../librays/raylib.h" //Biblioteca raylib
#include "../player/player.h" //Arquivo player 

#define MAX_STAMINA 150 //Estamina total
#define STAMINA_FRAME_WIDTH 32 //largura dos frames
#define STAMINA_FRAME_HEIGHT 32 //altura dos frames
#define STAMINA_FRAME_COUNT 8 //total de frames

typedef struct staminaBar //struct barra de stamina
{
    Texture2D barStamina; //textura da barra de stamina
} Stamina;

void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale); //Func para desenhar a barra
void UpdateStaminaBar(Player *player, float delta); //Func para atulizar a barra

#endif //FIM
