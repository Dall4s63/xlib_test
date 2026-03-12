#ifndef DOUBLE_VEC2
#define DOUBLE_VEC2

typedef struct _double_vec2 {
    double x;
    double y;
} DoubleVec2;

typedef struct _double_line {
    double a;
    double b;
    double c;
} DoubleLine;

typedef struct _double_ray {
    DoubleVec2 origin;
    DoubleVec2 dir;
} DoubleRay;

double dot_product(DoubleVec2 a, DoubleVec2 b);
DoubleVec2 scalar_product(DoubleVec2 a, double x);
DoubleVec2 vec_rotate(DoubleVec2 a, double angle);
bool double_ray_in_segment(DoubleRay ray, DoubleVec2 a, DoubleVec2 b, DoubleVec2 *ret);
bool double_line_in_segment(DoubleLine l1, DoubleVec2 a, DoubleVec2 b, DoubleVec2 *ret);
bool double_line_intersect(DoubleLine l1, DoubleLine l2, DoubleVec2 *ret);
DoubleLine double_line_from(DoubleVec2 a, DoubleVec2 b);

#endif
