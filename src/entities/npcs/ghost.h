#ifndef GHOST_H
#define GHOST_H

#include "raylib.h"
#include "entities/player/player.h"
#include "npc.h"
#include "interaction.h"
#include "core/define.h"

typedef struct npc Npc;
typedef struct interaction Interaction;

// MAP
#define START_MAP 1

// OFFSETS
#define GHOST_INTERACTION_OFFSET_X 15
#define GHOST_INTERACTION_OFFSET_Y -35
#define GHOST_EXCLAMATION_OFFSET 25

// DIALOG
#define GHOST_NUM_MAPS 4

typedef enum {
    DIALOG_CLOSED_GHOST,
    DIALOG_PLAYER_GHOST_TALKING,
    DIALOG_GHOST_TALKING,
    DIALOG_PLAYER_GHOST_TALKING2,
    DIALOG_GHOST_TALKING2,
    DIALOG_PLAYER_GHOST_TALKING3,
    DIALOG_GHOST_TALKING3
} DialogStateGhost;

typedef struct {
    Sound sound;
} Dialogue;

typedef struct ghost {
    Vector2 position;

    Texture2D ghostSpeech;
    Texture2D ghostIdle;
    Texture2D ghostBtnE;
    Texture2D ghostExclamation;

    Dialogue dialogues[GHOST_NUM_MAPS][TOTAL_DIALOGUES_PER_MAP];

    bool isInteraction;

    int frameIdle;

    int frameWidth;   
    int frameHeight; 
    float scale;

    int currentFrame; 
    int frameCounter; 

    Font textFont; 
    int speechFontSize;
} Ghost;

void InitGhost(Ghost *ghost);
void UpdateGhost(Ghost *ghost, Player *player, float delta, Interaction *interaction, DialogStateGhost *dialogStateGhost, float *dialogoTimer, int currentMapIndex);
void DrawGhost(Ghost *ghost, Player *player, DialogStateGhost dialogStateGhost, Interaction *interaction, int currentMapIndex);
void MapsGhost(Ghost *ghost, Player *player, DialogStateGhost dialogStateGhost, Interaction *interaction, float delta, int currentMapIndex);
void UnloadGhost(Ghost *ghost);

#endif