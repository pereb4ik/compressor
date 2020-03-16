#include <stdlib.h>
#include <form.h>
#include "collections/hashtable.h"
#include "utils.c"

#ifndef BEST_LEXER_C
#define BEST_LEXER_C


#define long long long

const int NumOfSamples = 5;

typedef struct {
    char *str;
    long fx[NumOfSamples];
    int count;
    int len;
} vertex;

vertex *makeVert(char *str, long count) {
    vertex p;
    p.count = count;
    p.str = str;
    int len = strlen(str);
    p.len = len;
    for (int i = 0; i < NumOfSamples; ++i) {
        p.fx[i] = count * (len - (i + 1)) - (10 + len + (i + 1));
    }
    return &p;
}

int compare(vertex *a, vertex *b) {
    for (int i = 0; i < NumOfSamples; ++i) {
        if (a->fx[i] < b->fx[i]) {
            return -1;
        } else {
            if (a->fx[i] > b->fx[i]) {
                return 1;
            }
        }
    }
    return 0;
}


int class[256];

int curV = 0;

HashTable *lexems;

HashTable *mapper;


/**
 * transition functions
 */

/// very big C in O(n)

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

void startLexem() {
    // you should free this space
    buff = allocstring(1000);
    ind = 0;
}


//fuck yeah string lex
//copypaste of flex
void slex() {
    static int even = 1;
    even = 1 - even;
    if (even == 0) {
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
    static int even = 1;
    even = 1 - even;
    if (even == 0) {
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
 * 3 [\n]
 * 4 [\]
 * 5 [ \t]
 * 6 ["]
 * 7 [']
 * 8 [a-zA-Z0-9_]
 * 9 [#]
 */

/**
 * please, add space/tab scipper
 * maybe add EOF???
 */
int goV[13][10] = {
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

void (*f[13][10])() = {
        {V,    V,    V,    V,    V,    V,    slex, slex, flex, V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {addC, addC, addC, addC, addC, addC, slex, addC, addC, addC},
        {addC, addC, addC, addC, addC, addC, addC, addC, addC, addC},
        {addC, addC, addC, addC, addC, addC, addC, slex, addC, addC},
        {addC, addC, addC, addC, addC, addC, addC, addC, addC, addC},
        {flex, flex, flex, flex, flex, flex, flex, flex, addC, flex},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V},
        {V,    V,    V,    V,    V,    V,    V,    V,    V,    V}
};

int go(char c) {
    curChar = c;
    int e = class[128 + c];
    f[curV][e]();
    curV = goV[curV][e];
    //printf("%d\n", curV);
    return curV;
}

/**
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

/**
 * Build 'lexems' hashtable,
 *       class mapper,
 *       'mapper' hashtable
 *
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


    goV[0][2] = 1;
    goV[1][1] = 2;
    goV[2][1] = 3;
}

#endif