#include <stdlib.h>
#include <stdio.h>

// #include "tactics_render.h"
#define KB_MY_DEFAULTS
#include "keyboard.h"
#include "game.h"

// typedef struct object {
//     int x;
//     int y;
//     DrawableId sprite;
// } Object;

// static Object cursor;

void window_destroyed(void) {
    // here we can do the game cleanup
}

void key_press(KeyId id, unsigned int scancode, bool is_repeat) {
    if (is_repeat) {
        printf("Recieved a repeat\n");
    }
    switch (id) {
    case KB_W:
        break;
    case KB_A:
        break;
    case KB_D: 
        break;
    case KB_S:
        break;
    }
}

void key_release(KeyId id, unsigned int scancode) {
}

void game_load() {
}

void game_update(double dt) {
}
