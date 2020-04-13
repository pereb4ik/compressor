#include <stdio.h>
#include <stdlib.h>
#include "collections/hashtable.h"

#include "lexer2.c"

#include "lexer.c"
#include "utils.c"

int curSize;
char *curShift;

void nextStringShift() {
    static int firstRun = 1;
    static char next[256];
    if (firstRun) {
        firstRun = 0;
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
    }

    int i;
    for (i = curSize - 1; i > -1 && curShift[i] == '_'; --i) {
        curShift[i] = 'a';
    }
    if (i < 0) {
        curSize++;
        char *NewShift = allocstring(curSize + 1);
        for (int j = 0; j < curSize; ++j) {
            NewShift[j] = 'a';
        }
        NewShift[curSize] = '\0';
        curShift = NewShift;
    } else {
        if (i == 0 && curShift[i] == 'Z') {
            curShift[i] = '_';
        } else {
            curShift[i] = next[curShift[i]];
        }
    }
}

/**
 * First lex
 */
void lex(int size, char *str) {
    for (int i = 0; i < size; ++i) {
        go(str[i]);
    }
}

/**
 * Build mapper
 */
void calck() {
    //build curShift
    curShift = allocstring(2);
    curShift[0] = 'a';
    curShift[1] = '\0';
    curSize = 1;
    //

    int hsize = hashtable_size(lexems);
    //vertex *vert[hsize];
    vertex **vert = allocVertArray(hsize);
    HashTableIter iter;
    hashtable_iter_init(&iter, lexems);
    TableEntry *cur;

    for (int i = 0; hashtable_iter_next(&iter, &cur) != CC_ITER_END; ++i) {
        int *val = cur->value;
        vert[i] = makeVert(cur->key, *val);
    }

    qsort(vert, hsize, sizeof(vertex *), &comparator);

    int *trash;
    for (int i = hsize - 1; i > -1; i--) {
        while (hashtable_get(lexems, curShift, (void **) &trash) == CC_OK) {
            nextStringShift();
        }
        // ATTENTION, fx[a] is f(a + 1)
        if (vert[i]->fx[curSize - 1] > 0) {
            char *cur = allocstring(curSize + 1);
            strcpy(cur, curShift);
            hashtable_add(mapper, vert[i]->str, cur);
        } else {
            break;
        }
        nextStringShift();
    }
}

/**
 * Read, define and write
 */
void write(int size, char *str, char *filename) {
    indf = 0;
    curV = 0;
    hasTokens = false;
    for (int i = 0; i < size; ++i) {
        go2(str[i]);
    }
    outFile[indf - 1] = '\n';
    outFile[indf] = '\0';
    FILE *output = fopen(filename, "wt");
    if (hasTokens) {
        fprintf(output, "%s", "#include \"ALL_DEFINES.h\"\n");
    }
    int c = 0;
    int i = 0;
    int ln = 10;
    int lr = 13;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(_WIN64) \
 || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__) || defined(WINNT) \
 || defined(__WINNT) || defined(__WINNT__) || defined(_MSC_VER) || defined(_MSC_FULL_VER)
    while ((c = outFile[i++]) != '\0') {
        if (c == ln) {
            fputc(lr, output);
        }
        fputc(c, output);
    }
#else
    while ((c = outFile[i++]) != '\0') {
        fputc(c, output);
    }
#endif
    fclose(output);
}

/**
 * Write defines to file
 */
void writeHeader() {
    if (hashtable_size(mapper) > 0) {
        printf("%zu size of mapper\n", hashtable_size(mapper));
        FILE *head = fopen("ALL_DEFINES.h", "wt");
        HashTableIter iter;
        hashtable_iter_init(&iter, mapper);
        TableEntry *cur;
        while (hashtable_iter_next(&iter, &cur) != CC_ITER_END) {
            fprintf(head, "%s", "#define ");
            fprintf(head, "%s", (char *) (cur->value));
            fprintf(head, "%s", " ");
            fprintf(head, "%s", (char *) (cur->key));
            fprintf(head, "%s", "\n");
        }
        fclose(head);
    }
}

int main(int argsn, char *args[]) {
    build();
    char **files = allocStringArray(argsn);
    int *sizes = allocIntArray(argsn);
    int maxSize = 0;

    for (int i = 1; i < argsn; ++i) {
        sizes[i] = fileSize(args[i]);
        maxSize = mx(maxSize, sizes[i]);
        files[i] = readFile(args[i], sizes[i]);
        lex(sizes[i] + 1, files[i]);
    }

    calck();
    build2(maxSize + 1);

    for (int i = 1; i < argsn; ++i) {
        write(sizes[i] + 1, files[i], args[i]);
    }

    writeHeader();

    //destroy all
    freeSpace();
    hashtable_destroy(mapper);
    hashtable_destroy(lexems);

    printf("%s", "You should add ALL_DEFINES.h to includes of compiler\n");
    return EXIT_SUCCESS;
}