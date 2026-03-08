#include "double_vec2.h"

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
