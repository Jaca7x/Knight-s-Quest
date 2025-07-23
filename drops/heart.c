#include "heart.h"

void InitHearts(Heart hearts[]) {
    for (int i = 0; i < MAX_HEARTS; i++) {
        hearts[i].position = (Vector2){0, 0};
        hearts[i].texture = LoadTexture("resources/sprites/life/heartDrop.png");
        hearts[i].isActive = false;
        hearts[i].healthValue = 50;
    }
}

void UpdateHearts(Heart hearts[], float delta, Player *player, Wolf *wolf, Goblin *goblin, GoblinArcher *goblinArcher, WolfRun *wolfRun) {
    // Coração 0: Goblin
    hearts[0].isActive = goblin->isDead;
    hearts[0].position = goblin->position;

    // Coração 1: Wolf
    hearts[1].isActive = wolf->isDead;
    hearts[1].position.x = wolf->position.x + 50; // Ajusta a posição do coração
    hearts[1].position.y = wolf->position.y + 100;

    // Coração 2: GoblinArcher
    hearts[2].isActive = goblinArcher->isDead;
    hearts[2].position = goblinArcher->position;

    // Coração 3: WolfRun
    hearts[3].isActive = wolfRun->isDead;
    hearts[3].position = wolfRun->position;
}

void DrawHearts(const Heart hearts[]) {
    for (int i = 0; i < MAX_HEARTS; i++) {
        if (hearts[i].isActive) {
            Vector2 origin = {hearts[i].texture.width / 2.0f, hearts[i].texture.height / 2.0f};
            Rectangle source = {0, 0, hearts[i].texture.width, hearts[i].texture.height};
            Rectangle dest = {hearts[i].position.x + 30, hearts[i].position.y + 30, hearts[i].texture.width * 2, hearts[i].texture.height * 2};
            DrawTexturePro(hearts[i].texture, source, dest, origin, 0.0f, WHITE);
        }
    }
}

void UnloadHearts(Heart hearts[]) {
    for (int i = 0; i < MAX_HEARTS; i++) {
        UnloadTexture(hearts[i].texture);
    }
}
