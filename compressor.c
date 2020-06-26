#include <stdio.h>
#include <stdlib.h>
#include "collections/hashtable.h"

#include "renamer.c"

#include "lexer.c"
#include "utils.c"

char *stringShift;
int shiftSize;

// Generating next string in lexicographical order
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
    for (i = shiftSize - 1; i > -1 && stringShift[i] == '_'; --i) {
        stringShift[i] = 'a';
    }
    if (i < 0) {
        shiftSize++;
        char *NewShift = allocString(shiftSize + 1);
        for (int j = 0; j < shiftSize; ++j) {
            NewShift[j] = 'a';
        }
        NewShift[shiftSize] = '\0';
        stringShift = NewShift;
    } else {
        if (i == 0 && stringShift[i] == 'Z') {
            stringShift[i] = '_';
        } else {
            stringShift[i] = next[stringShift[i]];
        }
    }
}

// Lexing one file and storing uniq words to 'lexems'
void lex(int size, char *str) {
    for (int i = 0; i < size; ++i) {
        go(str[i]);
    }
}

/**
 * Calculating profitable replacement to words
 * with adding it to mapper
 */
void buildMapper() {
    // Build stringShift
    stringShift = allocString(2);
    stringShift[0] = 'a';
    stringShift[1] = '\0';
    shiftSize = 1;
    //

    int hsize = hashtable_size(lexems);
    Vertex **vertices = allocVertexArray(hsize);

    char *key;
    int *val;
    int k = 0;
    HASHTABLE_FOREACH(lexems, key, val, {
        vertices[k++] = makeVertex(key, *val);
    })

    qsort(vertices, hsize, sizeof(Vertex *), &comparator);

    int *trash;
    for (int i = hsize - 1; i > -1; i--) {
        while (hashtable_get(lexems, stringShift, (void **) &trash) == CC_OK) {
            nextStringShift();
        }
        // ATTENTION, fx[a] is f(a + 1)
        if (vertices[i]->fx[shiftSize - 1] > 0) {
            char *cur = allocString(shiftSize + 1);
            strcpy(cur, stringShift);
            hashtable_add(mapper, vertices[i]->word, cur);
        } else {
            break;
        }
        nextStringShift();
    }
}

// Read, rename and write
void write(int size, char *str, char *filename) {
    indf = 0;
    v = 0;
    hasTokens = false;
    // Lexing with defining
    for (int i = 0; i < size; ++i) {
        go2(str[i]);
    }
    outFile[indf - 1] = '\n';
    outFile[indf] = '\0';
    FILE *output = fopen(filename, "wt");
    if (hasTokens) {
        fprintf(output, "%s", "#include \"ALL_DEFINES.h\"\n");
    }
    int c;
    int i = 0;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(_WIN64) \
 || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__) || defined(WINNT) \
 || defined(__WINNT) || defined(__WINNT__) || defined(_MSC_VER) || defined(_MSC_FULL_VER)
    while ((c = outFile[i++]) != '\0') {
        if (c == 10) {
            fputc(13, output);
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

// Writing defines to header file
void writeHeader() {
    if (hashtable_size(mapper) > 0) {
        printf("%zu size of mapper\n", hashtable_size(mapper));
        FILE *head = fopen("ALL_DEFINES.h", "wt");
        char *key;
        char *value;
        HASHTABLE_FOREACH(mapper, key, value, {
            fprintf(head, "%s", "#define ");
            fprintf(head, "%s", value);
            fprintf(head, "%s", " ");
            fprintf(head, "%s", key);
            fprintf(head, "%s", "\n");
        })
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

    buildMapper();
    build2(maxSize + 1);

    for (int i = 1; i < argsn; ++i) {
        write(sizes[i] + 1, files[i], args[i]);
    }

    writeHeader();

    freeSpace();
    hashtable_destroy(mapper);
    hashtable_destroy(lexems);

    printf("%s", "You should add ALL_DEFINES.h to includes of compiler\n");
    return EXIT_SUCCESS;
}