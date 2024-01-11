#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef uint64_t(hashfunction)(const char *, size_t);

typedef struct entry {
        char *key;
        void *object;
        struct entry *next;
} entry;

typedef struct hashtable {
        int capacity;
        entry **elements;
        hashfunction *hash;
} hashtable;

hashtable *hashtable_create(int capacity, hashfunction *hash);

bool hashtable_insert(hashtable *ht, char *key, void *value);
bool hashtable_delete(hashtable *ht, char *key);
void *hashtable_lookup(hashtable *ht, const char *key);

#endif
