#include "peasant.h"

bool InteractionWithPeasant(const Peasant *peasant, Player *player)
{
    float dx = player->position.x - peasant->position.x;
    return (dx > MIN_DISTANCE_FOR_INTERACTION_NPCS && dx < MAX_DISTANCE_FOR_INTERACTION_NPCS);
}

int GetPeasantDialogIndexByMap(int currentMapIndex)
{
    switch (currentMapIndex)
    {
    case 6:
        return DIALOGUE_ZERO;
    case 7:
        return DIALOGUE_ONE;
    case 8:
        return DIALOGUE_TWO;
    default:
        return -1;
    }
}

void PlayPeasantSound(Peasant *peasant, int currentMapIndex, int dialogueIndex)
{
    if (currentMapIndex >= 6 && currentMapIndex <= 8)
    {
        Sound s = peasant->dialogues[currentMapIndex - 6][dialogueIndex].sound;
        if (s.frameCount > FRAME_COUNTER_ZERO)
        {
            StopSound(s);
            PlaySound(s);
        }
    }
}

const char **GetPlayerDialogPeasant(int mapIndex, int *numLines)
{
    static const char *dialogs[PEASANT_NUM_MAPS][DIALOGS_PER_MAP] =
        {
            {"Gareth II: Ei! Você aí! O que está fazendo\n no meio dessa floresta infestada de goblins?!",
             "Gareth II: Você enlouqueceu? Eles vão te matar!",
             "Gareth II: Você veio sozinho?! Volte para\na vila agora antes que seja tarde demais!"},
            {"Gareth II: Vamos rápido, antes que os goblins\nnos vejam!",
             "Gareth II: Você está louco? Você é apenas um\nfazendeiro, está indo para a morte!",
             "Gareth II: Certo, vamos mais rápido.\nNão podemos perder mais tempo aqui!"},
            {"Gareth II: Mas… que estranho, há tão poucos\ngoblins por aqui...",
             "Gareth II: Sim, algo não está certo. Sinto\nque estamos sendo observados...",
             "Gareth II: ESCONDA-SE!"},
        };

    *numLines = 3;
    return dialogs[mapIndex];
}

const char **GetPeasantDialog(int mapIndex, int *numLines)
{
    static const char *dialogs[PEASANT_NUM_MAPS][DIALOGS_PER_MAP] =
        {
            {"Fazendeiro: Shhh! Fale baixo… essas pragas têm\nouvidos melhores do que parecem.",
             "Fazendeiro: Eu sei... mas minhas cabras fugiram\npra cá. Sem elas, minha família passa fome.",
             "Fazendeiro: Acho que os goblins as levaram até seu\ncovil mais adiante na floresta, me ajude a\nencontra-las!"},
            {"Fazendeiro: Fique alerta agora, há cabanas de\ngoblins por toda parte.\nEstamos chegando perto do reino.",
             "Fazendeiro: Não estou vendo minhas cabras ainda...\nacho que estão ainda mais à frente!",
             "Fazendeiro: Prefiro não pensar nisso agora..."

            },
            {"Fazendeiro: Minha nossa, este deve ser o centro\ndo reino goblin…",
             "Fazendeiro: Sim, é melhor ficarmos longe daqui e\nvoltarmos para a vila.",
             "Fazendeiro: O QUE É AQUILO CAINDO DAS\nÁRVORES?!"},
        };

    *numLines = 3;
    return dialogs[mapIndex];
}

