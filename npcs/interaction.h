#ifndef INTERACTION_H
#define INTERACTION_H

#include "../librays/raylib.h"
#include "npcs/ghost.h"
#include "npc.h"
#include "player/player.h"

typedef struct ghost Ghost;
typedef struct player Player;
typedef struct npc Npc;

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
void DrawInteractionNPC(Npc *npc, Interaction *interaction);

#endif //INTERACTION_H
