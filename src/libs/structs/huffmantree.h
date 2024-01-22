#include <stdio.h>

#ifndef HUFFTREE_H
#define HUFFTREE_H

struct huffman_tree;
struct huffman_node;

struct huffman_tree *create_huffman_tree(FILE *stream);

char *tree_to_string(struct huffman_node *root);

static struct huffman_node *merge_nodes(struct huffman_node *left,
                                        struct huffman_node *right);

static void insert_node(struct huffman_node *htree, struct huffman_node *hnode);

#endif
