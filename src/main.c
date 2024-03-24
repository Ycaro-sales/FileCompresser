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

int main() {

        test_compress();
        test_decompress();
        return 0;
}

void test_decompress() {
        FILE *compressedFile = fopen("./test.huff", "r");

        if (compressedFile == NULL) {
                printf("Couldn't open the file\n");
                return;
        }

        decompress(compressedFile);

        fclose(compressedFile);
}

void test_compress() {
        FILE *stream = fopen("./public/file.txt", "r");

        printf("opening file\n");

        if (stream == NULL) {
                printf("Couldn't open the file\n");
                return;
        }

        compress(stream);

        fclose(stream);
}
