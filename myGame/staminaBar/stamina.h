#ifndef STAMINA_H
#define STAMINA_H

#include "../librays/raylib.h"   // Biblioteca Raylib
#include "../player/player.h"    // Arquivo do player (usa a stamina do player)

/// @brief Quantidade máxima de stamina
#define MAX_STAMINA 150

/// @brief Dimensões dos frames da barra de stamina
#define STAMINA_FRAME_WIDTH 32
#define STAMINA_FRAME_HEIGHT 32

/// @brief Número total de frames da barra (animação ou variação de preenchimento)
#define STAMINA_FRAME_COUNT 8

/// @brief Struct que representa a barra de stamina
typedef struct staminaBar
{
    Texture2D barStamina;   // Textura da barra de stamina
} Stamina;

/// @brief Desenha a barra de stamina na tela
/// @param bar Textura da barra
/// @param stamina Valor atual da stamina (0 até MAX_STAMINA)
/// @param position Posição na tela (x, y)
/// @param scale Fator de escala da barra (1.0f = tamanho original)
void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale);

/// @brief Atualiza o valor da stamina do jogador (consumo e regeneração)
/// @param player Ponteiro para o player
/// @param delta Tempo entre os frames (GetFrameTime())
void UpdateStaminaBar(Player *player, float delta);

#endif // STAMINA_H
