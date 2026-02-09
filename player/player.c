#include "player.h"

Rectangle GetPlayerHitbox(Player *player)
{
    float scale = 2.0f;
    float hitboxWidth  = player->frameWidth  * scale * 0.3f;    // Reduz largura 
    float hitboxHeight = player->frameHeight * scale * 0.35f;    // Reduz altura 

    // Centraliza a hitbox no sprite
    float offsetX = (player->frameWidth  * scale - hitboxWidth)  / 2.0f;
    float offsetY = (player->frameHeight * scale - hitboxHeight) / 2.0f;

    return (Rectangle){
        player->position.x + offsetX,
        player->position.y + offsetY,
        hitboxWidth,
        hitboxHeight
    };
}

void PlayPlayerSound(Player *player, int currentMapIndex, int dialogueIndex)
{
    if (currentMapIndex >= 1 && currentMapIndex <= 4)
    {
        Sound s = player->dialogues[currentMapIndex - 1][dialogueIndex].sound;

        if (s.frameCount > 0)  
        {
            StopSound(s);
            PlaySound(s);
        }
    }
}

void PlayPlayerSoundWithPeasant(Player *player, int currentMapIndex, int dialogueIndex)
{
    if (currentMapIndex >= 6 && currentMapIndex <= 8)
    {
        Sound s = player->dialoguesWithPeasant[currentMapIndex - 6][dialogueIndex].sound;
        if (s.frameCount > 0)  
        {
            StopSound(s);
            PlaySound(s);
        }
    }
}

void AttackMonsters(int currentMapIndex, int map, bool *isDead, int positionMonster, bool *monsterHasHit, int *monsterLife, 
    Sound *monsterSoundLight, Sound *monsterSoundHeavy, Player *player)
{
    if (currentMapIndex == map && !*isDead)
    {
        float distance = fabs(positionMonster - player->position.x);
        if (player->isAttacking && distance <= player->attackRange)
        {   
            if (!*monsterHasHit)
            {
                if (player->isAttackingLight)
                {
                    *monsterLife -= player->lightDamage;
                    PlaySound(*monsterSoundLight);
                }
                else if (player->isAttackingHeavy)
                {
                    *monsterLife -= player->heavyDamage;
                    PlaySound(*monsterSoundHeavy);
                }
                *monsterHasHit = true;
            }
        } 
        else 
        {
            *monsterHasHit = false;
        }   
    }
}

