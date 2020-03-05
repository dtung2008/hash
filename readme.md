# A note of hash.c and hash.h

by David Tung Mar. 2020

hash.c and hash.h provide functions for using hash tables in C. It only supports string (char array) for key, but users can choose whatever data type as value.

## How to use the package

### 1. Create a Hash type

    Create an instance of hash with

    Hash create(int hash_size, Hashdup dupfunc, Hashfree freefunc)
    
    , where hash_size is a hash index size. dupfunc is the duplicate function that provide a duplicate with malloc memory for date. freefunc is the extra free function for the memory user malloc themselves. (See how to write dupfunc and freefunc for more information.) It returns a hash pointer if success, NULL if fail.

### 2. Use lookup to check entry of a hash

    Use

    void *lookup(Hash hash, char* key) 
    
    to check if the hash has the key in the table. If yes, the function returns a pointer to its associated data, otherwise it returns NULL.

### 3. Use insert to enter a new entry into a hash

    Use 
    
    void insert(Hash hash, char *key, void *value)

    value point to your data. The dupfunc and freefunc you provide will be used to duplicate and free data. Package will free the nearest dynamic memory.  

### 4. Two ways to replace existing data, use insert or lookup

    Use insert for convenience, just like insert a new entry.

    Use lookup to get a pointer to your data, you need to free and duplicate data if necessary. If the data is immutable, such as char array, you can not modify the data directly.

### 5. Use del to delete an entry

    Use

    void del(Hash hash, char *key)

    to delete an entry. If the entry does not exist, del will do nothing. Otherwise it removes the key and its assciated data.

### 6. Use destroy to free hash resource

    Use

    void destroy(Hash hash) 
    
    to free the resource of a hash table.

### 7. To loop through all entries in a hash, use firsthnode and nexthnode

    Use

    Hnode firsthnode(Hash hash)

    To get the pointer of the first Hnode of the hash. Hnode is a pointer the the struture contains key and value.

    Use

    Hnode nexthnode(Hash hash)

    To get the pointer of the next Hnode. It return NULL if it reaches the end of the hash.

### 8. How to write a dupfunc

    void *dupfunc(void *data) 
    
    duplicate a copy of your data. We recomend you write it as concrete type and type case to Hashcp when calling. The input must be a point to your data, or just the data if data is an array. For example to copy an int type, you write

    int *intdup(int *i) {
        int *p;

        p = malloc(sizeof(int));
        if (p != NULL)
            *p = *i;
        return p;
    }

    To copy an (char *) type, you can just use strdup as cpfunc.

    I have built a version of intdup in the package, strdup is a function standard library. You need to build other dupfunc by youselves.

### 9. How to write a freefunc

    You need to write a free function for those dynamic memory not removable by freeing the return pointer of lookup. If there is no such memory, just pass NULL to freefunc. Package will free the nearest dynamic memory. For examples, you just pass NULL for int and (char *) data.

## Example codes

test.c shows how to use hash with string (char array) value, and hash with int value.

dup.c show how to use hash with int value to find the duplicate lines from standard input.
