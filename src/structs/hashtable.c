#include "hashtable.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 256
#define TABLE_SIZE 10

#define DELETED_ITEM (void *)(0xFFFFFFFFFFFFFFFFUL)

struct hashtable {
        int capacity;
        void **table;
};

unsigned int hash(char *key) {
        int length = strnlen(key, MAX_NAME);
        unsigned int hash_value = 0;
        for (int i = 0; i < length; i++) {
                hash_value += key[i];
                hash_value = (hash_value * key[i]) % TABLE_SIZE;
        }
        return hash_value;
}

hashtable *create_hashtable(int capacity) {
        hashtable *ht = malloc(sizeof *ht);
        ht->table = malloc(sizeof(void *) * capacity);

        for (int i = 0; i < capacity; i++) {
                ht->table[i] = NULL;
        }

        return ht;
}

// TODO: deal with conflicts
void hashtable_insert(hashtable *ht, char *key, void *value) {
        int index = hash(key);
        ht->table[index] = value;
}

bool hashtable_delete(hashtable *ht, char *key) {
        if (ht->table[hash(key)] != NULL) {
                ht->table[hash(key)] = DELETED_ITEM;
                return true;
        }

        return false;
}
