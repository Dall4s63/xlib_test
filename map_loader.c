#include <stdio.h>
#include <stdlib.h>

#include "map_loader.h"
#include "float_vec2.h"
#include "sprite_system.h"

/*
 * Room File Format:
 *
 * #walls nwalls wall_height
 * a1, a2, b1, b2, sprite_fname
 * _, _, b1, b2, sprite_fname
 *
 * the underscores are replaced by the a2 b2 values 
 * from the previous wall.
 */

#define MAX_LINE_LEN 200

int load_room(char *filename, MapRoom *room) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("failed to open file \"%s\"\n", filename);
        return 1;
    }
    char line_buf[MAX_LINE_LEN];
    fgets(line_buf, MAX_LINE_LEN, f);
    while (line_buf[0] != '#') {
        fgets(line_buf, MAX_LINE_LEN, f);
    }
    char sfname[200];
    sscanf(line_buf, "#ceiling %s\n", sfname);
    room->ceil_spr = sprite_new(sfname);
    fgets(line_buf, MAX_LINE_LEN, f);
    while (line_buf[0] != '#') {
        fgets(line_buf, MAX_LINE_LEN, f);
    }
    sscanf(line_buf, "#floor %s\n", sfname);
    fgets(line_buf, MAX_LINE_LEN, f);
    while (line_buf[0] != '#') {
        fgets(line_buf, MAX_LINE_LEN, f);
    }
    room->floor_spr = sprite_new(sfname);
    int nwalls;
    float wall_height;
    sscanf(line_buf, "#walls %d %f\n", &nwalls, &wall_height);
    // printf("nwalls: %d %f\n", nwalls, wall_height);

    room->walls = malloc(sizeof(MapWall) * nwalls);
    room->walls_len = nwalls;
    room->walls_buf_len = nwalls;
    room->wall_height = wall_height;

    float prev_b1 = 0.0, prev_b2 = 0.0;
    for (int i = 0; i < nwalls; ++i) {
        fgets(line_buf, MAX_LINE_LEN, f);
        float a1, a2, b1, b2;
        if (line_buf[0] == '_') {
            a1 = prev_b1;
            a2 = prev_b2;
            sscanf(line_buf, "_, _, %f, %f, %s\n", &b1, &b2, sfname);
        } else {
            sscanf(line_buf, "%f, %f, %f, %f, %s\n", &a1, &a2, &b1, &b2, sfname);
        }
        room->walls[i].a = (FloatVec2) {.x = a1, .y = a2};
        room->walls[i].b = (FloatVec2) {.x = b1, .y = b2};
        room->walls[i].spr = sprite_new(sfname);
        prev_b1 = b1;
        prev_b2 = b2;
        // printf("%f, %f, %f, %f, \"%s\" %d\n",
        //     a1, a2, b1, b2, sfname, room->walls[i].spr);
    }

    return 0;
}

#undef MAX_LINE_LEN
