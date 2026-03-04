#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

#include "display.h"
#include "tactics_render.h"

int win_width = 600;
int win_height = 400;

int fps = 30;

Image test_image = {0, 0, 0};

void temp_img(void) {
    test_image.width = win_width;
    test_image.height = win_height;
    test_image.data = malloc(sizeof(char) * 4 * test_image.width * test_image.height);
    if (test_image.data == NULL) { printf("Helpppp\n"); }
    memset(test_image.data, 0, 4 * test_image.width * test_image.height);
    // for (int i = 0; i < 4 * test_image.width * test_image.height; i += 4) {
    //     int row = i / 4 / test_image.width;
    //     int col = (i / 4) % test_image.width;
    //     test_image.data[i] = 0xff * row / test_image.height;
    //     test_image.data[i+1] = 0xff * col / test_image.width;
    //     test_image.data[i+2] = 0x80;
    //     test_image.data[i+3] = 0;
    // }
}

Image temp_img_gen(void) {
    Image new_image;
    new_image.width = 30;
    new_image.height = 30;
    new_image.data = malloc(sizeof(char) * 4 * new_image.width * new_image.height);
    if (new_image.data == NULL) { printf("Helpppp\n"); }
    for (int i = 0; i < 4 * new_image.width * new_image.height; i += 4) {
        int row = i / 4 / new_image.width;
        int col = (i / 4) % new_image.width;
        new_image.data[i] = 0xff * row / new_image.height;
        new_image.data[i+1] = 0xff * col / new_image.width;
        new_image.data[i+2] = 0x80;
        new_image.data[i+3] = 0;
    }
    return new_image;
}

bool window_closed = false;

void window_destroyed(void) {
    // printf("window_destroyed callback called\n");
    window_closed = true;
}

void window_resized(int width, int height) {
    win_width = width;
    win_height = height;
    free(test_image.data);
    test_image.data = NULL;
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
    setup_window(800, 600);
    render_setup(200, 150);

    Image temp = temp_img_gen();
    DrawableId temp_id = new_sprite(20, 20, temp.width, temp.height, 0, temp.data);

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

        SpriteInfo s_info;

        sprite_get(temp_id, &s_info);

        s_info.x = (s_info.x + 1) % 100;

        sprite_set(temp_id, s_info, SPRITE_X);

        if (test_image.data == NULL) {
            temp_img();
        }
        render_run(test_image);
        draw(test_image);
        handle_events(&callbacks);
        clock_gettime(CLOCK_REALTIME, &end);
        diff = timespec_sub(end, start);
        rem = timespec_sub(frame_time, diff);
        nanosleep(&rem, NULL);
    }

    return 0;
}
