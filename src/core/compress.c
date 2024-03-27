#include "../libs/file_functions.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Header {
        charArray *treeString;
        unsigned short int treeSize;
        unsigned short int thrashSize;
} Header;

Header *createHeader(Tree *tree);

FILE *writeHeader(FILE *compressed_file, Header *header);

FILE *writeCompressedFileData(FILE *compressedFile, charArray *fileData,
                              struct huffman_tree *tree);
int getThrashSize(struct huffman_tree *tree);

void compress(FILE *file) {
        // Pega o conteúdo do arquivo e coloca em um charArray
        charArray *charArray = getStringFromFile(file);

        char *filename;
        char *compressedFilename;

        printf("Enter the name of the compressed file: ");
        scanf("%[^\n]", filename);

        compressedFilename = strcat(filename, ".huff");

        // Cria a árvore de Huffman
        struct huffman_tree *tree = hufftree_create(charArray);
        hufftree_printTree(tree->root);

        // cria o cabeçalho
        Header *header = createHeader(tree);

        // cria o arquivo comprimido
        FILE *compressedFile = fopen(compressedFilename, "wr");

        // escreve o cabeçalho no arquivo comprimido
        writeHeader(compressedFile, header);

        // escreve o conteúdo do arquivo comprimido
        writeCompressedFileData(compressedFile, charArray, tree);
}

Header *createHeader(Tree *tree) {
        Header *tmp = malloc(sizeof(Header));

        tmp->treeString = tree->stringfied;
        tmp->treeSize = tmp->treeString->size;
        tmp->thrashSize = getThrashSize(tree);

        return tmp;
}

// TODO: Test function
FILE *writeHeader(FILE *compressed_file, Header *header) {
        unsigned char charBuffer = 0;
        printf("------------writing header------------\n");
        printf("header values:\n");
        printf("\ttree size: %d, %b\n", header->treeSize, header->treeSize);
        printf("\tthrash size: %d, %b\n", header->thrashSize,
               header->thrashSize);

        // escreve o tamanho do thrash e da árvore no arquivo
        // primeiro fazendo um cast no tamanho do thrash para
        // unsigned char de maneira que ele tenha o tamanho de 1 byte
        // entao faz um shift de 5 bits para a esquerda de maneira que
        // os 3 primeiros bits contenham o tamanho do lixo
        // e os 5 bits restantes sejam 0
        charBuffer = ((unsigned char)(header->thrashSize) << 5) | charBuffer;

        // faz um shift de 8 bits para a direita dos 8 primeiros bits do tamanho
        // da arvore de maneira que os 3 primeiros bits sejam o tamanho do lixo
        // os 5 bits restantes sejam o tamanho da arvore
        charBuffer = charBuffer | (unsigned char)(header->treeSize >> 8);

        // insere o byte no arquivo
        fputc(charBuffer, compressed_file);

        // faz o cast do tamanho da arvore para unsigned char de maneira que ele
        // tenha o tamanho de 1 byte e perca o primeiro byte inserindo os 8 bits
        // restantes no arquivo
        charBuffer = ((unsigned char)(header->treeSize));
        fputc(charBuffer, compressed_file);

        // insere a string da arvore no arquivo
        for (int i = 0; i < header->treeSize; i++) {
                fputc(header->treeString->array[i], compressed_file);
        }

        return compressed_file;
}

FILE *writeCompressedFileData(FILE *compressedFile, charArray *fileData,
                              struct huffman_tree *tree) {

        // inicializa o contador de bits
        int bit_count = 7;

        // inicializa um byte zerado (00000000)
        unsigned char current_byte = 0;

        // para cada caractere do arquivo
        for (int i = 0; i < fileData->size; i++) {

                // pega o caminho do caractere atual
                char *path = tree->paths[fileData->array[i]];

                // percorre o caminho do caractere
                // e vai setando os bits no byte atual
                // do arquivo comprimido

                // 1 << 7 = 10000000
                // 1 << 6 = 01000000
                // 1 << 5 = 00100000
                // 1 << 4 = 00010000
                // 1 << 3 = 00001000
                // 1 << 2 = 00000100
                // 1 << 1 = 00000010
                // 1 << 0 = 00000001

                for (int j = 0; j < strlen(path); j++) {

                        // se o bit atual do caminho for 1,
                        // seta o bit no byte atual e no offset do bit_count
                        if (path[j] == '1') {
                                current_byte = set_bit(current_byte, bit_count);
                        }

                        bit_count--;

                        if (bit_count == -1 || (i == fileData->size - 1 &&
                                                j == strlen(path) - 1)) {
                                // insere o byte no arquivo comprimido
                                fputc(current_byte, compressedFile);

                                // reseta o contador de bits e o byte atual
                                bit_count = 7;
                                current_byte = 0;
                        }
                }
        }

        return compressedFile;
}

int getThrashSize(struct huffman_tree *tree) {
        int thrash_size = 0;

        // calcula o tamanho do thrash
        // para cada caractere, calcula o tamanho do caminho
        // multiplicado pela frequência do caractere e soma ao tamanho do thrash
        for (int i = 0; i < 256; i++) {
                if (tree->char_frequency[i] > 0) {
                        thrash_size +=
                            strlen(tree->paths[i]) * tree->char_frequency[i];
                }
        }

        // retorna o tamanho do thrash
        return 8 - (thrash_size % 8);
}
