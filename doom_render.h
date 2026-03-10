/* hehe
 *
 */

#ifndef DOOM_RENDER_H
#define DOOM_RENDER_H

#include "display.h"
#include "double_vec2.h"

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

int render_setup(int v_width, int v_height);
void render_run(Image out_canvas);

#endif
