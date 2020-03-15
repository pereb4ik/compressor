#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BEST_UTILS_H
#define BEST_UTILS_H


int fileSize(FILE *f) {
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}

char *concat(char *s1, char *s2) {
    char *buff = calloc(strlen(s1) + strlen(s2) + 1, 1);
    //please free this
    strcat(buff, s1);
    strcat(buff, s2);
    return buff;
}

#endif