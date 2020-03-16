#include "ALL_DEFINES.h"
#include <stdio.h>
#include <stdlib.h>

#define FL FILE*

int fileSize(FILE *f) {
    fseek(f, 0, SEEK_END);
    int b = ftell(f);
    fseek(f, 0, SEEK_SET);
    return b;
}

static int curSize = 0;

c *removeComments(c *a, int b) {
    c *out = (c *) malloc((b + 1) * sizeof(c));
    int j = 0;
    for (int i = 0; i < b; ++i) {
                if (a[i] == '\"') {
            out[j++] = a[i];
            i++;
            while (a[i] != '\"' && i < b) {
                out[j++] = a[i];
                i++;
            }
            out[j++] = a[i];
            d;
        }
                if (a[i] == '\'') {
            out[j++] = a[i];
            i++;
            while (a[i] != '\'') {
                out[j++] = a[i];
                i++;
            }
            out[j++] = a[i];
            d;
        }

        if (a[i] == '/') {
            
            i++;

            if (a[i] == '*') {
                i++;
                                while (i < b && (a[i - 1] != '*' || a[i] != '/')) {
                    i++;
                }
                d;
            }

            if (a[i] == '/') {
                i++;
                                                while (i < b && (a[i - 1] == '\\' || a[i] != '\n')) {
                    i++;
                }
                d;
            }

        } else {
            out[j++] = a[i];
        }
    }
    out[j] = '\0';
    curSize = j;
    return out;
    }

c *removeDelimers(c *a, int b) {
    }


int main() {
    FILE *input = fopen("kek.txt", "rt");
            int b = fileSize(input);
    c *a = (c *) malloc((b + 1) * sizeof(c));
        fread(a, b + 1, 1, input);
        printf("%s", removeComments(a, b));
    printf("%d", "kekch");
        return 0;
}
