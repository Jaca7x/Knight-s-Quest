#ifndef PLAYER_H
#define PLAYER_H

#include "../librays/raylib.h" // Biblioteca Raylib
#include "../wolf/wolf.h" // Biblioteca wolf

/// @brief Struct que representa o jogador
typedef struct player
{
    // Posição
    Vector2 position;

    // Velocidades
    float speedWalk;       // Velocidade andando
    float speedRun;        // Velocidade correndo
    float speedWalkBack;   // (Opcional) Velocidade andando para trás

    // Animação
    int currentFrame;      // Frame atual da animação
    int frameCounter;      // Contador de frames (controle de tempo)
    int frameWidth;        // Largura de cada frame
    int frameHeight;       // Altura de cada frame

    // Texturas (sprites)
    Texture2D spritePlayerRun;   // Sprite correndo
    Texture2D spritePlayerWalk;  // Sprite andando
    Texture2D spritePlayerIdle;  // Sprite parado
    Texture2D spritePlayerJump;  // Sprite pulando
    Texture2D spritePlayerAttack1; // Sprite atacando leve
    Texture2D spritePlayerAttack2; // Sprite atacando pesado

    // Estados
    bool isRunning;   // Está correndo?
    bool isMoving;    // Está se movendo?
    bool isJumping;   // Está pulando?
    bool isAttacking; // Está atacando?

    // Stamina
    float stamina;    // Energia para correr

    float life;

    // Frames por animação
    int frameRun;
    int frameWalk;
    int frameIdle;
    int frameJump;
    int frameAtk;

    // Física
    float groundY;    // Posição Y do chão
    float gravity;    // Gravidade
    float velocityY;  // Velocidade vertical

    // Direção (1 = direita, -1 = esquerda)
    float direction;

} Player;

/// @brief Inicializa os dados e texturas do jogador
void InitPlayer(Player *player);

/// @brief Atualiza a física, movimento e animação do jogador
/// @param delta Tempo entre os frames (GetFrameTime())
void UpdatePlayer(Player *player, Wolf *wolf, float delta);

/// @brief Desenha o jogador na tela
void DrawPlayer(Player *player);

/// @brief Descarrega as texturas do jogador da memória
void UnloadPlayer(Player *player);

/// @brief Verifica a colisão 
void CheckCollisionWolf(Player *player, Wolf *wolf, float delta);

#endif // PLAYER_H
