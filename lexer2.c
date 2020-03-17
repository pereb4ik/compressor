#include <stdlib.h>
#include <form.h>
#include "collections/hashtable.h"
#include "utils.c"

#include "lexer.c"

#ifndef BEST_LEXER2_C
#define BEST_LEXER2_C

/**
 * Here used ind, curV, classes, goV from lexer
 */

char *bufff;

char *outFile;

int indf = 0;

bool hasTokens;

void metadef();

void adC() {
    bufff[ind++] = curChar;
}

void add();

void siex() {
    static int even = 1;
    even = 1 - even;
    if (even == 0) {
        ind = 0;
        adC();
    } else {
        adC();
        bufff[ind] = '\0';
        metadef();
    }
}

void fiex() {
    static int even = 1;
    even = 1 - even;
    if (even == 0) {
        ind = 0;
        adC();
    } else {
        bufff[ind] = '\0';
        metadef();
        if (curChar != '/') {
            outFile[indf++] = curChar;
        }
    }
}

void metadef() {
    char *def;
    if (hashtable_get(mapper, bufff, &def) == CC_OK) {
        hasTokens = true;
        for (int i = 0; def[i] != '\0'; ++i) {
            outFile[indf++] = def[i];
        }
    } else {
        for (int i = 0; bufff[i] != '\0'; ++i) {
            outFile[indf++] = bufff[i];
        }
    }
}

void add() {
    outFile[indf++] = curChar;
}

//print slash, crutch
void pla() {
    outFile[indf++] = '/';
    add();
}

void sla() {
    outFile[indf++] = '/';
    siex();
}

void fla() {
    outFile[indf++] = '/';
    fiex();
}

//end of /* comments crutch (to a/**/b -> a b)
void end() {
    outFile[indf++] = ' ';
}

/**
 * 0 - all
 * 1 [*]
 * 2 [/]
 * 3 [\n\0\r]
 * 4 [\]
 * 5 [ \t]
 * 6 ["]
 * 7 [']
 * 8 [a-zA-Z0-9_]
 * 9 [#]
 */

void (*F[16][10])() = {
        {add,  add,  V,    add,  add,  add,  siex, siex, fiex, add}, // 0
        {pla,  V,    V,    pla,  pla,  pla,  sla,  sla,  fla,  pla}, // 1
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 2
        {V,    V,    end,  V,    V,    V,    V,    V,    V,    V},   // 3
        {V,    V,    V,    add,  V,    V,    V,    V,    V,    V},   // 4
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 5
        {adC,  adC,  adC,  adC,  adC,  adC,  siex, adC,  adC,  adC}, // 6
        {adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC}, // 7
        {adC,  adC,  adC,  adC,  adC,  adC,  adC,  siex, adC,  adC}, // 8
        {adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC,  adC}, // 9
        {fiex, fiex, fiex, fiex, fiex, fiex, fiex, fiex, adC,  fiex},// 10
        {add,  add,  add,  add,  add,  add,  add,  add,  add,  add}, // 11
        {add,  add,  add,  add,  add,  add,  add,  add,  add,  add}, // 12
        {add,  add,  V,    add,  add,  V,    siex, siex, fiex, add}, // 13
        {add,  add,  V,    V,    add,  add,  siex, siex, fiex, add}, // 14
        {add,  add,  V,    V,    add,  V,    siex, siex, fiex, add}  // 15
};

int go2(char c) {
    curChar = c;
    int e = class[128 + c];
    F[curV][e]();
    curV = goV[curV][e];
    return curV;
}


/**
 * Build bufff and outfile by size
 */

void build2(int size) {
    ind = 0;
    bufff = allocstring(size);
    outFile = allocstring(size);
}

#endif