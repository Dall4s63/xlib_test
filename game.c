#include <stdlib.h>
#include <stdio.h>

#include "tactics_render.h"
#include "keyboard.h"
#include "game.h"

typedef struct object {
    int x;
    int y;
    DrawableId sprite;
} Object;

static Object cursor;

void window_destroyed(void) {
    // here we can do the game cleanup
}

void key_press(KeyId id, unsigned int scancode, bool is_repeat) {
    switch (id) {
    case KB_W:
        cursor.y -= 24;
        break;
    case KB_A:
        cursor.x -= 16;
        break;
    case KB_D: 
        cursor.x += 16;
        break;
    case KB_S:
        cursor.y += 24;
        break;
    }
    SpriteInfo info;
    info.x = cursor.x;
    info.y = cursor.y;
    sprite_set(cursor.sprite, info, SPRITE_X | SPRITE_Y);
}

void key_release(KeyId id, unsigned int scancode) {
    switch (id) {
    case KB_D: 
        break;
    case KB_S:
        break;
    }
}

void game_load() {
    cursor.x = 0;
    cursor.y = 0;

    Image new_image;
    new_image.width = 16;
    new_image.height = 24;
    new_image.data = malloc(sizeof(char) * 4 * new_image.width * new_image.height);
    if (new_image.data == NULL) { printf("Helpppp\n"); }
    for (int i = 0; i < 4 * new_image.width * new_image.height; i += 4) {
        int row = i / 4 / new_image.width;
        int col = (i / 4) % new_image.width;
        if (row == 0 || row == new_image.height-1 || col == 0 || col == new_image.width-1) {
            new_image.data[i] = 0xff * row / new_image.height;
            new_image.data[i+1] = 0xff * col / new_image.width;
            new_image.data[i+2] = 0x80;
            new_image.data[i+3] = 0;
        } else {
            new_image.data[i] = 0;
            new_image.data[i+1] = 0;
            new_image.data[i+2] = 0;
            new_image.data[i+3] = 0;
        }
    }

    cursor.sprite = new_sprite(0, 0, new_image.width, new_image.height, 0, new_image.data);
}

void game_update(double dt) {
}
