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
#define MAP_COUNT 6            


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

void DrawTileMapIndividual(const TileMap *map, Texture2D tileset1, Texture2D tileset2, Texture2D tileset3, Texture2D tileset4, Texture2D tileset5)
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
            else if (gid >= TILESET5_FIRSTGID)
            {
                texture = tileset5;
                localId = gid - TILESET5_FIRSTGID;
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
                Npc *npc, Ghost *ghost, TileMap *map, const char *mapFiles[])
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

    currentMapIndex = 0;

    UnloadTileMap(map);
    *map = LoadTileMap(mapFiles[currentMapIndex]);
}


int main(void)
{
    GameState gameState = MENU;

    const char *mapFiles[MAP_COUNT] = {
        "assets/maps/castle_map.json",
        "assets/maps/castle_map2.json",
        "assets/maps/castle_map3.json",
        "assets/maps/florest1.json",
        "assets/maps/florest2.json",
        "assets/maps/florest3.json"
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
    float dialogoTimer = 0.0f;  

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

    Texture2D tileset1 = LoadTexture("assets/maps/tiles_map/castlemap.png");
    Texture2D tileset2 = LoadTexture("assets/maps/tiles_map/castlesky.png");
    Texture2D tileset3 = LoadTexture("assets/maps/tiles_map/endcastle.png");
    Texture2D tileset4 = LoadTexture("assets/maps/tiles_map/floresta1.png");
    Texture2D tileset5 = LoadTexture("assets/maps/tiles_map/goblin1.png");

    Texture2D staminaBar = LoadTexture("resources/sprites/stamina/staminaBar.png");

    Texture2D barLifeSprite = LoadTexture("resources/sprites/life/life.png");

    Texture2D deathImage = LoadTexture("resources/img/deathImage.png");

    Texture2D menuImage = LoadTexture("resources/img/menu.png");

    Font titleMaps = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);

    float textTime = 0.0f;

while (!WindowShouldClose())
{
    float delta = GetFrameTime();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    Vector2 mousePos = GetMousePosition();
    printf("Mouse X: %i | Mouse Y: %i\n", (int)mousePos.x, (int)mousePos.y);

    switch (gameState)
    {
        // ========================================================
        // ESTADO: MENU
        // ========================================================
        case MENU:
        {
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
                gameState = PLAYING;
            }

            if (CheckCollisionPointRec(mousePos, checkCredits)) {
                DrawRectangleRec(rectCredits, (Color){247,173,7,75});
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), rectCredits))
            {
                gameState = CREDITS;
            }

            if (CheckCollisionPointCircle(GetMousePosition(), center, radius)) 
            {
                DrawCircleV(center, radius, (Color){247,173,7,75});

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
                {
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
            // --- Atualizações ---
            UpdateStaminaBar(&player, delta);
            UpdateNpc(&npc, delta, &player, &dialogState, &dialogoTimer);
            UpdateGhost(&ghost, delta);

            if (player.life <= 0 && player.deathAnimationDone)
            {
                gameState = GAME_OVER;
            }

            // --- Desenho ---
            DrawTileMapIndividual(&map, tileset1, tileset2, tileset3, tileset4, tileset5);

            if (player.position.x + player.frameWidth * 2 > map.tileWidth * map.width)
        {
            currentMapIndex = (currentMapIndex + 1) % MAP_COUNT;

            UnloadTileMap(&map);
            map = LoadTileMap(mapFiles[currentMapIndex]);

            SetWindowSize(
                map.width * map.tileWidth,
                map.height * map.tileHeight
            );

            player.position.x = 0;
            player.position.y = 520;
        }   
            if (currentMapIndex == MAP_WOLF_WHITE_AREA)
            {
                UpdateWolf(&whiteWolf, &player, delta);
                DrawWolf(&whiteWolf);
            }

            if (currentMapIndex == MAP_WOLF_RED_AREA)
            {
                UpdateWolf(&redWolf, &player, delta);
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
            
            
            if (currentMapIndex == MAP_WOLF_RUNNING_AREA)
            {
                UpdateRunningWolf(&wolfRun, &player, delta);
                DrawRunningWolf(&wolfRun);
                
                UpdateWolf(&wolf, &player, delta);
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
                DrawNpc(&npc, &player, dialogState);
            }

            DrawGhost(&ghost);

            UpdatePlayer(&player, &wolf, &wolfRun, &redWolf, &whiteWolf, &goblin, &redGoblin, &goblinArcher, currentMapIndex, delta, &npc);
            DrawPlayer(&player);

            DrawStaminaBar(staminaBar, player.stamina, (Vector2){1350, 20}, 2.0f, &player);
            DrawLifeBar(barLifeSprite, player.life, (Vector2){20, 10}, 2.0f);

            if (currentMapIndex == 0 && textTime < 2.0f)
            {
                textTime += delta;
                const char* titulo = "Castelo Bastion de Eldur";
                float fontSize = 60.0f;
                float spacing = 1.0f;

                Vector2 textSize = MeasureTextEx(titleMaps, titulo, fontSize, spacing);
                Vector2 textPosition = {
                    GetScreenWidth() / 2 - textSize.x / 2,
                    GetScreenHeight() / 2 - 220
                };

                DrawRectangle((int)(textPosition.x - 20), (int)(textPosition.y - 10),
                            (int)(textSize.x + 40), (int)(textSize.y + 20),
                            (Color){0, 0, 0, 160});

                DrawTextPro(titleMaps, titulo, textPosition, (Vector2){0, 0}, 0.0f, fontSize, spacing, GOLD);
            }
        } break;

        case CREDITS:
        {
            Rectangle checkX 
            = {
                5,
                5,
                120 - 5,
                40 - 5
            };

            DrawRectangleLines(5, 5, 120, 40, WHITE);
            DrawText("VOLTAR", 20, 15, 20, WHITE);

            if (CheckCollisionPointRec(mousePos, checkX)) 
            {
                DrawRectangleLines(5, 5, 120, 40, GOLD);
                DrawText("VOLTAR", 20, 15, 20, GOLD);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
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
                resetGame(&player, &wolf, &redWolf, &whiteWolf, &wolfRun, &goblin, &redGoblin, &goblinArcher, hearts, &npc, &ghost, &map, mapFiles);
            }
        } break;

        case GITHUB: 
        {
            Rectangle checkX 
            = {
                5,
                5,
                120 - 5,
                40 - 5
            };

            DrawRectangleLines(5, 5, 120, 40, WHITE);
            DrawText("VOLTAR", 20, 15, 20, WHITE);

            if (CheckCollisionPointRec(mousePos, checkX)) 
            {
                DrawRectangleLines(5, 5, 120, 40, GOLD);
                DrawText("VOLTAR", 20, 15, 20, GOLD);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
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


    CloseWindow();
    return 0;
}   
