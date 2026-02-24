#include <stdio.h>

#include <X11/Xlib.h>

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
    VisualID vid = XVisualIDFromVisual(v);

    XSetWindowAttributes w_attr;
    w_attr.event_mask = StructureNotifyMask;

    Window w = XCreateWindow(d, s->root, 0, 0, 600, 400, 0,
        CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &w_attr);

    XMapWindow(d, w);

    // XWindowAttributes w_attr;
    // XGetWindowAttributes(d, w, &w_attr);
    // printf("window width: %d, height: %d\n", w_attr.width, w_attr.height);

    // XFlush(d);
    
    printf("Events pending: %d\n", XPending(d));
    
    getchar();

    return 0;
}
