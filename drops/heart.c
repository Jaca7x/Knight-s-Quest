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
        
    // Goblin
    hearts[0].isActive = goblin->isDead;
    hearts[0].position = goblin->position;

    // Wolf
    hearts[1].isActive = wolf->isDead;
    hearts[1].position.x = wolf->position.x + 50;
    hearts[1].position.y = wolf->position.y + 100;

    // GoblinArcher
    hearts[2].isActive = goblinArcher->isDead;
    hearts[2].position = goblinArcher->position;

    // WolfRun
    hearts[3].isActive = wolfRun->isDead;
    hearts[3].position.x = wolfRun->position.x + 50;
    hearts[3].position.y = wolfRun->position.y + 100;

    // Verifica colisão com o player
    for (int i = 0; i < MAX_HEARTS; i++) {
        if (hearts[i].isActive) {
            Rectangle heartRect = {
                hearts[i].position.x,
                hearts[i].position.y,
                hearts[i].texture.width * 2.0f,
                hearts[i].texture.height * 2.0f
            };

            Rectangle playerRect = {
                player->position.x,
                player->position.y,
                player->frameWidth * 2.0f,
                player->frameHeight * 2.0f
            };

            if (CheckCollisionRecs(heartRect, playerRect)) {
                player->life += hearts[i].healthValue;
                if (player->life > 100) player->life = 100; // define um limite fixo

 

                // Desativa o coração
                hearts[i].isActive = false;
            }
        }
    }
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
