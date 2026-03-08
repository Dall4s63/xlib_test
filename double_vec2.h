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

bool double_line_intersect(DoubleLine l1, DoubleLine l2, DoubleVec2 *ret);
DoubleLine double_line_from(DoubleVec2 a, DoubleVec2 b);

#endif
