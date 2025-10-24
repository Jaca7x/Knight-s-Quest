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

void UpdateInteraction(Interaction *interaction, float delta) 
{
        interaction->frameCounter++;
        if (interaction->frameCounter >= (100 / 5))
        {
            interaction->frameCounter = 0;
            interaction->currentFrame = (interaction->currentFrame + 1) % interaction->totalFrames;
        }
    
}

void DrawInteractionGhost(Ghost *ghost, Interaction *interaction) 
{
    Vector2 offset = {20, -20};

    Rectangle source = {
        (interaction->currentFrame % interaction->columns) * interaction->frameWidth,
        (interaction->currentFrame / interaction->columns) * interaction->frameHeight,
        interaction->frameWidth,
        interaction->frameHeight
    };

    Rectangle dest = {
        ghost->position.x + offset.x,
        ghost->position.y + offset.y,
        interaction->frameWidth,
        interaction->frameHeight
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(interaction->interactionTexture, source, dest, origin, 0.0f, WHITE);
}

void DrawInteractionNPC(Npc *npc, Interaction *interaction) 
{
    Vector2 offset = {12, -25};
    Rectangle source = {
        (interaction->currentFrame % interaction->columns) * interaction->frameWidth,
        (interaction->currentFrame / interaction->columns) * interaction->frameHeight,
        interaction->frameWidth,
        interaction->frameHeight
    };

    Rectangle dest = {
        npc->position.x + offset.x,
        npc->position.y + offset.y,
        interaction->frameWidth,
        interaction->frameHeight
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(interaction->interactionTexture, source, dest, origin, 0.0f, WHITE);
}
