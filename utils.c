#include <stdio.h>
#include <stdlib.h>

#ifndef BEST_UTILS_C
#define BEST_UTILS_C

int fileSize(FILE *f) {
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}

void *ALL_ALLOCATED_STRINGS[100000];
int INDEX_ALL = 0;

char *allocstring(int size) {
    ALL_ALLOCATED_STRINGS[INDEX_ALL] = (char *) (malloc(sizeof(char) * (size)));
    return ALL_ALLOCATED_STRINGS[INDEX_ALL++];
}

int *allocint() {
    ALL_ALLOCATED_STRINGS[INDEX_ALL] = (int *) (malloc(sizeof(int)));
    return ALL_ALLOCATED_STRINGS[INDEX_ALL++];

}

void destroyStrings() {
    INDEX_ALL++;
    for (int i = 0; i < INDEX_ALL; ++i) {
        free(ALL_ALLOCATED_STRINGS[i]);
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

#endif