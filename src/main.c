#include "./core/compress.h"
#include "./core/decompress.h"
#include "./libs/file_functions.h"
#include "./libs/structs/huffmantree.h"
#include "./utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
        FILE *stream = fopen("./public/file.txt", "r");
        printf("opening file\n");

        if (stream == NULL) {
                printf("Couldn't open the file\n");
                return 0;
        }

        test_hufftree_toString();

        // compress(stream);

        fclose(stream);

        return 0;
}

// int main(int argc, char *argv[]) {
//         char option;
//
//         for (int i = 0; i < argc; i++) {
//                 FILE *stream = fopen(argv[i], "r");
//                 printf("Compress or decompress? (c/d): ");
//                 scanf("%c", &option);
//
//                 if (option == 'c') {
//                         printf("Compressing...\n");
//                         compress(stream);
//                 } else if (option == 'd') {
//                         printf("Decompressing...\n");
//                         decompress(stream);
//                 } else {
//                         printf("Invalid option\n");
//                         return EXIT_FAILURE;
//                 }
//         }
//         return EXIT_SUCCESS;
// }
