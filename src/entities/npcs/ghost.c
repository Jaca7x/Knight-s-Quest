#include "ghost.h"

bool InteractionWithGhost(const Ghost *ghost, Player *player)
{
    float dx = player->position.x - ghost->position.x;
    return (dx > MIN_DISTANCE_FOR_INTERACTION_NPCS && dx < MAX_DISTANCE_FOR_INTERACTION_NPCS);
}

void PlayGhostSound(Ghost *ghost, int currentMapIndex, int dialogueIndex)
{
    if (currentMapIndex >= START_MAP && currentMapIndex <= NUM_MAPS)
    {
        Sound s = ghost->dialogues[currentMapIndex - PREVIOUS_FRAME][dialogueIndex].sound;

        if (s.frameCount > FRAME_COUNTER_ZERO)
        {
            StopSound(s);
            PlaySound(s);
        }
    }
}

const char **GetGhostDialog(int mapIndex, int *numLines)
{
    static const char *dialogs[GHOST_NUM_MAPS][DIALOGS_PER_MAP] = {
        {"Cavaleiro Fantasma: Olá, eu sou um cavaleiro\nfantasma, irei te ajudar na sua jornada\naté o reino goblin!",
         "Cavaleiro Fantasma: Eles são do reino de Gorzugar!\nUm antigo reino na Floresta Negra de Eldruin.",
         "Cavaleiro Fantasma: Foque em defender o reino,\ndepois conversamos mais sobre isso."},
        {"Cavaleiro Fantasma: São lobos sombrios de Eldruin,\neles vivem juntos com os goblins e atacam\nqualquer um que se aproxime.",
         "Cavaleiro Fantasma: Sim!, Cuidado, eles são rápidos\ne ferozes.\nUse sua espada com sabedoria.",
         "Cavaleiro Fantasma: Boa sorte, cavaleiro!."},
        {"Cavaleiro Fantasma: Sim, aqui sua jornada\nrealmente começa.\nO bosque estará tomado pelos goblins.",
         "Cavaleiro Fantasma: Lembre-se, cavaleiro, a\ncoragem e a sabedoria serão suas maiores armas.",
         "Cavaleiro Fantasma: Sim, esses monstros vermelhos\nsão mais fortes e brutos do que os outros,\ncuidado e redobre sua atenção."},
        {"Cavaleiro Fantasma: Sim, você está indo bem,\nmas a parte mais difícil ainda está por vir.",
         "Cavaleiro Fantasma: Logo à frente você irá chegar\nà floresta negra, onde é o reino goblin,\no mais perigoso de todos.",
         "Cavaleiro Fantasma: Exatamente, esses lobos\nbrancos são os mais rápidos e inteligentes,\neles são os guardiões da floresta goblin."}};

    *numLines = 3;
    return dialogs[mapIndex - 1];
}

const char **GetPlayerDialog(int mapIndex, int *numLines)
{
    static const char *dialogs[GHOST_NUM_MAPS][DIALOGS_PER_MAP] = {
        {
            "Ahh! Quem é você?!",
            "Ainda bem que apareceu! Quem são esses goblins?",
            "Mas... por que esse ataque? Estávamos em paz\nhá 200 anos!"
        },
        {
            "O quê é aquilo!?",
            "Preciso passar por eles para chegar até a floresta?",
            "Entendido, obrigado pela dica!"
        },
        {
            "Esse é o bosque de Arvendel?!",
            "Sim, terei que ser cauteloso daqui pra frente.",
            "Mas, espera aí aqueles monstros...\nSão vermelhos! eles parecem diferentes"
        },
        {
            "Ufa!\nFinalmente passei daqueles monstros vermelhos.",
            "O que quer dizer com isso?",
            "Parece que tenho mais desafios pela frente!\nUm lobo branco logo à frente!"
        }};

    *numLines = 3;
    return dialogs[mapIndex - 1];
}

