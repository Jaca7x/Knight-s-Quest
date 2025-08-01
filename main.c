// ============================================================================
// Inclusão de bibliotecas
// ============================================================================
#include "librays/raylib.h"       // Biblioteca Raylib para gráficos, janelas e entrada
#include "librays/cJSON.h"        // Biblioteca cJSON para ler arquivos JSON (mapas do Tiled)
#include <stdlib.h>               // Funções padrão (malloc, free, etc.)
#include <stdio.h>                // Entrada e saída padrão (fopen, fprintf, etc.)
#include "player/player.h"        // Cabeçalho do jogador
#include "staminaBar/stamina.h"   // Cabeçalho da barra de stamina
#include "wolf/wolf.h"
#include "wolf/wolfRunning.h"
#include "lifeBar/lifeBar.h"
#include "goblin/goblin.h"
#include "goblin/goblinArcher.h"
#include "drops/heart.h"         // Cabeçalho do coração


// ============================================================================
// Definições constantes
// ============================================================================
#define TILE_SIZE 32              // Tamanho de cada tile em pixels
#define TILESET1_TILECOUNT 36     // Número de tiles no primeiro tileset
#define TILESET1_FIRSTGID 1       // Primeiro GID do tileset 1
#define TILESET2_FIRSTGID 37      // Primeiro GID do tileset 2 (37 = 1 + 36)
#define TILESET3_FIRSTGID 73      // Primeiro GID do tileset 3
#define TILESET4_FIRSTGID 109     // Primeiro GID do tileset 4
#define TILESET5_FIRSTGID 145   // Número de tiles no quarto tileset
#define MAP_COUNT 6            // Quantidade de mapas


// ============================================================================
// Estruturas
// ============================================================================

// Estrutura que representa um mapa carregado do Tiled
typedef struct
{
    int width, height;           // Largura e altura do mapa em tiles
    int tileWidth, tileHeight;   // Dimensões de cada tile
    int *data;                   // Array contendo os GIDs dos tiles
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

    // Informações gerais do mapa
    map.width      = cJSON_GetObjectItem(root, "width")->valueint;
    map.height     = cJSON_GetObjectItem(root, "height")->valueint;
    map.tileWidth  = cJSON_GetObjectItem(root, "tilewidth")->valueint;
    map.tileHeight = cJSON_GetObjectItem(root, "tileheight")->valueint;

    // Dados da primeira camada (layer 0)
    cJSON *layer0 = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "layers"), 0);
    cJSON *data   = cJSON_GetObjectItem(layer0, "data");

    map.data = malloc(sizeof(int) * map.width * map.height);

    // Copia os dados dos tiles para o array
    for (int i = 0; i < map.width * map.height; i++)
    {
        map.data[i] = cJSON_GetArrayItem(data, i)->valueint;
    }

    cJSON_Delete(root);
    return map;
}

// Libera a memória alocada para o mapa
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

            if (gid <= 0) continue; // Tile vazio, não desenha

            Texture2D texture;
            int localId;

            // Determina qual tileset usar baseado no GID
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

            // Calcula qual parte do tileset desenhar
            Rectangle sourceRec = {
                (float)(localId % 6) * TILE_SIZE,
                (float)(localId / 6) * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            // Calcula a posição na tela
            Vector2 position = {
                x * map->tileWidth,
                y * map->tileHeight
            };

            // Desenha o tile na tela
            DrawTextureRec(texture, sourceRec, position, RAYWHITE);
        }
    }
}


// ============================================================================
// Função principal
// ============================================================================

