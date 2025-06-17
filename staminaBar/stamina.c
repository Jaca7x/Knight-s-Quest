#include "stamina.h"

/// @brief Desenha a barra de stamina na tela.
/// @param bar Texture2D da barra (spritesheet com os frames da barra)
/// @param stamina Valor atual da stamina
/// @param position Posição na tela para desenhar
/// @param scale Fator de escala para aumentar ou diminuir o tamanho
void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale)
{
    int frame;

    // Define qual frame será usado de acordo com a stamina atual
    if (stamina == 0)
    {
        // Se stamina zerada, usa o penúltimo frame (índice = STAMINA_FRAME_COUNT - 2)
        frame = STAMINA_FRAME_COUNT - 2;
    }
    else 
    {
        // Calcula o frame proporcional à stamina atual
        frame = (int)((1.0f - stamina / MAX_STAMINA) * (STAMINA_FRAME_COUNT - 1));
    }

    // Retângulo da textura (source) que será recortado
    Rectangle source = {
        0,                                       // X inicial
        frame * STAMINA_FRAME_HEIGHT,            // Y inicial baseado no frame
        STAMINA_FRAME_WIDTH,                     // Largura do frame
        STAMINA_FRAME_HEIGHT                     // Altura do frame
    };

    // Retângulo de destino (dest) onde será desenhado na tela
    Rectangle dest = {
        position.x,                              // Posição X na tela
        position.y,                              // Posição Y na tela
        STAMINA_FRAME_WIDTH * scale,             // Largura ajustada pela escala
        STAMINA_FRAME_HEIGHT * scale             // Altura ajustada pela escala
    };

    // Origem no canto superior esquerdo
    Vector2 origin = {0, 0};

    // Desenha na tela a barra de stamina com o frame correspondente
    DrawTexturePro(bar, source, dest, origin, 0.0f, WHITE);
}

/// @brief Atualiza o valor da stamina do player.
/// @param player Ponteiro para a struct Player
/// @param delta Delta time para movimento suave independente de FPS
void UpdateStaminaBar(Player *player, float delta)
{   
// -------- Corrida --------
if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && IsKeyDown(KEY_LEFT_SHIFT) && player->stamina > 0)
{
    player->isRunning = true;
    player->stamina -= 25.0f * delta;
    if (player->stamina < 0) player->stamina = 0;
}
else
{
    player->isRunning = false;
}

// -------- Pulo correndo --------
if (player->isRunning && IsKeyDown(KEY_SPACE) && (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)))
{
    if (player->stamina > 50)
    {
        player->stamina -= 35.0f * delta;
        if (player->stamina < 0) player->stamina = 0;
    } 
    else 
    {
        player->position.y = player->groundY;
        player->velocityY = 0.0f;
        player->isJumping = false;
    }
}

// -------- Pulo normal --------
else if (IsKeyDown(KEY_SPACE))
{
    if (player->stamina > 50)
    {
        player->stamina -= 35.0f * delta;
        if (player->stamina < 0) player->stamina = 0;
    } 
    else 
    {
        player->position.y = player->groundY;
        player->velocityY = 0.0f;
        player->isJumping = false;
    }
}

float regenTimer = 0.0f;



// -------- Regeneração --------
if ( IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_SPACE)
        || IsMouseButtonDown(MOUSE_BUTTON_LEFT) || 
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
{
    regenTimer = 1.0f;
}
else 
{
    regenTimer -= delta;

    if (regenTimer <= 0.0f)
    {
    player->stamina += 20.0f * delta;
    if (player->stamina > MAX_STAMINA) player->stamina = MAX_STAMINA;
    }
}   
}

