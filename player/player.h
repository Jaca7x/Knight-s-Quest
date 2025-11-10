#ifndef PLAYER_H
#define PLAYER_H

#include "../librays/raylib.h"       
#include "../wolf/wolf.h"            
#include "../wolf/wolfRunning.h" 
#include "../goblin/goblin.h" 
#include "../goblin/goblinArcher.h" 
#include "../npcs/npc.h"    

typedef struct wolf Wolf; 
typedef struct wolfRun WolfRun; 
typedef struct goblin Goblin; 
typedef struct goblinArcher GoblinArcher;
typedef struct npc Npc;

typedef struct player
{
    Vector2 position;

    Sound attackLightSound;
    Sound attackHeavySound;
    Sound jumpSound;

    Sound playerDialogueWithNPC1;
    Sound playerDialogueWithNPC2;

    bool attackSoundPlayed;
    bool jumpSoundPlayed;
    bool walkSoundPlayingCastle;
    bool walkSoundPlayingGrass;
    float speedWalk;       
    float speedRun;       

    int currentFrame;     
    int frameCounter;      
    int frameWidth;        
    int frameHeight;       

    Texture2D spritePlayerRun;
    Texture2D spritePlayerWalk;
    Texture2D spritePlayerIdle;
    Texture2D spritePlayerJump;
    Texture2D spritePlayerAttack1;
    Texture2D spritePlayerAttack2;
    Texture2D spritePlayerHurt;
    Texture2D spritePlayerDead;
    Texture2D playerSpeech;

    bool isRunning;
    bool isMoving;
    bool isJumping;
    bool isAttacking;
    bool isAttackingLight;
    bool isAttackingHeavy;
    bool hasHit;
    bool isDead;
    bool deathAnimationDone;     
    bool isAttackingInProgress;

    float attackCooldownTimer;
    float attackTimer;
    float hitTimer;
    float deathAnimTimer;    
    float walkTimeCastle;  
    float walkTimeGrass; 

    float stamina;
    float life;
    float attackRange;
    int lightDamage;
    int heavyDamage;

    int frameRun;
    int frameWalk;
    int frameIdle;
    int frameJump;
    int frameAtk;
    int frameHurt;
    int frameDead;

    float groundY;
    float gravity;
    float velocityY;

    float direction;

} Player;


void InitPlayer(Player *player);
void UpdatePlayer(Player *player, Wolf *wolf, WolfRun *wolfRun, Wolf *redWolf, Wolf *whiteWolf,
    Goblin *goblin, Goblin *redGoblin, GoblinArcher *goblinArcher, int currentMapIndex, float delta, 
    Npc *npc);
void DrawPlayer(Player *player);
void UnloadPlayer(Player *player);

Rectangle GetPlayerHitbox(Player *player);

#endif // PLAYER_H