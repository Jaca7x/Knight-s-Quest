#include "goblin.h"

/// Função de colisão AABB
bool CheckCollisionGoblin(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void DrawGoblinLifeBar(Goblin *goblin)
{
    float barWidth = 60.0f;
    float barHeight = 8.0f;
    float x = goblin->position.x + 10;  // ajuste horizontal
    float y = goblin->position.y - 10;  // acima do sprite

    float maxLife = (goblin->life > 50) ? 70.0f : 50.0f;

    float lifePercent = goblin->life / maxLife;
    if (lifePercent < 0) lifePercent = 0;

    float currentBarWidth = barWidth * lifePercent;

    // Fundo (vermelho)
    DrawRectangle(x, y, barWidth, barHeight, RED);

    // Vida atual (verde)
    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN);

    // Contorno
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
}

void InitGoblinBase(Goblin *goblin, Vector2 pos) 
{
    goblin->position = pos;

    goblin->goblinSpriteWalk = LoadTexture("resources/sprites/goblin/goblin-walk.png");
    goblin->goblinSpriteHurt = LoadTexture("resources/sprites/goblin/goblin-hurt.png");
    goblin->goblinSpriteDead = LoadTexture("resources/sprites/goblin/goblin-dead.png");
    goblin->goblinSpriteIdle = LoadTexture("resources/sprites/goblin/goblin-idle.png");
    goblin->goblinSpriteAtk = LoadTexture("resources/sprites/goblin/goblin-atk.png");

    goblin->frameWalk = 6;
    goblin->frameHurt = 4;
    goblin->frameDead = 8;
    goblin->frameIdle = 8;
    goblin->frameAtk = 6;

    goblin->currentFrame = 0;
    goblin->frameCounter = 8;
    goblin->frameWidth = goblin->goblinSpriteIdle.width / goblin->frameIdle;
    goblin->frameHeight = goblin->goblinSpriteIdle.height;

    goblin->isIdle = true;
    goblin->isWalking = false;
    goblin->isAtacking = false;
    goblin->isDead = false;
    goblin->goblinHasHit = false;
    goblin->deathAnimationDone = false;

    goblin->viewPlayer = 300.0f;
    goblin->goblinAttackRangeRight = 100.0f;
    goblin->goblinAttackRangeLeft = 15.0f;
    goblin->goblinHasHitPlayer = false;
    goblin->direction = 1;

    goblin->hurtTimer = 0.0f;
    goblin->hurtDuration = 0.4f;

    goblin->attackTimer = 0.0f;
    goblin->attackCooldown = 1.0f;

    goblin->scale = 0.02f;
    goblin->frameFactor = 0.15f;

    goblin->life = 50;
    goblin->maxLife = 50;
    goblin->speed = 100.0f;
    goblin->baseSpeed = 100.0f;

    goblin->damage = 20;

    goblin->droppedHeart = false;

    goblin->goblinDeathSound = LoadSound("resources/sounds/sound_effects/goblin/goblin-death.wav");
    goblin->goblinCutSound = LoadSound("resources/sounds/sound_effects/goblin/goblin-cut.wav");  

    goblin->RedGoblinHitSound = LoadSound("resources/sounds/sound_effects/goblin/red-goblin-hit.wav");
}

void InitRedGoblin(Goblin *goblin, Vector2 pos) 
{
    InitGoblinBase(goblin, pos);

    goblin->life = 70;
    goblin->maxLife = 70;
    goblin->speed = 80.0f;
    goblin->baseSpeed = 80.0f;
    goblin->damage = 40;

    goblin->goblinSpriteWalk = LoadTexture("resources/sprites/redGoblin/Walk.png");
    goblin->goblinSpriteHurt = LoadTexture("resources/sprites/redGoblin/Hurt.png");
    goblin->goblinSpriteDead = LoadTexture("resources/sprites/redGoblin/Dead.png");
    goblin->goblinSpriteIdle = LoadTexture("resources/sprites/redGoblin/Idle.png");
    goblin->goblinSpriteAtk = LoadTexture("resources/sprites/redGoblin/Attack.png");

    goblin->goblinDeathSound = LoadSound("resources/sounds/sound_effects/goblin/red-goblin-death.wav");
    goblin->RedGoblinHitSound = LoadSound("resources/sounds/sound_effects/goblin/red-goblin-hit.wav");
}

