/*
Copyright © 2020 David Tung.
hash.c and hash.h provide functions for hash table in C. 
It only supports string (char array) for key, use users 
can choose whatever data type as value.

see readme.md for more information.
*/
#ifndef HASH_H
#define HASH_H

typedef void *(*Hashdup)(void *value);  // define duplicate type
typedef void (*Hashfree)(void *value);  // define free type

typedef struct hnode *Hnode;

struct hnode
{
    Hnode next;
    char *key;   // only string key
    void *value; // define your own value data
};

typedef struct hash
{
    Hnode *htbl;    // hash table
    int hsize;      // adjustable hash index size
    int hindex;     // current working hash index
    Hnode hnodePtr; // current working hnode index
    Hashdup dup;     // register duplicate method for your data
                     // must copy to a fresh memory
                     // see strdup and intdup for examples
    Hashfree free; // register free method for extra dynamic memory
                   // use NULL for data type without underlying pointers
} *Hash;

Hash create(int hashSize, Hashdup dup, Hashfree free);

void *insert(Hash h, char *key, void *value);

void *lookup(Hash h, char *key);

void del(Hash h, char *key);

Hnode gethnode(Hash h, char *key);

Hnode firsthnode(Hash h);

Hnode nexthnode(Hash h);

void destroy(Hash h);

int *intdup(int *i);
#endif
