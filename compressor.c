#include <stdio.h>
#include <stdlib.h>
#include "collections/hashtable.h"

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
    }

    int i;
    for (i = curSize - 1; i > -1 && curShift[i] == '_'; --i) {
        curShift[i] = 'a';
    }
    if (i < 0) {
        curSize++;
        char *NewShift = allocstring(curSize + 1);
        for (int j = 0; j < curSize; ++j) {
            NewShift[j] = 'a';
        }
        NewShift[curSize] = '\0';
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
 * First lex
 */
void lex(int size, char *str) {
    for (int i = 0; i < size; ++i) {
        go(str[i]);
    }
    printf("%d last char\n", (int) (str[size]));
    printf("%d last v\n", curV);
}

void calck() {
    //build curShift
    curShift = allocstring(2);
    curShift[0] = 'a';
    curShift[1] = '\0';
    curSize = 1;
    //

    int hsize = hashtable_size(lexems);
    vertex *vert[hsize];
    HashTableIter iter;
    hashtable_iter_init(&iter, lexems);
    TableEntry *cur;

    for (int i = 0; hashtable_iter_next(&iter, &cur) != CC_ITER_END; ++i) {
        int *val = cur->value;
        printf("%s %d\n", cur->key, *val);
        vert[i] = makeVert(cur->key, *val);
    }

    mergesort(vert, hsize, sizeof(vertex *), &compare);

    int *kek;
    for (int i = hsize - 1; i > -1; i--) {
        while (hashtable_get(lexems, curShift, &kek) == CC_OK) {
            printf("%s :has\n", curShift);
            nextStringShift();
        }
        // ATTENTION, fx[a] is f(a + 1)
        if (vert[i]->fx[curSize - 1] > 0) {
            char *cur = allocstring(curSize + 1);
            strcpy(cur, curShift);
            hashtable_add(mapper, vert[i]->str, cur);
        } else {
            break;
        }
        nextStringShift();
    }
}

/*
 * Read, define and write
 */
void write(int size, char *str, char *filename) {
    indf = 0;
    curV = 0;
    hasTokens = false;
    for (int i = 0; i < size; ++i) {
        go2(str[i]);
    }
    outFile[indf] = '\0';
    FILE *output = fopen(filename, "wt");
    if (hasTokens) {
        fprintf(output, "%s", "#include \"ALL_DEFINES.h\"\n");
    }
    fprintf(output, "%s", outFile);
    fclose(output);
}

/**
  * Write defines to file
  */
void writeHead() {
    if (hashtable_size(mapper) > 0) {
        printf("%lu size of mapper\n", hashtable_size(mapper));
        FILE *head = fopen("ALL_DEFINES.h", "wt");
        HashTableIter iter;
        hashtable_iter_init(&iter, mapper);
        TableEntry *cur;
        while (hashtable_iter_next(&iter, &cur) != CC_ITER_END) {
            fprintf(head, "%s", "#define ");
            fprintf(head, "%s", cur->value);
            fprintf(head, "%s", " ");
            fprintf(head, "%s", cur->key);
            fprintf(head, "%s", "\n");
        }
        fclose(head);
    }
}

int main(int argsn, char *args[]) {
    build();
    char *files[argsn];
    int sizes[argsn];
    int maxSize = 0;

    for (int i = 1; i < argsn; ++i) {
        FILE *input = fopen(args[i], "rt");
        if (input == NULL) {
            printf("%s %s %s", "File", args[i], "not found");
            return EXIT_FAILURE;
        }
        int size = fileSize(input);
        sizes[i] = size;
        maxSize = max(maxSize, size);
        files[i] = allocstring(size + 1);
        fread(files[i], size + 1, 1, input);
        files[i][size] = '\0';
        fclose(input);
        lex(size + 1, files[i]);
    }

    calck();
    build2(maxSize + 1);

    for (int i = 1; i < argsn; ++i) {
        write(sizes[i] + 1, files[i], args[i]);
    }
    writeHead();

    //destroy all
    freeSpace();
    hashtable_destroy(mapper);
    hashtable_destroy(lexems);
    printf("%s", "You should add ALL_DEFINES.h to includes of compiler\n");
    return EXIT_SUCCESS;
}