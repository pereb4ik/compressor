#include <stdio.h>
#include <stdlib.h>

#ifndef COMPRESSOR_UTILS_C
#define COMPRESSOR_UTILS_C

#define long long long
#define NumOfSamples 5

typedef struct {
    char *word;
    long fx[NumOfSamples];
} Vertex;

void **ALL_ALLOCATED[33];
int INDEX_ALLOC = 10;
int CURRENT_BUCKET = 0;
int SHIFT = (1 << 0);

int mx(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

void *next(void *link) {
    INDEX_ALLOC++;
    if (SHIFT <= INDEX_ALLOC) {
        CURRENT_BUCKET++;
        SHIFT = 1 << CURRENT_BUCKET;
        INDEX_ALLOC = 0;
        ALL_ALLOCATED[CURRENT_BUCKET] = (void *) (malloc(sizeof(void *) * (1 << CURRENT_BUCKET)));
    }
    ALL_ALLOCATED[CURRENT_BUCKET][INDEX_ALLOC] = link;
    return link;
}

int *allocInt() {
    return next(malloc(sizeof(int)));
}

Vertex *allocVertex() {
    return next(malloc(sizeof(Vertex)));
}

char *allocString(int size) {
    return next(malloc(sizeof(char) * size));
}

int *allocIntArray(int size) {
    return next(malloc(sizeof(int) * size));
}

char **allocStringArray(int size) {
    return next(malloc(sizeof(char *) * size));
}

Vertex **allocVertexArray(int size) {
    return next(malloc(sizeof(Vertex *) * size));
}

void freeSpace() {
    for (int i = 1; i <= CURRENT_BUCKET; ++i) {
        for (int j = 0; j < (1 << i); ++j) {
            free(ALL_ALLOCATED[i][j]);
        }
    }
    for (int i = 1; i <= CURRENT_BUCKET; ++i) {
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