void InitPeasant(Peasant *peasant)
{
    // Position
    peasant->position = (Vector2){50.0f, 526.0f};

    // Sprites
    peasant->peasantSpeech = LoadTexture("assets/resources/sprites/npc/peasant-speech.png");
    peasant->peasantIdle = LoadTexture("assets/resources/sprites/npc/peasant.png");
    peasant->peasantBtnE = LoadTexture("assets/resources/sprites/btns/btn-E.png");
    peasant->peasantExclamation = LoadTexture("assets/resources/sprites/npc/exclamation.png");

    // Sprites size
    peasant->frameWidth = peasant->peasantIdle.width / peasant->frameIdle;
    peasant->frameHeight = peasant->peasantIdle.height;

    // Scale
    peasant->scale = 7.50f;

    // Frames 
    peasant->frameIdle = 2;

    // Map 1
    peasant->dialogues[0][0].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue1-0.wav");

    peasant->dialogues[0][1].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue1-1.wav");

    peasant->dialogues[0][2].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue1-2.wav");

    // Map 2
    peasant->dialogues[1][0].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue2-0.wav");

    peasant->dialogues[1][1].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue2-1.wav");

    peasant->dialogues[1][2].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue2-2.wav");

    // Map 3
    peasant->dialogues[2][0].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue3-0.wav");

    peasant->dialogues[2][1].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue3-1.wav");

    peasant->dialogues[2][2].sound = 
        LoadSound("assets/resources/sounds/voices/peasant/peasant-dialogue3-2.wav");

    // States
    peasant->isInteraction = false;
    peasant->dialogueFinished = false;

    // Animation and control
    peasant->currentFrame = 0;
    peasant->frameCounter = 0;

    // Font
    peasant->textFont = LoadFontEx("assets/resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);
    peasant->speechFontSize = 30;
}

void UpdatePeasant(Peasant *peasant, Player *player, float deltaTime, Interaction *interaction, DialogStatePeasant *dialogStatePeasant, float *dialogoTimerPeasant, int currentMapIndex, bool *bossTriggered)
{
    if (peasant->isInteraction)
    {
        UpdateInteraction(interaction, deltaTime);
    }

    peasant->frameCounter++;
    if (!peasant->isInteraction)
    {
        if (peasant->frameCounter >= NPCS_FRAME_SPEED)
        {
            peasant->frameCounter = FRAME_COUNTER_ZERO;
            peasant->currentFrame = (peasant->currentFrame + NEXT_FRAME) % peasant->frameIdle;
        }
    }

    if (*dialogStatePeasant == DIALOG_CLOSED_PEASANT)
    {
        peasant->isInteraction = false;

        if (InteractionWithPeasant(peasant, player) && IsKeyPressed(KEY_E))
        {
            peasant->isInteraction = true;
            *dialogStatePeasant = DIALOG_PLAYER_PEASANT_TALKING;
            *dialogoTimerPeasant = TIMER_ZERO;
            PlayPlayerSoundWithPeasant(player, currentMapIndex, DIALOGUE_ZERO);
        }
        return;
    }
    peasant->isInteraction = true;

    if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 4.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PEASANT_TALKING;
            *dialogoTimerPeasant = TIMER_ZERO;
            PlayPeasantSound(peasant, currentMapIndex, DIALOGUE_ZERO);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 5.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PLAYER_PEASANT_TALKING2;
            *dialogoTimerPeasant = TIMER_ZERO;
            PlayPlayerSoundWithPeasant(player, currentMapIndex, DIALOGUE_ONE);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING2)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 4.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PEASANT_TALKING2;
            *dialogoTimerPeasant = TIMER_ZERO;
            PlayPeasantSound(peasant, currentMapIndex, DIALOGUE_ONE);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING2)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 5.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PLAYER_PEASANT_TALKING3;
            *dialogoTimerPeasant = TIMER_ZERO;
            PlayPlayerSoundWithPeasant(player, currentMapIndex, DIALOGUE_TWO);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING3)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 3.0f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PEASANT_TALKING3;
            *dialogoTimerPeasant = TIMER_ZERO;
            PlayPeasantSound(peasant, currentMapIndex, DIALOGUE_TWO);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING3)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 4.0f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_CLOSED_PEASANT;
            *dialogoTimerPeasant = TIMER_ZERO;
            peasant->isInteraction = false;

            if (currentMapIndex == PEASANT_LAST_MAP_DIALOGUE)
            {
                peasant->dialogueFinished = true;
            }
        }
    }
}

