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

void hufftree_printTree(Node *root) {
        if (root != NULL) {
                printf("Char: %d\n", root->character);
                printf("Frequency: %i\n", root->frequency);
                hufftree_printTree(root->left);
                hufftree_printTree(root->right);
        }
}

static int *createCharFrequencyTable(charArray *buffer) {
        int *char_frequency = calloc(256, sizeof(int));

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

        // soma as frequências dos dois nós
        frequencySum = left->frequency + right->frequency;

        // cria um novo nó com o caractere '*' e a soma das frequências
        merged = hufftree_createNode('*', frequencySum);

        // aponta para os dois nós mesclados
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

Tree *hufftree_create(charArray *buffer) {
        // aloca espaço para a árvore
        Tree *tree = malloc(sizeof(Tree));

        // cria uma tabela de frequência de caracteres
        int *char_frequency = createCharFrequencyTable(buffer);

        // cria um nó para cada caractere que aparece no arquivo
        // e o insere ordenadamente na árvore
        for (int i = 0; i < 256; i++) {
                if (char_frequency[i] > 0) {
                        Node *tmpNode = hufftree_createNode((unsigned char)i,
                                                            char_frequency[i]);
                        hufftree_insert(tree, tmpNode);
                }
        }

        // cria a árvore de Huffman mesclando os dois primeiros nós
        // da lista de nós ordenada criando um novo nó com a soma
        // das frequências dos dois nós mesclados e os inserindo na lista
        // ordenadamente até que reste apenas um nó na lista
        while (tree->root->next != NULL) {
                Node *merged;

                // remove os dois primeiros nós da lista e armazena
                Node *left = hufftree_pop(tree);
                Node *right = hufftree_pop(tree);

                // mescla os dois nós
                merged = mergeNodes(left, right);

                // insere o nó mesclado na lista ordenadamente
                hufftree_insert(tree, merged);
        }

        tree->char_frequency = char_frequency;
        tree->stringfied = hufftree_toString(tree);

        // cria uma tabela de caminhos para cada caractere
        tree->paths = hufftree_getPaths(tree);

        for (int i = 0; i < 256; i++) {
                if (!strcmp(tree->paths[i], "")) {
                        printf("Path[%c] = %s\n", i, tree->paths[i]);
                }
        }

        return tree;
}

void _hufftree_toString(Node *node, charArray *buffer, int *index) {
        if (node != NULL) {
                // se o caractere for '*' ou '\', insere um caractere de escape
                if ((node->character == '*' && isLeaf(node)) ||
                    node->character == '\\') {
                        char escape = '\\';

                        buffer->array[*index] = escape;
                        (*index)++;
                }

                // insere o caractere na string
                buffer->array[*index] = node->character;
                (*index)++;

                // se for um nó intermediário, chama a função recursivamente
                _hufftree_toString(node->left, buffer, index);
                _hufftree_toString(node->right, buffer, index);
        }

        return;
}

charArray *hufftree_toString(Tree *tree) {
        // aloca espaço para o buffer
        charArray *buffer = malloc(sizeof(charArray));

        // aloca espaço para o array de caracteres
        buffer->array = malloc(sizeof(char) * 2000);
        int index = 0;

        _hufftree_toString(tree->root, buffer, &index);
        buffer->size = index;

        return buffer;
}

Node *_hufftree_fromString(charArray *str, int *index) {
        if (*index >= str->size) {
                return NULL;
        }

        // pega o caractere atual da string
        char value = str->array[*index];
        (*index)++;

        // se o caractere for um caractere de escape, pega o próximo caractere
        if (value == '\\') {
                value = str->array[*index];
                (*index)++;
                if (value == '*') {
                        return hufftree_createNode(value, 0);
                }
        }

        // se o caractere for '*' e nao possuir um caractere de escape antes,
        // cria um nó intermediário
        if (value == '*') {
                // cria um nó intermediário e chama a função recursivamente
                Node *root = hufftree_createNode(value, 0);

                // Cria filhos para o nó intermediário
                root->left = _hufftree_fromString(str, index);
                root->right = _hufftree_fromString(str, index);

                return root;
        } else {
                // se o caractere for um caractere normal, retorna um nó folha
                return hufftree_createNode(value, 0);
        }
}

Tree *hufftree_fromString(charArray *buffer) {
        // aloca espaço para a árvore
        Tree *tree = malloc(sizeof(Tree));
        int index = 0;

        // cria a árvore de Huffman a partir da string
        tree->root = _hufftree_fromString(buffer, &index);
        tree->paths = NULL;
        tree->stringfied = NULL;
        tree->char_frequency = NULL;

        return tree;
}

static void _hufftree_getPaths(struct huffman_node *node, char *paths[],
                               char *path) {
        // se for uma folha, insere o caminho na tabela de caminhos
        if (isLeaf(node)) {
                paths[node->character] = path;
                printf("path[%c] = %s\n", node->character, path);
        } else {
                // se não for uma folha, chama a função recursivamente
                // para a esquerda e para a direita
                // concatenando 0 para a esquerda e 1 para a direita
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