int main(void)
{
    // Lista dos arquivos de mapas
    const char *mapFiles[MAP_COUNT] = {
        "assets/maps/castle_map.json",
        "assets/maps/castle_map2.json",
        "assets/maps/castle_map3.json",
        "assets/maps/florest1.json",
        "assets/maps/florest2.json",
        "assets/maps/florest3.json"
    };

    int currentMapIndex = 0;

    // Carrega o primeiro mapa
    TileMap map = LoadTileMap(mapFiles[currentMapIndex]);
    if (!map.data) return 1;

    // Cria a janela do jogo com o tamanho do mapa
    InitWindow(
        map.width * map.tileWidth,
        map.height * map.tileHeight,
        "Knights`s Quest the Goblin Saga"
    );

    SetTargetFPS(60);

    // Inicializa o player
    Player player;
    InitPlayer(&player);

    Wolf wolf;
    InitWolf(&wolf);

    WolfRun wolfRun;
    InitRunningWolf(&wolfRun);

    Goblin goblin;
    InitGoblin(&goblin);

    GoblinArcher goblinArcher;
    InitGoblinArcher(&goblinArcher);

    Heart hearts[MAX_HEARTS];
    InitHearts(hearts);

    // Carrega os tilesets
    Texture2D tileset1 = LoadTexture("assets/maps/tiles_map/castlemap.png");
    Texture2D tileset2 = LoadTexture("assets/maps/tiles_map/castlesky.png");
    Texture2D tileset3 = LoadTexture("assets/maps/tiles_map/endcastle.png");
    Texture2D tileset4 = LoadTexture("assets/maps/tiles_map/floresta1.png");
    Texture2D tileset5 = LoadTexture("assets/maps/tiles_map/goblin1.png");

    // Carrega a barra de stamina
    Texture2D staminaBar = LoadTexture("resources/sprites/stamina/staminaBar.png");

    // Carrega a barra de vida
    Texture2D barLifeSprite = LoadTexture("resources/sprites/life/life.png");

    //Carregar imagem de morte
    Texture2D deathImage = LoadTexture("resources/img/deathImage.png");

    Font titleMaps = LoadFontEx("resources/fonts/UncialAntiqua-Regular.ttf", 32, 0, 250);

    bool isGameOver = false;
    
    // Loop principal do jogo
while (!WindowShouldClose())
{
    float delta = GetFrameTime();

    // =====================
    // Atualizações do jogo
    // =====================
    if (!isGameOver)
    {
        UpdatePlayer(&player, &wolf, &wolfRun, &goblin, &goblinArcher, currentMapIndex, delta);
        UpdateStaminaBar(&player, delta);
        
        if (player.life <= 0 && player.deathAnimationDone)
        {
            isGameOver = true;
        }

        // Avança para o próximo mapa se o player chegar ao fim
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
    }

    // =====================
    // Desenho da tela
    // =====================
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // ----- 1. Desenha o mapa primeiro -----
    DrawTileMapIndividual(&map, tileset1, tileset2, tileset3, tileset4, tileset5);

    // ----- 2. Desenha o jogador e inimigos -----
    if (!isGameOver)
    {
        DrawPlayer(&player);

        if (currentMapIndex == GOBLIN_MAP)
        {
            DrawGoblin(&goblin);
            UpdateGoblin(&goblin, &player, currentMapIndex, delta);
        }

        if (currentMapIndex == MAP_WOLF_AREA)
        {
            DrawWolf(&wolf);
            UpdateWolf(&wolf, &player, delta);

            DrawRunningWolf(&wolfRun);
            UpdateRunningWolf(&wolfRun, &player, delta);
        }

        if (currentMapIndex == MAP_GOBLIN_ARCHER_AREA)
        {
            DrawGoblinArcher(&goblinArcher);
            UpdateGoblinArcher(&goblinArcher, &player, delta);
        }

        // Corações (drops de vida)
        UpdateHearts(hearts, delta, &player, &wolf, &goblin, &goblinArcher, &wolfRun);
        DrawHearts(hearts, delta, &player);
    }

   // ----- 3. Texto de título (por cima do mapa) -----
if (currentMapIndex == 0)
{
    const char* titulo = "Castelo Bastion de Eldur";
    float fontSize = 60.0f;
    float spacing = 1.0f;

    // Medir tamanho do texto
    Vector2 textSize = MeasureTextEx(titleMaps, titulo, fontSize, spacing);

    // Centraliza
    Vector2 textPosition = {
        GetScreenWidth() / 2 - textSize.x / 2,
        GetScreenHeight() / 2 - 220
    };

    // Desenha o fundo preto com transparência (alpha 160)
    DrawRectangle((int)(textPosition.x - 20), (int)(textPosition.y - 10), 
                  (int)(textSize.x + 40), (int)(textSize.y + 20), 
                  (Color){0, 0, 0, 160});

    // Desenha o texto por cima
    DrawTextPro(titleMaps, titulo, textPosition, (Vector2){0, 0}, 0.0f, fontSize, spacing, GOLD);
}

    // ----- 4. Interface do usuário (HUD) -----
    DrawStaminaBar(staminaBar, player.stamina, (Vector2){1350, 20}, 2.0f);
    DrawLifeBar(barLifeSprite, player.life, (Vector2){20, 10}, 2.0f);

    // ----- 5. Tela de game over -----
    if (isGameOver)
    {
        DrawTexture(deathImage, 
                    (GetScreenWidth() - deathImage.width) / 2, 
                    (GetScreenHeight() - deathImage.height) / 2, 
                    WHITE);
    }

    EndDrawing();
}

    // Libera todos os recursos carregados
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


    // Encerra a janela
    CloseWindow();
    return 0;
}   
