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
huff_node *create_huffman_node() {}

huff_tree *create_huffman_tree(FILE *stream) {
        huff_tree *tmp = malloc(sizeof(huff_tree));
        char *buffer = get_buffer_from_file(stream);
        hashtable *char_frequency = create_char_frequency_table(buffer);

        for (int i = 0; i < char_frequency->capacity; i++) {
                if (char_frequency->elements[i] != NULL) {
                }
        }
        tmp->tree_string = tree_to_string(tmp->root);

        return tmp;
}
unsigned long int hash(const char *letra, size_t size) {
        return letra[0] % size;
}

hashtable *create_char_frequency_table(char *buffer) {
        hashtable *ht = hashtable_create(256, &hash);
        for (int i = 0; i < strlen(buffer); i++) {
                if (hashtable_lookup(ht->elements, buffer[i]) == NULL) {
                        huff_node *tmp = create_huffman_node();
                        hashtable_insert(ht, *buffer[i], tmp);
                }
        }

        return ht;
}

static char *tree_to_string(huff_node *root);
static char *get_buffer_from_file(FILE *stream);

static huff_node *merge_nodes(huff_node *left, huff_node *right);

static void insert_node(huff_tree *htree, huff_node *hnode);
