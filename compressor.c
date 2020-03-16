#include <stdio.h>
#include <stdlib.h>
#include <mach/boolean.h>
#include "collections/hashtable.h"
#include "collections/stack.h"

#include "lexer2.c"

#include "lexer.c"
#include "utils.c"

int curSize;
char *curShift;

void nextStringShift() {
    static int first = 1;
    static char next[256];
    if (first) {
        first = 0;
        for (char c = 'a'; c < 'z'; ++c) {
            next[c] = c + 1;
        }
        next['z'] = 'A';
        for (char c = 'A'; c < 'Z'; ++c) {
            next[c] = c + 1;
        }
        next['Z'] = '0';
        for (char c = '0'; c < '9'; ++c) {
            next[c] = c + 1;
        }
        next['9'] = '_';
        next['a' - 1] = 'a';
    }

    int i = curSize - 1;
    while (i > -1 && curShift[i] == '_') {
        curShift[i] = 'a';
        i--;
    }
    if (i < 0) {
        curSize++;
        char *NewShift = allocstring(curSize);
        for (int j = 0; j < curSize; ++j) {
            NewShift[j] = 'a';
        }
        curShift = NewShift;
    } else {
        if (i == 0 && curShift[i] == 'Z') {
            curShift[i] = '_';
        } else {
            curShift[i] = next[curShift[i]];
        }
    }
}

/*
 * // First read
 * Read and lex
 */
void lex(int size, char *str) {
    //build curShift

    curShift = allocstring(2);
    curShift[0] = 'a' - 1;
    curShift[1] = '\0';
    curSize = 1;
    //

    for (int i = 0; i < size; ++i) {
        go(str[i]);
    }

    int sz = hashtable_size(lexems);
    vertex vert[sz];
    int itr = 0;
    HashTableIter iter;
    hashtable_iter_init(&iter, lexems);
    TableEntry *cur;

    while (hashtable_iter_next(&iter, &cur) != CC_ITER_END) {
        int *key = cur->value;
        vert[itr++] = *makeVert(cur->key, *key);
    }

    mergesort(vert, sz, sizeof(vertex), &compare);

    curSize = 1;
    int *kek;
    for (int ind = itr - 2; ind > -1; ind--) {
        nextStringShift();
        while (hashtable_get(lexems, curShift, &kek) == CC_OK) {
            printf("%s lexem\n", curShift);
            nextStringShift();
        }
        if (vert[ind].fx[curSize] > 0) {
            char *cur = allocstring(curSize + 1);
            strcpy(cur, curShift);
            hashtable_add(mapper, vert[ind].str, cur);
            //add define here
        } else {
            break;
        }
    }
    hashtable_iter_init(&iter, mapper);
    while (hashtable_iter_next(&iter, &cur) != CC_ITER_END) {
        printf("%s %s\n", cur->key, cur->value);
    }
}

/* // Second read
 * Read, define and write
 */
void write(int size, char *str, char *filename) {
    for (int i = 0; i < size; ++i) {
        go2(str[i]);
    }
    outFile[indf] = '\0';
    //printf("%s", outFile);
    FILE *output = fopen(filename, "wt");
    if (hasTokens) {
        fprintf(output, "#include \"ALL_DEFINES.h\"\n");
    }
    fprintf(output, "%s", outFile);
    fclose(output);
}

void writeHead() {
    /**
     * write defines to file
     */
    if (hashtable_size(mapper) > 0) {
        FILE *head = fopen("ALL_DEFINES.h", "wt");
        HashTableIter *iter;
        hashtable_iter_init(iter, mapper);
        TableEntry *cur;
        while (hashtable_iter_next(iter, &cur) != CC_ITER_END) {
            fprintf(head, "#define ");
            fprintf(head, cur->value);
            fprintf(head, " ");
            fprintf(head, cur->key);
            fprintf(head, "\n");
        }
        fclose(head);
    }
}

int main(int argsn, char *args[]) {
    build();
    build2();

    char *files[argsn];
    int sizes[argsn];

    for (int i = 0; i < argsn; ++i) {
        FILE *input = fopen(args[i], "rt");
        int size = fileSize(input);
        sizes[i] = size;
        files[i] = allocstring(size + 1);
        fread(files[i], size + 1, 1, input);
        lex(size + 1, files[i]);
    }

    writeHead();

    for (int i = 0; i < argsn; ++i) {
        write(sizes[i] + 1, files[i], args[i]);
    }
    destroyStrings();
    hashtable_destroy(mapper);
    hashtable_destroy(lexems);
    return 0;
}