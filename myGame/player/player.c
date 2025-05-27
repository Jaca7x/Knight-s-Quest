#include "player.h" //player.h onde define a struct

void InitPlayer(Player *player) // Func pra inicializar o player
{
    player->spritePlayerRun = LoadTexture("resources/sprites/player/RUN.png");   // carrega o sprite de correr
    player->spritePlayerWalk = LoadTexture("resources/sprites/player/WALK.png"); // carrega o sprite de andar
    player->spritePlayerIdle = LoadTexture("resources/sprites/player/IDLE.png"); // carrega o sprite de ficar parado
    player->spritePlayerJump = LoadTexture("resources/sprites/player/JUMP.png"); // carrega o sprite de pular

    player->frameRun = 8;  // quantidade de frames quando está correndo (8)
    player->frameWalk = 8; // quantidade de frames quando está andando (8)
    player->frameIdle = 7; // quantidade de frames quando está parado (7)
    player->frameJump = 5; // quantidade de frames quando está pulando (5)

    player->frameWidth = player->spritePlayerWalk.width / 8; // calcula a largura dos frames com 8 frames (RUN e WALK)
    player->frameHeight = player->spritePlayerWalk.height;   // calcula a altura dos frames

    player->position = (Vector2){120, 520}; // posição x = 100 e y = 520 do player na tela
    player->speedWalk = 200.0f;             // velocidade quando está andando
    player->speedRun = 280.0f;              // velocidade quiando está correndo
    player->speedWalkBack = 80.0f;          // velocidade quando está andando pra tras
    player->gravity = 800.0f;               // gravidade
    player->groundY = 520.0f;               // posição y do chão
    player->velocityY = 0.0f;               // velocidade vertical do player

    player->currentFrame = 0;  // o frame atual sempre começa em 0
    player->frameCounter = 0;  // o contador de frames começa em 0
    player->isRunning = false; // o player começa parado ent ñ está correndo
    player->isMoving = false;  // o player começa parado ent ñ está se movendo
    player->isJumping = false; // o player começa parado então ñ está pulando
    player->stamina = 150;     // define a stamina com 150
}

void UpdatePlayer(Player *player, float delta) // Func pra atualizar o player
{
    bool isRunning = (IsKeyDown(KEY_D) && IsKeyDown(KEY_LEFT_SHIFT) && player->stamina > 0);                           // verifica se esta correndo
    bool isWalking = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && (!IsKeyDown(KEY_LEFT_SHIFT) || player->stamina <= 0)); // verifica se esta andando

    player->isRunning = isRunning;
    player->isMoving = isRunning || isWalking; // verifica se está se movendo

    if (isRunning)
    {
        if (IsKeyDown(KEY_D))
        {
            player->position.x += player->speedRun * delta;
        }
    } // <- essa chave estava faltando aqui!!!
    else if (isWalking)
    {
        if (IsKeyDown(KEY_D))
        {
            player->position.x += player->speedWalk * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            player->position.x -= player->speedWalkBack * delta;
        }
    }

    player->frameCounter++;
    if (player->frameCounter >= (60 / 10)) // 10 fps
    {
        player->frameCounter = 0;
        // Calcula a quantidade de frames de cada sprite

        if (player->isJumping)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameJump;
        }
        if (player->isRunning)
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

    // Pulando
    if (IsKeyPressed(KEY_SPACE) && !player->isJumping)
    {
        player->velocityY = -400.0f;
        player->isJumping = true;
    }

    player->velocityY += player->gravity * delta;
    player->position.y += player->velocityY * delta;

    if (player->position.y >= player->groundY)
    {
        player->position.y = player->groundY;
        player->velocityY = 0;
        player->isJumping = false;
    }

    // Colisão com a bordas da janela (apenas 2 lados)
    if (player->position.x < 0)
        player->position.x = 0;
    if (player->position.y < 0)
        player->position.y = 0;
}

void DrawPlayer(Player *player) // Func para desenhar o player
{
    Rectangle source;
    Rectangle dest = {// Rectangle dest que faz com o player receba sua position e tamanho * 2
                      player->position.x,
                      player->position.y,
                      player->frameWidth * 2,
                      player->frameHeight * 2
                    };
    Vector2 origin = {0, 0}; // origin canto superior esquerdo

    if (player->isJumping == true) // Se o player estiver correndo
    {
        int jumpFrameWidth = player->spritePlayerJump.width / player->frameJump;
        source = (Rectangle){// Rectangle source usando frameWidth para 8 frames
                             player->currentFrame * jumpFrameWidth,
                             0,
                             jumpFrameWidth,
                             player->frameHeight
                            };
        DrawTexturePro(player->spritePlayerJump, source, dest, origin, 0.0f, WHITE); // Desenha o sprite quando está correndo
    }
    else if (!player->isMoving) // Se o player ñ está se movendo
    {
        int idleFrameWidth = player->spritePlayerIdle.width / player->frameIdle; // Calcula a largura do frame diferente pois são 7 frames nã 8

        source = (Rectangle){// Cria um rectangle source diferente utilizando o idleFrameWidth ao inves do frameWidth
                             player->currentFrame * idleFrameWidth,
                             0,
                             idleFrameWidth,
                             player->frameHeight
                            };
        DrawTexturePro(player->spritePlayerIdle, source, dest, origin, 0.0f, WHITE); // Desenha o sprite quando está parado
    }
    else if (player->isRunning) // Se o player estiver correndo
    {
        source = (Rectangle){// Rectangle source usando frameWidth para 8 frames
                             player->currentFrame * player->frameWidth,
                             0,
                             player->frameWidth,
                             player->frameHeight
                            };
        DrawTexturePro(player->spritePlayerRun, source, dest, origin, 0.0f, WHITE); // Desenha o sprite quando está correndo
    }
    else // Se ñ o player está andando então
    {
        source = (Rectangle){// Rectangle source usando frameWidth para 8 frames
                             player->currentFrame * player->frameWidth,
                             0,
                             player->frameWidth,
                             player->frameHeight
                            };
        DrawTexturePro(player->spritePlayerWalk, source, dest, origin, 0.0f, WHITE); // Desenha o sprite quando está correndo
    }
}

void UnloadPlayer(Player *player) // Func para descarregar os sprites
{
    UnloadTexture(player->spritePlayerRun);  // Descarrega o sprie de corrida
    UnloadTexture(player->spritePlayerWalk); // Descarrega o sprite de quando está andando
    UnloadTexture(player->spritePlayerIdle); // Descarrega o sprie de quando está parado
    UnloadTexture(player->spritePlayerJump); // Descarrega o sprie de quando está pulando
}
