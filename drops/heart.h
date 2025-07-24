#ifndef HEART_H
#define HEART_H

#include "../librays/raylib.h" // Biblioteca Raylib
#include "../player/player.h" // Biblioteca player
#include "../wolf/wolf.h" // Biblioteca wolf
#include "../goblin/goblin.h" // Biblioteca goblin
#include "../goblin/goblinArcher.h" // Biblioteca goblinArcher
#include "../wolf/wolfRunning.h" // Biblioteca wolfRunning

#define MAX_HEARTS 4

typedef struct player Player;
typedef struct wolf Wolf;
typedef struct goblin Goblin;
typedef struct goblinArcher GoblinArcher;
typedef struct wolfRun WolfRun;

/// @brief Struct que representa um coração
typedef struct heart
{
    Vector2 position;      // Posição do coração
    Texture2D texture;     // Textura do coração
    bool isActive;         // Indica se o coração está ativo
    int healthValue;       // Valor de vida que o coração fornece
} Heart;

/// @brief Inicializa todos os corações
void InitHearts(Heart hearts[]);

/// @brief Atualiza os corações com base nos inimigos mortos
void UpdateHearts(Heart hearts[], float delta, Player *player, 
                  Wolf *wolf, Goblin *goblin, 
                  GoblinArcher *goblinArcher, WolfRun *wolfRun);

/// @brief Desenha todos os corações ativos
void DrawHearts(const Heart hearts[]);
    
/// @brief Libera a textura de todos os corações
void UnloadHearts(Heart hearts[]);

#endif // HEART_H
