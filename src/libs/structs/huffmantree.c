#include "huffmantree.h"
#include "../file_functions.h"
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

typedef struct huffman_node {
        char character;
        int frequency;
        struct huffman_node *next;
        struct huffman_node *left;
        struct huffman_node *right;
} HN;

typedef struct huffman_tree {
        HN *root;
        hashtable *compressed_table;
        char *tree_string;
} HT;

HN *create_huffman_node(entry *data) {
        HN *tmp = malloc(sizeof *tmp);
        tmp->character = data->key[0];
        tmp->frequency = *(int *)data->object;
        tmp->next = NULL;
        tmp->left = NULL;
        tmp->right = NULL;
        return tmp;
}

HT *create_huffman_tree(FILE *stream) {
        HT *tmp = malloc(sizeof(HT));
        unsigned char *buffer = get_buffer_from_file(stream);
        hashtable *char_frequency = create_char_frequency_table(&buffer);

        for (int i = 0; i < char_frequency->capacity; i++) {
                if (char_frequency->elements[i] != NULL) {
                        create_huffman_node(char_frequency->elements[i]);
                }
        }
        // tmp->tree_string = tree_to_string(tmp->root);

        return tmp;
}

unsigned long int hash(const char *letra, size_t size) {
        return letra[0] % size;
}

static char *tree_to_string(HN *root);
static char *get_buffer_from_file(FILE *stream);

static HN *merge_nodes(HN *left, HN *right);

// TODO: Test function
static void insert_sorted(HT *htree, HN *hnode) {
        HN *curr = htree->root;
        if (curr == NULL || curr->frequency > hnode->frequency) {
                hnode->next = curr;
                htree->root = hnode;
                return;
        }

        while (curr->next != NULL) {
                if (curr->next->frequency < hnode->frequency) {
                        continue;
                }
                if (curr->next->frequency >= hnode->frequency) {
                        hnode->next = curr->next;
                        curr->next = hnode;
                        return;
                } else if (!curr->next) {
                        curr->next = hnode;
                        return;
                }
                curr = curr->next;
        }
}
