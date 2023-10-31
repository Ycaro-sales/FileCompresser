#include "./core/compress.h"
#include "./core/decompress.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        bool result;
        char option;

        for (int i = 0; i < argc; i++) {
                FILE *stream = fopen(argv[i], "r");
                printf("Compress or decompress? (c/d): ");
                scanf("%c", &option);

                if (option == 'c') {
                        printf("Compressing...\n");
                        compress(stream);
                } else if (option == 'd') {
                        printf("Decompressing...\n");
                        decompress(stream);
                } else {
                        printf("Invalid option\n");
                        return EXIT_FAILURE;
                }
        }
        return EXIT_SUCCESS;
}
