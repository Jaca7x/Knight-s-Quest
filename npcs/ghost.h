#ifndef GHOST_H
#define GHOST_H

#include "librays/raylib.h"
#include "player/player.h"
#include "interaction.h"

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

// Ghost NPC structure
typedef struct ghost {
    Vector2 position;
    Texture2D ghostSpeech;
    Texture2D ghostIdle;
    Texture2D ghostBtnE;
    Texture2D ghostExclamation;

    int frameIdle;

    bool isInteraction;

    int frameWidth;   
    int frameHeight; 
    int currentFrame; 
    int frameCounter; 

    Font textFont; 
} Ghost;

void InitGhost(Ghost *ghost);
void UpdateGhost(Ghost *ghost, Player *player, float delta, DialogStateGhost *dialogStateGhost, float *dialogoTimer);
void DrawGhost(Ghost *ghost, Player *player, DialogStateGhost dialogStateGhost, Interaction *interaction);
void UnloadGhost(Ghost *ghost);

#endif // GHOST_H