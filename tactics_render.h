#ifndef TACTICS_RENDER_H
#define TACTICS_RENDER_H

#include "display.h"

typedef int DrawableId;

int render_setup(int v_width, int v_height);
int render_run(Image canvas);

DrawableId new_sprite(int x, int y, int width, int height, int depth, char *data);

#define SPRITE_X        0x01
#define SPRITE_Y        0x02
#define SPRITE_DEPTH    0x04
#define SPRITE_WIDTH    0x08
#define SPRITE_HEIGHT   0x10
#define SPRITE_DATA     0x20

typedef struct _sprite_info {
    int x;
    int y;
    int depth;
    int width;
    int height;
    char *data;
} SpriteInfo;
int sprite_set(DrawableId id, SpriteInfo vals, int mask);
int sprite_get(DrawableId id, SpriteInfo *ret);

#endif
