#include "peasant.h"

bool InteractionWithPeasant(const Peasant *peasant, Player *player)
{
    float dx = player->position.x - peasant->position.x;
    return (dx > -150.0f && dx < 10.0f);
}

int GetPeasantDialogIndexByMap(int currentMapIndex)
{
    switch (currentMapIndex)
    {
        case 6: return 0;
        case 7: return 1;
        case 8: return 2;
        default: return -1;
    }
}

const char **GetPlayerDialogPeasant(int mapIndex, int *numLines)
{
    static const char *dialogs[3][3] = {
        {
            "Gareth II: Ei! Você aí! O que está fazendo\n no meio dessa floresta infestada de goblins?!",
            "Gareth II: Você enlouqueceu? Eles vão te matar!",
            "Gareth II: Você veio sozinho?! Volte para\na vila agora antes que seja tarde demais!"
        },
        {
            "Gareth II: Vamos rápido, antes que os goblins\nnos vejam!",
            "Gareth II: Você está louco? Você é apenas um\nfazendeiro, está indo para a morte!",
            "Gareth II: Certo, vamos mais rápido.\nNão podemos perder mais tempo aqui!" 
        },
        {
            "Gareth II: Mas… que estranho, há tão poucos\ngoblins por aqui…",
            "Gareth II: Sim, algo não está certo. Sinto que estamos\nsendo observados…",
            "Gareth II: CUIDADO!!"
        },
     };

    *numLines = 3;
    return dialogs[mapIndex];
}

const char **GetPeasantDialog(int mapIndex, int *numLines)
{
    static const char *dialogs[3][3] = {
        {
            "Fazendeiro: Shhh! Fale baixo… essas pragas têm\nouvidos melhores do que parecem.",
            "Fazendeiro: Eu sei... mas minhas cabras fugiram\npra cá. Sem elas, minha família passa fome.",
            "Fazendeiro: Acho que os goblins as levaram até seu\ncovil mais adiante na floresta, me ajude a\nencontra-las!"
        },
        {
            "Fazendeiro: Fique alerta agora, há cabanas de\ngoblins por toda parte.\nEstamos chegando perto do reino.",
            "Fazendeiro: Não estou vendo minhas cabras ainda...\nacho que estão ainda mais à frente!",
            "Fazendeiro: Prefiro não pensar nisso agora..."

        },
        {
            "Fazendeiro: Minha nossa, este deve ser o centro\ndo reino goblin…",
            "Fazendeiro: Sim, é melhor ficarmos longe daqui e\nvoltarmos para a vila.",
            "Fazendeiro: O QUE É AQUILO CAINDO DAS\nÁRVORES?!"
        },
    };

    *numLines = 3;
    return dialogs[mapIndex];
}

void InitPeasant(Peasant *peasant)
{
    peasant->position = (Vector2){50.0f, 526.0f };

    peasant->peasantSpeech = LoadTexture("resources/sprites/npc/peasant-speech.png");
    peasant->peasantIdle = LoadTexture("resources/sprites/npc/peasant.png");
    peasant->peasantBtnE = LoadTexture("resources/sprites/btns/btn-E.png");
    peasant->peasantExclamation = LoadTexture("resources/sprites/npc/exclamation.png");

    peasant->dialogues[0][0].text =
    "Shhh! Fale baixo… essas pragas têm ouvidos melhores do que parecem.";

    peasant->dialogues[0][1].text =
    "Eu sei… mas minhas cabras fugiram pra cá. Sem elas, minha família passa fome.";

    peasant->dialogues[0][2].text =
    "Acho que os goblins as levaram até seu covil mais adiante na floresta, me ajude a encontra-las!";
    
    peasant->dialogues[1][0].text =
    "Fique alerta agora, há cabanas de goblins por toda parte. Estamos chegando perto do reino.";

    peasant->dialogues[1][1].text =
    "Não estou vendo minhas cabras ainda… acho que estão ainda mais à frente?";

    peasant->dialogues[1][2].text =
    "Prefiro não pensar nisso agora…";

    peasant->dialogues[2][0].text =
    "Minha nossa, este deve ser o centro do reino goblin…";

    peasant->dialogues[2][1].text =
    "Sim, é melhor ficarmos longe daqui e voltarmos para a vila.";

    peasant->dialogues[2][2].text =
    "O QUE É AQUILO CAINDO DAS ÁRVORES?!";

    peasant->frameIdle = 2;

    peasant->isInteraction = false;

    peasant->frameWidth = peasant->peasantIdle.width / peasant->frameIdle;
    peasant->frameHeight = peasant->peasantIdle.height;
    peasant->currentFrame = 0;
    peasant->frameCounter = 0;

    peasant->textFont = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);

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
        if (peasant->frameCounter >= (200 / 5))
        {
            peasant->frameCounter = 0;
            peasant->currentFrame = (peasant->currentFrame + 1) % peasant->frameIdle;
        }
    }

    if (*dialogStatePeasant == DIALOG_CLOSED_PEASANT)
    {
        peasant->isInteraction = false;
        
        if (InteractionWithPeasant(peasant, player) && IsKeyPressed(KEY_E))
        {
            peasant->isInteraction = true;
            *dialogStatePeasant = DIALOG_PLAYER_PEASANT_TALKING;
            *dialogoTimerPeasant = 0.0f;
            //PlayPlayerSound(player, currentMapIndex, 0);
        }
        return;
    }
    peasant->isInteraction = true;

    if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 1.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PEASANT_TALKING;
            *dialogoTimerPeasant = 0.0f;
            //PlayPeasantSound(peasant, currentMapIndex, 0);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 5.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PLAYER_PEASANT_TALKING2;
            *dialogoTimerPeasant = 0.0f;
           // PlayPlayerSound(player, currentMapIndex, 1);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING2)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 3.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PEASANT_TALKING2;
            *dialogoTimerPeasant = 0.0f;
            //PlayPeasantSound(peasant, currentMapIndex, 1);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING2)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 6.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PLAYER_PEASANT_TALKING3;
            *dialogoTimerPeasant = 0.0f;
            //PlayPlayerSound(player, currentMapIndex, 2);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING3)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 3.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_PEASANT_TALKING3;
            *dialogoTimerPeasant = 0.0f;
            //PlayPeasantSound(peasant, currentMapIndex, 2);
        }
    }
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING3)
    {
        peasant->isInteraction = true;
        *dialogoTimerPeasant += deltaTime;
        if (*dialogoTimerPeasant >= 4.5f && IsKeyPressed(KEY_SPACE))
        {
            *dialogStatePeasant = DIALOG_CLOSED_PEASANT;
            *dialogoTimerPeasant = 0.0f;
            peasant->isInteraction = false;
            *bossTriggered = true;
        }
    }
}

