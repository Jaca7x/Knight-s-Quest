#include "npc.h"

bool checkNpcInteraction(Npc *npc, Player *player)
{
    float distance = player->position.x - npc->position.x;
    return (distance > MIN_DISTANCE_FOR_INTERACTION_NPCS && distance < MAX_DISTANCE_FOR_INTERACTION_NPCS);
}

void InitNpc(Npc *npc)
{
    // Position
    npc->position = (Vector2){1000, 546};

    // Animation and control
    npc->currentFrame = 0;
    npc->frameCounter = 0;

    // Sprites
    npc->spriteNpc = LoadTexture("assets/resources/sprites/npc/npc-map1.png");
    npc->btnE = LoadTexture("assets/resources/sprites/btns/btn-E.png");
    npc->spriteNpcIdle = LoadTexture("assets/resources/sprites/npc/npc1-idle.png");
    npc->npcSpeech = LoadTexture("assets/resources/sprites/npc/npc-speech.png");
    npc->exclamation = LoadTexture("assets/resources/sprites/npc/exclamation.png");

    npc->textSpeech = LoadFontEx("assets/resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);

    // Frames
    npc->frameTalking = 4;
    npc->frameIdle = 4;

    // Sizes 
    npc->frameWidth = npc->spriteNpc.width / npc->frameIdle;
    npc->frameHeight = npc->spriteNpc.height;

    // Sounds
    npc->dialogueWithPlayer1 = 
        LoadSound("assets/resources/sounds/voices/npc/npc-1-dialogue.wav");
    npc->dialogueWithPlayer2 = 
        LoadSound("assets/resources/sounds/voices/npc/npc-2-dialogue.wav");

    // States
    npc->showExclamation = true;
}

void UpdateNpc(Npc *npc, float deltaTime, Player *player, DialogState *dialogState, float *dialogTimer)
{
    static float idleTimer = TIMER_ZERO;
    static float talkingTimer = TIMER_ZERO;

    if (*dialogState == DIALOG_CLOSED)
    {
        npc->showExclamation = true;

        idleTimer += deltaTime;
        if (idleTimer >= 0.5f)
        {
            idleTimer = TIMER_ZERO;
            npc->currentFrame = (npc->currentFrame + NEXT_FRAME) % npc->frameIdle;
        }

        if (checkNpcInteraction(npc, player) && IsKeyPressed(KEY_E))
        {
            PlaySound(npc->dialogueWithPlayer1);
            npc->showExclamation = false;
            *dialogState = DIALOG_NPC_TALKING;
            *dialogTimer = TIMER_ZERO;
            npc->currentFrame = CURRENT_FRAME_ZERO;
        }
    }
    else if (*dialogState == DIALOG_NPC_TALKING)
    {
        npc->showExclamation = false;

        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        if (talkingTimer >= 1.0f)
        {
            talkingTimer = TIMER_ZERO;
            npc->currentFrame = (npc->currentFrame + NEXT_FRAME) % npc->frameTalking;
        }

        if (*dialogTimer >= 2.5f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                PlaySound(player->playerDialogueWithNPC1);
                npc->showExclamation = false;
                *dialogState = DIALOG_PLAYER_TALKING;
                *dialogTimer = TIMER_ZERO;
                npc->currentFrame = CURRENT_FRAME_ZERO;
            }
        }
    }
    else if (*dialogState == DIALOG_PLAYER_TALKING)
    {
        npc->showExclamation = false;
        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        npc->currentFrame = CURRENT_FRAME_ZERO;

        if (*dialogTimer >= 1.5f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                PlaySound(npc->dialogueWithPlayer2);
                npc->showExclamation = false;
                *dialogState = DIALOG_NPC_TALKING2;
                *dialogTimer = TIMER_ZERO;
                npc->currentFrame = CURRENT_FRAME_ZERO;
            }
        }
    }
    else if (*dialogState == DIALOG_NPC_TALKING2)
    {
        npc->showExclamation = false;
        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        npc->currentFrame = CURRENT_FRAME_ZERO;

        if (*dialogTimer >= 4.0f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                PlaySound(player->playerDialogueWithNPC2);
                npc->showExclamation = false;
                *dialogState = DIALOG_PLAYER_TALKING2;
                *dialogTimer = CURRENT_FRAME_ZERO;
                npc->currentFrame = TIMER_ZERO;
            }
        }
    }
    else if (*dialogState == DIALOG_PLAYER_TALKING2)
    {
        npc->showExclamation = false;
        talkingTimer += deltaTime;
        *dialogTimer += deltaTime;

        npc->currentFrame = CURRENT_FRAME_ZERO;

        if (*dialogTimer >= 5.0f)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                npc->showExclamation = true;
                *dialogState = DIALOG_CLOSED;
                *dialogTimer = TIMER_ZERO;
            }
        }
    }
}


