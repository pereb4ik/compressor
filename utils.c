#include <stdio.h>
#include <stdlib.h>

#ifndef COMPRESSOR_UTILS_C
#define COMPRESSOR_UTILS_C

#define long long long
#define NumOfSamples 5
#define MaxNumOfAllocations 100000

typedef struct {
    char *word;
    long fx[NumOfSamples];
} Vertex;

void *ALL_ALLOCATED[MaxNumOfAllocations];
int INDEX_ALLOC = 0;

int mx(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int *allocInt() {
    ALL_ALLOCATED[INDEX_ALLOC] = (int *) (malloc(sizeof(int)));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

Vertex *allocVertex() {
    ALL_ALLOCATED[INDEX_ALLOC] = (Vertex *) (malloc(sizeof(Vertex)));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

char *allocString(int size) {
    ALL_ALLOCATED[INDEX_ALLOC] = (char *) (malloc(sizeof(char) * size));
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

Vertex **allocVertexArray(int size) {
    ALL_ALLOCATED[INDEX_ALLOC] = (Vertex **) (malloc(sizeof(Vertex *) * size));
    return ALL_ALLOCATED[INDEX_ALLOC++];
}

void freeSpace() {
    for (int i = 0; i < INDEX_ALLOC; ++i) {
        free(ALL_ALLOCATED[i]);
    }
}

Vertex *makeVertex(char *str, long count) {
    Vertex *p = allocVertex();
    p->word = str;
    int len = strlen(str);
    for (int i = 0; i < NumOfSamples; ++i) {
        p->fx[i] = count * (len - (i + 1)) - (10 + len + (i + 1));
    }
    return p;
}

// Compare in lexicographic order in points
int comparator(const void *a, const void *b) {
    Vertex **A = (Vertex **) a;
    Vertex **B = (Vertex **) b;
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

int fileSize(char *filename) {
    FILE *f = fopen(filename, "rt");
    int file_size = 0;
    while (getc(f) != EOF)
        file_size++;
    fclose(f);
    return file_size;
}

// Reading with changing \t to \n
char *readFile(char *filename, int filesize) {
    char *buff = allocString(filesize + 1);
    FILE *f = fopen(filename, "rt");
    int i = 0;
    int c;
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

#endif // COMPRESSOR_UTILS_C