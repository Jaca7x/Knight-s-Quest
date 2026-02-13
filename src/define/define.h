/* ================================================== */
/* ===================== BASE ===================== */
/* ================================================== */

#define SPRITE_ROW_BASE                   0 // Base row in sprite sheet
#define OFFSET_ZERO                       0 // Offset 0     
#define LIFE_ZERO                         0 // Life 0
#define SPEED_ZERO                        0 // Speed 0
#define POSITION_ZERO 0

/* ===================== COMBAT / COOLDOWN ===================== */

#define COOL_DOWN_ATTACK                  1.0f // Attack cooldown duration
#define COOL_DOWN_ZERO                    0.0f // Zero value for cooldown checks
#define LIFE_ZERO                         0    // Zero life threshol
#define HURT_TIMER_ZERO                   0.0f // Reset hurt timer
#define ANIM_ATTACK_ZERO                  0.0f // Reset atack timer
#define ANIM_DEAD_ZERO                    0.0f // Reset dead timer
#define ANIM_ATTACK                       1.0f // Anim timer
#define DEATH_ANIM_FRAME_TIME            0.2f  // Time per death animation frame

/* ===================== FPS ===================== */

#define GAME_FPS                          60 // The game runs at the (theoretical) FPS rate.
#define ANIMATION_FPS                     10 // Animation frames per second

/* ===================== FRAME / ANIMATION ===================== */
#define NEXT_FRAME                        1 // Frame increment step
#define PREVIOUS_FRAME                    1 // Used to clamp last frame

#define CURRENT_FRAME_ZERO                0 // Initial frame value
#define FRAME_COUNTER_ZERO                0 // Initial frame counter value 

/* ===================== DIRECTION ===================== */

#define DIRECTION_LEFT                   -1 // Facing left
#define DIRECTION_RIGHT                   1 // Facing right

/* ================================================== */
/* ===================== PLAYER ===================== */
/* ================================================== */

/* ===================== TIMERS ===================== */
#define HIT_TIMER                         0.4f //Hit timer player

/* ===================== OFFSET HITBOX ===================== */
#define PLAYER_HITBOX_OFFSET_X   50 //Offset hitbox player
#define PLAYER_HITBOX_OFFSET_Y   35 //Offset hitbox player

#define PLAYER_HITBOX_WIDTH_DIV  1.5f
