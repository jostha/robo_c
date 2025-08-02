#ifndef SPRITE_H
#define SPRITE_H

// Create a struct for direction focused tasks
typedef enum { DIR_LEFT, DIR_RIGHT, DIR_IDLE } direction;

// Add an element of objects to the sprites to make management easier
typedef struct {
    const unsigned char * const *frames;
    int num_frames;
    int current_frame;
    int delay_counter;
    int frame_delay;
    unsigned char *sprite_mem;
    char sprite_number;
    int x;
    int y;
} sprite_object;

sprite_object init_sprite(const unsigned char * const *frames, int num_frames,
                          unsigned char *sprite_mem, int sprite_number,
                          int current_frame, int x, int y);

#endif // SPRITE_H
