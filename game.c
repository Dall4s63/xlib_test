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

static Object obj;
static bool d_pressed = false;
static bool s_pressed = false;

void window_destroyed(void) {
    // here we can do the game cleanup
}

void key_press(KeyId id, unsigned int scancode) {
    switch (id) {
    case KB_D: 
        d_pressed = true;
        break;
    case KB_S:
        s_pressed = true;
        break;
    }
}

void key_release(KeyId id, unsigned int scancode) {
    switch (id) {
    case KB_D: 
        d_pressed = false;
        break;
    case KB_S:
        s_pressed = false;
        break;
    }
}

void game_load() {
    obj.x = 0;
    obj.y = 0;

    Image new_image;
    new_image.width = 16;
    new_image.height = 24;
    new_image.data = malloc(sizeof(char) * 4 * new_image.width * new_image.height);
    if (new_image.data == NULL) { printf("Helpppp\n"); }
    for (int i = 0; i < 4 * new_image.width * new_image.height; i += 4) {
        int row = i / 4 / new_image.width;
        int col = (i / 4) % new_image.width;
        new_image.data[i] = 0xff * row / new_image.height;
        new_image.data[i+1] = 0xff * col / new_image.width;
        new_image.data[i+2] = 0x80;
        new_image.data[i+3] = 0;
    }

    obj.sprite = new_sprite(0, 0, new_image.width, new_image.height, 0, new_image.data);
}

void game_update(double dt) {
    if (d_pressed) {
        obj.x += 1;
        SpriteInfo new_vals;
        new_vals.x = obj.x;
        sprite_set(obj.sprite, new_vals, SPRITE_X);
    }
    if (s_pressed) {
        obj.y += 1;
        SpriteInfo new_vals;
        new_vals.y = obj.y;
        sprite_set(obj.sprite, new_vals, SPRITE_Y);
    }
}
