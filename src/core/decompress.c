#include "../libs/file_functions.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Header {
        charArray *treeString;
        unsigned short int treeSize;
        unsigned short int thrashSize;
} Header;

Header *getHeaderFromBuffer(charArray *buffer);

void decompress(FILE *stream) {
        charArray *charArray = getStringFromFile(stream);
        printf("Buffer: ");

        for (int i = 0; i < charArray->size; i++) {
                printf("%c", charArray->array[i]);
        }

        printf("\n");

        Header *header = getHeaderFromBuffer(charArray);

        Tree *hufftree = hufftree_fromString(header->treeString);
        printf("Tree: ");
        for (int i = 0; i < header->treeString->size; i++)
                printf("%c", header->treeString->array[i]);

        printf("Thrash: %d\n", header->thrashSize);
        printf("Tree Size: %d\n", header->treeSize);

        FILE *decompressedFile = fopen("descompressimg.jpg", "w");

        Node *currNode = hufftree->root;
        printf("Buffer size: %d\n", charArray->size);

        printf("Buffer pointer: %p\n", currNode);

        for (int i = (2 + header->treeSize); i < charArray->size; i++) {
                for (int currBit = 7; currBit >= 0; currBit--) {
                        // Se for o último byte e o tamanho do thrash e o bit
                        // atual for menor que o thrash, para a descompressão
                        if (currNode->left == NULL && currNode->right == NULL) {
                                printf("currNode: %c\n", currNode->character);
                        }

                        if (header->thrashSize != 0 &&
                            i == charArray->size - 1 &&
                            currBit < (header->thrashSize)) {
                                break;
                        }

                        // Se o bit atual for 1, vai para a direita, senão, vai
                        // para a esquerda
                        if (is_bit_i_set((unsigned char)charArray->array[i],
                                         currBit)) {
                                printf("right\n");
                                currNode = currNode->right;
                        } else if (!is_bit_i_set(
                                       (unsigned char)charArray->array[i],
                                       currBit)) {
                                currNode = currNode->left;
                                printf("left\n");
                        }

                        // Se for uma folha, escreve o caractere no arquivo e
                        // volta para a raiz

                        if (currNode->left == NULL && currNode->right == NULL) {
                                printf("currNode: %c\n", currNode->character);
                        }

                        if (isLeaf(currNode)) {
                                fputc(currNode->character, decompressedFile);
                                currNode = hufftree->root;
                        }
                }
        }
        return;
}

Header *getHeaderFromBuffer(charArray *buffer) {
        Header *header = malloc(sizeof(Header));

        header->thrashSize = ((unsigned char)buffer->array[0] >> 5);
        unsigned char firstByte = buffer->array[0];
        firstByte <<= 3;
        firstByte >>= 3;

        printf("size: %lu\n", sizeof(buffer->array[0]));
        printf("tmp: %b\n", (unsigned char)(buffer->array[0] << 3));
        printf("tmp: %b\n", (unsigned short int)buffer->array[0]);

        printf("buffer: %b\n", buffer->array[1]);
        header->treeSize = (unsigned short int)buffer->array[1] |
                           ((unsigned short int)firstByte) << 8;

        charArray *string = malloc(sizeof(charArray));
        string->size = 0;
        string->array = malloc(sizeof(char) * 1);

        if (string->array == NULL) {
                printf("Error allocating memory\n");
                exit(1);
        }

        for (int i = 2; i < header->treeSize + 2; i++) {
                string = concat(string, buffer->array[i]);
        }

        header->treeString = string;

        return header;
}
