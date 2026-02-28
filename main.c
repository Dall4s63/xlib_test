#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <time.h>

#include "display.h"

int win_width = 600;
int win_height = 400;

int fps = 60;

Image temp_img(void) {
    Image out;
    out.width = win_width;
    out.height = win_height;
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
    // printf("window_destroyed callback called\n");
    window_closed = true;
}

void window_resized(int width, int height) {
    win_width = width;
    win_height = height;
}

void key_press(KeyId key, unsigned int scancode) {
    char *key_name = keyidstr(key);
    printf("Received key: %s\n", key_name);
}

struct timespec timespec_sub(struct timespec a, struct timespec b) {
    time_t sec_diff = a.tv_sec - b.tv_sec;
    long nsec_diff = a.tv_nsec - b.tv_nsec;
    if (nsec_diff < 0) {
        sec_diff -= 1;
        nsec_diff += 1000000000;
    }
    return (struct timespec){sec_diff, nsec_diff};
}

int main(void) {

    EventCallbacks callbacks;
    callbacks.window_destroyed = &window_destroyed;
    callbacks.window_resized = &window_resized;
    callbacks.key_press = &key_press;
    setup_window(600, 400);

    // clock_gettime(CLOCK_REALTIME, &time);
    // nanosleep(&time, NULL);
    struct timespec start;
    struct timespec end;
    struct timespec diff;
    struct timespec rem;
    struct timespec frame_time;
    frame_time.tv_sec = 0;
    frame_time.tv_nsec = 1000000000 / fps;
    double dt = (double)frame_time.tv_nsec / 1.0e10;
    while (!window_closed) {
        clock_gettime(CLOCK_REALTIME, &start);
        handle_events(&callbacks);
        draw(temp_img());
        handle_events(&callbacks);
        clock_gettime(CLOCK_REALTIME, &end);
        diff = timespec_sub(end, start);
        rem = timespec_sub(frame_time, diff);
        nanosleep(&rem, NULL);
    }

    return 0;
}