void DrawNpc(Npc *npc, Player *player, Interaction *interaction, DialogState dialogState)
{
    
    static int visibleLetters = 0;
    static float timeWriting = TIMER_ZERO;
    float writingSpeed = 0.04f;

    Rectangle source = 
    {
        npc->currentFrame * npc->frameWidth,
        SPRITE_ROW_BASE,
        npc->frameWidth,
        npc->frameHeight
    };

    Rectangle dest = 
    {
        npc->position.x,
        npc->position.y,
        npc->frameWidth,
        npc->frameHeight
    };

    Vector2 origin = ORIGIN_TOPLEFT;

    int speechFontSize = 30;
    int hintFontSize = 14;
    float textSpacing = 0.0f;

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
    int nextMsgTextY = 880;

    // Dialog lines
    const char *lines[] =
        {
            "Cavaleiro Jovem: Rápido, eles estão vindo!",
            "Gareth II: Quem são eles!?",
            "Cavaleiro Jovem: Os goblins do reino de Gorzugar!\nEles estão atacando o castelo!",
            "Gareth II: Não se preocupe, avise aos soldados para\nse prepararem! Vou ganhar tempo enquanto isso!"
        };

    int lineIndex = -1;
    if (dialogState == DIALOG_NPC_TALKING)
        lineIndex = DIALOGUE_ZERO;
    else if (dialogState == DIALOG_PLAYER_TALKING)
        lineIndex = DIALOGUE_ONE;
    else if (dialogState == DIALOG_NPC_TALKING2)
        lineIndex = DIALOGUE_TWO;
    else if (dialogState == DIALOG_PLAYER_TALKING2)
        lineIndex = DIALOGUE_THREE;

    static DialogState lastState = DIALOG_CLOSED;
    if (dialogState != lastState)
    {
        visibleLetters = 0;
        timeWriting = TIMER_ZERO;
    }
    lastState = dialogState;

    if (checkNpcInteraction(npc, player) && dialogState == DIALOG_CLOSED)
    {
        DrawTexture(npc->btnE, npc->position.x + NPC_OFFSET_X_INTERACTION, npc->position.y - NPC_OFFSET_Y_INTERACTION, WHITE);
    }
    else if (npc->showExclamation)
    {
        DrawTexture(npc->exclamation, npc->position.x + NPC_OFFSET_X_EXCLAMATION, npc->position.y - NPC_OFFSET_Y_EXCLAMATION, WHITE);
    }

    if (dialogState == DIALOG_CLOSED)
    {
        DrawTexturePro(npc->spriteNpcIdle, source, dest, origin, ROTATION, WHITE);
    }
    else
    {
        DrawTexturePro(npc->spriteNpc, source, dest, origin, ROTATION, WHITE);
        DrawRectangle(POSITION_ZERO, POSITION_ZERO, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 160});
        DrawInteractionNPC(npc, interaction);

        if (lineIndex >= DIALOGUE_ZERO)
        {
            timeWriting += GetFrameTime();
            if (timeWriting >= writingSpeed && visibleLetters < strlen(lines[lineIndex]))
            {
                visibleLetters++;
                timeWriting = TIMER_ZERO;
            }

            if (dialogState == DIALOG_NPC_TALKING || dialogState == DIALOG_NPC_TALKING2)
            {
                DrawTexture(npc->npcSpeech, npcSpeechX, speechY, WHITE);
                DrawTextEx(npc->textSpeech,
                           TextSubtext(lines[lineIndex], POSITION_ZERO, visibleLetters),
                           (Vector2){npcSpeechX + npcSpeechTextOffsetX, speechY + npcSpeechTextOffsetY},
                           speechFontSize, textSpacing, BLACK);

                if (visibleLetters >= strlen(lines[lineIndex]))
                    DrawText("Pressione ESPAÇO para continuar", nextMsgTextNpcX, nextMsgTextY, hintFontSize, BLACK);
            }
            else
            {
                DrawTexture(player->playerSpeech, playerSpeechX, speechY, WHITE);
                DrawTextEx(npc->textSpeech,
                           TextSubtext(lines[lineIndex], POSITION_ZERO, visibleLetters),
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
    UnloadTexture(npc->spriteNpc);
    UnloadTexture(npc->btnE);
    UnloadTexture(npc->spriteNpcIdle);
    UnloadTexture(npc->npcSpeech);
    UnloadTexture(npc->exclamation);

    UnloadFont(npc->textSpeech);
}
