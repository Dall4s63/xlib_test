/* I can't decide what to do, but this seems fun
 *
 * maybe can make a dungeon crawler
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "double_vec2.h"
#include "doom_render.h"

static int virtual_width;
static int virtual_height;

static Image v_canvas;
static char *zbuffer;

static double viewport_width = 0.5;

static DoubleVec2 cam_pos = { .x = 0.0, .y = 0.0 };
// static DoubleVec2 cam_dir = { .x = 1.0, .y = 0.0 };
static double cam_angle = 0.0;
static double fov = 90.0;
static double cam_height = 1.8;

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

    temp_room.walls_buf_len = 12;
    temp_room.walls_len = 12;
    temp_room.walls = malloc(sizeof(MapWall) * temp_room.walls_buf_len);
    temp_room.walls[0] = (MapWall) {
        .a = (DoubleVec2){ .x = -10.0, .y = - 5.0 },
        .b = (DoubleVec2){ .x = -10.0, .y =   7.0 }
    };
    temp_room.walls[1] = (MapWall) {
        .a = (DoubleVec2){ .x = -10.0, .y =   7.0 },
        .b = (DoubleVec2){ .x = - 5.0, .y =   7.0 }
    };
    temp_room.walls[2] = (MapWall) {
        .a = (DoubleVec2){ .x = - 5.0, .y =   7.0 },
        .b = (DoubleVec2){ .x = - 5.0, .y =  10.0 }
    };
    temp_room.walls[3] = (MapWall) {
        .a = (DoubleVec2){ .x = - 5.0, .y =  10.0 },
        .b = (DoubleVec2){ .x =   5.0, .y =  10.0 }
    };
    temp_room.walls[4] = (MapWall) {
        .a = (DoubleVec2){ .x =   5.0, .y =  10.0 },
        .b = (DoubleVec2){ .x =   5.0, .y =   7.0 }
    };
    temp_room.walls[5] = (MapWall) {
        .a = (DoubleVec2){ .x =   5.0, .y =   7.0 },
        .b = (DoubleVec2){ .x =  10.0, .y =   7.0 }
    };
    temp_room.walls[6] = (MapWall) {
        .a = (DoubleVec2){ .x =  10.0, .y =   7.0 },
        .b = (DoubleVec2){ .x =  10.0, .y = - 5.0 }
    };
    temp_room.walls[7] = (MapWall) {
        .a = (DoubleVec2){ .x =  10.0, .y = - 5.0 },
        .b = (DoubleVec2){ .x =   5.0, .y = - 5.0 }
    };
    temp_room.walls[8] = (MapWall) {
        .a = (DoubleVec2){ .x =   5.0, .y = - 5.0 },
        .b = (DoubleVec2){ .x =   5.0, .y = -10.0 }
    };
    temp_room.walls[9] = (MapWall) {
        .a = (DoubleVec2){ .x =   5.0, .y = -10.0 },
        .b = (DoubleVec2){ .x = - 5.0, .y = -10.0 }
    };
    temp_room.walls[10] = (MapWall) {
        .a = (DoubleVec2){ .x = - 5.0, .y = -10.0 },
        .b = (DoubleVec2){ .x = - 5.0, .y = - 5.0 }
    };
    temp_room.walls[11] = (MapWall) {
        .a = (DoubleVec2){ .x = - 5.0, .y = - 5.0 },
        .b = (DoubleVec2){ .x = -10.0, .y = - 5.0 }
    };

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

void render_run(Image canvas) {

    // TODO walls/rooms should determine the height of the walls
    double wall_height = 4.0;

    for (int col = 0; col < v_canvas.width; ++col) {

        // TODO for now just assume the fov is 90
        double viewport_dist = viewport_width / 2;

        // TODO for now also we won't rotate the viewport to match the camera
        DoubleRay ray;
        ray.dir.x = ((double)col + 0.5) / (double)v_canvas.width * viewport_width - viewport_width / 2.0;
        ray.dir.y = viewport_dist; 
        ray.dir = vec_normalize(ray.dir);
        ray.dir = vec_rotate(ray.dir, cam_angle);
        ray.origin = cam_pos;
        // TODO find a suitably large number
        double distance = 100000.0;
        int wall_i = -1;

        for (int i = 0; i < temp_room.walls_len; ++i) {
            DoubleVec2 end_a = temp_room.walls[i].a;
            DoubleVec2 end_b = temp_room.walls[i].b;
            DoubleVec2 i_point;
            bool res = double_ray_in_segment(ray, end_a, end_b, &i_point);
            if (res) {
                // printf("i_point: (%lf, %lf)\n", i_point.x, i_point.y);
                DoubleVec2 vec = (DoubleVec2) { .x = i_point.x - cam_pos.x, .y = i_point.y - cam_pos.y };
                // TODO this also needs to be fixed
                // double d = i_point.y - cam_pos.y;
                double d = sqrt(vec.x * vec.x + vec.y * vec.y);
                if (d < distance) {
                    distance = d;
                    wall_i = i;
                }
            }
        }

        double aspect = (double)v_canvas.width / (double)v_canvas.height;
        double viewport_height = viewport_width / aspect;
        double v_from_center = ((double)col - (double)v_canvas.width / 2.0) / (double)v_canvas.width * viewport_width;
        double dtp = sqrt(viewport_dist * viewport_dist + v_from_center * v_from_center);
        int wall_top = (int)((wall_height - cam_height) * (dtp / distance) * (double)v_canvas.height + (double)v_canvas.height/2) - 1;
        // printf("wall_top: %lf, %d\n", (wall_height - cam_height) * (viewport_dist / distance), wall_top);
        // int wall_top = (int)((wall_height - cam_height) * (viewport_dist / distance) + viewport_height/2);
        int wall_bot = (int)((double)v_canvas.height/2 - cam_height * (dtp / distance) * (double)v_canvas.height) + 1;

        wall_top = v_canvas.height - wall_top;
        wall_bot = v_canvas.height - wall_bot;
        // printf("wall_top: %d\n", wall_top);
        // printf("wall_bot: %d\n", wall_bot);
        
        if (distance < dtp) { 
            wall_top = v_canvas.height / 2 + 1;
            wall_bot = v_canvas.height / 2 - 1;
        }

        for (int row = 0; row < v_canvas.height; ++row) {
            int i = (row * v_canvas.width + col) * 4;

            if (row < wall_top) {
                // ceiling
                v_canvas.data[i]   = 0x10;
                v_canvas.data[i+1] = 0x10;
                v_canvas.data[i+2] = 0x80;
                v_canvas.data[i+3] = 0xff;
                continue;
            }

            if (row > wall_bot) {
                // floor
                double row_height = ((double)(row) + 0.5) / (double)v_canvas.height * viewport_height - viewport_height / 2.0;
                DoubleVec2 spot;
                // TODO figure out why we need to multiply by viewport_height to make it look right
                double dist = dtp * viewport_height * cam_height / row_height;
                spot.x = cam_pos.x + ray.dir.x * dist;
                spot.y = cam_pos.y + ray.dir.y * dist;
                // printf("spot.x: %lf, spot.y: %lf\n", row_height, dtp);
                v_canvas.data[i+0] = 0x60;
                v_canvas.data[i+1] = 0x20;
                v_canvas.data[i+2] = 0x40;
                v_canvas.data[i+3] = 0xff;
                continue;
            }

            // wall
            if (wall_i % 2) {
                v_canvas.data[i]   = 0x10;
                v_canvas.data[i+1] = 0xc0;
                v_canvas.data[i+2] = 0x10;
                v_canvas.data[i+3] = 0xff;
            } else {
                v_canvas.data[i]   = 0x10;
                v_canvas.data[i+1] = 0x90;
                v_canvas.data[i+2] = 0x10;
                v_canvas.data[i+3] = 0xff;
            }
        }
    }

    for (int i = 0; i < canvas.width * canvas.height * 4; i += 4) {
        int row = i / 4 / canvas.width;
        int col = i / 4 - row * canvas.width;
        int v_row = row * v_canvas.height / canvas.height;
        int v_col = col * v_canvas.width / canvas.width;
        int v_index = (v_row * v_canvas.width + v_col) * 4;
        canvas.data[i + 0] = v_canvas.data[v_index + 0];
        canvas.data[i + 1] = v_canvas.data[v_index + 1];
        canvas.data[i + 2] = v_canvas.data[v_index + 2];
        canvas.data[i + 3] = v_canvas.data[v_index + 3];
    }
}

DoubleVec2 cam_pos_add(DoubleVec2 v) {
    return (DoubleVec2) {
        .x = cam_pos.x += v.x,
        .y = cam_pos.y += v.y,
    };
}

double cam_angle_add(double a) {
    return cam_angle += a;
}

