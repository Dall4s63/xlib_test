#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "float_vec2.h"
#include "doom_render.h"
#include "sprite_system.h"
#include "map_loader.h"

static Image virtual_canvas;
float *zbuffer;

static float vp_dist = 0.25;
// static float vp_width = 0.5;

static Camera cam = (Camera) { 
    .pos = (FloatVec2) { .x = 0.0, .y = 0.0 },
    .angle = 0.0,
    .fov = 100.0,
    .height = 1.8,
};

static bool state_update = true;

static PointLight cam_light = (PointLight) {
    .c = (FColor) { .r = 1.0, .g = 0.9, .b = 0.85, .a = 1.0 },
    .r = 12.0,
};

static FColor global_illum = (FColor) { .r = 0.04, .g = 0.04, .b = 0.04, .a = 0.04 };
// static FColor global_illum = (FColor) { .r = 0.30, .g = 0.30, .b = 0.30, .a = 0.30 };

static MapRoom temp_room;

static SprObject *objs;
static int objs_len;

// Image debug_canvas;

FColor get_plight_color(PointLight l, float dist);

int render_setup(int v_width, int v_height) {
    // spr_id = sprite_new("assets/frog.qoi");

    virtual_canvas.width = v_width;
    virtual_canvas.height = v_height;
    virtual_canvas.data = malloc(sizeof(char) * 4 * v_width * v_height);
    if (virtual_canvas.data == NULL) {
        return 1;
    }

    zbuffer = malloc(sizeof(*zbuffer) * v_width * v_height);

    // TODO debug stuff
    // debug_canvas.width = v_width;
    // debug_canvas.height = v_height;
    // debug_canvas.data = malloc(sizeof(char) * 4 * v_width * v_height);

    load_room("assets/rooms/test.room", &temp_room);

    objs_len = 3;
    objs = malloc(sizeof(SprObject) * objs_len);
    objs[0].spr = sprite_new("assets/mushroom.qoi");
    objs[0].pos.x = 0.0;
    objs[0].pos.y = 0.0;
    objs[0].width = 3.0;
    objs[0].height = 3.0;
    objs[0].elev = 0.0;
    objs[1].spr = sprite_new("assets/mushroom.qoi");
    objs[1].pos.x = 1.5;
    objs[1].pos.y = 1.5;
    objs[1].width = 1.0;
    objs[1].height = 1.0;
    objs[1].elev = 2.0;
    objs[2].spr = sprite_new("assets/mushroom.qoi");
    objs[2].pos.x = -1.5;
    objs[2].pos.y = -1.5;
    objs[2].width = 1.2;
    objs[2].height = 1.2;
    objs[2].elev = 0.5;


    return 0;
}

static void set_pixel_inv(Image canvas, int row, int col, char c[4]) {
    int i = (col * canvas.height + row) * 4;
    canvas.data[i + 0] = c[0];
    canvas.data[i + 1] = c[1];
    canvas.data[i + 2] = c[2];
    canvas.data[i + 3] = c[3];
}

static void get_pixel(Image canvas, int row, int col, unsigned char c[4]) {
    int i = (row * canvas.width + col) * 4;
    // TODO lsb vs msb
    c[2] = canvas.data[i + 0];
    c[1] = canvas.data[i + 1];
    c[0] = canvas.data[i + 2];
    c[3] = canvas.data[i + 3];
}

static void set_pixel(Image canvas, int row, int col, char c[4]) {
    int i = (row * canvas.width + col) * 4;
    // TODO lsb vs msb
    canvas.data[i + 0] = c[2];
    canvas.data[i + 1] = c[1];
    canvas.data[i + 2] = c[0];
    canvas.data[i + 3] = c[3];
}

