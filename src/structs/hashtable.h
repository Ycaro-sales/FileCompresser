#include <stdbool.h>

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct hashtable hashtable;

unsigned int hash(char *key);
hashtable *create_hashtable(int capacity);

void hashtable_insert(hashtable *ht, char *key, void *value);
bool hashtable_delete(hashtable *ht, char *key);

#endif
