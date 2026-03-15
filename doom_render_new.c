#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "double_vec2.h"
#include "doom_render.h"

static Image virtual_canvas;

static double viewport_width = 0.5;

static Camera cam = (Camera) { 
    (DoubleVec2) { .x = 0.0, .y = 0.0 },
    0.0,
    90.0,
    1.8
};

static Map temp_map;

Image debug_canvas;

int render_setup(int v_width, int v_height) {
}