void DrawPeasant(Peasant *peasant, Player *player, Interaction *interaction, int currentMapIndex, DialogStatePeasant *dialogStatePeasant)
{

    static int visibleLetters = 0;
    static float timeWriting = TIMER_ZERO;
    float writingSpeed = 0.04f;

    Font textSpeech = peasant->textFont;

    Rectangle source = 
    {
        peasant->currentFrame * peasant->frameWidth,
        SPRITE_ROW_BASE,
        peasant->frameWidth,
        peasant->frameHeight
    };

    Rectangle dest = 
    {
        peasant->position.x,
        peasant->position.y,
        peasant->frameWidth / peasant->scale,
        peasant->frameHeight / peasant->scale
    };

    Vector2 origin = ORIGIN_TOPLEFT;

    if (peasant->isInteraction)
    {
        DrawRectangle(POSITION_ZERO, POSITION_ZERO, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 160});
        DrawInteractionPeasant(peasant, interaction);
    }
    else if (InteractionWithPeasant(peasant, player))
    {
        DrawTexture(peasant->peasantBtnE, peasant->position.x + PEASANT_OFFSET_X_INTERACTION, peasant->position.y - PEASANT_OFFSET_Y_INTERACTION, WHITE);
    }
    else
    {
        DrawTexture(peasant->peasantExclamation, peasant->position.x + PEASANT_OFFSET_X_EXCLAMATION, peasant->position.y - PEASANT_OFFSET_Y_EXCLAMATION, WHITE);
    }

    int hintFontSize = 14;
    float textSpacing = 0.0f;

    int speechMarginBottom = 10;
    int speechMarginSide = 20;
    int speechMarginSideNpc = 20;

    int playerSpeechTextOffsetX = 210;
    int playerSpeechTextOffsetY = 80;

    int npcSpeechTextOffsetX;

    if (peasant->speechFontSize >= 30)
    {
        npcSpeechTextOffsetX = 30;
    }
    else
    {
        npcSpeechTextOffsetX = 100;
    }

    int npcSpeechTextOffsetY = 80;

    int speechY = GetScreenHeight() - peasant->peasantSpeech.height - speechMarginBottom;
    int peasantSpeechX = GetScreenWidth() - peasant->peasantSpeech.width - speechMarginSideNpc;
    int playerSpeechX = speechMarginSide;

    int nextMsgTextPlayerX = 246;
    int nextMsgTextNpcX = 996;
    int nextMsgTextY = 880;

    int numLinespeasant = 0;
    int numLinesPlayer = 0;

    int dialogIndex = GetPeasantDialogIndexByMap(currentMapIndex);
    if (dialogIndex < DIALOGUE_ZERO)
        return;

    const char **peasantLines = GetPeasantDialog(dialogIndex, &numLinespeasant);
    const char **playerLines = GetPlayerDialogPeasant(dialogIndex, &numLinesPlayer);

    const char **lines = NULL;

    if (*dialogStatePeasant == DIALOG_PEASANT_TALKING ||
        *dialogStatePeasant == DIALOG_PEASANT_TALKING2 ||
        *dialogStatePeasant == DIALOG_PEASANT_TALKING3)
    {
        lines = peasantLines;
    }
    else
    {
        lines = playerLines;
    }

    int lineIndex = -1;
    if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING)
        lineIndex = DIALOGUE_ZERO;
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING)
        lineIndex = DIALOGUE_ZERO;
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING2)
        lineIndex = DIALOGUE_ONE;
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING2)
        lineIndex = DIALOGUE_ONE;
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING3)
        lineIndex = DIALOGUE_TWO;
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING3)
        lineIndex = DIALOGUE_TWO;

    static DialogStatePeasant lastState = DIALOG_CLOSED_PEASANT;
    if (*dialogStatePeasant != lastState)
    {
        visibleLetters = 0;
        timeWriting = TIMER_ZERO;
    }
    lastState = *dialogStatePeasant;

    if (lineIndex >= DIALOGUE_ZERO)
    {
        timeWriting += GetFrameTime();
        if (timeWriting >= writingSpeed && visibleLetters < (int)strlen(lines[lineIndex]))
        {
            visibleLetters++;
            timeWriting = TIMER_ZERO;
        }

        if (*dialogStatePeasant == DIALOG_PEASANT_TALKING || *dialogStatePeasant == DIALOG_PEASANT_TALKING2 || *dialogStatePeasant == DIALOG_PEASANT_TALKING3)
        {
            DrawTexture(peasant->peasantSpeech, peasantSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], POSITION_ZERO, visibleLetters),
                       (Vector2){peasantSpeechX + npcSpeechTextOffsetX, speechY + npcSpeechTextOffsetY},
                       peasant->speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para continuar", nextMsgTextNpcX, nextMsgTextY, hintFontSize, BLACK);
        }
        else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING || *dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING2 || *dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING3)
        {
            DrawTexture(player->playerSpeech, playerSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], POSITION_ZERO, visibleLetters),
                       (Vector2){playerSpeechX + playerSpeechTextOffsetX, speechY + playerSpeechTextOffsetY},
                       peasant->speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para continuar", nextMsgTextPlayerX, nextMsgTextY, hintFontSize, BLACK);
        }
    }

    DrawTexturePro(peasant->peasantIdle, source, dest, origin, ROTATION, WHITE);
}

void DrawMapsPeasant(Peasant *peasant, Player *player, Interaction *interaction, int currentMapIndex, float delta, DialogStatePeasant *dialogStatePeasant)
{
    int mapsPeasants[3] = {6, 7, 8};

    for (int i = 0; i < 3; i++)
    {
        if (currentMapIndex == mapsPeasants[i])
        {
            DrawPeasant(peasant, player, interaction, currentMapIndex, dialogStatePeasant);
            UpdateInteraction(interaction, delta);
        }
    }
}

void UnloadPeasant(Peasant *peasant)
{
    UnloadTexture(peasant->peasantSpeech);
    UnloadTexture(peasant->peasantIdle);
    UnloadTexture(peasant->peasantBtnE);
    UnloadTexture(peasant->peasantExclamation);

    UnloadFont(peasant->textFont);

    for (int map = 0; map < NUM_MAPS; map++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (peasant->dialogues[map][i].sound.frameCount > FRAME_COUNTER_ZERO)
            {
                UnloadSound(peasant->dialogues[map][i].sound);
            }
        }
    }
}