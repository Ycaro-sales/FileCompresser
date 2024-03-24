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

int main() {
        FILE *stream = fopen("./public/img038.jpg", "r");
        printf("opening file\n");

        if (stream == NULL) {
                printf("Couldn't open the file\n");
                return 0;
        }

        compress(stream);

        fclose(stream);

        // FILE *compressedFile = fopen("./test.huff", "r");
        //
        // if (compressedFile == NULL) {
        // printf("Couldn't open the file\n");
        // return 0;
        // }

        // decompress(compressedFile);

        // fclose(compressedFile);

        return 0;
}
