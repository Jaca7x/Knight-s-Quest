//Main defines 

//Title maps defines
#define TITLE_MAP_DURATION        2.0f
#define TITLE_MAP_FONT_SIZE       60.0f
#define TITLE_MAP_SPACING         1.0f

#define TITLE_MAP_OFFSET_Y        220

#define TITLE_MAP_PADDING_X       20
#define TITLE_MAP_PADDING_Y       10

#define TITLE_MAP_BG_ALPHA        160

// Button UI Config
#define BUTTON_BORDER_THICKNESS 1
#define BUTTON_TEXT_OFFSET_X    20
#define BUTTON_TEXT_OFFSET_Y    10
#define BUTTON_FONT_SIZE        20

// Monster Tutorial UI Config
#define TUTORIAL_PANEL_X              250
#define TUTORIAL_PANEL_Y              100
#define TUTORIAL_PANEL_WIDTH          1000
#define TUTORIAL_PANEL_HEIGHT         600

#define TUTORIAL_BORDER_THICKNESS     2

#define TUTORIAL_TITLE_FONT_SIZE      26
#define TUTORIAL_INFO_FONT_SIZE       18
#define TUTORIAL_CLOSE_FONT_SIZE      14

#define TUTORIAL_TITLE_OFFSET_Y       100
#define TUTORIAL_TITLE1_OFFSET_X      150
#define TUTORIAL_TITLE2_OFFSET_X      650

#define TUTORIAL_INFO_OFFSET_Y        380
#define TUTORIAL_INFO1_OFFSET_X       50
#define TUTORIAL_INFO2_OFFSET_X       580

#define TUTORIAL_CLOSE_OFFSET_X       20
#define TUTORIAL_CLOSE_OFFSET_Y       25

// Base defines
#define SPRITE_ROW_BASE 0 // Base row in sprite sheet
#define OFFSET_ZERO 0   

#define LIFE_ZERO 0
#define SPEED_ZERO 0 
#define STAMINA_ZERO 0.0f

#define POSITION_ZERO 0
#define ORIGIN_TOPLEFT (Vector2){ 0.0f, 0.0f }
#define ROTATION 0.0f

#define COOLDOWN 1.0f
#define COOLDOWN_ZERO 0.0f 
#define TIMER_ZERO 0.0f

#define SPACING 1.0f 

#define WIDTH_LIFE_BAR 60.0f
#define HEIGHT_LIFE_BAR 8.0f

#define VOLUME_MAX 1.0f

//Combat defines

#define COOLDOWN_ATTACK 1.0f 
#define ANIM_ATTACK 1.0f 

#define HURT_TIMER_ZERO 0.0f 
#define ANIM_ATTACK_ZERO 0.0f 

#define ANIM_DEAD_ZERO 0.0f 
#define DEATH_ANIM_FRAME_TIME 0.2f  

// FPS
#define GAME_FPS 60 // The game runs at the (theoretical) FPS rate.
#define ANIMATION_FPS  10 // Animation frames per second

//Freames & Animations
#define NEXT_FRAME 1 
#define PREVIOUS_FRAME 1 

#define CURRENT_FRAME_ZERO 0
#define FRAME_COUNTER_ZERO 0 

//Direction
#define DIRECTION_LEFT -1 // Facing left
#define DIRECTION_RIGHT 1 // Facing right

//Player defines

#define HIT_TIMER 0.4f //Hit timer player

//Offsets
#define PLAYER_HITBOX_OFFSET_X 60 //Offset hitbox player
#define PLAYER_HITBOX_OFFSET_Y 40 //Offset hitbox player

#define PLAYER_HITBOX_WIDTH_DIV 1.5f

// FONT
#define CODEPOINT_BASE  0
#define CODEPOINT_COUNT 250.0f

//Wolfs defines

//Frames
#define WOLFS_FRAME_DEAD 1
#define WOLFS_MAX_FRAMES_DEAD 2

//Atack
#define WOLFS_ATTACK_COOLDOWN_AND_DAMAGE 0.4f

//Npc defines

//Distance
#define MAX_DISTANCE_FOR_INTERACTION_NPCS 10.0f
#define MIN_DISTANCE_FOR_INTERACTION_NPCS -150.0f

//Dilogues index
#define DIALOGUE_ZERO 0
#define DIALOGUE_ONE 1
#define DIALOGUE_TWO 2
#define DIALOGUE_THREE 3

#define DIALOGS_PER_MAP 3

//Speed animation
#define NPCS_FRAME_SPEED (200 / 5)

//Maps 
#define GHOST_NUM_MAPS 4
#define PEASANT_NUM_MAPS 3