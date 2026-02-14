#ifndef BOMBGOBLIN_H
#define BOMBGOBLIN_H

#include "raylib.h"
#include "entities/player/player.h"
#include "ui/lifeBar/lifeBarMob.h"
#include "render/drawMonsters.h"
#include "core/define.h"


// MAP

#define MAP_BOMB_GOBLIN 7

// HITBOX && SCALE

#define BOMB_GOBLIN_HITBOX_OFFSET_X 110
#define BOMB_GOBLIN_HITBOX_OFFSET_Y 130
#define BOMB_GOBLIN_HITBOX_SCALE 0.5f

#define BOMB_GOBLIN_SCALE_WIDTH 1.98f
#define BOMB_GOBLIN_SCALE_HEIGHT 1.87f

// BOMB DEFINES

#define BOMB_CENTER_OFFSET_X 5
#define BOMB_CENTER_OFFSET_Y 50
#define BOMB_CENTER_SCALE 1.2f
#define BOMB_EXPLOSION_DELAY 3.14f
#define FRAME_COUNTER_BOMB_ZERO 0
#define FRAME_CURRENT_BOMB_ZERO 0
#define FRAME_TO_SOUND_EXPLOSION 14
#define FRAME_TO_DAMAGE_EXPLOSION 14

// COOLDOWN / TIMER
#define ATTACK_BOMB_TIMER_ZERO 0.0f
#define ATTACK_BOMB_TIMER 1.5f

// OFFSETS 
#define BOMB_GOBLIN_OFFSET_X_LIFE_BAR 120
#define BOMB_GOBLIN_OFFSET_Y_LIFE_BAR -90

// FRAMES 
#define BOMB_GOBLIN_FRAME_DELAY 10
#define FRAME_TO_ATTACK_BOMB_GOBLIN 7

// SOUND
#define FRAME_TO_SOUND_BAG 2
#define FRAME_TO_SOUND_ATTACK_BOMB_GOBLIN 6

typedef struct player Player;

typedef struct bomb
{
    Vector2 pos;
    float speed;

    bool isActive;
    bool playerIsDamage;

    int frameBomb;
    int frameWidthBomb;
    int frameHeightBomb;

    int currentFrameBomb;
    int frameCounterBomb;

    Sound explosion;
    Sound timer;

} Bomb;

typedef struct bombGoblin
{
    Entity entity;
    Monsters base;

    Texture2D spriteAttackBomb;
    Texture2D spriteBomb;

    float maxLife;
    int life;
    int damage;
    int damageBomb;
    float speed;

    int frameIdle;
    int frameAttack;
    int frameAttackBomb;
    int frameHurt;
    int frameDeath;
    int frameRun;

    int frameWidthAttackBomb;
    int frameHeightAttackBomb;

    int frameCounter;

    bool isAttackBomb;
    bool bombExplode;
    bool hasThrownBomb;
    bool animAttackBombEnd;
    bool wasWalking;
    bool attackSoundPlayed;
    bool soundBagPlayed;
    bool droppedHeart;

    Bomb bomb;

    float timerForExplosion;
    float radiusToDamage;
    float bombRange;
    float atackRange;
    float atackRangeRight;
    int push;

    float timerAttackBomb;
    float animAttackTimer;
    float hurtDuration;
    float hurtTimer;
    float viewPlayer;

    Sound soundAttack;
    Sound soundBagGoblin;

} BombGoblin;

void InitBombGoblin(BombGoblin *bombgoblin);
void UpdateBombGoblin(BombGoblin *bombgoblin, float delta, Player *player);
void DrawBombGoblin(BombGoblin *bombgoblin, Player *player);
void UnloadBombGoblin(BombGoblin *bombgoblin);

#endif
