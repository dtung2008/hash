/*
Copyright © 2020 David Tung.
record.c provide examples of how to use struct with hash.c and hash.h.

see readme.md for more information.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"

void recdump(Hash h);

#define HASHSIZE 100

typedef struct info {
    int age;
    int grader;
} info_t;

typedef struct record {
    char *name;
    info_t *data; 
} record_t;

info_t *recdup(info_t *data) {
    info_t *p = malloc(sizeof(info_t));
    if (p != NULL)
        *p = *data;
    return p;
}

int main() {
    info_t data[] = {{.age = 10, .grader = 4},
                     {.age = 16, .grader = 10},
                     {.age = 17, .grader = 11},
                     {.age = 13, .grader = 6}};
    record_t rec[] = {{.name = "James", .data = &data[0]},
                      {.name = "Noah", .data = &data[1]},
                      {.name = "Isabella", .data = &data[2]},
                      {.name = "Ava", .data = &data[3]}};
    int rec_size = sizeof(rec)/sizeof(rec[0]);

    /* no customized free is needed for info_t */
    Hash map = create(HASHSIZE, (Hashdup) recdup, NULL);

    for (int i = 0; i < rec_size; i++)
        if (insert(map, rec[i].name, rec[i].data) == NULL) {
            fprintf(stderr, "memory allocation error\n");
            return 1;
        }

    /* test hash table */
    info_t *p = lookup(map, "Ava");
    printf("name: %s, age: %d, grade: %d\n", "Ava", p->age, p->grader);
    /* update struct data directly */
    p->age++; p->grader++;
    p = lookup(map, "Ava");
    printf("name: %s, age: %d, grade: %d\n", "Ava", p->age, p->grader);

    recdump(map);

    destroy(map);
    return 0;
}

void recdump(Hash h) {
   int i = 0;
   printf("{");
   for (Hnode np = firsthnode(h); np; np = nexthnode(h)) {
      info_t *p = np->value;
      printf("%s{name: %s, age: %d, grade: %d}", i++ ? ", ": "", np->key, p->age, p->grader);
   }
   printf("}\n");
}

