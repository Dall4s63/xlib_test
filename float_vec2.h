#ifndef FLOAT_VEC2
#define FLOAT_VEC2

typedef struct _float_vec2 {
    float x;
    float y;
} FloatVec2;

typedef struct _float_line {
    float a;
    float b;
    float c;
} FloatLine;

typedef struct _float_ray {
    FloatVec2 origin;
    FloatVec2 dir;
} FloatRay;

float dot_product(FloatVec2 a, FloatVec2 b);
float vec_mag(FloatVec2 a);
FloatVec2 vec_add(FloatVec2 a, FloatVec2 b);
FloatVec2 vec_sub(FloatVec2 a, FloatVec2 b);
FloatVec2 vec_scalar_product(FloatVec2 a, float x);
FloatVec2 vec_normalize(FloatVec2 a);
float vec_angle(FloatVec2 a, FloatVec2 b);
FloatVec2 vec_rotate(FloatVec2 a, float angle);
bool float_ray_intersect(FloatRay r1, FloatRay r2, FloatVec2 *ret);
bool float_ray_in_segment(FloatRay ray, FloatVec2 a, FloatVec2 b, FloatVec2 *ret);
bool float_line_in_segment(FloatLine l1, FloatVec2 a, FloatVec2 b, FloatVec2 *ret);
bool float_line_intersect(FloatLine l1, FloatLine l2, FloatVec2 *ret);
FloatLine float_line_from(FloatVec2 a, FloatVec2 b);

#endif
