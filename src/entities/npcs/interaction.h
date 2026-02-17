#ifndef INTERACTION_H
#define INTERACTION_H

#include "raylib.h"
#include "ghost.h"
#include "npc.h"
#include "peasant.h"
#include "entities/player/player.h"
#include "core/define.h"

typedef struct ghost Ghost;
typedef struct player Player;
typedef struct npc Npc;
typedef struct peasant Peasant;

//FRAME
#define INTERACTION_FRAME_SPEED (100 / 5)

//OFFSETS
#define GHOST_INTERACTION_OFFSET 20

#define PEASANT_INTERACTION_OFFSET_X 18
#define PEASANT_INTERACTION_OFFSET_Y -22

#define NPC_INTERACTION_OFFSET_X 12
#define NPC_INTERACTION_OFFSET_Y -25

typedef struct interaction 
{
    int currentFrame; 
    int frameCounter; 

    int columns;
    int rows;

    int frameWidth;
    int frameHeight;

    int totalFrames;

    Texture2D interactionTexture;
} Interaction;

void InitInteraction(Interaction *interaction);
void UpdateInteraction(Interaction *interaction, float delta);
void DrawInteractionGhost(Ghost *ghost, Interaction *interaction);
void DrawInteractionPeasant(Peasant *peasant, Interaction *interaction);
void DrawInteractionNPC(Npc *npc, Interaction *interaction);

#endif 
