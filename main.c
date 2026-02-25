#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
// #include <X11/Xcms.h>

int main(void) 
{
    Display *d = XOpenDisplay(NULL);
    if (d == NULL) { return 1; }
    int s_no = DefaultScreen(d);
    // printf("default screen: %d\n", DefaultScreen(d));

    // int pmfv_count;
    // XPixmapFormatValues *pmfv = XListPixmapFormats(d, &pmfv_count);
    // for (int i = 0; i < pmfv_count; i++) {
    //     printf("value: %d\n", i);
    //     printf("    depth: \t\t%d\n", pmfv[i].depth);
    //     printf("    bits per pixel: \t%d\n", pmfv[i].bits_per_pixel);
    //     printf("    scanline pad: \t%d\n", pmfv[i].scanline_pad);
    // }


    printf("screen width: %u\n", DisplayWidth(d, s_no));

    Screen *s = DefaultScreenOfDisplay(d);

    Visual *v = DefaultVisualOfScreen(s);
    printf("Visual Id: %d\n", TrueColor);
    // VisualID vid = XVisualIDFromVisual(v);

    XSetWindowAttributes w_attr;
    w_attr.background_pixel = 0x202020;
    w_attr.event_mask = StructureNotifyMask | ExposureMask;
    unsigned long w_attr_mask = CWEventMask | CWBackPixel;

    Window w = XCreateWindow(d, s->root, 0, 0, 600, 400, 0,
        CopyFromParent, InputOutput, CopyFromParent, w_attr_mask, &w_attr);

    Atom wm_state = XInternAtom(d, "_NET_WM_STATE", true);
    Atom wm_state_full = XInternAtom(d, "_NET_WM_STATE_FULLSCREEN", true);

    XChangeProperty(d, w, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_state_full, 1);

    // Atom ret_type;
    // int ret_format;
    // unsigned long ret_nitems;
    // unsigned long bytes_after;
    // unsigned char *values;
    // int res = XGetWindowProperty(d, w, net_state, 0, 5, false, AnyPropertyType, &ret_type, &ret_format, &ret_nitems,
    //     &bytes_after, &values);

    XMapWindow(d, w);

    // GC gc = XCreateGC(d, w, 0, NULL);
    GC gc = DefaultGC(d, XScreenNumberOfScreen(s));

    XClearArea(d, w, 0, 0, 600, 200, false);
    XDrawLine(d, w, gc, 100, 100, 200, 200);

    // printf("white pixel %lx\n", WhitePixel(d, 0));
    // XSetForeground(d, gc, WhitePixel(d, 0));
    XSetForeground(d, gc, 0xff0000);

    // XWindowAttributes w_attr;
    // XGetWindowAttributes(d, w, &w_attr);
    // printf("window width: %d, height: %d\n", w_attr.width, w_attr.height);

    // XFlush(d);
    
    // printf("byte order: %d\n", ImageByteOrder(d));
    // printf("LSB: %d\n", LSBFirst);
    // printf("MSB: %d\n", MSBFirst);

    int img_w = 30;
    int img_h = 30;
    unsigned char img_data[4*img_w*img_h];
    for (int i = 0; i < 4 * img_w * img_h; i += 4) {
        img_data[i] = 0xff / img_h * i / 4 / img_h;
        img_data[i+1] = 0xff / img_w * (i / 4 % img_w);
        img_data[i+2] = 0xff;
        img_data[i+3] = 0;
    }

    XImage *img = XCreateImage(d, DefaultVisual(d, 0), DefaultDepth(d, 0), ZPixmap, 0, (char*)img_data, img_w, img_h, 32, 0);

    bool window_destroyed = false;

    while (!window_destroyed) {
        XEvent e;
        XNextEvent(d, &e);
        switch (e.type) {
        // ExposureMask
        case Expose:
            XPutImage(d, w, gc, img, 0, 0, 100, 100, img_w, img_h);
            break;

        // StructureNotifyMask
        case CirculateNotify:
            break;
        case ConfigureNotify:
            XWindowAttributes w_attr;
            XGetWindowAttributes(d, w, &w_attr);
            printf("window width: %d, height: %d\n", w_attr.width, w_attr.height);
            break;
        case DestroyNotify:
            window_destroyed = true;
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

    // char c = getchar();
    // while (c != 'q') {

    //     XClearArea(d, w, 0, 0, 600, 200, false);
    //     XDrawLine(d, w, gc, 50, 50, 200, 50);
    //     XPutImage(d, w, gc, img, 0, 0, 100, 100, img_w, img_h);
    //     printf("Events pending: %d\n", XPending(d));
    //     c = getchar();
    // }

    return 0;
}
