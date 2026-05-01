#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "float_vec2.h"

float dot_product(FloatVec2 a, FloatVec2 b) {
    return a.x * b.x + a.y * b.y;
}

float vec_mag(FloatVec2 a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

FloatVec2 vec_add(FloatVec2 a, FloatVec2 b) {
    return (FloatVec2) { .x = a.x + b.x, .y = a.y + b.y };
}

FloatVec2 vec_sub(FloatVec2 a, FloatVec2 b) {
    return (FloatVec2) { .x = a.x - b.x, .y = a.y - b.y };
}

FloatVec2 vec_scalar_product(FloatVec2 a, float x) {
    return (FloatVec2) { .x = a.x * x, .y = a.y * x };
}

FloatVec2 vec_normalize(FloatVec2 a) {
    float length = vec_mag(a);
    return (FloatVec2) { .x = a.x / length, .y = a.y / length };
}

float vec_angle(FloatVec2 a, FloatVec2 b) {
    a = vec_normalize(a);
    b = vec_normalize(b);
    return acos(dot_product(a, b));
}

FloatVec2 vec_rotate(FloatVec2 a, float angle) {
    return (FloatVec2) { 
        .x = a.x * cos(angle) - a.y * sin(angle), 
        .y = a.x * sin(angle) + a.y * cos(angle) 
    };
}

static bool is_almost(float a, float b) {
    return b-1e-12 <= a && a <= b+1e-12;
}

bool float_ray_intersect(FloatRay r1, FloatRay r2, FloatVec2 *ret) {
    FloatVec2 a = r1.dir;
    assert(!is_almost(a.x, 0.0) || !is_almost(a.y, 0.0));
    FloatVec2 b = r1.origin;

    FloatVec2 c = r2.dir;
    assert(!is_almost(c.x, 0.0) || !is_almost(c.y, 0.0));
    FloatVec2 d = r2.origin;

    if (is_almost(a.x, 0.0) && is_almost(c.x, 0.0)) {
        return false;
    }

    if (is_almost(a.y, 0.0) && is_almost(c.y, 0.0)) {
        return false;
    }

    float lambda1 = -1.0;
    float lambda2 = -1.0;
    if (is_almost(a.x, 0.0)) {
        // printf("running a.x is zero\n");
        lambda2 = (b.x - d.x) / c.x;
        lambda1 = (d.y - b.y + lambda2*c.y) / a.y;
        // printf("lambda1: %lf, lambda2: %lf\n", lambda1, lambda2);
    } else if (is_almost(a.y, 0.0)) {
        // printf("running a.y is zero\n");
        lambda2 = (b.y - d.y) / c.y;
        lambda1 = (d.x - b.x + lambda2*c.x) / a.x;
    } else {
        float temp1 = d.y - b.y - a.y / a.x * (d.x - b.x);
        float temp2 = a.y * c.x / a.x - c.y;
        if (is_almost(temp2, 0.0)) {
            return false;
        }

        lambda2 = temp1 / temp2;
        lambda1 = (d.x - b.x + c.x * lambda2) / a.x;
    }

    ret->x = r1.origin.x + r1.dir.x * lambda1;
    ret->y = r1.origin.y + r1.dir.y * lambda1;

    return true;
}

bool float_ray_in_segment(FloatRay ray, FloatVec2 point1, FloatVec2 point2, FloatVec2 *ret) {
    FloatVec2 a = ray.dir;
    assert(!is_almost(a.x, 0.0) || !is_almost(a.y, 0.0));
    FloatVec2 b = ray.origin;

    FloatVec2 c;
    c.x = point2.x - point1.x;
    c.y = point2.y - point1.y;
    assert(!is_almost(c.x, 0.0) || !is_almost(c.y, 0.0));
    FloatVec2 d = point1;

    if (is_almost(a.x, 0.0) && is_almost(c.x, 0.0)) {
        return false;
    }

    if (is_almost(a.y, 0.0) && is_almost(c.y, 0.0)) {
        return false;
    }

    float lambda1 = -1.0;
    float lambda2 = -1.0;
    if (is_almost(a.x, 0.0)) {
        // printf("running a.x is zero\n");
        lambda2 = (b.x - d.x) / c.x;
        lambda1 = (d.y - b.y + lambda2*c.y) / a.y;
        // printf("lambda1: %lf, lambda2: %lf\n", lambda1, lambda2);
    } else if (is_almost(a.y, 0.0)) {
        // printf("running a.y is zero\n");
        lambda2 = (b.y - d.y) / c.y;
        lambda1 = (d.x - b.x + lambda2*c.x) / a.x;
    } else {
        float temp1 = d.y - b.y - a.y / a.x * (d.x - b.x);
        float temp2 = a.y * c.x / a.x - c.y;
        if (is_almost(temp2, 0.0)) {
            return false;
        }

        lambda2 = temp1 / temp2;
        lambda1 = (d.x - b.x + c.x * lambda2) / a.x;
    }

    if (lambda1 < 0) {
        return false;
    }

    if (lambda2 < 0 || lambda2 > 1) {
        return false;
    }

    ret->x = ray.origin.x + ray.dir.x * lambda1;
    ret->y = ray.origin.y + ray.dir.y * lambda1;

    return true;
}

bool float_line_in_segment(FloatLine l1, FloatVec2 a, FloatVec2 b, FloatVec2 *ret) {
    FloatLine other_line = float_line_from(a, b);
    bool res = float_line_intersect(l1, other_line, ret);
    if (!res) {
        return false;
    }
    float x1, x2, y1, y2;
    if (a.x < b.x) { x1 = a.x; x2 = b.x; } else { x1 = b.x; x2 = a.x; }
    if (a.y < b.y) { y1 = a.y; y2 = b.y; } else { y1 = b.y; y2 = a.y; }
    if (ret->x < x1 || ret->x > x2 || ret->y < y1 || ret->y > y2) {
        return false;
    }
    return true;
}

bool float_line_intersect(FloatLine l1, FloatLine l2, FloatVec2 *ret) {
    float mul1 = l2.a / l1.a;
    float b2 = l2.b - l1.b * mul1;
    if (b2 <= 1e-12 && b2 >= -1e-12) {
        return false;
    }
    float c2 = -l2.c - (-l1.c * mul1);
    float y_val = c2 / b2;
    float x_val = (-l1.c - y_val * l1.b) / l1.a;
    ret->x = x_val;
    ret->y = y_val;
    return true;
}

FloatLine float_line_from(FloatVec2 a, FloatVec2 b) {
    FloatLine out;
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    out.a = dy;
    out.b = -dx;
    out.c = a.y * dx - a.x * dy;
    return out;
}
