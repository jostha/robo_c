#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/vic.h>
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
} sprite_object;

sprite_object shiny;
sprite_object rusty;

void update_sprite(sprite_object *spr) {
    if (spr->delay_counter == 0) {
        memcpy(spr->sprite_mem, spr->frames[spr->current_frame], 64);
        spr->current_frame++;
        if (spr->current_frame >= spr->num_frames) spr->current_frame = 0;
    }

    spr->delay_counter++;
    if (spr->delay_counter >= spr->frame_delay) spr->delay_counter = 0;
    spr_move(spr->sprite_number, spr->x, spr->y);
}

int init(void)
{
	vic.color_border = VCOL_BLACK;
	vic.color_back   = VCOL_BLUE;
	memset(Screen, ' ', 1000);	// clear screen

	spr_init(Screen);

	shiny.frames = robo_frames_left;
	shiny.num_frames = 4;
	shiny.current_frame = 0;
	shiny.delay_counter = 0;
	shiny.frame_delay = 4;
	shiny.sprite_mem = Sprite0;
        shiny.sprite_number = 0;
        shiny.x = 300;
        shiny.y = 229;

	rusty.frames = robo_frames_right;
	rusty.num_frames = 4;
	rusty.current_frame = 1;
	rusty.delay_counter = 0;
	rusty.frame_delay = 4;
	rusty.sprite_mem = Sprite1;
        rusty.sprite_number = 1;
        rusty.x = 20;
        rusty.y = 229;

	// Initiate our heroes
	spr_set(0, true, shiny.x, 228, (unsigned)Sprite0/ 64, 0, true, false, false);
	spr_set(1, true, rusty.x, 228, (unsigned)Sprite1/ 64, 1, true, false, false);

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
	int sx = 0;

	while (true) {
		joy_poll(0);
		sx += joyx[0];
		shiny.x + sx;
		rusty.x++;

		update_sprite(&shiny);
		update_sprite(&rusty);

		vic_waitFrame();
		delay_counter++;
		if (delay_counter >= FRAME_DELAY) delay_counter = 0;
	}

	return 0;
}
