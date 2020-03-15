#include <stdio.h>
#include <stdlib.h>
#include <mach/boolean.h>
#include "hashtable.h"
#include "stack.h"


#include "lexer.c"
#include "utils.c"

int curSize;
char *curShift = {'a', '\0'};

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


    int curV = 0;
    for (int i = 0; i < size; ++i) {
        curV = go(str[i]);
    }

    int sz = hashtable_size(lexems);
    vertex vert[sz];
    int itr = 0;
    HashTableIter *iter;
    hashtable_iter_init(iter, lexems);
    TableEntry *cur;
    while (hashtable_iter_next(iter, &cur) != CC_ITER_END) {
        vert[itr++] = *makeVert(cur->key, cur->value);
        printf("%s ", cur->key);
        printf("%d \n", cur->value);
    }
    mergesort(vert, sz, sizeof(vertex), &compare);
    curSize = 1;
    //int kek = 0;
    //int ind = 0;

    /**while (true) {
        while (hashtable_get(lexems, &curShift, &kek) == CC_OK) {
            nextStringShift();
        }
        if (vert[ind].fx[curSize] > 0) {
            char cur[curSize + 1];
            strcpy(cur, curShift);
            hashtable_add(mapper, &(vert[ind].str), &cur);
            //add define here
        } else {
            break;
        }
        ind++;
    }**/
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

/**void test() {
    build();

    FILE *input = fopen("../main.c", "rt");
    int size = fileSize(input);

    char *s = (char *) malloc((size + 1) * sizeof(char));
    fread(s, size + 1, 1, input);
    lex(size + 1, s);
}**/

int main(int argsn, char *args[]) {
    char *files[argsn];
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
    }


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