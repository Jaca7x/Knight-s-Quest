#include "goblinTank.h"

void InitGoblinTank(GoblinTank *goblinTank)
{
    goblinTank->position = (Vector2){0, 0};

    goblinTank->goblinTankSpriteWalk = LoadTexture("resources/sprites/goblin/goblinTank-walk.png");
    goblinTank->goblinTankSpriteHurt = LoadTexture("resources/sprites/goblin/goblinTank-hurt.png");
    goblinTank->goblinTankSpriteDead = LoadTexture("resources/sprites/goblin/goblinTank-dead.png");
    goblinTank->goblinTankSpriteIdle = LoadTexture("resources/sprites/goblin/goblinTank-idle.png");
    goblinTank->goblinTankSpriteAtk = LoadTexture("resources/sprites/goblin/goblinTank-attack.png");
    goblinTank->frameWalk = 6;
    goblinTank->frameHurt = 4;
    goblinTank->frameDead = 6;
    goblinTank->frameIdle = 4;
    goblinTank->frameAtk = 6;

    goblinTank->currentFrame = 0;
    goblinTank->frameCounter = 0;
    goblinTank->frameWidth = goblinTank->goblinTankSpriteWalk.width / goblinTank->frameWalk;
    goblinTank->frameHeight = goblinTank->goblinTankSpriteWalk.height;
    goblinTank->frameFactor = 0.1f;

    goblinTank->isIdle = true;
    goblinTank->isWalking = false;
    goblinTank->isAtacking = false;
    goblinTank->isDead = false;
    goblinTank->goblinTankHasHit = false;
    goblinTank->deathAnimationDone = false;
}