#include <stdint.h>

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

struct qoi_header {
    char        magic[4];
    uint32_t    width;
    uint32_t    height;
    uint8_t     channels;
    uint8_t     colorspace;
};

int qoi_load(char *fname, Sprite *ret) {
    // TODO revisit this for the windows port
    FILE *file = fopen(fname, "rb");
    if (file == NULL) {
        printf("failed to open file\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);
    char *buf = malloc(len);
    size_t res = fread(buf, 1, len, file);
    if (res != len) {
        printf("didn't read the right number of bytes\n");
        return 1;
    }
    fclose(file);

    unsigned char prev_r = 0;
    unsigned char prev_g = 0;
    unsigned char prev_b = 0;
    unsigned char prev_a = 255;

    unsigned char prevs[64 * 4];
    memset(prevs, 0, sizeof(prevs));

    if (buf[0] != 'q' || buf[1] != 'o' || buf[2] != 'i' || buf[3] != 'f') {
        printf("wrong image format\n");
        return 0;
    }

    struct qoi_header *header = (struct qoi_header *) buf;
    uint32_t width = header->width;
    uint32_t height = header->height;
    uint8_t channels = header->channels;

    free(buf);
    return 0;
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

