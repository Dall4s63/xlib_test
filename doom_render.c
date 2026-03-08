/* I can't decide what to do, but this seems fun
 *
 * maybe can make a dungeon crawler
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "double_vec2.h"
#include "doom_render.h"

static int virtual_width;
static int virtual_height;

static Image v_canvas;
static char *zbuffer;

static double viewport_width = 0.5;

static DoubleVec2 cam_pos = { .x = 0.0, .y = 0.0 };
static DoubleVec2 cam_dir = { .x = 0.0, .y = 1.0 };
static double fov = 90.0;
static double cam_height = 1.5;

static MapRoom temp_room;

int render_setup(int v_width, int v_height) {
    virtual_width = v_width;
    virtual_height = v_height;
    v_canvas.width = v_width;
    v_canvas.height = v_height;
    v_canvas.data = malloc(sizeof(char) * 4 * v_width * v_height);
    if (v_canvas.data == NULL) {
        return 1;
    }
    zbuffer = malloc(sizeof(int) * v_width * v_height);
    if (zbuffer == NULL) {
        return 1;
    }

    temp_room.walls = malloc(sizeof(MapWall) * 4);
    temp_room.walls[0] = (MapWall) {
        .a = (DoubleVec2){ .x = -20.0, .y = -20.0 },
        .b = (DoubleVec2){ .x = -20.0, .y =  20.0 }
    };
    temp_room.walls[1] = (MapWall) {
        .a = (DoubleVec2){ .x = -20.0, .y =  20.0 },
        .b = (DoubleVec2){ .x =  20.0, .y =  20.0 }
    };
    temp_room.walls[2] = (MapWall) {
        .a = (DoubleVec2){ .x =  20.0, .y =  20.0 },
        .b = (DoubleVec2){ .x =  20.0, .y = -20.0 }
    };
    temp_room.walls[3] = (MapWall) {
        .a = (DoubleVec2){ .x = -20.0, .y = -20.0 },
        .b = (DoubleVec2){ .x =  20.0, .y = -20.0 }
    };

    return 0;
}

void render_run(Image out_canvas) {
    /*
     * The plan:
     *  - loop over each column of the virtual canvas
     *  - for each column, find its position using the 
     *    viewport_width and fov, then draw a line from the 
     *    camera's position.
     *  - find the closest intersecting wall and calculate
     *    the height that the wall would appear on screen
     *  - draw the the column
     */
    double wall_height = 15.0;
    for (int col = 0; col < v_canvas.width; ++col) {
    }
}

