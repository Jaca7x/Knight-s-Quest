#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"      
#include "src/entities/wolf/wolf.h"            
#include "src/entities/wolf/runningWolf.h" 
#include "entities/goblin/goblin.h" 
#include "entities/goblin/goblinArcher.h" 
#include "entities/npcs/npc.h"   
#include "entities/boss/boss.h"
#include "entities/goblin/goblinTank.h"
#include "entities/goblin/bombGoblin.h"
#include "core/define.h"

// STATS
#define PLAYER_MAX_LIFE 100

#define MIN_STAMINA_FOR_LIGHT_ATTACK 30
#define MIN_STAMINA_FOR_HEAVY_ATTACK 45

#define PLAYER_STAMINA_SPENT_LIGHT_ATTACKS 30.0f
#define PLAYER_STAMINA_SPENT_HEAVY_ATTACKS 75.0f

// TIMER & COOLDOWN
#define PLAYER_ATTACK_TIMER 0.5f
#define PLAYER_ATTACK_COOLDOWN_TIMER 0.3f

// JUMP
#define JUMP_FORCE_HIGH -400.0f
#define JUMP_FORCE_MEDIUM -300.0f

#define JUMP_COST_HIGH 35.0f
#define JUMP_COST_MEDIUM 20.0f

#define JUMP_STAMINA_HIGH 40.0f
#define JUMP_STAMINA_MEDIUM 20.0f

#define JUMP_FORCE_NONE 0.0f

// OFFSETS
#define PLAYER_SCALE 2.0f
#define PLAYER_HITBOX_WIDTH_RATIO 0.3f
#define PLAYER_HITBOX_HEIGHT_RATIO 0.35f

void PlayPlayerSoundWithGhost(Player *player, int currentMapIndex, int dialogueIndex);
void PlayPlayerSoundWithPeasant(Player *player, int currentMapIndex, int dialogueIndex);

typedef struct wolf Wolf; 
typedef struct runningWolf RunningWolf; 
typedef struct goblin Goblin; 
typedef struct goblinArcher GoblinArcher;
typedef struct npc Npc;
typedef struct boss Boss;
typedef struct goblinTank GoblinTank;
typedef struct bombGoblin BombGoblin;

typedef struct {
    const char *text;
    Sound sound;
} DialoguePlayer;

typedef struct {
    const char *text;
    Sound sound;
} DialoguePlayerWithPeasant;

typedef struct player
{
    Vector2 position;

    Sound playerDialogueWithNPC1;
    Sound playerDialogueWithNPC2;

    DialoguePlayer dialogues[GHOST_NUM_MAPS][TOTAL_DIALOGUES_PER_MAP];
    DialoguePlayerWithPeasant dialoguesWithPeasant[PEASANT_NUM_MAPS][TOTAL_DIALOGUES_PER_MAP];

    bool attackSoundPlayed;
    bool jumpSoundPlayed;
    bool walkSoundPlayingCastle;
    bool walkSoundPlayingGrass;
    
    float speedWalk;       
    float speedRun;       

    int scale;
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

    int direction;

    Sound walkingInCastle;
    Sound walkingInGrass;
    Sound playerHurtSound;
    Sound death;
    Sound attackLightSound;
    Sound attackHeavySound;
    Sound jumpSound;

} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player, Wolf *wolf, RunningWolf *runningWolf, Wolf *redWolf, Wolf *whiteWolf,
    Goblin *goblin, Goblin *redGoblin, GoblinArcher *goblinArcher, int currentMapIndex, float delta, 
    Npc *npc, Boss *boss, GoblinTank *goblinTank, BombGoblin *bombGoblin);
void DrawPlayer(Player *player);
void UnloadPlayer(Player *player);

Rectangle GetPlayerHitbox(Player *player);

#endif 