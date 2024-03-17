#include "decompress.h"
#include "../libs/structs/huffmantree.h"
#include "../utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decompress(FILE *stream) {
        // Pega o buffer do arquivo
        charArray *charArray = getStringFromFile(stream);

        // Pega o cabeçalho do buffer
        Header *header = getHeaderFromBuffer(charArray);

        // Cria a árvore de Huffman a partir da string
        Tree *hufftree = hufftree_fromString(header->treeString);

        char filePath[100] = "public/decompressed/";
        char *fileName;

        printf("Escolha o nome do arquivo descomprimido: ");
        scanf("%s\n", fileName);
        strcat(filePath, fileName);

        FILE *decompressedFile = fopen(fileName, "w");

        Node *currNode = hufftree->root;

        // Percorre o buffer, descomprimindo os bits
        for (int i = (2 + header->treeSize); i < charArray->size; i++) {
                for (int currBit = 7; currBit >= 0; currBit--) {
                        // Se for o último byte e o tamanho do thrash e o bit
                        // atual for menor que o thrash, para a descompressão
                        if (header->thrashSize != 0 &&
                            i == charArray->size - 1 &&
                            currBit < (header->thrashSize)) {
                                break;
                        }

                        // Se o bit atual for 1, vai para a direita, senão, vai
                        // para a esquerda
                        if (is_bit_i_set((unsigned char)charArray->array[i],
                                         currBit)) {
                                currNode = currNode->right;
                        } else {
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

        // Pega os 3 primeiros bits do primeiro byte
        header->thrashSize = ((unsigned char)buffer->array[0] >> 5);
        unsigned char firstByte = buffer->array[0];

        // Remove os 3 primeiros bits
        firstByte <<= 3;
        firstByte >>= 3;

        // Pega os 8 bits do segundo byte
        header->treeSize = (unsigned short int)buffer->array[1] |
                           ((unsigned short int)firstByte) << 8;

        // Pega a string da árvore
        charArray *string = malloc(sizeof(charArray));
        string->size = 0;
        string->array = malloc(sizeof(char) * 1);

        for (int i = 2; i < header->treeSize + 2; i++) {
                string = concat(string, buffer->array[i]);
        }

        header->treeString = string;

        return header;
}