static void draw_line(Image canvas, FloatVec2 a, FloatVec2 b, char c[4]) {
    // printf("drawing line from (%lf, %lf) to (%lf, %lf)\n",
    //     a.x, a.y, b.x, b.y);
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float l_diff = abs((abs(dx) > abs(dy)) ? dx : dy);
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

static int imin(int a, int b) {
    return (a < b) ? a : b;
}

static int imax(int a, int b) {
    return (a > b) ? a : b;
}

static bool is_almost(float a, float b) {
    return b-1e-12 <= a && a <= b+1e-12;
}

void render_run(Image canvas) {
    if (!state_update) {
        return;
    }
    state_update = false;
    // memset(debug_canvas.data, 0, debug_canvas.width * debug_canvas.height * 4);
    memset(canvas.data, 0, sizeof(char) * canvas.width * canvas.height * 4);
    memset(zbuffer, 0, sizeof(float) * virtual_canvas.width * virtual_canvas.height);
    // printf("w: %d, h: %d\n", canvas_width, canvas_height);

    // TODO debug stuff
    // float debug_xoff = 60.0;
    // float debug_yoff = 60.0;
    // float debug_sf = 3.0;

    // TODO fix this
    // float vp_dist = (vp_width/2) / tan(cam.fov * M_PI / (180.0 * 2.0));
    // printf("%lf\n", vp_dist);
    float vp_width = tan(cam.fov * M_PI / (180.0 * 2.0)) * vp_dist * 2;
    float vc_width = (float) virtual_canvas.width;
    float vc_height = (float) virtual_canvas.height;
    float vp_height;
    {
    float aspect_ratio = vc_height / vc_width;
    vp_height = vp_width * aspect_ratio;
    }

    for (int i = 0; i < objs_len; ++i) {
        FloatVec2 to_obj = vec_sub(objs[i].pos, cam.pos);
        // TODO threshold values
        if (is_almost(to_obj.x, 0.0) && is_almost(to_obj.y, 0.0)) {
            continue;
        }
        to_obj = vec_rotate(to_obj, -cam.angle);
        FloatVec2 cam_dir = (FloatVec2) { .x = 0.0, .y = vp_dist };
        float a = fabs(vec_angle(cam_dir, to_obj)) * 180 / M_PI;
        // printf("a: %lf\n", a);
        if (a > 90.0) {
            continue;
        }
        FloatVec2 vp_dir = (FloatVec2) { .x = 1.0, .y = 0.0};
        FloatRay spr_ray;
        spr_ray.dir = to_obj;
        spr_ray.origin = cam.pos;
        FloatRay vp_ray;
        vp_ray.dir = vp_dir;
        vp_ray.origin = vec_add(cam_dir, cam.pos);
        FloatVec2 inp;
        float_ray_intersect(vp_ray, spr_ray, &inp);
        inp = vec_sub(inp, cam.pos);
        // printf("inp: (%lf, %lf)\n", inp.x, inp.y);
        float dtp = vec_mag(inp);
        float dist = vec_mag(to_obj);
        // TODO figure out if there is a more elegant solution
        if (dist < dtp) { continue; }
        float ratio = dtp / dist;
        float height = objs[i].height * ratio / vp_width * vc_width;
        float top_lim = ((cam.height - objs[i].height - objs[i].elev) * ratio / vp_height + 0.5) * vc_height;
        int blim = (int)(((cam.height - objs[i].elev) * ratio / vp_height + 0.5) * vc_height);
        int tlim = (int)top_lim;
        float width = objs[i].width * ratio / vp_width * vc_width;
        float left_lim = -width / 2.0 + (inp.x / vp_width + 0.5) * vc_width;
        int llim = (int)left_lim;
        int rlim = (int)(width / 2.0 + (inp.x / vp_width + 0.5) * vc_width);
        // int count = 0;
        int left_pixel = imax(llim, 0);
        int right_pixel = imin(rlim, virtual_canvas.width - 1);
        int top_pixel = imax(tlim, 0);
        int bot_pixel = imin(blim, virtual_canvas.height - 1);
        // printf("l: %d, r: %d, t: %d, b: %d\n", left_pixel, right_pixel, top_pixel, bot_pixel);
        for (int x = left_pixel; x <= right_pixel; ++x) {
            float dx = ((float)x - left_lim) / width;
            // printf("dx: %lf\n", dx);
            for (int y = top_pixel; y <= bot_pixel; ++y) {
                float dy = ((float)y - top_lim) / height;
                FColor fc = sprite_fsample(objs[i].spr, dx, dy);
                FColor lc = get_plight_color(cam_light, dist);
                lc = fcolor_add(lc, global_illum);
                fc = fcolor_mul(lc, fc);
                float z = zbuffer[y * virtual_canvas.width + x];
                if (z > 0.0 && dist < z) {
                    // this pixel is over 
                    unsigned char ca[4];
                    get_pixel(virtual_canvas, y, x, ca);
                    FColor a = fcolor_from(ca);
                    fc = fcolor_over(fc, a);
                    zbuffer[y * virtual_canvas.width + x] = dist;
                } else if (z > 0.0) {
                    // this pixel is under
                    unsigned char ca[4];
                    get_pixel(virtual_canvas, y, x, ca);
                    FColor a = fcolor_from(ca);
                    fc = fcolor_over(a, fc);
                } else {
                    zbuffer[y * virtual_canvas.width + x] = dist;
                }
                char c[4];
                c[0] = (unsigned char)(fc.r * 255);
                c[1] = (unsigned char)(fc.g * 255);
                c[2] = (unsigned char)(fc.b * 255);
                c[3] = (unsigned char)(fc.a * 255);
                set_pixel(virtual_canvas, y, x, c);
                // count++;
            }
        }
        // printf("count: %d\n", count);
    }

    float col = 0.0;
    for (int column_i = 0; column_i < virtual_canvas.width; ++column_i, ++col) {

        FloatRay ray;
        float vp_dwidth = (col / vc_width - 0.5) * vp_width;
        ray.dir.x = vp_dwidth;
        ray.dir.y = vp_dist;
        ray.dir = vec_normalize(ray.dir);
        ray.dir = vec_rotate(ray.dir, cam.angle);
        ray.origin = cam.pos;
        // TODO find a suitably large number
        float distance = 100000.0;
        int wall_i = -1;
        // float flat_dtp = vp_dist;
        float flat_dtp = sqrt(vp_dwidth * vp_dwidth + vp_dist * vp_dist);
        float wall_xdist;
        float wall_len;

        for (int i = 0; i < temp_room.walls_len; ++i) {
            FloatVec2 end_a = temp_room.walls[i].a;
            FloatVec2 end_b = temp_room.walls[i].b;
            FloatVec2 i_point;
            bool res = float_ray_in_segment(ray, end_a, end_b, &i_point);
            if (!res) {
                continue;
            }
            FloatVec2 vec = (FloatVec2) { .x = i_point.x - cam.pos.x, .y = i_point.y - cam.pos.y };
            float d = sqrt(vec.x * vec.x + vec.y * vec.y);
            if (d < distance && d > flat_dtp) {
                distance = d;
                wall_i = i;
                vec.x = i_point.x - temp_room.walls[i].a.x;
                vec.y = i_point.y - temp_room.walls[i].a.y;
                wall_xdist = sqrt(vec.x * vec.x + vec.y * vec.y);
                vec.x = temp_room.walls[i].b.x - temp_room.walls[i].a.x;
                vec.y = temp_room.walls[i].b.y - temp_room.walls[i].a.y;
                wall_len = sqrt(vec.x * vec.x + vec.y * vec.y);
            }
        }

        int wall_top;
        int wall_bot;
        {
        float wall_above_cam = temp_room.wall_height - cam.height;
        float wall_below_cam = cam.height;
        float swh = wall_above_cam / distance * flat_dtp;
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

        int row_i = 0;
        float row = 0.0;
        for (; row_i < wall_top; ++row_i, ++row) {
            // ceiling
            char c[4];
            FloatVec2 spot;
            // printf("row: %lf\n", fabs(row / vc_height) * vp_height);
            float dv = fabs(row/vc_height - 0.5) * vp_height;
            float spot_len = flat_dtp / dv * (temp_room.wall_height - cam.height);
            spot.x = ray.dir.x * spot_len + cam.pos.x;
            spot.y = ray.dir.y * spot_len + cam.pos.y;
            float ldist = sqrt(spot_len * spot_len + (temp_room.wall_height - cam.height) * (temp_room.wall_height - cam.height));
            float _;
            float samplex = modff(spot.x, &_);
            if (samplex < 0) {
                samplex += 1.0;
            }
            float sampley = modff(spot.y, &_);
            if (sampley < 0) {
                sampley += 1.0;
            }
            FColor c_sample = sprite_fsample(temp_room.ceil_spr, samplex, sampley);
            FColor lc = get_plight_color(cam_light, ldist);
            lc = fcolor_add(lc, global_illum);
            c_sample = fcolor_mul(lc, c_sample);
            // float vert_dtp = sqrt(flat_dtp * flat_dtp + dv * dv);
            float z = zbuffer[row_i * virtual_canvas.width + column_i];
            if (z > 0.0) {
                unsigned char ca[4];
                get_pixel(virtual_canvas, row_i, column_i, ca);
                FColor a = fcolor_from(ca);
                c_sample = fcolor_over(a, c_sample);
            }
            c[0] = (unsigned char)(c_sample.r * 255);
            c[1] = (unsigned char)(c_sample.g * 255);
            c[2] = (unsigned char)(c_sample.b * 255);
            c[3] = (unsigned char)(c_sample.a * 255);
            set_pixel(virtual_canvas, row_i, column_i, c);
        }

        for (; row_i <= wall_bot && row_i < virtual_canvas.height; ++row_i, ++row) {
            // wall
            float row = (float)row_i;
            char c[4];
            float dv = (row / vc_height - 0.5) * vp_height;
            dv = dv / flat_dtp * distance;
            float wall_ydist = temp_room.wall_height - cam.height + dv;
            float ldist = sqrt(distance * distance + dv * dv);
            float _;
            float samplex = wall_xdist / wall_len;
            float sampley = wall_ydist / temp_room.wall_height;
            // float samplex = modff(wall_xdist, &_);
            // float sampley = modff(wall_ydist, &_);
            if (sampley < 0.0) {
                sampley = 0.0;
            }
            FColor c_sample = sprite_fsample(temp_room.walls[wall_i].spr, samplex, sampley);
            FColor lc = get_plight_color(cam_light, ldist);
            lc = fcolor_add(lc, global_illum);
            c_sample = fcolor_mul(lc, c_sample);
            // float vert_dtp = sqrt(flat_dtp * flat_dtp + dv * dv);
            float z = zbuffer[row_i * virtual_canvas.width + column_i];
            if (distance > z && z > 0.0) {
                unsigned char ca[4];
                get_pixel(virtual_canvas, row_i, column_i, ca);
                FColor a = fcolor_from(ca);
                c_sample = fcolor_over(a, c_sample);
            }
            c[0] = (unsigned char)(c_sample.r * 255);
            c[1] = (unsigned char)(c_sample.g * 255);
            c[2] = (unsigned char)(c_sample.b * 255);
            c[3] = (unsigned char)(c_sample.a * 255);
            set_pixel(virtual_canvas, row_i, column_i, c);
        }

        for (; row_i < virtual_canvas.height; ++row_i, ++row) {
            // floor
            float row = (float)row_i;
            char c[4];
            FloatVec2 spot;
            // printf("row: %lf\n", fabs(row / vc_height) * vp_height);
            float dv = fabs(row/vc_height - 0.5) * vp_height;
            float spot_len = flat_dtp / dv * cam.height;
            spot.x = ray.dir.x * spot_len + cam.pos.x;
            spot.y = ray.dir.y * spot_len + cam.pos.y;
            float ldist = sqrt(spot_len * spot_len + cam.height * cam.height);
            float _;
            float samplex = modff(spot.x * 0.25, &_);
            if (samplex < 0) {
                samplex += 1.0;
            }
            float sampley = modff(spot.y * 0.25, &_);
            if (sampley < 0) {
                sampley += 1.0;
            }
            FColor c_sample = sprite_fsample(temp_room.floor_spr, samplex, sampley);
            FColor lc = get_plight_color(cam_light, ldist);
            lc = fcolor_add(lc, global_illum);
            c_sample = fcolor_mul(lc, c_sample);
            // float vert_dtp = sqrt(flat_dtp * flat_dtp + dv * dv);
            float z = zbuffer[row_i * virtual_canvas.width + column_i];
            if (z > 0.0) {
                unsigned char ca[4];
                get_pixel(virtual_canvas, row_i, column_i, ca);
                FColor a = fcolor_from(ca);
                c_sample = fcolor_over(a, c_sample);
            }
            c[0] = (unsigned char)(c_sample.r * 255);
            c[1] = (unsigned char)(c_sample.g * 255);
            c[2] = (unsigned char)(c_sample.b * 255);
            c[3] = (unsigned char)(c_sample.a * 255);
            set_pixel(virtual_canvas, row_i, column_i, c);
        }
    }

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

FloatVec2 cam_pos_add(FloatVec2 v) {
    state_update = true;
    return (FloatVec2) {
        .x = cam.pos.x += v.x,
        .y = cam.pos.y += v.y,
    };
}

float cam_angle_add(float a) {
    state_update = true;
    return cam.angle += a;
}

FColor get_plight_color(PointLight l, float dist) {
    float step = l.r / 6.0;
    float dc = l.r;
    float ri = 0.0;

    for (; dc > 0.0; dc -= step) {
        if (dist <= dc) {
            float r = dc / step;
            ri = 1.0 / (r * r);
        }
    }

    return (FColor) {
        .r = l.c.r * ri,
        .g = l.c.g * ri,
        .b = l.c.b * ri,
        .a = l.c.a * ri,
    };
}

