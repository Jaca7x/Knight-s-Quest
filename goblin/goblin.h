#ifndef GOBLIN_H
#define GOBLIN_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "../src/render/drawMonsters.h"

typedef struct player Player;

#define OFFSET_ZERO 0
#define GOBLIN_MAP 1
#define RED_GOBLIN_MAP 3

typedef struct goblin {

    Entity entity;
    Monsters base;

    int frameWalk;
    int frameHurt;
    int frameDead;
    int frameIdle;
    int frameAtk;
      
    int frameCounter;            
    
    bool deathAnimationDone;


    float deathAnimTimer; 
    float viewPlayer;
    float goblinAttackRangeRight;
    float goblinAttackRangeLeft;
    bool goblinHasHitPlayer;

    float hurtTimer;
    float hurtDuration;

    float attackTimer;
    float attackCooldown;

    float scaleHitbox;
    
    int life;
    float maxLife;
    float speed;
    float baseSpeed; 

    int damage;

    bool droppedHeart; 

    Sound goblinDeathSound;
    Sound goblinCutSound;
    Sound RedGoblinHitSound;

} Goblin;


void InitGoblinBase(Goblin *goblin, Vector2 pos);

void InitRedGoblin(Goblin *goblin, Vector2 pos);

void UpdateGoblin(Goblin *goblin, Player *player, int currentMapIndex, float delta);
void DrawGoblin(Goblin *goblin);
void UnloadGoblin(Goblin *goblin);

#endif // GOBLIN_H