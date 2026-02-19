#ifndef WOLF_H
#define WOLF_H

#include "raylib.h"
#include "entities/player/player.h"
#include "ui/lifeBar/lifeBarMob.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include "core/define.h"
#include "render/drawMonsters.h"

// MAPS
#define MAP_WOLF_WHITE_AREA 4

// STATS
#define WOLF_BASE_SPEED 95.0f

// TIMERS & COOLDOWN
#define TIME_SINCE_LAST_GROWL 15.0f

// OFFSETS
#define WOLF_OFFSET_X_LIFE_BAR 20
#define WOLF_OFFSET_Y_LIFE_BAR -30

typedef struct player Player;

typedef struct wolf
{
    Entity entity;
    Monsters base;

    Vector2 start;
    Vector2 end;

    float speed;
    float push;

    int life;
    int maxLife;
    bool droppedHeart;

    int frameCounter;

    int frameIdle;
    int frameWalk;
    int frameAtk;
    int frameHurt;
    int frameDead;

    bool isMoving;
    bool isPatrolling;

    float deathAnimTimer;
    bool deathAnimationDone;

    float attackRange;
    float attackCooldown;
    float attackDamageTimer;
    int damage;
    bool hasHitPlayer;

    Sound wolfHitSound;
    Sound wolfHitSoundHeavy;
    Sound wolfDeathSound;
    Sound wolfScratch;

    Sound wolfGrowl;
    bool growlPlayed;
    float timeSinceLastGrowl;

} Wolf;

void InitWolfBase(Wolf *wolf, Vector2 pos);

void InitWhiteWolf(Wolf *wolf, Vector2 pos);

void InitRedWolf(Wolf *wolf, Vector2 pos);

void UpdateWolf(Wolf *wolf, Player *player, float delta, int currentMapIndex);
void DrawWolf(Wolf *wolf);
void UnloadWolf(Wolf *wolf);

#endif // WOLF_H