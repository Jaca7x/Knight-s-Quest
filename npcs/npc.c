#include "npc.h"

bool checkNpcInteraction(Npc *npc, Player *player)
{
    // Verifica se o jogador está próximo do NPC
    float distance = Vector2Distance(npc->position, player->position);
    return distance < 100.0f; // Distância de interação
}

void talkingNpc(Npc *npc, Player *player)
{
    if (npc->isTalking)
    {
        DrawText("Rápido eles estão vindo!", 
                 npc->position.x + 10, 
                 npc->position.y - 30, 
                 20, 
                 WHITE);
    }
    else 
    {
        npc->isTalking = false;
        npc->isIdle = true; // Se não está falando, volta para idle
    }
}

void InitNpc(Npc *npc)
{
    npc->spriteNpc = LoadTexture("resources/sprites/npc/npc-map1.png"); // Carrega o sprite do NPC
    npc->btnE = LoadTexture("resources/sprites/btns/btn-E.png"); // Carrega o botão de interação (E)
    npc->spriteNpcIdle = LoadTexture("resources/sprites/npc/npc1-idle.png"); // Carrega o sprite de idle do NPC
    npc->npcSpeech = LoadTexture("resources/sprites/npc/npc-speech.png"); // Carrega o sprite de fala do NPC (se necessário)
    npc->position = (Vector2){1000, 546}; // Define a posição inicial do NPC

    npc->frameTalking = 4; // Define o número de frames para a animação de fala
    npc->frameIdle = 4; // Define o número de frames para a animação de idle    

    npc->frameWidth = npc->spriteNpc.width / npc->frameIdle; // Define a largura de cada frame do sprite
    npc->frameHeight = npc->spriteNpc.height; // Define a altura de cada frame do sprite
    npc->currentFrame = 0; // Inicia no primeiro frame
    npc->frameCounter = 0; // Inicia o contador de frames
     
    bool isTalking = false;
    bool isIdle = true; // Inicia o NPC como idle
}

void UpdateNpc(Npc *npc, float deltaTime, Player *player, DialogoEstado *dialogoEstado, float *dialogoTimer)
{
    static float idleTimer = 0.0f;
    static float talkingTimer = 0.0f;

    // Suponha que você tenha uma flag para saber se o NPC está falando
   
    if (npc->isIdle)
    {
        idleTimer += deltaTime;
        if (idleTimer >= 0.5f) // 2 FPS para idle
        {
            idleTimer = 0.0f;
            npc->currentFrame = (npc->currentFrame + 1) % npc->frameIdle;
        }
    }
    else if (npc->isTalking)
    {
        talkingTimer += deltaTime;
        if (talkingTimer >= 1.0f) // 1 FPS para fala
        {
            talkingTimer = 0.0f;
            npc->currentFrame = (npc->currentFrame + 1) % npc->frameTalking;
        }
    }
    

    if (checkNpcInteraction(npc, player))
    {
        // Se o jogador pressionar a tecla de interação (E)
        if (IsKeyPressed(KEY_E))
        {
            npc->isTalking = true; // Alterna entre falar e não falar
            npc->isIdle = false; // Se está falando, não está idle
            npc->currentFrame = 0; // Reseta o frame atual ao iniciar fala
        }
    }
}


void DrawNpc(Npc *npc, Player *player, DialogoEstado dialogoEstado)
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
    

// No seu loop de desenho:
Rectangle sourceSpeech = { 0, 0, npc->npcSpeech.width, npc->npcSpeech.height }; // Usa a imagem toda
Rectangle destSpeech = { 500, 655, npc->npcSpeech.width, npc->npcSpeech.height }; // Desenha na posição desejada
Vector2 origin2 = { 0, 0 };


    Vector2 origin = {0, 0};

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

    if (npc->isIdle)
    {
        DrawTexturePro(npc->spriteNpcIdle, source, dest, origin, 0.0f, WHITE);
    }
    else if (npc->isTalking)  // Se o NPC estiver falando, desenha o sprite de fala
    {
        DrawTexturePro(npc->spriteNpc, source, dest, origin, 0.0f, WHITE);

        DrawRectangle(0, 0, 2000, 2000, (Color){0, 0, 0, 160}); // Fundo semi-transparente para o diálogo

        DrawTexturePro(npc->npcSpeech, sourceSpeech, destSpeech, origin2, 0.0f, WHITE);

        talkingNpc(npc, player); // Desenha o texto de interação
    }
}




void UnloadNpc(Npc *npc)
{
    UnloadTexture(npc->spriteNpc); // Libera a memória do sprite do NPC
}