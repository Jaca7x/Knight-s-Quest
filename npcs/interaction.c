#include "interaction.h"

void InitInteraction(Interaction *interaction) 
{
    interaction->currentFrame = 0;
    interaction->frameCounter = 0;

    interaction->columns = 3;
    interaction->rows = 3;

    interaction->totalFrames = interaction->columns * interaction->rows;

    interaction->interactionTexture = LoadTexture("resources/sprites/npc/interaction.png");

    interaction->frameWidth = interaction->interactionTexture.width / interaction->columns;
    interaction->frameHeight = interaction->interactionTexture.height / interaction->rows;
}

void UpdateInteraction(Ghost *ghost, Player *player, Interaction *interaction, float delta) 
{
    
    if (ghost->isInteraction)
    {
        interaction->frameCounter++;
        if (interaction->frameCounter >= (100 / 5))
        {
            interaction->frameCounter = 0;
            interaction->currentFrame = (interaction->currentFrame + 1) % interaction->totalFrames;
        }
    }
}

void DrawInteraction(Ghost *ghost, Player *player, Interaction *interaction) 
{
    Rectangle source = {
        (interaction->currentFrame % interaction->columns) * interaction->frameWidth,
        (interaction->currentFrame / interaction->columns) * interaction->frameHeight,
        interaction->frameWidth,
        interaction->frameHeight
    };

    Rectangle dest = {
        ghost->position.x + 20,
        ghost->position.y - 20,
        interaction->frameWidth * 2,
        interaction->frameHeight * 2
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(interaction->interactionTexture, source, dest, origin, 0.0f, WHITE);
}
