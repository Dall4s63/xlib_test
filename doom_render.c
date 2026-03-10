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
    temp_room.walls_buf_len = 4;
    temp_room.walls_len = 4;

    return 0;
}

// static double viewport_width = 0.5;
// 
// static DoubleVec2 cam_pos = { .x = 0.0, .y = 0.0 };
// static DoubleVec2 cam_dir = { .x = 0.0, .y = 1.0 };
// static double fov = 90.0;
// static double cam_height = 1.5;
// 
// static MapRoom temp_room;

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
    double wall_height = 3.0;
    for (int col = 0; col < v_canvas.width; ++col) {
        // TODO for now just assume the fov is 90
        double viewport_dist = viewport_width / 2;
        // TODO for now also we won't rotate the viewport to match the camera
        DoubleVec2 viewport_point;
        viewport_point.x = (double)col / (double)v_canvas.width * viewport_width - viewport_width / 2.0 + cam_pos.x;
        viewport_point.y = viewport_dist + cam_pos.y;
        DoubleLine ray = double_line_from(cam_pos, viewport_point);
        double distance = 0.0;
        int wall_i = -1;
        for (int i = 0; i < temp_room.walls_len; ++i) {
            DoubleVec2 end_a = temp_room.walls[i].a;
            DoubleVec2 end_b = temp_room.walls[i].b;
            DoubleVec2 i_point;
            bool res = double_line_in_segment(ray, end_a, end_b, &i_point);
            if (res) {
            }
        }
        for (int row = 0; row < v_canvas.height; ++row) {
        }
    }
}

