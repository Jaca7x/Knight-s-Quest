// ============================================================================
// Inclusão de bibliotecas
// ============================================================================
#include "librays/raylib.h"       
#include "librays/raymath.h"
#include "librays/cJSON.h"        
#include <stdlib.h>               
#include <stdio.h>                
#include "player/player.h"       
#include "staminaBar/stamina.h"   
#include "wolf/wolf.h"
#include "wolf/wolfRunning.h"
#include "lifeBar/lifeBar.h"
#include "goblin/goblin.h"
#include "goblin/goblinArcher.h"
#include "drops/heart.h"         
#include "npcs/npc.h"   
#include "npcs/ghost.h"        
#include "npcs/interaction.h"
#include "boss/boss.h"
#include "npcs/peasant.h"
#include "goblin/goblinTank.h"


// ============================================================================
// Definições constantes
// ============================================================================
#define TILE_SIZE 32              
#define TILESET1_TILECOUNT 36     
#define TILESET1_FIRSTGID 1       
#define TILESET2_FIRSTGID 37     
#define TILESET3_FIRSTGID 73      
#define TILESET4_FIRSTGID 109    
#define TILESET5_FIRSTGID 145   
#define TILESET6_FIRSTGID 181 
#define TILESET7_FIRSTGID 217
#define MAP_COUNT 9        


// ============================================================================
// Estruturas
// ============================================================================

typedef enum GameState 
{
    MENU,
    CREDITS,
    PLAYING,
    GAME_OVER,
    GITHUB
} GameState;

typedef enum ConfigState
{
    CONFIG_CLOSED,
    CONFIG_OPEN,
    CONFIG_AUDIO,
    CONFIG,
    AJUDA,
    ATALHOS
} ConfigState;

typedef enum {
    MUSIC_MENU,
    MUSIC_GAMEPLAY,
    MUSIC_BOSS
} MusicState;


// Estrutura que representa um mapa carregado do Tiled
typedef struct
{
    int width, height;           
    int tileWidth, tileHeight;   
    int *data;                   
} TileMap;


// ============================================================================
// Funções auxiliares para mapas
// ============================================================================

// Carrega um mapa no formato JSON exportado pelo Tiled
TileMap LoadTileMap(const char *fileName)
{
    TileMap map = {0};

    FILE *f = fopen(fileName, "rb");
    if (!f)
    {
        fprintf(stderr, "Erro ao abrir %s\n", fileName);
        return map;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *jsonData = malloc(len + 1);
    fread(jsonData, 1, len, f);
    jsonData[len] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(jsonData);
    free(jsonData);

    if (!root)
    {
        fprintf(stderr, "Erro ao parsear JSON: %s\n", cJSON_GetErrorPtr());
        return map;
    }

    map.width      = cJSON_GetObjectItem(root, "width")->valueint;
    map.height     = cJSON_GetObjectItem(root, "height")->valueint;
    map.tileWidth  = cJSON_GetObjectItem(root, "tilewidth")->valueint;
    map.tileHeight = cJSON_GetObjectItem(root, "tileheight")->valueint;

    cJSON *layer0 = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "layers"), 0);
    cJSON *data   = cJSON_GetObjectItem(layer0, "data");

    map.data = malloc(sizeof(int) * map.width * map.height);

    for (int i = 0; i < map.width * map.height; i++)
    {
        map.data[i] = cJSON_GetArrayItem(data, i)->valueint;
    }

    cJSON_Delete(root);
    return map;
}

void UnloadTileMap(TileMap *map)
{
    if (map->data)
    {
        free(map->data);
    }
}

// ============================================================================
// Função de desenho do mapa com múltiplos tilesets
// ============================================================================

void DrawTileMapIndividual(const TileMap *map, Texture2D tileset1, Texture2D tileset2, Texture2D tileset3, Texture2D tileset4, Texture2D tileset5, Texture2D tileset6, Texture2D tileset7)
{
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            int gid = map->data[y * map->width + x];

            if (gid <= 0) continue; 

            Texture2D texture;
            int localId;

            // Determina qual tileset usar baseado no GID
            if (gid >= TILESET1_FIRSTGID && gid < TILESET2_FIRSTGID)
            {
                texture = tileset1;
                localId = gid - TILESET1_FIRSTGID;
            }
            else if (gid >= TILESET2_FIRSTGID && gid < TILESET3_FIRSTGID)
            {
                texture = tileset2;
                localId = gid - TILESET2_FIRSTGID;
            }
            else if (gid >= TILESET3_FIRSTGID && gid < TILESET4_FIRSTGID)
            {
                texture = tileset3;
                localId = gid - TILESET3_FIRSTGID;
            }
            else if (gid >= TILESET4_FIRSTGID && gid < TILESET5_FIRSTGID)
            {
                texture = tileset4;
                localId = gid - TILESET4_FIRSTGID;
            }
            else if (gid >= TILESET5_FIRSTGID && gid < TILESET6_FIRSTGID)
            {
                texture = tileset5;
                localId = gid - TILESET5_FIRSTGID;
            }
            else if (gid >= TILESET6_FIRSTGID && gid < TILESET7_FIRSTGID)
            {
                texture = tileset6;
                localId = gid - TILESET6_FIRSTGID;
            }
            else if (gid >= TILESET7_FIRSTGID)
            {
                texture = tileset7;
                localId = gid - TILESET7_FIRSTGID;
            }
            else
            {
                continue;
            }

            Rectangle sourceRec = {
                (float)(localId % 6) * TILE_SIZE,
                (float)(localId / 6) * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            Vector2 position = {
                x * map->tileWidth,
                y * map->tileHeight
            };
  
            DrawTextureRec(texture, sourceRec, position, RAYWHITE);
        }
    }
}
// ============================================================================
// Função principal
// ============================================================================
int currentMapIndex = 0;

