#include "../libs/file_functions.h"
#include "../libs/structs/compressionhash.h"
#include "../libs/structs/huffmantree.c"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Header {
        char *treeString;
        int treeSize;
        int thrashSize;
} Header;

bool compress(FILE *stream) {
        unsigned char *buffer = get_buffer_from_file(stream);

        struct huffman_tree *huff_tree = hufftree_create(buffer);

        return true;
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

Header *createHeader(Tree *tree) {
        Header *tmp = malloc(sizeof(Header));

        tmp->treeString = hufftree_toString(tree);
        tmp->treeSize = strlen(tmp->treeString);
        tmp->thrashSize = getThrashSize(tree);

        return tmp;
}

FILE *writeHeaderToFile(FILE *compressed_file, Header *header) {
        int treeSize = header->treeSize;
        treeSize = treeSize << 5;
        treeSize = treeSize | header->thrashSize;

        fputc(treeSize, compressed_file);

        return compressed_file;
}
