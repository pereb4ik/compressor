#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "stack.h"

#include "lexer.c"


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


void run(int size, char *str) {
    for (int i = 0; i < size; ++i) {
        go(str[i]);
    }
    int sz = hashtable_size(lexems);
    vertex vert[sz];
    int itr = 0;
    HashTableIter *iter;
    hashtable_iter_init(iter, lexems);
    TableEntry *cur;
    while (hashtable_iter_next(iter, &cur) != CC_ITER_END) {
        vert[itr++] = *makeVert(cur->key, cur->value);
    }
    mergesort(vert, sz, sizeof(vertex), &compare);
    curSize = 1;
    int kek = 0;
    int ind = 0;
    while (true) {
        while (hashtable_get(lexems, &curShift, &kek) == CC_OK) {
            nextStringShift();
        }
        if (vert[ind].fx[curSize] > 0) {
            /**
             * add define here
             */
        } else {
            break;
        }
        ind++;
    }
}

/**char *randString() {
    char heh[10];
    int c;
    for (int i = 0; i < 9; ++i) {
        heh[i] = (char) c;
    }
    heh[9] = '\0';
}**/

int main(int argsn, char *args[]) {
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