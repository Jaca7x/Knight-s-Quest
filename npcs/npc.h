#ifndef NPC_H
#define NPC_H

#include "../lib/raylib.h"
#include "../player/player.h"
#include "interaction.h"
#include "../lib/raymath.h"
#include <string.h>

#define MAP_NPC 0

typedef struct Player player;
typedef struct interaction Interaction;


typedef enum {
    DIALOG_CLOSED,
    DIALOG_NPC_TALKING,
    DIALOG_PLAYER_TALKING,
    DIALOG_NPC_TALKING2,
    DIALOG_PLAYER_TALKING2,
} DialogState;

typedef struct npc {
    Texture2D spriteNpc; 
    Texture2D btnE;     
    Texture2D spriteNpcIdle;
    Texture2D npcSpeech; 
    Texture2D exclamation;
    Vector2 position;   

    Sound dialogueWithPlayer1;
    Sound dialogueWithPlayer2;
    
    Font textSpeech;

    bool showExclamation;
    
    int frameTalking;
    int frameIdle;

    int frameWidth;   
    int frameHeight;  
    int currentFrame; 
    int frameCounter; 

    bool isTalking;   
    bool isIdle;      
    
} Npc;

void InitNpc(Npc *npc);
void UpdateNpc(Npc *npc, float deltaTime, Player *player, DialogState *dialogState, float *dialogoTimer);
void DrawNpc(Npc *npc, Player *player, Interaction *interaction, DialogState dialogState);
void UnloadNpc(Npc *npc);

#endif // NPC_H