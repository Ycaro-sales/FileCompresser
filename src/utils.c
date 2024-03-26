#include "utils.h"
#include <stdlib.h>
#include <string.h>

unsigned char set_bit(unsigned char c, int i) {
        unsigned char mask = 1 << i;
        return mask | c;
}

bool is_bit_i_set(unsigned char c, int i) {
        unsigned char mask = 1 << i;
        return mask & c;
}

char *remove_extension(char *file_name) {
        char *tmp = malloc(sizeof(file_name));
        for (int i = 0; i < strlen(file_name); i++) {
                tmp[i] = file_name[i];
                if (file_name[i] == '.') {
                        tmp[i] = '\0';
                        break;
                }
        }

        return tmp;
}

charArray *concat(charArray *dest, char src) {
        charArray *buffer = malloc(dest->size + 1);
        buffer->size = dest->size + 1;
        // printf("dest size: %d\n", dest->size);
        // printf("dest: %p\n", dest->array);
        buffer->array = malloc(buffer->size * sizeof(char));

        memcpy(buffer->array, dest->array, sizeof(char) * dest->size);

        buffer->array[buffer->size - 1] = src;
        // printf("buffer: %s\n", buffer->array);
        // printf("buffer size: %d\n", buffer->size);

        return buffer;
}
