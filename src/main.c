#include "./core/compress.h"
#include "./core/decompress.h"
#include "./libs/file_functions.h"
#include "./libs/structs/huffmantree.h"
#include "./libs/structs/tests/hufftree_tests.c"
#include "./utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        int option = 0;
        char *file_name;
        FILE *file;

        if (argc < 2) {
                printf("Digite o nome do arquivo: ");
                scanf("%s", file_name);
        } else {
                file_name = argv[1];
        }

        file = fopen(file_name, "r");

        printf("Escolha uma opção:\n");
        printf("1 - Comprimir\n");
        printf("2 - Descomprimir\n");
        printf("3 - Testar\n");
        printf("4 - Sair\n");
        scanf("%d\n", &option);

        switch (option) {
        case 1:
                compress(file);
                break;
        case 2:
                decompress(file);
                break;
        case 3:
                break;
        case 4:
                return EXIT_SUCCESS;
        default:
                printf("Opção inválida\n");
                break;
        }

        return EXIT_SUCCESS;
}
