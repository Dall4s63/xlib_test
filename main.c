#include <stdio.h>

#include <X11/Xlib.h>

int main(void) 
{
    Display *d = XOpenDisplay(NULL);
    if (d == NULL) { return 1; }
    int s_no = DefaultScreen(d);
    // printf("default screen: %d\n", DefaultScreen(d));

    printf("screen width: %u\n", DisplayWidth(d, s_no));

    Screen *s = DefaultScreenOfDisplay(d);
    return 0;
}