void resetGame(Player *player, Wolf *wolf, Wolf *redWolf, Wolf *whiteWolf, WolfRun *wolfRun, 
                Goblin *goblin, Goblin *redGoblin, GoblinArcher *goblinArcher, Heart hearts[],
                Npc *npc, Ghost *ghost, Interaction *interaction, Boss *boss, GoblinTank *goblinTank, 
                Peasant *peasant, TileMap *map, const char *mapFiles[])
{
    InitPlayer(player);
    InitWolfBase(wolf, (Vector2){500, 450});
    InitWhiteWolf(wolf, (Vector2){800, 450});
    InitRedWolf(redWolf, (Vector2){800, 450});
    InitRunningWolf(wolfRun);
    InitGoblinBase(goblin, (Vector2){780, 567});
    InitRedGoblin(redGoblin, (Vector2){450, 569});
    InitGoblinArcher(goblinArcher);
    InitHearts(hearts);
    InitNpc(npc);
    InitGhost(ghost);
    InitInteraction(interaction);
    InitBoss(boss);
    InitGoblinTank(goblinTank);
    InitPeasant(peasant);

    currentMapIndex = 0;

    UnloadTileMap(map);
    *map = LoadTileMap(mapFiles[currentMapIndex]);
}

void drawTitleMaps(const char* titulo, Font titleMaps, float delta, int currentMapIndex)
{
    static float textTime = 0.0f;
    static int lastMapIndex = -1;

    if (currentMapIndex != lastMapIndex)
    {
        textTime = 0.0f;
        lastMapIndex = currentMapIndex;
    }

    textTime += delta;

    float fontSize = 60.0f;
    float spacing = 1.0f;

    Vector2 textSize = MeasureTextEx(titleMaps, titulo, fontSize, spacing);
    Vector2 textPosition =
    {
        GetScreenWidth() / 2 - textSize.x / 2,
        GetScreenHeight() / 2 - 220
    };

    if (textTime <= 2.0f)
    {
        DrawRectangle(
            (int)(textPosition.x - 20),
            (int)(textPosition.y - 10),
            (int)(textSize.x + 40),
            (int)(textSize.y + 20),
            (Color){ 0, 0, 0, 160 }
        );

        DrawTextPro(
            titleMaps,
            titulo,
            textPosition,
            (Vector2){ 0, 0 },
            0.0f,
            fontSize,
            spacing,
            GOLD
        );
    }
}

bool drawHoverButton(Rectangle button, Vector2 mousePos, const char *text)
{
    DrawRectangleLinesEx(button, 1, WHITE);
    DrawText(text, button.x + 20, button.y + 10, 20, WHITE);   

    if (CheckCollisionPointRec(mousePos, button)) 
    {
        DrawRectangleLinesEx(button, 1, GOLD);
        DrawText(text, button.x + 20, button.y + 10, 20, GOLD);     
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            return true;;
        }
    }

    return false;
}

