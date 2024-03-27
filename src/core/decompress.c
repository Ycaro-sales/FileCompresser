#include "../libs/file_functions.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Header {
        charArray *treeString;
        unsigned short int treeSize;
        unsigned short int thrashSize;
} Header;

Header *getHeaderFromBuffer(charArray *buffer);

void decompress(FILE *stream) {
        char *decompressedFilename;
        scanf("%[^\n]", decompressedFilename);

        // Pega o arquivo em formato de charArray
        charArray *string = getStringFromFile(stream);

        // pegar o cabeçalho do arquivo
        Header *header = getHeaderFromBuffer(string);

        // Cria a árvore de Huffman a partir da string da árvore
        Tree *hufftree = hufftree_fromString(header->treeString);

        hufftree_printTree(hufftree->root);

        FILE *decompressedFile = fopen(decompressedFilename, "w");

        Node *currNode = hufftree->root;

        for (int i = (2 + header->treeSize); i < string->size; i++) {
                for (int currBit = 7; currBit >= 0; currBit--) {
                        // Se for o último byte e o tamanho do thrash e o bit
                        if (header->thrashSize != 0 && i == string->size - 1 &&
                            currBit < (header->thrashSize)) {
                                break;
                        }

                        // Se o bit atual for 1, vai para a direita, senão, vai
                        // para a esquerda
                        if (is_bit_i_set((unsigned char)string->array[i],
                                         currBit)) {
                                currNode = currNode->right;
                        } else if (!is_bit_i_set(
                                       (unsigned char)string->array[i],
                                       currBit)) {
                                currNode = currNode->left;
                        }

                        // Se for uma folha, escreve o caractere no arquivo e
                        // volta para a raiz
                        if (isLeaf(currNode)) {
                                fputc(currNode->character, decompressedFile);
                                currNode = hufftree->root;
                        }
                }
        }
        return;
}

Header *getHeaderFromBuffer(charArray *buffer) {
        Header *header = malloc(sizeof(Header));

        // Pega o tamanho do lixo e da árvore
        // o lixo sao os 3 primeiros caracteres do primeiro byte
        header->thrashSize = ((unsigned char)buffer->array[0] >> 5);
        unsigned char firstByte = buffer->array[0];
        firstByte <<= 3;
        firstByte >>= 3;

        // o tamanho da arvore sao os 5 bits restantes do primeiro byte
        // e os 8 bits do segundo byte
        header->treeSize = (unsigned short int)buffer->array[1] |
                           ((unsigned short int)firstByte) << 8;

        charArray *treeString = malloc(sizeof(charArray));

        // Aloca o espaço necessário para a string da árvore
        treeString->size = header->treeSize;
        treeString->array = malloc(sizeof(char) * treeString->size);

        // Pega a string da árvore
        for (int i = 2; i < header->treeSize + 2; i++) {
                treeString->array[i - 2] = buffer->array[i];
        }

        header->treeString = treeString;

        return header;
}
