#ifndef RUNNING_WOLF_H
#define RUNNING_WOLF_H

#include "raylib.h"
#include "entities/player/player.h"
#include "ui/lifeBar/lifeBarMob.h"
#include "math.h"
#include "core/define.h"
#include "render/drawMonsters.h"

// MAP
#define MAP_WOLF_RUNNING_AREA 2

// OFFSETS
#define RUNNING_WOLF_OFFSET_X_LIFE_BAR 80
#define RUNNING_WOLF_OFFSET_Y_LIFE_BAR -30

typedef struct player Player;

typedef struct runningWolf
{
    Entity entity;
    Monsters base;

    Vector2 start;
    Vector2 end;

    Texture2D spriteRunWolf;

    float speed;
    float speedRun;
    float push;

    int life;
    float maxLife;
    bool droppedHeart;

    int frameCounter;

    int frameIdle;
    int frameWalk;
    int frameRun;
    int frameAtk;
    int frameHurt;
    int frameDead;

    bool isRunning;

    float deathAnimTimer;
    bool deathAnimationDone;

    bool hasRunAttack;
    float attackRange;
    float attackCooldown;
    float attackDamageTimer;
    int damage;
    float viewPlayer;
    bool hasHitPlayer;

    Sound wolfHitSound;
    Sound wolfHitSoundHeavy;
    Sound wolfDeathSound;
    Sound wolfScratch;
    Sound wolfGrowl;

} RunningWolf;


void InitRunningWolf(RunningWolf *runningWolf);
void UpdateRunningWolf(RunningWolf *runningWolf, Player *player, float delta);
void DrawRunningWolf(RunningWolf *runningWolf);
void UnloadRunningWolf(RunningWolf *runningWolf);

#endif