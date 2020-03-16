#include <stdio.h>
#include <stdlib.h>

#include "utils.c"

int curSize;
char *curShift;

void nextStringShift() {
    static int first = 1;
    static char next[256];
    if (first) {
        first = 0;
        for (char c = 'a'; c < 'z'; ++c) {
            next[c] = c + 1;
        }
        next['z'] = 'A';
        for (char c = 'A'; c < 'Z'; ++c) {
            next[c] = c + 1;
        }
        next['Z'] = '0';
        for (char c = '0'; c < '9'; ++c) {
            next[c] = c + 1;
        }
        next['9'] = '_';
        next['a' - 1] = 'a';
    }

    int i = curSize - 1;
    while (i > -1 && curShift[i] == '_') {
        curShift[i] = 'a';
        i--;
    }
    if (i < 0) {
        curSize++;
        char *NewShift = allocstring(curSize);
        for (int j = 0; j < curSize; ++j) {
            NewShift[j] = 'a';
        }
        curShift = NewShift;
    } else {
        if (i == 0 && curShift[i] == 'Z') {
            curShift[i] = '_';
        } else {
            curShift[i] = next[curShift[i]];
        }
    }
}

int main() {
    //build curShift

    curShift = allocstring(2);
    curShift[0] = 'a' - 1;
    curShift[1] = '\0';

    curSize = 1;
    //
    for (int i = 0; i < 10000; ++i) {
        nextStringShift();
        printf("%s\n", curShift);
    }
    destroyStrings();
    return 0;
}