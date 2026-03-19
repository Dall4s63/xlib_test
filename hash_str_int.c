#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_str_int.h"

#define HASH_BUF_LEN 60
#define HASH_PRIME 67

STOIHashMap stoi_hash_new(void) {
    STOIHashMap out;
    out.values = malloc(sizeof(STOIHashUnit) * HASH_BUF_LEN);
    if (out.values == NULL) {
        // TODO we are in trouble
        printf("failed allocation\n");
    }
    memset(out.values, 0, sizeof(STOIHashUnit) * HASH_BUF_LEN);
    for (int i = 0; i < HASH_BUF_LEN; ++i) {
        out.values[i].string == NULL;
    }
    return out;
}

static void free_unit(STOIHashUnit *u) {
    if (u == NULL) {
        return;
    }
    free_unit(u->next);
    free(u->string);
    free(u);
}

void stoi_hash_free(STOIHashMap m) {
    for (int i = 0; i < HASH_BUF_LEN; ++i) {
        free_unit(m.values[i].next);
    }
    free(m.values);
}

static unsigned int calc_hash(char *s) {
    unsigned int v = 0;
    unsigned int mul = 1;
    while (*s++ != '\0') {
        v += (int)*s * mul;
        mul *= HASH_PRIME;
    }
    return v % HASH_BUF_LEN;
}

bool stoi_hash_get(STOIHashMap m, char *s, int *ret) {
    unsigned int i = calc_hash(s);
    if (m.values[i].string == NULL) {
        return false;
    }
    if (strcmp(m.values[i].string, s) == 0) {
        *ret = m.values[i].v;
        return true;
    }
    STOIHashUnit *cur = m.values[i].next;
    while (cur != NULL) {
        if (strcmp(cur->string, s) == 0) {
            *ret = cur->v;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

bool stoi_hash_put(STOIHashMap m, char *s, int v) {
    int i = calc_hash(s);
    if (m.values[i].string == NULL) {
        int len = strlen(s) + 1;
        m.values[i].string = malloc(sizeof(char) * len);
        strcpy(m.values[i].string, s);
        m.values[i].v = v;
        m.values[i].next = NULL;
        return false;
    }
    if (strcmp(m.values[i].string, s) == 0) {
        m.values[i].v = v;
        return true;
    }
    STOIHashUnit *prev = &(m.values[i]);
    STOIHashUnit *cur = m.values[i].next;
    while (cur != NULL) {
        if (strcmp(cur->string, s) == 0) {
            cur->v = v;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    STOIHashUnit *new = malloc(sizeof(STOIHashUnit));
    prev->next = new;
    int len = strlen(s) + 1;
    new->string = malloc(sizeof(char) * len);
    strcpy(new->string, s);
    new->v = v;
    new->next = NULL;
    return false;
}

bool stoi_hash_del(STOIHashMap m, char *s) {
    int i = calc_hash(s);
    if (m.values[i].string == NULL) {
        return false;
    }
    if (strcmp(m.values[i].string, s) == 0) {
        free(m.values[i].string);
        m.values[i].string = NULL;
        return true;
    }
    STOIHashUnit *prev = &(m.values[i]);
    STOIHashUnit *cur = m.values[i].next;
    while (cur != NULL) {
        if (strcmp(cur->string, s) == 0) {
            free(cur->string);
            prev->next = cur->next;
            free(cur);
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

#undef HASH_BUF_LEN
#undef HASH_PRIME

