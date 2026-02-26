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
int default_depth;
Window main_window;
GC main_gc;

int byte_order;

int setup_window(void) {
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

    return 0;
}

int draw(Image in) {
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


//     while (!window_destroyed) {
//         XEvent e;
//         XNextEvent(d, &e);
//         switch (e.type) {
//         // ExposureMask
//         case Expose:
//             {
//             int count = e.xexpose.count;
//             if (count == 0) {
//                 XPutImage(d, w, gc, img, 0, 0, 100, 100, img_w, img_h);
//             }
//             }
//             break;
// 
//         // KeyPressMask
//         case KeyPress:
//             {
//             char buffer[10];
//             int count = XLookupString(&e.xkey, buffer, 10, NULL, NULL);
//             printf("====> char recieved\n");
//             for (int i = 0; i < count; i++) {
//                 printf("\t%c\n", buffer[i]);
//             }
//             }
//             break;
// 
//         // StructureNotifyMask
//         case CirculateNotify:
//             break;
//         case ConfigureNotify:
//             // XWindowAttributes w_attr;
//             // XGetWindowAttributes(d, w, &w_attr);
//             int width = e.xconfigure.width;
//             int height = e.xconfigure.height;
//             printf("window width: %d, height: %d\n", width, height);
//             break;
//         case DestroyNotify:
//             window_destroyed = true;
//             break;
//         case GravityNotify:
//             break;
//         case MapNotify:
//             break;
//         case ReparentNotify:
//             break;
//         case UnmapNotify:
//             break;
// 
//         default:
//             printf("Unhandled event id: %d\n", e.type);
//         }
//     }

