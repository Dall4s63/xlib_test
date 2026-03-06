/* The interface to allow the engine to call into the game */

#ifndef GAME_H
#define GAME_H

#include "keyboard.h"

void window_destroyed(void);
// void window_resized();
void key_press(KeyId id, unsigned int scancode, bool is_repeat);
void key_release(KeyId id, unsigned int scancode);

void game_load();
void game_update(double dt);

#endif
