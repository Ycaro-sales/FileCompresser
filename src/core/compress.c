#include "compress.h"
#include "../libs/file_functions.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool compress(FILE *stream) {
        charArray *charArray = getStringFromFile(stream);

        struct huffman_tree *tree = hufftree_create(charArray);
        Header *header = createHeader(tree);

        FILE *compressedFile = fopen("test.huff", "wr");

        writeHeader(compressedFile, header);
        writeCompressedFileData(compressedFile, charArray, tree);

        return true;
}

Header *createHeader(Tree *tree) {
        Header *tmp = malloc(sizeof(Header));

        tmp->treeString = hufftree_toString(tree);
        tmp->treeSize = tmp->treeString->size;
        tmp->thrashSize = getThrashSize(tree);

        return tmp;
}

// TODO: Test function
FILE *writeHeader(FILE *compressed_file, Header *header) {
        unsigned char charBuffer = 0;
        printf("------------writing header------------\n");
        printf("header values:\n");
        printf("\ttree size: %d, %b\n", header->treeSize, header->treeSize);
        printf("\tthrash size: %d, %b\n", header->thrashSize,
               header->thrashSize);

        charBuffer = ((unsigned char)(header->thrashSize) << 5) | charBuffer;

        charBuffer = charBuffer | (unsigned char)(header->treeSize >> 8);
        fputc(charBuffer, compressed_file);

        charBuffer = ((unsigned char)(header->treeSize));
        fputc(charBuffer, compressed_file);

        for (int i = 0; i < header->treeSize; i++) {
                fputc(header->treeString->array[i], compressed_file);
        }

        return compressed_file;
}

FILE *writeCompressedFileData(FILE *compressedFile, charArray *fileData,
                              struct huffman_tree *tree) {

        int bit_count = 7;
        unsigned char current_byte = 0;

        for (int i = 0; i < fileData->size; i++) {
                char *path = tree->paths[fileData->array[i]];
                printf("file data [%c] = %s\n", fileData->array[i], path);

                for (int j = 0; j < strlen(path); j++) {
                        if (path[j] == '1') {
                                current_byte = set_bit(current_byte, bit_count);
                        }

                        bit_count--;

                        if (bit_count == -1 || (i == fileData->size - 1 &&
                                                j == strlen(path) - 1)) {
                                fputc(current_byte, compressedFile);
                                printf("current byte: %b\n", current_byte);
                                bit_count = 7;
                        }
                }
        }

        return compressedFile;
}

int getThrashSize(struct huffman_tree *tree) {
        int thrash_size = 0;

        for (int i = 0; i < 256; i++) {
                if (tree->char_frequency[i] > 0) {
                        thrash_size +=
                            strlen(tree->paths[i]) * tree->char_frequency[i];
                }
        }

        return 8 - (thrash_size % 8);
}