int main(void)
{
    bool bossTriggered = false;

    InitAudioDevice();

    Music menuMusic = LoadMusicStream("resources/music/menu-music.mp3");
    Music soundTrack = LoadMusicStream("resources/music/soundtrack.mp3");

    Sound buttonSelect = LoadSound("resources/sounds/sound_effects/buttons/menu-select-button.wav");
    Sound onOFF = LoadSound("resources/sounds/sound_effects/buttons/ON-OFF.wav");
    
    float textTime = 0.0f;

    GameState gameState = MENU;
    static MusicState currentMusic = MUSIC_MENU;
    ConfigState configState = CONFIG_CLOSED;

    PlayMusicStream(menuMusic);

    const char *mapFiles[MAP_COUNT] = {
        "assets/maps/castle_map.json",
        "assets/maps/castle_map2.json",
        "assets/maps/castle_map3.json",
        "assets/maps/florest1.json",
        "assets/maps/florest2.json",
        "assets/maps/florest3.json",
        "assets/maps/goblin1.json",
        "assets/maps/goblin2.json",
        "assets/maps/goblin3.json"
    };
    
    TileMap map = LoadTileMap(mapFiles[currentMapIndex]);
    if (!map.data) return 1;

    InitWindow(
        map.width * map.tileWidth,
        map.height * map.tileHeight,
        "Knights`s Quest the Goblin Saga"
    );

    SetTargetFPS(60);

    DialogState dialogState = DIALOG_CLOSED;
    DialogStateGhost dialogStateGhost = DIALOG_CLOSED_GHOST;

    DialogStatePeasant dialogStatePeasant = DIALOG_CLOSED_PEASANT;
    float dialogoTimer = 0.0f;  
    float dialogoTimerPeasant = 0.0f;

    Player player;
    InitPlayer(&player);

    Wolf wolf;
    InitWolfBase(&wolf, (Vector2){500, 450});

    Wolf whiteWolf;
    InitWhiteWolf(&whiteWolf, (Vector2){800, 450});

    Wolf redWolf;
    InitRedWolf(&redWolf, (Vector2){800, 450});

    WolfRun wolfRun;
    InitRunningWolf(&wolfRun);

    Goblin goblin;
    InitGoblinBase(&goblin, (Vector2){780, 567});

    Goblin redGoblin;
    InitRedGoblin(&redGoblin, (Vector2){450, 569});

    GoblinArcher goblinArcher;
    InitGoblinArcher(&goblinArcher);

    Heart hearts[MAX_HEARTS];
    InitHearts(hearts);

    Npc npc;
    InitNpc(&npc);

    Ghost ghost;
    InitGhost(&ghost);

    Interaction interaction;
    InitInteraction(&interaction);

    Boss boss;
    InitBoss(&boss);

    Peasant peasant;
    InitPeasant(&peasant);

    GoblinTank goblinTank;
    InitGoblinTank(&goblinTank);

    Texture2D tileset1 = LoadTexture("assets/maps/tiles_map/castlemap.png");
    Texture2D tileset2 = LoadTexture("assets/maps/tiles_map/castlesky.png");
    Texture2D tileset3 = LoadTexture("assets/maps/tiles_map/endcastle.png");
    Texture2D tileset4 = LoadTexture("assets/maps/tiles_map/floresta1.png");
    Texture2D tileset5 = LoadTexture("assets/maps/tiles_map/goblin1.png");
    Texture2D tileset6 = LoadTexture("assets/maps/tiles_map/goblin2.png");
    Texture2D tileset7 = LoadTexture("assets/maps/tiles_map/goblin3.png");

    Texture2D staminaBar = LoadTexture("resources/sprites/stamina/staminaBar.png");

    Texture2D barLifeSprite = LoadTexture("resources/sprites/life/life.png");

    Texture2D deathImage = LoadTexture("resources/img/deathImage.png");

    Texture2D menuImage = LoadTexture("resources/img/menu.png");

    Font titleMaps = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);

    float volume = 0.5;
    float volumeEfects = 1.0;
    float volumeDialogue = 1.0;
    float slideX = 700;
    float slideXEfects = 720;
    float slideXDialogue = 700;
    float slideY = 200;
    float slideYEfects = 275;
    float slideYDialogue = 350;
    float slideWidth = 200;
    float slideHeight = 10;
    float ballRadius = 10;

    bool button_hovered = false;
    Rectangle button_rect = {750, 195, 70, 20};
    bool button_pressed = false;

    bool deathSoundPlay = false;

    Rectangle checkX 
            = {
                5,
                5,
                120 - 5,
                40 - 5
            };

    Rectangle checkXConfigs
            = {
                250,
                100,
                120 - 5,
                40 - 5
            };
    
    Sound effectSounds[] = {
    player.attackLightSound,
    player.attackHeavySound,
    player.jumpSound,
    player.walkingInCastle,
    player.walkingInGrass,
    player.playerHurtSound,

    wolf.wolfHitSound,
    wolf.wolfHitSoundHeavy,
    wolf.wolfDeathSound,
    wolf.wolfScratch,
    wolf.wolfGrowl,

    wolfRun.wolfHitSound,
    wolfRun.wolfHitSoundHeavy,
    wolfRun.wolfDeathSound,
    wolfRun.wolfScratch,
    wolfRun.wolfGrowl,

    redWolf.wolfHitSound,
    redWolf.wolfHitSoundHeavy,
    redWolf.wolfDeathSound,
    redWolf.wolfScratch,
    redWolf.wolfGrowl,

    whiteWolf.wolfScratch,
    whiteWolf.wolfGrowl,
    whiteWolf.wolfHitSound,
    whiteWolf.wolfHitSoundHeavy,
    whiteWolf.wolfDeathSound,

    goblin.goblinCutSound,
    goblin.goblinDeathSound,

    redGoblin.RedGoblinHitSound,    
    redGoblin.goblinDeathSound,
    redGoblin.goblinCutSound,

    goblinArcher.arrowHitSound,
    goblinArcher.goblinArcherDeathSound,
    goblinArcher.goblinArcherLoadingSound,

};

int effectSoundCount = sizeof(effectSounds) / sizeof(effectSounds[0]);

