
static int virtual_width;
static int virtual_height;

static Image v_canvas;

typedef struct _drawables {
    int data_len;
    int data_buf_len;
    Drawable *data;
    int ids_len;
    int ids_buf_len;
    int *ids_to_index;
    int *index_to_ids;
} Drawables;

static Drawables drawables;

int setup(int v_width, int v_height) {
    virtual_width = v_width;
    virtual_height = v_height;
    v_canvas.width = v_width;
    v_canvas.height = v_height;
    v_canvas.data = malloc(sizeof(char) * 4 * v_width * v_height);
    if (v_canvas.data == NULL) {
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

int render(Image canvas) {
    // first render to the virtual canvas
    for (int i = 0; i < drawables.data_len; i++) {
    }
    // then copy the virtual canvas to the provided 
    // canvas 
    return 0;
}
