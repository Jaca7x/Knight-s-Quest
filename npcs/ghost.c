#include "ghost.h"

// ================= InteractionWithGhost =================
bool InteractionWithGhost(const Ghost *ghost, Player *player)
{
    // Usa deslocamento horizontal (dx). Ajuste os valores conforme necessário.
    float dx = player->position.x - ghost->position.x;
    // jogador entre 150px à esquerda e 60px à direita do fantasma
    return (dx > -150.0f && dx < 60.0f);
}

void InitGhost(Ghost *ghost)
{
    ghost->position = (Vector2){800, 540};
    ghost->ghostIdle = LoadTexture("resources/sprites/npc/ghost-idle.png");
    ghost->ghostBtnE = LoadTexture("resources/sprites/btns/btn-E.png");
    ghost->ghostSpeech = LoadTexture("resources/sprites/npc/ghost-speech.png");
    ghost->ghostExclamation = LoadTexture("resources/sprites/npc/exclamation.png");
    ghost->ghostInteraction = LoadTexture("resources/sprites/npc/interaction.png");

    int columnsInteraction = 3;  
    int rowsInteraction = 3; 

    ghost->frameIdle = 3;
    ghost->frameInteraction = columnsInteraction * rowsInteraction;

    bool isInteraction = false;

    ghost->frameWidth = ghost->ghostIdle.width / ghost->frameIdle;
    ghost->frameHeight = ghost->ghostIdle.height;
    ghost->frameWidthInteraction = ghost->ghostInteraction.width / columnsInteraction;
    ghost->frameHeightInteraction = ghost->ghostInteraction.height / rowsInteraction;
    ghost->currentFrame = 0;
    ghost->frameCounter = 0;

    ghost->textFont = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250); // <- adicione Font textFont no struct Ghost
}

void UpdateGhost(Ghost *ghost, Player *player, float delta, DialogStateGhost *dialogStateGhost, float *dialogoTimer)
{
    ghost->frameCounter++;
    if (ghost->frameCounter >= (200 / 5))
    {
        ghost->frameCounter = 0;
        ghost->currentFrame = (ghost->currentFrame + 1) % ghost->frameIdle;
    }

    ghost->frameCounter++;
    if (ghost->frameCounter >= (100 / 5))
    {
        ghost->frameCounter = 0;
        ghost->currentFrame = (ghost->currentFrame + 1) % ghost->frameInteraction;
    }

    if (*dialogStateGhost == DIALOG_CLOSED_GHOST)
    {
        ghost->isInteraction = false;
        
        if (InteractionWithGhost(ghost, player) && IsKeyPressed(KEY_E))
        {
            ghost->isInteraction = true;
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING;
            *dialogoTimer = 0.0f;
        }
        return;
    }
    ghost->isInteraction = true;

    if (*dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 2.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_GHOST_TALKING;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 2.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING2;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING2)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 2.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_GHOST_TALKING2;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING2)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 2.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING3;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogoTimer == DIALOG_PLAYER_GHOST_TALKING3)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 2.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_CLOSED_GHOST;
            *dialogoTimer = 0.0f;
            ghost->isInteraction = false;
        }
    }
}

