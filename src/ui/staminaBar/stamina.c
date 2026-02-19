#include "stamina.h"

bool isRegenerating = false;
float textTimer = 0.0f;

void UpdateStaminaBar(Player *player, float delta)
{
    if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && IsKeyDown(KEY_LEFT_SHIFT) && player->stamina > STAMINA_ZERO)
    {
        player->isRunning = true;
        player->stamina -= RUNNING_STAMINA_EXPENDITURE * delta;
        if (player->stamina < STAMINA_ZERO)
            player->stamina = STAMINA_ZERO;
    }
    else
    {
        player->isRunning = false;
    }

    float regenTimer = TIMER_ZERO;

    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        regenTimer = 1.0f;
    }
    else
    {
        regenTimer -= delta;

        if (regenTimer <= TIMER_ZERO)
        {
            if (player->stamina < MAX_STAMINA)
            {
                isRegenerating = true;
            }
            else
            {
                isRegenerating = false;
            }
            player->stamina += STAMINA_REGENERATION * delta;
            if (player->stamina > MAX_STAMINA)
                player->stamina = MAX_STAMINA;
        }
    }
}

void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale, Player *player, bool button_pressed)
{
    int frame;
    float delta = GetFrameTime();

    if (stamina == STAMINA_ZERO)
    {
        frame = STAMINA_FRAME_COUNT - PREVIOUS_FRAME;
        DrawText("Estamina Esgotada!", player->position.x + STAMINA_OFFSET, player->position.y - STAMINA_OFFSET, 20, BLACK);
    }
    else
    {
        frame = (int)((1.0f - stamina / MAX_STAMINA) * (STAMINA_FRAME_COUNT - PREVIOUS_FRAME));
    }

    if (button_pressed)
    {
        if (isRegenerating)
        {
            DrawText("Estamina Regenerando...", player->position.x + STAMINA_OFFSET, player->position.y - STAMINA_OFFSET, 20, BLACK);
            textTimer = TIMER_ZERO;
        }
        else
        {
            textTimer += delta;
            if (textTimer < 1.0f)
            {
                DrawText("Estamina cheia!", player->position.x + STAMINA_OFFSET, player->position.y - STAMINA_OFFSET, 20, BLACK);
            }
        }
    }

    Rectangle source = 
    {
        0,
        frame * STAMINA_FRAME_HEIGHT,
        STAMINA_FRAME_WIDTH,
        STAMINA_FRAME_HEIGHT
    };

    Rectangle dest = 
    {
        position.x,
        position.y,
        STAMINA_FRAME_WIDTH * scale,
        STAMINA_FRAME_HEIGHT * scale
    };

    Vector2 origin = ORIGIN_TOPLEFT;

    DrawTexturePro(bar, source, dest, origin, ROTATION, WHITE);
}
