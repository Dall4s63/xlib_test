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

typedef struct _event_callbacks {
    void (*button_press)(int b, int x, int y);
    void (*button_release)(int b, int x, int y);
} EventCallbacks;

int setup_window(void);

int draw(Image in);

int handle_events(EventCallbacks ecbs);

#endif
