#ifndef GHOST_H
#define GHOST_H

#include "librays/raylib.h"

// Ghost NPC structure
typedef struct ghost {
    Vector2 position;
    Texture2D ghostSppech;
    Texture2D ghostIdle;
    Texture2D ghostBtnE;

    int frameIdle;

    int frameWidth;   
    int frameHeight;  
    int currentFrame; 
    int frameCounter; 
} Ghost;

void InitGhost(Ghost *ghost);
void UpdateGhost(Ghost *ghost, float delta);
void DrawGhost(const Ghost *ghost);
void UnloadGhost(Ghost *ghost);

#endif // GHOST_H