#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sprite_system.h"
#include "hash_str_int.h"

#define INIT_LEN 60

static float min(float a, float b) {
    return (a < b) ? a : b;
}

static float max(float a, float b) {
    return (a > b) ? a : b;
}

FColor fcolor_from(unsigned char c[4]) {
    FColor new;

    new.r = (float)c[0] / 255.0;
    new.g = (float)c[1] / 255.0;
    new.b = (float)c[2] / 255.0;
    new.a = (float)c[3] / 255.0;

    return new;
}

/*
 * NOTE i want the color operations to assume 
 * premultiplied alpha
 */
FColor fcolor_mul(FColor a, FColor b) {
    FColor new;

    new.r = a.r * b.r;
    new.g = a.g * b.g;
    new.b = a.b * b.b;
    new.a = b.a;

    return new;
}

FColor fcolor_add(FColor a, FColor b) {
    FColor new;

    new.r = min(a.r + b.r, 1.0);
    new.g = min(a.g + b.g, 1.0);
    new.b = min(a.b + b.b, 1.0);
    new.a = min(a.a + b.a, 1.0);

    return new;
}

FColor fcolor_over(FColor a, FColor b) {
    FColor new;

    new.r = min(a.r + b.r * (1.0 - a.a), 1.0);
    new.g = min(a.g + b.g * (1.0 - a.a), 1.0);
    new.b = min(a.b + b.b * (1.0 - a.a), 1.0);
    new.a = min(a.a + b.a * (1.0 - a.a), 1.0);

    return new;
}

typedef struct _sprite {
    FColor *data;
    int width;
    int height;
} Sprite;

// typedef struct _sprite {
//     unsigned char *data;
//     int data_width;
//     int data_height;
//     FColor *colors;
//     int colors_len;
//     int colors_buf_len;
// } Sprite;

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

static STOIHashMap name_map;

static void sprite_initialize() {
    printf("running initialize\n");
    name_map = stoi_hash_new();
    sprites = malloc(sizeof(Sprite) * INIT_LEN);
    sprites_blen = INIT_LEN;
    sprites_id_to_i = malloc(sizeof(int) * INIT_LEN);
    memset(sprites_id_to_i, 0, sizeof(int) * INIT_LEN);
    sprites_id_to_i_blen = INIT_LEN;
    sprites_i_to_id = malloc(sizeof(int) * INIT_LEN);
    memset(sprites_i_to_id, 0, sizeof(int) * INIT_LEN);
    sprites_i_to_id_blen = INIT_LEN;
    sprites_id_to_ref = malloc(sizeof(int) * INIT_LEN);
    memset(sprites_id_to_ref, 0, sizeof(int) * INIT_LEN);
    sprites_id_to_ref_blen = INIT_LEN;
}

static int qoi_load(char *fname, Sprite *ret);

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
    sprites_len += 1;
    // TODO expand the buffers
    // load the image
    res = qoi_load(filename, &(sprites[index]));
    if (index >= sprites_i_to_id_len) {
        sprites_i_to_id[index] = index;
        sprites_id_to_i[index] = index;
        sprites_i_to_id_len += 1;
        sprites_id_to_i_len += 1;
    }
    id = sprites_i_to_id[index];
    sprites_id_to_ref[id] += 1;
    stoi_hash_put(name_map, filename, id);
    return id;
}

void sprite_free(int id) {
}

int sprite_width(int id) {
    int i = sprites_i_to_id[id];
    return sprites[i].width;
}

int sprite_height(int id) {
    int i = sprites_i_to_id[id];
    return sprites[i].height;
}

// x and y are between 0 and 1
FColor sprite_fsample(int id, float x, float y) {
    if (x < 0.0) { x = 0.0; }
    if (x > 1.0) { x = 1.0; }
    if (y < 0.0) { y = 0.0; }
    if (y > 1.0) { y = 1.0; }
    int i = sprites_i_to_id[id];
    int iwidth = sprites[i].width;
    float width = (float)iwidth;
    float height = (float)sprites[i].height;
    int col = (int)floor(width * x);
    int row = (int)floor(height * y);
    return sprites[i].data[row * iwidth + col];
}

FColor sprite_sample(int id, int x, int y) {
    int i = sprites_i_to_id[id];
    int width = sprites[i].width;
    return sprites[i].data[y * width + x];
}

struct qoi_header {
    char        magic[4];
    uint32_t    width;
    uint32_t    height;
    uint8_t     channels;
    uint8_t     colorspace;
};

static void qoi_insert(uint8_t *arr, uint8_t c[4]) {
    unsigned int i = ((unsigned int)c[0] * 3 + (unsigned int)c[1] * 5 + (unsigned int)c[2] * 7 + (unsigned int)c[3] * 11) % 64;
    arr[i*4 + 0] = c[0];
    arr[i*4 + 1] = c[1];
    arr[i*4 + 2] = c[2];
    arr[i*4 + 3] = c[3];
}

static uint32_t betole(uint32_t v) {
    uint32_t a = v&0x0000000ff;
    uint32_t b = (v&0x0000ff00) >> 8;
    uint32_t c = (v&0x00ff0000) >> 16;
    uint32_t d = (v&0xff000000) >> 24;
    return (a << 24) | (b << 16) | (c << 8) | (d);
}

static void set_color(uint8_t c[4], FColor *dst) {
    *dst = (FColor) {
        .r = (float)c[0] / 255.0,
        .g = (float)c[1] / 255.0,
        .b = (float)c[2] / 255.0,
        .a = (float)c[3] / 255.0,
    };
}

