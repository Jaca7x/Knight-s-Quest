#ifndef PLAYER_H
#define PLAYER_H


#include "../librays/raylib.h" //Biblioteca raylib

typedef struct player //struct player
{
    Vector2 position; //posição X e Y
    float speedWalk; //velocidade de caminhada
    float speedRun; //velocidade de corrida
    float speedWalkBack; //velocidade andando pra trás
    int currentFrame; //frame atual
    int frameCounter; //contador de frames
    int frameWidth; //largura do frame
    int frameHeight; //altura do frame
    Texture2D spritePlayerRun; //textura da corrida
    Texture2D spritePlayerWalk; //textura da caminhada
    Texture2D spritePlayerIdle; //textura quando está parado
    Texture2D spritePlayerJump; //textura quando está pulando
    bool isRunning; //está correndo?
    bool isMoving; //está se movendo?
    bool isJumping; //está pulando?
    float stamina; //estamina 
    int frameRun; //quantidade de frames da textura de corrida
    int frameWalk; //quantidade de frames da textura de caminhada
    int frameIdle; //quantidade de frames da textura de ficar parado
    int frameJump; //quantidade de frames da textura de pular
    float groundY; //posição y do chão
    float gravity; //gravidade
    float velocityY; //velocidade vertical do player
    float direction;
} Player;

void InitPlayer(Player *player); //Func para inicializar o player
void UpdatePlayer(Player *player, float delta); //Func para atualizar o player
void DrawPlayer(Player *player); //Func para desenhar o player  
void UnloadPlayer(Player *player); //Func para descarregar as texturas

#endif //FIM
