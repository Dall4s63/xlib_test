#include <stdlib.h>
#include <stdio.h>

// #include "tactics_render.h"
#include "doom_render.h"
#include "double_vec2.h"
#define KB_MY_DEFAULTS
#include "keyboard.h"
#include "game.h"

// typedef struct object {
//     int x;
//     int y;
//     DrawableId sprite;
// } Object;

// static Object cursor;
static DoubleVec2 cam_vel = (DoubleVec2){ .x = 0.0, .y = 0.0 };
static double cam_angle_vel = 0.0;

void window_destroyed(void) {
    // here we can do the game cleanup
}

void key_press(KeyId id, unsigned int scancode, bool is_repeat) {
    if (is_repeat) {
        printf("Recieved a repeat\n");
    }
    switch (id) {
    case KB_W:
        cam_vel.y += 20.0;
        break;
    case KB_A:
        cam_vel.x -= 20.0;
        break;
    case KB_D: 
        cam_vel.x += 20.0;
        break;
    case KB_S:
        cam_vel.y -= 20.0;
        break;
    case KB_Left:
        cam_angle_vel += 4.0;
        break;
    case KB_Right:
        cam_angle_vel -= 4.0;
        break;
    }
}

void key_release(KeyId id, unsigned int scancode) {
    switch (id) {
    case KB_W:
        cam_vel.y -= 20.0;
        break;
    case KB_A:
        cam_vel.x += 20.0;
        break;
    case KB_D: 
        cam_vel.x -= 20.0;
        break;
    case KB_S:
        cam_vel.y += 20.0;
        break;
    case KB_Left:
        cam_angle_vel -= 4.0;
        break;
    case KB_Right:
        cam_angle_vel += 4.0;
        break;
    }
}

void game_load() {
}

void game_update(double dt) {
    cam_pos_add(scalar_product(cam_vel, dt));
    cam_angle_add(cam_angle_vel * dt);
}
