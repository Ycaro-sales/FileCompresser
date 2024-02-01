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
} Node;

typedef struct huffman_tree {
        Node *root;
        hashtable *compressed_table;
} Tree;

static int *createCharFrequencyTable(unsigned char *buffer) {
        int *char_frequency = calloc(256, sizeof(int));

        for (int i = 0; i < strlen((char *)buffer); i++) {
                char_frequency[buffer[i]]++;
        }

        return char_frequency;
}

Node *hufftree_createNode(unsigned char character, int frequency) {
        Node *tmp = malloc(sizeof *tmp);
        tmp->character = character;
        tmp->frequency = frequency;
        tmp->next = NULL;
        tmp->left = NULL;
        tmp->right = NULL;
        return tmp;
}

// TODO: Create function
Node *merge_nodes(Node *left, Node *right) {
        Node *merged;
        int frequencySum;

        frequencySum = left->frequency + right->frequency;
        merged = hufftree_createNode('*', frequencySum);

        merged->left = left;
        merged->right = right;

        merged->next = NULL;

        return merged;
}

// NOTE: Test succeed
void hufftree_insert(Tree *tree, Node *node) {
        Node *curr = tree->root;

        if (tree->root == NULL || tree->root->frequency > node->frequency) {
                node->next = curr;
                tree->root = node;
                return;
        }

        while (curr->next != NULL) {
                if (node->frequency > curr->next->frequency) {
                        curr = curr->next;
                        continue;
                }
                if (curr->next->frequency >= node->frequency) {
                        node->next = curr->next;
                        curr->next = node;
                        return;
                }
                curr = curr->next;
        }
        curr->next = node;
}

Tree *hufftree_create(unsigned char *buffer) {
        Tree *tree = malloc(sizeof(Tree));
        int *char_frequency = createCharFrequencyTable(buffer);

        for (int i = 0; i < 256; i++) {
                if (char_frequency[i] > 0) {
                        Node *tmpNode = hufftree_createNode((unsigned char)i,
                                                            char_frequency[i]);
                        hufftree_insert(tree, tmpNode);
                }
        }

        Node *curr = tree->root;

        while (curr != NULL) {
                printf("%c %i", curr->character, curr->frequency);
                curr = curr->next;
        }

        while (tree->root->next != NULL) {
                Node *merged;
                merged = merge_nodes(tree->root, tree->root->next);
                tree->root = tree->root->next->next;
                hufftree_insert(tree, merged);
        }

        char *stringBuffer = "";
        hufftree_toString(tree->root, stringBuffer);
        printf("%s\n", stringBuffer);

        return tree;
}

// TODO: Create function
void hufftree_toString(Node *node, char *buffer) {
        if (node == NULL)
                return;

        strcat(buffer, &node->character);

        hufftree_toString(node->left, buffer);
        hufftree_toString(node->left, buffer);
}