void DrawPeasant(Peasant *peasant, Player *player, Interaction *interaction, int currentMapIndex, DialogStatePeasant *dialogStatePeasant)
{

    static int visibleLetters = 0;
    static float timeWriting = 0.0f;
    float writingSpeed = 0.04f; 

    Font textSpeech = peasant->textFont;

    Rectangle source = {
        peasant->currentFrame * peasant->frameWidth,
        0,
        peasant->frameWidth,
        peasant->frameHeight
    };

    Rectangle dest = {
        peasant->position.x,
        peasant->position.y,
        peasant->frameWidth / 7.5f,
        peasant->frameHeight / 7.5f
    };

    Vector2 origin = {0, 0};

    if (peasant->isInteraction)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 160});
        DrawInteractionPeasant(peasant, interaction);
    }
    else if (InteractionWithPeasant(peasant, player))
    {
        DrawTexture(peasant->peasantBtnE, peasant->position.x + 10, peasant->position.y - 40, WHITE);
    }
    else 
    {
        DrawTexture(peasant->peasantExclamation, peasant->position.x + 15, peasant->position.y - 25, WHITE);
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
    if (dialogIndex < 0) return;

    const char **peasantLines = GetPeasantDialog(dialogIndex, &numLinespeasant);
    const char **playerLines  = GetPlayerDialogPeasant(dialogIndex, &numLinesPlayer);


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
    if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING) lineIndex = 0;
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING) lineIndex = 0;
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING2) lineIndex = 1;
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING2) lineIndex = 1;
    else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING3) lineIndex = 2;
    else if (*dialogStatePeasant == DIALOG_PEASANT_TALKING3) lineIndex = 2;

    static DialogStatePeasant lastState = DIALOG_CLOSED_PEASANT; 
    if (*dialogStatePeasant != lastState)
    {
        visibleLetters = 0;
        timeWriting = 0.0f;
    }
    lastState = *dialogStatePeasant;

    if (lineIndex >= 0)
    {
        timeWriting += GetFrameTime();
        if (timeWriting >= writingSpeed && visibleLetters < (int)strlen(lines[lineIndex]))
        {
            visibleLetters++;
            timeWriting = 0.0f;
        }

        if (*dialogStatePeasant == DIALOG_PEASANT_TALKING || *dialogStatePeasant == DIALOG_PEASANT_TALKING2 || *dialogStatePeasant == DIALOG_PEASANT_TALKING3)
        {
            DrawTexture(peasant->peasantSpeech, peasantSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], 0, visibleLetters),
                       (Vector2){peasantSpeechX + npcSpeechTextOffsetX, speechY + npcSpeechTextOffsetY},
                       peasant->speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para continuar", nextMsgTextNpcX, nextMsgTextY, hintFontSize, BLACK);
        }
        else if (*dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING || *dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING2 || *dialogStatePeasant == DIALOG_PLAYER_PEASANT_TALKING3)
        {
            DrawTexture(player->playerSpeech, playerSpeechX, speechY, WHITE);
            DrawTextEx(textSpeech,
                       TextSubtext(lines[lineIndex], 0, visibleLetters),
                       (Vector2){playerSpeechX + playerSpeechTextOffsetX, speechY + playerSpeechTextOffsetY},
                       peasant->speechFontSize, textSpacing, BLACK);

            if (visibleLetters >= (int)strlen(lines[lineIndex]))
                DrawText("Pressione ESPAÇO para continuar", nextMsgTextPlayerX, nextMsgTextY, hintFontSize, BLACK);
        }
    }

    DrawTexturePro(peasant->peasantIdle, source, dest, origin, 0.0f, WHITE);
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

    for (int i = 0; i < NUM_MAPS; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            UnloadSound(peasant->dialogues[i][j].sound);
        }
    }

    UnloadFont(peasant->textFont);
}