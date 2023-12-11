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

static size_t hashtable_index(hashtable *ht, const char *key) {
        size_t result = (ht->hash(key, strlen(key)) % ht->capacity);
        return result;
}

hashtable *hashtable_create(int capacity, hashfunction *hf) {
        hashtable *ht = malloc(sizeof *ht);

        ht->capacity = capacity;
        ht->elements = calloc(sizeof(entry *), capacity);
        ht->hash = hf;

        return ht;
}

// TODO: deal with conflicts
bool hashtable_insert(hashtable *ht, char *key, void *obj) {
        if (key == NULL || ht == NULL)
                return false;
        size_t index = hashtable_index(ht, key);

        if (hashtable_lookup(ht, key) != NULL)
                return false;

        entry *e = malloc(sizeof *e);

        e->object = obj;
        e->key = malloc(strlen(key) + 1);
        strcpy(e->key, key);

        e->next = ht->elements[index];
        ht->elements[index] = e;

        return true;
}

void *hashtable_lookup(hashtable *ht, const char *key) {
        if (key == NULL || ht == NULL)
                return false;
        size_t index = hashtable_index(ht, key);

        entry *tmp = ht->elements[index];
        while (tmp != NULL && strcmp(tmp->key, key) != 0) {
                tmp = tmp->next;
        }
        if (tmp == NULL)
                return NULL;
        return tmp->object;
}

bool hashtable_delete(hashtable *ht, char *key) {
        if (key == NULL || ht == NULL)
                return false;
        size_t index = hashtable_index(ht, key);

        entry *tmp = ht->elements[index];
        entry *prev = NULL;

        while (tmp != NULL && strcmp(tmp->key, key) != 0) {
                prev = tmp;
                tmp = tmp->next;
        }

        if (tmp == NULL)
                return NULL;
        if (prev == NULL) {
                ht->elements[index] = tmp->next;
        } else {
                prev->next = tmp->next;
        }

        void *result = tmp->object;
        free(tmp);

        return result;
}
