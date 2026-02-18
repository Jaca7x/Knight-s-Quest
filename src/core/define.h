/* ================================================== */
/* ===================== BASE ===================== */
/* ================================================== */

#define SPRITE_ROW_BASE                   0 // Base row in sprite sheet
#define OFFSET_ZERO                       0 // Offset 0     
#define LIFE_ZERO                         0 // Life 0
#define SPEED_ZERO                        0 // Speed 0
#define POSITION_ZERO 0
#define COOLDOWN 1.0f
#define ORIGIN_TOPLEFT (Vector2){ 0.0f, 0.0f }
#define ROTATION 0.0f
#define SPACING 1.0f 
#define STAMINA_ZERO 0.0f
#define WIDTH_LIFE_BAR 60.0f
#define HEIGHT_LIFE_BAR 8.0f
#define VOLUME_MAX 1.0f

/* ===================== COMBAT / COOLDOWN ===================== */


#define COOLDOWN_ATTACK                  1.0f // Attack cooldown duration
#define COOLDOWN_ZERO                    0.0f // Zero value for cooldown checks
#define TIMER_ZERO 0.0f
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
#define PLAYER_HITBOX_OFFSET_X   60 //Offset hitbox player
#define PLAYER_HITBOX_OFFSET_Y   40 //Offset hitbox player

#define PLAYER_HITBOX_WIDTH_DIV  1.5f

// FONT
#define CODEPOINT_BASE   0
#define CODEPOINT_COUNT  250.0f

#define WOLFS_FRAME_DEAD 1
#define WOLFS_MAX_FRAMES_DEAD 2
#define WOLFS_ATTACK_COOLDOWN_AND_DAMAGE 0.4f

#define MAX_DISTANCE_FOR_INTERACTION_NPCS 10.0f
#define MIN_DISTANCE_FOR_INTERACTION_NPCS -150.0f

#define DIALOGUE_ZERO 0
#define DIALOGUE_ONE 1
#define DIALOGUE_TWO 2
#define DIALOGUE_THREE 3

#define TOTAL_DIALOGUES_PER_MAP 6
#define DIALOGS_PER_MAP 3
#define NPCS_FRAME_SPEED (200 / 5)