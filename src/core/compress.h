#include "../libs/file_functions.h"
#include "../libs/structs/huffmantree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef COMPRESS_H
#define COMPRESS_H

typedef struct Header {
        charArray *treeString;
        unsigned short int treeSize;
        unsigned short int thrashSize;
} Header;

Header *createHeader(Tree *tree);

FILE *writeHeader(FILE *compressed_file, Header *header);

FILE *writeCompressedFileData(FILE *compressedFile, charArray *fileData,
                              struct huffman_tree *tree);

int getThrashSize(struct huffman_tree *tree);

bool compress(FILE *file);

int *create_char_frequency_table(unsigned char *buffer);

#endif
