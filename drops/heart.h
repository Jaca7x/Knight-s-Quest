#ifndef HEART_H
#define HEART_H

#include "../librays/raylib.h" // Biblioteca Raylib
#include "../player/player.h" // Biblioteca player
#include "../wolf/wolf.h" // Biblioteca wolf
#include "../goblin/goblin.h" // Biblioteca goblin
#include "../goblin/goblinArcher.h" // Biblioteca goblinArcher
#include "../wolf/wolfRunning.h" // Biblioteca wolfRunning

typedef struct player Player;
typedef struct wolf Wolf;
typedef struct goblin Goblin;
typedef struct goblinArcher GoblinArcher;
typedef struct wolfRun WolfRun;

/// @brief Struct que representa um coração
typedef struct heart
{
    Vector2 position;          // Posição do coração
    Texture2D texture;        // Textura do coração
    bool isActive;            // Indica se o coração está ativo
    int healthValue;       // Valor de vida que o coração representa
} Heart;

void initHeart(Heart *heart);
void updateHeart(Heart *heart, float deltaTime, Player *player, 
                 Wolf *wolf, Goblin *goblin, 
                 GoblinArcher *goblinArcher, WolfRun *wolfRun);
void drawHeart(const Heart *heart);
void unloadHeart(Heart *heart);

#endif // HEART_H
