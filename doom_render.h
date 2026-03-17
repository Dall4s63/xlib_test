/* hehe
 *
 */

#ifndef DOOM_RENDER_H
#define DOOM_RENDER_H

#include <stdint.h>

#include "display.h"
#include "double_vec2.h"

typedef struct _camera {
    DoubleVec2 pos;
    double angle;
    double fov;
    double height;
} Camera;

typedef struct _map_wall {
    DoubleVec2 a;
    DoubleVec2 b;
    int spr;
    // TODO wall sprite stuff
} MapWall;

typedef struct _map_room {
    MapWall *walls; 
    int walls_buf_len;
    int walls_len;
    double wall_height;
    // TODO lights information.
    // TODO link to other rooms?
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
