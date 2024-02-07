#include <stdbool.h>
#include <stdio.h>

#ifndef HUFFTREE_H
#define HUFFTREE_H

struct huffman_tree;
struct huffman_node;

typedef struct huffman_node Node;
typedef struct huffman_tree Tree;

Tree *hufftree_create(unsigned char *buffer);
Node *hufftree_createNode(unsigned char character, int frequency);
char **hufftree_getPaths(struct huffman_tree *tree);

void hufftree_insert(Tree *tree, Node *node);
char *hufftree_toString(Tree *tree);
bool isLeaf(Node *node);

#endif
