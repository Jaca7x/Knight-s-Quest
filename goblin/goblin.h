#ifndef GOBLIN_H
#define GOBLIN_H

#include "../librays/raylib.h"
#include "../player/player.h"

typedef struct player Player;

#define GOBLIN_MAP 0

typedef struct goblin {

    Vector2 position;

    Texture2D goblinSpriteWalk;
    Texture2D goblinSpriteHurt;
    Texture2D goblinSpriteDead;
    Texture2D goblinSpriteIdle;
    Texture2D goblinSpriteAtk;

    int frameWalk;
    int frameHurt;
    int frameDead;
    int frameIdle;
    int frameAtk;

    int currentFrame;      // Frame atual da animação
    int frameCounter;      // Contador de frames (controle de tempo)
    int frameWidth;        // Largusra de cada frame
    int frameHeight;       // Altura de cada frame
    float frameFactor;     // Fator de escala do frame
    
    bool isIdle;
    bool isWalking;
    bool isAtacking;
    bool isDead;
    bool goblinHasHit;
    bool deathAnimationDone;


    float deathAnimTimer; 
    float viewPlayer;
    float goblinAttackRangeRight;
    float goblinAttackRangeLeft;
    bool goblinHasHitPlayer;
    int direction;

    float attackTimer;
    float attackCooldown;

    float scale;
    
    int life;
    float speed;

    int damage;

    bool droppedHeart; // Indica se o coração foi solto

} Goblin;


void InitGoblin(Goblin *goblin);
void UpdateGoblin(Goblin *goblin, Player *player, int currentMapIndex, float delta);
void DrawGoblin(Goblin *goblin);
void UnloadGoblin(Goblin *goblin);

#endif // GOBLIN_H