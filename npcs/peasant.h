#ifndef PEASANT_H
#define PEASANT_H
#define NUM_MAPS_PEASANT 3

#include "../librays/raylib.h"
#include "../player/player.h"
#include "interaction.h"

typedef struct interaction Interaction;

typedef enum {
    DIALOG_CLOSED_PEASANT,
    DIALOG_PLAYER_PEASANT_TALKING,
    DIALOG_PEASANT_TALKING
} DialogStatePeasant;

typedef struct {
    const char *text;
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
} Peasant;

void InitPeasant(Peasant *peasant);
void UpdatePeasant(Peasant *peasant, Player *player, float deltaTime, Interaction *interaction, DialogStatePeasant *dialogStatePeasant, float *dialogoTimerPeasant, int currentMapIndex);
void DrawPeasant(Peasant *peasant, Interaction *interaction);
void DrawMapsPeasant(Peasant *peasant, Interaction *interaction, int currentMapIndex, float delta);
void UnloadPeasant(Peasant *peasant);

#endif // PEASANT_H