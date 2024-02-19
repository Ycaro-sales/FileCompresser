#include "huffmantree.h"
#ifndef COMPHASH_H
#define COMPHASH_H

typedef struct paths {
        char *paths[255];
        int thrash_size;
} Paths;

char *comphash_create(struct huffman_tree *tree);

#endif
