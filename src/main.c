#include "./core/compress.h"
#include "./core/decompress.h"
#include "./libs/file_functions.h"
#include "./libs/structs/huffmantree.h"
#include "./libs/structs/tests/hufftree_tests.c"
#include "./utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_decompress();
void test_compress();

int main(int argc, char *argv[]) {
        char *filename;
        int option;

        if (argc < 2) {
                scanf("%s", filename);
        } else {
                filename = argv[1];
        }

        FILE *stream = fopen(filename, "r");

        if (stream == NULL) {
                printf("Couldn't open the file\n");
                return 1;
        }

        printf("1 - Compress\n2 - Decompress\n");

        switch (option) {
        case 1:
                compress(stream);
                break;
        case 2:
                decompress(stream);
                break;
        default:
                printf("Invalid option\n");
                break;
        }

        return 0;
}

void test_compress() {
        FILE *stream = fopen("./Untitled.png", "r");

        printf("opening file\n");

        if (stream == NULL) {
                printf("Couldn't open the file\n");
                return;
        }

        compress(stream);

        fclose(stream);
}

void test_decompress() {
        FILE *compressedFile = fopen("./testbandeira.huff", "r");

        if (compressedFile == NULL) {
                printf("Couldn't open the file\n");
                return;
        }

        decompress(compressedFile);

        fclose(compressedFile);
}
