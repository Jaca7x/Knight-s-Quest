#ifndef NPC_H
#define NPC_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../librays/raymath.h"
    
typedef struct Player player;

typedef enum {
    DIALOGO_FECHADO,
    DIALOGO_NPC_FALANDO,
    DIALOGO_PLAYER_FALANDO
} DialogoEstado;


typedef struct npc {
    Texture2D spriteNpc; // Sprite do NPC
    Texture2D btnE;     // Botão de interação (E)
    Texture2D spriteNpcIdle;
    Texture2D npcSpeech; // Sprite de fala do NPC (se necessário)
    Vector2 position;    // Posição do NPC na tela
    
    int frameTalking;
    int frameIdle;

    int frameWidth;   // Largura de cada frame do sprite
    int frameHeight;  // Altura de cada frame do sprite
    int currentFrame; // Frame atual do sprite
    int frameCounter; // Contador de frames para animação

    bool isTalking;   // Flag para saber se o NPC está falando
    bool isIdle;      // Flag para saber se o NPC está idle

    
} Npc;

void InitNpc(Npc *npc);
void UpdateNpc(Npc *npc, float deltaTime, Player *player, DialogoEstado *dialogoEstado, float *dialogoTimer);
void DrawNpc(Npc *npc, Player *player, DialogoEstado dialogoEstado);
void UnloadNpc(Npc *npc);

#endif // NPC_H