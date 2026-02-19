#ifndef GOBLIN_ARCHER_H
#define GOBLIN_ARCHER_H

#include "raylib.h"
#include "entities/player/player.h"
#include "ui/lifeBar/lifeBarMob.h"
#include "render/drawMonsters.h"
#include "core/define.h"


// MAP

#define MAP_GOBLIN_ARCHER_AREA 1

// ARROW DEFINES

#define ARROW_DAMAGE_ZERO 0
#define ARROW_SCALE 0.2f
#define ARROW_ROTATION 0.0f
#define ARROW_OFFSET_Y 0.05f

// ARROW COOLDOWN

#define ARROW_COOLDOWN_ZERO 0.0f
#define ARROW_COOLDOWN 1.5f
#define GOBLIN_ARCHER_ATTACK_ANIM_TIMER 0.4f

// OFFSETS

#define GOBLIN_ARCHER_OFFSET_BAR_LIFE_X 10
#define GOBLIN_ARCHER_OFFSET_BAR_LIFE_Y 10

// FRAMES

#define GOBLIN_ARCHER_FRAME_DEAD 8

//SOUND

#define FRAME_FOR_LOADING_SOUND 5

typedef struct arrow
{
    Vector2 position;

    float speed;
    float scale;
    float rotation;
    float arrowYOffset;

    int direction;
    bool active;

} Arrow;

typedef struct goblinArcher
{
    Entity entity;
    Monsters base;

    float speed;
    float maxLife;
    int life;
    
    bool droppedHeart;

    int frameCounter;
    int frameIdle;
    int frameWalk;
    int frameAtk;
    int frameHurt;
    int frameDead;

    float attackAnimTimer;

    bool hasHitPlayer;

    float deathAnimTimer;
    bool deathAnimationDone;

    Texture2D arrowTexture;
    Arrow arrow;

    float arrowSpeed;
    float arrowCooldown;
    float attackCooldown;
    float attackDamageTimer;

    float goblinView;
    float attackRange;

    int arrowTolerance;
    int arrowDamage;

    bool arrowFired;

    Sound goblinArcherDeathSound;
    Sound goblinArcherLoadingSound;
    Sound arrowHitSound;

} GoblinArcher;

void InitGoblinArcher(GoblinArcher *goblinArcher);
void UpdateGoblinArcher(GoblinArcher *goblinArcher, Player *player, float delta);
void DrawGoblinArcher(GoblinArcher *goblinArcher);
void UnloadGoblinArcher(GoblinArcher *goblinArcher);

#endif