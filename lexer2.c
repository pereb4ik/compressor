#include <stdlib.h>
#include <form.h>
#include "collections/hashtable.h"
#include "utils.c"

#include "lexer.c"

#ifndef BEST_LEXER2_C
#define BEST_LEXER2_C

char Char;

char *bufff;

char *outFile;
int curv = 0;

int indf = 0;
int ind0 = 0;

void metadef();

void adC() {
    bufff[ind0++] = Char;
}


void add();

void siex() {
    static int even = 1;
    even = 1 - even;
    if (even == 0) {
        ind0 = 0;
        adC();
    } else {
        adC();
        bufff[ind0] = '\0';
        metadef();
        add();
    }
}

void fiex() {
    static int even = 1;
    even = 1 - even;
    if (even == 0) {
        ind0 = 0;
        adC();
    } else {
        bufff[ind0] = '\0';
        metadef();
        add();
    }
}

void metadef() {
    char *def;
    enum cc_stat heh;
    if ((heh = hashtable_get(mapper, bufff, &def)) == CC_OK) {
        for (int i = 0; def[i] != '\0'; ++i) {
            outFile[indf++] = def[i];
        }
    } else {
        for (int i = 0; i < ind0; ++i) {
            outFile[indf++] = bufff[i];
        }
    }
    printf("\"%s\" :String %d\n", bufff, heh);
}

void add() {
    outFile[indf++] = Char;
}

/**
 * 0 - all
 * 1 [*]
 * 2 [/]
 * 3 [\n]
 * 4 [\]
 * 5 [ \t]
 * 6 ["]
 * 7 [']
 * 8 [a-zA-Z0-9_]
 * 9 [#]
 */

int gov[13][10] = {
        {0,  0,  1,  0,  0,  0,  6,  8,  10, 11}, // 0 start vertex
        {0,  2,  4,  0,  0,  0,  0,  0,  0,  0}, // 1 comments start
        {2,  3,  2,  2,  2,  2,  2,  2,  2,  2}, // 2 /* comments
        {2,  3,  0,  2,  2,  2,  2,  2,  2,  2}, // 3 /* comments
        {4,  4,  4,  0,  5,  4,  4,  4,  4,  4}, // 4 // comments
        {4,  4,  4,  4,  5,  5,  4,  4,  4,  4}, // 5 // comments
        {6,  6,  6,  6,  7,  6,  0,  6,  6,  6}, // 6 "" string
        {6,  6,  6,  6,  6,  6,  6,  6,  6,  6}, // 7 "" string
        {8,  8,  8,  8,  9,  8,  8,  0,  8,  8}, // 8 '' string
        {8,  8,  8,  8,  8,  8,  8,  8,  8,  8}, // 9 '' string
        {0,  0,  0,  0,  0,  0,  0,  0,  10, 0}, // 10 lexeme or number
        {11, 11, 11, 0,  12, 11, 11, 11, 11, 11},// 11 # scipper
        {11, 11, 11, 11, 12, 12, 11, 11, 11, 11} // 12 # scipper
};

void (*F[13][10])() = {
        {add,  add,  V,    add,  add,  add,  siex, siex, fiex, add},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {adC,  adC,  adC,  adC,  adC,  adC,  siex, adC,  adC,  adC},
        {adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC},
        {adC,  adC,  adC,  adC,  adC,  adC,  adC,  siex, adC,  adC},
        {adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC},
        {fiex, fiex, fiex, fiex, fiex, fiex, fiex, fiex, adC,  fiex},
        {add,  add,  add,  add,  add,  add,  add,  add,  add,  add},
        {add,  add,  add,  add,  add,  add,  add,  add,  add,  add}
};

int go2(char c) {
    Char = c;
    int e = class[128 + c];
    F[curv][e]();
    curv = gov[curv][e];
    return curv;
}

void build2() {
    bufff = allocstring(1000);
    outFile = allocstring(10000);
}

#endif