// Inicializa as variáveis do jogador e carrega os sprites.
void InitPlayer(Player *player)
{
    // Carregar efeitos sonoros
    player->attackLightSound = LoadSound("resources/sounds/sound_effects/player/attack-1.wav");
    player->attackHeavySound = LoadSound("resources/sounds/sound_effects/player/attack-2.wav");
    player->jumpSound        = LoadSound("resources/sounds/sound_effects/player/jump.wav");

    player->playerDialogueWithNPC1 = LoadSound("resources/sounds/voices/player/player-dialogue-npc-1.wav");
    player->playerDialogueWithNPC2 = LoadSound("resources/sounds/voices/player/player-dialogue-npc-2.wav");

    player->dialogues[0][0].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost1-1.wav");
    player->dialogues[0][1].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost1-2.wav");
    player->dialogues[0][2].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost1-3.wav");

    // MAPA 2
    player->dialogues[1][0].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost2-1.wav");
    player->dialogues[1][1].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost2-2.wav");
    player->dialogues[1][2].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost2-3.wav");

    // MAPA 3
    player->dialogues[2][0].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost3-1.wav");
    player->dialogues[2][1].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost3-2.wav");
    player->dialogues[2][2].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost3-3.wav");

    // MAPA 4
    player->dialogues[3][0].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost4-1.wav");
    player->dialogues[3][1].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost4-2.wav");
    player->dialogues[3][2].sound = LoadSound("resources/sounds/voices/player/player-dialogueghost4-3.wav");

    //PEASANT DIALOGUES
    player->dialoguesWithPeasant[0][0].text = "Gareth II: Ei! Você aí! O que está fazendo\n no meio dessa floresta infestada de goblins?!";
    player->dialoguesWithPeasant[0][0].sound = LoadSound("resources/sounds/voices/player/player-peasant1-0.wav");

    player->dialoguesWithPeasant[0][1].text = "Gareth II: Você enlouqueceu? Eles vão te matar!";
    player->dialoguesWithPeasant[0][1].sound = LoadSound("resources/sounds/voices/player/player-peasant1-1.wav");

    player->dialoguesWithPeasant[0][2].text = "Gareth II: Você veio sozinho?! Volte para\na vila agora antes que seja tarde demais!";
    player->dialoguesWithPeasant[0][2].sound = LoadSound("resources/sounds/voices/player/player-peasant1-2.wav");

    player->dialoguesWithPeasant[1][0].text = "Gareth II: Vamos rápido, antes que os goblins\nnos vejam!";
    player->dialoguesWithPeasant[1][0].sound = LoadSound("resources/sounds/voices/player/player-peasant2-0.wav");

    player->dialoguesWithPeasant[1][1].text = "Gareth II: Você está louco? Você é apenas um\nfazendeiro, está indo para a morte!";
    player->dialoguesWithPeasant[1][1].sound = LoadSound("resources/sounds/voices/player/player-peasant2-1.wav");

    player->dialoguesWithPeasant[1][2].text = "Gareth II: Certo, vamos mais rápido.\nNão podemos perder mais tempo aqui!";
    player->dialoguesWithPeasant[1][2].sound = LoadSound("resources/sounds/voices/player/player-peasant2-2.wav");

    player->dialoguesWithPeasant[2][0].text = "Gareth II: Mas… que estranho, há tão poucos\ngoblins por aqui...";
    player->dialoguesWithPeasant[2][0].sound = LoadSound("resources/sounds/voices/player/player-peasant3-0.wav");

    player->dialoguesWithPeasant[2][1].text = "Gareth II: Sim, algo não está certo. Sinto que\nestamos sendo observados...";
    player->dialoguesWithPeasant[2][1].sound = LoadSound("resources/sounds/voices/player/player-peasant3-1.wav");

    player->dialoguesWithPeasant[2][2].text = "Gareth II: ESCONDA-SE!";
    player->dialoguesWithPeasant[2][2].sound = LoadSound("resources/sounds/voices/player/player-peasant3-2.wav");

    player->attackSoundPlayed = false;
    player->jumpSoundPlayed   = false;
    player->walkSoundPlayingCastle  = false;
    player->walkSoundPlayingGrass   = false;

    // Carregar sprites
    player->spritePlayerRun      = LoadTexture("resources/sprites/player/RUN.png");
    player->spritePlayerWalk     = LoadTexture("resources/sprites/player/WALK.png");
    player->spritePlayerIdle     = LoadTexture("resources/sprites/player/IDLE.png");
    player->spritePlayerJump     = LoadTexture("resources/sprites/player/JUMP.png");
    player->spritePlayerAttack1  = LoadTexture("resources/sprites/player/ATTACK 1.png");
    player->spritePlayerAttack2  = LoadTexture("resources/sprites/player/ATTACK 2.png");
    player->spritePlayerHurt     = LoadTexture("resources/sprites/player/HURT.png");
    player->spritePlayerDead     = LoadTexture("resources/sprites/player/DEATH.png");
    player->playerSpeech         = LoadTexture("resources/sprites/player/player-speech.png");

    // Quantidade de frames de cada animação
    player->frameRun    = 8;
    player->frameWalk   = 8;
    player->frameIdle   = 7;
    player->frameJump   = 5;
    player->frameAtk    = 6;
    player->frameHurt   = 4;
    player->frameDead   = 12;

    // Frame padrão
    player->frameWidth  = player->spritePlayerWalk.width / player->frameWalk;
    player->frameHeight = player->spritePlayerWalk.height;

    // Posição inicial
    player->position = (Vector2){ 120, 518 };

    // Física
    player->speedWalk = 200.0f;
    player->speedRun  = 300.0f;
    player->gravity   = 950.0f;
    player->groundY   = 518.0f;
    player->velocityY = 0.0f;
    player->walkTimeCastle   = 0.0f;
    player->walkTimeGrass    = 0.0f;

    // Direção inicial
    player->direction = 1.0f;

    // Controle de animação
    player->currentFrame = 0;
    player->frameCounter = 0;

    // Estados
    player->isRunning   = false;
    player->isMoving    = false;
    player->isJumping   = false;
    player->isAttacking = false;
    player->isAttackingHeavy = false;
    player->isAttackingLight = false;
    player->hasHit      = false;
    player->isDead      = false;
    player->deathAnimationDone = false;

    player->attackCooldownTimer = 0.0f;
    player->attackTimer = 0.0f;
    player->isAttackingInProgress = false;

    // Atributos
    player->stamina = 150;
    player->life    = 100;

    player->hitTimer = 0.0f;
    player->attackRange = 115.0f;
    player->lightDamage = 50;
    player->heavyDamage = 70;

    player->walkingInCastle = LoadSound("resources/sounds/sound_effects/player/walking-castle.wav");
    player->walkingInGrass = LoadSound("resources/sounds/sound_effects/player/walking-grass.wav");

    player->playerHurtSound = LoadSound("resources/sounds/sound_effects/player/player-hurt.wav");
    player->death = LoadSound("resources/sounds/sound_effects/player/game-over.wav");
}

