#ifndef HASH_STR_INT_H
#define HASH_STR_INT_H

typedef struct _stoi_hash_unit {
    char                   *string;
    int                     v;
    struct _stoi_hash_unit *next;
} STOIHashUnit;

typedef struct _stoi_hash_map {
    STOIHashUnit *values;
} STOIHashMap;

STOIHashMap stoi_hash_new(void);
void stoi_hash_free(STOIHashMap m);

bool stoi_hash_get(STOIHashMap m, char *s, int *ret);
bool stoi_hash_put(STOIHashMap m, char *s, int v);

#endif
