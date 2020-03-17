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
        add();
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

void (*F[16][10])() = {
        {add,  add,  V,    add,  add,  add,  siex, siex, fiex, add},
        {pla,  V,    V,    pla,  pla,  pla,  sla,  sla,  fla,  pla},
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
        {add,  add,  add,  add,  add,  add,  add,  add,  add,  add},
        {add,  add,  V,    add,  add,  V,    siex, siex, fiex, add},
        {add,  add,  V,    V,    add,  add,  siex, siex, fiex, add},
        {add,  add,  V,    V,    add,  V,    siex, siex, fiex, add}
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