// Atualiza o estado do jogador (movimento, física e animação).
void UpdatePlayer(Player *player, Wolf *wolf, WolfRun *wolfRun, Wolf *redWolf, Wolf *whiteWolf, Goblin *goblin, Goblin *redGoblin, 
    GoblinArcher *goblinArcher, int currentMapIndex, float delta, Npc *npc, Boss *boss, GoblinTank *goblinTank, GoblinBomb *goblinBomb)
{  
    // Verificar se morreu pela primeira vez
    if (player->life <= 0 && !player->isDead)
    {
        PlaySound(player->death);
        player->isDead = true;
        player->isMoving = false;
        player->isRunning = false;
        player->isJumping = false;
        player->isAttacking = false;
        player->hasHit = false;
        player->attackCooldownTimer = 0.0f;
        player->attackTimer = 0.0f;
        player->currentFrame = 0;
        player->frameCounter = 0;
        player->deathAnimTimer = 0.0f;
        player->deathAnimationDone = false;
    }

    if (player->isDead && !player->deathAnimationDone)
    {
        player->deathAnimTimer += delta;

        if (player->deathAnimTimer >= 0.15f) 
        {
            player->deathAnimTimer = 0.0f;
            player->currentFrame++;

            if (player->currentFrame >= player->frameDead)
            {
                player->currentFrame = player->frameDead - 1; 
                player->deathAnimationDone = true;
            }
        }
        return; 
    }

    // Verificar entradas de movimento
    bool isRunning = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && IsKeyDown(KEY_LEFT_SHIFT) && player->stamina > 0);
    bool isWalking = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && (!IsKeyDown(KEY_LEFT_SHIFT) || player->stamina <= 0));

    player->isRunning = isRunning;
    player->isMoving = isRunning || isWalking;

    // Movimento horizontal
    if (isRunning)
    {
        if (IsKeyDown(KEY_D))
        {
            player->direction = 1.0f;
            player->position.x += player->speedRun * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            player->direction = -1.0f;
            player->position.x -= player->speedRun * delta;
        }
    }
    else if (isWalking)
    {
        if (IsKeyDown(KEY_D))
        {
            player->direction = 1.0f;
            player->position.x += player->speedWalk * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            player->direction = -1.0f;
            player->position.x -= player->speedWalk * delta;
        }
    }

    //WOLFS
    AttackMonsters(currentMapIndex, RED_GOBLIN_MAP, &redWolf->isDead, redWolf->position.x, &redWolf->wolfHasHit, &redWolf->life, &redWolf->wolfHitSound, &redWolf->wolfHitSoundHeavy, player);
    AttackMonsters(currentMapIndex, MAP_WOLF_WHITE_AREA, &whiteWolf->isDead, whiteWolf->position.x, &whiteWolf->wolfHasHit, &whiteWolf->life, &whiteWolf->wolfHitSound, &whiteWolf->wolfHitSoundHeavy, player);
    AttackMonsters(currentMapIndex, MAP_WOLF_RUNNING_AREA, &wolf->isDead, wolf->position.x, &wolf->wolfHasHit, &wolf->life, &wolf->wolfHitSound, &wolf->wolfHitSoundHeavy, player);
    AttackMonsters(currentMapIndex, MAP_WOLF_RUNNING_AREA, &wolfRun->isDead, wolfRun->position.x, &wolfRun->wolfHasHit, &wolfRun->life, &wolfRun->wolfHitSound, &redWolf->wolfHitSoundHeavy, player);

    //GOBLINS
    AttackMonsters(currentMapIndex, GOBLIN_MAP, &goblin->isDead, goblin->position.x, &goblin->goblinHasHit, &goblin->life, &goblin->goblinDeathSound, &goblin->goblinDeathSound, player);
    AttackMonsters(currentMapIndex, RED_GOBLIN_MAP, &redGoblin->isDead, redGoblin->position.x, &redGoblin->goblinHasHit, &redGoblin->life, &redGoblin->RedGoblinHitSound, &redGoblin->RedGoblinHitSound, player);
    AttackMonsters(currentMapIndex, MAP_GOBLIN_ARCHER_AREA, &goblinArcher->isDead, goblinArcher->position.x, &goblinArcher->goblinHasHit, &goblinArcher->life, &goblinArcher->goblinArcherDeathSound, &goblinArcher->goblinArcherDeathSound, player);
    AttackMonsters(currentMapIndex, GOBLIN_TANK_MAP, &goblinTank->isDead, goblinTank->position.x, &goblinTank->goblinTankHasHit, &goblinTank->life, &goblinTank->soundHurtGoblinTank, &goblinTank->soundHurtGoblinTank, player);
    AttackMonsters(currentMapIndex, MAP_GOBLIN_BOMB, &goblinBomb->isDead, goblinBomb->position.x + PLAYER_HITBOX_OFFSET_X, &goblinBomb->goblinHasHit, &goblinBomb->life, &goblinTank->soundHurtGoblinTank, &goblinTank->soundHurtGoblinTank, player);
    
    //BOSS
    AttackMonsters(currentMapIndex, BOSS_MAP, &boss->isDead, boss->position.x, &boss->bossHasHit, &boss->life, &boss->bossHurtSound, &boss->bossHurtSound, player);

    // Ataque com cooldown
    player->attackCooldownTimer -= delta;
    player->attackTimer -= delta;

    if (!player->isAttackingInProgress && player->attackCooldownTimer <= 0.0f)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player->stamina > 30)
        {
            player->isAttackingInProgress = true;
            player->isAttacking = true;
            player->isAttackingLight = true;
            player->isAttackingHeavy = false;
            player->attackTimer = 0.6f;
            player->attackCooldownTimer = 0.3f;

            player->stamina -= 30.0f;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && player->stamina > 45)
        {
            player->isAttackingInProgress = true;
            player->isAttacking = true;
            player->isAttackingLight = false;
            player->isAttackingHeavy = true;
            player->attackTimer = 0.5f;
            player->attackCooldownTimer = 0.3f;

            player->stamina -= 75.0f;
        }
    }

    // Finaliza ataque após o tempo
    if (player->isAttackingInProgress && player->attackTimer <= 0.0f)
    {
        player->isAttackingInProgress = false;
        player->isAttacking = false;
        player->isAttackingLight = false;
        player->isAttackingHeavy = false;
    }

    // Controle de animação (10 FPS)
    player->frameCounter++;
    if (player->frameCounter >= (60 / 10))
    {
        player->frameCounter = 0;

        if (player->hasHit) 
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameHurt;

            if (player->currentFrame == 2)
            {
                PlaySound(player->playerHurtSound);
            }
        }
        else if (player->isJumping)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameJump;
        }
        else if (player->isAttacking)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameAtk;
        }
        else if (player->isRunning)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameRun;
        }
        else if (player->isMoving)
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameWalk;
        }
        else
        {
            player->currentFrame = (player->currentFrame + 1) % player->frameIdle;
        }
    }

    if (player->hitTimer > 0) 
    {
        player->hitTimer -= delta;
    }
    else 
    {
        player->hasHit = false;
    }
    
    if (player->stamina >= 40.0f)
    {
        if (IsKeyDown(KEY_SPACE) && !player->isJumping)
        {
            {
                player->velocityY = -400.0f;
                player->isJumping = true;
                player->stamina -= 35.0f;

                if (player->stamina <= 0)
                {
                    player->velocityY = 0.0f;
                }
            }
        }
    }
    else
    {
        if (IsKeyDown(KEY_SPACE) && !player->isJumping)
        {
            {
                player->velocityY = -300.0f;
                player->isJumping = true;
                player->stamina -= 20.0f;

                if (player->stamina <= 0)
                {
                    player->velocityY = 0.0f;
                }
            }
        }
    }

    // Gravidade
    player->velocityY += player->gravity * delta;
    player->position.y += player->velocityY * delta;

    // Colisão com chão
    if (player->position.y >= player->groundY)
    {
        player->position.y = player->groundY;
        player->velocityY  = 0;
        player->isJumping  = false;
    }

    // Limites da tela
    if (player->position.x < 0) player->position.x = 0;
    if (player->position.y < 0) player->position.y = 0;
}


