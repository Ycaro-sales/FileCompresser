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

        FILE *decompressedFile = fopen("descompresstest.txt", "w");

        Node *currNode = hufftree->root;
        printf("Buffer size: %d\n", charArray->size);
        for (int i = (2 + header->treeSize); i < charArray->size; i++) {
                for (int currBit = 7; currBit >= 0; currBit--) {
                        printf("%d\n", currBit);

                        if (header->thrashSize != 0 &&
                            i == charArray->size - 1 &&
                            currBit > (8 - header->thrashSize)) {
                                break;
                        }

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
                        if (isLeaf(currNode)) {
                                fputc(currNode->character, decompressedFile);
                                if (i == charArray->size - 1 &&
                                    currBit == header->thrashSize - 1) {
                                        return;
                                }
                                currNode = hufftree->root;
                                printf("%d\n", currBit);
                        }
                }
        }

        return;
}

Header *getHeaderFromBuffer(charArray *buffer) {
        Header *header = malloc(sizeof(Header));

        header->thrashSize = ((unsigned char)buffer->array[0] >> 5);

        unsigned short int tmp = (unsigned short int)buffer->array[0] << 8;

        header->treeSize = (unsigned short int)buffer->array[1] | tmp;

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
