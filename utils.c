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

int *allocIntArray(int size) {
    ALL_ALLOCATED[INDEX_ALLOC] = (int *) (malloc(sizeof(int) * size));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

char **allocStringArray(int size) {
    ALL_ALLOCATED[INDEX_ALLOC] = (char **) (malloc(sizeof(char *) * size));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

vertex *allocvert() {
    ALL_ALLOCATED[INDEX_ALLOC] = (vertex *) (malloc(sizeof(vertex)));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

vertex **allocVertArray(int size) {
    ALL_ALLOCATED[INDEX_ALLOC] = (vertex **) (malloc(sizeof(vertex *) * size));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

void freeSpace() {
    for (int i = 0; i < INDEX_ALLOC; ++i) {
        free(ALL_ALLOCATED[i]);
    }
}

int mx(int a, int b) {
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
int comparator(const void *a, const void *b) {
    vertex **A = (vertex **) a;
    vertex **B = (vertex **) b;
    for (int i = 0; i < NumOfSamples; ++i) {
        if ((*A)->fx[i] < (*B)->fx[i]) {
            return -1;
        } else {
            if ((*A)->fx[i] > (*B)->fx[i]) {
                return 1;
            }
        }
    }
    return 0;
}

//ohh
int fileSize(char *filename) {
    FILE *f = fopen(filename, "rt");
    int file_size = 0;
    while (getc(f) != EOF)
        file_size++;
    fclose(f);
    return file_size;
}

char *readFile(char *filename, int filesize) {
    char *buff = allocstring(filesize + 1);
    FILE *f = fopen(filename, "rt");
    int i = 0;
    int c = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c == 13) {
            buff[i++] = 10;
        } else {
            buff[i++] = (char) c;
        }
    }
    buff[i] = '\0';
    fclose(f);
    return buff;
}

#endif