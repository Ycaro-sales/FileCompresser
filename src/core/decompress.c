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
        charArray *string = getStringFromFile(stream);

        Header *header = getHeaderFromBuffer(string);

        Tree *hufftree = hufftree_fromString(header->treeString);

        hufftree_printTree(hufftree->root);

        printf("Tree: ");
        for (int i = 0; i < header->treeString->size; i++)
                printf("%d ", header->treeString->array[i]);
        printf("\n");

        printf("Thrash: %d\n", header->thrashSize);
        printf("Tree Size: %d\n", header->treeSize);

        FILE *decompressedFile = fopen("decompressedimg.jpg", "w");

        Node *currNode = hufftree->root;

        for (int i = (2 + header->treeSize); i < string->size; i++) {
                for (int currBit = 7; currBit >= 0; currBit--) {
                        // Se for o último byte e o tamanho do thrash e o bit
                        if (header->thrashSize != 0 && i == string->size - 1 &&
                            currBit < (header->thrashSize)) {
                                break;
                        }

                        // Se o bit atual for 1, vai para a direita, senão, vai
                        // para a esquerda
                        if (is_bit_i_set((unsigned char)string->array[i],
                                         currBit)) {
                                currNode = currNode->right;
                        } else if (!is_bit_i_set(
                                       (unsigned char)string->array[i],
                                       currBit)) {
                                currNode = currNode->left;
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

        header->treeSize = (unsigned short int)buffer->array[1] |
                           ((unsigned short int)firstByte) << 8;

        charArray *string = malloc(sizeof(charArray));
        string->size = header->treeSize;
        string->array = malloc(sizeof(char) * string->size);

        for (int i = 2; i < header->treeSize + 2; i++) {
                string->array[i - 2] = buffer->array[i];
        }

        header->treeString = string;

        return header;
}
