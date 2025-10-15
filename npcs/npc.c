#include "npc.h"

bool checkNpcInteraction(Npc *npc, Player *player)
{
    float distance = player->position.x - npc->position.x;
    return (distance > -150.0f && distance < 10.0f);
}

void InitNpc(Npc *npc)
{
    npc->spriteNpc = LoadTexture("resources/sprites/npc/npc-map1.png"); // Load NPC sprite
    npc->btnE = LoadTexture("resources/sprites/btns/btn-E.png"); // Load interaction button (E)
    npc->spriteNpcIdle = LoadTexture("resources/sprites/npc/npc1-idle.png"); // Load NPC idle sprite
    npc->npcSpeech = LoadTexture("resources/sprites/npc/npc-speech.png"); // Load NPC speech bubble sprite (if necessary)
    npc->position = (Vector2){1000, 546}; // Set NPC initial position

    npc->frameTalking = 4; // Number of frames for talking animation
    npc->frameIdle = 4; // Number of frames for idle animation    

    npc->frameWidth = npc->spriteNpc.width / npc->frameIdle; // Frame width
    npc->frameHeight = npc->spriteNpc.height; // Frame height
    npc->currentFrame = 0; // Start at first frame
    npc->frameCounter = 0; // Start frame counter
     
    bool isTalking = false;
    bool isIdle = true; // NPC starts idle
}

void UpdateNpc(Npc *npc, float deltaTime, Player *player, DialogState *dialogState, float *dialogTimer)
{
    static float idleTimer = 0.0f;
    static float talkingTimer = 0.0f;

    if (*dialogState == DIALOG_CLOSED)
    {
        idleTimer += deltaTime;
        if (idleTimer >= 0.5f)
        {
            idleTimer = 0.0f;
            npc->currentFrame = (npc->currentFrame + 1) % npc->frameIdle;
        }

        if (checkNpcInteraction(npc, player) && IsKeyPressed(KEY_E))
        {
            *dialogState = DIALOG_NPC_TALKING;
            *dialogTimer = 0.0f;
            npc->currentFrame = 0;
        }
    }
    else if (*dialogState == DIALOG_NPC_TALKING)
    {
        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        if (talkingTimer >= 1.0f)
        {
            talkingTimer = 0.0f;
            npc->currentFrame = (npc->currentFrame + 1) % npc->frameTalking;
        }

        if (*dialogTimer >= 2.5f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                *dialogState = DIALOG_PLAYER_TALKING;
                *dialogTimer = 0.0f;
                npc->currentFrame = 0;
            }
        }
    }
    else if (*dialogState == DIALOG_PLAYER_TALKING)
    {
        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        npc->currentFrame = 0;

        if (*dialogTimer >= 1.5f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                *dialogState = DIALOG_NPC_TALKING2;
                *dialogTimer = 0.0f;
                npc->currentFrame = 0;
            }
        }
    }
    else if (*dialogState == DIALOG_NPC_TALKING2)
    {
        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        npc->currentFrame = 0;

        if (*dialogTimer >= 4.0f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                *dialogState = DIALOG_PLAYER_TALKING2;
                *dialogTimer = 0.0f;
                npc->currentFrame = 0;
            }
        }
    }  
    else if (*dialogState == DIALOG_PLAYER_TALKING2)
    {
        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        npc->currentFrame = 0;

        if (*dialogTimer >= 5.0f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                *dialogState = DIALOG_CLOSED;
                *dialogTimer = 0.0f;
            }
        }   
    }
}


