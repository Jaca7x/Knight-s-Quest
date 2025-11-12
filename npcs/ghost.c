#include "ghost.h"

bool InteractionWithGhost(const Ghost *ghost, Player *player)
{
    float dx = player->position.x - ghost->position.x;
    return (dx > -150.0f && dx < 10.0f);
}

void PlayGhostSound(Ghost *ghost, int currentMapIndex, int dialogueIndex)
{
    if (currentMapIndex >= 1 && currentMapIndex <= 4)
    {
        Sound s = ghost->dialogues[currentMapIndex - 1][dialogueIndex].sound;

        if (s.frameCount > 0)  
        {
            StopSound(s);
            PlaySound(s);
        }
    }
}

const char **GetGhostDialog(int mapIndex, int *numLines)
{
    static const char *dialogs[4][3] = {
        {
            "Cavaleiro Fantasma: Olá, eu sou um cavaleiro\nfantasma, irei te ajudar na sua jornada\naté o reino goblin!",
            "Cavaleiro Fantasma: Eles são do reino de Gorzugar!\nUm antigo reino na Floresta Negra de Eldruin.",
            "Cavaleiro Fantasma: Foque em defender o reino,\ndepois conversamos mais sobre isso."
        },
        {
            "Cavaleiro Fantasma: São lobos sombrios de Eldruin,\neles vivem juntos com os goblins e atacam\nqualquer um que se aproxime.",
            "Cavaleiro Fantasma: Sim!, Cuidado, eles são rápidos\ne ferozes.\nUse sua espada com sabedoria.",
            "Cavaleiro Fantasma: Boa sorte, cavaleiro!."
        },
        {
            "Cavaleiro Fantasma: Sim, aqui sua jornada\nrealmente começa.\nO bosque estará tomado pelos goblins.",
            "Cavaleiro Fantasma: Lembre-se, cavaleiro, a\ncoragem e a sabedoria serão suas maiores armas.",
            "Cavaleiro Fantasma: Sim, esses monstros vermelhos\nsão mais fortes e brutos do que os outros,\ncuidado e redobre sua atenção."
        },
        {
            "Cavaleiro Fantasma: Sim, você está indo bem,\nmas a parte mais difícil ainda está por vir.",
            "Cavaleiro Fantasma: Logo à frente você irá chegar\nà floresta negra, onde é o reino goblin,\no mais perigoso de todos.",
            "Cavaleiro Fantasma: Exatamente, esses lobos\nbrancos são os mais rápidos e inteligentes,\neles são os guardiões da floresta goblin."
        }
    };

    *numLines = 3;
    return dialogs[mapIndex - 1];
}


const char **GetPlayerDialog(int mapIndex, int *numLines)
{
    static const char *dialogs[4][3] = {
        {
            "Ahh! Quem é você?!",
            "Ainda bem que apareceu! Quem são esses goblins?",
            "Mas... por que esse ataque? Estávamos em paz há 200 anos!"
        },
        {
            "O quê é aquilo!?",
            "Preciso passar por eles para chegar até a floresta?",
            "Entendido, obrigado pela dica!"
        },
        {
            "Esse é o bosque de Arvendel?!",
            "Sim, terei que ser cauteloso daqui pra frente.",
            "Mas... espera aí, aqueles monstros são vermelhos?!"
        },
        {
            "Ufa! Finalmente passei daqueles monstros vermelhos.",
            "O que quer dizer com isso?",
            "Parece que tenho mais desafios pela frente!"
        }
    };

    *numLines = 3;
    return dialogs[mapIndex - 1];
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

    ghost->dialogues[0][0].text = "Cavaleiro Fantasma: Olá, eu sou um cavaleiro\nfantasma, irei te ajudar na sua jornada\naté o reino goblin!";
    ghost->dialogues[0][0].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue1-1.wav");

    ghost->dialogues[0][1].text = "Cavaleiro Fantasma: Eles são do reino de Gorzugar!\nUm antigo reino na Floresta Negra de Eldruin.";
    ghost->dialogues[0][1].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue1-2.wav");

    ghost->dialogues[0][2].text = "Cavaleiro Fantasma: Foque em defender o reino,\ndepois conversamos mais sobre isso.";
    ghost->dialogues[0][2].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue1-3.wav");

    // MAPA 2
    ghost->dialogues[1][0].text = "Cavaleiro Fantasma: São lobos sombrios de Eldruin,\neles vivem juntos com os goblins e atacam\nqualquer um que se aproxime.";
    ghost->dialogues[1][0].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue2-1.wav");

    ghost->dialogues[1][1].text = "Cavaleiro Fantasma: Sim!, Cuidado, eles são rápidos\ne ferozes.\nUse sua espada com sabedoria.";
    ghost->dialogues[1][1].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue2-2.wav");

    ghost->dialogues[1][2].text = "Cavaleiro Fantasma: Boa sorte, cavaleiro!.";
    ghost->dialogues[1][2].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue2-3.wav");

    // MAPA 3
    ghost->dialogues[2][0].text = "Cavaleiro Fantasma: Sim, aqui sua jornada\nrealmente começa.\nO bosque estará tomado pelos goblins.";
    ghost->dialogues[2][0].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue3-1.wav");

    ghost->dialogues[2][1].text = "Cavaleiro Fantasma: Lembre-se, cavaleiro, a\ncoragem e a sabedoria serão suas maiores armas.";
    ghost->dialogues[2][1].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue3-2.wav");

    ghost->dialogues[2][2].text = "Cavaleiro Fantasma: Sim, esses monstros vermelhos\nsão mais fortes e brutos do que os outros,\ncuidado e redobre sua atenção.";
    ghost->dialogues[2][2].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue3-3.wav");

    // MAPA 4
    ghost->dialogues[3][0].text = "Cavaleiro Fantasma: Sim, você está indo bem,\nmas a parte mais difícil ainda está por vir.";
    ghost->dialogues[3][0].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue4-1.wav");

    ghost->dialogues[3][1].text = "Cavaleiro Fantasma: Logo à frente você irá chegar\nà floresta negra, onde é o reino goblin,\no mais perigoso de todos.";
    ghost->dialogues[3][1].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue4-2.wav");

    ghost->dialogues[3][2].text = "Cavaleiro Fantasma: Exatamente, esses lobos\nbrancos são os mais rápidos e inteligentes,\neles são os guardiões da floresta goblin.";
    ghost->dialogues[3][2].sound = LoadSound("resources/sounds/voices/ghost/ghost-dialogue4-3.wav");

    bool isInteraction = false;

    ghost->frameWidth = ghost->ghostIdle.width / ghost->frameIdle;
    ghost->frameHeight = ghost->ghostIdle.height;
    ghost->currentFrame = 0;
    ghost->frameCounter = 0;

    ghost->textFont = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250); 
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
            PlayPlayerSound(player, currentMapIndex, 0);
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
            PlayGhostSound(ghost, currentMapIndex, 0);
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
            PlayPlayerSound(player, currentMapIndex, 1);
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
            PlayGhostSound(ghost, currentMapIndex, 1);
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
            PlayPlayerSound(player, currentMapIndex, 2);
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
            PlayGhostSound(ghost, currentMapIndex, 2);
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
    if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING) lineIndex = 0;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING) lineIndex = 0;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING2) lineIndex = 1;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING2) lineIndex = 1;
    else if (dialogStateGhost == DIALOG_PLAYER_GHOST_TALKING3) lineIndex = 2;
    else if (dialogStateGhost == DIALOG_GHOST_TALKING3) lineIndex = 2;

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
