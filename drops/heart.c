    #include "heart.h"

    float messageTimer = 0.0f;
    bool showMessage = false;

    void triggerMassage() {
        messageTimer = 0.0f;
        showMessage = true;
    }

    void drawMassage(float delta, Player *player) {
    if (showMessage) 
    {
        messageTimer += delta;

        if (messageTimer < 1.0f) 
        {
            Font medieval = LoadFontEx("resources/fonts/Goudy-Mediaeval-DemiBold.ttf", 32, 0, 250);
            DrawTextPro(
                medieval,
                "Coração coletado! +25 de vida!", 
                (Vector2){player->position.x - 50, player->position.y}, 
                (Vector2){0, 0}, 
                0.0f, 
                30.0f, 
                1.0f, 
                (Color){255, 0, 0, 255}
            );
        } else 
        {
            showMessage = false;
        }
    }
}

    void InitHearts(Heart hearts[]) {
        for (int i = 0; i < MAX_HEARTS; i++) {
            hearts[i].position = (Vector2){0, 0};
            hearts[i].texture = LoadTexture("resources/sprites/life/heartDrop.png");
            hearts[i].isActive = false;
            hearts[i].healthValue = 25;

            hearts[i].collectSound = LoadSound("resources/sounds/sound_effects/drops/heart-collect.wav");
            hearts[i].spawnSound = LoadSound("resources/sounds/sound_effects/drops/heart-spawn.wav");
        }
    }

void UpdateHearts(Heart hearts[], float delta, Player *player, Wolf *wolf, Wolf *redWolf, Wolf *whiteWolf, 
    Goblin *goblin, Goblin *redGoblin, GoblinArcher *goblinArcher, WolfRun *wolfRun, GoblinTank *goblinTank) 
{
    // Goblin
    if (goblin->isDead && !goblin->droppedHeart) 
    {
        int chanceGoblin = GetRandomValue(0, 100); 
        if (chanceGoblin < 70)
        {
            hearts[0].isActive = true; 
            PlaySound(hearts[0].spawnSound);
            hearts[0].position = goblin->position;
        } 
            goblin->droppedHeart = true;
    }

    //Wolf
    if (wolf->isDead && !wolf->droppedHeart) 
    { 
        int chanceWolf = GetRandomValue(0, 100); 
        if (chanceWolf < 50)
        {
            hearts[1].isActive = true; 
            PlaySound(hearts[1].spawnSound);    
            hearts[1].position.x = wolf->position.x + 50; 
            hearts[1].position.y = wolf->position.y + 100;
        }
        wolf->droppedHeart = true;
    }
  
    // GoblinArcher
    if (goblinArcher->isDead && !goblinArcher->droppedHeart) 
    {
        int chanceGoblinArcher = GetRandomValue(0, 100); 
        if (chanceGoblinArcher < 70) 
        {
            hearts[2].isActive = true; 
            PlaySound(hearts[2].spawnSound);
            hearts[2].position = goblinArcher->position;
        }
        goblinArcher->droppedHeart = true; 
    }    

    // WolfRun 
    if (wolfRun->isDead && !wolfRun->droppedHeart) 
    {
        int chanceWolfRun = GetRandomValue(0, 100); 
        if (chanceWolfRun < 60)
        {
            hearts[3].isActive = true; 
            PlaySound(hearts[3].spawnSound);
            hearts[3].position.x = wolfRun->position.x + 50; 
            hearts[3].position.y = wolfRun->position.y + 100; 
        }
        wolfRun->droppedHeart = true; 
    }

    // RedWolf
    if (redWolf->isDead && !redWolf->droppedHeart) 
    {
        int chanceRedWolf = GetRandomValue(0, 100);
        if (chanceRedWolf < 60) 
        {
            hearts[4].isActive = true; 
            PlaySound(hearts[4].spawnSound);
            hearts[4].position.x = redWolf->position.x + 50; 
            hearts[4].position.y = redWolf->position.y + 100; 
        }
        redWolf->droppedHeart = true;
    }

    // WhiteWolf
    if (whiteWolf->isDead && !whiteWolf->droppedHeart) 
    {
        int chanceWhiteWolf = GetRandomValue(0, 100); 
        if (chanceWhiteWolf < 60) 
        {
            hearts[5].isActive = true; 
            PlaySound(hearts[5].spawnSound);
            hearts[5].position.x = whiteWolf->position.x + 50;
            hearts[5].position.y = whiteWolf->position.y + 100; 
        }
        whiteWolf->droppedHeart = true; 
    }

    // RedGoblin
    if (redGoblin->isDead && !redGoblin->droppedHeart) 
    {
        int chanceRedGoblin = GetRandomValue(0, 100); 
        if (chanceRedGoblin < 60) 
        {
            hearts[6].isActive = true; 
            PlaySound(hearts[6].spawnSound);
            hearts[6].position.x = redGoblin->position.x; 
            hearts[6].position.y = redGoblin->position.y; 
        }
        redGoblin->droppedHeart = true; 
    }

    // GoblinTank
    if (goblinTank->isDead && !goblinTank->droppedHeart)
    {
        int chanceGoblinTank = GetRandomValue(0, 100);

        if (chanceGoblinTank > 60)
        {
            hearts[7].isActive = true; 
            PlaySound(hearts[7].spawnSound);
            hearts[7].position.x = goblinTank->position.x; 
            hearts[7].position.y =  goblinTank->position.y; 
        }
        
        goblinTank->droppedHeart = true;
    }
    
    for (int i = 0; i < MAX_HEARTS; i++) {
        if (hearts[i].isActive) 
        {
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

            if (CheckCollisionRecs(heartRect, playerRect)) 
            {
                PlaySound(hearts[i].collectSound);
                player->life += hearts[i].healthValue;
                if (player->life > 100) player->life = 100; 
                hearts[i].isActive = false;
                triggerMassage(); 
            }
        }
    }
}
    void DrawHearts(const Heart hearts[], float delta, Player *player) {
        for (int i = 0; i < MAX_HEARTS; i++) 
        {
            if (hearts[i].isActive) 
            {
                Vector2 origin = {hearts[i].texture.width / 2.0f, hearts[i].texture.height / 2.0f};
                Rectangle source = {0, 0, hearts[i].texture.width, hearts[i].texture.height};
                Rectangle dest = {hearts[i].position.x + 30, hearts[i].position.y + 30, hearts[i].texture.width * 2, hearts[i].texture.height * 2};
                DrawTexturePro(hearts[i].texture, source, dest, origin, 0.0f, WHITE);
            }
        }
        drawMassage(delta, player);
    }

    void UnloadHearts(Heart hearts[]) {
        for (int i = 0; i < MAX_HEARTS; i++) {
            UnloadTexture(hearts[i].texture);
            UnloadSound(hearts[i].collectSound);
            UnloadSound(hearts[i].spawnSound);
        }
    }
