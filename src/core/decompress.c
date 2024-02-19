#include "../libs/file_functions.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Header {
        char *treeString;
        unsigned short int treeSize;
        unsigned short int thrashSize;
} Header;

Header *getHeaderFromBuffer(char *buffer);

void decompress(FILE *stream) {
        char *buffer = getStringFromFile(stream);

        Header *header = getHeaderFromBuffer(buffer);

        Tree *hufftree = hufftree_fromString(buffer);

        FILE *decompressedFile = fopen("test.txt", "w");

        Node *currNode = hufftree->root;
        for (int i = 0; i < strlen(buffer); i++) {
                for (int currBit = 1; currBit <= 8; i++) {
                        if (header->thrashSize != 0 &&
                            i == strlen(buffer - 1) &&
                            currBit > (8 - header->thrashSize)) {
                                break;
                        }
                        if (isLeaf(currNode)) {
                                fputc(currNode->character, decompressedFile);
                                currNode = hufftree->root;
                                currBit++;
                        }
                        if (is_bit_i_set(buffer[i], currBit)) {
                                currNode = currNode->right;
                        } else {
                                currNode = currNode->left;
                        }
                }
        }

        return;
}

char *concat(const char *dest, const char *src) {
        char *buffer = malloc(strlen(dest) + strlen(src) + 1);
        strcpy(buffer, dest);
        strcat(buffer, src);
        return buffer;
}

Header *getHeaderFromBuffer(char *buffer) {
        Header *header = malloc(sizeof(Header));

        header->thrashSize = ((unsigned char)buffer[0] >> 5);

        unsigned short int tmp = (unsigned short int)buffer[0] << 8;

        header->treeSize = (unsigned short int)buffer[1] | tmp;
        char string[1000];

        for (int i = 2; i < header->treeSize + 2; i++) {
                concat(string, &buffer[i]);
        }

        header->treeString = string;

        return header;
}
