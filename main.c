#include <stdio.h>

#include <X11/Xlib.h>

int main(void) 
{
    Display *d = XOpenDisplay(NULL);
    if (d == NULL) { return 1; }
    int s_no = DefaultScreen(d);
    // printf("default screen: %d\n", DefaultScreen(d));

    int pmfv_count;
    XPixmapFormatValues *pmfv = XListPixmapFormats(d, &pmfv_count);
    for (int i = 0; i < pmfv_count; i++) {
        printf("value: %d\n", i);
        printf("    depth: \t\t%d\n", pmfv[i].depth);
        printf("    bits per pixel: \t%d\n", pmfv[i].bits_per_pixel);
        printf("    scanline pad: \t%d\n", pmfv[i].scanline_pad);
    }

    printf("screen width: %u\n", DisplayWidth(d, s_no));

    Screen *s = DefaultScreenOfDisplay(d);
    return 0;
}
