#include "stamina.h"

// Função para desenhar a barra de stamina na tela
void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale)
{
    int frame;

    if (stamina == 0)
    {
        // Quando a stamina chega a zero, usa o penúltimo frame da barra
        frame = STAMINA_FRAME_COUNT - 2;
    }
    else 
    {
        // Calcula o frame da barra proporcional à stamina atual
        frame = (int)((1.0f - stamina / MAX_STAMINA) * (STAMINA_FRAME_COUNT - 1));
    }

    // Define qual parte da textura da barra será desenhada
    Rectangle source = {
        0, 
        frame * STAMINA_FRAME_HEIGHT,
        STAMINA_FRAME_WIDTH, 
        STAMINA_FRAME_HEIGHT
    };

    // Define onde a barra será desenhada na tela e seu tamanho
    Rectangle dest = {
        position.x, 
        position.y,
        STAMINA_FRAME_WIDTH * scale, 
        STAMINA_FRAME_HEIGHT * scale
    };

    Vector2 origin = {0, 0}; // Origem no canto superior esquerdo

    // Desenha a barra na tela
    DrawTexturePro(bar, source, dest, origin, 0.0f, WHITE);
}

// Função para atualizar a stamina do player
void UpdateStaminaBar(Player *player, float delta)
{
    // Verifica se o player está apertando D + SHIFT (correndo)
    if (IsKeyDown(KEY_D) && IsKeyDown(KEY_LEFT_SHIFT))
    {
        // Se tiver stamina, ela diminui enquanto corre
        if (player->stamina > 0)
        {
            player->stamina -= 20.0f * delta;
            if (player->stamina < 0)
                player->stamina = 0;
        }
    }
    else
    {
        // Se não estiver correndo, stamina regenera
        player->stamina += 10.0f * delta;
        if (player->stamina > MAX_STAMINA)
            player->stamina = MAX_STAMINA;
    }
}