void DrawGhost(Ghost *ghost, Player *player, DialogStateGhost dialogStateGhost)
{
    static int visibleLetters = 0;
    static float timeWriting = 0.0f;
    float writingSpeed = 0.04f; 

    Font textSpeech = ghost->textFont;

    Rectangle source = {
        ghost->currentFrame * ghost->frameWidth,
        0,
        ghost->frameWidth,
        ghost->frameHeight
    };

    int columnsInteraction = 3; 
    int rowsInteraction = 3;

    int frameX = ghost->currentFrame % columnsInteraction;
    int frameY = ghost->currentFrame / columnsInteraction;

    Rectangle sourceInteraction = {
    frameX * ghost->frameWidthInteraction,
    frameY * ghost->frameHeightInteraction,
    ghost->frameWidthInteraction,
    ghost->frameHeightInteraction
    };


    Rectangle dest2 = {
        ghost->position.x,
        ghost->position.y,
        ghost->frameWidthInteraction,
        ghost->frameHeightInteraction
    };

    Rectangle dest = {
        ghost->position.x,
        ghost->position.y,
        ghost->frameWidth / 2.5f,
        ghost->frameHeight / 2.5f
    };

    Vector2 origin = {0, 0};

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

    int speechY = GetScreenHeight() - ghost->ghostSpeech.height - speechMarginBottom;
    int ghostSpeechX = GetScreenWidth() - ghost->ghostSpeech.width - speechMarginSideNpc;
    int playerSpeechX = speechMarginSide;

    int nextMsgTextPlayerX = 246;
    int nextMsgTextNpcX = 996;
    int nextMsgTextY = 848;

    const char *lines[] = {
        "Gareth II: Ah! Quem é você?",
        "Cavaleiro Fantasma: Olá, eu sou um cavaleiro fantasma, ire te ajudar na sua jornada até o reino goblin",
        "Gareth II: Ainda, bem que você apareceu! Quem são esses goblins?",
        "Cavaleiro Fantasma: Os goblins do reino de Gorzugar!\nEles estão atacando o castelo!",
        "Gareth II: Não se preocupe, avise aos soldados para\nse prepararem! Vou ganhar tempo enquanto isso!",  
        "HHHHHHHHHHHHHHHHHH"
    };

    int lineIndex = -1;
    if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING) lineIndex = 0;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING) lineIndex = 1;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING2) lineIndex = 2;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING2) lineIndex = 3;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING3) lineIndex = 4;

    static DialogStateGhost lastState = DIALOG_CLOSED_GHOST; 
    if (dialogStateGhost != lastState)
    {
        visibleLetters = 0;
        timeWriting = 0.0f;
    }
    lastState = dialogStateGhost;

    if (lineIndex >= 0)
    {
        timeWriting += GetFrameTime();
        if (timeWriting >= writingSpeed && visibleLetters < (int)strlen(lines[lineIndex]))
        {
            visibleLetters++;
            timeWriting = 0.0f;
        }

        if (dialogStateGhost == DIALOG_GHOST_TALKING || dialogStateGhost == DIALOG_GHOST_TALKING2)
        {
            DrawTexture(ghost->ghostSpeech, ghostSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], 0, visibleLetters),
                       (Vector2){ghostSpeechX + npcSpeechTextOffsetX, speechY + npcSpeechTextOffsetY},
                       speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para pular", nextMsgTextNpcX, nextMsgTextY, hintFontSize, BLACK);
        }
        else
        {
            DrawTexture(player->playerSpeech, playerSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], 0, visibleLetters),
                       (Vector2){playerSpeechX + playerSpeechTextOffsetX, speechY + playerSpeechTextOffsetY},
                       speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para continuar", nextMsgTextPlayerX, nextMsgTextY, hintFontSize, BLACK);
        }
    }

    DrawTexturePro(ghost->ghostIdle, source, dest, origin, 0.0f, Fade(WHITE, 0.7f));

    if (InteractionWithGhost(ghost, player))
    {
        DrawTexture(ghost->ghostBtnE, ghost->position.x + 15, ghost->position.y - 35, WHITE);
    }
    else
    {
        DrawTexture(ghost->ghostExclamation, ghost->position.x + 25, ghost->position.y - 25, WHITE);
    }

    if (ghost->isInteraction)
    {
        DrawTexturePro(ghost->ghostInteraction, sourceInteraction, dest2, origin, 0.0f, WHITE);
    }
    
}

void UnloadGhost(Ghost *ghost)
{
    UnloadTexture(ghost->ghostIdle);
    UnloadTexture(ghost->ghostBtnE);
    UnloadTexture(ghost->ghostSpeech);
    UnloadTexture(ghost->ghostExclamation);
    UnloadFont(ghost->textFont);
}
