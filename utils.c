#include <stdio.h>
#include <stdlib.h>

#define long long long

#ifndef BEST_UTILS_C
#define BEST_UTILS_C

#define NumOfSamples 5

typedef struct {
    char *str;
    long fx[NumOfSamples];
    int count;
    int len;
} vertex;

int fileSize(FILE *f) {
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}

void *ALL_ALLOCATED[100000];
int INDEX_ALLOC = 0;

char *allocstring(int size) {
    ALL_ALLOCATED[INDEX_ALLOC] = (char *) (malloc(sizeof(char) * size));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

int *allocint() {
    ALL_ALLOCATED[INDEX_ALLOC] = (int *) (malloc(sizeof(int)));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

vertex *allocvert() {
    ALL_ALLOCATED[INDEX_ALLOC] = (vertex *) (malloc(sizeof(vertex)));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

void freeSpace() {
    for (int i = 0; i < INDEX_ALLOC; ++i) {
        free(ALL_ALLOCATED[i]);
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

vertex *makeVert(char *str, long count) {
    vertex *p = allocvert();
    p->count = count;
    p->str = str;
    int len = strlen(str);
    p->len = len;
    for (int i = 0; i < NumOfSamples; ++i) {
        p->fx[i] = count * (len - (i + 1)) - (10 + len + (i + 1));
    }
    return p;
}

/**
 * Compare in lexicographic order in points
 */
int compare(vertex **a, vertex **b) {
    for (int i = 0; i < NumOfSamples; ++i) {
        if ((*a)->fx[i] < (*b)->fx[i]) {
            return -1;
        } else {
            if ((*a)->fx[i] > (*b)->fx[i]) {
                return 1;
            }
        }
    }
    return 0;
}

#endif