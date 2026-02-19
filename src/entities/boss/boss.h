#ifndef BOSS_H
#define BOSS_H

#include "raylib.h"
#include "entities/player/player.h"
#include "ui/lifeBar/lifeBarMob.h"
#include "core/define.h"
#include "render/drawMonsters.h"

// Map
#define BOSS_MAP 8

// Speed
#define DESCENT_SPEED 250.0f;

// Frames
#define BOSS_FRAME_DEAD 4

#define BOSS_BASE_FRAME_ANIMATION 40
#define BOSS_HURT_FRAME_ANIMATION 30
#define BOSS_ATACK_FRAME_ANIMATION 10

// Cooldown
#define BOSS_ATACK_COOLDOWN 1.5f

// Life bar
#define BOSS_WIDTH_LIFE_BAR 800.0f
#define BOSS_HEIGHT_LIFE_BAR 18.0f

#define BOSS_NAME_WIDTH 625
#define BOSS_NAME_HEIGHT 2
#define BOSS_NAME_FONT_SIZE 20

#define BOSS_POSITION_X_LIFE_BAR (GetScreenWidth() / 2) - (800 / 2)
#define BOSS_POSITION_Y_LIFE_BAR 20

typedef struct boss
{
    Entity entity;
    Monsters base;

    int life;
    float maxLife;
    int damage;
    float speed;
    float push;

    float viewPlayer;
    float attackRange;

    int frameIdle;
    int frameWalk;
    int frameDead;
    int frameHurt;
    int frameAtk;
    int frameCounter;

    float attackTime;
    float attackCooldown;
    int hitFrame;
    bool hasAppliedDamage;

    float hurtTimer;
    float hurtDuration;

    Music bossMusic;
    bool musicStarted;

    Sound bossGrounImpact;
    Sound bossWalkSound;
    Sound bossHurtSound;
    Sound bossDeathSound;
    Sound bossAttackSound;

} Boss;

void InitBoss(Boss *boss);
void UpdateBoss(Boss *boss, Player *player, float delta, bool *bossTriggered);
void DrawBoss(Boss *boss);
void UnloadBoss(Boss *boss);

#endif
