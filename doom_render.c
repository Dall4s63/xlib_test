#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "double_vec2.h"
#include "doom_render.h"
#include "sprite_system.h"

static Image virtual_canvas;

static double vp_dist = 0.25;
// static double vp_width = 0.5;

static Camera cam = (Camera) { 
    (DoubleVec2) { .x = 0.0, .y = 0.0 },
    0.0,
    100.0,
    1.8
};

static MapRoom temp_room;

Image debug_canvas;

static int spr_id;

int render_setup(int v_width, int v_height) {
    spr_id = sprite_new("assets/test3.qoi");

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
    temp_room.wall_height = 4.0;

    return 0;
}

static void set_pixel_inv(Image canvas, int row, int col, char c[4]) {
    int i = (col * canvas.height + row) * 4;
    canvas.data[i + 0] = c[0];
    canvas.data[i + 1] = c[1];
    canvas.data[i + 2] = c[2];
    canvas.data[i + 3] = c[3];
}

static void set_pixel(Image canvas, int row, int col, char c[4]) {
    int i = (row * canvas.width + col) * 4;
    // TODO lsb vs msb
    canvas.data[i + 0] = c[2];
    canvas.data[i + 1] = c[1];
    canvas.data[i + 2] = c[0];
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
    memset(debug_canvas.data, 0, debug_canvas.width * debug_canvas.height * 4);
    // memset(canvas, 0, canvas_width * canvas_height * 4);
    // printf("w: %d, h: %d\n", canvas_width, canvas_height);

    // TODO debug stuff
    double debug_xoff = 60.0;
    double debug_yoff = 60.0;
    double debug_sf = 3.0;

    // TODO fix this
    // double vp_dist = (vp_width/2) / tan(cam.fov * M_PI / (180.0 * 2.0));
    // printf("%lf\n", vp_dist);
    double vp_width = tan(cam.fov * M_PI / (180.0 * 2.0)) * vp_dist * 2;
    double vc_width = (double) virtual_canvas.width;
    double vc_height = (double) virtual_canvas.height;
    double vp_height;
    {
    double aspect_ratio = vc_height / vc_width;
    vp_height = vp_width * aspect_ratio;
    }
    
    for (int column_i = 0; column_i < virtual_canvas.width; ++column_i) {
        double col = (double)column_i;

        DoubleRay ray;
        double vp_dwidth = (col / vc_width - 0.5) * vp_width;
        ray.dir.x = vp_dwidth;
        ray.dir.y = vp_dist;
        ray.dir = vec_normalize(ray.dir);
        ray.dir = vec_rotate(ray.dir, cam.angle);
        ray.origin = cam.pos;
        // TODO find a suitably large number
        double distance = 100000.0;
        int wall_i = -1;
        // double flat_dtp = vp_dist;
        double flat_dtp = sqrt(vp_dwidth * vp_dwidth + vp_dist * vp_dist);
        double wall_xdist;

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
                vec.x = i_point.x - temp_room.walls[i].a.x;
                vec.y = i_point.y - temp_room.walls[i].a.y;
                wall_xdist = sqrt(vec.x * vec.x + vec.y * vec.y);
            }
        }

        int wall_top;
        int wall_bot;
        {
        double wall_above_cam = temp_room.wall_height - cam.height;
        double wall_below_cam = cam.height;
        double swh = wall_above_cam / distance * flat_dtp;
        // wall_top = (int)((swh / vp_height + 0.5) * vc_height) - 1; 
        // wall_top = (int)((0.5 - swh / vp_height) * vc_height) + 1; 
        wall_top = (int)((0.5 - swh / vp_height) * vc_height); 
        // printf("%lf\n", swh/vp_height);
        // wall_top = (int)((swh / vp_height) * vc_height) - 1; 
        swh = wall_below_cam / distance * flat_dtp;
        // printf("%lf\n", swh/vp_height);
        //wall_bot = (int)((0.5 + swh / vp_height) * vc_height) - 1;
        wall_bot = (int)((0.5 + swh / vp_height) * vc_height);

        if (distance >= 100000.0) {
            wall_top += 1;
            wall_bot -= 1;
        }
        // wall_bot = (int)((swh / vp_height) * vc_height) + 1;
        // printf("%d, %d\n", wall_top, wall_bot);
        // wall_top = virtual_canvas.height - wall_top;
        // wall_bot = virtual_canvas.height - wall_bot;
        }

        for (int row_i = 0; row_i < virtual_canvas.height; ++row_i) {
            // int i = (row_i * virtual_canvas.width + column_i) * 4;
            double row = (double)row_i;
            char c[4];

            if (row_i <= wall_bot && row_i >= wall_top) {
                // printf("wall\n");
                // wall
                DoubleVec2 spot;
                double dv = (row / vc_height - 0.5) * vp_height;
                double wall_ydist = temp_room.wall_height - cam.height + dv / flat_dtp * distance;
                double _;
                double samplex = modf(wall_xdist, &_);
                double sampley = modf(wall_ydist, &_);
                if (sampley < 0.0) {
                    sampley = 0.0;
                }
                FColor c_sample = sprite_fsample(spr_id, samplex, sampley);
                // double vert_dtp = sqrt(flat_dtp * flat_dtp + dv * dv);
                c[0] = (unsigned char)(c_sample.r * 255);
                c[1] = (unsigned char)(c_sample.g * 255);
                c[2] = (unsigned char)(c_sample.b * 255);
                c[3] = (unsigned char)(c_sample.a * 255);
                // if (wall_i % 2) {
                //     c[0] = 0x30;
                //     c[1] = 0x50;
                //     c[2] = 0x30;
                //     c[3] = 0xff;
                // } else {
                //     c[0] = 0x25;
                //     c[1] = 0x40;
                //     c[2] = 0x25;
                //     c[3] = 0xff;
                // }

            } else {
                // floor / ceiling
                if (row_i <= virtual_canvas.height/2) {
                    // ceiling
                    c[0] = 0x40;
                    c[1] = 0x30;
                    c[2] = 0x25;
                    c[3] = 0xff;
                } else {
                    // floor
                    DoubleVec2 spot;
                    // printf("row: %lf\n", fabs(row / vc_height) * vp_height);
                    double dv = fabs((row)/vc_height - 0.5) * vp_height;
                    double spot_len = flat_dtp / dv * cam.height;
                    spot.x = ray.dir.x * spot_len + cam.pos.x;
                    spot.y = ray.dir.y * spot_len + cam.pos.y;
                    double _;
                    double samplex = modf(spot.x, &_);
                    if (samplex < 0) {
                        samplex += 1.0;
                    }
                    double sampley = modf(spot.y, &_);
                    if (sampley < 0) {
                        sampley += 1.0;
                    }
                    FColor c_sample = sprite_fsample(spr_id, samplex, sampley);
                    // double vert_dtp = sqrt(flat_dtp * flat_dtp + dv * dv);
                    c[0] = (unsigned char)(c_sample.r * 255);
                    c[1] = (unsigned char)(c_sample.g * 255);
                    c[2] = (unsigned char)(c_sample.b * 255);
                    c[3] = (unsigned char)(c_sample.a * 255);
                }
            }
            set_pixel(virtual_canvas, row_i, column_i, c);
        }
    }

    // {
    // int width = sprite_width(spr_id);
    // int height = sprite_height(spr_id);
    // for (int x = 0; x < width; ++x) {
    //     for (int y = 0; y < height; ++y) {
    //         FColor fc = sprite_sample(spr_id, x, y);
    //         // printf("%f %f %f %f\n", fc.r, fc.g, fc.b, fc.a);
    //         unsigned char c[4];
    //         c[0] = (unsigned char) (fc.r * 255.0);
    //         c[1] = (unsigned char) (fc.g * 255.0);
    //         c[2] = (unsigned char) (fc.b * 255.0);
    //         c[3] = (unsigned char) (fc.a * 255.0);
    //         // printf("%x%x%x%x\n", c[0], c[1], c[2], c[3]);
    //         set_pixel(virtual_canvas, y + 100, x + 100, c);
    //     }
    // }
    // }

    for (int i = 0; i < canvas.width * canvas.height * 4; i += 4) {
        int row = i / 4 / canvas.width;
        int col = i / 4 - row * canvas.width;
        int v_row = row * virtual_canvas.height / canvas.height;
        int v_col = col * virtual_canvas.width / canvas.width;
        int v_index = (v_row * virtual_canvas.width + v_col) * 4;
        // int v_index = (v_col * virtual_canvas.height + v_row) * 4;
        canvas.data[i + 0] = virtual_canvas.data[v_index + 0];
        canvas.data[i + 1] = virtual_canvas.data[v_index + 1];
        canvas.data[i + 2] = virtual_canvas.data[v_index + 2];
        canvas.data[i + 3] = virtual_canvas.data[v_index + 3];
    }
}

DoubleVec2 cam_pos_add(DoubleVec2 v) {
    return (DoubleVec2) {
        .x = cam.pos.x += v.x,
        .y = cam.pos.y += v.y,
    };
}

double cam_angle_add(double a) {
    return cam.angle += a;
}

