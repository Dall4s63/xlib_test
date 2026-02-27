/* This file will hold the implementation details for 
 * dealing with the x display server.
 * This should include
 *  - Making a window
 *  - Drawing to the window on command using an image passed to it
 *  - Handling window events, and passing back input events
 */
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "display.h"

Display *display;
int screen_number;
Screen *screen;
Visual *visual;
int default_depth;
Window main_window;
GC main_gc;

KeySym *keysyms;
int keysyms_per_code;
int min_keycodes;

int byte_order;

bool bad_drawable = false;

int error_handler(Display *d, XErrorEvent *e) {
    if (e->error_code == BadDrawable) {
        bad_drawable = true;
        return 0;
    }
    char buffer[200];
    XGetErrorText(d, e->error_code, buffer, 200);
    fprintf(stderr, "\033[1;31mERROR:\033[0m ");
    fprintf(stderr, (char*)buffer);
    fprintf(stderr, "\n");
    return 0;
}

int setup_window(void) {
    XSetErrorHandler(&error_handler);

    display = XOpenDisplay(NULL);
    // TODO error handling
    // if (d == NULL) { return 1; }
    screen_number = DefaultScreen(display);

    screen = DefaultScreenOfDisplay(display);

    visual = DefaultVisualOfScreen(screen);

    default_depth = DefaultDepth(display, screen_number);

    XSetWindowAttributes w_attr;
    w_attr.background_pixel = 0x202020;
    w_attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
    unsigned long w_attr_mask = CWEventMask | CWBackPixel;

    main_window = XCreateWindow(display, screen->root, 0, 0, 600, 400, 0,
        default_depth, InputOutput, CopyFromParent, w_attr_mask, &w_attr);

    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", true);
    Atom wm_state_full = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", true);

    XChangeProperty(display, main_window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_state_full, 1);

    XMapWindow(display, main_window);

    // GC gc = XCreateGC(d, w, 0, NULL);
    main_gc = DefaultGC(display, screen_number);

    byte_order = ImageByteOrder(display);

    int max_keycodes;
    XDisplayKeycodes(display, &min_keycodes, &max_keycodes);

    keysyms = XGetKeyboardMapping(display, min_keycodes, 
        max_keycodes + 1 - min_keycodes, &keysyms_per_code);

    return 0;
}

int draw(Image in) {
    // if (bad_drawable) {
    //     free(in.data);
    //     return 0;
    // }
    if (byte_order == LSBFirst) {
        for (int i = 0; i < 4 * in.width * in.height; i += 4) {
            char temp = in.data[i];
            in.data[i] = in.data[i+2];
            in.data[i+2] = temp;
        }
    }
    XImage *img = XCreateImage(display, visual, default_depth,
        ZPixmap, 0, in.data, in.width, in.height, 32, 0);
    // Pixmap temp = XCreatePixmap(display, main_window, in.width, in.height, default_depth);
    XPutImage(display, main_window, main_gc, img, 0, 0, 0, 0, in.width, in.height);
    XFlush(display);
    XDestroyImage(img);
    return 0;
}

int handle_events(EventCallbacks *callbacks) {
    XSync(display, false);
    while (XPending(display) > 0) {
        XEvent e;
        XNextEvent(display, &e);
        switch (e.type) {
        case Expose: 
            break;

        // ButtonPressMask
        case ButtonPress:
            break;
        // ButtonReleaseMask
        case ButtonRelease:
            break;

        // KeyPressMask
        case KeyPress: 
            {
            int index = (e.xkey.keycode - min_keycodes) * keysyms_per_code;
            KeySym key = keysyms[index];
            }
            // {
            // char buffer[10];
            // int count = XLookupString(&e.xkey, buffer, 10, NULL, NULL);
            // printf("====> char received\n");
            // for (int i = 0; i < count; i++) {
            //     printf("\t%c\n", buffer[i]);
            // }}
            break;

        // KeyReleaseMask
        case KeyRelease:
            break;

        // StructureNotifyMask
        case CirculateNotify:
            break;
        case ConfigureNotify: {
            int width = e.xconfigure.width;
            int height = e.xconfigure.height;
            (*callbacks->window_resized)(width, height);
            // printf("window width: %d, height: %d\n", width, height);
            break; }
        case DestroyNotify:
            // printf("window destroyed\n");
            (*callbacks->window_destroyed)();
            break;
        case GravityNotify:
            break;
        case MapNotify:
            break;
        case ReparentNotify:
            break;
        case UnmapNotify:
            break;

        default:
            printf("Unhandled event id: %d\n", e.type);
        }
    }
    return 0;
}

