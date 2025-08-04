#include "npc.h"

bool checkNpcInteraction(Npc *npc, Player *player)
{
    // Verifica se o jogador está próximo do NPC
    float distance = Vector2Distance(npc->position, player->position);
    return distance < 100.0f; // Distância de interação
}

void InitNpc(Npc *npc)
{
    npc->spriteNpc = LoadTexture("resources/sprites/npc/npc-map1.png"); // Carrega o sprite do NPC
    npc->btnE = LoadTexture("resources/sprites/btns/btn-E.png"); // Carrega o botão de interação (E)
    npc->position = (Vector2){1000, 546}; // Define a posição inicial do NPC

    npc->frameTalking = 4; // Define o número de frames para a animação de fala

    npc->frameWidth = npc->spriteNpc.width / npc->frameTalking; // Define a largura de cada frame do sprite
    npc->frameHeight = npc->spriteNpc.height; // Define a altura de cada frame do sprite
    npc->currentFrame = 0; // Inicia no primeiro frame
    npc->frameCounter = 0; // Inicia o contador de frames

}

void UpdateNpc(Npc *npc, float deltaTime)
{
    npc->frameCounter++;
    if (npc->frameCounter >= (60 / 1)) // Atualiza o frame a cada 1 FPS
    {
        npc->frameCounter = 0;
        npc->currentFrame = (npc->currentFrame + 1) % (npc->spriteNpc.width / npc->frameWidth);
    }

}

void DrawNpc(Npc *npc, Player *player)
{
    Rectangle source = {
        npc->currentFrame * npc->frameWidth, 
        0,
        npc->frameWidth,
        npc->frameHeight
    };

    Rectangle dest = {
        npc->position.x,
        npc->position.y,
        npc->frameWidth,   
        npc->frameHeight
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(npc->spriteNpc, source, dest, origin, 0.0f, WHITE);

    // Desenha o botão de interação (E) se o NPC estiver próximo
    if (checkNpcInteraction(npc, player))
    {
        Rectangle btnDest = {
            npc->position.x + 5, // Posição do botão em relação ao NPC
            npc->position.y - 40, // Acima do NPC
            npc->btnE.width,
            npc->btnE.height
        };
        DrawTexture(npc->btnE, btnDest.x, btnDest.y, WHITE);
    }
}


void UnloadNpc(Npc *npc)
{
    UnloadTexture(npc->spriteNpc); // Libera a memória do sprite do NPC
}