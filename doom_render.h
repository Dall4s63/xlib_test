/* hehe
 *
 */

#ifndef DOOM_RENDER_H
#define DOOM_RENDER_H

#include <stdint.h>

#include "display.h"
#include "float_vec2.h"
#include "sprite_system.h"

typedef struct _camera {
    FloatVec2 pos;
    float angle;
    float fov;
    float height;
} Camera;

typedef struct _point_light {
    FColor c;
    float r;
} PointLight;

typedef struct _spr_object {
    FloatVec2 pos;
    float width;
    float height;
    float elev;
    int spr;
} SprObject;

typedef struct _map_wall {
    FloatVec2 a;
    FloatVec2 b;
    int spr;
    // TODO wall sprite stuff
} MapWall;

typedef struct _map_room {
    MapWall *walls; 
    int walls_buf_len;
    int walls_len;
    float wall_height;
    int floor_spr;
    int ceil_spr;
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
// void render_run(int in_width, int in_height, unsigned char *canvas);

FloatVec2 cam_pos_add(FloatVec2 v);
float cam_angle_add(float a);

#endif
