#include "huffmantree.h"
#include "../../utils.h"
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

bool isLeaf(Node *node) { return (node->right == NULL && node->left == NULL); }

static int *createCharFrequencyTable(const char *buffer) {
        int *char_frequency = calloc(256, sizeof(int));

        for (int i = 0; i < strlen((char *)buffer); i++) {
                char_frequency[(unsigned char)buffer[i]]++;
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

Node *hufftree_pop(Tree *tree) {
        if (tree->root == NULL)
                return NULL;
        Node *node = tree->root;
        tree->root = tree->root->next;

        return node;
}

// TODO: Create function
static Node *mergeNodes(Node *left, Node *right) {
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

Tree *hufftree_create(const char *buffer) {
        Tree *tree = malloc(sizeof(Tree));
        int *char_frequency = createCharFrequencyTable(buffer);
        printf("Char Frequency:");
        for (int i = 0; i < 256; i++) {
                if (char_frequency[i] > 0) {
                        Node *tmpNode = hufftree_createNode((unsigned char)i,
                                                            char_frequency[i]);
                        printf("\t[%c] = [%i]\n", i, char_frequency[i]);
                        hufftree_insert(tree, tmpNode);
                }
        }
        printf("\n");

        Node *curr = tree->root;

        printf("merging...\n");
        while (tree->root->next != NULL) {
                Node *merged;
                Node *left = hufftree_pop(tree);
                Node *right = hufftree_pop(tree);

                merged = mergeNodes(left, right);

                printf("Merged Node:\n\n");
                printf("\tChar: %c\n", merged->character);
                printf("\tChar frequency: %i\n\n", merged->frequency);

                printf("\tLeft:\n");
                printf("\t\tChar: %c\n", left->character);
                printf("\t\tChar frequency: %i\n\n", left->frequency);

                printf("\tRight:\n");
                printf("\t\tChar: %c\n", right->character);
                printf("\t\tChar frequency: %i\n\n", right->frequency);

                hufftree_insert(tree, merged);
        }

        tree->char_frequency = char_frequency;
        tree->stringfied = hufftree_toString(tree);
        tree->paths = hufftree_getPaths(tree);

        for (int i = 0; i < 256; i++) {
                if (!strcmp(tree->paths[i], "")) {
                        printf("Path[%c] = %s\n", i, tree->paths[i]);
                }
        }

        return tree;
}

static char *_hufftree_toString(Node *node, char *buffer) {
        if (node != NULL) {
                if (node->character == '*' || node->character == '\\') {
                        char escape = '\\';
                        concat(buffer, &escape);
                }

                buffer = concat(buffer, &node->character);
                buffer = _hufftree_toString(node->left, buffer);
                buffer = _hufftree_toString(node->right, buffer);
        }
        return buffer;
}

char *hufftree_toString(Tree *tree) {
        char *buffer = "";
        buffer = _hufftree_toString(tree->root, buffer);
        return buffer;
}

static void _hufftree_getPaths(struct huffman_node *node, char *paths[],
                               char *path) {
        if (isLeaf(node)) {
                paths[node->character] = path;
                printf("path[%c] = %s\n", node->character, path);
        } else {
                char *tmpRight = strdup(path);
                char *tmpLeft = strdup(path);

                _hufftree_getPaths(node->left, paths, strcat(tmpLeft, "0"));
                _hufftree_getPaths(node->right, paths, strcat(tmpRight, "1"));
        }
}

char **hufftree_getPaths(struct huffman_tree *tree) {
        char **paths = malloc(sizeof(char) * 256);

        for (int i = 0; i < 256; i++) {
                paths[i] = malloc(sizeof(unsigned char) * 8);
        }

        char path[256] = "";

        _hufftree_getPaths(tree->root, paths, path);

        return paths;
}
