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
const int NUM_FRAMES = 4;

char shiny = 0;
char rusty = 1;

typedef struct {
    const unsigned char * const *frames;  // Pointer to an array of sprite frame pointers
    int num_frames;          // Total number of frames
    int current_frame;       // Current frame index
    int delay_counter;       // Frame delay counter
    int frame_delay;         // Frames to wait before switching
    unsigned char *sprite_mem; // Pointer to sprite memory (e.g. Sprite0 or Sprite1)
} sprite_anim;

sprite_anim shiny_anim;

void update_sprite(sprite_anim *anim);

void wait(unsigned int ticks)
{
    for (volatile unsigned int i = 0; i < ticks * 1000; i++)
        ;
}

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
	vic.color_border = VCOL_BLUE;
	vic.color_back   = VCOL_BLUE;
	memset(Screen, ' ', 1000);	// clear screen

	spr_init(Screen);

	vic.spr_mcolor0 = VCOL_WHITE;
	vic.spr_mcolor1 = VCOL_BLACK;

	// Initiate our heroes
	spr_set(shiny, true, 30 + 20, 60 + 20, (unsigned)Sprite0/ 64, shiny, true, false, false);
	spr_set(rusty, true, 90 + 20, 60 + 20, (unsigned)Sprite1/ 64, rusty, true, false, false);

	spr_color(shiny, VCOL_LT_BLUE);
	spr_color(rusty, VCOL_BROWN);

	shiny_anim.frames = robo_frames_right;
	shiny_anim.num_frames = NUM_FRAMES;
	shiny_anim.current_frame = 0;
	shiny_anim.delay_counter = 0;
	shiny_anim.frame_delay = 4;
	shiny_anim.sprite_mem = Sprite0;

	return 0;
}

int main(void)
{
	init();

	int frame = 0;
	int delay_counter = 0;

	while (true) {
		if (delay_counter == 0) {
	//		memcpy(Sprite0, robo_frames_right[frame], 64);
    		memcpy(Sprite1, robo_frames_right[frame], 64);

//			spr_image(shiny, (unsigned)Sprite0/ 64);
			spr_image(rusty, (unsigned)Sprite1/ 64);
		
			update_sprite(&shiny_anim);

			frame++;
			if (frame >= NUM_FRAMES) frame = 0;
		}

		vic_waitFrame();
		delay_counter++;
		if (delay_counter >= FRAME_DELAY) delay_counter = 0;
	}

	return 0;
}
