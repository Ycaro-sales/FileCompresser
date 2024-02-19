#include "file_functions.h"
#include <stdio.h>
#include <stdlib.h>

char *getBufferFromFile(FILE *stream) {

        fseek(stream, 0, SEEK_END);

        int file_size = ftell(stream);

        char *buffer = malloc(file_size * sizeof(unsigned char));

        rewind(stream);

        fread(buffer, file_size, 1, stream);

        return buffer;
}
