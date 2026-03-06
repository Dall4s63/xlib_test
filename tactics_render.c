#include "tactics_render.h"

#include <stdlib.h>
#include <string.h>

/* Type Declarations */

#define SPRITE_TYPE 1

typedef struct _sprite_drawable {
    int type;
    int x;
    int y;
    int width;
    int height;
    int depth;
    char *data;
} SpriteDrawable;

typedef union _drawable {
    int type;
    SpriteDrawable sprite;
    long pad[4];
} Drawable;

typedef struct _drawables {
    int data_len;
    int data_buf_len;
    Drawable *data;
    int ids_len;
    int ids_buf_len;
    int *ids_to_index;
    int *index_to_ids;
} Drawables;

/* Functional Code */

static int virtual_width;
static int virtual_height;

static Image v_canvas;
static char *zbuffer;

static Drawables drawables;

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
    drawables.data_len = 0;
    drawables.data_buf_len = 100;
    drawables.data = malloc(sizeof(Drawable) * drawables.data_buf_len);
    if (drawables.data == NULL) {
        return 1;
    }
    drawables.ids_len = 0;
    drawables.ids_buf_len = drawables.data_buf_len;
    drawables.ids_to_index = malloc(sizeof(int) * drawables.ids_buf_len);
    if (drawables.ids_to_index == NULL) {
        return 1;
    }
    drawables.index_to_ids = malloc(sizeof(int) * drawables.ids_buf_len);
    if (drawables.index_to_ids == NULL) {
        return 1;
    }
    return 0;
}

int render_run(Image canvas) {
    memset(v_canvas.data, 0, 4 * v_canvas.width * v_canvas.height);
    // first render to the virtual canvas
    for (int data_i = 0; data_i < drawables.data_len; data_i++) {
        Drawable d = drawables.data[data_i];
        switch (d.type) {
        case SPRITE_TYPE: {
            int x = d.sprite.x;
            int y = d.sprite.y;
            int width = d.sprite.width;
            int height = d.sprite.height;
            int depth = d.sprite.depth;
            for (int i = 0; i < 4 * width * height; i += 4) {
                int row = (i / 4) / width;
                int col = (i / 4) - row * width;
                int final_x = x + col;
                int final_y = y + row;
                if (final_x < 0 || final_x >= v_canvas.width || final_y < 0 || final_y >= v_canvas.height) {
                    continue;
                }
                int v_index = final_y * v_canvas.width + final_x;
                if (zbuffer[v_index] > depth) {
                    continue;
                }
                zbuffer[v_index] = depth;
                v_canvas.data[v_index * 4 + 0] = d.sprite.data[i + 0];
                v_canvas.data[v_index * 4 + 1] = d.sprite.data[i + 1];
                v_canvas.data[v_index * 4 + 2] = d.sprite.data[i + 2];
                v_canvas.data[v_index * 4 + 3] = d.sprite.data[i + 3];
            }
            break; }
        }
    }

    // then copy the virtual canvas to the provided 
    // canvas 
    for (int i = 0; i < canvas.width * canvas.height * 4; i += 4) {
        int row = i / 4 / canvas.width;
        int col = i / 4 - row * canvas.width;
        int v_row = row * v_canvas.height / canvas.height;
        int v_col = col * v_canvas.width / canvas.width;
        int v_index = (v_row * v_canvas.width + v_col) * 4;
        canvas.data[i + 0] = v_canvas.data[v_index + 0];
        canvas.data[i + 1] = v_canvas.data[v_index + 1];
        canvas.data[i + 2] = v_canvas.data[v_index + 2];
        canvas.data[i + 3] = v_canvas.data[v_index + 3];
    }
    return 0;
}

int destroy_drawable(DrawableId id) {
    // TODO check if the id is valid
    if (id >= drawables.ids_len) {
        return 1;
    }
    int index = drawables.ids_to_index[id];
    if (index >= drawables.data_len) {
        return 1;
    }

    switch (drawables.data[index].type) {
    case SPRITE_TYPE:
        free(drawables.data[index].sprite.data);
        break;
    }

    int end_index = drawables.data_len - 1;
    int end_id = drawables.index_to_ids[end_index];
    drawables.data[index] = drawables.data[end_index];
    drawables.ids_to_index[id] = end_index;
    drawables.ids_to_index[end_id] = index;
    drawables.index_to_ids[index] = end_id;
    drawables.index_to_ids[end_index] = id;
    drawables.data_len -= 1;

    return 0;
}

DrawableId new_sprite(int x, int y, int width, int height, int depth, char *data) {
    if (drawables.data_len >= drawables.data_buf_len) {
        // TODO: resize the buffer
    }
    Drawable new;
    new.sprite.type = SPRITE_TYPE;
    new.sprite.x = x;
    new.sprite.y = y;
    new.sprite.width = width;
    new.sprite.height = height;
    new.sprite.depth = depth;
    new.sprite.data = data;
    int new_index = drawables.data_len;
    drawables.data[new_index] = new;
    drawables.data_len += 1;
    if (new_index < drawables.ids_len) {
        return drawables.index_to_ids[new_index];
    } else {
        if (drawables.ids_len >= drawables.ids_buf_len) {
            // TODO resize buffer
        }
        drawables.ids_to_index[new_index] = new_index;
        drawables.index_to_ids[new_index] = new_index;
        drawables.ids_len += 1;
        return new_index;
    }
}

int sprite_set(DrawableId id, SpriteInfo vals, int mask) {
    int index = drawables.ids_to_index[id];
    int type = drawables.data[index].type;
    if (type != SPRITE_TYPE) {
        return 1;
    }
    if (mask & SPRITE_X) {
        drawables.data[index].sprite.x = vals.x;
    }
    if (mask & SPRITE_Y) {
        drawables.data[index].sprite.y = vals.y;
    }
    if (mask & SPRITE_DEPTH) {
        drawables.data[index].sprite.depth = vals.depth;
    }
    if (mask & SPRITE_WIDTH) {
        drawables.data[index].sprite.width = vals.width;
    }
    if (mask & SPRITE_HEIGHT) {
        drawables.data[index].sprite.height = vals.height;
    }
    if (mask & SPRITE_DATA) {
        drawables.data[index].sprite.data = vals.data;
    }
    return 0;
}

int sprite_get(DrawableId id, SpriteInfo *ret) {
    int index = drawables.ids_to_index[id];
    int type = drawables.data[index].type;
    if (type != SPRITE_TYPE) {
        return 1;
    }
    ret->x = drawables.data[index].sprite.x;
    ret->y = drawables.data[index].sprite.y;
    ret->data = drawables.data[index].sprite.data;
    ret->width = drawables.data[index].sprite.width;
    ret->height = drawables.data[index].sprite.height;
    ret->depth = drawables.data[index].sprite.depth;
    return 0;
}
