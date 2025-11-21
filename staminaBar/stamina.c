#include "stamina.h"

bool isRegenerating = false;
float textTimer = 0.0f;

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

// -------- Regeneração --------
float regenTimer = 0.0f;

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
        if (player->stamina < MAX_STAMINA)
        {
            isRegenerating = true;
        }
        else 
        {
            isRegenerating = false;
        }
        player->stamina += 20.0f * delta;
        if (player->stamina > MAX_STAMINA) player->stamina = MAX_STAMINA;  
    }
}

printf  ("Stamina: %.2f\n", player->stamina);
}


void DrawStaminaBar(Texture2D bar, float stamina, Vector2 position, float scale, Player *player, bool button_pressed)
{
    int frame;
    float delta = GetFrameTime();

    // Define qual frame será usado de acordo com a stamina atual
    if (stamina == 0)
    {
        // Se stamina zerada, usa o penúltimo frame (índice = STAMINA_FRAME_COUNT - 2)
        frame = STAMINA_FRAME_COUNT + 5;
        DrawText("Estamina Esgotada!", player->position.x + 20, player->position.y - 20, 20, BLACK);
    }
    else 
    {
        // Calcula o frame proporcional à stamina atual
        frame = (int)((1.0f - stamina / MAX_STAMINA) * (STAMINA_FRAME_COUNT - 1));
    }
    
    if (button_pressed)
    {
        if (isRegenerating)
    {
        DrawText("Estamina Regenerando...", player->position.x + 20, player->position.y - 20, 20, BLACK);
        textTimer = 0.0f;
    } 
    else 
    {
        textTimer += delta;
        if (textTimer < 1.0f)
        {
             DrawText("Estamina cheia!", player->position.x + 20, player->position.y - 20, 20, BLACK);
        }
    }
    }
    
    

    Rectangle source = {
        0,                                      
        frame * STAMINA_FRAME_HEIGHT,            
        STAMINA_FRAME_WIDTH,                     
        STAMINA_FRAME_HEIGHT                     
    };

    Rectangle dest = {
        position.x,                              
        position.y,                              
        STAMINA_FRAME_WIDTH * scale,             
        STAMINA_FRAME_HEIGHT * scale             
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(bar, source, dest, origin, 0.0f, WHITE);
}


