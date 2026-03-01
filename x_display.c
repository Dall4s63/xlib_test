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

#define KB_MY_DEFAULTS
#include "keyboard.h"

static Display *display;
static int screen_number;
static Screen *screen;
static Visual *visual;
static int default_depth;
static Window main_window;
static GC main_gc;

static KeySym *keysyms;
static int keysyms_per_code;
static int min_keycodes;

static int byte_order;

static bool bad_drawable = false;

static XImage *cur_img = NULL;

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

int setup_image(int width, int height) {
    char *new_data = malloc(sizeof(char) * 4 * width * height);
    if (new_data == NULL) { return 1; }
    cur_img = XCreateImage(display, visual, default_depth, ZPixmap,
        0, new_data, width, height, 32, 0);
    return 0;
}

int setup_window(int width, int height) {
    XSetErrorHandler(&error_handler);

    display = XOpenDisplay(NULL);
    screen_number = DefaultScreen(display);

    screen = DefaultScreenOfDisplay(display);

    visual = DefaultVisualOfScreen(screen);

    default_depth = DefaultDepth(display, screen_number);

    XSetWindowAttributes w_attr;
    w_attr.background_pixel = 0x202020;
    w_attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
    unsigned long w_attr_mask = CWEventMask | CWBackPixel;

    main_window = XCreateWindow(display, screen->root, 0, 0, width, height, 0,
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

    // XImage *cur_img = XCreateImage(display, visual, default_depth,
    //     ZPixmap, 0, in.data, in.width, in.height, 32, 0);

    return 0;
}

int draw(Image in) {
    if (cur_img == NULL) {
        setup_image(in.width, in.height);
    }
    if ((in.width != cur_img->width) || (in.height != cur_img->height)) {
        XDestroyImage(cur_img);
        setup_image(in.width, in.height);
    }
    if (byte_order == LSBFirst) {
        for (int i = 0; i < 4 * in.width * in.height; i += 4) {
            cur_img->data[i] = in.data[i+2];
            cur_img->data[i+1] = in.data[i+1];
            cur_img->data[i+2] = in.data[i];
            cur_img->data[i+3] = 0;
        }
    } else {
        for (int i = 0; i < 4 * in.width * in.height; i += 4) {
            cur_img->data[i] = in.data[i];
            cur_img->data[i+1] = in.data[i+1];
            cur_img->data[i+2] = in.data[i+2];
            cur_img->data[i+3] = 0;
        }
    }

    // XImage *img = XCreateImage(display, visual, default_depth,
    //     ZPixmap, 0, in.data, in.width, in.height, 32, 0);
    // Pixmap temp = XCreatePixmap(display, main_window, in.width, in.height, default_depth);
    XPutImage(display, main_window, main_gc, cur_img, 0, 0, 0, 0, in.width, in.height);
    XFlush(display);
    return 0;
}

KeyId keysym_to_keyid(KeySym key) {
    switch (key) {
    case XK_Return: return KB_Return;
    case XK_Escape: return KB_Escape;
    case XK_Left: return KB_Left;
    case XK_Up: return KB_Up;
    case XK_Right: return KB_Right;
    case XK_Down: return KB_Down;
    case XK_a: case XK_A: return KB_A;
    case XK_b: case XK_B: return KB_B;
    case XK_c: case XK_C: return KB_C;
    case XK_d: case XK_D: return KB_D;
    case XK_e: case XK_E: return KB_E;
    case XK_f: case XK_F: return KB_F;
    case XK_g: case XK_G: return KB_G;
    case XK_h: case XK_H: return KB_H;
    case XK_i: case XK_I: return KB_I;
    case XK_j: case XK_J: return KB_J;
    case XK_k: case XK_K: return KB_K;
    case XK_l: case XK_L: return KB_L;
    case XK_m: case XK_M: return KB_M;
    case XK_n: case XK_N: return KB_N;
    case XK_o: case XK_O: return KB_O;
    case XK_p: case XK_P: return KB_P;
    case XK_q: case XK_Q: return KB_Q;
    case XK_r: case XK_R: return KB_R;
    case XK_s: case XK_S: return KB_S;
    case XK_t: case XK_T: return KB_T;
    case XK_u: case XK_U: return KB_U;
    case XK_v: case XK_V: return KB_V;
    case XK_w: case XK_W: return KB_W;
    case XK_x: case XK_X: return KB_X;
    case XK_y: case XK_Y: return KB_Y;
    case XK_z: case XK_Z: return KB_Z;
    default: return KB_NoSymbol;
    }
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
            KeyId id = keysym_to_keyid(key);
            (*callbacks->key_press)(id, e.xkey.keycode);
            }
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

