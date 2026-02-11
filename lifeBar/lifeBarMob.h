#ifndef LIFE_BAR_MOB_H
#define LIFE_BAR_MOB_H

#include "../librays/raylib.h"

typedef struct entity
{
    Vector2 position;
    int life;
    bool isDead;
    float maxLife;
} Entity;

void DrawBar(Entity *entity, int offsetX, int offsetY);

#endif // LIFE_BAR_MOB_H