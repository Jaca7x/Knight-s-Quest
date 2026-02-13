#ifndef GOBLINBOMB_H
#define GOBLINBOMB_H

#include "../librays/raylib.h"
#include "../player/player.h"
#include "../lifeBar/lifeBarMob.h"
#include "../src/render/drawMonsters.h"
#include "../src/define/define.h"

/* MAP / GENERAL DEFINES */

#define MAP_GOBLIN_BOMB    7

/* GOBLIN HITBOX / SCALE */

#define GOBLIN_BOMB_HITBOX_OFFSET_X   110
#define GOBLIN_BOMB_HITBOX_OFFSET_Y   130
#define GOBLIN_BOMB_HITBOX_SCALE      0.5f

#define GOBLIN_BOMB_SCALE_WIDTH       1.98f
#define GOBLIN_BOMB_SCALE_HEIGHT      1.87f

/* BOMB DEFINES */

#define BOMB_CENTER_OFFSET_X        5
#define BOMB_CENTER_OFFSET_Y        50
#define BOMB_CENTER_SCALE           1.2f
#define BOMB_EXPLOSION_DELAY        3.14f
#define FRAME_COUNTER_BOMB_ZERO     0
#define FRAME_CURRENT_BOMB_ZERO     0
#define FRAME_TO_SOUND_EXPLOSION    14
#define FRAME_TO_DAMAGE_EXPLOSION   14

/* OFFSETS */
#define OFFSET_BAR_LIFE_X_GOBLIN_BOMB        120
#define OFFSET_BAR_LIFE_Y_GOBLIN_BOMB       -90

/* FRAMES */
#define FRAME_DELAY_GOBLIN_BOMB             10
#define FRAME_TO_ATTACK_GOBLIN_BOMB          7

/* COOLDOWN / TIMER */
#define ATTACK_BOMB_TIMER_ZERO   0.0f

/* SOUND */
#define FRAME_TO_SOUND_BAG                   2
#define FRAME_TO_SOUND_ATTACK_GOBLIN_BOMB    6

typedef struct player Player;

/* BOMB STRUCT */

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

/* GOBLIN BOMB STRUCT */

typedef struct goblinBomb
{
    Entity entity;
    Monsters base;

    Texture2D spriteAttackBomb;
    Texture2D spriteBomb;

    float maxLife;
    int life;
    float speed;
    int damage;
    int damageBomb;

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
    bool isDead;
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

} GoblinBomb;

/* FUNCTIONS */

void InitGoblinBomb(GoblinBomb *goblinBomb);
void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player);
void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player);
void UnloadGoblinBomb(GoblinBomb *goblinBomb);

#endif // GOBLINBOMB_H
