#include "player.h"

/// @brief Inicializa as variáveis do jogador e carrega os sprites.
/// @param player Ponteiro para a struct Player
void InitPlayer(Player *player)
{
    // Carregamento dos sprites do player
    player->spritePlayerRun  = LoadTexture("resources/sprites/player/RUN.png");
    player->spritePlayerWalk = LoadTexture("resources/sprites/player/WALK.png");
    player->spritePlayerIdle = LoadTexture("resources/sprites/player/IDLE.png");
    player->spritePlayerJump = LoadTexture("resources/sprites/player/JUMP.png");
    player->spritePlayerAttack1 = LoadTexture("resources/sprites/player/ATTACK 1.png");
    player->spritePlayerAttack2 = LoadTexture("resources/sprites/player/ATTACK 2.png");

    // Quantidade de frames de cada animação
    player->frameRun  = 8;
    player->frameWalk = 8;
    player->frameIdle = 7;
    player->frameJump = 5;
    player->frameAtk = 6;

    // Tamanho dos frames baseado no sprite WALK (todos seguem o mesmo padrão)
    player->frameWidth  = player->spritePlayerWalk.width / 8;
    player->frameHeight = player->spritePlayerWalk.height;

    // Posição inicial
    player->position = (Vector2){120, 518};

    // Configurações físicas
    player->speedWalk = 200.0f;
    player->speedRun  = 280.0f;
    player->gravity   = 950.0f;
    player->groundY   = 518.0f;
    player->velocityY = 0.0f;

    // Direção inicial (1 = direita, -1 = esquerda)
    player->direction = 1.0f;

    // Controle de animação
    player->currentFrame = 0;
    player->frameCounter = 0;

    // Estados
    player->isRunning = false;
    player->isMoving  = false;
    player->isJumping = false;
    player->isAttacking = false;

    // Stamina inicial
    player->stamina = 150;

    player->life = 200;
}

/// @brief Atualiza o estado do jogador (movimento, física e animação).
/// @param player Ponteiro para a struct Player
/// @param delta Delta time (tempo entre frames)
void UpdatePlayer(Player *player, Wolf *wolf, float delta)
{
    // Verificar estado de movimento
    bool isRunning = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && IsKeyDown(KEY_LEFT_SHIFT) && player->stamina > 0);
    bool isWalking = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && (!IsKeyDown(KEY_LEFT_SHIFT) || player->stamina <= 0));
    bool isAttacking = ((IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) && player->stamina > 0);

    player->isRunning = isRunning;
    player->isAttacking = isAttacking;
    player->isMoving  = isRunning || isWalking;

    

    // Movimento horizontal com controle de direção
    if (isRunning)
    {
        if (IsKeyDown(KEY_D))
        {
            player->direction = 1.0f;
            player->position.x += player->speedRun * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            player->direction = -1.0f;
            player->position.x -= player->speedRun * delta;
        }
    }
    else if (isWalking)
    {
        if (IsKeyDown(KEY_D))
        {
            player->direction = 1.0f;
            player->position.x += player->speedWalk * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            player->direction = -1.0f;
            player->position.x -= player->speedWalk * delta;
        }
    }

    // Controle de animação (10 frames por segundo)
    player->frameCounter++;
    if (player->frameCounter >= (60 / 10))
    {
        player->frameCounter = 0;

        if (player->isJumping)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameJump;
        }
        else if (player->isAttacking) 
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                player->currentFrame = (player->currentFrame + 1) % player->frameAtk;
            } 
            else 
            {
                player->currentFrame = (player->currentFrame + 1) % player->frameAtk;
            } 
        }
        else if (player->isRunning)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameRun;
        }
        else if (player->isMoving)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameWalk;
        }
        else
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameIdle;
        }
    }

    // Pulo (apertar espaço)
    if (IsKeyDown(KEY_SPACE) && !player->isJumping)
    {
        player->velocityY = -400.0f;
        player->isJumping = true;
            if (player->stamina <= 0)
            {
                player->velocityY = 0.0f;
            }
    }

    // Aplicar gravidade
    player->velocityY += player->gravity * delta;
    player->position.y += player->velocityY * delta;

    // Verificar se está no chão
    if (player->position.y >= player->groundY)
    {
        player->position.y = player->groundY;
        player->velocityY = 0;
        player->isJumping = false;
    }

    // Limites da tela
    if (player->position.x < 0) player->position.x = 0;
    if (player->position.y < 0) player->position.y = 0;

    
}

/// @brief Desenha o jogador na tela com base no estado atual.
/// @param player Ponteiro para a struct Player
void DrawPlayer(Player *player)
{
    Rectangle source;      // Parte da textura a ser desenhada
    Rectangle dest;        // Onde será desenhado na tela
    Vector2 origin = {0, 0};

    int frameWidth;
    Texture2D texture;

    // Seleciona o sprite com base no estado
    if (player->isJumping)
    {
        texture = player->spritePlayerJump;
        frameWidth = texture.width / player->frameJump;
    }
    else if (player->isAttacking)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            texture = player->spritePlayerAttack1;
            frameWidth = texture.width / player->frameAtk;
        } 
        else
        {
            texture = player->spritePlayerAttack2;
            frameWidth = texture.width / player->frameAtk;
        }   
    }
    else if (!player->isMoving)
    {
        texture = player->spritePlayerIdle;
        frameWidth = texture.width / player->frameIdle;
    }
    else if (player->isRunning)
    {
        texture = player->spritePlayerRun;
        frameWidth = texture.width / player->frameRun;
    }
     else {
        texture = player->spritePlayerWalk;
        frameWidth = texture.width / player->frameWalk;
    }

    // Retângulo de origem (flip horizontal usando direção)
    source = (Rectangle){
        player->currentFrame * frameWidth, // X do frame
        0,                                 // Y (sempre zero)
        frameWidth * player->direction,    // Flip horizontal se direção = -1
        player->frameHeight                // Altura
    };

    // Retângulo de destino (na tela com escala 2x)
    dest = (Rectangle){
        player->position.x,                // Posição X
        player->position.y,                // Posição Y
        frameWidth * 2,                    // Largura
        player->frameHeight * 2            // Altura
    };

    // Desenha o sprite na tela
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
}

/// @brief Libera da memória as texturas do player.
/// @param player Ponteiro para a struct Player
void UnloadPlayer(Player *player)
{
    UnloadTexture(player->spritePlayerRun);
    UnloadTexture(player->spritePlayerWalk);
    UnloadTexture(player->spritePlayerIdle);
    UnloadTexture(player->spritePlayerJump);
    UnloadTexture(player->spritePlayerAttack1);
}

void CheckCollisionWolf(Player *player, Wolf *wolf, float delta) 
{
    if (player->position.x < wolf->position.x + wolf->frameWidth &&
        player->position.x + player->frameWidth > wolf->position.x &&
        player->position.y < wolf->position.y + wolf->frameHeight &&
        player->position.y + player->frameHeight > wolf->position.y
    )
    {
        player->position.x -= 140;
    }
}
