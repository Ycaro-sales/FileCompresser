#include "../libs/file_functions.h"
#include "../libs/structs/compressionhash.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Header {
        char *treeString;
        unsigned short int treeSize;
        unsigned short int thrashSize;
} Header;

Header *createHeader(Tree *tree);

FILE *writeHeader(FILE *compressed_file, Header *header);

FILE *writeCompressedFileData(FILE *compressedFile, char *fileData,
                              struct huffman_tree *tree);
int getThrashSize(struct huffman_tree *tree);

bool compress(FILE *stream) {
        // char *buffer = getBufferFromFile(stream);
        char *buffer = "AAABBCC\n";

        struct huffman_tree *tree = hufftree_create(buffer);
        Header *header = createHeader(tree);

        FILE *compressedFile = fopen("test.huff", "wr");

        writeHeader(compressedFile, header);
        writeCompressedFileData(compressedFile, buffer, tree);

        return true;
}

Header *createHeader(Tree *tree) {
        Header *tmp = malloc(sizeof(Header));

        tmp->treeString = hufftree_toString(tree);
        tmp->treeSize = strlen(tmp->treeString);
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

        charBuffer = (unsigned char)(header->thrashSize << 13) | charBuffer;

        charBuffer = charBuffer | (unsigned char)(header->treeSize >> 8);
        fputc(charBuffer, compressed_file);

        charBuffer = ((unsigned char)(header->treeSize));
        fputc(charBuffer, compressed_file);

        for (int i = 0; i < header->treeSize; i++) {
                fputc(header->treeString[i], compressed_file);
        }

        return compressed_file;
}

FILE *writeCompressedFileData(FILE *compressedFile, char *fileData,
                              struct huffman_tree *tree) {

        int bit_count = 7;
        unsigned char current_byte = 0;

        for (int i = 0; i < strlen(fileData); i++) {
                char *path = tree->paths[fileData[i]];
                printf("file data [%c] = %s\n", fileData[i], path);

                for (int j = 0; j < strlen(path); j++) {
                        if (path[j] == '1') {
                                current_byte = set_bit(current_byte, bit_count);
                        }

                        bit_count--;

                        if (bit_count == -1) {
                                fputc(current_byte, compressedFile);
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

        return thrash_size % 8;
}
