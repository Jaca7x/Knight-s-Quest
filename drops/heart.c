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

        void ChanceSpawnDrop(Heart hearts[], bool isDead, bool *isDrop, int max, int min, int chanceDrop, int posX, int posY, Sound sound)
        {
            if(!isDead) return;
            if(*isDrop) return;

            *isDrop = true;

            int chance = GetRandomValue(min, max);

            if (chance < chanceDrop)
            {
                for (int i = 0; i < MAX_HEARTS; i++)
                {
                    if (!hearts[i].isActive)
                    {
                        hearts[i].isActive = true;
                        PlaySound(sound);
                        hearts[i].position.x = posX;
                        hearts[i].position.y = posY;
                        break;
                    }
                }
            }
        }
        

    void InitHearts(Heart hearts[]) 
    {
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
    Goblin *goblin, Goblin *redGoblin, GoblinArcher *goblinArcher, WolfRun *wolfRun, GoblinTank *goblinTank,
    GoblinBomb *goblinBomb) 
{
   
    ChanceSpawnDrop(hearts, goblin->isDead, &goblin->droppedHeart, 100, 0, 70, goblin->position.x, goblin->position.y, hearts[0].spawnSound);
    ChanceSpawnDrop(hearts, goblinArcher->isDead, &goblinArcher->droppedHeart, 100, 0, 70, goblinArcher->position.x, goblinArcher->position.y, hearts[1].spawnSound);
    ChanceSpawnDrop(hearts, redGoblin->isDead, &redGoblin->droppedHeart, 100, 0, 50, redGoblin->position.x, redGoblin->position.y, hearts[4].spawnSound);
    ChanceSpawnDrop(hearts, goblinTank->base.isDead, &goblinTank->droppedHeart, 100, 0, 70, goblinTank->base.position.x + 130, goblinTank->base.position.y + 60, hearts[7].spawnSound);
    ChanceSpawnDrop(hearts, goblinBomb->isDead, &goblinBomb->droppedHeart, 100, 0, 60, goblinBomb->position.x + 80, goblinBomb->position.y + 130, hearts[8].spawnSound);

    ChanceSpawnDrop(hearts, wolf->isDead, &wolf->droppedHeart, 100, 0, 60, wolf->position.x + 50, wolf->position.y + 100, hearts[2].spawnSound);
    ChanceSpawnDrop(hearts, wolfRun->isDead, &wolfRun->droppedHeart, 100, 0, 60, wolfRun->position.x + 50, wolfRun->position.y + 100, hearts[3].spawnSound);
    ChanceSpawnDrop(hearts, redWolf->isDead, &redWolf->droppedHeart, 100, 0, 60, redWolf->position.x + 50, redWolf->position.y + 100, hearts[5].spawnSound);
    ChanceSpawnDrop(hearts, whiteWolf->isDead, &whiteWolf->droppedHeart, 100, 0, 60, whiteWolf->position.x + 50, whiteWolf->position.y + 100, hearts[6].spawnSound);
    
    

    Rectangle playerRect = 
        {
            player->position.x,
            player->position.y,
            player->frameWidth,
            player->frameHeight
        };
            
    for (int i = 0; i < MAX_HEARTS; i++) {
        if (hearts[i].isActive) 
        {
            Rectangle heartRect = {
                hearts[i].position.x,   
                hearts[i].position.y,
                hearts[i].texture.width,
                hearts[i].texture.height
            };

            if (CheckCollisionRecs(heartRect, playerRect)) 
            {   
                SetSoundVolume(hearts[i].collectSound, 1.0f);
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
