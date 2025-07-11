#ifndef GOBLINARCHER_H
#define GOBLINARCHER_H

#include "../librays/raylib.h"
#include "../player/player.h"

#define MAP_GOBLIN_ARCHER_AREA 0

typedef struct player Player; // declaração antecipada

typedef struct Arrow {
    Vector2 position;
    float speed;
    int direction;     // -1 = esquerda, 1 = direita
    bool active;
} Arrow;

typedef struct GoblinArcher
{
    // Posição e movimento
    Vector2 position;
    float speed;
    int direction;     // -1 = esquerda, 1 = direita

    // Vida
    int life;

    // Animação
    int currentFrame;
    int frameCounter;
    int frameWidth;
    int frameHeight;

    int frameWalk;
    int frameAtk;
    int frameIdle;
    int frameDead;
    int frameHurt;

    Texture2D spriteWalkGoblinArcher;
    Texture2D spriteAtkGoblinArcher;
    Texture2D spriteIdleGoblinArcher;
    Texture2D spriteDeadGoblinArcher;
    Texture2D spriteHurtGoblinArcher;

    // Estado
    bool isWalking;
    bool isRunning;
    bool isIdle;
    bool isAtacking;
    bool isDead;
    bool goblinHasHit;
    bool hasHitPlayer;

    float deathAnimTimer;
    bool deathAnimationDone;

    // Ataque (flecha)
    Texture2D arrowTexture;
    Arrow arrow;
    float arrowSpeed;
    int arrowTolerance;
    int arrowDamage;
    bool arrowFired;            // indica se flecha foi disparada
    float attackCooldown;
    float attackDamageTimer;
    float goblinView;           // distância para enxergar o player
    float attackRange;          // distância para atacar

} GoblinArcher;

// Funções
void InitGoblinArcher(GoblinArcher *goblinArcher);
void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta);
void DrawGoblinArcher(GoblinArcher *goblinArcher);
void UnloadGoblinArcher(GoblinArcher *goblinArcher);

#endif // GOBLINARCHER_H
