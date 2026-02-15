#include "player.h"

Rectangle GetPlayerHitbox(Player *player)
{
    float scale = 2.0f;
    float hitboxWidth = player->frameWidth * scale * 0.3f;    // Reduz largura
    float hitboxHeight = player->frameHeight * scale * 0.35f; // Reduz altura

    // Centraliza a hitbox no sprite
    float offsetX = (player->frameWidth * scale - hitboxWidth) / 2.0f;
    float offsetY = (player->frameHeight * scale - hitboxHeight) / 2.0f;

    return (Rectangle){
        player->position.x + offsetX,
        player->position.y + offsetY,
        hitboxWidth,
        hitboxHeight};
}

void PlayPlayerSound(Player *player, int currentMapIndex, int dialogueIndex)
{
    if (currentMapIndex >= 1 && currentMapIndex <= 4)
    {
        Sound s = player->dialogues[currentMapIndex - 1][dialogueIndex].sound;

        if (s.frameCount > FRAME_COUNTER_ZERO)
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
        if (s.frameCount > FRAME_COUNTER_ZERO)
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

void InitPlayer(Player *player)
{
    // Dialogues with Knight NPC
    player->playerDialogueWithNPC1 = LoadSound("assets/resources/sounds/voices/player/player-dialogue-npc-1.wav");
    player->playerDialogueWithNPC2 = LoadSound("assets/resources/sounds/voices/player/player-dialogue-npc-2.wav");

    // Dialogues with Ghost NPC & MAP 1
    player->dialogues[0][0].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost1-1.wav");
    player->dialogues[0][1].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost1-2.wav");
    player->dialogues[0][2].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost1-3.wav");

    // MAP 2
    player->dialogues[1][0].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost2-1.wav");
    player->dialogues[1][1].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost2-2.wav");
    player->dialogues[1][2].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost2-3.wav");

    // MAP 3
    player->dialogues[2][0].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost3-1.wav");
    player->dialogues[2][1].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost3-2.wav");
    player->dialogues[2][2].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost3-3.wav");

    // MAP 4
    player->dialogues[3][0].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost4-1.wav");
    player->dialogues[3][1].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost4-2.wav");
    player->dialogues[3][2].sound = LoadSound("assets/resources/sounds/voices/player/player-dialogueghost4-3.wav");

    // Dialogue with Peasant NPC & MAP 1
    player->dialoguesWithPeasant[0][0].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant1-0.wav");
    player->dialoguesWithPeasant[0][1].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant1-1.wav");
    player->dialoguesWithPeasant[0][2].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant1-2.wav");
    
    // MAP 2
    player->dialoguesWithPeasant[1][0].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant2-0.wav");
    player->dialoguesWithPeasant[1][1].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant2-1.wav");
    player->dialoguesWithPeasant[1][2].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant2-2.wav");
    
    // MAP 3
    player->dialoguesWithPeasant[2][0].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant3-0.wav");
    player->dialoguesWithPeasant[2][1].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant3-1.wav");
    player->dialoguesWithPeasant[2][2].sound = LoadSound("assets/resources/sounds/voices/player/player-peasant3-2.wav");


    player->attackSoundPlayed = false;
    player->jumpSoundPlayed = false;
    player->walkSoundPlayingCastle = false;
    player->walkSoundPlayingGrass = false;

    // Textures
    player->spritePlayerRun = LoadTexture("assets/resources/sprites/player/RUN.png");
    player->spritePlayerWalk = LoadTexture("assets/resources/sprites/player/WALK.png");
    player->spritePlayerIdle = LoadTexture("assets/resources/sprites/player/IDLE.png");
    player->spritePlayerJump = LoadTexture("assets/resources/sprites/player/JUMP.png");
    player->spritePlayerAttack1 = LoadTexture("assets/resources/sprites/player/ATTACK 1.png");
    player->spritePlayerAttack2 = LoadTexture("assets/resources/sprites/player/ATTACK 2.png");
    player->spritePlayerHurt = LoadTexture("assets/resources/sprites/player/HURT.png");
    player->spritePlayerDead = LoadTexture("assets/resources/sprites/player/DEATH.png");

    player->playerSpeech = LoadTexture("assets/resources/sprites/player/player-speech.png");

    // Position
    player->position = (Vector2){120, 518};
    player->direction = 1;

    // Stats
    player->stamina = 150;
    player->life = 100;
    player->speedWalk = 200.0f;
    player->speedRun = 300.0f;
    player->lightDamage = 50;
    player->heavyDamage = 70;

    // Animation frame count
    player->frameRun = 8;
    player->frameWalk = 8;
    player->frameIdle = 7;
    player->frameJump = 5;
    player->frameAtk = 6;
    player->frameHurt = 4;
    player->frameDead = 12;

    player->scale = 2;

    // Frame size   
    player->frameWidth = player->spritePlayerWalk.width / player->frameWalk;
    player->frameHeight = player->spritePlayerWalk.height;

    player->currentFrame = 0;
    player->frameCounter = 0;

    // Combat & range
    player->attackCooldownTimer = 0.0f;
    player->attackTimer = 0.0f;
    player->hitTimer = 0.0f;

    player->attackRange = 115.0f;

    // Physical
    player->gravity = 950.0f;
    player->groundY = 518.0f;
    player->velocityY = 0.0f;

    // Time
    player->walkTimeCastle = 0.0f;
    player->walkTimeGrass = 0.0f;

    // States
    player->isRunning = false;
    player->isMoving = false;
    player->isJumping = false;
    player->isAttacking = false;
    player->isAttackingHeavy = false;
    player->isAttackingLight = false;
    player->hasHit = false;
    player->isDead = false;
    player->deathAnimationDone = false;

    player->isAttackingInProgress = false;
    
    // Sounds
    player->walkingInCastle =
        LoadSound("assets/resources/sounds/sound_effects/player/walking-castle.wav");

    player->walkingInGrass =
        LoadSound("assets/resources/sounds/sound_effects/player/walking-grass.wav");

    player->playerHurtSound =
        LoadSound("assets/resources/sounds/sound_effects/player/player-hurt.wav");

    player->death =
        LoadSound("assets/resources/sounds/sound_effects/player/game-over.wav");

    player->attackLightSound = 
        LoadSound("assets/resources/sounds/sound_effects/player/attack-1.wav");

    player->attackHeavySound = 
        LoadSound("assets/resources/sounds/sound_effects/player/attack-2.wav");

    player->jumpSound = 
        LoadSound("assets/resources/sounds/sound_effects/player/jump.wav");
}

void UpdatePlayer(Player *player, Wolf *wolf, WolfRun *wolfRun, Wolf *redWolf, Wolf *whiteWolf, Goblin *goblin,
                  Goblin *redGoblin, GoblinArcher *goblinArcher, int currentMapIndex, float delta, Npc *npc,
                  Boss *boss, GoblinTank *goblinTank, BombGoblin *bombGoblin)
{
    if (player->life <= LIFE_ZERO && !player->isDead)
    {
        PlaySound(player->death);
        player->isDead = true;
        player->isMoving = false;
        player->isRunning = false;
        player->isJumping = false;
        player->isAttacking = false;
        player->hasHit = false;
        player->attackCooldownTimer = COOLDOWN_ZERO;
        player->attackTimer = TIMER_ZERO;
        player->currentFrame = CURRENT_FRAME_ZERO;
        player->frameCounter = FRAME_COUNTER_ZERO;
        player->deathAnimTimer = TIMER_ZERO;
        player->deathAnimationDone = false;
    }

    if (player->isDead && !player->deathAnimationDone)
    {
        player->deathAnimTimer += delta;

        if (player->deathAnimTimer >= DEATH_ANIM_FRAME_TIME)
        {
            player->deathAnimTimer = ANIM_DEAD_ZERO;
            player->currentFrame++;

            if (player->currentFrame >= player->frameDead)
            {
                player->currentFrame = player->frameDead - PREVIOUS_FRAME;
                player->deathAnimationDone = true;
            }
        }
        return;
    }

    bool isRunning = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && IsKeyDown(KEY_LEFT_SHIFT) && player->stamina > STAMINA_ZERO);
    bool isWalking = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && (!IsKeyDown(KEY_LEFT_SHIFT) || player->stamina <= STAMINA_ZERO));

    player->isRunning = isRunning;
    player->isMoving = isRunning || isWalking;

    if (isRunning)
    {
        if (IsKeyDown(KEY_D))
        {
            player->direction = DIRECTION_RIGHT;
            player->position.x += player->speedRun * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            player->direction = DIRECTION_LEFT;
            player->position.x -= player->speedRun * delta;
        }
    }
    else if (isWalking)
    {
        if (IsKeyDown(KEY_D))
        {
            player->direction = DIRECTION_RIGHT;
            player->position.x += player->speedWalk * delta;
        }
        if (IsKeyDown(KEY_A))
        {
            player->direction = DIRECTION_LEFT;
            player->position.x -= player->speedWalk * delta;
        }
    }

    // WOLFS
    AttackMonsters(currentMapIndex, RED_GOBLIN_MAP, &redWolf->isDead, redWolf->position.x, &redWolf->wolfHasHit, &redWolf->life, &redWolf->wolfHitSound, &redWolf->wolfHitSoundHeavy, player);
    AttackMonsters(currentMapIndex, MAP_WOLF_WHITE_AREA, &whiteWolf->isDead, whiteWolf->position.x, &whiteWolf->wolfHasHit, &whiteWolf->life, &whiteWolf->wolfHitSound, &whiteWolf->wolfHitSoundHeavy, player);
    AttackMonsters(currentMapIndex, MAP_WOLF_RUNNING_AREA, &wolf->isDead, wolf->position.x, &wolf->wolfHasHit, &wolf->life, &wolf->wolfHitSound, &wolf->wolfHitSoundHeavy, player);
    AttackMonsters(currentMapIndex, MAP_WOLF_RUNNING_AREA, &wolfRun->isDead, wolfRun->position.x, &wolfRun->wolfHasHit, &wolfRun->life, &wolfRun->wolfHitSound, &redWolf->wolfHitSoundHeavy, player);

    // GOBLINS
    AttackMonsters(currentMapIndex, GOBLIN_MAP, &goblin->base.isDead, goblin->base.position.x, &goblin->base.monsterHasHit, &goblin->life, &goblin->goblinDeathSound, &goblin->goblinDeathSound, player);
    AttackMonsters(currentMapIndex, RED_GOBLIN_MAP, &redGoblin->base.isDead, redGoblin->base.position.x, &redGoblin->base.monsterHasHit, &redGoblin->life, &redGoblin->RedGoblinHitSound, &redGoblin->RedGoblinHitSound, player);
    AttackMonsters(currentMapIndex, MAP_GOBLIN_ARCHER_AREA, &goblinArcher->base.isDead, goblinArcher->base.position.x, &goblinArcher->base.monsterHasHit, &goblinArcher->life, &goblinArcher->goblinArcherDeathSound, &goblinArcher->goblinArcherDeathSound, player);
    AttackMonsters(currentMapIndex, GOBLIN_TANK_MAP, &goblinTank->base.isDead, goblinTank->base.position.x + GOBLIN_TANK_HURTBOX_OFFSET_X, &goblinTank->base.monsterHasHit, &goblinTank->life, &goblinTank->soundHurtGoblinTank, &goblinTank->soundHurtGoblinTank, player);
    AttackMonsters(currentMapIndex, MAP_BOMB_GOBLIN, &bombGoblin->base.isDead, bombGoblin->base.position.x + PLAYER_HITBOX_OFFSET_X, &bombGoblin->base.monsterHasHit, &bombGoblin->life, &goblinTank->soundHurtGoblinTank, &goblinTank->soundHurtGoblinTank, player);

    // BOSS
    AttackMonsters(currentMapIndex, BOSS_MAP, &boss->isDead, boss->position.x, &boss->bossHasHit, &boss->life, &boss->bossHurtSound, &boss->bossHurtSound, player);

    player->attackCooldownTimer -= delta;
    player->attackTimer -= delta;

    if (!player->isAttackingInProgress && player->attackCooldownTimer <= COOLDOWN_ZERO)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player->stamina > 30)
        {
            player->isAttackingInProgress = true;
            player->isAttacking = true;
            player->isAttackingLight = true;
            player->isAttackingHeavy = false;
            player->attackTimer = PLAYER_ATTACK_TIMER;
            player->attackCooldownTimer = PLAYER_ATTACK_COOLDOWN_TIMER;

            player->stamina -= PLAYER_STAMINA_SPENT_LIGHT_ATTACKS;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && player->stamina > 45)
        {
            player->isAttackingInProgress = true;
            player->isAttacking = true;
            player->isAttackingLight = false;
            player->isAttackingHeavy = true;
            player->attackTimer = PLAYER_ATTACK_TIMER;
            player->attackCooldownTimer = PLAYER_ATTACK_COOLDOWN_TIMER;

            player->stamina -= PLAYER_STAMINA_SPENT_HEAVY_ATTACKS;
        }

        if (player->stamina <= STAMINA_ZERO)
            player->stamina = STAMINA_ZERO;
    }

    if (player->isAttackingInProgress && player->attackTimer <= TIMER_ZERO)
    {
        player->isAttackingInProgress = false;
        player->isAttacking = false;
        player->isAttackingLight = false;
        player->isAttackingHeavy = false;
    }

    player->frameCounter++;
    if (player->frameCounter >= (GAME_FPS / ANIMATION_FPS))
    {
        player->frameCounter = FRAME_COUNTER_ZERO;

        if (player->hasHit)
        {
            player->currentFrame = (player->currentFrame + NEXT_FRAME) % player->frameHurt;

            if (player->currentFrame == 2)
            {
                PlaySound(player->playerHurtSound);
            }
        }
        else if (player->isJumping)
        {
            player->currentFrame = (player->currentFrame + NEXT_FRAME) % player->frameJump;
        }
        else if (player->isAttacking)
        {
            player->currentFrame = (player->currentFrame + NEXT_FRAME) % player->frameAtk;
        }
        else if (player->isRunning)
        {
            player->currentFrame = (player->currentFrame + NEXT_FRAME) % player->frameRun;
        }
        else if (player->isMoving)
        {
            player->currentFrame = (player->currentFrame + NEXT_FRAME) % player->frameWalk;
        }
        else
        {
            player->currentFrame = (player->currentFrame + NEXT_FRAME) % player->frameIdle;
        }
    }

    if (player->hitTimer > TIMER_ZERO)
    {
        player->hitTimer -= delta;
    }
    else
    {
        player->hasHit = false;
    }

    float jumpForce = JUMP_FORCE_NONE;
    float jumpCost = 0.0f;

    if (IsKeyPressed(KEY_SPACE) && !player->isJumping)
    {
        if (player->stamina >= JUMP_STAMINA_HIGH)
        {
            jumpForce = JUMP_FORCE_HIGH;
            jumpCost = JUMP_COST_HIGH;
        }
        else if (player->stamina >= JUMP_STAMINA_MEDIUM)
        {
            jumpForce = JUMP_FORCE_MEDIUM;
            jumpCost = JUMP_COST_MEDIUM;
        }

        if (jumpForce != JUMP_FORCE_NONE)
        {
            player->velocityY = jumpForce;
            player->isJumping = true;

            player->stamina -= jumpCost;
            if (player->stamina < STAMINA_ZERO)
                player->stamina = STAMINA_ZERO;
        }
    }

    // Gravity
    player->velocityY += player->gravity * delta;
    player->position.y += player->velocityY * delta;

    // Ground colision
    if (player->position.y >= player->groundY)
    {
        player->position.y = player->groundY;
        player->velocityY = 0;
        player->isJumping = false;
    }

    // Screen limits
    if (player->position.x < POSITION_ZERO)
        player->position.x = POSITION_ZERO;
    if (player->position.y < POSITION_ZERO)
        player->position.y = POSITION_ZERO;
}

