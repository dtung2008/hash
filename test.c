/*
Copyright © 2020 David Tung.
test.c provide examples of how to use hash.c and hash.h.

see readme.md for more information.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

void strdump(Hash h);
void intdump(Hash h);

#define HASHSIZE 100

int main() {
   // create hash with hash size and the copy and free method of your value type 
   Hash h = create(HASHSIZE, (Hashdup) strdup, NULL);
   // insert the first key
   insert(h, "greeting", "hello");
   char *p;
   if ((p = lookup(h, "greeting")) != NULL)
      printf("%s\n", p);
   // update value with the same key
   insert(h, "greeting", "aloha");
   strdump(h);
   // insert different key
   insert(h, "leaving", "bye");
   strdump(h);
   // delete an entry
   del(h, "greeting");
   strdump(h);
   destroy(h);

   Hash g = create(HASHSIZE, (Hashdup) intdup, NULL);
   int i = 10;
   insert(g, "account", &i);
   int *q;
   if ((q = lookup(g, "account")) != NULL) {
      printf("%d\n", *q);
      // update value directly if mutable
      *q += 100;
      intdump(g);
   }
   destroy(g);
   
   return 0;
}


void strdump(Hash h) {
   int i = 0;
   printf("{");
   for (Hnode np = firsthnode(h); np != NULL; np = nexthnode(h))
      printf("%s%s: %s", i++ ? ", " : "", np->key, (char *)np->value);
   printf("}\n");
}

void intdump(Hash h) {
   int i = 0;
   printf("{");
   for (Hnode np = firsthnode(h); np != NULL; np = nexthnode(h))
      printf("%s%s: %d", i++ ? ", " : "" , np->key, *(int *)np->value);
   printf("}\n");
}