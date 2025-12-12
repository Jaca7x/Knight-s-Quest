#ifndef PLAYER_H
#define PLAYER_H
#define NUM_MAPS 4
#define DIALOGS_PER_MAP 3

#include "../librays/raylib.h"       
#include "../wolf/wolf.h"            
#include "../wolf/wolfRunning.h" 
#include "../goblin/goblin.h" 
#include "../goblin/goblinArcher.h" 
#include "../npcs/npc.h"   
#include "../boss/boss.h"

void PlayPlayerSound(Player *player, int currentMapIndex, int dialogueIndex);

typedef struct wolf Wolf; 
typedef struct wolfRun WolfRun; 
typedef struct goblin Goblin; 
typedef struct goblinArcher GoblinArcher;
typedef struct npc Npc;
typedef struct boss Boss;

typedef struct {
    const char *text;
    Sound sound;
} DialoguePlayer;


typedef struct player
{
    Vector2 position;

    Sound attackLightSound;
    Sound attackHeavySound;
    Sound jumpSound;

    Sound playerDialogueWithNPC1;
    Sound playerDialogueWithNPC2;

    DialoguePlayer dialogues[NUM_MAPS][6];

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
    Npc *npc, Boss *boss);
void DrawPlayer(Player *player);
void UnloadPlayer(Player *player);

Rectangle GetPlayerHitbox(Player *player);

#endif // PLAYER_H