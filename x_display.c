/* This file will hold the implementation details for 
 * dealing with the x display server.
 * This should include
 *  - Making a window
 *  - Drawing to the window on command using an image passed to it
 *  - Handling window events, and passing back input events
 */

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "display.h"

Display *display;
int screen_number;
Screen *screen;
Visual *visual;
Window main_window;
GC main_gc;

// TODO probably should have some kind of generic configuration input
int setup_window(void) {
    display = XOpenDisplay(NULL);
    // TODO error handling
    // if (d == NULL) { return 1; }
    screen_number = DefaultScreen(d);

    screen = DefaultScreenOfDisplay(d);

    visual = DefaultVisualOfScreen(s);

    XSetWindowAttributes w_attr;
    w_attr.background_pixel = 0x202020;
    w_attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
    unsigned long w_attr_mask = CWEventMask | CWBackPixel;

    main_window = XCreateWindow(d, s->root, 0, 0, 600, 400, 0,
        CopyFromParent, InputOutput, CopyFromParent, w_attr_mask, &w_attr);

    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", true);
    Atom wm_state_full = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", true);

    XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_state_full, 1);

    XMapWindow(display, window);

    // GC gc = XCreateGC(d, w, 0, NULL);
    main_gc = DefaultGC(display, screen_number);

    return 0;
}



