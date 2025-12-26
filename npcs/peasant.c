#include "peasant.h"

bool InteractionWithPeasant(const Peasant *peasant, Player *player)
{
    float dx = player->position.x - peasant->position.x;
    return (dx > -150.0f && dx < 10.0f);
}

void InitPeasant(Peasant *peasant)
{
    peasant->position = (Vector2){600.0f, 528.0f };

    peasant->peasantSpeech = LoadTexture("resources/npcs/peasant/peasant_speech.png");
    peasant->peasantIdle = LoadTexture("resources/sprites/npc//peasant.png");
    peasant->peasantBtnE = LoadTexture("resources/npcs/peasant/btn-E.png");
    peasant->peasantExclamation = LoadTexture("resources/sprites/npc/exclamation.png");

    peasant->dialogues[0][0].text = "Hello there, traveler!";
    peasant->dialogues[0][0].sound = LoadSound("resources/npcs/peasant/dialogue1.wav");

    peasant->dialogues[0][1].text = "The village is just ahead.";
    peasant->dialogues[0][1].sound = LoadSound("resources/npcs/peasant/dialogue2.wav");

    peasant->dialogues[0][2].text = "Safe travels!";
    peasant->dialogues[0][2].sound = LoadSound("resources/npcs/peasant/dialogue3.wav");

    peasant->frameIdle = 2;

    peasant->isInteraction = false;

    peasant->frameWidth = peasant->peasantIdle.width / peasant->frameIdle;
    peasant->frameHeight = peasant->peasantIdle.height;
    peasant->currentFrame = 0;
    peasant->frameCounter = 0;

    peasant->textFont = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);

    peasant->speechFontSize = 20;
}

void UpdatePeasant(Peasant *peasant, Player *player, float deltaTime, Interaction *interaction, DialogStatePeasant *dialogStatePeasant, float *dialogoTimerPeasant, int currentMapIndex)
{
    if (peasant->isInteraction)
    {
        UpdateInteraction(interaction, deltaTime);
    }
    
    peasant->frameCounter++;
    if (!peasant->isInteraction)
    {
        if (peasant->frameCounter >= (200 / 5))
        {
            peasant->frameCounter = 0;
            peasant->currentFrame = (peasant->currentFrame + 1) % peasant->frameIdle;
        }
    }

    if (*dialogStatePeasant == DIALOG_CLOSED_PEASANT)
    {
        peasant->isInteraction = false;
        
        if (InteractionWithPeasant(peasant, player) && IsKeyPressed(KEY_E))
        {
            peasant->isInteraction = true;
            *dialogStatePeasant = DIALOG_PLAYER_PEASANT_TALKING;
            *dialogoTimerPeasant = 0.0f;
            PlayPlayerSound(player, currentMapIndex, 0);
        }
        return;
    }
    peasant->isInteraction = true;
}

void DrawPeasant(Peasant *peasant, Interaction *interaction)
{
    Rectangle source = {
        peasant->currentFrame * peasant->frameWidth,
        0,
        peasant->frameWidth,
        peasant->frameHeight
    };

    Rectangle dest = {
        peasant->position.x,
        peasant->position.y,
        peasant->frameWidth / 7.5f,
        peasant->frameHeight / 7.5f
    };

    Vector2 origin = {0, 0};

    if (peasant->isInteraction)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 160});
        DrawInteractionPeasant(peasant, interaction);
    }
    else 
    {
        DrawTexture(peasant->peasantExclamation, peasant->position.x + 15, peasant->position.y - 25, WHITE);
    }
    
    DrawTexturePro(peasant->peasantIdle, source, dest, origin, 0.0f, WHITE);
}

void DrawMapsPeasant(Peasant *peasant, Interaction *interaction, int currentMapIndex, float delta)
{
    int mapsPeasants[3] = {0, 1, 6};

    for (int i = 0; i < 3; i++)
    {
        if (currentMapIndex == mapsPeasants[i])
        {
            DrawPeasant(peasant, interaction);
            UpdateInteraction(interaction, delta);
        }
    }
}

void UnloadPeasant(Peasant *peasant)
{
    UnloadTexture(peasant->peasantSpeech);
    UnloadTexture(peasant->peasantIdle);
    UnloadTexture(peasant->peasantBtnE);

    for (int i = 0; i < NUM_MAPS; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            UnloadSound(peasant->dialogues[i][j].sound);
        }
    }

    UnloadFont(peasant->textFont);
}