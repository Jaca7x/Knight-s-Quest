#include "interaction.h"

void InitInteraction(Interaction *interaction)
{
    interaction->currentFrame = 0;
    interaction->frameCounter = 0;

    interaction->columns = 3;
    interaction->rows = 3;

    interaction->totalFrames = interaction->columns * interaction->rows;

    interaction->interactionTexture = LoadTexture("assets/resources/sprites/npc/interaction.png");

    interaction->frameWidth = interaction->interactionTexture.width / interaction->columns;
    interaction->frameHeight = interaction->interactionTexture.height / interaction->rows;
}

void UpdateInteraction(Interaction *interaction, float delta)
{
    interaction->frameCounter++;
    if (interaction->frameCounter >= INTERACTION_FRAME_SPEED)
    {
        interaction->frameCounter = FRAME_COUNTER_ZERO;
        interaction->currentFrame = (interaction->currentFrame + NEXT_FRAME) % interaction->totalFrames;
    }
}

void DrawInteractionGhost(Ghost *ghost, Interaction *interaction)
{
    Rectangle source = {
        (interaction->currentFrame % interaction->columns) * interaction->frameWidth,
        (interaction->currentFrame / interaction->columns) * interaction->frameHeight,
        interaction->frameWidth,
        interaction->frameHeight};

    Rectangle dest = {
        ghost->position.x + GHOST_INTERACTION_OFFSET,
        ghost->position.y - GHOST_INTERACTION_OFFSET,
        interaction->frameWidth,
        interaction->frameHeight};

    DrawTexturePro(interaction->interactionTexture, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
}

void DrawInteractionPeasant(Peasant *peasant, Interaction *interaction)
{
    Rectangle source = {
        (interaction->currentFrame % interaction->columns) * interaction->frameWidth,
        (interaction->currentFrame / interaction->columns) * interaction->frameHeight,
        interaction->frameWidth,
        interaction->frameHeight};

    Rectangle dest = {
        peasant->position.x + PEASANT_INTERACTION_OFFSET_X,
        peasant->position.y + PEASANT_INTERACTION_OFFSET_Y,
        interaction->frameWidth,
        interaction->frameHeight};
        
    DrawTexturePro(interaction->interactionTexture, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
}

void DrawInteractionNPC(Npc *npc, Interaction *interaction)
{
    Rectangle source = {
        (interaction->currentFrame % interaction->columns) * interaction->frameWidth,
        (interaction->currentFrame / interaction->columns) * interaction->frameHeight,
        interaction->frameWidth,
        interaction->frameHeight};

    Rectangle dest = {
        npc->position.x + NPC_INTERACTION_OFFSET_X,
        npc->position.y + NPC_INTERACTION_OFFSET_Y,
        interaction->frameWidth,
        interaction->frameHeight};

    DrawTexturePro(interaction->interactionTexture, source, dest, ORIGIN_TOPLEFT, ROTATION, WHITE);
}
