#include "heart.h"

float messageTimer = COOLDOWN_ZERO;
bool showMessage = false;

void triggerMassage()
{
    messageTimer = COOLDOWN_ZERO;
    showMessage = true;
}

void drawMassage(float delta, Player *player)
{
    if (showMessage)
    {
        messageTimer += delta;

        if (messageTimer < COOLDOWN)
        {
            Font medieval = LoadFontEx("assets/resources/fonts/Goudy-Mediaeval-DemiBold.ttf", FONT_SIZE, CODEPOINT_BASE, CODEPOINT_COUNT);
            DrawTextPro(
                medieval,
                "Coração coletado! +25 de vida!",
                (Vector2){player->position.x + HEART_COLLECTION_TEXT_OFFSET_X, player->position.y},
                ORIGIN_TOPLEFT,
                ROTATION,
                FONT_SIZE,
                SPACING,
                RED);
        }
        else
        {
            showMessage = false;
        }
    }
}

void ChanceSpawnDrop(Heart hearts[], bool isDead, bool *isDrop, int max, int min, int chanceDrop, int posX, int posY, Sound sound)
{
    if (!isDead)
        return;
    if (*isDrop)
        return;

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
    for (int i = 0; i < MAX_HEARTS; i++)
    {
        //POSITION
        hearts[i].position = (Vector2){POSITION_ZERO, POSITION_ZERO};

        // SPRITE
        hearts[i].texture = LoadTexture("assets/resources/sprites/life/heartDrop.png");

        // STATES
        hearts[i].isActive = false;

        // HEALING LIFE
        hearts[i].healthValue = VALUE_OF_THE_HEART;

        // SOUNDS
        hearts[i].collectSound = 
            LoadSound("assets/resources/sounds/sound_effects/drops/heart-collect.wav");
        hearts[i].spawnSound = 
            LoadSound("assets/resources/sounds/sound_effects/drops/heart-spawn.wav");
    }
}

void UpdateHearts(Heart hearts[], float delta, Player *player, Wolf *wolf, Wolf *redWolf, Wolf *whiteWolf,
                  Goblin *goblin, Goblin *redGoblin, GoblinArcher *goblinArcher, WolfRun *wolfRun, GoblinTank *goblinTank,
                  BombGoblin *bombGoblin)
{

    ChanceSpawnDrop(hearts, goblin->base.isDead, &goblin->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, GOBLIN_SPAWNING_DROP_CHANCE, goblin->base.position.x, goblin->base.position.y, hearts[0].spawnSound);
    ChanceSpawnDrop(hearts, goblinArcher->base.isDead, &goblinArcher->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, GOBLIN_SPAWNING_DROP_CHANCE, goblinArcher->base.position.x, goblinArcher->base.position.y, hearts[1].spawnSound);
    ChanceSpawnDrop(hearts, redGoblin->base.isDead, &redGoblin->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, GOBLIN_SPAWNING_DROP_CHANCE, redGoblin->base.position.x, redGoblin->base.position.y, hearts[4].spawnSound);
    ChanceSpawnDrop(hearts, goblinTank->base.isDead, &goblinTank->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, GOBLIN_SPAWNING_DROP_CHANCE, goblinTank->base.position.x + GOBLIN_TANK_HEART_OFFSET_X, goblinTank->base.position.y + GOBLIN_TANK_HEART_OFFSET_Y, hearts[7].spawnSound);
    ChanceSpawnDrop(hearts, bombGoblin->base.isDead, &bombGoblin->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, GOBLIN_SPAWNING_DROP_CHANCE, bombGoblin->base.position.x + BOMB_GOBLIN_HEART_OFFSET_X, bombGoblin->base.position.y + BOMB_GOBLIN_HEART_OFFSET_Y, hearts[8].spawnSound);

    ChanceSpawnDrop(hearts, wolf->isDead, &wolf->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, WOLF_SPAWNING_DROP_CHANCE, wolf->position.x + HEART_SPAWN_WOLF_OFFSET_X, wolf->position.y + HEART_SPAWN_WOLF_OFFSET_Y, hearts[2].spawnSound);
    ChanceSpawnDrop(hearts, wolfRun->isDead, &wolfRun->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, WOLF_SPAWNING_DROP_CHANCE, wolfRun->position.x + HEART_SPAWN_WOLF_OFFSET_X, wolfRun->position.y + HEART_SPAWN_WOLF_OFFSET_Y, hearts[3].spawnSound);
    ChanceSpawnDrop(hearts, redWolf->isDead, &redWolf->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, WOLF_SPAWNING_DROP_CHANCE, redWolf->position.x + HEART_SPAWN_WOLF_OFFSET_X, redWolf->position.y + HEART_SPAWN_WOLF_OFFSET_Y, hearts[5].spawnSound);
    ChanceSpawnDrop(hearts, whiteWolf->isDead, &whiteWolf->droppedHeart, MAX_SPAWN_CHANCE, MIN_SPAWN_CHANCE, WOLF_SPAWNING_DROP_CHANCE, whiteWolf->position.x + HEART_SPAWN_WOLF_OFFSET_X, whiteWolf->position.y + HEART_SPAWN_WOLF_OFFSET_Y, hearts[6].spawnSound);

    Rectangle playerRect =
        {
            player->position.x,
            player->position.y,
            player->frameWidth,
            player->frameHeight};

    for (int i = 0; i < MAX_HEARTS; i++)
    {
        if (hearts[i].isActive)
        {
            Rectangle heartRect = {
                hearts[i].position.x,
                hearts[i].position.y,
                hearts[i].texture.width,
                hearts[i].texture.height};

            if (CheckCollisionRecs(heartRect, playerRect))
            {
                SetSoundVolume(hearts[i].collectSound, VOLUME_MAX);
                PlaySound(hearts[i].collectSound);

                player->life += hearts[i].healthValue;
                if (player->life > PLAYER_MAX_LIFE)
                    player->life = PLAYER_MAX_LIFE;

                hearts[i].isActive = false;
                triggerMassage();
            }
        }
    }
}
void DrawHearts(const Heart hearts[], float delta, Player *player)
{
    for (int i = 0; i < MAX_HEARTS; i++)
    {
        if (hearts[i].isActive)
        {
            Vector2 origin = {
                hearts[i].texture.width / 2.0f,
                hearts[i].texture.height / 2.0f
            };

            Rectangle source = {
                0,
                0,
                hearts[i].texture.width,
                hearts[i].texture.height
            };

            Rectangle dest = {
                hearts[i].position.x + HEART_DRAW_OFFSET_X,
                hearts[i].position.y + HEART_DRAW_OFFSET_Y,
                hearts[i].texture.width * HEART_DRAW_SCALE,
                hearts[i].texture.height * HEART_DRAW_SCALE
            };

            DrawTexturePro(
                hearts[i].texture,
                source,
                dest,
                origin,
                HEART_DRAW_ROTATION,
                WHITE
            );
        }
    }
    drawMassage(delta, player);
}

void UnloadHearts(Heart hearts[])
{
    for (int i = 0; i < MAX_HEARTS; i++)
    {
        UnloadTexture(hearts[i].texture);
        UnloadSound(hearts[i].collectSound);
        UnloadSound(hearts[i].spawnSound);
    }
}
