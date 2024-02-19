#include "../huffmantree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_hufftree_toString() {
        printf("test tree to string\n");
        Tree *tree = malloc(sizeof(Tree));
        tree->root = hufftree_createNode('1', 1);
        tree->root->left = hufftree_createNode('2', 2);
        tree->root->right = hufftree_createNode('3', 3);
        char *buffer = hufftree_toString(tree);
        for (int i = 0; i < strlen(buffer); i++) {
                if (buffer[i] == 0)
                        continue;
                printf("%c, %i, %x, %b\n", (unsigned char)buffer[i],
                       (unsigned char)buffer[i], (unsigned char)buffer[i],
                       (unsigned char)buffer[i]);
        }
        printf("%s", buffer);
}

void test_hufftree_create() {
        Tree *tree = hufftree_create("AAABBCC\n");

        printf("A = 0 \n");
        printf("returned: A = %s\n\n", tree->paths['A']);

        printf("B = 10\n");
        printf("returned: B = %s\n\n", tree->paths['B']);

        printf("\\n = 110\n");
        printf("returned: \\n = %s\n\n", tree->paths['\n']);

        printf("C = 111\n");
        printf("returned: C = %s\n\n", tree->paths['C']);

        printf("Tree string: %s\n", tree->stringfied);
}
