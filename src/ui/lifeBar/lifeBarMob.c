#include "lifeBarMob.h"

void DrawBar(Entity *entity, int offsetX, int offsetY, float width, float height)
{
    if (entity->isDead) return;
    
    float barWidth = width;
    float barHeight = height;
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
