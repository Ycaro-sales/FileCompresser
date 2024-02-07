#include <stdio.h>

#ifndef HUFFTREE_H
#define HUFFTREE_H

struct huffman_tree;
struct huffman_node;

typedef struct huffman_node Node;
typedef struct huffman_tree Tree;

Tree *hufftree_create(unsigned char *buffer);
Node *hufftree_createNode(unsigned char character, int frequency);

void test_hufftree_toString();
void hufftree_insert(Tree *tree, Node *node);
char *hufftree_toString(Tree *tree);
char *concat(const char *dest, const char *src);

#endif
