#include "collections/hashtable.h"
#include "utils.c"

#include "lexer.c"

#ifndef COMPRESSOR_REPLACER_C
#define COMPRESSOR_REPLACER_C

// In this file reused ind, v, classes, goV from lexer

// Buffer for lexeme
char *bufff;
char *outFile;

int indf = 0;

bool hasTokens;

// Rename(or not) lexeme by mapper
void renamer() {
    char *def;
    if (hashtable_get(mapper, bufff, (void **) &def) == CC_OK) {
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

// Add character to bufff
void adC() {
    bufff[ind++] = curChar;
}

// Add character to outfile
void add() {
    outFile[indf++] = curChar;
}

// start/end string const
void siex() {
    static int even = 0;
    even = 1 - even;
    if (even) {
        ind = 0;
        adC();
    } else {
        adC();
        bufff[ind] = '\0';
        renamer();
    }
}

// start/end lexeme
void fiex() {
    static int even = 0;
    even = 1 - even;
    if (even) {
        ind = 0;
        adC();
    } else {
        bufff[ind] = '\0';
        renamer();
        if (curChar != '/') {
            outFile[indf++] = curChar;
        }
    }
}

// Print slash, crutch
void pla() {
    outFile[indf++] = '/';
    add();
}

// slash before string
void sla() {
    outFile[indf++] = '/';
    siex();
}

// slash before lexem
void fla() {
    outFile[indf++] = '/';
    fiex();
}

// End of /* comments crutch (to a/**/b -> a b)
void end() {
    outFile[indf++] = ' ';
}

/**
 *
 * Classes of characters
 *
 * 0 - all another
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

/**
 * Actions on edge pass
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
    F[v][e]();
    v = goV[v][e];
    return v;
}

// Build bufff and outfile by his size
void build2(int size) {
    bufff = allocString(size);
    outFile = allocString(size);
}

#endif // COMPRESSOR_REPLACER_C