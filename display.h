/* This file should have the generic definitions for 
 * opening a window, drawing to the window, and 
 * callback functions for event handling.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "keyboard.h"

typedef struct _image {
    char *data;
    int width;
    int height;
} Image;

typedef struct _event_callbacks {
    void (*button_press)(int button, int x, int y);
    void (*button_release)(int button, int x, int y);

    void (*key_press)(KeyId key, unsigned int scancode);
    void (*key_release)(KeyId key, unsigned int scancode);

    void (*window_resized)(int width, int height);
    void (*window_destroyed)(void);
} EventCallbacks;

int setup_window(void);

int draw(Image in);

int handle_events(EventCallbacks *callbacks);

#endif
