#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

int main(int argsn, char *args[]) {
    HashTable *tbl;
    hashtable_new(&tbl);
    hashtable_add(tbl, "lol", 123321);
    hashtable_add(tbl, "trol", 12321);
    hashtable_add(tbl, "a", 12312312312312312313L);
    int *heh;
    hashtable_get(tbl, "lol", &heh);
    printf("%d\n", heh);
    hashtable_get(tbl, "trol", &heh);
    printf("%d\n", heh);
    hashtable_get(tbl, "a", &heh);
    printf("%d\n", heh);
    return 0;
}