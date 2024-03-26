#include "../file_functions.h"
#include <stdbool.h>
#include <stdio.h>

#ifndef HUFFTREE_H
#define HUFFTREE_H

struct huffman_tree;
struct huffman_node;

typedef struct huffman_node {
        char character;
        int frequency;
        struct huffman_node *next;
        struct huffman_node *left;
        struct huffman_node *right;
} Node;

typedef struct huffman_tree {
        Node *root;
        charArray *stringfied;
        char **paths;
        int *char_frequency;
} Tree;

void hufftree_printTree(Node *root);
Tree *hufftree_fromString(charArray *buffer);

void test_hufftree_fromString();

Tree *hufftree_create(charArray *buffer);
Node *hufftree_createNode(unsigned char character, int frequency);
char **hufftree_getPaths(struct huffman_tree *tree);

void hufftree_insert(Tree *tree, Node *node);
charArray *hufftree_toString(Tree *tree);
Node *hufftree_pop(Tree *tree);
bool isLeaf(Node *node);

#endif
