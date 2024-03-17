#include "file_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

charArray *getStringFromFile(FILE *stream) {
        charArray *tmp = malloc(sizeof(charArray));
        fseek(stream, 0L, SEEK_END);

        int file_size = ftell(stream);
        printf("File size: %d\n", file_size);

        tmp->size = file_size;
        tmp->array = malloc(file_size * sizeof(unsigned char));

        rewind(stream);

        fread(tmp->array, file_size, 1, stream);
        for (int i = 0; i < file_size; i++) {
                printf("%d %c\n", i, tmp->array[i]);
        }
        printf("\n");

        return tmp;
}

charArray *getCharArrayfromString(char *string) {
        charArray *tmp = malloc(sizeof(charArray));
        tmp->size = strlen(string);

        for (int i = 0; i < tmp->size; i++)
                tmp->array[i] = string[i];

        return tmp;
}
