#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include <sys/time.h>

// #include <X11/Xlib.h>
// #include <X11/Xatom.h>
// #include <X11/Xutil.h>
// #include <X11/Xkeysym.h>
// #include <X11/Xcms.h>

#include "display.h"

Image temp_img(void) {
    Image out;
    out.width = 600;
    out.height = 400;
    out.data = malloc(sizeof(char) * 4 * out.width * out.height);
    if (out.data == NULL) { printf("Helpppp\n"); }
    for (int i = 0; i < 4 * out.width * out.height; i += 4) {
        int row = i / 4 / out.width;
        int col = (i / 4) % out.width;
        out.data[i] = 0xff * row / out.height;
        out.data[i+1] = 0xff * col / out.width;
        out.data[i+2] = 0x80;
        out.data[i+3] = 0;
    }
    return out;
}

bool window_closed = false;

void window_destroyed(void) {
    printf("window_destroyed callback called\n");
    window_closed = true;
}

int main(void) {
    EventCallbacks callbacks;
    callbacks.window_destroyed = &window_destroyed;
    setup_window();
    
    while (!window_closed) {
        handle_events(&callbacks);
        draw(temp_img());
        handle_events(&callbacks);
    }

    return 0;
}
