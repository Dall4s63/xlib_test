#include <stdio.h>
#include <stdlib.h>

#include "map_loader.h"

/*
 * Room File Format:
 *
 * #walls nwalls
 * a1, b1, a2, b2, "sprite_fname"
 * _, _, a2, b2, "sprite_fname"
 *
 * the underscores are replaced by the a2 b2 values 
 * from the previous wall.
 */

#define MAX_LINE_LEN 200

int load_room(char *filename, MapRoom *ret) {
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
    int nwalls;
    sscanf(line_buf, "#walls %d\n", &nwalls);
    printf("nwalls: %d\n", nwalls);
    return 0;
}

#undef MAX_LINE_LEN
