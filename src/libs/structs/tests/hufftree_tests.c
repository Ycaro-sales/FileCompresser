#include "../../file_functions.h"
#include "../huffmantree.h"
#include <stdio.h>
#include <stdlib.h>

void test_hufftree_toString() {
        printf("test tree to string\n");
        Tree *tree = malloc(sizeof(Tree));
        tree->root = hufftree_createNode('1', 1);
        tree->root->left = hufftree_createNode('2', 2);
        tree->root->right = hufftree_createNode('3', 3);
        charArray *buffer = hufftree_toString(tree);
        for (int i = 0; i < buffer->size; i++) {
                if (buffer->array[i] == 0)
                        continue;
                printf("%c, %i, %x, %b\n", (unsigned char)buffer->array[i],
                       (unsigned char)buffer->array[i],
                       (unsigned char)buffer->array[i],
                       (unsigned char)buffer->array[i]);
        }
        for (int i = 0; i < buffer->size; i++) {
                printf("%c", buffer->array[i]);
        }
        printf("\n");
}

void test_hufftree_create() {
        char *string = "AAABBCC\n";
        charArray *charArray = getCharArrayfromString(string);

        Tree *tree = hufftree_create(charArray);

        printf("A = 0 \n");
        printf("B = 10\n");
        printf("returned: B = %s\n\n", tree->paths['B']);

        printf("\\n = 110\n");
        printf("returned: \\n = %s\n\n", tree->paths['\n']);

        printf("C = 111\n");
        printf("returned: C = %s\n\n", tree->paths['C']);

        printf("Tree string: ");

        for (int i = 0; i < tree->stringfied->size; i++) {
                printf("%c", tree->stringfied->array[i]);
        }

        printf("\n");
}
