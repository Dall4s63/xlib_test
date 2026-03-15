/* hehe
 *
 */

#ifndef DOOM_RENDER_H
#define DOOM_RENDER_H

#include <stdint.h>

#include "display.h"
#include "double_vec2.h"

typedef struct _rgba_fcolor {
    float r;
    float g;
    float b;
    float a;
} FColor;

typedef struct _camera {
    DoubleVec2 pos;
    double angle;
    double fov;
    double height;
} Camera;

typedef struct _map_wall {
    DoubleVec2 a;
    DoubleVec2 b;
} MapWall;

typedef struct _map_room {
    // TODO include information about doors and stuff
    MapWall *walls; 
    int walls_buf_len;
    int walls_len;
} MapRoom;

typedef struct _map {
    MapRoom *rooms;
    int rooms_buf_len;
    int rooms_len;
} Map;

int render_setup(int v_width, int v_height);
void render_run(Image out_canvas);

DoubleVec2 cam_pos_add(DoubleVec2 v);
double cam_angle_add(double a);

#endif
