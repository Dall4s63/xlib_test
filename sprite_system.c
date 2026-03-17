#include "sprite_system.h"

typedef struct _sprite {
    unsigned char *data;
    int data_width;
    int data_height;
    FColor *colors;
    int colors_len;
    int colors_buf_len;
} Sprite;

static Sprite *sprites = NULL;
static int     sprites_blen = 0;
static int     sprites_len = 0;
static int    *sprites_id_to_i = NULL;
static int     sprites_id_to_i_blen = 0;
static int     sprites_id_to_i_len = 0;
static int    *sprites_i_to_id = NULL;
static int     sprites_i_to_id_blen = 0;
static int     sprites_i_to_id_len = 0;

static void sprite_initialize() {
}

int sprite_new(char *filename) {
    if (sprites == NULL) {
        sprite_initialize();
    }
    // 1. check if there is already a sprite resource 
    // associated with this filename, and if there is 
    // just return the associated id and increment the 
    // ref-count.
    //
    // 2. store the string and create a new sprite 
    // resource 
}

void sprite_free(int id) {
}

int sprite_width(int id) {
}

int sprite_height(int id) {
}

FColor sprite_sample(int id, int x, int y) {
}

