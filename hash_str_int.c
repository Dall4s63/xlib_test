#include <stdio.h>

#include "hash_str_int.h"

#define HASH_INIT_LEN 60
#define HASH_PRIME 67

STOIHashMap stoi_hash_new(void) {
    STOIHashMap out;
    out.values = malloc(sizeof(STOIHashUnit) * HASH_INIT_LEN);
    if (out.values == NULL) {
        // TODO we are in trouble
        printf("failed allocation\n");
    }
    memset(out.values, 0, sizeof(STOIHashUnit) * HASH_INIT_LEN);
    return out;
}

void stoi_hash_free(STOIHashMap m) {
}

bool stoi_hash_get(STOIHashMap m, char *s, int *ret) {
}

bool stoi_hash_put(STOIHashMap m, char *s, int v) {
}

#undef HASH_INIT_LEN

