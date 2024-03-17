#include "../libs/file_functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef DECOMPRESS_H
#define DECOMPRESS_H

typedef struct Header {
        charArray *treeString;
        unsigned short int treeSize;
        unsigned short int thrashSize;
} Header;

void decompress(FILE *file);
Header *getHeaderFromBuffer(charArray *buffer);

#endif
