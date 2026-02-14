#include "lifeBarMob.h"

void DrawBar(Entity *entity, int offsetX, int offsetY)
{
    if (entity->isDead) return;

    float barWidth = 60.0f;
    float barHeight = 8.0f;
    float x = entity->position.x + offsetX;
    float y = entity->position.y - offsetY;

    float lifePercent = entity->life / entity->maxLife;
    if (lifePercent < 0) lifePercent = 0;
    if (lifePercent > 1) lifePercent = 1;

    float currentBarWidth = barWidth * lifePercent;

    DrawRectangle(x, y, barWidth, barHeight, RED);        
    DrawRectangle(x, y, currentBarWidth, barHeight, GREEN); 
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);   
}
