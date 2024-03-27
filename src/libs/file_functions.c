#include "file_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

charArray *getStringFromFile(FILE *stream) {
        charArray *tmp = malloc(sizeof(charArray));

        // Coloca o ponteiro no final do arquivo
        fseek(stream, 0L, SEEK_END);

        // Pega o tamanho do arquivo
        // comparando a posição do ponteiro com o início do arquivo
        int file_size = ftell(stream);
        printf("File size: %d\n", file_size);

        // Aloca o espaço necessário para o charArray
        tmp->size = file_size;
        tmp->array = malloc(file_size * sizeof(unsigned char));

        // Volta o ponteiro para o início do arquivo
        rewind(stream);

        // Lê o arquivo e coloca no charArray
        fread(tmp->array, file_size, 1, stream);

        return tmp;
}

charArray *getCharArrayfromString(char *string) {
        charArray *tmp = malloc(sizeof(charArray));
        tmp->size = strlen(string);

        for (int i = 0; i < tmp->size; i++)
                tmp->array[i] = string[i];

        return tmp;
}