static int qoi_load(char *fname, Sprite *ret) {
    // TODO revisit this for the windows port
    FILE *file = fopen(fname, "rb");
    if (file == NULL) {
        printf("failed to open file\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);
    uint8_t *buf = malloc(len);
    size_t res = fread(buf, 1, len, file);
    if (res != len) {
        printf("didn't read the right number of bytes\n");
        return 1;
    }
    fclose(file);

    uint8_t prev[4];
    prev[0] = 0;
    prev[1] = 0;
    prev[2] = 0;
    prev[3] = 255;

    uint8_t prevs[64 * 4];
    memset(prevs, 0, sizeof(prevs));

    if (buf[0] != 'q' || buf[1] != 'o' || buf[2] != 'i' || buf[3] != 'f') {
        printf("wrong image format\n");
        return 0;
    }

    struct qoi_header *header = (struct qoi_header *) buf;
    // TODO FIX BIG ENDIAN TO LITTLE ENDIAN PROBLEM
    uint32_t width = betole(header->width);
    uint32_t height = betole(header->height);
    // TODO work out how to use channels and color space
    // uint8_t channels = header->channels;
    // uint8_t colorspace = header->colorspace;
    // printf("colorspace %u\n", colorspace);

    ret->width = width;
    ret->height = height;
    ret->data = malloc(sizeof(FColor) * width * height);
    if (ret->data == NULL) {
        printf("failed to allocate\n");
    }
    int ret_i = 0;

    unsigned char *cur = buf + 14;

    while (!(cur[0] == 0 
             && cur[1] == 0
             && cur[2] == 0
             && cur[3] == 0
             && cur[4] == 0
             && cur[5] == 0
             && cur[6] == 0
             && cur[7] == 1
           )) {

        if (*cur == 0xfe) {
            // OP RGB
            // printf("OP RGB\n");
            prev[0] = *(cur + 1);
            prev[1] = *(cur + 2);
            prev[2] = *(cur + 3);
            qoi_insert(prevs, prev);
            set_color(prev, ret->data + ret_i++);
            cur += 4;
            continue;

        } else if (*cur == 0xff) {
            // OP RGBA
            // printf("OP RGBA\n");
            prev[0] = *(cur + 1);
            prev[1] = *(cur + 2);
            prev[2] = *(cur + 3);
            prev[3] = *(cur + 4);
            qoi_insert(prevs, prev);
            // printf("pixel: %x%x%x%x\n", prev[0], prev[1], prev[2], prev[3]);
            set_color(prev, ret->data + ret_i++);
            cur += 5;
            continue;
        }

        switch (*cur & 0xc0) {
        case 0x00:
            // OP INDEX
            // printf("OP INDEX\n");
            {
            unsigned int i = (*cur)&0x3f;
            prev[0] = prevs[i * 4 + 0];
            prev[1] = prevs[i * 4 + 1];
            prev[2] = prevs[i * 4 + 2];
            prev[3] = prevs[i * 4 + 3];
            // printf("pixel: %x%x%x%x\n", prev[0], prev[1], prev[2], prev[3]);
            set_color(prev, ret->data + ret_i++);
            }
            cur += 1;
            break;

        case 0x40:
            // OP DIFF
            // printf("OP DIFF\n");
            {
            int dr = ((*cur & 0b00110000) >> 4) - 2;
            int dg = ((*cur & 0b00001100) >> 2) - 2;
            int db = (*cur & 0b00000011) - 2;
            // printf("dr: %d, dg: %d, db: %d\n", dr, dg, db);
            int new_r = (int)prev[0] + dr;
            if (new_r < 0) { new_r += 256; }
            if (new_r > 255) { new_r -= 256; }
            int new_g = (int)prev[1] + dg;
            if (new_g < 0) { new_g += 256; }
            if (new_g > 255) { new_g -= 256; }
            int new_b = (int)prev[2] + db;
            if (new_b < 0) { new_b += 256; }
            if (new_b > 255) { new_b -= 256; }
            prev[0] = (uint8_t)new_r;
            prev[1] = (uint8_t)new_g;
            prev[2] = (uint8_t)new_b;
            set_color(prev, ret->data + ret_i++);
            }
            cur += 1;
            break;

        case 0x80:
            // OP LUMA
            // printf("OP LUMA\n");
            {
            int dg = (*cur & 0x3f) - 32;
            int dr_dg = ((*(cur+1) & 0xf0) >> 4) - 8;
            int db_dg = (*(cur+1) & 0x0f) - 8;
            int dr = dr_dg + dg;
            int db = db_dg + dg;
            printf("dr: %d, dg: %d, db: %d\n", dr, dg, db);
            int new_r = (int)prev[0] + dr;
            if (new_r < 0) { new_r += 256; }
            if (new_r > 255) { new_r -= 256; }
            int new_g = (int)prev[1] + dg;
            if (new_g < 0) { new_g += 256; }
            if (new_g > 255) { new_g -= 256; }
            int new_b = (int)prev[2] + db;
            if (new_b < 0) { new_b += 256; }
            if (new_b > 255) { new_b -= 256; }
            prev[0] = (uint8_t)new_r;
            prev[1] = (uint8_t)new_g;
            prev[2] = (uint8_t)new_b;
            set_color(prev, ret->data + ret_i++);
            }
            cur += 2;
            break;

        case 0xc0:
            // printf("OP RUN\n");
            {
            unsigned int len = ((*cur)&0x3f) + 1;
            for (unsigned int i = 0; i < len; ++i) {
                // printf("pixel: %x%x%x%x\n", prev[0], prev[1], prev[2], prev[3]);
                set_color(prev, ret->data + ret_i++);
            }
            }
            cur += 1;
            break;

        default:
            // this should never occur
            printf("Found problematic byte %x\n", *cur);
        }
    }

    free(buf);
    return 0;
}

#undef INIT_LEN