void DrawPlayer(Player *player)
{
    Rectangle source;
    Rectangle dest;
    Vector2 origin = ORIGIN_TOPLEFT;

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

    source = (Rectangle)
    {
        player->currentFrame * frameWidth,
        SPRITE_ROW_BASE,
        frameWidth * player->direction,
        player->frameHeight 
    };

    dest = (Rectangle)
    {
        player->position.x,
        player->position.y,
        frameWidth * player->scale,
        player->frameHeight * player->scale 
    };

    DrawTexturePro(texture, source, dest, origin, ROTATION, WHITE);
}

void UnloadPlayer(Player *player)
{
    UnloadTexture(player->spritePlayerRun);
    UnloadTexture(player->spritePlayerWalk);
    UnloadTexture(player->spritePlayerIdle);
    UnloadTexture(player->spritePlayerJump);
    UnloadTexture(player->spritePlayerAttack1);
    UnloadTexture(player->spritePlayerAttack2);
    UnloadTexture(player->spritePlayerHurt);

    UnloadTexture(player->playerSpeech);
    UnloadTexture(player->spritePlayerDead);
    
    UnloadSound(player->attackLightSound);
    UnloadSound(player->attackHeavySound);
    UnloadSound(player->jumpSound);
    UnloadSound(player->playerDialogueWithNPC1);
    UnloadSound(player->playerDialogueWithNPC2);

    UnloadSound(player->walkingInCastle);
    UnloadSound(player->walkingInGrass);
    UnloadSound(player->playerHurtSound);
    UnloadSound(player->death);

    for (int map = 0; map < NUM_MAPS; map++)
    {
        for (int i = 0; i < 6; i++)
        {
            if (player->dialogues[map][i].sound.frameCount > FRAME_COUNTER_ZERO)
            {
                UnloadSound(player->dialogues[map][i].sound);
            }
        }
    }

    for (int map = 0; map < NUM_MAPS_WITH_PEASANT; map++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (player->dialoguesWithPeasant[map][i].sound.frameCount > FRAME_COUNTER_ZERO)
            {
                UnloadSound(player->dialoguesWithPeasant[map][i].sound);
            }
        }
    }
}
