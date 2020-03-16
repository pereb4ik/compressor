#include <stdio.h>
#include <stdlib.h>

#define FL FILE*

int fileSize(FILE *f) {
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

static int curSize = 0;

char *removeComments(char *str, int size) {
    char *out = (char *) malloc((size + 1) * sizeof(char));
    int j = 0;
    for (int i = 0; i < size; ++i) {
        // skip quotes
        if (str[i] == '\"') {
            out[j++] = str[i];
            i++;
            while (str[i] != '\"' && i < size) {
                out[j++] = str[i];
                i++;
            }
            out[j++] = str[i];
            continue;
        }
        // skip quotes
        if (str[i] == '\'') {
            out[j++] = str[i];
            i++;
            while (str[i] != '\'') {
                out[j++] = str[i];
                i++;
            }
            out[j++] = str[i];
            continue;
        }

        if (str[i] == '/') {
            /**
             * We think, that code in file is correct
             * (all / is parts of comments)
             */
            i++;

            if (str[i] == '*') {
                i++;
                // this code accept "/*/" like comments
                while (i < size && (str[i - 1] != '*' || str[i] != '/')) {
                    i++;
                }
                continue;
            }

            if (str[i] == '/') {
                i++;
                // here you should realize "\" transfer
                // (bad transfer, work only with "\"+"\n")
                while (i < size && (str[i - 1] == '\\' || str[i] != '\n')) {
                    i++;
                }
                continue;
            }

        } else {
            out[j++] = str[i];
        }
    }
    out[j] = '\0';
    curSize = j;
    return out;
    //out[size] = '\0';
}

char *removeDelimers(char *str, int size) {
    //skip lines with #
}


int main() {
    FILE *input = fopen("kek.txt", "rt");
    //FILE *output = fopen("lel.txt", "wt");
    //int size = fileSize(input);
    int size = fileSize(input);
    char *str = (char *) malloc((size + 1) * sizeof(char));
    //fgets(str, size + 1, input);
    fread(str, size + 1, 1, input);
    //fprintf(input, "hehmda");
    printf("%s", removeComments(str, size));
    printf("%d", "kekch");
    //fclose(input);
    return 0;
}
