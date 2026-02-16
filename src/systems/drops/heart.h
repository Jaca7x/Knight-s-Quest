#ifndef HEART_H
#define HEART_H

#include "raylib.h"
#include "entities/player/player.h" 
#include "entities/wolf/wolf.h" 
#include "entities/goblin/goblin.h" 
#include "entities/goblin/goblinArcher.h" 
#include "entities/wolf/wolfRunning.h" 
#include "entities/goblin/goblinTank.h"
#include "entities/goblin/bombGoblin.h"
#include "core/define.h"


// MAXIMUM NUMBER OF HEARTS SPAWNED
#define MAX_HEARTS 9

// FONT
#define FONT_SIZE 30.0f

// OFFSETS
#define HEART_COLLECTION_TEXT_OFFSET_X -50.0f
#define HEART_SPAWN_WOLF_OFFSET_X 50.0f
#define HEART_SPAWN_WOLF_OFFSET_Y 100.0f

#define GOBLIN_TANK_HEART_OFFSET_X   130.0f
#define GOBLIN_TANK_HEART_OFFSET_Y    60.0f

#define BOMB_GOBLIN_HEART_OFFSET_X    80.0f
#define BOMB_GOBLIN_HEART_OFFSET_Y   130.0f

#define HEART_DRAW_OFFSET_X     30.0f
#define HEART_DRAW_OFFSET_Y     30.0f
#define HEART_DRAW_SCALE        2.0f
#define HEART_DRAW_ROTATION     0.0f

// HEALING VALUE
#define VALUE_OF_THE_HEART 25

// DROP SPAWN CHANCE
#define MAX_SPAWN_CHANCE 100
#define MIN_SPAWN_CHANCE 0

#define GOBLIN_SPAWNING_DROP_CHANCE 70
#define WOLF_SPAWNING_DROP_CHANCE 60

typedef struct player Player;
typedef struct wolf Wolf;
typedef struct goblin Goblin;
typedef struct goblinArcher GoblinArcher;
typedef struct wolfRun WolfRun;
typedef struct goblinTank GoblinTank;
typedef struct bombGoblin BombGoblin;

typedef struct heart
{
    Vector2 position;   

    Texture2D texture; 

    bool isActive;    

    int healthValue; 

    Sound collectSound;
    Sound spawnSound;
    
} Heart;

void InitHearts(Heart hearts[]);
void UpdateHearts(Heart hearts[], float delta, Player *player, Wolf *wolf, 
                    Wolf *redWolf, Wolf *whiteWolf, Goblin *goblin, 
                    Goblin *redGoblin, GoblinArcher *goblinArcher, 
                    WolfRun *wolfRun, GoblinTank *goblinTank, 
                    BombGoblin *bombGoblin);
void DrawHearts(const Heart hearts[], float delta, Player *player);
void UnloadHearts(Heart hearts[]);

#endif // HEART_H
