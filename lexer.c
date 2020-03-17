#include <stdlib.h>
#include <form.h>
#include "collections/hashtable.h"
#include "utils.c"

#ifndef BEST_LEXER_C
#define BEST_LEXER_C

int class[256];

int curV = 0;

HashTable *lexems;
HashTable *mapper;

/**
 * transition functions
 */

void countLexeme(char *token) {
    int *count;
    if (hashtable_get(lexems, token, &count) == CC_OK) {
        (*count)++;
    } else {
        count = allocint();
        *count = 1;
    }
    hashtable_add(lexems, token, count);
}

char *buff;
char curChar;
int ind;

void addC() {
    buff[ind++] = curChar;
}

const int MAX_LEXEM_LEN = 1000;

void startLexem() {
    buff = allocstring(MAX_LEXEM_LEN);
    ind = 0;
}

//fuck yeah string lex
//copypaste of flex
void slex() {
    static int even = 0;
    even = 1 - even;
    if (even) {
        startLexem();
        addC();
    } else {
        addC();
        buff[ind] = '\0';
        countLexeme(buff);
    }
}

//fuck yeah lex
void flex() {
    static int even = 0;
    even = 1 - even;
    if (even) {
        startLexem();
        addC();
    } else {
        buff[ind] = '\0';
        countLexeme(buff);
    }
}

//void function
void V() {

}
//////////////////

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

/**
 * please, add space/tab scipper
 * is this correct?
 */
int goV[16][10] = {
        {0,  0,  1,  14, 0,  13, 6,  8,  10, 11},// 0 start vertex
        {0,  2,  4,  14, 0,  13, 6,  8,  10, 11},// 1 comments start
        {2,  3,  2,  2,  2,  2,  2,  2,  2,  2}, // 2 /* comments
        {2,  3,  0,  2,  2,  2,  2,  2,  2,  2}, // 3 /* comments
        {4,  4,  4,  14, 5,  4,  4,  4,  4,  4}, // 4 // comments
        {4,  4,  4,  4,  5,  5,  4,  4,  4,  4}, // 5 // comments
        {6,  6,  6,  6,  7,  6,  0,  6,  6,  6}, // 6 "" string
        {6,  6,  6,  6,  6,  6,  6,  6,  6,  6}, // 7 "" string
        {8,  8,  8,  8,  9,  8,  8,  0,  8,  8}, // 8 '' string
        {8,  8,  8,  8,  8,  8,  8,  8,  8,  8}, // 9 '' string
        {0,  0,  1,  14, 0,  13, 0,  0,  10, 0}, // 10 lexeme or number
        {11, 11, 11, 14, 12, 11, 11, 11, 11, 11},// 11 # scipper
        {11, 11, 11, 11, 12, 12, 11, 11, 11, 11},// 12 # scipper
        {0,  0,  1,  15, 0,  13, 6,  8,  10, 11},// 13 [ \t] scipper
        {0,  0,  1,  14, 0,  15, 6,  8,  10, 11},// 14 [\n] scipper
        {0,  0,  1,  15, 0,  15, 6,  8,  10, 11} // 15 [ \t\n] scipper
};

void (*f[16][10])() = {
        {V,    V,    V,    V,    V,    V,    slex, slex, flex, V},   // 0
        {V,    V,    V,    V,    V,    V,    slex, slex, flex, V},   // 1
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 2
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 3
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 4
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 5
        {addC, addC, addC, addC, addC, addC, slex, addC, addC, addC},// 6
        {addC, addC, addC, addC, addC, addC, addC, addC, addC, addC},// 7
        {addC, addC, addC, addC, addC, addC, addC, slex, addC, addC},// 8
        {addC, addC, addC, addC, addC, addC, addC, addC, addC, addC},// 9
        {flex, flex, flex, flex, flex, flex, flex, flex, addC, flex},// 10
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 11
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},   // 12
        {V,    V,    V,    V,    V,    V,    slex, slex, flex, V},   // 13
        {V,    V,    V,    V,    V,    V,    slex, slex, flex, V},   // 14
        {V,    V,    V,    V,    V,    V,    slex, slex, flex, V}    // 15
};

int go(char c) {
    curChar = c;
    int e = class[128 + c];
    f[curV][e]();
    curV = goV[curV][e];
    return curV;
}

/**
 * Build 'lexems' hashtable,
 *       'mapper' hashtable,
 *       class mapper
 */

void build() {
    hashtable_new(&lexems);
    hashtable_new(&mapper);

    // define classes

    for (int i = 0; i < 256; ++i) {
        class[i] = 0;
    }
    class[128 + '*'] = 1;
    class[128 + '/'] = 2;
    class[128 + '\n'] = 3;
    class[128 + '\0'] = 3;
    class[128 + '\r'] = 3;
    class[128 + '\\'] = 4;
    class[128 + '\t'] = 5;
    class[128 + ' '] = 5;
    class[128 + '\"'] = 6;
    class[128 + '\''] = 7;
    for (char c = 'a'; c <= 'z'; ++c) {
        class[128 + c] = 8;
    }
    for (char c = 'A'; c <= 'Z'; ++c) {
        class[128 + c] = 8;
    }
    for (char c = '0'; c <= '9'; ++c) {
        class[128 + c] = 8;
    }
    class[128 + '_'] = 8;
    class[128 + '#'] = 9;

}

#endif