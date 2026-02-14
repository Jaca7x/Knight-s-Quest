#ifndef PEASANT_H
#define PEASANT_H
#define NUM_MAPS_PEASANT 3

#include "raylib.h"
#include "entities/player/player.h"
#include "interaction.h"

typedef struct interaction Interaction;

typedef enum {
    DIALOG_CLOSED_PEASANT,
    DIALOG_PLAYER_PEASANT_TALKING,
    DIALOG_PEASANT_TALKING, 
    DIALOG_PLAYER_PEASANT_TALKING2,
    DIALOG_PEASANT_TALKING2,
    DIALOG_PLAYER_PEASANT_TALKING3,
    DIALOG_PEASANT_TALKING3
} DialogStatePeasant;

typedef struct {
    Sound sound;
} DialoguePeasant;

typedef struct peasant {
    Vector2 position;
    Texture2D peasantSpeech;
    Texture2D peasantIdle;
    Texture2D peasantBtnE;
    Texture2D peasantExclamation;
    
    DialoguePeasant dialogues[NUM_MAPS_PEASANT][3];

    int frameIdle;

    bool isInteraction;

    int frameWidth;   
    int frameHeight; 
    int currentFrame; 
    int frameCounter; 

    Font textFont; 

    int speechFontSize;

    bool dialogueFinished;
} Peasant;

void PlayPeasantSound(Peasant *peasant, int currentMapIndex, int dialogueIndex);
void InitPeasant(Peasant *peasant);
void UpdatePeasant(Peasant *peasant, Player *player, float deltaTime, Interaction *interaction, DialogStatePeasant *dialogStatePeasant, float *dialogoTimerPeasant, int currentMapIndex, bool *bossTriggered);
void DrawPeasant(Peasant *peasant, Player *player, Interaction *interaction, int currentMapIndex, DialogStatePeasant *dialogStatePeasant);
void DrawMapsPeasant(Peasant *peasant, Player *player, Interaction *interaction, int currentMapIndex, float delta, DialogStatePeasant *dialogStatePeasant);
void UnloadPeasant(Peasant *peasant);

#endif // PEASANT_H