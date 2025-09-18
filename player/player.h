#ifndef PLAYER_H
#define PLAYER_H

#include "../librays/raylib.h"       // Biblioteca Raylib
#include "../wolf/wolf.h"            // Biblioteca wolf
#include "../wolf/wolfRunning.h" 
#include "../goblin/goblin.h" 
#include "../goblin/goblinArcher.h" // Biblioteca goblinArcher
#include "../npcs/npc.h"         // Biblioteca heart

typedef struct wolf Wolf; 
typedef struct wolfRun WolfRun; 
typedef struct goblin Goblin; 
typedef struct goblinArcher GoblinArcher;
typedef struct npc Npc;

/// @brief Struct que representa o jogador
typedef struct player
{
    // Posição
    Vector2 position;

    // Velocidades
    float speedWalk;       // Velocidade andando
    float speedRun;        // Velocidade correndo

    // Animação
    int currentFrame;      // Frame atual da animação
    int frameCounter;      // Contador para trocar frames
    int frameWidth;        // Largura de cada frame
    int frameHeight;       // Altura de cada frame

    // Texturas (sprites)
    Texture2D spritePlayerRun;
    Texture2D spritePlayerWalk;
    Texture2D spritePlayerIdle;
    Texture2D spritePlayerJump;
    Texture2D spritePlayerAttack1;
    Texture2D spritePlayerAttack2;
    Texture2D spritePlayerHurt;
    Texture2D spritePlayerDead;
    Texture2D playerSpeech;

    // Estados
    bool isRunning;
    bool isMoving;
    bool isJumping;
    bool isAttacking;
    bool isAttackingLight;
    bool isAttackingHeavy;
    bool hasHit;
    bool isDead;
    bool deathAnimationDone;     // Concluiu animação de morte
    bool isAttackingInProgress; // Controle do ataque

    // Timers
    float attackCooldownTimer;
    float attackTimer;
    float hitTimer;
    float deathAnimTimer;       // Timer da animação de morte

    // Atributos
    float stamina;
    float life;
    float attackRange;
    int lightDamage;
    int heavyDamage;

    // Frames por animação
    int frameRun;
    int frameWalk;
    int frameIdle;
    int frameJump;
    int frameAtk;
    int frameHurt;
    int frameDead;

    // Física
    float groundY;
    float gravity;
    float velocityY;

    // Direção (1 = direita, -1 = esquerda)
    float direction;

} Player;

/// @brief Inicializa os dados e texturas do jogador
void InitPlayer(Player *player);

/// @brief Atualiza a física, movimento e animação do jogador
void UpdatePlayer(Player *player, Wolf *wolf, WolfRun *wolfRun, Wolf *redWolf, Wolf *whiteWolf, Goblin *goblin, Goblin *redGoblin, GoblinArcher *goblinArcher, int currentMapIndex, float delta, Npc *npc);

/// @brief Desenha o jogador na tela
void DrawPlayer(Player *player);

/// @brief Descarrega as texturas do jogador da memória
void UnloadPlayer(Player *player);

Rectangle GetPlayerHitbox(Player *player);

#endif // PLAYER_H
