#include <stdio.h>
#include <stdlib.h>
#include <mach/boolean.h>
#include "collections/hashtable.h"
#include "collections/stack.h"


#include "lexer.c"
#include "utils.c"

int curSize;
char *curShift;

/**
 * Realize normal shifter, with black jack and A-Z
 */
void nextStringShift() {
    int i = curSize - 1;
    while (i > -1 && curShift[i] == 'z') {
        curShift[i] = 'a';
        i--;
    }
    if (i < 0) {
        curSize++;
        char NewShift[curSize];
        for (int j = 0; j < curSize; ++j) {
            NewShift[j] = 'a';
        }
        curShift = NewShift;
    } else {
        curShift[i]++;
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

    int curV = 0;
    for (int i = 0; i < size; ++i) {
        curV = go(str[i]);
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
        //printf("%s ", cur->key);
        //printf("%d\n", *key);
    }

    mergesort(vert, sz, sizeof(vertex), &compare);
    for (int i = 0; i < itr - 1; ++i) {
        printf("%s ", vert[i].str);
        printf("%d\n", vert[i].count);
    }
    curSize = 1;
    int *kek;
    for (int ind = itr - 2; ind > -1; ind--) {
        nextStringShift();
        while (hashtable_get(lexems, &curShift, &kek) == CC_OK) {
            nextStringShift();
        }
        if (vert[ind].fx[curSize] > 0) {
            //char cur[curSize + 1];
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
        printf("%s ", cur->key);
        printf("%s\n", cur->value);
    }
}

/* // Second read
 * Read, define and write
 */
void write(int size, char *str, char *filename) {
    bool hasTokens = false;
    hasTokens = true;

    FILE *output = fopen(filename, "wt");
    if (hasTokens) {
        fprintf(output, "#include \"ALL_DEFINES.h\"\n");
    }
}

void writeHead() {
    /**
     * write defines to file
     */
    FILE *head = fopen("ALL_DEFINES.h", "wt");
    HashTableIter *iter;
    hashtable_iter_init(iter, mapper);
    TableEntry *cur;
    while (hashtable_iter_next(iter, &cur) != CC_ITER_END) {
        fprintf(head, "#define ");
        fprintf(head, cur->key);
        fprintf(head, " ");
        fprintf(head, cur->value);
        fprintf(head, "\n");
    }
    fclose(head);
}


char *testFilename = "../main.c";

void test() {
    build();

    FILE *input = fopen(testFilename, "rt");
    int size = fileSize(input);

    char *s = (char *) malloc((size + 1) * sizeof(char));
    fread(s, size + 1, 1, input);
    lex(size + 1, s);
}

int main(int argsn, char *args[]) {
    test();

    /*char *files[argsn];
    int sizes[argsn];

    for (int i = 0; i < argsn; ++i) {
        FILE *input = fopen(args[i], "rt");
        int size = fileSize(input);
        sizes[i] = size;
        files[i] = (char *) malloc((size + 1) * sizeof(char));
        fread(files[i], size + 1, 1, input);
        lex(size + 1, files[i]);
    }

    writeHead();

    for (int i = 0; i < argsn; ++i) {
        write(sizes[i] + 1, files[i], args[i]);
    }*/


    /**HashTable *table;
    //hashtable_new(&table);
    HashTableConf cnf;
    hashtable_conf_init(&cnf);
    cnf.initial_capacity = 20000000;
    hashtable_new_conf(&cnf, &table);
    for (int i = 0; i < 10000000; ++i) {
        char *lol = randString();
        int val;
        hashtable_add(table, &lol, rand());
        hashtable_get(table, &lol, &val);
    }**/
    return 0;
}