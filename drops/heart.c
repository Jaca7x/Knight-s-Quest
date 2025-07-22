#include "heart.h"


void InitHeart(Heart *heart) {
    heart->position = (Vector2){0, 0};
    heart->texture = LoadTexture("resources/sprites/life/heartDrop.png"); 
    heart->isActive = false;
    heart->healthValue = 50;
}

void UpdateHeart(Heart *heart, float deltaTime, Player *player, Wolf *wolf, Goblin *goblin, GoblinArcher *goblinArcher, WolfRun *wolfRun) {
    heart->isActive = false;

    if (goblin->isDead) {
        heart->isActive = true;
        heart->position = goblin->position;
        return;
    }

    if (wolf->isDead) {
        heart->isActive = true;
        heart->position = wolf->position;
        return;
    }

    if (goblinArcher->isDead) {
        heart->isActive = true;
        heart->position = goblinArcher->position;
        return;
    }

    if (wolfRun->isDead) {
        heart->isActive = true;
        heart->position = wolfRun->position;
        return;
    }
}

void DrawHeart(const Heart *heart) {
   Vector2 origin = {heart->texture.width / 2.0f, heart->texture.height / 2.0f};
    Rectangle source = {0, 0, heart->texture.width, heart->texture.height};
    Rectangle dest = {heart->position.x + 30, heart->position.y + 30, heart->texture.width * 2, heart->texture.height * 2};
    
    if (heart->isActive) {
        DrawTexturePro(heart->texture, source, dest, origin, 0.0f, WHITE);
    }
}

void UnloadHeart(Heart *heart) {

}



