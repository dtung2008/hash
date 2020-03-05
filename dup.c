/*
Copyright © 2020 David Tung.
dup.c provide examples of how to use hash.c and hash.h.

see readme.md for more information.
*/
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define MAXLINE 1000
#define HASHSIZE 100

int main() {
    char s[MAXLINE];
    int one = 1;
    int *p;

    Hash counts = create(HASHSIZE, (Hashdup) intdup, NULL);
    while (fgets(s, MAXLINE, stdin) != NULL)
        if ((p = lookup(counts, s)) == NULL)
            insert(counts, s, &one);
        else
            (*p)++;

    for (Hnode np = firsthnode(counts); np != NULL; np = nexthnode(counts)) {
        p = np->value;
        if (*p > 1)
            printf("%3d %s", *p, np->key);
    }

    destroy(counts);
    return 0;
}
