#include <c64/joystick.h>
#include <c64/sprites.h>
#include <c64/vic.h>
#include <string.h>
#include "sprite.h"
#include "sprite_data.h"
#define FRAME_DELAY 16 		// Animation frame delay
		       
char * const Screen = (char *)0x0400;

/* all 8 sprites will need 8x64 = 512 bytes ($0200) so place 
   them in upper memory and away from potential character sets */
char * const Sprite0 = (char *)0x3600;        //5hin-3y
char * const Sprite1 = (char *)(0x3600 + 64); //Ru5-t13

const char * robo_frames_right[] = { sprite_robo5, sprite_robo6, sprite_robo7,  sprite_robo6 };
const char * robo_frames_left[]  = { sprite_robo8, sprite_robo9, sprite_robo10, sprite_robo9 };
const char * robo_frames_bored[] = { sprite_robo_still_eyesleft, sprite_robo_still_eyesright,
                                     sprite_robo_still_eyesleft, sprite_robo_still_eyesright,
                                     sprite_robo_still_foottap,  sprite_robo_still_eyesleft, sprite_robo_still_foottap,  sprite_robo_still_eyesleft,
                                     sprite_robo_still_naillook, sprite_robo_still_naillook, sprite_robo_still_naillook, sprite_robo_still_eyesright };

// Global defs
sprite_object shiny;
sprite_object rusty;

direction get_robo_direction(int x) {
    return (x < 0) ? DIR_LEFT : (x > 0) ? DIR_RIGHT : DIR_IDLE;
}

// Animate and update on screen
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

// Read joysticks and alter x accordingly
void update_sprite_joystick_input(sprite_object* s, int joystick_id) {
    joy_poll(joystick_id);
    direction dir = get_robo_direction(joyx[joystick_id]);

    // There is a bug here switching from a high frame number being bored
    // To a movement, I need to put something here to reset the frame number when the
    // case changes

    switch (dir) {
        case DIR_LEFT:  s->frames = robo_frames_left;
                        s->num_frames =  4;
                        s->frame_delay = 4;            break;
        case DIR_RIGHT: s->frames = robo_frames_right;
                        s->num_frames =  4;
                        s->frame_delay = 4;            break;
        default:        s->frames = robo_frames_bored;
                        s->num_frames  = 12;
                        s->frame_delay = 12;           break;
    }

    s->x += joyx[joystick_id];
}
	
int init(void)
{
	vic.color_border = VCOL_BLACK;
	vic.color_back   = VCOL_BLUE;
	memset(Screen, ' ', 1000);	// clear screen

	spr_init(Screen);

	shiny = init_sprite(robo_frames_left, 4, Sprite0, 0, 0, 300, 229);
	rusty = init_sprite(robo_frames_right, 4, Sprite1, 1, 1, 20, 229);

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

	// This will be main game loop I think
	while (true) {
		update_sprite_joystick_input(&shiny, 0);
		update_sprite_joystick_input(&rusty, 1);

		update_sprite(&shiny);
		update_sprite(&rusty);

		vic_waitFrame();
		delay_counter++;
		if (delay_counter >= FRAME_DELAY) delay_counter = 0;
	}

	return 0;
}
