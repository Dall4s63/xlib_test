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

// I think it makes sense to store a line using x,y as its position
// with x1,y1 and x2,y2 relative to x,y.
typedef struct _line_drawable {
    int type;
    int x;
    int y;
    int x1;
    int y1;
    int x2;
    int y2;
} LineDrawable;

typedef union _drawable {
    int type;
    long pad[10]; // TODO fix this 
} Drawable;

#endif
