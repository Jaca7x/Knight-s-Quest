#ifndef GHOST_H
#define GHOST_H
#define NUM_MAPS 4
#define DIALOGS_PER_MAP 3

#include "raylib.h"
#include "entities/player/player.h"
#include "npc.h"
#include "interaction.h"

typedef struct npc Npc;
typedef struct interaction Interaction;

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

    Dialogue dialogues[NUM_MAPS][6];

    int frameIdle;

    bool isInteraction;

    int frameWidth;   
    int frameHeight; 
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

#endif // GHOST_H