while (!WindowShouldClose())
{
    UpdateMusicStream(menuMusic);
    UpdateMusicStream(soundTrack);
    UpdateMusicStream(boss.bossMusic);

    float delta = GetFrameTime();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    Vector2 mousePos = GetMousePosition();
    printf("Mouse X: %i | Mouse Y: %i\n", (int)mousePos.x, (int)mousePos.y);

    MusicState desiredMusic;

    if (gameState == MENU)
    {
        desiredMusic = MUSIC_MENU;
    }
    else if (bossTriggered)
    {
        desiredMusic = MUSIC_BOSS;
    }
    else
    {
        desiredMusic = MUSIC_GAMEPLAY;
    }

    if (desiredMusic != currentMusic)
    {
        StopMusicStream(menuMusic);
        StopMusicStream(soundTrack);
        StopMusicStream(boss.bossMusic);

        switch (desiredMusic)
        {
            case MUSIC_MENU:
                PlayMusicStream(menuMusic);
                break;
            case MUSIC_GAMEPLAY:
                PlayMusicStream(soundTrack);
                break;
            case MUSIC_BOSS:
                PlayMusicStream(boss.bossMusic);
                break;
        }

        currentMusic = desiredMusic;
    }

    switch (gameState)
    {
        // ========================================================
        // ESTADO: MENU
        // ========================================================
        case MENU:
        {

            bossTriggered = false;

            float x1Play = 930, y1Play = 418;  
            float x2Play = 1300, y2Play = 532;  
            float checkY1Play = 418, checkY2Play = 530;

            Rectangle rectPlay
            = {
                x1Play,
                y1Play,
                x2Play - x1Play,   
                y2Play - y1Play    
            };
    
            Rectangle checkPlay
            = {
                x1Play,
                checkY1Play,
                x2Play - x1Play,
                checkY2Play - checkY1Play
            };

            float x1Credits = 890, y1Credits = 555;  
            float x2Credits = 1340, y2Credits = 660;
            float checkY1Credits = 554, checkY2Credits = 660;
            
            Rectangle rectCredits
            = {
                x1Credits,
                y1Credits,
                x2Credits - x1Credits,  
                y2Credits - y1Credits    
            };
    
            Rectangle checkCredits
            = {
                x1Credits,
                checkY1Credits,
                x2Credits - x1Credits,
                checkY2Credits - checkY1Credits
            };

            Vector2 center = {1352, 835};
            float radius = 59;

            DrawTexture(menuImage, 0, 0, WHITE);

            if (CheckCollisionPointRec(mousePos, checkPlay)) {
                DrawRectangleRec(rectPlay, (Color){247,173,7,75});
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), rectPlay))
            {
                PlaySound(buttonSelect);
                WaitTime(0.5f);
                gameState = PLAYING;
            }

            if (CheckCollisionPointRec(mousePos, checkCredits)) {
                DrawRectangleRec(rectCredits, (Color){247,173,7,75});
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), rectCredits))
            {
                PlaySound(buttonSelect);
                WaitTime(0.5f);
                gameState = CREDITS;
            }

            if (CheckCollisionPointCircle(GetMousePosition(), center, radius)) 
            {
                DrawCircleV(center, radius, (Color){247,173,7,75});

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
                {
                    PlaySound(buttonSelect);
                    WaitTime(0.5f);

                    #if defined(_WIN32)
                        system("start https://github.com/Jaca7x/Knight-s-Quest");
                    #elif defined(__APPLE__)
                        system("open https://github.com/Jaca7x/Knight-s-Quest");
                    #else
                        system("xdg-open https://github.com/Jaca7x/Knight-s-Quest");
                    #endif

                    gameState = GITHUB;
                }
            }
        } break;

        // ========================================================
        // ESTADO: PLAYING
        // ========================================================
        case PLAYING:
        {
             Rectangle configAudio
                = {
                    520,
                    280,
                    280,
                    40
                };

                Rectangle configGame
                = {
                    520,
                    380,
                    280,
                    40
                };

                Rectangle atalhos
                = {
                    520, 
                    280,
                    280,
                    40
                };

                Rectangle ajuda 
                = {
                    520,
                    380,
                    280,
                    40
                };

                Rectangle buttonFontSize
                = {
                    750,
                    280,
                    80,
                    30
                };

            switch (configState)
            {   
            case CONFIG_CLOSED:
                if (IsKeyPressed(KEY_K))
                    configState = CONFIG_OPEN;
                break;

            case CONFIG_OPEN:
                player.isAttacking = true;

                if (CheckCollisionPointRec(mousePos, configAudio) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    PlaySound(buttonSelect);
                    configState = CONFIG_AUDIO;
                }

                if (CheckCollisionPointRec(mousePos, configGame) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    PlaySound(buttonSelect);
                    configState = CONFIG;
                }
                
                if (IsKeyPressed(KEY_K))
                    configState = CONFIG_CLOSED;
                    player.isAttacking = false;
                break;

            case CONFIG_AUDIO:
                player.isAttacking = false;
                if (IsKeyPressed(KEY_K))
                    configState = CONFIG_CLOSED;
                break;

            case CONFIG:
                player.isAttacking = false;
                if (CheckCollisionPointRec(mousePos, atalhos) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    PlaySound(buttonSelect);
                    configState = ATALHOS;
                }

                if (CheckCollisionPointRec(mousePos, ajuda) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    PlaySound(buttonSelect);
                    configState = AJUDA;
                }
        
                if (IsKeyPressed(KEY_K))
                    configState = CONFIG_CLOSED;
                break;

            case ATALHOS:
                player.isAttacking = false;
                if (IsKeyPressed(KEY_K))
                    configState = CONFIG_CLOSED;
                break;
                
            case AJUDA:
                player.isAttacking = false;
                if (IsKeyPressed(KEY_K))
                    configState = CONFIG_CLOSED;
                break;
            }
            
            if ((currentMapIndex == 0 || currentMapIndex == 1) && player.isMoving && !player.isJumping)
            {
                if (!player.walkSoundPlayingCastle)
                {
                    PlaySound(player.walkingInCastle);
                    player.walkSoundPlayingCastle = true;
                    player.walkTimeCastle = 0.0f;
                }
                player.walkTimeCastle += delta;

                if (player.walkTimeCastle > 0.5f)
                {
                    StopSound(player.walkingInCastle);
                    player.walkSoundPlayingCastle = false;
                }
            }
            else
            {
                if (player.walkSoundPlayingCastle)
                {
                    StopSound(player.walkingInCastle);
                    player.walkSoundPlayingCastle = false;
                }
            }

            if (currentMapIndex >= 2 && player.isMoving && !player.isJumping)
            {
                if (!player.walkSoundPlayingGrass)
                {
                    PlaySound(player.walkingInGrass);
                    player.walkSoundPlayingGrass = true;
                    player.walkTimeGrass = 0.0f;
                }
                player.walkTimeGrass += delta;

                if (player.walkTimeGrass > 0.5f)
                {
                    StopSound(player.walkingInGrass);
                    player.walkSoundPlayingGrass = false;
                }
            }
            else
            {
                if (player.walkSoundPlayingGrass)
                {
                    StopSound(player.walkingInGrass);
                    player.walkSoundPlayingGrass = false;
                }
            }

            if (currentMapIndex == 8 &&
                peasant.dialogueFinished &&
                !bossTriggered)
            {
                bossTriggered = true;
            }

            // --- Atualizações ---
            UpdateStaminaBar(&player, delta);
            UpdateNpc(&npc, delta, &player, &dialogState, &dialogoTimer);
            UpdateGhost(&ghost, &player, delta, &interaction, &dialogStateGhost, &dialogoTimer, currentMapIndex);
            UpdatePeasant(&peasant, &player, delta, &interaction, &dialogStatePeasant, &dialogoTimerPeasant, currentMapIndex, &bossTriggered);
            UpdateInteraction(&interaction, delta); 

            if (player.life <= 0 )
            {
                if (player.deathAnimationDone)
                {
                    gameState = GAME_OVER;
                }
            }

            // --- Desenho ---
            DrawTileMapIndividual(&map, tileset1, tileset2, tileset3, tileset4, tileset5, tileset6, tileset7);

            if (player.position.x + player.frameWidth * 2 > map.tileWidth * map.width)
            {
                currentMapIndex = (currentMapIndex + 1) % MAP_COUNT;
                dialogStateGhost = DIALOG_CLOSED_GHOST;
                ghost.isInteraction = false;

                dialogState = DIALOG_CLOSED;

                UnloadTileMap(&map);
                map = LoadTileMap(mapFiles[currentMapIndex]);

                SetWindowSize(
                    map.width * map.tileWidth,
                    map.height * map.tileHeight
                );

                player.position.x = 0;
                player.position.y = 520;
            }   

            if (bossTriggered)
            {
                DrawBoss(&boss);
                UpdateBoss(&boss, &player, delta, &bossTriggered);
            }

        
            if (currentMapIndex == MAP_WOLF_WHITE_AREA)
            {
                UpdateWolf(&whiteWolf, &player, delta, currentMapIndex);
                DrawWolf(&whiteWolf);
            }

            if (currentMapIndex == MAP_WOLF_RED_AREA)
            {
                UpdateWolf(&redWolf, &player, delta, currentMapIndex);
                DrawWolf(&redWolf);
            }
            
            if (currentMapIndex == GOBLIN_MAP)
            {
                DrawGoblin(&goblin);
                UpdateGoblin(&goblin, &player, currentMapIndex, delta);
            }

            if (currentMapIndex == RED_GOBLIN_MAP)
            {
                DrawGoblin(&redGoblin);
                UpdateGoblin(&redGoblin, &player, currentMapIndex, delta);
            }
            
            if (currentMapIndex == GOBLIN_TANK_MAP)
            {
                DrawGoblinTank(&goblinTank);
                UpdateGoblinTank(&goblinTank, delta, &player);
            }
            
            if (currentMapIndex == MAP_WOLF_RUNNING_AREA)
            {
                UpdateRunningWolf(&wolfRun, &player, delta);
                DrawRunningWolf(&wolfRun);

                UpdateWolf(&wolf, &player, delta, currentMapIndex);
                DrawWolf(&wolf);
            }

            if (currentMapIndex == MAP_GOBLIN_ARCHER_AREA)
            {
                DrawGoblinArcher(&goblinArcher);
                UpdateGoblinArcher(&goblinArcher, &player, delta);
            }

            UpdateHearts(hearts, delta, &player, &wolf, &redWolf, &whiteWolf, &goblin, &redGoblin, &goblinArcher, &wolfRun);
            DrawHearts(hearts, delta, &player);

            if (MAP_NPC == currentMapIndex) {
                DrawNpc(&npc, &player, &interaction, dialogState);
                UpdateInteraction(&interaction, delta);
            }

            MapsGhost(&ghost, &player, dialogStateGhost, &interaction, delta, currentMapIndex);
            DrawMapsPeasant(&peasant, &player, &interaction, currentMapIndex, delta, &dialogStatePeasant);
            
            textTime += delta;
            
            if (currentMapIndex == 0 && textTime <= 2)
            {
                DrawRectangle(player.position.x - 60, player.position.y, 340, 30, (Color){0,0,0,180});
                DrawText("Aperte 'K', para abrir às configurações", player.position.x - 40, player.position.y + 7, 16, WHITE);
            }
    
            UpdatePlayer(&player, &wolf, &wolfRun, &redWolf, &whiteWolf, &goblin, &redGoblin, &goblinArcher, currentMapIndex, delta, &npc, &boss);
            DrawPlayer(&player);

            DrawStaminaBar(staminaBar, player.stamina, (Vector2){1350, 20}, 2.0f, &player, button_pressed);
            DrawLifeBar(barLifeSprite, player.life, (Vector2){20, 10}, 2.0f);

            if (configState == CONFIG_OPEN)
            {   
                DrawRectangle(250, 100, 800, 600, (Color){0, 0, 0, 200});
                DrawText("Configurações (Pressione 'K' para fechar)", 450, 120, 20, WHITE);

                drawHoverButton(configAudio, mousePos, "Configurações de Áudio");

                drawHoverButton(configGame, mousePos, "Configurações de jogo");
            }

            if (configState == CONFIG_AUDIO)
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)
                && CheckCollisionPointRec(mousePos, (Rectangle){slideX, slideY - ballRadius, slideWidth, slideHeight + ballRadius * 2}))
                    {
                        float percent = (mousePos.x - slideX) / slideWidth;
                        if (percent < 0) percent = 0;
                        if (percent > 1) percent = 1;

                        volume = percent;

                        SetMusicVolume(menuMusic, volume);
                        SetMusicVolume(soundTrack, volume);
                        SetMusicVolume(boss.bossMusic, volume);
                    }

                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)
                && CheckCollisionPointRec(mousePos, (Rectangle){slideXEfects, slideYEfects - ballRadius, slideWidth, slideHeight + ballRadius * 2}))
                    {
                        float percent = (mousePos.x - slideXEfects) / slideWidth;
                        if (percent < 0) percent = 0;
                        if (percent > 1) percent = 1;

                        volumeEfects = percent;

                        for (int i = 0; i < effectSoundCount; i++)
                        {
                            SetSoundVolume(effectSounds[i], volumeEfects);
                        }

                        for (int i = 0; i < MAX_HEARTS; i++)
                        {
                            SetSoundVolume(hearts[i].collectSound, volumeEfects);
                            SetSoundVolume(hearts[i].spawnSound, volumeEfects);
                        }
                    }

                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)
                && CheckCollisionPointRec(mousePos, (Rectangle){slideXDialogue, slideYDialogue - ballRadius, slideWidth, slideHeight + ballRadius * 2}))
                    {
                        float percent = (mousePos.x - slideXDialogue) / slideWidth;
                        if (percent < 0) percent = 0;
                        if (percent > 1) percent = 1;

                        volumeDialogue = percent;

                        SetSoundVolume(player.playerDialogueWithNPC1, volumeDialogue);
                        SetSoundVolume(player.playerDialogueWithNPC2, volumeDialogue);

                        SetSoundVolume(npc.dialogueWithPlayer1, volumeDialogue);
                        SetSoundVolume(npc.dialogueWithPlayer2, volumeDialogue);

                        for (int i = 0; i < NUM_MAPS; i++)
                        {
                            for (int j = 0; j < DIALOGS_PER_MAP; j++)
                            {   
                                SetSoundVolume(player.dialogues[i][j].sound, volumeDialogue);
                                SetSoundVolume(ghost.dialogues[i][j].sound, volumeDialogue);
                            }
                        }

                        for (int i = 0; i < NUM_MAPS_PEASANT; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                SetSoundVolume(peasant.dialogues[i][j].sound, volumeDialogue);
                                SetSoundVolume(player.dialoguesWithPeasant[i][j].sound, volumeDialogue);
                            }
                        }

                    }

                DrawRectangle(250, 100, 800, 600, (Color){0,0,0,200});

                if (drawHoverButton(checkXConfigs, mousePos, "VOLTAR"))
                {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        configState = CONFIG_OPEN;
                        PlaySound(buttonSelect);
                    }
                }

                DrawText("Configurações de Áudio(Pressione 'K' para fechar)", 400, 120, 20, WHITE);

                DrawText("Volume Música:", 450, 195, 20, WHITE);

                DrawRectangle(slideX, slideY, slideWidth, slideHeight, LIGHTGRAY);

                DrawRectangle(slideX, slideY, slideWidth * volume, slideHeight, SKYBLUE);

                DrawCircle(slideX + slideWidth * volume, slideY + slideHeight / 2, ballRadius, DARKBLUE);

                DrawText("Volume Efeitos Sonoros:", 450, 270, 20, WHITE);

                DrawRectangle(slideXEfects, slideYEfects, slideWidth, slideHeight, LIGHTGRAY);

                DrawRectangle(slideXEfects, slideYEfects, slideWidth * volumeEfects, slideHeight, SKYBLUE);

                DrawCircle(slideXEfects + slideWidth * volumeEfects, slideYEfects + slideHeight / 2, ballRadius, DARKBLUE);

                DrawText("Volume Dialogos:", 450, 350, 20, WHITE);

                DrawRectangle(slideXDialogue, slideYDialogue, slideWidth, slideHeight, LIGHTGRAY);

                DrawRectangle(slideXDialogue, slideYDialogue, slideWidth * volumeDialogue, slideHeight, SKYBLUE);

                DrawCircle(slideXDialogue + slideWidth * volumeDialogue, slideYDialogue + slideHeight / 2, ballRadius, DARKBLUE);
            }

             if (configState == CONFIG)
            {
                DrawRectangle(250, 100, 800, 600, (Color){0,0,0,200});

                if (drawHoverButton(checkXConfigs, mousePos, "VOLTAR"))
                {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        configState = CONFIG_OPEN;
                        PlaySound(buttonSelect);
                    }
                }

                DrawText("Configurações (Pressione 'K' para fechar)", 450, 120, 20, WHITE);

                drawHoverButton(atalhos, mousePos, "Atalhos");
                drawHoverButton(ajuda, mousePos, "Ajuda");      
            }
            
            if (configState == AJUDA)
            {
                button_hovered = CheckCollisionPointRec(GetMousePosition(), button_rect);

                if (button_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        PlaySound(onOFF);
                        button_pressed = !button_pressed;
                    }

                DrawRectangle(250, 100, 800, 600, (Color){0,0,0,200});

                if (drawHoverButton(checkXConfigs, mousePos, "VOLTAR"))
                {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        configState = CONFIG_OPEN;
                        PlaySound(buttonSelect);
                    }
                }

                DrawRectangleLinesEx(buttonFontSize, 1, WHITE);
                DrawText("Tamanho Fonte Diálogo:", 455, 285, 20, WHITE);

                Vector2 centerIncrese = {850, 295};
                Vector2 centerDecrease = {730, 295};

                float radius = 12;

                DrawCircleLines(730, 295, 12, WHITE);
                DrawText("-", 726, 285, 20, WHITE);

                DrawCircleLines(850, 295, 12, WHITE);
                DrawText("+", 846, 285, 20, WHITE);

                if (CheckCollisionPointCircle(GetMousePosition(), centerDecrease, radius) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && ghost.speechFontSize > 28)
                {
                    DrawCircleLines(730, 295, 12, GOLD);
                    DrawText("-", 726, 285, 20, GOLD);
                    ghost.speechFontSize--;
                    peasant.speechFontSize--;
                }

                if (CheckCollisionPointCircle(GetMousePosition(), centerIncrese, radius) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && ghost.speechFontSize < 33)
                {
                    DrawCircleLines(850, 295, 12, GOLD);
                    DrawText("+", 846, 285, 20, GOLD);
                    ghost.speechFontSize++;
                    peasant.speechFontSize++;
                }

                DrawText(TextFormat("%d", ghost.speechFontSize), 780, 285, 20, WHITE);

                DrawText("Configurações de Ajuda (Pressione 'K' para fechar)", 420, 120, 20, WHITE);

                DrawText("Assistência de estamina:", 450, 195, 20, WHITE);

                DrawRectangleRounded(button_rect, 0.30f, 0, (Color){128,128,128,160});

                if (button_pressed)
                {   
                    DrawText("OFF", 755, 200, 11, WHITE);
                    Rectangle button_rect = {750 + 30, 195, 40, 20};
                    DrawRectangleRounded(button_rect, 0.30f, 0, GREEN);
                }
                else
                {
                    DrawText("ON", 795, 200, 12, WHITE);
                    Rectangle button_rect = {750, 195, 40, 20};
                    DrawRectangleRounded(button_rect, 0.30f, 0, RED);
                }
            }

            if (configState == ATALHOS)
            {
                Rectangle table 
                = {
                    460,
                    200,
                    400,
                    460
                };

                DrawRectangle(250, 100, 800, 600, (Color){0,0,0,200});

                if (drawHoverButton(checkXConfigs, mousePos, "VOLTAR"))
                {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        configState = CONFIG_OPEN;
                        PlaySound(buttonSelect);
                    }
                }

                DrawText("Atalhos (Pressione 'K' para fechar)", 480, 120, 20, WHITE);

                DrawRectangleLinesEx(table, 1 , WHITE);
                DrawRectangleLines(620, 200, 0, 460, WHITE);
                for (int i = 0; i < 8; i++)
                {
                    int posY = 250 + (i * 50);

                    DrawRectangleLines(460, posY, 400, 0, WHITE);
                }
                
                DrawText("      W                  ANDAR PARA FRENTE", 505, 220, 16, WHITE);
                DrawText("      A                  ANDAR PARA ESQUERDA", 505, 270, 16, WHITE);
                DrawText("      S                  ANDAR PARA TRÁS", 505, 320, 16, WHITE);
                DrawText("      D                  ANDAR PARA DIREITA", 505,370, 16, WHITE);

                DrawText("ESPAÇO                       PULAR", 515, 420, 16, WHITE);
                DrawText("      E                         INTERAGIR", 505, 470, 16, WHITE);
                DrawText("      K                      CONFIGURAÇÕES", 505, 520, 16, WHITE);


                DrawText("SHIFT                       CORRER", 520, 570, 16, WHITE);

                DrawText("M1 & M2           ATAQUE PESADO & LEVE", 515, 620, 16, WHITE);
            }

        const char* titulo = NULL;

        if (currentMapIndex == 0)
        {
            titulo = "Castelo Bastion de Eldurin";
        }
        else if (currentMapIndex == 3)
        {
            titulo = "Bosque de Arvendel";
        }
        else if (currentMapIndex == 6)
        {
            titulo = "Floresta Negra de Eldruin";
        }

        if (titulo != NULL)
        {
            drawTitleMaps(titulo, titleMaps, GetFrameTime(), currentMapIndex);
        }
           
        } break;

        case CREDITS:
        {
            if (drawHoverButton(checkX, mousePos, "VOLTAR"))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    PlaySound(buttonSelect);
                    gameState = MENU;
                }
            }

            ClearBackground(BLACK);
            DrawText("Créditos", GetScreenWidth()/2 - 70, 50, 40, WHITE);
            DrawText("Obrigado por jogar este jogo!\n", GetScreenWidth()/2 - 140, 150, 20, WHITE);
            DrawText("Ele foi uma das minhas primeiras experiencias\n", GetScreenWidth()/2 - 210, 180, 20, WHITE);
            DrawText("no mundo do game dev, e com certeza ainda existem\n", GetScreenWidth()/2 - 230, 210, 20, WHITE);
            DrawText("alguns problemas (como era de se esperar).\n", GetScreenWidth()/2 - 210, 240, 20, WHITE);
            DrawText("\n", GetScreenWidth()/2 - 140, 270, 20, WHITE);
            DrawText("Mas esse e apenas o inicio da minha jornada\n", GetScreenWidth()/2 - 210, 300, 20, WHITE);
            DrawText("no desenvolvimento de jogos, e cada passo dado\n", GetScreenWidth()/2 - 230, 330, 20, WHITE);
            DrawText("me motiva a aprender e evoluir cada vez mais.\n", GetScreenWidth()/2 - 210, 360, 20, WHITE);
            DrawText("\n", GetScreenWidth()/2 - 140, 390, 20, WHITE);
            DrawText("Muito obrigado por me dar essa chance\n", GetScreenWidth()/2 - 210, 420, 20, WHITE);
            DrawText("e por dedicar seu tempo a jogar algo que\n", GetScreenWidth()/2 - 220, 450, 20, WHITE);
            DrawText("fiz com tanto carinho e dedicação.\n", GetScreenWidth()/2 - 190, 480, 20, WHITE);
            DrawText("\n", GetScreenWidth()/2 - 140, 510, 20, WHITE);
            DrawText("Ass: Jaca7x\n", GetScreenWidth()/2 - 70, 540, 20, WHITE);

        } break; 

        // ========================================================
        // ESTADO: GAME_OVER
        // ========================================================
        case GAME_OVER:
        {
            // Tela de morte
            DrawTexture(deathImage, 
                        (GetScreenWidth() - deathImage.width) / 2, 
                        (GetScreenHeight() - deathImage.height) / 2, 
                        WHITE);

            DrawText("Pressione ENTER para voltar ao menu",
                     GetScreenWidth()/2 - 200, GetScreenHeight() - 100, 20, RAYWHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                gameState = MENU;
                PlaySound(buttonSelect);
                resetGame(&player, &wolf, &redWolf, &whiteWolf, &wolfRun, &goblin, &redGoblin, &goblinArcher, hearts, &npc, &ghost, &interaction, &boss, &goblinTank, &peasant, &map, mapFiles);
            }
        } break;

        case GITHUB: 
        {
            if (drawHoverButton(checkX, mousePos, "VOLTAR"))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    PlaySound(buttonSelect);
                    gameState = MENU;
                }
            }
            
            ClearBackground(BLACK);
            DrawText("Abrindo repositório no seu navegador! Aguarde...", GetScreenWidth()/2 - 500, GetScreenHeight()/2, 40, WHITE);
        }
    }
    EndDrawing();
}

    UnloadTexture(tileset1);
    UnloadTexture(tileset2);
    UnloadTexture(tileset3);
    UnloadTexture(tileset4);
    UnloadTexture(tileset5);
    UnloadTexture(tileset6);
    UnloadTileMap(&map);
    UnloadPlayer(&player);
    UnloadTexture(staminaBar);
    UnloadTexture(barLifeSprite);
    UnloadWolf(&wolf);
    UnloadRunningWolf(&wolfRun);
    UnloadGoblin(&goblin);
    UnloadGoblinArcher(&goblinArcher);
    UnloadHearts(hearts);
    UnloadFont(titleMaps);
    UnloadNpc(&npc);
    UnloadTexture(deathImage);
    UnloadTexture(menuImage);
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(soundTrack);
    UnloadSound(buttonSelect);
    CloseWindow();
    return 0;
}   
