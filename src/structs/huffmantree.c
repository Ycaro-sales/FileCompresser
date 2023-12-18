#include "huffmantree.h"
#include "hashtable.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UCHAR_AMOUNT 256

struct huffman_node {
        char character;
        int frequency;
        huff_node *next;
        huff_node *left;
        huff_node *right;
};

struct huffman_tree {
        huff_node *root;
        hashtable *compressed_table;
        char *tree_string;
};
huff_node *create_huffman_node(entry *data) {
        huff_node *tmp = malloc(sizeof *tmp);
        tmp->character = data->key[0];
        tmp->frequency = *(int *)data->object;
        tmp->next = NULL;
        tmp->left = NULL;
        tmp->right = NULL;
        return tmp;
}

huff_tree *create_huffman_tree(FILE *stream) {
        huff_tree *tmp = malloc(sizeof(huff_tree));
        char *buffer = get_buffer_from_file(stream);
        hashtable *char_frequency = create_char_frequency_table(buffer);

        for (int i = 0; i < char_frequency->capacity; i++) {
                if (char_frequency->elements[i] != NULL) {
                        insert_node(tmp, char_frequency->elements[i]->object);
                }
        }
        // tmp->tree_string = tree_to_string(tmp->root);

        return tmp;
}
unsigned long int hash(const char *letra, size_t size) {
        return letra[0] % size;
}

hashtable *create_char_frequency_table(char *buffer) {
        hashtable *ht = hashtable_create(256, &hash);
        for (int i = 0; i < strlen(buffer); i++) {
                if (hashtable_lookup(ht, &buffer[i]) == NULL) {
                        huff_node *tmp = create_huffman_node(ht->elements[i]);
                        hashtable_insert(ht, &buffer[i], tmp);
                }
        }

        return ht;
}

static char *tree_to_string(huff_node *root);
static char *get_buffer_from_file(FILE *stream);

static huff_node *merge_nodes(huff_node *left, huff_node *right);

static void insert_node(huff_tree *htree, huff_node *hnode) {
        huff_node *curr = htree->root;
        if (curr == NULL) {
                htree->root = hnode;
                return;
        }

        // TODO: sorted insert
        while (curr->next != NULL) {
                if (curr->next->frequency == hnode->frequency) {
                        hnode->next = curr->next;
                        curr->next = hnode;
                        return;
                }
                else if (curr->next->frequency < hnode->frequency{

                }
                curr = curr->next;
        }
}
