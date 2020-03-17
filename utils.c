#include <stdio.h>
#include <stdlib.h>

#define long long long

#ifndef BEST_UTILS_C
#define BEST_UTILS_C

const int NumOfSamples = 5;

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
int INDEX_ALL = 0;

char *allocstring(int size) {
    ALL_ALLOCATED[INDEX_ALL] = (char *) (malloc(sizeof(char) * size));
    return ALL_ALLOCATED[INDEX_ALL++];
}

int *allocint() {
    ALL_ALLOCATED[INDEX_ALL] = (int *) (malloc(sizeof(int)));
    return ALL_ALLOCATED[INDEX_ALL++];
}

vertex *allocvert() {
    ALL_ALLOCATED[INDEX_ALL] = (vertex *) (malloc(sizeof(vertex)));
    return ALL_ALLOCATED[INDEX_ALL++];
}

void freeSpace() {
    for (int i = 0; i < INDEX_ALL; ++i) {
        free(ALL_ALLOCATED[i]);
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

#endif