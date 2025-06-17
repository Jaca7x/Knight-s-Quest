#ifndef GOBLIN_H
#define GOBLIN_H

#include "../librays/raylib.h"

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

    bool isIdle;
    bool isWalking;
    bool isAtacking;

    int direction;

    int life;

    int damage;

} Goblin;


void InitGoblin(Goblin *goblin);
void UpdateGoblin(Goblin *goblin, float delta);
void DrawGoblin(Goblin *goblin);
void UnloadGoblin(Goblin *goblin);

#endif // GOBLIN_H