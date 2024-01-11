#include "hashtable.h"
#include <stdio.h>

#ifndef HUFFTREE_H
#define HUFFTREE_H

typedef struct huffman_tree huff_tree;
typedef struct huffman_node huff_node;

huff_tree *create_huffman_tree(FILE *stream);

static char *tree_to_string(huff_node *root);
static char *get_buffer_from_file(FILE *stream);

static hashtable *create_char_frequency_table(char *buffer);
static huff_node *merge_nodes(huff_node *left, huff_node *right);

static void insert_node(huff_tree *htree, huff_node *hnode);

#endif