void DrawNpc(Npc *npc, Player *player, DialogState dialogState)
{
    static int visibleLetters = 0;
    static float timeWriting = 0.0f;
    float writingSpeed = 0.04f; // Seconds per letter

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

    // Layout variables
    int speechFontSize = 30;
    int hintFontSize = 14;
    float textSpacing = 0.0f;
    int lineHeightOffset = 20;

    int speechMarginBottom = 10;
    int speechMarginSide = 20;
    int speechMarginSideNpc = 20;

    int playerSpeechTextOffsetX = 210;
    int playerSpeechTextOffsetY = 80;

    int npcSpeechTextOffsetX = 100;
    int npcSpeechTextOffsetY = 80;

    int speechY = GetScreenHeight() - npc->npcSpeech.height - speechMarginBottom;
    int npcSpeechX = GetScreenWidth() - npc->npcSpeech.width - speechMarginSideNpc;
    int playerSpeechX = speechMarginSide;

    int nextMsgTextPlayerX = 246;
    int nextMsgTextNpcX = 996;
    int nextMsgTextY = 848;

    // Dialog lines
    const char *lines[] = {
        "Cavaleiro Jovem: Rápido, eles estão vindo!", //DIALOG_NPC_TALKING
        "Gareth II: Quem são eles!?",            // DIALOG_PLAYER_TALKING
        "Cavaleiro Jovem: Os goblins do reino de Gorzugar!\nEles estão atacando o castelo!", // DIALOG_NPC_TALKING2
        "Gareth II: Não se preocupe, avise aos soldados para\nse prepararem! Vou ganhar tempo enquanto isso!" // DIALOG_PLAYER_TALKING2
    };

    // Determine current line index
    int lineIndex = -1;
    if (dialogState == DIALOG_NPC_TALKING) lineIndex = 0;
    else if (dialogState == DIALOG_PLAYER_TALKING) lineIndex = 1;
    else if (dialogState == DIALOG_NPC_TALKING2) lineIndex = 2;
    else if (dialogState == DIALOG_PLAYER_TALKING2) lineIndex = 3;

    // Reset writing when dialog changes
    static DialogState lastState = DIALOG_CLOSED;
    if (dialogState != lastState)
    {
        visibleLetters = 0;
        timeWriting = 0.0f;
    }
    lastState = dialogState;

    // Interaction button (E)
    if (checkNpcInteraction(npc, player) && dialogState == DIALOG_CLOSED)
    {
        DrawTexture(npc->btnE, npc->position.x + 5, npc->position.y - 40, WHITE);
    }
    
    if (dialogState == DIALOG_CLOSED)
    {
        DrawTexturePro(npc->spriteNpcIdle, source, dest, origin, 0.0f, WHITE);
    }
    else
    {
        DrawTexturePro(npc->spriteNpc, source, dest, origin, 0.0f, WHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0,0,0,160});

        // Update animated writing
        if (lineIndex >= 0)
        {
            timeWriting += GetFrameTime();
            if (timeWriting >= writingSpeed && visibleLetters < strlen(lines[lineIndex]))
            {
                visibleLetters++;
                timeWriting = 0.0f;
            }

            // Choose speech bubble and text position
            if (dialogState == DIALOG_NPC_TALKING || dialogState == DIALOG_NPC_TALKING2)
            {
                DrawTexture(npc->npcSpeech, npcSpeechX, speechY, WHITE);
                DrawTextEx(textSpeech,
                           TextSubtext(lines[lineIndex], 0, visibleLetters),
                           (Vector2){npcSpeechX + npcSpeechTextOffsetX, speechY + npcSpeechTextOffsetY},
                           speechFontSize, textSpacing, BLACK);

                if (visibleLetters >= strlen(lines[lineIndex]))
                    DrawText("Pressione ESPAÇO para pular", nextMsgTextNpcX, nextMsgTextY, hintFontSize, BLACK);
            }
            else
            {
                DrawTexture(player->playerSpeech, playerSpeechX, speechY, WHITE);
                DrawTextEx(textSpeech,
                           TextSubtext(lines[lineIndex], 0, visibleLetters),
                           (Vector2){playerSpeechX + playerSpeechTextOffsetX, speechY + playerSpeechTextOffsetY},
                           speechFontSize, textSpacing, BLACK);

                if (visibleLetters >= strlen(lines[lineIndex]))
                    DrawText("Pressione ESPAÇO para continuar", nextMsgTextPlayerX, nextMsgTextY, hintFontSize, BLACK);
            }
        }
    }
}

void UnloadNpc(Npc *npc)
{
    UnloadTexture(npc->spriteNpc); // Free NPC sprite memory
}
