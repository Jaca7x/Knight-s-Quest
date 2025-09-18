#ifndef GOBLIN_H
#define GOBLIN_H

#include "../librays/raylib.h"
#include "../player/player.h"

typedef struct player Player;

#define GOBLIN_MAP 1
#define RED_GOBLIN_MAP 3

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

    int currentFrame;      
    int frameCounter;      
    int frameWidth;       
    int frameHeight;       
    float frameFactor;     
    
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

    float hurtTimer;
    float hurtDuration;

    float attackTimer;
    float attackCooldown;

    float scale;
    
    int life;
    int maxLife;
    float speed;
    float baseSpeed; 

    int damage;

    bool droppedHeart; 

} Goblin;


void InitGoblinBase(Goblin *goblin, Vector2 pos);

void initRedGoblin(Goblin *goblin, Vector2 pos);

void UpdateGoblin(Goblin *goblin, Player *player, int currentMapIndex, float delta);
void DrawGoblin(Goblin *goblin);
void UnloadGoblin(Goblin *goblin);

#endif // GOBLIN_H