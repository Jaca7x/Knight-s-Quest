#include "heart.h"


void initHeart(Heart *heart) {
    heart->position = (Vector2){0, 0};
    heart->texture = LoadTexture("resources/life/heartDrop.png"); 
    heart->isActive = false;
    heart->healthValue = 50;
}

void updateHeart(Heart *heart, float deltaTime, Player *player, Wolf *wolf, Goblin *goblin, GoblinArcher *goblinArcher, WolfRun *wolfRun) {
    
}

void drawHeart(const Heart *heart) {

}

void unloadHeart(Heart *heart) {

}



