#include "file_functions.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char *get_buffer_from_file(FILE *stream) {

        fseek(stream, 0, SEEK_END);

        int file_size = ftell(stream);

        unsigned char *buffer =
            (unsigned char *)malloc(file_size * sizeof(unsigned char));

        rewind(stream);

        fread(buffer, file_size, 1, stream);

        return buffer;
}