// Desenha o jogador na tela
void DrawPlayer(Player *player)
{
    DrawText(TextFormat("stamina: %.2f", player->stamina), 10, 10, 20, BLACK);

    Rectangle source;
    Rectangle dest;
    Vector2 origin = { 0, 0 };

    int frameWidth;
    Texture2D texture;

    if (player->isDead)
    {
        texture = player->spritePlayerDead;
        frameWidth = texture.width / player->frameDead;
    }
    else if (player->hasHit)
    {
        texture = player->spritePlayerHurt;
        frameWidth = texture.width / player->frameHurt;
    }   
    else if (player->isJumping)
    {   
        if (player->currentFrame == 2 && !player->jumpSoundPlayed)
        {
            PlaySound(player->jumpSound);
            player->jumpSoundPlayed = true;
        }
        
        texture = player->spritePlayerJump;
        frameWidth = texture.width / player->frameJump;
    }
    else if (player->isAttacking)
    {
        if (player->currentFrame == 5 && !player->attackSoundPlayed)
        {
            if (player->isAttackingLight)
            {
                PlaySound(player->attackLightSound);
                player->attackSoundPlayed = true;
                player->jumpSoundPlayed = false;
            }
        }
        else if (player->currentFrame == 2 && !player->attackSoundPlayed)
        {
            if (player->isAttackingHeavy)
            {
                PlaySound(player->attackHeavySound);
                player->attackSoundPlayed = true;
                player->jumpSoundPlayed = false;
            }
                
        }

        if (player->isAttackingLight)
            texture = player->spritePlayerAttack1;
        else if (player->isAttackingHeavy)
            texture = player->spritePlayerAttack2;

        frameWidth = texture.width / player->frameAtk;
    }
    else if (!player->isMoving)
    {
        texture = player->spritePlayerIdle;
        frameWidth = texture.width / player->frameIdle;
        player->attackSoundPlayed = false;
        player->jumpSoundPlayed = false;
    }
    else if (player->isRunning)
    {
        texture = player->spritePlayerRun;
        frameWidth = texture.width / player->frameRun;
        player->attackSoundPlayed = false;
        player->jumpSoundPlayed = false;
    }
    else
    {
        texture = player->spritePlayerWalk;
        frameWidth = texture.width / player->frameWalk;
        player->attackSoundPlayed = false;
        player->jumpSoundPlayed = false;
    }

    // Retângulo de origem
    source = (Rectangle){
        player->currentFrame * frameWidth,
        0,
        frameWidth * player->direction, // Flip com direção
        player->frameHeight
    };

    // Retângulo de destino (2x escala)
    dest = (Rectangle){
        player->position.x,
        player->position.y,
        frameWidth * 2,
        player->frameHeight * 2
    };

    // Desenhar
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
}