void InitGhost(Ghost *ghost)
{
    // Position
    ghost->position = (Vector2){200, 540};

    // Sprites
    ghost->ghostIdle = LoadTexture("assets/resources/sprites/npc/ghost-idle.png");
    ghost->ghostBtnE = LoadTexture("assets/resources/sprites/btns/btn-E.png");
    ghost->ghostSpeech = LoadTexture("assets/resources/sprites/npc/ghost-speech.png");
    ghost->ghostExclamation = LoadTexture("assets/resources/sprites/npc/exclamation.png");

    //Sprite size
    ghost->frameWidth = ghost->ghostIdle.width / ghost->frameIdle;
    ghost->frameHeight = ghost->ghostIdle.height;

    ghost->scale = 2.5f;

    // Animation and control
    ghost->currentFrame = 0;
    ghost->frameCounter = 0;

    // Frames
    ghost->frameIdle = 3;
   
    // MAPA 1
    ghost->dialogues[0][0].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue1-1.wav");
    ghost->dialogues[0][1].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue1-2.wav");
    ghost->dialogues[0][2].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue1-3.wav");

    // MAPA 2
    ghost->dialogues[1][0].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue2-1.wav");
    ghost->dialogues[1][1].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue2-2.wav");
    ghost->dialogues[1][2].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue2-3.wav");

    // MAPA 3
    ghost->dialogues[2][0].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue3-1.wav");
    ghost->dialogues[2][1].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue3-2.wav");
    ghost->dialogues[2][2].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue3-3.wav");

    // MAPA 4
    ghost->dialogues[3][0].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue4-1.wav");
    ghost->dialogues[3][1].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue4-2.wav");
    ghost->dialogues[3][2].sound = LoadSound("assets/resources/sounds/voices/ghost/ghost-dialogue4-3.wav");

    // State
    bool isInteraction = false;

    // Text
    ghost->textFont = LoadFontEx("assets/resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);
    ghost->speechFontSize = 30;
}

void UpdateGhost(Ghost *ghost, Player *player, float delta, Interaction *interaction, DialogStateGhost *dialogStateGhost, float *dialogoTimer, int currentMapIndex)
{
    if (ghost->isInteraction)
    {
        UpdateInteraction(interaction, delta);
    }

    ghost->frameCounter++;
    if (!ghost->isInteraction)
    {
        if (ghost->frameCounter >= (NPCS_FRAME_SPEED))
        {
            ghost->frameCounter = FRAME_COUNTER_ZERO;
            ghost->currentFrame = (ghost->currentFrame + NEXT_FRAME) % ghost->frameIdle;
        }
    }

    if (*dialogStateGhost == DIALOG_CLOSED_GHOST)
    {
        ghost->isInteraction = false;

        if (InteractionWithGhost(ghost, player) && IsKeyPressed(KEY_E))
        {
            ghost->isInteraction = true;
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING;
            *dialogoTimer = TIMER_ZERO;
            PlayPlayerSound(player, currentMapIndex, DIALOGUE_ZERO);
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
            *dialogoTimer = TIMER_ZERO;
            PlayGhostSound(ghost, currentMapIndex, DIALOGUE_ZERO);
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 5.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING2;
            *dialogoTimer = TIMER_ZERO;
            PlayPlayerSound(player, currentMapIndex, DIALOGUE_ONE);
        }
    }
    else if (*dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING2)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 3.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_GHOST_TALKING2;
            *dialogoTimer = TIMER_ZERO;
            PlayGhostSound(ghost, currentMapIndex, DIALOGUE_ONE);
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING2)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 6.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_PLAYER_GHOST_TALKING3;
            *dialogoTimer = TIMER_ZERO;
            PlayPlayerSound(player, currentMapIndex, DIALOGUE_TWO);
        }
    }
    else if (*dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING3)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 3.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_GHOST_TALKING3;
            *dialogoTimer = TIMER_ZERO;
            PlayGhostSound(ghost, currentMapIndex, DIALOGUE_TWO);
        }
    }
    else if (*dialogStateGhost == DIALOG_GHOST_TALKING3)
    {
        ghost->isInteraction = true;
        *dialogoTimer += delta;
        if (*dialogoTimer >= 4.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStateGhost = DIALOG_CLOSED_GHOST;
            *dialogoTimer = TIMER_ZERO;
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
        SPRITE_ROW_BASE,
        ghost->frameWidth,
        ghost->frameHeight};

    Rectangle dest = {
        ghost->position.x,
        ghost->position.y,
        ghost->frameWidth / ghost->scale,
        ghost->frameHeight / ghost->scale};

    Vector2 origin = ORIGIN_TOPLEFT;

    if (ghost->isInteraction)
    {
        DrawRectangle(POSITION_ZERO, POSITION_ZERO, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 160});
        DrawInteractionGhost(ghost, interaction);
    }
    else if (dialogStateGhost != DIALOG_CLOSED_GHOST)
    {
        DrawTexturePro(ghost->ghostIdle, source, dest, origin, ROTATION, WHITE);
    }
    else if (InteractionWithGhost(ghost, player))
    {
        DrawTexture(ghost->ghostBtnE, ghost->position.x + GHOST_INTERACTION_OFFSET_X, ghost->position.y + GHOST_INTERACTION_OFFSET_Y, WHITE);
    }
    else
    {
        DrawTexture(ghost->ghostExclamation, ghost->position.x + GHOST_EXCLAMATION_OFFSET, ghost->position.y - GHOST_EXCLAMATION_OFFSET, WHITE);
    }

    int hintFontSize = 14;
    float textSpacing = 0.0f;

    int speechMarginBottom = 10;
    int speechMarginSide = 20;
    int speechMarginSideNpc = 20;

    int playerSpeechTextOffsetX = 210;
    int playerSpeechTextOffsetY = 80;

    int npcSpeechTextOffsetX = 30;
    int npcSpeechTextOffsetY = 80;

    int speechY = GetScreenHeight() - ghost->ghostSpeech.height - speechMarginBottom;
    int ghostSpeechX = GetScreenWidth() - ghost->ghostSpeech.width - speechMarginSideNpc;
    int playerSpeechX = speechMarginSide;

    int nextMsgTextPlayerX = 246;
    int nextMsgTextNpcX = 996;
    int nextMsgTextY = 880;

    int numLinesGhost = 0;
    int numLinesPlayer = 0;

    const char **ghostLines = GetGhostDialog(currentMapIndex, &numLinesGhost);
    const char **playerLines = GetPlayerDialog(currentMapIndex, &numLinesPlayer);

    const char **lines = NULL;

    if (dialogStateGhost == DIALOG_GHOST_TALKING ||
        dialogStateGhost == DIALOG_GHOST_TALKING2 ||
        dialogStateGhost == DIALOG_GHOST_TALKING3)
    {
        lines = ghostLines;
    }
    else
    {
        lines = playerLines;
    }

    int lineIndex = -1;
    if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING)
        lineIndex = DIALOGUE_ZERO;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING)
        lineIndex = DIALOGUE_ZERO;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING2)
        lineIndex = DIALOGUE_ONE;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING2)
        lineIndex = DIALOGUE_ONE;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING3)
        lineIndex = DIALOGUE_TWO;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING3)
        lineIndex = DIALOGUE_TWO;

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
            timeWriting = TIMER_ZERO;
        }

        if (dialogStateGhost == DIALOG_GHOST_TALKING || dialogStateGhost == DIALOG_GHOST_TALKING2 || dialogStateGhost == DIALOG_GHOST_TALKING3)
        {
            DrawTexture(ghost->ghostSpeech, ghostSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], POSITION_ZERO, visibleLetters),
                       (Vector2){ghostSpeechX + npcSpeechTextOffsetX, speechY + npcSpeechTextOffsetY},
                       ghost->speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para continuar", nextMsgTextNpcX, nextMsgTextY, hintFontSize, BLACK);
        }
        else
        {
            DrawTexture(player->playerSpeech, playerSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], POSITION_ZERO, visibleLetters),
                       (Vector2){playerSpeechX + playerSpeechTextOffsetX, speechY + playerSpeechTextOffsetY},
                       ghost->speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para continuar", nextMsgTextPlayerX, nextMsgTextY, hintFontSize, BLACK);
        }
    }

    DrawTexturePro(ghost->ghostIdle, source, dest, origin, ROTATION, WHITE);
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

    for (int map = 0; map < NUM_MAPS; map++)
    {
        for (int i = 0; i < 6; i++)
        {
            if (ghost->dialogues[map][i].sound.frameCount > FRAME_COUNTER_ZERO)
            {
                UnloadSound(ghost->dialogues[map][i].sound);
            }
        }
    }
}
