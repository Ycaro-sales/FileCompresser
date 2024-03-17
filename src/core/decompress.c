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

        Header *header = getHeaderFromBuffer(charArray);

        Tree *hufftree = hufftree_fromString(header->treeString);

        char *fileName;

        printf("Escolha o nome do arquivo descomprimido: ");
        scanf("%s\n", fileName);

        FILE *decompressedFile = fopen(fileName, "w");

        Node *currNode = hufftree->root;

        for (int i = (2 + header->treeSize); i < charArray->size; i++) {
                for (int currBit = 7; currBit >= 0; currBit--) {

                        if (header->thrashSize != 0 &&
                            i == charArray->size - 1 &&
                            currBit < (header->thrashSize)) {
                                break;
                        }

                        if (is_bit_i_set((unsigned char)charArray->array[i],
                                         currBit)) {
                                currNode = currNode->right;
                        } else if (!is_bit_i_set(
                                       (unsigned char)charArray->array[i],
                                       currBit)) {
                                currNode = currNode->left;
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
        string->size = 0;
        string->array = malloc(sizeof(char) * 1);

        for (int i = 2; i < header->treeSize + 2; i++) {
                string = concat(string, buffer->array[i]);
        }

        header->treeString = string;

        return header;
}