// Libera as texturas do jogador da memória
void UnloadPlayer(Player *player)
{
    UnloadTexture(player->spritePlayerRun);
    UnloadTexture(player->spritePlayerWalk);
    UnloadTexture(player->spritePlayerIdle);
    UnloadTexture(player->spritePlayerJump);
    UnloadTexture(player->spritePlayerAttack1);
    UnloadTexture(player->spritePlayerAttack2);
    UnloadTexture(player->spritePlayerHurt);
    UnloadSound(player->attackLightSound);
    UnloadSound(player->attackHeavySound);
    UnloadSound(player->jumpSound);
    UnloadSound(player->playerDialogueWithNPC1);
    UnloadSound(player->playerDialogueWithNPC2);
    UnloadTexture(player->playerSpeech);
    UnloadTexture(player->spritePlayerDead);
    UnloadSound(player->walkingInCastle);
    UnloadSound(player->walkingInGrass);
    UnloadSound(player->playerHurtSound);
    UnloadSound(player->death);

    for (int map = 0; map < NUM_MAPS; map++)
    {
        for (int i = 0; i < 6; i++)
        {
            if (player->dialogues[map][i].sound.frameCount > 0)
            {
                UnloadSound(player->dialogues[map][i].sound);
            }
        }
    }

    for (int map = 0; map < NUM_MAPS_WITH_PEASANT; map++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (player->dialoguesWithPeasant[map][i].sound.frameCount > 0)
            {
                UnloadSound(player->dialoguesWithPeasant[map][i].sound);
            }
        }
    }
}
