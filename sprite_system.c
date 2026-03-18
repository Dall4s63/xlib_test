#include "sprite_system.h"

#define INIT_LEN 60

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
static int    *sprites_id_to_ref = NULL;
static int     sprites_id_to_ref_blen = 0;
static int     sprites_id_to_ref_len = 0;

static STOIHashMap name_map;

static void sprite_initialize() {
    name_map = stoi_hash_new();
    sprites = malloc(sizeof(Sprite) * INIT_LEN);
    sprites_blen = INIT_LEN;
    sprites_id_to_i = malloc(sizeof(int) * INIT_LEN);
    sprites_id_to_i_blen = INIT_LEN;
    sprites_i_to_id = malloc(sizeof(int) * INIT_LEN);
    sprites_i_to_id_blen = INIT_LEN;
    sprites_id_to_ref = malloc(sizeof(int) * INIT_LEN);
    sprites_id_to_ref_blen = INIT_LEN;
}

int sprite_new(char *filename) {
    if (sprites == NULL) {
        sprite_initialize();
    }
    int id;
    bool res = stoi_hash_get(name_map, filename, &id);
    if (res) {
        sprites_id_to_ref[id] += 1;
        return id;
    }
    int index = sprites_len;
    // TODO expand the buffers
    // load the image
}

void sprite_free(int id) {
}

int sprite_width(int id) {
}

int sprite_height(int id) {
}

FColor sprite_sample(int id, int x, int y) {
}

#undef INIT_LEN

