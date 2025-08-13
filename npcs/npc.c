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
    Vector2 mousePos = GetMousePosition();


    printf("Mouse X: %i | Mouse Y: %i\n", (int)mousePos.x, (int)mousePos.y);

    static float idleTimer = 0.0f;
    static float talkingTimer = 0.0f;

    if (*dialogoEstado == DIALOGO_FECHADO)
    {
        idleTimer += deltaTime;
        if (idleTimer >= 0.5f)
        {
            idleTimer = 0.0f;
            npc->currentFrame = (npc->currentFrame + 1) % npc->frameIdle;
        }

        if (checkNpcInteraction(npc, player) && IsKeyPressed(KEY_E))
        {
            *dialogoEstado = DIALOGO_NPC_FALANDO;
            *dialogoTimer = 0.0f;
            npc->currentFrame = 0;
        }
    }
    else if (*dialogoEstado == DIALOGO_NPC_FALANDO)
    {
        talkingTimer += deltaTime;
        *dialogoTimer += deltaTime;

        if (talkingTimer >= 1.0f)
        {
            talkingTimer = 0.0f;
            npc->currentFrame = (npc->currentFrame + 1) % npc->frameTalking;
        }

        if (*dialogoTimer >= 3.0f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                *dialogoEstado = DIALOGO_PLAYER_FALANDO;
                *dialogoTimer = 0.0f;
                npc->currentFrame = 0;
            }
        }
    }
    else if (*dialogoEstado == DIALOGO_PLAYER_FALANDO)
    {
        npc->currentFrame = 0;

        if (IsKeyPressed(KEY_SPACE))
        {
            *dialogoEstado = DIALOGO_FECHADO;
            *dialogoTimer = 0.0f;
        }
    }
}


void DrawNpc(Npc *npc, Player *player, DialogoEstado dialogoEstado)
{
    Font textSpeech = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);

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

    // Desenha botão E se perto
    if (checkNpcInteraction(npc, player) && dialogoEstado == DIALOGO_FECHADO)
    {
        Rectangle btnDest = {
            npc->position.x + 5,
            npc->position.y - 40,
            npc->btnE.width,
            npc->btnE.height
        };
        DrawTexture(npc->btnE, btnDest.x, btnDest.y, WHITE);
    }

    // Desenha o NPC
    if (dialogoEstado == DIALOGO_FECHADO)
    {
        DrawTexturePro(npc->spriteNpcIdle, source, dest, origin, 0.0f, WHITE);
    }
    else
    {
        DrawTexturePro(npc->spriteNpc, source, dest, origin, 0.0f, WHITE);

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0,0,0,160});
        
        int ySpeech = GetScreenHeight() - npc->npcSpeech.height - 10;

        int playerX = 20;

        int npcX = GetScreenWidth() - npc->npcSpeech.width - 20;

        if (dialogoEstado == DIALOGO_NPC_FALANDO)
        {
            DrawTexture(npc->npcSpeech, npcX, ySpeech, WHITE);
            DrawTextEx(textSpeech, "Cavaleiro Jovem: Rápido, eles estão vindo!", 
               (Vector2){npcX + 100, ySpeech + 80}, 30, 0, BLACK);
            DrawText("Aperte ESPAÇO para continuar", 996, 848, 14,  BLACK);
        }
        else if (dialogoEstado == DIALOGO_PLAYER_FALANDO)
        {
            DrawTexture(player->playerSpeech, playerX, ySpeech, WHITE);
            DrawTextEx(textSpeech, "Gareth II: Quem são eles!?",
                 (Vector2){playerX + 260, ySpeech + 100}, 30, 0, BLACK);
            DrawText("Aperte ESPAÇO para continuar", 246, 848, 14,  BLACK);
        }

    }
}


void UnloadNpc(Npc *npc)
{
    UnloadTexture(npc->spriteNpc); // Libera a memória do sprite do NPC
}