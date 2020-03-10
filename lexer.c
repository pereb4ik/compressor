#include <form.h>
#include "hashtable.h"

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

int curV;

void (*f[])();

HashTable *lexems;


/// very big C in O(n)

void countLexeme(char *token) {
    int *count = 0;
    if (hashtable_get(lexems, token, &count) == CC_OK) {
        count++;
    } else {
        count = 1;
    }
    hashtable_add(lexems, token, count);
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
 * please, add space/tab scipper
 * maybe add EOF???
 */
int goV[13][10] = {
        {0,  0,  1,  0,  0,  0,  6,  8,  10, 0}, // 0 start vertex
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

int go(char c) {
    int e = class[c];
    f[e]();
    curV = goV[curV][e];
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



void build() {
    hashtable_new(&lexems);

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