#include "collections/hashtable.h"
#include "utils.c"

#ifndef COMPRESSOR_LEXER_C
#define COMPRESSOR_LEXER_C

#define MAX_LEXEM_LEN 1000

// Class of character by ascii code
int class[256];

int v = 0;

// Table of all lexemes(tokens+string constants) in all files
HashTable *lexems;

// Table of lexeme defines
HashTable *mapper;

// Buffer for lexeme
char *buff;
char curChar;
// Current index in buff
int ind;
int buffSize = 10;

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
 * Graph of finite state automata of lexer
 */
int goV[16][10] = {
        {0,  0,  1,  14, 0,  13, 6,  8,  10, 11},// 0 start Vertex
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

void addC();

void V();

void flex();

void slex();

/**
 * Actions on edge pass
 */
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

void countLexeme(char *token) {
    int *count;
    if (hashtable_get(lexems, token, (void **) (&count)) == CC_OK) {
        (*count)++;
    } else {
        count = allocInt();
        *count = 1;
        hashtable_add(lexems, token, count);
    }
}

void startLexem() {
    buffSize = 50;
    buff = allocString(buffSize + 1);
    ind = 0;
}

// Here starts transition functions

// Add char to buff(of lexem)
void addC() {
    if (ind >= buffSize) {
        buffSize = buffSize * 2;
        char *bf = allocString(buffSize + 1);
        buff[ind] = '\0';
        strcpy(bf, buff);
        buff = bf;
    }
    buff[ind++] = curChar;
}

// String lex, copypaste of flex
// start/end string const
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

// lex start/end lexeme
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

void V() {}

// Going over the graph from vertex to another by char class
int go(char c) {
    curChar = c;
    int e = class[128 + c];
    f[v][e]();
    v = goV[v][e];
    return v;
}

// Build 'lexems' hashtable, 'mapper' hashtable, class mapper
void build() {
    hashtable_new(&lexems);
    hashtable_new(&mapper);

    // Define classes

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

#endif // COMPRESSOR_LEXER_C