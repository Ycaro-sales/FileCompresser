#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct hashtable hashtable;
typedef uint64_t(hashfunction)(const char *, size_t);

hashtable *hashtable_create(int capacity, hashfunction *hash);

bool hashtable_insert(hashtable *ht, char *key, void *value);
bool hashtable_delete(hashtable *ht, char *key);
void *hashtable_lookup(hashtable *ht, const char *key);

#endif
