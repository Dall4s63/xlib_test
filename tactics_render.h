#ifndef TACTICS_RENDER_H
#define TACTICS_RENDER_H

#include "display.h"

typedef int DrawableId;

int render_setup(int v_width, int v_height);
int render_run(Image canvas);

DrawableId new_sprite(int x, int y, int width, int height, int depth, char *data);

#endif
