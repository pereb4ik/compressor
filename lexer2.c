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

bool hasTokens = false;

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
    outFile[indf++] = Char;
}

//print slash, crutch
void sla() {
    outFile[indf++] = '/';
    add();
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
        {sla,  V,    V,    sla,  sla,  sla,  sla,  sla,  sla,  sla},
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
    Char = c;
    int e = class[128 + c];
    F[curv][e]();
    curv = goV[curv][e];
    return curv;
}

void build2(int size) {
    bufff = allocstring(size);
    outFile = allocstring(size);
}

#endif