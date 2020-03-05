/*
Copyright © 2020 David Tung.
hash.c and hash.h provide functions for hash table in C. 
It only supports string (char array) for key, but users 
can choose whatever data type as value.

see readme.md for more information.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"

static unsigned hash(char *s, int hsize);
static Hnode gethnodeEx(Hash hp, char *key, Hnode prv);
static void freeValue(Hash hp, void *value); 
static void freeHnode(Hnode);
static void freeHash(Hash hp);

unsigned hash(char *s, int hsize)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % hsize;
}

Hash create(int hsize, Hashdup dup, Hashfree free) {
    Hash hp = (Hash)malloc(sizeof(struct hash));
    hp->free = free;
    hp->hsize = hsize;
    hp->dup = dup;
    hp->free = free;

    hp->hindex = 0;;
    hp->htbl = (Hnode *)malloc(hsize*sizeof(Hnode));
    if (hp->htbl == NULL)
        return NULL;
    hp->hnodePtr = hp->htbl[0];
    for (int i = 0; i < hsize; i++)
        hp->htbl[i] = NULL;

    return hp;
}

void *lookup(Hash hp, char *key) {
    Hnode np;

    np = gethnode(hp, key);
    if (np == NULL)
        return NULL;

    return np->value;
}

Hnode gethnode(Hash hp, char *key) {
    Hnode np;

    for (np = hp->htbl[hash(key, hp->hsize)]; np != NULL; np = np->next) {
        if (strcmp(key, np->key) == 0)
            return np;
    }
    return NULL;
}

Hnode nexthnodehelp(Hash hp) {
    while (hp->hnodePtr == NULL) {
        hp->hindex++;
        if (hp->hindex < hp->hsize)
            hp->hnodePtr = hp->htbl[hp->hindex];
        else
        {
            hp->hindex = 0;
            hp->hnodePtr = hp->htbl[0];
            return NULL;
        }
    }
    return hp->hnodePtr;
}

Hnode firsthnode(Hash hp) {
    hp->hindex = 0;
    hp->hnodePtr = hp->htbl[0];
    return nexthnodehelp(hp);
}

Hnode nexthnode(Hash hp) {
    hp->hnodePtr = hp->hnodePtr->next;
    return nexthnodehelp(hp);
}

Hnode gethnodeEx(Hash hp, char *key, Hnode prv) {
    Hnode np;

    for (np = hp->htbl[hash(key, hp->hsize)]; np != NULL; np = np->next)
    {
        if (strcmp(key, np->key) == 0)
            return np;
        prv = np;
    }
    return NULL;
}

void *insert(Hash hp, char *key, void *value) {
    Hnode np;
    unsigned hashval;
    void *t;
    int freeit = 0;

    if ((np = gethnode(hp, key)) == NULL) {
        np = (Hnode)malloc(sizeof(struct hnode));
        if (np == NULL || (np->key = strdup(key)) == NULL)
            return NULL;
        hashval = hash(key, hp->hsize);
        np->next = hp->htbl[hashval];
        hp->htbl[hashval] = np;
    } else {
        freeit = 1;
        t = np->value;  // delay free after copy
    }
    np->value = hp->dup(value);
    if (freeit)
        freeValue(hp, t);
    return np->value;
}

void del(Hash hp, char *key) {
    Hnode np, prv;

    prv = NULL;
    if ((np = gethnodeEx(hp, key, prv)) == NULL)
        return;
    if (prv == NULL) {  // delete the first node
        hp->htbl[hash(key, hp->hsize)] = np->next;
    } else {
        prv->next = np->next;
    }
    freeValue(hp, np->value);
    freeHnode(np);
}

void destroy(Hash hp) {
    int i;
    Hnode np;

    if (hp == NULL)
        return;
    for (i = 0; i < hp->hsize; i++)
        for (np = hp->htbl[i]; np != NULL; np = hp->htbl[i]){
            hp->htbl[i] = np->next;
            freeValue(hp, np->value);
            freeHnode(np);
        }
    freeHash(hp);
}

void freeValue(Hash hp, void *value) {
    if (hp->free != NULL)
        hp->free(value);
    free(value);
}

void freeHnode(Hnode np) {
    if (np == NULL)
        return;
    free((void *)np->key);
    free((void *)np);
}

void freeHash(Hash hp) {
    if (hp == NULL)
        return;
    free((void *)hp->htbl);
    free((void *)hp);
}

int *intdup(int *i) {
    int *p;

    p = malloc(sizeof(int));
    if (p != NULL)
        *p = *i;
    return p;
}