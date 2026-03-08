/* I can't decide what to do, but this seems fun
 *
 * maybe can make a dungeon crawler
 */

#include <stdlib.h>
#include <string.h>

#include "doom_render.c"

static int virtual_width;
static int virtual_height;

static Image v_canvas;
static char *zbuffer;

int render_setup(int v_width, int v_height) {
    virtual_width = v_width;
    virtual_height = v_height;
    v_canvas.width = v_width;
    v_canvas.height = v_height;
    v_canvas.data = malloc(sizeof(char) * 4 * v_width * v_height);
    if (v_canvas.data == NULL) {
        return 1;
    }
    zbuffer = malloc(sizeof(int) * v_width * v_height);
    if (zbuffer == NULL) {
        return 1;
    }
    return 0;
}

