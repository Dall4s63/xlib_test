/* This file should have the generic definitions for 
 * opening a window, drawing to the window, and 
 * callback functions for event handling.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

typedef struct _image {
    char *data;
    int width;
    int height;
} Image;

int setup_window(void);

int draw(Image in);

#endif
