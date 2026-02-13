#ifndef HEART_H
#define HEART_H

#include "../librays/raylib.h" 
#include "../player/player.h" 
#include "../wolf/wolf.h" 
#include "../goblin/goblin.h" 
#include "../goblin/goblinArcher.h" 
#include "../wolf/wolfRunning.h" 
#include "../goblin/goblinTank.h"
#include "../goblin/bombGoblin.h"

#define MAX_HEARTS 9

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
                    WolfRun *wolfRun, GoblinTank *goblinTank, BombGoblin *bombGoblin) ;
void DrawHearts(const Heart hearts[], float delta, Player *player);
void UnloadHearts(Heart hearts[]);

#endif // HEART_H
