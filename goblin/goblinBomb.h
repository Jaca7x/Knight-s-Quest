#ifndef GOBLINBOMB_H
#define GOBLINBOMB_H

#include "../librays/raylib.h"
#include "../player/player.h"

#define MAP_GOBLIN_BOMB 7

// Player hitbox offsets
#define PLAYER_HITBOX_OFFSET_X 50
#define PLAYER_HITBOX_OFFSET_Y 35
#define PLAYER_DIRECTION_OFFSET_X 60
#define PLAYER_SACLE_WIDTH 1.06f

// Goblin hitbox offsets
#define GOBLIN_HITBOX_OFFSET_X 110
#define GOBLIN_HITBOX_OFFSET_Y 130
#define GOBLIN_HITBOX_CHECK_OFFSET_Y 120
#define GOBLIN_HITBOX_SCALE    0.5f
#define GOBLIN_SCALE_WIDTH 1.98f
#define GOBLIN_SCALE_HEIGHT 1.87f

// Bomb explosion offsets
#define BOMB_CENTER_OFFSET_X 5
#define BOMB_CENTER_OFFSET_Y 50
#define BOMB_CENTER_SCALE    1.2f
#define BOMB_EXPLOSION_DELAY 3.14f

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

typedef struct goblinBomb
{
    Texture2D spriteIdle;
    Texture2D spriteAttackBomb;
    Texture2D spriteAttack;
    Texture2D spriteHurt;
    Texture2D spriteDeath;
    Texture2D spriteRun;

    Vector2 position;

    float maxLife;
    int life;
    float speed;
    int damage;

    int frameIdle;
    int frameAttackBomb;
    int frameAttack;
    int frameHurt;
    int frameDeath;
    int frameRun;
    float frameScale;

    int frameWidthIdle;
    int frameHeightIdle;

    int frameWidthAttackBomb;
    int frameHeightAttackBomb;

    int frameWidthAttack;
    int frameHeightAttack;

    int frameWidthHurt;
    int frameHeightHurt;

    int frameWidthDeath;
    int frameHeightDeath;

    int frameWidthRun;
    int frameHeightRun;

    int frameCounter;
    int currentFrame;

    bool isIdle;
    bool isAttackBomb;
    bool bombExplode;
    bool hasThrownBomb;
    bool isAttack;
    bool isWalking;

    bool droppedHeart;
    Texture2D spriteBomb;

    Bomb bomb;
    int damageBomb;
    float timerForExplosion;
    float radiusToDamage;
    float bombRange;
    float attackRange;

    float timerAttackBomb;
    bool animAttackBombEnd;

    int direction;

    float animAttackTimer;

    bool goblinHasHit;
    bool isDead;
    bool wasWalking;
    bool attackSoundPlayed;
    bool soundBagPlayed;

    float hurtDuration;
    float hurtTimer;

    float viewPlayer;
    
    Sound soundAttack;
    Sound soundBagGoblin;
} GoblinBomb;




void InitGoblinBomb(GoblinBomb *goblinBomb);
void UpdateGoblinBomb(GoblinBomb *goblinBomb, float delta, Player *player);
void DrawGoblinBomb(GoblinBomb *goblinBomb, Player *player);
void UnloadGoblinBomb(GoblinBomb *goblinBomb);

#endif // GOBLINBOMB_H