void UpdateGoblin(Goblin *goblin, Player *player, int currentMapIndex, float delta)
{
    // --- MORTE ---
    if (goblin->life <= 0 && !goblin->isDead) 
    {
        PlaySound(goblin->goblinDeathSound);
        goblin->isDead = true;
        goblin->goblinHasHit = false;
        goblin->currentFrame = 0;
        goblin->goblinHasHitPlayer = false;
        goblin->attackTimer = 0.0f; 
        goblin->frameDead = 7;
        goblin->deathAnimationDone = false;
        goblin->isWalking = false;
        goblin->isAtacking = false;
        goblin->isIdle = false;
    }

    if (goblin->isDead && goblin->deathAnimationDone)
        return; 

    // --- HURT ---
    if (goblin->goblinHasHit) 
    {
        goblin->isIdle = false;
        goblin->isWalking = false;
        goblin->isAtacking = false;

        goblin->hurtTimer += delta;

        if (goblin->hurtTimer >= goblin->hurtDuration) 
        {
            goblin->goblinHasHit = false;
            goblin->hurtTimer = 0.0f;
            goblin->isIdle = true;
        }
    }

    // --- FRAME UPDATE ---
    goblin->frameCounter++;
    if (goblin->frameCounter >= (60 / 10))
    {
        goblin->frameCounter = 0;
        if (goblin->goblinHasHit && !goblin->isDead)
        {
            goblin->currentFrame = (goblin->currentFrame + 1) % goblin->frameHurt;
        }
        else if (goblin->isDead && !goblin->deathAnimationDone) 
        {
            goblin->deathAnimTimer += delta;
            if (goblin->deathAnimTimer >= 0.2f)
            {   
                goblin->frameDead++;
                goblin->deathAnimTimer = 0.0f;

                if (goblin->frameDead >= 8)
                {
                    goblin->frameDead = 7;
                    goblin->deathAnimationDone = true;
                }
            }
            goblin->currentFrame = goblin->frameDead;   
        }
        else if (goblin->isAtacking) 
        {
            if (goblin->currentFrame == 1)
            {
                PlaySound(goblin->goblinCutSound);
            }   
            goblin->currentFrame = (goblin->currentFrame + 1) % goblin->frameAtk;
        }
        else if (goblin->isWalking) 
        {
            goblin->currentFrame = (goblin->currentFrame + 1) % goblin->frameWalk;
        } 
        else if (goblin->isIdle) 
        {
            goblin->currentFrame = (goblin->currentFrame + 1) % goblin->frameIdle;
        }  
    }

    // --- ATAQUE / COOLDOWN ---
    if (goblin->attackTimer > 0.0f) 
    {
        goblin->attackTimer -= delta;
        goblin->isAtacking = true;
        goblin->isWalking = false;
        goblin->isIdle = false;
    }
    else
    {
        // --- MOVIMENTO ---
        float push = 50.0f;
        float distanceToPlayer = fabs(player->position.x - goblin->position.x);

        if (!goblin->isDead && distanceToPlayer <= goblin->viewPlayer && !goblin->goblinHasHit)
        {
            goblin->direction = (player->position.x > goblin->position.x) ? 1 : -1;

            goblin->isAtacking = false;
            goblin->isWalking = true;
            goblin->isIdle = false;

            goblin->position.x += goblin->speed * goblin->direction * delta;

            // Range de ataque
            if (distanceToPlayer <= goblin->goblinAttackRangeRight && goblin->direction == -1)
            {
                goblin->isAtacking = true;
                goblin->attackTimer = goblin->attackCooldown;
                player->life -= goblin->damage;
                goblin->goblinHasHitPlayer = true;
                player->hasHit = true;
                player->hitTimer = 0.4f;

                // Knockback
                player->position.x += (player->position.x < goblin->position.x) ? -push : push;
            }
            else if (distanceToPlayer <= goblin->goblinAttackRangeLeft && goblin->direction == 1)
            {
                goblin->isAtacking = true;
                goblin->attackTimer = goblin->attackCooldown;
                player->life -= goblin->damage;
                goblin->goblinHasHitPlayer = true;
                player->hasHit = true;
                player->hitTimer = 0.4f;

                // Knockback
                player->position.x += (player->position.x < goblin->position.x) ? -push : push;
            }
        }
        else
        {
            goblin->isIdle = true;
            goblin->isWalking = false;
            goblin->isAtacking = false;
            goblin->goblinHasHitPlayer = false;
        }

        // Colisão simples com o jogador
        if (!goblin->isDead && CheckCollisionGoblin(
                player->position.x, player->position.y, player->frameWidth , player->frameHeight,
                goblin->position.x, goblin->position.y, goblin->frameWidth * goblin->scale, goblin->frameHeight * goblin->scale ))
        {
            player->position.x += (player->position.x < goblin->position.x) ? -push : push;
        }
    }
}


void DrawGoblin(Goblin *goblin) 
{
    Rectangle source = {
        goblin->currentFrame * goblin->frameWidth,
        0,
        goblin->frameWidth * goblin->direction,
        goblin->frameHeight
    };

    Rectangle dest = {
        goblin->position.x,
        goblin->position.y,
        goblin->frameWidth * goblin->frameFactor,
        goblin->frameHeight * goblin->frameFactor
    };

    Vector2 origin = {0, 0};

    float rotation = 0.0f;

    if (goblin->isDead)
    {
        DrawTexturePro(goblin->goblinSpriteDead, source, dest, origin, rotation, WHITE);
    }
    else if (goblin->goblinHasHit)
    {
       DrawTexturePro(goblin->goblinSpriteHurt, source, dest, origin, rotation, WHITE); 
    }
    else if (goblin->isIdle)
    {
        DrawTexturePro(goblin->goblinSpriteIdle, source, dest, origin, rotation, WHITE);
    } 
    else if (goblin->isWalking)
    {
        DrawTexturePro(goblin->goblinSpriteWalk, source, dest, origin, rotation, WHITE);
    } 
    else if (goblin->isAtacking)
    {
        DrawTexturePro(goblin->goblinSpriteAtk, source, dest, origin, rotation, WHITE);
    }

    if (!goblin->isDead)
    {
        DrawGoblinLifeBar(goblin);
    }
}

void UnloadGoblin(Goblin *goblin) 
{
    UnloadTexture(goblin->goblinSpriteWalk);
    UnloadTexture(goblin->goblinSpriteHurt);
    UnloadTexture(goblin->goblinSpriteDead);
    UnloadTexture(goblin->goblinSpriteIdle);
    UnloadTexture(goblin->goblinSpriteAtk);
}