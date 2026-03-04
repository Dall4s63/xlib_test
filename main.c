#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

#include "display.h"
#include "tactics_render.h"
#include "game.h"

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
}

bool window_closed = false;

void engine_window_destroyed(void) {
    // printf("window_destroyed callback called\n");
    window_closed = true;
    window_destroyed();
}

void engine_window_resized(int width, int height) {
    win_width = width;
    win_height = height;
    free(test_image.data);
    test_image.data = NULL;
}

void engine_key_press(KeyId key, unsigned int scancode) {
    char *key_name = keyidstr(key);
    printf("Received key: %s\n", key_name);
    key_press(key, scancode);
}

void engine_key_release(KeyId key, unsigned int scancode) {
    // char *key_name = keyidstr(key);
    // printf("Released key: %s\n", key_name);
    key_release(key, scancode);
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
    callbacks.window_destroyed = &engine_window_destroyed;
    callbacks.window_resized = &engine_window_resized;
    callbacks.key_press = &engine_key_press;
    callbacks.key_release = &engine_key_release;
    setup_window(800, 600);
    render_setup(320, 180);

    // Image temp = temp_img_gen();
    // DrawableId temp_id = new_sprite(20, 20, temp.width, temp.height, 0, temp.data);
    game_load();

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

        game_update(dt);

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
