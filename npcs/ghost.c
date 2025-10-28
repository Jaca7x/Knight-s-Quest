#include "ghost.h"

bool InteractionWithGhost(const Ghost *ghost, Player *player)
{
    float dx = player->position.x - ghost->position.x;
    return (dx > -150.0f && dx < 10.0f);
}

const char **GetGhostDialog(int mapIndex, int *numLines)
{
    static const char *dialogMap1[] = {
        "Gareth II: Ahh! Quem é você?",
        "Cavaleiro Fantasma: Olá, eu sou um cavaleiro\nfantasma, irei te ajudar na sua jornada\naté o reino goblin!",
        "Gareth II: Ainda bem que você apareceu!\nQuem são esses goblins?",
        "Cavaleiro Fantasma: Eles são do reino de Gorzugar!\nUm antigo reino na Floresta Negra de Eldruin.",
        "Gareth II: Mas por que esse ataque?\nEstávamos em paz há 200 anos...",
        "Cavaleiro Fantasma: Foque em defender o reino,\ndepois conversamos mais sobre isso."
    };

    static const char *dialogMap2[] = {
        "Cavaleiro Fantasma: Vejo que você sobreviveu à floresta...",
        "Gareth II: Sim... mas encontrei algo estranho lá.",
        "Cavaleiro Fantasma: Cuidado. As sombras de Eldruin\nainda espreitam nas árvores.",
        "Gareth II: Parece que até o vento sussurra segredos antigos..."
    };

    static const char *dialogMap3[] = {
        "Cavaleiro Fantasma: O reino goblin está próximo.",
        "Gareth II: Já sinto o cheiro de enxofre no ar.",
        "Cavaleiro Fantasma: Este é o território deles...\nesteja pronto para lutar."
    };

    static const char *dialogMap4[] = {
        "Cavaleiro Fantasma: Finalmente chegamos ao castelo de Gorzugar.",
        "Gareth II: Então é aqui que tudo começou...",
        "Cavaleiro Fantasma: A verdade está além dessas portas..."
    };

    switch (mapIndex)
    {
        case 1: *numLines = sizeof(dialogMap1) / sizeof(dialogMap1[0]); return dialogMap1;
        case 2: *numLines = sizeof(dialogMap2) / sizeof(dialogMap2[0]); return dialogMap2;
        case 3: *numLines = sizeof(dialogMap3) / sizeof(dialogMap3[0]); return dialogMap3;
        case 4: *numLines = sizeof(dialogMap4) / sizeof(dialogMap4[0]); return dialogMap4;
        default: *numLines = 0; return NULL;
    }
}

void InitGhost(Ghost *ghost)
{
    ghost->position = (Vector2){200, 540};
    ghost->ghostIdle = LoadTexture("resources/sprites/npc/ghost-idle.png");
    ghost->ghostBtnE = LoadTexture("resources/sprites/btns/btn-E.png");
    ghost->ghostSpeech = LoadTexture("resources/sprites/npc/ghost-speech.png");
    ghost->ghostExclamation = LoadTexture("resources/sprites/npc/exclamation.png");

    int columnsInteraction = 3;  
    int rowsInteraction = 3; 

    ghost->frameIdle = 3;

    bool isInteraction = false;

    ghost->frameWidth = ghost->ghostIdle.width / ghost->frameIdle;
    ghost->frameHeight = ghost->ghostIdle.height;
    ghost->currentFrame = 0;
    ghost->frameCounter = 0;

    ghost->textFont = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250); 
}

void UpdateGhost(Ghost *ghost, Player *player, float delta, Interaction *interaction, DialogStateGhost *dialogStateGhost, float *dialogoTimer)
{
    if (ghost->isInteraction)
    {
        UpdateInteraction(interaction, delta);
    }
    
    ghost->frameCounter++;
    if (!ghost->isInteraction)
    {
        if (ghost->frameCounter >= (200 / 5))
        {
            ghost->frameCounter = 0;
            ghost->currentFrame = (ghost->currentFrame + 1) % ghost->frameIdle;
        }
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
        if (*dialogoTimer >= 1.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_GHOST_TALKING;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 5.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING2;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING2)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 3.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_GHOST_TALKING2;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING2)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 6.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING3;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING3)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 3.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_GHOST_TALKING3;
            *dialogoTimer = 0.0f;
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING3)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 4.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_CLOSED_GHOST;
            *dialogoTimer = 0.0f;
            ghost->isInteraction = false;
        }
    }
}

void DrawGhost(Ghost *ghost, Player *player, DialogStateGhost dialogStateGhost, Interaction *interaction, int currentMapIndex)
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

    Rectangle dest = {
        ghost->position.x,
        ghost->position.y,
        ghost->frameWidth / 2.5f,
        ghost->frameHeight / 2.5f
    };

    Vector2 origin = {0, 0};

    if (ghost->isInteraction)
    {   
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 160});
        DrawInteractionGhost(ghost, interaction);
    }
    else if (dialogStateGhost != DIALOG_CLOSED_GHOST)
    {   
        DrawTexturePro(ghost->ghostIdle, source, dest, origin, 0.0f, WHITE);
    }
    else if (InteractionWithGhost(ghost, player))
    {
        DrawTexture(ghost->ghostBtnE, ghost->position.x + 15, ghost->position.y - 35, WHITE);
    }
    else
    {
        DrawTexture(ghost->ghostExclamation, ghost->position.x + 25, ghost->position.y - 25, WHITE);
    }

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

   int numLines = 0;
   const char **lines = GetGhostDialog(currentMapIndex, &numLines);

    int lineIndex = -1;
    if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING) lineIndex = 0;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING) lineIndex = 1;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING2) lineIndex = 2;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING2) lineIndex = 3;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING3) lineIndex = 4;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING3) lineIndex = 5;

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

        if (dialogStateGhost == DIALOG_GHOST_TALKING || dialogStateGhost == DIALOG_GHOST_TALKING2 || dialogStateGhost == DIALOG_GHOST_TALKING3)
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
    DrawTexturePro(ghost->ghostIdle, source, dest, origin, 0.0f, WHITE);
}

void MapsGhost(Ghost *ghost, Player *player, DialogStateGhost dialogStateGhost, Interaction *interaction, float delta, int currentMapIndex)
{
    int mapsGhosts[4] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++)
    {
        if (currentMapIndex == mapsGhosts[i])
        {
            DrawGhost(ghost, player, dialogStateGhost, interaction, currentMapIndex);
            UpdateInteraction(interaction, delta);
        }
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
