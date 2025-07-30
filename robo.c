#include <c64/vic.h>
#include <c64/sprites.h>
#include <string.h>
#include "sprite_data.h"
#define FRAME_DELAY 16 		// Animation frame delay
		       
char * const Screen = (char *)0x0400;

/* all 8 sprites will need 8x64 = 512 bytes ($0200) so place 
   them in upper memory and away from potential character sets */
char * const Sprite0 = (char *)0x3600;        //5hin-3y
char * const Sprite1 = (char *)(0x3600 + 64); //Ru5-t13

const char * robo_frames_right[] = { sprite_robo5, sprite_robo6, sprite_robo7, sprite_robo6 };
const char * robo_frames_left[] = { sprite_robo8, sprite_robo9, sprite_robo10, sprite_robo9 };

typedef struct {
    const unsigned char * const *frames;  // Pointer to an array of sprite frame pointers
    int num_frames;                       // Total number of frames
    int current_frame;                    // Current frame index
    int delay_counter;                    // Frame delay counter
    int frame_delay;                      // Frames to wait before switching
    unsigned char *sprite_mem;            // Pointer to sprite memory (e.g. Sprite0 or Sprite1)
    char sprite_number;                   // Sprite hardware index (0-7)
    int x;                                // X pos
    int y;                                // Y pos
} sprite_anim;

sprite_anim shiny_anim;
sprite_anim rusty_anim;

void update_sprite(sprite_anim *anim) {
    if (anim->delay_counter == 0) {
        memcpy(anim->sprite_mem, anim->frames[anim->current_frame], 64);
        anim->current_frame++;
        if (anim->current_frame >= anim->num_frames) anim->current_frame = 0;
    }

    anim->delay_counter++;
    if (anim->delay_counter >= anim->frame_delay) anim->delay_counter = 0;
}

int init(void)
{
	vic.color_border = VCOL_BLACK;
	vic.color_back   = VCOL_BLUE;
	memset(Screen, ' ', 1000);	// clear screen

	spr_init(Screen);

	// Initiate our heroes
//	spr_set(0, true, 300, 228, (unsigned)Sprite0/ 64, 0, true, false, false);
//	spr_set(1, true, 20, 228, (unsigned)Sprite1/ 64, 1, true, false, false);

	shiny_anim.frames = robo_frames_left;
	shiny_anim.num_frames = 4;
	shiny_anim.current_frame = 0;
	shiny_anim.delay_counter = 0;
	shiny_anim.frame_delay = 4;
	shiny_anim.sprite_mem = Sprite0;
        shiny_anim.sprite_number = 0;
        shiny_anim.x = 300;
        shiny_anim.y = 229;

	rusty_anim.frames = robo_frames_right;
	rusty_anim.num_frames = 4;
	rusty_anim.current_frame = 1;
	rusty_anim.delay_counter = 0;
	rusty_anim.frame_delay = 4;
	rusty_anim.sprite_mem = Sprite1;
        rusty_anim.sprite_number = 1;
        rusty_anim.x = 20;
        rusty_anim.y = 229;

	// Initiate our heroes
	spr_set(0, true, shiny_anim.x, 228, (unsigned)Sprite0/ 64, 0, true, false, false);
	spr_set(1, true, rusty_anim.x, 228, (unsigned)Sprite1/ 64, 1, true, false, false);

	vic.spr_mcolor0 = VCOL_WHITE;
	vic.spr_mcolor1 = VCOL_BLACK;
	spr_color(0, VCOL_LT_BLUE);
	spr_color(1, VCOL_BROWN);

	return 0;
}

int main(void)
{
	init();

	int delay_counter = 0;

	while (true) {
		update_sprite(&shiny_anim);
		update_sprite(&rusty_anim);

		vic_waitFrame();
		delay_counter++;
		if (delay_counter >= FRAME_DELAY) delay_counter = 0;
		
		shiny_anim.x--;
		rusty_anim.x++;
		spr_move(0, shiny_anim.x, shiny_anim.y);
		spr_move(1, rusty_anim.x, rusty_anim.y);
	}

	return 0;
}
