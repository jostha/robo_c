#include "sprite.h"

// Make it easier to define new sprite objects
sprite_object init_sprite(const unsigned char * const *frames, int num_frames,
                          unsigned char *sprite_mem, int sprite_number,
                          int current_frame, int x, int y) {
    sprite_object s;
    s.frames = frames;
    s.num_frames = num_frames;
    s.current_frame = current_frame;
    s.delay_counter = 0;
    s.frame_delay = 4; // Optional: expose this as a parameter
    s.sprite_mem = sprite_mem;
    s.sprite_number = sprite_number;
    s.x = x;
    s.y = y;
    return s;
}
