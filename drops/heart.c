    #include "heart.h"

    void InitHearts(Heart hearts[]) {
        for (int i = 0; i < MAX_HEARTS; i++) {
            hearts[i].position = (Vector2){0, 0};
            hearts[i].texture = LoadTexture("resources/sprites/life/heartDrop.png");
            hearts[i].isActive = false;
            hearts[i].healthValue = 25;
        }
    }

    void UpdateHearts(Heart hearts[], float delta, Player *player, Wolf *wolf, Goblin *goblin, GoblinArcher *goblinArcher, WolfRun *wolfRun) {
        
    

    // Goblin
    if (goblin->isDead && !goblin->droppedHeart) {
        int chanceGoblin = GetRandomValue(0, 99); // Chance de drop do coração (0 a 99)
        if (chanceGoblin < 70) // 70% de chance de drop
        {
            hearts[0].isActive = true; 
            hearts[0].position = goblin->position;
        } 
            goblin->droppedHeart = true;
    }

    //Wolf
    if (wolf->isDead && !wolf->droppedHeart) { 
        int chanceWolf = GetRandomValue(0, 99); // Chance de drop do coração (0 a 99)
        if (chanceWolf < 70) // 70% de chance de drop
        {
            hearts[1].isActive = true; 
            hearts[1].position.x = wolf->position.x + 50; // Ajusta a posição do coração
            hearts[1].position.y = wolf->position.y + 100; // Ajusta a posição do coração
        }
        wolf->droppedHeart = true; // Marca que o coração foi solto
    }
  
    // GoblinArcher
    if (goblinArcher->isDead && !goblinArcher->droppedHeart) {
        int chanceGoblinArcher = GetRandomValue(0, 99); // Chance de drop do coração (0 a 99)
        if (chanceGoblinArcher < 70) // 70% de chance de drop
        {
            hearts[2].isActive = true; 
            hearts[2].position = goblinArcher->position;// Ajusta a posição do coração
        }
        goblinArcher->droppedHeart = true; // Marca que o coração foi solto
    }    

    // WolfRun 
    if (wolfRun->isDead && !wolfRun->droppedHeart) {
        int chanceWolfRun = GetRandomValue(0, 99); // Chance de drop do coração (0 a 99)
        if (chanceWolfRun < 70) // 70% de chance de drop
        {
            hearts[3].isActive = true; 
            hearts[3].position.x = wolfRun->position.x + 50; // Ajusta a posição do coração
            hearts[3].position.y = wolfRun->position.y + 100; // Ajusta a posição do coração
        }
        wolfRun->droppedHeart = true; // Marca que o coração foi solto
    }
    
    // Verifica colisão com o player
    for (int i = 0; i < MAX_HEARTS; i++) {
        if (hearts[i].isActive) {
            Rectangle heartRect = {
                hearts[i].position.x,   
                hearts[i].position.y,
                hearts[i].texture.width,
                hearts[i].texture.height
            };

            Rectangle playerRect = {
                player->position.x,
                player->position.y,
                player->frameWidth,
                player->frameHeight
            };

            if (CheckCollisionRecs(heartRect, playerRect)) {
                player->life += hearts[i].healthValue;
                if (player->life > 100) player->life = 100; // define um limite fixo
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
