#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "double_vec2.h"
#include "doom_render.h"

static Image virtual_canvas;

static double vp_width = 0.5;

static Camera cam = (Camera) { 
    (DoubleVec2) { .x = 0.0, .y = 0.0 },
    0.0,
    90.0,
    1.8
};

static MapRoom temp_room;

Image debug_canvas;

int render_setup(int v_width, int v_height) {
    virtual_canvas.width = v_width;
    virtual_canvas.height = v_height;
    virtual_canvas.data = malloc(sizeof(char) * 4 * v_width * v_height);
    if (virtual_canvas.data == NULL) {
        return 1;
    }

    // TODO debug stuff
    debug_canvas.width = v_width;
    debug_canvas.height = v_height;
    debug_canvas.data = malloc(sizeof(char) * 4 * v_width * v_height);

    // TODO initialise the room

    return 0;
}

static void set_pixel(Image canvas, int row, int col, char c[4]) {
    int i = (row * canvas.width + col) * 4;
    canvas.data[i + 0] = c[0];
    canvas.data[i + 1] = c[1];
    canvas.data[i + 2] = c[2];
    canvas.data[i + 3] = c[3];
}

static void draw_line(Image canvas, DoubleVec2 a, DoubleVec2 b, char c[4]) {
    // printf("drawing line from (%lf, %lf) to (%lf, %lf)\n",
    //     a.x, a.y, b.x, b.y);
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double l_diff = abs((abs(dx) > abs(dy)) ? dx : dy);
    dx = dx / l_diff;
    dy = dy / l_diff;
    bool axl = a.x < b.x;
    bool ayl = a.y < b.y;
    for (; (axl ? a.x <= b.x : a.x >= b.x) && (ayl ? a.y <= b.y : a.y >= b.y); a.x += dx, a.y += dy) {
        // printf("%lf, %lf\n", a.x, a.y);
        int x = (int)a.x;
        int y = (int)a.y;
        if (x < 0 || x >= canvas.width || y < 0 || y >= canvas.height) {
            continue;
        }
        int i = (x * canvas.width + y) * 4;
        canvas.data[i + 0] = c[0];
        canvas.data[i + 1] = c[1];
        canvas.data[i + 2] = c[2];
        canvas.data[i + 3] = c[3];
    }
}

void render_run(Image canvas) {
    memset(debug_c.data, 0, debug_c.width * debug_c.height * 4);

    // TODO debug stuff
    double debug_xoff = 60.0;
    double debug_yoff = 60.0;
    double debug_sf = 3.0;

    double vp_dist = (vp_width/2) / tan(cam.fov * M_PI / (180.0 * 2.0));
    double vc_width = (double) virtual_canvas.width;
    double vc_height = (double) virtual_canvas.height;
    double vp_height;
    {
    double aspect_ratio = vc_height / vc_width ;
    vp_height = vp_width * aspect_ratio;
    }
    
    for (int column_i = 0; column_i < virtual_canvas.width; ++column_i) {
        double col = (double)column_i;

        DoubleRay ray;
        double vp_dwidth = (col / vc_width - 0.5) * vp_width
        ray.dir.x = vp_dwidth;
        ray.dir.y = vp_dist;
        ray.dir = vec_normalize(ray.dir);
        ray.dir = vec_rotate(ray.dir, cam.angle);
        ray.origin = cam.pos;
        // TODO find a suitably large number
        double distance = 100000.0;
        int wall_i = -1;
        double flat_dtp = sqrt(vp_dwidth * vp_dwidth + vp_dist * vp_dist);

        for (int i = 0; i < temp_room.walls_len; ++i) {
            DoubleVec2 end_a = temp_room.walls[i].a;
            DoubleVec2 end_b = temp_room.walls[i].b;
            DoubleVec2 i_point;
            bool res = double_ray_in_segment(ray, end_a, end_b, &i_point);
            if (!res) {
                continue;
            }
            DoubleVec2 vec = (DoubleVec2) { .x = i_point.x - cam.pos.x, .y = i_point.y - cam.pos.y };
            double d = sqrt(vec.x * vec.x + vec.y * vec.y);
            if (d < distance && d > flat_dtp) {
                distance = d;
                wall_i = i;
            }
        }

        int wall_top;
        int wall_bot;
        {
        double wall_above_cam = temp_room.wall_height - cam.height;
        double wall_below_cam = cam.height;
        double swh = wall_above_cam / distance * dtp;
        wall_top = (int) ((swh / vp_height + 0.5) * vc_height) - 1; 
        double swh = wall_below_cam / distance * dtp;
        wall_bot = (int) ((0.5 - swh / vp_height) * vc_height) + 1;
        wall_top = virtual_canvas.height - wall_top;
        wall_bot = virtual_canvas.height - wall_bot;
        }

        for (int row_i = 0; row_i < virtual_canvas.height; ++row_i) {
            int i = (row_i * virtual_canvas.width + column_i) * 4;
            double row = (double)row_i;

            if (row_i <= wall_bot && row_i >= wall_top) {
                // wall
            }

            // floor / ceiling
        }
    }
}
