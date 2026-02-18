#ifndef NPC_H
#define NPC_H

#include "raylib.h"
#include "entities/player/player.h"
#include "interaction.h"
#include "raymath.h"
#include <string.h>

#define MAP_NPC 0

#define NPC_OFFSET_X_INTERACTION 5
#define NPC_OFFSET_Y_INTERACTION 40

#define NPC_OFFSET_X_EXCLAMATION 15
#define NPC_OFFSET_Y_EXCLAMATION 30

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

    Vector2 position;

    Texture2D spriteNpc;
    Texture2D spriteNpcIdle;
    Texture2D npcSpeech;
    Texture2D btnE;
    Texture2D exclamation;

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

} Npc;

void InitNpc(Npc *npc);
void UpdateNpc(Npc *npc, float deltaTime, Player *player, DialogState *dialogState, float *dialogoTimer);
void DrawNpc(Npc *npc, Player *player, Interaction *interaction, DialogState dialogState);
void UnloadNpc(Npc *npc);

#endif
