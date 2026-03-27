#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "double_vec2.h"

double dot_product(DoubleVec2 a, DoubleVec2 b) {
    return a.x * b.x + a.y * b.y;
}

double vec_mag(DoubleVec2 a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

DoubleVec2 vec_add(DoubleVec2 a, DoubleVec2 b) {
    return (DoubleVec2) { .x = a.x + b.x, .y = a.y + b.y };
}

DoubleVec2 vec_sub(DoubleVec2 a, DoubleVec2 b) {
    return (DoubleVec2) { .x = a.x - b.x, .y = a.y - b.y };
}

DoubleVec2 vec_scalar_product(DoubleVec2 a, double x) {
    return (DoubleVec2) { .x = a.x * x, .y = a.y * x };
}

DoubleVec2 vec_normalize(DoubleVec2 a) {
    double length = vec_mag(a);
    return (DoubleVec2) { .x = a.x / length, .y = a.y / length };
}

double vec_angle(DoubleVec2 a, DoubleVec2 b) {
    a = vec_normalize(a);
    b = vec_normalize(b);
    return acos(dot_product(a, b));
}

DoubleVec2 vec_rotate(DoubleVec2 a, double angle) {
    return (DoubleVec2) { 
        .x = a.x * cos(angle) - a.y * sin(angle), 
        .y = a.x * sin(angle) + a.y * cos(angle) 
    };
}

static bool is_almost(double a, double b) {
    return b-1e-12 <= a && a <= b+1e-12;
}

bool double_ray_intersect(DoubleRay r1, DoubleRay r2, DoubleVec2 *ret) {
    DoubleVec2 a = r1.dir;
    assert(!is_almost(a.x, 0.0) || !is_almost(a.y, 0.0));
    DoubleVec2 b = r1.origin;

    DoubleVec2 c = r2.dir;
    assert(!is_almost(c.x, 0.0) || !is_almost(c.y, 0.0));
    DoubleVec2 d = r2.origin;

    if (is_almost(a.x, 0.0) && is_almost(c.x, 0.0)) {
        return false;
    }

    if (is_almost(a.y, 0.0) && is_almost(c.y, 0.0)) {
        return false;
    }

    double lambda1 = -1.0;
    double lambda2 = -1.0;
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
        double temp1 = d.y - b.y - a.y / a.x * (d.x - b.x);
        double temp2 = a.y * c.x / a.x - c.y;
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

bool double_ray_in_segment(DoubleRay ray, DoubleVec2 point1, DoubleVec2 point2, DoubleVec2 *ret) {
    DoubleVec2 a = ray.dir;
    assert(!is_almost(a.x, 0.0) || !is_almost(a.y, 0.0));
    DoubleVec2 b = ray.origin;

    DoubleVec2 c;
    c.x = point2.x - point1.x;
    c.y = point2.y - point1.y;
    assert(!is_almost(c.x, 0.0) || !is_almost(c.y, 0.0));
    DoubleVec2 d = point1;

    if (is_almost(a.x, 0.0) && is_almost(c.x, 0.0)) {
        return false;
    }

    if (is_almost(a.y, 0.0) && is_almost(c.y, 0.0)) {
        return false;
    }

    double lambda1 = -1.0;
    double lambda2 = -1.0;
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
        double temp1 = d.y - b.y - a.y / a.x * (d.x - b.x);
        double temp2 = a.y * c.x / a.x - c.y;
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

bool double_line_in_segment(DoubleLine l1, DoubleVec2 a, DoubleVec2 b, DoubleVec2 *ret) {
    DoubleLine other_line = double_line_from(a, b);
    bool res = double_line_intersect(l1, other_line, ret);
    if (!res) {
        return false;
    }
    double x1, x2, y1, y2;
    if (a.x < b.x) { x1 = a.x; x2 = b.x; } else { x1 = b.x; x2 = a.x; }
    if (a.y < b.y) { y1 = a.y; y2 = b.y; } else { y1 = b.y; y2 = a.y; }
    if (ret->x < x1 || ret->x > x2 || ret->y < y1 || ret->y > y2) {
        return false;
    }
    return true;
}

bool double_line_intersect(DoubleLine l1, DoubleLine l2, DoubleVec2 *ret) {
    double mul1 = l2.a / l1.a;
    double b2 = l2.b - l1.b * mul1;
    if (b2 <= 1e-12 && b2 >= -1e-12) {
        return false;
    }
    double c2 = -l2.c - (-l1.c * mul1);
    double y_val = c2 / b2;
    double x_val = (-l1.c - y_val * l1.b) / l1.a;
    ret->x = x_val;
    ret->y = y_val;
    return true;
}

DoubleLine double_line_from(DoubleVec2 a, DoubleVec2 b) {
    DoubleLine out;
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    out.a = dy;
    out.b = -dx;
    out.c = a.y * dx - a.x * dy;
    return out;
}
