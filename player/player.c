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


// Inicializa as variáveis do jogador e carrega os sprites.
void InitPlayer(Player *player)
{
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
    player->speedRun  = 280.0f;
    player->gravity   = 950.0f;
    player->groundY   = 518.0f;
    player->velocityY = 0.0f;

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
}

// Atualiza o estado do jogador (movimento, física e animação).
void UpdatePlayer(Player *player, Wolf *wolf, WolfRun *wolfRun, Wolf *redWolf, Wolf *whiteWolf, Goblin *goblin, GoblinArcher *goblinArcher, int currentMapIndex, float delta, Npc *npc)
{   
    // Verificar se morreu pela primeira vez
    if (player->life <= 0 && !player->isDead)
    {
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

    // Se morreu, toca a animação de morte e para o resto
    if (player->isDead && !player->deathAnimationDone)
    {
        player->deathAnimTimer += delta;

        if (player->deathAnimTimer >= 0.15f) // ajusta a velocidade do frame
        {
            player->deathAnimTimer = 0.0f;
            player->currentFrame++;

            if (player->currentFrame >= player->frameDead)
            {
                player->currentFrame = player->frameDead - 1; // para no último frame
                player->deathAnimationDone = true;
            }
        }
        return; // não processa mais nada enquanto toca animação de morte
    }

    // Se chegou aqui, ainda está vivo: resto do seu código original

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

    if (currentMapIndex == MAP_WOLF_RED_AREA)
    {
        float distanceToRedWolf = fabs(redWolf->position.x - player->position.x);

        if (player->isAttacking && distanceToRedWolf <= player->attackRange)
        {   
            if (!redWolf->wolfHasHit)
            {
                if (player->isAttackingLight)
                    redWolf->life -= player->lightDamage;
                else if (player->isAttackingHeavy)
                    redWolf->life -= player->heavyDamage;

                redWolf->wolfHasHit = true;
            }
        } 
        else 
        {
            redWolf->wolfHasHit = false;
        }   
    }

    if (currentMapIndex == MAP_WOLF_WHITE_AREA)
    {
        float distanceToWhiteWolf = fabs(whiteWolf->position.x - player->position.x);

        if (player->isAttacking && distanceToWhiteWolf <= player->attackRange)
        {   
            if (!whiteWolf->wolfHasHit)
            {
                if (player->isAttackingLight)
                    whiteWolf->life -= player->lightDamage;
                else if (player->isAttackingHeavy)
                    whiteWolf->life -= player->heavyDamage;

                whiteWolf->wolfHasHit = true;
            }
        } 
        else 
        {
            whiteWolf->wolfHasHit = false;
        }   
    }
    
    // Lógica de dano nos lobos (apenas se estiver no mapa certo)
    if (currentMapIndex == MAP_WOLF_RUNNING_AREA)
    {
        float distanceToWolf = fabs(wolf->position.x - player->position.x);

        if (player->isAttacking && distanceToWolf <= player->attackRange)
        {   
            if (!wolf->wolfHasHit)
            {
                if (player->isAttackingLight)
                    wolf->life -= player->lightDamage;
                else if (player->isAttackingHeavy)
                    wolf->life -= player->heavyDamage;

                wolf->wolfHasHit = true;
            }
        } 
        else 
        {
            wolf->wolfHasHit = false;
        }   
    }

    if (currentMapIndex == MAP_WOLF_RUNNING_AREA)
    {
        float distanceToRunningWolf = fabs(wolfRun->position.x - player->position.x);

        if (player->isAttacking && distanceToRunningWolf <= player->attackRange)
        {   
            if (!wolfRun->wolfHasHit)
            {
                if (player->isAttackingLight)
                    wolfRun->life -= player->lightDamage;
                else if (player->isAttackingHeavy)
                    wolfRun->life -= player->heavyDamage;

                wolfRun->wolfHasHit = true;
            }
        } 
        else 
        {
            wolfRun->wolfHasHit = false;
        }
    }
    

    if (currentMapIndex == GOBLIN_MAP)
    {
        float distanceToGoblin = fabs(goblin->position.x - player->position.x);

        if (player->isAttacking && distanceToGoblin <= player->attackRange)
        {
            if (!goblin->goblinHasHit)
            {
                if (player->isAttackingLight)
                {
                    goblin->life -= player->lightDamage;
                }
                else if (player->isAttackingHeavy)
                {
                    goblin->life -= player->heavyDamage;
                }

                goblin->goblinHasHit = true;
            }  
        }
        else
        {
            goblin->goblinHasHit = false;
        }   
    }

    if (currentMapIndex == MAP_GOBLIN_ARCHER_AREA)
    {
        float distanceToGoblinArcher = fabs(goblinArcher->position.x - player->position.x);

        if (player->isAttacking && distanceToGoblinArcher <= player->attackRange)
        {
            if (!goblinArcher->goblinHasHit)
            {
                if (player->isAttackingLight)
                {
                    goblinArcher->life -= player->lightDamage;
                }
                else if (player->isAttackingHeavy)
                {
                    goblinArcher->life -= player->heavyDamage;
                }

                goblinArcher->goblinHasHit = true;
            }  
        }
        else
        {
            goblinArcher->goblinHasHit = false;
        }   
    }

    // Ataque com cooldown
    player->attackCooldownTimer -= delta;
    player->attackTimer -= delta;

    if (!player->isAttackingInProgress && player->attackCooldownTimer <= 0.0f)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player->stamina > 25)
        {
            player->isAttackingInProgress = true;
            player->isAttacking = true;
            player->isAttackingLight = true;
            player->isAttackingHeavy = false;
            player->attackTimer = 0.6f;
            player->attackCooldownTimer = 0.3f;

            player->stamina -= 20.0f;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && player->stamina > 45)
        {
            player->isAttackingInProgress = true;
            player->isAttacking = true;
            player->isAttackingLight = false;
            player->isAttackingHeavy = true;
            player->attackTimer = 0.5f;
            player->attackCooldownTimer = 0.3f;

            player->stamina -= 40.0f;
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


    // Pulo
    if (IsKeyDown(KEY_SPACE) && !player->isJumping)
    {
        player->velocityY = -400.0f;
        player->isJumping = true;

        if (player->stamina <= 0)
        {
            player->velocityY = 0.0f;
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
        texture = player->spritePlayerJump;
        frameWidth = texture.width / player->frameJump;
    }
    else if (player->isAttacking)
    {
        if (player->isAttackingLight)
        {
            texture = player->spritePlayerAttack1;
        }
        else if (player->isAttackingHeavy)
        {
            texture = player->spritePlayerAttack2;
        }
        frameWidth = texture.width / player->frameAtk;
    }
    else if (!player->isMoving)
    {
        texture = player->spritePlayerIdle;
        frameWidth = texture.width / player->frameIdle;
    }
    else if (player->isRunning)
    {
        texture = player->spritePlayerRun;
        frameWidth = texture.width / player->frameRun;
    }
    else
    {
        texture = player->spritePlayerWalk;
        frameWidth = texture.width / player->frameWalk;
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

    Rectangle hitbox = GetPlayerHitbox(player);
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
}
