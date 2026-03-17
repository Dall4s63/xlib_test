#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

typedef struct _rgba_fcolor {
    float r;
    float g;
    float b;
    float a;
} FColor;

// IDEA:
//  the sprite system stores a filename with an id
//  the id is ref-counted
//  when all the refs for an id have been freed, the 
//  name is also removed and the resource is freed.
int sprite_new(char *filename);

void sprite_free(int id);

int sprite_width(int id);
int sprite_height(int id);
FColor sprite_sample(int id, int x, int y);

#endif
