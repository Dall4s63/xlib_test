#ifndef RENDER2D_H
#define RENDER2D_H

/* This file should contain the types and functions that 
 * allow the creation of drawable objects and then 
 * rendering those images into one image for drawing.
 *
 * For now it would be nice to support drawing 
 *  - images
 *  - rectangles
 *  - generic polygons?
 *  - circles
 *  - text (eventually)
 */

#include <types.h>

typedef union _rgb_color {
    uint32_t val;
    char     rgb[4]
} RGBColor;

// I think it makes sense to store a line using x,y as its position
// with x1,y1 and x2,y2 relative to x,y.
typedef struct _line_drawable {
    double x;
    double y;
    double x1;
    double y1;
    double x2;
    double y2;
    int type;
    RGBColor set_color;
    RGBColor pc_color;
    int blend mode;
    bool visible;
} LineDrawable;

typedef union _drawable {
    int type;
    LineDrawable line;
    long pad[8]; // TODO fix this 
} Drawable;

int setup(int v_width, int v_height);

int render(Image canvas);

#endif
