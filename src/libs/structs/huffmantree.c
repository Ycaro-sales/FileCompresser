#include "huffmantree.h"
#include "../../utils.h"
#include "../file_functions.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UCHAR_AMOUNT 256

bool isLeaf(Node *node) { return (node->right == NULL && node->left == NULL); }

static int *createCharFrequencyTable(charArray *buffer) {
        int *char_frequency = calloc(256, sizeof(int));

        // Conta a frequência de cada caractere
        for (int i = 0; i < buffer->size; i++) {
                char_frequency[(unsigned char)buffer->array[i]]++;
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

        // Cria um novo nó com a soma das frequências dos dois nós
        frequencySum = left->frequency + right->frequency;
        merged = hufftree_createNode('*', frequencySum);

        // Define os filhos do novo nó
        merged->left = left;
        merged->right = right;

        merged->next = NULL;

        return merged;
}

// NOTE: Test succeed
void hufftree_insert(Tree *tree, Node *node) {
        Node *curr = tree->root;

        // Se a árvore estiver vazia ou a frequência do nó for menor que a raiz
        // da árvore, insere o nó na raiz
        if (tree->root == NULL || tree->root->frequency > node->frequency) {
                node->next = curr;
                tree->root = node;
                return;
        }

        // Percorre a lista até encontrar um nó com frequência maior ou igual
        // e insere o nó na posição
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

Tree *hufftree_create(charArray *buffer) {
        Tree *tree = malloc(sizeof(Tree));

        int *char_frequency = createCharFrequencyTable(buffer);

        // Cria um nó para cada caractere com frequência maior que 0
        // e insere na árvore
        printf("Char Frequency:");
        for (int i = 0; i < 256; i++) {
                if (char_frequency[i] > 0) {
                        Node *tmpNode = hufftree_createNode((unsigned char)i,
                                                            char_frequency[i]);
                        printf("\t[%x] = [%i]\n", i, char_frequency[i]);
                        hufftree_insert(tree, tmpNode);
                }
        }
        printf("\n");

        Node *curr = tree->root;

        // Cria a árvore de Huffman
        // Percorre a lista de nós, retirando os dois primeiros e inserindo um
        // novo nó com a soma das frequências dos dois
        while (tree->root->next != NULL) {
                Node *merged;
                Node *left = hufftree_pop(tree);
                Node *right = hufftree_pop(tree);

                merged = mergeNodes(left, right);

                hufftree_insert(tree, merged);
        }

        tree->char_frequency = char_frequency;
        tree->stringfied = hufftree_toString(tree);
        tree->paths = hufftree_getPaths(tree);

        // Imprime os caminhos de cada caractere
        for (int i = 0; i < 256; i++) {
                if (!strcmp(tree->paths[i], "")) {
                        printf("Path[%c] = %s\n", i, tree->paths[i]);
                }
        }

        return tree;
}

static charArray *_hufftree_toString(Node *node, charArray *buffer) {
        // Transforma a árvore de Huffman em uma string percorrendo a árvore em
        // pré-ordem e concatenando os caracteres no buffer

        // Se o nó não for nulo, insere o caractere no buffer e chama a função
        if (node != NULL) {
                if ((node->character == '*' && isLeaf(node)) ||
                    node->character == '\\') {
                        char escape = '\\';
                        buffer = concat(buffer, escape);
                }

                buffer = concat(buffer, node->character);
                buffer = _hufftree_toString(node->left, buffer);
                buffer = _hufftree_toString(node->right, buffer);
        }

        return buffer;
}

charArray *hufftree_toString(Tree *tree) {
        charArray *buffer = malloc(sizeof(charArray));
        buffer = _hufftree_toString(tree->root, buffer);
        return buffer;
}

Node *_hufftree_fromString(charArray *str, int *index) {

        if (*index >= str->size) {
                return NULL;
        }

        char value = str->array[*index];
        (*index)++;

        // Se o caractere for '*', cria um nó com dois filhos
        if (value == '*') {
                Node *root = hufftree_createNode(value, 0);
                root->left = _hufftree_fromString(str, index);
                root->right = _hufftree_fromString(str, index);
                return root;
        } else {
                if (value == '\\') {
                        (*index)++;
                }

                // If leaf node, create a leaf node
                return hufftree_createNode(value, 0);
        }
}

Tree *hufftree_fromString(charArray *buffer) {
        Tree *tree = malloc(sizeof(Tree));
        int index = 0;

        tree->root = _hufftree_fromString(buffer, &index);
        tree->paths = NULL;
        tree->stringfied = NULL;
        tree->char_frequency = NULL;

        return tree;
}

static void _hufftree_getPaths(struct huffman_node *node, char *paths[],
                               char *path) {
        if (isLeaf(node)) {
                paths[node->character] = path;
        } else {
                char *tmpRight = strdup(path);
                char *tmpLeft = strdup(path);

                _hufftree_getPaths(node->left, paths, strcat(tmpLeft, "0"));
                _hufftree_getPaths(node->right, paths, strcat(tmpRight, "1"));
        }
}

char **hufftree_getPaths(struct huffman_tree *tree) {
        char **paths = malloc(sizeof(char) * 256);

        // Inicializa o array de caminhos
        for (int i = 0; i < 256; i++) {
                paths[i] = malloc(sizeof(unsigned char) * 8);
        }

        char path[256] = "";

        // Preenche o array de caminhos
        _hufftree_getPaths(tree->root, paths, path);

        return paths;
}
