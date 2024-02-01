#include "../libs/file_functions.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool compress(FILE *stream) {
        unsigned char *buffer = get_buffer_from_file(stream);

        printf("tamanho do buffer: %lu \n", sizeof(buffer));

        for (int i = 0; i < sizeof(buffer); i++) {
                if (buffer[i] == 0)
                        continue;
                printf("%c, %i, %x, %b\n", buffer[i], buffer[i], buffer[i],
                       buffer[i]);
        }

        struct huffman_tree *huff_tree = hufftree_create(buffer);

        return true;
}
