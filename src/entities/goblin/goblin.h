#ifndef GOBLIN_H
#define GOBLIN_H

#include "raylib.h"
#include "entities/player/player.h"
#include "ui/lifeBar/lifeBarMob.h"
#include "render/drawMonsters.h"
#include "core/define.h"


typedef struct player Player;

// MAPS
#define GOBLIN_MAP 1
#define RED_GOBLIN_MAP 0

//OFFSETS
#define GOBLIN_BAR_LIFE_OFFSET_X 0
#define GOBLIN_BAR_LIFE_OFFSET_Y 10

//FRAMES
#define GOBLIN_FRAME_DEATH 7
#define GOBLIN_TOTAL_FRAME_DEATH 8

//SOUNDS
#define GOBLIN_FRAME_FOR_ATTACK_SOUND 1

typedef struct goblin
{
    Entity entity;
    Monsters base;

    int frameWalk;
    int frameHurt;
    int frameDead;
    int frameIdle;
    int frameAtk;
    int frameCounter;

    bool deathAnimationDone;
    bool goblinHasHitPlayer;
    bool droppedHeart;

    float deathAnimTimer;
    float hurtTimer;
    float hurtDuration;
    float attackTimer;
    float attackCooldown;

    float viewPlayer;
    float goblinAttackRangeRight;
    float goblinAttackRangeLeft;

    int life;
    float maxLife;

    float speed;
    float baseSpeed;
    float push;

    int damage;
    float scaleHitbox;

    Sound goblinDeathSound;
    Sound goblinCutSound;
    Sound RedGoblinHitSound;

} Goblin;

void InitGoblinBase(Goblin *goblin, Vector2 pos);

void InitRedGoblin(Goblin *goblin, Vector2 pos);

void UpdateGoblin(Goblin *goblin, Player *player, int currentMapIndex, float delta);
void DrawGoblin(Goblin *goblin);
void UnloadGoblin(Goblin *goblin);

#endif